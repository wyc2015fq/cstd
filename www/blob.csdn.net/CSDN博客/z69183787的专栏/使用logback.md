# 使用logback - z69183787的专栏 - CSDN博客
2014年06月12日 19:44:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1528
1 一定要使用slf4j的jar包，不要使用apache commons的jar。否则滚动生成文件不生效，不滚动的时候却生效~~
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
2  举例子说吧
scan    属性设置为true时，配置文件如果发生改变，将会被重新加载，默认值为true。
scanPeriod   设置监测配置文件是否有修改的时间间隔，如果没有给出时间单位，默认单位是毫秒。当scan为true时，此属性生效。默认的时间间隔为1分钟。
debug    当此属性设置为true时，将打印出logback内部日志信息，实时查看logback运行状态。默认值为false。
contextName 标签   每个logger都关联到logger上下文，默认上下文名称为“default”。但可以使用<contextName>设置成其他名字，用于区分不同应用程序的记录。一旦设置，不能修改。
**[html]**[view
 plain](http://blog.csdn.net/arkblue/article/details/7772971#)[copy](http://blog.csdn.net/arkblue/article/details/7772971#)
- <configurationscan="true"scanPeriod="60 seconds"debug="false">
- <contextName>myAppName</contextName>
- <!-- 其他配置省略-->
- </configuration>
例子1
**[html]**[view
 plain](http://blog.csdn.net/arkblue/article/details/7772971#)[copy](http://blog.csdn.net/arkblue/article/details/7772971#)
- <configuration>
- <appendername="STDOUT"class="ch.qos.logback.core.ConsoleAppender">
- <!-- encoder 默认配置为PatternLayoutEncoder -->
- <encoder>
- <pattern>%d{HH:mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n  
- </pattern>
- </encoder>
- </appender>
- 
- <rootlevel="INFO">
- <appender-refref="STDOUT"/>
- </root>
- </configuration>
%logger{36} 表示logger名字最长36个字符，否则按照句点分割。
logger{length}输出日志的logger名，可有一个整形参数，功能是缩短logger名，设置为0表示只输入logger最右边。
|Conversion specifier|Logger name|Result|
|----|----|----|
|%logger|mainPackage.sub.sample.Bar|mainPackage.sub.sample.Bar|
|%logger{0}|mainPackage.sub.sample.Bar|Bar|
|%logger{5}|mainPackage.sub.sample.Bar|m.s.s.Bar|
|%logger{10}|mainPackage.sub.sample.Bar|m.s.s.Bar|
|%logger{15}|mainPackage.sub.sample.Bar|m.s.sample.Bar|
|%logger{16}|mainPackage.sub.sample.Bar|m.sub.sample.Bar|
|%logger{26}|mainPackage.sub.sample.Bar|mainPackage.sub.sample.Bar|
%d{HH:mm:ss.SSS}输出日志的打印日志，模式语法与`java.text.SimpleDateFormat` 兼容。看上去%d就已经够好了~~
|Conversion Pattern|Result|
|----|----|
|%d|2006-10-20 14:06:49,812|
|%date|2006-10-20 14:06:49,812|
|%date{ISO8601}|2006-10-20 14:06:49,812|
|%date{HH:mm:ss.SSS}|14:06:49.812|
|%date{dd MMM yyyy ;HH:mm:ss.SSS}|20 oct. 2006;14:06:49.812|
%msg%n  其中msg表示打印输出的消息， %n表示换行
|**m / msg / message**|输出应用程序提供的信息。|
可选的格式修饰符位于“%”和转换符之间。第一个可选修饰符是**左对齐** 标志，符号是减号“-”；
接着是可选的**最小宽度** 修饰符，用十进制数表示。如果字符小于最小宽度，则左填充或右填充，默认是左填充（即右对齐），填充符为空格。
如果字符大于最小宽度，字符永远不会被截断。
|**t / thread**|输出产生日志的线程名。|
|**p / le / level**|输出日志级别。|
例子2
**[html]**[view
 plain](http://blog.csdn.net/arkblue/article/details/7772971#)[copy](http://blog.csdn.net/arkblue/article/details/7772971#)
- <appendername="MONITOR"
- class="ch.qos.logback.core.rolling.RollingFileAppender">
- <file>${jingwei.loggingRoot}/monitor.log</file>
- 
- <!-- Policy定义如何滚动，按文件大小滚动生成日志 -->
- <!-- 如果是按文件大小滚动生成日志，前面的file标签可省略，而使用fileNamePattern标签定义的名字 -->
- <rollingPolicyclass="ch.qos.logback.core.rolling.FixedWindowRollingPolicy">
- <fileNamePattern>${jingwei.loggingRoot}/monitor.%i.log.gz  
- </fileNamePattern>
- <!-- 归档日志的下标，替换fileNamePattern的%i，最多3个归档文件 -->
- <minIndex>1</minIndex>
- <maxIndex>3</maxIndex>
- </rollingPolicy>
- 
- <!-- triggeringPolicy定义什么时候滚动，下面是定义了文件大小超过100M的时候产生归档文件 -->
- <triggeringPolicy
- class="ch.qos.logback.core.rolling.SizeBasedTriggeringPolicy">
- <maxFileSize>100MB</maxFileSize>
- </triggeringPolicy>
- 
- <!-- append是否接着上次写文件结尾继续写，默认为true -->
- <append>true</append>
- <encoding>GBK</encoding>
- 
- <!-- layout，望文生义，就是定义格式的 -->
- <layoutclass="ch.qos.logback.classic.PatternLayout">
- <pattern>
- <![CDATA[
-                 %n%-4r [%d{yyyy-MM-dd HH:mm:ss}] - %X{method}  %-5level %logger{35} - %m%n
-                 ]]>
- </pattern>
- </layout>
- </appender>
- 
- <!-- name 属性表示匹配的logger类型前缀 -->
- <loggername="com.taobao.jingwei.monitor">
- <levelvalue="WARN"/>
- <!-- 引用的appender，类似于spring的ref -->
- <appender-refref="MONITOR"/>
- </logger>
**logger中有一个additivity属性，**作用在于 children-logger是否使用 rootLogger配置的appender进行输出。
false：表示只用当前logger的appender-ref。
true：表示当前logger的appender-ref和rootLogger的appender-ref都有效。
**[html]**[view
 plain](http://blog.csdn.net/arkblue/article/details/7772971#)[copy](http://blog.csdn.net/arkblue/article/details/7772971#)
- <!-- name 属性表示匹配的logger类型前缀 -->
- <loggername="com.taobao.jingwei.monitor">
- <levelvalue="WARN"/>
- <!-- 引用的appender，类似于spring的ref -->
- <appender-refref="MONITOR"/>
- </logger>
这样logger名字匹配com.taobao.jingwei.monitor的信息就只在MONITOR的appender输出，而不会在root logger中输出了
