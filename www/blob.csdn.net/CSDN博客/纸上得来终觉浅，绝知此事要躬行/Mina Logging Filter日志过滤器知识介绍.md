# Mina Logging Filter日志过滤器知识介绍 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年06月16日 10:24:17[boonya](https://me.csdn.net/boonya)阅读数：3680
所属专栏：[Apache Mina学习实践](https://blog.csdn.net/column/details/i-love-mina.html)









原文地址：[Mina Logging Filter（Apache
 Mina User Guide Chapter12 Logging Filter）](http://mina.apache.org/mina-project/userguide/ch12-logging-filter/ch12-logging-filter.html)

### 1.1.1. **背景**

Apache MINA使用一个允许基于MINA应用程序的开发人员使用他们自己的日志系统。

#### 1.1.1.1. **SLF4J**

Mina使用简单的Java日志Facade(SLF4J)。你可以在这里找到SLF4J信息。这个日志记录工具允许任意数量的日志系统的实现。你可以使用log4j,java.util.logging或其他日志系统。好的部分是,如果你想改变java.util.logging记录log4j后来在开发过程中，你不需要改变你的源代码。

##### 1.1.1.1.1. **选择正确的JARS**

SLF4J使用静态绑定。这意味着有一个为每个支持的日志框架JAR文件。你可以用你最喜欢的日志框架通过选择JAR文件调用静态日志框架你选择。下面是所需的JAR文件的表使用一个特定的日志框架。
|**Logging framework**|**Required JARs**|
|----|----|
|Log4J 1.2.x|**sl**f4j-api.jar**, **slf4j-log4j12.jar**|
|Log4J 1.3.x|**slf4j-api.jar**, **slf4j-log4j13.jar**|
|java.util.logging|slf4j-api.jar**, **slf4j-jdk14.jar**|
|Commons Logging|**slf4j-api.jar**, **slf4j-jcl.jar**|



这里有几件事要记住:
- slf4j-api.jar通常被使用在任何实现jar。
- 重要的是：你不应该把多个实现的JAR文件的类路径(例如slf4j-log4j12。jar和slf4j-jdk14.jar);它可能导致应用程序异常行为。　　
- slf4j-api.jar的版本和slf4j -.jar应该是相同的。

一旦正确配置后,您可以继续配置实际的日志框架你选择(如修改log4j
 . properties)。

##### 1.1.1.1.2. **重载Jakarta普通日志**

SLF4J还提供了一种方式可以将现有的应用程序,使用Jakarta Commons Logging SLF4J不改变应用程序代码。只是删除commons-loggong
 JAR文件的类路径,并添加jcl104-over-slf4j.jar的类路径。

#### 1.1.1.2. **Log4j例子**

对于这个示例,我们将使用log4j日志记录系统。我们建立了一个项目,并将以下代码片段为一个名为log4j
 . properties的文件:

```
<span style="font-size:12px;"># Set root logger level to DEBUG and its only appender to A1.log4j.rootLogger=DEBUG, A1

# A1 is set to be a ConsoleAppender.log4j.appender.A1=org.apache.log4j.ConsoleAppender

# A1 uses PatternLayout.log4j.appender.A1.layout=org.apache.log4j.PatternLayoutlog4j.appender.A1.layout.ConversionPattern=%-4r [%t] %-5p %c{1} %x - %m%n</span>
```

这个文件将被放置在我们的项目的src目录。如果您正在使用一个IDE，当你测试你的代码时，您基本上希望配置文件在JVM的类路径中。

虽然这向您展示了如何建立一个IoAcceptor使用日志,了解SLF4J
 API可以使用在你的程序以生成适当的日志信息以适合您的需要。

接下来我们将设置一个简单的示例服务器为了生成一些日志。这里我们有了EchoServer示例项目和日志记录添加到类:

```java
<span style="font-size:12px;">public static void main(String[] args) throws Exception {

    IoAcceptor acceptor = new SocketAcceptor();

    DefaultIoFilterChainBuilder chain = acceptor.getFilterChain();

LoggingFilter loggingFilter = new LoggingFilter();

    chain.addLast("logging", loggingFilter);

 

    acceptor.setLocalAddress(new InetSocketAddress(PORT));

    acceptor.setHandler(new EchoProtocolHandler());

    acceptor.bind();

 

    System.out.println("Listening on port " + PORT);}</span>
```



正如你所看到的我们移除addLogger方法和添加两行添加到EchoServer示例。LoggingFilter的引用,您可以设置日志级别/事件类型与IoAcceptor相关联的处理程序。为了指定IoHandler触发的事件日志和日志执行级别，有一个方法LoggingFilter调用setLogLevel(IoEventType
 LogLevel)。下面是这个方法的选项:


|**IoEventType**|**Description**|
|----|----|
|SESSION_CREATED|当创建了一个新的会话时被调用|
|SESSION_OPENED|当一个新的会话已被打开时被调用|
|SESSION_CLOSED|当一个会话已经关闭时被调用|
|MESSAGE_RECEIVED|当数据已经收到时被调用|
|MESSAGE_SENT|当一个消息被发送时被调用|
|SESSION_IDLE|当一个会话空闲时被调用|
|EXCEPTION_CAUGHT|当异常被抛出时被调用|

这里是日志级别的描述：
|**LogLevel**|**Description**|
|----|----|
|NONE|这将导致没有日志事件无论创建配置|
|TRACE|创建一个跟踪事件的日志系统|
|DEBUG|生成日志记录系统的调试消息|
|INFO|生成日志系统的信息消息|
|WARN|生成日志系统的警告消息|
|ERROR|在日志系统生成错误消息|

根据这些信息,您应该能够得到一个基本系统启动和运行,并能够扩展这个简单的例子为您的系统产生日志信息。



