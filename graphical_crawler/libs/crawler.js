/**
 * Crawler module.
 * @module libs/crawler.js
 * @author Sam Ford
 */
var url = require('url');
var request = require('request');
var cheerio = require('cheerio');
var BFS = 'bfs';
var DFS = 'dfs';
var TIMEOUT = 10000;
var DELAY = 5000;
var httpStatusCodes = require('./http-status-codes.js');

/**
 * Create a Crawler object from an options object.
 * @class
 * @param {Object} opts The crawler parameters.
 */
var Crawler = function(opts) {
  var limit = 0;
  var keys, key;
  var i, count;

  this.pages = {};

  if (opts.hasOwnProperty('sessionId')) {
    this.sessionId = opts.sessionId;
  } else {
    this.sessionId = null;
  }

  if (opts.hasOwnProperty('url')) {
    this.queue = [{url: url.resolve(opts.url, ''), depth: 0}];
  } else {
    this.queue = [];
  }

  if (opts.hasOwnProperty('type')) {
    // Standardize the type to lowercase before validating
    opts.type = opts.type.toLowerCase();

    if (opts.type === BFS || opts.type === DFS) {
      this.type = opts.type;
    }
  } else {
    this.type = null;
  }

  this.pathCounter = 0;

  if (
    opts.hasOwnProperty('limit')
    && ((limit = parseInt(opts.limit, 10)) > 0)
  ) {
    this.limit = limit;
  } else {
    this.limit = 0;
  }

  if (opts.hasOwnProperty('keyword') && (opts.keyword.length > 0)) {
    this.keyword = opts.keyword.toLowerCase();
    this.keywordRegex = new RegExp('\\b' + this.keyword + '\\b', 'gi');
  } else {
    this.keyword = null;
  }

  if (
    opts.hasOwnProperty('timeout')
    && (typeof opts.timeout === 'number')
    && opts.timeout > 0
  ) {
    this.timeout = opts.timeout;
  } else {
    this.timeout = TIMEOUT;
  }

  if (
    opts.hasOwnProperty('delay')
    && (typeof opts.delay === 'number')
    && (opts.delay >= 0)
  ) {
    this.delay = opts.delay;
  } else {
    this.delay = DELAY;
  }

  this.callbacks = {};
  if (opts.hasOwnProperty('callbacks')) {
    // Validate the provided callbacks
    keys = Object.keys(opts.callbacks);
    for (i = 0, count = keys.length; i < count; ++i) {
      key = keys[i];
      if (typeof opts.callbacks[key] === 'function') {
        // Add the callback to this.callbacks if it's a function
        this.callbacks[key] = opts.callbacks[key];
      }
    }
  }

  this.terminated = false;

  this.wait = 0;
};

/**
 * Validate input parameters and initiate the crawler session.
 */
Crawler.prototype.start = function() {
  var crawler = this;
  if (!crawler.type || (crawler.limit && crawler.limit < 0)) {
    return;
  }

  if (crawler.queue.length > 0) {
    // Start crawling on the first page in the queue
    crawler.fetch(crawler.queue[0]);
  }
};

/**
 * Stop the crawler session before it fetches the next page.
 */
Crawler.prototype.stop = function() {
  this.terminated = true;

  if (this.callbacks.hasOwnProperty('finished')) {
    this.callbacks.finished(this.sessionId);
  }
}

/**
 * Fetch the next URL in the queue.
 * @param {Object} page The page object, containing "url" and "depth" props.
 */
