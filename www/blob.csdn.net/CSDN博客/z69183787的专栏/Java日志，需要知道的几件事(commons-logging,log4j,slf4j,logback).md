# Java日志，需要知道的几件事(commons-logging,log4j,slf4j,logback) - z69183787的专栏 - CSDN博客
2014年06月11日 16:54:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1672
个人分类：[日志-Logback																[日志-Log4j&Log4j2](https://blog.csdn.net/z69183787/article/category/2175479)](https://blog.csdn.net/z69183787/article/category/2327177)
如果对于commons-loging、log4j、slf4j、LogBack等都已经非常清楚了，可以忽略本文。几次解决日志冲突问题时对这几个概念的简单总结，希望对这块基础没有理解透的同学能有所帮助，当然如果对这块有更深刻理解的同学，也贡献出自己的知识和见解。
**一、    概念**
**Commons-logging**:apache最早提供的日志的门面接口。避免和具体的日志方案直接耦合。类似于JDBC的api接口，具体的的JDBC driver实现由各数据库提供商实现。通过统一接口解耦，不过其内部也实现了一些简单日志方案。
**Log4j**:经典的一种日志解决方案。内部把日志系统抽象封装成Logger、appender、pattern等实现。我们可以通过配置文件轻松的实现日志系统的管理和多样化配置。
**Slf4j**: 全称为Simple Logging Facade for JAVA：java简单日志门面。是对不同日志框架提供的一个门面封装。可以在部署的时候不修改任何配置即可接入一种日志实现方案。和commons-loging应该有一样的初衷。个人感觉设从计上更好一些，没有commons那么多潜规则。同时有两个额外特点：
1. 能支持多个参数，并通过{}占位符进行替换，避免老写logger.isXXXEnabled这种无奈的判断，带来性能提升见：http://www.slf4j.org/faq.html#logging_performance。
2.OSGI机制更好兼容支持
一图胜千言，官网上的一个图：
![](http://dl.iteye.com/upload/attachment/426780/0c184c55-5f4f-305d-b019-04ce68389fd6.png)
从上图可以发现，选择还是很多的。
**Logback**: Lack作为一个通用可靠、快速灵活的日志框架，将作为Log4j的替代和SLF4J组成新的日志系统的完整实现。官网上称具有极佳的性能，在关键路径上执行速度是log4j的10倍，且内存消耗更少。
二、 **SLF4J 的几种实际应用模式--之一：SLF4J+Log4J**
SLF4J(Simple Logging Facade for Java) 是一个通用的日志框架，不能何以谓之 Facade(门面)，所扮眼的角色相当于 Jakarta Commons Logging。就像 JCL 需要底层的日志实现，如 Log4J、java.util.logging、Simple Logger 等来完成具体的信息输出，事实上基本总是 JCL+Log4J 那么一个绝配。SLF4J 的原旨也是能支持多种下层日志框架实现，但最好的日志实现仍然是 Log4J，所以本篇讲述 SLF4J 的第一种用法
 SLF4J+Log4J。
需要的配置文件和组件包，下面三个 jar 文件和一个 properties 文件都是要放在项目的 ClassPath 上。
1. slf4j-api-1.5.11.jar
2. slf4j-log4j12-1.5.11.jar
3. log4j-1.2.15.jar
4. log4j.properties(也可以是 log4j.xml，本例中用 log4j.propertes)
前两个包在 [http://www.slf4j.org/download.html](http://www.slf4j.org/download.html) 处下载，后一个包在[http://logging.apache.org/log4j/1.2/download.html](http://logging.apache.org/log4j/1.2/download.html) 下载，可能包文件名中的版本号有些差，不要紧。
log4j.properties 以前该是怎么写，现在还是怎么写，比如一个最简单的内容，只向控制台输出日志信息，如下：
**[xml]**[view
 plain](http://blog.csdn.net/kobejayandy/article/details/17335407#)[copy](http://blog.csdn.net/kobejayandy/article/details/17335407#)
- log4j.rootLogger=DEBUG,console  
- log4j.appender.console=org.apache.log4j.ConsoleAppender  
- log4j.appender.console.layout=org.apache.log4j.PatternLayout  
- log4j.appender.console.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss,SSS} [%c]-[%p] %m%n  
**三、 SLF4J 的几种实际应用模式--之一：SLF4J+Logback**
前面讲的 SLF4J 的用法之一是 SLF4J+Log4J，而这里要推出的组合是 SLF4J+LogBack。不用 Log4J？难道还有比 Log4J 更好的日志实现吗？是的，答案就是[LogBack](http://logback.qos.ch/)。假如你知道 LogBack 和 Log4J 是同出一位大师之手，你就不会觉得惊讶了。LogBack
 对 Log4J 进行了增强，并投入了 SLF4J 的怀抱，它还被作者认定为 Log4J 的继任者。
为什么叫做 Logback，我就不知道了，反正不要叫做 Brokeback 就行。这里是推崇用 Logback 替代 Log4J 的十几个理由：[Reasons to prefer logback over log4j](http://logback.qos.ch/reasonsToSwitch.html)，说的大至是更快；好测试；与
 SLF4J 关系紧；文档丰富；能自动加载配置文件；多个 JVM 写一个日志文件，或其他 I/O 错误时不影响程序执行；配置文件中加入条件控制；强大的日志过滤；更强的日志切分功能；自动压缩、删除日志文件；异常栈中更多的数据信息。
Logback 分为三个模块：logback-core，logback-classic，logback-access。logback-core 是核心；logback-classic 改善了 log4j，且自身实现了 SLF4J API，所以即使用 Logback 你仍然可以使用其他的日志实现，如原始的 Log4J，java.util.logging 等；logback-access 让你方便的访问日志信息，如通过 http 的方式。
还得提一点，Logback 能更好的放到 OSGI 环境中。好了，简单绍完了 Logback，就来看看具体怎么用它。
需要的配置文件和组件包，下面三个 jar 文件和一个 xml文件都是要放在项目的 ClassPath 上。
1. slf4j-api-1.5.11.jar
2. logback-core-0.9.20.jar
3. logback-classic-0.9.20.jar
4. logback.xml 或 logback-test.xml (类似于 log4j.properties/log4j.xml)
**四、SLF4J怎样定位到具体用哪个log**
在应用中，通过LoggerFactory类的静态getLogger()获取logger。通过查看该类的代码可以看出，最终是通过StaticLoggerBinder.SINGLETON.getLoggerFactory()方法获取LoggerFactory然后，在通过该具体的LoggerFactory来获取logger的。类org.slf4j.impl.StaticLoggerBinder并不在slf4j-api-1.5.2.jar包中，仔细查看每个与具体日志系统对应的jar包，就会发现，相应的jar包都有一个org.slf4j.impl.StaticLoggerBinder的实现，不同的实现返回与该日志系统对应的LoggerFactory，因此就实现了所谓的静态绑定，达到只要选取不同jar包就能简单灵活配置的目的。 
