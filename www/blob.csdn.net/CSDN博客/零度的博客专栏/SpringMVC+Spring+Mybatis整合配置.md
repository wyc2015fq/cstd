# SpringMVC+Spring+Mybatis整合配置 - 零度的博客专栏 - CSDN博客
2016年07月13日 11:18:57[零度anngle](https://me.csdn.net/zmx729618)阅读数：2698
### 1、Maven依赖文件：pom.xml
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
  <modelVersion>4.0.0</modelVersion> 
  <groupId>org.nercita.ltxx</groupId>
  <artifactId>ssm.sample</artifactId>
  <packaging>war</packaging>
  <version>0.0.1-SNAPSHOT</version>
  <name>ssm</name>
  <url>http://maven.apache.org</url>
   
  <properties>
        <mysql.connector.version>5.1.21</mysql.connector.version>
        <spring.version>4.1.6.RELEASE</spring.version>
        <spring.security.version>3.2.3.RELEASE</spring.security.version>
        <junit.version>4.12</junit.version>
        <velocity.version>1.6.1</velocity.version>
        <velocity.tools.version>2.0</velocity.tools.version>
        <mock.version>2.0.8</mock.version>
        <mybatis.version>3.3.1</mybatis.version>
        <mybatis-spring.version>1.2.2</mybatis-spring.version>
  </properties>
  
  <dependencies>
        <!-- velocity -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context-support</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.velocity</groupId>
            <artifactId>velocity</artifactId>
            <version>${velocity.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.velocity</groupId>
            <artifactId>velocity-tools</artifactId>
            <version>${velocity.tools.version}</version>
        </dependency>
        <!-- jsp support -->
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>javax.servlet-api</artifactId>
            <version>3.1.0</version>
            <scope>provided</scope>
        </dependency>
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>jstl</artifactId>
            <version>1.2</version> <!-- just used the latest version, make sure you use the one you need -->
            <scope>provided</scope>
        </dependency>
        <!-- test support -->
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>${junit.version}</version>
            <scope>test</scope>
        </dependency>
        <!-- mybatis support -->
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
            <version>${mybatis.version}</version>
        </dependency>
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis-spring</artifactId>
            <version>${mybatis-spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.apache.commons</groupId>
            <artifactId>commons-dbcp2</artifactId>
            <version>2.0</version>
        </dependency>
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>${mysql.connector.version}</version>
        </dependency>
        <!-- Spring support -->
	    <dependency>
		      <groupId>org.springframework</groupId>
		      <artifactId>spring-core</artifactId>
		      <version>${spring.version}</version>
	    </dependency>
	    
	    <dependency>
		      <groupId>org.springframework</groupId>
		      <artifactId>spring-aop</artifactId>
		      <version>${spring.version}</version>
	    </dependency>
	    <dependency>
		      <groupId>org.springframework</groupId>
		      <artifactId>spring-context</artifactId>
		      <version>${spring.version}</version>
	    </dependency>    
	    <dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-context-support</artifactId>
			<version>${spring.version}</version> 
		</dependency> 
	    <dependency>
		      <groupId>org.springframework</groupId>
		      <artifactId>spring-jdbc</artifactId>
		      <version>${spring.version}</version>
	    </dependency>		
	    <dependency>
		      <groupId>org.springframework</groupId>
		      <artifactId>spring-beans</artifactId>
		      <version>${spring.version}</version>
	    </dependency>
	    <dependency>
		      <groupId>org.springframework</groupId>
		      <artifactId>spring-web</artifactId>
		      <version>${spring.version}</version>
	    </dependency>    
	    <dependency>
		      <groupId>org.springframework</groupId>
		      <artifactId>spring-expression</artifactId>
		      <version>${spring.version}</version>
	    </dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-orm</artifactId>
			<version>${spring.version}</version>
		</dependency> 
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-oxm</artifactId>
			<version>${spring.version}</version>
		</dependency>		
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-aspects</artifactId>
			<version>${spring.version}</version>
	    </dependency>
	    <dependency>
	        <groupId>org.springframework</groupId>
	        <artifactId>spring-tx</artifactId>
	        <version>${spring.version}</version>
	    </dependency>    	   
	    <dependency>
	      <groupId>org.springframework</groupId>
	      <artifactId>spring-test</artifactId>
	      <version>${spring.version}</version>
	    </dependency>    
	    <!-- spring mvc -->
	    <dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-webmvc</artifactId>
			<version>${spring.version}</version>
		</dependency>
		
	
	    
	    <!-- spring security -->
	    <dependency>
			<groupId>org.springframework.security</groupId>
			<artifactId>spring-security-core</artifactId>
			<version>${spring.security.version}</version>
		</dependency>
		
		<dependency>
			<groupId>org.springframework.security</groupId>
			<artifactId>spring-security-config</artifactId>
			<version>${spring.security.version}</version>
		</dependency>
		
		<dependency>
			<groupId>org.springframework.security</groupId>
			<artifactId>spring-security-taglibs</artifactId>
			<version>${spring.security.version}</version>
		</dependency>
		
		<dependency>
			<groupId>org.springframework.security</groupId>
			<artifactId>spring-security-web</artifactId>
			<version>${spring.security.version}</version>
		</dependency>
        
        <!-- aspectj weaver.jar 这是SpringAOP所要用到的依赖包 -->
	    <dependency>
	        <groupId>org.aspectj</groupId>
		    <artifactId>aspectjweaver</artifactId>
		    <version>1.8.2</version>
	    </dependency>
	    <dependency>
	        <groupId>org.aspectj</groupId>
	        <artifactId>aspectjrt</artifactId>
	        <version>1.8.2</version>
	    </dependency>
  </dependencies>
  <build>
    <finalName>ssm</finalName>
  </build>
</project>
```
### 2、web.xml
       web.xml规定了有哪些显式的servlet，规定了最顶层的Context环境，是整个的纲领。它同时规定了对于哪些URL的请求归哪些servlet处理，哪些URL使用哪种编码方式。需要注意的是，对于没有人为指定配置文件的servlet，会自动在WEB-INF文件夹下找servletName-servlet.xml的文件作为其配置。
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="3.0"  xmlns="http://java.sun.com/xml/ns/javaee" 
	     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
    
  <display-name>SSM Web Application</display-name>
  
  <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>classpath*:applicationContext.xml</param-value>
        <!--指定Context的配置文件-->
  </context-param>
  
  <!-- spring ContextLoaderListener 监听器 -->
  <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
  </listener>
  
  <!-- 编码过滤器 -->
  <filter>
        <filter-name>characterEncodingFilter</filter-name>
        <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
        <init-param>
            <param-name>encoding</param-name>
            <param-value>UTF-8</param-value>
        </init-param>
        <init-param>
            <param-name>forceEncoding</param-name>
            <param-value>true</param-value>
        </init-param>
    </filter>
    <filter-mapping>
        <filter-name>characterEncodingFilter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
    <!-- spring mvc DispatcherServlet -->
    <servlet>
        <servlet-name>ssm</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
			<param-name>contextConfigLocation</param-name>
			<param-value>classpath:config/webmvc-config.xml</param-value>
		</init-param>
        <load-on-startup>1</load-on-startup>
    </servlet>
    <servlet-mapping>
        <servlet-name>ssm</servlet-name>
        <url-pattern>/</url-pattern>
    </servlet-mapping>
  
</web-app>
```
### 3、applicationContext.xml
```
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:aop="http://www.springframework.org/schema/aop" 
	xmlns:context="http://www.springframework.org/schema/context"
	xmlns:jee="http://www.springframework.org/schema/jee" 
	xmlns:tx="http://www.springframework.org/schema/tx"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-4.0.xsd   
	                    http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.0.xsd   
	                    http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.0.xsd   
	                    http://www.springframework.org/schema/jee http://www.springframework.org/schema/jee/spring-jee-4.0.xsd   
	                    http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-4.0.xsd">
    <!-- 引入属性文件 -->
	<context:property-placeholder location="classpath*:/application.properties" />
	
    <context:spring-configured /> 
    <!-- 注解bean及依赖注入 -->
	<context:component-scan base-package="com.zmx.ssm">
		<context:exclude-filter expression="org.springframework.stereotype.Controller" type="annotation" />
	</context:component-scan>
    <!--数源设置-->
    <bean id="dataSource" class="org.apache.commons.dbcp2.BasicDataSource"
        destroy-method="close">
        <property name="driverClassName" value="${db.driverClass}" />
        <property name="url" value="${db.url}" />
        <property name="username" value="${db.username}" />
        <property name="password" value="${db.password}" />
    </bean>
    <!--sqlSessionFactory-->
    <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <property name="dataSource" ref="dataSource" />
        <!--指定了Mybatis配置文件的路径-->
        <property name="configLocation" value="classpath:mybatis-config.xml"/>
    </bean>
    <bean id="sqlSession" class="org.mybatis.spring.SqlSessionTemplate">
        <constructor-arg index="0" ref="sqlSessionFactory" />
    </bean>
    <!--这一段让我们的MapperInterface能够最终在Spring中生成Bean最后被AutoWired-->
    <bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
        <property name="basePackage" value="com.zmx.ssm.**.dao" />
    </bean>
    <bean id="transactionManager"
        class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="dataSource" />
    </bean>
    <!-- 开启注解事务 -->
    <tx:annotation-driven transaction-manager="transactionManager"/>
</beans>
```
application.properties
```
#Mysql settings
db.url=jdbc:mysql://localhost:3306/ssm
db.driverClass=com.mysql.jdbc.Driver
db.username=root
db.password=0729
```
### 4、mvc-config.xml
```
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:mvc="http://www.springframework.org/schema/mvc" xmlns:context="http://www.springframework.org/schema/context"
	xmlns:p="http://www.springframework.org/schema/p" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.0.xsd     
	http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.0.xsd 
	http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd">
    
	
	<!--处理对静态资源的访问-->
	<mvc:resources location="/common/"   mapping="/common/**"/>
	<mvc:resources location="/images/"   mapping="/images/**" />
	<mvc:resources location="/js/"       mapping="/js/**" /> 
	<mvc:resources location="/css/"      mapping="/css/**" />
	<mvc:resources location="/styles/"   mapping="/styles/**" />
	<mvc:resources location="/htm/"      mapping="/htm/**" />
	
    <import resource="view-controller.xml" />
    
    <context:annotation-config/>
	
	<context:component-scan base-package="com.zmx.ssm" use-default-filters="false">
		 <context:include-filter expression="org.springframework.stereotype.Controller" type="annotation" />
	</context:component-scan>
    <!--规定模板文件的类型和位置-->
    <bean id="velocityConfigurer"
        class="org.springframework.web.servlet.view.velocity.VelocityConfigurer">
        <property name="resourceLoaderPath" value="templates/" />
        <property name="velocityProperties">
            <props>
                <prop key="input.encoding">utf-8</prop>
                <prop key="output.encoding">utf-8</prop>
            </props>
        </property>
    </bean>
    
    <!--配置附加工具，以及将后缀为vm的文件交给下面的Resolver处理-->
    <bean id="velocityViewResolver"
        class="org.springframework.web.servlet.view.velocity.VelocityViewResolver">
        <property name="suffix" value=".vm" />
        <property name="contentType" value="text/html;charset=utf-8" />
        <property name="toolboxConfigLocation" value="/WEB-INF/toolbox.xml"/>
    </bean>
    
 
</beans>
```
### 5、Mybatis配置:mybatis-config.xml
```
<?xml version="1.0" encoding="UTF-8" ?>  
<!DOCTYPE configuration PUBLIC "-//mybatis.org//DTD Config 3.0//EN" 
"http://mybatis.org/dtd/mybatis-3-config.dtd"> 
<configuration>
     <typeAliases>       
        <package name="com.zmx.ssm.user.domain"/>
    </typeAliases>
    <mappers>
        <mapper resource="mapper/UserMapper.xml" /> 
        <mapper resource="mapper/OrderMapper.xml"/>  
    </mappers> 
    
<!-- 
<typeAliases>
	 <package name="com.zmx.ssm.user.domain"/>
</typeAliases>
  <environments default="development">
    <environment id="development">
      <transactionManager type="JDBC"/>
      <dataSource type="POOLED">
        <property name="driver" value="com.mysql.jdbc.Driver"/>
        <property name="url" value="jdbc:mysql://localhost:3306/ssm"/>
        <property name="username" value="root"/>
        <property name="password" value="0729"/>
      </dataSource>
    </environment>
  </environments>
  <mappers>
	<mapper resource="mapper/UserMapper.xml"/>
	<mapper resource="mapper/OrderMapper.xml"/>
  </mappers> 
  -->
    
    
    
</configuration>
```
### 6、velocity模板引擎：toolbox.xml  
```
<?xml version="1.0" encoding="UTF-8"?>
<toolbox>
    <tool>
        <key>date</key>
        <scope>request</scope>
        <class>
            org.apache.velocity.tools.generic.DateTool
        </class>
        <parameter name="format" value="yyyy-MM-dd HH:mm:ss"/>
    </tool>
    <tool>
        <key>link</key>
        <scope>request</scope>
        <class>org.apache.velocity.tools.view.tools.LinkTool</class>
    </tool>
    <tool>
        <key>stringUtils</key>
        <scope>request</scope>
        <class>org.apache.velocity.util.StringUtils</class>
    </tool>
    <tool>
        <key>math</key>
        <scope>application</scope>
        <class>org.apache.velocity.tools.generic.MathTool</class>
    </tool>
    <tool>
        <key>esc</key>
        <scope>request</scope>
        <class>org.apache.velocity.tools.generic.EscapeTool</class>
    </tool>
    <tool>
        <key>params</key>
        <scope>request</scope>
        <class>org.apache.velocity.tools.view.tools.ParameterParser</class>
    </tool>
</toolbox>
```
### 7、实体Bean
```
package com.zmx.ssm.user.domain;
import java.util.List;
public class User {
	private int id;
	private String account;
	private String password;
	private List<Order> orderList;
	public int getId() {
		return id; 
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getAccount() {
		return account;
	}
	public void setAccount(String account) {
		this.account = account;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public List<Order> getOrderList() {
		return orderList;
	}
	public void setOrderList(List<Order> orderList) {
		this.orderList = orderList;
	}
	@Override
	public String toString() {
		return "User [id=" + id + ", account=" + account + ", password="
				+ password + "]";
	}
	
	
}
```
```
package com.zmx.ssm.user.domain;
public class Order {
	
    private int id;  
    private double price;   
    private User user;
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public double getPrice() {
		return price;
	}
	public void setPrice(double price) {
		this.price = price;
	}
	public User getUser() {
		return user;
	}
	public void setUser(User user) {
		this.user = user;
	}
	@Override
	public String toString() {
		return "Order [id=" + id + ", price=" + price + "]";
	} 
    
    
}
```
### 8、Mybatis映射文件
UserMapper.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper
  PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
  "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
	
<mapper namespace="com.zmx.ssm.user.domain.User">
    
	<resultMap id="userResultMap" type="com.zmx.ssm.user.domain.User" >
		<id column="u_id" property="id"/>
		<result column="account" property="account"/>
		<result column="password" property="password"/>
		<collection property="orderList" javaType="ArrayList" ofType="com.zmx.ssm.user.domain.Order" column="u_id">
			<id column="o_id" property="id"/>
			<result column="price" property="price"/>
		</collection>
		
	</resultMap>
	
	<select id="selectUserFetchOrder" parameterType="int" resultMap="userResultMap" >
		select u.*, o.* from t_user u, t_order o where o.u_id=u.u_id and u.u_id=#{id}
	</select>
	
	<insert id="addUser" parameterType="User">
	    insert into t_user(u_id, account, password) values(#{id}, #{account}, #{password})	    
	</insert>
	
	
</mapper>
```
OrderMapper.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper
  PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
  "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<mapper namespace="com.zmx.ssm.user.domain.Order" >
	<resultMap id="orderResultMap" type="com.zmx.ssm.user.domain.Order" >
		<id column="o_id" property="id"/>
		<result column="price" property="price"/>
		<association property="user" javaType="com.zmx.ssm.user.domain.User" column="u_id">
			<id column="u_id" property="id"/>
			<result column="account" property="account"/>
		    <result column="password" property="password"/>
		</association>
	</resultMap>
	
	<select id="selectOrderFetchUser" parameterType="int" resultMap="orderResultMap">
		select u.*,o.* from t_user u, t_order o where o.u_id=u.u_id and o.o_id=#{id} 
	</select>
</mapper>
```
### 9、UserDao.java
```
package com.zmx.ssm.user.dao;
import javax.annotation.Resource;
import org.mybatis.spring.SqlSessionTemplate;
import org.springframework.stereotype.Repository;
import com.zmx.ssm.user.domain.User;
@Repository
public class UserDao {
	@Resource(name="sqlSession")
	private SqlSessionTemplate sqlSession; 
	
	public void addUser(User user) {
		
		sqlSession.insert("addUser",user);
	}
}
```
### 10、使用Spring-test测试
```
package test.com.zmx.ssm.user;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.transaction.TransactionConfiguration;
import org.springframework.transaction.annotation.Transactional;
import com.zmx.ssm.user.dao.UserDao;
import com.zmx.ssm.user.domain.User;
  
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:applicationContext.xml"})
@TransactionConfiguration(transactionManager="transactionManager",defaultRollback=false)
@Transactional
public class UserDaoTest {
	
    @Autowired
    private UserDao userDao;
    @Test
    public void testInsert() {
        User user=new User();
        user.setId(3);
        user.setAccount("manager");
        user.setPassword("123456");        
        userDao.addUser(user);
    }
}
```
### 11、小结
Spring常用的注解主要有以下几种：
- @Component
- @Service--业务层
- @Respority--持久化层
- @Controller--视图层
       尽管这几种注解对应的意义不一样，但它们都有一个共同的作用，那就是让Spring找到并且最终生成对应的Bean。举个例子，我们在Service中使用接口UserDAO，但该接口的实例我们并没有指定使用哪一个，虽然我们写了对应的Impl Class。 通过注解生成Bean，Spring就能够在Service调用该接口的时候从容器里得到正确的实体对象。
