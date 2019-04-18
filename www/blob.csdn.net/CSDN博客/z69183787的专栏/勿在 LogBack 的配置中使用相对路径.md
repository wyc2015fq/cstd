# 勿在 LogBack 的配置中使用相对路径 - z69183787的专栏 - CSDN博客
2014年06月12日 19:33:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：33127
请看下面这段配置，这是无法工作的：
<?xml version="1.0" encoding="UTF-8" ?>
<configuration>
    <contextName>JTheque</contextName>
 
    <appender name="FILE" class="ch.qos.logback.core.rolling.RollingFileAppender">
        <file>logs/jtheque.log</file>
 
        <rollingPolicy class="ch.qos.logback.core.rolling.FixedWindowRollingPolicy">
            <FileNamePattern>logs/jtheque.%i.log.zip</FileNamePattern>
            <MinIndex>1</MinIndex>
            <MaxIndex>5</MaxIndex>
        </rollingPolicy>
 
        <triggeringPolicy class="ch.qos.logback.core.rolling.SizeBasedTriggeringPolicy">
            <MaxFileSize>5MB</MaxFileSize>
        </triggeringPolicy>
 
        <layout class="ch.qos.logback.classic.PatternLayout">
            <Pattern>%d{HH:mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n</Pattern>
        </layout>
    </appender>
 
    <root level="DEBUG">
        <appender-ref ref="FILE"/>
    </root>
</configuration>
使用该配置，不会生成任何日志文件，这可能是 LogBack 的 bug，解决的办法就是使用绝对路径，你可以用一些系统变量来代替，例如：
...
<file>${user.dir}/logs/jtheque.log</file>
<rollingPolicy class="ch.qos.logback.core.rolling.FixedWindowRollingPolicy">
    <FileNamePattern>${user.dir}/logs/jtheque.%i.log.zip</FileNamePattern>
    <MinIndex>1</MinIndex>
    <MaxIndex>5</MaxIndex>
</rollingPolicy>
...
现在就好了，希望对某些使用 LogBack 的人有帮助。
其实使用相对路径是能产生日志文件的，只是这个相对路径是相对与Eclipse(我是使用eclipse开发的，在eclipse启动的)，我发现日志全部跑到eclipse安装目录里面去了
不过看样子，logback是不推荐使用相对路径来记录日志文件，个人觉得确实使用一些环境变量来引用绝对路径要更好控制一点
tomcat下可以用：${catalina.base}/logs/your_log.log
 
 一直使用相对路径. 没发现问题.  使用你这个配置也没问题. 可能你用的版本比较老,用最新的时会有警告,
[http://logback.qos.ch/codes.html#layoutInsteadOfEncoder](http://logback.qos.ch/codes.html#layoutInsteadOfEncoder)
   ps:  logback在当打包时目录不存在时不会自动创建的目录,  需要做小小的修改才行.
1, 把日志发送到邮件中
2, 把日志保存到数据库中（有异步么？）
官方文档有: [http://logback.qos.ch/manual/appenders.html](http://logback.qos.ch/manual/appenders.html)
