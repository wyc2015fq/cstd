# logback - z69183787的专栏 - CSDN博客
2014年06月11日 16:52:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1259
个人分类：[日志-Logback](https://blog.csdn.net/z69183787/article/category/2327177)
新的公司日志管理统一用logback，由于之前一直在用log4j，所以其实对logback这个由log4j长出来的家伙还不算陌生。
首先介绍下logback的前世今生。
slf4j由log4j作者Ceki开发，逐步取代apahce commons logging。
logback由log4j作者Ceki开发，逐步取代log4j。
slf4j等于commons-logging，是各种日志实现的通用入口，会根据classpath中存在下面哪一个Jar来决定具体的日志实现库。
logback相比较log4j的优势
slf4j支持参数化的logger.error("帐号ID：{}不存在", userId);
告别了if(logger.isDebugEnable()) 时代。
另外logback的整体性能比log4j也较佳，hibernate等项目已经采用了slf4j。
slf4j和logback的使用
1.如果日志的参数超过3个，需要写成
Java代码  ![收藏代码](http://yuri-liuyu.iteye.com/images/icon_star.png)
- Object[] params = {newVal, below, above};  
- logger.debug("Value {} was inserted between {} and {}.", params);  
2.因为内部已优化，作者认为slf4j的logger不需要定义为static。
3.可设置缓存后批量写日志文件(但服务器如果重启，可能会丢失未写到磁盘的记录)
4.MDC，用Filter，将当前用户名等业务信息放入MDC中，在日志format定义中即可使用该变量。
5.JMS Appender用于告警, DB Appender用于业务日志等可以使用插件，如生成Log代码的Eclipse插件Log4E。
6.tomcat和glassfish中，设定日志路径为../logs/xxxx.log 都能将日志放入应用服务器本身的logs目录。
最后把最近完善的一个logback.xml贴上，毕竟实际项目中的文件最能说明问题。
Xml代码  ![收藏代码](http://yuri-liuyu.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- 
- <configuration>
- <substitutionPropertyname="log.base"value="d:\\logback\\logback"/>
- <jmxConfigurator/>
- <appendername="stdout"class="ch.qos.logback.core.ConsoleAppender">
- <filterclass="ch.qos.logback.core.filter.EvaluatorFilter">
- <evaluatorname="myEval">
- <expression>message.contains("dao")</expression>
- </evaluator>
- <onMatch>ACCEPT</onMatch>
- <onMismatch>DENY</onMismatch>
- </filter>
- <layoutclass="ch.qos.logback.classic.PatternLayout">
- <pattern>%date [%thread] %-5level %logger{80} - %msg%n</pattern>
- </layout>
- </appender>
- <appendername="logfile-dao"
- class="ch.qos.logback.core.rolling.RollingFileAppender">
- <filterclass="ch.qos.logback.core.filter.EvaluatorFilter">
- <evaluatorname="myEval_dao">
- <expression>message.contains("dao")</expression>
- </evaluator>
- <onMatch>ACCEPT</onMatch>
- <onMismatch>DENY</onMismatch>
- </filter>
- <Encoding>UTF-8</Encoding>
- <File>${log.base}_dao.log</File>
- <rollingPolicyclass="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
- <FileNamePattern>${log.base}.%d{yyyy-MM-dd}_dao.log.zip  
- </FileNamePattern>
- </rollingPolicy>
- <layoutclass="ch.qos.logback.classic.PatternLayout">
- <pattern>%date [%thread] %-5level %logger{80} - %msg%n</pattern>
- </layout>
- </appender>
- <appendername="logfile-service"
- class="ch.qos.logback.core.rolling.RollingFileAppender">
- <filterclass="ch.qos.logback.core.filter.EvaluatorFilter">
- <evaluatorname="myEval_service">
- <expression>message.contains("service.impl")</expression>
- </evaluator>
- <onMatch>ACCEPT</onMatch>
- <onMismatch>DENY</onMismatch>
- </filter>
- <Encoding>UTF-8</Encoding>
- <File>${log.base}_service.log</File>
- <rollingPolicyclass="ch.qos.logback.core.rolling.TimeBasedRollingPolicy">
- <FileNamePattern>${log.base}.%d{yyyy-MM-dd}_service.log.zip  
- </FileNamePattern>
- </rollingPolicy>
- <layoutclass="ch.qos.logback.classic.PatternLayout">
- <pattern>%date [%thread] %-5level %logger{80} - %msg%n</pattern>
- </layout>
- </appender>
- <appendername="jms_dao"class="ch.qos.logback.classic.net.JMSQueueAppender">
- <InitialContextFactoryName>
-             org.apache.activemq.jndi.ActiveMQInitialContextFactory  
- </InitialContextFactoryName>
- <ProviderURL>tcp://192.168.1.120:61616</ProviderURL>
- <filterclass="ch.qos.logback.core.filter.EvaluatorFilter">
- <evaluatorname="myEval_service">
- <expression>message.contains("dao")</expression>
- </evaluator>
- <onMatch>ACCEPT</onMatch>
- <onMismatch>DENY</onMismatch>
- </filter>
- <QueueConnectionFactoryBindingName>ConnectionFactory  
- </QueueConnectionFactoryBindingName>
- <QueueBindingName>cms_dao_log</QueueBindingName>
- </appender>
- <appendername="jms_service"class="ch.qos.logback.classic.net.JMSQueueAppender">
- <InitialContextFactoryName>
-             org.apache.activemq.jndi.ActiveMQInitialContextFactory  
- </InitialContextFactoryName>
- <ProviderURL>tcp://192.168.1.120:61616</ProviderURL>
- <filterclass="ch.qos.logback.core.filter.EvaluatorFilter">
- <evaluatorname="myEval_service">
- <expression>message.contains("service.impl")</expression>
- </evaluator>
- <onMatch>ACCEPT</onMatch>
- <onMismatch>DENY</onMismatch>
- </filter>
- <QueueConnectionFactoryBindingName>ConnectionFactory  
- </QueueConnectionFactoryBindingName>
- <QueueBindingName>cms_service_log</QueueBindingName>
- </appender>
- <loggername="com.cms5.cmsservice.jms">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="java.sql.PreparedStatement">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="java.sql.Connection">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="java.sql.Statement">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="com.ibatis">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="com.ibatis.common.jdbc.SimpleDataSource">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="com.ibatis.common.jdbc.ScriptRunner">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="com.ibatis.sqlmap.engine.impl.SqlMapClientDelegate">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="com.danga.MemCached">
- <levelvalue="INFO"/>
- </logger>
- <loggername="org.springframework.test">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="org.apache.struts2">
- <levelvalue="DEBUG"/>
- </logger>
- <root>
- <levelvalue="INFO"/>
- <!--<appender-ref ref="stdout" />
-         -->
- <appender-refref="logfile-dao"/>
- <appender-refref="logfile-service"/>
- <appender-refref="jms_dao"/>
- <appender-refref="jms_service"/>
- </root>
- </configuration>
就先不做说明了，里面的配置还是比较明确的，关于jmsappender那部分，之前也做过简单说明。
