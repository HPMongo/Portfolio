/**
 * Created by Huy on 4/23/2016.
 */
var express = require('express');
var bodyParser = require('body-parser');
var AWS = require("aws-sdk");

var app = express();
var table = "Music";
var userTable = "Users";

AWS.config.update({
    region: "us-west-2",
    endpoint: "http://localhost:8000"
});

app.use(bodyParser.json());     // Parse request body
app.use(bodyParser.urlencoded({ extended: false }));

//
//This function will get more detail for a given song and artist
//
function addSong(inArtist, inSong, inRating, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();

    console.log("Adding " + inSong + " by " + inArtist);

    var params = {
        TableName: table,
        Item: {
            "Artist": inArtist,
            "SongTitle": inSong,
            "Rating": inRating,
            "Reviews":0
        },
        "ConditionExpression": "attribute_not_exists(Artist) and attribute_not_exists(SongTitle)"
    };

    docClient.put(params, function(err, data) {
        if (err) {
            if (err.code === 'ConditionalCheckFailedException') {
                cb("Song already exists", null);
            } else {
                cb("Encounter error adding " + inSong + " by " + inArtist, null);
            }
        }
        else
            cb(null, data);
    });
}

//
//This function will update rating of a song
//
function updateRating(inArtist, inSong, inRating, inAction, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();
    var oldRating;
    var oldReviews;
    var newRating;
    var newReviews;

    var params1 = {
        TableName: table,
        Key: {
            "Artist": inArtist,
            "SongTitle": inSong
        }
    };

    docClient.get(params1, function(err, data) {
        if (err) {
            cb("Encounter error retrieving " + inSong + " by " + inArtist, null);
        }
        else {
            oldRating = data.Item.Rating;
            oldReviews = data.Item.Reviews;
            if (oldRating === undefined || isNaN(oldRating)) {
                oldRating = 5;  // default value
            }
            if (oldReviews === undefined || isNaN(oldReviews)) {
                oldReviews = 0;  // default value
            }
            if (inAction === 'add') {
                newReviews = oldReviews + 1;
                newRating = ((oldRating * oldReviews) + inRating) / newReviews;
            } else {
                newReviews = oldReviews - 1;
                newRating = ((oldRating * oldReviews) - inRating) / newReviews;
            }
            // round rating
            newRating = Math.round(newRating * 10) / 10;

            if (newRating < 0) {
                newRating = 0;
            }

            var params2 = {
                TableName: table,
                Key: {
                    "Artist":inArtist,
                    "SongTitle":inSong
                },
                UpdateExpression: "SET Rating = :updateRating, Reviews = :updateReview",
                ExpressionAttributeValues: {
                    ":updateRating": newRating,
                    ":updateReview": newReviews
                },
                ReturnValues: "ALL_NEW"
            };

            docClient.update(params2, function (err, data) {
                if (err) {
                    cb("Unable to update review", null);
                }
                else
                    cb(null, data);
            });
        }
    });
}


//
//This function will update rating of a song
//
/*
function updateRating(inArtist, inSong, inRating, inAction, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();
    var oldRating;
    var oldReviews;
    var newRating;
    var newReviews;

    var params1 = {
        TableName: table,
        Key: {
            "Artist": inArtist,
            "SongTitle": inSong
        }
    };

    docClient.get(params1, function(err, data) {
        if (err) {
            cb("Encounter error retrieving " + inSong + " by " + inArtist, null);
        }
        else {
            oldRating = data.Item.Rating;
            oldReviews = data.Item.Reviews;
            if (oldRating === undefined || isNaN(oldRating)) {
                oldRating = 5;  // default value
            }
            if (oldReviews === undefined || isNaN(oldReviews)) {
                oldReviews = 0;  // default value
            }
            if (inAction === 'add') {
                newReviews = oldReviews + 1;
                newRating = ((oldRating * oldReviews) + inRating) / newReviews;
            } else {
                newReviews = oldReviews - 1;
                newRating = ((oldRating * oldReviews) - inRating) / newReviews;
            }
            // round rating
            newRating = Math.round(newRating * 10) / 10;

            var params2 = {
                TableName: table,
                Key: {
                    "Artist":inArtist,
                    "SongTitle":inSong
                },
                UpdateExpression: "SET Rating = :updateRating, Reviews = :updateReview",
                ExpressionAttributeValues: {
                    ":updateRating": newRating,
                    ":updateReview": newReviews
                },
                ReturnValues: "ALL_NEW"
            };

            docClient.update(params2, function (err, data) {
                if (err) {
                    cb("Unable to update review", null);
                }
                else
                    cb(null, data);
            });
        }
    });
}
*/
//
//This function will get more detail for a given song and artist
//
function getSong(inArtist, inSong, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();

    console.log("Querying for " + inSong + " by " + inArtist);

    var params = {
        TableName: table,
        Key: {
            "Artist": inArtist,
            "SongTitle": inSong
        }
    };

    docClient.get(params, function(err, data) {
        if (err)
            cb("Encounter error retrieving " + inSong + " by " + inArtist, null);
        else
            cb(null, data);
    });
}

