# log4j2应用详解 - 零度的博客专栏 - CSDN博客
2017年10月11日 11:38:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：853
### 1、log4j2使用
这里采用slf4j和log4j2集成的方式。
一，所需jar包或依赖：
slf4j-api
log4j-api
log4j-core
log4j-slf4j-impl(集成包)
log4j-web(web项目需要)
注意：最新的log4j2所需要的jdk版本比较高，由于这里采用的jdk1.6，所以版本最高只能使用2.3的，这里使用的具体版本如下：
slf4j-api-1.7.25.jar
log4j-api-2.3.jar
log4j-core-2.3.jar
log4j-slf4j-impl-2.3.jar
log4j-web-2.3.jar
二，log4j的配置文件：
之前log4j的1.x版本支持properties配置文件的方式，但是在log4j2这里，只支持xml配置文件的方式，或者json,jsn后缀的文件。
系统选择配置文件的优先级(从先到后)如下：
(1).classpath下的名为log4j2-test.json 或者log4j2-test.jsn的文件.
(2).classpath下的名为log4j2-test.xml的文件.
(3).classpath下名为log4j2.json 或者log4j2.jsn的文件.
(4).classpath下名为log4j2.xml的文件.
我们一般默认使用log4j2.xml进行命名。如果本地要测试，可以把log4j2-test.xml放到classpath，而正式环境使用log4j2.xml，则在打包部署的时候不要打包log4j2-test.xml即可。
log4j2.xml配置文件默认配置为：
- <?xml version="1.0" encoding="UTF-8"?>
- <Configurationstatus="WARN">
- <Appenders>
- <Consolename="Console"target="SYSTEM_OUT">
- <PatternLayoutpattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n"/>
- </Console>
- </Appenders>
- <Loggers>
- <Rootlevel="error">
- <AppenderRefref="Console"/>
- </Root>
- </Loggers>
- </Configuration>
下面是一个比较详细的配置：
<?xml version="1.0" encoding="UTF-8"?>
<Configurationstatus="INFO"monitorInterval="10">
<Properties>
<!-- 配置日志文件输出目录 -->
<Propertyname="LOG_DIR">/export/data/logs/myProject_web</Property>
</Properties>
<Appenders>
<Consolename="Console"target="SYSTEM_OUT">
<PatternLayoutpattern="[%d{yyyy-MM-dd HH:mm:ss}] [%5p] %l --> %m %n"/>
</Console>
<RollingFilename="debug_logger_appender"fileName="${LOG_DIR}/web_debug.log"
filePattern="${LOG_DIR}/web_debug.log.%d{yyyy-MM-dd}">
<!--控制台只输出level及以上级别的信息（onMatch），其他的直接拒绝（onMismatch）-->
<ThresholdFilterlevel="DEBUG"onMatch="ACCEPT"onMismatch="DENY"/>
<PatternLayoutpattern="[%d{yyyy-MM-dd HH:mm:ss}] [%5p] %l --> %m %n"/>
<Policies>
<TimeBasedTriggeringPolicymodulate="true"interval="1"/>
</Policies>
<DefaultRolloverStrategymax="10"/>
</RollingFile>
<RollingFilename="info_logger_appender"fileName="${LOG_DIR}/web_info.log"
filePattern="${LOG_DIR}/web_info.log.%d{yyyy-MM-dd}">
<!--控制台只输出level及以上级别的信息（onMatch），其他的直接拒绝（onMismatch）-->
<ThresholdFilterlevel="INFO"onMatch="ACCEPT"onMismatch="DENY"/>
<PatternLayoutpattern="[%d{yyyy-MM-dd HH:mm:ss}] [%5p] %l --> %m %n"/>
<Policies>
<TimeBasedTriggeringPolicymodulate="true"interval="1"/>
</Policies>
<DefaultRolloverStrategymax="10"/>
</RollingFile>
<RollingFilename="error_logger_appender"fileName="${LOG_DIR}/web_error.log"
filePattern="${LOG_DIR}/web_error.log.%d{yyyy-MM-dd}">
<!--控制台只输出level及以上级别的信息（onMatch），其他的直接拒绝（onMismatch）-->
<ThresholdFilterlevel="ERROR"onMatch="ACCEPT"onMismatch="DENY"/>
<PatternLayoutpattern="[%d{yyyy-MM-dd HH:mm:ss}] [%5p] %l --> %m %n"/>
<Policies>
<TimeBasedTriggeringPolicymodulate="true"interval="1"/>
</Policies>
<DefaultRolloverStrategymax="10"/>
</RollingFile>
</Appenders>
<Loggers>
<Rootlevel="INFO">
<AppenderRefref="Console"/>
<!--<AppenderRef ref="debug_logger_appender"/>-->
<AppenderRefref="info_logger_appender"/>
<AppenderRefref="error_logger_appender"/>
</Root>
<!--过滤掉spring的一些无用的DEBUG信息-->
<Loggername="org.springframework"level="ERROR"/>
</Loggers>
</Configuration>
三、java中如何使用log4j2
// log4j2方式
privatestaticfinal org.apache.logging.log4j.Logger logger = org.apache.logging.log4j.LogManager.getLogger(Test.class);
// slf4j方式
privatestaticfinal org.slf4j.Logger logger = org.slf4j.LoggerFactory.getLogger(Test.class);
这里要说明的一点是，使用.class为参数生成的logger，在配置文件中，与用全路径的包名或者类名，配置Logger标签相对应，例如：<Logger name="org.springframework" level="ERROR"/>。 如果参数是字符串，例如org.slf4j.LoggerFactory.getLogger("test")，对应的Logger标签配置为<Logger
 name="test" level="ERROR"/>。
