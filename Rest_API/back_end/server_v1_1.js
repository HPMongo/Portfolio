/**
 * Created by Huy on 4/23/2016.
 * Updated on 5/23/2016 to include get request for users
 */
var express = require('express')
   , cors = require('cors')
   , app = express();
var bodyParser = require('body-parser');
var AWS = require("aws-sdk");
var crypto = require('crypto');

//var app = express();
var table = "Music";
var userTable = "Users";
var accountTable = "Account";

AWS.config.update({
    region: "us-west-2",
    endpoint: "http://dynamodb.us-west-2.amazonaws.com/"
});

/*
AWS.config.update({
    region: "us-west-2",
    endpoint: "http://localhost:8000"
});
*/

app.use(bodyParser.json());     // Parse request body
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cors());
app.options('*',cors()); // enable cors pre-flight
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
            cb("Encounter error retrieving " + inSong + " by " + inArtist, null, 500);
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
                    cb("Request completed but unable to update review rating", data, 200);
                }
                else
                    cb("Rating is updated", data, 200);
            });
        }
    });
}

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
//This function will check if a given account exists
//
function checkAccount(inUser, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();
    var msg;
    console.log("Checking account for user - " + inUser);
    var params = {
        TableName: accountTable,
        KeyConditionExpression: "UserID = :userID",
        ExpressionAttributeValues: {
            ":userID": inUser
        }
    };

    docClient.query(params, function(err, data) {
        if (err)
            cb("Encounter error retrieving account", null);
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
// This function will get reviews by a given user
//
function getUser(inUser, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();
    console.log("Querying for " + inUser);

    var params = {
        TableName: userTable,
        ProjectionExpression: "UserID, SongTitle",
        KeyConditionExpression: "UserID = :user",
        ExpressionAttributeValues: {
            ":user": inUser
        }
    };

    docClient.query(params, function(err, data) {
        if (err) {
            cb("Encounter error retrieving reviews by " + inUser, null, 500);
        }
        else
            cb("Review retrieved", data, 200);
    });
}
//
// This function will get a specific review by a given user
//
function getReview(inUser, inSong, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();

    console.log("Querying for " + inUser + " with review on " + inSong);

    var params = {
        TableName: userTable,
        Key: {
            "UserID": inUser,
            "SongTitle": inSong
        }
    };

    docClient.get(params, function(err, data) {
        if (err)
            cb("Encounter error retrieving review of " + inSong + " by " + inUser, null, 500);
        else
            cb("Review detail retrieved", data, 200);
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
            cb("Encounter error retrieving artists", null, 500);
        }
        else
            cb("Returning result from music table", data, 200);
    });
}
//
//This function will list all the artists and songs
//
function listUsers(cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();

    console.log("Querying for all users");

    var params = {
        TableName: userTable,
        ProjectionExpression: "UserID, SongTitle"
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
//This function will add a new account
//
/*
function addAccount(inUser, inPassword, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();
    var salt;
    var msg;
    console.log("In addAccount");
    checkAccount(inUser, function(err, data) {
        if(err) {
	    msg = "Encounter error retrieve account information";
	    console.log(msg);
            cb(msg, null);
        }
        else
        {
            if(data.Count === 0)
            {
                crypto.randomBytes(256, function (err, buf) {
                    if (err) {
			msg = "Error generating salt";
			console.log(msg);
                        cb(msg, null);
                    }
                    else {
                        salt = buf.toString('hex');
                        crypto.pbkdf2(inPassword, salt, 100000, 512, 'sha512', function (err, key) {
                            if (err) {
 				msg = "Error encrypting message";
				console.log(msg);
                        	cb(msg, null);
                            }
                            else {
                                var hashKey = key.toString('hex');
                                var params = {
                                    TableName: accountTable,
                                    Item: {
                                        "UserID": inUser,
                                        "HashValue": hashKey,
                                        "SaltValue": salt
                                    },
                                    "ConditionExpression": "attribute_not_exists(UserID)"
                                };

                                docClient.put(params, function (err, data) {
                                    if (err) {
                                        if (err.code === 'ConditionalCheckFailedException') {
					    msg = "Error - account already exists";
				            console.log(msg);
                        	   	    cb(msg, null);
                                        } else {
					    msg = "Encounter error adding account for " + inUser;
					    console.log(msg);
					    cb(msg, null);
                                        }
                                    }
                                    else
					msg = "Account added";
					console.log(msg);
                                        cb(null, msg);
                                });
                            }
                        });
                    }
                });
            }
            else
            {
		msg = "Account already exists";
		console.log(msg);
                cb(msg, null);
            }
        }
    });
}
*/
function addAccount(inUser, inPassword, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();
    var salt, msg;
    if (inPassword === undefined || inPassword === "" || inUser === undefined || inUser === "") {
        msg = "'invalid request',valid_request:'/addAccount', data:'user':'bob','userPW':'xyz'"
        cb(msg, null, 400);
    }
    else {
        checkAccount(inUser, function (err, data) {
            if (err) {
                cb("Encounter error retrieve account information", null, 500);
            }
            else {
                if (data !== null && data !== undefined && data.Count === 0) {
                    crypto.randomBytes(256, function (err, buf) {
                        if (err) {
                            msg = "Error generating salt";
                            cb(msg, null, 500);
                        }
                        else {
                            salt = buf.toString('hex');
                            crypto.pbkdf2(inPassword, salt, 100000, 512, 'sha512', function (err, key) {
                                if (err) {
                                    msg = "Error encrypting message";
                                    cb(msg, null, 500);
                                }
                                else {
                                    var hashKey = key.toString('hex');
                                    var params = {
                                        TableName: accountTable,
                                        Item: {
                                            "UserID": inUser,
                                            "HashValue": hashKey,
                                            "SaltValue": salt
                                        },
                                        "ConditionExpression": "attribute_not_exists(UserID)"
                                    };

                                    docClient.put(params, function (err, data) {
                                        if (err) {
                                            if (err.code === 'ConditionalCheckFailedException') {
                                                msg = "Error - account already exists. Please sign in.";
                                                cb(msg, null, 400);
                                            } else {
                                                msg = "Encounter error adding account for " + inUser;
                                                cb(msg, null, 500);
                                            }
                                        }
                                        else {
                                            var result = inUser;
                                            msg = "Account added";
                                            cb(msg, result, 200);
                                        }
                                    });
                                }
                            });
                        }
                    });
                }
                else {
                    msg = "Account already exists. Please sign in.";
                    cb(msg, null, 400);
                }
            }
        });
    }
}
//
//This function will validate credential of an account
//
/*
function validateAccount(inUser, inPassword, cb) {
    var salt, storedHash;

    checkAccount(inUser, function(err, data) {
        if(err) {
            cb("Encounter error retrieve account information", null);
        }
        else
        {
            if(data.Count === 0)
            {
                cb("Account does not exist", null);
            }
            else
            {
                salt = data.Items[0].SaltValue;
                storedHash = data.Items[0].HashValue;
                crypto.pbkdf2(inPassword, salt, 100000, 512, 'sha512', function (err, key) {
                    if (err) {
                        cb("Error encypting message", null);
                    }
                    else {
                        var hashKey = key.toString('hex');
                        if(hashKey === storedHash) {
			    console.log("Account is valid");
                            cb(null, "Account validated");
                        }
                        else
                        {
			    console.log("Invalid password");
                            cb("Invalid password", null);
                        }
                    }
                });
            }
        }
    });
}
*/
function validateAccount(inUser, inPassword, cb) {
    var salt, storedHash;
    if (inPassword === undefined || inPassword === "" || inUser === undefined || inUser === "") {
        var msg = "'invalid request',valid_request:'/validateAccount', data:'user':'bob','userPW':'xyz'"
        cb(msg, null, 400);
    } else {
        checkAccount(inUser, function (err, data) {
            if (err) {
                cb("Encounter error retrieve account information", null, 500);
            }
            else {
                if(data !== null && data !== undefined && data.Count === 0) {
                    cb("Account does not exist", null, 100);
                }
                else {
                    salt = data.Items[0].SaltValue;
                    storedHash = data.Items[0].HashValue;
                    crypto.pbkdf2(inPassword, salt, 100000, 512, 'sha512', function (err, key) {
                        if (err) {
                            cb("Error encypting message", null, 600);
                        }
                        else {
                            var hashKey = key.toString('hex');
                            if (hashKey === storedHash) {
                                console.log("Account is valid");
                                var result = inUser;
                                cb("Account validated", result, 200);
                            }
                            else {
                                console.log("Invalid password");
                                cb("Invalid password", null, 300);
                            }
                        }
                    });
                }
            }
        });
    }
}
//
//This function will add detail for a review
//
function addReview(inUser, inArtist, inSong, inRating, inDetail, cb) {
    var docClient = new AWS.DynamoDB.DocumentClient();
    var inAction = "add";

    checkSong(inArtist, inSong, function(err, data, rstat) {
        if(err) {
            cb("Song does not exist in the database", null, 100);
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

            docClient.put(params, function (err, data, rstat) {
                if (err) {
                    if (err.code === 'ConditionalCheckFailedException') {
                        cb("Review already exists", null, 400);
                    } else {
                        cb("Encounter error adding review for " + inSong + " by " + inArtist, null, 500);
                    }
                }
                else
                    updateRating(inArtist, inSong, inRating, inAction, function (err, data, rstat) {
                        if (err) {
                            cb('Review added', data, 200);
                        } else {
                            cb('Review added successfully', data, 200);
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

    checkSong(inArtist, inSong, function(err, data, rstat) {
        if(err) {
            cb("Song does not exist in the database", null, 100);
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
                    cb("Unable to update review detail", null, 500);
                }
                else
                    cb("Review updated", data, 200);
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

    checkReview(inUser, inSong, function(err, data, rstat) {
        if(err) {
            cb("Song does not exist in the database", null, 100);
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

                docClient.delete(params, function (err, data, rstat) {
                    if (err) {
                        cb("Unable to delete review detail", null, 400);
                    }
                    else
                        updateRating(inArtist, inSong, inRating, inAction, function (err, data, rstat) {
                            if (err) {
                                cb("Review deleted", data, 200);
                            } else {
                                cb("Review deleted successfully", data, 200);
                            }
                        });
                });
            } else {
                cb("Review does not exist", null, 100);
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
    var msg;
    if (inArr.length !== 2)
        res.send(JSON.parse('{"results":"invalid request","valid request":"/Artists",message: "Invalid request", status: 400}'));
    else
        listArtists(function (err, data, rstat) {
            if(data !== null && data !== undefined) {
                if(data.Count !== 0) {
                    console.log('Return query result to client - set rstat to ' + 200);
                    res.json({results: data, message: 'Query successful',status: 200});
                } else {
                    console.log('Music database is empty ' + 100);
                    res.json({results: data, message: 'It appears that there is no song to review at the moment.',status: 100});}
            } else {
                console.log('Encounter error querying the music database - set rstat to' + 500);
                res.json({results: data, message: err,status: 500});
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
        res.send(JSON.parse('{"results":"invalid request","valid request":"/addReview"}, "message":"invalid request", "status":400'));
    }
    else {
        var inUser = req.body.user;
        var inArtist = req.body.artist;
        var inSong = req.body.song;
        var inRating = req.body.rating;
        var inDetail = req.body.reviewDetail;
        if (inArtist === undefined || inArtist === "" || inSong === undefined || inSong === ""
            || inUser === undefined || inUser === "") {
            res.json({results:'invalid request',valid_request:'/addReview', data:{'user':'bob','song':'xyz','artist':'New Artist', 'rating':3.4, 'reviewDetail':'detail here', message: 'invalid request', status: 400}});
        } else {
            if (inRating === undefined || inRating === "" || isNaN(inRating) || inRating < 0 || inRating > 5){
                inRating = 5;   //set default value
            }
            addReview(inUser, inArtist, inSong, inRating, inDetail, function (err, data, rstat) {
                if (rstat !== 200) {
                    console.log('Encounter error adding review set rstat to ' + rstat);
                    res.json({results: data, message: err,status: rstat});
                }
                else {
                    console.log('Review added - set rstat to' + rstat);
                    res.json({results: data, message: err,status: rstat});
                }
            });
        }
    }
});

//Respond to PUT request to update review
app.put('/updateReview', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2) {
        res.send(JSON.parse('{"results":"invalid request","valid request":"/updateReview"}'));
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
        console.log('Invalid update request');
        res.json({results:'invalid request',valid_request:'/updateReview', data:{'user':'bob','song':'xyz','artist':'New Artist', 'reviewDetail':'detail here'}, message: 'Invalid request', status: 400});
    } else {
        updateReview(inUser, inArtist, inSong, inDetail, function (err, data, rstat) {
            if (rstat !== 200) {
                console.log('Update invalid - set rstat to' + rstat);
                res.json({results: data, message: err,status: rstat});
            }
            else {
                console.log('Update valid - set rstat to' + rstat);
                res.json({results: data, message: err,status: rstat});
            }
        });
    }
});

//Respond to DELETE request to remove a review
app.delete('/deleteReview', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2) {
        res.send(JSON.parse('{"results":"invalid request","valid request":"/deleteReview", "message":"invalid request", "status":400}'));
    }
    else {
        var inUser = req.body.user;
        var inArtist = req.body.artist;
        var inSong = req.body.song;
        if (inArtist === undefined || inArtist === "" || inSong === undefined || inSong === ""
            || inUser === undefined || inUser === "") {
            res.json({results:'invalid request',valid_request:'/deleteReview', data:{'user':'bob','song':'xyz','artist':'New Artist', message: 'invalid request', status: 400}});
        } else {
            deleteReview(inUser, inArtist, inSong, function (err, data, rstat) {
                if (rstat !== 200) {
                    console.log('Encounter error deleting review - set rstat to' + rstat);
                    res.json({results: data, message: err,status: rstat});
                }
                else {
                    console.log('Review deleted - set rstat to' + rstat);
                    res.json({results: data, message: err,status: rstat});
                }
            });
        }
    }
});
//Respond to POST request to add new account
app.post('/addAccount', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2) {
	    console.log("Invalid request");
        res.send(JSON.parse('{"results":"invalid request","valid request":"/addAccount", "status":"400"}'));
    }
    else {
        addAccount(req.body.user, req.body.userPW, function (err, data, rstat) {
            if (err) {
                console.log('Add account message: ' + err + ' -- set stat code to ' + rstat);
                res.json({results: data, message: err, status: rstat});
            }
            else {
                console.log('Add account valid - set rstat to' + rstat);
                res.json({results: data, message: err, status: rstat});
            }
        });
    }
});
//Respond to POST request to validate account
app.post('/validateAccount', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2) {
        res.send(JSON.parse('{"results":"invalid request","valid request":"/validatedAccount", "status":"400"}'));
    }
    else {
        validateAccount(req.body.user, req.body.userPW, function (err, data, rstat) {
            if (rstat !== 200) {
                console.log('Account valid - set rstat to' + rstat);
                res.json({results: data, message: err,status: rstat});
            }
            else {
                console.log('Account valid - set rstat to' + rstat);
                res.json({results: data, message: err,status: rstat});
            }
        });
    }
});
// Response to GET all users request
app.get('/Users', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 2)
        res.send(JSON.parse('{"results":"invalid request","valid request":"/Users"}'));
    else
        listUsers(function (err, data) {
            if(data.Count !== 0) {
		res.status(200);
                res.json({results: data});
            } else {
		//res.status(200);
                res.writeHead(
                    "200",
                    "OK",
                    {
                        "access-control-allow-origin": origin,
		    }
		);
                res.json({results: 'database is empty'});
            }
        });
});

// Response to GET an user request
app.get('/Users/:user', function (req, res) {
    var inArr = req.url.split('/');
    var msg;
    if (inArr.length !== 3 || inArr[2] === "") {
        res.send(JSON.parse('{"results":"invalid request","valid request":"/Users/user"},status:400'));
    }
    else {
        var inUser = decodeURI(inArr[2]);
        getUser(inUser, function (err, data, rstat) {
            if (rstat !== 200) {
                console.log('Reviews retrieved - set rstat to' + rstat);
                res.json({results: data, message: err,status: rstat});
            } else {
                if(data === null || data === undefined) {
                    msg = 'Encounter error retrieving user review';
                    console.log(msg + ' - set rstat to' + 500);
                    res.json({results: data, message: err,status: 500});
                } else {
                    if (data.Count !== 0) {
                        console.log('Review(s) exist - set rstat to' + 200);
                        res.json({results: data, message: err,status: 200});
                    } else {
                        msg = 'No review exist';
                        console.log(msg + 'set rstat to ' + 100);
                        res.json({results: data, message: err,status: 100});
                    }
                }
            }
        });
    }
});

// Response to GET artist/song request
app.get('/Users/:user/:song', function (req, res) {
    var inArr = req.url.split('/');
    if (inArr.length !== 4 || inArr[2] === "" || inArr[3] === "") {
        res.send(JSON.parse('{"results":"invalid request","valid request":"/Users/user/song"}, status:400'));
    }
    else {
        var inUser = decodeURI(inArr[2]);
        var inSong = decodeURI(inArr[3]);
        var msg;
        getReview(inUser, inSong, function (err, data, rstat) {
            if (rstat !== 200) {
                console.log('Review detail retrieved - set rstat to' + rstat);
                res.json({results: data, message: err,status: rstat});            }
            else {
                if(data === null || data === undefined) {
                    msg = 'Encounter error retrieving user review';
                    console.log(msg + ' - set rstat to' + 500);
                    res.json({results: data, message: err, status: 500});
                } else {
                    if (data.Count !== 0) {
                        console.log('Detail review exist - set rstat to' + 200);
                        res.json({results: data, message: err,status: 200});
                    } else {
                        msg = 'No detail review exist';
                        console.log(msg + 'set rstat to ' + 100);
                        res.json({results: data, message: err,status: 100});
                    }
                }
            }
        });
    }
});

app.listen(3000, function () {
    console.log('Listening on port 3000!');
});