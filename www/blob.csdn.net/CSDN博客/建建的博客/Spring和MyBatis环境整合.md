# Spring和MyBatis环境整合 - 建建的博客 - CSDN博客
2017年11月18日 18:19:25[纪建](https://me.csdn.net/u013898698)阅读数：86
　　SSH框架的结合几乎家喻户晓，但是一般的中小项目，使用Spring和MyBatis就够了，而且MyBatis轻便好使，易上手，值得大家尝试一次。
　　开篇简介：
　　Spring：
Spring是一个轻量级的控制反转(IoC)和面向切面(AOP)的容器框架。
两个重要模块：Spring 面向方面编程（AOP）和控制反转 （IOC） 容器。
控制反转模式（也称作依赖性介入）的基本概念是：不创建对象，但是描述创建它们的方式。在代码中不直接与对象和服务连接，但在配置文件中描述哪一个组件需要哪一项服务。
容器 （在 Spring 框架中是 IOC 容器） 负责将这些联系在一起。在典型的 IOC 场景中，容器创建了所有对象，并设置必要的属性将它们连接在一起，决定什么时间调用方法。
 　　MyBatis:
MyBatis 是支持普通 SQL查询，存储过程和高级映射的优秀**持久层**框架。MyBatis 消除了几乎所有的JDBC代码和参数的手工设置以及
结果集的检索。
MyBatis 使用简单的 XML或注解用于配置和原始映射，将接口和 Java 的POJOs（Plain Old Java Objects，普通的 Java
对象）映射成数据库中的记录。
 　　开始整合Spring和MyBatis：
　　1、在Eclipse里新建一个web项目，导入我们需要的jar包，可以在这里下载：[http://pan.baidu.com/s/1qW8Y6yo](http://pan.baidu.com/s/1qW8Y6yo)
　　jar列表截图：
![](http://images.cnitblog.com/i/360964/201407/211036363387882.png)
　　2、创建一个简单的数据库（springmybaitis）和数据表（user---id,username,password）。
![](http://images.cnitblog.com/i/360964/201407/211049485575521.png)
　　3、以用户管理为例，创建model和dao.
　　文件列表截图如下：
![](http://images.cnitblog.com/i/360964/201407/211046550884851.png)
　　model----User.java
```
1 package com.springMyBatis.system.model;
 2 
 3 public class User {
 4     private int id;
 5     private String username;
 6     private String password;
 7     public User(){}
 8     public int getId() {
 9         return id;
10     }
11     public void setId(int id) {
12         this.id = id;
13     }
14     public String getUsername() {
15         return username;
16     }
17     public void setUsername(String username) {
18         this.username = username;
19     }
20     public String getPassword() {
21         return password;
22     }
23     public void setPassword(String password) {
24         this.password = password;
25     }
26     public String toString(){
27         return "User[id="+id+" , username="+username+" , password="+password+"]";
28     }
29     
30 
31 }
```
　dao------接口UserDao.java　
```
1 package com.springMyBatis.system.dao;
 2 
 3 import com.springMyBatis.system.model.User;
 4 
 5 public interface UserDao {
 6     public User getUser(User user);
 7     public void addUser(User user);
 8     public void updateUser(User user);
 9     public void deleteUser(int UserId);
10 }
```
　　dao-----UserDao.xml
```
1 <?xml version="1.0" encoding="UTF-8"?>
 2 <!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"   
 3 "http://mybatis.org/dtd/mybatis-3-mapper.dtd">  
 4 <mapper namespace="com.springMyBatis.system.dao.UserDao">  
 5 <select id="getUser" parameterType="com.springMyBatis.system.model.User" resultType="com.springMyBatis.system.model.User">  
 6     SELECT * FROM user WHERE username=#{username} AND password=#{password}  
 7 </select>  
 8 <insert id="addUser" parameterType="com.springMyBatis.system.model.User" flushCache="true">  
 9    INSERT INTO user (id,username,password) VALUES (#{id},#{username},#{password})  
10 </insert>  
11 <update id="updateUser" parameterType="com.springMyBatis.system.model.User">  
12     UPDATE user SET password=#{password} WHERE id=#{id}  
13 </update>  
14 <delete id="deleteUser" parameterType="int">  
15     DELETE FROM user WHERE id=#{id}  
16 </delete>  
17 </mapper>
```
　　4、Spring配置文件----ApplicationContext.xml
![](http://images.cnitblog.com/i/360964/201407/211100352913141.png)
```
1 <?xml version="1.0" encoding="UTF-8"?>
 2 <beans  
 3     xmlns="http://www.springframework.org/schema/beans"  
 4     xmlns:tx="http://www.springframework.org/schema/tx"  
 5     xmlns:p="http://www.springframework.org/schema/p"  
 6     xmlns:aop="http://www.springframework.org/schema/aop"   
 7     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
 8     xsi:schemaLocation="http://www.springframework.org/schema/beans   
 9     http://www.springframework.org/schema/beans/spring-beans-3.0.xsd   
10     http://www.springframework.org/schema/tx   
11     http://www.springframework.org/schema/tx/spring-tx-3.0.xsd   
12     http://www.springframework.org/schema/aop    
13     http://www.springframework.org/schema/aop/spring-aop-3.0.xsd">  
14 <!-- 配置数据源-->  
15     <bean id="jdbcDataSource" class="org.springframework.jdbc.datasource.DriverManagerDataSource">  
16     <property name="driverClassName">  
17         <value>org.gjt.mm.mysql.Driver</value>  
18     </property>  
19     <property name="url">  
20         <value>jdbc:mysql://localhost:3306/springmybaitis?useUnicode=true&characterEncoding=UTF-8</value>  
21        <!--springmybaitis是我的数据库  -->
22     </property>  
23     <property name="username">  
24         <value>root</value>  
25     </property>  
26     <property name="password">  
27         <value>123456</value>  
28     </property>  
29 </bean>  
30 <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">  
31     <property name="dataSource" ref="jdbcDataSource" />  
32     <property name="configLocation" value="classpath:mybatis-config.xml"></property>  
33 </bean>  
34 <bean id="userDao" class="org.mybatis.spring.mapper.MapperFactoryBean">  
35     <property name="mapperInterface" value="com.springMyBatis.system.dao.UserDao"></property>  
36     <property name="sqlSessionFactory" ref="sqlSessionFactory"></property>  
37 </bean>  
38 </beans>
```
　　5、MyBatis的配置文件------mybatis-config.xml
![](http://images.cnitblog.com/i/360964/201407/211100544632584.png)
```
1 <?xml version="1.0" encoding="UTF-8"?>
2 <!DOCTYPE configuration PUBLIC  
3     "-//mybatis.org//DTD Config 3.0//EN"  
4     "http://mybatis.org/dtd/mybatis-3-config.dtd">  
5 <configuration>  
6     <mappers>  
7         <mapper resource="com/springMyBatis/system/dao/UserDao.xml"/>  
8     </mappers>  
9 </configuration>
```
　　6、测试
　　首先测试添加一条数据：
```
1 package com.springMyBatis.system.test;
 2 
 3 import org.springframework.context.ApplicationContext;
 4 import org.springframework.context.support.ClassPathXmlApplicationContext;
 5 
 6 import com.springMyBatis.system.dao.UserDao;
 7 import com.springMyBatis.system.model.User;
 8 
 9 public class UserController {
10     
11     /**
12      * @param args
13      */
14     public static void main(String[] args) {
15         ApplicationContext ctx=null;
16         ctx=new ClassPathXmlApplicationContext("applicationContext.xml");
17         UserDao userDao=(UserDao) ctx.getBean("userDao");
18         User user=new User();
19         //添加两条数据
20         user.setId(1);
21         user.setUsername("Jessica");
22         user.setPassword("123");
23         userDao.addUser(user);
24         user.setId(2);
25         user.setUsername("Jessica2");
26         user.setPassword("123");
27         userDao.addUser(user);
28         System.out.println("添加成功");
29         //查询数据
30         user.setUsername("Jessica");
31         user.setPassword("123");
32         System.out.println(userDao.getUser(user).toString());
33         user.setUsername("Jessica2");
34         user.setPassword("123");
35         System.out.println(userDao.getUser(user).toString());
36         //修改数据
37         user.setId(2);
38         user.setPassword("802");
39         userDao.updateUser(user);
40         System.out.println("修改成功");
41         //删除数据
42         userDao.deleteUser(1);
43         System.out.println("删除成功");
44         
45     }
46 
47 }
```
![](http://images.cnitblog.com/i/360964/201407/211111053699414.png)
　　最后数据库只剩一条数据：
![](http://images.cnitblog.com/i/360964/201407/211111413381460.png)
　　　　结合Spring和MyBatis很简单，对于一些小一些的项目，想要做到精致易掌控，这两个框架的结合足以。虽然现在Hibernate也很红，但是感觉它内部很神秘，很难配置，也不好优化，就是有种不敢下手的感觉。虽说Hibernate配置有点难，但是搭好了环境后，开发效率还是很高的，只需要配置一下表关系什么的就Ok了。但是比如说查个数据什么的，很难掌控的到细节。Mybatis就可以控制sql的细节，虽然写sql语句比较耗费时间，也需要你的sql基础，但是也正因为这样，使用MyBatis的人的sql水平才会提高啊，看到我们项目经理二三分钟弄出一个看似复杂却又很效率的sql语句，感觉很帅，很牛的样子。这么分析起来，两者皆不可抛，结合学习一下。本文只是简单的实例，在*dao.xml的配置文件里，还有其他帮助优化和简化sql，帮助提高效率的一些知识，有待我们进一步学习呢。还有，也可以通过结合Spring自动扫描装配dao等，也可以为配置文件减轻负担不少，期待下一次更成熟的使用。