//
//This function will check if a given song/artist exists
//
function checkSong(inArtist, inSong, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();

    var params = {
        TableName: table,
        Key: {
            "Artist": inArtist,
            "SongTitle": inSong
        }
    };

    docClient.get(params, function(err, data) {
        if (err)
            cb("Song does not exist", null);
        else
            cb(null, data);
    });
}

//
//This function will check if a given song review exists
//
function checkReview(inUser, inSong, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();

    var params = {
        TableName: userTable,
        Key: {
            "UserID": inUser,
            "SongTitle": inSong
        }
    };

    docClient.get(params, function(err, data) {
        if (err)
            cb("Review does not exist", null);
        else
            cb(null, data);
    });
}

//
// This function will get songs by a given artist
//
function getArtist(inArtist, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();

    console.log("Querying for " + inArtist);

    var params = {
        TableName: table,
        ProjectionExpression: "Artist, SongTitle",
        KeyConditionExpression: "Artist = :artist",
        ExpressionAttributeValues: {
            ":artist": inArtist
        }
    };

    docClient.query(params, function(err, data) {
        if (err) {
            cb("Encounter error retrieving songs by " + inArtist, null);
        }
        else
            cb(null, data);
    });
}

//
//This function will list all the artists and songs
//
function listArtists(cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();

    console.log("Querying for all artist");

    var params = {
        TableName: table,
        ProjectionExpression: "Artist, SongTitle"
    };

    docClient.scan(params, function(err, data) {
        if (err) {
            cb("Encounter error retrieving artists", null);
        }
        else
            cb(null, data);
    });
}
//
//This function will list all the reviews
//
function listReviews(cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();

    var params = {
        TableName: userTable,
        ProjectionExpression: "UserID, SongTitle, Artist, Rating, Review"
    };

    docClient.scan(params, function(err, data) {
        if (err) {
            cb("Encounter error retrieving reviews", null);
        }
        else
            cb(null, data);
    });
}
//
//This function will add detail for a review
//
function addReview(inUser, inArtist, inSong, inRating, inDetail, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();
    var inAction = "add";

    checkSong(inArtist, inSong, function(err, data) {
        if(err) {
            cb("Song does not exist in the database", null);
        }
        else {
            var params = {
                TableName: userTable,
                Item: {
                    "UserID": inUser,
                    "SongTitle": inSong,
                    "Artist": inArtist,
                    "Rating": inRating,
                    "Review": inDetail
                },
                "ConditionExpression": "attribute_not_exists(UserID) and attribute_not_exists(SongTitle)"
            };

            docClient.put(params, function (err, data) {
                if (err) {
                    if (err.code === 'ConditionalCheckFailedException') {
                        cb("Review already exists", null);
                    } else {
                        cb("Encounter error adding review for " + inSong + " by " + inArtist, null);
                    }
                }
                else
                    updateRating(inArtist, inSong, inRating, inAction, function (err, data) {
                        if (err) {
                            console.log("Error - update rating");
                        } else {
                            cb(null, data);
                        }
                    });
            });
        }
    });
}

