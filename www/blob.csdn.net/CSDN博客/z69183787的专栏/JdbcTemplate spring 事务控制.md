# JdbcTemplate  spring 事务控制 - z69183787的专栏 - CSDN博客
2013年12月11日 19:02:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11055
编程式事务：
```java
public static PlatformTransactionManager getTransactionManager() {
		return transactionManager;
	}
	@Resource(name="dsTransactionManager")
	public void setTransactionManager(PlatformTransactionManager transactionManager) {
		ContractManageServiceImpl.transactionManager = transactionManager;
	}
public boolean save(String reason,String attach,String operator,String mainId,String process,String incident){
		HtxxStop bo = new HtxxStop();
		boolean flag = false;
		try{
			final CMInstance instance = new CMInstance(reason,attach,operator,mainId,process,incident,bo.getOperateTime(),bo.getRemoved());
			
			TransactionTemplate tt = new TransactionTemplate(transactionManager);
			flag = tt.execute(new TransactionCallback<Boolean>() {
				boolean flag = true;
				public Boolean doInTransaction(TransactionStatus status) {
					try{
						JdbcTemplate jt = dbUtil.getJdbcTemplate();
						instance.action(jt);
					}catch(Exception e){
						e.printStackTrace();
						status.setRollbackOnly();
						flag = false;
					}
					return flag;
				}
			});  
		}catch(Exception e){
			
		}
		return flag;
	}
```
```java
/** 
 */
package com.wonders.stpt.contractManage.instance;
import org.springframework.jdbc.core.JdbcTemplate;
/** 
 * @ClassName: CMInstance 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013-6-13 上午11:03:13 
 *  
 */
public class CMInstance {
	private String reason, attach, operator, mainId, process, incident,operateTime,removed;
	private static final String insertSql = "insert into ht_xx_stop t " +
			"(t.id,t.reason,t.attach,t.operator,t.operate_time,t.main_id,t.removed)values(sys_guid(),?,?,?,?,?,?)";
	private static final String updateSql1 = "update ht_xx h set  h.contract_name =  h.contract_name || '<font color=''red''>（此合同异常终止）</font>' , h.flag='99',h.removed=1 where h.id = ?";
	private static final String updateSql2 = "update incidents i set i.processname=? ,i.status = '33' where i.processname = ?  and i.incident = ?";
	
	public CMInstance(String reason,String attach,String operator,String mainId,String process,String incident,String operateTime,String removed){
		this.reason = reason;
		this.attach = attach;
		this.operator = operator;
		this.mainId = mainId;
		this.process = process;
		this.incident = incident;
		this.operateTime = operateTime;
		this.removed = removed;
	}
	
	public void action(JdbcTemplate jt) throws Exception{
		int n0 = jt.update(insertSql,new Object[]{reason,attach,operator,operateTime,mainId,removed});
		int n1 = jt.update(updateSql1,new Object[]{mainId});
		int n2 = jt.update(updateSql2,new Object[]{process+"(删除)",process,incident});
		if(n0 > 0 && n1 > 0 && n2>0){
		}else{
			throw new Exception("update failed");
		}
	}
}
```
声明式事务
```java
@Transactional(value = "stfb-txManager",propagation=Propagation.REQUIRED,rollbackFor=Exception.class)
@Service("coccMain")
```
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns:context="http://www.springframework.org/schema/context" 
	xmlns:aop="http://www.springframework.org/schema/aop"
	xmlns:tx="http://www.springframework.org/schema/tx"
	xsi:schemaLocation="
	http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
	http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd
	http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.0.xsd
	http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.0.xsd">
	
	<description>
		Schedule Application
	</description>
	<!-- =================================================================== -->
	<!-- Context Define 123                                                     -->
	<!-- =================================================================== -->
	<bean id="propertyConfigurer"
		class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
		<property name="locations">
			<list>
				<value>classpath:database.properties</value>
			</list>
		</property>
	</bean>
		
	<context:component-scan base-package="com.wonders" />
	
	<!-- =================================================================== -->
	<!-- Data Source Define (jdbc & jndi)                                    -->
	<!-- =================================================================== -->
	<!-- JDBC Configuration  -->
	
	<bean id="stfb"
		class="org.springframework.jdbc.datasource.DriverManagerDataSource">
		<property name="driverClassName"
			value="${hibernate.connection.driver_class.stfb}" />
		<property name="url" value="${hibernate.connection.url.stfb}" />
		<property name="username"
			value="${hibernate.connection.username.stfb}" />
		<property name="password"
			value="${hibernate.connection.password.stfb}" />
	</bean>
	
	<bean id="stptinc"
		class="org.springframework.jdbc.datasource.DriverManagerDataSource">
		<property name="driverClassName"
			value="${hibernate.connection.driver_class}" />
		<property name="url" value="${hibernate.connection.url}" />
		<property name="username"
			value="${hibernate.connection.username}" />
		<property name="password"
			value="${hibernate.connection.password}" />
	</bean>
	
	<!-- <bean id="stptdemo"
		class="org.springframework.jdbc.datasource.DriverManagerDataSource">
		<property name="driverClassName"
			value="${hibernate.connection.driver_class.new}" />
		<property name="url" value="${hibernate.connection.url.new}" />
		<property name="username"
			value="${hibernate.connection.username.new}" />
		<property name="password"
			value="${hibernate.connection.password.new}" />
	</bean>-->
	
	<!--<bean id="stptdemo" class="org.apache.commons.dbcp.BasicDataSource"> 
              <property name="driverClassName"> 
                     <value>${hibernate.connection.driver_class.new}</value> 
              </property> 
              <property name="url"> 
                     <value>${hibernate.connection.url.new}</value> 
              </property> 
              <property name="username"> 
                     <value>${hibernate.connection.username.new}</value> 
              </property> 
              <property name="password"> 
                     <value>${hibernate.connection.password.new}</value> 
              </property> 
              <property name="maxActive"> 
                     <value>255</value> 
              </property> 
              <property name="maxIdle"> 
                     <value>2</value> 
              </property> 
              <property name="maxWait"> 
                     <value>120000</value> 
              </property> 
       </bean> -->
       
	<bean id="stptdemo" class="com.mchange.v2.c3p0.ComboPooledDataSource"       
        destroy-method="close">      
    	<property name="driverClass" value="${hibernate.connection.driver_class.new}"/>      
    	<property name="jdbcUrl" value="${hibernate.connection.url.new}"/>      
    	<property name="user" value="${hibernate.connection.username.new}"/>      
    	<property name="password" value="${hibernate.connection.password.new}"/>   
    	<!--此处继续增加c3p0属性-->
		<!--当连接池中的连接耗尽的时候c3p0一次同时获取的连接数。-->
		<property name="acquireIncrement">
			<value>${jdbc.pool.c3p0.acquire_increment}</value>
		</property>
		<!--连接池中保留的最大连接数。-->
		<property name="maxPoolSize" value="${jdbc.pool.c3p0.max_size}" />
		<!--连接池中保留的最小连接数。-->
		<property name="minPoolSize" value="${jdbc.pool.c3p0.min_size}" />
		<!--最大空闲时间,60秒内未使用则连接被丢弃。若为0则永不丢弃。Default: 0 -->
		<property name="maxIdleTime"
			value="${jdbc.pool.c3p0.max_idle_time}" />
		<!--定义所有连接测试都执行的测试语句。在使用连接测试的情况下这个一显著提高测试速度。注意：
			测试的表必须在初始数据源的时候就存在。Default: null-->
		<property name="preferredTestQuery"
			value="${jdbc.pool.c3p0.preferred_test_query}" />
		<!--每60秒检查所有连接池中的空闲连接。Default: 0 -->
		<property name="idleConnectionTestPeriod"
			value="${jdbc.pool.c3p0.idle_connection_test_period}" />
		<!--定义在从数据库获取新连接失败后重复尝试的次数。Default: 30 
			<property name="acquireRetryAttempts">30</property>-->
		<!--两次连接中间隔时间，单位毫秒。Default: 1000 
			<property name="acquireRetryDelay">1000</property>-->
		<!--连接关闭时默认将所有未提交的操作回滚。Default: false 
			<property name="autoCommitOnClose">false</property> -->   
	</bean> 
		<!-- JNDI Configuration 
			<bean id="stptinc" class="org.springframework.jndi.JndiObjectFactoryBean">
			<property name="jndiName" value="jdbc/stptinc"/>
			</bean>
			
			<bean id="stptdemo" class="org.springframework.jndi.JndiObjectFactoryBean">
			<property name="jndiName" value="jdbc/stptdemo"/>
			</bean>
		-->
		<!-- JNDI Configuration tomcat 
	
		<bean id="stptinc" class="org.springframework.jndi.JndiObjectFactoryBean">
		<property name="jndiName" value="java:comp/env/jdbc/stptinc"/>
		</bean>
		
		<bean id="stptdemo" class="org.springframework.jndi.JndiObjectFactoryBean">
		<property name="jndiName" value="java:comp/env/jdbc/stptdemo"/>
		</bean>
