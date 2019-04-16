# ActiveMQ –JMS开源框架入门介绍 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年05月25日 11:16:32[gloomyfish](https://me.csdn.net/jia20003)阅读数：15929








介绍基本的JMS概念与开源的JMS框架ActiveMQ应用，内容涵盖一下几点：

1.      基本的JMS概念

2.       JMS的消息模式

3.       介绍ActiveMQ

4.      一个基于ActiveMQ的JMS例子程序

**一：JMS基本概念**

1.      JMS的目标

        为企业级的应用提供一种智能的消息系统，JMS定义了一整套的企业级的消息概念与工具，

        尽可能最小化的Java语言概念去构建最大化企业消息应用。统一已经存在的企业级消息系

        统功能。

2.      JMS提供者

        JMS提供者是指那些完全完成JMS功能与管理功能的JMS消息厂商，理论上JMS提供者完成  

        JMS消息产品必须是100%的纯Java语言实现，可以运行在跨平台的架构与操作系统上，当前

        一些JMS厂商包括IBM,Oracle, JBoss社区 (JBoss Community), Apache 社区(ApacheCommunity)。

3.      JMS应用程序, 一个完整的JMS应用应该实现以下功能：

         JMS 客户端 – Java语言开发的接受与发送消息的程序

         非JMS客户端 – 基于消息系统的本地API实现而不是JMS

         消息 – 应用程序用来相互交流信息的载体

          被管理对象–预先配置的JMS对象，JMS管理员创建，被客户端运用。如链接工厂，主题等

         JMS提供者–完成JMS功能与管理功能的消息系统



**二：JMS的消息模式**

**1.      点对点的消息模式(Point to Point Messaging)**

