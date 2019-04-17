# maven+springmvc+spring+mybatis+velocity整合 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月13日 00:23:27[boonya](https://me.csdn.net/boonya)阅读数：806









原文地址：[http://www.cnblogs.com/java-zhao/p/5096811.html](http://www.cnblogs.com/java-zhao/p/5096811.html)

Github代码：[https://github.com/zhaojigang/ssmm0](https://github.com/zhaojigang/ssmm0)

**一、ssmm简介**
- ssmm是当下企业最常用的开发框架架构
- maven：管理项目jar包，构建项目
- spring：IOC容器，事务管理
- springmvc：mvc框架
- myBatis：持久层框架
- velocity：前端视图模板（**相较于jsp，速度非常快，而且不需要占据jvm的永久代内存**）

上述这些框架再加上版本控制工具（git）、自动化部署工具（jenkins），就组成了当下中大型企业最常用的项目开发部署架构；以上各种框架以后我也会依次做笔记去写的，所以在下边不会做详细介绍。还有，在以下的整合过程中会有一些细节方面的内容，我会在后续的本系列博客中仔细去说。



**二、下面介绍怎样整合ssmm**

环境：
- eclipse-jee-indigo-SR2-win32（新出的一些eclipse不支持jdk1.6）
- jdk1.6.45（当下企业最常用的jdk版本）
- spring3.2.6
- mybatis3.1.1
- mybatis-spring1.1.1（mybatis与spring集成的一个工具jar）
- mysql5.1.27
- maven3.0.5
- velocity1.5



**2.1、构建maven项目**

步骤：

**2.1.1、手工创建maven项目（整个过程就是创建一个符合maven格式的目录结构）**

注意：这里使用手工创建，而不是在eclipse中使用maven插件去创建，是因为个人感觉eclipse的maven插件不好用。

首先自己创建一个文件夹ssmm（这个文件夹名称就是我们的项目名称），然后在ssmm下创建src文件夹和pom.xml文件。

![](http://images2015.cnblogs.com/blog/866881/201601/866881-20160103125352635-1565441131.jpg)

其中，pom.xml文件中的内容如下：

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 <?xml version="1.0" encoding="UTF-8"?>
 2 <project xmlns="http://maven.apache.org/POM/4.0.0" 
 3          xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 4          xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
 5 
 6     <modelVersion>4.0.0</modelVersion>
 7 
 8     <groupId>com.xxx</groupId>
 9     <artifactId>ssmm</artifactId>
10     <version>1.0-SNAPSHOT</version>
11     
12     <name>ssm</name>
13     <packaging>war</packaging>
14 
15 </project>

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

以上内容，会在之后的maven模块中讲解。

然后，在src下创建main、test两个文件夹；在main文件夹下创建java、resources、webapp三个文件夹，在test文件夹下建立java、resources两个文件夹；之后在webapp下创建META-INF和WEB-INF两个文件夹，在META-INF文件夹下创建MANIFEST.MF文件，该文件内容如下：

```
Manifest-Version:1.0
```

在WEB-INF文件夹下创建templates（该文件夹将来用于存放模板文件）和web.xml文件，web.xml文件内容如下：

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

1 <?xml version="1.0" encoding="utf-8"?>
2 <web-app version="2.5" xmlns="http://java.sun.com/xml/ns/javaee"
3     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
4     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
5         
6 </web-app>
```

当构建好以上目录结构后，在src/main/java下创建一个TestMaven.java文件，文件内容如下：

```
public class TestMaven {
    public static void main(String[] args) {
        System.out.println("hello zhaojigang!!!");
    }
}
```

**2.1.2、使用命令窗口编译该项目**

在ssmm文件夹下（即pom.xml所在的目录）使用打开命令窗口（shift+鼠标右键），使用"mvn compile"编译该项目（前提：电脑安装了maven）。编译成功出现"BUILD SUCCESS"。

**2.1.3、引入eclipse**

在eclipse中"import"-->"Existing Maven Projects"即可（执行此步之前，需要安装eclipse的maven插件）。

引入项目后，在ssmm项目上右击-->"Properties"-->Text file encoding改为UTF-8

然后对项目进行测试（在webapp目录下建立index.jsp，然后运行项目，在我这里，是使用jetty来运行的，然后通过访问浏览器查看项目是否建立成功）。



**2.2、引入spring与springmvc**

**2.2.1、引入jar包**

在这里为了方便，我直接给出这些框架整合的完整的pom.xml（具体引入的每个包做什么用，看pom.xml的注释），之后其他框架的整合就不再讨论jar包问题了。

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


![复制代码](http://common.cnblogs.com/images/copycode.gif)

  1 <?xml version="1.0" encoding="UTF-8"?>
  2 <project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  3     xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
  4 
  5     <modelVersion>4.0.0</modelVersion>
  6 
  7     <groupId>com.xxx</groupId>
  8     <artifactId>ssmm</artifactId>
  9     <version>1.0-SNAPSHOT</version>
 10 
 11     <name>ssmm</name>
 12     <packaging>war</packaging>
 13 
 14     <properties>
 15         <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
 16         <project.reporting.outputEncoding>UTF-8</project.reporting.outputEncoding>
 17     </properties>
 18 
 19     <!-- 引入实际依赖 -->
 20     <dependencies>
 21         <!-- json -->
 22         <dependency>
 23             <groupId>com.alibaba</groupId>
 24             <artifactId>fastjson</artifactId>
 25             <version>1.1.39</version>
 26         </dependency>
 27         <!-- servlet -->
 28         <dependency>
 29             <groupId>javax.servlet</groupId>
 30             <artifactId>servlet-api</artifactId>
 31             <version>2.5</version>
 32             <scope>provided</scope>
 33         </dependency>
 34         <!-- spring -->
 35         <dependency>
 36             <groupId>org.springframework</groupId>
 37             <artifactId>spring-core</artifactId>
 38             <version>3.2.6.RELEASE</version>
 39         </dependency>
 40         <dependency>
 41             <groupId>org.springframework</groupId>
 42             <artifactId>spring-beans</artifactId>
 43             <version>3.2.6.RELEASE</version>
 44         </dependency>
 45         <dependency>
 46             <groupId>org.springframework</groupId>
 47             <artifactId>spring-context</artifactId>
 48             <version>3.2.6.RELEASE</version>
 49         </dependency>
 50         <dependency>
 51             <groupId>org.springframework</groupId>
 52             <artifactId>spring-web</artifactId>
 53             <version>3.2.6.RELEASE</version>
 54         </dependency>
 55         <dependency>
 56             <groupId>org.springframework</groupId>
 57             <artifactId>spring-webmvc</artifactId>
 58             <version>3.2.6.RELEASE</version>
 59         </dependency>
 60         <!-- 这个是使用velocity的必备包 -->
 61         <dependency>
 62             <groupId>org.springframework</groupId>
 63             <artifactId>spring-context-support</artifactId>
 64             <version>3.2.6.RELEASE</version>
 65             <scope>compile</scope>
 66         </dependency>
 67         <!-- mysql -->
 68         <dependency>
 69             <groupId>mysql</groupId>
 70             <artifactId>mysql-connector-java</artifactId>
 71             <version>5.1.27</version>
 72             <scope>runtime</scope>
 73         </dependency>
 74         <!-- 数据源 -->
 75         <dependency>
 76             <groupId>org.apache.tomcat</groupId>
 77             <artifactId>tomcat-jdbc</artifactId>
 78             <version>7.0.47</version>
 79         </dependency>
 80         <!-- mybatis -->
 81         <dependency>
 82             <groupId>org.mybatis</groupId>
 83             <artifactId>mybatis</artifactId>
 84             <version>3.1.1</version>
 85         </dependency>
 86         <dependency>
 87             <groupId>org.mybatis</groupId>
 88             <artifactId>mybatis-spring</artifactId>
 89             <version>1.1.1</version>
 90         </dependency>
 91         <!-- velocity -->
 92         <dependency>
 93             <groupId>org.apache.velocity</groupId>
 94             <artifactId>velocity</artifactId>
 95             <version>1.5</version>
 96         </dependency>
 97         <dependency>
 98             <groupId>velocity-tools</groupId>
 99             <artifactId>velocity-tools-generic</artifactId>
100             <version>1.2</version>
101         </dependency>
102     </dependencies>
103 </project>

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

**2.2.2、配置spring.xml文件**

这里直接列出spring.xml的完整版，包括读取属性文件、配置数据源、配置fastjson转换器、配置mybatis、配置velocity。

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 <?xml version="1.0" encoding="UTF-8"?>
 2 <beans xmlns="http://www.springframework.org/schema/beans"
 3     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context"
 4     xmlns:mvc="http://www.springframework.org/schema/mvc"
 5     xsi:schemaLocation="http://www.springframework.org/schema/beans 
 6                            http://www.springframework.org/schema/beans/spring-beans-3.2.xsd
 7                            http://www.springframework.org/schema/context 
 8                            http://www.springframework.org/schema/context/spring-context-3.2.xsd
 9                            http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc-3.2.xsd">
10                            
11     <!-- 注解扫描 -->
12     <context:component-scan base-package="com.xxx" />
13     
14     <!-- 引入属性文件 -->
15     <context:property-placeholder location="classpath:jdbc.properties"/>
16     
17     <!-- 配置fastjson转换器 -->
18     <mvc:annotation-driven>
19         <mvc:message-converters register-defaults="true">
20             <bean class="com.alibaba.fastjson.support.spring.FastJsonHttpMessageConverter"></bean>
21         </mvc:message-converters>
22     </mvc:annotation-driven>
23     
24     <!-- 引入数据源 -->
25     <bean id="xxxDataSource" class="org.apache.tomcat.jdbc.pool.DataSource" destroy-method="close">
26         <property name="driverClassName" value="${jdbc.driverClassName}" />
27         <property name="url" value="${jdbc.url}" />
28         <property name="username" value="${jdbc.username}" />
29         <property name="password" value="${jdbc.password}" />
30     </bean>
31     
32     <!-- 引入mybatis -->
33     <bean id="xxxSqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">
34         <property name="dataSource" ref="xxxDataSource" />
35     </bean>
36     <bean id="xxxMapperScannerConfigurer" class="org.mybatis.spring.mapper.MapperScannerConfigurer">
37         <property name="basePackage" value="com.xxx.mapper" />
38         <property name="sqlSessionFactoryBeanName" value="xxxSqlSessionFactory" />
39     </bean>
40     
41     <!-- 配置velocity -->
42     <bean id="velocityConfigurer" class="org.springframework.web.servlet.view.velocity.VelocityConfigurer">
43         <property name="resourceLoaderPath">
44             <value>WEB-INF/templates/</value>
45         </property>
46         <property name="velocityProperties">
47             <props>
48                 <prop key="input.encoding">UTF-8</prop>
49                 <prop key="output.encoding">UTF-8</prop>
50             </props>
51         </property>
52     </bean>
53     <bean id="viewResolver" class="org.springframework.web.servlet.view.velocity.VelocityViewResolver"> 
54         <property name="suffix" value=".vm" /> 
55         <property name="contentType" value="text/html;charset=utf-8" />  
56         <property name="dateToolAttribute" value="date"/>
57         <property name="numberToolAttribute" value="number"/>
58     </bean>
59 </beans>

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

在读取数据源的时候，用一个jdbc.properties文件来存放所有的信息。

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

1 jdbc.driverClassName = com.mysql.jdbc.Driver
2 jdbc.url = jdbc:mysql://localhost:3306/blog?zeroDateTimeBehavior=convertToNull&useUnicode=true&characterEncoding=utf-8
3 jdbc.username = root
4 jdbc.password = 123456
```

注意：上述属性文件中有一些细节，之后会讲。

**2.2.3、配置web.xml文件**

这里直接给出web.xml的完整版，包括配置DispatcherServlet、CharacterEncodingFilter与欢迎页面。

**注意：不需要配置spring的ContextLoaderListener了。**

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 <?xml version="1.0" encoding="utf-8"?>
 2 <web-app version="2.5" xmlns="http://java.sun.com/xml/ns/javaee"
 3     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 4     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
 5 
 6     <servlet>
 7         <servlet-name>dispatcherServlet</servlet-name>
 8         <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
 9         <init-param>
10             <param-name>contextConfigLocation</param-name>
11             <param-value>classpath*:spring*.xml</param-value>
12         </init-param>
13         <load-on-startup>1</load-on-startup>
14     </servlet>
15     <servlet-mapping>
16         <servlet-name>dispatcherServlet</servlet-name>
17         <url-pattern>/</url-pattern>
18     </servlet-mapping>
19 
20     <filter>
21         <filter-name>encodingFilter</filter-name>
22         <filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
23         <init-param>
24             <param-name>encoding</param-name>
25             <param-value>UTF-8</param-value>
26         </init-param>
27         <init-param>
28             <param-name>forceEncoding</param-name>
29             <param-value>true</param-value>
30         </init-param>
31     </filter>
32     <filter-mapping>
33         <filter-name>encodingFilter</filter-name>
34         <url-pattern>/*</url-pattern>
35     </filter-mapping>
36 
37     <welcome-file-list>
38         <welcome-file>/index.jsp</welcome-file>
39     </welcome-file-list>
40 </web-app>

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



**2.3、引入mybatis**

引入jar在pom.xml中搞定了；配置mybatis在spring.xml中搞定了。

注意：这里对于mybatis的配置并不完全，因为这里我只用到了mybatis的注解，并没有用到xml形式，关于注解与xml的使用场景以及mybatis的一些其他方面的知识，以后再做记录。



**2.4、引入velocity**

引入jar在pom.xml中搞定了；配置mybatis在spring.xml中搞定了。具体的配置信息在以后会做记录。



**2.5、测试**

在以上程序搭建完成后，形成下边的结构（自己注意看上述所述的各个文件的位置所在）

![](http://images2015.cnblogs.com/blog/866881/201601/866881-20160103172018589-1370516145.jpg)

然后进行测试：

测试的内容包含两部分逻辑
- 注册：注册成功后返回true，注册失败后返回false（注意：在下边的controller中，这一块json的返回会在之后详细讲解）
- 登录：登录成功后跳到userinfo.vm文件，并输出登录的用户信息；登录失败后跳到error.vm文件，并输出相关的错误提示信息。

数据库表：

![](http://images2015.cnblogs.com/blog/866881/201601/866881-20160103173736979-510975171.jpg)

1）编写后台

后台代码结构如图

![](http://images2015.cnblogs.com/blog/866881/201601/866881-20160103172632698-1032590001.jpg)

UserController：

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 package com.xxx.web;
 2 
 3 import org.springframework.beans.factory.annotation.Autowired;
 4 import org.springframework.stereotype.Controller;
 5 import org.springframework.web.bind.annotation.RequestMapping;
 6 import org.springframework.web.bind.annotation.RequestParam;
 7 import org.springframework.web.bind.annotation.ResponseBody;
 8 import org.springframework.web.servlet.ModelAndView;
 9 
10 import com.xxx.model.User;
11 import com.xxx.service.UserService;
12 
13 @Controller
14 @RequestMapping("user")
15 public class UserController {
16     
17     @Autowired
18     private UserService userService;
19     
20     @ResponseBody
21     @RequestMapping("register")
22     public boolean register(@RequestParam("username") String username,
23                             @RequestParam("password") String password){
24         User user = new User();
25         user.setUsername(username);
26         user.setPassword(password);
27         
28         boolean isRegisterSuccess = userService.register(user);
29         
30         return isRegisterSuccess;
31     }
32     
33     @RequestMapping("login")
34     public ModelAndView login(@RequestParam("username") String username,
35                               @RequestParam("password") String password){
36         User user = userService.login(username, password);
37         
38         ModelAndView modelAndView = new ModelAndView();
39         if(user == null){
40             modelAndView.addObject("message", "用户不存在或者密码错误！请重新输入");
41             modelAndView.setViewName("error");
42         }else{
43             modelAndView.addObject("user", user);
44             modelAndView.setViewName("userinfo");
45         }
46         
47         return modelAndView;
48     }
49 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

UserService：

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 package com.xxx.service;
 2 
 3 import org.springframework.beans.factory.annotation.Autowired;
 4 import org.springframework.stereotype.Service;
 5 
 6 import com.xxx.dao.UserDAO;
 7 import com.xxx.model.User;
 8 
 9 @Service
10 public class UserService {
11     
12     @Autowired
13     private UserDAO userDao;
14     
15     public boolean register(User user){
16         return userDao.register(user);
17     }
18     
19     public User login(String username, String password) {
20         return userDao.login(username, password);
21     }
22 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

UserDAO：

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 package com.xxx.dao;
 2 
 3 import org.springframework.beans.factory.annotation.Autowired;
 4 import org.springframework.stereotype.Repository;
 5 
 6 import com.xxx.mapper.UserMapper;
 7 import com.xxx.model.User;
 8 
 9 @Repository
10 public class UserDAO {
11     
12     @Autowired
13     private UserMapper userMapper;
14     
15     public boolean register(User user){
16         return userMapper.insertUser(user)==1?true:false;
17     }
18     
19     public User login(String username ,String password){
20         return userMapper.selectByUsernameAndPwd(username, password);
21     }
22 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

UserMapper：

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 package com.xxx.mapper;
 2 
 3 import org.apache.ibatis.annotations.Insert;
 4 import org.apache.ibatis.annotations.Param;
 5 import org.apache.ibatis.annotations.Result;
 6 import org.apache.ibatis.annotations.Results;
 7 import org.apache.ibatis.annotations.Select;
 8 
 9 import com.xxx.model.User;
10 
11 public interface UserMapper {
12     
13     @Insert("INSERT INTO userinfo(username, password) VALUES(#{username},#{password})")
14     public int insertUser(User user);
15     
16     @Select("SELECT * FROM userinfo WHERE username = #{username} AND password = #{password}")
17     @Results(value = { @Result(id = true, column = "id", property = "id"),
18                        @Result(column = "username", property = "username"), 
19                        @Result(column = "password", property = "password")})
20     public User selectByUsernameAndPwd(@Param("username")String username ,@Param("password")String password);
21 }

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

具体逻辑自己看吧，很简单的。

2）编写前台

error.vm

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 <!DOCTYPE html>
 2 <html lang="zh-cn">
 3 <head>
 4     <meta charset="UTF-8">
 5     <title>登录失败</title>
 6 </head>
 7 <body>
 8     <div>
 9         $message
10     </div>
11 </body>
12 </html>

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

userinfo.vm

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)


![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 <!DOCTYPE html>
 2 <html lang="zh-cn">
 3 <head>
 4     <meta charset="UTF-8">
 5     <title>登录成功</title>
 6 </head>
 7 <body>
 8     <div>
 9         id:$user.id
10         username:$user.username
11         password:$user.password
12     </div>
13 </body>
14 </html>

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

velocity的语法自己查看相关的文档吧。

最后，通过浏览器输入带参数URL的形式对程序进行测试。

整个整合过程就是这样的。




**另外还有一篇比较经典的示例：**[Spring MVC+Mybatis+Maven+Velocity+Mysql整合实例](http://blog.csdn.net/a123demi/article/details/43156479)





