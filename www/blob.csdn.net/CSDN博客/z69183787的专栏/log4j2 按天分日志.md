# log4j2 按天分日志 - z69183787的专栏 - CSDN博客
2017年08月29日 12:24:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1685
[http://blog.csdn.net/zhang168/article/details/46814489](http://blog.csdn.net/zhang168/article/details/46814489)
log4j2的按天分日志文件
**[html]**[view
 plain](http://blog.csdn.net/zhang168/article/details/46814489#)[copy](http://blog.csdn.net/zhang168/article/details/46814489#)
- <RollingFilename="error_appender"fileName="${LOG_HOME}/error.log"filePattern="${LOG_HOME}/error-%d{yyyy-MM-dd}.log">
- <PatternLayoutpattern="%-d{yyyy-MM-dd HH:mm:ss} [%thread] %m%n"/>
- <Policies>
- <TimeBasedTriggeringPolicymodulate="true"interval="1"/>
- </Policies>
- </RollingFile>
按大小分
**[html]**[view
 plain](http://blog.csdn.net/zhang168/article/details/46814489#)[copy](http://blog.csdn.net/zhang168/article/details/46814489#)
- <RollingFilename="error_appender"fileName="${LOG_HOME}/error.log"filePattern="${LOG_HOME}/error-%d{yyyy-MM-dd}-%i.log.gz">
- <PatternLayoutpattern="%-d{yyyy-MM-dd HH:mm:ss} [%thread] %m%n"/>
- <SizeBasedTriggeringPolicysize="100 MB"/>
- </RollingFile>
按分钟分
**[html]**[view
 plain](http://blog.csdn.net/zhang168/article/details/46814489#)[copy](http://blog.csdn.net/zhang168/article/details/46814489#)
- <RollingRandomAccessFilename="_1min_appender"fileName="${MINUTE_HOME}/minute"filePattern="${MINUTE_HOME}/minute-%d{yyyy-MM-dd-HH-mm}.log">
- <PatternLayoutpattern="%m%n"/>
- <Policies>
- <TimeBasedTriggeringPolicyinterval="1"modulate="true"/>
- </Policies>
- </RollingRandomAccessFile>
关键点在于 filePattern后的日期格式，以及TimeBasedTriggeringPolicy的interval，日期格式精确到哪一位，interval也精确到哪一个单位
log4j2 设置同一类型日志文件个数
**[html]**[view
 plain](http://blog.csdn.net/zhang168/article/details/46814489#)[copy](http://blog.csdn.net/zhang168/article/details/46814489#)
- <Appenders>
- <Consolename="Console"target="SYSTEM_OUT">
- <PatternLayoutpattern="[%-5p] %d %c - %m%n"/>
- </Console>
- <Filename="File"fileName="dist/my.log">
- <PatternLayoutpattern="%m%n"/>
- </File>
- <RollingFilename="RollingFile"fileName="dist/my2.log"
- 
- filePattern="dist/$${date:yyyy-MM}/app-%d{MM-dd-yyyy}-%i.log.gz">
- <PatternLayoutpattern="[%-5p] %d %c - %m%n"/>
- <Policies>
- <TimeBasedTriggeringPolicy/>
- <SizeBasedTriggeringPolicysize="25 KB"/>
- </Policies>
- <DefaultRolloverStrategymax="20"/>
- </RollingFile>
- </Appenders>
关键在于 <DefaultRolloverStrategy max="20"/>，如果不做配置，默认是7，这个7指的是上面i的最大值，超过了就会覆盖之前的
