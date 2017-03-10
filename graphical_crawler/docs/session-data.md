# Session Data

The web server is responsible for establishing a new session in the database when it receives a request from the web page to initiate a crawler session.  Unlike pages data, the web server doesn't explicitly rely on data from the crawler when it comes to sessions.  The session information consists of data from the client's request and only relies on the crawler to tell it when it's done crawling.

## Database Schema

```sql
CREATE TABLE sessions(
  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  starting_url TEXT NOT NULL,
  search_type TEXT NOT NULL,
  n_limit INTEGER NOT NULL,
  keyword TEXT DEFAULT NULL,
  started NUMERIC DEFAULT NULL,
  finished NUMERIC DEFAULT NULL
);
```

To log a new session to the database, the web server will simply need to create a new session entry containing the request data and the start datetime.  An `INSERT` query would look like the following examples.

**Depth-first search without a keyword:**
```sql
INSERT INTO sessions (starting_url, search_type, n_limit, keyword, started, finished)
  VALUES
  ('http://www.example.com/', 'dfs', 5, NULL, datetime('now'), NULL);
```

**Breadth-first search with a keyword ("glasses"):**
```sql
INSERT INTO sessions (starting_url, search_type, n_limit, keyword, started, finished)
  VALUES
  ('http://localhost:3000/test-pages/12.html', 'bfs', 2, 'glasses', datetime('now'), NULL);
```

As you can see, the `finished` field is left blank when the session is created.  When the crawler tells the web server that it's done crawling, it's the web server's job to update the session to include the finish datetime:

```sql
UPDATE sessions SET finished = datetime('now') WHERE id = 1;
```

In the interim time, if you need to check whether the crawler is finished (e.g. in a different route), you could do something similar to this:

```sql
SELECT COUNT(*) FROM sessions WHERE id = 2 AND finished IS NOT NULL;
```
