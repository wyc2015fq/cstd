# Spring+iBatis+Atomikos实现JTA事务 - z69183787的专栏 - CSDN博客
2016年09月13日 18:11:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：721
Spring+iBatis+Atomikos实现JTA事务
Atomikos是一个公司名字，旗下最著名的莫过于其Atomikos的事务管理器产品。
产品分两个：一个是开源的TransactionEssentials，一个是商业的ExtremeTransactions。
TransactionEssentials的主要特征：
JTA/XA 事务管理 —— 提供事务管理和连接池
不需要应用服务器 —— TransactionEssentials可以在任何Java EE应用服务器中运行，也就是不依赖于任何应用服务器
开源 —— TransactionEssentials是遵守Apache版本2许可的开源软件
专注于JDBC/JMS —— 支持所有XA资源，但是资源池和消息监听是专供JDBC和JMS的
与Spring 和 Hibernate 集成 —— 提供了描述如何与Spring和Hibernate集成的文档
ExtremeTransactions 是基于TransactionEssentials之上的，增加了对非XA事务的支持，在servlet容器中提供了图形化管理控制面板。Atomikos还提供了基于订阅的支持服务，一份订阅可以得到访问ExtremeTransactions中额外功能的权限。
Pardon继续说道，在很多情况下，一个应用服务器对于一个应用也许不是最佳方案 —— 他以SOA/ESB终端作为例子，阐述了通过JDBC来处理JMS消息可能是一个非常轻量级的可伸缩的方案。
关于TransactionEssentials的后继版本，Pardon提到了更强大的JDBC和JMS连接池，支持OSGi 和JMX事务管理工具，是计划中的3.3版的主要特征。Pardon还说明为JDBC数据源和JMS连接器增加JMX是4.0版的目标。
下面是Spring+iBatis+Atomikos实现JTA事务的测试过程：
一、环境
1、准备软件环境
 spring-framework-2.5.6.SEC01-with-dependencies.zip
 ibatis-2.3.4
 AtomikosTransactionsEssentials-3.5.5.zip
 MySQL-5.1
 JDK1.5