//
//This function will update the review detail but not the rating
//
function updateReview(inUser, inArtist, inSong, inDetail, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();

    checkSong(inArtist, inSong, function(err, data) {
        if(err) {
            cb("Song does not exist in the database", null);
        }
        else {
            var params = {
                TableName: userTable,
                Key: {
                    "UserID":inUser,
                    "SongTitle":inSong
                },
                UpdateExpression: "SET Review = :updateReview",
                ExpressionAttributeValues: {
                    ":updateReview": inDetail
                },
                ReturnValues: "ALL_NEW"
            };

            docClient.update(params, function (err, data) {
                if (err) {
                    cb("Unable to update review detail", null);
                }
                else
                    cb(null, data);
            });
        }
    });
}

//
//This function will delete the review
//
function deleteReview(inUser, inArtist, inSong, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();
    var inAction = "remove";
    var inRating;

    checkReview(inUser, inSong, function(err, data) {
        if(err) {
            cb("Song does not exist in the database", null);
        }
        else {
            if (data.Item !== undefined) {
                inRating = data.Item.Rating;
                if (inRating === undefined) {
                    inRating = 0;
                }
                var params = {
                    TableName: userTable,
                    Key: {
                        "UserID": inUser,
                        "SongTitle": inSong
                    },
                };

                docClient.delete(params, function (err, data) {
                    if (err) {
                        cb("Unable to delete review detail", null);
                    }
                    else
                        updateRating(inArtist, inSong, inRating, inAction, function (err, data) {
                            if (err) {
                                console.log("Error - update rating");
                            } else {
                                cb(null, data);
                            }
                        });
                });
            } else {
                cb("Review does not exist", null);
            }
        }
    });
}

// Response to GET request
app.get('/', function (req, res) {
    res.format({
        'default': function() {
            res.send('{"get_all_artists" : "https://localhost/Artists"'
                + ',"get_all_songs_by_artist" : "https://localhost/Artists/artist"'
                + ',"get_song_detail_by_artist" : "https://localhost/Artists/artist/song"}');
        }
    });
});

// Response to GET all songs request
app.get('/Artists', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2)
        res.send(JSON.parse('{"results":"invalid request","valid request":"/Artists"}'));
    else
        listArtists(function (err, data) {
            if(data.Count !== 0) {
                res.json({results: data});
            } else {
                res.json({results: 'database is empty'});
            }
        });
});

// Response to GET artist request
app.get('/Artists/:artist', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 3 || inArr[2] === "") {
        res.send(JSON.parse('{"results":"invalid request","valid request":"/Artists/artist"}'));
    }
    else {
        var inArtist = decodeURI(inArr[2]);
        getArtist(inArtist, function (err, data) {
            if (err) {
                res.json({errors: err});
            }
            else {
                if (data.Count !== 0) {
                    res.json({results: data});
                } else {
                    res.json({results: 'artist not found'});
                }
            }
        });
    }
});

// Response to GET artist/song request
app.get('/Artists/:artist/:song', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 4 || inArr[2] === "" || inArr[3] === "") {
        res.send(JSON.parse('{"results":"invalid request","valid request":"/Artists/artist/song"}'));
    }
    else {
        var inArtist = decodeURI(inArr[2]);
        var inSong = decodeURI(inArr[3]);
        getSong(inArtist, inSong, function (err, data) {
            if (err) {
                res.json({errors: err});
            }
            else {
                if (data.Count !== 0) {
                    res.json({results: data});
                } else {
                    res.json({results: 'song not found'});
                }
            }
        });
    }
});

//Respond to POST request to add song
app.post('/addSong', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2)
        res.send(JSON.parse('{"results":"invalid request","valid request":"/addSong"}'));
    else
        console.log(req.body);
        var inArtist = req.body.artist;
        var inSong = req.body.song;
        var inRating = req.body.rating;
        var validRequest = true;
        if (inArtist === undefined || inArtist === "" || inSong === undefined || inSong === "") {
            validRequest = false;
        }
        if (inRating === undefined || inRating === "" || isNaN(inRating) || inRating < 0 || inRating > 5){
            inRating = 5;   //set default value
        }
        if(validRequest) {
            addSong(inArtist, inSong, inRating, function (err, data) {
                if (err) {
                    res.json({errors: err});
                }
                else {
                        res.json({results: 'song added'});
                }
            });
        } else {
            res.json({results:'invalid request',valid_request:'/addSong', data:{'song':'xyz','artist':'New Artist', 'rating':3.4}});
        }
});