![](https://img-my.csdn.net/uploads/201205/25/1337915714_4642.png)


下面的JMS对象在点对点消息模式中是必须的：

a.      队列(Queue) – 一个提供者命名的队列对象，客户端将会使用这个命名的队列对象

b.     队列链接工厂(QueueConnectionFactory) – 客户端使用队列链接工厂创建链接队列

        ConnectionQueue来取得与JMS点对点消息提供者的链接。

c.      链接队列(ConnectionQueue) – 一个活动的链接队列存在在客户端与点对点消息提供者之

        间，客户用它创建一个或者多个JMS队列会话(QueueSession)

d.     队列会话(QueueSession) – 用来创建队列消息的发送者与接受者(QueueSenderand 

         QueueReceiver)

e.     消息发送者(QueueSender 或者MessageProducer)– 发送消息到已经声明的队列

f.       消息接受者(QueueReceiver或者MessageConsumer) – 接受已经被发送到指定队列的消息

**2.      发布订阅模式(publish – subscribe Mode)**

![](https://img-my.csdn.net/uploads/201205/25/1337915755_8015.png)


a.      主题Topic(Destination) – 一个提供者命名的主题对象，客户端将会使用这个命名的主题对象

b.     主题链接工厂(TopciConnectionFactory) – 客户端使用主题链接工厂创建链接主题

         ConnectionTopic来取得与JMS消息Pub/Sub提供者的链接。

c.      链接主题(ConnectionTopic) – 一个活动的链接主题存在发布者与订阅者之间

d.     会话(TopicSession) – 用来创建主题消息的发布者与订阅者 (TopicPublisher  and 

         TopicSubscribers)

e.     消息发送者MessageProducer) – 发送消息到已经声明的主题

f.       消息接受者(MessageConsumer) – 接受已经被发送到指定主题的消息



**三：介绍ActiveMQ**

ActiveMQ是apache社区完成的JMS开源消息组件，客户端支持多种语言调用，包括Java,C++, C#, 

Perl, Python等。支持Spring配置集成等。更多信息访问这里：

[http://activemq.apache.org/index.html](http://activemq.apache.org/index.html)

四：基于ActiveMQ的Publish/subscribe模式Demo程序



消息Broker，JMSprovider

```java
import java.net.URI;
import java.net.URISyntaxException;

import javax.jms.Connection;
import javax.jms.ConnectionFactory;
import javax.jms.Destination;
import javax.jms.JMSException;
import javax.jms.MessageProducer;
import javax.jms.Session;
import javax.jms.TextMessage;
import javax.naming.Context;
import javax.naming.InitialContext;
import javax.naming.NamingException;

import org.apache.activemq.broker.BrokerFactory;
import org.apache.activemq.broker.BrokerService;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * refer to http://activemq.apache.org/jndi-support.html
 * http://activemq.apache.org/how-do-i-embed-a-broker-inside-a-connection.html
 * @author gloomyfish
 *
 */
public class PureJMSProducer {
	
	private static final Log LOG = LogFactory.getLog(PureJMSProducer.class);

    private PureJMSProducer() {
    }

    /**
     * @param args the destination name to send to and optionally, the number of
     *                messages to send
     */
    public static void main(String[] args) {
        Context jndiContext = null;
        ConnectionFactory connectionFactory = null;
        Connection connection = null;
        Session session = null;
        Destination destination = null;
        MessageProducer producer = null;
        BrokerService broker = null;
        final int numMsgs = 10;

        /*
         * Create a JNDI API InitialContext object
         */
        try {
            jndiContext = new InitialContext();
        } catch (NamingException e) {
            LOG.info("Could not create JNDI API context: " + e.toString());
            System.exit(1);
        }
        
        // create external TCP broker
		try {
			broker = BrokerFactory.createBroker(new URI("broker:tcp://localhost:61616"));
			broker.start(); 
		} catch (URISyntaxException e) {
			LOG.info("Could not create broker: " + e.toString());
		} catch (Exception e) {
			LOG.info("Could not create broker: " + e.toString());
		}
//        try {
//        	
//        }

        /*
         * Look up connection factory and destination.
         */
        try {
            connectionFactory = (ConnectionFactory)jndiContext.lookup("ConnectionFactory");
            destination = (Destination)jndiContext.lookup("MyTopic");
        } catch (NamingException e) {
            LOG.info("JNDI API lookup failed: " + e);
            System.exit(1);
        }
        
        /*
         * Create connection. Create session from connection; false means
         * session is not transacted. Create sender and text message. Send
         * messages, varying text slightly. Send end-of-messages message.
         * Finally, close connection.
         */
        try {
            connection = connectionFactory.createConnection();
            session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);
            producer = session.createProducer(destination);
            TextMessage message = session.createTextMessage();
            Thread.sleep(3000);
            for (int i = 0; i < numMsgs; i++) {
                message.setText("This is message " + (i + 1));
                LOG.info("Sending message: " + message.getText());
                producer.send(message);
                Thread.sleep(3000);
            }

            /*
             * Send a non-text control message indicating end of messages.
             */
            producer.send(session.createMessage());
        } catch (JMSException e) {
            LOG.info("Exception occurred: " + e);
        } catch (InterruptedException e) {
        	LOG.info("Exception occurred: " + e);
		} finally {
            if (connection != null) {
                try {
                    connection.close();
                } catch (JMSException e) {
                }
            }
        }
		
		// stop the TCP broker
		try {
			broker.stop();
		} catch (Exception e) {
			LOG.info("stop the broker failed: " + e);
		}
    }


}
```
客户端：



```java
import java.io.IOException;

import javax.jms.Connection;
import javax.jms.JMSException;
import javax.jms.Message;
import javax.jms.MessageConsumer;
import javax.jms.MessageListener;
import javax.jms.Session;
import javax.jms.TextMessage;
import javax.jms.Topic;
import javax.naming.InitialContext;

import org.apache.activemq.ActiveMQConnectionFactory;


public class ActiveMQClient {
	
	public static void main(String[] args) throws IOException {
		
		// -- http://dlc.sun.com/pdf//816-5904-10/816-5904-10.pdf
		try {
		ActiveMQConnectionFactory factory = new ActiveMQConnectionFactory("tcp://localhost:61616");
		// ActiveMQConnectionFactory factory = new ActiveMQConnectionFactory("vm://locahost");
        Connection connection = factory.createConnection();
        connection.start();
        
        // create message topic
        //Topic topic= new ActiveMQTopic("MyTopic");
        InitialContext jndiContext=new InitialContext();
        Topic topic=(Topic)jndiContext.lookup("MyTopic"); 
        Session session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);
        
        // register message consumer
        MessageConsumer comsumer1 = session.createConsumer(topic);
        comsumer1.setMessageListener(new MessageListener(){
            public void onMessage(Message m) {
                try {
                	System.out.println("Consumer get " + ((TextMessage)m).getText());
                } catch (JMSException e) {
                	e.printStackTrace();
                } 
            }
        });
        Thread.sleep(30000);
        session.close();
        connection.stop();
        
		} catch(Exception e) {
			e.printStackTrace();
		}
    }

}
```
**项目配置，Jar依赖：**


![](https://img-my.csdn.net/uploads/201205/25/1337915919_7952.png)



**依赖的三个Jar分别为：**
- activemq-all.jar 
- geronimo-jms_1.1_spec-1.1.1.jar 
- xbean-spring.jar


**补充JNDI内容：**



```
java.naming.factory.initial = org.apache.activemq.jndi.ActiveMQInitialContextFactory
java.naming.provider.url = tcp://localhost:61616
connectionFactory=topicConnectionFactry
topic.MyTopic = example.MyTopic
```





