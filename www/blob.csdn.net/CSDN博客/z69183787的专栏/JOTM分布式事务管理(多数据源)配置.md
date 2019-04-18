# JOTM分布式事务管理(多数据源)配置 - z69183787的专栏 - CSDN博客
2014年04月02日 10:43:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：947
项目接近尾声，老大让我们几个研究一些新技术，以便为将来项目的扩展做打算，我被分配到JOTM这一块。说实话，自己对分布式事务也不了解，至于JTA，只是了解概念。
至于什么是JOTM、JOTM有什么用，大家可以访问官网[http://jotm.objectweb.org/index.html](http://jotm.objectweb.org/index.html)或者百度、google。趁着周末闲，于是搞了个DEMO。这个demo是用spring和jotm实现，数据库采用的oracle.本文只做简单的配置介绍，以及实现基本的同步回滚、提交效果。
首先，下载jar包，我的项目用的jar包如下：
![](https://img-my.csdn.net/uploads/201204/21/1335002184_9318.jpg)
基本上就是spring、JOTM以及oracle驱动jar包。
其次：引入carol.properties log4j.properties两个配置文件，这些文件可以再下载的JOTM的conf目录里找到。同时，新建一个spring的配置文件，用来初始化数据源之类的。
这是我的文件内容：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
     xmlns:aop="http://www.springframework.org/schema/aop"
     xmlns:tx="http://www.springframework.org/schema/tx"
     xsi:schemaLocation="
     http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
     http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.5.xsd
     http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd">
<bean id="jotm"	class="org.springframework.transaction.jta.JotmFactoryBean" />
<bean id="txManager" class="org.springframework.transaction.jta.JtaTransactionManager">
	<property name="userTransaction" ref="jotm" />
</bean>
<bean id="topicDS"
	class="org.enhydra.jdbc.pool.StandardXAPoolDataSource"
	destroy-method="shutdown">
	<property name="dataSource">
		<bean class="org.enhydra.jdbc.standard.StandardXADataSource"
			destroy-method="shutdown">
			<property name="transactionManager" ref="jotm" />
			<property name="driverName" value="oracle.jdbc.driver.OracleDriver" />
			<property name="url" value="jdbc:oracle:thin:@192.168.7.223:1521:ORCL" />
		</bean>
	</property>
	<property name="user" value="crmg1" />
	<property name="password" value="crmg" />
</bean>
<bean id="postDS" class="org.enhydra.jdbc.pool.StandardXAPoolDataSource"
	destroy-method="shutdown">
	<property name="dataSource">
		<bean class="org.enhydra.jdbc.standard.StandardXADataSource"
			destroy-method="shutdown">
			<property name="transactionManager" ref="jotm" />
			<property name="driverName" value="oracle.jdbc.driver.OracleDriver" />
			<property name="url" value="jdbc:oracle:thin:@192.168.7.223:1521:ORCL" />
		</bean>
	</property>
	<property name="user" value="crm20120224" />
	<property name="password" value="crm" />
</bean>
<bean id="topicTemplate"
	class="org.springframework.jdbc.core.JdbcTemplate">
	<property name="dataSource" ref="topicDS" />
</bean>
<bean id="postTemplate"
	class="org.springframework.jdbc.core.JdbcTemplate">
	<property name="dataSource" ref="postDS" />
</bean>
<bean id="topicDao" class="com.test.Dao1ServiceImpl">
	<property name="jdbcTemplate" ref="topicTemplate" />
	<property name="dao2Service" ref="postDao" />
</bean>
<bean id="postDao" class="com.test.Dao2ServiceImpl">
	<property name="jdbcTemplate" ref="postTemplate" />
</bean>
<tx:annotation-driven transaction-manager="txManager" />
</beans>
```
我配置了两个oracle的数据源。两个数据源都交给JOTM来管理。同时有两个service:Dao1ServiceImpl\Dao2ServiceImpl分别操作两个数据源的表。为这两个service添加事务
两个service实现如下：
```java
package com.test;
import org.springframework.jdbc.core.JdbcTemplate;
public class Dao1ServiceImpl implements IDao1Service {
	private JdbcTemplate jdbcTemplate;
	private IDao2Service dao2Service;
	
	public IDao2Service getDao2Service() {
		return dao2Service;
	}
	public void setDao2Service(IDao2Service dao2Service) {
		this.dao2Service = dao2Service;
	}
	public JdbcTemplate getJdbcTemplate() {
		return jdbcTemplate;
	}
	public void setJdbcTemplate(JdbcTemplate jdbcTemplate) {
		this.jdbcTemplate = jdbcTemplate;
	}
	
	public void add() {
		String str = "insert into t_staff(PK_ID,NAME,STAFF_TYPE,IS_DISABLE) values(2,'wyb','客服','0')";
		jdbcTemplate.update(str);
		int i=5/0;
		dao2Service.add();
		
	}
}
```
```java
package com.test;
import org.springframework.jdbc.core.JdbcTemplate;
public class Dao2ServiceImpl implements IDao2Service {
	private JdbcTemplate jdbcTemplate;
	
	public JdbcTemplate getJdbcTemplate() {
		return jdbcTemplate;
	}
	public void setJdbcTemplate(JdbcTemplate jdbcTemplate) {
		this.jdbcTemplate = jdbcTemplate;
	}
	public void add() {
		
		String str = "insert into t_user values(200,'wyb','123','0',null,null,null,null)";
		jdbcTemplate.update(str);
	}
}
```
把service1的add方法和service2的add方法在service1中调用service2的方法，同时让service1出现异常，这样在分布式事务的情况下会得到结果：两条数据都米有插入成功，同时回滚。
测试类：
```java
public class TestJOTM {
	public static void main(String[] args) {
		
		ApplicationContext ctx = new ClassPathXmlApplicationContext("applicationContext-jta.xml", TestJOTM.class);
		IDao1Service d1 = (IDao1Service)ctx.getBean("topicDao");
		try{
			d1.add();
		}catch (Exception e) {
			// TODO: handle exception
		}
		
	}	
}
```
记录下配置的步骤，给自己留个备份。
刚开始的时候是用MySQL进行测试的，一直不行。搜了很多，原来MySQL不支持事务管理，晕。
demo我已经上传到我的CSDN资源里面了，大家可以去下载（免下载积分）[http://download.csdn.net/detail/wangyinbin/4246381](http://download.csdn.net/detail/wangyinbin/4246381)。PS:CSDN下载专区，为啥刚刚上传的资源自己看不到咯？
