# Spring+JMS+ActiveMQ+Tomcat实现消息服务 - 零度的博客专栏 - CSDN博客
2016年07月19日 10:56:04[零度anngle](https://me.csdn.net/zmx729618)阅读数：912
                
基于Spring+JMS+ActiveMQ+Tomcat，我使用的版本情况如下所示：
- Spring 2.5
- ActiveMQ 5.4.0
- Tomcat 6.0.30
下面通过学习与配置，实现消息服务的基本功能：发送与接收。Spring对JMS提供了很好的支持，可以通过JmsTemplate来方便地实现消息服务。这里，我们的消息服务不涉及事务管理。下面简单说明实现过程：
先看一下，我们最终的Spring配置文件applicationContext.xml的内容，如下所示：
**[xhtml]**[view
 plain](http://blog.csdn.net/shirdrn/article/details/6362792#)[copy](http://blog.csdn.net/shirdrn/article/details/6362792#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:amq="http://activemq.apache.org/schema/core"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd  
-         http://activemq.apache.org/schema/core http://activemq.apache.org/schema/core/activemq-core.xsd">
- 
- <beanid="listenerContainer"
- class="org.springframework.jms.listener.DefaultMessageListenerContainer">
- <propertyname="connectionFactory"ref="connectionFactory"></property>
- <propertyname="destination"ref="messageQueue"></property>
- <propertyname="messageListener"ref="receiveMessageListener"></property>
- </bean>
- <beanid="connectionFactory"class="org.springframework.jndi.JndiObjectFactoryBean">
- <propertyname="jndiName"value="java:comp/env/myJMS/ConnectionFactory"></property>
- </bean>
- <beanid="messageQueue"class="org.springframework.jndi.JndiObjectFactoryBean">
- <propertyname="jndiName"value="java:comp/env/myJMS/MessageQueue"></property>
- </bean>
- <beanid="receiveMessageListener"
- class="org.shirdrn.spring.jms.integration.ReceiveMessageListener"></bean>
- 
- <beanid="messageSender"class="org.shirdrn.spring.jms.integration.MessageSender">
- <propertyname="jmsTemplate"ref="jmsTemplate"></property>
- </bean>
- <beanid="jmsTemplate"class="org.springframework.jms.core.JmsTemplate">
- <propertyname="connectionFactory"ref="connectionFactory"></property>
- <propertyname="defaultDestination"ref="messageQueue"></property>
- </bean>
- 
- <beanid="sendMessageController"
- class="org.shirdrn.spring.jms.integration.SendMessageController">
- <propertyname="messageSender"ref="messageSender"/>
- <propertyname="successView"value="/success"/>
- </bean>
- <beanid="urlMapping"
- class="org.springframework.web.servlet.handler.SimpleUrlHandlerMapping">
- <propertyname="mappings">
- <props>
- <propkey="/sendMessage.do">sendMessageController</prop>
- </props>
- </property>
- </bean>
- <beanid="viewResolver"
- class="org.springframework.web.servlet.view.InternalResourceViewResolver">
- <propertyname="requestContextAttribute"value="rc"/>
- <propertyname="viewClass"
- value="org.springframework.web.servlet.view.JstlView"/>
- <propertyname="prefix"value="/"/>
- <propertyname="suffix"value=".jsp"/>
- </bean>
- 
- </beans>
![](http://static.blog.csdn.net/images/save_snippets.png)
我们使用Spring的org.springframework.jms.listener.DefaultMessageListenerContainer来收集消息，通过设置一个消息监听器，具体实现类为org.shirdrn.spring.jms.integration.ReceiveMessageListener，代码如下所示：
**[java]**[view
 plain](http://blog.csdn.net/shirdrn/article/details/6362792#)[copy](http://blog.csdn.net/shirdrn/article/details/6362792#)
- package org.shirdrn.spring.jms.integration;  
- 
- import javax.jms.JMSException;  
- import javax.jms.Message;  
- import javax.jms.MessageListener;  
- import javax.jms.TextMessage;  
- 
- import org.apache.log4j.Logger;  
- 
- publicclass ReceiveMessageListener implements MessageListener {  
- 
- privatestaticfinal Logger LOG = Logger.getLogger(ReceiveMessageListener.class);  
- 
- publicvoid onMessage(Message message) {  
- if (message instanceof TextMessage) {  
-             TextMessage text = (TextMessage) message;  
- try {  
-                 LOG.info("Received message:" + text.getText());  
-             } catch (JMSException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
上面，对发送的消息进行监听，并接收处理，我们只是简单地打印出一条日志内容。
对于listenerContainer，还需要注入连接工厂connectionFactory和消息目的destination这两个属性：connectionFactory我们使用ActiveMQ的org.apache.activemq.ActiveMQConnectionFactory，并通过JNDI服务，绑定到名字java:comp/env/myJMS/ConnectionFactory上；而destination属性通过使用ActiveMQ的org.apache.activemq.command.ActiveMQQueue消息队列，也是通过JNDI服务绑定到名字java:comp/env/myJMS/MessageQueue上。所以，在Tomcat的conf/context.xml中的<Context>元素里面加上如下配置：
**[xhtml]**[view
 plain](http://blog.csdn.net/shirdrn/article/details/6362792#)[copy](http://blog.csdn.net/shirdrn/article/details/6362792#)
- <Resourcename="myJMS/ConnectionFactory"
- auth="Container"
- type="org.apache.activemq.ActiveMQConnectionFactory"
- description="JMS Connection Factory"
- factory="org.apache.activemq.jndi.JNDIReferenceFactory"
- brokerURL="vm://shirdrnUrl"
- brokerName="MyActiveMQBroker"/>
- 
- <Resourcename="myJMS/MessageQueue"
- auth="Container"
- type="org.apache.activemq.command.ActiveMQQueue"
- description="My Message Queue"
- factory="org.apache.activemq.jndi.JNDIReferenceFactory"
- physicalName="MyMessageQueue"/>
![](http://static.blog.csdn.net/images/save_snippets.png)
我们通过使用JmsTemplate来实现消息的发送，所以实现的发送类要将JmsTemplate注入进去，实现代码如下所示：
**[java]**[view
 plain](http://blog.csdn.net/shirdrn/article/details/6362792#)[copy](http://blog.csdn.net/shirdrn/article/details/6362792#)
- package org.shirdrn.spring.jms.integration;  
- 
- import javax.jms.JMSException;  
- import javax.jms.Message;  
- import javax.jms.Session;  
- import javax.jms.TextMessage;  
- 
- import org.apache.log4j.Logger;  
- import org.springframework.jms.core.JmsTemplate;  
- import org.springframework.jms.core.MessageCreator;  
- 
- publicclass MessageSender {  
- 
- privatestaticfinal Logger LOG = Logger.getLogger(MessageSender.class);  
- private JmsTemplate jmsTemplate;  
- 
- publicvoid setJmsTemplate(JmsTemplate jmsTemplate) {  
- this.jmsTemplate = jmsTemplate;  
-     }  
- 
- publicvoid sendMessage(final String message) {  
-         LOG.info("Send message: " + message);  
-         jmsTemplate.send(new MessageCreator() {  
- 
- public Message createMessage(Session session) throws JMSException {  
-                 TextMessage textMessage = session.createTextMessage(message);  
- return textMessage;  
-             }  
- 
-         });  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
上面基于Spring的MessageCreator来创建消息，通过调用JmsTemplate的send方法发送出去。
对于Web，我们使用了Spring MVC，通过实现一个控制器org.shirdrn.spring.jms.integration.SendMessageController来控制页面消息的发送及其视图的派发。我们实现的SendMessageController类继承自MultiActionController，可以在一个控制器中实现多个Action，代码实现如下所示：
**[java]**[view
 plain](http://blog.csdn.net/shirdrn/article/details/6362792#)[copy](http://blog.csdn.net/shirdrn/article/details/6362792#)
- package org.shirdrn.spring.jms.integration;  
- 
- import java.util.HashMap;  
- import java.util.Map;  
- 
- import javax.servlet.http.HttpServletRequest;  
- import javax.servlet.http.HttpServletResponse;  
- 
- import org.springframework.web.servlet.ModelAndView;  
- import org.springframework.web.servlet.mvc.multiaction.MultiActionController;  
- 
- publicclass SendMessageController extends MultiActionController {  
- 
- private String successView;  
- private MessageSender messageSender;  
- 
- public ModelAndView sendMessage(HttpServletRequest request,  
-             HttpServletResponse response) throws Exception {  
- 
-         Map<String, Object> retMap = new HashMap<String, Object>();  
-         String message = request.getParameter("message");  
-         messageSender.sendMessage(message);  
- 
- returnnew ModelAndView(successView, retMap);  
-     }  
- 
- public String getSuccessView() {  
- return successView;  
-     }  
- 
- publicvoid setSuccessView(String successView) {  
- this.successView = successView;  
-     }  
- 
- public MessageSender getMessageSender() {  
- return messageSender;  
-     }  
- 
- publicvoid setMessageSender(MessageSender messageSender) {  
- this.messageSender = messageSender;  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
上面调用模型层（Model）的MessageSender来实现发送消息的处理逻辑，如果发送成功，视图派发到successView指定的页面。可以看到，最前面我们给出的Spring配置内容分为三组，最后一组是对控制器的配置：
viewResolver                     视图解析器配置，可以将控制器中指定前缀（/）解析为后缀是.jsp的页面，例如/success解析为/sucess.jsp
urlMapping                         请求URL与控制器的映射，例如对于满足/sendMessage.do模式的请求，都会被指派给sendMessageController去处理
sendMessageController      控制器实现类，里面的方法名称可以自定义，但要在org.springframework.web.servlet.handler.SimpleUrlHandlerMapping中的mappings属性中配置映射
然后，我们需要一个web部署描述文件，web.xml文件配置内容，如下所示：
**[xhtml]**[view
 plain](http://blog.csdn.net/shirdrn/article/details/6362792#)[copy](http://blog.csdn.net/shirdrn/article/details/6362792#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <web-appversion="2.5"xmlns="http://java.sun.com/xml/ns/javaee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/javaee   
-     http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
- 
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>
-             classpath:org/shirdrn/spring/jms/integration/applicationContext.xml  
- </param-value>
- </context-param>
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
- 
- <servlet>
- <servlet-name>controller</servlet-name>
- <servlet-class>
-             org.springframework.web.servlet.DispatcherServlet  
- </servlet-class>
- <init-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>
-                 classpath:org/shirdrn/spring/jms/integration/applicationContext.xml  
- </param-value>
- </init-param>
- <load-on-startup>1</load-on-startup>
- </servlet>
- 
- <servlet-mapping>
- <servlet-name>controller</servlet-name>
- <url-pattern>*.do</url-pattern>
- </servlet-mapping>
- 
- 
- <welcome-file-list>
- <welcome-file>index.jsp</welcome-file>
- </welcome-file-list>
- </web-app>
![](http://static.blog.csdn.net/images/save_snippets.png)
另外，我们还要实现一个页面，提供输入发送消息的表单，提交后交给后台处理，成功发送后跳转到一个成功页面。表单输入页面为index.jsp，如下所示：
**[java]**[view
 plain](http://blog.csdn.net/shirdrn/article/details/6362792#)[copy](http://blog.csdn.net/shirdrn/article/details/6362792#)
- <%@ page language="java"import="java.util.*" pageEncoding="UTF-8"%>  
- <%  
-     String path = request.getContextPath();  
-     String basePath = request.getScheme() + "://"
-             + request.getServerName() + ":" + request.getServerPort()  
-             + path + "/";  
- %>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <head>  
-         <base href="<%=basePath%>">  
- 
-         <title>My JSP 'index.jsp' starting page</title>  
-         <meta http-equiv="pragma" content="no-cache">  
-         <meta http-equiv="cache-control" content="no-cache">  
-         <meta http-equiv="expires" content="0">  
-         <meta http-equiv="keywords" content="keyword1,keyword2,keyword3">  
-         <meta http-equiv="description" content="This is my page">  
-         <!--  
-     <link rel="stylesheet" type="text/css" href="styles.css" mce_href="styles.css">  
-     -->  
-     </head>  
- 
-     <body>  
-         <div align="center" style="width: 500px; height: 300px; border:2px; borderColor:black">  
-             <form action="sendMessage.do" method="post">  
-                 <table align="center">  
-                     <tr>  
-                         <th colspan="2">  
-                             消息发送控制台  
-                         </th>  
-                     </tr>  
-                     <tr>  
-                         <td>  
-                             消息内容：  
-                         </td>  
-                         <td>  
-                             <input type="text" name="message">  
-                         </td>  
-                     </tr>  
-                     <tr>  
-                         <td align="center" colspan="2">  
-                             <input type="reset" value="清除">  
- 
-                             <input type="submit" value="发送">  
-                         </td>  
-                     </tr>  
-                 </table>  
-             </form>  
-         </div>  
-     </body>  
- </html>  
![](http://static.blog.csdn.net/images/save_snippets.png)
成功页面为success.jsp，就是给一个成功的提示信息，如下所示：
**[java]**[view
 plain](http://blog.csdn.net/shirdrn/article/details/6362792#)[copy](http://blog.csdn.net/shirdrn/article/details/6362792#)
- <%@ page language="java"import="java.util.*" pageEncoding="UTF-8"%>  
- <%  
-     String path = request.getContextPath();  
-     String basePath = request.getScheme() + "://"
-             + request.getServerName() + ":" + request.getServerPort()  
-             + path + "/";  
- %>  
- 
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">  
- <html>  
-     <head>  
-         <base href="<%=basePath%>">  
- 
-         <title>My JSP 'index.jsp' starting page</title>  
-         <meta http-equiv="pragma" content="no-cache">  
-         <meta http-equiv="cache-control" content="no-cache">  
-         <meta http-equiv="expires" content="0">  
-         <meta http-equiv="keywords" content="keyword1,keyword2,keyword3">  
-         <meta http-equiv="description" content="This is my page">  
-         <!--  
-     <link rel="stylesheet" type="text/css" href="styles.css" mce_href="styles.css">  
-     -->  
-     </head>  
- 
-     <body>  
-         <div align="center" style="width: 500px; height: 300px; border:2px; borderColor:black">  
-             <form action="sendMessage.do" method="post">  
-                 <table align="center">  
-                     <tr>  
-                         <th colspan="2">  
-                             消息发送报告  
-                         </th>  
-                     </tr>  
-                     <tr>  
-                         <td colspan="2">  
-                             状态：发送成功  
-                         </td>  
-                     </tr>  
-                     <tr>  
-                         <td align="center" colspan="2">  
-                             <a href="index.jsp" mce_href="index.jsp">返回</a>  
-                         </td>  
-                     </tr>  
-                 </table>  
-             </form>  
-         </div>  
-     </body>  
- </html>  
![](http://static.blog.csdn.net/images/save_snippets.png)
至此，我们可以将实现的简单web工程发布到Tomcat容器，然后启动Tomcat服务器，通过页面可以发送消息，并通过日志查看，实际消息发送和接收的情况。 
