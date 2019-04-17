# OpenJMS程序实现消息的发送和接收 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年09月24日 23:47:39[boonya](https://me.csdn.net/boonya)阅读数：2065








**1、JMS方式简介**




Java消息服务(JMS Java Message Services)提供了点对点模式(Point-to-Point　Queue)和发布-订阅模式(Publish-Subscribe　Topics).

**Queue仅允许一个消息传送给一个客户（一对一）:**


Java消息服务JMS的接收者和发送者之间不存在时间上的依赖关系。不论发送者发送消息时接收者是否在运行，接收者都可以提取信息。接收者对于成功处理的消息给出回执。

**Topics可以有多个客户端（一对多，多对多）:**


向某个话题订阅的客户程序只能收到那些在它订阅之后发布的消息。为了接收到消息，订阅者必须保持活动状态。因此，发布者和订阅者之间存在时间上的依赖关系。


点对点消息模式通过一个消息队列(Queue)实现，消息的生产者向队列写入消息，消息的订阅者从队列提取消息。发布-订阅消息模式通过一个话题（Topic）节点构成的层次结构实现，消息的生产者向这个层次结构发布消息，消息的订阅者向这个结构订阅消息。


这里先介绍第一种方式。

**2、消息发送者（Sender）**



```java
package com.boonya.openjms;

import java.util.Hashtable;
import javax.jms.JMSException;
import javax.jms.Queue;
import javax.jms.QueueConnection;
import javax.jms.QueueSender;
import javax.jms.QueueSession;
import javax.jms.Session;
import javax.jms.TextMessage;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;
import javax.jms.QueueConnectionFactory;
/**
 * 消息发送(先发消息)
 * 
 * @author BOONYACHENGDU@GMAIL.COM
 * <p>编写步骤：</p>
 * 1，jndi查找ConnectionFactory
 * 2，Factory创建Connection
 * 3，Connection创建sesson
 * 4，session创建生产者、消费者
 * 5，发送、接受消息
 * 6，关闭资源
 * @date  2013-9-17
 * <p>注：确定系统变量配置OPEN_HOME,导入下载到的openjms下lib的所有Jars,启动openjms服务，运行main方法就行了</p>
 */
public class QueueSend {

	public static void main(String[] args) throws NamingException, JMSException {
		// 读取配置熟悉文件，这里模拟属性文件
		Hashtable<String, String> properties = new Hashtable<String, String>();
		properties.put(Context.INITIAL_CONTEXT_FACTORY,"org.exolab.jms.jndi.InitialContextFactory");
		properties.put(Context.PROVIDER_URL, "rmi://localhost:1099/");
		Context context = new InitialContext(properties);
		// 通过JNDI查找连接工厂ConnectionFactory
		QueueConnectionFactory factory = (QueueConnectionFactory) context.lookup("JmsQueueConnectionFactory");
		// 使用工厂创建连接Connection
		QueueConnection conn = factory.createQueueConnection();
		// 使用javax.jms.Connection创建Session，AUTO_ACKNOWLEDGE设定自动应答
		QueueSession session = conn.createQueueSession(false,Session.AUTO_ACKNOWLEDGE);
        
        Queue queue = (Queue) context.lookup("queue1");
        QueueSender queueSender = session.createSender(queue);
        TextMessage message = session.createTextMessage();
        message.setText("Hello, I'm openJms.");
        queueSender.send(message);
        System.out.println("info in jms queue ");
        // 释放资源
		context.close();
		session.close();
		conn.close();
	}
}
```

**3、消息接受者（Receiver）**




```java
package com.boonya.openjms;

import java.util.Hashtable;
import javax.jms.JMSException;
import javax.jms.Message;
import javax.jms.Queue;
import javax.jms.QueueConnection;
import javax.jms.QueueConnectionFactory;
import javax.jms.QueueReceiver;
import javax.jms.QueueSession;
import javax.jms.Session;
import javax.jms.TextMessage;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;

/**
 * 异步接收消息（后收消息）
 * 
 * @author BOONYACHENGDU@GMAIL.COM
 * <p>编写步骤：</p>
 * <span> 1，jndi查找ConnectionFactory
 * 2，Factory创建Connection
 * 3，Connection创建sesson
 * 4，session创建生产者、消费者
 * 5，发送、接受消息
 * 6，关闭资源</span>
 * @date  2013-9-17
 * <p>注：确定系统变量配置OPEN_HOME,导入下载到的openjms下lib的所有Jars,启动openjms服务，运行main方法就行了</p>
 */
public class QueueReceiveAsynchronous {

	/**
	 * @param args
	 * @throws JMSException
	 * @throws NamingException
	 */
	public static void main(String[] args) throws JMSException, NamingException {
		// 读取配置熟悉文件，这里模拟属性文件
		Hashtable<String, String> properties = new Hashtable<String, String>();
		properties.put(Context.INITIAL_CONTEXT_FACTORY,	"org.exolab.jms.jndi.InitialContextFactory");
		properties.put(Context.PROVIDER_URL, "rmi://localhost:1099/");
		Context context = new InitialContext(properties);
		// 通过JNDI查找连接工厂ConnectionFactory
		QueueConnectionFactory factory = (QueueConnectionFactory) context.lookup("JmsQueueConnectionFactory");
		// 使用工厂创建连接Connection
		QueueConnection conn = factory.createQueueConnection();
		conn.start();
		// 使用javax.jms.Connection创建Session，AUTO_ACKNOWLEDGE设定自动应答
		QueueSession session = conn.createQueueSession(false,Session.AUTO_ACKNOWLEDGE);

		Queue queue = (Queue) context.lookup("queue1");
		QueueReceiver receiver = session.createReceiver(queue);
		Message message = receiver.receive();
		String messageText = null;
		if (message instanceof TextMessage)
			messageText = ((TextMessage) message).getText();
		System.out.println(messageText);
		// 释放资源
		context.close();
		session.close();
		conn.close();
	}

}
```

**注：队列（queue）点对点模式是一对一的关系，消息发送和接收的必要条件是必须确保OpenJMS服务器是启动的。**








