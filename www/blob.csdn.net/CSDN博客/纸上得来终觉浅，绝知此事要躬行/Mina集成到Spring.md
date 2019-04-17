# Mina集成到Spring - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年07月14日 13:48:55[boonya](https://me.csdn.net/boonya)阅读数：3627
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Apache Mina ch17-spring-integration](http://mina.apache.org/mina-project/userguide/ch17-spring-integration/ch17-spring-integration.html)


本文演示了将Spring集成MINA应用。我写这篇文章在我的博客上,虽然放在这里,实际上这些信息属于博客空间。可以找到原始的附件：[Spring集成Mina](http://www.ashishpaliwal.com/blog/2008/11/integrating-apache-mina-with-spring/)

### **应用结构**

我们应当采取一个标准Mina应用构造：
- 一个handler
- 两个过滤器——Logging过滤器和一个ProtocolCodec过滤器
- NioDatagram 套接字


#### **初始化代码**

首先让我们看看代码。为简单起见,我们省略了无关代码。



```java
public void initialize() throws IOException {

    // Create an Acceptor
    NioDatagramAcceptor acceptor = new NioDatagramAcceptor();

    // Add Handler
    acceptor.setHandler(new ServerHandler());

    acceptor.getFilterChain().addLast("logging",
                new LoggingFilter());
    acceptor.getFilterChain().addLast("codec",
                new ProtocolCodecFilter(new SNMPCodecFactory()));

    // Create Session Configuration
    DatagramSessionConfig dcfg = acceptor.getSessionConfig();
        dcfg.setReuseAddress(true);
        logger.debug("Starting Server......");
        // Bind and be ready to listen
        acceptor.bind(new InetSocketAddress(DEFAULT_PORT));
        logger.debug("Server listening on "+DEFAULT_PORT);
}
```

#### 集成处理

集成Spring我们需要做如下操作：
- 设置Io handler

- 创建过滤器添加到过滤器链
- 创建Socket并设置Socket参数

注:最新的MINA版本没有指定的Spring包,倾向于Spring早期版本。包现在被叫作集成Beans,即所有DI框架的实现工作。

让我们看看Spring xml文件。请注意我已经从xml删除了通用部分,只把具体事情需要打开实现。这个例子来自聊天示例附带Mina
 发布版。请参考附带的xml聊天例子。

现在让我们把东西放在一起。让我们在Spring上下文中设置Io处理器（handler）。



```
<!-- The IoHandler implementation -->
<bean id="trapHandler" class="com.ashishpaliwal.udp.mina.server.ServerHandler">
```
让我们创建过滤器链：



```
<bean id="snmpCodecFilter" class="org.apache.mina.filter.codec.ProtocolCodecFilter">
  <constructor-arg>
    <bean class="com.ashishpaliwal.udp.mina.snmp.SNMPCodecFactory" />
  </constructor-arg>
</bean>

<bean id="loggingFilter" class="org.apache.mina.filter.logging.LoggingFilter" />

<!-- The filter chain. -->
<bean id="filterChainBuilder" class="org.apache.mina.core.filterchain.DefaultIoFilterChainBuilder">
  <property name="filters">
    <map>
      <entry key="loggingFilter" value-ref="loggingFilter"/>
      <entry key="codecFilter" value-ref="snmpCodecFilter"/>
    </map>
  </property>
</bean>
```
在这里,我们创建我们的IoFilter的实例。看ProtocolCodec工厂,我们使用构造函数注入。日志过滤器直接创建。一旦我们完成bean定义的过滤,我们现在创建过滤器链用于实现。我们定义了一个bean
 id为“FilterChainBuidler”和添加定义的过滤器。我们几乎准备好了,我们只需要创建套接字和调用绑定。
允许完成创建套接字和完成的最后一部分链：



```
<bean class="org.springframework.beans.factory.config.CustomEditorConfigurer">
    <property name="customEditors">
      <map>
        <entry key="java.net.SocketAddress">
          <bean class="org.apache.mina.integration.beans.InetSocketAddressEditor" />
        </entry>
      </map>
    </property>
</bean>

<!-- The IoAcceptor which binds to port 161 -->
<bean id="ioAcceptor" class="org.apache.mina.transport.socket.nio.NioDatagramAcceptor" init-method="bind" destroy-method="unbind">
  <property name="defaultLocalAddress" value=":161" />
  <property name="handler" ref="trapHandler" />
  <property name="filterChainBuilder" ref="filterChainBuilder" />
</bean>
```
现在我们创建ioAcceptor,设置输入输出处理程序和过滤器链。现在我们要使用Spring编写一个函数来读取这个文件并启动我们的应用程序。这是代码：

```java
public void initializeViaSpring() throws Exception {
    new ClassPathXmlApplicationContext("trapReceiverContext.xml");
}
```




