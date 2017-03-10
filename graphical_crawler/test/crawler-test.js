var Crawler = require('../libs/crawler.js');
var server;
var assert = require('assert');
var request = require('request');
var clone = require('clone');
var PORTNUM = 3000;
var localhostUrl = 'http://localhost:' + PORTNUM;

var defaultParams = {
  sessionId: 1,
  url: localhostUrl + '/test-pages/12.html',
  type: 'DFS',
  limit: 5,
  keyword: 'Rosebud',
  timeout: 5000,      // Default is 10000
  delay: 0,           // Default is 5000
  callbacks: {
    newPage: function(sessionId, page) {},
    finished: function(sessionId) {}
  }
};

before(function() {
  process.env.NODE_ENV = 'production';    // Avoid console output

  // Start the server, so we can fetch test pages
  server = require('../server.js');
});

after(function() {
  // Stop the server after executing the tests
  server.close();
});

describe('Crawler', function() {
  describe('Constructor', function() {
    var crawler;

    before(function() {
      // Create a new crawler instance
      crawler = new Crawler(defaultParams);
    });

    it('should set sessionId using argument', function() {
      assert.equal(crawler.sessionId, defaultParams.sessionId);
    });

    it('should initialize queue using url argument', function() {
      assert.deepEqual(
        crawler.queue,
        [{ url: defaultParams.url, depth: 0}]
      );
    });

    it('should set type using argument', function() {
      assert.equal(crawler.type, defaultParams.type);
    });

    it('should set limit using argument', function() {
      assert.equal(crawler.limit, defaultParams.limit);
    });

    it('should set keyword using argument', function() {
      assert.equal(crawler.keyword, defaultParams.keyword.toLowerCase());
    });

    it('should set timeout using argument', function() {
      assert.equal(crawler.timeout, defaultParams.timeout);
    });

    it('should set delay using argument', function() {
      assert.equal(crawler.delay, defaultParams.delay);
    });

    it('should set callbacks using argument', function() {
      assert.deepEqual(crawler.callbacks, defaultParams.callbacks);
    });
  });

  describe('start()', function() {
    var pages;
    var finished;
    var params;
    var crawler;

    before(function(done) {
      pages = [];
      finished = false;

      // Overide default params for this test
      params = clone(defaultParams);
      params.limit = 1;
      params.keyword = '';
      params.callbacks.newPage = function(sessionId, page) {
        pages.push(page);
      };
      params.callbacks.finished = function(sessionId) {
        finished = true;
      };

      // Start the crawler
      crawler = new Crawler(params);
      crawler.start();

      setTimeout(function() {
        // Stop the crawler so we can test the finished callback
        crawler.stop();
        done();
      }, 50);
    });

    it('should fetch the starting page', function() {
      assert.notEqual(crawler.queue[0], params.url);
      assert.equal(pages[0].url, params.url);
    });

    it('should remove the first page from the queue', function() {
      assert.notEqual(crawler.queue[0], params.url);
    });

    it('should pass a page object to the newPage callback', function() {
      assert(pages.length > 0);

      // Check page object properties
      assert(pages[0].hasOwnProperty('url'));
      assert(pages[0].hasOwnProperty('pathOrder'));
      assert(pages[0].hasOwnProperty('depth'));
      assert(pages[0].hasOwnProperty('links'));
    });

    it('should execute the finished callback when done', function() {
      assert.equal(finished, true);
    });
  });

  describe('stop()', function() {
    // This test assumes that the links in the test pages are all in the same
    // domain (which they should be, since they're relative)
    var pages;
    var finished;
    var params;
    var crawler;

    before(function(done) {
      pages = [];
      finished = false;

      // Overide default params for this test
      params = clone(defaultParams);
      params.limit = 30;
      params.keyword = '';
      params.delay = 60000;   // Set a long delay so we can test stopping
      params.callbacks.newPage = function(sessionId, page) {
        pages.push(page);
      };
      params.callbacks.finished = function(sessionId) {
        finished = true;
      };

      // Start the crawler and stop it
      crawler = new Crawler(params);
      crawler.start();
      crawler.stop();
      done();
    });

    it('should execute the finished callback', function() {
      assert.equal(finished, true);
    });

    it('should stop before the limit is reached', function() {
      assert(pages.length < params.limit);
    });
  });

  describe('fetch()', function() {
    var pages;
    var finished;
    var params;
    var crawler;

    before(function(done) {
      pages = [];
      finished = false;

      // Overide default params for this test
      params = clone(defaultParams);
      params.limit = 1;
      params.keyword = '';
      params.delay = 60000;   // Set a long delay so we can test queueing
      params.callbacks.newPage = function(sessionId, page) {
        pages.push(page);
      };
      params.callbacks.finished = function(sessionId) {
        finished = true;
      };

      // Start the crawler and fetch the first page in the queue
      crawler = new Crawler(params);
      crawler.fetch(crawler.queue[0]);

      setTimeout(function() {
        // Stop the crawler so we can test the finished callback
        crawler.stop();
        done();
      }, 50);
    });

    it('should fetch the first page in the queue', function() {
      assert.equal(pages[0].url, params.url);
    });

    it('should remove the first page from the queue', function() {
      assert.notEqual(crawler.queue[0], params.url);
    });

    it('should queue links found on the current page', function() {
      assert(crawler.queue.length > 0);
    });

    it('should pass a page object to the newPage callback', function() {
      assert(pages.length > 0);

      // Check page object properties
      assert(pages[0].hasOwnProperty('url'));
      assert(pages[0].hasOwnProperty('pathOrder'));
      assert(pages[0].hasOwnProperty('depth'));
      assert(pages[0].hasOwnProperty('links'));
    });

    it('should execute the finished callback when done', function() {
      assert.equal(finished, true);
    });
  });

  describe('queueUrls()', function() {
    var i, count;
    var urls;
    var dfsParams, dfsCrawler;
    var bfsParams, bfsCrawler;

    before(function() {
      urls = [
        defaultParams.url,
        localhostUrl + '/test-pages/10.html',
        localhostUrl + '/test-pages/20.html',
        localhostUrl + '/test-pages/30.html',
      ];

      // Create a crawler using DFS
      dfsParams = clone(defaultParams);
      dfsParams.type = 'dfs';
      dfsCrawler = new Crawler(dfsParams);

      // Create a new crawler using BFS
      bfsParams = clone(defaultParams);
      bfsParams.type = 'bfs';
      bfsCrawler = new Crawler(bfsParams);

      // Queue the URLs
      dfsCrawler.queueUrls(urls, dfsParams.url, 1);
      bfsCrawler.queueUrls(urls, bfsParams.url, 1);
    });

    it('should only queue the first unvisited URL for DFS', function() {
      // The queue started with one page, so now it should have two
      assert(dfsCrawler.queue.length === 2);

      if (
        (dfsCrawler.queue.length > 1)
        && dfsCrawler.queue[1].hasOwnProperty('url')
      ) {
        // The starting URL is the first element, so make sure it was skipped
        assert(urls.indexOf(dfsCrawler.queue[1].url) > 0);
      }
    });

    it('should queue all provided URLs for BFS', function() {
      // The queue started with one page, so we add urls.length - 1 (skipping
      // the first URL, which was already in the queue)
      assert(bfsCrawler.queue.length === 1 + (urls.length - 1));

      // Make sure the new URLs match
      for (i = 1, count = bfsCrawler.queue.length; i < count; ++i) {
        assert.equal(bfsCrawler.queue[i].url, urls[i]);
      }
    });
  });
});
