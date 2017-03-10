var path = require('path');
var gulp = require('gulp');
var cleanCSS = require('gulp-clean-css');
var concat = require('gulp-concat');
var gulpif = require('gulp-if');
var jade = require('gulp-jade');
var less = require('gulp-less');
var rename = require('gulp-rename');
var uglify = require('gulp-uglify');

var destinations = {
  html: './public/',
  js: './public/js/',
  css: './public/css/',
  fonts: './public/fonts/'
};

function isNotMinified(file) {
  return (path.basename(file.path).indexOf('.min') === -1);
}

gulp.task('default', [
  'copyCssDeps',
  'copyJsDeps',
  'bootstrapCss',
  'bootstrapJs',
  'bootstrapFonts',
  'jade'
]);

gulp.task('watch', function() {
  gulp.watch('./views/*.jade', ['jade']);
});

// Compile Bootstrap CSS from LESS, using only what we need
gulp.task('bootstrapCss', function() {
  return gulp.src([
      './node_modules/bootstrap/less/vendor-prefixes.less',
      './node_modules/bootstrap/less/variables.less',
      './node_modules/bootstrap/less/mixins.less',
      './node_modules/bootstrap/less/normalize.less',
      './node_modules/bootstrap/less/print.less',
      './node_modules/bootstrap/less/scaffolding.less',
      './node_modules/bootstrap/less/type.less',
      './node_modules/bootstrap/less/grid.less',
      './node_modules/bootstrap/less/forms.less',
      './node_modules/bootstrap/less/buttons.less',
      './node_modules/bootstrap/less/labels.less',
      './node_modules/bootstrap/less/utilities.less',
      './node_modules/bootstrap/less/responsive-utilities.less',
      './node_modules/bootstrap/less/tooltip.less',
      './node_modules/bootstrap/less/alerts.less',
      './node_modules/bootstrap/less/close.less',
      './node_modules/bootstrap/less/component-animations.less',
      './node_modules/bootstrap/less/glyphicons.less'
    ])
    .pipe(concat('bootstrap.min.css'))
    .pipe(less({}))
    .pipe(cleanCSS({ compatibility: 'ie8' }))
    .pipe(gulp.dest(destinations.css));
});

// Compile Bootstrap JS, using only what we need
gulp.task('bootstrapJs', function() {
  return gulp.src([
      './node_modules/bootstrap/js/tooltip.js',
      './node_modules/bootstrap/js/alert.js',
      './node_modules/bootstrap/js/button.js',
      './node_modules/bootstrap/js/affix.js',
      './node_modules/bootstrap/js/transition.js',
      './node_modules/bootstrap/js/collapse.js'
    ])
    .pipe(concat('bootstrap.min.js'))
    .pipe(uglify())
    .pipe(gulp.dest(destinations.js));
});

// Copy over font files.
gulp.task('bootstrapFonts', function() {
  return gulp.src([
    './node_modules/bootstrap/fonts/glyphicons-halflings-regular.eot',
    './node_modules/bootstrap/fonts/glyphicons-halflings-regular.svg',
    './node_modules/bootstrap/fonts/glyphicons-halflings-regular.ttf',
    './node_modules/bootstrap/fonts/glyphicons-halflings-regular.woff',
    './node_modules/bootstrap/fonts/glyphicons-halflings-regular.woff2'
  ])
  .pipe(gulp.dest(destinations.fonts));
});


gulp.task('copyCssDeps', function() {
  // Copy minified CSS dependencies
  return gulp.src([])
    .pipe(gulp.dest(destinations.css));
});

gulp.task('copyJsDeps', function() {
  // Copy JavaScript dependencies (minifying if not already minified)
  return gulp.src([
      './node_modules/jquery/dist/jquery.min.js',
      './node_modules/jquery-validation/dist/jquery.validate.js',
      './node_modules/raphael/raphael.min.js',
      './node_modules/springy/springy.js'
    ])
    .pipe(gulpif(isNotMinified, uglify()))
    .pipe(gulpif(isNotMinified, rename({ extname: '.min.js' })))
    .pipe(gulp.dest(destinations.js));
});



gulp.task('jade', function() {
  // Render Jade files into static HTML files (skipping layout.jade)
  return gulp.src([
      '!./views/layout.jade',
      './views/*.jade'
    ])
    .pipe(jade({ pretty: true }))
    .pipe(gulp.dest(destinations.html));
});
