# Java 学习笔记07：使用Spring的Dao连接数据库 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月19日 16:44:33[initphp](https://me.csdn.net/initphp)阅读数：4035
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- 关于DAO

DAO组件是整个java应用的吃就成访问的重要组件，Dao模式的主要内容为：应用中所有对数据库的访问都通过Dao组件完成，Dao组件封装了对数据库的增、删、改、查等操作。
- 具体实现
- 创建一个新的项目地址，名称为Spring
- 既然是数据库操作，肯定要有数据库配置，创建一个jdbc.properties的文件，放置在src/目录下，里面包含数据库连接的重要信息。


```
jdbc.driverClassName=com.mysql.jdbc.Driver
jdbc.url = jdbc:mysql://127.0.0.1:3306/test
jdbc.username = root
jdbc.password = root
```
![](https://img-my.csdn.net/uploads/201211/19/1353313436_8529.jpg)


- 创建一个bean.xml

```
<?xml version="1.0" encoding="UTF-8"?>
<beans  
    xmlns="http://www.springframework.org/schema/beans"  
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
    xmlns:context="http://www.springframework.org/schema/context"
    xmlns:aop="http://www.springframework.org/schema/aop"
    xmlns:tx="http://www.springframework.org/schema/tx" 
    xsi:schemaLocation="http://www.springframework.org/schema/beans 
    http://www.springframework.org/schema/beans/spring-beans-2.0.xsd
    http://www.springframework.org/schema/context
    http://www.springframework.org/schema/context/spring-context-2.5.xsd
    http://www.springframework.org/schema/aop
    http://www.springframework.org/schema/aop/spring-aop-2.5.xsd
    http://www.springframework.org/schema/tx
    http://www.springframework.org/schema/tx/spring-tx-2.5.xsd
    ">  
    <context:annotation-config/>
    <aop:aspectj-autoproxy/>
    
    <bean id="jdbc" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer" >
    	<property name="locations" value="jdbc.properties"/>    
    </bean>
    <bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource"  destroy-method="close">        
     <property name="driverClassName" value="${jdbc.driverClassName}" />       
     <property name="url" value="${jdbc.url}" />       
     <property name="username" value="${jdbc.username}" />       
     <property name="password" value="${jdbc.password}" />       
	</bean> 
	<bean id="txManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="dataSource"/>
    </bean>
	<tx:annotation-driven transaction-manager="transactionManager" proxy-target-class="true" />
	
	<bean id="UserImpl" class="com.spring.test.dao.UserImpl" ></bean>
	<bean id="User" class="com.spring.test.service.User" ></bean>
	<bean id="UserService" class="com.spring.test.service.UserService" ></bean>
</beans>
```


这个xml中需要在beans头部添加：http://www.springframework.org/schema/tx 地址以及，http://www.springframework.org/schema/tx

    http://www.springframework.org/schema/tx/spring-tx-2.5.xsd。

```
<bean id="jdbc" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer" >
    	<property name="locations" value="jdbc.properties"/>    
    </bean>
    <bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource"  destroy-method="close">        
     <property name="driverClassName" value="${jdbc.driverClassName}" />       
     <property name="url" value="${jdbc.url}" />       
     <property name="username" value="${jdbc.username}" />       
     <property name="password" value="${jdbc.password}" />       
	</bean>
```
上面的xml内容为调用jdbc.properties文件，然后放进xml bean的dataSource对象中。


```
<bean id="txManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="dataSource"/>
    </bean>
	<tx:annotation-driven transaction-manager="transactionManager" proxy-target-class="true" />
```
这段xml中是配置了事务管理器。


- 创建数据操作层，创建包com.spring.test.dao，然后创建UserDao.java（接口文件，定义需要实现的数据操作方法）和UserImpl（具体实现）。

```java
package com.spring.test.dao;

import com.spring.test.service.User;

public interface UserDao {
	public int userSave(User user);
}
```


```java
package com.spring.test.dao;

import javax.annotation.Resource;
import javax.sql.DataSource;

import org.springframework.jdbc.core.JdbcTemplate;

import com.spring.test.service.User;


public class UserImpl implements UserDao {
	
	private JdbcTemplate jdbcTemplate;
	
	//数据库实例化，通过注解@Resource 将dataSource 实例化
	@Resource(name="dataSource")
	public void setDataSource(DataSource dataSource) {
		this.jdbcTemplate = new JdbcTemplate(dataSource);
	}
	
	/**
	 * 保存用户数据，返回int类型，表示数据库中的总记录数
	 */
	public int userSave(User user) {
		jdbcTemplate.update(
			"INSERT INTO user (username, password) values (?, ?)",
			new Object[] {user.getUsername(), user.getPassword()}
		);
		int count = jdbcTemplate.queryForInt("SELECT COUNT(*) FROM USER");  
		return count;
	}
}
```


- 创建服务层，创建包com.spring.test.service，然后创建User.java（数据结构）和UserService （用户服务类）

```java
package com.spring.test.service;

public class User {
	private String username;
	
	private String password;
	
	public void setUsername(String username) {
		this.username = username;
	}
	
	public String getUsername() {
		return this.username;
	}
	
	public void setPassword(String password) {
		this.password = password;
	}
	
	public String getPassword() {
		return this.password;
	}
}
```


```java
package com.spring.test.service;

import org.springframework.beans.factory.annotation.Autowired;

import com.spring.test.dao.UserImpl;

public class UserService {
	
	//通过注解的方式，获取UserImpl对象
	@Autowired
	private UserImpl UserImpl;
	
	public int saveUserInfo(User User) {
		return UserImpl.userSave(User);
	}
}
```


- 创建入口执行文件



```java
package com.spring.test.main;

import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;

import com.spring.test.service.User;
import com.spring.test.service.UserService;


public class Main {
	
	/**
	 * 入口文件
	 * @param args
	 */
	public static void main(String args[]) {
		ApplicationContext Factory = null;
		Factory = new ClassPathXmlApplicationContext("bean.xml");
		
		User User = (User)Factory.getBean("User");
		User.setUsername("zhuli");
		User.setPassword("saodosadoas");
		
		UserService UserService = (UserService)Factory.getBean("UserService");
		int i = UserService.saveUserInfo(User);
		System.out.print(i);
	}
}
```


- 运行，返回结果：
`8`









