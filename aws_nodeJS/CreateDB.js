// Load the SDK and UUID
/*
var AWS = require('aws-sdk');
var uuid = require('node-uuid');

// Create an S3 client
var s3 = new AWS.S3();

// Create a bucket and upload something into it
var bucketName = 'node-sdk-sample-' + uuid.v4();
var keyName = 'hello_world.txt';

s3.createBucket({Bucket: bucketName}, function() {
  var params = {Bucket: bucketName, Key: keyName, Body: 'Hello World!'};
  s3.putObject(params, function(err, data) {
    if (err)
      console.log(err)
    else
      console.log("Successfully uploaded data to " + bucketName + "/" + keyName);
  });
});
*/
/// create database
/**
 * Created by Huy on 4/21/2016.
 */
var AWS = require("aws-sdk");

AWS.config.update({
  region: "us-west-2",
  endpoint: "http://localhost:8000"
});

var dynamodb = new AWS.DynamoDB();

var params = {
  TableName : "Users",
  KeySchema: [
    { AttributeName: "UserID", KeyType: "HASH" },  //Partition key
    { AttributeName: "SongTitle", KeyType: "RANGE" }  //Sort key
  ],
  AttributeDefinitions: [
    { AttributeName: "UserID", AttributeType: "S" },
    { AttributeName: "SongTitle", AttributeType: "S" }
  ],
  ProvisionedThroughput: {
    ReadCapacityUnits: 1,
    WriteCapacityUnits: 1
  }
};

dynamodb.createTable(params, function(err, data) {
  if (err) {
    console.error("Unable to create table. Error JSON:", JSON.stringify(err, null, 2));
  } else {
    console.log("Created table. Table description JSON:", JSON.stringify(data, null, 2));
  }
});