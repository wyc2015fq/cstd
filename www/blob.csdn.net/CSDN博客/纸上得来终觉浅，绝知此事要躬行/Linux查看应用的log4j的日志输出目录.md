# Linux查看应用的log4j的日志输出目录 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年05月25日 15:08:05[boonya](https://me.csdn.net/boonya)阅读数：5969








在Windows Tomcat下部署的log4j日志自动保存到Tomcat下的logs目录下，如果不熟悉Linux的日志输出方式，那么很有可能找不到对应的日志。这篇文章讲述的是如何去找Linux配置的应用输出日志。Linux下配置路径默认是系统路径。

## log4j.properties配置



```
log4j.rootLogger=INFO, Console,RollingFile, D
 
#Console
log4j.appender.Console=org.apache.log4j.ConsoleAppender
log4j.appender.Console.layout=org.apache.log4j.PatternLayout
log4j.appender.Console.layout.ConversionPattern=%d [%t] %-5p [%c] - %m%n
 
log4j.logger.java.sql.ResultSet=INFO
log4j.logger.org.apache=INFO
log4j.logger.java.sql.Connection=debug
log4j.logger.java.sql.Statement=debug
log4j.logger.java.sql.PreparedStatement=debug

### \u8f93\u51fa\u65e5\u5fd7\u5230\u6587\u4ef6 ### 
log4j.appender.RollingFile=org.apache.log4j.DailyRollingFileAppender
log4j.appender.RollingFile.File=logs/fmcgwms.log
log4j.appender.RollingFile.layout=org.apache.log4j.PatternLayout
log4j.appender.RollingFile.layout.ConversionPattern=%d [%t] %-5p [%c] - %m%n

### \u8f93\u51fa\u5f02\u5e38\u5230\u6307\u5b9a\u6587\u4ef6 ### 
log4j.appender.D = org.apache.log4j.DailyRollingFileAppender 
log4j.appender.D.File = logs/fmcgwms_error.log ## 
log4j.appender.D.Append = true 
log4j.appender.D.Threshold = ERROR 
log4j.appender.D.layout = org.apache.log4j.PatternLayout 
log4j.appender.D.layout.ConversionPattern = %d [%t] %-5p [%c] - %m%n  %m%n
```




## Windows Tomcat日志输出

Windows下采用的是Tomcat的相对路径，如：C:\App\Tomcat\apache-tomcat-7.0.62\logs,即${TOMCAT_HOME}/logs


![](https://img-blog.csdn.net/20170525150228108)





## Linux Tomcat输出日志

Linux下采用的路径是系统的绝对路径：跟上面log4j配置的路径一致
`logs/fmcgwms.log #相当于是/logs/fmcgwms.log`


![](https://img-blog.csdn.net/20170525150501750)




