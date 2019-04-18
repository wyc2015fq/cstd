# log4j2的使用详解（1） - z69183787的专栏 - CSDN博客
2017年08月29日 20:21:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1498
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
三，java代码中如何使用log4j2
// log4j2方式
privatestaticfinal org.apache.logging.log4j.Logger logger = org.apache.logging.log4j.LogManager.getLogger(Test.class);
// slf4j方式
privatestaticfinal org.slf4j.Logger logger = org.slf4j.LoggerFactory.getLogger(Test.class);
这里要说明的一点是，使用.class为参数生成的logger，在配置文件中，与用全路径的包名或者类名，配置Logger标签相对应，例如：<Logger name="org.springframework" level="ERROR"/>。 如果参数是字符串，例如
org.slf4j.LoggerFactory.getLogger("test")，对应的Logger标签配置为<Logger name="test" level="ERROR"/>。
四，web项目使用log4j2注意事项
参考官网和各种资料显示，web项目需要使用log4j-web的依赖，并且servlet3.0和servlet2.5的处理方式还有不同，不支持servlet2.4及以下版本。而servlet的版本使用，也是受到tomcat版本的约束。
下面是每个版本所支持的servlet和jsp版本对应表。
|Servlet Spec|JSP Spec|EL Spec|WebSocket Spec|Apache Tomcat version|Actual release revision|Support Java Versions|
|----|----|----|----|----|----|----|
|4.0|TBD (2.4?)|TBD (3.1?)|TBD (1.2?)|9.0.x|None|8 and later|
|3.1|2.3|3.0|1.1|8.0.x|8.0.15|7 and later|
|3.0|2.2|2.2|1.1|7.0.x|7.0.57|6 and later(WebSocket 1.1 requires 7 or later)|
|2.5|2.1|2.1|N/A|6.0.x|6.0.43|5 and later|
|2.4|2.0|N/A|N/A|5.5.x (archived)|5.5.36 (archived)|1.4 and later|
|2.3|1.2|N/A|N/A|4.1.x (archived)|4.1.40 (archived)|1.3 and later|
|2.2|1.1|N/A|N/A|3.3.x (archived)|3.3.2 (archived)|1.1 and later|
这里使用的tomcat6.0.33版本，支持的servlet版本2.5。（找到tomcat下lib的jsp-api.jar和servlet-api.jar两个jar包，分别用winrar这类的解压软件打开，里面的META-INF文件夹下的MANIFEST.MF文件，里面有相应的版本号）
官网给出，如果是servlet2.5，那么在添加log4j-web的依赖之后，需要在web.xml中进行如下配置，并且需要注意，都要放在其它的定义之前：
- <listener>
- <listener-class>org.apache.logging.log4j.web.Log4jServletContextListener</listener-class>
- </listener>
- 
- <filter>
- <filter-name>log4jServletFilter</filter-name>
- <filter-class>org.apache.logging.log4j.web.Log4jServletFilter</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>log4jServletFilter</filter-name>
- <url-pattern>/*</url-pattern>
- <dispatcher>REQUEST</dispatcher>
- <dispatcher>FORWARD</dispatcher>
- <dispatcher>INCLUDE</dispatcher>
- <dispatcher>ERROR</dispatcher>
- <dispatcher>ASYNC</dispatcher><!-- Servlet 3.0 w/ disabled auto-initialization only; not supported in 2.5 -->
- </filter-mapping>
但是这里测试，没有进行上面的配置，也没有添加log4j-web依赖，也能正常使用，打印日志，难道性能上面，文件资源的打开关闭上面有影响？
这里参考的文章：
https://logging.apache.org/log4j/2.x/manual/webapp.html
http://blog.csdn.net/sushengmiyan/article/details/52390211
http://www.cnblogs.com/yoyotl/p/4920073.html
另外，不同版本(2.3,2.4,2.5,3.0)的Servlet web.xml 头信息可以参考文章：
http://blog.csdn.net/z69183787/article/details/36008097
