# 使用Spring WebSocket实现消息推送 - 零度的博客专栏 - CSDN博客
2017年11月20日 17:43:50[零度anngle](https://me.csdn.net/zmx729618)阅读数：6195
### 一、Spring WebSocket配置与实现
第一步： 添加Spring WebSocket的依赖jar包
（注：这里使用maven方式添加 手动添加的同学请自行下载相应jar包放到lib目录，本文使用的版本为4.3.5.RELEASE）
```
<!-- spring websocket-->
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-messaging</artifactId>
        <version>${spring.version}</version>
    </dependency>
    <dependency>
        <groupId>org.springframework</groupId>
        <artifactId>spring-websocket</artifactId>
        <version>${spring.version}</version>
    </dependency>
```
第二步：建立一个类实现WebSocketConfigurer接口
（用于配置spring webSocket）
```
```
package org.zwc.ssm.websocket;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.servlet.config.annotation.EnableWebMvc;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;
import org.springframework.web.socket.config.annotation.EnableWebSocket;
import org.springframework.web.socket.config.annotation.WebSocketConfigurer;
import org.springframework.web.socket.config.annotation.WebSocketHandlerRegistry;
import org.springframework.web.socket.handler.TextWebSocketHandler;
/**
 * Created by zhangwenchao on 2017/11/20.
 */
@Configuration
@EnableWebMvc
@EnableWebSocket
public class SpringWebSocketConfig extends WebMvcConfigurerAdapter implements WebSocketConfigurer {
    
    public void registerWebSocketHandlers(WebSocketHandlerRegistry registry) {
        registry.addHandler(webSocketHandler(),"/websocket/socketServer.do")
                .addInterceptors(new SpringWebSocketHandlerInterceptor());
        
        registry.addHandler(webSocketHandler(), "/sockjs/socketServer.do")
                .addInterceptors(new SpringWebSocketHandlerInterceptor()).withSockJS();
    }
    @Bean
    public TextWebSocketHandler webSocketHandler(){
        return new SpringWebSocketHandler();
    }
}
```
```
第三步：继承WebSocketHandler对象
(住：该对象提供了客户端连接,关闭,错误,发送等方法,重写这几个方法即可实现自定义业务逻辑)
```java
package org.zwc.ssm.websocket;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.socket.CloseStatus;
import org.springframework.web.socket.TextMessage;
import org.springframework.web.socket.WebSocketSession;
import org.springframework.web.socket.handler.TextWebSocketHandler;
/**
 * Created by zhangwenchao on 2017/11/20.
 */
public class SpringWebSocketHandler extends TextWebSocketHandler {
    private static Logger logger = LoggerFactory.getLogger(SpringWebSocketHandler.class);
    private static final Map<String, WebSocketSession> users;  //Map来存储WebSocketSession，key用USER_ID 即在线用户列表
    //用户标识
    private static final String USER_ID = "WEBSOCKET_USERID";   //对应监听器从的key
    static {
        users =  new HashMap<String, WebSocketSession>();
    }
    public SpringWebSocketHandler() {}
    /**
     * 连接成功时候，会触发页面上onopen方法
     */
    public void afterConnectionEstablished(WebSocketSession session) throws Exception {
        System.out.println("成功建立websocket连接!");
        String userId = (String) session.getAttributes().get(USER_ID);
        users.put(userId,session);
        System.out.println("当前线上用户数量:"+users.size());
        //这块会实现自己业务，比如，当用户登录后，会把离线消息推送给用户
        //TextMessage returnMessage = new TextMessage("成功建立socket连接，你将收到的离线");
        //session.sendMessage(returnMessage);
    }
    /**
     * 关闭连接时触发
     */
    public void afterConnectionClosed(WebSocketSession session, CloseStatus closeStatus) throws Exception {
        logger.debug("关闭websocket连接");
        String userId= (String) session.getAttributes().get(USER_ID);
        System.out.println("用户"+userId+"已退出！");
        users.remove(userId);
        System.out.println("剩余在线用户"+users.size());
    }
    /**
     * js调用websocket.send时候，会调用该方法
     */
    @Override
    protected void handleTextMessage(WebSocketSession session, TextMessage message) throws Exception {
        super.handleTextMessage(session, message);
        /**
         * 收到消息，自定义处理机制，实现业务
         */
        System.out.println("服务器收到消息："+message);
        if(message.getPayload().startsWith("#anyone#")){ //单发某人
             sendMessageToUser((String)session.getAttributes().get(USER_ID), new TextMessage("服务器单发：" +message.getPayload())) ;
        }else if(message.getPayload().startsWith("#everyone#")){
             sendMessageToUsers(new TextMessage("服务器群发：" +message.getPayload()));
        }else{
        }
    }
    public void handleTransportError(WebSocketSession session, Throwable exception) throws Exception {
        if(session.isOpen()){
            session.close();
        }
        logger.debug("传输出现异常，关闭websocket连接... ");
        String userId= (String) session.getAttributes().get(USER_ID);
        users.remove(userId);
    }
    public boolean supportsPartialMessages() {
        return false;
    }
    /**
     * 给某个用户发送消息
     *
     * @param userId
     * @param message
     */
    public void sendMessageToUser(String userId, TextMessage message) {
        for (String id : users.keySet()) {
            if (id.equals(userId)) {
                try {
                    if (users.get(id).isOpen()) {
                        users.get(id).sendMessage(message);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }
                break;
            }
        }
    }
    /**
     * 给所有在线用户发送消息
     *
     * @param message
     */
    public void sendMessageToUsers(TextMessage message) {
        for (String userId : users.keySet()) {
            try {
                if (users.get(userId).isOpen()) {
                    users.get(userId).sendMessage(message);
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
```
第四步：继承HttpSessionHandshakeInterceptor对象
该对象作为页面连接websocket服务的拦截器
```
```java
package org.zwc.ssm.websocket;
import java.util.Map;
import javax.servlet.http.HttpSession;
import org.springframework.http.server.ServerHttpRequest;
import org.springframework.http.server.ServerHttpResponse;
import org.springframework.http.server.ServletServerHttpRequest;
import org.springframework.web.socket.WebSocketHandler;
import org.springframework.web.socket.server.support.HttpSessionHandshakeInterceptor;
/**
 * Created by zhangwenchao on 2017/11/20.
 * WebSocket拦截器----握手之前将登陆用户信息从session设置到WebSocketSession
 *
 */
public class SpringWebSocketHandlerInterceptor extends HttpSessionHandshakeInterceptor {
    @Override
    public boolean beforeHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler,
                                   Map<String, Object> attributes) throws Exception {
        System.out.println("Before Handshake");
        if (request instanceof ServletServerHttpRequest) {
            ServletServerHttpRequest servletRequest = (ServletServerHttpRequest) request;
            HttpSession session = servletRequest.getServletRequest().getSession(false);
            if (session != null) {
                //使用userName区分WebSocketHandler，以便定向发送消息
                String userName = (String) session.getAttribute("SESSION_USERNAME");  //一般直接保存user实体
                if (userName!=null) {
                    attributes.put("WEBSOCKET_USERID",userName);
                }
            }
        }
        return super.beforeHandshake(request, response, wsHandler, attributes);
    }
    @Override
    public void afterHandshake(ServerHttpRequest request, ServerHttpResponse response, WebSocketHandler wsHandler,
                               Exception ex) {
        super.afterHandshake(request, response, wsHandler, ex);
    }
}
```
```
第5步 让SpringWebSocketConfig配置类随spring容器启动
spring文件中加入如下代码：
```
<!-- websocket相关扫描，主要扫描：WebSocketConfig  如果前面配置能扫描到此类则可以不加 -->
<context:component-scan base-package="org.zwc.ssm" />
```
上述完成之后，spring-websocket配置和实现就已经完成，接下来实现进行测试。
### 二、测试过程
1.定义一个控制器用来做登录连接和发送消息
```
```java
package org.zwc.ssm.web;
import org.springframework.context.annotation.Bean;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.socket.TextMessage;
import org.zwc.ssm.websocket.SpringWebSocketHandler;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
/**
 * Created by zhangwenchao on 2017/11/20.
 */
