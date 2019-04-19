# 基于netty-socketio的web推送服务 - 零度的博客专栏 - CSDN博客
2016年11月03日 15:16:57[零度anngle](https://me.csdn.net/zmx729618)阅读数：629
                
实时消息的推送，PC端的推送技术可以使用socket建立一个长连接来实现。传统的web服务都是客户端发出请求，服务端给出响应。但是现在直观的要求是允许特定时间内在没有客户端发起请求的情况下服务端主动推送消息到客户端。
有哪些可以实现web消息推送的技术：
- 
不断地轮询（俗称“拉”，polling）是获取实时消息的一个手段：Ajax 隔一段时间（通常使用 JavaScript 的 setTimeout 函数）就去服务器查询是否有改变，从而进行增量式的更新。但是间隔多长时间去查询成了问题，因为性能和即时性造成了严重的反比关系。间隔太短，连续不断的请求会冲垮服务器，间隔太长，务器上的新数据就需要越多的时间才能到达客户机。
- 
优点：服务端逻辑简单；
- 
缺点：其中大多数请求可能是无效请求，在大量用户轮询很频繁的情况下对服务器的压力很大；
- 
应用：并发用户量少，而且要求消息的实时性不高，一般很少采用；
- 
长轮询技术（long-polling）：客户端向服务器发送Ajax请求，服务器接到请求后hold住连接，直到有新消息或超时（设置）才返回响应信息并关闭连接，客户端处理完响应信息后再向服务器发送新的请求。
- 
优点：实时性高，无消息的情况下不会进行频繁的请求；
- 
缺点：服务器维持着连接期间会消耗资源；
- 
基于Iframe及htmlfile的流（streaming）方式：iframe流方式是在页面中插入一个隐藏的iframe，利用其src属性在服务器和客户端之间创建一条长链接，服务器向iframe传输数据（通常是HTML，内有负责插入信息的javascript），来实时更新页面。
- 
优点：消息能够实时到达；
- 
缺点：服务器维持着长连接期会消耗资源；
- 
插件提供socket方式：比如利用Flash XMLSocket，Java Applet套接口，Activex包装的socket。
- 
优点：原生socket的支持，和PC端和移动端的实现方式相似；
- 
缺点：浏览器端需要装相应的插件；
- 
WebSocket：是HTML5开始提供的一种浏览器与服务器间进行全双工通讯的网络技术。
- 
优点：更好的节省服务器资源和带宽并达到实时通讯；
- 
缺点：目前还未普及，浏览器支持不好；
综上，考虑到浏览器兼容性和性能问题，采用长轮询（long-polling）是一种比较好的方式。
netty-socketio是一个开源的Socket.io服务器端的一个java的实现， 它基于Netty框架。 项目地址为： [https://github.com/mrniko/netty-socketio](https://github.com/mrniko/netty-socketio)
以下是一个来自netty-socketio的推送示例，web聊天系统。
服务器端push server：
消息实体：
**[java]**[view plain](http://blog.csdn.net/z69183787/article/details/52505092#)[copy](http://blog.csdn.net/z69183787/article/details/52505092#)[print](http://blog.csdn.net/z69183787/article/details/52505092#)[?](http://blog.csdn.net/z69183787/article/details/52505092#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1877445/fork)
- package com.nettysocketio.test;  
- 
- publicclass ChatObject {  
- private String userName;  
- private String message;  
- 
- public ChatObject() {  
-     }  
- 
- public ChatObject(String userName, String message) {  
- super();  
- this.userName = userName;  
- this.message = message;  
-     }  
- 
- public String getUserName() {  
- return userName;  
-     }  
- 
- publicvoid setUserName(String userName) {  
- this.userName = userName;  
-     }  
- 
- public String getMessage() {  
- return message;  
-     }  
- 
- publicvoid setMessage(String message) {  
- this.message = message;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.nettysocketio.test;
public class ChatObject {
    private String userName;
    private String message;
    public ChatObject() {
    }
    public ChatObject(String userName, String message) {
        super();
        this.userName = userName;
        this.message = message;
    }
    public String getUserName() {
        return userName;
    }
    public void setUserName(String userName) {
        this.userName = userName;
    }
    public String getMessage() {
        return message;
    }
    public void setMessage(String message) {
        this.message = message;
    }
}
```
监听事件：
```
package com.nettysocketio.test;
import com.corundumstudio.socketio.AckRequest;
import com.corundumstudio.socketio.SocketIOClient;
import com.corundumstudio.socketio.SocketIOServer;
import com.corundumstudio.socketio.listener.DataListener;
public class CharteventListener implements DataListener<ChatObject> {
    SocketIOServer server;
    public void setServer(SocketIOServer server) {
        this.server = server;
    }
    public void onData(SocketIOClient client, ChatObject data,
            AckRequest ackSender) throws Exception {
        // chatevent为 事件的名称， data为发送的内容
        this.server.getBroadcastOperations().sendEvent("chatevent", data);
    }
}
```
推送服务：
```
package com.nettysocketio.test;
import com.corundumstudio.socketio.Configuration;
import com.corundumstudio.socketio.SocketIOServer;
public class App {
    public static void main(String[] args) throws InterruptedException
    {
        Configuration config = new Configuration();
        config.setHostname("localhost");
        config.setPort(9092);
        SocketIOServer server = new SocketIOServer(config);
        CharteventListener listner = new CharteventListener();
        listner. setServer(server);
        // chatevent为事件名称
        server.addEventListener("chatevent", ChatObject.class, listner);
        //启动服务
        server.start();
        Thread.sleep(Integer.MAX_VALUE) ;
        server.stop();
    }
}
```
浏览器端Client：
客户端使用的是socket.io-client，项目地址为：[https://github.com/Automattic/socket.io-client](https://github.com/Automattic/socket.io-client)
客户端聊天代码：
```
<!DOCTYPE html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Socketio chat</title>
<script src="./jquery-1.7.2.min.js" type="text/javascript"></script>
<script type="text/javascript" src="./socket.io/socket.io.js"></script>
<style>
body {
    padding: 20px;
}
#console {
    height: 400px;
    overflow: auto;
}
.username-msg {
    color: orange;
}
.connect-msg {
    color: green;
}
.disconnect-msg {
    color: red;
}
.send-msg {
    color: #888
}
</style>
</head>
<body>
    <h1>Netty-socketio chat demo</h1>
    <br />
    <div id="console" class="well"></div>
    <form class="well form-inline" onsubmit="return false;">
        <input id="name" class="input-xlarge" type="text" placeholder="用户名称. . . " />
        <input id="msg" class="input-xlarge" type="text" placeholder="发送内容. . . " />
        <button type="button" onClick="sendMessage()" class="btn">Send</button>
        <button type="button" onClick="sendDisconnect()" class="btn">Disconnect</button>
    </form>
</body>
<script type="text/javascript">
    var socket = io.connect('http://localhost:9092');
    socket.on('connect',function() {
        output('<span class="connect-msg">Client has connected to the server!</span>');
    });
    
    socket.on('chatevent', function(data) {
        output('<span class="username-msg">' + data.userName + ' : </span>'
                + data.message);
    });
    
    socket.on('disconnect',function() {
        output('<span class="disconnect-msg">The client has disconnected! </span>');
    });
    
    function sendDisconnect() {
        socket.disconnect();
    }
    
    function sendMessage() {
        var userName = $("#name").val()
        var message = $('#msg').val();
        $('#msg').val('');
        socket.emit('chatevent', {
            userName : userName,
            message : message
        });
    }
    
    function output(message) {
        var currentTime = "<span class='time' >" + new Date() + "</span>";
        var element = $("<div>" + currentTime + " " + message + "</div>");
        $('#console').prepend(element);
    }
</script>
</html>
```
先运行push server，再打开chat html就可以看到连接信息和服务器推送的聊天信息。
