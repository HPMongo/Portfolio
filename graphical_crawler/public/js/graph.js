// Override Springy's initial node positioning for better centering
Springy.Vector.random = function() {
  return new Springy.Vector((Math.random() - 0.5), (Math.random() - 0.5));
};

function Graph(opts) {
  var that = this;

  this.model = opts.model;
  this.type = opts.type;
  this.paper = Raphael(this.model, 0, 0);
  this.paper.renderfix();

  this.path = [];
  this.vertices = {};

  this.springy = new Springy.Graph();
  this.layout = new Springy.Layout.ForceDirected(
    this.springy,
    100,          // Stiffness
    50,           // Repulsion
    0.25          // Damping
  );
  this.renderer = new Springy.Renderer(
    this.layout,
    function clear() {},
    function drawEdge(edge, p1, p2) {
      that.renderEdge(edge, p1, p2, that);
    },
    function drawNode(node, p) {
      that.renderVertex(node, p, that);
    },
    function onRenderStop() {},
    function onRenderStart() {}
  );

  // Borrowed from springyui.js
  this.currentBB = this.layout.getBoundingBox();
  this.targetBB = {
    bottomleft: new Springy.Vector(-2, -2),
    topright: new Springy.Vector(2, 2)
  };

  // Element styling
  this.strokeWidth = 2;
  this.vertexRadius = 7.5;

  this.black = '#333';
  this.blue = '#337ab7';
  this.red = '#a94442';
  this.grey = '#ddd';

  this.edgeAttributes = {
    stroke: 'black',
    strokeWidth: this.strokeWidth
  };

  this.labelAttributes = {
    'font-family': 'Arial, sans-serif',
    'font-size': 11,
    'font-weight': 'normal',
    stroke: 'white'
  };

  this.vertexAttributes = {
    'stroke-width': this.strokeWidth
  };

  // Auto adjusting bounding box -- borrowed from springyui.js
  Springy.requestAnimationFrame(function adjust() {
    that.targetBB = that.layout.getBoundingBox();
    // Current gets 20% closer to target every iteration
    that.currentBB = {
      bottomleft: that.currentBB.bottomleft.add(
        that.targetBB.bottomleft.subtract(that.currentBB.bottomleft).divide(10)
      ),
      topright: that.currentBB.topright.add(
        that.targetBB.topright.subtract(that.currentBB.topright).divide(10)
      )
    };

    Springy.requestAnimationFrame(adjust);
  });
}

// Convert to screen coordinates -- borrowed from springyui.js
Graph.prototype.toScreen = function(p) {
  var size = this.currentBB.topright.subtract(this.currentBB.bottomleft);
  var sx = p.subtract(this.currentBB.bottomleft).divide(size.x).x
    * this.paper.width;
  var sy = p.subtract(this.currentBB.bottomleft).divide(size.y).y
    * this.paper.height;
  return new Springy.Vector(sx, sy);
};

// Format the path string to create a straight line path from a to b
Graph.prototype.formatPathString = function(a, b) {
  return 'M' + a.x + ',' + a.y + 'L' + b.x + ',' + b.y;
};

// Size the paper relative to its parent's width and the window's height
Graph.prototype.setSize = function() {
  var parent = $(this.model.parentElement);
  var width, minWidth = 275;
  var height, minHeight = 300;

  // Size the model to fit the width of its parent element (minus padding)
  width = parent.innerWidth()
    - parseInt(parent.css('padding-left'), 10)
    - parseInt(parent.css('padding-right'), 10);

  // Ensure the paper isn't smaller than the minimum width
  if (width < minWidth) {
    width = minWidth;
  }

  // Size the paper based on the window height
  height = parseInt(($(window).height() * 0.75), 10);

  // Ensure the paper is at least minHeight
  if (height < minHeight) {
    height = minHeight;
  }

  // Resize the paper to the new width/height
  this.paper.setSize(width, height);

  // Update the bounding box
  this.currentBB = this.layout.getBoundingBox();
};


// Setup the graphical model from provided data (and fire off rendering)
Graph.prototype.render = function(data) {
  var i = 0, iCount = 0, j = 0, jCount = 0, pagesKeys;
  var vertex, url, page, links, link;

  if (data && data.hasOwnProperty('pages')) {
    // Organize the page object keys based on pathOrder
    pagesKeys = Object.keys(data.pages);
    pagesKeys.sort(function(a, b) {
      if (data.pages[a].pathOrder < data.pages[b].pathOrder) {
        return -1;
      } else if (data.pages[a].pathOrder > data.pages[b].pathOrder) {
        return 1;
      } else {
        return 0;
      }
    });

    // Add the pagesKeys to the path and initialize the vertex
    for (i = 0, iCount = pagesKeys.length; i < iCount; ++i) {
      url = pagesKeys[i];
      page = data.pages[url];

      if (
        (page.pathOrder > 0)
        || ((page.pathOrder === 0) && (this.path.indexOf(url) < 0))
      ) {
        // Add the url to the path array (ensuring the first node isn't
        // duplicated)
        this.path.push(url);
      }

      if ((page.pathOrder === 0) && this.vertices.hasOwnProperty(url)) {
        vertex = this.vertices[url];

        // Re-render the starting vertex (in case there were changes between
        // the initial and actual data that affect how the vertex is displayed)
        vertex.data.element.remove();
        vertex.data.label.remove();
        this.springy.removeNode(vertex);
      }

      // Initialize the vertex object
      vertex = {};
      vertex.visited = true;
      vertex.element = null;
      vertex.label = null;

      // Store or update properties from page data
      vertex.url = page.url;
      vertex.parentUrl = page.parentUrl;
      vertex.depth = page.depth;
      vertex.pathOrder = page.pathOrder;
      vertex.links = page.links;
      vertex.tooltipText = page.url + '<br>Depth: ' + page.depth;
      vertex.edges = {};

      if (page.hasOwnProperty('error')) {
        vertex.error = page.error;
        vertex.tooltipText += '<br>Error: ' + page.error;
      }

      if (page.hasOwnProperty('keywords')) {
        vertex.keywords = page.keywords;
        vertex.tooltipText += '<br>Keywords: &quot;'
          + page.keywords.join('&quot, &quot;') + '&quot;';
      }

      // Add the new vertex
      this.vertices[url] = this.springy.newNode(vertex);
    }
  }

  // Create the edges for each vertex
  for (i = 0, iCount = this.path.length; i < iCount; ++i) {
    url = this.path[i];
    vertex = this.vertices[url];
    links = this.vertices[url].data.links;

    for (j = 0, jCount = links.length; j < jCount; ++j) {
      link = links[j];
      linkVertex = this.vertices[link];

      // Only consider links to pages that the crawler visited
      if (this.vertices.hasOwnProperty(link)) {
        // Only render edges from parent to child (i.e., the crawler's path)
        if (linkVertex.data.parentUrl === vertex.data.url) {
          vertex.data.edges[link] = this.springy.newEdge(
            vertex,
            linkVertex,
            { element: null }
          );
        }
      }
    }
  }

  // Start the renderer (if it's not already started)
  this.renderer.start();
};

