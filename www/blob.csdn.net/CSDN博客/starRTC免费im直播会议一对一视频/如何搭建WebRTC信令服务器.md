# 如何搭建WebRTC信令服务器 - starRTC免费im直播会议一对一视频 - CSDN博客
2019年04月09日 09:52:13[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：4
WebRTC 有一整套规范，如怎样使用它的接口、使用SDP进行媒体协商、通过ICE收集地址并进行连通性检测等等。除此之外，WebRTC还需要房间服务器将多端聚集到一起管理，以及信令服务器进行信令数据交换（如媒体描述信息SDP的交换，连接地址的交换等），但在WebRTC的规范中没有对这部分内容进行规定，所以需要由用户自己处理。
安装 Nodejs
下面我们就来看看具体如何安装 Nodejs。
安装 Nodejs 非常的简单:
在Ubuntu系统下执行：
apt install nodejs
或在Mac 系统下执行：
brew install nodejs
通过上面的步骤我们就将 Nodejs 安装好了。我这里安装的 Nodejs版本为：v8.10.0。
安装NPM
除了安装 Nodejs 之外，我们还要安装NPM(Node Package Manager），也就是 Nodejs 的包管理器。它就像Ubuntu下的 apt 或Mac 系统下的brew 命令类似，是专门用来管理各种依赖库的。
对于 Nodejs的安装包也是如此，NPM 就是相当于 Linux 下的 apt，它的出现大大提高了人们的工作效率。
NPM 的安装像安装 Nodejs 一样简单:
在Ubuntu下执行:
apt install npm
或在Mac下执行：
brew install npm
[socket.io 6](http://socket.io/)
此次，我们使用 Nodejs 下的 [socket.io 6](http://socket.io/) 库来实现 WebRTC 信令服务器。socket.io特别适合用来开发WebRTC的信令服务器，通过它来构建信令服务器特别的简单，这主要是因为它内置了房间 的概念。
socket.io.jpg2308×895 139 KB
上图是 [socket.io 6](http://socket.io/) 与 Nodejs配合使用的逻辑关系图， 其逻辑非常简单。[socket.io 6](http://socket.io/) 分为服务端和客户端两部分。服务端由 Nodejs加载后侦听某个服务端口，客户端要想与服务端相连，首先要加载 [socket.io 6](http://socket.io/) 的客户端库，然后调用 io.connect();就与服务端连上了。
需要特别强调的是 [socket.io 6](http://socket.io/) 消息的发送与接收。[socket.io 6](http://socket.io/) 有很多种发送消息的方式，其中最常见的有下面几种，是我们必须要撑握的：
- 给本次连接发消息
socket.emit()
- 给某个房间内所有人发消息
io.in(room).emit()
- 除本连接外，给某个房间内所有人发消息
socket.to(room).emit()
- 除本连接外，给所以人发消息
socket.broadcast.emit()
消息又该如何接收呢？
- 发送 command 命令
S: socket.emit('cmd’); C: socket.on('cmd',function(){...});
- 送了一个 command 命令，带 data 数据
S: socket.emit('action', data); C: socket.on('action',function(data){...});
- 发送了command命令，还有两个数据
S: socket.emit(action,arg1,arg2); C: socket.on('action',function(arg1,arg2){...});
有了以上这些知识，我们就可以实现信令数据通讯了。
搭建信令服务器
接下来我们来看一下，如何通过 Nodejs下的 [socket.io 6](http://socket.io/) 来构建的一个服务器：
这是客户端代码，也就是在浏览器里执行的代码。index.html：
<!DOCTYPE html> <html> <head> <title>WebRTC client</title> </head> <body> <script src='/socket.io/socket.io.js'></script> <script src='js/client.js'></script> </body> </html>
该代码十分简单，就是在body里引入了两段 JS 代码。其中，socket.io.js 是用来与服务端建立 socket 连接的。client.js 的作用是做一些业务逻辑，并最终通过 socket 与服务端通讯。
首先，在server.js目录下创建 js 子目录，然后在 js目录下生成 client.js。
下面是client.js的代码：
var isInitiator; room = prompt('Enter room name:'); //弹出一个输入窗口 const socket = io.connect(); //与服务端建立socket连接 if (room !== '') { //如果房间不空，则发送 "create or join" 消息 console.log('Joining room ' + room); socket.emit('create or join', room); } socket.on('full', (room) => { //如果从服务端收到 "full" 消息 console.log('Room ' + room + ' is full'); }); socket.on('empty', (room) => { //如果从服务端收到 "empty" 消息 isInitiator = true; console.log('Room ' + room + ' is empty'); }); socket.on('join', (room) => { //如果从服务端收到 “join" 消息 console.log('Making request to join room ' + room); console.log('You are the initiator!'); }); socket.on('log', (array) => { console.log.apply(console, array); });
在该代码中：
- 首先弹出一个输入框，要求用户写入要加入的房间。
- 然后，通过 io.connect() 建立与服务端的连接，
- 根据socket返回的消息做不同的处理：
- - 当收到房间满"full"时的情况;
- 当收到房间空“empty"时的情况；
- 当收到加入“join"时的情况；
以上是客户端（也就是在浏览器）中执行的代码。下面我们来看一下服务端的处理逻辑：
服务器端代码，server.js：
const static = require('node-static'); const http = require('http'); const file = new(static.Server)(); const app = http.createServer(function (req, res) { file.serve(req, res); }).listen(2013); const io = require('socket.io').listen(app); //侦听 2013 io.sockets.on('connection', (socket) => { // convenience function to log server messages to the client function log(){ const array = ['>>> Message from server: ']; for (var i = 0; i < arguments.length; i++) { array.push(arguments[i]); } socket.emit('log', array); } socket.on('message', (message) => { //收到message时，进行广播 log('Got message:', message); // for a real app, would be room only (not broadcast) socket.broadcast.emit('message', message); //在真实的应用中，应该只在房间内广播 }); socket.on('create or join', (room) => { //收到 “create or join” 消息 var clientsInRoom = io.sockets.adapter.rooms[room]; var numClients = clientsInRoom ? Object.keys(clientsInRoom.sockets).length : 0; //房间里的人数 log('Room ' + room + ' has ' + numClients + ' client(s)'); log('Request to create or join room ' + room); if (numClients === 0){ //如果房间里没人 socket.join(room); socket.emit('created', room); //发送 "created" 消息 } else if (numClients === 1) { //如果房间里有一个人 io.sockets.in(room).emit('join', room); socket.join(room); socket.emit('joined', room); //发送 “joined”消息 } else { // max two clients socket.emit('full', room); //发送 "full" 消息 } socket.emit('emit(): client ' + socket.id + ' joined room ' + room); socket.broadcast.emit('broadcast(): client ' + socket.id + ' joined room ' + room); }); });
在服务端引入了 node-static 库，使服务器具有发布静态文件的功能。服务器具有此功能后，当客户端（浏览器）向服务端发起请求时，服务器通过该模块获得客户端（浏览器）运行的代码，也就是上我面我们讲到的 index.html 和 client.js 并下发给客户端（浏览器）。
服务端侦听 2013 这个端口，对不同的消息做相应的处理：
- 服务器收到 message 消息时，它会直接进行广播，所有连接到该服务器的客户端都会收收广播的消息。
- 服务端收到 “create or join”消息时，它会对房间里有人数进行统计，如果房间里没有人，则发送"created" 消息；如果房间里有一个人，发送"join"消息和“joined"消息；如果超过两个人，发送"full"消息。
要运行该程序，需要使用 NPM 安装 [socket.io 6](http://socket.io/) 和 [node-static 1](https://github.com/cloudhead/node-static)，安装方法如下：
进入到 server.js 所在的目录，然后执行下面的命令。
npm install socket.io npm install node-static
启动服务器并测试
通过上面的步骤我们就使用 [socket.io 6](http://socket.io/) 构建好一个服务器，现在可以通过下面的命令将服务启动起来了：
node server.js
如果你是在本机上搭建的服务，则可以在浏览器中输入 localhost:2013 ，然后新建一个tab 在里边再次输入localhost:2013 。此时，打开控制台看看发生了什么?
小结
以上我向大家介绍了 Nodejs 的工作原理、Nodejs的安装与布署，以及如何使用 [要sokcet.io 1](http://xn--sokcet-vo4r.io/) 构建 WebRTC 信令消息服务器。[socket.io 6](http://socket.io/) 由于有房间的概念所以与WebRTC非常匹配，用它开发WebRTC信令服务器非常方便。
IM和视频聊天的，可以参考下这个 https://github.com/starrtc/starrtc-android-demo
[https://rtcdeveloper.com/t/topic/13341](https://rtcdeveloper.com/t/topic/13341)
