# slf4j+logback实现多个日志输出配置实例 - 零度的博客专栏 - CSDN博客
2016年08月04日 15:56:48[零度anngle](https://me.csdn.net/zmx729618)阅读数：4558
1、Maven依赖包：pom.xml
```
<!--LogBack -->
        <dependency>
            <groupId>org.slf4j</groupId>
            <artifactId>slf4j-api</artifactId>
            <version>1.6.0</version>
        </dependency>
        <dependency>
             <groupId>ch.qos.logback</groupId>
             <artifactId>logback-core</artifactId>
             <version>0.9.28</version>            
         </dependency>
         <dependency>
             <groupId>ch.qos.logback</groupId>
             <artifactId>logback-classic</artifactId>
             <version>0.9.28</version>            
         </dependency>
```
2、日志配置文件：logback.xml
```
<?xml version="1.0"?>
<configuration>     
         
      <substitutionProperty name="log.base" value="D:/Workspaces/projects/ssm/logback" />
      <jmxConfigurator />
       
      <appender name="stdout" class="ch.qos.logback.core.ConsoleAppender">  
           <layout class="ch.qos.logback.classic.PatternLayout">  
                   <pattern>%date [%thread] %-5level %logger{80} - %msg%n</pattern>  
           </layout>  
      </appender>  
     
     <appender name="FILE"  class="ch.qos.logback.core.rolling.RollingFileAppender">
         <Encoding>UTF-8</Encoding>
         <File>${log.base}/log/testProject.log</File>
         <Prudent>true</Prudent>
         <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
             <FileNamePattern>${log.base}/log/testProject-%d{yyyy-MM-dd_HH}-%i.log</FileNamePattern>
             <MaxHistory>5</MaxHistory>
             <TimeBasedFileNamingAndTriggeringPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
                    <MaxFileSize>5MB</MaxFileSize>
             </TimeBasedFileNamingAndTriggeringPolicy>
         </rollingPolicy>
         <layout class="ch.qos.logback.classic.PatternLayout">
             <Pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n</Pattern>
         </layout>
     </appender>  
     
     <appender name="warningLoggerAppender"  class="ch.qos.logback.core.rolling.RollingFileAppender">
         <Encoding>UTF-8</Encoding>
         <File>${log.base}/alarm/testProject-warning.txt</File>
         <Prudent>true</Prudent>
         <rollingPolicy class="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
             <FileNamePattern>${log.base}/alarm/testProject-warning-%d{yyyyMMddHHmmss}.txt</FileNamePattern>
             <MaxHistory>3</MaxHistory>
             <TimeBasedFileNamingAndTriggeringPolicy class="ch.qos.logback.core.rolling.SizeAndTimeBasedFNATP">
                    <MaxFileSize>5MB</MaxFileSize>
             </TimeBasedFileNamingAndTriggeringPolicy>
         </rollingPolicy>
         <layout class="ch.qos.logback.classic.PatternLayout">
             <Pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n</Pattern>             
         </layout>
     </appender>    
     
     <root>  
        <level value="INFO" />  
        <appender-ref ref="stdout" />           
        <appender-ref ref="FILE" />  
        <appender-ref ref="warningLoggerAppender" />  
    </root>    
     
</configuration>
```
3、代码使用
```
package com.zmx.ssm.logback;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
public class LogBackTest {
	
	private static Logger logger = LoggerFactory.getLogger(LogBackTest.class);
	
	public static void main(String[] args) {
		
		logger.debug("日志记录");
		logger.info(" 日志记录");
		logger.warn(" 日志记录");
	    logger.error(" 日志记录");
		
	}
}
```
