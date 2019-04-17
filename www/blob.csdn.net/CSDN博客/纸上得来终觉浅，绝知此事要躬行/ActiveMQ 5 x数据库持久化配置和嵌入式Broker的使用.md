# ActiveMQ 5.x数据库持久化配置和嵌入式Broker的使用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年04月27日 16:18:21[boonya](https://me.csdn.net/boonya)阅读数：2848








### 1.持久化配置

ActiveMQ 5.x以上实现数据库持久化配置：



```
<beans
  xmlns="http://www.springframework.org/schema/beans"
  xmlns:amq="http://activemq.apache.org/schema/core"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.0.xsd
  http://activemq.apache.org/schema/core http://activemq.apache.org/schema/core/activemq-core.xsd">
  <bean class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer"/>
  <broker useJmx="true" xmlns="http://activemq.apache.org/schema/core">
    <networkConnectors>
      <!-- 
      <networkConnector uri="multicast://default?initialReconnectDelay=100" />
      <networkConnector uri="static://(tcp://localhost:61616)" />
       -->
    </networkConnectors>
    <persistenceFactory>
      <journalPersistenceAdapterFactory journalLogFiles="5" dataDirectory="${basedir}/target" />
       
      <!-- To use a different dataSource, use the following syntax : -->
      <!-- 
      <journalPersistenceAdapterFactory journalLogFiles="5" dataDirectory="${basedir}/activemq-data" dataSource="#mysql-ds"/>
       -->
    </persistenceFactory>
    <transportConnectors>
      <transportConnector uri="tcp://localhost:61636" />
    </transportConnectors>
  </broker>
  <!-- MySql DataSource Sample Setup -->
  <!-- 
  <bean id="mysql-ds" class="org.apache.commons.dbcp2.BasicDataSource" destroy-method="close">
    <property name="driverClassName" value="com.mysql.jdbc.Driver"/>
    <property name="url" value="jdbc:mysql://localhost/activemq?relaxAutoCommit=true"/>
    <property name="username" value="activemq"/>
    <property name="password" value="activemq"/>
    <property name="poolPreparedStatements" value="true"/>
  </bean>
  --> 
</beans>
```