// Response to GET all reviews
app.get('/Reviews', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2)
        res.send(JSON.parse('{"results":"invalid request","valid request":"/Reviews"}'));
    else
        listReviews(function (err, data) {
            if(data.Count !== 0) {
                res.json({results: data});
            } else {
                res.json({results: 'there is no review'});
            }
        });
});
//Respond to POST request to add review
app.post('/addReview', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2) {
        res.send(JSON.parse('{"results":"invalid request","valid request":"/addReview"}'));
    }
    else {
        console.log(req.body);
    }
    var inUser = req.body.user;
    var inArtist = req.body.artist;
    var inSong = req.body.song;
    var inRating = req.body.rating;
    var inDetail = req.body.reviewDetail;
    var validRequest = true;
    if (inArtist === undefined || inArtist === "" || inSong === undefined || inSong === ""
        || inUser === undefined || inUser === "") {
        validRequest = false;
    }
    if (inRating === undefined || inRating === "" || isNaN(inRating) || inRating < 0 || inRating > 5){
        inRating = 5;   //set default value
    }
    if(validRequest) {
        addReview(inUser, inArtist, inSong, inRating, inDetail, function (err, data) {
            if (err) {
                res.json({errors: err});
            }
            else {
                res.json({results: 'review added'});
            }
        });
    } else {
        res.json({results:'invalid request',valid_request:'/addReview', data:{'user':'bob','song':'xyz','artist':'New Artist', 'rating':3.4, 'reviewDetail':'detail here'}});
    }
});

//Respond to PUT request to update review
app.put('/updateReview', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2) {
        res.send(JSON.parse('{"results":"invalid request","valid request":"/addReview"}'));
    }
    else {
        console.log(req.body);
    }
    var inUser = req.body.user;
    var inArtist = req.body.artist;
    var inSong = req.body.song;
    var inDetail = req.body.reviewDetail;
    var validRequest = true;
    if (inArtist === undefined || inArtist === "" || inSong === undefined || inSong === ""
        || inUser === undefined || inUser === "") {
        validRequest = false;
    }
    if(validRequest) {
        updateReview(inUser, inArtist, inSong, inDetail, function (err, data) {
            if (err) {
                res.json({errors: err});
            }
            else {
                res.json({results: 'review updated'});
            }
        });
    } else {
        res.json({results:'invalid request',valid_request:'/updateReview', data:{'user':'bob','song':'xyz','artist':'New Artist', 'reviewDetail':'detail here'}});
    }
});

//Respond to DELETE request to remove a review
app.delete('/deleteReview', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2) {
        res.send(JSON.parse('{"results":"invalid request","valid request":"/deleteReview"}'));
    }
    else {
        console.log(req.body);
    }
    var inUser = req.body.user;
    var inArtist = req.body.artist;
    var inSong = req.body.song;
    var validRequest = true;
    if (inArtist === undefined || inArtist === "" || inSong === undefined || inSong === ""
        || inUser === undefined || inUser === "") {
        validRequest = false;
    }
    if(validRequest) {
        deleteReview(inUser, inArtist, inSong, function (err, data) {
            if (err) {
                res.json({errors: err});
            }
            else {
                res.json({results: 'review deleted'});
            }
        });
    } else {
        res.json({results:'invalid request',valid_request:'/deleteReview', data:{'user':'bob','song':'xyz','artist':'New Artist'}});
    }
});
//////////////////////////////////////////
//Test function
//Respond to POST request on the root route (/), the application’s home page:
app.post('/updateRating', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2)
        res.send(JSON.parse('{"results":"invalid request","valid request":"/updateRating"}'));
    else
        console.log(req.body);
    var inArtist = req.body.artist;
    var inSong = req.body.song;
    var inRating = req.body.rating;
    var inAction = req.body.inAction;
    var validRequest = true;

    if(validRequest) {
        updateRating(inArtist, inSong, inRating, inAction, function (err, data) {
            if (err) {
                res.json({errors: err});
            }
            else {
                res.json({results: 'Rating added'});
            }
        });
    }
});

//Respond to a DELETE request to the /user route:
app.delete('/user', function (req, res) {
        res.send('Got a DELETE request at /user');
    });

app.listen(3000, function () {
    console.log('Example app listening on port 3000!');
});