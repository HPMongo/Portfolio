$(document).ready(function() {
  // Inform user local storage is not available.
  if (!storageAvailable('localStorage')) {
    $('div.container').before(
      '<div class="alert alert-warning alert-dismissible" role="alert">'
      + '<button type="button" class="close" data-dismiss="alert" '
      + 'aria-label="Close"><span aria-hidden="true">&times;</span></button>'
      + '<strong>Oh No!</strong> It looks like localStorage is disabled on '
      + 'your browser, some features may not work properly.</div>'
    );
  } else {
    getStoredSearches();
  }
});

// Use $(document) so we can handle clicks on elements added after render.
$(document).on('click', '.previous-query', populateSearchForm);

// Code from:
// https://developer.mozilla.org/en-US/docs/Web/API/Web_Storage_API
// /Using_the_Web_Storage_API
function storageAvailable(type) {
  try {
    var storage = window[type],
      x = '__storage_test__';
    storage.setItem(x, x);        // Set a test storage item.
    storage.removeItem(x);        // Remove it.
    return true;                  // Both add/remove was successful.
  } catch(e) {
    return false;                 // Something went wrong.
  }
}

// Populate previous session options with localStorage elements.
function getStoredSearches() {
  $('ul.previous-queries').html('');         // Clear previous elements.
  for (var i in localStorage) {              // Add them back.
    var obj = JSON.parse(localStorage[i]);
    if ('url' in obj && 'type' in obj && 'limit' in obj) {
      $('ul.previous-queries').append(
        '<li><span class="previous-query">' + obj.url + '</span></li>'
      );
    }
  }
}

// Run each time a users selects a previous search.
function populateSearchForm() {
  var prevSearch = JSON.parse(localStorage.getItem($(this).text()));

  // Fill out search form.
  $('#input-url').val(prevSearch.url);
  if (prevSearch.type === 'dfs') {
    $('#form-group-type').find('input[value="dfs"]').prop('checked', true);
  } else {
    $('#form-group-type').find('input[value="bfs"]').prop('checked', true);
  }
  $('#input-limit').val(prevSearch.limit);
  $('#input-keyword').val(prevSearch.keyword);
}
