# ActiveMQ实现负载均衡+高可用部署方案 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年04月07日 13:08:52[boonya](https://me.csdn.net/boonya)阅读数：2936









#### 原文地址：[http://www.open-open.com/lib/view/open1400126457817.html](http://www.open-open.com/lib/view/open1400126457817.html)

#### 一、架构和技术介绍

##### 1、简介

ActiveMQ 
是Apache出品，最流行的，能力强劲的开源消息总线。完全支持JMS1.1和J2EE 1.4规范的 JMS Provider实现

##### 2、activemq的特性

1. 多种语言和协议编写客户端。语言: Java, C, C++, C#, Ruby, Perl, Python, PHP。应用协议: OpenWire,Stomp REST,WS Notification,XMPP,AMQP

2. 完全支持JMS1.1和J2EE 1.4规范 (持久化,XA消息,事务)

3. 对Spring的支持,ActiveMQ可以很容易内嵌到使用Spring的系统里面去,而且也支持Spring2.0的特性

4. 通过了常见J2EE服务器(如 Geronimo,JBoss 4, GlassFish,WebLogic)的测试,其中通过JCA
 1.5 resourceadaptors的配置,可以让ActiveMQ可以自动的部署到任何兼容J2EE1.4商业服务器上

5. 支持多种传送协议:in-VM,TCP,SSL,NIO,UDP,JGroups,JXTA

6. 支持通过JDBC和journal提供高速的消息持久化

7. 从设计上保证了高性能的集群,客户端-服务器,点对点

8. 支持Ajax

9. 支持与Axis的整合

10. 可以很容易得调用内嵌JMS provider,进行测试

##### 3、下载和安装ActiveMQ

1、下载

