# 使用idea搭建SSM框架 - 零度的博客专栏 - CSDN博客
2017年11月02日 10:43:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：878
环境：
　　idea 2017.1
　　jdk1.8
　　Maven 3.3.9
步骤：
一、新建Maven webapp项目
![](http://images2015.cnblogs.com/blog/1028599/201703/1028599-20170330133558811-1365797169.png)
![](http://images2015.cnblogs.com/blog/1028599/201703/1028599-20170330133622139-1403001733.png)
![](http://images2015.cnblogs.com/blog/1028599/201703/1028599-20170330133635654-502904805.png)
![](http://images2015.cnblogs.com/blog/1028599/201703/1028599-20170330133647342-1577021820.png)
　　点击Finish，第一次搭建可能会很慢，甚至可能需要VPN才能搭建成功
二、项目目录结构
　　我这里列出的是搭建完了之后所有的目录和文件，诸位先把目录文件建起来，然后我在给出文件内容
![](http://images2015.cnblogs.com/blog/1028599/201703/1028599-20170330134119920-351921480.png)
　　这里的目录建好之后还需要设置一下，让idea识别目录作用，选择File-Project Structure
![](http://images2015.cnblogs.com/blog/1028599/201703/1028599-20170330134447826-929310673.png)
　　设置完成后ok
三、配置文件内容
pom.xml
```
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.chatRobot</groupId>
    <artifactId>ChatRobot</artifactId>
    <packaging>war</packaging>
    <version>1.0-SNAPSHOT</version>
    <name>ChatRobot Maven Webapp</name>
    <url>http://maven.apache.org</url>
    <properties>
        <!-- 设置项目编码编码 -->
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
        <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
        <!-- spring版本号 -->
        <spring.version>4.3.5.RELEASE</spring.version>
        <!-- mybatis版本号 -->
        <mybatis.version>3.4.1</mybatis.version>
    </properties>
    <dependencies>
        <!-- java ee -->
        <dependency>
            <groupId>javax</groupId>
            <artifactId>javaee-api</artifactId>
            <version>7.0</version>
        </dependency>
        <!-- 单元测试 -->
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.12</version>
        </dependency>
        <!-- 实现slf4j接口并整合 -->
        <dependency>
            <groupId>ch.qos.logback</groupId>
            <artifactId>logback-classic</artifactId>
            <version>1.2.2</version>
        </dependency>
        <!-- JSON -->
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
            <version>2.8.7</version>
        </dependency>
        <!-- 数据库 -->
        <dependency>
            <groupId>mysql</groupId>
            <artifactId>mysql-connector-java</artifactId>
            <version>5.1.41</version>
            <scope>runtime</scope>
        </dependency>
        <!-- 数据库连接池 -->
        <dependency>
            <groupId>com.mchange</groupId>
            <artifactId>c3p0</artifactId>
            <version>0.9.5.2</version>
        </dependency>
        <!-- MyBatis -->
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis</artifactId>
            <version>${mybatis.version}</version>
        </dependency>
        <!-- mybatis/spring整合包 -->
        <dependency>
            <groupId>org.mybatis</groupId>
            <artifactId>mybatis-spring</artifactId>
            <version>1.3.1</version>
        </dependency>
        <!-- Spring -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-core</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-beans</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-context</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-jdbc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-tx</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <version>${spring.version}</version>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-test</artifactId>
            <version>${spring.version}</version>
        </dependency>
    </dependencies>
    <build>
        <finalName>ChatRobot</finalName>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <configuration>
                    <!-- 设置JDK版本 -->
                    <source>1.8</source>
                    <target>1.8</target>
                </configuration>
            </plugin>
        </plugins>
    </build>
</project>
 注意右下角更新pom
logback.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<configuration debug="true">
    <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
        <encoder>
            <pattern>%d{HH:mm:ss.SSS} [%thread] %-5level %logger{36} - %msg%n</pattern>
        </encoder>
    </appender>
    <root level="debug">
        <appender-ref ref="STDOUT"/>
    </root>
</configuration>
这里可以控制输出格式和内容，有兴趣的可以自己设置
jdbc.properties
```
jdbc.driver=com.mysql.jdbc.Driver
#数据库地址
jdbc.url=jdbc:mysql://xxxxxxxxx:3306/ChatRobot?useUnicode=true&characterEncoding=utf8
#用户名
jdbc.username=xxxx
#密码
jdbc.password=xxxxx
#最大连接数
c3p0.maxPoolSize=30
#最小连接数
c3p0.minPoolSize=10
#关闭连接后不自动commit
c3p0.autoCommitOnClose=false
#获取连接超时时间
c3p0.checkoutTimeout=10000
#当获取连接失败重试次数
c3p0.acquireRetryAttempts=2
spring-mybatis.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:tx="http://www.springframework.org/schema/tx"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context.xsd
       http://www.springframework.org/schema/tx
       http://www.springframework.org/schema/tx/spring-tx.xsd">
    <!-- 扫描service包下所有使用注解的类型 -->
    <context:component-scan base-package="com.chatRobot.service"/>
    <!-- 配置数据库相关参数properties的属性：${url} -->
    <context:property-placeholder location="classpath:jdbc.properties"/>
    <!-- 数据库连接池 -->
    <bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource">
        <property name="driverClass" value="${jdbc.driver}"/>
        <property name="jdbcUrl" value="${jdbc.url}"/>
        <property name="user" value="${jdbc.username}"/>
        <property name="password" value="${jdbc.password}"/>
        <property name="maxPoolSize" value="${c3p0.maxPoolSize}"/>
        <property name="minPoolSize" value="${c3p0.minPoolSize}"/>
        <property name="autoCommitOnClose" value="${c3p0.autoCommitOnClose}"/>
        <property name="checkoutTimeout" value="${c3p0.checkoutTimeout}"/>
        <property name="acquireRetryAttempts" value="${c3p0.acquireRetryAttempts}"/>
    </bean>
    <!-- 配置SqlSessionFactory对象 -->
    <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
        <!-- 注入数据库连接池 -->
        <property name="dataSource" ref="dataSource"/>
        <!-- 扫描model包 使用别名 -->
        <property name="typeAliasesPackage" value="com.chatRobot.model"/>
        <!-- 扫描sql配置文件:mapper需要的xml文件 -->
        <property name="mapperLocations" value="classpath:mapper/*.xml"/>
    </bean>
    <!-- 配置扫描Dao接口包，动态实现Dao接口，注入到spring容器中 -->
    <bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">
        <!-- 注入sqlSessionFactory -->
        <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory"/>
        <!-- 给出需要扫描Dao接口包 -->
        <property name="basePackage" value="com.chatRobot.dao"/>
    </bean>
    <!-- 配置事务管理器 -->
    <bean id="transactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <!-- 注入数据库连接池 -->
        <property name="dataSource" ref="dataSource"/>
    </bean>
    <!-- 配置基于注解的声明式事务 -->
    <tx:annotation-driven transaction-manager="transactionManager"/>
</beans>
spring-mvc.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:mvc="http://www.springframework.org/schema/mvc"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context.xsd
       http://www.springframework.org/schema/mvc
       http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd">
    <!-- 扫描web相关的bean -->
    <context:component-scan base-package="com.chatRobot.controller"/>
    <!-- 开启SpringMVC注解模式 -->
    <mvc:annotation-driven/>
    <!-- 静态资源默认servlet配置 -->
    <mvc:default-servlet-handler/>
    <!-- 配置jsp 显示ViewResolver -->
    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>
        <property name="prefix" value="/WEB-INF/views/"/>
        <property name="suffix" value=".jsp"/>
    </bean>
</beans>
web.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_3_1.xsd"
         version="3.1">
    <display-name>ChatRobot</display-name>
    <description>ChatRobot_Alpha_0.0.1</description>
    <!-- 编码过滤器 -->
    <filter>
        <filter-name>encodingFilter</filter-name>
        <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
        <init-param>
            <param-name>encoding</param-name>
            <param-value>UTF-8</param-value>
        </init-param>
    </filter>
    <filter-mapping>
        <filter-name>encodingFilter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>
    <!-- 配置DispatcherServlet -->
    <servlet>
        <servlet-name>SpringMVC</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <!-- 配置springMVC需要加载的配置文件-->
        <init-param>
            <param-name>contextConfigLocation</param-name>
            <param-value>classpath:spring-*.xml</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
        <async-supported>true</async-supported>
    </servlet>
    <servlet-mapping>
        <servlet-name>SpringMVC</servlet-name>
        <!-- 匹配所有请求，此处也可以配置成 *.do 形式 -->
        <url-pattern>/</url-pattern>
    </servlet-mapping>
    <welcome-file-list>
        <welcome-file>index.html</welcome-file>
    </welcome-file-list>
</web-app>
到这里基本上环境就搭建完成了，下面开始测试
四、测试
先导入一份数据库测试文件
```
CREATE TABLE `user` (
  `id` int(**11**) NOT NULL AUTO_INCREMENT COMMENT '用户ID',
  `email` varchar(**255**) NOT NULL COMMENT '用户邮箱',
  `password` varchar(**255**) NOT NULL COMMENT '用户密码',
  `username` varchar(**255**) NOT NULL COMMENT '用户昵称',
  `role` varchar(**255**) NOT NULL COMMENT '用户身份',
  `status` int(**1**) NOT NULL COMMENT '用户状态',
  `regTime` datetime NOT NULL COMMENT '注册时间',
  `regIp` varchar(**255**) NOT NULL COMMENT '注册IP',
  PRIMARY KEY (`id`),
  UNIQUE KEY `email` (`email`) USING BTREE
) ENGINE=InnoDB AUTO_INCREMENT=**2** DEFAULT CHARSET=utf8;
INSERT INTO `user` VALUES ('1', 'xxx', 'xxxxx', 'xxxxx', 'root', '0', '2017-03-28 09:40:31', '127.0.0.1');
SET FOREIGN_KEY_CHECKS=**1**;
接下来配置类
UserController
```
package com.chatRobot.controller;
import javax.servlet.http.HttpServletRequest;
import com.chatRobot.model.User;
import com.chatRobot.service.IUserService;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import javax.annotation.Resource;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
@Controller
@RequestMapping("/user")
public class UserController {
    @Resource
    private IUserService userService;
    @RequestMapping("/showUser.do")
    public void selectUser(HttpServletRequest request, HttpServletResponse response) throws IOException {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");
        long userId = Long.parseLong(request.getParameter("id"));
        User user = this.userService.selectUser(userId);
        ObjectMapper mapper = new ObjectMapper();
        response.getWriter().write(mapper.writeValueAsString(user));
        response.getWriter().close();
    }
}
IUserDao
```
package com.chatRobot.dao;
import org.springframework.stereotype.Repository
import com.chatRobot.model.User;
@Repository
public interface IUserDao {
    User selectUser(long id);
}
User
```
package com.chatRobot.model;
import java.util.Date;
public class User {
    private long id;
    private String email;
    private String password;
    private String username;
    private String role;
    private int status;
    private Date regTime;
    private String regIp;
    public long getId() {
        return id;
    }
    public void setId(int id) {
        this.id = id;
    }
    public String getEmail() {
        return email;
    }
    public void setEmail(String email) {
        this.email = email;
    }
    public String getPassword() {
        return password;
    }
    public void setPassword(String password) {
        this.password = password;
    }
    public String getUsername() {
        return username;
    }
    public void setUsername(String username) {
        this.username = username;
    }
    public String getRole() {
        return role;
    }
    public void setRole(String role) {
        this.role = role;
    }
    public int getStatus() {
        return status;
    }
    public void setStatus(int status) {
        this.status = status;
    }
    public Date getRegTime() {
        return regTime;
    }
    public void setRegTime(Date regTime) {
        this.regTime = regTime;
    }
    public String getRegIp() {
        return regIp;
    }
    public void setRegIp(String regIp) {
        this.regIp = regIp;
    }
}
IUserService
```
package com.chatRobot.service;
import com.chatRobot.model.User;
public interface IUserService {
    public User selectUser(long userId);
}
UserServiceImpl
```
package com.chatRobot.service.impl;
import com.chatRobot.dao.IUserDao;
import com.chatRobot.model.User;
import com.chatRobot.service.IUserService;
import org.springframework.stereotype.Service;
import javax.annotation.Resource;
@Service("userService")
public class UserServiceImpl implements IUserService {
    @Resource
    private IUserDao userDao;
    public User selectUser(long userId) {
        return this.userDao.selectUser(userId);
    }
}
UserDao.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd">
<!-- 设置为IUserDao接口方法提供sql语句配置 -->
<mapper namespace="com.chatRobot.dao.IUserDao">
    <select id="selectUser" resultType="User" parameterType="long">
        SELECT * FROM user WHERE id = #{id}
    </select>
</mapper>
然后新建个测试类，来测试mybatis
IUserDaoTest
```
package com.chatRobot.dao;
import com.chatRobot.model.User;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
// 加载spring配置文件
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration({"classpath:spring-mybatis.xml"})
public class IUserDaoTest {
    @Autowired
    private IUserDao dao;
    @Test
    public void testSelectUser() throws Exception {
        long id = 1;
        User user = dao.selectUser(id);
        System.out.println(user.getUsername());
    }
}
运行后结果应该是会在控制台输出id为1的用户名，没成功的话就找BUG去吧。
![](http://images2015.cnblogs.com/blog/1028599/201703/1028599-20170330140048873-1889210725.png)
继续新建个页面来测试springmvc和mybatis
index.html
```
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>test</title>
</head>
<script>
    function selectUser() {
        var xmlhttp = new XMLHttpRequest();
        xmlhttp.onreadystatechange = function () {
            if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
                document.getElementById("test").innerHTML = xmlhttp.responseText;
            }
        }
        xmlhttp.open("POST", "user/showUser.do", true);
        xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
        xmlhttp.send("id=1");
    }
</script>
<body>
<p id="test">Hello World!</p>
<button type="button" onclick="selectUser()">onclick test</button>
</body>
</html>
新建完成后配置项目运行环境，点击Run-Edit Configurations...
点击加号新建运行环境，选择Tomcat Server-Local
![](http://images2015.cnblogs.com/blog/1028599/201703/1028599-20170330140431014-1273897059.png)
选中新建好的服务器，右边选择Deployment,点击加号-Atifact...
![](http://images2015.cnblogs.com/blog/1028599/201703/1028599-20170330140540920-1603755980.png)
选择第二项
![](http://images2015.cnblogs.com/blog/1028599/201703/1028599-20170330140551592-371587676.png)
然后在右边Application context配置你的项目名
![](http://images2015.cnblogs.com/blog/1028599/201703/1028599-20170330140700936-245748613.png)
 最后运行项目，在打开的页面中点击按钮测试，成功的话会在页面上显示id为1的用户信息
```
```
```
```
```
```
```
```
```
```
```
```
```
```
```
