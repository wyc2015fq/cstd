# Java工程中使用Log4j小记 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月20日 11:11:26[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2873








1、工程中引入log4j的jar包，可到http://logging.apache.org/log4j/1.2/download.html下载；




2、在src目录下配置log4j.properties文件，可到官网参考配置，这里简单配置了debug和error输出到文件，并按照日期分开。

     log4j.properties文件可代码中通过PropertyConfigurator.configure指定路径



```
### set log levels ###
log4j.rootLogger = debug , D , E

### 输出到日志文件 ##### 输出DEBUG级别以上的日志
log4j.appender.D = org.apache.log4j.DailyRollingFileAppender
log4j.appender.D.File = d:\\tmp\\debug.log
log4j.appender.D.Append = true
log4j.appender.D.Threshold = DEBUG 
log4j.appender.D.layout = org.apache.log4j.PatternLayout
log4j.appender.D.layout.ConversionPattern = %-d{yyyy-MM-dd HH:mm:ss} [ %t:%r ] - [ %p ] %m%n

### 保存异常信息到单独文件 ##### 只输出ERROR级别以上的日志!!!
log4j.appender.E = org.apache.log4j.DailyRollingFileAppender
log4j.appender.E.File = d:\\tmp\\error.log
log4j.appender.E.Append = true
log4j.appender.E.Threshold = ERROR 
log4j.appender.E.layout = org.apache.log4j.PatternLayout
log4j.appender.E.layout.ConversionPattern = %-d{yyyy-MM-dd HH:mm:ss} [ %t:%r ] - [ %p ] %m%n
```
3、代码中使用，打包并执行，java -jar TestLog4j.jar，d:\\tmp路径下有log文件并查看输出内容



```java
package ct.tool;

import org.apache.log4j.Logger;

public class TestLog4j {
	 public static void main(String[] args) {
	        Logger logger = Logger.getLogger(TestLog4j. class );
	        logger.debug( " debug " );
	        logger.error( " error " );
	    } 
}
```






