# log4j平滑升级为log4j2(不需改动源码) - 零度的博客专栏 - CSDN博客
2017年07月14日 11:21:28[零度anngle](https://me.csdn.net/zmx729618)阅读数：2042
        公司的项目决定升级log4j,因为log4j2有一个自动删除日志的功能，这样可以减轻运维的一些工作，而且在多线程环境下，log4j2的异步日志系统比log4j和logback提高了十倍的性能（吞吐量和延迟率），官方原文如下：Log4j
 2 contains next-generation Asynchronous Loggers based on the LMAX Disruptor library. In multi-threaded scenarios Asynchronous Loggers have 10 times higher throughput and orders of magnitude lower latency than Log4j 1.x and Logback.
因为是升级，原项目用的是log4j或者slf4j打印日志，获取logger的方式不同。
比如log4j：
import org.apache.log4j.Logger;
方法：private static final Logger LOGGER = Logger.getLogger(xxxx.class);
比如slf4j：
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
方法：private static final Logger logger = LoggerFactory.getLogger(xxxx.class);
而如果用log4j2的话，API将有所不同：
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
方法：private static final Logger logger = LogManager.getLogger(xxxx.class);
        如果在升级的过程中，我们要去改代码的话，那将是一项很大的繁琐工程，log4j2[架构](http://lib.csdn.net/base/architecture)已经考虑到这一点，给我们提供了转换的包，我们只需要导入几个转换包就可以使用log4j2，而且还不需要修改代码。
官方原文：
        Perhaps the simplest way to convert to using Log4j 2 is to replace the log4j 1.x jar file with Log4j 2's log4j-1.2-api.jar.
 However, to use this successfully applications must meet the following requirements:
- They must not access methods and classes internal to the Log4j 1.x implementation such as Appenders, LoggerRepository or Category's callAppendersmethod.
- They must not programmatically configure Log4j.
- They must not configure by calling the classes DOMConfigurator or PropertyConfigurator
第一步：我是maven项目，就以maven项目为例子，进行讲解了。
删掉pom.xml文件中关于log4j的依赖，添加新依赖
**[html]**[view
 plain](http://blog.csdn.net/zouxucong/article/details/56013053?utm_source=itdadao&utm_medium=referral#)[copy](http://blog.csdn.net/zouxucong/article/details/56013053?utm_source=itdadao&utm_medium=referral#)
- <dependency>
- <groupId>org.apache.logging.log4j</groupId>
- <artifactId>log4j-api</artifactId>
- <version>2.7</version>
- <!-- <scope>provided</scope> -->
- </dependency>
- <dependency>
- <groupId>org.apache.logging.log4j</groupId>
- <artifactId>log4j-1.2-api</artifactId>
- <version>2.7</version>
- </dependency>
- <dependency>
- <groupId>org.apache.logging.log4j</groupId>
- <artifactId>log4j-core</artifactId>
- <version>2.7</version>
- <!-- <scope>provided</scope> -->
- </dependency>
- <dependency>
- <groupId>org.apache.logging.log4j</groupId>
- <artifactId>log4j-web</artifactId>
- <version>2.7</version>
- <scope>runtime</scope>
- </dependency>
PS：log4j-1.2-api为log4j和log4j2的连接包。
第二步：删掉以前的log4j.properties,改为log4j2.xml，这里起名字的时候，不要写错了
**[html]**[view
 plain](http://blog.csdn.net/zouxucong/article/details/56013053?utm_source=itdadao&utm_medium=referral#)[copy](http://blog.csdn.net/zouxucong/article/details/56013053?utm_source=itdadao&utm_medium=referral#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <configurationstatus="info">
- <Properties>
- <Propertyname="LOG_HOME">${sys:catalina.base}/var/logs</Property>
- <Propertyname="APP_LOG_HOME">${LOG_HOME}/uas</Property>
- </Properties>
- 
- <Appenders>
- <Consolename="Console"target="SYSTEM_OUT">
- <!-- 控制台只输出level及以上级别的信息(onMatch),其他的直接拒绝(onMismatch) -->
-             <!-- <ThresholdFilterlevel="trace"onMatch="ACCEPT"onMismatch="DENY"
- /> -->
- <!-- 输出日志的格式 -->
- <PatternLayout
- pattern="%d{yyyy-MM-dd HH:mm:ss.SSS} %-5level %class{36} %L %M - %msg%xEx%n"/>
- </Console>
- <RollingRandomAccessFilename="uas_appender"
- immediateFlush="true"fileName="${APP_LOG_HOME}/uas.log"
- filePattern="${APP_LOG_HOME}/uas.log.%d{yyyy-MM-dd}.gz">
-             <!-- %d{yyyy-MM-dd HH:mm:ss, SSS} : 日志生产时间 %p : 日志输出格式 %c : logger的名称   
-                 %m : 日志内容，即 logger.info("message") %n : 换行符 %C : Java类名 %L : 日志输出所在行数 %M   
-                 : 日志输出所在方法名 hostName : 本地机器名 hostAddress : 本地ip地址 -->
- <PatternLayout>
- <pattern>%d{yyyy-MM-dd HH:mm:ss.SSS} %-5level %class{36} %L %M --  
-                     %msg%xEx%n</pattern>
- </PatternLayout>
- <Policies>
- <TimeBasedTriggeringPolicyinterval="1"
- modulate="true"/>
- </Policies>
- <!-- 最多备份30天以内的日志，此处为策略限制，Delete中可以按自己需要用正则表达式编写 -->
- <!-- DefaultRolloverStrategy字段中加入max=“30”经测试是配合SizeBasedTriggeringPolicy限制%i的存在数量，并没有发现是网上流传的是最多保存多少个文件的限制，也或许是我写的有问题 -->
- <DefaultRolloverStrategy>
- <DeletebasePath="${APP_LOG_HOME}"maxDepth="1">
- <IfFileNameglob="uas.log.*.gz"/>
- <IfLastModifiedage="30d"/>
- </Delete>
- </DefaultRolloverStrategy>
- </RollingRandomAccessFile>
- </Appenders>
- 
- <Loggers>
- <loggername="org.spring"level="error"additivity="false">
- <AppenderRefref="Console"/>
- <AppenderRefref="uas_appender"/>
- </logger>
- <loggername="org.springframework"level="error"additivity="false">
- <AppenderRefref="Console"/>
- <AppenderRefref="uas_appender"/>
- </logger>
- <loggername="org.apache.zookeeper"level="error"additivity="false">
- <AppenderRefref="Console"/>
- <AppenderRefref="uas_appender"/>
- </logger>
- <loggername="com.alibaba.dubbo"level="error"additivity="false">
- <AppenderRefref="Console"/>
- <AppenderRefref="uas_appender"/>
- </logger>
- <loggername="xx.xx.xx"level="info"additivity="false">
- <AppenderRefref="Console"/>
- <appender-refref="uas_appender"/>
- </logger>
- <rootlevel="info">
- <appender-refref="Console"/>
- <appender-refref="uas_appender"/>
- </root>
- </Loggers>
- 
- </configuration>
这里需要注意的有以下几点：
1.log4j2.*版本中获取home路径是${sys:catalina.base}
2.要在<root>标签中添加<appender-ref ref="Console"/>,<appender-ref
 ref="log"/>才能在控制台、和文件中打印日志
第三步：在web.xml中配置，注释掉以前关于加载log4j的配置，重新配置，这里我的log4j2.xml放在classpath/conf下
**[html]**[view
 plain](http://blog.csdn.net/zouxucong/article/details/56013053?utm_source=itdadao&utm_medium=referral#)[copy](http://blog.csdn.net/zouxucong/article/details/56013053?utm_source=itdadao&utm_medium=referral#)
- <context-param>
- <param-name>log4jConfiguration</param-name>
- <param-value>classpath:conf/log4j2.xml</param-value>
- </context-param>
配置放在web.xml的<display-name/>下方，如此log4j升级log4j2升级成功。
扩展：如果原项目中用的是slf4j打印日志，那么导包的时候需要导入：
**[html]**[view
 plain](http://blog.csdn.net/zouxucong/article/details/56013053?utm_source=itdadao&utm_medium=referral#)[copy](http://blog.csdn.net/zouxucong/article/details/56013053?utm_source=itdadao&utm_medium=referral#)
- <dependency>
- <groupId>org.slf4j</groupId>
- <artifactId>slf4j-api</artifactId>
- <version>${slf4j.version}</version>
- </dependency>
- <dependency>
- <groupId>org.slf4j</groupId>
- <artifactId>slf4j-log4j12</artifactId>
- <version>${slf4j.version}</version>
- </dependency>
也是不需要修改代码，关于slf4j继承log4j2的详细解决方案，可参考：[http://blog.csdn.net/zouxucong/article/details/56005725](http://blog.csdn.net/zouxucong/article/details/56005725)