@Controller
public class WebSocketController {
    @Bean//这个注解会从Spring容器拿出Bean
    public SpringWebSocketHandler infoHandler() {
        return new SpringWebSocketHandler();
    }
    @RequestMapping("/websocket/loginPage")
    public String loginPage(HttpServletRequest request, HttpServletResponse response) throws Exception {
        return "/websocket/login";
    }
    @RequestMapping("/websocket/login")
    public String login(HttpServletRequest request, HttpServletResponse response) throws Exception {
        String username = request.getParameter("username");
        System.out.println(username+"登录");
        HttpSession session = request.getSession(false);
        session.setAttribute("SESSION_USERNAME", username); //一般直接保存user实体
        return "/websocket/send";
    }
    @RequestMapping("/websocket/send")
    @ResponseBody
    public String send(HttpServletRequest request) {
        String username = request.getParameter("username");
        infoHandler().sendMessageToUser(username, new TextMessage("你好，测试！！！！"));
        return null;
    }
    @RequestMapping("/websocket/broad")
    @ResponseBody
    public  String broad() {
        infoHandler().sendMessageToUsers(new TextMessage("发送一条小Broad"));
        System.out.println("群发成功");
        return "broad";
    }
}
```
```
2.建立登录页面
```
```
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<c:set var="ctx" value="${pageContext.request.contextPath}"/>
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
    <title>测试spring websocket</title>
    <!--
	<link rel="stylesheet" href="/css/style.css"/>
	<script type="text/javascript" src="/js/jquery/jquery.js"></script>
    <script type="text/javascript">  
    </script>
    -->
