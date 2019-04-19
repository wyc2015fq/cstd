# 用spring boot提供websocket后台 - 左直拳的马桶_日用桶 - CSDN博客
2019年04月09日 19:00:51[左直拳](https://me.csdn.net/leftfist)阅读数：36
个人分类：[Java](https://blog.csdn.net/leftfist/article/category/2412493)
websocket，提供客户端与服务器之间交流的双工协议（见拙作《[WebSocket](https://leftfist.blog.csdn.net/article/details/88130302)》），使用spring boot，可以很方便地提供websocket的服务器端。
主要是标上几个注解。
主体代码：mySocket.java
```java
import javax.websocket.*;
import javax.websocket.server.ServerEndpoint;
import java.io.IOException;
import java.util.concurrent.CopyOnWriteArraySet;
@ServerEndpoint("/ws")
@Component
public class CaptureSocket {
    private final static Logger LOGGER = LoggerFactory.getLogger(CaptureSocket.class);
    private static CopyOnWriteArraySet<CaptureSocket> set = new CopyOnWriteArraySet<>();
    private Session session;
    @OnOpen
    public void onOpen(Session session) throws IOException,InterruptedException {
        this.session=session;
        set.add(this);
        LOGGER.info("新连接来自：" + session.getRequestURI().getHost());
    }
    @OnClose
    public void onClose(){
        set.remove(this);
        LOGGER.info("连接关闭：" + this.session.getRequestURI().getHost());
    }
    @OnMessage
    public void onMessage(String message,Session session){
        LOGGER.info("来自客户端消息：" + message);
    }
    @OnError
    public void onError(Session session,Throwable err){
        LOGGER.info(err.getMessage());
        err.printStackTrace();
    }
}
```
配置文件：mySocketConfig.java
```java
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.web.socket.server.standard.ServerEndpointExporter;
@Configuration
public class CaptureSocketConfig {
    @Bean
    public ServerEndpointExporter serverEndpointExporter(){
        return new ServerEndpointExporter();
    }
}
```
这个配置文件无须显示调用，系统会自动解释。究其原因，应该是标注“@configuration”起了作用。
此配置文件的作用，从代码来看，是为了配合解释服务器端点用的？，主体代码mySocket.java里有个标注：`@ServerEndpoint("/ws")`
相应的客户端代码有：
```
var ws;
function wsconnect() {
	var ws = new WebSocket("ws://192.168.0.98:8085/ws");//最后一个ws应该就是EndPoint
	ws.onopen = function(evt) {
		console.log("Connection open ...");
		ws.send("hello socket!");
	};
	ws.onmessage = function(evt) {
		console.log("receive message: " + evt.data + ":" + (new Date()));
	};
	ws.onclose = function(evt) {
		console.log("Connection closed.Reconnect will be attempted in 1 second.", evt.reason);
		setTimeout(function() {
			ws = wsconnect();
		}, 1000);
	};
	ws.onerror = function(err) {
		console.error('Socket encountered error: ', err.message, 'Closing socket');
		ws.close();
	};
	return ws;
}
$(function(){
	ws = wsconnect();
});
```
