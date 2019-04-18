# springboot-25-springboot 集成 ActiveMq - weixin_33985507的博客 - CSDN博客
2017年06月19日 22:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
消息的发布有2种形式, 队列式(点对点) 和主题式(pub/sub) 模式, 队列式发布后, 接收者从队列中获取消息后, 消息就会消失, 但任意消费者都可以从队列中接受消息, 消息只能被接受一次
主题式则为接受后消息不消失
JMS 是(java message service) 是 基于JVM代理的规范, ActiveMQ是他的一种实现
```
2、JMS消息基本组件
2.1、ConnectionFactory
创建Connection对象的工厂，针对两种不同的jms消息模型，分别有QueueConnectionFactory和TopicConnectionFactory两种。可以通过JNDI来查找ConnectionFactory对象。
2.2、Destination
Destination的意思是消息生产者的消息发送目标或者说消息消费者的消息来源。对于消息生产者来说，它的Destination是某个队列（Queue）或某个主题（Topic）;对于消息消费者来说，它的Destination也是某个队列或主题（即消息来源）。
所以，Destination实际上就是两种类型的对象：Queue、Topic可以通过JNDI来查找Destination。
2.3、Connection
Connection表示在客户端和JMS系统之间建立的链接（对TCP/IP socket的包装）。Connection可以产生一个或多个Session。跟ConnectionFactory一样，Connection也有两种类型：QueueConnection和TopicConnection。
2.4、Session
Session是操作消息的接口。可以通过session创建生产者、消费者、消息等。Session提供了事务的功能。当需要使用session发送/接收多个消息时，可以将这些发送/接收动作放到一个事务中。同样，也分QueueSession和TopicSession。
2.5、消息的生产者
消息生产者由Session创建，并用于将消息发送到Destination。同样，消息生产者分两种类型：QueueSender和TopicPublisher。可以调用消息生产者的方法（send或publish方法）发送消息。
2.6、消息消费者
消息消费者由Session创建，用于接收被发送到Destination的消息。两种类型：QueueReceiver和TopicSubscriber。可分别通过session的createReceiver(Queue)或createSubscriber(Topic)来创建。当然，也可以session的creatDurableSubscriber方法来创建持久化的订阅者。
2.7、MessageListener
消息监听器。如果注册了消息监听器，一旦消息到达，将自动调用监听器的onMessage方法。EJB中的MDB（Message-Driven Bean）就是一种MessageListener。
```
## 1, 安装activeMQ
1), linux安装
```
访问: http://activemq.apache.org/activemq-5111-release.html
```
2), docker安装
```
sudo docker run -d -p 61616:61616 -p 8161:8161 cloudesire/activemq
```
安装成功后, 可访问: 
![](https://images2015.cnblogs.com/blog/957395/201706/957395-20170613141609415-724749847.png)
3) 使用springboot的内嵌 activemq
```
<!--springboot 内嵌 activemq-->
        <dependency>
            <groupId>org.apacke.activemq</groupId>
            <artifactId>activemq-broker</artifactId>
        </dependency>
```
## 2, 新建springboot项目
#### 1), 添加依赖
```
<dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jms</artifactId>
        </dependency>
        <dependency>
            <groupId>org.apache.activemq</groupId>
            <artifactId>activemq-client</artifactId>
        </dependency>
```
#### 2), 添加配置:  
```
spring: 
  activemq:
    broker-url: tcp://192.168.50.202:61616
```
支持的配置有: 
```
activemq:
    broker-url: tcp://192.168.50.202:61616
    user:
    password:
    in-memory:
```
3) 消息发送者: 
```
package com.wenbronk.enterprise.jms.message;
import org.springframework.jms.core.MessageCreator;
import javax.jms.JMSException;
import javax.jms.Message;
import javax.jms.Session;
/**
 * 消息创建者
 * Created by wenbronk on 2017/6/13.
 */
public class CreateMessage implements MessageCreator{
    @Override
    public Message createMessage(Session session) throws JMSException {
        return session.createTextMessage("测试消息");
    }
}
```
4) 消息接受着: 
```
package com.wenbronk.enterprise.jms.message;
import org.springframework.jms.annotation.JmsListener;
import org.springframework.stereotype.Component;
/**
 * 消息接受者
 * Created by wenbronk on 2017/6/13.
 */
@Component
public class ReceiMessage {
    /**
     * 使用 @JmsListener 指定要监听的域, 有消息发送时就会发送到此域中
     * @param message
     */
    @JmsListener(destination = "my-destination")
    public void receiveMessage(String message) {
        System.out.println("接受到的消息是: " + message);
    }
}
```
5), 消息发送: 
```
package com.wenbronk.enterprise.jms.config;
import com.wenbronk.enterprise.jms.message.CreateMessage;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.jms.core.JmsTemplate;
import org.springframework.stereotype.Component;
/**
 * 消息发送及目的地定义
 * 实现commandLineRunner, 使得程序启动后执行该类的run() 方法
 * Created by wenbronk on 2017/6/13.
 */
@Component
public class MessageConfig implements CommandLineRunner {
    @Autowired
    private JmsTemplate jmsTemplate;
    /**
     * 消息域为  my-destination
     * 发送者为 createMessage()
     * @param args
     * @throws Exception
     */
    @Override
    public void run(String... args) throws Exception {
        jmsTemplate.send("my-destination", new CreateMessage());
    }
}
```
如果想自定义使用 topic 或者 queue 模式, 需要自己指定: 
MessageConfig
```
package com.wenbronk.enterprise.jms.config;
import org.apache.activemq.command.ActiveMQQueue;
import org.apache.activemq.command.ActiveMQTopic;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import javax.jms.Queue;
import javax.jms.Topic;
/**
 * 创建 点对点模式的对象和发布订阅模式的对象
 * Created by wenbronk on 2017/6/13.
 */
@Configuration
public class TopicConfig {
    @Bean
    public Queue queue() {
        return new ActiveMQQueue("sample.queue");
    }
    @Bean
    public Topic topic() {
        return new ActiveMQTopic("sample.topic");
    }
}
```
消息发布者:
```
package com.wenbronk.enterprise.jms.producer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.jms.core.JmsMessagingTemplate;
import org.springframework.scheduling.annotation.EnableScheduling;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;
import javax.jms.Queue;
import javax.jms.Topic;
/**
 * 点对点和订阅模式的消息发布
 * Created by wenbronk on 2017/6/13.
 */
@Component
@EnableScheduling
public class MessageProducer {
    @Autowired
    private JmsMessagingTemplate jmsMessagingTemplate;
    @Autowired
    private Queue queue;
    @Autowired
    private Topic topic;
    @Scheduled(fixedDelay=3000)//每3s执行1次
    public void send() {
        //send queue.
        this.jmsMessagingTemplate.convertAndSend(this.queue, "hi,activeMQ");
        //send topic.
        this.jmsMessagingTemplate.convertAndSend(this.topic, "hi,activeMQ(topic)");
    }
}
```
消息接受者
```
package com.wenbronk.enterprise.jms.message;
import org.springframework.jms.annotation.JmsListener;
import org.springframework.stereotype.Component;
/**
 * 点对点和订阅模式的消息接受着
 * Created by wenbronk on 2017/6/13.
 */
@Component
public class MessageConsumer {
    @JmsListener(destination = "sample.topic")
    public void receiveQueue(String text) {
        System.out.println("Consumer2="+text);
    }
    @JmsListener(destination = "sample.topic")
    public void receiveTopic(String text) {
        System.out.println("Consumer3="+text);
    }
}
```
需要在配置文件中开启　发布订阅模式的支持：　
```
spring:   
　activemq:
    broker-url: tcp://192.168.50.202:61616
  jms:
    pub-sub-domain: true
```
