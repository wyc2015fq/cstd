# 基于Java的WebSocket推送 - z69183787的专栏 - CSDN博客
2016年09月11日 19:11:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4925
### WebSocket的主动推送
关于消息推送，现在的解决方案如轮询、长连接或者短连接，当然还有其他的一些技术框架，有的是客户端直接去服务端拿数据。
其实推送推送主要讲的是一个推的概念，WebSocket是一种主动推送消息的技术。
### 基于Java的WebSocket
这里主要是结合网上的例子实现下。
#### 简单的客户端JavaScript
```
```xml
<!DOCTYPE html>
<html>
<head>
<title>Testing websockets</title>
</head>
<body>
  <div>
    <input type="submit" value="Start" onclick="start()" />
  </div>
  <div id="messages"></div>
  <script type="text/javascript">
    var webSocket =
      new WebSocket('ws://localhost:8080/wesocket/websocketTest');
    webSocket.onerror = function(event) {
      onError(event)
    };
 
    webSocket.onopen = function(event) {
      onOpen(event)
    };
 
    webSocket.onmessage = function(event) {
      onMessage(event)
    };
 
    function onMessage(event) {
      document.getElementById('messages').innerHTML
        += '<br />' + event.data;
    }
 
    function onOpen(event) {
      document.getElementById('messages').innerHTML
        = 'Connection established';
    }
 
    function onError(event) {
      alert(event.data);
    }
 
    function start() {
      webSocket.send('hello');
      return false;
    }
  </script>
</body>
</html>
```
```
#### 服务端
```java
```java
package com.tony.websocket;
import javax.websocket.OnClose;
import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;
import java.io.IOException;
@ServerEndpoint("/websocketTest")
public class WebSocketTest {
    @OnMessage
    public void onMessage(String message, Session session) throws IOException, InterruptedException {
        // Print the client message for testing purposes
        System.out.println("Received: " + message);
        // Send the first message to the client
        session.getBasicRemote().sendText("This is the first server message");
        // Send 3 messages to the client every 5 seconds
        int sentMessages = 0;
        while (sentMessages < 3) {
            Thread.sleep(5000);
            session.getBasicRemote().sendText("This is an intermediate server message. Count: " + sentMessages);
            sentMessages++;
        }
        // Send a final message to the client
        session.getBasicRemote().sendText("This is the last server message");
    }
    @OnOpen
    public void onOpen() {
        System.out.println("Client connected");
    }
    @OnClose
    public void onClose() {
        System.out.println("Connection closed");
    }
}
```
```
#### pom.xml 文件
```
```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.tony</groupId>
    <artifactId>wesocket</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <dependencies>
        <dependency>
            <groupId>javax.websocket</groupId>
            <artifactId>javax.websocket-api</artifactId>
            <version>1.0-rc5</version>
            <scope>provided</scope>
        </dependency>
    </dependencies>
</project>
```
```
我这里用的是JDK是 `jdk1.8.0_60` ，Tomcat用的是 `apache-tomcat-7.0.64` 。
**1、建立连接**
![](http://images2015.cnblogs.com/blog/801359/201605/801359-20160515161026055-1290556618.png)
**2、发送消息**
![](http://images2015.cnblogs.com/blog/801359/201605/801359-20160515161034133-714524427.png)
**3、服务端推送消息**
![](http://images2015.cnblogs.com/blog/801359/201605/801359-20160515161244352-2070636340.png)