Crawler.prototype.fetch = function(page) {
  var crawler = this;
  if (crawler.queue.length <= 0 || crawler.terminated) {
    return;
  }

  request(page.url, {timeout: crawler.timeout}, function(err, res, body) {
    var i = 0, count = 0;
    var matches;
    var currentPageObj, nextPageObj;

    // Initialize an object in crawler.pages for page.url
    if (!crawler.pages.hasOwnProperty(page.url)) {
      crawler.pages[page.url] = {
        url: page.url,
        parentUrl: page.parentUrl,
        pathOrder: crawler.pathCounter,
        depth: page.depth,
        links: []
      };
    }

    if (!page.parentUrl) {
      delete crawler.pages[page.url].parentUrl;
    }

    if (!err && (res.statusCode >= 200 && res.statusCode < 400)) {
      // Parse the response body
      var $ = cheerio.load(body);

      // Search the body for links, filter and format them, and store them
      $('a[href]')
        .filter(function() {
          // Filter out empty/useless href strings
          var hrefUrl = $(this).attr('href');
          return !((hrefUrl === '') || (hrefUrl.indexOf('#') === 0));
        })
        .map(function() {
          // Make all the URLs absolute
          return url.resolve(res.request.uri.href, $(this).attr('href'));
        })
        .filter(function(i, el) {
          // Filter out non-HTTP URLs and URLs to files
          var urlObj = url.parse(el);
          return !(
            (urlObj.protocol.indexOf('http') !== 0)
            || (
              (urlObj.path.indexOf('.') >= 0)
              && (urlObj.path.toLowerCase().indexOf('.htm') < 0)
            )
          );
        })
        .each(function(i, el) {
          // Add the URL to the links array (if it's not already present)
          if (crawler.pages[page.url].links.indexOf(el) < 0) {
            crawler.pages[page.url].links.push(el);
          }
        });

      // Queue the current page's URLs, making sure not to exceed the limit
      if (page.depth + 1 <= crawler.limit) {
        crawler.queueUrls(
          crawler.pages[page.url].links,
          page.url,
          (page.depth + 1)
        );
      }

      // Search for the keyword (if provided) in the body text
      if (
        crawler.keywordRegex
        && (matches = $('body').text().match(crawler.keywordRegex))
      ) {
        // Stop crawling if the keyword is found
        crawler.queue = [];            // Clear the queue to stop crawling
        crawler.pages[page.url].keywords = [];

        for (i = 0, count = matches.length; i < count; ++i) {
          // Add the matched word to the keywords array if it's not present
          if (crawler.pages[page.url].keywords.indexOf(matches[i]) < 0) {
            crawler.pages[page.url].keywords.push(matches[i]);
          }
        }
      }
    } else if (!err && (res.statusCode >= 400 && res.statusCode < 600)) {
      // Store 4XX and 5XX error response status messages
      if (httpStatusCodes.hasOwnProperty(res.statusCode)) {
        crawler.pages[page.url].error = httpStatusCodes[res.statusCode];
      } else {
        crawler.pages[page.url].error = 'Status code ' + res.statusCode;
      }
    } else if (err) {
      // Store failure messages for some error codes
      if (err.code === 'ENOTFOUND') {
        crawler.pages[page.url].error = 'Not found';
      } else if (err.code === 'ETIMEDOUT') {
        crawler.pages[page.url].error = 'Timed out';
      } else if (err.code) {
        crawler.pages[page.url].error = err.code;
      }
    }

    // Provide the data for the fetched page to the newPage callback
    if (crawler.callbacks.hasOwnProperty('newPage')) {
      crawler.callbacks.newPage(crawler.sessionId, crawler.pages[page.url]);
    }

    // TODO: Null the page object's links to free up some memory?
    // crawler.pages[page.url].links = null;

    // Remove the current page from the queue
    // TODO: Is this performant...? Could we queue more efficiently?
    if (crawler.queue.length > 0) {
      crawler.queue.splice(0, 1);
    }

    if (crawler.queue.length > 0) {
      currentPageObj = url.parse(page.url);
      nextPageObj = url.parse(crawler.queue[0].url);

      if (nextPageObj.host === currentPageObj.host) {
        // Add a delay if the next URL is in the same domain
        crawler.wait = crawler.delay;
      } else {
        crawler.wait = 0;
      }

      crawler.pathCounter += 1;

      setTimeout(function() {
        // Fetch the next page in the queue, after any delay
        crawler.fetch(crawler.queue[0]);
      }, crawler.wait);
    } else if (crawler.callbacks.hasOwnProperty('finished')) {
      // Execute the finished callback
      crawler.callbacks.finished(crawler.sessionId);
    }
  });
};

/**
 * Add URLs to the queue.
 * @param {Array} urls URLs to add to the queue.
 * @param {String} parentUrl The page which contained the URLs.
 * @param {Number} depth The depth of the URLs to add.
 */
Crawler.prototype.queueUrls = function(urls, parentUrl, depth) {
  if (urls.length <= 0) {
    return;
  }
  var crawler = this;
  var i = 0, count = 0;

  // Create an array of queued URLs so we can check if a URL is queued
  // TODO: Find a better method of checking if a URL is queued?
  var queuedUrls = [];
  for (i = 0, count = crawler.queue.length; i < count; ++i) {
    queuedUrls.push(crawler.queue[i].url);
  }

  for (i = 0, count = urls.length; i < count; ++i) {
    if (
      crawler.pages.hasOwnProperty(urls[i])
      || (queuedUrls.indexOf(urls[i]) >= 0)
    ) {
      continue;
    }

    crawler.queue.push({
      url: url.resolve(urls[i], ''),
      parentUrl: parentUrl,
      depth: depth
    });

    // Only queue the first unvisited URL for DFS
    if (crawler.type === DFS) {
      break;
    }
  }
};

module.exports = Crawler;
