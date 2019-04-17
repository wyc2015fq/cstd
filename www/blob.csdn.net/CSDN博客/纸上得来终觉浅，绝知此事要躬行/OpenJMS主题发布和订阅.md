# OpenJMS主题发布和订阅 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年09月25日 00:08:42[boonya](https://me.csdn.net/boonya)阅读数：1491








**1、主题发布（Publisher）**



```java
package com.boonya.openjms;

import java.util.Hashtable;
import javax.jms.JMSException;
import javax.jms.Session;
import javax.jms.TextMessage;
import javax.jms.Topic;
import javax.jms.TopicConnection;
import javax.jms.TopicConnectionFactory;
import javax.jms.TopicPublisher;
import javax.jms.TopicSession;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;
/**
 * 主题发布
 * 
 * @author BOONYACHENGDU@GMAIL.COM
 * @date  2013-9-17
 * <p>注：确定系统变量配置OPEN_HOME,导入下载到的openjms下lib的所有Jars,启动openjms服务，运行main方法就行了</p>
 */
public class TopicPublish {

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static void main(String[] args) {
		try {
			// 取得JNDI上下文和连接 
			Hashtable properties = new Hashtable();
			properties.put(Context.INITIAL_CONTEXT_FACTORY,
					"org.exolab.jms.jndi.InitialContextFactory");
			// openJms默认的端口是1099 
			properties.put(Context.PROVIDER_URL, "rmi://localhost:1099/");
			Context context = new InitialContext(properties);
			// 获得JMS Topic连接队列工厂 
			TopicConnectionFactory factory = (TopicConnectionFactory) context
					.lookup("JmsTopicConnectionFactory");
			// 创建一个Topic连接，并启动 
			TopicConnection topicConnection = factory.createTopicConnection();
			topicConnection.start();
			// 创建一个Topic会话，并设置自动应答 
			TopicSession topicSession = topicConnection.createTopicSession(
					false, Session.AUTO_ACKNOWLEDGE);
			// lookup 得到 topic1 
			Topic topic = (Topic) context.lookup("topic1");
			// 用Topic会话生成Topic发布器 
			TopicPublisher topicPublisher = topicSession.createPublisher(topic);
			// 发布消息到Topic 
			System.out.println("消息发布到Topic");
			TextMessage message = topicSession
					.createTextMessage("你好，欢迎定购Topic类消息");
			topicPublisher.publish(message);
			// 资源清除
			context.close();
			topicSession.close();
			topicConnection.close();
		} catch (NamingException e) {
			e.printStackTrace();
		} catch (JMSException e) {
			e.printStackTrace();
		}
	}
}
```
**2、主题订阅（Subscriber）**




```java
package com.boonya.openjms;

import java.util.Hashtable;

import javax.jms.JMSException;
import javax.jms.Session;
import javax.jms.TextMessage;
import javax.jms.Topic;
import javax.jms.TopicConnection;
import javax.jms.TopicConnectionFactory;
import javax.jms.TopicSession;
import javax.jms.TopicSubscriber;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;
/**
 * 主题同步接收消息
 * 
 * @author BOONYACHENGDU@GMAIL.COM
 * @date  2013-9-17
 * <p>注：确定系统变量配置OPEN_HOME,导入下载到的openjms下lib的所有Jars,启动openjms服务，运行main方法就行了</p>
 * <p>先启动此类然后再发布主题</p>
 */
public class TopicSubscribeSynchronous {

	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static void main(String[] args) {
		try {
			System.out.println("定购消息接收启动："); // 取得JNDI上下文和连接
			Hashtable properties = new Hashtable();
			properties.put(Context.INITIAL_CONTEXT_FACTORY,
					"org.exolab.jms.jndi.InitialContextFactory");
			properties.put(Context.PROVIDER_URL, "rmi://localhost:1099/");
			Context context = new InitialContext(properties);
			// 获得Topic工厂和Connection
			TopicConnectionFactory factory = (TopicConnectionFactory) context
					.lookup("JmsTopicConnectionFactory");
			TopicConnection topicConnection = factory.createTopicConnection();
			topicConnection.start();
			// 创建Topic的会话，用于接收信息
			TopicSession topicSession = topicConnection.createTopicSession(
					false, Session.AUTO_ACKNOWLEDGE);
			// lookup topic1
			Topic topic = (Topic) context.lookup("topic1");
			// 创建Topic subscriber
			TopicSubscriber topicSubscriber = topicSession
					.createSubscriber(topic);
			// 收满10条订阅消息则退出
			for (int i = 0; i < 10; i++) {
				// 同步消息接收，使用receive方法，堵塞等待，直到接收消息
				TextMessage message = (TextMessage) topicSubscriber.receive();
				System.out.println("接收订阅消息[" + i + "]: " + message.getText());
			}
			// 资源清除
			context.close();
			topicSession.close();
			topicConnection.close();
			System.out.println("订阅接收结束.");
		} catch (NamingException e) {
			e.printStackTrace();
		} catch (JMSException e) {
			e.printStackTrace();
		}
	}
}
```

**3、订阅与发布的关系**


用户必须先订阅相应的主题，一旦有新的主题发布时，订阅者若在线可以及时收到主题消息；在程序设计中主题发布只对应其对应的订阅者，订阅者只能订阅本主题最近的消息，对于已经发布过的主题订阅者不会接受到此类主题，订阅者必须保证在线才能接收到主题。即，某个话题订阅的客户程序只能收到那些在它订阅之后发布的消息。为了接收到消息，订阅者必须保持活动状态。因此，发布者和订阅者之间存在时间上的依赖关系。主题订阅模式中，发布者和订阅者可以是一对多或多对多。

**注：订阅者须保证在线的情况下，主题发布后订阅者才能接收到此主题消息，所以在调用主题发布方法之前需要启动订阅者的方法。**






