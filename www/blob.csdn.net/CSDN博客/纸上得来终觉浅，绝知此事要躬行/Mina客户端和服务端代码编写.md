# Mina客户端和服务端代码编写 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年05月02日 21:52:13[boonya](https://me.csdn.net/boonya)阅读数：1826标签：[Java																[Mina](https://so.csdn.net/so/search/s.do?q=Mina&t=blog)](https://so.csdn.net/so/search/s.do?q=Java&t=blog)
个人分类：[Mina](https://blog.csdn.net/boonya/article/category/1401237)

所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)










1.客户端

1.1编写客户端

```java
package com.boonya.protocol;  
import java.net.InetSocketAddress;  
import java.nio.charset.Charset;  
import org.apache.log4j.Logger;  
import org.apache.mina.core.future.ConnectFuture;  
import org.apache.mina.core.service.IoConnector;  
import org.apache.mina.core.session.IoSession;  
import org.apache.mina.filter.codec.ProtocolCodecFilter;  
import org.apache.mina.filter.codec.textline.LineDelimiter;  
import org.apache.mina.filter.codec.textline.TextLineCodecFactory;  
import org.apache.mina.transport.socket.nio.NioSocketConnector;  
import com.boonya.protocol.handler.request.ClientMessageHandler;  
  
public class MyClient {  
  
    private static Logger logger = Logger.getLogger(MyClient.class);  
  
    private static String HOST = "127.0.0.1";  
  
    private static int PORT = 8900;  
  
    public static void main(String[] args) {  
        // 创建一个非组设的客户端客户端   
        IoConnector connector = new NioSocketConnector();  
        // 设置链接超时时间   
        connector.setConnectTimeoutMillis(30000);  
        // 添加过滤器   
        connector.getFilterChain().addLast( // 添加消息过滤器   
                "codec",  
                // Mina自带的根据文本换行符编解码的TextLineCodec过滤器 看到\r\n就认为一个完整的消息结束了   
                new ProtocolCodecFilter(new TextLineCodecFactory(Charset  
                        .forName("UTF-8"), LineDelimiter.WINDOWS.getValue(),  
                        LineDelimiter.WINDOWS.getValue())));  
        // 添加业务逻辑处理器类   
        connector.setHandler(new ClientMessageHandler());  
        IoSession session = null;  
        try {  
            ConnectFuture future = connector.connect(new InetSocketAddress(  
                    HOST, PORT));  
            future.awaitUninterruptibly(); // 等待连接创建完成   
            session = future.getSession();  
            session.write("mina");  
        } catch (Exception e) {  
            logger.info("客户端链接异常...");  
        }  
  
        session.getCloseFuture().awaitUninterruptibly();  
        logger.info("Mina要关闭了");  
        connector.dispose();  
    }  
  
}
```



1.2客户端逻辑处理

```java
package com.boonya.protocol.handler.request;  
import org.apache.log4j.Logger;  
import org.apache.mina.core.service.IoHandlerAdapter;  
import org.apache.mina.core.session.IoSession;  
  
public class ClientMessageHandler extends IoHandlerAdapter {  
  
    private static Logger logger = Logger.getLogger(ClientMessageHandler.class);  
  
    public void messageReceived(IoSession session, Object message)  
            throws Exception {  
        String msg = message.toString();  
        logger.info("客户端接收到的信息为：" + msg);  
    }  
  
    @Override  
    public void exceptionCaught(IoSession session, Throwable cause)  
            throws Exception {  
        logger.info("客户端发生异常..." + cause);  
    }  
}
```



2.服务端

2.1 编写服务端

```java
package com.boonya.protocol;  
import java.net.InetSocketAddress;  
import java.nio.charset.Charset;  
import org.apache.log4j.Logger;  
import org.apache.mina.core.service.IoAcceptor;  
import org.apache.mina.core.session.IdleStatus;  
import org.apache.mina.filter.codec.ProtocolCodecFilter;  
import org.apache.mina.filter.codec.textline.LineDelimiter;  
import org.apache.mina.filter.codec.textline.TextLineCodecFactory;  
import org.apache.mina.filter.logging.LoggingFilter;  
import org.apache.mina.transport.socket.nio.NioSocketAcceptor;  
import com.boonya.protocol.handler.response.ServerMessageHandler;  
  
public class Myserver {  
      
    private static Logger logger = Logger.getLogger(Myserver.class);  
  
    private static int PORT = 8900;  
  
    public static void main(String[] args) {  
        // 创建非阻塞的server端的socket   
        IoAcceptor acceptor = null;  
        try {  
            acceptor = new NioSocketAcceptor();  
            // 设置日志过滤器   
            acceptor.getFilterChain().addLast("logger", new LoggingFilter());  
            acceptor.getFilterChain().addLast( // 添加消息过滤器   
                    "codec",  
                    // Mina自带的根据文本换行符编解码的TextLineCodec过滤器 看到\r\n就认为一个完整的消息结束了   
                    new ProtocolCodecFilter(new TextLineCodecFactory(Charset  
                            .forName("UTF-8"),  
                            LineDelimiter.WINDOWS.getValue(),  
                            LineDelimiter.WINDOWS.getValue())));  
            // 设置读取数据的缓冲区的大小   
            acceptor.getSessionConfig().setReadBufferSize(2048);  
            // 读取通道10s内无操作进入空闲状态   
            acceptor.getSessionConfig().setIdleTime(IdleStatus.BOTH_IDLE, 10);  
            // 绑定逻辑处理器   
            acceptor.setHandler(new ServerMessageHandler());  
            // 绑定端口   
            acceptor.bind(new InetSocketAddress(PORT));  
            logger.info("服务器启动成功.. 端口号为:" + PORT);  
  
        } catch (Exception e) {  
            e.printStackTrace();  
        }  
    }
```




2.2服务端业务处理


```java
package com.boonya.protocol.handler.response;  
import java.util.Date;  
import org.apache.log4j.Logger;  
import org.apache.mina.core.service.IoHandlerAdapter;  
import org.apache.mina.core.session.IdleStatus;  
import org.apache.mina.core.session.IoSession;  
  
public class ServerMessageHandler extends IoHandlerAdapter {  
  
    public static Logger logger = Logger.getLogger(ServerMessageHandler.class);  
  
    public void sessionCreated(IoSession session) throws Exception {  
        logger.info("服务端与客户端创建连接");  
    }  
  
    public void sessionOpened(IoSession session) throws Exception {  
        logger.info("服务器与客户端连接打开...");  
    }  
  
    public void messageReceived(IoSession session, java.lang.Object message)  
            throws Exception {  
        String msg = message.toString();  
        logger.info("服务端接收的数据为:" + msg);  
        if ("bye".equals(msg)) {// 服务器断开的条件   
            session.close(true);  
        }  
        Date date = new Date();  
        session.write(date);  
    }  
  
    public void messageSent(IoSession session, Object message) throws Exception {  
        logger.info("服务端发送消息成功...");  
    }  
  
    public void sessionIdle(IoSession session, IdleStatus status)  
            throws java.lang.Exception {  
        logger.info("服务端进入空闲状态...");  
    }  
  
    public void exceptionCaught(IoSession session, Throwable cause)  
            throws Exception {  
        logger.info("服务端发送异常..."+cause);  
    }  
  
    public void sessionClosed(IoSession session) throws Exception {  
        logger.info("关闭连接...");  
    }  
  
}
```






