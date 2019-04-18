# Log4j2介绍和特性实例(二)--一个实例和初学注意事项(configuration标签中的 status 和 monitorInterval) - z69183787的专栏 - CSDN博客
2016年12月12日 16:45:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：934
上一篇对官方手册中给出的Log4j2新特性进行了翻译，本篇给出一个Log4j2使用的例子。
本例子包括XML配置文件，需要引用的Jar包，需要Import的文件和代码，这样大家对Log4j2的使用有一个直观的认识。
代码都是调试OK的。
1. XML配置文件默认可以放在源码src目录下，log4j2.xml：
**[html]**[view
 plain](http://blog.csdn.net/chenhaotong/article/details/50281675#)[copy](http://blog.csdn.net/chenhaotong/article/details/50281675#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!-- 主要配置为error日志与debug日志分别打印文件，errorLog按照分钟存档，debugLog按照日志文件大小存档，最多保存10个。 -->
- <configurationstatus="debug"monitorInterval="30">
- <Properties>
- <Propertyname="log.path">.</Property>
- </Properties>
- 
- <appenders>
- <!--Appender 1. 输出到Console，指定输出格式和过滤器等级为ERROR -->
- <spanstyle="white-space:pre"></span><Consolename="Console"target="SYSTEM_OUT">
- <PatternLayoutpattern="[%-5level][%d{yyyy-MM-dd HH:mm:ss}][%F:%L] - %m%n"/>
- <ThresholdFilterlevel="ERROR"onMatch="ACCEPT"onMismatch="DENY"/>
- </Console>
- 
- <!--Appender 2. 输出到滚动保存的文件, 触发保存日志文件的条件是日志文件大于3KB，只保存最新的10个日志-->
- <RollingFilename="debugLog"fileName="${log.path}/debug.log"filePattern="${log.path}/debug-%i.log">
- <ThresholdFilterlevel="DEBUG"onMatch="ACCEPT"onMismatch="DENY"/>
- <PatternLayoutpattern="[%-5level][%d{yyyy-MM-dd HH:mm:ss}][%F:%L] - %m%n"/>
- <SizeBasedTriggeringPolicysize="3KB"/>
- <!-- DefaultRolloverStrategy 中的参数max，可以限制 SizeBasedTriggeringPolicy中size超出后，只保留max个存档-->
- <DefaultRolloverStrategymax="10"/>
- </RollingFile>
- 
- <!--Appender 3. 输出到滚动保存的文件, 触发保存日志文件的条件是每分钟第一次的日志事件。ERROR日志是按分钟产生日志 -->
- <RollingFilename="errorLog"fileName="${log.path}/error.log"filePattern="${log.path}/error-%d{yyyy-MM-dd_HH-mm}.log">
- <ThresholdFilterlevel="ERROR"onMatch="ACCEPT"onMismatch="DENY"/>
- <PatternLayoutpattern="[%-5level][%d{yyyy-MM-dd HH:mm:ss}][%C:%F:%L] - %m%n"/>
- <TimeBasedTriggeringPolicy/>
- </RollingFile>
- 
- </appenders>
- 
- <loggers>
- <rootlevel="trace">
- <appender-refref="Console"/>
- <appender-refref="debugLog"/>
- <appender-refref="errorLog"/>
- </root>
- </loggers>
- </configuration>
XML配置文件的结构就是在logger下可以关联一个到多个appender，即输出的目的地。在[Java](http://lib.csdn.net/base/javase)代码中则可以通过logger去输出日志。
下面是是用户手册中给出的类图，其中Configuration指的就是XML配置文件。
2. 在Java Build Path需要引入external jar包：
log4j-api-2.5.jar
log4j-core-2.5.jar
3. Java[测试](http://lib.csdn.net/base/softwaretest)代码：
**[java]**[view
 plain](http://blog.csdn.net/chenhaotong/article/details/50281675#)[copy](http://blog.csdn.net/chenhaotong/article/details/50281675#)
- package com.ocean.log4j2;  
- 
- import org.apache.logging.log4j.LogManager;  
- import org.apache.logging.log4j.Logger;  
- 
- publicclass Log4j2Testing {  
- 
- privatestatic Logger logger = LogManager.getLogger(LogManager.ROOT_LOGGER_NAME);  
- 
- publicstaticvoid main(String[] args) {  
- // TODO Auto-generated method stub
- 
- for(int i=0; i<10; i++){  
-             logger.trace("===root trace");  
-             logger.debug("===root debug");  
-             logger.info("===root info");  
-             logger.warn("===root warn");  
-             logger.error("===root error");  
-             logger.fatal("===root fatal");  
- 
- try {  
-                 Thread.sleep(10000);//等10秒钟
-             } catch (InterruptedException e) {  
- // TODO Auto-generated catch block
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- 
- }  
对于初次使用Log4j2的朋友，有两点需要注意：
1）在调试的过程中，可以将配置文件中，configuration标签(<configuration status="debug" monitorInterval="30">)中的status设置为debug，这样用户可以在Eclipse的console看到Log4j2启动和加载配置文件时的打印信息。
2）configuration标签(<configuration status="debug" monitorInterval="30">)中的monitorInterval是用来设置配置文件的动态加载时间的，单位是秒。monitorInterval="30"表示每30秒配置文件会动态加载一次。在程序运行过程中，如果修改配置文件，程序会随之改变。
后面的文章将介绍一些Log4j2特性的验证，包括日志文件存档压缩，异步日志，过滤器，自定义日志等级，XML配置文件自定义路径，日志文件动态命名等特性。敬请关注: )
