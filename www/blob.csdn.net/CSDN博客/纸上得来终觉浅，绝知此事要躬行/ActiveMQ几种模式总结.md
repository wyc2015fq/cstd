# ActiveMQ几种模式总结 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年04月06日 14:25:29[boonya](https://me.csdn.net/boonya)阅读数：10936








原文地址：[http://blog.csdn.net/czp11210/article/details/8822070](http://blog.csdn.net/czp11210/article/details/8822070)


# Queue(点到点)模式

在点对点的传输方式中，消息数据被持久化，每条消息都能被消费，没有监听QUEUE地址也能被消费，数据不会丢失，一对一的发布接受策略，保证数据完整。

## 创建MAVEN项目

点击下一步，填写grupId为me.czp,artifactId为example-mq,name为exampleMq,点完成

然后在Package Explorer看到example-mq项目，如下：

## 创建生产者

```java
package mq.p2p;

import java.util.Date;

import javax.jms.Connection;

import javax.jms.ConnectionFactory;

import javax.jms.Destination;

import javax.jms.JMSException;

import javax.jms.MapMessage;

import javax.jms.MessageProducer;

import javax.jms.Session;

import org.apache.activemq.ActiveMQConnection;

importorg.apache.activemq.ActiveMQConnectionFactory;

public class Producer {

         publicstatic void main(String[] args) {

                   Stringuser = ActiveMQConnection.DEFAULT_USER;

                   Stringpassword = ActiveMQConnection.DEFAULT_PASSWORD;

                   Stringurl = ActiveMQConnection.DEFAULT_BROKER_URL;

                   Stringsubject = "test.queue";

                   ConnectionFactorycontectionFactory = new ActiveMQConnectionFactory( user, password, url);

                   try{

                            Connectionconnection = contectionFactory.createConnection();

                            connection.start();

                            Sessionsession = connection.createSession(Boolean.TRUE, Session.AUTO_ACKNOWLEDGE);

                            Destinationdestination = session.createQueue(subject);

                            MessageProducerproducer = session.createProducer(destination);

                            for(int i = 0; i <= 20; i++) {

                                     MapMessagemessage = session.createMapMessage();

                                     Datedate = new Date();

                                     message.setLong("count",date.getTime());

                                     Thread.sleep(1000);

                                     producer.send(message);

                                     System.out.println("--发送消息：" +date);

                            }

                            Thread.sleep(2000);

                            session.commit();

                            session.close();

                            connection.close();

                   }catch (JMSException e) {

                            e.printStackTrace();

                   }catch (InterruptedException e) {

                            e.printStackTrace();

                   }

         }

}
```



## 创建消费者

```java
package mq.p2p;

import java.util.Date;

import javax.jms.Connection;

import javax.jms.ConnectionFactory;

import javax.jms.Destination;

import javax.jms.JMSException;

import javax.jms.MapMessage;

import javax.jms.Message;

import javax.jms.MessageConsumer;

import javax.jms.MessageListener;

import javax.jms.Session;

import org.apache.activemq.ActiveMQConnection;

import org.apache.activemq.ActiveMQConnectionFactory;

 

public class Customer {

    public static void main(String[] args) {

        Stringuser = ActiveMQConnection.DEFAULT_USER;

        Stringpassword = ActiveMQConnection.DEFAULT_PASSWORD;

        Stringurl = ActiveMQConnection.DEFAULT_BROKER_URL;

        Stringsubject = "test.queue";

        ConnectionFactoryconnectionFactory = new ActiveMQConnectionFactory( user, password, url);

        Connectionconnection;

        try {

            connection= connectionFactory.createConnection();

            connection.start();

            final Session session =connection.createSession(Boolean.TRUE, Session.AUTO_ACKNOWLEDGE);

            Destinationdestination = session.createQueue(subject);

            MessageConsumermessage = session.createConsumer(destination);

            message.setMessageListener(new MessageListener() {

                publicvoid onMessage(Message msg){

                    MapMessagemessage = (MapMessage) msg;

                    try {

                        System.out.println("--收到消息：" +new Date());

                        session.commit();

                    }catch(JMSException e) {

                        e.printStackTrace();

                    }

                }

            });

            Thread.sleep(30000);

            session.close();

            Thread.sleep(30000);

            connection.close();

            Thread.sleep(30000);

        }catch(JMSException e) {

            e.printStackTrace();

        }catch(InterruptedException e) {

            e.printStackTrace();

        }

    }

}
```





```java
package mq.p2p;

 

import java.util.Date;

import javax.jms.Connection;

import javax.jms.ConnectionFactory;

import javax.jms.Destination;

import javax.jms.JMSException;

import javax.jms.MapMessage;

import javax.jms.Message;

import javax.jms.MessageConsumer;

import javax.jms.MessageListener;

import javax.jms.Session;

import org.apache.activemq.ActiveMQConnection;

import org.apache.activemq.ActiveMQConnectionFactory;

 

/*

 *第二个消费者

 */

public class Customer2 {

    publicstatic void main(String[] args) {

        Stringuser = ActiveMQConnection.DEFAULT_USER;

        Stringpassword = ActiveMQConnection.DEFAULT_PASSWORD;

        Stringurl = ActiveMQConnection.DEFAULT_BROKER_URL;

        Stringsubject = "test.queue";

        ConnectionFactoryconnectionFactory = new ActiveMQConnectionFactory( user, password, url);

        Connectionconnection;

        try{

            connection= connectionFactory.createConnection();

            connection.start();

            finalSession session = connection.createSession(Boolean.TRUE,Session.AUTO_ACKNOWLEDGE);

            Destinationdestination = session.createQueue(subject);

            MessageConsumermessage = session.createConsumer(destination);

            message.setMessageListener(newMessageListener() {

                publicvoid onMessage(Message msg) {

                    MapMessagemessage = (MapMessage) msg;

                    try{

                        System.out.println("--收到消息2："+ new Date(message.getLong("count")));

                        session.commit();

                    }catch (JMSException e) {

                        e.printStackTrace();

                    }

                }

            });

            Thread.sleep(30000);

            session.close();

            Thread.sleep(30000);

            connection.close();

            Thread.sleep(30000);

        }catch (JMSException e) {

            e.printStackTrace();

        }catch (InterruptedException e) {

            e.printStackTrace();

        }

    }

}
```


## 学习体会

1.      运行结果，一个消息只能由一个消费者消费，不能同时被多个消费者获取

2.      测试表明,消息消费者消费消息时机是队列没有被消息提供者锁住，也就是说只有消息提供者执行了session.close()后消费者才会执行onMessage()方法

3.      启动顺序没有要求，可以先启动消费者，再启动提供者，也可以先启动提供者，然后再启动消费者

# Topic（发布/订阅）模式

## 创建生产者

```java
package mq.topic;

import java.util.Date;

import javax.jms.Connection;

import javax.jms.Destination;

import javax.jms.MapMessage;

import javax.jms.MessageProducer;

import javax.jms.Session;

import org.apache.activemq.ActiveMQConnection;

import org.apache.activemq.ActiveMQConnectionFactory;

public classPublisher {

    public static void main(String[] arg){

        Stringuser = ActiveMQConnection.DEFAULT_USER;

        Stringpassword = ActiveMQConnection.DEFAULT_PASSWORD;

        Stringurl = "tcp://localhost:61616";

        Stringsubject = "mq.topic";

        ActiveMQConnectionFactory amcf = newActiveMQConnectionFactory(user, password, url);

        try {

            Connectionconn = amcf.createConnection();

            conn.start();

            Sessionsession = conn.createSession(Boolean.TRUE,Session.AUTO_ACKNOWLEDGE);

            Destinationd = session.createTopic(subject);

            MessageProducerproducer = session.createProducer(d);

            for (int i = 0; i <= 20; i++){

                MapMessagemessage = session.createMapMessage();

                Datedate = newDate();

                message.setLong("count",date.getTime());

                Thread.sleep(1000);

                producer.send(message);

                System.out.println("--发送消息：" + date);

            }

            session.commit();

            session.close();

            conn.close();

        }catch(Exception e) {

            e.printStackTrace();

        }

    }

}
```





## 创建消费者

```java
package mq.topic;

import java.util.Date;

import javax.jms.Connection;

import javax.jms.JMSException;

import javax.jms.MapMessage;

import javax.jms.Message;

import javax.jms.MessageConsumer;

import javax.jms.MessageListener;

import javax.jms.Session;

import javax.jms.Topic;

import org.apache.activemq.ActiveMQConnection;

import org.apache.activemq.ActiveMQConnectionFactory;

public class SubscriberFirst {

    public static void main(String[] args) {

        Stringuser = ActiveMQConnection.DEFAULT_USER;

        Stringpassword = ActiveMQConnection.DEFAULT_PASSWORD;

        Stringurl = "tcp://localhost:61616";

        Stringsubject = "mq.topic";

        ActiveMQConnectionFactoryfactory = newActiveMQConnectionFactory(user, password, url);

        Connectionconnection;

        try {

            connection= factory.createConnection();

            connection.start();

            final Session session =connection.createSession(Boolean.TRUE, Session.AUTO_ACKNOWLEDGE);

            Topictopic = session.createTopic(subject);

            MessageConsumerconsumer = session.createConsumer(topic);

            consumer.setMessageListener(new MessageListener() {

                publicvoid onMessage(Message msg){

                    MapMessagemessage = (MapMessage) msg;

                    try {

                        System.out.println("--订阅者一收到消息：" +new Date(message.getLong("count")));

                        session.commit();

                    }catch(JMSException e) {

                        e.printStackTrace();

                    }

                }

            });

        }catch(JMSException e) {

            e.printStackTrace();

        }

    }

}
```


```java
package mq.topic;

import java.util.Date;

import javax.jms.Connection;

import javax.jms.JMSException;

import javax.jms.MapMessage;

import javax.jms.Message;

import javax.jms.MessageConsumer;

import javax.jms.MessageListener;

import javax.jms.Session;

import javax.jms.Topic;

importorg.apache.activemq.ActiveMQConnection;

importorg.apache.activemq.ActiveMQConnectionFactory;

public class SubscriberSecond {

 

         publicstatic void main(String[] args) {

                   Stringuser = ActiveMQConnection.DEFAULT_USER;

                   Stringpassword = ActiveMQConnection.DEFAULT_PASSWORD;

                   Stringurl = "tcp://localhost:61616";

                   Stringsubject = "mq.topic";

                   ActiveMQConnectionFactoryfactory = new ActiveMQConnectionFactory(user, password, url);

                   Connectionconnection;

                   try{

                            connection= factory.createConnection();

                            connection.start();

                            finalSession session = connection.createSession(Boolean.TRUE,Session.AUTO_ACKNOWLEDGE);

                            Topictopic = session.createTopic(subject);

                            MessageConsumerconsumer = session.createConsumer(topic);

                            consumer.setMessageListener(newMessageListener() {

                                     publicvoid onMessage(Message msg) {

                                               MapMessagemessage = (MapMessage) msg;

                                               try{

                                                        System.out.println("--订阅者二收到消息："+ new Date(message.getLong("count")));

                                                        session.commit();

                                               }catch (JMSException e) {

                                                        e.printStackTrace();

                                               }

                                     }

                            });

                   }catch (JMSException e) {

                            e.printStackTrace();

                   }

         }

}
```


## 学习体会

1.生产者和消费者之间有时间上的相关性。订阅一个主题的消费者只能消费自它订阅之后发布的消息。JMS 规范允许客户创建持久订阅，这在一定程度上放松了时间上的相关性要求。持久订阅允许消费者消费它在未处于激活状态时发送的消息。

2.订阅者一和订阅者二都能收到一样的消息，也就是说在发布订阅模式下，一份消息可以被多个消费者消费

# 消息相关概念

## 消息类型

JMS 消息由以下三部分组成：

 消息头。每个消息头字段都有相应的getter 和setter 方法。

 消息属性。如果需要除消息头字段以外的值，那么可以使用消息属性。

 消息体。消息主题内容,JMS定义消息包括TextMessage、MapMessage、BytesMessage、StreamMessage、ObjectMessage类型

## 消息确认

JMS 消息只有在被确认之后，才认为已经被成功地消费了。消息的成功消费通

常包含三个阶段：客户接收消息、客户处理消息和消息被确认。

在事务性会话中，当一个事务被提交的时候，确认自动发生。在非事务性会

话中，消息何时被确认取决于创建会话时的应答模式（acknowledgementmode）。

该参数有以下三个可选值：

Session.AUTO_ACKNOWLEDGE。当客户成功的从receive方法返回的时候，

或者从MessageListener.onMessage 方法成功返回的时候，会话自动确认

客户收到的消息。

 Session.CLIENT_ACKNOWLEDGE。客户通过消息的acknowledge 方法确认消

息。需要注意的是，在这种模式中，确认是在会话层上进行：确认一个被

消费的消息将自动确认所有已被会话消费的消息。例如，如果一个消息消

费者消费了10 个消息，然后确认第5 个消息，那么所有10 个消息都被确

认。

 Session.DUPS_ACKNOWLEDGE。该选择只是会话迟钝的确认消息的提交。如

果JMS provider 失败，那么可能会导致一些重复的消息。如果是重复的

消息，那么JMS provider 必须把消息头的JMSRedelivered 字段设置为

true。

## 消息持久化

**AMQ Message Store**

AMQ Message Store 是ActiveMQ5.0缺省的持久化存储。Message commands 被

保存到transactional journal（由rolling data logs 组成）。Messages 被保

存到data logs 中，同时被reference store 进行索引以提高存取速度。Date logs

由一些单独的data log 文件组成，缺省的文件大小是32M，如果某个消息的大

小超过了data log 文件的大小，那么可以修改配置以增加data log 文件的大小。

Generated by FoxitPDF Creator © Foxit Software

http://www.foxitsoftware.comFor evaluation only.

如果某个data log 文件中所有的消息都被成功消费了，那么这个data log 文件

将会被标记，以便在下一轮的清理中被删除或者归档。以下是其配置的一个例子：

Xml 代码

```
<brokerbrokerName="broker" persistent="true"useShutdownHook="

false">

<persistenceAdapter>

 <amqPersistenceAdapterdirectory="${activemq.base}/data" m

axFileLength="32mb"/>

</persistenceAdapter>

</broker>
```



**Kaha Persistence**

Kaha Persistence 是一个专门针对消息持久化的解决方案。它对典型的消息

使用模式进行了优化。在Kaha 中，数据被追加到data logs 中。当不再需要log

文件中的数据的时候，log 文件会被丢弃。以下是其配置的一个例子：

Xml 代码

```
<brokerbrokerName="broker" persistent="true"useShutdownHook="

false">

 <persistenceAdapter>

<kahaPersistenceAdapterdirectory="activemq-data" maxDa

taFileLength="33554432"/>

 </persistenceAdapter>

 </broker>
```



**JDBC Persistence**



目前支持的数据库有Apache Derby,Axion, DB2, HSQL, Informix, MaxDB,

MySQL, Oracle,Postgresql, SQLServer, Sybase。

如果你使用的数据库不被支持，那么可以调整StatementProvider来保证使

用正确的SQL 方言（flavour of SQL）。通常绝大多数数据库支持以下adaptor：

org.activemq.store.jdbc.adapter.BlobJDBCAdapter

org.activemq.store.jdbc.adapter.BytesJDBCAdapter

org.activemq.store.jdbc.adapter.DefaultJDBCAdapter

 org.activemq.store.jdbc.adapter.ImageJDBCAdapter

Generated by FoxitPDF Creator © Foxit Software

http://www.foxitsoftware.comFor evaluation only.

也可以在配置文件中直接指定JDBC adaptor，例如：

Xml 代码


`<jdbcPersistenceAdapter adapterClass="org.apache.activemq.store.jdbc.adapter.ImageBasedJDBCAdaptor"/>`


以下是其配置的一个例子：

Xml 代码

```
<persistence>

 <jdbcPersistence dataSourceRef="mysql-ds"/>

 </persistence>

 <bean id="mysql-ds"class="org.apache.commons.dbcp.BasicDataSource"destroy-method="close">

 <property name="driverClassName"value="com.mysql.jdbc.Driver"/>

 <property name="url" value="jdbc:mysql://localhost/activemq?relaxAutoCommit=true"/>

<propertyname="username" value="activemq"/>

 <property name="password"value="activemq"/>

 <propertyname="poolPreparedStatements" value="true"/>

</bean>
```



需要注意的是，如果使用MySQL，那么需要设置relaxAutoCommit 标志为true。

# ActiveMQ集群

## Broker集群

一个常见的场景是有多个JMS broker，有一个客户连接到其中一个broker。如果这个broker失效，那么客户会自动重新连接到其它的broker。在ActiveMQ中使用failover://协议来实现这个功能。如果某个网络上有多个brokers而且客户使用静态发现（使用Static
 Transport或Failover Transport）或动态发现（使用Discovery
 Transport），那么客户可以容易地在某个broker失效的情况下切换到其它的brokers。当有多个broker时，如果某个broker上没有consumers，那么这个broker上的消息可能会因得不到处理而积压起来。目前的解决方案是使用Network
 of brokers，以便在broker之间存储转发消息。



### Static-broker1的配置

```
<broker   xmlns="http://activemq.apache.org/schema/core"brokerName="static-broker1"dataDirectory="${activemq.base}/data"> 

       <networkConnectors>

                     <networkConnectoruri="static:(tcp://localhost:7777)"

                           name="bridge"

                           dynamicOnly="false"

                           conduitSubscriptions="true"

                            decreaseNetworkConsumerPriority="false">

                     </networkConnector>

        </networkConnectors>

       <persistenceAdapter>

           <kahaDBdirectory="${activemq.base}/data/static-broker1/kahadb" />

       </persistenceAdapter>

        <transportConnectors>

           <transportConnector name="openwire" uri="tcp://0.0.0.0:6666"/>

       </transportConnectors>       

   </broker>
```



### Static-broker2的配置



```
<broker  xmlns="http://activemq.apache.org/schema/core"  brokerName="static-broker2"dataDirectory="${activemq.base}/data">

          <networkConnectors>

                    <networkConnector uri="static:(tcp://localhost:6666)"

                           name="bridge"

                           dynamicOnly="false"

                           conduitSubscriptions="true"

                           decreaseNetworkConsumerPriority="false">

                    </networkConnector>

       </networkConnectors>

        <persistenceAdapter>

            <kahaDBdirectory="${activemq.base}/data/static-broker2/kahadb" />

        </persistenceAdapter>      

        <transportConnectors>

            <transportConnectorname="openwire" uri="tcp://0.0.0.0:7777"/>

        </transportConnectors>      

</broker>
```



现在分别启动static-broker1与static-broker2两个消息中间实例

然后在在应用层编写一个消息提供者和一个消息消费者，提供者对static-broker1里的队列test.queue写5笔消息，然后消费者从static-broker2里面的队列test.queue读数据，测试证明是可以读到那五笔记录的

具体见sample-mq例子工程代码包mq.bloker.cluster下面的代码



### 学习体会

1.  networkConnectors网络链接并不是把static-broker1里面的队列消息复制给static-broker2，而是static-broker2上的消费者访问test.queue上的消息时，当static-broker2上面没有这个队列时才会去static-broker1里面去找test.queue上面的消息,

2. static-broker1队列里面的消息一旦被static-broker2上面消费者消费后，static-broker1上面的消费者就没有了，说明一个队列在集群里面消息只存在一份，没有任何副本消息

3. 当static-broker1与static-broker2上面都有队列test.queue,而且他们各自都有五条消息，在这时如果static-broker2有一个消费者去消费，它是先消费static-broker2上面五条，然后再消费static-broker1上面的五条

## 主从集群

### Share Nothing Master/Slave

  Master 收到消息后保证先转发给slave后再发给客户端，slave有自己独立存储, 在slave配置文件中需指定Master URI， 这样当Master宕掉后，客户端会Failover到Slave.

#### 配置说明

Masterbloker无需其他配置，Slavebloker配置如下：

```
<services> 

  <masterConnector remoteURI="tcp://localhost:62001" userName="user"

password="password"/>

</services>
```



其中如果broker无需认证的话，用户名、密码就可以不配置了 

#### 缺陷总结

1.Master bloker只能配置一个slaver bloker

2.如果主MQ发生故障，要手动停止所有服务，手动停止slave，复制slave数据文件到master，然后手动启动master




注：该特性在5.8版本废弃掉了，masterConnector属性已经无效

Attribute 'masterConnectorURI' is not allowed to appear in element'broker'.



### Share Database Master/Slave

        和 Share File System Master/Slave 类似，只不过用数据库锁代替了共享文件系统锁。



#### 1)文件配置

Activemq-jdbc-master.xml:

```
<brokerxmlns="http://activemq.apache.org/schema/core"brokerName="static-broker1">

         <persistenceAdapter>

       <jdbcPersistenceAdapter dataDirectory="activemq-data"dataSource="#mysql-ds"/>

   </persistenceAdapter>

        <transportConnectors>

           <transportConnector name="openwire"uri="tcp://0.0.0.0:6666"/>

       </transportConnectors>       

   </broker>

           <bean id="mysql-ds"class="org.apache.commons.dbcp.BasicDataSource"destroy-method="close">

   <property name="driverClassName"value="com.mysql.jdbc.Driver"/>

   <property name="url"value="jdbc:mysql://localhost/activemq?relaxAutoCommit=true"/>

     <property name="username" value="root"/>

   <property name="password" value="1229"/>

   <property name="poolPreparedStatements"value="true"/>

 </bean>
```



Activemq-jdbc-slave.xml:



```
<broker  xmlns="http://activemq.apache.org/schema/core"brokerName="static-broker2"  >

                   <persistenceAdapter>

        <jdbcPersistenceAdapterdataDirectory="activemq-data" dataSource="#mysql-ds"/>

    </persistenceAdapter>

        <transportConnectors>

            <transportConnectorname="openwire" uri="tcp://0.0.0.0:7777"/>

        </transportConnectors>      

    </broker>

  <bean  id="mysql-ds"  class="org.apache.commons.dbcp.BasicDataSource"  destroy-method="close">

    <propertyname="driverClassName" value="com.mysql.jdbc.Driver"/>

    <property name="url"value="jdbc:mysql://localhost/activemq?relaxAutoCommit=true"/>

    <property name="username"value="root"/>

    <property name="password"value="1229"/>

    <propertyname="poolPreparedStatements" value="true"/>

  </bean>
```


#### 2)加载数据库驱动

把mssql-jtds-1.2.jar与mysql-connector-java-5.0.7-bin.jar数据库mysql的驱动包放入F:\apache-activemq-5.8.0\lib文件夹里

#### 3)启动主从两实例MQ

注意：如果是第一次启动时，会在数据库里生成存储消息所需要的三张表

#### 4)编写客户端

编写消息提供者-向master节点写消息

```java
package mq.broker.master.slave;

import java.util.Date;

import javax.jms.Connection;

import javax.jms.ConnectionFactory;

import javax.jms.Destination;

import javax.jms.JMSException;

import javax.jms.MapMessage;

import javax.jms.MessageProducer;

import javax.jms.Session;

import org.apache.activemq.ActiveMQConnection;

importorg.apache.activemq.ActiveMQConnectionFactory;

public class Producer {

         publicstatic void main(String[] args) {

                   Stringuser = ActiveMQConnection.DEFAULT_USER;

                   Stringpassword = ActiveMQConnection.DEFAULT_PASSWORD;

                   String url = "tcp://localhost:6666";

                   Stringsubject = "test.queue";

                   ConnectionFactorycontectionFactory = new ActiveMQConnectionFactory( user, password, url);

                   try{

                            Connectionconnection = contectionFactory.createConnection();

                            connection.start();

                            Sessionsession = connection.createSession(Boolean.TRUE, Session.AUTO_ACKNOWLEDGE);

                            Destinationdestination = session.createQueue(subject);

                            MessageProducerproducer = session.createProducer(destination);

                            for(int i = 0; i <= 5; i++) {

                                     MapMessagemessage = session.createMapMessage();

                                     Datedate = new Date();

                                     message.setLong("count",date.getTime());

                                     Thread.sleep(1000);

                                     producer.send(message);

                                     System.out.println("--发送消息：" +date);

                            }

                            session.commit();

                            session.close();

                            connection.close();

                   }catch (JMSException e) {

                            e.printStackTrace();

                   }catch (InterruptedException e) {

                            e.printStackTrace();

                   }

         }

}
```


**编写消息消费者-从slave节点拿消息进行消费**

```java
packagemq.broker.master.slave;

importjava.util.Date;

importjavax.jms.Connection;

importjavax.jms.ConnectionFactory;

importjavax.jms.Destination;

importjavax.jms.JMSException;

importjavax.jms.MapMessage;

importjavax.jms.Message;

importjavax.jms.MessageConsumer;

importjavax.jms.MessageListener;

importjavax.jms.Session;

importorg.apache.activemq.ActiveMQConnection;

importorg.apache.activemq.ActiveMQConnectionFactory;

publicclass Customer {

         public static void main(String[] args){

                   String user =ActiveMQConnection.DEFAULT_USER;

                   String password =ActiveMQConnection.DEFAULT_PASSWORD;

                  String url= "failover:(tcp://localhost:7777)";

                   String subject ="test.queue";

                   ConnectionFactoryconnectionFactory = new ActiveMQConnectionFactory( user, password, url);

                   Connection connection;

                   try {

                            connection =connectionFactory.createConnection();

                            connection.start();

                            final Sessionsession = connection.createSession(Boolean.TRUE, Session.AUTO_ACKNOWLEDGE);

                            Destinationdestination = session.createQueue(subject);

                            MessageConsumermessage = session.createConsumer(destination);

                            message.setMessageListener(newMessageListener() {

                                     public voidonMessage(Message msg) {

                                               MapMessagemessage = (MapMessage) msg;

                                               try{

                                                        System.out.println("--收到消息：" + newDate(message.getLong("count")));

                                                        session.commit();

                                               }catch (JMSException e) {

                                                        e.printStackTrace();

                                               }

                                     }

                            });

                            Thread.sleep(30000);

                            session.close();

                            Thread.sleep(30000);

                            connection.close();

                            Thread.sleep(30000);

                   } catch (JMSException e) {

                            e.printStackTrace();

                   } catch (InterruptedExceptione) {

                            e.printStackTrace();

                   }

         }

}
```



#### 5)学习体会

 1.存储消息的三张数据库表是在MQ实例第一次启动时生成，而非是写消息进去时生成，从这意义上讲，算是个“饿模式”，呵呵

 2.从master节点写消息，然后从slave读消息，是读不到消息的，只有当把master节点给关闭后，slave里面才能读到消息

 Share File System Master/Slave

       没有物理上的Master, Slave之分， 只有逻辑上的Master 和Slave, Master与Slave共享一个存储文件， 谁最先启动就会持有共享文件锁,成为Master, 后启动的由于获取文件锁时会被阻塞，成为Slave，当Master宕掉时，共享文件锁被释放掉，Slave获取的文件锁后就成为Master, 客户端使用failover 协议后会failover到可用的broker上。



后续完成，呵呵

# 具体见sample-mq例子工程

activeMQ性能优化、大数据量、安全控制后续补充了，现在没有时间，呵呵！



