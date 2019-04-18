# ibatIS打印SQL的log4j配置 - z69183787的专栏 - CSDN博客
2015年06月15日 16:33:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6062
今天转换数据，遇到了用ibatIS做持久层的系统，弄不清数据的关联关系，就想着打印出后台SQL出来，在网上找了找相关的方法，现在记录下来以便以后自己用着方便。ibatIS的配置文件有两种，一种是xml的，一种是properties的。我用的是xml文件的，也把properties记下来，说不定哪天用的到。
第一种：log4j.xml文件的
**[html]**[view
 plain](http://blog.csdn.net/puppetmaster_c/article/details/7596115#)[copy](http://blog.csdn.net/puppetmaster_c/article/details/7596115#)
- <appendername="IBatis"class="org.apache.log4j.ConsoleAppender">
- <layoutclass="org.apache.log4j.PatternLayout">
- <paramname="ConversionPattern"value="%-4r [%t] %-5p %c %x - %m%n"/>
- </layout>
- <filterclass="org.apache.log4j.varia.LevelRangeFilter">
- <paramname="LevelMin"value="DEBUG"/>
- <paramname="LevelMax"value="DEBUG"/>
- </filter>
- </appender>
- 
- <loggername="com.ibatis"additivity="true">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="java.sql.Connection"additivity="true">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="java.sql.Statement"additivity="true">
- <levelvalue="DEBUG"/>
- </logger>
- <loggername="java.sql.PreparedStatement"additivity="true">
- <levelvalue="DEBUG"/>
- <appender-refref="IBatis"/>
- </logger>
- <loggername="java.sql.ResultSet"additivity="true">
- <levelvalue="DEBUG"/>
- <appender-refref="IBatis"/>
- </logger>
第二种：log4j.properties
**[plain]**[view
 plain](http://blog.csdn.net/puppetmaster_c/article/details/7596115#)[copy](http://blog.csdn.net/puppetmaster_c/article/details/7596115#)
- log4j.appender.stdout=org.apache.log4j.ConsoleAppender  
- log4j.appender.stdout.layout=org.apache.log4j.PatternLayout  
- log4j.appender.stdout.layout.ConversionPattern=%d %p [%c] - %m%n  
- 
- 
- log4j.logger.com.ibatis=debug  
- log4j.logger.com.ibatis.common.jdbc.SimpleDataSource=debug  
- log4j.logger.com.ibatis.common.jdbc.ScriptRunner=debug  
- log4j.logger.com.ibatis.sqlmap.engine.impl.SqlMapClientDelegate=debug  
- log4j.logger.java.sql.Connection=debug  
- log4j.logger.java.sql.Statement=debug  
- log4j.logger.java.sql.PreparedStatement=debug,stdout  
- 
