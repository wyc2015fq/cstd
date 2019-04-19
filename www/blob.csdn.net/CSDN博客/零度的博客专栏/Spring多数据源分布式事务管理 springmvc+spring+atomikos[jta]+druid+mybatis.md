# Spring多数据源分布式事务管理/springmvc+spring+atomikos[jta]+druid+mybatis - 零度的博客专栏 - CSDN博客
2017年01月11日 16:06:50[零度anngle](https://me.csdn.net/zmx729618)阅读数：4870
        项目进行读写分离及分库分表，在一个业务中，在一个事务中处理时候将切换多个数据源，需要保证同一事务多个数据源数据的一致性。
        此处使用atomikos来实现(附源码)。另外需要注意以下：
       1：spring3.0之后不再支持jtom[jta]了，支持第三方开源软件atomikos([http://www.atomikos.com/](http://www.atomikos.com/))来实现． 
       2：org.springframework.transaction.jta.JotmFactoryBean类，spring-tx-2.5.6.jar中有此类，spring-tx-3.0.0.RELEASE.jar之后没有此类。
       3：atomikos事务控制框架，其中看到有3种数据源，分别是，SimpleDataSourceBean,AtomikosDataSourceBean,AtomikosNonXADataSourceBean。
            1、SimpleDataSourceBean: 这个是最简单地数据源配置,需要配置XA驱动。
            2、AtomikosDataSourceBean:  分布式数据源,Atomikos实现的数据源，需要配置XA驱动，推荐此配置，可以配置连接池的信息。
            3、AtomikosNonXADataSourceBean: 非分布式数据源,该数据源配置需要普通JDBC的驱动，可以配置连接池：
       4：Atomikos支持XA(全局事务)和NON-XA(非全局事务)，NON-XA[nonxadatasource]效率高于XA。XA事务往往是包括多个数据源的全局事务，非XA是单个数据源的.
       5：XA连接是一个JTA事务中的参与者。XA连接不支持JDBC的自动提交特性。也就是说应用程序不必在xadatasource[XA]连接上调用java.sql.Connection.commit()或java.sql.Connection.rollback()；而应用程序应该使用UserTransaction.begin(),UserTransaction.commit()和UserTransaction.rollback().
看看pom.xml依赖：
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- <!-- transaction -->
- <dependency>
- <groupId>org.springframework</groupId>
- <artifactId>spring-tx</artifactId>
- <version>4.2.5.RELEASE</version>
- </dependency>
- <dependency>
- <groupId>javax.transaction</groupId>
- <artifactId>jta</artifactId>
- <version>1.1</version>
- </dependency>
- <dependency>
- <groupId>com.atomikos</groupId>
- <artifactId>atomikos-util</artifactId>
- <version>4.0.2</version>
- </dependency>
- <dependency>
- <groupId>com.atomikos</groupId>
- <artifactId>transactions</artifactId>
- <version>4.0.2</version>
- </dependency>
- <dependency>
- <groupId>com.atomikos</groupId>
- <artifactId>transactions-jta</artifactId>
- <version>4.0.2</version>
- </dependency>
- <dependency>
- <groupId>com.atomikos</groupId>
- <artifactId>transactions-jdbc</artifactId>
- <version>4.0.2</version>
- </dependency>
- <dependency>
- <groupId>com.atomikos</groupId>
- <artifactId>transactions-api</artifactId>
- <version>4.0.2</version>
- </dependency>
- <dependency>
- <groupId>cglib</groupId>
- <artifactId>cglib-nodep</artifactId>
- <version>3.2.2</version>
- </dependency>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<!-- transaction -->
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-tx</artifactId>
			<version>4.2.5.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>javax.transaction</groupId>
			<artifactId>jta</artifactId>
			<version>1.1</version>
		</dependency>
		<dependency>
			<groupId>com.atomikos</groupId>
			<artifactId>atomikos-util</artifactId>
			<version>4.0.2</version>
		</dependency>
		<dependency>
			<groupId>com.atomikos</groupId>
			<artifactId>transactions</artifactId>
			<version>4.0.2</version>
		</dependency>
		<dependency>
			<groupId>com.atomikos</groupId>
			<artifactId>transactions-jta</artifactId>
			<version>4.0.2</version>
		</dependency>
		<dependency>
			<groupId>com.atomikos</groupId>
			<artifactId>transactions-jdbc</artifactId>
			<version>4.0.2</version>
		</dependency>
		<dependency>
			<groupId>com.atomikos</groupId>
			<artifactId>transactions-api</artifactId>
			<version>4.0.2</version>
		</dependency>
		<dependency>
			<groupId>cglib</groupId>
			<artifactId>cglib-nodep</artifactId>
			<version>3.2.2</version>
		</dependency>
```
1：AtomikosDataSourceBean[XA(全局事务)]数据源配置datasource-context.xml：
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:context="http://www.springframework.org/schema/context"
- xmlns:mvc="http://www.springframework.org/schema/mvc"xmlns:tx="http://www.springframework.org/schema/tx"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xsi:schemaLocation="http://www.springframework.org/schema/mvc   
-     http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd  
-     http://www.springframework.org/schema/beans   
-     http://www.springframework.org/schema/beans/spring-beans-4.0.xsd  
-     http://www.springframework.org/schema/context   
-     http://www.springframework.org/schema/context/spring-context-4.0.xsd  
-     http://www.springframework.org/schema/tx   
-     http://www.springframework.org/schema/tx/spring-tx-4.0.xsd  
-     http://www.springframework.org/schema/aop   
-     http://www.springframework.org/schema/aop/spring-aop-4.0.xsd" default-lazy-init="true">
- <description>配置主-从数据源信息</description>
- <!-- com.atomikos.jdbc.nonxa.AtomikosNonXADataSourceBean -->
- <beanid="abstractXADataSource"class="com.atomikos.jdbc.AtomikosDataSourceBean"init-method="init"destroy-method="close"abstract="true">
- <propertyname="xaDataSourceClassName"value="${jdbc.xaDataSourceClassName}"/><!-- SQLErrorCodes loaded: [DB2, Derby, H2, HSQL, Informix, MS-SQL, MySQL, Oracle, PostgreSQL, Sybase, Hana]  -->
- <propertyname="poolSize"value="10"/>
- <propertyname="minPoolSize"value="10"/>
- <propertyname="maxPoolSize"value="30"/>
- <propertyname="borrowConnectionTimeout"value="60"/>
- <propertyname="reapTimeout"value="20"/>
- <propertyname="maxIdleTime"value="60"/>
- <propertyname="maintenanceInterval"value="60"/>
- <propertyname="loginTimeout"value="60"/>
- <propertyname="testQuery"value="${validationQuery}"/>
- </bean>
- <beanid="masterDataSource"parent="abstractXADataSource">
- <propertyname="uniqueResourceName"value="masterDB"/>
- <propertyname="xaProperties">
- <props>
- <propkey="driverClassName">${jdbc.driverClassName}</prop>
- <propkey="url">${master.jdbc.url}</prop>
- <propkey="password">${jdbc.password}</prop>
- <!--  <prop key="user">${jdbc.username}</prop> --><!-- mysql -->
- <propkey="username">${jdbc.username}</prop><!-- durid -->
- <propkey="initialSize">0</prop>
- <propkey="maxActive">20</prop><!-- 若不配置则代码执行"{dataSource-1} inited"此处停止  -->
- <propkey="minIdle">0</prop>
- <propkey="maxWait">60000</prop>
- <propkey="validationQuery">${validationQuery}</prop>
- <propkey="testOnBorrow">false</prop>
- <propkey="testOnReturn">false</prop>
- <propkey="testWhileIdle">true</prop>
- <propkey="removeAbandoned">true</prop>
- <propkey="removeAbandonedTimeout">1800</prop>
- <propkey="logAbandoned">true</prop>
- <propkey="filters">mergeStat</prop>
- </props>
- </property>
- </bean>
- <beanid="slaveDataSource"parent="abstractXADataSource">
- <propertyname="uniqueResourceName"value="slaveDB"/>
- <propertyname="xaProperties">
- <props>
- <propkey="driverClassName">${jdbc.driverClassName}</prop>
- <propkey="url">${slave.jdbc.url}</prop>
- <propkey="password">${jdbc.password}</prop>
- <!--  <prop key="user">${jdbc.username}</prop> -->
- <propkey="username">${jdbc.username}</prop>
- <propkey="initialSize">0</prop>
- <propkey="maxActive">20</prop>
- <propkey="minIdle">0</prop>
- <propkey="maxWait">60000</prop>
- <propkey="validationQuery">${validationQuery}</prop>
- <propkey="testOnBorrow">false</prop>
- <propkey="testOnReturn">false</prop>
- <propkey="testWhileIdle">true</prop>
- <propkey="removeAbandoned">true</prop>
- <propkey="removeAbandonedTimeout">1800</prop>
- <propkey="logAbandoned">true</prop>
- <propkey="filters">mergeStat</prop>
- </props>
- </property>
- </bean>
- </beans>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context"
	xmlns:mvc="http://www.springframework.org/schema/mvc" xmlns:tx="http://www.springframework.org/schema/tx"
	xmlns:aop="http://www.springframework.org/schema/aop"
	xsi:schemaLocation="http://www.springframework.org/schema/mvc 
	http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd
	http://www.springframework.org/schema/beans 
	http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
	http://www.springframework.org/schema/context 
	http://www.springframework.org/schema/context/spring-context-4.0.xsd
	http://www.springframework.org/schema/tx 
	http://www.springframework.org/schema/tx/spring-tx-4.0.xsd
	http://www.springframework.org/schema/aop 
	http://www.springframework.org/schema/aop/spring-aop-4.0.xsd" default-lazy-init="true">
	<description>配置主-从数据源信息</description>
	<!-- com.atomikos.jdbc.nonxa.AtomikosNonXADataSourceBean -->
 	<bean id="abstractXADataSource" class="com.atomikos.jdbc.AtomikosDataSourceBean" init-method="init" destroy-method="close" abstract="true">  
       <property name="xaDataSourceClassName" value="${jdbc.xaDataSourceClassName}"/>  <!-- SQLErrorCodes loaded: [DB2, Derby, H2, HSQL, Informix, MS-SQL, MySQL, Oracle, PostgreSQL, Sybase, Hana]  -->
       <property name="poolSize" value="10" />  
       <property name="minPoolSize" value="10"/>  
       <property name="maxPoolSize" value="30"/>  
       <property name="borrowConnectionTimeout" value="60"/>  
       <property name="reapTimeout" value="20"/>  
       <property name="maxIdleTime" value="60"/>  
       <property name="maintenanceInterval" value="60"/>  
       <property name="loginTimeout" value="60"/>  
       <property name="testQuery" value="${validationQuery}"/>  
   	</bean>  
 	<bean id="masterDataSource" parent="abstractXADataSource">  
 		<property name="uniqueResourceName" value="masterDB" />  
	    <property name="xaProperties">
            <props>
           		<prop key="driverClassName">${jdbc.driverClassName}</prop>
                <prop key="url">${master.jdbc.url}</prop>
                <prop key="password">${jdbc.password}</prop>
                 <!--  <prop key="user">${jdbc.username}</prop> --> <!-- mysql -->
                <prop key="username">${jdbc.username}</prop>   <!-- durid -->
             	<prop key="initialSize">0</prop>
				<prop key="maxActive">20</prop> <!-- 若不配置则代码执行"{dataSource-1} inited"此处停止  -->
				<prop key="minIdle">0</prop>
				<prop key="maxWait">60000</prop>
           		<prop key="validationQuery">${validationQuery}</prop>
				<prop key="testOnBorrow">false</prop>
				<prop key="testOnReturn">false</prop>
				<prop key="testWhileIdle">true</prop>
				<prop key="removeAbandoned">true</prop>
				<prop key="removeAbandonedTimeout">1800</prop>
				<prop key="logAbandoned">true</prop>
				<prop key="filters">mergeStat</prop>
            </props>
        </property>
    </bean>  
   	<bean id="slaveDataSource" parent="abstractXADataSource">  
   		<property name="uniqueResourceName" value="slaveDB" />  
	    <property name="xaProperties">
            <props>
          		<prop key="driverClassName">${jdbc.driverClassName}</prop>
                <prop key="url">${slave.jdbc.url}</prop>
                <prop key="password">${jdbc.password}</prop>
                <!--  <prop key="user">${jdbc.username}</prop> -->
                <prop key="username">${jdbc.username}</prop>
               	<prop key="initialSize">0</prop>
				<prop key="maxActive">20</prop>
				<prop key="minIdle">0</prop>
				<prop key="maxWait">60000</prop>
                <prop key="validationQuery">${validationQuery}</prop>
				<prop key="testOnBorrow">false</prop>
				<prop key="testOnReturn">false</prop>
				<prop key="testWhileIdle">true</prop>
				<prop key="removeAbandoned">true</prop>
				<prop key="removeAbandonedTimeout">1800</prop>
				<prop key="logAbandoned">true</prop>
				<prop key="filters">mergeStat</prop>
            </props>
        </property>
    </bean>  
</beans>
```
2：spring主配置文件spring-context.xml:
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:p="http://www.springframework.org/schema/p"
- xmlns:context="http://www.springframework.org/schema/context"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xmlns:mvc="http://www.springframework.org/schema/mvc"
- xsi:schemaLocation="http://www.springframework.org/schema/beans      
-                         http://www.springframework.org/schema/beans/spring-beans-3.2.xsd      
-                         http://www.springframework.org/schema/context      
-                         http://www.springframework.org/schema/context/spring-context-3.2.xsd  
-                         http://www.springframework.org/schema/aop   
-                         http://www.springframework.org/schema/aop/spring-aop-3.2.xsd      
-                         http://www.springframework.org/schema/mvc      
-                         http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd">
- 
- <!-- 使用annotation 自动注册bean,并检查@Required,@Autowired的属性已被注入 -->
- <context:component-scanbase-package="com.tx"/>
- 
- <!-- 使用AspectJ方式配置AOP -->
- <aop:aspectj-autoproxy/>
- 
- <!-- 引入属性配置文件 -->
- <beanid="propertyConfigurer"class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
- <propertyname="location"value="classpath:properties/database.properties"/>
- </bean>
- 
- <!--或 <context:property-placeholder location="classpath*:*.properties" /> -->
- </beans>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8"?>  
<beans xmlns="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"  
    xmlns:context="http://www.springframework.org/schema/context"  
    xmlns:aop="http://www.springframework.org/schema/aop"
    xmlns:mvc="http://www.springframework.org/schema/mvc"  
    xsi:schemaLocation="http://www.springframework.org/schema/beans    
                        http://www.springframework.org/schema/beans/spring-beans-3.2.xsd    
                        http://www.springframework.org/schema/context    
                        http://www.springframework.org/schema/context/spring-context-3.2.xsd
                        http://www.springframework.org/schema/aop 
            			http://www.springframework.org/schema/aop/spring-aop-3.2.xsd    
                        http://www.springframework.org/schema/mvc    
                        http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd">  
                        
	<!-- 使用annotation 自动注册bean,并检查@Required,@Autowired的属性已被注入 -->
	<context:component-scan base-package="com.tx" />
	
 	<!-- 使用AspectJ方式配置AOP -->  
	<aop:aspectj-autoproxy />
	
	<!-- 引入属性配置文件 -->
 	<bean id="propertyConfigurer" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer"> 
		<property name="location" value="classpath:properties/database.properties" />        
	</bean>  
	
	<!--或 <context:property-placeholder location="classpath*:*.properties" /> -->
</beans>
```
3:数据源配置参数database.properties:
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- #mysql-Used to verify the effectiveness of the database connection   
- validationQuery=SELECT 1  
- jdbc.initialSize=5
- jdbc.maxActive=20
- jdbc.maxWait=60000
- jdbc.poolPreparedStatements=false
- jdbc.poolMaximumIdleConnections=0
- jdbc.driverClassName=org.gjt.mm.mysql.Driver  
- jdbc.xaDataSourceClassName=com.alibaba.druid.pool.xa.DruidXADataSource  
- #jdbc.xaDataSourceClassName=com.mysql.jdbc.jdbc2.optional.MysqlXADataSource  
- #1.tms business.  2.The db level optimization,data concurrency,desirable.  
- master.jdbc.url=jdbc:mysql://your ip:3306/master?useUnicode=true&characterEncoding=UTF-8&zeroDateTimeBehavior=convertToNull
- slave.jdbc.url=jdbc:mysql://your ip:3306/slave?useUnicode=true&characterEncoding=UTF-8&zeroDateTimeBehavior=convertToNull
- jdbc.username=username
- jdbc.password=password
![](http://static.blog.csdn.net/images/save_snippets.png)
```
#mysql-Used to verify the effectiveness of the database connection 
validationQuery=SELECT 1
jdbc.initialSize=5
jdbc.maxActive=20
jdbc.maxWait=60000
jdbc.poolPreparedStatements=false
jdbc.poolMaximumIdleConnections=0
jdbc.driverClassName=org.gjt.mm.mysql.Driver
jdbc.xaDataSourceClassName=com.alibaba.druid.pool.xa.DruidXADataSource
#jdbc.xaDataSourceClassName=com.mysql.jdbc.jdbc2.optional.MysqlXADataSource
#1.tms business.  2.The db level optimization,data concurrency,desirable.
master.jdbc.url=jdbc:mysql://your ip:3306/master?useUnicode=true&characterEncoding=UTF-8&zeroDateTimeBehavior=convertToNull
slave.jdbc.url=jdbc:mysql://your ip:3306/slave?useUnicode=true&characterEncoding=UTF-8&zeroDateTimeBehavior=convertToNull
jdbc.username=username
jdbc.password=password
```
4:mybatis的配置mybatis-context.xml：
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:context="http://www.springframework.org/schema/context"
- xmlns:mvc="http://www.springframework.org/schema/mvc"
- xmlns:tx="http://www.springframework.org/schema/tx"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xsi:schemaLocation="http://www.springframework.org/schema/mvc   
-     http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd  
-     http://www.springframework.org/schema/beans   
-     http://www.springframework.org/schema/beans/spring-beans-4.0.xsd  
-     http://www.springframework.org/schema/context   
-     http://www.springframework.org/schema/context/spring-context-4.0.xsd  
-     http://www.springframework.org/schema/tx   
-     http://www.springframework.org/schema/tx/spring-tx-4.0.xsd  
-     http://www.springframework.org/schema/aop   
-     http://www.springframework.org/schema/aop/spring-aop-4.0.xsd" default-lazy-init="true">
- <description>MyBatis的数据库持久层配置/配置主-从数据源</description>
- <beanid="masterSqlSessionFactory"class="org.mybatis.spring.SqlSessionFactoryBean">
- <propertyname="configLocation"value="classpath:mybatis/mybatis-config-master.xml"/>
- <propertyname="dataSource"ref="masterDataSource"/>
- </bean>
- 
- <beanid="slaveSqlSessionFactory"class="org.mybatis.spring.SqlSessionFactoryBean">
- <propertyname="configLocation"value="classpath:mybatis/mybatis-config-slave.xml"/>
- <propertyname="dataSource"ref="slaveDataSource"/>
- </bean>
- </beans>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xmlns:context="http://www.springframework.org/schema/context"
	xmlns:mvc="http://www.springframework.org/schema/mvc" 
	xmlns:tx="http://www.springframework.org/schema/tx"
	xmlns:aop="http://www.springframework.org/schema/aop" 
	xsi:schemaLocation="http://www.springframework.org/schema/mvc 
	http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd
	http://www.springframework.org/schema/beans 
	http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
	http://www.springframework.org/schema/context 
	http://www.springframework.org/schema/context/spring-context-4.0.xsd
	http://www.springframework.org/schema/tx 
	http://www.springframework.org/schema/tx/spring-tx-4.0.xsd
	http://www.springframework.org/schema/aop 
	http://www.springframework.org/schema/aop/spring-aop-4.0.xsd" default-lazy-init="true">
	<description>MyBatis的数据库持久层配置/配置主-从数据源</description>
	<bean id="masterSqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <property name="configLocation" value="classpath:mybatis/mybatis-config-master.xml" />
        <property name="dataSource" ref="masterDataSource" />
    </bean>
 
    <bean id="slaveSqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <property name="configLocation" value="classpath:mybatis/mybatis-config-slave.xml" />
        <property name="dataSource" ref="slaveDataSource" />
    </bean>
</beans>
```
配置mybatis-config(此代码，只为测试分布式事务，并不涉及真实的业务！！！)：
mybatis-config-master.xml：
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" "http://mybatis.org/dtd/mybatis-3-config.dtd">
- <configuration>
- <typeAliases>
- <typeAliasalias="Member"type="com.tx.entity.Member"/>
- </typeAliases>
- <mappers>
- <mapperresource="com/tx/xml/MemberMapper.xml"/>
- </mappers>
- </configuration>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <typeAliases>
        <typeAlias alias="Member"  type="com.tx.entity.Member"/>
    </typeAliases>
    <mappers>
        <mapper resource="com/tx/xml/MemberMapper.xml" />
    </mappers>
</configuration>
```
mybatis-config-slave.xml
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" "http://mybatis.org/dtd/mybatis-3-config.dtd">
- <configuration>
- <typeAliases>
- <typeAliasalias="MemberInfo"type="com.tx.entity.MemberInfo"/>
- </typeAliases>
- <mappers>
- <mapperresource="com/tx/xml/MemberInfoMapper.xml"/>
- </mappers>
- </configuration>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" "http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <typeAliases>
        <typeAlias alias="MemberInfo"  type="com.tx.entity.MemberInfo"/>
    </typeAliases>
    <mappers>
        <mapper resource="com/tx/xml/MemberInfoMapper.xml" />
    </mappers>
</configuration>
```
5：Mapper的管理及注入，为mybatis的dao层mapper接口注入[绑定]sqlSessionFactory：
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:context="http://www.springframework.org/schema/context"
- xmlns:mvc="http://www.springframework.org/schema/mvc"
- xmlns:tx="http://www.springframework.org/schema/tx"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xsi:schemaLocation="http://www.springframework.org/schema/mvc   
-     http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd  
-     http://www.springframework.org/schema/beans   
-     http://www.springframework.org/schema/beans/spring-beans-4.0.xsd  
-     http://www.springframework.org/schema/context   
-     http://www.springframework.org/schema/context/spring-context-4.0.xsd  
-     http://www.springframework.org/schema/tx   
-     http://www.springframework.org/schema/tx/spring-tx-4.0.xsd  
-     http://www.springframework.org/schema/aop   
-     http://www.springframework.org/schema/aop/spring-aop-4.0.xsd" default-lazy-init="true">
- <description>MyBatis为不同的mapper注入sqlSessionFactory</description>
- <!-- Mapper的管理及注入 -->
- <beanid="memberMapper"class="org.mybatis.spring.mapper.MapperFactoryBean">
- <propertyname="sqlSessionFactory"ref="masterSqlSessionFactory"/>
- <propertyname="mapperInterface"value="com.tx.dao.MemberMapper"/>
- </bean>
- 
- <beanid="memberInfoMapper"class="org.mybatis.spring.mapper.MapperFactoryBean">
- <propertyname="sqlSessionFactory"ref="slaveSqlSessionFactory"/>
- <propertyname="mapperInterface"value="com.tx.dao.MemberInfoMapper"/>
- </bean>
- </beans>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xmlns:context="http://www.springframework.org/schema/context"
	xmlns:mvc="http://www.springframework.org/schema/mvc" 
	xmlns:tx="http://www.springframework.org/schema/tx"
	xmlns:aop="http://www.springframework.org/schema/aop" 
	xsi:schemaLocation="http://www.springframework.org/schema/mvc 
	http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd
	http://www.springframework.org/schema/beans 
	http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
	http://www.springframework.org/schema/context 
	http://www.springframework.org/schema/context/spring-context-4.0.xsd
	http://www.springframework.org/schema/tx 
	http://www.springframework.org/schema/tx/spring-tx-4.0.xsd
	http://www.springframework.org/schema/aop 
	http://www.springframework.org/schema/aop/spring-aop-4.0.xsd" default-lazy-init="true">
	<description>MyBatis为不同的mapper注入sqlSessionFactory</description>
    <!-- Mapper的管理及注入 -->
    <bean id="memberMapper" class="org.mybatis.spring.mapper.MapperFactoryBean">
        <property name="sqlSessionFactory" ref="masterSqlSessionFactory" />
        <property name="mapperInterface" value="com.tx.dao.MemberMapper" />
    </bean>
     
    <bean id="memberInfoMapper" class="org.mybatis.spring.mapper.MapperFactoryBean">
        <property name="sqlSessionFactory" ref="slaveSqlSessionFactory" />
        <property name="mapperInterface" value="com.tx.dao.MemberInfoMapper" />
    </bean>
</beans>
```
6：atomikos事务配置transaction-context.xml：
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:context="http://www.springframework.org/schema/context"
- xmlns:mvc="http://www.springframework.org/schema/mvc"xmlns:tx="http://www.springframework.org/schema/tx"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xsi:schemaLocation="http://www.springframework.org/schema/mvc   
-     http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd  
-     http://www.springframework.org/schema/beans   
-     http://www.springframework.org/schema/beans/spring-beans-4.0.xsd  
-     http://www.springframework.org/schema/context   
-     http://www.springframework.org/schema/context/spring-context-4.0.xsd  
-     http://www.springframework.org/schema/tx   
-     http://www.springframework.org/schema/tx/spring-tx-4.0.xsd  
-     http://www.springframework.org/schema/aop   
-     http://www.springframework.org/schema/aop/spring-aop-4.0.xsd" default-lazy-init="true">
- <description>配置事物</description>
- <!-- atomikos事务管理器 -->
- <beanid="atomikosTransactionManager"class="com.atomikos.icatch.jta.UserTransactionManager"init-method="init"destroy-method="close">
- <propertyname="forceShutdown">
- <value>true</value>
- </property>
- </bean>
- 
- <beanid="atomikosUserTransaction"class="com.atomikos.icatch.jta.UserTransactionImp">
- <propertyname="transactionTimeout"value="300"/>
- </bean>
- <!-- spring 事务管理器 -->
- <beanid="springTransactionManager"class="org.springframework.transaction.jta.JtaTransactionManager">
- <propertyname="transactionManager"ref="atomikosTransactionManager"/>
- <propertyname="userTransaction"ref="atomikosUserTransaction"/>
- <!-- 必须设置，否则程序出现异常 JtaTransactionManager does not support custom isolation levels by default -->
- <propertyname="allowCustomIsolationLevels"value="true"/>
- </bean>
- 
- <aop:configproxy-target-class="true">
- <aop:advisorpointcut="(execution(* com.tx.service.*.* (..)))"advice-ref="txAdvice"/>
- </aop:config>
- 
- <tx:adviceid="txAdvice"transaction-manager="springTransactionManager">
- <tx:attributes>
- <tx:methodname="get*"propagation="REQUIRED"read-only="true"/>
- <tx:methodname="find*"propagation="REQUIRED"read-only="true"/>
- <tx:methodname="has*"propagation="REQUIRED"read-only="true"/>
- <tx:methodname="locate*"propagation="REQUIRED"read-only="true"/>
- <tx:methodname="register*"propagation="REQUIRED"rollback-for="java.lang.Exception"/>
- </tx:attributes>
- </tx:advice>
- </beans>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context"
	xmlns:mvc="http://www.springframework.org/schema/mvc" xmlns:tx="http://www.springframework.org/schema/tx"
	xmlns:aop="http://www.springframework.org/schema/aop"
	xsi:schemaLocation="http://www.springframework.org/schema/mvc 
	http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd
	http://www.springframework.org/schema/beans 
	http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
	http://www.springframework.org/schema/context 
	http://www.springframework.org/schema/context/spring-context-4.0.xsd
	http://www.springframework.org/schema/tx 
	http://www.springframework.org/schema/tx/spring-tx-4.0.xsd
	http://www.springframework.org/schema/aop 
	http://www.springframework.org/schema/aop/spring-aop-4.0.xsd" default-lazy-init="true">
	<description>配置事物</description>
	<!-- atomikos事务管理器 -->
	<bean id="atomikosTransactionManager" class="com.atomikos.icatch.jta.UserTransactionManager" init-method="init" destroy-method="close">
        <property name="forceShutdown">
            <value>true</value>
        </property>
    </bean>
 
    <bean id="atomikosUserTransaction" class="com.atomikos.icatch.jta.UserTransactionImp">
        <property name="transactionTimeout" value="300" />
    </bean>
 	<!-- spring 事务管理器 -->  
    <bean id="springTransactionManager" class="org.springframework.transaction.jta.JtaTransactionManager">
        <property name="transactionManager" ref="atomikosTransactionManager" />
        <property name="userTransaction" ref="atomikosUserTransaction" />
        <!-- 必须设置，否则程序出现异常 JtaTransactionManager does not support custom isolation levels by default -->
        <property name="allowCustomIsolationLevels" value="true"/> 
    </bean>
    <aop:config  proxy-target-class="true">
        <aop:advisor pointcut="(execution(* com.tx.service.*.* (..)))" advice-ref="txAdvice" />
    </aop:config>
 
    <tx:advice id="txAdvice" transaction-manager="springTransactionManager">
        <tx:attributes>
            <tx:method name="get*"  propagation="REQUIRED"  read-only="true" />
            <tx:method name="find*"  propagation="REQUIRED"  read-only="true" />
            <tx:method name="has*"  propagation="REQUIRED"  read-only="true" />
            <tx:method name="locate*"  propagation="REQUIRED"  read-only="true" />
            <tx:method name="register*" propagation="REQUIRED" rollback-for="java.lang.Exception" />
        </tx:attributes>
    </tx:advice>
</beans>
```
7：配置jta启动参数在src下，最后追加详细：
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- com.atomikos.icatch.service=com.atomikos.icatch.standalone.UserTransactionServiceFactory  
- com.atomikos.icatch.console_file_name = /home/logs/tx/tx.out.log  
- com.atomikos.icatch.log_base_name = txlog
- com.atomikos.icatch.tm_unique_name = com.atomikos.spring.jdbc.tm  
- com.atomikos.icatch.console_log_level=DEBUG
![](http://static.blog.csdn.net/images/save_snippets.png)
```
com.atomikos.icatch.service=com.atomikos.icatch.standalone.UserTransactionServiceFactory
com.atomikos.icatch.console_file_name = /home/logs/tx/tx.out.log
com.atomikos.icatch.log_base_name = txlog
com.atomikos.icatch.tm_unique_name = com.atomikos.spring.jdbc.tm
com.atomikos.icatch.console_log_level=DEBUG
```
8：代码(部分不涉及代码已删除)：
a:mybatis的mapper和dao接口[MemberMapper/MemberInfoMapper]：
**[java]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- package com.tx.dao;  
- import com.tx.entity.Member;  
- publicinterface MemberMapper {  
- int insert(Member record);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.tx.dao;
import com.tx.entity.Member;
public interface MemberMapper {
    int insert(Member record);
}
```
**[java]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- package com.tx.dao;  
- import com.tx.entity.MemberInfo;  
- publicinterface MemberInfoMapper {  
- int insert(MemberInfo record);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.tx.dao;
import com.tx.entity.MemberInfo;
public interface MemberInfoMapper {
    int insert(MemberInfo record);
}
```
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
- <mappernamespace="com.tx.dao.MemberMapper">
- <resultMapid="BaseResultMap"type="com.tx.entity.Member">
- <idcolumn="id"property="id"jdbcType="INTEGER"/>
- <resultcolumn="username"property="username"jdbcType="VARCHAR"/>
- <resultcolumn="password"property="password"jdbcType="VARCHAR"/>
- <resultcolumn="status"property="status"jdbcType="TINYINT"/>
- </resultMap>
- <sqlid="Base_Column_List">
-     id, username, password, status  
- </sql>
- <insertid="insert"parameterType="com.tx.entity.Member">
-     insert into member (id, username, password,   
-       status)  
-     values (#{id,jdbcType=INTEGER}, #{username,jdbcType=VARCHAR}, #{password,jdbcType=VARCHAR},   
-       #{status,jdbcType=TINYINT})  
- </insert>
- </mapper>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
<mapper namespace="com.tx.dao.MemberMapper" >
  <resultMap id="BaseResultMap" type="com.tx.entity.Member" >
    <id column="id" property="id" jdbcType="INTEGER" />
    <result column="username" property="username" jdbcType="VARCHAR" />
    <result column="password" property="password" jdbcType="VARCHAR" />
    <result column="status" property="status" jdbcType="TINYINT" />
  </resultMap>
  <sql id="Base_Column_List" >
    id, username, password, status
  </sql>
  <insert id="insert" parameterType="com.tx.entity.Member" >
    insert into member (id, username, password, 
      status)
    values (#{id,jdbcType=INTEGER}, #{username,jdbcType=VARCHAR}, #{password,jdbcType=VARCHAR}, 
      #{status,jdbcType=TINYINT})
  </insert>
</mapper>
```
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
- <mappernamespace="com.tx.dao.MemberInfoMapper">
- <resultMapid="BaseResultMap"type="com.tx.entity.MemberInfo">
- <idcolumn="id"property="id"jdbcType="INTEGER"/>
- <resultcolumn="nickname"property="nickname"jdbcType="VARCHAR"/>
- <resultcolumn="realname"property="realname"jdbcType="VARCHAR"/>
- <resultcolumn="age"property="age"jdbcType="TINYINT"/>
- </resultMap>
- <sqlid="Base_Column_List">
-     id, nickname, realname, age  
- </sql>
- <insertid="insert"parameterType="com.tx.entity.MemberInfo">
-     insert into member_info (id, nickname, realname,   
-       age)  
-     values (#{id,jdbcType=INTEGER}, #{nickname,jdbcType=VARCHAR}, #{realname,jdbcType=VARCHAR},   
-       #{age,jdbcType=TINYINT})  
- </insert>
- </mapper>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd" >
<mapper namespace="com.tx.dao.MemberInfoMapper" >
  <resultMap id="BaseResultMap" type="com.tx.entity.MemberInfo" >
    <id column="id" property="id" jdbcType="INTEGER" />
    <result column="nickname" property="nickname" jdbcType="VARCHAR" />
    <result column="realname" property="realname" jdbcType="VARCHAR" />
    <result column="age" property="age" jdbcType="TINYINT" />
  </resultMap>
  <sql id="Base_Column_List" >
    id, nickname, realname, age
  </sql>
  <insert id="insert" parameterType="com.tx.entity.MemberInfo" >
    insert into member_info (id, nickname, realname, 
      age)
    values (#{id,jdbcType=INTEGER}, #{nickname,jdbcType=VARCHAR}, #{realname,jdbcType=VARCHAR}, 
      #{age,jdbcType=TINYINT})
  </insert>
</mapper>
```
b:服务层接口和实现：
**[java]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- package com.tx.sevice;  
- 
- import com.tx.entity.Member;  
- import com.tx.entity.MemberInfo;  
- 
- publicinterface MemberService {  
- boolean registerMember(Member member, MemberInfo memberInfo);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.tx.sevice;
import com.tx.entity.Member;
import com.tx.entity.MemberInfo;
public interface MemberService {
	boolean registerMember(Member member, MemberInfo memberInfo);
}
```
**[java]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- package com.tx.sevice.impl;  
- 
- import org.slf4j.Logger;  
- import org.slf4j.LoggerFactory;  
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.stereotype.Service;  
- 
- import com.tx.dao.MemberInfoMapper;  
- import com.tx.dao.MemberMapper;  
- import com.tx.entity.Member;  
- import com.tx.entity.MemberInfo;  
- import com.tx.sevice.MemberService;  
- 
- @Service("memberService")  
- publicclass MemberServiceImpl implements MemberService {  
- 
- //log
- privatestaticfinal Logger LOG = LoggerFactory.getLogger(MemberServiceImpl.class);  
- 
- @Autowired
- private MemberMapper memberMapper;  
- @Autowired
- private MemberInfoMapper memberInfoMapper;  
- 
- @Override
- publicboolean registerMember(Member member, MemberInfo memberInfo) {  
- boolean resRegister = false;  
- try {  
- if(memberMapper.insert(member) != 1){  
- thrownew RuntimeException("注册用户:Member表数据插入不一致.");  
-             }  
- if(memberInfoMapper.insert(memberInfo) != 1){  
- thrownew RuntimeException("注册用户:MemberInfo表数据插入不一致.");  
-             }  
-             resRegister = true;  
-         } catch (Exception e) {  
-             LOG.info("注册用户:数据库保存异常." + e.getMessage(), e);  
- thrownew RuntimeException("注册用户:数据库保存异常");  
-         }  
- return resRegister;  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.tx.sevice.impl;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import com.tx.dao.MemberInfoMapper;
import com.tx.dao.MemberMapper;
import com.tx.entity.Member;
import com.tx.entity.MemberInfo;
import com.tx.sevice.MemberService;
@Service("memberService")
public class MemberServiceImpl implements MemberService {
	
	//log
	private static final Logger LOG = LoggerFactory.getLogger(MemberServiceImpl.class);
	
	@Autowired
	private MemberMapper memberMapper;
	@Autowired
	private MemberInfoMapper memberInfoMapper;
	@Override
	public boolean registerMember(Member member, MemberInfo memberInfo) {
		boolean resRegister = false;
		try {
			if(memberMapper.insert(member) != 1){
				throw new RuntimeException("注册用户:Member表数据插入不一致.");
			}
			if(memberInfoMapper.insert(memberInfo) != 1){
				throw new RuntimeException("注册用户:MemberInfo表数据插入不一致.");
			}
			resRegister = true;
		} catch (Exception e) {
			LOG.info("注册用户:数据库保存异常." + e.getMessage(), e);
			throw new RuntimeException("注册用户:数据库保存异常");
		}
		return resRegister;
	}
}
```
c：junit测试代码：
**[java]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- package com.tx.test;  
- 
- import org.junit.Test;  
- import org.junit.runner.RunWith;  
- import org.slf4j.Logger;  
- import org.slf4j.LoggerFactory;  
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.test.context.ContextConfiguration;  
- import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;  
- 
- import com.tx.entity.Member;  
- import com.tx.entity.MemberInfo;  
- import com.tx.sevice.MemberService;  
- 
- @RunWith(SpringJUnit4ClassRunner.class)  
- @ContextConfiguration(locations = {"classpath:spring-context.xml","classpath:datasource-context.xml",  
- "classpath:mybatis-context.xml","classpath:mapper-context.xml","classpath:transaction-context.xml"})  
- publicclass JTATest {  
- 
- //log
- privatestaticfinal Logger LOG = LoggerFactory.getLogger(JTATest.class);  
- 
- @Autowired
- private MemberService memberService;  
- 
- @Test
- publicvoid testRegister(){  
-         Member member = new Member();  
-         member.setId(2);  
-         member.setUsername("童可可");  
-         member.setPassword("12345678");  
-         member.setStatus((byte)0);  
-         MemberInfo memberInfo = new MemberInfo();  
-         memberInfo.setId(2);  
-         memberInfo.setAge((byte)25);  
-         memberInfo.setNickname("keke");  
-         memberInfo.setRealname("童可可");  
- if(memberService.registerMember(member, memberInfo)){  
-             LOG.info("##用户注册成功");  
-         }else{  
-             LOG.info("##用户注册失败");  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package com.tx.test;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import com.tx.entity.Member;
import com.tx.entity.MemberInfo;
import com.tx.sevice.MemberService;
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations = {"classpath:spring-context.xml","classpath:datasource-context.xml",
		"classpath:mybatis-context.xml","classpath:mapper-context.xml","classpath:transaction-context.xml"})
public class JTATest {
	
	//log
	private static final Logger LOG = LoggerFactory.getLogger(JTATest.class);
	
	@Autowired
	private MemberService memberService;
	
	@Test
	public void testRegister(){
		Member member = new Member();
		member.setId(2);
		member.setUsername("童可可");
		member.setPassword("12345678");
		member.setStatus((byte)0);
		MemberInfo memberInfo = new MemberInfo();
		memberInfo.setId(2);
		memberInfo.setAge((byte)25);
		memberInfo.setNickname("keke");
		memberInfo.setRealname("童可可");
		if(memberService.registerMember(member, memberInfo)){
			LOG.info("##用户注册成功");
		}else{
			LOG.info("##用户注册失败");
		}
	}
}
```
注：通过主键重复可以测试回滚，数据没问题，正常提交不同数据库！log4j和web.xml和sql,其他代码：[Spring实现数据库读写分离/spring事务配置解释](http://blog.csdn.net/typa01_kk/article/details/51123706)
![](https://img-blog.csdn.net/20160419145334776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
效果图：
执行前，master数据库：
![](https://img-blog.csdn.net/20160419145421606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
执行前，slave数据库：
![](https://img-blog.csdn.net/20160419145430151?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
执行后，master数据库：
![](https://img-blog.csdn.net/20160419145437731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
执行后，slave数据库：
![](https://img-blog.csdn.net/20160419145446323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
spring-tx-2.5.6.jar和spring-tx-3.0.0.RELEASE.jar目录：
![](https://img-blog.csdn.net/20160419145528745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160419145537089?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
jta.properties启动参数：
**[html]**[view plain](http://blog.csdn.net/typa01_kk/article/details/51190147#)[copy](http://blog.csdn.net/typa01_kk/article/details/51190147#)[print](http://blog.csdn.net/typa01_kk/article/details/51190147#)[?](http://blog.csdn.net/typa01_kk/article/details/51190147#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1652806/fork)
- # SAMPLE PROPERTIES FILE FOR THE TRANSACTION SERVICE  
- # THIS FILE ILLUSTRATES THE DIFFERENT SETTINGS FOR THE TRANSACTION MANAGER  
- # UNCOMMENT THE ASSIGNMENTS TO OVERRIDE DEFAULT VALUES;  
- 
- # Required: factory implementation class of the transaction core.  
- # NOTE: there is no default for this, so it MUST be specified!   
- #   
- com.atomikos.icatch.service=com.atomikos.icatch.standalone.UserTransactionServiceFactory  
- 
- 
- # Set base name of file where messages are output   
- # (also known as the 'console file').  
- #  
- com.atomikos.icatch.console_file_name = tm.out  
- 
- # Size limit (in bytes) for the console file;  
- # negative means unlimited.  
- #  
- # com.atomikos.icatch.console_file_limit=-1  
- 
- # For size-limited console files, this option  
- # specifies a number of rotating files to   
- # maintain.  
- #  
- # com.atomikos.icatch.console_file_count=1
- 
- # Set the number of log writes between checkpoints  
- #  
- # com.atomikos.icatch.checkpoint_interval=500
- 
- # Set output directory where console file and other files are to be put  
- # make sure this directory exists!  
- #  
- # com.atomikos.icatch.output_dir = ./  
- 
- # Set directory of log files; make sure this directory exists!  
- #  
- # com.atomikos.icatch.log_base_dir = ./  
- 
- # Set base name of log file  
- # this name will be  used as the first part of   
- # the system-generated log file name  
- #  
- com.atomikos.icatch.log_base_name = tmlog
- 
- # Set the max number of active local transactions   
- # or -1 for unlimited.  
- #  
- # com.atomikos.icatch.max_actives = 50
- 
- # Set the default timeout (in milliseconds) for local transactions  
- #  
- # com.atomikos.icatch.default_jta_timeout = 10000
- 
- # Set the max timeout (in milliseconds) for local transactions  
- #  
- # com.atomikos.icatch.max_timeout = 300000
- 
- # The globally unique name of this transaction manager process  
- # override this value with a globally unique name  
- #  
- com.atomikos.icatch.tm_unique_name = tm
- 
- # Do we want to use parallel subtransactions? JTA's default  
- # is NO for J2EE compatibility  
- #  
- # com.atomikos.icatch.serial_jta_transactions=true
- 
- # If you want to do explicit resource registration then  
- # you need to set this value to false.  
- #  
- # com.atomikos.icatch.automatic_resource_registration=true
- 
- # Set this to WARN, INFO or DEBUG to control the granularity  
- # of output to the console file.  
- #  
- com.atomikos.icatch.console_log_level=INFO
- 
- # Do you want transaction logging to be enabled or not?  
- # If set to false, then no logging overhead will be done  
- # at the risk of losing data after restart or crash.  
- #  
- # com.atomikos.icatch.enable_logging=true
- 
- # Should two-phase commit be done in (multi-)threaded mode or not?  
- # Set this to false if you want commits to be ordered according  
- # to the order in which resources are added to the transaction.  
- #  
- # NOTE: threads are reused on JDK 1.5 or higher.   
- # For JDK 1.4, thread reuse is enabled as soon as the   
- # concurrent backport is in the classpath - see   
- # http://mirrors.ibiblio.org/pub/mirrors/maven2/backport-util-concurrent/backport-util-concurrent/  
- #  
- # com.atomikos.icatch.threaded_2pc=false
- 
- # Should shutdown of the VM trigger shutdown of the transaction core too?  
- #  
- # com.atomikos.icatch.force_shutdown_on_vm_exit=false
![](http://static.blog.csdn.net/images/save_snippets.png)
```
# SAMPLE PROPERTIES FILE FOR THE TRANSACTION SERVICE
# THIS FILE ILLUSTRATES THE DIFFERENT SETTINGS FOR THE TRANSACTION MANAGER
# UNCOMMENT THE ASSIGNMENTS TO OVERRIDE DEFAULT VALUES;
# Required: factory implementation class of the transaction core.
# NOTE: there is no default for this, so it MUST be specified! 
# 
com.atomikos.icatch.service=com.atomikos.icatch.standalone.UserTransactionServiceFactory
        
# Set base name of file where messages are output 
# (also known as the 'console file').
#
com.atomikos.icatch.console_file_name = tm.out
# Size limit (in bytes) for the console file;
# negative means unlimited.
#
# com.atomikos.icatch.console_file_limit=-1
# For size-limited console files, this option
# specifies a number of rotating files to 
# maintain.
#
# com.atomikos.icatch.console_file_count=1
# Set the number of log writes between checkpoints
#
# com.atomikos.icatch.checkpoint_interval=500
# Set output directory where console file and other files are to be put
# make sure this directory exists!
#
# com.atomikos.icatch.output_dir = ./
# Set directory of log files; make sure this directory exists!
#
# com.atomikos.icatch.log_base_dir = ./
# Set base name of log file
# this name will be  used as the first part of 
# the system-generated log file name
#
com.atomikos.icatch.log_base_name = tmlog
# Set the max number of active local transactions 
# or -1 for unlimited.
#
# com.atomikos.icatch.max_actives = 50
# Set the default timeout (in milliseconds) for local transactions
#
# com.atomikos.icatch.default_jta_timeout = 10000
# Set the max timeout (in milliseconds) for local transactions
#
# com.atomikos.icatch.max_timeout = 300000
# The globally unique name of this transaction manager process
# override this value with a globally unique name
#
com.atomikos.icatch.tm_unique_name = tm
    
# Do we want to use parallel subtransactions? JTA's default
# is NO for J2EE compatibility
#
# com.atomikos.icatch.serial_jta_transactions=true
                    
# If you want to do explicit resource registration then
# you need to set this value to false.
#
# com.atomikos.icatch.automatic_resource_registration=true  
    
# Set this to WARN, INFO or DEBUG to control the granularity
# of output to the console file.
#
com.atomikos.icatch.console_log_level=INFO
    
# Do you want transaction logging to be enabled or not?
# If set to false, then no logging overhead will be done
# at the risk of losing data after restart or crash.
#
# com.atomikos.icatch.enable_logging=true
# Should two-phase commit be done in (multi-)threaded mode or not?
# Set this to false if you want commits to be ordered according
# to the order in which resources are added to the transaction.
#
# NOTE: threads are reused on JDK 1.5 or higher. 
# For JDK 1.4, thread reuse is enabled as soon as the 
# concurrent backport is in the classpath - see 
# http://mirrors.ibiblio.org/pub/mirrors/maven2/backport-util-concurrent/backport-util-concurrent/
#
# com.atomikos.icatch.threaded_2pc=false
# Should shutdown of the VM trigger shutdown of the transaction core too?
#
# com.atomikos.icatch.force_shutdown_on_vm_exit=false
```
[分布式事务操作之Spring+JTA可参照](http://www.cnblogs.com/wangyong/p/4174326.html): http://www.cnblogs.com/wangyong/p/4174326.html
[Atomikos 中文说明文档](http://blog.csdn.net/sun8288/article/details/8674016)：http://blog.csdn.net/sun8288/article/details/8674016
源代码：[Spring多数据源分布式事务管理](http://download.csdn.net/detail/typa01_kk/9495830)
参考该文本人也写了一个demo有需要的可以下载：[https://github.com/zmx729618/jta/](https://github.com/zmx729618/jta/)


