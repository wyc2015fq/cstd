# Mina快速编码测试验证示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月01日 11:52:02[boonya](https://me.csdn.net/boonya)阅读数：1384








此文借鉴于Apache Mina 官网，[Apache Mina快速入门指南(Quick start guide)](http://mina.apache.org/mina-project/quick-start-guide.html)

### NIO服务端接收器



```java
package com.boonya.mina.quickstart;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.charset.Charset;

import org.apache.mina.core.service.IoAcceptor;
import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.filter.codec.ProtocolCodecFilter;
import org.apache.mina.filter.codec.textline.TextLineCodecFactory;
import org.apache.mina.filter.logging.LoggingFilter;
import org.apache.mina.transport.socket.nio.NioSocketAcceptor;

/**
 * MinaTimeServer 服务端
 * 
 * @packge com.boonya.mina.quickstart.MinaTimeServer
 * @date 2016年6月1日 上午9:49:34
 * @author pengjunlin
 * @comment
 * @update
 */
public class MinaTimeServer {
	
	 // 定义一个NioSocketAcceptor(服务端)监听端口
	 private static final int PORT = 9123;
	
	/**
	 * @throws IOException 
	 * 主函数
	 * 
	 * @MethodName: main
	 * @Description:
	 * @param args
	 * @throws
	 */
	public static void main(String[] args) throws IOException {
		// 第一步，我们需要一个监听进来的链接：一个基于TCP/IP的SocketAcceptor
		IoAcceptor acceptor = new NioSocketAcceptor();

		// 添加过滤器配置，过滤器会日志记录最新的session和接收到的消息
		acceptor.getFilterChain().addLast("logger", new LoggingFilter());
		// 将二进制数据转化为协议对应的消息类型，TextLineCodecFactory处理text类型的数据
		acceptor.getFilterChain().addLast("codec",new ProtocolCodecFilter(new TextLineCodecFactory(Charset.forName("UTF-8"))));

		// 使用对应的服务器handler 被用于与客户端提供请求时间服务
		acceptor.setHandler(new TimeServerHandler());

		// 设置被用户客户端请求的NioSocketAcceptor 设置
		// 对buffer设定缓冲区大小，目的是告知潜在的操作系统分配多大空间去处理进来的消息
		acceptor.getSessionConfig().setReadBufferSize(2048);
		// 指定空闲session的空闲时间，IdleStatus设置超时action
		acceptor.getSessionConfig().setIdleTime(IdleStatus.BOTH_IDLE, 10);
		
		// 绑定服务端socket通信端口
		acceptor.bind( new InetSocketAddress(PORT) );
	}

}
```



### NIO消息处理器



```java
package com.boonya.mina.quickstart;

import java.util.Date;

import org.apache.mina.core.service.IoHandlerAdapter;
import org.apache.mina.core.session.IdleStatus;
import org.apache.mina.core.session.IoSession;

/**
 * 时间服务器端-消息处理器
 * 
 * @packge com.boonya.mina.quickstart.TimeServerHandler
 * @date 2016年6月1日 上午9:55:42
 * @author pengjunlin
 * @comment Mina中*Handler继承自IoHandler
 * @update
 */
public class TimeServerHandler extends IoHandlerAdapter {
	/**
	 * 处理异常捕获方法（如不定义此方法，异常可能不会被正确捕获）
	 */
	@SuppressWarnings("deprecation")
	@Override
	public void exceptionCaught(IoSession session, Throwable cause)	throws Exception {
		// 打印异常输出
		cause.printStackTrace();
		// 有时候需要关闭session,根据实际情况决定
		session.close();
	}

	/**
	 * 消息接收处理方法
	 */
	@SuppressWarnings("deprecation")
	@Override
	public void messageReceived(IoSession session, Object message)	throws Exception {
		String str = message.toString();
		// 如果从客户端接收到'quit'指令，session将会被关闭
		if (str.trim().equalsIgnoreCase("quit")) {
			session.close();
			return;
		}
		// 根据str指令做相应的处理
		

		Date date = new Date();
		// write back to the client,如果没有定义codec,收到的数据是一个IoBuffer object（此处没有采用message）
		session.write(date.toString());
		System.out.println("Message written...");
	}

	/**
	 * session空闲处理方法
	 */
	@Override
	public void sessionIdle(IoSession session, IdleStatus status)throws Exception {
		System.out.println("IDLE " + session.getIdleCount(status));
	}

}
```



### Telnet客户端测试

首先检查开启你的操作系统telnet服务，我使用的是win7系统，设置如下图所示：

![](https://img-blog.csdn.net/20160601114620381)

然后，cmd命令窗口执行：telnet   [host]  [port]，如不知道本地机器的IP地址，ipconfig查找自己的局域网IP(当然，你也可以使用127.0.0.1或localhost)。侧视图如下所示：

![](https://img-blog.csdn.net/20160601115007742)执行成功后会弹出一个黑色窗口；


执行指令窗口（输入任意字符，输入'quit'命令停止）：


![](https://img-blog.csdn.net/20160601114855225)







