# Spring WebSocket+SockJS+Stomp实现一对一、一对多通信 - 隔壁老王的专栏 - CSDN博客





2016年11月27日 14:30:14[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：35942








最近项目上要做扫码登录，所以研究了一下Spring WebSocket。网上找了很多资料[springmvc(18)使用WebSocket 和 STOMP 实现消息功能](http://blog.csdn.net/pacosonswjtu/article/details/51914567)、[spring
 websocket + stomp 实现广播通信和一对一通信](http://www.cnblogs.com/winkey4986/p/5622758.html)，要么就是不是自己想要的，要么就是只有中间一部分。所以特别写了这篇文章，一方面怕自己遗忘，另一方面是希望可以给大家一些参考。

先放代码，在文章的最后我会把项目地址给大家。这个项目是可以运行的，直接导入Idea就可以了。

pom.xml文件：（这里只有最基本的包，Spring必须得是4.0+）



```
<properties>
        <spring.version>4.2.8.RELEASE</spring.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>${spring.version}</version>
        </dependency>
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
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>javax.servlet-api</artifactId>
            <version>3.1.0</version>
            <scope>provided</scope>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-aop</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context-support</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
            <version>2.5.3</version>
            <scope>runtime</scope>
        </dependency>
    </dependencies>
```
web.xml：（servlet和所有的filter都要加<async-supported>true</async-supported>）
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_3_1.xsd"
         version="3.1">
    <welcome-file-list>
        <welcome-file>index.jsp</welcome-file>
    </welcome-file-list>

    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>classpath:config/spring/*.xml</param-value>
    </context-param>

    <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>

    <servlet>
        <servlet-name>dispatcherServlet</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>classpath:config/spring/*.xml</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
        <async-supported>true</async-supported>
    </servlet>
    <servlet-mapping>
        <servlet-name>dispatcherServlet</servlet-name>
        <url-pattern>/</url-pattern>
    </servlet-mapping>

    <filter>
        <filter-name>encodingFilter</filter-name>
        <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
        <init-param>
            <param-name>encoding</param-name>
            <param-value>UTF-8</param-value>
        </init-param>
        <init-param>
            <param-name>forceEncoding</param-name>
            <param-value>true</param-value>
        </init-param>
        <async-supported>true</async-supported>
    </filter>

    <filter-mapping>
        <filter-name>encodingFilter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
</web-app>
```
dispatcher.xml：（SpringMVC配置文件）
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
                        http://www.springframework.org/schema/beans/spring-beans-3.1.xsd
                        http://www.springframework.org/schema/context
                        http://www.springframework.org/schema/context/spring-context-3.1.xsd
                        http://www.springframework.org/schema/mvc
                        http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd">
    <context:annotation-config />
    <mvc:annotation-driven />
    <context:component-scan base-package="com.hyy" />

    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="prefix" value="/WEB-INF/views/" />
        <property name="suffix" value=".jsp" />
    </bean>
</beans>
```
WebSocketConfig类：（Spring WebSocket的配置文件，这里采用的是注解的方式）
```java
import org.springframework.context.annotation.Configuration;
import org.springframework.messaging.simp.config.MessageBrokerRegistry;
import org.springframework.web.socket.config.annotation.AbstractWebSocketMessageBrokerConfigurer;
import org.springframework.web.socket.config.annotation.EnableWebSocketMessageBroker;
import org.springframework.web.socket.config.annotation.StompEndpointRegistry;

/**
 * Created by haoyuyang on 2016/11/25.
 */
@Configuration
@EnableWebSocketMessageBroker
public class WebSocketConfig extends AbstractWebSocketMessageBrokerConfigurer {

    /**
     * 将"/hello"路径注册为STOMP端点，这个路径与发送和接收消息的目的路径有所不同，这是一个端点，客户端在订阅或发布消息到目的地址前，要连接该端点，
     * 即用户发送请求url="/applicationName/hello"与STOMP server进行连接。之后再转发到订阅url；
     * PS：端点的作用——客户端在订阅或发布消息到目的地址前，要连接该端点。
     * @param stompEndpointRegistry
     */
    public void registerStompEndpoints(StompEndpointRegistry stompEndpointRegistry) {
        //在网页上可以通过"/applicationName/hello"来和服务器的WebSocket连接
        stompEndpointRegistry.addEndpoint("/hello").setAllowedOrigins("*").withSockJS();
    }

    /**
     * 配置了一个简单的消息代理，如果不重载，默认情况下回自动配置一个简单的内存消息代理，用来处理以"/topic"为前缀的消息。这里重载configureMessageBroker()方法，
     * 消息代理将会处理前缀为"/topic"和"/queue"的消息。
     * @param registry
     */
    @Override
    public void configureMessageBroker(MessageBrokerRegistry registry) {
         //应用程序以/app为前缀，代理目的地以/topic、/user为前缀
        registry.enableSimpleBroker("/topic", "/user");
        registry.setApplicationDestinationPrefixes("/app");
        registry.setUserDestinationPrefix("/user");
    }
}
```
**PS：**


registry.enableSimpleBroker("/topic", "/user");这句话表示在topic和user这两个域上可以向客户端发消息。

registry.setUserDestinationPrefix("/user");这句话表示给指定用户发送一对一的主题前缀是"/user"。


registry.setApplicationDestinationPrefixes("/app");这句话表示客户单向服务器端发送时的主题上面需要加"/app"作为前缀。


stompEndpointRegistry.addEndpoint("/hello").setAllowedOrigins("*").withSokJS();这个和客户端创建连接时的url有关，其中setAllowedOrigins()方法表示允许连接的域名，withSockJS()方法表示支持以SockJS方式连接服务器。

接下来是测试类GreetingController：

```java
import com.hyy.model.Greeting;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.handler.annotation.Header;
import org.springframework.messaging.handler.annotation.Headers;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.messaging.simp.SimpMessageSendingOperations;
import org.springframework.messaging.simp.annotation.SendToUser;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.Map;

/**
 * Created by haoyuyang on 2016/11/25.
 */
@RestController
public class GreetingController {

    @Autowired
    private SimpMessageSendingOperations simpMessageSendingOperations;

    /**
     * 表示服务端可以接收客户端通过主题“/app/hello”发送过来的消息，客户端需要在主题"/topic/hello"上监听并接收服务端发回的消息
     * @param topic
     * @param headers
     */
    @MessageMapping("/hello") //"/hello"为WebSocketConfig类中registerStompEndpoints()方法配置的
    @SendTo("/topic/greetings")
    public void greeting(@Header("atytopic") String topic, @Headers Map<String, Object> headers) {
        System.out.println("connected successfully....");
        System.out.println(topic);
        System.out.println(headers);
    }

    /**
     * 这里用的是@SendToUser，这就是发送给单一客户端的标志。本例中，
     * 客户端接收一对一消息的主题应该是“/user/” + 用户Id + “/message” ,这里的用户id可以是一个普通的字符串，只要每个用户端都使用自己的id并且服务端知道每个用户的id就行。
     * @return
     */
    @MessageMapping("/message")
    @SendToUser("/message")
    public Greeting handleSubscribe() {
        System.out.println("this is the @SubscribeMapping('/marco')");
        return new Greeting("I am a msg from SubscribeMapping('/macro').");
    }

    /**
     * 测试对指定用户发送消息方法
     * @return
     */
    @RequestMapping(path = "/send", method = RequestMethod.GET)
    public Greeting send() {
        simpMessageSendingOperations.convertAndSendToUser("1", "/message", new Greeting("I am a msg from SubscribeMapping('/macro')."));
        return new Greeting("I am a msg from SubscribeMapping('/macro').");
    }

}
```





**PS:**

这个类里面注入了SimpMessagingTemplete对象，后面动态发送消息时需要这个对象。

第一个方法，表示服务器端可以接收客户端通过主题"/app/hello"发送过来的消息，客户端需要在主题"/topic/hello"上监听并接收服务器发回的消息。

第二个方法道理相同，只是注意这里用的是@SendToUser注解，这就是发送给单一客户端的标志。本例中，客户端接收一对一消息的主题应该是"/user/"+userId+"/message"，这里的用户id可以是一个普通字符串，只要每个客户端都使用自己的id并且服务器端知道每个用户的id就行了。

发送消息使用SimpMessagingTemplete类的convertAndSend("/topic/hello", greeting); //广播，和convertAndSendToUser(userId, "/message", userMessage); //一对一发送给特定用户。

Greeting类：
```java
public class Greeting {
	private String content;

    public Greeting(String content) {
        this.content = content;
    }

    public String getContent() {
        return content;
    }
}
```
测试页面websocket.jsp：
```
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html lang="en">
<head>
    <title>Hello WebSocket</title>
    <script src="http://cdn.bootcss.com/sockjs-client/1.1.1/sockjs.min.js"></script>
    <script src="http://cdn.bootcss.com/stomp.js/2.3.3/stomp.js"></script>
    <script src="http://cdn.bootcss.com/jquery/3.1.1/jquery.min.js"></script>
    <script type="text/javascript">
        $(document).ready(function(){
            connect();
            //checkoutUserlist();
        });

        var stompClient = null;

        function setConnected(connected) {
            document.getElementById('connect').disabled = connected;
            document.getElementById('disconnect').disabled = !connected;
            document.getElementById('conversationDiv').style.visibility = connected ? 'visible' : 'hidden';
            document.getElementById('response').innerHTML = '';
        }

        //this line.
        function connect() {
            var userid = document.getElementById('name').value;
            var socket = new SockJS("http://192.168.3.149:8080/springmvc/hello");
            stompClient = Stomp.over(socket);
            stompClient.connect({}, function(frame) {
                setConnected(true);
                console.log('Connected: ' + frame);
                stompClient.subscribe('/topic/greetings', function(greeting){
                    showGreeting(JSON.parse(greeting.body).content);
                });

                stompClient.subscribe('/user/' + userid + '/message',function(greeting){
                    alert(JSON.parse(greeting.body).content);
                    showGreeting(JSON.parse(greeting.body).content);
                });
            });
        }

        function sendName() {
            var name = document.getElementById('name').value;
            stompClient.send("/app/hello", {atytopic:"greetings"}, JSON.stringify({ 'name': name }));
        }

        function connectAny() {
            var socket = new SockJS("http://localhost:8080/springmvc/hello");
            stompClient = Stomp.over(socket);
            stompClient.connect({}, function(frame) {
                setConnected(true);
                console.log('Connected: ' + frame);
                stompClient.subscribe('/topic/feed', function(greeting){
                    alert(JSON.parse(greeting.body).content);
                    showGreeting(JSON.parse(greeting.body).content);
                });
            });
        }

        function disconnect() {
            if (stompClient != null) {
                stompClient.disconnect();
            }
            setConnected(false);
            console.log("Disconnected");
        }


        function showGreeting(message) {
            var response = document.getElementById('response');
            var p = document.createElement('p');
            p.style.wordWrap = 'break-word';
            p.appendChild(document.createTextNode(message));
            response.appendChild(p);
        }
    </script>
</head>
<body>
<noscript><h2 style="color: #ff0000">Seems your browser doesn't support Javascript! Websocket relies on Javascript being enabled. Please enable
    Javascript and reload this page!</h2></noscript>
<div>
    <div>
        <button id="connect" onclick="connect();">Connect</button>
        <button id="connectAny" onclick="connectAny();">ConnectAny</button>
        <button id="disconnect" disabled="disabled" onclick="disconnect();">Disconnect</button>
    </div>
    <div id="conversationDiv">
        <label>What is your name?</label><input type="text" id="name" />
        <button id="sendName" onclick="sendName();">Send</button>
        <p id="response"></p>
    </div>
</div>
</body>
</html>
```
**PS:**
stompClient.subscribe('/topic/greetings', function(greeting){
showGreeting(JSON.parse(greeting.body).content);

});该方法是接收广播消息。


stompClient.subscribe('/user/' + userid + '/message',function(greeting){
alert(JSON.parse(greeting.body).content);

        showGreeting(JSON.parse(greeting.body).content);

});该方法表示接收一对一消息，其主题是"/user/"+userId+"/message"，不同客户端具有不同的id。如果两个或多个客户端具有相同的id，那么服务器端给该userId发送消息时，这些客户端都可以收到。


**如果项目中配置了拦截器，浏览器console标签中会报如下错误：**

**sockjs.min.js:2 GET http://localhost:8200/s3captrue/endpoint/info?t=1480493527907XMLHttpRequest cannot load http://localhost:8200/s3captrue/endpoint/info?t=1480493527907. No 'Access-Control-Allow-Origin' header is present on the requested resource. Origin 'null' is therefore not allowed access. The response had HTTP status code 500.**

**JAVA的控制台会输出如下错误:**

**java.lang.ClassCastException: org.springframework.web.socket.sockjs.support.SockJsHttpRequestHandler cannot be cast to org.springframework.web.method.HandlerMethod**

**则需要过滤掉对websoket的拦截：**

```
<mvc:interceptors>
		<mvc:interceptor>
			<mvc:mapping path="/**"/>
			<mvc:exclude-mapping path="/endpoint/**" />
			<bean class="com.hyy.common.interceptor.AuthInterceptor"/>
		</mvc:interceptor>
	</mvc:interceptors>
```
**PS:<mvc:exlude-mapping path="/endpoint/**" />标签中的**/endpoint为WebConfig.java中registerStompEndpoints()方法中配置的stompEndpointRegistry.addEndpoint("/endpoint").setAllowedOrigins("*").withSockJS();****


如果使用了Nginx服务器，需要在location中加入如下代码：



**proxy_set_header Upgrade $http_upgrade;proxy_set_header Connection "upgrade";**

本例的代码：https://github.com/haoyuyang/SpringWebSocket-SockJS-STOMP



