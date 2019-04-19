# NodeJs的核心模块 - LC900730的博客 - CSDN博客
2017年09月11日 13:47:52[lc900730](https://me.csdn.net/LC900730)阅读数：220
## 全局对象
Node.js中全局对象是global，所有全局变量(除了global)都是global对象的属性。 
如console、process。
### 全局变量
ECMAScript定义，满足以下条件的变量是全局变量。 
1.在最外层定义的变量 
2.全局对象的属性 
3.隐式定义的变量(未定义直接赋值的变量) 
在node.js中不可能在最外层定义变量。 
永远使用var定义变量，以免引入全局变量。
### process
global对象的属性。用于描述当前Node.js进程状态对象，提供了一个与操作系统的简单接口。 
### Http模块
http.Server是一个基于事件的HTTP服务器，所有的请求都被封装为独立的事件，继承自EventEmitter，提供了以下事件 
- request
- connection:粒度大于request，当TCP连接建立时，该事件被触发
- close
最常用的就是request了。因此http提供了一个捷径。 
http.createServer,功能是创建一个HTTP服务器并且将requestListener作为request事件的监听函数。
```java
var http=require("http");
var server=new http.Server();
server.on('request',function(req,res){
    res.writeHead(200,{'Content-Type':'text/html'});
    res.write({'<h1>Node.js</h1>'});
    res.end('<p>Hello world</p>'});
});
server.listen(3000);
```
## http.ServerReqest
http.ServerRequest是HTTP请求的信息，一般是http.Server的request事件发送，作为第一个参数传递。 
http.ServerReqest提供了3个事件用于控制请求体传输。
```
data：当请求数据到来时，该事件被触发。该事件提供一个参数chunk,表示接收到的数据。如果该事件没有被监听，那么请求体会被抛弃。该事件可能会被调用多次。
end：当请求体数据传输完成，该事件被触发。此后不会有数据到来。
close：用户当前请求结束，该事件被触发。不同于end，如果用户强制终止了传输，也还是调用close。
```
### 获取get请求内容
```java
var http=require("http");
var url=require('url');
var util=require('util');
http.createServer(function(req,res){
    res.writeHead(200,{'Content-Type':'text/html'});
    res.end(util.inspect(url.parse(req.url,true)));
}).listen(3000);
```
浏览器返回
```bash
{
search:'?name=byvoid&email=byvoid@byvoid.com',
query:{name:"byvoid",email:'byvoid@byvoid.com'}.
pathname:"/user",
path:'/user?name=byvoid&email=byvoid@byvoid.com',
href:'/user?name=byvoid&email=byvoid@byvoid.com',
}
```
通过url.parse原始的path会被解析为一个对象，其中query就是我们所谓GET请求的内容，而路径是pathname
### 获取POST请求内容
```java
var http=require("http");
var querystring=require('querystring');
var util=require('util');
http.createServer(function(req,res){
    var post='';
    res.on('data',function(chunk){
        post+=chunk;
    });
    res.on('end',function(){
        post=querystring.parse(post);
        res.end(util.inspect(post));
    });
}).listen(3000);
```
### http.ServerResponse
http.ServerResponse是返回给客户端的信息，决定了用户最终能看到的结果。 
它也是由http.Server的request事件发送的，作为第二个参数传递。
