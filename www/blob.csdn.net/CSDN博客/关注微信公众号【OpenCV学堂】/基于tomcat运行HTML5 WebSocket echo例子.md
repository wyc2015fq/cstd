# 基于tomcat运行HTML5 WebSocket echo例子 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年09月26日 16:01:31[gloomyfish](https://me.csdn.net/jia20003)阅读数：8456








**一：概述**

作为HTML5新特性之一的WebSocket组件，在实时性有一定要求的WEB应用开发中还是有一定用武之地，高版本的IE、Chrome、FF浏览器都支持Websocket，标准的Websocket通信是基于RFC6455实现服务器端与客户端握手与消息接发的。如果对Websocket通信不是太理解，可以查看RFC文档即可，简单说就是通过发送HTTP请求，实现双方握手，将无状态的HTTP通信协议进一步升级成有状态的通信协议，同时Websocket还支持子协议选项与安全传输。标准的websocket连接URL以ws开头，如果是基于TLS的则以wss开头。基于Websocket可以很方便的开发基于web聊天程序，各种网页消息通知与推送通知。

         如果非要扒一扒websocket的今生前世的话，还记得最早的基于HTTP轮询实现网页即时通信的方式，那种做法比较消耗资源、于是有人改进了编程CometD长连接方式，可是本质上还是换汤不换药，而websocket的出现正好解决了这些问题，但是很多浏览器的低版本还是不支持websocket,于是还催生了一些基于websocket理念实现的JS通信框架，其中学得比较像的有SockJS与socket.io，他们都号称支持websocket，然后如果浏览器端不支持原生的websocket，它们会自动启用fallback选项使用其它诸如ajax、Http轮询、长轮询/连接、甚至是flash的socket等机制实现模拟websocket的工作方式，但是他们最大的弊端是如果客户端使用了这些框架，服务器必须用它们，否则等待开发者就是一大堆无法回避的问题，同时很多都是无解的。主要原因在于它们实现自己的协议集，不照它们的格式处理数据没法玩。闲话说的有点多。

二 ： 实现步骤

Tomcat7的高版本中实现了websocket服务器端RFC6455标准协议，可以跟浏览器端websocket进行通信，首先要做好如下几步：



1.      安装高版本JDK – JDK8

2.      安装tomcat 7.0.64

3.      在eclipse中建立一个动态的web项目



根据JSR标准，Java中实现websocket的标准接口可以基于注解方式，tomcat也搞好了，只有我们实现如下代码，即可创建一个websocket回声服务器：



```java
package com.websocket.demo;

import java.io.IOException;
import java.nio.ByteBuffer;

import javax.websocket.OnMessage;
import javax.websocket.OnOpen;
import javax.websocket.Session;
import javax.websocket.server.ServerEndpoint;

@ServerEndpoint(value = "/echo")
public class EchoExample {
	
	@OnMessage
	public void echoTextMessage(Session session, String msg, boolean last) {
		try {
			if (session.isOpen()) {
				System.out.println("received from client message = " + msg);
				session.getBasicRemote().sendText(msg, last);
			}
		} catch (IOException e) {
			try {
				session.close();
			} catch (IOException e1) {
			}
		}
	}
	
	@OnOpen
    public void openConn(Session session) throws IOException {
    	session.getBasicRemote().sendText("hello web socket"); // means open it
    }
	
    @OnMessage
    public void echoBinaryMessage(Session session, ByteBuffer bb, boolean last) {
    	System.out.println("send binary message...");
        try {
            if (session.isOpen()) {
            	System.out.println("byte buffer lenghth : " + bb.array().length);
            	System.out.println("byte buffer content: " + ((bb.array()[0]) & 0xff));
            	System.out.println("byte buffer content: " + ((bb.array()[1]) & 0xff));
            	System.out.println("byte buffer content: " + ((bb.array()[2]) & 0xff));
                session.getBasicRemote().sendBinary(bb, last);
            }
        } catch (IOException e) {
            try {
                session.close();
            } catch (IOException e1) {
                // Ignore
            }
        }
    }

}
```




如何在tomcat中启动websocket服务器，首先需要在web.xml添加如下配置：



```
<listener>
		<listener-class>org.apache.tomcat.websocket.server.WsContextListener</listener-class>
	</listener>
```

然后实现ServerApplicationConfig接口，实现如下：




