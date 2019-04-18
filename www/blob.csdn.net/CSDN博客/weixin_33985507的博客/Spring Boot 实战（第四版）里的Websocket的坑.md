# Spring Boot 实战（第四版）里的Websocket的坑 - weixin_33985507的博客 - CSDN博客
2018年08月26日 03:10:28[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
# 书中的路径有问题
导致我浪费了一个小时的时间，现在整理出来
## 首先，如果你遇到了下面的问题，那么可以看看这篇文章
- 
404
- 
200
## 首先看要注意的点：
- 
### 书里的配置类没有加@Configuration注解
- 
### 映射配置类的路径并不是会自动加websocket前缀，所以两个映射不能为了方便就配成相同的
- 
### JS中的路径在书中配置有误，并没有websocket前缀（当然，自己想加也是可以加的，但要和**WebSocketConfig类里配置的路径保持一致性**）
## 这时候一个Websocket的坑就走完了
## 然后书上之后开始介绍sockjs,对应的CDN也更新了，如果用老版本的话，浏览器控制台会提醒我们可能会出问题
我们换成github上面声明的路径，目前是这个
# 最后，我们一起来看一下整个用例项目
- pom.xml中的依赖
```xml
<dependencies>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-freemarker</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-web</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-websocket</artifactId>
        </dependency>
        <dependency>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-test</artifactId>
        </dependency>
    </dependencies>
复制代码
```
- MarcoHandler
```java
//package com.fufu.socket2;
import org.springframework.stereotype.Component;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.AbstractWebSocketHandler;
@Component
public class MarcoHandler extends AbstractWebSocketHandler{
    @Override
    protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        System.out.println(message.toString());
        Thread.sleep(200);
        if(message.getPayload().indexOf("sockjs")>=0){
            session.sendMessage((new TextMessage("sockjs Polo!")));
        }else{
            session.sendMessage((new TextMessage("Polo!")));
        }
    }
    @Override
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {
        System.out.println("established");
    }
    @Override
    public void afterConnectionClosed(WebSocketSession session, CloseStatus status) throws Exception {
        System.out.println("closed");
    }
}
复制代码
```
- MyController
```java
//package com.fufu.socket2;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
@Controller
public class MyController {
    @RequestMapping("chat")
    public String  chat(){
        return "/chat";
    }
}
复制代码
```
- WebSocketConfig
```
//package com.fufu.socket2;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.socket.config.annotation.EnableWebSocket;
import org.springframework.web.socket.config.annotation.WebSocketConfigurer;
import org.springframework.web.socket.config.annotation.WebSocketHandlerRegistry;
@Configuration
@EnableWebSocket
public class WebSocketConfig implements WebSocketConfigurer {
    @Autowired
    MarcoHandler marcoHandler;  //利用Autowried注入，在该类头注解@Component即可
    @Override
    public void registerWebSocketHandlers(WebSocketHandlerRegistry webSocketHandlerRegistry) {
        webSocketHandlerRegistry.addHandler(marcoHandler,"/marco")/*.withSockJS()*/;
        webSocketHandlerRegistry.addHandler(marcoHandler,"/marco").withSockJS();
    }
}
复制代码
```
- chat.ftl
```
<!doctype html>
<html lang="cn">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, user-scalable=no, initial-scale=1.0, maximum-scale=1.0, minimum-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>Document</title>
</head>
<body>
<script src="https://cdn.jsdelivr.net/npm/sockjs-client@1/dist/sockjs.min.js"></script>
<script>
    var url="ws://"+window.location.host+"/marco";
    var sockjsUrl="/marco";
    var sock=new WebSocket(url);
    sock.onopen=function () {
        console.log("opening")
    }
    sock.onmessage=function (ev) {
        console.log(ev.data);
        setTimeout(function () {
            sock.send("Marco");
        },500)
    }
    sock.onclose=function () {
        console.log("closing")
    }
    var sockjs=new SockJS(sockjsUrl)
    sockjs.onopen=function () {
        console.log("sockjs opening")
    }
    sockjs.onmessage=function (ev) {
        console.log(ev.data);
        setTimeout(function () {
            sock.send("sockjs Marco");
        },500)
    }
    sockjs.onclose=function () {
        console.log("sockjs closing")
    }
    setTimeout(function () {
        sock.send("Marco");
        sockjs.send("sockjs Marco")
    },1500)
</script>
</body>
</html>
复制代码
```
## 整个项目跑起来的效果是
### 进入localhost:8080/chat页面时，在浏览器控制台里会出现
### java控制台里出现
# END