### 2、XML配置详解
xml默认配置为：
<?xml version="1.0" encoding="UTF-8"?>
 2<Configurationstatus="WARN">
 3<Appenders>
 4<Consolename="Console"target="SYSTEM_OUT">
 5<PatternLayoutpattern="%d{HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n"/>
 6</Console>
 7</Appenders>
 8<Loggers>
 9<Rootlevel="error">
10<AppenderRefref="Console"/>
11</Root>
12</Loggers>
13</Configuration>
具体标签解释：
       (1).根节点Configuration有两个属性:status和monitorinterval,有两个子节点:Appenders和Loggers(表明可以定义多个Appender和Logger).
　　　　　　　status用来指定log4j本身的打印日志的级别.
　　　　　　　monitorinterval用于指定log4j自动重新配置的监测间隔时间，单位是s,最小是5s.
　    (2).Appenders节点，常见的有三种子节点:Console、RollingFile、File.
　　　　　 Console节点用来定义输出到控制台的Appender.
　　　　　　　    name:指定Appender的名字.
　　　　　　　    target:SYSTEM_OUT 或 SYSTEM_ERR,一般只设置默认:SYSTEM_OUT.
　　　　　　　    PatternLayout:输出格式，不设置默认为:%m%n.
　　　　　File节点用来定义输出到指定位置的文件的Appender.
　　　　　　　　name:指定Appender的名字.
　　　　　　　　fileName:指定输出日志的目的文件带全路径的文件名.
                 append:如果为false每次运行都会自动清空日志文件，常用于临时测试.