</head>
<body>
<form action="${ctx}/websocket/login">
    登录名：<input type="text" name="username"/>
    <input type="submit" value="登录聊天室"/>
</form>
</body>
</html>
```
```
3.建立发消息页面
```
<%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ taglib uri="http://java.sun.com/jsp/jstl/fmt" prefix="fmt" %>
<c:set var="ctx" value="${pageContext.request.contextPath}"/>
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
    <title></title>
    <!--
	<link rel="stylesheet" href="/css/style.css"/>
    -->
    <script type="text/javascript" src="http://cdn.bootcss.com/jquery/3.1.0/jquery.min.js"></script>
    <script type="text/javascript" src="http://cdn.bootcss.com/sockjs-client/1.1.1/sockjs.js"></script>
    <script type="text/javascript">
        var websocket = null;
        if ('WebSocket' in window) {
            websocket = new WebSocket("ws://localhost:8080/JavaWebTest/websocket/socketServer.do");
        }
        else if ('MozWebSocket' in window) {
            websocket = new MozWebSocket("ws://localhost:8080/JavaWebTest/websocket/socketServer.do");
        }
        else {
            websocket = new SockJS("http://localhost:8080/JavaWebTest/sockjs/socketServer.do");
        }
        websocket.onopen = onOpen;
        websocket.onmessage = onMessage;
        websocket.onerror = onError;
        websocket.onclose = onClose;
        function onOpen(openEvt) {
            alert(openEvt.Data);
        }
        function onMessage(evt) {
            alert(evt.data);
        }
        function onError() {}
        function onClose() {}
        function doSendUser() {
            if (websocket.readyState == websocket.OPEN) {
                var msg = document.getElementById("inputMsg").value;
                websocket.send("#anyone#"+msg);//调用后台handleTextMessage方法
                alert("发送成功!");
            } else {
                alert("连接失败!");
            }
        }
        function doSendUsers() {
            if (websocket.readyState == websocket.OPEN) {
                var msg = document.getElementById("inputMsg").value;
                websocket.send("#everyone#"+msg);//调用后台handleTextMessage方法
                alert("发送成功!");
            } else {
                alert("连接失败!");
            }
        }
        window.close=function()
        {
            websocket.onclose();
        }
    </script>
</head>
<body>
请输入：<textarea rows="5" cols="10" id="inputMsg" name="inputMsg"></textarea>
<button onclick="doSendUser();">发送</button>
<button onclick="doSendUsers();">群发</button>
</body>
</html>
```
4、测试结果
如下图所示：前台登录两个用户aaaa和bbbb，aaaa发送“哈哈，大家好”，群发给所用用户，用户aaaa和bbbb同时收到消息。
![](https://img-blog.csdn.net/20171121105046258?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem14NzI5NjE4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
后台上线用户日志如图：
![](https://img-blog.csdn.net/20171121105923082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem14NzI5NjE4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

