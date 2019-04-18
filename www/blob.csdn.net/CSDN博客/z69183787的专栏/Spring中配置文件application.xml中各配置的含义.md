# Spring中配置文件application.xml中各配置的含义 - z69183787的专栏 - CSDN博客
2012年10月22日 11:18:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：18044
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns:context="http://www.springframework.org/schema/context" 
	xmlns:aop="http://www.springframework.org/schema/aop"
	xmlns:tx="http://www.springframework.org/schema/tx"
	xsi:schemaLocation="
	http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.0.xsd
	http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.0.xsd
	http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd
	http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-2.5.xsd">
	
	<description>
		Spring Configuration.
	</description>
	<import resource="classpath:frameworkContext.xml" /> <!--引入其他context文件-->
	
	<!-- =================================================================== -->
	<!-- Context Define                                                      -->
	<!-- =================================================================== -->
        <!--预加载配置文件 之后可通过${XXX}读取-->
	<bean id="propertyConfigurer"
		class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
		<property name="locations">
			<list>
				<value>classpath:database.properties</value>
			</list>
		</property>
	</bean>
	
	
	<!-- =================================================================== -->
	<!-- Data Source Define (jdbc & jndi)                                    -->
	<!-- =================================================================== -->
	<!-- JDBC Configuration  通过配置文件 配置直连 数据源 -->
	<!--
	<bean id="dataSource"
		class="org.springframework.jdbc.datasource.DriverManagerDataSource">
		<property name="driverClassName"
			value="${hibernate.connection.driver_class}" />
		<property name="url" value="${hibernate.connection.url}" />
		<property name="username"
			value="${hibernate.connection.username}" />
		<property name="password"
			value="${hibernate.connection.password}" />
	</bean>
	 -->
	 
	<!-- JNDI Configuration -->
	
	<!--通过jndi配置数据源-->
	<bean id="dataSource" class="org.springframework.jndi.JndiObjectFactoryBean">
		<property name="jndiName" value="jdbc/stptdemo"/>
              <!--  <property name="jndiName" value="java:comp/env/jdbc/stpt"/>  tomcat数据源-->
	</bean>
	
	<!-- =================================================================== -->
	<!-- SessionFactory(For Hibernate)/ManagerFactory(For JPA) Define        -->
	<!--  ONLY ONE OF THEM SHOULD BE USED                                    -->
	<!-- =================================================================== -->
	<!--  class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean" 
	spring 2.5之后继承的工厂类 通过jpa注释 配置BO-->
	<!--  class="org.springframework.orm.hibernate3.LocalSessionFactoryBean" 
	spring 2.5之前继承的工厂类 通过hbm配置文件 配置BO -->
	
	<!--配置sessionfactory-->
	<bean id="sessionFactory"
		class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean">
		<property name="dataSource" ref="dataSource" />
		<property name="configLocation"
			value="classpath:hibernate.cfg.xml" />
                <!--此属性可写在此，也可写在hibernate配置文件 cfg.xml 中-->
		<property name="hibernateProperties">
			<props>
				<prop key="hibernate.dialect">
					${hibernate.dialect}
				</prop>
			</props>
		</property>
		<!--若是注解bo映射，则写在hibernate.cfg.xml中 或者 使用属性annotatedClasses-->
                 <!--
                 <property name="annotatedClasses"> 
                 <list> 
                   <value>com.pojo.DT_RGNCD</value> 
                   </list>     
                   </property>    
                    但这种方式只能一个个吧bo写出来比较繁琐
                     -->
                <!--使用packagesToScan属性扫描包下的实体类，可以避免1个个写了
                  <property name="packagesToScan"><!-- 自动扫描注解 -->
                  <list>
                      <value>com.wonders.*</value>
                  </list>
                 -->
		<!--需要hbm配置文件查找映射路径 配置
		<property name="mappingDirectoryLocations">
		<list>
		<value>classpath:/com/wondersgroup</value>
		</list>
		</property>
		-->
 		<!--处理lob类型-->
 		<property name="lobHandler" ref="${jdbc.handler}" />
 	</bean>
 	
 	<!--配置hibernate操作类-->
	<bean id="hibernateTemplate" class="org.springframework.orm.hibernate3.HibernateTemplate">
		<property name="sessionFactory" ref="sessionFactory"></property>
	</bean>
	<!--配置jdbc操作类-->
 	<bean id = "jdbcTemplate" class = "org.springframework.jdbc.core.JdbcTemplate"> 
 		<property name = "dataSource" ref="dataSource"/>
 	</bean> 
	<!-- =================================================================== -->
	<!-- Transaction Define                                                  -->
	<!-- =================================================================== -->
	<!--配置hibernate事务管理类 -->
	<bean id="txManager" class="org.springframework.orm.hibernate3.HibernateTransactionManager">
        	<property name="sessionFactory" ref="sessionFactory"/>
	</bean>
	<!--配置jdbc事务管理类 -->
	<bean id="dsTransactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource" ref="dataSource" />
	</bean>
	<!--引入注解配置 设置扫描类 包含struts2 spring -->	
		<context:annotation-config/>
		<context:component-scan base-package="com.wonders" /><!--包括<context:annotation-config/>，可代替-->
	<!--注解配置 指定事务代理类-->
		<tx:annotation-driven transaction-manager="txManager" proxy-target-class="true"/>
	<!--引入aop 自动代理-->
		<aop:aspectj-autoproxy/>
	
	<!-- =================================================================== -->
	<!-- Transaction proxy define                                           -->
	<!-- =================================================================== -->
	<!--事务代理类 ，用于xml配置 时 为代理service，增加事务管理；使用时作为service的基类进行事务管理-->
	<bean id="basicTxProxy" abstract="true"class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean">
		<property name="transactionManager" ref="transactionManager" />
		<property name="transactionAttributes">
		<!-- //ProxyConfig类的exposeProxy属性，表示是否要将当前的代理对象放入AopContext中，
		 //这样在你的Service中可以用AopContext.currentProxy()取得当前代理，相当于this引用， 
		 <property name="exposeProxy" value="true" /> -->
			<props>
				<prop key="save*">PROPAGATION_REQUIRED</prop> <!--如果当前没有事务，就新建一个事务，如果已经存在一个事务中，加入到这个事务中。这是最常见的选择-->
				<prop key="add*">PROPAGATION_REQUIRED</prop>
				<prop key="remove*">PROPAGATION_REQUIRED</prop>
				<prop key="update*">PROPAGATION_REQUIRED</prop>
				<prop key="*">PROPAGATION_REQUIRED</prop>
			</props>
		</property>
	</bean>
