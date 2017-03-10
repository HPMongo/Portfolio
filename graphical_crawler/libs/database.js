var sqlite3 = require('sqlite3').verbose();
var Q = require('q');

var Database = function(opts) {
  if (opts.hasOwnProperty('debug')) {
    this.debug = opts.debug;
  } else {
    this.debug = false;
  }

  if (opts.hasOwnProperty('dbFile')) {
    this.dbFile = opts.dbFile;
  } else {
    this.dbFile = null;
  }
};

Database.prototype.newSession = function(params) {
  var deferred = Q.defer();

  // Create database entry for session
  var db = new sqlite3.Database(this.dbFile);
  db.run(
    'INSERT INTO sessions (starting_url, search_type, n_limit, keyword, '
    + 'started, finished) VALUES(?, ?, ?, ?, datetime(\'now\'), NULL)',
    [params.url, params.type, params.limit, params.keyword],
    function(err) {
      db.close();
      if (err) {
        deferred.reject(new Error(err));
      } else {
        params.sessionId = this.lastID;
        deferred.resolve(params);
      }
    }
  );

  return deferred.promise;
};

Database.prototype.newPage = function(sessionId, page) {
  var deferred = Q.defer();

  // Ensure we have a sessionId argument
  if (!sessionId || (typeof sessionId !== 'number')) {
    if (this.debug) {
      console.log(
        'Invalid sessionId argument provided to newPage callback:\n%o',
        sessionId
      );
    }
    return;
  }

  // Ensure we have a page object
  if (!page || (typeof page !== 'object')) {
    if (this.debug) {
      console.log(
        'Invalid page argument provided to newPage callback:\n%o',
        page
      );
    }
    return;
  }

  // Ensure we have the required page properties
  if (
    !page.hasOwnProperty('url')
    || !page.hasOwnProperty('pathOrder')
    || !page.hasOwnProperty('depth')
    || !page.hasOwnProperty('links')
  ) {
    if (this.debug) {
      console.log('Page object is missing required properties:\n%o', page);
    }
    return;
  }

  // Deal with optional properties
  if (!page.hasOwnProperty('parentUrl')) {
    page.parentUrl = null;
  }

  if (!page.hasOwnProperty('keywords')) {
    page.keywords = null;
  }

  if (!page.hasOwnProperty('error')) {
    page.error = null;
  }

  // Create a database entry for the new page
  var db = new sqlite3.Database(this.dbFile);
  db.run(
    'INSERT INTO pages (session_id, url, parent_url, path_order, depth,'
    + ' links, keywords, error) VALUES (?, ?, ?, ?, ?, ?, ?, ?)',
    [
      sessionId,
      page.url,
      page.parentUrl,
      page.pathOrder,
      page.depth,
      JSON.stringify(page.links),
      ((page.keywords) ? JSON.stringify(page.keywords) : null),
      page.error
    ],
    function(err) {
      db.close();
      if (err) {
        deferred.reject(new Error(err));
      } else {
        deferred.resolve(page);
      }
    }
  );

  return deferred.promise;
};

Database.prototype.finishSession = function(sessionId) {
  var deferred = Q.defer();

  // Store the finish time in the session database entry (if not already set)
  var db = new sqlite3.Database(this.dbFile);
  db.run(
    'UPDATE sessions SET finished = datetime(\'now\') WHERE id = ? '
    + 'AND finished IS NULL;',
    [sessionId],
    function(err) {
      db.close();
      if (err) {
        deferred.reject(new Error(err));
      } else {
        deferred.resolve();
      }
    }
  );

  return deferred.promise;
};

Database.prototype.sessionFinished = function(sessionId) {
  var deferred = Q.defer();

  // Check if the crawler is finished
  var db = new sqlite3.Database(this.dbFile);
  db.get(
    'SELECT COUNT(*) AS finished FROM sessions '
    + 'WHERE id = ? AND finished IS NOT NULL;',
    [sessionId],
    function(err, row) {
      db.close();
      if (err) {
        deferred.reject(new Error(err));
      } else {
        var finished = (row.finished === 1) ? true : false;
        deferred.resolve(finished);
      }
    }
  );

  return deferred.promise;
};

Database.prototype.getSessionPages = function(
  response,
  sessionId,
  pathOrderGT
) {
  var deferred = Q.defer();
  var that = this;

  var pathOrderCondition = '';
  var queryVars = [sessionId];
  if (typeof pathOrderGT === 'number') {
    pathOrderCondition = 'AND path_order > ? ';
    queryVars.push(pathOrderGT);
  }
  var query = 'SELECT url, parent_url, path_order, depth, links, keywords,'
    + ' error FROM pages WHERE session_id = ? ' + pathOrderCondition
    + 'ORDER BY path_order ASC;';

  // Get the pages for the provided session ID
  var db = new sqlite3.Database(this.dbFile);
  db.all(
    query,
    queryVars,
    function(err, rows) {
      db.close();
      if (err) {
        deferred.reject(new Error(err));
      } else {
        var i = 0, count = 0;

        // Create the pages object
        for (i = 0, count = rows.length; i < count; ++i) {
          if (!rows[i].hasOwnProperty('url') || !rows[i].url) {
            if (that.debug) {
              console.log('Page object missing URL property:\n%o', rows[i]);
            }
            continue;
          }

          if (rows[i].hasOwnProperty('parent_url')) {
            if (rows[i].parent_url) {
              // Rename property to parentUrl
              rows[i].parentUrl = rows[i].parent_url;
            }
            delete rows[i].parent_url;
          }

          if (rows[i].hasOwnProperty('path_order')) {
            if (rows[i].path_order === 0) {
              // Set the starting URL if the page's path_order is 0
              response.startingUrl = rows[i].url;
            }

            // Rename the "path_order" property to "pathOrder"
            rows[i].pathOrder = rows[i].path_order;
            delete rows[i].path_order;
          }

          // Parse the stringified JSON properties of the row objects
          if (rows[i].hasOwnProperty('links')) {
            rows[i].links = JSON.parse(rows[i].links);
          }

          if (rows[i].hasOwnProperty('keywords')) {
            if (rows[i].keywords) {
              // Parse non-null keywords properties
              rows[i].keywords = JSON.parse(rows[i].keywords);
            } else {
              // Delete null keywords properties
              delete rows[i].keywords;
            }
          }

          if (rows[i].hasOwnProperty('error') && !rows[i].error) {
            // Delete null error properties
            delete rows[i].error;
          }

          // Add the page to response.pages object, using the URL as the key
          response.pages[rows[i].url] = rows[i];
        }

        // Add the firstPagePathOrder and lastPagePathOrder properties
        if (count > 0) {
          if (rows[0].hasOwnProperty('pathOrder')) {
            response.firstPagePathOrder = rows[0].pathOrder;
          }

          if (rows[count-1].hasOwnProperty('pathOrder')) {
            response.lastPagePathOrder = rows[count-1].pathOrder;
          }
        }

        deferred.resolve(response);
      }
    }
  );

  return deferred.promise;
};

module.exports = Database;
