# log4j日志配置 - z69183787的专栏 - CSDN博客
2013年11月20日 10:27:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：926标签：[log4j](https://so.csdn.net/so/search/s.do?q=log4j&t=blog)
个人分类：[日志-Log4j&Log4j2](https://blog.csdn.net/z69183787/article/category/2175479)
**1、配置根Logger:**
log4j.rootLogger = [ level ] , appenderName, appenderName2
level：日志的级别，指定这条日志信息的重要性。分为ALL < DEBUG < INFO < WARN 一般常用的为 
DEBUG ， INFO ，WARN ，ERROR四种，分别对应Logger类的四种方法
debug(Object message ) ;
info(Object message ) ;
warn(Object message ) ;
error(Object message ) ;
如果设置级别为INFO，则优先级大于等于INFO级别（如：INFO、WARN、ERROR）的日志信息将可以被输出,
小于该级别的如：DEBUG将不会被输出
appenderName :就是指定日志信息输出目的地，比如（打印到控制台，输出到文件等）。同一条日志信息
可以配置多个输出目的地。
**2、配置log输出目的地:**
org.apache.log4j.ConsoleAppender（控制台）
org.apache.log4j.FileAppender（文件）
org.apache.log4j.DailyRollingFileAppender（每天产生一个日志文件）
org.apache.log4j.RollingFileAppender（文件大小到达指定尺寸的时候产生一个新的文件）
org.apache.log4j.WriterAppender（将日志信息以流格式发送到任意指定的地方）
**3、log信息的格式:**
org.apache.log4j.HTMLLayout（HTML表格形式）
org.apache.log4j.SimpleLayout（简单格式的日志，只包括日志信息的级别和指定的信息字符串 ，如:DEBUG - Hello）
org.apache.log4j.TTCCLayout（日志的格式包括日志产生的时间、线程、类别等等信息）
org.apache.log4j.PatternLayout（灵活地自定义日志格式）
当使用org.apache.log4j.PatternLayout来自定义信息格式时，可以使用
log4j.appender.A1.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss} %p -%m%n 来格式化信息
%c 输出所属类的全名，可写为 %c{Num} ,Num类名输出的范围 如："com.sun.aaa.classB", %C{2}将使日志输出输出范围为：aaa.classB
%d 输出日志时间其格式为 可指定格式 如 %d{HH:mm:ss}等
%l 输出日志事件发生位置，包括类目名、发生线程，在代码中的行数
%n 换行符
%m 输出代码指定信息，如info(“message”),输出message
%p 输出日志的优先级，即 FATAL ,ERROR 等
%r 输出从启动到显示该条日志信息所耗费的时间（毫秒数）
%t 输出产生该日志事件的线程名
**4、同时使用commons-logging和Log4j**
1)首先在classpath下寻找自己的配置文件commons-logging.properties，如果找到，则使用其中定义的
Log实现类
2)如果找不到commons-logging.properties文件，则在查找是否已定义系统环境变量
org.apache.commons.logging.Log，找到则使用其定义的Log实现类
3)否则，查看classpath中是否有Log4j的包，如果发现，则自动使用Log4j作为日志实现类
4)否则，使用JDK自身的日志实现类（JDK1.4以后才有日志实现类）
5)否则，使用commons-logging自己提供的一个简单的日志实现类SimpleLog
**5.多个日志文件(log4j.rootLogger=INFO, A1 ,A2)**
#A2 输出到文件 RollingFileAppender的扩展，可以提供一种日志的备份功能。 
log4j.appender.A2=org.apache.log4j.RollingFileAppender 
#日志文件的名称 log4j.appender.A2.File=log4j.log 
#日志文件的大小 log4j.appender.A2.MaxFileSize=100KB 
#保存一个备份文件 log4j.appender.A2.MaxBackupIndex=1 
log4j.appender.A2.layout=org.apache.log4j.TTCCLayout 
#log4j.appender.A2.layout.ConversionPattern=%-d{yyyy-MM-dd HH:mm:ss} [%c]-[%p] %m%n
#log4j.appender.A2.layout.ConversionPattern=%5p [%t] (%F:%L) - %d - %m%n
**附例:(log4j.properties)**
例1:
log4j.rootLogger=DEBUG, stdout
log4j.appender.stdout=org.apache.log4j.ConsoleAppender
log4j.appender.stdout.layout=org.apache.log4j.PatternLayout
log4j.appender.stdout.layout.ConversionPattern=%c{1} - %m%n
例2:
#指定根Logger，及日志输出级别，大于等于该级别的日志将被输出（ DEBUG < INFO < WARN < ERROR < FATAL ） 设为OFF可以关闭日志 
log4j.rootLogger=DEBUG, A1,A2 ,A3
#指定log输出目的,这里设为输出日志到指定目录的文件my.log中 
log4j.appender.A1=org.apache.log4j.FileAppender 
log4j.appender.A1.File=\\logs\\my.log #当前根目录下 
#指定日志信息的格式 
log4j.appender.A1.layout=org.apache.log4j.PatternLayout 
log4j.appender.A1.layout.ConversionPattern=%r %d{yyyy-MM-dd HH:mm:ss} %c %p -%m%n 
#把A2输出到控制台 
log4j.appender.A2=org.apache.log4j.ConsoleAppender 
log4j.appender.A2.layout=org.apache.log4j.SimpleLayout
#A3按天输出日志
log4j.appender.A3 =org.apache.log4j.DailyRollingFileAppender //按日输出
log4j.appender.A3.File=logs/my.log //文件位置
log4j.appender.A3.DatePattern='.'yyyy-MM-dd //文件格式
log4j.appender.A3.layout=org.apache.log4j.PatternLayout
log4j.appender.A3.layout.ConversionPattern=%d{yyyy-MM-dd HH:mm:ss,SSS} %5p %c{1}:%L - %m%n 
#还可以单独指定输出某个包的日志级别 
#log4j.logger.com.study.HelloLog4j=INFO
