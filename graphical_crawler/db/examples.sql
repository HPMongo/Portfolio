----- Example Session queries -----

---- Create a new session
-- We'll create sessions one by one but the example below simply shows a
-- DFS example with no keyword and a BFS example with a keyword ("glasses").
INSERT INTO sessions (starting_url, search_type, n_limit, keyword, started, finished)
  VALUES
  ('http://www.example.com/', 'dfs', 5, NULL, datetime('now'), NULL),
  ('http://localhost:3000/test-pages/12.html', 'bfs', 2, 'glasses', datetime('now'), NULL),
  ('http://www.aksmdflaskmfowefm.com', 'dfs', 3, NULL, datetime('now'), NULL);

---- Set the finish time for an existing session
-- As seen above, the finished time is NULL when the session is created and
-- we update it later when the crawler finishes.
UPDATE sessions SET finished = datetime('now') WHERE id = 1;
UPDATE sessions SET finished = datetime('now') WHERE id = 3;

---- Checking if a given crawler session is finished
-- You could handle this differently but this is just one way to do it.
SELECT COUNT(*) FROM sessions WHERE id = 2 AND finished IS NOT NULL;



----- Example Page queries -----
-- NOTE: The links field contains stringified JSON of the page's links array.

---- Create a new page
-- We'll create pages one by one but the example below simply shows:
-- 1. A session (1) with two crawled pages (where the second page doesn't
--    contain any unfollowed links).
-- 2. The starting page of a session with a ton of links on the page (which
--    is pretty normal for most web pages).
INSERT INTO pages (session_id, url, path_order, depth, links, keywords)
  VALUES
  (1, 'http://www.example.com/', 0, 0, '["http://www.example.com/example-page/"]', NULL),
  (1, 'http://www.example.com/example-page/', 1, 1, NULL, NULL),
  (2, 'http://localhost:3000/test-pages/12.html', 0, 0, '["http://localhost:3000/test-pages/4.html","http://localhost:3000/test-pages/5.html","http://localhost:3000/test-pages/10.html","http://localhost:3000/test-pages/13.html","http://localhost:3000/test-pages/14.html","http://localhost:3000/test-pages/15.html","http://localhost:3000/test-pages/18.html","http://localhost:3000/test-pages/19.html","http://localhost:3000/test-pages/21.html","http://localhost:3000/test-pages/22.html","http://localhost:3000/test-pages/23.html","http://localhost:3000/test-pages/24.html","http://localhost:3000/test-pages/25.html","http://localhost:3000/test-pages/26.html","http://localhost:3000/test-pages/29.html","http://localhost:3000/test-pages/30.html","http://localhost:3000/test-pages/32.html","http://localhost:3000/test-pages/33.html","http://localhost:3000/test-pages/36.html","http://localhost:3000/test-pages/38.html","http://localhost:3000/test-pages/39.html","http://localhost:3000/test-pages/40.html","http://localhost:3000/test-pages/42.html","http://localhost:3000/test-pages/43.html","http://localhost:3000/test-pages/45.html","http://localhost:3000/test-pages/51.html","http://localhost:3000/test-pages/52.html","http://localhost:3000/test-pages/53.html","http://localhost:3000/test-pages/54.html","http://localhost:3000/test-pages/55.html","http://localhost:3000/test-pages/56.html","http://localhost:3000/test-pages/60.html","http://localhost:3000/test-pages/62.html","http://localhost:3000/test-pages/64.html","http://localhost:3000/test-pages/65.html","http://localhost:3000/test-pages/66.html","http://localhost:3000/test-pages/71.html","http://localhost:3000/test-pages/72.html","http://localhost:3000/test-pages/73.html","http://localhost:3000/test-pages/74.html","http://localhost:3000/test-pages/77.html","http://localhost:3000/test-pages/78.html","http://localhost:3000/test-pages/79.html","http://localhost:3000/test-pages/82.html","http://localhost:3000/test-pages/89.html","http://localhost:3000/test-pages/90.html","http://localhost:3000/test-pages/91.html","http://localhost:3000/test-pages/92.html","http://localhost:3000/test-pages/93.html","http://localhost:3000/test-pages/97.html","http://localhost:3000/test-pages/100.html","http://localhost:3000/test-pages/103.html","http://localhost:3000/test-pages/105.html","http://localhost:3000/test-pages/106.html","http://localhost:3000/test-pages/107.html","http://localhost:3000/test-pages/112.html","http://localhost:3000/test-pages/115.html","http://localhost:3000/test-pages/116.html","http://localhost:3000/test-pages/117.html","http://localhost:3000/test-pages/118.html","http://localhost:3000/test-pages/122.html","http://localhost:3000/test-pages/123.html","http://localhost:3000/test-pages/124.html","http://localhost:3000/test-pages/126.html","http://localhost:3000/test-pages/127.html","http://localhost:3000/test-pages/131.html","http://localhost:3000/test-pages/132.html","http://localhost:3000/test-pages/133.html","http://localhost:3000/test-pages/134.html","http://localhost:3000/test-pages/136.html","http://localhost:3000/test-pages/137.html","http://localhost:3000/test-pages/142.html","http://localhost:3000/test-pages/143.html","http://localhost:3000/test-pages/144.html","http://localhost:3000/test-pages/149.html","http://localhost:3000/test-pages/151.html","http://localhost:3000/test-pages/152.html","http://localhost:3000/test-pages/154.html","http://localhost:3000/test-pages/157.html","http://localhost:3000/test-pages/160.html","http://localhost:3000/test-pages/161.html","http://localhost:3000/test-pages/163.html","http://localhost:3000/test-pages/165.html","http://localhost:3000/test-pages/166.html","http://localhost:3000/test-pages/167.html","http://localhost:3000/test-pages/168.html","http://localhost:3000/test-pages/170.html","http://localhost:3000/test-pages/171.html","http://localhost:3000/test-pages/173.html","http://localhost:3000/test-pages/174.html","http://localhost:3000/test-pages/175.html","http://localhost:3000/test-pages/178.html","http://localhost:3000/test-pages/179.html","http://localhost:3000/test-pages/180.html","http://localhost:3000/test-pages/181.html","http://localhost:3000/test-pages/182.html","http://localhost:3000/test-pages/184.html","http://localhost:3000/test-pages/186.html","http://localhost:3000/test-pages/187.html","http://localhost:3000/test-pages/188.html","http://localhost:3000/test-pages/189.html","http://localhost:3000/test-pages/192.html","http://localhost:3000/test-pages/195.html","http://localhost:3000/test-pages/196.html","http://localhost:3000/test-pages/199.html","http://localhost:3000/test-pages/203.html","http://localhost:3000/test-pages/204.html","http://localhost:3000/test-pages/209.html","http://localhost:3000/test-pages/212.html","http://localhost:3000/test-pages/213.html","http://localhost:3000/test-pages/218.html","http://localhost:3000/test-pages/219.html","http://localhost:3000/test-pages/221.html","http://localhost:3000/test-pages/222.html","http://localhost:3000/test-pages/223.html","http://localhost:3000/test-pages/224.html","http://localhost:3000/test-pages/225.html","http://localhost:3000/test-pages/226.html","http://localhost:3000/test-pages/228.html","http://localhost:3000/test-pages/229.html","http://localhost:3000/test-pages/232.html","http://localhost:3000/test-pages/233.html","http://localhost:3000/test-pages/234.html","http://localhost:3000/test-pages/238.html","http://localhost:3000/test-pages/242.html","http://localhost:3000/test-pages/243.html","http://localhost:3000/test-pages/245.html","http://localhost:3000/test-pages/246.html","http://localhost:3000/test-pages/247.html","http://localhost:3000/test-pages/249.html","http://localhost:3000/test-pages/250.html"]', '["Glasses","glasses"]');

-- Create a new page with an error
INSERT INTO pages (session_id, url, path_order, depth, links, keywords, error)
  VALUES
  (3, 'http://www.aksmdflaskmfowefm.com', 0, 0, NULL, NULL, 'Not Found');

---- Get the page data for a given session, ordered by path_order
SELECT session_id, url, path_order, depth, links FROM pages WHERE session_id = 1 ORDER BY path_order ASC;
