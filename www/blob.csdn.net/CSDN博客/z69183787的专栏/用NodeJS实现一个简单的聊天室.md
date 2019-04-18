# 用NodeJS实现一个简单的聊天室 - z69183787的专栏 - CSDN博客
2016年09月11日 19:18:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2051
今天我来实现一个简单的聊天室，后台用`nodejs`, 客户端与服务端通信用`socket.io`,这是一个比较成熟的`websocket`框架.
### 初始工作
- 
安装`express`, 用这个来托管`socket.io`,以及静态页面,命令
```
npm
 install express --save
```
,`--save`可以使包添加到`package.json`文件里.
- 
安装`socket.io`,命令
```
npm
 install socket.io --save
```
.
### 编写服务端代码
首先我们通过express来托管网站，并附加到`socket.io`实例里，因为`socket.io`初次连接需要`http`协议
```
var express = require('express'),
    io = require('socket.io');
var app = express();
app.use(express.static(__dirname));
var server = app.listen(8888);
var ws = io.listen(server);
```
添加服务器连接事件，当客户端连接成功之后，发公告告诉所有在线用户，并且，当用户发送消息时，发广播通知其它用户.
```
ws.on('connection', function(client){
    console.log('\033[96msomeone is connect\033[39m \n');
    client.on('join', function(msg){
        // 检查是否有重复
        if(checkNickname(msg)){
            client.emit('nickname', '昵称有重复!');
        }else{
            client.nickname = msg;
            ws.sockets.emit('announcement', '系统', msg + ' 加入了聊天室!');
        }
    });
    // 监听发送消息
    client.on('send.message', function(msg){
        client.broadcast.emit('send.message',client.nickname,  msg);
    });
    // 断开连接时，通知其它用户
    client.on('disconnect', function(){
        if(client.nickname){
            client.broadcast.emit('send.message','系统',  client.nickname + '离开聊天室!');
        }
    })
})
```
由于客户端是通过昵称来标识的，所以服务端需要一个检测昵称重复的函数
```
// 检查昵称是否重复
var checkNickname = function(name){
    for(var k in ws.sockets.sockets){
        if(ws.sockets.sockets.hasOwnProperty(k)){
            if(ws.sockets.sockets[k] && ws.sockets.sockets[k].nickname == name){
                return true;
            }
        }
    }
    return false;
}
```
### 编写客服端代码
由于服务端采用第三方`websokcet`框架，所以前端页面需要单独引用`socket.io`客户端代码，源文件可以从`socket.io`模块里找，windows下路径为`node_modules\socket.io\node_modules\socket.io-client\dist`,这里有开发版和压缩版的，默认引用开发版就行.
前端主要处理输入昵称检查，消息处理，完整代码如下
```
```
<!DOCTYPE html>
<html>
<head>
    <title>socket.io 聊天室例子</title>
    <meta charset="utf-8">
    <link rel="stylesheet" href="css/reset.css"/>
    <link rel="stylesheet" href="css/bootstrap.css"/>
    <link rel="stylesheet" href="css/app.css"/>
</head>
<body>
    <div class="wrapper">
         <div class="content" id="chat">
             <ul id="chat_conatiner">
             </ul>
         </div>
         <div class="action">
             <textarea ></textarea>
             <button class="btn btn-success" id="clear">清屏</button>
             <button class="btn btn-success" id="send">发送</button>
         </div>
    </div>
    <script type="text/javascript" src="js/socket.io.js"></script>
    <script type="text/javascript">
          var ws = io.connect('http://172.16.2.184:8888');
          var sendMsg = function(msg){
              ws.emit('send.message', msg);
          }
          var addMessage = function(from, msg){
              var li = document.createElement('li');
              li.innerHTML = '<span>' + from + '</span>' + ' : ' + msg;
              document.querySelector('#chat_conatiner').appendChild(li);
              // 设置内容区的滚动条到底部
              document.querySelector('#chat').scrollTop = document.querySelector('#chat').scrollHeight;
              // 并设置焦点
              document.querySelector('textarea').focus();
          }
          var send = function(){
              var ele_msg = document.querySelector('textarea');
              var msg = ele_msg.value.replace('\r\n', '').trim();
              console.log(msg);
              if(!msg) return;
              sendMsg(msg);
              // 添加消息到自己的内容区
              addMessage('你', msg);
              ele_msg.value = '';
          }
          ws.on('connect', function(){
              var nickname = window.prompt('输入你的昵称!');
              while(!nickname){
                  nickname = window.prompt('昵称不能为空，请重新输入!')
              }
              ws.emit('join', nickname);
          });
          // 昵称有重复
          ws.on('nickname', function(){
              var nickname = window.prompt('昵称有重复，请重新输入!');
              while(!nickname){
                  nickname = window.prompt('昵称不能为空，请重新输入!')
              }
              ws.emit('join', nickname);
          });
          ws.on('send.message', function(from, msg){
              addMessage(from, msg);
          });
          ws.on('announcement', function(from, msg){
              addMessage(from, msg);
          });
          document.querySelector('textarea').addEventListener('keypress', function(event){
              if(event.which == 13){
                  send();
              }
          });
          document.querySelector('textarea').addEventListener('keydown', function(event){
              if(event.which == 13){
                  send();
              }
          });
          document.querySelector('#send').addEventListener('click', function(){
              send();
          });
          document.querySelector('#clear').addEventListener('click', function(){
              document.querySelector('#chat_conatiner').innerHTML = '';
          });
    </script>
</body>
</html>
```
```
这里提供[完整的代码压缩文件](http://pan.baidu.com/s/1eQkTbqy)
### 总结
nodejs是一个好东西，尤其是在处理消息通讯，网络编程方面，天生的异步IO.