```java
/*
 *
 */
package com.config.websocket.client;

import java.util.HashSet;
import java.util.Set;

import javax.websocket.Endpoint;
import javax.websocket.server.ServerApplicationConfig;
import javax.websocket.server.ServerEndpointConfig;

public class ScanWebSocketSeverConfig implements ServerApplicationConfig {

	@Override
	public Set<ServerEndpointConfig> getEndpointConfigs(Set<Class<? extends Endpoint>> scanned) {

		Set<ServerEndpointConfig> result = new HashSet<ServerEndpointConfig>();
/*		if (scanned.contains(EchoWsChatSever.class)) {
			result.add(ServerEndpointConfig.Builder.create(EchoWsChatSever.class, "/echo").build());
		}*/
		return result;
	}

	@Override
	public Set<Class<?>> getAnnotatedEndpointClasses(Set<Class<?>> scanned) {
		Set<Class<?>> results = new HashSet<Class<?>>();
		for (Class<?> clazz : scanned) {
			if (clazz.getPackage().getName().startsWith("com.websocket.")) {
				System.out.println("find end point : " + clazz.getName());
				results.add(clazz);
			}
		}
		return results;
	}
}
```

创建网页echo.html，内容如下：




```
<html>
<head>
<title>Web Socket Echo Test</title>
<script>
        var ws = null;
		var count = 0;
        function setConnected(connected) {
            document.getElementById('connect').disabled = connected;
            document.getElementById('disconnect').disabled = !connected;
            document.getElementById('echo').disabled  = !connected;
        }

        function connect() {
            var target = document.getElementById('target').value;
            if (target == '') {
                alert('Please select server side connection implementation.');
                return;
            }

            if ('WebSocket' in window) {
                ws = new WebSocket(target);
            } else if ('MozWebSocket' in window) {
                ws = new MozWebSocket(target);
            } else {
                alert('WebSocket is not supported by this browser.');
                return;
            }
            
            ws.onopen = function () {
                setConnected(true);
                log('Info: WebSocket connection opened.');
            };
            ws.onmessage = function (event) {
                log('Received: ' + event.data);
                if(event.data instanceof ArrayBuffer)
               	{
                	var bytes = new Uint8Array(event.data);
                	alert(bytes.length + " : " + bytes[0]);
               	}
            };
            ws.onclose = function (event) {
                setConnected(false);
                log('Info: WebSocket connection closed, Code: ' + event.code + (event.reason == "" ? "" : ", Reason: " + event.reason));
            };
            
        }

        function disconnect() {
            if (ws != null) {
            	ws.doClose();
            	ws = null;
            }
            setConnected(false);
        }

        function echo() {
            if (ws != null) {
                var message = document.getElementById('message').value;
                log('Sent: ' + message);
                ws.send(JSON.stringify({'textMessage': message})); 
                count++
            } else {
                alert('WebSocket connection not established, please connect.');
            }
        }

        function log(message) {
            var echomsg = document.getElementById('echomsg');
            var p = document.createElement('p');
            p.style.wordWrap = 'break-word';
            p.appendChild(document.createTextNode(message));
            echomsg.appendChild(p);
            while (echomsg.childNodes.length > 25) {
            	echomsg.removeChild(console.firstChild);
            }
            echomsg.scrollTop = console.scrollHeight;
        }

        document.addEventListener("DOMContentLoaded", function() {
            // Remove elements with "noscript" class - <noscript> is not allowed in XHTML
            var noscripts = document.getElementsByClassName("noscript");
            for (var i = 0; i < noscripts.length; i++) {
                noscripts[i].parentNode.removeChild(noscripts[i]);
            }
        }, false);
</script>
</head>
<body>
	<div>
		<h4>URL - ws://localhost:8080/websocket/echo</h4>
		<input id="target" type="text" size="40" style="width: 350px" />
	</div>
	<div>
		<button id="connect" onclick="connect();">Connect</button>
		<button id="disconnect" disabled="disabled" onclick="disconnect();">Disconnect</button>
	</div>
	<div>
		<textarea id="message" style="width: 350px">Here is a message!</textarea>
	</div>
	<div>
		<button id="echo" onclick="echo();" disabled="disabled">Echo message</button>
	</div>
	<div id="echomsg">
	</div>
</body>
</html>
```
三 ：运行与测试


打包部署到tomcat之后，启动chrom浏览器，输入地址：

http://localhost:8080/websocket/echo.html 

![](https://img-blog.csdn.net/20150926155838137)


后来，我还发现，tomcat实现websocket服务器端居然不支持子协议

跟3W上的测试URL结果不一样。