</beans>
```
同时，附上 hibernate.cfg.xml
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
    <!-- set hbm2ddl strategy -->  
    <!--
			<property name="hibernate.hbm2ddl.auto">update</property>
		-->  
    <!-- add BO class Mapping here optionally -->  
    <mapping class="com.wonders.stpt.metroExpress.entity.bo.MetroExpress"/>  
    <mapping class="com.wonders.stpt.metroIndicator.entity.bo.MetroProduction"/>  
    <mapping class="com.wonders.stpt.metroIndicator.entity.bo.MetroQuality"/>  
    <mapping class="com.wonders.stpt.metroIndicator.entity.bo.MetroScale"/>
    <mapping class="com.wonders.stpt.core.login.entity.bo.Tuser"/>  
    <mapping class="com.wonders.stpt.core.login.entity.bo.TuserToken"/> 
    <mapping class="com.wonders.stpt.core.login.entity.bo.TuserRelation"/>  
    <mapping class="com.wonders.stpt.core.domainAuthentication.entity.bo.DomainAuthentication"/>  
    <mapping class="com.wonders.stpt.metroLine.entity.bo.MetroLine"/>  
    <mapping class="com.wonders.stpt.constructionNotice.entity.bo.ConstructionNotice"/>  
    <mapping class="com.wonders.stpt.organTree.entity.bo.TorganRelation"/>  
    <mapping class="com.wonders.stpt.indicatorControl.entity.bo.MetroProductionControl"/>
    <mapping class="com.wonders.stpt.indicatorControl.entity.bo.MetroQualityControl"/>
    <mapping class="com.wonders.stpt.indicatorControl.entity.bo.MetroScaleControl"/>
    
 
    	<mapping class="com.wonders.stpt.core.userManage.entity.bo.StptUser"/>  
    	<mapping class="com.wonders.stpt.core.cfconsole.entity.bo.TuserLog"/>
    	<mapping class="com.wonders.stpt.userMsg.entity.bo.TuserMsg"/>
    	<mapping class="com.wonders.stpt.attach.model.bo.AttachFile"/>
  
  </session-factory> 
</hibernate-configuration>
```
及database.properties
```
jdbc.handler oracleLobHandler
hibernate.dialect org.hibernate.dialect.Oracle9Dialect
hibernate.connection.driver_class oracle.jdbc.OracleDriver
hibernate.connection.username stpt
hibernate.connection.password shstpt
hibernate.connection.url jdbc:oracle:thin:@10.1.13.10:1521:stptdemo
```
