# Node.Js - LC900730的博客 - CSDN博客
2017年08月25日 12:41:37[lc900730](https://me.csdn.net/LC900730)阅读数：162标签：[node.js](https://so.csdn.net/so/search/s.do?q=node.js&t=blog)
个人分类：[nodejs](https://blog.csdn.net/LC900730/article/category/7127126)
## 第一章
```java
var http=require('http');
http.createServer(function(req,res){
    res.writeHead(200,{'Content-Type':'text/plain'});
    res.end('Hello World\n');
}).listen(8124,'127.0.0.1');
console.log('Server running at http://127.0.0.1:8124/');
```
1.require方法把HTTP库包含到程序中，HTTP库所有的功能已经赋给了http对象。 
2.调用HTTP模块的一个工厂模式方法(createServer)来创建新的HTTP服务器。新创建的HTTP服务器没有赋值给任何变量，它只会成为存活在全局范围内的匿名对象。我们可以通过链式调用来初始化服务器，并且告诉它监听8124端口。 
3.当调用createServer时候，我们传递一个匿名函数作为参数。此函数绑定在新创建服务器的事件监听器上进行request事件处理。消息事件是javascript和node核心。在这个例子中，每当一个新请求到达web服务器，它都调用我们指定的函数方法来处理。称为回调，每当一个事件发生时候，我们回调监听此事件的所有函数。 
4.end方法会关闭HTTP连接。(将内容发送给客户端完毕后才关闭连接)
## 第二章
### 创建一个新的聊天服务器 
```java
//引入net模块,包含Node需要的所有TCP功能
var net=require('net');
//创建新的TCP服务器
var chatServer=net.createServer()
//调用on()添加事件监听器
//每当新的客户端通过网络连接到服务器，会触发connection事件，事件监听器会调用我们指定的函数。
//连接事件在调用回调函数时候，会传入我们新客户端所对应的TCP Socket对象的引用，命名为client。
//调用client.write()就能发送信息给该客户端。
chatServer.on('connection',function(client){
    client.write('Hi!\n');
    client.write('Bye!\n');
    client.end()
})
//调用listen()函数，指定监听端口
chatServer.listen(9000);
```
### 监听所有连接请求
```java
var net=require('net');
var chatServer=net.createServer()
chateServer.on('connection',function(client){
    client.write('Hi!\n');
//添加另外一个事件监听器，调用的是client.on().
//我们是在connection回调函数作用域中添加的这个事件监听器，这样就可以访问到连接事件对应的client对象。
    client.on('data',function(data){
        console.log(data)
    })
})
chatServer.listen(9000)
```
JavaScript无法很好处理二进制数据，所以node增加一个buffer库来帮助服务器，Node并不知道客户端发送的是什么类型数据，因此在告诉其用什么编码之前，Node只会保存原始二进制数据。
监听所有客户端的连接
```java
var net=require('net');
var chatServer=net.createServer()
    clientList=[]
    chatServer.on('connection',function(client)){
        client.write('Hi!\n');
    clientList.push(client);
    client.on('data',function(data){
        for(var i=0;i<clientList.length;i+=1){
            //发送数据给所以客户端
            clientList[i].wtrite(data)
        }
    })
    })
    chatServer.listen(9000)
```
### 上述问题
console1运行nodejs，console 2，console3运行2个telnet 
在console2的Telnet发送消息后，消息转发到了console3的Telnet客户端上，同时也发送到了console2的Telnet客户端。因为我们在发送消息的时候，没有检查发送者是谁，只是简单把消息转发给所有客户端。而且Telnet无法区分哪些消息是自己发送的，哪些是别人发送的。
```java
var net=require('net');
var chatServer=net.createServer()
    clientList=[]
    chatServer.on('connection',function(client)){
        client.name=client.remoteAddress+':'+client.remotePort
client.write('Hi!'+client.name+'\n');
    clientList.push(client);
    client.on('data',function(data){
        broadcast(data,client);
    })
})
function brordcast(message,client){
        for(var i=0;i<clientList.length;i+=1){
            //发送数据给所以客户端
            if(client!==clientList[i]){
                clientList[i].wtrite(client.name+'says'+message)
        }
    }
}
    chatServer.listen(9000)
```
在connection事件监听器上为每个client对象增加name属性。 
为什么能为client对象增加属性呢？因为闭包绑定了每个client对象和相应的请求，于是在闭包内就可以利用client.remoteAddress和client.remotePort来创建client的name属性
### 改进
当某一个客户端如console2断开连接，那么在发送消息时候，会往一个已经断开连接的客户端写入数据。当console2端开时候，它对应的socket就无法写入数据了。而对已经关闭的socket进行write()操作会引发异常。
```
首先如果客户端断开连接，那么从客户端列表移除，防止其在调用write()方法。V8引擎会相应的socket对象作为垃圾回收，并且释放相应的内存。
```
```
//一个socket端开连接的时候，会触发end事件。
client.on('end',function(){
    clientList.splice(clientList.indexOf(client),1)
})
```
```
其次，要采用更保险的方式调用write()方法。我们要确保socket从上次被写入到现在，没有任何阻碍我们调用write()方法的事情。
```
```java
//更加保险的方式
function broadcast(message,client){
    var cleanup=[];
    for(var i=0;i<clientList.length;i+=1){
        if(client!==clientList[i]){
    //调用之前检查是否可写
            if(clientList[i].writable){
                clientList[i].write(client.name+"says"+message)
                }else{
                    cleanup.push(clientList[i])
                    clientList[i].destroy()
                }
            }
    }
//在写入循环中 删除死节点，消除垃圾索引
for(i=0;i<cleanup.length;i+=1){
    clientList.splice(clientList.indexOf(cleanup[i],1))
}
}
```
### 记录日志
```matlab
client.on('error',function(e)){
    console.log(e)
}
```
## 第三章
### 事件循环
Node一个核心功能就是事件循环，这一概念也用于Js底层行为和许多交互交互系统中。
## Express
Express是把http的基础功能封装起来，并且提供众多的功能来丰富它，使得创建真正的应用非常便捷。不需要在每次处理HTTP请求的时候自己编写代码来处理逻辑，Express都准备好了。
### 路由
```java
var express=require('express');
var app=express.createServer();
app.get('/:id?',function(req,res){
    if(req.params.id){
        res.send(req.params.id);
    }else{
        res.send('oh hai');
    }
});
app.listen(9001);
```
在Express中我们使用冒号：来标记想要使用的变量，那么在URL中传递的字符串就会被捕获并保持在该变量中。Express中所有路由最终会变成正则表达式来处理。
### 中间件
Connect库提供了Express使用的中间件功能。Connect扩展了Node的基础http模块，为他赋予了http能提供的所有基础服务，然后在此基础上又增加了自己的功能。Express是从Connect继承下来的，同时获得了http和Connect的功能。任何添加到Connect的模块都会自动被Express所使用。Connect是链接Express和网络的中间层。 
JavaScript的闭包让我们能够在Node内部实现工厂模式，并且为网站路由提供上下文处理功能。
Express路由功能会在处理环节使用内部的中间件，可以通过重载来添加额外的功能。
### 事件驱动
通常是主循环监听事件，然后触发回调函数时被检测到这些事件之一。
每当一个事件被触发时，它的监听函数就开始执行。
Node.js具有多个内置通过事件模块和用于将事件绑定和事件监听，如EventEmittter类可用事件：
```
//import events module
var event=require('events');
//Create an eventEmitter object
var eventEmitter=new events.EventEmitter();
//事件处理程序绑定事件的语法
//Bind event and even handler as follows
eventEmitter.on('eventName',eventHandler);
我们可以通过编程触发一个事件
eventEmitter.emit('eventName');
```
## Node的应用程序如何工作
在Node应用程序中，任何异步函数接受回调作为最后的参数和回调函数接受错误的第一个参数。
```
var fs=require('fs');
    fs.readFile('input.txt',function(err,data){
        if(err){
            console.log(err,stack);
            retutn;
        }
        console.log(data.toString());
    });
    console.log('Program Ended');
```
### 回调和异步调用
回调并非是异步调用，回调是一种解决异步函数执行结果的处理方法。在异步调用，如果我们希望将执行的结果返回并且处理时候，可以通过回调的方法解决。 
回调其实还是一种堵塞式调用。
异步函数往往不是直接返回执行结果，而是通过事件驱动方式，将执行结果返回到回调函数中，之后在回调函数中处理相应的逻辑代码。
#### 同步函数与异步函数
//异步函数
```java
var dns=require('dns');
    var address=dns.resolve4('www.qq.com',function(address){});
    console.log(address);
//打印结果为null
//原因是异步函数dns.resolve4()还未执行结束时，就已经执行到consoloe.log(address),因此最终结果为null
```
回调函数
```java
var dns=require('dns');
    var address=dns.resolve4('www.qq.com',function(address){
        console.log(address);
});
//主要是通过回调函数获取执行结果
```
