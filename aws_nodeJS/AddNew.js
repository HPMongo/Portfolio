/**
 * Created by Huy on 4/21/2016.
 */
var AWS = require("aws-sdk");

AWS.config.update({
    region: "us-west-2",
    endpoint: "http://localhost:8000"
});

var docClient = new AWS.DynamoDB.DocumentClient();

var table = "Music";

var params = {
    TableName: table,
    Item: {
        "User":"U2",
        "SongTitle":"U2 Song",
        "Artist":"",
        "Rating": 5.0,
        "Review":"Best song evar"
    },
    "ConditionExpression": "attribute_not_exists(Artist) and attribute_not_exists(SongTitle)"
};

console.log("Adding a new item...");
docClient.put(params, function(err, data) {
    if (err) {
        console.error("Unable to add item. Error JSON:", JSON.stringify(err, null, 2));
    } else {
        console.log("Added item:", JSON.stringify(data, null, 2));
    }
});