// Draw a line from vertex a to vertex b to represent an edge
Graph.prototype.renderEdge = function(edge, p1, p2, that) {
  var x1 = Math.round(that.toScreen(p1).x);
  var y1 = Math.round(that.toScreen(p1).y);
  var x2 = Math.round(that.toScreen(p2).x);
  var y2 = Math.round(that.toScreen(p2).y);
  var oldPathString, newPathString;
  var sourceData = edge.source.data;
  var targetData = edge.target.data;

  if (!edge.data.element) {
    // Create a new edge from a to b and store it in a.edges[b.url]
    edge.data.element = that.paper.path(that.formatPathString(
      { x: x1, y: y1 },
      { x: x2, y: y2 }
    ));
    edge.data.element.attr(that.edgeAttributes);
    edge.data.element.node.setAttribute('class', 'edge');

    // Make the edge grey if it only represents a link (not the crawler's path)
    if (
      ((that.type === 'dfs')
        && (targetData.pathOrder !== sourceData.pathOrder + 1))
      || ((that.type === 'bfs')
        && (targetData.depth !== sourceData.depth + 1))
    ) {
      edge.data.element.attr('stroke', that.grey);
    }

    // Ensure edges appear below vertices
    edge.data.element.toBack();
  } else {
    // Update the location of the existing edge
    oldPathString = edge.data.element.node.getAttribute('d');
    newPathString = that.formatPathString({ x: x1, y: y1 }, { x: x2, y: y2 });

    if (newPathString !== oldPathString) {
      edge.data.element.node.setAttribute('d', newPathString);
    }
  }
};

// Draw a circle to represent a vertex
Graph.prototype.renderVertex = function(vertex, p, that) {
  var x = Math.round(that.toScreen(p).x);
  var y = Math.round(that.toScreen(p).y);
  var vertexAttributes = {};
  var oldX, oldY;

  if (!vertex.data.element) {
    // Create a circle for the vertex
    vertex.data.element = that.paper.circle(x, y, that.vertexRadius);
    vertex.data.element.node.setAttribute('class', 'vertex');
    vertex.data.element.attr(that.vertexAttributes);

    // Style the circle based on if it has an error or keywords, was visited or
    // only linked
    if (vertex.data.hasOwnProperty('error')) {
      vertexAttributes.fill = that.red;
      vertexAttributes.stroke = that.red;
    } else if (vertex.data.hasOwnProperty('keywords')) {
      vertexAttributes.fill = that.blue;
      vertexAttributes.stroke = that.blue;
    } else if (vertex.data.hasOwnProperty('visited') && vertex.data.visited) {
      vertexAttributes.fill = that.black;
      vertexAttributes.stroke = that.black;
    } else {
      vertexAttributes.fill = 'white';
      vertexAttributes.stroke = that.black;
    }

    // Style the vertex circle
    vertex.data.element.attr(vertexAttributes);

    // Set up the tooltip and enable it
    vertex.data.element.node.setAttribute('data-tooltip', 'tooltip');
    vertex.data.element.node.setAttribute('title', vertex.data.tooltipText);
    $(vertex.data.element.node).tooltip({ container: 'body', html: true });
  } else {
    oldX = vertex.data.element.node.getAttribute('cx');
    oldY = vertex.data.element.node.getAttribute('cy');

    if ((x !== oldX) || (y !== oldY)) {
      // Move the vertex to the new location
      vertex.data.element.node.setAttribute('cx', x);
      vertex.data.element.node.setAttribute('cy', y);
    }
  }

  if (!vertex.data.label) {
    // Create a pathOrder label over the vertex
    vertex.data.label = that.paper.text(x, y, vertex.data.pathOrder);
    vertex.data.label.attr(that.labelAttributes);
    vertex.data.label.node.setAttribute('class', 'label');
  } else {
    oldX = vertex.data.label.node.getAttribute('x');
    oldY = vertex.data.label.node.getAttribute('y');

    if ((x !== oldX) || (y !== oldY)) {
      // Move the label to the new location
      vertex.data.label.node.setAttribute('x', x);
      vertex.data.label.node.setAttribute('y', y);
    }
  }
};
