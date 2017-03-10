/**
 * Created by Huy on 6/5/2016.
 */
// Attach a submit handler to the form
$( "#searchForm" ).submit(function( event ) {

    // Stop form from submitting normally
    event.preventDefault();

    // Get some values from elements on the page:
    var $form = $( this ),
        inUser = $form.find( "input[id='user']" ).val(),
        inPW = $form.find("input[id='pass']").val(),
        url = 'http://ec2-52-39-193-144.us-west-2.compute.amazonaws.com:3000/validateAccount';
    // Send the data using post
    var posting = $.post( url, { user: inUser , userPW: inPW} );

    // Put the results in a div
    posting.done(function( data ) {
        var status = data.status;
        var message = data.message;
        $( "#result" ).empty().append( message );
    });
});