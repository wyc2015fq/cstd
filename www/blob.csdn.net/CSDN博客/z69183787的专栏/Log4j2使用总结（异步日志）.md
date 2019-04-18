# Log4j2使用总结（异步日志） - z69183787的专栏 - CSDN博客
2016年06月28日 15:34:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11020
TimeBased Triggering Policy
基于时间的触发策略。该策略主要是完成周期性的log文件封存工作。有两个参数：
interval，integer型，指定两次封存动作之间的时间间隔。单位:以日志的命名精度来确定单位，比如yyyy-MM-dd-HH 单位为小时，yyyy-MM-dd-HH-mm 单位为分钟
modulate，boolean型，说明是否对封存时间进行调制。若modulate=true，则封存时间将以0点为边界进行偏移计算。比如，modulate=true，interval=4hours，那么假设上次封存日志的时间为03:00，则下次封存日志的时间为04:00，之后的封存时间依次为08:00，12:00，16:00，。。。
存在问题:在Eclipse下一切运行正常，如果把应用打包成jar包发布后，cmd命令行模式下，即使Console开着的情况下，也没法输出，文件输出也没有任何日志。
问题已经解决，需要在MANIFEST.MF文件里Class-Path 最前加个'.',目的是让与jar包平级的配置文件log4j2.xml加载进来。
比如
Class-Path: . lib/antlr-2.7.6.jar lib/other.jar
这样，就把包含了log4j2.xml的路径放到了classpath,因此，log4j2能读取这个文件。感谢老外Jacob Kjome的回复！
log4j2 在Web的应用，把log4j2.xml放在src根目录下，不需要任何在代码级别的设置，直接类似JAVA应用一样使用，但把servlet改成了servlet3.0，因为官网上描述如下：
- The Short Story  
- Log4j 2 "just works" in Servlet 3.0 and newer web applications. It is capable of automatically starting when the application deploys and shutting down when the application undeploys.Important Note! For performance reasons, containers often ignore certain JARs known not to contain TLDs or ServletContainerInitializers and do not scan them for web-fragments and initializers. Importantly, Tomcat 7 <7.0.43 ignores all JAR files named log4j*.jar, which prevents this feature from working. This has been fixed in Tomcat 7.0.43, Tomcat 8, and later. In Tomcat 7 <7.0.43 you will need to change catalina.properties and remove "log4j*.jar" from the jarsToSkip property. You may need to do something similar on other containers if they skip scanning Log4j JAR files.
  
但发现在eclipse下servlet2.0
 也可以用的，并且不用修改catalina.properties文件。我用的是apache-tomcat-7.0.42，很奇怪！
