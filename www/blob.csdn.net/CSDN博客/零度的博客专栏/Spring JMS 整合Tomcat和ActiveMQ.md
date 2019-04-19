# Spring JMS 整合Tomcat和ActiveMQ - 零度的博客专栏 - CSDN博客
2016年04月07日 11:07:27[零度anngle](https://me.csdn.net/zmx729618)阅读数：899
1.Active MQ安装配置 
1.1.下载并解压Active MQ 
1.2.双击bin/activemq.bat启动Active MQ 
1.3.浏览器输入http://localhost:8161/admin/ ,可以图形化界面管理activemq 
1.4.点击Queue，新建一个队列名字叫TestQueue1 
![](http://dl.iteye.com/upload/attachment/598984/fa106a47-0428-3986-8c6b-4d8ddbf2519b.jpg)
2.在Eclipse里面配置Tomcat的context.xml，这样Spring就能以JNDI方式访问ActiveMQ了 
![](http://dl.iteye.com/upload/attachment/598986/91153192-a5c8-3486-9ddb-bbbc27eb16a5.jpg)
加入以下代码 
Xml代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- <Resourcename="jms/ConnectionFactory"
- auth="Container"
- type="org.apache.activemq.ActiveMQConnectionFactory"
- description="JMS Connection Factory"
- factory="org.apache.activemq.jndi.JNDIReferenceFactory"
- brokerURL="tcp://localhost:61616"
- brokerName="LocalActiveMQBroker"/>
- 
- <Resourcename="jms/Queue"
- auth="Container"
- type="org.apache.activemq.command.ActiveMQQueue"
- description="my Queue"
- factory="org.apache.activemq.jndi.JNDIReferenceFactory"
- physicalName="TestQueue1"/>
3.导入以下Jar包 
activemq-all-5.4.1.jar 
commons-logging-1.1.1.jar 
javax.jms_1.1.1.jar 
spring.asm-3.0.0.RELEASE.jar 
spring.beans-3.0.0.RELEASE.jar 
spring.context-3.0.0.RELEASE.jar 
spring.core-3.0.0.RELEASE.jar 
spring.expression-3.0.0.RELEASE.jar 
spring.jms-3.0.0.RELEASE.jar 
spring.transaction-3.0.0.RELEASE.jar 
spring.web-3.0.0.RELEASE.jar 
4.编写2个类，一个MessageQueueSender发消息，一个MessageQueueReceiver收消息，为了方便测试，我们还写了一个Servlet调用MessageQueueSender，通过JSP来调用Servlet 
5.写Spring的配置文件，假设存在这个文件里：jms-activemq.xml 
Xml代码  ![收藏代码](http://xpenxpen.iteye.com/images/icon_star.png)
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd">
- 
- <!-- Connection factory -->
- <beanid="jmsConnectionFactory"class="org.springframework.jndi.JndiObjectFactoryBean">
- <propertyname="jndiName"value="java:comp/env/jms/ConnectionFactory"></property>
- </bean>
- 
- <!-- Queue -->
- <beanid="jmsQueue"class="org.springframework.jndi.JndiObjectFactoryBean"
- lazy-init="true">
- <propertyname="jndiName"value="java:comp/env/jms/Queue"></property>
- </bean>
- 
- <!-- Spring JMS Template -->
- <beanid="jmsTemplate"class="org.springframework.jms.core.JmsTemplate"
- lazy-init="true">
- <propertyname="connectionFactory"ref="jmsConnectionFactory"></property>
- <propertyname="defaultDestination"ref="jmsQueue"></property>
- </bean>
- 
- <!-- Sender -->
- <beanid="jmsSender"class="com.xp.MessageQueueSender"
- lazy-init="true">
- <propertyname="jmsTemplate"ref="jmsTemplate"></property>
- </bean>
- 
- <!-- Receiver -->
- <beanid="jmsReceiver"class="com.xp.MessageQueueReceiver">
- </bean>
- 
- <!-- Message Listener -->
- <beanid="listenerContainer"
- class="org.springframework.jms.listener.DefaultMessageListenerContainer">
- <propertyname="connectionFactory"ref="jmsConnectionFactory"></property>
- <propertyname="destination"ref="jmsQueue"></property>
- <propertyname="messageListener"ref="jmsReceiver"></property>
- <propertyname="autoStartup"value="true"/>
- </bean>
- 
- </beans>
6.启动tomcat测试看效果 
6.1.浏览器输入http://localhost:8080/JMSTest/，随便输入消息比如"Hello",点击submit 
![](http://dl.iteye.com/upload/attachment/598991/b90bd9ed-f9ea-3b24-944b-a9daa69730d2.jpg)
6.2.可在控制台看到Hello的信息，表明发送和接受者都已经OK 
![](http://dl.iteye.com/upload/attachment/598993/da65dd1b-df67-382c-8d24-3e6f5c94f9d9.jpg)
6.3.还可以在ActiveMQ管理台创建一条消息，点击"Send To" 
![](http://dl.iteye.com/upload/attachment/598995/ec21ce44-ddf0-3579-b54a-b56f7d4f7aef.jpg)
随便打入一点消息 
![](http://dl.iteye.com/upload/attachment/598997/c10e76a6-f489-3d19-88f7-40c3833339b8.jpg)
6.4.可在控制台看到接受者能收到此消息 
![](http://dl.iteye.com/upload/attachment/598999/1dcdb45b-f051-379b-aa91-d47d6b6b7913.jpg)
[](http://blog.csdn.net/z69183787/article/details/20537293#)[](http://blog.csdn.net/z69183787/article/details/20537293#)[](http://blog.csdn.net/z69183787/article/details/20537293#)[](http://blog.csdn.net/z69183787/article/details/20537293#)[](http://blog.csdn.net/z69183787/article/details/20537293#)[](http://blog.csdn.net/z69183787/article/details/20537293#)
