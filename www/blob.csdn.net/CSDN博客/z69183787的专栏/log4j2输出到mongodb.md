# log4j2输出到mongodb - z69183787的专栏 - CSDN博客
2016年12月13日 17:29:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3141
摘要: log4j2输出到mongodb
以log4j2 v2.7版本为例，低版本可能不支持。
参见：
http://logging.apache.org/log4j/2.x/manual/appenders.html#NoSQLAppender
https://logging.apache.org/log4j/2.x/runtime-dependencies.html
https://logging.apache.org/log4j/2.x/log4j-nosql/index.html
http://www.cnblogs.com/autfish/p/5557568.html
maven:
```
<dependency>
    <groupId>org.mongodb</groupId>
    <artifactId>mongo-java-driver</artifactId> <!--必需 -->
    <version>2.12.3</version>
</dependency>
<dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-api</artifactId>
    <version>2.7</version>
</dependency>
<dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-core</artifactId>
    <version>2.7</version>
</dependency>
<dependency>
    <groupId>org.apache.logging.log4j</groupId>
    <artifactId>log4j-nosql</artifactId> <!-- 必需，否则报错 -->
    <version>2.7</version>
</dependency>
```
log4j2.xml:
```
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="off" monitorInterval="30">
    <Appenders>
        <NoSql name="mongoAppender" bufferSize="10"> <!--建议bufferSize配置,减少频繁写操作。 -->
            <MongoDb databaseName="sl0" collectionName="log4j2" server="127.0.0.1" port="27017"/>
        </NoSql>
        <Console name="STDOUT" target="SYSTEM_OUT">
            <PatternLayout pattern="%m%n"/>
        </Console>
    </Appenders>
    <Loggers>
        <Logger name="mongologger" level="debug">
            <appender-ref ref="mongoAppender"/>
        </Logger>
        <Logger name="org" level="debug">
            <appender-ref ref="mongoAppender"/>
            <appender-ref ref="STDOUT" />
        </Logger>
        <Root level="debug">
            <appender-ref ref="STDOUT" />
        </Root>
    </Loggers>
</Configuration>
```
unit:
```java
package org.sl;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.junit.Test;
public class Test2 {
//都会输出到mongo中，见log4j2.xml配置
    static Logger clzlogger = LogManager.getLogger(Test2.class); 
    static Logger mongologger = LogManager.getLogger("mongologger");
    @Test
    public void t1(){
        mongologger.info("mongologger");
        clzlogger.info("clzlogger");
    }
}
```
