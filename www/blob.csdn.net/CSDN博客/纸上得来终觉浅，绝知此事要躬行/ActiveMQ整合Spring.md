# ActiveMQ整合Spring - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年04月20日 13:29:41[boonya](https://me.csdn.net/boonya)阅读数：879标签：[apache																[activemq																[java																[jms](https://so.csdn.net/so/search/s.do?q=jms&t=blog)
个人分类：[ActiveMQ](https://blog.csdn.net/boonya/article/category/1714191)






原文地址：[http://blog.csdn.net/u013256816/article/details/51180509](http://blog.csdn.net/u013256816/article/details/51180509)


通过前一篇《[ActiveMQ简述](http://blog.csdn.net/u013256816/article/details/51161548)》大概对ActiveMQ有了一个大概的认识，本篇所阐述的是如何通过Spring继承ActiveMQ进而更有效、更灵活的运用ActiveMQ.

Spring和ActiveMQ整合需要在项目中包含以下这几个jar包（缺一不可）：activeio-core-3.1.4.jar，activemq-all-5.13.2.jar，activemq-pool-5.13.2.jar，commons-pool2-2.4.2.jar，这些jar可以在ActiveMQ的安装包中的/lib/optional/下找到，或者从[这里](http://download.csdn.net/detail/u013256816/9492418)下载。

## 配置ConnectionFactory

ConnectionFactory是用于产生到JMS服务器的链接的，Spring为我们提供了多个ConnectionFactory，有SingleConnectionFactory和CachingConnectionFactory。SingleConnectionFactory对于建立JMS服务器链接的请求会一直返回同一个链接，并且会忽略Connection的close方法调用。CachingConnectionFactory继承了SingleConnectionFactory，所以它拥有SingleConnectionFactory的所有功能，同时它还新增了缓存功能，可以缓存Session,
 MessageProducer和MessageConsumer。

Spring提供的ConnectionFactory只是Spring用于管理ConnectionFactory的，真正产生到JMS服务器链接的ConnectionFactory还得是JMS服务厂商提供的，并且需要把它注入到Spring提供的ConnectionFactory中，这里使用ActiveMQ提供的ConnectionFactory，所以定义如下（10.10.195.187是博主的测试ip地址）：
<code class="hljs applescript has-numbering">    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"targetConnectionFactory"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.activemq.ActiveMQConnectionFactory"</span>>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"brokerURL"</span> value=<span class="hljs-string">"tcp://10.10.195.187:61616"</span> />
    </bean>
    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"connectionFactory"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.springframework.jms.connection.SingleConnectionFactory"</span>>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"targetConnectionFactory"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"targetConnectionFactory"</span>/>
    </bean></code>
ActiveMQ为我们提供了一个PooledConnectionFactory，通过往里面注入一个ActiveMQConnectionFactory可以用来将Connection, Session和MessageProducer池话，这样可以大大的减少我们的资源消耗。当使用PooledConnectionFactory时，我们在定义一个ConnectionFactory时这样定义：
<code class="hljs applescript has-numbering">    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"targetConnectionFactory"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.activemq.ActiveMQConnectionFactory"</span>>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"brokerURL"</span> value=<span class="hljs-string">"tcp://10.10.195.187:61616"</span> />
    </bean>
    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"poolConnectionFactory"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.activemq.pool.PooledConnectionFactory"</span>  >
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"connectionFactory"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"targetConnectionFactory"</span> />
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"maxConnections"</span> value=<span class="hljs-string">"10"</span>/>
    </bean>
    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"connectionFactory"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.springframework.jms.connection.SingleConnectionFactory"</span>>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"targetConnectionFactory"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"poolConnectionFactory"</span>/>
    </bean></code>
这里也可以去掉spring提供的SingleConnectionFactory,类似这样：
<code class="hljs applescript has-numbering">    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"targetConnectionFactory"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.activemq.ActiveMQConnectionFactory"</span>>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"brokerURL"</span> value=<span class="hljs-string">"tcp://10.10.195.187:61616"</span> />
    </bean>
    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"connectionFactory"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.activemq.pool.PooledConnectionFactory"</span>  >
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"connectionFactory"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"targetConnectionFactory"</span> />
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"maxConnections"</span> value=<span class="hljs-string">"10"</span>/>
    </bean></code>
## 配置消息发送（生产）者

配置好ConnectionFactory之后我们就需要配置生产者。生产者负责生产消息并发送到JMS服务器，这通常对应的是我们的一个业务逻辑服务实现类。但是我们的服务实现类是怎么进行消息的发送的呢？这通常是利用Spring为我们提供的JmsTemplate类来实现，所以配置生产者其实最核心的就是配置进行消息发送的JmsTemplate。对于消息发送者而言，它在发送消息的时候要知道自己该往哪里发，为此，我们在定义JmsTemplate的时候需要往里面注入一个Spring提供的ConnectionFactory对象。

spring配置文件中添加:
<code class="hljs applescript has-numbering">    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"jmsTemplate"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.springframework.jms.core.JmsTemplate"</span>>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"connectionFactory"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"poolConnectionFactory"</span>/>
    </bean></code>
在Java相关处理文件中添加(这里用的是@Inject注解，当然也可以用@Autowired)：
<code class="hljs java has-numbering">    <span class="hljs-annotation">@Inject</span>
    <span class="hljs-annotation">@Named</span>(<span class="hljs-string">"jmsTemplate"</span>)
    <span class="hljs-keyword">private</span> JmsTemplate jmsTemplate;</code>
这样就可以通过jmsTemplate对象来处理相关信息，譬如：
<code class="hljs avrasm has-numbering">    jmsTemplate<span class="hljs-preprocessor">.convertAndSend</span>(<span class="hljs-string">"sqlQueue"</span>,sql)<span class="hljs-comment">;</span></code>
真正利用JmsTemplate进行消息发送的时候，我们需要知道消息发送的目的地，即Destination。在Jms中有一个用来表示目的地的Destination接口，它里面没有任何方法定义，只是用来做一个标志而已。当我们在使用JmsTemplate进行消息发送时没有指定destination的时候将使用默认的Destination。默认Destination可以通过在定义jmsTemplate bean对象时通过属性defaultDestination或defaultDestinationName来进行诸如，defaultDestinationName对于的就是一个普通字符串。在ActiveMQ中实现了两种类型的Destination，一个是点对点的ActiveMQQueue,另一个就是支持订阅-发布模式的ActiveMQTopic。
<code class="hljs xml has-numbering">    <span class="hljs-tag"><<span class="hljs-title">bean</span> <span class="hljs-attribute">id</span>=<span class="hljs-value">"queueDestination"</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"org.apache.activemq.command.ActiveMQQueue"</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">constructor-arg</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">value</span>></span>sqlQueue<span class="hljs-tag"></<span class="hljs-title">value</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">constructor-arg</span>></span>
    <span class="hljs-tag"></<span class="hljs-title">bean</span>></span>
    <span class="hljs-tag"><<span class="hljs-title">bean</span> <span class="hljs-attribute">id</span>=<span class="hljs-value">"topicDestination"</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"org.apache.activemq.command.ActiveMQTopic"</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">constructor-arg</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">value</span>></span>topic<span class="hljs-tag"></<span class="hljs-title">value</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">constructor-arg</span>></span>
    <span class="hljs-tag"></<span class="hljs-title">bean</span></span></code><code>></code>
对面上面的那个例子可以在Java程序中添加：
<code class="hljs java has-numbering">    <span class="hljs-annotation">@Inject</span>
    <span class="hljs-annotation">@Named</span>(<span class="hljs-string">"queueDestination"</span>)
    <span class="hljs-keyword">private</span> Destination queueDestination;</code>
进而【jmsTemplate.convertAndSend(“sqlQueue”,sql);】可以改为【jmsTemplate.convertAndSend(queueDestination,sql);】

也可以这样修改jmsTemplate：
<code class="hljs applescript has-numbering">    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"jmsTemplate"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.springframework.jms.core.JmsTemplate"</span>>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"connectionFactory"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"connectionFactory"</span>/>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"defaultDestination"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"queueDestination"</span>/>
    </bean></code>
或者：
<code class="hljs applescript has-numbering">    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"jmsTemplate"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.springframework.jms.core.JmsTemplate"</span>>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"connectionFactory"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"connectionFactory"</span>/>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"defaultDestinationName"</span> value=<span class="hljs-string">"sqlQueue"</span>/>
    </bean></code>
进而在java程序中这样调用：jmsTemplate.convertAndSend(sql);

## 配置消息接收（消费）者

生产者往指定目的地Destination发送消息后，接下来就是消费者对指定目的地的消息进行消费了。那么消费者是如何知道有生产者发送消息到指定目的地Destination了呢？这是通过Spring为我们封装的消息监听容器MessageListenerContainer实现的，它负责接收信息，并把接收到的信息分发给真正的MessageListener进行处理。每个消费者对应每个目的地都需要有对应的MessageListenerContainer。对于消息监听容器而言，除了要知道监听哪个目的地之外，还需要知道到哪里去监听，也就是说它还需要知道去监听那个JMS服务器，这是通过在配置MessageConnectionFactory的时候往里面注入一个ConnectionFactory来实现的。所以我们在配置一个MessageListenerContainer的时候有三个属性必须指定，一个是表示从哪里监听的ConnectionFactory，一个是表示监听什么的Destination，一个是接收到消息以后进行消息处理的MessageListener.

Spring为我们听过了两种类型的MessageListenerContainer：SimpleMessageListenerContainer和DefaultMessageListenerContainer。MessageListenerContainer：SimpleMessageListenerContainer会在一开始的时候就创建一个会话Session和消费者Consumer，并且会适用标准的JMS的MessageConsumer.setMessageListener()方法注册监听器让JMS提供调用监听器的回调函数。它不会动态的适应运行时需要和参与外部的事务管理。兼容性方面，它非常接近于独立的JMS规范，但一般不兼容J2EE的JMS限制。大多数情况下，我们还是使用DefaultMessageListenerContainer，跟MessageListenerContainer：SimpleMessageListenerContainer想必，它会动态的适应运行时的需求，并且能够参与外部的事务管理。它很好的平衡了JMS提供者要求低，先进功能如事务参与和兼容J2EE环境。

spring配置文件中添加:
<code class="hljs applescript has-numbering">    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"queueDestination"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.activemq.command.ActiveMQQueue"</span>>
        <constructor-arg>
            <value>sqlQueue</value>
        </constructor-arg>
    </bean>
    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"listenerContainer"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.springframework.jms.listener.DefaultMessageListenerContainer"</span>>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"connectionFactory"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"poolConnectionFactory"</span> />
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"messageListener"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"jmsQueueReceiver"</span> />
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"destination"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"queueDestination"</span> />
    </bean></code>
其中的jmsQueueReceiver代码如下：
<code class="hljs java has-numbering"><span class="hljs-keyword">import</span> javax.jms.JMSException;
<span class="hljs-keyword">import</span> javax.jms.Message;
<span class="hljs-keyword">import</span> javax.jms.MessageListener;
<span class="hljs-keyword">import</span> javax.jms.TextMessage;

<span class="hljs-keyword">import</span> org.springframework.stereotype.Component;

<span class="hljs-annotation">@Component</span>(<span class="hljs-string">"jmsQueueReceiver"</span>)
<span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">JmsQueueReceiver</span> <span class="hljs-keyword">implements</span> <span class="hljs-title">MessageListener</span>
{</span>
    <span class="hljs-annotation">@Override</span>
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">onMessage</span>(Message messgae)
    {
        <span class="hljs-keyword">if</span>(messgae <span class="hljs-keyword">instanceof</span> TextMessage)
        {
            TextMessage textMessage = (TextMessage) messgae;
            <span class="hljs-keyword">try</span>
            {
                String text = textMessage.getText();
                System.out.println(<span class="hljs-string">"######################["</span>+text+<span class="hljs-string">"]######################"</span>);
            }
            <span class="hljs-keyword">catch</span> (JMSException e)
            {
                e.printStackTrace();
            }
        }
    }
}</code>
## 事务管理

Spring提供了一个JmsTransactionManager用于对JMS ConnectionFactory做事务管理。这将允许JMS应用利用Spring的事务管理特性。JmsTransactionManager在执行本地资源事务管理时将从指定的ConnectionFactory绑定一个ConnectionFactory/Session这样的配对到线程中。JmsTemplate会自动检测这样的事务资源，并对他们进行相应的操作。

在J2EE环境中，ConnectionFactory会赤化Connection和Session，这样这些资源将会在整个事务中被有效地重复利用。在一个独立的环境中，使用Spring的SingleConnectionFactory时所有的事务将公用一个Connection，但是每个事务将保留自己独立的Session.

JmsTemplate可以利用JtaTransactionManager和能够进行分布式的JMS ConnectionFactory处理分布式事务。

在Spring整合JMS的应用中，如果我们要进行本地的事务管理的话非常简单，只需要在定义对于的消息监听容器时指定其sessionTransacted属性为true(默认为false)：
<code class="hljs applescript has-numbering">    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"listenerContainer"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.springframework.jms.listener.DefaultMessageListenerContainer"</span>>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"connectionFactory"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"connectionFactory"</span> />
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"messageListener"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"jmsQueueReceiver"</span> />
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"destination"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"queueDestination"</span> />
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"sessionTransacted"</span> value=<span class="hljs-string">"true"</span>/>
    </bean></code>
这样JMS在进行消息监听的时候就会进行事务控制，当在接收消息时监听器执行失败时JMS就会对接收到的消息进行回滚。这里需要注意的是对于其他操作如数据库等访问不属于该事务控制。

可以在JmsQueueReceiver中修改一下代码从而检测到发送异常时是否会进行事务回滚：
<code class="hljs java has-numbering"><span class="hljs-annotation">@Component</span>(<span class="hljs-string">"jmsQueueReceiver"</span>)
<span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">JmsQueueReceiver</span> <span class="hljs-keyword">implements</span> <span class="hljs-title">MessageListener</span>
{</span>
    <span class="hljs-annotation">@Override</span>
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">onMessage</span>(Message messgae)
    {
        <span class="hljs-keyword">if</span>(messgae <span class="hljs-keyword">instanceof</span> TextMessage)
        {
            TextMessage textMessage = (TextMessage) messgae;
            <span class="hljs-keyword">try</span>
            {
                String text = textMessage.getText();
                System.out.println(<span class="hljs-string">"######################["</span>+text+<span class="hljs-string">"]######################"</span>);
                <span class="hljs-keyword">if</span>(<span class="hljs-keyword">true</span>)
                {
                    <span class="hljs-keyword">throw</span> <span class="hljs-keyword">new</span> RuntimeException(<span class="hljs-string">"Error"</span>);
                }
            }
            <span class="hljs-keyword">catch</span> (JMSException e)
            {
                e.printStackTrace();
            }
        }
    }
}</code>
你可以通过向Destination发送一条信息，通过JmsQueueReceiver处理后，发送异常进而事务混滚。可以通过[http://10.10.195.187:8161/admin/queues.jsp](http://10.10.195.187:8161/admin/queues.jsp)查看相关信息（ip地址是博主的ActiveMQ服务器所在id，根据实际情况修改）。

如果想要接收消息和数据库访问处于同一事务中，那么我们就可以配置一个外部的事务管理同时配置一个支持外部事务管理的消息监听容器（如DefaultMessageListenerContainer)。要配置这样一个参与分布式事务管理的消息监听容器，我们可以配置一个JtaTransactionManager，当然底层的JMS ConnectionFactory需要能够支持分布式事务管理，并正确地注册我们的JtaTransactionManager。这样消息监听器进行消息接收和对应的数据库访问就会处于同一数据库控制下，当消息接收失败或数据库访问失败都会进行事务回滚操作。
<code class="hljs applescript has-numbering">    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"listenerContainer"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.springframework.jms.listener.DefaultMessageListenerContainer"</span>>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"connectionFactory"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"connectionFactory"</span> />
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"messageListener"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"jmsQueueReceiver"</span> />
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"destination"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"queueDestination"</span> />
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"sessionTransacted"</span> value=<span class="hljs-string">"true"</span>/>
        <<span class="hljs-keyword">property</span> <span class="hljs-property">name</span>=<span class="hljs-string">"transactionManager"</span> <span class="hljs-keyword">ref</span>=<span class="hljs-string">"jtaTransactionManager"</span>/>
    </bean>
    <bean <span class="hljs-property">id</span>=<span class="hljs-string">"jtaTransactionManager"</span> <span class="hljs-type">class</span>=<span class="hljs-string">"org.springframework.transaction.jta.JtaTransactionManager"</span>/></code>
当给小时监听容器指定了transactionManager时，消息监听容器将忽略sessionTransacted的值。

到这里大概的Spring+ActiveMQ整合告一段落，所有代码经博主亲测，确保有效性及正确性。如有疑问，可在下方留言。

## 参考项目：

Spring和ActiveMQ集成实现队列消息以及PUB/SUB模型：[http://pan.baidu.com/s/1c0ms8mc](http://pan.baidu.com/s/1c0ms8mc)


## 参考资料： 

1. [Spring整合JMS(一)——基于ActiveMQ实现](http://haohaoxuexi.iteye.com/blog/1893038)

2. [Spring+ActiveMQ+Mysql 配置JMS](http://blog.csdn.net/wilsonke/article/details/42191015)

3. [ActiveMQ简述](http://blog.csdn.net/u013256816/article/details/51161548)







[](http://blog.csdn.net/u013256816/article/details/51161548)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=activemq&t=blog)](https://so.csdn.net/so/search/s.do?q=apache&t=blog)




