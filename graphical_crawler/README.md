Install Instructions
====================

Upgrading npm
-------------
We're targeting an old version of Node (0.10.42) on the Flip servers and the version of npm that's packaged with it is really outdated, so the first thing we'll want to do is check our npm version.  If you run `npm --version` and get something like "1.3.6", then you'll need to upgrade.

If you're working in your local development environment then you can install the latest version of npm globally using `npm install -g npm@latest`.  At this point, whenever you execute the `npm` command, it will be using the new version.

However, things get more complicated on the Flip servers due to access restrictions.  We can't install npm globally there, so we have to install npm locally (in node_modules) using `npm install npm@latest`.  With this setup, you will still get the old version when you run `npm`, so you have to run `./node_modules/.bin/npm` to access the newer version installed locally.  You'll want to make sure to run all the npm commands using that newer version or else you'll run into trouble.  [Substitute `./node_modules/.bin/npm` in place of `npm` in the commands below with this setup (i.e., `./node_modules/.bin/npm install`).]


Setting Up the Project
----------------------
To install the project's packages, run `npm install` while in the root directory of the project.

To set up the SQLite database using the `sqlite3` command line client (from the project's root directory):

1. `sqlite3 db/crawler.db`
2. `.read db/schemas.sql`
3. `.exit`

If you want some data to play with, you can technically import the db/examples.sql file (after importing the schemas) using `.read db/examples.sql`.  It will display the output from the example SELECT statements but the data will otherwise import fine.  [You can easily start fresh again by importing the schemas.sql file again, since it contains DROP TABLE statements at the start.]


Populating the Public Folder
----------------------------
Once you have the packages installed, you can execute the local version of gulp using `npm run gulp`.  This will copy dependencies from node_modules into the public folder and compile our Jade templates into static HTML.  [We'll run Gulp to keep the public folder up to date as we make changes, so this step shouldn't be necessary when checking out the project.]
If changes are made the gulp file, the `npm run gulp` command will need to be ran.

If you're going to be making a number of changes to the template files, you can use the Gulp "watch" task to have gulp regenerate the static HTML files whenever one of the Jade files in the "views" folder is modified.  To pass the "watch" argument to Gulp, you can run `npm run gulp -- watch` (note the " -- " between the task name and arguments).  Gulp will then watch the Jade files for modifications until you terminate the process.



How to Operate the Project
==========================
Once you have a working setup, you can start the web server by running `npm start` (or simply `node server.js` from the project's root directory).  The server defaults to port 3000 but you can run it on a different port by setting the PORT environment variable before the command to start the server (like `PORT=3005 npm start`).

When the server is running, the front-end will be available at something like [http://localhost:3000/](http://localhost:3000/).  This will load the index.html file and display the submission form.  If you fill in the values and submit the form, you should see the crawler visiting pages in the console.  The server is currently configured to log data to the console as it receives it from the crawler, so you'll be able to observe the crawler as it operates; right now, it prints out page URLs (after they're visited and parsed) and the final "pages" object.  [We'll want to remove these `console.log()` calls for production but they're useful at the moment for debugging.]

One thing to keep in mind is that the crawler adds a five second wait between fetching pages in the same domain (to be courteous and not hammer websites).  You can potentially run the crawler on real URLs but I've created a script that will generate a suite of local test pages, so you might want to use those; you can let the crawler loose on these pages without being concerned with courtesy.  When crawling the test pages, you can set the `delay` property of the object that's passed into the `Crawler` constructor to `0` to remove the delay.  [With the current setup, you could accomplish this by setting `req.body.delay = 0;` before the `var crawler = new Crawler(req.body);` statement in server.js.  Make sure to ***ONLY*** do this with our test pages and not with real websites.]

You can run the `libs/create-test-pages.js` script using `npm run create-test-pages` and it will generate 50 HTML pages in public/test-pages/ that pseudo-randomly link to each other.  If you want to generate fewer or more than 50 pages, you'll have to run the script directly like `node libs/create-test-pages.js 200` (since the old version of npm bundled with Node 0.10.42 doesn't allow you to pass arguments to scripts).  Once you have some test pages created, you could create a crawler session with a starting URL like http://localhost:3000/test-pages/12.html.  [I'll be expanding the create-test-pages script once I start working on unit tests, but it's not a bad start.]

Anyway, that's the gist of the experimentation branch's implementation of the project.  Once the other parts of the project start to solidify in the master branch, I'll write up some operation instructions there.