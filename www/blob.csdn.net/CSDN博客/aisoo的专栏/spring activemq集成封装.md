# spring activemq集成封装 - aisoo的专栏 - CSDN博客
2012年11月20日 14:57:48[aisoo](https://me.csdn.net/aisoo)阅读数：1939
private JmsTemplate salerMqProvider;
/***
* 下发MQ消息
* 
* @param body
*/
public void send(final int messageId, final String body) {
salerMqProvider.send(new MessageCreator() {
public Message createMessage(Session session) throws JMSException {
Message message = session.createTextMessage(body);
message.setStringProperty("messageId", messageId + "");
return message;
}
});
}
配置
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
xmlns:mq="http://code.360buy.com/schema/mq"
xsi:schemaLocation="http://www.springframework.org/schema/beans 
http://www.springframework.org/schema/beans/spring-beans.xsd
http://code.360buy.com/schema/mq 
http://code.360buy.com/schema/mq/mq-1.0.xsd
">
<bean id="propertyConfigurer"
class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
<property name="locations">
<list>
<value>classpath:mq.properties</value>
</list>
</property>
</bean>
<mq:zkClient id="zkClient" url="${zk.url}"/>
<mq:connectionFactory id="connectionFactory" user="mq" password="mq" address="zk://zkClient/${mq.destination}" systemId="${mq.systemId.producer}" />
<mq:jmsTemplate id="salerMqProvider" connectionFactory="connectionFactory" destination="test.queue" />
</beans>
