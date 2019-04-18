# idea+maven + spring security +springmvc入门 （自定义登录页面），附idea如何创建web项目 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年06月24日 21:50:06[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：681


第一次使用idea，上午在eclipse中 学习了spring security 入门，下午试试在idea中搭建。

刚开始 我以为 直接将eclipse的 文件 copy过来就行了，结果发现copy过来以后 各种报错。

后来把maven本地仓库jar包都删了，并在setting文件中 配置了阿里云镜像，就好了。

然后就是在 eclipse中运行正常的 springmvc.xml,头信息开始报错。找了很久头信息，最终终于找到可以使用的版本。

如果你想看eclipse版本的 spring security +springmvc入门 

请看我上一篇文章     [点击打开链接](https://blog.csdn.net/weixin_40247263/article/details/80793208)

一：点击Create New Project

![](https://img-blog.csdn.net/20180624193355457?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

二：点Maven 选 JDK ，勾Create from archetype ，在下面选 webapp结尾的。

![](https://img-blog.csdn.net/20180624194246338?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

三：输入GroupId和ArtifactId

GroupId是项目组名

ArtifactId是项目名

![](https://img-blog.csdn.net/20180624194435752?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

四：选maven位置，setting文件位置，maven本地仓库位置。

注意：如果不用默认的setting文件位置和maven本地仓库位置，必须要勾选上override。

setting里面最好配置上阿里云的镜像，这样下载jar会比较快，官方的maven中心仓库下载的慢，容易jar包下残导致不能使用。

如果你以前用过eclipse，本地仓库自己最好再建一个文件夹，别和eclipse共用一个本地仓库。我怕冲突。

![](https://img-blog.csdn.net/20180624195342274?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180624195154849?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
<mirror>
      <id>alimaven</id>
      <name>aliyun maven</name>
      <url>http://maven.aliyun.com/nexus/content/groups/public/</url>
      <mirrorOf>central</mirrorOf>        
    </mirror>
```

五：起项目名和项目位置，不用改

![](https://img-blog.csdn.net/20180624195424161?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

六：进入以后如下图所示，如果你没有这些工具栏，可以点击View，然后开启，同时看一下项目结构

![](https://img-blog.csdn.net/20180624195549790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180624195657836?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

七：建文件夹   -- 右键 main -->new -->directory-->起名 为 java

![](https://img-blog.csdn.net/20180624200126780?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后 右键 java --Mark Directory As  --Source Root   (意思就是你的java代码在这下面写) 点完以后java文件夹变蓝色了。

![](https://img-blog.csdn.net/20180624200231863?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后右键java -- new --package    起名 如com.zzh.controller  

然后再 右键 main  创建个 resource文件夹。 右键resource这个 文件夹，还是Makr Directory as  --> Resources Root

![](https://img-blog.csdn.net/20180624200944542?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最终项目结构 如下

![](https://img-blog.csdn.net/20180624201505714?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

八：配置tomcat

![](https://img-blog.csdn.net/20180624201701289?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点进去以后 左上角 有个绿色的加号，然后点最下面那个，将列表显示完全，然后选择tomcatserver - local（如果没有tomcat，需要先安装tomcat插件，这个自己上网搜吧，我懒得写了）

![](https://img-blog.csdn.net/20180624201751576?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180624202157163?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

准备工作完成， 下面进入spring service和 spring mvc 部分。

首先按照我的项目结构，去添加各种文件，下面列出文件内容。

pom.xml

```
<?xml version="1.0" encoding="UTF-8"?>

<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
  <modelVersion>4.0.0</modelVersion>

  <groupId>com.zzh</groupId>
  <artifactId>ssm</artifactId>
  <version>1.0-SNAPSHOT</version>
  <packaging>war</packaging>

  <name>ssm Maven Webapp</name>
  <!-- FIXME change it to the project's website -->
  <url>http://www.example.com</url>

  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <maven.compiler.source>1.7</maven.compiler.source>
    <maven.compiler.target>1.7</maven.compiler.target>
    <jdk.version>1.6</jdk.version>
    <spring.version>4.1.6.RELEASE</spring.version>
    <spring.security.version>3.2.3.RELEASE</spring.security.version>
    <jstl.version>1.2</jstl.version>
  </properties>

  <dependencies>

    <!-- Spring dependencies -->
    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-context</artifactId>
      <version>${spring.version}</version>
    </dependency>
    <dependency>
      <groupId>org.springframework</groupId>
      <artifactId>spring-webmvc</artifactId>
      <version>${spring.version}</version>
    </dependency>

    <!-- Spring Security -->
    <dependency>
      <groupId>org.springframework.security</groupId>
      <artifactId>spring-security-web</artifactId>
      <version>${spring.security.version}</version>
    </dependency>

    <dependency>
      <groupId>org.springframework.security</groupId>
      <artifactId>spring-security-config</artifactId>
      <version>${spring.security.version}</version>
    </dependency>

    <!-- jstl for jsp page -->
    <dependency>
      <groupId>jstl</groupId>
      <artifactId>jstl</artifactId>
      <version>${jstl.version}</version>
    </dependency>

  </dependencies>

  <build>
    <finalName>ssm</finalName>
    <pluginManagement><!-- lock down plugins versions to avoid using Maven defaults (may be moved to parent pom) -->
      <plugins>
        <plugin>
          <artifactId>maven-clean-plugin</artifactId>
          <version>3.0.0</version>
        </plugin>
        <!-- see http://maven.apache.org/ref/current/maven-core/default-bindings.html#Plugin_bindings_for_war_packaging -->
        <plugin>
          <artifactId>maven-resources-plugin</artifactId>
          <version>3.0.2</version>
        </plugin>
        <plugin>
          <artifactId>maven-compiler-plugin</artifactId>
          <version>3.7.0</version>
        </plugin>
        <plugin>
          <artifactId>maven-surefire-plugin</artifactId>
          <version>2.20.1</version>
        </plugin>
        <plugin>
          <artifactId>maven-war-plugin</artifactId>
          <version>3.2.0</version>
        </plugin>
        <plugin>
          <artifactId>maven-install-plugin</artifactId>
          <version>2.5.2</version>
        </plugin>
        <plugin>
          <artifactId>maven-deploy-plugin</artifactId>
          <version>2.8.2</version>
        </plugin>
      </plugins>
    </pluginManagement>
  </build>
</project>
```

web.xml

```java
<!DOCTYPE web-app PUBLIC
 "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN"
 "http://java.sun.com/dtd/web-app_2_3.dtd" >

<web-app xmlns="http://xmlns.jcp.org/xml/ns/javaee"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaee http://xmlns.jcp.org/xml/ns/javaee/web-app_3_1.xsd"
         version="3.1">
  <!-- Spring MVC -->
  <servlet>
    <servlet-name>mvc-dispatcher</servlet-name>
    <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
    <init-param>
      <param-name>contextConfigLocation</param-name>
      <param-value>classpath:springmvc.xml</param-value>
    </init-param>
    <load-on-startup>1</load-on-startup>
  </servlet>
  <servlet-mapping>
    <servlet-name>mvc-dispatcher</servlet-name>
    <url-pattern>/</url-pattern>
  </servlet-mapping>

  <listener>
    <listener-class>org.springframework.web.context.ContextLoaderListener
    </listener-class>
  </listener>

  <!-- Loads Spring Security config file -->
  <context-param>
    <param-name>contextConfigLocation</param-name>
    <param-value>
      classpath:spring-security.xml
    </param-value>
  </context-param>

  <!-- Spring Security -->
  <filter>
    <filter-name>springSecurityFilterChain</filter-name>
    <filter-class>org.springframework.web.filter.DelegatingFilterProxy
    </filter-class>
  </filter>

  <filter-mapping>
    <filter-name>springSecurityFilterChain</filter-name>
    <url-pattern>/*</url-pattern>
  </filter-mapping>
</web-app>
```

springmvc.xml

```
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context.xsd">

    <context:component-scan base-package="com.zzh.controllor"></context:component-scan>
    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"></property>
        <property name="prefix" value="/WEB-INF/pages/"></property>
        <property name="suffix" value=".jsp"></property>
    </bean>

</beans>
```

spring-security.xml

```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	   xmlns:sec="http://www.springframework.org/schema/security"
	   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	   xsi:schemaLocation="http://www.springframework.org/schema/beans
        http://www.springframework.org/schema/beans/spring-beans-4.0.xsd  
        http://www.springframework.org/schema/security  
        http://www.springframework.org/schema/security/spring-security-3.2.xsd">

    <!-- 不拦截的页面，我下面这行在我这个案例中没意义，可以设置一些静态资源 或 下面配置拦截所有请求的时候，选择不拦截登录页面 -->  
    <sec:http pattern="/login.jsp" security="none"></sec:http>  
    
	<sec:http auto-config="false">
		<!-- 对/admin** 的url进行拦截，只有拥有ROLE_USER权限的用户才能进入 -->
		<sec:intercept-url pattern="/admin**" access="ROLE_USER" />
		<!-- 
			login-page="/login.jsp" 拦截以后重定向到自定义的登录页面
			authentication-failure-url="/login.jsp"登录的用户没有 ROLE_USER权限跳转的页面
			default-target-url="/admin"  登录的用户有  ROLE_USER权限跳转的页面
		-->
		<sec:form-login login-page="/login.jsp" authentication-failure-url="/login.jsp"  
        default-target-url="/admin" />  
        <!-- 
        		点击注销  跳转的页面
       			与 <a href="<c:url value="/logout" />" > 注销</a> 配合使用
         -->
	    <sec:logout logout-url="/logout" logout-success-url="/login.jsp" invalidate-session="true"/>
	</sec:http>

	<sec:authentication-manager>
	  <sec:authentication-provider>
	    <sec:user-service>
	    <!-- 给root 用户 配置 "ROLE_USER" 权限 -->
		<sec:user name="root" password="123456" authorities="ROLE_USER" />
	    </sec:user-service>
	  </sec:authentication-provider>
	</sec:authentication-manager>

</beans>
```

HelloController.java

```java
package com.zzh.controllor;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.servlet.ModelAndView;

@Controller
public class HelloController {

	@RequestMapping(value = { "/", "/welcome**" }, method = RequestMethod.GET)
	public ModelAndView welcomePage() {

		ModelAndView model = new ModelAndView();
		model.addObject("title", "Spring Security Hello World");
		model.addObject("message", "This is welcome page!");
		model.setViewName("hello");
		return model;

	}

	@RequestMapping(value = "/admin**", method = RequestMethod.GET)
	public ModelAndView adminPage() {

		ModelAndView model = new ModelAndView();
		model.addObject("title", "Spring Security Hello World");
		model.addObject("message", "This is protected page!");
		model.setViewName("admin");

		return model;

	}

}
```

admin.jsp

```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" session="true"%>
<%@taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core"%>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
	<h1>标题: ${title}</h1>
	<h1>消息 : ${message}</h1>

	<c:if test="${pageContext.request.userPrincipal.name != null}">
	   <h2>欢迎: ${pageContext.request.userPrincipal.name} 
           | <a href="<c:url value="/logout" />" > 注销</a></h2>  
	</c:if>
</body>
</html>
```

hello.jsp

```java
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8" session="false"%>

<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
	<h1>标题: ${title}</h1>	
	<h1>消息 : ${message}</h1>	
</body>
</html>
```

login.jsp

```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
       <form action="j_spring_security_check" method="post">

                <input type="text" name="j_username"
                       placeholder=用户名 maxlength="11"  style="border-radius: 4px; outline:none;
                           ;width: 300px;height: 35px;text-indent: 10px"/><br/>
                <input type="password" name="j_password"
                       placeholder=密码 maxlength="11"  style="border-radius: 4px; outline:none;
                           ;width: 300px;height: 35px;text-indent: 10px"/><br/>           

                <input type="submit" 
                       style="width: 305px;height: 45px;background-color: orange" value="登录"
                       onmousemove="this.style.backgroundColor='darkorange';"
                       onmouseout="this.style.backgroundColor='orange   ';"/>

        </form>
</body>
</html>
```

index.jsp

```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
</head>
<body>
<h1>index</h1>
</body>
</html>
```

以上为 文件 内容。 

![](https://img-blog.csdn.net/20180624203034391?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面 开始配置项目信息

![](https://img-blog.csdn.net/20180624203132668?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

选择 Modules  ，   Project不用管  。 选择Dependencies 添加Tomcat Library

![](https://img-blog.csdn.net/20180624213523649?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

同时检查 文件类型 以及 Artifacts是否已经自动配置好

![](https://img-blog.csdn.net/20180624213724406?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018062421384672?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

出现如上2图所示就 没问题了。

最后 在tomcat中部署项目。 选择文件夹形式发布 即 exploded 结尾的。

![](https://img-blog.csdn.net/20180624214049526?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

运行项目

![](https://img-blog.csdn.net/20180624214144534?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

效果 展示   index页面  

![](https://img-blog.csdn.net/20180624214224463?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

spring security 实现效果   输入/admin，自己冲定向到 登录页面

注意看 右面的network

![](https://img-blog.csdn.net/20180624214320437?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180624214449641?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

用户名密码 输入正确(有权限的用户)  会 进入 我们原本请求的 RequestMapping对应的方法

![](https://img-blog.csdn.net/20180624214632587?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击 注销 回到登录页面  

![](https://img-blog.csdn.net/20180624214701301?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


如果你 按照我的步骤 成功了，帮忙点个赞，写这么久真心累。支持一下。或者遇到什么问题，可以留言，或许可以帮你解决。 