From：[http://activemq.apache.org/persistence.html](http://activemq.apache.org/persistence.html)


### 2.Broker的使用

在许多消息拓扑有JMS代理(服务器端)和JMS客户端。通常是有意义的部署一个Broker在您的JVM。这允许您优化了网络跃点;使JMS作为高效的网络纯RMI,但通常的位置独立性JMS的特性,可靠性、负载平衡等。有多种方法嵌入一个Broker在ActiveMQ取决于如果您使用的是Java,Spring,XBean
 ActiveMQConnectionFactory或使用ActiveMQConnectionFactory。

下面的Java代码将创建一个嵌入式broker；



```java
BrokerService broker = new BrokerService();
 
// configure the broker
broker.addConnector("tcp://localhost:61616");
 
broker.start();
```

如果你想延迟绑定传输连接器的一部分start(),有用的时候start()将阻塞等待一个存储锁(如一个slave节点启动),您可以使用下面的代码；




```java
BrokerService broker = new BrokerService();
 
TransportConnector connector = new TransportConnector();
connector.setUri(new URI("tcp://localhost:61616"));
broker.addConnector(connector);
broker.start();
```

在同一个JVM客户可以使用vm:/ /transport连接到嵌入式broker,同时外部客户可以使用tcp://protocol
如果你有超过一个嵌入式代理,确保你给他们一个唯一的名称,如:




```java
BrokerService broker = new BrokerService();
// configure the broker
broker.setBrokerName("fred");
broker.addConnector("tcp://localhost:61616");
broker.start();
```
然后如果你想连接到代理名叫“fred”在相同的JVM中,您可以通过使用uri vm:/ /fred可以通过应用程序代码如完全配置代理;

```java
BrokerService broker = new BrokerService();
broker.setBrokerName("fred");
broker.setUseShutdownHook(false);
//Add plugin
broker.setPlugins(new BrokerPlugin[]{new JaasAuthenticationPlugin()});
//Add a network connection
NetworkConnector connector = answer.addNetworkConnector("static://"+"tcp://somehost:61616");
connector.setDuplex(true);
broker.addConnector("tcp://localhost:61616");
broker.start();
```

**BrokerFactory**




```java
BrokerService broker = BrokerFactory.createBroker(new URI(someURI));
```

|URI scheme|Example|Description|
|----|----|----|
|xbean:|xbean:activemq.xml|Searches the classpath (and file system) for an XML document with the given URI (activemq.xml in this case) which will then be used as the[Xml Configuration](http://activemq.apache.org/xml-configuration.html)|
|broker:|broker:tcp://localhost:61616|Uses the [Broker Configuration URI](http://activemq.apache.org/broker-configuration-uri.html) to confgure the broker|



**Spring**



```
<bean id="broker" class="org.apache.activemq.xbean.BrokerFactoryBean">
    <property name="config" value="classpath:org/apache/activemq/xbean/activemq.xml" />
    <property name="start" value="true" />
  </bean>
```

**XBean**




```
<beans 
  xmlns="http://www.springframework.org/schema/beans"
  xmlns:amq="http://activemq.apache.org/schema/core"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.0.xsd
  http://activemq.apache.org/schema/core http://activemq.apache.org/schema/core/activemq-core.xsd">
 
  <bean class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer"/>
 
  <broker useJmx="true" xmlns="http://activemq.apache.org/schema/core">
 
    <persistenceFactory>
      <kahaDB directory="${basedir}/target" />
    </persistenceFactory>
 
    <transportConnectors>
      <transportConnector uri="tcp://localhost:61636" />
    </transportConnectors>
 
  </broker>
</beans>
```
**Spring 2.0 Example**




```
<beans 
  xmlns="http://www.springframework.org/schema/beans"
  xmlns:amq="http://activemq.apache.org/schema/core"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.0.xsd
  http://activemq.apache.org/schema/core http://activemq.apache.org/schema/core/activemq-core.xsd">
   
  <!--  lets create an embedded ActiveMQ Broker -->
  <amq:broker useJmx="false" persistent="false">
    <amq:transportConnectors>
      <amq:transportConnector uri="tcp://localhost:0" />
    </amq:transportConnectors>
  </amq:broker>
 
   <!--  ActiveMQ destinations to use  -->
  <amq:queue id="destination"  physicalName="org.apache.activemq.spring.Test.spring.embedded"/>
 
  <!-- JMS ConnectionFactory to use, configuring the embedded broker using XML -->
  <amq:connectionFactory id="jmsFactory" brokerURL="vm://localhost"/>
   
 
  <!-- Spring JMS Template -->
  <bean id="myJmsTemplate" class="org.springframework.jms.core.JmsTemplate">
    <property name="connectionFactory">
      <!-- lets wrap in a pool to avoid creating a connection per send -->
      <bean class="org.springframework.jms.connection.SingleConnectionFactory">
        <property name="targetConnectionFactory">
          <ref local="jmsFactory" />
        </property>
      </bean>
    </property>
  </bean>
 
  <bean id="consumerJmsTemplate" class="org.springframework.jms.core.JmsTemplate">
    <property name="connectionFactory" ref="jmsFactory"/>
  </bean>
 
  <!-- a sample POJO which uses a Spring JmsTemplate -->
  <bean id="producer" class="org.apache.activemq.spring.SpringProducer">
    <property name="template">
      <ref bean="myJmsTemplate"></ref>
    </property>
 
    <property name="destination">
      <ref bean="destination" />
    </property>
 
    <property name="messageCount">
      <value>10</value>
    </property>
  </bean>
 
  <!-- a sample POJO consumer -->
  <bean id="consumer" class="org.apache.activemq.spring.SpringConsumer">
    <property name="template" ref="consumerJmsTemplate"/>
    <property name="destination" ref="destination"/>
  </bean>
 
</beans>
```

**ActiveMQConnectionFactory**


1.嵌入式broker也可以使用一个ActiveMQConnectionFactory创建和使用一个vm连接器作为一个uri:


```java
ActiveMQConnectionFactory cf = new ActiveMQConnectionFactory("vm://localhost?broker.persistent=false");
```
2.使用查询参数”broker.<property>”来配置代理,<property>匹配BrokerService
 bean属性。
broker将在创建第一个创建连接,你可以关掉自动创建通过设置在VM创建属性传输错误:




```java
ActiveMQConnectionFactory cf = new ActiveMQConnectionFactory("vm://localhost?create=false");
```


From:[http://activemq.apache.org/how-do-i-embed-a-broker-inside-a-connection.html](http://activemq.apache.org/how-do-i-embed-a-broker-inside-a-connection.html)
**更多经典文章参考：**[http://topmanopensource.iteye.com/category/64329](http://topmanopensource.iteye.com/category/64329)











