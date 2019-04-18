# log4j2日志异步打印 （3） - z69183787的专栏 - CSDN博客
2017年08月29日 20:18:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2361
                
log4j2支持日志的异步打印，日志异步输出的好处在于，使用单独的进程来执行日志打印的功能，可以提高日志执行效率，减少日志功能对正常业务的影响。
异步日志在程序的classpath需要加载disruptor-3.0.0.jar或者更高的版本。
Asynchronous Loggers是一个新增特性在Log4j 2 ，可以实现完全异步也可以和同步混合使用，还可以只异步化Appender，以提升系统性能，官方数据显示混合没有完全异步化效果好。
1,完全异步模式：
这种异步日志方式，不需要修改原来的配置文件，Logger仍然使用<root> and <logger>
只需要在主程序代码开头，加一句系统属性的代码：
System.setProperty("Log4jContextSelector","org.apache.logging.log4j.core.async.AsyncLoggerContextSelector");
或者设置启动参数：
-DLog4jContextSelector=org.apache.logging.log4j.core.async.AsyncLoggerContextSelector
2,异步和非异步混合输出模式
在配置文件中Logger使用<asyncRoot> 或 <asyncLogger>,而且<asyncRoot> 或 <asyncLogger>可以和<root> 或 <logger>混合使用。
<?xml version="1.0" encoding="UTF-8"?>
<!-- No need to set system property "Log4jContextSelector" to any value
when using <asyncLogger> or <asyncRoot>. -->
<Configurationstatus="WARN">
<Appenders>
<!-- Async Loggers will auto-flush in batches, so switch off immediateFlush. -->
<RandomAccessFilename="RandomAccessFile"fileName="asyncWithLocation.log"
immediateFlush="false"append="false">
<PatternLayout>
<Pattern>%d %p %class{1.} [%t] %location %m %ex%n</Pattern>
</PatternLayout>
</RandomAccessFile>
</Appenders>
<Loggers>
<!-- pattern layout actually uses location, so we need to include it -->
<AsyncLoggername="com.foo.Bar"level="trace"includeLocation="true">
<AppenderRefref="RandomAccessFile"/>
</AsyncLogger>
<Rootlevel="info"includeLocation="true">
<AppenderRefref="RandomAccessFile"/>
</Root>
</Loggers>
</Configuration>
这里需要注意的是，如果使用<asyncRoot> 或 <asyncLogger>，includeLocation="true"是必须要设置才会有类路径等一些信息打印出来。
3，只异步化Appender
在<Appenders>标签里增加如下<Async>标签
<Asyncname="asyncAppender"includeLocation="true">
<AppenderRefref="RandomAccessFile"/>
</Async>
然后在<Root>或者<Logger>标签中引用asyncAppender即可,这里includeLocation是增加在<Async>标签中的。
<Rootlevel="info">
<AppenderRefref="RandomAccessFile"/>
</Root>
无论是完全异步模式还是混合模式，在Appender标签中，immediateFlush属性无论为true或者false，效果都是和设置false是一样的。
4，性能对比
完全异步 > 混合模式 > 只异步化Appender > 同步
5，疑问
使用混合异步模式进行多线程写日志测试的时候，偶尔会出现日志没有写完的情况。是不是主线程执行完了，不会等待写日志的线程执行完，就把进程给停掉了？ 在主线程的最后sleep几秒，就没有再出现日志写不完的情况了。
