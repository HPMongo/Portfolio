/**
 * Created by Huy on 6/2/2016.
 */
/*
   Cookies functions are created using example from http://www.quirksmode.org/js/cookies.html
 */
function createCookie(name,value,days) {
    if (days) {
        var date = new Date();
        date.setTime(date.getTime()+(days*24*60*60*1000));
        var expires = "; expires="+date.toGMTString();
    }
    else var expires = "";
    document.cookie = name+"="+value+expires+"; path=/";
}

function readCookie(name) {
    var nameEQ = name + "=";
    var ca = document.cookie.split(';');
    for(var i=0;i < ca.length;i++) {
        var c = ca[i];
        while (c.charAt(0)==' ') c = c.substring(1,c.length);
        if (c.indexOf(nameEQ) == 0) return c.substring(nameEQ.length,c.length);
    }
    return null;
}

function eraseCookie(name) {
    createCookie(name,"",-1);
}

function setSong() {
    var song = document.getElementsByName('optradio');
    var song_value;
    for(var i = 0; i < song.length; i++){
        if(song[i].checked){
            song_value = song[i].value;
        }
    }
    if (song_value !== null && song_value !== undefined) {
        createCookie('apisong',song_value,1);
        // redirect to detail page
        window.location="reviewdetail.html";
    }
}

function setNewSong() {
    var song = document.getElementsByName('songradio');
    var song_value;
    for(var i = 0; i < song.length; i++){
        if(song[i].checked){
            song_value = song[i].value;
        }
    }
    if (song_value !== null && song_value !== undefined) {
        createCookie('apinewsong',song_value,1);
        // redirect to detail page
        window.location="addreview.html";
    }
}
