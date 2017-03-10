var env = process.env.NODE_ENV || 'development';
var DEV = (env === 'development');
var express = require('express');
var compression = require('compression');
var bodyParser = require('body-parser');
var Crawler = require('./libs/crawler.js');
var crawlers = {};
var Database = require('./libs/database.js');
var db = new Database({
  debug: DEV,
  dbFile: 'db/crawler.db'
});
var validUrl = require('valid-url');
var path = require('path');
var port = process.env.PORT || 3000;

var app = express();

app.disable('x-powered-by');    // Disable the X-Powered-By header

app.use(compression());
app.use(bodyParser.json());     // Parse request body
app.use(express.static(path.join(__dirname, 'public')));

function newPage(sessionId, page) {
  if (DEV) {
    console.log(page);
  }
  return db.newPage(sessionId, page);
}

function crawlerFinished(sessionId) {
  if (DEV) {
    console.log('Session ' + sessionId + ' finished');
  }

  if (crawlers.hasOwnProperty(sessionId)) {
    // Destroy the crawler instance
    delete crawlers[sessionId];
  }

  return db.finishSession(sessionId);
}

app.post('/crawl', function(req, res) {
  if (DEV) {
    console.log('POST /crawl');
    console.log(req.body);
  }

  var errors = {};

  // Validate the starting URL input
  if (!req.body.hasOwnProperty('url')) {
    errors.url = 'Please enter a starting URL';
  } else {
    req.body.url = req.body.url.trim();

    if (!validUrl.isWebUri(req.body.url)) {
      errors.url = 'Please enter a valid URL';
    }
  }

  // Validate the search type input
  if (!req.body.hasOwnProperty('type')) {
    errors.type = 'Please select a search type';
  } else {
    req.body.type = req.body.type.trim().toLowerCase();

    if (!(req.body.type === 'dfs' || req.body.type === 'bfs')) {
      errors.type = 'Please select a valid search type';
    }
  }

  // Validate the limit input
  if (!req.body.hasOwnProperty('limit')) {
    errors.limit = 'Please enter a numeric limit';
  } else {
    if (typeof req.body.limit === 'string') {
      if (!req.body.limit.match(/^-?\d+$/)) {
        errors.limit = 'Please enter only digits';
      } else {
        // Parse the limit as an integer if it looks like a number
        req.body.limit = parseInt(req.body.limit, 10);
      }
    }

    if (typeof req.body.limit === 'number') {
      // Validate the limit bounds
      if (req.body.limit < 0) {
        errors.limit = 'Please enter a limit greater than 0';
      } else if (req.body.limit > 99) {
        errors.limit = 'Please enter a limit less than 100';
      }
    }
  }

  // Clean up the keyword input, if provided
  if (req.body.hasOwnProperty('keyword')) {
    req.body.keyword = req.body.keyword.trim();
  }

  // Don't proceed if there are validation errors
  if (Object.keys(errors).length > 0) {
    // Send the validation errors in the response and don't proceed
    res.status(400).json({ errors: errors });
    return;
  }

  // Log a new crawler session to the database and start the crawler
  db.newSession(req.body)
  .then(function(params) {
    // Provide initial sessionData in the response object
    var response = {
      id: params.sessionId,
      finished: false,
      pages: {},
      startingUrl: params.url
    };
    response.pages[params.url] = {
      url: params.url,
      depth: 0,
      links: [],
      pathOrder: 0
    };

    // Add the callbacks to the params object
    params.callbacks = {
      newPage: newPage,
      finished: crawlerFinished
    };

    // Send the sessionData in the JSON response
    res.json(response);

    // Create a Crawler instance
    crawlers[params.sessionId] = new Crawler(params);

    // Start the crawler
    crawlers[params.sessionId].start();
  })
  .done();
});

app.get('/session/:id/pages', function(req, res) {
  if (DEV) {
    console.log('GET /session/' + req.params.id + '/pages');
  }

  var sessionId;
  var pathOrderGT = null;
  var errors = {};

  // Validate the session id URL parameter
  if (!req.params.hasOwnProperty('id')) {
    errors.sessionId = 'Please provide a session ID in the URL';
  } else if (!req.params.id.match(/^\d+$/)) {
    errors.sessionId = 'Please provide a valid sessionId in the URL';
  } else {
    sessionId = parseInt(req.params.id, 10);

    if (sessionId < 1) {
      errors.sessionId = 'Please provide a valid sessionId in the URL';
    }
  }

  // Validate the pathOrderGT query string parameter, if provided
  if (
    req.query.hasOwnProperty('pathOrderGT')
    && req.query.pathOrderGT.match(/^\d+$/)
  ) {
    pathOrderGT = parseInt(req.query.pathOrderGT, 10);
  }

  // Don't proceed if there are validation errors
  if (Object.keys(errors).length > 0) {
    // Send the validation errors in the response and don't proceed
    res.status(400).json({ errors: errors });
    return;
  }

  var response = {
    id: sessionId,
    finished: false,
    pages: {}
  };

  db.sessionFinished(sessionId)
  .then(function(finished) {
    response.finished = finished;
  })
  .then(function() {
    return db.getSessionPages(response, sessionId, pathOrderGT);
  })
  .then(function(response) {
    res.json(response);
  })
  .fail(function() {
    res.json({ errors: ['Error executing "pages" query'] });
  })
  .done();
});

app.get('/session/:id/stop', function(req, res) {
  if (DEV) {
    console.log('GET /session/' + req.params.id + '/stop');
  }

  var sessionId;
  var errors = {};
  var stopped;

  // Validate the session id URL parameter
  if (!req.params.hasOwnProperty('id')) {
    errors.sessionId = 'Please provide a session ID in the URL';
  } else if (!req.params.id.match(/^\d+$/)) {
    errors.sessionId = 'Please provide a valid sessionId in the URL';
  } else {
    sessionId = parseInt(req.params.id, 10);

    if (sessionId < 1) {
      errors.sessionId = 'Please provide a valid sessionId in the URL';
    }
  }

  // Don't proceed if there are validation errors
  if (Object.keys(errors).length > 0) {
    // Send the validation errors in the response and don't proceed
    res.status(400).json({ errors: errors });
    return;
  }

  if (crawlers.hasOwnProperty(sessionId)) {
    // Stop the crawler (if it hasn't already finished)
    crawlers[sessionId].stop();
    stopped = true;
  }

  if (stopped) {
    // Indicate that the crawler was stopped
    res.json({ stopped: true });
  } else {
    // Indicate that the crawler had already finished
    res.json({ finished: true });
  }
});

module.exports = app.listen(port, function() {
  if (DEV) {
    console.log('Server listening on port %d', port);
  }
});
