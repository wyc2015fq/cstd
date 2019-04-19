# spring mvc打印信息 - BlueSky - CSDN博客
2016年01月15日 15:38:19[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：1122
平常为了测试会在项目程序中打印一些信息但是，如果用System.out.println（）；会消耗很多资源因此，可以用以下的方式来打印帮助。
配置好Spring MVC关于log4j.properties，它的内容为以下：
```
### set log levels ###
#log4j.rootLogger = debug , stdout , D , E
log4j.rootLogger = info , stdout , D
 
###  output to the console ###
log4j.appender.stdout = org.apache.log4j.ConsoleAppender
log4j.appender.stdout.Target = System.out
log4j.appender.stdout.layout = org.apache.log4j.PatternLayout
#log4j.appender.stdout.layout.ConversionPattern = %d{ABSOLUTE} %5p %c{ 1 }:%L - %m%n
log4j.appender.stdout.layout.ConversionPattern = %-d{yyyy-MM-dd HH:mm:ss} [%c]-[%p] %m%n
 
### Output to the log file ###
log4j.appender.D = org.apache.log4j.DailyRollingFileAppender
log4j.appender.D.File = ${webapp.root}/WEB-INF/logs/error.log 
log4j.appender.D.Append = true
log4j.appender.D.Threshold = ERROR 
log4j.appender.D.layout = org.apache.log4j.PatternLayout
log4j.appender.D.layout.ConversionPattern = %-d{yyyy-MM-dd HH:mm:ss} [ %t:%r ] - [ %p ] %m%n
```
`<span style="font-size:18px;">然后，在定义</span>`
```
<span style="font-size:18px;">private static Logger _logger=Logger.getLogger(LoneUrlController.class);
</span>
```
`<span style="font-size:18px;">再使用就可以了，</span>`
```
<span style="font-size:18px;">_logger.info("想要打印的内容");</span>
```
