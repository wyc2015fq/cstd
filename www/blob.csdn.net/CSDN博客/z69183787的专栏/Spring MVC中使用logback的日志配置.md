# Spring MVC中使用logback的日志配置 - z69183787的专栏 - CSDN博客
2014年06月11日 16:51:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：14660
一开始在SpringMVC中就配置了logback.xml，但是由于放置的路径不正确，不知道怎么启动这个日志系统。
后来在网上看了别人的说明，logback.xml配置都差不多，只需要放置src包下面就会自动启动日志了。
需要导入的jar包有：logback-core-0.9.29.jar，logback-classic-0.9.29.jar，slf4j-api-1.6.1.jar
下面来看看logback.xml里面的内容配置：
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
<!-- 日志保存路径为tomcat下面的logs下面的mobileLog文件夹，logback会自动创建文件夹，这样设置了就可以输出日志文件了 -->
<substitutionProperty name="logbase" value="${catalina.base}/logs/mobileLog/" />
<!-- 这个是要配置输出文件的-->
<jmxConfigurator /> 
<appender name="stdout" class="ch.qos.logback.core.ConsoleAppender">
<layout class="ch.qos.logback.classic.PatternLayout">
<pattern>%date [%thread] %-5level %logger{80} - %msg%n</pattern>
</layout>
</appender>
<!-- 文件输出日志 (文件大小策略进行文件输出，超过指定大小对文件备份) -->
<appender name="logfile"
class="ch.qos.logback.core.rolling.RollingFileAppender">
<Encoding>UTF-8</Encoding>
<rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
<File>${logbase}%d{yyyy-MM-dd}.log.html</File>
<FileNamePattern>${logbase}.%d{yyyy-MM-dd}.log.html.zip</FileNamePattern>
</rollingPolicy>
<triggeringPolicy
class="ch.qos.logback.core.rolling.SizeBasedTriggeringPolicy">
<MaxFileSize>2MB</MaxFileSize>
</triggeringPolicy>
<layout class="ch.qos.logback.classic.html.HTMLLayout">
<pattern>%date%level%thread%10logger%file%line%msg</pattern>
</layout>
</appender>
<!-- Output by Email -->
<!-- <appender name="Email" class="ch.qos.logback.classic.net.SMTPAppender"> 
<SMTPHost>stmp host name</SMTPHost> <To>Email Address</To> <To>Email Address</To> 
<From>Email Address</From> <Subject>TESTING Email Function: %logger{20} - 
%m</Subject> <layout class="ch.qos.logback.classic.html.HTMLLayout"> <pattern>%date%level%thread%10logger%file%line%msg</pattern> 
</layout> </appender> -->
<!-- Output to Database -->
<!-- <appender name="DB" class="ch.qos.logback.classic.db.DBAppender"> <connectionSource 
class="ch.qos.logback.core.db.DriverManagerConnectionSource"> <driverClass>com.mysql.jdbc.Driver</driverClass> 
<url>jdbc:mysql://localhost:3306/test</url> <user>root</user> <password>trend_dev</password> 
</connectionSource> </appender> -->
<root>
<level value="ERROR" /> 
<appender-ref ref="stdout" />
<appender-ref ref="logfile" />
</root>
</configuration>
使用的时候可以这样定义变量：Logger logger = LoggerFactory.getLogger(AdministratorController.class);
需要import两个内容：
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
后面的使用就简单啦~
logger.debug(" 日志记录");
logger.info(" 日志记录");
logger.error(" 日志记录");
设置好后，可以输出到文件中，也可以输出到输出框中。
