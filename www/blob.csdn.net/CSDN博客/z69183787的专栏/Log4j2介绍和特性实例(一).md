# Log4j2介绍和特性实例(一) - z69183787的专栏 - CSDN博客
2016年12月12日 16:45:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1117
Log4j是Apache的著名项目，随着[Java](http://lib.csdn.net/base/javase)应用的越来越广泛，对日志性能等方面的要求也越来越高。Log4j的升级版本Log4j2在前些年发布。
在log4j2发布前，应经有了logback和SLF4J，功能也是非常强大，那么为什么作者还要发布log4j2呢？在《log4j-users-guide》对这个问题做了解释。
Its alternative, SLF4J/Logback made many needed improvements to the framework. So why bother with Log4j 2? Here are a few of the reasons.
下面是官方用户手册2.5版中，给出的说明。原文和自己的翻译都放在这：
1.Log4j 2 is designed to be usable as an audit logging framework. Both Log4j 1.x and Logback will lose events while reconfiguring. Log4j 2 will not. In Logback, exceptions in Appenders are never visible to the application. In Log4j
 2 Appenders can be configured to allow the exception to percolate to the application. (Log4j 2被设计成一个具有审计功能的日志框架。在配置更新时，Log4j 1.x and Logback对日志事件没有反应，Log4j 2则会正常工作。另外，Logback不允许Appenders中的异常是不可见的，而Log4j2可以配置成发送异常给应用程序。)
2. Log4j 2 contains next-generation lock-free Asynchronous Loggers based on the LMAX Disruptor library. In multi-threaded scenarios Asynchronous Loggers have 10 times higher throughput and orders of magnitude lower latency than Log4j 1.x and Logback. (主要说明Log4j2有强大的基于LMAX Disruptor
 library的异步日志功能。)
3. Log4j 2 uses a Plugin system that makes it extremely easy to extend the framework by adding new Appenders, Filters, Layouts, Lookups, and Pattern Converters without requiring any changes to Log4j. (基于插件的软件[架构](http://lib.csdn.net/base/architecture)，使得Log4j2更易于扩展，而不需会改变Log4j2自身的功能。也就是遵循了设计模式的一个重要原则，对修改封闭，对扩展开放。)
4. Due to the Plugin system configuration is simpler. Entries in the configuration do not require a class name to be specified. (基于插件的配置文件更加简单，配置文件中的实体不必指定一个类名。--这一点暂时没有搞清楚。)
5. Support for custom log levels. Custom log levels can be defined in code or in configuration. (支持用户自定义的日志等级，在代码或者配置文件中都可以定义。)
6. Support for lambda expressions. Client code running on [Java](http://lib.csdn.net/base/java)8 can use lambda
 expressions to lazily construct a log message only if the requested log level is enabled. Explicit level checks are not needed, resulting in cleaner code. (支持lambda表达式。--这是Java 8的新特性，不了解的同学可以查一下。)
7. Support for Message objects. Messages allow support for interesting and complex constructs to be passed through the logging system and be efficiently manipulated. Users are free to create their own Message types and write custom Layouts, Filters and Lookups
 to manipulate them. (支持消息对象。...。用户可以自由的创建他们自己的消息类型，写用户自定义的输出，过滤器和Lookups来操作这些消息类型。)
8. Log4j 1.x supports Filters on Appenders. Logback added TurboFilters to allow filtering of events before they are processed by a Logger. Log4j 2 supports Filters that can be configured to process events before they are handled by a Logger, as they are processed
 by a Logger or on an Appender. (比Log4j1和Logback更加全面的过滤器，Log4j2可以在Logger处理前，处理中和到Appender后都添加过滤器。)
9. Many Logback Appenders do not accept a Layout and will only send data in a fixed format. Most Log4j 2 Appenders accept a Layout, allowing the data to be transported in any format desired. (很多Logback Appenders不接收Layout参数，只用固定的格式发送数据。大多数的Log4j2的Appenders接收Layout参数，允许以任何预期的格式传送。)
10.Layouts in Log4j 1.x and Logback return a String. This resulted in the problems discussed at Logback Encoders. Log4j 2 takes the simpler approach that Layouts always return a byte array. This has the advantage that it means they can be used in virtually
 any Appender, not just the ones that write to an OutputStream. (Log4j1和Logback中的Layouts都返回String类型值。这导致了Logback Encoders中讨论的一些问题。Log4j2用一个简单的方法，Layouts返回字节数组。这使得Log4j2的Layouts可以用于任何Appender，而不只是使用OutputStream写日志的Appenders.)
11.The Syslog Appender supports both TCP and UDP as well as support for the BSD syslog and the RFC 5424 formats. (Syslog Appender即支持TCP和UDP，同时还支持BSD syslog和RFC 5424格式。)
12.Log4j 2 takes advantage of Java 5 concurrency support and performs locking at the lowest level possible. Log4j 1.x has known deadlock issues. Many of these are fixed in Logback but many Logback classes still require synchronization at a fairly high level.
 (Log4j2使用了Java 5对并发性的支持，而且可以在最低的等级执行枷锁操作。Log4j1中有很多死锁的问题。其中一些在Logback中得到了解决，但是很多Logback的类在相对高的层级，仍然需要同步保证。)
13.It is an Apache Software Foundation project following the community and support model used by all ASF projects. If you want to contribute or gain the right to commit changes just follow the path outlined at Contributing. (Log4j2是Apache的项目，所以同所有的Apache软件基金会的支持方案相同。
 按照Contributing路径下的步骤，就可以获得修改软件的许可。)
终于翻译完了，有些地方可能与原文本意有出入，欢迎大家指正：）。
后续的文章中，我会根据项目中Log4j2的使用，写一些例子和Log4j2新特性的实验。
