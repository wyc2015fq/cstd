# [jms那点事儿] JMS ( java message service )入门 + activemq消息持久化 - z69183787的专栏 - CSDN博客
2015年06月08日 22:37:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1128
JMS 即 java message service 是为java提供了一种创建、发送、接收消息的通用方法。可以将复杂的系统进行业务分离，变成灵活的高度解耦合的布局。同时对我们的日常业务需求开发，提供了非常灵活的业务解决方案。比如缴费还款送积分，送积分的业务逻辑不能影响到缴费还款的业务逻辑，所以最好的，就是缴费/还款逻辑执行完成之后，通过一种方式告诉积分系统，给用户发送积分，发送积分的结果不要影响到复杂的缴费还款的过程。这种情况下，采用jms进行异步处理，便是一个很好的选择。
要使用消息的方式来进行系统交互，我们需要一个消息中间平台，来进行消息的接受转发，同时处理复杂的消息持久化等问题。本文我们采用activemq来做实验。这样的架构下，我们的系统通常会变成如下架构：
 消息产生者 -> 消息中心 -> 消息消费者
**1、消息的两种传播方式**
JMS支持两种消息传播：PTP 和 PUB/SUB
PTP ： 点对点发送。消息的发送方将消息放入管道中，消息的接收方从管道中取出消息并处理。
PUB/SUB ： 发布/订阅方式。消息的发布者将自己的主题放入消息中心，同时进行消息投递，消息订阅者只获取自己订阅的消息。
jms为了支持上述两种模式，提供了两套针对同样接口的实现，对照关系如下：
![](http://hi.csdn.net/attachment/201101/11/7349002_1294717357byPS.jpg)
ConnectionFacatory：被管理的对象，由客户端（发布者/接受者）使用，用来创建一个链接。
Connection：提供一个JMS消息的活动链接。
Destination：封装了消息目的地，或者主题类型。
Session：一个用来发送和接受消息的线上上下文。
MessageProducer：由session创建的，用来发送消息的对象。
MessageConsumer：由session创建的用来接受消息的对象。
![](http://hi.csdn.net/attachment/201101/11/7349002_1294717593Msz9.jpg)
**2、jms消息模型**
Jms的消息分为三部分：消息头、消息属性、消息体
**消息头**：包含了消息的客户端和提供者用来路由和识别消息的数据。
 消息头包含的字段：
 JMSDestination：包含了消息发往的目的地或者主题信息。
 JMSDeliveryMode：消息传送模式。spring提供额jms模板提供了2种模式(有默认模式)：DEFAULT_DELEVERY_MODE 默认模式、DEFAULT_PRIORITY、DEFAULT_TIME_TO_LIVE
 JMSMessageID：消息标示，唯一性，每个消息都不同，即便是承载者相同消息体的消息。
 JMSTimestamp：发送时间
 JMSCorrelationID：与当前消息关联的其他消息的标示
 JMSReplyTo：回复消息的目的地。带有这样属性的消息通常是发送方希望有一个响应，这个响应是可选的。
 JMSRedelivered：带有该字段的消息通常过去发送过但是没有被确认，如果要再次发送，提供者必须设置该字段。如果true，则消息接受者必须进行消息重复处理的逻辑。
 JMSType：消息类型标示。官方文档的解释：
JMSType头字段包含了由客户端在发送消息时提供的消息类型标识。一些消息提供者使用消息库来存储由应用发送的消息定义。type头字段可以引用提供者库中的消息定义。JMS没有定义一个标准的消息定义库，也没有定义这个库中所包含的各种定义的命名策略。一些消息系统要求每个被创建的应用消息都必须有一个消息类型定义，并且每个消息都指定它的类型。为了能够使JMS工作于这些消息系统提供者，无论应用是否使用，JMS客户端最好赋值JMSType ，这样可以保证为需要该头字段的提供者提供了正确的设置。为了保证移植性，JMS客户端应使用安装时在提供者消息库中定义的语义值作为JMSType的值。 
 JMSExpiration ：过期时间。
 JMSPriority：优先级。
**消息属性**：包括了标准投资段之外的额外添加给消息的可选的字段。比如 应用指定的属性。
**消息体**：消息携带的内容。
**3、消息传输编程步骤**
 1）使用jndi获取一个ConnectionFacatory对象；
 2）使用jndi获取一个或者多个Destination对象；
 3）使用ConnectionFactory创建一个JMS连接；
 4）使用连接创建Jms session；
 5）使用session和destination创建MessageProducers和MessageConsumers
 6）使用Connection进行传输消息；
上述是jms的基础知识，简单了解可以便于下面的应用。jms本身提供了jar可以下载并使用相关配置，结合消息系统来完成消息的发送和接受等操作。但是一种便捷的方式，为加快开发，可以使用spring提供的jms模板，即JmsTemplate，这个类似于jdbcTemplate。
我们演示PTP和PUB/SUB两种模式的配置。
先看下基础公用的类：
我们定义：消息发送者、消息接受者、消息转换器
**[java]**[view
 plain](http://blog.csdn.net/quzishen/article/details/6128781#)[copy](http://blog.csdn.net/quzishen/article/details/6128781#)
- /**
-  * message sender
-  * @description <p></p>
-  * @author quzishen
-  * @project NormandyPositionII
-  * @class MessageSender.java
-  * @version 1.0
-  * @time 2011-1-11
-  */
- publicclass MessageSender {  
- // ~~~ jmsTemplate
- public JmsTemplate jmsTemplate;  
- 
- /**
-      * send message
-      */
- publicvoid sendMessage(){  
-         jmsTemplate.convertAndSend("hello jms!");  
-     }  
- publicvoid setJmsTemplate(JmsTemplate jmsTemplate) {  
- this.jmsTemplate = jmsTemplate;  
-     }  
- }  
**[java]**[view
 plain](http://blog.csdn.net/quzishen/article/details/6128781#)[copy](http://blog.csdn.net/quzishen/article/details/6128781#)
- /**
-  * message receiver
-  * @description <p></p>
-  * @author quzishen
-  * @project NormandyPositionII
-  * @class MessageReceiver.java
-  * @version 1.0
-  * @time 2011-1-11
-  */
- publicclass MessageReceiver implements MessageListener {  
- /* (non-Javadoc)
-      * @see javax.jms.MessageListener#onMessage(javax.jms.Message)
-      */
- publicvoid onMessage(Message m) {  
-         System.out.println("[receive message]");  
- 
-         ObjectMessage om = (ObjectMessage) m;  
- try {  
-             String key1 = om.getStringProperty("key1");  
- 
-             System.out.println(key1);  
- 
-             System.out.println("model:"+om.getJMSDeliveryMode());  
-             System.out.println("destination:"+om.getJMSDestination());  
-             System.out.println("type:"+om.getJMSType());  
-             System.out.println("messageId:"+om.getJMSMessageID());  
-             System.out.println("time:"+om.getJMSTimestamp());  
-             System.out.println("expiredTime:"+om.getJMSExpiration());  
-             System.out.println("priority:"+om.getJMSPriority());  
-         } catch (JMSException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
**[java]**[view
 plain](http://blog.csdn.net/quzishen/article/details/6128781#)[copy](http://blog.csdn.net/quzishen/article/details/6128781#)
- /**
-  * message converter
-  * @description <p></p>
-  * @author quzishen
-  * @project NormandyPositionII
-  * @class MessageConvertForSys.java
-  * @version 1.0
-  * @time 2011-1-11
-  */
- publicclass MessageConvertForSys implements MessageConverter {  
- /* (non-Javadoc)
-      * @see org.springframework.jms.support.converter.MessageConverter#toMessage(java.lang.Object, javax.jms.Session)
-      */
- public Message toMessage(Object object, Session session)  
- throws JMSException, MessageConversionException {  
-         System.out.println("[toMessage]");  
-         ObjectMessage objectMessage = session.createObjectMessage();  
- 
-         objectMessage.setJMSExpiration(1000);  
-         objectMessage.setStringProperty("key1", object+"_add");  
- 
- return objectMessage;  
-     }  
- /* (non-Javadoc)
-      * @see org.springframework.jms.support.converter.MessageConverter#fromMessage(javax.jms.Message)
-      */
- public Object fromMessage(Message message) throws JMSException,  
-             MessageConversionException {  
-         System.out.println("[fromMessage]");  
-         ObjectMessage objectMessage = (ObjectMessage) message;  
- 
- return objectMessage.getObjectProperty("key1");  
-     }  
- }  
第一种，PTP方式的配置：
**[java]**[view
 plain](http://blog.csdn.net/quzishen/article/details/6128781#)[copy](http://blog.csdn.net/quzishen/article/details/6128781#)
- <?xml version="1.0" encoding="UTF-8"?>  
- <beans xmlns="http://www.springframework.org/schema/beans"
-     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
-     xmlns:context="http://www.springframework.org/schema/context"
-     xmlns:aop="http://www.springframework.org/schema/aop"
-     xsi:schemaLocation="http://www.springframework.org/schema/beans 
-     http://www.springframework.org/schema/beans/spring-beans-2.5.xsd 
-     http://www.springframework.org/schema/context 
-     http://www.springframework.org/schema/context/spring-context-2.5.xsd "
- default-autowire="byName">  
- 
-     <!-- JMS PTP MODEL -->  
-     <!-- PTP链接工厂 -->  
-     <bean id="queueConnectionFactory"class="org.apache.activemq.spring.ActiveMQConnectionFactory">  
-         <property name="brokerURL" value="tcp://127.0.0.1:61616" />  
-         <!-- <property name="brokerURL" value="vm://normandy.notify" /> -->  
-         <property name="useAsyncSend" value="true" />  
-     </bean>  
-     <!-- 定义消息队列 -->  
-     <bean id="dest"class="org.apache.activemq.command.ActiveMQQueue">  
-         <constructor-arg value="queueDest" />  
-     </bean>  
-     <!-- PTP jms模板 -->  
-     <bean id="jmsTemplate"class="org.springframework.jms.core.JmsTemplate">  
-         <property name="connectionFactory" ref="queueConnectionFactory"></property>  
-         <property name="defaultDestination" ref="dest" />  
-         <property name="messageConverter" ref="messageConvertForSys" />  
-         <property name="pubSubDomain" value="false" />  
-     </bean>  
- 
-     <!-- 消息转换器 -->  
-     <bean id="messageConvertForSys"class="com.normandy.tech.test.MessageConvertForSys"></bean>  
- 
-     <!-- 消息发送方 -->  
-     <bean id="messageSender"class="com.normandy.tech.test.MessageSender"></bean>  
-     <!-- 消息接收方 -->  
-     <bean id="messageReceiver"class="com.normandy.tech.test.MessageReceiver"></bean>  
- 
-     <!-- 消息监听容器 -->  
-     <bean id="listenerContainer"
- class="org.springframework.jms.listener.DefaultMessageListenerContainer">    
-         <property name="connectionFactory" ref="queueConnectionFactory" />    
-         <property name="destination" ref="dest" />    
-         <property name="messageListener" ref="messageReceiver" />    
-     </bean>  
- </beans>  
第二种：PUB/SUB方式的配置
我们配置两个消息订阅者，分别订阅不同的消息，这样用于判断是否成功执行了消息的发布和消息的订阅
**[java]**[view
 plain](http://blog.csdn.net/quzishen/article/details/6128781#)[copy](http://blog.csdn.net/quzishen/article/details/6128781#)
- <?xml version="1.0" encoding="UTF-8"?>  
- <beans xmlns="http://www.springframework.org/schema/beans"
-     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
-     xmlns:context="http://www.springframework.org/schema/context"
-     xmlns:aop="http://www.springframework.org/schema/aop"
-     xsi:schemaLocation="http://www.springframework.org/schema/beans 
-     http://www.springframework.org/schema/beans/spring-beans-2.5.xsd 
-     http://www.springframework.org/schema/context 
-     http://www.springframework.org/schema/context/spring-context-2.5.xsd "
- default-autowire="byName">  
- 
-     <!-- JMS TOPIC MODEL -->  
-     <!-- TOPIC链接工厂 -->  
-     <bean id="topicSendConnectionFactory"class="org.apache.activemq.spring.ActiveMQConnectionFactory">  
-         <property name="brokerURL" value="tcp://127.0.0.1:61616" />  
-         <property name="useAsyncSend" value="true" />  
-     </bean>  
- 
-     <bean id="topicListenConnectionFactory"class="org.apache.activemq.spring.ActiveMQConnectionFactory">  
-         <property name="brokerURL" value="tcp://127.0.0.1:61616" />  
-     </bean>  
- 
-     <!-- 定义主题 -->  
-     <bean id="myTopic"class="org.apache.activemq.command.ActiveMQTopic">  
-       <constructor-arg value="normandy.topic"/>  
-     </bean>  
- 
-     <bean id="myTopic2"class="org.apache.activemq.command.ActiveMQTopic">  
-       <constructor-arg value="normandy.topic2"/>  
-     </bean>  
- 
-     <!-- 消息转换器 -->  
-     <bean id="messageConvertForSys"class="com.normandy.tech.test.MessageConvertForSys"></bean>  
- 
-     <!-- TOPIC send jms模板 -->  
-     <bean id="topicSendJmsTemplate"class="org.springframework.jms.core.JmsTemplate">  
-         <property name="connectionFactory" ref="topicSendConnectionFactory"></property>  
-         <property name="defaultDestination" ref="myTopic" />  
-         <property name="messageConverter" ref="messageConvertForSys" />  
-         <!-- 开启订阅模式 -->  
-         <property name="pubSubDomain" value="true"/>  
-     </bean>  
- 
-     <!-- 消息发送方 -->  
-     <bean id="topicMessageSender"class="com.normandy.tech.test.MessageSender">  
-         <property name="jmsTemplate" ref="topicSendJmsTemplate"></property>  
-     </bean>  
- 
-     <!-- 消息接收方 -->  
-     <bean id="topicMessageReceiver"class="com.normandy.tech.test.MessageReceiver">  
-     </bean>  
- 
-     <!-- 主题消息监听容器 -->  
-     <bean id="listenerContainer"
- class="org.springframework.jms.listener.DefaultMessageListenerContainer">    
-         <property name="connectionFactory" ref="topicListenConnectionFactory" />    
-         <property name="pubSubDomain" value="true"/><!-- default is false -->  
-         <property name="destination" ref="myTopic" />  <!-- listen topic: myTopic -->  
-         <property name="subscriptionDurable" value="true"/>  
-         <property name="clientId" value="clientId_001"/>  
-         <property name="messageListener" ref="topicMessageReceiver" />    
-     </bean>  
- 
-     <!-- 主题消息监听容器2 -->  
-     <bean id="listenerContainer2"
- class="org.springframework.jms.listener.DefaultMessageListenerContainer">    
-         <property name="connectionFactory" ref="topicListenConnectionFactory" />    
-         <property name="pubSubDomain" value="true"/><!-- default is false -->  
-         <property name="destination" ref="myTopic2" />  <!-- listen topic: myTopic2 -->  
-         <property name="subscriptionDurable" value="true"/>  
-         <property name="clientId" value="clientId_002"/>  
-         <property name="messageListener" ref="topicMessageReceiver" />    
-     </bean>  
- </beans>  
单元测试代码：
**[java]**[view
 plain](http://blog.csdn.net/quzishen/article/details/6128781#)[copy](http://blog.csdn.net/quzishen/article/details/6128781#)
- publicclass TechTest extends TestCase {  
-     ApplicationContext ptpApplicationContext;  
-     ApplicationContext topicApplicationContext;  
- 
- @Override
- protectedvoid setUp() throws Exception {  
- super.setUp();  
-         ptpApplicationContext = new ClassPathXmlApplicationContext(  
- "com/normandy/tech/test/ptpContext.xml");  
-         topicApplicationContext = new ClassPathXmlApplicationContext(  
- "com/normandy/tech/test/topicContext.xml");  
-     }  
- 
- protected Object getPtpBean(String name) {  
- return ptpApplicationContext.getBean(name);  
-     }  
- 
- protected Object getTopicBean(String name) {  
- return topicApplicationContext.getBean(name);  
-     }  
- }  
**[java]**[view
 plain](http://blog.csdn.net/quzishen/article/details/6128781#)[copy](http://blog.csdn.net/quzishen/article/details/6128781#)
- /**
-  * 测试消息发送
-  * @description <p></p>
-  * @author quzishen
-  * @project NormandyPositionII
-  * @class JmsQueueTest.java
-  * @version 1.0
-  * @time 2011-1-11
-  */
- publicclass JmsQueueTest extends TechTest {  
- 
- /**
-      * 测试消息发送
-      */
- publicvoid testQueueSend() {  
- long beginTime = System.currentTimeMillis();  
- // PTP
- //      MessageSender messageSender = (MessageSender) getPtpBean("messageSender");
- //      messageSender.sendMessage();
- 
- // TOPIC
-         MessageSender messageSender = (MessageSender) getTopicBean("topicMessageSender");  
-         messageSender.sendMessage();  
-         System.out.println("cost time:"+ (System.currentTimeMillis() - beginTime));  
-     }  
- }  
测试结果执行便可。
在这里，消息系统我们采用的是activemq，试想一个问题，如果消息过多，这个时候发生了宕机，消息是否会丢失？
这里就涉及到了一个新问题，即消息持久化。
activemq的消息持久化分成两种：文件和数据库（支持mysql/oracle）。可以再其配置文件中进行配置，activemq配置文件采用的是spring的方式，所以配置起来非常的方便。
通常下载了activemq后，会有一系列的配置文件demo，可以参照其中的样例修改即可。
这里我们使用mysql作为消息持久化的数据库服务器。
将mysql的驱动包，拷贝到activemq的lib目录，配置如下：
conf/activemq.xml
**[xhtml]**[view
 plain](http://blog.csdn.net/quzishen/article/details/6128781#)[copy](http://blog.csdn.net/quzishen/article/details/6128781#)
- <persistenceAdapter>
- <!--<kahaDB directory="${activemq.base}/data/kahadb"/>-->
- <jdbcPersistenceAdapterdataDirectory="${activemq.base}/data"dataSource="#mysql-ds"/>
- </persistenceAdapter>
**[java]**[view
 plain](http://blog.csdn.net/quzishen/article/details/6128781#)[copy](http://blog.csdn.net/quzishen/article/details/6128781#)
- <bean id="mysql-ds"class="org.apache.commons.dbcp.BasicDataSource" destroy-method="close">  
-         <property name="driverClassName" value="com.mysql.jdbc.Driver"/>  
-         <property name="url" value="jdbc:mysql://localhost/activemq?relaxAutoCommit=true"/>  
-         <property name="username" value="root"/>  
-         <property name="password" value="root"/>  
-         <property name="maxActive" value="200"/>  
-         <property name="poolPreparedStatements" value="true"/>  
-     </bean>  
特别注意的是，这里指定的数据库名称，需要事先在mysql中创建好schema。
运行activemq，可以发现自动创建了三张表：
activemq_acks
activemq_lock
activemq_msgs
