DROP TABLE IF EXISTS sessions;
DROP TABLE IF EXISTS pages;

-- Crawler Sessions
CREATE TABLE sessions(
  id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
  starting_url TEXT NOT NULL,
  search_type TEXT NOT NULL,
  n_limit INTEGER NOT NULL,
  keyword TEXT DEFAULT NULL,
  started NUMERIC DEFAULT NULL,
  finished NUMERIC DEFAULT NULL
);

-- Crawler Pages
CREATE TABLE pages(
  session_id INTEGER NOT NULL,
  url TEXT NOT NULL,
  parent_url TEXT DEFAULT NULL,
  path_order INTEGER NOT NULL,
  depth INTEGER NOT NULL,
  links TEXT DEFAULT NULL,
  keywords TEXT DEFAULT NULL,
  error TEXT DEFAULT NULL,
  PRIMARY KEY (session_id, url),
  FOREIGN KEY (session_id)
    REFERENCES sessions(id)
    ON DELETE CASCADE
    ON UPDATE CASCADE
);
