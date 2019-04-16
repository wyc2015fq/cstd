# Java学习笔记32：Spring整合mybatis使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年02月25日 17:57:14[initphp](https://me.csdn.net/initphp)阅读数：4156
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









MyBatis 是支持普通 SQL查询，[存储过程](http://baike.baidu.com/view/68525.htm)和高级映射的优秀[持久层](http://baike.baidu.com/view/198047.htm)框架。MyBatis 消除了几乎所有的[JDBC](http://baike.baidu.com/view/25611.htm)
 代码和参数的手工设置以及[结果集](http://baike.baidu.com/view/691831.htm)的检索。MyBatis 使用简单的 XML或注解用于配置和原始映射，将接口和 Java 的POJOs（Plain Old Java Objects，普通的 Java对象）映射成数据库中的记录。

Spring框架中整合mybatis主要下面几个步骤：




1. 需要解决依赖包。在POM.XML中，配置一下依赖（希望你在看这篇文章的时候，已经熟练使用mvn了，否则挺麻烦的！）



```
<!-- mybatis -->
		 <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
            <version>3.1.1</version>
        </dependency>  
            <dependency>  
                <groupId>org.mybatis</groupId>  
                <artifactId>mybatis-spring</artifactId>  
                <version>1.1.1</version>  
            </dependency>  
        <dependency>
            <groupId>commons-dbcp</groupId>
            <artifactId>commons-dbcp</artifactId>
            <version>1.4</version>
        </dependency>
        <dependency>
			<groupId>mysql</groupId>
			<artifactId>mysql-connector-java</artifactId>
			<version>5.1.23</version>
		</dependency>
```



2. 依赖包解决之后，我们首先要配置spring的xml文件，我们配置了一个service-dal.xml的文件，这个文件会被web.xml包含进去，这个文件中主要做了几个工作：JDBC的数据库链接，sqlSessionFactory，SqlSessionTemplate以及扫描Impl的包。



```
<?xml version="1.0" encoding="UTF-8"?>
<beans:beans xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns:beans="http://www.springframework.org/schema/beans"
	xmlns:context="http://www.springframework.org/schema/context"
	xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
		http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context.xsd"
	default-autowire="byName">
	<!-- 数据库JDBC的配置 -->
    <beans:bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource">
        <beans:property name="driverClassName" value="com.mysql.jdbc.Driver"></beans:property>
        <beans:property name="url" value="${account.mysql.url}"></beans:property>
        <beans:property name="username" value="${account.mysql.username}"></beans:property>
        <beans:property name="password" value="${account.mysql.password}"></beans:property>
        <beans:property name="defaultAutoCommit" value="true"></beans:property>
    </beans:bean>
	<!-- 加载myBatis-config.xml配置文件，以及扫描myBatis/目录下每个DAO对应的SQL配置的XML文件 -->
    <beans:bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <beans:property name="dataSource" ref="dataSource" />
        <beans:property name="configLocation" value="classpath:myBatis-config.xml" />
        <beans:property name="mapperLocations" value="classpath:myBatis/*.xml" />
    </beans:bean>
    <!-- SqlSessionTemplate是我们代码中使用的SQL模板对象，用来操作数据库 -->
    <beans:bean class="org.mybatis.spring.SqlSessionTemplate">
    	<beans:constructor-arg ref="sqlSessionFactory"/>
    </beans:bean>
	<!-- Spring Bean扫描 -->
	<context:component-scan base-package="com.xxx.account.dal">
		<context:include-filter type="regex" expression=".*Impl" />
	</context:component-scan>
</beans:beans>
```


3. 上面这个配置文件中，我们导入了myBatis-config.xml配置文件。由于我们开启了mapperLocations自动扫描DAO对应的SQL配置的XML文件，所以myBatis-config.xml非常简单。



```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN"
"http://mybatis.org/dtd/mybatis-3-config.dtd">
<configuration>
    <mappers></mappers>
</configuration>
```


4. myBatis/目录下是SQL配置的文件，以一个简单的XML文件为例，我们使用如下：（下面的例子是删除一个数据）





```
<?xml version="1.0" encoding="UTF-8" ?>  
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd"> 
<!--这边的namespace很重要哦，调用的时候需要这个namespace来调哦！-->
<mapper namespace="com.mybatis.UserDao">
    <delete id="countAll">
       	DELETE FROM guest_book WHERE id = 41
    </delete>
</mapper>
```


5. 配置已经基本完成了，剩下，我们需要写一个DAO了，因为删除语句比较简单，DAO也比较简单

声明接口




```java
package com.xxx.account.dal;

public interface OpenapiInfoDal {
	
	
	Integer getAll();
}
```



基类，注解的方式载入SqlSessionTemplate实例


```java
package com.xxx.account.dal;

import org.mybatis.spring.SqlSessionTemplate;
import org.springframework.beans.factory.annotation.Autowired;

public class BaseDal {
	
	@Autowired
	protected SqlSessionTemplate sqlSessionTemplate;
}
```


具体实现：





```java
package com.xxx.account.dal.impl;

import com.xxxx.account.dal.BaseDal;
import com.xxx.account.dal.OpenapiInfoDal;


public class OpenapiInfoDalImpl extends BaseDal implements OpenapiInfoDal {
	
	public Integer getAll() {
		sqlSessionTemplate.delete("com.mybatis.UserDao.countAll");
		return 1;
	}
}
```


Controller中调用：





```java
package com.xxx.account.web;


/**
 * Handles requests for the application home page.
 */
@Controller
public class HomeController {
	
	
	@Autowired
	private OpenapiInfoDal openapiInfoDal;
	
	@RequestMapping(value = "/test", method = RequestMethod.GET)
	@ResponseBody
	public String test(HttpServletRequest request) {
		openapiInfoDal.getAll(); //调用删除方法
		
	}
	
}
```


说明：这边只是最简单的整合，具体的SQL XML如何配置，详细的MyBatis的配置等等还需要自己去看下手册










