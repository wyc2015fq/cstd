# JMS使用ActiveMQ实现﻿﻿Queue和Topic两种模式 - 零度的博客专栏 - CSDN博客
2016年04月07日 10:58:05[零度anngle](https://me.csdn.net/zmx729618)阅读数：10366
       jms就是java message service，是J2EE规范的一部分，跟jdbc差不多，sun只提供了接口，由各个厂商（provider）来进行具体的实现，然后使用者使用他们的jar包进行开发使用即可。
        另外在jms的API中，jms传递消息有两种方式，一种是点对点的Queue，还有一个是发布订阅的Topic方式。区别在于：
        对于Queue模式，一个发布者发布消息，下面的接收者按队列顺序接收，比如发布了10个消息，两个接收者A,B那就是A,B总共会收到10条消息，不重复。
        对于Topic模式，一个发布者发布消息，有两个接收者A,B来订阅，那么发布了10条消息，A,B各收到10条消息。
       使用activeMQ来完成jms的发送，必须要下载activeMQ，然后再本机安装，并且启动activeMQ的服务才行。在官网下载完成之后，运行bin目录下面的activemq.bat，将activeMQ成功启动。启动成功之后可以运行：[http://localhost:8161/admin/index.jsp](http://localhost:8161/admin/index.jsp) 查看一下。
# 1、Queue模式实现：
 1.1、发送端Sender
```java
package jms.test;
import javax.jms.Connection;
import javax.jms.ConnectionFactory;
import javax.jms.DeliveryMode;
import javax.jms.Destination;
import javax.jms.JMSException;
import javax.jms.MessageProducer;
import javax.jms.Session;
import javax.jms.TextMessage;
import org.apache.activemq.ActiveMQConnection;
import org.apache.activemq.ActiveMQConnectionFactory;
public class Sender {
	
	public static void main(String[] args) throws JMSException, InterruptedException {
		// ConnectionFactory ：连接工厂，JMS 用它创建连接
		ConnectionFactory connectionFactory = new ActiveMQConnectionFactory(
				                                  ActiveMQConnection.DEFAULT_USER,
				                                  ActiveMQConnection.DEFAULT_PASSWORD,
				                                  "tcp://localhost:61616");
		
		// Connection ：JMS 客户端到JMS Provider 的连接
		Connection connection =  connectionFactory.createConnection();
		
		connection.start();
		// Session： 一个发送或接收消息的线程
		Session session = connection.createSession(true, Session.AUTO_ACKNOWLEDGE);
		
		// Destination ：消息的目的地;消息发送给谁.
		Destination destination =  session.createQueue("my-queue");
		
		// MessageProducer：消息发送者
		MessageProducer producer =  session.createProducer(destination);
		
		// 设置不持久化，可以更改
        producer.setDeliveryMode(DeliveryMode.NON_PERSISTENT);
        
		for(int i=0;i<10;i++){
			//创建文本消息
			TextMessage message = session.createTextMessage("hello everyone, this is a test message"+i);
			
			Thread.sleep(1000);
			//发送消息
			producer.send(message);
		}
		
		session.commit();
		session.close();
		connection.close();
	}
}
```
运行之后控制台输出：
hello everyone, this is a test message0
hello everyone, this is a test message1
hello everyone, this is a test message2
hello everyone, this is a test message3
hello everyone, this is a test message4
hello everyone, this is a test message5
hello everyone, this is a test message6
hello everyone, this is a test message7
hello everyone, this is a test message8
hello everyone, this is a test message9
 1.2、接收端，Receiver代码：
```java
package jms.test;
import javax.jms.Connection;
import javax.jms.ConnectionFactory;
import javax.jms.Destination;
import javax.jms.JMSException;
import javax.jms.Message;
import javax.jms.MessageConsumer;
import javax.jms.MessageListener;
import javax.jms.Session;
import javax.jms.TextMessage;
import org.apache.activemq.ActiveMQConnection;
import org.apache.activemq.ActiveMQConnectionFactory;
public class Receiver {
	// ConnectionFactory ：连接工厂，JMS 用它创建连接
   private static ConnectionFactory connectionFactory = new ActiveMQConnectionFactory(ActiveMQConnection.DEFAULT_USER,
           ActiveMQConnection.DEFAULT_PASSWORD, "tcp://localhost:61616");
	
   public static void main(String[] args) throws JMSException {
	    // Connection ：JMS 客户端到JMS Provider 的连接
		final Connection connection =  connectionFactory.createConnection();
		
		connection.start();
		// Session： 一个发送或接收消息的线程
		final Session session = connection.createSession(true, Session.AUTO_ACKNOWLEDGE);
		// Destination ：消息的目的地;消息送谁那获取.
		Destination destination =  session.createQueue("my-queue");
		// 消费者，消息接收者
		MessageConsumer consumer1 =  session.createConsumer(destination);
		
		consumer1.setMessageListener(new MessageListener() {
				@Override
				public void onMessage(Message msg) {
					try {
						
						TextMessage message = (TextMessage)msg ;
						System.out.println("consumer1收到消息： "+message.getText());
						session.commit();
					} catch (JMSException e) {				
						e.printStackTrace();
					}
					
				}
			});
	
		// 再来一个消费者，消息接收者
		MessageConsumer consumer2 =  session.createConsumer(destination);
		
		consumer2.setMessageListener(new MessageListener() {
				@Override
				public void onMessage(Message msg) {
					try {
						
						TextMessage message = (TextMessage)msg ;
						System.out.println("consumer2收到消息： "+message.getText());
						session.commit();
					} catch (JMSException e) {				
						e.printStackTrace();
					}
					
				}
			});
		
		
	
}
	
	
}
```
运行之后控制台不会退出一直监听消息库，对于消息发送者的十条信息，控制输出：
consumer1收到消息： hello everyone, this is a test message0
consumer2收到消息： hello everyone, this is a test message1
consumer1收到消息： hello everyone, this is a test message2
consumer2收到消息： hello everyone, this is a test message3
consumer1收到消息： hello everyone, this is a test message4
consumer2收到消息： hello everyone, this is a test message5
consumer1收到消息： hello everyone, this is a test message6
consumer2收到消息： hello everyone, this is a test message7
consumer1收到消息： hello everyone, this is a test message8
consumer2收到消息： hello everyone, this is a test message9
# 2、Topic模式实现
2.1 、 消息发布者，Publisher代码：
```
﻿﻿﻿﻿package jms.test;
import java.util.Hashtable;  
import java.util.Map;  
  
import javax.jms.Connection;  
import javax.jms.ConnectionFactory;  
import javax.jms.Destination;  
import javax.jms.JMSException;  
import javax.jms.MapMessage;  
import javax.jms.Message;  
import javax.jms.MessageProducer;  
import javax.jms.Session;  
  
import org.apache.activemq.ActiveMQConnectionFactory;  
import org.apache.activemq.command.ActiveMQMapMessage;
public class Publisher {
	
    protected int MAX_DELTA_PERCENT = 1;  
    protected Map<String, Double> LAST_PRICES = new Hashtable<String, Double>();  
    protected static int count = 1;  
    protected static int total;  
      
    protected static String brokerURL = "tcp://localhost:61616";  
    protected static transient ConnectionFactory factory;  
    protected transient Connection connection;  
    protected transient Session session; 
    protected transient Destination destination;
    protected transient MessageProducer producer;  
      
    public Publisher() throws JMSException {  
        factory = new ActiveMQConnectionFactory(brokerURL);  //创建连接工场
        connection = factory.createConnection();   //创建连接
        try {  
            connection.start();   //打开连接
        } catch (JMSException jmse) {  
            connection.close();  
            throw jmse;  
        }  
        session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);  //创建session 不带事务
        destination = session.createTopic("STOCKS.topic"); //创建topic
        producer = session.createProducer(destination);    //创建publisher
    }  
      
    public void close() throws JMSException {  
        if (connection != null) {  
            connection.close();  
        }  
    }  
      
    public static void main(String[] args) throws JMSException {  
        Publisher publisher = new Publisher();  
        while (total < 50) {  
            for (int i = 0; i < count; i++) {  
                publisher.sendMessage();  
            }  
            total += count;  
            System.out.println("Published '" + count + "' of '" + total + "' price messages");  
            try {  
              Thread.sleep(1000);  
            } catch (InterruptedException x) {  
            }  
        }  
        publisher.close();  
    }  
  
    protected void sendMessage() throws JMSException {                  
        Message message = createStockMessage(session);  
        System.out.println("Sending: " + ((ActiveMQMapMessage)message).getContentMap() + " on destination: " + destination);  
        producer.send(destination, message);  
    }  
  
    protected Message createStockMessage(Session session) throws JMSException {  
        Double value = LAST_PRICES.get("topic");  
        if (value == null) {  
            value = new Double(Math.random() * 100);  
        }  
 
        double oldPrice = value.doubleValue();  
        value = new Double(mutatePrice(oldPrice));  
        LAST_PRICES.put("topic", value);  
        double price = value.doubleValue();  
        double offer = price * 1.001;  
        boolean up = (price > oldPrice);  
  
        MapMessage message = session.createMapMessage();  
        message.setString("topic", "topic");  
        message.setDouble("price", price);  
        message.setDouble("offer", offer);  
        message.setBoolean("up", up);  
        return message;  
    }
```
控制台打印：
Sending: {topic=topic, price=26.707862686880095, offer=26.73457054956697, up=false} on destination: topic://STOCKS.topic
Published '1' of '1' price messages
Sending: {topic=topic, price=26.904104854950052, offer=26.931008959805, up=true} on destination: topic://STOCKS.topic
Published '1' of '2' price messages
Sending: {topic=topic, price=26.84132968750509, offer=26.868171017192594, up=false} on destination: topic://STOCKS.topic
Published '1' of '3' price messages
Sending: {topic=topic, price=26.652301297312082, offer=26.67895359860939, up=false} on destination: topic://STOCKS.topic
Published '1' of '4' price messages
Sending: {topic=topic, price=26.60180161811153, offer=26.62840341972964, up=false} on destination: topic://STOCKS.topic
Published '1' of '5' price messages
Sending: {topic=topic, price=26.527634257875008, offer=26.55416189213288, up=false} on destination: topic://STOCKS.topic
Published '1' of '6' price messages
Sending: {topic=topic, price=26.758846815361945, offer=26.785605662177304, up=true} on destination: topic://STOCKS.topic
Published '1' of '7' price messages
Sending: {topic=topic, price=26.93616880981415, offer=26.96310497862396, up=true} on destination: topic://STOCKS.topic
Published '1' of '8' price messages
Sending: {topic=topic, price=26.80748490634637, offer=26.834292391252713, up=false} on destination: topic://STOCKS.topic
Published '1' of '9' price messages
Sending: {topic=topic, price=26.845968050168864, offer=26.872814018219028, up=true} on destination: topic://STOCKS.topic
Published '1' of '10' price messages
2.2 消息订阅者，Subscriber代码：
```java
package jms.test;
import javax.jms.Connection;  
import javax.jms.ConnectionFactory;  
import javax.jms.Destination;  
import javax.jms.JMSException;  
import javax.jms.MessageConsumer;  
import javax.jms.Session;
import java.text.DecimalFormat;  
import javax.jms.MapMessage;  
import javax.jms.Message;  
import javax.jms.MessageListener;
import org.apache.activemq.ActiveMQConnectionFactory;
public class Subscriber {
	
	
	private static String brokerURL = "tcp://localhost:61616";  
    private static transient ConnectionFactory factory;  
    private transient Connection connection;  
    private transient Session session;  
    private transient Destination destination;  
    private transient MessageConsumer messageConsumer;  
      
    public Subscriber() throws JMSException {  
        factory = new ActiveMQConnectionFactory(brokerURL);  
        connection = factory.createConnection();  
        connection.start();  
        session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE); 
        destination = session.createTopic("STOCKS.topic" );   
        messageConsumer = session.createConsumer(destination);
        
    } 
    
    public MessageConsumer getMessageConsumer() {  
        return messageConsumer;  
    }
  
      
    public void close() throws JMSException {  
        if (connection != null) {  
            connection.close();  
        }  
    }      
      
    public static void main(String[] args) throws JMSException {  
    	Subscriber consumer1 = new Subscriber();  
    	consumer1.messageConsumer.setMessageListener(new Listener());     
    	
    	Subscriber consumer2 = new Subscriber();  
    	consumer2.messageConsumer.setMessageListener(new Listener());  
    	
    	
    	Subscriber consumer3 = new Subscriber();  
    	consumer3.messageConsumer.setMessageListener(new Listener());  
    } 
    
    
    private static class Listener implements MessageListener {  
    	  
        public void onMessage(Message message) {  
            try {  
                MapMessage map = (MapMessage)message;  
                String topic = map.getString("topic");  
                double price = map.getDouble("price");  
                double offer = map.getDouble("offer");  
                boolean up = map.getBoolean("up");  
                DecimalFormat df = new DecimalFormat( "#,###,###,##0.00" );  
                System.out.println(topic + "\t" + df.format(price) + "\t" + df.format(offer) + "\t" + (up?"up":"down"));  
            } catch (Exception e) {  
                e.printStackTrace();  
            }  
        }  
      
    }
   
	
}
```
控制台打印：
topic 26.71 26.73 down
topic 26.71 26.73 down
topic 26.71 26.73 down
topic 26.90 26.93 up
topic 26.90 26.93 up
topic 26.90 26.93 up
topic 26.84 26.87 down
topic 26.84 26.87 down
topic 26.84 26.87 down
topic 26.65 26.68 down