ActiveMQ的最新版本是5.10.0，但由于我们内网下载存在问题，所以目前通过内网只能下载到5.9.0，下载地址：[http://activemq.apache.org/activemq-590-release.html](http://activemq.apache.org/activemq-590-release.html)。

2、安装


如果是在windows系统中运行，可以直接解压apache-activemq-5.9.0-bin.zip，并运行bin目录下的activemq.bat文件，此时使用的是默认的服务端口：61616和默认的console端口：8161。


如果是在linux或unix下运行，在bin目录下执行命令：./activemq setup

3、修改ActiveMQ的服务端口和console端口

         A、修改服务端口：打开conf/activemq.xml文件，修改以下红色字体部分



```
<span style="font-size:12px;">    <transportConnectors>

           <transportConnector name="openwire" uri="tcp://10.42.220.72:61618"discoveryUri="multicast://default"/>

       </transportConnectors></span>
```





B、修改console的地址和端口:打开conf/jetty.xml文件，修改以下红色字体部分

```
<bean id="jettyPort"class="org.apache.activemq.web.WebConsolePort"init-method="start">

       <property name="port" value="8162"/>

 </bean>
```



##### 4、通过客户端代码试用ActiveMQ

        需要提前将activemq解压包中的lib目录下的相关包引入到工程中，再进行如下编码：

**1、发送端的代码：**

```java
importjavax.jms.Connection;

importjavax.jms.ConnectionFactory;

importjavax.jms.DeliveryMode;

importjavax.jms.Destination;

importjavax.jms.MessageProducer;

importjavax.jms.Session;

importjavax.jms.TextMessage;

importorg.apache.activemq.ActiveMQConnection;

importorg.apache.activemq.ActiveMQConnectionFactory;

publicclass Sender {

    privatestaticfinalintSEND_NUMBER = 5;

 

    publicstaticvoid main(String[] args) {

        // ConnectionFactory：连接工厂，JMS用它创建连接

        ConnectionFactory connectionFactory;

        // Connection：JMS客户端到JMS Provider的连接

        Connection connection = null;

        // Session：一个发送或接收消息的线程

        Session session;

        // Destination：消息的目的地;消息发送给谁.

        Destination destination;

        // MessageProducer：消息发送者

        MessageProducer producer;

        // TextMessage message;

        //构造ConnectionFactory实例对象，此处采用ActiveMq的实现jar

        connectionFactory = new ActiveMQConnectionFactory(

                ActiveMQConnection.DEFAULT_USER,

                ActiveMQConnection.DEFAULT_PASSWORD,

                "failover:(tcp://10.42.220.72:61617,tcp://10.42.220.72:61618)");

        try {

            //构造从工厂得到连接对象

            connection =connectionFactory.createConnection();

            //启动

            connection.start();

            //获取操作连接

            session = connection.createSession(true, Session.AUTO_ACKNOWLEDGE);

            //获取session

            destination = session.createQueue("FirstQueue");

            //得到消息生成者【发送者】

            producer =session.createProducer(destination);

            //设置不持久化，此处学习，实际根据项目决定

           producer.setDeliveryMode(DeliveryMode.NON_PERSISTENT);

            //构造消息，此处写死，项目就是参数，或者方法获取

            sendMessage(session, producer);

            session.commit();

        } catch (Exception e) {

            e.printStackTrace();

        } finally {

            try {

                if (null != connection)

                    connection.close();

            } catch (Throwable ignore) {

            }

        }

        }

   

    publicstaticvoid sendMessage(Session session,MessageProducer producer)

            throws Exception {

        for (int i = 1; i <=SEND_NUMBER; i++) {

            TextMessage message = session

                    .createTextMessage("ActiveMq发送的消息" + i);

            //发送消息到目的地方

            System.out.println("发送消息：" + "ActiveMq 发送的消息" + i);

            producer.send(message);

        }

    }

}
```



**2、接收端代码：**

```java
importjavax.jms.Connection;

importjavax.jms.ConnectionFactory;

importjavax.jms.Destination;

importjavax.jms.MessageConsumer;

importjavax.jms.Session;

importjavax.jms.TextMessage;

importorg.apache.activemq.ActiveMQConnection;

importorg.apache.activemq.ActiveMQConnectionFactory;

 

publicclass Receive {

    publicstaticvoid main(String[] args) {

        // ConnectionFactory：连接工厂，JMS用它创建连接

        ConnectionFactory connectionFactory;

        // Connection：JMS客户端到JMS Provider的连接

        Connection connection = null;

        // Session：一个发送或接收消息的线程

        Session session;

        // Destination：消息的目的地;消息发送给谁.

        Destination destination;

        //消费者，消息接收者

        MessageConsumer consumer;

        connectionFactory = new ActiveMQConnectionFactory(

                ActiveMQConnection.DEFAULT_USER,

                ActiveMQConnection.DEFAULT_PASSWORD,

                "failover:(tcp://10.42.220.72:61617,tcp://10.42.220.72:61618)");

        try {

            //构造从工厂得到连接对象

            connection =connectionFactory.createConnection();

            //启动

            connection.start();

            //获取操作连接

            session = connection.createSession(false,

                    Session.AUTO_ACKNOWLEDGE);

            //获取session

            destination = session.createQueue("FirstQueue");

            consumer =session.createConsumer(destination);

            while (true) {

                //设置接收者接收消息的时间，为了便于测试，这里谁定为100s

                TextMessage message =(TextMessage) consumer.receive(100000);

                if (null != message) {

                    System.out.println("收到消息" + message.getText());

                } else {

                    break;

                }

            }

        } catch (Exception e) {

            e.printStackTrace();

        } finally {

            try {

                if (null != connection)

                    connection.close();

            } catch (Throwable ignore) {

            }

        }

    }

}
```



3、通过监控查看消息堆栈的记录：

登陆[http://localhost:8162/admin/queues.jsp](http://localhost:8162/admin/queues.jsp)，默认的用户名和密码：admin/admin

![p1.png](http://static.open-open.com/lib/uploadImg/20140515/20140515120044_198.png)

#### 二、ActiveMQ的多种部署方式


单点的ActiveMQ作为企业应用无法满足高可用和集群的需求，所以ActiveMQ提供了master-slave、broker cluster等多种部署方式，但通过分析多种部署方式之后我认为需要将两种部署方式相结合才能满足我们公司分布式和高可用的需求，所以后面就重点将解如何将两种部署方式相结合。

##### 1、Master-Slave部署方式

###### 1）shared filesystem Master-Slave部署方式

主要是通过共享存储目录来实现master和slave的热备，所有的ActiveMQ应用都在不断地获取共享目录的控制权，哪个应用抢到了控制权，它就成为master。

多个共享存储目录的应用，谁先启动，谁就可以最早取得共享目录的控制权成为master，其他的应用就只能作为slave。

![p2.png](http://static.open-open.com/lib/uploadImg/20140515/20140515120001_591.png)

###### 2）shared database Master-Slave方式

与shared filesystem方式类似，只是共享的存储介质由文件系统改成了数据库而已。

###### 3）Replicated LevelDB Store方式

这种主备方式是ActiveMQ5.9以后才新增的特性，使用ZooKeeper协调选择一个node作为master。被选择的master
 broker node开启并接受客户端连接。

其他node转入slave模式，连接master并同步他们的存储状态。slave不接受客户端连接。所有的存储操作都将被复制到连接至Master的slaves。

如果master死了，得到了最新更新的slave被允许成为master。fialed node能够重新加入到网络中并连接master进入slave
 mode。所有需要同步的disk的消息操作都将等待存储状态被复制到其他法定节点的操作完成才能完成。所以，如果你配置了replicas=3，那么法定大小是(3/2)+1=2. Master将会存储并更新然后等待
 (2-1)=1个slave存储和更新完成，才汇报success。至于为什么是2-1，熟悉Zookeeper的应该知道，有一个node要作为观擦者存在。

单一个新的master被选中，你需要至少保障一个法定node在线以能够找到拥有最新状态的node。这个node将会成为新的master。因此，推荐运行至少3个replica
 nodes，以防止一个node失败了，服务中断。

![p3.png](http://static.open-open.com/lib/uploadImg/20140515/20140515115951_604.png)

##### 2、Broker-Cluster部署方式

前面的Master-Slave的方式虽然能解决多服务热备的高可用问题，但无法解决负载均衡和分布式的问题。Broker-Cluster的部署方式就可以解决负载均衡的问题。

         Broker-Cluster部署方式中，各个broker通过网络互相连接，并共享queue。当broker-A上面指定的queue-A中接收到一个message处于pending状态，而此时没有consumer连接broker-A时。如果cluster中的broker-B上面由一个consumer在消费queue-A的消息，那么broker-B会先通过内部网络获取到broker-A上面的message，并通知自己的consumer来消费。

###### 1）static Broker-Cluster部署

在activemq.xml文件中静态指定Broker需要建立桥连接的其他Broker：

1、
首先在Broker-A节点中添加networkConnector节点：

```
<networkConnectors> 
       <networkConnector   uri="static:(tcp:// 0.0.0.0:61617)"duplex="false"/>
</networkConnectors>
```


2、
修改Broker-A节点中的服务提供端口为61616：

```
<transportConnectors>
         <transportConnectorname="openwire"uri="tcp://0.0.0.0:61616?maximumConnections=1000&wireFormat.maxFrameSize=104857600"/>
</transportConnectors>
```


3、
在Broker-B节点中添加networkConnector节点：

```
<networkConnectors> 
         <networkConnector   uri="static:(tcp:// 0.0.0.0:61616)"duplex="false"/>
</networkConnectors>
```


4、
修改Broker-A节点中的服务提供端口为61617：

```
<transportConnectors>
         <transportConnectorname="openwire"uri="tcp://0.0.0.0:61617?maximumConnections=1000&wireFormat.maxFrameSize=104857600"/>
</transportConnectors>
```


5、分别启动Broker-A和Broker-B。

###### 2）Dynamic Broker-Cluster部署

在activemq.xml文件中不直接指定Broker需要建立桥连接的其他Broker，由activemq在启动后动态查找：

1、
首先在Broker-A节点中添加networkConnector节点：

```
<networkConnectors> 
       <networkConnectoruri="multicast://default"

           dynamicOnly="true"

           networkTTL="3"

           prefetchSize="1"

           decreaseNetworkConsumerPriority="true" />

</networkConnectors>
```


2、修改Broker-A节点中的服务提供端口为61616：

```
<transportConnectors>
         <transportConnectorname="openwire"uri="tcp://0.0.0.0:61616? " discoveryUri="multicast://default"/>
</transportConnectors>
```


3、在Broker-B节点中添加networkConnector节点：

```
<networkConnectors> 
      <networkConnectoruri="multicast://default"

           dynamicOnly="true"

           networkTTL="3"

           prefetchSize="1"

           decreaseNetworkConsumerPriority="true" />

</networkConnectors>
```



4、修改Broker-B节点中的服务提供端口为61617：

```
<transportConnectors>
         <transportConnectorname="openwire"uri="tcp://0.0.0.0:61617" discoveryUri="multicast://default"/>
</transportConnectors>
```


5、启动Broker-A和Broker-B

##### 2、Master-Slave与Broker-Cluster相结合的部署方式

可以看到Master-Slave的部署方式虽然解决了高可用的问题，但不支持负载均衡，Broker-Cluster解决了负载均衡，但当其中一个Broker突然宕掉的话，那么存在于该Broker上处于Pending状态的message将会丢失，无法达到高可用的目的。

由于目前ActiveMQ官网上并没有一个明确的将两种部署方式相结合的部署方案，所以我尝试者把两者结合起来部署：

![p4.png](http://static.open-open.com/lib/uploadImg/20140515/20140515115919_992.png)

###### 1、部署的配置修改

这里以Broker-A + Broker-B建立cluster，Broker-C作为Broker-B的slave为例：

1）首先在Broker-A节点中添加networkConnector节点：

```
<networkConnectors> 
                <networkConnector   uri="masterslave:(tcp://0.0.0.0:61617,tcp:// 0.0.0.0:61618)" duplex="false"/>
</networkConnectors>
```


2）修改Broker-A节点中的服务提供端口为61616：

```
<transportConnectors>
         <transportConnectorname="openwire"uri="tcp://0.0.0.0:61616?maximumConnections=1000&wireFormat.maxFrameSize=104857600"/>
</transportConnectors>
```

3）在Broker-B节点中添加networkConnector节点：

```
<networkConnectors> 
          <networkConnector   uri="static:(tcp:// 0.0.0.0:61616)"duplex="false"/>
</networkConnectors>
```


4）修改Broker-B节点中的服务提供端口为61617：

```
<transportConnectors>
         <transportConnectorname="openwire"uri="tcp://0.0.0.0:61617?maximumConnections=1000&wireFormat.maxFrameSize=104857600"/>
</transportConnectors>
```



5）修改Broker-B节点中的持久化方式：

```
<persistenceAdapter>
           <kahaDB directory="/localhost/kahadb"/>
        </persistenceAdapter>
```

6）在Broker-C节点中添加networkConnector节点：

```
<networkConnectors> 
          <networkConnector   uri="static:(tcp:// 0.0.0.0:61616)"duplex="false"/>
</networkConnectors>
```

7）修改Broker-C节点中的服务提供端口为61618：

```
<transportConnectors>

         <transportConnectorname="openwire"uri="tcp://0.0.0.0:61618?maximumConnections=1000&wireFormat.maxFrameSize=104857600"/>

</transportConnectors>
```



8）修改Broker-B节点中的持久化方式：

```
<persistenceAdapter>

           <kahaDB directory="/localhost/kahadb"/>

       </persistenceAdapter>
```



9）分别启动broker-A、broker-B、broker-C，因为是broker-B先启动，所以“/localhost/kahadb”目录被lock住，broker-C将一直处于挂起状态，当人为停掉broker-B之后，broker-C将获取目录“/localhost/kahadb”的控制权，重新与broker-A组成cluster提供服务。




