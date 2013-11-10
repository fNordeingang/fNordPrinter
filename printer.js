#!/usr/bin/node

var twitter = require('ntwitter');
var iconv = require('iconv-lite');
var fs = require('fs');
var util = require('util');

if ( process.argv.length < 8 ) {
  console.log("usage: node printer.js [consumer_key] [consumer_secret] [access_token_key] [access_token_secret] [track] [file]");
  process.exit(1);
}

var twit = new twitter({
    consumer_key        : process.argv[2],
    consumer_secret     : process.argv[3],
    access_token_key    : process.argv[4],
    access_token_secret : process.argv[5]
});

var fileStream = fs.createWriteStream(process.argv[7]);

fileStream.on("end", function() {
  fileStream.end();
});

var track = process.argv[6];
twit.stream('statuses/filter', {'track': track }, function(stream) {
  stream.on('data', function (data) {
    var user = data.user.screen_name;
    var date = data.created_at;
    var text = data.text;
    var tweet = util.format("%s @%s:\n%s\n\n", date, user, text);
    var converted = iconv.encode(tweet, 'CP437');
    fileStream.write(converted);
    console.log(tweet);
  });
});


