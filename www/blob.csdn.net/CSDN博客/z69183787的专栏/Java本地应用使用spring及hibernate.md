# Java本地应用使用spring及hibernate - z69183787的专栏 - CSDN博客
2012年11月25日 20:56:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1124
个人分类：[Java-知识																[Spring-知识																[对象映射框架-Hibernate](https://blog.csdn.net/z69183787/article/category/2175489)](https://blog.csdn.net/z69183787/article/category/2175163)](https://blog.csdn.net/z69183787/article/category/2175279)
ApplicationContext.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN" 
"http://www.springframework.org/dtd/spring-beans.dtd">
<beans>
	<!-- =================================================================== -->
	<!-- Context Define                                                      -->
	<!-- =================================================================== -->		
	<bean
		id="propertyConfigurer"
		class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
		<property name="locations">
			<list>
				<value>classpath:conf/hibernate.properties</value>
			</list>
		</property> 
	</bean>
	
	<!-- =================================================================== -->
	<!-- Data Source Define                                                  -->
	<!-- =================================================================== -->
	 
	
	<bean 
		id="dataSource" 
		class="org.springframework.jdbc.datasource.DriverManagerDataSource">
		<property name="driverClassName" value="${hibernate.connection.driver_class}"/>
		<property name="url" value="${hibernate.connection.url}"/>
		<property name="username" value="${hibernate.connection.username}"/>
		<property name="password" value="${hibernate.connection.password}"/>
	</bean>
		
	<!-- =================================================================== -->
	<!-- Hibernate Session Define                                            -->
	<!-- =================================================================== -->	
	<bean 
		id="sessionFactory" 
		class="org.springframework.orm.hibernate3.LocalSessionFactoryBean">
		<property name="dataSource" ref="dataSource"/>		
		<property name="hibernateProperties">
			<props>
				<prop key="hibernate.dialect">${hibernate.dialect}</prop>
				<prop key="hibernate.cache.provider_class">${hibernate.cache.provider_class}</prop>
				<prop key="hibernate.cache.use_query_cache">true</prop>
				<!-- prop key="hibernate.jdbc.batch_size">0</prop>
				<prop key="hibernate.connection.pool_size">3</prop -->
				<prop key="hibernate.show_sql">${hibernate.show_sql}</prop>
				<prop key="hibernate.query.factory_class">${hibernate.query.factory_class}</prop>
			</props>
		</property>
		<property name="lobHandler" ref="${jdbc.handler}"/>
	</bean>
	
	<bean
		id="oracleLobHandler"
		class="org.springframework.jdbc.support.lob.OracleLobHandler">
		<property name="nativeJdbcExtractor" ref="nativeJdbcExtractor"/>
	</bean>	
	
	<bean
		id="defaultLobHandler"
		class="org.springframework.jdbc.support.lob.DefaultLobHandler" lazy-init="true">
	</bean>	
	
	<bean 
		id="nativeJdbcExtractor" 
		class="org.springframework.jdbc.support.nativejdbc.SimpleNativeJdbcExtractor" 
		lazy-init="true"
	/>
	
	<!-- =================================================================== -->
	<!-- Transaction Define                                                  -->
	<!-- =================================================================== -->
	<bean 
		id="transactionManager" 
		class="org.springframework.orm.hibernate3.HibernateTransactionManager">
		<property name="sessionFactory" ref="sessionFactory"/>
	</bean>
	
	<bean
		id="basicTxProxy" abstract="true"
		class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean">
		<property name="transactionManager" ref="transactionManager"/>
		<property name="transactionAttributes">
			<props>
				<prop key="save*">PROPAGATION_REQUIRED</prop>
				<prop key="add*">PROPAGATION_REQUIRED</prop>
				<prop key="remove*">PROPAGATION_REQUIRED</prop>
				<prop key="update*">PROPAGATION_REQUIRED</prop>
				<prop key="*">PROPAGATION_REQUIRED</prop>
			</props>
		</property>
	</bean>
</beans>
```
datasource的另一种配置方式：jndi
```
<bean id="dataSource" class="org.springframework.jndi.JndiObjectFactoryBean">
		<property name="jndiName" value="jdbc/stptdemo"/>
		</bean>
```
hibernate.properties:
```
######################
### Query Language ###
######################
## define query language constants / function names
hibernate.query.substitutions true 1, false 0, yes 'Y', no 'N'
##Hibernate 2.1 query parser (for Weblogic 8.1)
#hibernate.query.factory_class org.hibernate.hql.classic.ClassicQueryTranslatorFactory
##Hibernate 3.0 query parser 
hibernate.query.factory_class org.hibernate.hql.ast.ASTQueryTranslatorFactory
###########################
### C3P0 Connection Pool###
###########################
#hibernate.c3p0.max_size 2
#hibernate.c3p0.min_size 2
#hibernate.c3p0.timeout 5000
#hibernate.c3p0.max_statements 100
#hibernate.c3p0.idle_test_period 3000
#hibernate.c3p0.acquire_increment 2
##hibernate.c3p0.validate false
#################################
### Hibernate Connection Pool ###
#################################
hibernate.connection.pool_size 1
hibernate.show_sql true
hibernate.format_sql false
##############################
### Proxool Connection Pool###
##############################
## Properties for external configuration of Proxool
hibernate.proxool.pool_alias pool1
##############################
### Miscellaneous Settings ###
##############################
## set the maximum JDBC 2 batch size (a nonzero value enables batching)
hibernate.jdbc.batch_size 0
## use streams when writing binary types to / from JDBC
hibernate.jdbc.use_streams_for_binary true
## set the maximum depth of the outer join fetch tree
hibernate.max_fetch_depth 1
##########################
### Second-level Cache ###
##########################
## enable the query cache
## hibernate.cache.use_query_cache true
## choose a cache implementation
hibernate.cache.provider_class org.hibernate.cache.OSCacheProvider
#################
### Platforms ###
#################
## Oracle
jdbc.handler oracleLobHandler
#jdbc.handler defaultLobHandler
hibernate.dialect org.hibernate.dialect.Oracle9Dialect
hibernate.connection.driver_class oracle.jdbc.driver.OracleDriver
hibernate.connection.username stpt
#hibernate.connection.password stpt.db.209
hibernate.connection.password shSTpt
hibernate.connection.url jdbc:oracle:thin:@10.1.41.101:1521:orcl
#hibernate.connection.url jdbc:oracle:thin:@10.1.44.209:1521:orcl
#hibernate.connection.url jdbc:oracle:thin:@10.1.43.11:1521:stnic1
## SQLServer
#hibernate.dialect org.hibernate.dialect.SQLServerDialect
#hibernate.connection.driver_class net.sourceforge.jtds.jdbc.Driver
#hibernate.connection.url jdbc:jtds:sqlserver://localhost:1433/CuteFramework
#hibernate.connection.username sa
#hibernate.connection.password sa
## Informix
#hibernate.dialect org.hibernate.dialect.Informix9Dialect
#hibernate.connection.driver_class com.informix.jdbc.IfxDriver
#hibernate.connection.username uaas
#hibernate.connection.password uaas
#hibernate.connection.url jdbc:informix-sqli://10.3.11.12:7515/uaas:INFORMIXSERVER=k460secsoc
## DB2
#hibernate.dialect org.hibernate.dialect.DB2Dialect
#hibernate.connection.driver_class com.ibm.db2.jcc.DB2Driver
#hibernate.connection.username db2admin
#hibernate.connection.password db2admin
#hibernate.connection.url jdbc:db2://wjh:50000/frame
## DO NOT specify hibernate.connection.sqlDialect
```
也可在 hibernate.cfg.xml中配置：
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE hibernate-configuration PUBLIC "-//Hibernate/Hibernate Configuration DTD 3.0//EN" "http://hibernate.sourceforge.net/hibernate-configuration-3.0.dtd">
<!-- NOTE: For Hibernate Session Factory Use Only -->
<hibernate-configuration> 
  <session-factory> 
    <property name="connection.pool_size">1</property>  
    <property name="show_sql">false</property>  
    <property name="format_sql">true</property>  
    <property name="cache.provider_class">org.hibernate.cache.OSCacheProvider</property>  
    <property name="cache.use_query_cache">true</property>  
    <property name="query.factory_class">org.hibernate.hql.ast.ASTQueryTranslatorFactory</property>  
    <property name="query.substitutions">true 1, false 0, yes 'Y', no 'N'</property>  
    <property name="proxool.pool_alias">pool1</property>  
    <property name="connection.SetBigStringTryClob">true</property>  
    <property name="jdbc.batch_size">0</property>  
    <property name="jdbc.use_streams_for_binary">true</property>  
    <property name="max_fetch_depth">1</property>  
</session-factory> 
</hibernate-configuration>
```
文件applicaitonContext.xml中sessionfactory bean中增加属性 ：
```
<property name="configLocation"
			value="classpath:hibernate.cfg.xml" />
```
读取配置。
main.java
```java
/**
	 * spring上下文
	 */
	ApplicationContext applicationContext = null;
	/**
	 * 初始化spring上下文
	 */
	protected void initAppContext() {
		String[] fileUrl = new String[]{"classpath*:conf/*Context*.xml"};
		applicationContext = new ClassPathXmlApplicationContext(fileUrl);
	}
```