有时候，为了同一份log4j文件要支持写到不同的log中（否则会导致打印的内容被覆盖，其中一个进程写不进log等问题），需要在载入的时候对内容进行动态修改，比如根据server id分别生成game1.log,game2.log
可以代码进行加载log4文件
```java
File file = new File("log4j2.xml");  
        BufferedInputStream in = null;  
        try {  
            in = new BufferedInputStream(new FileInputStream(file));  
            final ConfigurationSource source = new ConfigurationSource();  
            source.setInputStream(in);  
            Configurator.initialize(null, source);  
        } catch (FileNotFoundException e) {  
            e.printStackTrace();  
        }
```
要使用异步写日志的功能，必须引入[Disruptor](http://logging.apache.org/log4j/2.x/manual/async.html#UnderTheHood)
<asyncRoot> or <asyncLogger>
官方建议一般程序员查看的日志改成异步方式，一些运营日志改成同步
Asynchronous Appenders 和 Asynchronous Loggers 区别:
在</appenders> 节点里添加
  <Async name="Async">
      <AppenderRef ref="MyFile"/>
    </Async>
为Asynchronous Appenders 性能比同步快，比Asynchronous Loggers慢
在loggers节点添加
    <AsyncLogger name="com.foo.Bar" level="trace" includeLocation="true">
      <AppenderRef ref="RandomAccessFile"/>
    </AsyncLogger>
或者添加
<!-- Root Logger -->
<asyncRoot level="DEBUG">
<appender-ref ref="DevLog" />
<appender-ref ref="Console" />
</asyncRoot>
为logger async 用的是无锁并发技术，必须引入[Disruptor](http://logging.apache.org/log4j/2.x/manual/async.html#UnderTheHood)
测试了下，单线程异步比同步效率提高了1倍。线程越多性能提高越明显。
如果要加上位置信息比如哪个类，第几行，需要设置 includeLocation="true"
 但默认不设置好像也是true,
location 主要包含了如下:
If
 one of the layouts is configured with a location-related attribute like HTML [locationInfo](http://logging.apache.org/log4j/2.x/manual/layouts.html#HtmlLocationInfo),
 or one of the patterns [%C
 or $class](http://logging.apache.org/log4j/2.x/manual/layouts.html#PatternClass), [%F
 or %file](http://logging.apache.org/log4j/2.x/manual/layouts.html#PatternFile), [%l
 or %location](http://logging.apache.org/log4j/2.x/manual/layouts.html#PatternLocation), [%L
 or %line](http://logging.apache.org/log4j/2.x/manual/layouts.html#PatternLine), [%M
 or %method](http://logging.apache.org/log4j/2.x/manual/layouts.html#PatternMethod), Log4j will take a snapshot of the stack, and walk the stack trace to find the location information.
====================
log4j2最终输出点
AbstractOutputStreamAppender .append(final LogEvent event)
====================
以下要翻译
shutdownHook="disable"
|immediateFlush|boolean|When set to true - the default, each write will be followed by a flush. This will guarantee the data is written to disk but could impact performance.Flushing after every write is only useful when using this appender with synchronous loggers. Asynchronous loggers and appenders will automatically flush at the end of a batch of events, even if immediateFlush is set to false. This also guarantees the data is written to disk but is more efficient.|
|----|----|----|
|bufferSize|int|The buffer size, defaults to 262,144 bytes (256 * 1024).If an integer greater than 0, this causes the appender to buffer log events and flush whenever the buffer reaches this size.|
|AsyncLogger.RingBufferSize|256 * 1024|Size (number of slots) in the RingBuffer used by the asynchronous logging subsystem. Make this value large enough to deal with bursts of activity. The minimum size is 128. The RingBuffer will be pre-allocated at first use and will never grow or shrink during the life of the system.|
### Making All Loggers Asynchronous
*Requires disruptor-3.0.0.jar or higher on the classpath.*
This is simplest to configure and gives the best performance. To make all loggers asynchronous, add the disruptor jar to the classpath and set the system property Log4jContextSelector toorg.apache.logging.log4j.core.async.AsyncLoggerContextSelector.
By default, [location](http://logging.apache.org/log4j/2.x/manual/async.html#Location) is not passed to the I/O thread by asynchronous loggers. If one of your layouts
 or custom filters needs location information, you need to set "includeLocation=true" in the configuration of all relevant loggers, including the root logger.
A configuration that does not require location might look like:
```
<?xml version="1.0" encoding="UTF-8"?>
 
<!-- Don't forget to set system property
-DLog4jContextSelector=org.apache.logging.log4j.core.async.AsyncLoggerContextSelector
     to make all loggers asynchronous. -->
 
<Configuration status="WARN">
  <Appenders>
    <!-- Async Loggers will auto-flush in batches, so switch off immediateFlush. -->
    <RandomAccessFile name="RandomAccessFile" fileName="async.log" immediateFlush="false" append="false">
      <PatternLayout>
        <Pattern>%d %p %c{1.} [%t] %m %ex%n</Pattern>
      </PatternLayout>
    </RandomAccessFile>
  </Appenders>
  <Loggers>
    <Root level="info" includeLocation="false">
      <AppenderRef ref="RandomAccessFile"/>
    </Root>
  </Loggers>
</Configuration>
```
When AsyncLoggerContextSelector is used to make
 all loggers asynchronous, make sure to use normal <root> and <logger> elements
 in the configuration. The AsyncLoggerContextSelector will ensure that all loggers are asynchronous, using a mechanism that is different from what happens when you configure <asyncRoot> or <asyncLogger>.
 The latter elements are intended for mixing async with sync loggers. If you use both mechanisms together you will end up with two background threads, where your application passes the log message to thread A, which passes the message to thread B, which then
 finally logs the message to disk. This works, but there will be an unnecessary step in the middle.
There are a few system properties you can use to control aspects of the asynchronous logging subsystem. Some of these can be used to tune logging performance.
