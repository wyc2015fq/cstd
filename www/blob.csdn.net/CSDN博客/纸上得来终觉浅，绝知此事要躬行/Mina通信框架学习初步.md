# Mina通信框架学习初步 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年04月23日 00:36:33[boonya](https://me.csdn.net/boonya)阅读数：1369
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









**1、Mina通信整体框架**

![](http://dl.iteye.com/upload/attachment/0083/4671/85618756-c9ba-3d8c-9a7f-3a56789b8648.jpg)

**2、Mina通信流程**

![](http://dl.iteye.com/upload/attachment/0083/4673/591abc17-63d9-3523-a819-34985f9705d4.jpg)

**3、Mina示例**

3.1、客户端处理器

```
package com.boonya.mina.simple;
import org.apache.mina.core.service.IoHandlerAdapter;
import org.apache.mina.core.session.IoSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
/**
 * 客户端 IoHandler
 */

public class MyClinetIoHandler extends IoHandlerAdapter {

 private final static Logger logger = LoggerFactory.getLogger(MyClinetIoHandler.class);

 private final String values;

 public MyClinetIoHandler(String values) {
  this.values = values;
 }

 @Override
 public void sessionOpened(IoSession session) throws Exception {
  session.write(values);
  logger.debug("MyClinetIoHandler sessionOpened---------");
 }

}
```

```java
package com.boonya.mina.simple;
import java.net.InetSocketAddress;
import java.nio.charset.Charset;
import org.apache.mina.core.service.IoConnector;
import org.apache.mina.filter.codec.ProtocolCodecFilter;
import org.apache.mina.filter.codec.textline.LineDelimiter;
import org.apache.mina.filter.codec.textline.TextLineCodecFactory;
import org.apache.mina.transport.socket.nio.NioSocketConnector;
/**
 * mina框架中客户端与服务器端的执行流程一致，
 * 唯一的区别是: IoService的client实现是IoConnetor,server端是IoAcceptor
 */

public class MyClient {

	public static void main(String[] args) {
		// 1.创建ioservice
		IoConnector connector = new NioSocketConnector();
		connector.setConnectTimeoutMillis(30000);

		// 2.注册过滤器
		connector.getFilterChain().addLast("codec",new ProtocolCodecFilter(new TextLineCodecFactory(Charset
		    .forName("UTF-8"), LineDelimiter.WINDOWS.getValue(),LineDelimiter.WINDOWS.getValue())));

		// 3注册iohandler,到ioserivce

		connector.setHandler(new MyClinetIoHandler("hello！\r\nworld!"));
		// 4.绑定套接字,建立连接
		connector.connect(new InetSocketAddress("localhost", 1000));

	}

}
```

3.2、客户端处理

```java
package com.boonya.mina.simple;
import org.apache.mina.core.service.IoHandlerAdapter;
import org.apache.mina.core.session.IoSession;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
/**
 * IoHandler处理客户端的数据
 */
public class MyServerIoHandler extends IoHandlerAdapter {

	private final static Logger logs = LoggerFactory.getLogger(MyServerIoHandler.class);

	public void messageReceived(IoSession session, Object message)	throws Exception {

		String str = message.toString();

		logs.info("the message received is[" + str + "]");

		System.out.println("the message received is[" + str + "]");

		if (str.endsWith("quit")){
			session.close(true);
			return;

		}

	}

}
```

```java
package com.boonya.mina.simple;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.charset.Charset;
import org.apache.mina.core.service.IoAcceptor;
import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.filter.codec.ProtocolCodecFilter;
import org.apache.mina.filter.codec.textline.LineDelimiter;
import org.apache.mina.filter.codec.textline.TextLineCodecFactory;
import org.apache.mina.transport.socket.nio.NioSocketAcceptor;

public class MyServer {

	public static void main(String[] args) throws IOException {

		// 1.编写ioservice
		IoAcceptor acceptor = new NioSocketAcceptor();
		acceptor.getSessionConfig().setReadBufferSize(2048);
		// 读写通道在10秒内无任何操作进入空闲状态
		acceptor.getSessionConfig().setIdleTime(IdleStatus.BOTH_IDLE, 10);

		// 2.编写过滤器,
		// 通过TextLineCodeFactory编解码工厂对字符串进行编解码处理
		acceptor.getFilterChain().addLast("codec",
				new ProtocolCodecFilter(new TextLineCodecFactory(Charset
						.forName("UTF-8"), LineDelimiter.WINDOWS.getValue(),
						LineDelimiter.WINDOWS.getValue())));// 以换行符为标识的数据

		// 3.注册iohandler到ioservice
		acceptor.setHandler(new MyServerIoHandler());

		// 4绑定套接字
		acceptor.bind(new InetSocketAddress(1000));

	}

}
```











