# ActiveMQ异常排查： peer did not send his wire format. - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年04月27日 14:12:14[boonya](https://me.csdn.net/boonya)阅读数：6678








后台报出如下错误：



```
javax.jms.JMSException: Wire format negociation timeout: peer did not send his wire format.
at org.apache.activemq.util.JMSExceptionSupport.create(JMSExceptionSupport.java:58)
at org.apache.activemq.ActiveMQConnection.syncSendPacket(ActiveMQConnection.java:1185)
at org.apache.activemq.ActiveMQConnection.ensureConnectionInfoSent(ActiveMQConnection.java:1263)
at org.apache.activemq.ActiveMQConnection.start(ActiveMQConnection.java:449)
```


有三种可能情况：

**1. You're connecting to the port not used by ActiveMQ TCP transport**


Make sure to check that you're connecting to the appropriate host:port

**2. You're using log4j JMS appender and doesn't filter out ActiveMQ log messages**


Be sure to read How do I use log4j JMS appender with ActiveMQ and more importantly to never send ActiveMQ log messages to JMS appender

**3. Your broker is probably under heavy load (or network connection is unreliable), so connection setup cannot be completed in a reasonable time**


If you experience sporadic exceptions like this, the best solution is to use failover transport, so that your clients can try connecting again if the first attempt fails. If you're getting these kind of exceptions more frequently you can also try extending
 wire format negotiation period (default 10 sec). You can do that by using wireFormat.maxInactivityDurationInitalDelay property on the connection URL in your client. For example

tcp://localhost:61616?wireFormat.maxInactivityDurationInitalDelay=30000 will use 30 sec timeout.





From：[http://activemq.apache.org/javaxjmsjmsexception-wire-format-negociation-timeout-peer-did-not-send-his-wire-format.html](http://activemq.apache.org/javaxjmsjmsexception-wire-format-negociation-timeout-peer-did-not-send-his-wire-format.html)



