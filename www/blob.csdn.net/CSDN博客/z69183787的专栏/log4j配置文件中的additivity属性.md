# log4j配置文件中的additivity属性 - z69183787的专栏 - CSDN博客
2016年05月15日 15:31:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2020
最新写程序，需要采用log4j来收集日志，这样就需要把程序运行日志和收集的日志区分开来。
开始一直没有找到可以配置log4j让其独立输出到指定日志文件，于是采用了划分Log4j的等级来解决这个问题；
具体实施方案是将父Logger的输出等级设置的高一点，而用来收集日志的Logger的输出等级设置低一点，采用trace方法来打日志；
后来找到了log4j的additivity属性：
**它是 子Logger 是否继承 父Logger 的 输出源（appender） 的标志位。具体说，默认情况下子Logger会继承父Logger的appender，也就是说子Logger会在父Logger的appender里输出。若是additivity设为false，则子Logger只会在自己的appender里输出，而不会在父Logger的appender里输出。**
使用例子：下面是一个log4j的配置文件，父logger是console与R，子logger是log4j.logger.mobileLogger
log4j.rootLogger=DEBUG,console,R
log4j.appender.console=org.apache.log4j.ConsoleAppender
log4j.appender.console.layout=org.apache.log4j.PatternLayout
log4j.appender.console.layout.ConversionPattern=%-5p %d [%t] %c: %m%n
log4j.appender.R=org.apache.log4j.DailyRollingFileAppender
log4j.appender.R.File=/export/home/tomcat/mobile/tomcatLogs/tomcat.log
log4j.appender.R.DatePattern='.'yyyy-MM-dd
log4j.appender.R.layout=org.apache.log4j.PatternLayout
log4j.appender.R.layout.ConversionPattern=%-5p %d [%t] %c: %m%n
log4j.logger.mobileLogger=TRACE,mobileLogger
log4j.additivity.mobileLogger=false
log4j.appender.mobileLogger=org.apache.log4j.DailyRollingFileAppender
log4j.appender.mobileLogger.layout=org.apache.log4j.PatternLayout
log4j.appender.mobileLogger.DatePattern='.'yyyy-MM-dd-HH-mm
log4j.appender.mobileLogger.layout.ConversionPattern=%m%n
log4j.appender.mobileLogger.file=/export/home/tomcat/mobile/mobileLogs/mobile.log
log4j.appender.mobileLogger.encoding=UTF-8
