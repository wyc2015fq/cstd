# ActiveMQ学习-Network connectors（1） - 江南烟雨 - CSDN博客
2015年07月26日 15:48:13[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：5929
本文为network connectors的static connector学习笔记。
# Network connectors
broker网络能够创建多个相互连接的ActiveMq实例组成的簇，以应对更加复杂的消息场景。Network connectors提供了broker之间的通信。
默认情况下，network connector是单向通道，它只会把收到的消息投递给与之建立连接的另一个broker。这通常称为forwarding bridge。ActiveMQ也支持双向通道，即duplex connector。下图是一个包含了这两者的复杂网络。
![](https://img-blog.csdn.net/20150726153516845?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Network connector的XML配置如下：
```
<networkConnectors>
<networkConnector name="default-nc" uri="multicast://default"/>
</networkConnectors>
```
**一个重要的概念-discovery**
discovery：是一个检测远程broker服务的进程。client通常需要感知所有的broker。broker，需要感知其他存在的broker，以建立broker的网络。
当我们想配置一个broker网络时，首要问题是：我们知道每个broker的准确地址吗？如果是，可以以静态的方式配置，将客户端连接到提前定义好的broker URI，这在你想完全控制所有资源的生产环境中比较常见。
如果客户端以及broker相互不知道彼此的地址，那么必须使用一种discovery机制来发现已有的broker。这种设置在开发环境下比较常见，易于配置和维护。
## static network
### 概念介绍
只要我们知道了想要使用的broker的地址，就可以使用static配置方式。
### Static connector
用来创建网络中多个broker的静态配置。协议使用组合URI，即URI中包含其他URI。格式如下：
static:(uri1,uri2,uri3,...) ?key=value
XML中配置示例：
```
<networkConnectors>
<networkConnector name="local network"
uri="static://(tcp://remotehost1:61616,tcp://remotehost2:61616)"/>
</networkConnectors>
```
#### 程序实例
为了更好的理解，可以通过一个发布者-订阅者的例子来进行说明。（demo来自ActiveMQ in action上的例子）
这个例子使用下图所示的broker拓扑结构：
![](https://img-blog.csdn.net/20150726153639900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
BrokerA与brokerB单向相连，当生产者把消息发送给brokerA时，他们会被投递给有订阅需求的broker。这个时候，会被brokerA投递给brokerB。
详细代码如下。
brokerB配置（brokerB.xml）：
```
<beans
  xmlns="http://www.springframework.org/schema/beans"
  xmlns:amq="http://activemq.org/config/1.0"
  xmlns:broker="http://activemq.apache.org/schema/core"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.0.xsd
  http://activemq.org/config/1.0 http://activemq.apache.org/schema/activemq-core.xsd
  http://activemq.apache.org/schema/core http://activemq.apache.org/schema/core/activemq-core-5.2.0.xsd
  http://activemq.apache.org/camel/schema/spring http://activemq.apache.org/camel/schema/spring/camel-spring.xsd">
  <!-- Allows us to use system properties as variables in this configuration file -->
  <bean class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer"/>
  
  <!-- 定义一个broker -->
  <broker xmlns="http://activemq.apache.org/schema/core" brokerName="BrokerB" dataDirectory="${activemq.base}/data">
  
    <!-- The transport connectors ActiveMQ will listen to -->
    <transportConnectors>
       <transportConnector name="openwire" uri="tcp://localhost:61617" />
    </transportConnectors>
  </broker>
  
</beans>
```
brokerA配置（brokerA.xml）：
```
<beans
  xmlns="http://www.springframework.org/schema/beans"
  xmlns:amq="http://activemq.org/config/1.0"
  xmlns:broker="http://activemq.apache.org/schema/core"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.0.xsd
  http://activemq.org/config/1.0 http://activemq.apache.org/schema/activemq-core.xsd
  http://activemq.apache.org/schema/core http://activemq.apache.org/schema/core/activemq-core-5.2.0.xsd
  http://activemq.apache.org/camel/schema/spring http://activemq.apache.org/camel/schema/spring/camel-spring.xsd">
  <!-- Allows us to use system properties as variables in this configuration file -->
  <bean class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer"/>
  
  <broker xmlns="http://activemq.apache.org/schema/core" brokerName="BrokerA" dataDirectory="${activemq.base}/data">
  
    <!-- The transport connectors ActiveMQ will listen to -->
    <transportConnectors>
       <transportConnector name="openwire" uri="tcp://localhost:61616" />
    </transportConnectors>
    
    <!-- 定义一个network连接器，连接到其他broker -->
    <networkConnectors>
        <networkConnector uri="static:(tcp://localhost:61617)" />
    </networkConnectors>
  </broker>
  
</beans>
```
消息生产者（Publisher.java）：
```java
/**
 * XXX.com Inc.
 * Copyright (c) 2004-2015 All Rights Reserved.
 */
package com.test.SpringTest.activemqinaction.ch4;
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
/**
 * 消息产生者
 * 
 * @author jiangnan
 * @version $Id: Publisher.java, v 0.1 2015年7月4日 下午4:48:48 jiangnan Exp $
 */
public class Publisher {
    protected int                                MAX_DELTA_PERCENT = 1;
    protected Map<String, Double>                LAST_PRICES       = new Hashtable<String, Double>();
    protected static int                         count             = 10;
    protected static int                         total;
    protected static String                      brokerURL         = "tcp://localhost:61616";
    protected static transient ConnectionFactory factory;
    protected transient Connection               connection;
    protected transient Session                  session;
    protected transient MessageProducer          producer;
    public Publisher() throws JMSException {
        factory = new ActiveMQConnectionFactory(brokerURL);
        connection = factory.createConnection();
        connection.start();
        session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);
        producer = session.createProducer(null);
    }
    public void close() throws JMSException {
        if (connection != null) {
            connection.close();
        }
    }
    public static void main(String[] args) throws JMSException {
        String[] topics = { "topic1", "topic2" };
        Publisher publisher = new Publisher();
        while (total < 1000) {
            for (int i = 0; i < count; i++) {
                publisher.sendMessage(topics);
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
    protected void sendMessage(String[] stocks) throws JMSException {
        int idx = 0;
        while (true) {
            idx = (int) Math.round(stocks.length * Math.random());
            if (idx < stocks.length) {
                break;
            }
        }
        String stock = stocks[idx];
        Destination destination = session.createTopic("STOCKS." + stock);
        Message message = createStockMessage(stock, session);
        System.out.println("Sending: " + ((ActiveMQMapMessage) message).getContentMap()
                           + " on destination: " + destination);
        producer.send(destination, message);
    }
    protected Message createStockMessage(String stock, Session session) throws JMSException {
        Double value = LAST_PRICES.get(stock);
        if (value == null) {
            value = new Double(Math.random() * 100);
        }
        // lets mutate the value by some percentage
        double oldPrice = value.doubleValue();
        value = new Double(mutatePrice(oldPrice));
        LAST_PRICES.put(stock, value);
        double price = value.doubleValue();
        double offer = price * 1.001;
        boolean up = (price > oldPrice);
        MapMessage message = session.createMapMessage();
        message.setStringProperty("stock", stock);//设置消息的属性
        message.setString("stock", stock);
        message.setDouble("price", price);
        message.setDouble("offer", offer);
        message.setBoolean("up", up);
        return message;
    }
    protected double mutatePrice(double price) {
        double percentChange = (2 * Math.random() * MAX_DELTA_PERCENT) - MAX_DELTA_PERCENT;
        return price * (100 + percentChange) / 100;
    }
}
```
消息订阅者（Consumer.java）:
```java
/**
 * XXX.com Inc.
 * Copyright (c) 2004-2015 All Rights Reserved.
 */
package com.test.SpringTest.activemqinaction.ch4;
import javax.jms.Connection;
import javax.jms.ConnectionFactory;
import javax.jms.Destination;
import javax.jms.JMSException;
import javax.jms.MessageConsumer;
import javax.jms.Session;
import org.apache.activemq.ActiveMQConnectionFactory;
/**
 * 消息消费者
 * 
 * @author jiangnan
 * @version $Id: Consumer.java, v 0.1 2015年7月4日 下午4:37:48 jiangnan Exp $
 */
public class Consumer {
    private static String                      brokerURL = "tcp://localhost:61617";
    private static transient ConnectionFactory factory;
    private transient Connection               connection;
    private transient Session                  session;
    public Consumer() throws JMSException {
        factory = new ActiveMQConnectionFactory(brokerURL);
        connection = factory.createConnection();
        connection.start();
        session = connection.createSession(false, Session.AUTO_ACKNOWLEDGE);
    }
    public void close() throws JMSException {
        if (connection != null) {
            connection.close();
        }
    }
    public static void main(String[] args) throws JMSException {
        String[] topics = { "topic1", "topic2" };
        Consumer consumer = new Consumer();
        for (String stock : topics) {
            Destination destination = consumer.getSession().createTopic("STOCKS." + stock);
            //只接收部分消息的选择器
            String selector = "stock = 'topic1'";
            MessageConsumer messageConsumer = consumer.getSession().createConsumer(destination,
                selector);
            messageConsumer.setMessageListener(new Listener());
        }
    }
    public Session getSession() {
        return session;
    }
}
```
消息监听器（Listener.java）：
```java
/**
 * XXX.com Inc.
 * Copyright (c) 2004-2015 All Rights Reserved.
 */
package com.test.SpringTest.activemqinaction.ch4;
import java.text.DecimalFormat;
import javax.jms.MapMessage;
import javax.jms.Message;
import javax.jms.MessageListener;
/**
 * 消息监听器
 * 
 * @author jiangnan
 * @version $Id: Listener.java, v 0.1 2015年7月4日 下午4:40:25 jiangnan Exp $
 */
public class Listener implements MessageListener {
    public void onMessage(Message message) {
        try {
            MapMessage map = (MapMessage) message;
            String stock = map.getString("stock");
            double price = map.getDouble("price");
            double offer = map.getDouble("offer");
            boolean up = map.getBoolean("up");
            DecimalFormat df = new DecimalFormat("#,###,###,##0.00");
            System.out.println(stock + "\t" + df.format(price) + "\t" + df.format(offer) + "\t"
                               + (up ? "up" : "down"));
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
**运行过程：**
先启动brokerB：
D:\apache-activemq-5.8.0\bin>activemq xbean:file:D:/code/test/SpringTest/src/main/resource/META-INF/spring/activemqinaction/ch4/brokerB.xml
再启动brokerA：
D:\apache-activemq-5.8.0\bin>activemq xbean:file:D:/code/test/SpringTest/src/main/resource/META-INF/spring/activemqinaction/ch4/brokerA.xml
可以看到brokerA和brokerB建立连接成功。
![](https://img-blog.csdn.net/20150726154244063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后启动消费者和生产者。在控制台可以观察到消息发送和接收的日志。
#### Static protocol的使用场景
考虑这样一种场景，多个远程客户端与本地的一个broker建立连接。考虑到每个远程区域的客户端数量，与本地broker关联的连接数会很多。这会给网络带来不必要的负担。为了减小连接数，可以在每个远程区域设置一个broker，然后在远程broker和本地broker之间建立静态链接。这不仅会减小网络连接数，也会提高客户端工作效率。同时也会减少延时，降低等待客户端的时间。
### Failover connector
#### 概念介绍
在之前的例子中，客户端仅仅连接到一个特定的broker。如果连接失败或中断，怎么办？有两个选择：客户端会消亡，或者是重新连接到这个broker或者其他broker然后恢复工作。failover可以实现自动重连。有两种方式可以为客户端提供可以连接的broker，一是提供一个静态列表，二是使用动态发现机制。
静态列表配置格式如下：
failover:(uri1,...,uriN)?key=value
或者
failover:uri1,...,uriN
默认情况下，会随机挑选可使用的connector。如果连接失败，会挑选另一个URI尝试建立连接。默认配置实现了重连延迟逻辑：第一次重试失败后延迟10ms，之后延迟时间都在前一次的时间之上加倍，直至30000ms。
**使用场景**
强烈推荐为所有客户端使用failover，即时客户端只会连接到一个broker。这样做的好处是，broker挂掉之后不用手动重新连接，broker恢复后客户端会自动进行重连。简单的利用ActiveMQ的这一特性可以使应用更加稳定。
### 参考资料：
《ActiveMQ in Action》
### 附:demo的pom.xml配置
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>com.test</groupId>
  <artifactId>SpringTest</artifactId>
  <version>0.0.1-SNAPSHOT</version>
  <packaging>jar</packaging>
  <name>SpringTest</name>
  <url>http://maven.apache.org</url>
  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <org.springframework.version>3.1.2.RELEASE</org.springframework.version>
  </properties>
  <dependencies>
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.11</version>
      <scope>test</scope>
    </dependency>
    
    <!-- JMX -->
    <dependency>
		<groupId>com.sun.jdmk</groupId>
		<artifactId>jmxtools</artifactId>
		<version>1.2.1</version>
	</dependency>
	
	<!-- ActiveMQ -->
	<dependency>
	  <groupId>org.apache.activemq</groupId>
	  <artifactId>activemq-all</artifactId>
	  <version>5.6.0</version>
	</dependency>
	
	<dependency>
	  <groupId>org.apache.activemq</groupId>
	  <artifactId>activemq-core</artifactId>
	  <version>5.5.0</version>
	</dependency>
	
	<!-- xbean (ActiveMQ dependency) -->
	<dependency>
	  <groupId>org.apache.xbean</groupId>
	  <artifactId>xbean-spring</artifactId>
	  <version>3.7</version>
	</dependency>
    
    <!-- Spring -->
	<!--
	    Core utilities used by other modules.
	    Define this if you use Spring Utility APIs (org.springframework.core.*/org.springframework.util.*)
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-core</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    Expression Language (depends on spring-core)
	    Define this if you use Spring Expression APIs (org.springframework.expression.*)
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-expression</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!-- 
	    Bean Factory and JavaBeans utilities (depends on spring-core)
	    Define this if you use Spring Bean APIs (org.springframework.beans.*) 
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-beans</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    Aspect Oriented Programming (AOP) Framework (depends on spring-core, spring-beans)
	    Define this if you use Spring AOP APIs (org.springframework.aop.*)
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-aop</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    Application Context (depends on spring-core, spring-expression, spring-aop, spring-beans) 
	    This is the central artifact for Spring's Dependency Injection Container and is generally always defined
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-context</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    Various Application Context utilities, including EhCache, JavaMail, Quartz, and Freemarker integration
	    Define this if you need any of these integrations
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-context-support</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    Transaction Management Abstraction (depends on spring-core, spring-beans, spring-aop, spring-context)
	    Define this if you use Spring Transactions or DAO Exception Hierarchy
	    (org.springframework.transaction.*/org.springframework.dao.*)
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-tx</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    JDBC Data Access Library (depends on spring-core, spring-beans, spring-context, spring-tx)
	    Define this if you use Spring's JdbcTemplate API (org.springframework.jdbc.*)
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-jdbc</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    Object-to-Relation-Mapping (ORM) integration with Hibernate, JPA, and iBatis.
	    (depends on spring-core, spring-beans, spring-context, spring-tx)
	    Define this if you need ORM (org.springframework.orm.*)
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-orm</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    Object-to-XML Mapping (OXM) abstraction and integration with JAXB, JiBX, Castor, XStream, and XML Beans.
	    (depends on spring-core, spring-beans, spring-context)
	    Define this if you need OXM (org.springframework.oxm.*)
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-oxm</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    Web application development utilities applicable to both Servlet and Portlet Environments
	    (depends on spring-core, spring-beans, spring-context)
	    Define this if you use Spring MVC, or wish to use Struts, JSF, or another web framework with Spring (org.springframework.web.*)
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-web</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    Spring MVC for Servlet Environments (depends on spring-core, spring-beans, spring-context, spring-web)
	    Define this if you use Spring MVC with a Servlet Container such as Apache Tomcat (org.springframework.web.servlet.*)
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-webmvc</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    Spring MVC for Portlet Environments (depends on spring-core, spring-beans, spring-context, spring-web)
	    Define this if you use Spring MVC with a Portlet Container (org.springframework.web.portlet.*)
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-webmvc-portlet</artifactId>
	  <version>${org.springframework.version}</version>
	</dependency>
	<!--
	    Support for testing Spring applications with tools such as JUnit and TestNG
	    This artifact is generally always defined with a 'test' scope for the integration testing framework and unit testing stubs
	-->
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-test</artifactId>
	  <version>${org.springframework.version}</version>
	  <scope>test</scope>
	</dependency>
	
	<dependency>
	  <groupId>org.springframework</groupId>
	  <artifactId>spring-jms</artifactId>
	  <version>3.1.1.RELEASE</version>
	</dependency>
    
  </dependencies>
</project>
```
