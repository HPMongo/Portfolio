# Pages Data

## Object Format

Here is an example of the format that the crawler's pages data currently takes.  The client (web page) could potentially consume page objects like these directly in a Socket.io setup, so it may help to standardize on this format when re-creating the pages data from the data in the database at first.  [I set up the database schema to easily store the page object data, so it should be easy to recreate page objects in this format.]

```javascript
{
  'http://localhost:3000/test-pages/12.html': {
    url: 'http://localhost:3000/test-pages/12.html',
    pathOrder: 0,
    depth: 0,
    links: [
      'http://localhost:3000/test-pages/4.html',
      'http://localhost:3000/test-pages/5.html',
      'http://localhost:3000/test-pages/10.html',
      'http://localhost:3000/test-pages/13.html',
      'http://localhost:3000/test-pages/14.html'
    ]
  },
 'http://localhost:3000/test-pages/4.html': {
    url: 'http://localhost:3000/test-pages/4.html',
    pathOrder: 1,
    depth: 1,
    links: [
      'http://localhost:3000/test-pages/2.html',
      'http://localhost:3000/test-pages/3.html',
      'http://localhost:3000/test-pages/4.html',
      'http://localhost:3000/test-pages/5.html',
      'http://localhost:3000/test-pages/7.html',
      'http://localhost:3000/test-pages/8.html',
      'http://localhost:3000/test-pages/10.html'
    ]
  },
 'http://localhost:3000/test-pages/5.html': {
    url: 'http://localhost:3000/test-pages/5.html',
    pathOrder: 2,
    depth: 1,
    links: [
      'http://localhost:3000/test-pages/1.html',
      'http://localhost:3000/test-pages/4.html',
      'http://localhost:3000/test-pages/11.html'
    ],
    keywords: ['page', 'Page']
  }
}
```

If a page is not reachable for some reason, it will contain an "error" string property.  In this case, we can leave off the links property (since we couldn't fetch and parse the page content for links).

```javascript
{
  'http://www.aksmdflaskmfowefm.com': {
    url: 'http://www.aksmdflaskmfowefm.com',
    pathOrder: 0,
    depth: 0,
    error: 'Not Found'
  }
}
```


## Database Schema

```sql
CREATE TABLE pages(
  session_id INTEGER NOT NULL,
  url TEXT NOT NULL,
  path_order INTEGER NOT NULL,
  depth INTEGER NOT NULL,
  links TEXT DEFAULT NULL,
  keywords TEXT DEFAULT NULL,
  PRIMARY KEY (session_id, url),
  FOREIGN KEY (session_id)
    REFERENCES sessions(id)
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
```

The pages database schema is pretty straightforward outside of the links and keywords fields.  For these fields, we can simply `JSON.stringify()` that data and store the resulting strings in the database (using `JSON.parse()` when the data is fetched from the database).

An `INSERT` query for the first page object above would look something like this (assuming the session_id is 1):

```sql
INSERT INTO pages (session_id, url, path_order, depth, links, keywords)
  VALUES
  (1, 'http://localhost:3000/test-pages/12.html', 0, 0, "[\"http://localhost:3000/test-pages/4.html\",\"http://localhost:3000/test-pages/5.html\",\"http://localhost:3000/test-pages/10.html\",\"http://localhost:3000/test-pages/13.html\",\"http://localhost:3000/test-pages/14.html\"]", NULL);
```

An `INSERT` query for the third object above would look something like this:

```sql
INSERT INTO pages (session_id, url, path_order, depth, links, keywords)
  VALUES
  (1, 'http://localhost:3000/test-pages/5.html', 2, 1, "[\"http://localhost:3000/test-pages/1.html\",\"http://localhost:3000/test-pages/4.html\",\"http://localhost:3000/test-pages/11.html\"]", "[\"page\",\"Page\"]");
```

An `INSERT` query for a page with an error would look something like this:

```sql
INSERT INTO pages (session_id, url, path_order, depth, links, keywords, error)
  VALUES
  (3, 'http://www.aksmdflaskmfowefm.com', 0, 0, NULL, NULL, 'Not Found');
```


## /session/:id/pages Endpoint Response

The `/session/:id/pages` endpoint fetches the pages data from the database for a given session ID and creates a response object.  This object encapsulates the pages object while also providing the starting URL.  The format is as follows (using the "pages" object from the above example):

```javascript
{
  id: 1,
  finished: false,
  pages: {
    'http://localhost:3000/test-pages/12.html': {
      url: 'http://localhost:3000/test-pages/12.html',
      pathOrder: 0,
      depth: 0,
      links: [
        'http://localhost:3000/test-pages/4.html',
        'http://localhost:3000/test-pages/5.html',
        'http://localhost:3000/test-pages/10.html',
        'http://localhost:3000/test-pages/13.html',
        'http://localhost:3000/test-pages/14.html'
      ]
    },
   'http://localhost:3000/test-pages/4.html': {
      url: 'http://localhost:3000/test-pages/4.html',
      pathOrder: 1,
      depth: 1,
      links: [
        'http://localhost:3000/test-pages/2.html',
        'http://localhost:3000/test-pages/3.html',
        'http://localhost:3000/test-pages/4.html',
        'http://localhost:3000/test-pages/5.html',
        'http://localhost:3000/test-pages/7.html',
        'http://localhost:3000/test-pages/8.html',
        'http://localhost:3000/test-pages/10.html'
      ]
    },
   'http://localhost:3000/test-pages/5.html': {
      url: 'http://localhost:3000/test-pages/5.html',
      pathOrder: 2,
      depth: 1,
      links: [
        'http://localhost:3000/test-pages/1.html',
        'http://localhost:3000/test-pages/4.html',
        'http://localhost:3000/test-pages/11.html'
      ],
      keywords ['page', 'Page']
    },
    'http://localhost:3000/test-pages/10.html': {
      url: 'http://localhost:3000/test-pages/10.html',
      pathOrder: 3,
      depth: 1,
      error: 'Not Found'
    }
  },
  startingUrl: 'http://localhost:3000/test-pages/12.html',
  firstPagePathOrder: 0,
  lastPagePathOrder: 3
}
```

This format allows you to easily begin rendering the graphical model at the starting page (e.g. `response.pages[response.startingUrl]`).  You could go on to render the links for the starting page, checking if the "pages" object has a matching key (indicating the crawler fetched it).  If there isn't a page object with that key, then the link was present on the page but wasn't visited by the crawler.

When it comes to the path order, you would just refer to the `pathOrder` property and indicate the path order in the graphical model (e.g., rendering a node with a number on it for the path order).  [I mention this because the current example data in the "css-testing" branch uses an array for the pages data, rather than an object.  It's worth noting that I used the original object format in the endpoint response as well (since `Object.prototype.hasOwnProperty()` will be faster than `Array.indexOf()`).]

The "error" property in the last page object indicates that the crawler encountered an HTTP error when attempting to fetch that page.

The firstPagePathOrder property indicates the lowest pathOrder found in the provided response and the lastPagePathOrder indicates the highest pathOrder found in the provided response.  The firstPagePathOrder currently isn't used in any way on the front-end but the lastPagePathOrder is used to coordinate the fetching of partial data from the web server.  That is, the client provides the lastPagePathOrder to the server (in the request to the `/session/:id/pages` endpoint) and the server restricts the provided data to pages with a pathOrder greater than that number.  As a result, the client retrieves only the pages data it doesn't already have.
