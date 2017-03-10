$(document).ready(function() {
  var fetchInterval = 15000;
  var fetchPagesInterval = null;    // Used to store the setInterval reference
  var sessionData = {};
  var windowEl = $(window);
  var body = $('body');
  var stopButton = $('#stop-btn');
  var upperSection = $('#upper');
  var crawlerInputForm = $('#crawler-input');
  var model = document.getElementById('model');
  var graph;
  var resizeTimer;
  var canUseLocalStorage = storageAvailable('localStorage');

  // Resize the graph and re-render it when the window is resized
  windowEl.resize(function() {
    clearTimeout(resizeTimer);
    resizeTimer = setTimeout(function() {
      if (sessionData.hasOwnProperty('pages')) {
        graph.setSize();
        graph.render();
      }
    }, 250);
  });

  // Handle form submission
  function submitForm(form) {
    // Reset the sessionData, if necessary
    sessionData = {};

    if (fetchPagesInterval) {
      // Clear the fetchPagesInterval if it's set
      window.clearInterval(fetchPagesInterval);
    }

    // Disable submit button (until we receive a response)
    form.elements.submit.disabled = true;

    // Gather the form input into a JSON object
    var formInput = {
      url: form.elements.url.value.trim(),
      type: form.elements.type.value,
      limit: form.elements.limit.value
    };

    if (form.elements.keyword.value !== '') {
      formInput.keyword = form.elements.keyword.value.trim();
    }

    // Send the crawler input form data to the server
    $.ajax({
      url: form.getAttribute('action'),
      type: form.getAttribute('method'),
      contentType: 'application/json; charset=utf-8',
      dataType: 'json',
      data: JSON.stringify(formInput),
      success: function(data) {
        // Display the stop button
        stopButton.removeClass('hidden');

        // Collapse the Session Parameters section
        upperSection.collapse('hide');

        if (canUseLocalStorage) {
          // Store the input values for the session in LocalStorage
          localStorage.setItem(formInput.url, JSON.stringify(formInput));
          getStoredSearches();      // Refresh our stored search list.
        }

        // Store the session data from the JSON response
        sessionData = data;

        if (graph) {
          // Get rid of the existing graph before creating a new one
          graph.paper.remove();
          delete graph;
        }

        // Create a new graph and set the initial size
        graph = new Graph({ model: model, type: formInput.type });
        graph.setSize();

        // Scroll to the model element
        model.scrollIntoView();

        // Optimistically render the initial data
        graph.render(sessionData);

        // Fetch the pages data for the session from the server (on an
        // interval), rendering the graphical model until the session is
        // finished
        fetchPagesData(sessionData.id, function(data) {
          // Add the new data to the graph
          graph.render(data);
        });

        // Reset the form
        form.reset();
      },
      error: function(err) {
        if (err.responseJSON.hasOwnProperty('errors')) {
          // Display server-side validation errors using jquery-validation
          crawlerInputForm.validate().showErrors(err.responseJSON.errors);
        }
      },
      complete: function() {
        // Re-enable the submit button
        form.elements.submit.disabled = false;
      }
    });

    return false;   // Prevent form submission
  }

  function fetchPagesData(sessionId, callback) {
    if ((typeof sessionId !== 'number') || (sessionId < 0)) {
      return false;
    }

    fetchPagesInterval = window.setInterval(function() {
      var pagesUrl = 'session/' + sessionId + '/pages';

      if (sessionData.hasOwnProperty('lastPagePathOrder')) {
        // Add a query string parameter to only fetch the data past the
        // sessionData.lastPagePathOrder number (i.e. new data)
        pagesUrl += '?pathOrderGT=' + sessionData.lastPagePathOrder;
      }

      $.ajax({
        url: pagesUrl,
        type: 'GET',
        dataType: 'json',
        success: function(data) {
          if (
            data.hasOwnProperty('firstPagePathOrder')
            && data.firstPagePathOrder > 0
          ) {
            // Delete the new firstPagePathOrder property (so it's not merged)
            delete data.firstPagePathOrder;
          }

          // Merge the new data into the existing sessionData
          $.extend(true, sessionData, data);

          // Render the graphical model when new data is received
          callback(data);

          if (sessionData.finished === true) {
            // Stop fetching data and hide the Stop button
            window.clearInterval(fetchPagesInterval);
            stopButton.addClass('hidden');
          }
        },
        error: function(err) {
          window.clearInterval(fetchPagesInterval);

          if (err.responseJSON.hasOwnProperty('errors')) {
            // Log fetching errors to the console
            console.error('Error fetching pages: %o', err.responseJSON.errors);
          }
        }
      });
    }, fetchInterval);
  }

  // Validate the input form
  crawlerInputForm.validate({
    rules: {
      url: {
        required: true,
        url: true
      },
      type: {
        required: true
      },
      limit: {
        required: true,
        digits: true,
        min: 1,
        max: 99
      }
    },
    messages: {
      url: {
        required: 'Please enter a starting URL',
        url: 'Please enter a valid URL'
      },
      type: {
        required: 'Please select a search type'
      },
      limit: {
        required: 'Please enter a numeric limit',
        digits: 'Please enter only digits',
        min: 'Please enter a limit greater than 0',
        max: 'Please enter a limit less than 100'
      }
    },
    onkeyup: false,
    errorElement: 'em',
    errorPlacement: function(error, element) {
      error.addClass('help-block');
      error.appendTo(element.parents('.col-sm-9'));
    },
    highlight: function(element) {
      $(element).parents('.form-group').addClass('has-error');
    },
    unhighlight: function(element) {
      $(element).parents('.form-group').removeClass('has-error');
    },
    submitHandler: submitForm
  });

  // Send a request to stop the crawler session
  stopButton.on('click', function() {
    var stopUrl;

    if (!sessionData.hasOwnProperty('id')) {
      return;
    }

    // Disable the Stop button
    stopButton.attr('disabled', true);

    // Create the URL to stop the crawler
    stopUrl = 'session/' + sessionData.id + '/stop';

    // Send a request to the server to stop the crawler
    $.ajax({
      url: stopUrl,
      type: 'GET',
      dataType: 'json',
      success: function(data) {
        // Re-enable the stop button and hide it
        stopButton.attr('disabled', false);
        stopButton.addClass('hidden');

        // NOTE: The front-end will fetch the final data then stop fetching
      },
      error: function(err) {
        if (err.responseJSON.hasOwnProperty('errors')) {
          // Log fetching errors to the console
          console.error(
            'Error stopping crawler: %o',
            err.responseJSON.errors
          );
        }
      }
    });
  });
});