[http://www.atomikos.com/downloads/transactions-essentials/3.5.5/AtomikosTransactionsEssentials-3.5.5.zip](http://www.atomikos.com/downloads/transactions-essentials/3.5.5/AtomikosTransactionsEssentials-3.5.5.zip)
2、创建数据库环境，注意数据库引擎为InnoDB，只有这样才能支持事务。
CREATEDATABASEIFNOTEXISTS testdb_a        DEFAULT CHARACTER SET utf8;    
USE testdb_a;    
DROPTABLEIFEXISTS tab_a;    
CREATETABLE tab_a (    
        id bigint(20) NOTNULL,    
namevarchar(60) DEFAULTNULL,    
        address varchar(120) DEFAULTNULL,    
PRIMARYKEY (id)    
) ENGINE=InnoDB DEFAULT CHARSET=utf8;    
CREATEDATABASEIFNOTEXISTS testdb_b        DEFAULT CHARACTER SET utf8;    
USE testdb_b;    
DROPTABLEIFEXISTS tab_b;    
CREATETABLE tab_b (    
        id bigint(20) NOTNULL,    
namevarchar(60) DEFAULTNULL,    
        address varchar(120) DEFAULTNULL,    
PRIMARYKEY (id)    
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
二、创建项目
依赖包结构如下先：
│    spring-aop.jar 
│    spring-beans.jar 
│    spring-context-support.jar 
│    spring-context.jar 
│    spring-core.jar 
│    spring-jdbc.jar 
│    spring-jms.jar 
│    spring-orm.jar 
│    spring-test.jar 
│    spring-tx.jar 
│    spring-web.jar 
│    spring-webmvc-portlet.jar 
│    spring-webmvc-struts.jar 
│    spring-webmvc.jar 
│    aspectjrt.jar 
│    aspectjweaver.jar 
│    cglib-nodep-2.1_3.jar 
│    asm-2.2.3.jar 
│    log4j-1.2.15.jar 
│    asm-commons-2.2.3.jar 
│    asm-util-2.2.3.jar 
│    aopalliance.jar 
│    mysql-connector-java-5.1.6-bin.jar 
│ 
├─log4j 
│            log4j-1.2.15.jar 
│ 
├─junit 
│            junit-3.8.2.jar 
│            junit-4.4.jar 
│            license.txt 
│ 
├─jakarta-commons 
│            commons-attributes-api.jar 
│            commons-attributes-compiler.jar 
│            commons-beanutils.jar 
│            commons-codec.jar 
│            commons-collections.jar 
│            commons-dbcp.jar 
│            commons-digester.jar 
│            commons-discovery.jar 
│            commons-fileupload.jar 
│            commons-httpclient.jar 
│            commons-io.jar 
│            commons-lang.jar 
│            commons-logging.jar 
│            commons-pool.jar 
│            commons-validator.jar 
│ 
├─jotm 
│            license.txt 
│            xapool.jar 
│            jotm-core.jar 
│            jotm-standalone.jar 
│            jotm-jms.jar 
│            jotm-datasource.jar 
│            ow2-jta-1.1-spec.jar 
│            jotm-client.jar 
│ 
├─ibatis 
│            ibatis-2.3.4.726.jar 
│            sql-map-2.dtd 
│            sql-map-config-2.dtd 
│ 
├─atomikos 
│            atomikos-util.jar 
│            transactions-api.jar 
│            transactions-essentials-all.jar 
│            transactions-hibernate2.jar 
│            transactions-hibernate3.jar 
│            transactions-jdbc-deprecated.jar 
│            transactions-jdbc.jar 
│            transactions-jms-deprecated.jar 
│            transactions-jms.jar 
│            transactions-jta.jar 
│            transactions.jar 
│ 
└─alib 
                SLF4J_LICENSE.TXT 
                jca.jar 
                jms.jar 
                jmx.jar 
                jta.jar 
                servlet-2.3.jar 
                slf4j-api-1.4.3.jar 
                slf4j-nop-1.4.3.jar
三、配置
jta.properties
com.atomikos.icatch.service=com.atomikos.icatch.standalone.UserTransactionServiceFactory 
com.atomikos.icatch.console_file_name = tm.out 
com.atomikos.icatch.log_base_name = tmlog 
com.atomikos.icatch.tm_unique_name = com.atomikos.spring.jdbc.tm 
com.atomikos.icatch.console_log_level = INFO    
<?xmlversion="1.0"encoding="UTF-8"?>
<!-- 局部单元测试使用，不正式发布，不要删除 --> 
<beansxmlns="http://www.springframework.org/schema/beans"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns:jee="http://www.springframework.org/schema/jee"
xmlns:aop="http://www.springframework.org/schema/aop"
xmlns:tx="http://www.springframework.org/schema/tx"
             xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.0.xsd 
                     http://www.springframework.org/schema/jee http://www.springframework.org/schema/jee/spring-jee-2.0.xsd 
                     http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.0.xsd 
                     http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.0.xsd">
        <!--指定Spring配置中用到的属性文件--> 
<beanid="propertyConfig"
class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
<propertyname="locations">
<list>
<value>classpath:jdbc.properties</value>
</list>
</property>
</bean>
        <!-- 数据源A --> 
<beanid="dataSourceA"class="com.atomikos.jdbc.SimpleDataSourceBean"init-method="init"destroy-method="close">
<propertyname="uniqueResourceName">
<value>mysql/db_a</value>
</property>
<propertyname="xaDataSourceClassName">
<value>com.mysql.jdbc.jdbc2.optional.MysqlXADataSource</value>
</property>
<propertyname="xaDataSourceProperties">
<value>URL=${jdbc.url};user=${jdbc.username};password=${jdbc.password}</value>
</property>
<propertyname="exclusiveConnectionMode">
<value>true</value>
</property>
<propertyname="connectionPoolSize">
<value>3</value>
</property>
<propertyname="validatingQuery">
<value>SELECT 1</value>
</property>
</bean>
        <!-- 数据源B --> 
<beanid="dataSourceB"class="com.atomikos.jdbc.SimpleDataSourceBean"init-method="init"destroy-method="close">
<propertyname="uniqueResourceName">
<value>mysql/db_b</value>
</property>
<propertyname="xaDataSourceClassName">
<value>com.mysql.jdbc.jdbc2.optional.MysqlXADataSource</value>
</property>
<propertyname="xaDataSourceProperties">
<value>URL=${jdbc2.url};user=${jdbc2.username};password=${jdbc2.password}</value>
</property>
<propertyname="exclusiveConnectionMode">
<value>true</value>
</property>
<propertyname="connectionPoolSize">
<value>3</value>
</property>
<propertyname="validatingQuery">
<value>SELECT 1</value>
</property>
</bean>
<beanid="atomikosTransactionManager"class="com.atomikos.icatch.jta.UserTransactionManager"
init-method="init"destroy-method="close">
<propertyname="forceShutdown"value="true"/>
</bean>
<beanid="atomikosUserTransaction"class="com.atomikos.icatch.jta.UserTransactionImp">
<propertyname="transactionTimeout"value="300"/>
</bean>
        <!-- JTA事务管理器 --> 
<beanid="springTransactionManager"class="org.springframework.transaction.jta.JtaTransactionManager">
<propertyname="transactionManager"ref="atomikosTransactionManager"/>
<propertyname="userTransaction"ref="atomikosUserTransaction"/>
</bean>
        <!-- 事务切面配置 --> 
<aop:config>
<aop:pointcutid="serviceOperation"
expression="execution(* *..service*..*(..))"/>
<aop:advisorpointcut-ref="serviceOperation"
advice-ref="txAdvice"/>
</aop:config>
        <!-- 通知配置 --> 
<tx:adviceid="txAdvice"transaction-manager="springTransactionManager">
<tx:attributes>
<tx:methodname="delete*"rollback-for="Exception"/>
<tx:methodname="save*"rollback-for="Exception"/>
<tx:methodname="update*"rollback-for="Exception"/>
<tx:methodname="*"read-only="true"rollback-for="Exception"/>
</tx:attributes>
</tx:advice>
        <!--根据dataSourceA和sql-map-config_A.xml创建一个SqlMapClientA--> 
<beanid="sqlMapClientA"
class="org.springframework.orm.ibatis.SqlMapClientFactoryBean">
<propertyname="dataSource">
<reflocal="dataSourceA"/>
</property>
<propertyname="configLocation">
<value>classpath:/sql-map-config_A.xml</value>
</property>
</bean>
        <!--根据dataSourceB和sql-map-config_B.xml创建一个SqlMapClientB--> 
<beanid="sqlMapClientB"
class="org.springframework.orm.ibatis.SqlMapClientFactoryBean">
<propertyname="dataSource">
<reflocal="dataSourceB"/>
</property>
<propertyname="configLocation">
<value>classpath:/sql-map-config_B.xml</value>
</property>
</bean>
        <!--根据sqlMapClientA创建一个SqlMapClientTemplate的模版类实例sqlMapClientTemplateA--> 
<beanid="sqlMapClientTemplateA"
class="org.springframework.orm.ibatis.SqlMapClientTemplate">
<propertyname="sqlMapClient"ref="sqlMapClientA"/>
</bean>
        <!--根据sqlMapClientB创建一个SqlMapClientTemplate的模版类实例sqlMapClientTemplateB--> 
<beanid="sqlMapClientTemplateB"
class="org.springframework.orm.ibatis.SqlMapClientTemplate">
<propertyname="sqlMapClient"ref="sqlMapClientB"/>
</bean>
        <!-- 配置DAO，并注入所使用的sqlMapClientTemplate实例 --> 
<beanid="tabADAO"class="com.lavasoft.stu.atomikos.dao.impl.TabADAOImpl">
<propertyname="sqlMapClientTemplate"ref="sqlMapClientTemplateA"/>
</bean>
<beanid="tabBDAO"class="com.lavasoft.stu.atomikos.dao.impl.TabBDAOImpl">
<propertyname="sqlMapClientTemplate"ref="sqlMapClientTemplateB"/>
</bean>
        <!-- Service配置，注入DAO --> 
<beanid="stuJotmService"class="com.lavasoft.stu.atomikos.service.StuJotmServiceImpl">
<propertyname="tabADAO"ref="tabADAO"/>
<propertyname="tabBDAO"ref="tabBDAO"/>
</bean>
</beans>
