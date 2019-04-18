# Log4j2记录日志到数据库(MySQL&MongoDB) - z69183787的专栏 - CSDN博客
2016年10月25日 17:56:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3217
【摘要】
    本文主要介绍Log4j2记录日志到[MySQL](http://lib.csdn.net/base/mysql)和MongoDB[数据库](http://lib.csdn.net/base/mysql)，用简单的例子来讲解，并解决了一些官方配置可能出现的错误。具体内容可参见官方文档：[http://logging.apache.org/log4j/2.x/manual/appenders.html](http://logging.apache.org/log4j/2.x/manual/appenders.html)左侧导航栏的Appenders->JDBC和Appenders->NoSQL两项
【引用】
    Log4j2的基本配置我在前面的文章已经介绍，具体可参见：[http://blog.csdn.net/ererfei/article/details/46831807](http://blog.csdn.net/ererfei/article/details/46831807)
# 1.    Log4j2记录日志到MySQL
## 1.1.   首先创建存储日志的数据表ibs_log
创建语句如下：
**[sql]**[view
 plain](http://blog.csdn.net/ererfei/article/details/50884155#)[copy](http://blog.csdn.net/ererfei/article/details/50884155#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1609122/fork)
- DROPTABLE IF EXISTS `ibs_log`;  
- CREATETABLE `ibs_log` (  
-  `log_id` int(10) NOTNULL AUTO_INCREMENT,  
-  `log_date` datetime NOTNULL COMMENT '日期',  
-  `log_file` varchar(255) DEFAULTNULL COMMENT '文件名',  
-  `log_line` varchar(255) DEFAULTNULL COMMENT '行号',  
-   `log_thread`varchar(255) DEFAULTNULL COMMENT '线程',  
-  `log_level` varchar(255) DEFAULTNULL COMMENT 'Log级别 info warndebug error等',  
-  `log_message` varchar(2048) DEFAULTNULL COMMENT 'Log信息',  
- PRIMARYKEY (`log_id`,`log_date`),  
- KEY`INDEX_LOG_DATE` (`log_date`) USING BTREE  
- ) ENGINE=InnoDB DEFAULT CHARSET=utf8;  
## 1.2.   创建Log4j2.xml配置文件
需要创建数据表、数据行的对应关系
**[html]**[view
 plain](http://blog.csdn.net/ererfei/article/details/50884155#)[copy](http://blog.csdn.net/ererfei/article/details/50884155#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1609122/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!-- 不打印log4j内部日志，配置文件刷新频率为30秒 -->
- <Configurationstatus="off"monitorInterval="30">
- <Appenders>
- <JDBC name="databaseAppender"tableName="ibs_log">
- <ConnectionFactoryclass="com.*.*.ConnectionFactory"method="getDatabaseConnection" />
- <Columnname="log_date"isEventTimestamp="true"/>
- <Columnname="log_file"pattern="%file"/>
- <Columnname="log_line"pattern="%line"/>
- <Columnname="log_thread"pattern="%thread"/>
- <Columnname="log_level"pattern="%level"/>
- <Columnname="log_message"pattern="%message"/>
- </JDBC>
- </Appenders>
- <Loggers>
- <!-- 打印等级：off、fatal、error、warn、info、debug、trace、all -->
- <Loggername="项目包名（com.*.*）"level="debug">
- <appender-refref="databaseAppender"/>
- </Logger>
- <Rootlevel="off"></Root>
- </Loggers>
- </Configuration>
## 1.3.   创建数据库连接类ConnectionFactory
需要修改下面代码中用户名、密码、数据库名称为你使用的数据库对用的用户名密码以及名称：
【ConnectionFactory.[Java](http://lib.csdn.net/base/javaee)】
**[java]**[view
 plain](http://blog.csdn.net/ererfei/article/details/50884155#)[copy](http://blog.csdn.net/ererfei/article/details/50884155#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1609122/fork)
- package com.centerm.util;  
- 
- import java.sql.Connection;  
- import java.sql.SQLException;  
- import java.util.Properties;  
- import javax.sql.DataSource;  
- importorg.apache.commons.dbcp.DriverManagerConnectionFactory;  
- importorg.apache.commons.dbcp.PoolableConnection;  
- import org.apache.commons.dbcp.PoolableConnectionFactory;  
- importorg.apache.commons.dbcp.PoolingDataSource;  
- importorg.apache.commons.pool.impl.GenericObjectPool;  
- 
- publicclass ConnectionFactory {  
-          privatestatic interface Singleton {  
- final ConnectionFactory INSTANCE = new ConnectionFactory();  
-     }  
- 
- privatefinal DataSource dataSource;  
- 
- private ConnectionFactory() {  
- try {  
-            Class.forName("com.mysql.jdbc.Driver");  
-        } catch (ClassNotFoundException e) {  
-            e.printStackTrace();  
-            System.exit(0);  
-        }  
- 
-        Properties properties = new Properties();  
-        properties.setProperty("user", "用户名");  
-        properties.setProperty("password", "密码"); //or get properties from some configuration file
- 
-        GenericObjectPool<PoolableConnection>pool = new GenericObjectPool<PoolableConnection>();  
-        DriverManagerConnectionFactory connectionFactory = newDriverManagerConnectionFactory(  
- "jdbc:mysql://localhost:3306/数据库名称",properties  
-        );  
- new PoolableConnectionFactory(  
-                 connectionFactory, pool, null,"SELECT 1", 3, false, false, Connection.TRANSACTION_READ_COMMITTED  
-        );  
- 
- this.dataSource = new PoolingDataSource(pool);  
-     }  
- 
- publicstatic Connection getDatabaseConnection() throws SQLException {  
- return Singleton.INSTANCE.dataSource.getConnection();  
-     }  
- }  
至此就可以运行项目测试了
## 1.4.   特别注意
由于项目启动时Log4j2是最先加载的内容，所以有可能出现ConnectionFactory这个类启动报错，所以需要在官网提供实例的基础上添加代码（上面类中已经添加）：
![](https://img-blog.csdn.net/20160314101221767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 2.    Log4j2记录日志到MongoDB
安装运行MongoDB：[http://www.runoob.com/mongodb/mongodb-window-install.html](http://www.runoob.com/mongodb/mongodb-window-install.html)
MongoDB驱动下载：[http://central.maven.org/maven2/org/mongodb/mongo-java-driver/](http://central.maven.org/maven2/org/mongodb/mongo-java-driver/)
[Java](http://lib.csdn.net/base/java)MongoDB API学习：[http://blog.csdn.net/ererfei/article/details/50857103](http://blog.csdn.net/ererfei/article/details/50857103)
## 2.1.   创建数据库
运行MongoDB客户端mongo-cli.exe，使用命令”usetest”创建数据库test，创建该数据库的用户[root:password]，不需要手动创建collection。
## 2.2.   创建Log4j2.xml配置文件
**[html]**[view
 plain](http://blog.csdn.net/ererfei/article/details/50884155#)[copy](http://blog.csdn.net/ererfei/article/details/50884155#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1609122/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- 
- <!--不打印log4j内部日志，配置文件刷新频率为30秒 -->
- <Configurationstatus="off"monitorInterval="30">
- <Appenders>
- <NoSqlname="databaseAppender">
- <MongoDbdatabaseName="test"collectionName="logData" server="127.0.0.1"port="27017" userName="root"password="password"/>
- </NoSql>
- </Appenders>
- <Loggers>
- <!-- 打印等级：off、fatal、error、warn、info、debug、trace、all -->
- <Loggername="com.*.*"level="debug">
- <appender-refref="databaseAppender"/>
- </Logger>
- <Rootlevel="off"></Root>
- </Loggers>
- </Configuration>
！！！至此就可以运行项目测试了
log4j2可灵活的将日志打印在控制台、通过SMTP邮件发送、记录至文件、保存至[数据库](http://lib.csdn.net/base/mysql)。本文介绍如何配置log4j2将日志记录至数据库
建立用于保存日志的数据库表：
CREATE TABLE `sys_log` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `level` varchar(32) NOT NULL,
  `logger` varchar(100) NOT NULL,
  `message` varchar(1000) DEFAULT NULL,
  `exception` varchar(10000) DEFAULT NULL,
  `date_add` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=19 DEFAULT CHARSET=utf8mb4;
配置 databaseAppender ：
<JDBC name="databaseAppender" tableName="sys_log">
      <ConnectionFactory class="cc.s2m.web.s2mBlog.util.StaticProp" method="getDatabaseConnection" />
      <Column name="date_add" isEventTimestamp="true" />
      <Column name="level" pattern="%level" />
      <Column name="logger" pattern="%logger" />
      <Column name="message" pattern="%message" />
      <Column name="exception" pattern="%ex{full}" />
    </JDBC>
其中 cc.s2m.web.s2mBlog.util.StaticProp 类的getDatabaseConnection方法为获取可用的datasource：
DriverManagerDataSource ds = new DriverManagerDataSource();
ds.setDriverClassName("com.[MySQL](http://lib.csdn.net/base/mysql).jdbc.Driver");
ds.setUrl("jdbc:mysql://127.0.0.1/s2mBlog?characterEncoding=utf8");
ds.setUsername("root");
ds.setPassword("123456");
return ds.getConnection();
然后指派您需要记录的日志，使用 databaseAppender 即可：
<logger name="SYSLOG" level="INFO" additivity="false">
      <appender-ref ref="databaseAppender"/> 
</logger>
