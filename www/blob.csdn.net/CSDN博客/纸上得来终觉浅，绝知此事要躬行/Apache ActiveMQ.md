# Apache ActiveMQ - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年03月28日 15:59:57[boonya](https://me.csdn.net/boonya)阅读数：479标签：[apache																[activemq](https://so.csdn.net/so/search/s.do?q=activemq&t=blog)](https://so.csdn.net/so/search/s.do?q=apache&t=blog)
个人分类：[ActiveMQ](https://blog.csdn.net/boonya/article/category/1714191)







**一、概述与介绍**


    ActiveMQ 是Apache出品，最流行的、功能强大的即时通讯集成模式的开源服务器。ActiveMQ 是一个完全支持JMS1.1和J2EE 1.4规范的 JMS Provider实现。提供客户端支持跨语言和协议，带有易于在充分支持JMS 1.1和1.4使用J2EE企业集成模式和许多先进的功能。

**二、 特性**


    1、 多种语言和协议编写客户端。语言： Java、C、C++、C#、Ruby、Perl、Python、PHP。应用协议：OpenWire、Stomp REST、WS Notification、XMPP、AMQP


    2、完全支持JMS1.1和J2EE 1.4规范 （持久化，XA消息，事务)


    3、对Spring的支持，ActiveMQ可以很容易内嵌到使用Spring的系统里面去，而且也支持Spring2.0的特性


    4、通过了常见J2EE服务器（如 Geronimo、JBoss 4、GlassFish、WebLogic)的测试，其中通过JCA 1.5 resource adaptors的配置，可以让ActiveMQ可以自动的部署到任何兼容J2EE 1.4 商业服务器上


    5、支持多种传送协议：in-VM、TCP、SSL、NIO、UDP、JGroups、JXTA


    6、支持通过JDBC和journal提供高速的消息持久化


    7、从设计上保证了高性能的集群，客户端-服务器，点对点


    8、支持Ajax


    9、支持与Axis的整合


    10、可以很容易得调用内嵌JMS provider，进行测试
** 三、 消息示例**

ActiviteMQ消息有3中形式


|**JMS 公共**|**点对点域**|**发布/订阅域**|
|----|----|----|
|ConnectionFactory|QueueConnectionFactory|TopicConnectionFactory|
|Connection|QueueConnection|TopicConnection|
|Destination|Queue|Topic|
|Session|QueueSession|TopicSession|
|MessageProducer|QueueSender|TopicPublisher|
|MessageConsumer|QueueReceiver|TopicSubscriber|

(1)、点对点方式（point-to-point）


        点对点的消息发送方式主要建立在 Message Queue,Sender,reciever上，Message Queue 存贮消息，Sneder 发送消息，receive接收消息.具体点就是Sender Client发送Message Queue ,而 receiver Cliernt从Queue中接收消息和"发送消息已接受"到Quere,确认消息接收。消息发送客户端与接收客户端没有时间上的依赖，发送客户端可以在任何时刻发送信息到Queue，而不需要知道接收客户端是不是在运行


 (2)、发布/订阅 方式（publish/subscriber Messaging）


        发布/订阅方式用于多接收客户端的方式.作为发布订阅的方式，可能存在多个接收客户端，并且接收端客户端与发送客户端存在时间上的依赖。一个接收端只能接收他创建以后发送客户端发送的信息。作为subscriber ,在接收消息时有两种方法，destination的receive方法，和实现message listener 接口的onMessage 方法。


        2、ActiviteMQ接收和发送消息基本流程

![image](http://images.cnitblog.com/blog/151517/201306/27085728-78e9c08eff7d45b3b6d6d92fd7d50f23.png)

        发送消息的基本步骤：


        (1)、创建连接使用的工厂类JMS ConnectionFactory


        (2)、使用管理对象JMS ConnectionFactory建立连接Connection，并启动


        (3)、使用连接Connection 建立会话Session


        (4)、使用会话Session和管理对象Destination创建消息生产者MessageSender


        (5)、使用消息生产者MessageSender发送消息




        消息接收者从JMS接受消息的步骤


        (1)、创建连接使用的工厂类JMS ConnectionFactory


        (2)、使用管理对象JMS ConnectionFactory建立连接Connection，并启动


        (3)、使用连接Connection 建立会话Session


        (4)、使用会话Session和管理对象Destination创建消息接收者MessageReceiver


        (5)、使用消息接收者MessageReceiver接受消息，需要用setMessageListener将MessageListener接口绑定到MessageReceiver消息接收者必须实现了MessageListener接口，需要定义onMessage事件方法。

四、安装配置

1.安装很简单，下载[http://activemq.apache.org/](http://activemq.apache.org/)解压运行，网上很多这样的文章。

2.修改activemq.xml

![](https://img-blog.csdn.net/20160328155632729)


默认配置的地址是：*://0.0.0.0:xxxx，需要修改IP地址程序才能正常访问。

转载自：[http://www.cnblogs.com/hoojo/p/active_mq_jms_apache_activeMQ.html](http://www.cnblogs.com/hoojo/p/active_mq_jms_apache_activeMQ.html)