　　　　　　　　PatternLayout:输出格式，不设置默认为:%m%n.
　　　　　　　   RollingFile节点用来定义超过指定大小自动删除旧的创建新的的Appender.
　　　　　　　   name:指定Appender的名字.
　　　　　　　　fileName:指定输出日志的目的文件带全路径的文件名.
　　　　　　　　PatternLayout:输出格式，不设置默认为:%m%n.
　　　　　　　　filePattern:指定新建日志文件的名称格式.
　　　　　　　　Policies:指定滚动日志的策略，就是什么时候进行新建日志文件输出日志.
　　　　　　　   TimeBasedTriggeringPolicy:Policies子节点，基于时间的滚动策略，interval属性用来指定多久滚动一次，默认是1 hour。modulate=true用来调整时间：比如现在是早上3am，interval是4，那么第一次滚动是在4am，接着是8am，12am...而不是7am.
　　　　　　　　SizeBasedTriggeringPolicy:Policies子节点，基于指定文件大小的滚动策略，size属性用来定义每个日志文件的大小.（查阅多处资料显示，在低一些的版本中，存在bug，这个属性比起作用，比如这里使用的2.3版本）
　　　　　　　　DefaultRolloverStrategy:用来指定同一个文件夹下最多有几个日志文件时开始删除最旧的，创建新的(通过max属性，如果max属性不设置，则默认是同一个文件夹下最多7个文件)。
　       (3).Loggers节点，常见的有两种:Root和Logger.
　　　　　　　Root节点用来指定项目的根日志，如果没有单独指定Logger，那么就会默认使用该Root日志输出
　　　　　　　      level:日志输出级别，共有8个级别，按照从低到高为：All < Trace < Debug < Info < Warn < Error < Fatal < OFF.
　　　　　　　      AppenderRef：Root的子节点，用来指定该日志输出到哪个Appender.
　　　　　　　Logger节点用来单独指定日志的形式，比如要为指定包下的class指定不同的日志级别等。
　　　　　　　　　level:日志输出级别，共有8个级别，按照从低到高为：All < Trace < Debug < Info < Warn < Error < Fatal < OFF.
　　　　　　　　　name:用来指定该Logger所适用的类或者类所在的包全路径,继承自Root节点.
　　　　　　　　　AppenderRef：Logger的子节点，用来指定该日志输出到哪个Appender,如果没有指定，就会默认继承自Root.如果指定了，那么会在指定的这个Appender和Root的Appender中都会输出，此时我们可以设置Logger的additivity="false"只在自定义的Appender中进行输出。
　　(4).关于日志level.
　　　　　　共有8个级别，按照从低到高为：All < Trace < Debug < Info < Warn < Error < Fatal < OFF.
　　　　　　All:最低等级的，用于打开所有日志记录.
　　　　　　Trace:是追踪，就是程序推进以下，你就可以写个trace输出，所以trace应该会特别多，不过没关系，我们可以设置最低日志级别不让他输出.
　　　　　　Debug:指出细粒度信息事件对调试应用程序是非常有帮助的.
　　　　　　Info:消息在粗粒度级别上突出强调应用程序的运行过程.
　　　　　　Warn:输出警告及warn以下级别的日志.
　　　　　　Error:输出错误信息日志.
　　　　　　Fatal:输出每个严重的错误事件将会导致应用程序的退出的日志.
　　　　　   OFF:最高等级的，用于关闭所有日志记录.
（5）日志输出格式：
%d{yyyy-MM-dd HH:mm:ss, SSS} : 日志生产时间
                %p : 日志输出格式
                %c : logger的名称
                %m : 日志内容，即 logger.info("message")
                %n : 换行符
                %C : Java类名
                %L : 日志输出所在行数
                %M : 日志输出所在方法名
                hostName : 本地机器名
                hostAddress : 本地ip地址
### 3、log4j2异步日志使用
        log4j2支持日志的异步打印，日志异步输出的好处在于，使用单独的进程来执行日志打印的功能，可以提高日志执行效率，减少日志功能对正常业务的影响。异步日志在程序的classpath需要加载disruptor-3.0.0.jar或者更高的版本。Asynchronous
 Loggers是一个新增特性在Log4j 2 ，可以实现完全异步也可以和同步混合使用，还可以只异步化Appender，以提升系统性能，官方数据显示混合没有完全异步化效果好。
1,完全异步模式：
这种异步日志方式，不需要修改原来的配置文件，Logger仍然使用<root> and <logger>
只需要在主程序代码开头，加一句系统属性的代码： 
System.setProperty("Log4jContextSelector","org.apache.logging.log4j.core.async.AsyncLoggerContextSelector");
![](http://static.blog.csdn.net/images/save_snippets_01.png)
或者设置启动参数： 
-DLog4jContextSelector=org.apache.logging.log4j.core.async.AsyncLoggerContextSelector
![](http://static.blog.csdn.net/images/save_snippets_01.png)
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
</Configuration>这里需要注意的是，如果使用<asyncRoot> 或 <asyncLogger>，includeLocation="true"是必须要设置才会有类路径等一些信息打印出来。
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
