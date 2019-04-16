# 在Spring3 MVC中五步配置集成注解方式Hibernate3 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年11月14日 23:22:09[gloomyfish](https://me.csdn.net/jia20003)阅读数：3058








最近在搞一个WEB项目，以前在公司做项目用的都是JPA做ORM持久层，这次这个项目是我自己接的，我决定改一下，用Hibernate3来做ORM持久层、于是我网上搜索了Hibernate3怎么配置集成到Spring3 MVC上，发现千奇百怪，而且很多都是不是基于注解方式配置，显然那些文字上面的配置方式已经跟如今的Hibernate3注解支持方式脱节了，于是我决定自己搞一把，首先说一下网上那些配置方式的不好的地方，很多文章都提到要jdbc.properties文件与Hibernate config文件，其实是如今基于注解，完全没有必要。我自己总结一下，在已经存在的Spring3 MVC的项目中集成注解支持的Hibernate3，在做之前首先说一下我的整个配置环境：

**WEB服务器： tomcat7**

**开发环境： EclipseEE**

**Spring3版本：3.2.5 release**

**JDK版本: JDK8**

**数据库： MySQL 5.x**




然后执行如下五步即可：

**第一步：在Maven中添加包依赖支持**

在项目的pom.xml文件中添加Hibernate的包依赖，同时还要添加MySQL JDBC的包依赖与apache-common DBCP的包依赖，同时还要添加spring-orm与spring-context的包依赖，添加的pox.xml依赖如下：



```
<dependency>
	<groupId>org.hibernate</groupId>
	<artifactId>hibernate-core</artifactId>
	<version>3.6.10.Final</version>
</dependency>

<dependency>
	<groupId>mysql</groupId>
	<artifactId>mysql-connector-java</artifactId>
	<version>5.1.37</version>
</dependency>
<dependency>
	<groupId>commons-dbcp</groupId>
	<artifactId>commons-dbcp</artifactId>
	<version>1.4</version>
</dependency>
```
**spring-orm依赖添加：**


```
<dependency>
	<groupId>org.springframework</groupId>
	<artifactId>spring-orm</artifactId>
	<version>${spring.version}</version>
</dependency>
<dependency>
	<groupId>org.springframework</groupId>
	<artifactId>spring-context</artifactId>
	<version>${spring.version}</version>
</dependency>
```

**第二步：创建一个xml文件**

名字你自己随便取吧，但是文件必须位于WEB-INF目录下面。我的做法是在WEB-INF下面创建了一个子目录spring然后再创建了一个xml文件data-context.xml



**第三步：编辑web.xml文件**

在web.xml文件添加如下配置：



```
<context-param>
	<param-name>contextConfigLocation</param-name>
	<param-value>
		/WEB-INF/spring/data-context.xml
	</param-value>
</context-param>

<listener>
	<listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
</listener>
```

**第四步：在MySQL中创建应用数据库，**

在MySQL启动的情况下，通过HeidiSQL连接到MySQL之后创建一个新的数据库yourDB，同时在应用程序中创建一个package作为你的Entity类的包，创建一个测试Entity类你自己可以选择Class名称，**此步骤可以跳过**，如果你已经创建好了相应的数据库。



**第五步：添加Hibernate相关配置**

这里主要分为几个部分，一是支持注解配置、二是支持对Service与Dao层包的扫描、三是配置数据源dataSource、四是要配置连接管理的sessionFactory、五是配置支持事务管理，完整的配置文件如下：



```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	xmlns:aop="http://www.springframework.org/schema/aop"
	xmlns:context="http://www.springframework.org/schema/context"
	xmlns:tx="http://www.springframework.org/schema/tx"
	xsi:schemaLocation="
   http://www.springframework.org/schema/beans 
   http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
   http://www.springframework.org/schema/tx 
   http://www.springframework.org/schema/tx/spring-tx-3.0.xsd
   http://www.springframework.org/schema/context 
   http://www.springframework.org/schema/context/spring-context-3.0.xsd	
   http://www.springframework.org/schema/aop 
   http://www.springframework.org/schema/aop/spring-aop-3.0.xsd">
	<context:annotation-config />
	
	<!-- 组件扫描 -->
	<context:component-scan base-package="com.clotherdye.dao.impl" />
	<context:component-scan base-package="com.clotherdye.service.impl" />
	
	<!-- 添加数据源 -->
	<bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource"
		destroy-method="close">
		<property name="driverClassName" value="com.mysql.jdbc.Driver" />
		<property name="url" value="jdbc:mysql://localhost:3306/clother" />
		<property name="username" value="root" />
		<property name="password" value="password123" />
		<property name="initialSize" value="5"/>  
    	<property name="maxActive" value="10" />  
		<property name="testOnBorrow" value="true" />
		<property name="validationQuery" value="SELECT 1" />
	</bean>
	
	<!-- 配置会话工厂，连接 -->
	<bean id="sessionFactory"
		class="org.springframework.orm.hibernate3.annotation.AnnotationSessionFactoryBean">
		<property name="dataSource" ref="dataSource"></property>
		<property name="hibernateProperties">  
	        <props>  
	            <prop key="hibernate.dialect">org.hibernate.dialect.MySQL5Dialect</prop>  
	            <prop key="hibernate.show_sql">true</prop>  
	            <prop key="hibernate.format_sql">true</prop>  
	            <prop key="hibernate.hbm2ddl.auto">update</prop>  
	        </props>  
    	</property>  
		<property name="packagesToScan" value="com.clother.data.model" />
	</bean>
	
	<!-- 支持事务管理 -->
	<tx:annotation-driven />
    <bean id="transactionManager"
        class="org.springframework.orm.hibernate3.HibernateTransactionManager">
        <property name="sessionFactory" ref="sessionFactory"></property>
    </bean>
</beans>
```

然后保存全部修改，Maven重新编译打包之后，就可以直接使用了。

**特别要注意**的是配置会话工厂的时候使用的是*AnnotationSessionFactoryBean*作为会话工厂。然后还支持自动创建表，这个在项目开发过程中特别方便，项目结束之后改成validate即可。
**转载请务必注明出自《gloomyfish-jia20003》**