-->
	<!-- =================================================================== -->
	<!-- SessionFactory(For Hibernate)/ManagerFactory(For JPA) Define        -->
	<!--  ONLY ONE OF THEM SHOULD BE USED                                    -->
	<!-- =================================================================== -->
	<!--  class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean" spring 2.5之后继承的工厂类 -->
	
	<bean id="sessionFactory"
		class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean">
		<property name="dataSource" ref="stptdemo" />
		<property name="configLocation"
			value="classpath:hibernate.cfg.xml" />
			<property name="packagesToScan"><!-- 自动扫描注解 -->
		    <list>
		        <value>com.wonders.*</value>
		    </list>
			</property>
		<property name="configurationClass"  value="org.hibernate.cfg.AnnotationConfiguration"></property>
		<property name="lobHandler" ref="${jdbc.handler}" />
	</bean>
	
	<bean id="sessionFactory2"
		class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean">
		<property name="dataSource" ref="stptinc" />
		<property name="configLocation"
			value="classpath:hibernate.cfg.xml" />
			<property name="packagesToScan"><!-- 自动扫描注解 -->
		    <list>
		        <value>com.wonders.*</value>
		    </list>
			</property>
		<property name="configurationClass"  value="org.hibernate.cfg.AnnotationConfiguration"></property>
		<property name="lobHandler" ref="${jdbc.handler}" />
	</bean>
	
	<bean id="hibernateTemplate" class="org.springframework.orm.hibernate3.HibernateTemplate">
		<property name="sessionFactory" ref="sessionFactory"></property>
	</bean>
	
	<bean id="hibernateTemplate2" class="org.springframework.orm.hibernate3.HibernateTemplate">
		<property name="sessionFactory" ref="sessionFactory2"></property>
	</bean>
	
	<!--<bean id = "jdbcTemplate" class = "org.springframework.jdbc.core.JdbcTemplate">
	    <property name = "dataSource" ref="dataSource"/>
	</bean>
	
	<bean id = "jdbcTemplate2" class = "org.springframework.jdbc.core.JdbcTemplate">
	    <property name = "dataSource" ref="dataSource2"/>
	</bean>
	    
	-->
	<bean id = "stfb_jdbcTemplate" class = "org.springframework.jdbc.core.JdbcTemplate">
	    <property name = "dataSource" ref="stfb"/>
	</bean>
	
	<bean id="oracleLobHandler" class="org.springframework.jdbc.support.lob.OracleLobHandler">
		<property name="nativeJdbcExtractor" ref="nativeJdbcExtractor" />
	</bean>
	<bean id="defaultLobHandler" class="org.springframework.jdbc.support.lob.DefaultLobHandler">
	</bean>
	<bean id="nativeJdbcExtractor" class="org.springframework.jdbc.support.nativejdbc.SimpleNativeJdbcExtractor" lazy-init="true" />
	
	
	<bean id="txManager" class="org.springframework.orm.hibernate3.HibernateTransactionManager">
       	<property name="sessionFactory" ref="sessionFactory"/>
	</bean>
	
	<bean id="txManager2" class="org.springframework.orm.hibernate3.HibernateTransactionManager">
       	<property name="sessionFactory" ref="sessionFactory2"/>
	</bean>
	
	<bean id="dsTransactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource" ref="stptdemo" />
		<qualifier value="stptdemo"/>  <!-- “限定符” -->
	</bean>
	
	<bean id="dsTransactionManager2" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource" ref="stptinc" />
		<qualifier value="stptinc"/>   
	</bean>
	
	<bean id="stfb-txManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
      <property name="dataSource" ref="stfb"/>
  </bean>
	
	<tx:annotation-driven transaction-manager="txManager" proxy-target-class="true"/>
	
	<tx:annotation-driven transaction-manager="txManager2" proxy-target-class="true"/>
	
	<tx:annotation-driven transaction-manager="stfb-txManager" proxy-target-class="true"/>
	
	<!-- 
		<aop:aspectj-autoproxy />有一个proxy-target-class属性，默认为false，
		表示使用JDK动态代理织入增强，当配置 为<aop:aspectj-autoproxy proxy-target-class="true" />时，
		表示使用CGLib动态代理技术织入增强。不过即使proxy-target-class设置为false，
		如果目标类没有声明接口，则 Spring将自动使用CGLib动态代理。
	 -->
	<!-- jdk 动态代理  必须有接口实现-->
	<aop:aspectj-autoproxy/>
	<!-- 强制使用cglib 代理  针对类-->
	<!-- <aop:aspectj-autoproxy proxy-target-class="true" />-->
</beans>
```
