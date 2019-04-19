# node.js发http请求 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年01月31日 09:20:14[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：276
标准库中默认的HTTP模块
**const** https
 = require('https');
https.get('https://api.nasa.gov/planetary/apod?api_key=DEMO_KEY',
 (resp) => { **let** data =
'';
// A chunk of data has been recieved. resp.on('data',
 (chunk) => { data += chunk; }); // The whole response has been received. Print out the result.
 resp.on('end', () => {
console.log(JSON.parse(data).explanation);
 }); }).on("error", (err) => {
console.log("Error:
 " + err.message);});
需要以区块为单位接收响应数据
HTTP和HTTPS协议分属两个模块
Request
npm install request
npm install
[request@2.81.0](mailto:request@2.81.0)
**const** request
 = require('request');
 request('https://api.nasa.gov/planetary/apod?api_key=DEMO_KEY', { json:
true }, (err, res, body) => {
**if** (err) {
**return**console.log(err); }
console.log(body.url);
console.log(body.explanation);});
或者通过
if (!error && response.statusCode ==
200) { console.log(body)
// Show the HTML for the baidu homepage. }
判断

发post
application/x-www-form-urlencoded
request.post({url:'http://service.com/upload',
 form:{key:'value'}},
function(error, response, body) {
if (!error && response.statusCode ==
200) { }})
[https://github.com/request/request](https://github.com/request/request)
[https://segmentfault.com/a/1190000000385867](https://segmentfault.com/a/1190000000385867)
[https://segmentfault.com/a/1190000010698468](https://segmentfault.com/a/1190000010698468)
