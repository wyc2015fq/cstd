# ActiveMQ死信产生的原因及使用方案 - z69183787的专栏 - CSDN博客
2019年03月13日 16:11:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：85
[https://blog.csdn.net/m0_37609579/article/details/82216332](https://blog.csdn.net/m0_37609579/article/details/82216332)
DLQ-死信队列(Dead Letter Queue)用来保存处理失败或者过期的消息。
出现以下情况时，消息会被redelivered
A transacted session is used and rollback() is called(使用一个事务session，并且调用了rollback()方法).
A transacted session is closed before commit is called(一个事务session，关闭之前调用了commit).
A session is using CLIENT_ACKNOWLEDGE and Session.recover() is called(在session中使用CLIENT_ACKNOWLEDGE签收模式，并且调用了Session.recover()方法).
当一个消息被redelivered超过maximumRedeliveries(缺省为6次，具体设置请参考后面的内容)次数时，会给broker发送一个"Poison ack"，这个消息被认为是a poison pill(毒丸)，这时broker会将这个消息发送到DLQ，以便后续处理。
登录ActiveMQ管理端可以看到ActiveMq有一个默认的死信队：ActiveMQ.DLQ ，若未做设置则处理失败的消息会自动进入此队列。缺省持久消息过期，会被送到DLQ，非持久消息不会送到DLQ。本文将展示如何在Spring中引入私信重发机制。
一.死信的产生原因
1.死信队列堆积过多
测试的时候，每秒50条消息，堆积到3000 以上的时候，就会不定时地报错
因为每次连接MQ的时候，都创建了连接对象，占用内存太多，建议使用连接池，修改Spring 的配置
```
<bean id="amqConnectionFactory" class="org.apache.activemq.pool.PooledConnectionFactory">
    <property name="connectionFactory">
        <bean>
            <property name="brokedURL" value="tcp://127.0.0.1:61616?jms.prefetchPolicy.all=2"></property>
            <property name="sendTimeout" value="5000"></property>
            <property name="closeTimeout" value="5000"></property>
            <property name="useAsyncSend" value="true"></property>
            <property name="useDedicatedTaskRunner" value="true"></property>
            <property name="alwaysSessionAsync" value="true"></property>
        </bean>
    </property>
</bean>
```
2.网络故障:：udp通信网络通信不正常会出现消息丢失，或挤压状况。
3.连接超时：ActiveMQ服务器挂了,要搭建集群，保证高可用。
二.Spring + ActiveMQ配置 死信队列（重发机制/延时发送）
1.ActiveMQ 部署时修改activemq.xml
在policyEntries节点中增加如下策略配置。
```
<policyEntry queue=">">
    <deadLetterStrategy>
        <individualDeadLetterStrategy queuePrefix="DLQ." useQueueForQueueMessages="true" processNonPersistent="true"/>
    </deadLetterStrategy>
</policyEntry>
```
2.服务端spring-mq配置文件做如下配置
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xsi:schemaLocation="
      http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.0.xsd"
       default-lazy-init="true">
 
    <!-- ActiveMQ 连接工厂 -->
    <!-- 真正可以产生Connection的ConnectionFactory，由对应的 JMS服务厂商提供-->
    <!--<amq:connectionFactory id="amqConnectionFactory"
                           brokerURL="${brokerUrl}" userName="${mq.userName}" password="${mq.password}" redeliveryPolicy="activeMQPolicy" />-->
    <bean id="amqConnectionFactory" class="org.apache.activemq.ActiveMQConnectionFactory">
        <property name="brokerURL" value="${brokerUrl}"></property>
        <property name="userName" value="${mq.userName}"></property>
        <property name="password" value="${mq.password}"></property>
        <property name="redeliveryPolicy" ref="activeMQPolicy" />  <!-- 引用重发机制 -->
    </bean>
 
    <!-- 加入死信机制 -->
    <bean id="activeMQPolicy" class="org.apache.activemq.RedeliveryPolicy">
        <!--是否在每次尝试重新发送失败后,增长这个等待时间 -->
        <property name="useExponentialBackOff" value="true"></property>
        <!--重发次数,默认为6次   这里设置为1次 -->
        <property name="maximumRedeliveries" value="1"></property>
        <!--重发时间间隔,默认为1秒 -->
        <property name="initialRedeliveryDelay" value="1000"></property>
        <!--第一次失败后重新发送之前等待500毫秒,第二次失败再等待500 * 2毫秒,这里的2就是value -->
        <property name="backOffMultiplier" value="2"></property>
        <!--最大传送延迟，只在useExponentialBackOff为true时有效（V5.5），假设首次重连间隔为10ms，倍数为2，那么第
            二次重连时间间隔为 20ms，第三次重连时间间隔为40ms，当重连时间间隔大的最大重连时间间隔时，以后每次重连时间间隔都为最大重连时间间隔。 -->
        <property name="maximumRedeliveryDelay" value="1000"></property>
        <!---->
        <property name="destination" ref="allDestination"></property>
 
    </bean>
    <!-- Spring Caching连接工厂 -->
    <!-- Spring用于管理真正的ConnectionFactory的ConnectionFactory -->
    <bean id="connectionFactory" class="org.springframework.jms.connection.CachingConnectionFactory">
        <!-- 目标ConnectionFactory对应真实的可以产生JMS Connection的ConnectionFactory -->
        <property name="targetConnectionFactory" ref="amqConnectionFactory"></property>
        <!-- 同上，同理 -->
        <!-- <constructor-arg ref="amqConnectionFactory" /> -->
        <!-- Session缓存数量 -->
        <property name="sessionCacheSize" value="100"/>
 
    </bean>
 
    <!-- 定义Queue监听器 -->
    <bean id="allDestination" class="org.apache.activemq.command.ActiveMQQueue">
        <constructor-arg value="C2S.Q.TEST" />
    </bean>
    <bean id="listenerContainer" class="org.springframework.jms.listener.DefaultMessageListenerContainer">
    <property name="connectionFactory" ref="connectionFactory" />
    <property name="messageListener" ref="dataReceiver" /><!--dataReceiver为MQ处理类，根据实际情况配置-->
    <property name="destination" ref="allDestination" />
    <property name="sessionTransacted" value="true"/>
    </bean>
```
3.服务端消息处理代码抛出 RuntimeExpection异常
```java
catch (JMSException e) {
    e.printStackTrace();
    throw new RuntimeException("消息处理异常");
}
```
三. ActiveMQ配置死信队列（丢弃机制）
需求场景：
       由于测试环境应用复杂的原因，造成了jms死信队列一直挤压很多数据，从而导致存储爆满，进而造成了各个客户端不能正常发送消息。针对这些死信队列，一般都没有利用价值的。
        为了避免某队列的死信队列的挤压，而使整个jms不可用，我们选择了通过ActiveMQ的配置，直接丢弃掉死信队列的消息。缺省死信队列(Dead Letter Queue)叫做ActiveMQ.DLQ所有的未送达消息都会被发送到这个队列，以致会非常难于管理。
    所以我们为了防止内存挤压导致mq不可用的情况发生，就需要对死信进行配置。可以通过配置文件(activemq.xml)来调整死信发送策略。
一些配置策略：
1．  不使用缺省的死信队列
   缺省所有队列的死信消息都被发送到同一个缺省死信队列，不便于管理。可以通过individualDeadLetterStrategy或sharedDeadLetterStrategy策略来进行修改。如下：
```
<broker>
  <destinationPolicy>
    <policyMap>
      <policyEntries>
        <!— 设置所有队列，使用 '>' ，否则用队列名称 -->
        <policyEntry queue=">">
          <deadLetterStrategy>
            <!--
                    queuePrefix:设置死信队列前缀
                    useQueueForQueueMessages: 设置使用队列保存死信，还可以设置useQueueForTopicMessages，使用Topic来保存死信
            -->
            <individualDeadLetterStrategy   queuePrefix="DLQ." useQueueForQueueMessages="true" />
          </deadLetterStrategy>
        </policyEntry>
      </policyEntries>
    </policyMap>
  </destinationPolicy>
  ...
</broker>
```
2．  非持久消息保存到死信队列
```
<policyEntry queue=">">  
    <deadLetterStrategy>  
          <sharedDeadLetterStrategy processNonPersistent="true" />  
    </deadLetterStrategy>  
</policyEntry>
```
3．  过期消息不保存到死信队列
```
<policyEntry queue=">">  
     <deadLetterStrategy>  
          <sharedDeadLetterStrategy processExpired="false" />  
     </deadLetterStrategy>  
 </policyEntry>
```
4．  持久消息不保存到死信队列
对于过期的，可以通过processExpired属性来控制，对于redelivered的失败的消息，需要通过插件来实现如下：
1)丢弃所有死信
```
<beans>
    <broker>  
        <plugins>  
          <discardingDLQBrokerPlugin dropAll="true" dropTemporaryTopics="true" dropTemporaryQueues="true" />  
        </plugins>  
    </broker>  
</beans>
```
2)丢弃指定目的死信
```
<beans>
  <broker ...>
    <plugins>
      <discardingDLQBrokerPlugin dropOnly="MY.EXAMPLE.TOPIC.29 MY.EXAMPLE.QUEUE.87" reportInterval="1000" />
    </plugins>
  </broker>
</beans>
```
注意，目的名称使用空格分隔
3)用正则表达式过滤丢弃消息
```
<beans>
  <broker ...>
    <plugins>
      <discardingDLQBrokerPlugin dropOnly="MY.EXAMPLE.TOPIC.[0-9]{3} MY.EXAMPLE.QUEUE.[0-9]{3}" reportInterval="3000"/>
    </plugins>
  </broker>
</beans>
```
四.死信队列消息的属性
死信队列中的消息，会增加几个属性，比如原过期时间(originalExpiration)，原originalDeliveryMode等。
五.死信队列解决方案总结
1.重发：对于安全性要求比较高的系统，那需要将发送失败的消息进行重试发
送，甚至在消息一直不能到达的情况下给予相关的邮件、短信等必要的告警
措施以保证消息的正确投递。
2.丢弃：在消息不是很重要以及有其他通知手段的 情况下，那么对消息做丢弃
处理也不失为一种好办法，毕竟如果大量不可抵达的消息存在于消息系统中
会对我们的系统造成非常大的负荷，所以我们也会采用丢弃的方式进行处理。
3.不管是 重发还是丢弃，都要根据实际的业务场景而定，不一而足，同学们在
实际中要具体问题具体分析。
参考:https://blog.csdn.net/qq_16055867/article/details/80443716
https://bbs.csdn.net/topics/392189838
https://blog.csdn.net/zhou2s_101216/article/details/78322080
https://blog.csdn.net/Leonardo9029/article/details/49513713
