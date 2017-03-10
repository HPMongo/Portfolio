var fs = require('fs');
var path = require('path');
var TEST_PAGES_DIR = 'public/test-pages';
var pageCount = 50;
var wstream;
var i = 0, j = 0, count = 0;

if (process.argv.length > 2) {
  // Set the pageCount to the user-provided number
  pageCount = parseInt(process.argv[2], 10);
}

process.stdout.write('Creating test pages...');

// Make sure we have an empty publictest-pages directory before creating pages
fs.exists(TEST_PAGES_DIR, function(exists) {
  if (exists) {
    // Delete any existing files in the test-pages directory
    fs.readdir(TEST_PAGES_DIR, function(err, files) {
      if (err) { throw err; }

      if (files.length > 0) {
        // Delete any existing files in the test-pages directory
        for (i = 0, count = files.length; i < count; ++i) {
          var filePath = path.join(TEST_PAGES_DIR, files[i]);

          if (fs.statSync(filePath).isFile()) {
            fs.unlink(filePath, function(err) {
              if (err) { throw err; }
            });
          }
        }
      }

      createPages();
    });
  } else {
    // Make the publictest-pages directory if it doesn't exist
    fs.mkdir(TEST_PAGES_DIR, '0755', function(err) {
      if (err) { throw err; }

      createPages();
    });
  }
});

function createPages() {
  // Create pageCount number of pages which randomly link to each other
  for (i = 1; i <= pageCount; ++i) {
    wstream = fs.createWriteStream(TEST_PAGES_DIR + '/' + i + '.html', {
      flags: 'w',
      defaultEncoding: 'utf8',
      fd: null,
      mode: '0644',
      autoClose: true
    });

    wstream.write(
      '<!DOCTYPE html>\n<html>\n  <head>\n    <title>Page ' + i
      + '</title>\n  </head>\n  <body>\n    <ul>'
    );

    // Create a pseudo-random number of links to other pages
    for (j = 1; j <= pageCount; ++j) {
      if (Math.floor(Math.random() * 2)) {
        wstream.write(
          '\n      <li><a href="' + j + '.html">Page ' + j + '</a></li>');
      }
    }

    wstream.write('\n    </ul>\n  </body>\n</html>\n');
  }

  process.stdout.write('done.\n');
}
