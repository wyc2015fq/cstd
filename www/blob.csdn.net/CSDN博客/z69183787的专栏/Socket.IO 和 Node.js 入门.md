# Socket.IO 和 Node.js 入门 - z69183787的专栏 - CSDN博客
2016年09月11日 19:02:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：870
个人分类：[前端-NodeJs																[服务器推-SocketIo](https://blog.csdn.net/z69183787/article/category/6412785)](https://blog.csdn.net/z69183787/article/category/5669799)

[Node.js](http://www.oschina.net/p/nodejs) 并不能做所有事情，但它可通过 [Socket.IO](http://www.oschina.net/p/socket-io) 库轻松实现 [WebSockets](http://www.oschina.net/p/websocket)。使用
 WebSockets 可轻松构建实时多用户的应用程序或游戏，今天我们将讲述如何使用 Node.js 和 Socket.IO 构建简单的聊天程序。
注意: 在阅读文章之前你应该先安装好 Node.js，同时你需要一个自己喜欢的文本编辑器，本文提及的一些源码有些改自 [http://book.mixu.net/ch13.html](http://book.mixu.net/ch13.html)
下面我们开始入门教程：
首先在你的电脑上创建一个新目录，姑且命名为 chat，然后在该目录创建两个文件，分别是 app.js 和 index.html。
app.js 文件内容如下：
[?](http://www.oschina.net/question/12_54009/#)
```
`var`
```
fs = require(
```
`'fs'``)`
`    `
```
,
 http = require(
```
`'http'``)`
`    `
```
,
 socketio = require(
```
`'socket.io'``);`
` `
`var`
```
server = http.createServer(
```
`function`
```
(req,
 res) {
```
`    `
```
res.writeHead(200,
 {
```
`'Content-type'`
```
:
```
`'text/html'``});`
`    `
```
res.end(fs.readFileSync(__dirname
 +
```
`'/index.html'``));`
```
}).listen(8080,
```
`function`
```
()
 {
```
`    ``console.log(`
```
'Listening
 at: http://localhost:8080'
```
`);`
`});`
` `
`socketio.listen(server).on(``'connection'`
```
,
```
`function`
```
(socket) {
```
`    ``socket.on(``'message'`
```
,
```
`function`
```
(msg) {
```
`        ``console.log(`
```
'Message
 Received: '
```
```
,
 msg);
```
`        ``socket.broadcast.emit(``'message'`
```
,
 msg);
```
`    ``});`
`});`
```
上述代码是一个超级简单的聊天服务器的实现，该服务器发送 index.html 并侦听所有 WebSockets 请求，如果你发送一个 hi 的聊天信息，格式如下：
[?](http://www.oschina.net/question/12_54009/#)
```
`{``"name"``:``"message"``,``"args"``:[``"hi"``]}`
```
index.html 页面代码如下：
[?](http://www.oschina.net/question/12_54009/#)
```
`<``html``>`
`<``head``>`
`    ``<``script`
```
src
```
`=``"http://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"``></``script``>`
`    ``<``script`
```
src
```
`=``"/socket.io/socket.io.js"``></``script``>`
`    ``<``script``>`
`        ``$(function(){`
`            `
```
var
 iosocket = io.connect();
```
` `
`            `
```
iosocket.on('connect',
 function () {
```
`                ``$('#incomingChatMessages').append($('<``li``>Connected</``li``>'));`
` `
`                `
```
iosocket.on('message',
 function(message) {
```
`                    ``$('#incomingChatMessages').append($('<``li``></``li``>').text(message));`
`                ``});`
`                `
```
iosocket.on('disconnect',
 function() {
```
`                    ``$('#incomingChatMessages').append('<``li``>Disconnected</``li``>');`
`                ``});`
`            ``});`
` `
`            `
```
$('#outgoingChatMessage').keypress(function(event)
 {
```
`                `
```
if(event.which
 == 13) {
```
`                    ``event.preventDefault();`
`                    ``iosocket.send($('#outgoingChatMessage').val());`
`                    ``$('#incomingChatMessages').append($('<``li``></``li``>').text($('#outgoingChatMessage').val()));`
`                    ``$('#outgoingChatMessage').val('');`
`                ``}`
`            ``});`
`        ``});`
`    ``</``script``>`
`</``head``>`
`<``body``>`
```
Incoming
 Chat: <
```
`ul`
```
id
```
`=``"incomingChatMessages"``></``ul``>`
`<``br`
```
/>
```
`<``input`
```
type
```
`=``"text"`
```
id
```
`=``"outgoingChatMessage"``>`
`</``body``>`
`</``html``>`
```
该页面虽然不怎么吸引人，但是可正常工作。接下来打开两个浏览器，必须得是支持 WebSockets 的浏览器，例如 Chrome 或者 Safari。
下一步就是安装 Socket.IO 了，可在命令行窗口中执行如下命令：
[?](http://www.oschina.net/question/12_54009/#)
```
```
$
 npm
```
`install`
```
socket.io
```
```
然后运行 app.js 文件：
[?](http://www.oschina.net/question/12_54009/#)
```
```
$
 node app.js
```
```
[](http://www.giantflyingsaucer.com/blog/wp-content/uploads/2012/05/Screen-Shot-2012-05-05-at-7.32.41-PM.png)
现在你可以打开两个浏览器，访问 http://localhost:8080/ 地址开始互聊了。
[英文原文](http://www.giantflyingsaucer.com/blog/?p=3751)，[OSCHINA](http://www.oschina.net/question/12_54009)原创翻译
[Node.js](http://www.oschina.net/p/nodejs) 并不能做所有事情，但它可通过 [Socket.IO](http://www.oschina.net/p/socket-io) 库轻松实现 [WebSockets](http://www.oschina.net/p/websocket)。使用
 WebSockets 可轻松构建实时多用户的应用程序或游戏，今天我们将讲述如何使用 Node.js 和 Socket.IO 构建简单的聊天程序。
**注意:** 在阅读文章之前你应该先安装好 Node.js，同时你需要一个自己喜欢的文本编辑器，本文提及的一些源码有些改自 [http://book.mixu.net/ch13.html](http://book.mixu.net/ch13.html)
下面我们开始入门教程：
首先在你的电脑上创建一个新目录，姑且命名为 chat，然后在该目录创建两个文件，分别是 app.js 和 index.html。
app.js 文件内容如下：
[?](http://www.oschina.net/question/12_54009/#)
```
`var`
```
fs = require(
```
`'fs'``)`
`    `
```
,
 http = require(
```
`'http'``)`
`    `
```
,
 socketio = require(
```
`'socket.io'``);`
` `
`var`
```
server = http.createServer(
```
`function`
```
(req,
 res) {
```
`    `
```
res.writeHead(200,
 {
```
`'Content-type'`
```
:
```
`'text/html'``});`
`    `
```
res.end(fs.readFileSync(__dirname
 +
```
`'/index.html'``));`
```
}).listen(8080,
```
`function`
```
()
 {
```
`    ``console.log(`
```
'Listening
 at: http://localhost:8080'
```
`);`
`});`
` `
`socketio.listen(server).on(``'connection'`
```
,
```
`function`
```
(socket) {
```
`    ``socket.on(``'message'`
```
,
```
`function`
```
(msg) {
```
`        ``console.log(`
```
'Message
 Received: '
```
```
,
 msg);
```
`        ``socket.broadcast.emit(``'message'`
```
,
 msg);
```
`    ``});`
`});`
```
上述代码是一个超级简单的聊天服务器的实现，该服务器发送 index.html 并侦听所有 WebSockets 请求，如果你发送一个 hi 的聊天信息，格式如下：
[?](http://www.oschina.net/question/12_54009/#)
```
`{``"name"``:``"message"``,``"args"``:[``"hi"``]}`
```
**index.html** 页面代码如下：
[?](http://www.oschina.net/question/12_54009/#)
```
`<``html``>`
`<``head``>`
`    ``<``script`
```
src
```
`=``"http://ajax.googleapis.com/ajax/libs/jquery/1.7.2/jquery.min.js"``></``script``>`
`    ``<``script`
```
src
```
`=``"/socket.io/socket.io.js"``></``script``>`
`    ``<``script``>`
`        ``$(function(){`
`            `
```
var
 iosocket = io.connect();
```
` `
`            `
```
iosocket.on('connect',
 function () {
```
`                ``$('#incomingChatMessages').append($('<``li``>Connected</``li``>'));`
` `
`                `
```
iosocket.on('message',
 function(message) {
```
`                    ``$('#incomingChatMessages').append($('<``li``></``li``>').text(message));`
`                ``});`
`                `
```
iosocket.on('disconnect',
 function() {
```
`                    ``$('#incomingChatMessages').append('<``li``>Disconnected</``li``>');`
`                ``});`
`            ``});`
` `
`            `
```
$('#outgoingChatMessage').keypress(function(event)
 {
```
`                `
```
if(event.which
 == 13) {
```
`                    ``event.preventDefault();`
`                    ``iosocket.send($('#outgoingChatMessage').val());`
`                    ``$('#incomingChatMessages').append($('<``li``></``li``>').text($('#outgoingChatMessage').val()));`
`                    ``$('#outgoingChatMessage').val('');`
`                ``}`
`            ``});`
`        ``});`
`    ``</``script``>`
`</``head``>`
`<``body``>`
```
Incoming
 Chat: <
```
`ul`
```
id
```
`=``"incomingChatMessages"``></``ul``>`
`<``br`
```
/>
```
`<``input`
```
type
```
`=``"text"`
```
id
```
`=``"outgoingChatMessage"``>`
`</``body``>`
`</``html``>`
```
该页面虽然不怎么吸引人，但是可正常工作。接下来打开两个浏览器，必须得是支持 WebSockets 的浏览器，例如 Chrome 或者 Safari。
下一步就是安装 Socket.IO 了，可在命令行窗口中执行如下命令：
[?](http://www.oschina.net/question/12_54009/#)
```
```
$
 npm
```
`install`
```
socket.io
```
```
然后运行 **app.js** 文件：
[?](http://www.oschina.net/question/12_54009/#)
```
```
$
 node app.js
```
```
[](http://www.giantflyingsaucer.com/blog/wp-content/uploads/2012/05/Screen-Shot-2012-05-05-at-7.32.41-PM.png)
现在你可以打开两个浏览器，访问 http://localhost:8080/ 地址开始互聊了。
[英文原文](http://www.giantflyingsaucer.com/blog/?p=3751)，[OSCHINA](http://www.oschina.net/question/12_54009)原创翻译
