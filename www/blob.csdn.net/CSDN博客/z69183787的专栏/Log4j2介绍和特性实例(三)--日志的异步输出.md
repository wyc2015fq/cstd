# Log4j2介绍和特性实例(三)--日志的异步输出 - z69183787的专栏 - CSDN博客
2016年12月12日 16:47:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：895
上一篇介绍了一个Log4j2的使用实例，本编将介绍Log4j2的一个重要特性，日志异步输出。
日志异步输出的好处在于，使用单独的进程来执行日志打印的功能，可以提高日志执行效率，减少日志功能对正常业务的影响。
异步日志在程序的classpath需要加载disruptor-3.0.0.jar或者更高的版本。
异步日志分为两种：
a.全异步模式
这种异步日志方式，不需要修改修改原理的配置文件，Logger仍然使用<root> and <logger>
只需要在主程序代码开头，加一句系统属性的代码：
**[java]**[view
 plain](http://blog.csdn.net/chenhaotong/article/details/50411683#)[copy](http://blog.csdn.net/chenhaotong/article/details/50411683#)
- System.setProperty("Log4jContextSelector", "org.apache.logging.log4j.core.async.AsyncLoggerContextSelector");  
b.异步和非异步混合输出模式
在配置文件中Logger使用<asyncRoot> or <asyncLogger>
**[html]**[view
 plain](http://blog.csdn.net/chenhaotong/article/details/50411683#)[copy](http://blog.csdn.net/chenhaotong/article/details/50411683#)
- <loggers>
- <AsyncLoggername="AsyncLogger"level="trace"includeLocation="true">
- <appender-refref="Console"/>
- <appender-refref="debugLog"/>
- <appender-refref="errorLog"/>
- </AsyncLogger>
- 
- <asyncRootlevel="trace"includeLocation="true">
- <appender-refref="Console"/>
- </asyncRoot>
- </loggers>
用户手册中说全异步模式的性能要高于混合异步模式，但是如果Log4j2用作审计功能(Audit)的话，建议使用混合异步模式。
在异常导致程序退出的情况，担心Log4j2异步输出无法打印错误信息，也专门做了[测试](http://lib.csdn.net/base/softwaretest)。
从测试结果看，在程序异常退出前的所有日志都得到了保存。3,000,000条日志，全异步模式(All Logger Async)用了20分钟打印到日志文件，混合模式(Mixing)用了40分钟。
另外，Log4j2实现高性能的异步日志输出，主要是借助了LMAX Disruptor的开源框架，下面是Log4j2对其的描述，感兴趣的同学可以自己学习一下。
LMAX Disruptor technology. Asynchronous Loggers internally use the Disruptor, a lock-free inter-thread communication library, instead of queues, resulting in higher throughput
 and lower latency.
