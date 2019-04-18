# spring security 示例 - z69183787的专栏 - CSDN博客
2014年08月13日 14:48:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2786
个人分类：[安全框架-SpringSecurity](https://blog.csdn.net/z69183787/article/category/2175245)
jar包：pom.xml ：
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
	<modelVersion>4.0.0</modelVersion>
	<groupId>org.zhoushun</groupId>
	<artifactId>mvc-parent</artifactId>
	<packaging>war</packaging>
	<version>0.0.1-SNAPSHOT</version>
	<name>mvc-parent Maven Webapp</name>
	<url>http://maven.apache.org</url>
	<!-- 定义相关属性 <properties> <jdk-version>1.6</jdk-version> <junit-version>4.11</junit-version> 
		<spring-version>3.2.2.RELEASE</spring-version> </properties> -->
	<dependencies>
		<dependency>
			<groupId>junit</groupId>
			<artifactId>junit</artifactId>
			<version>3.8.1</version>
			<scope>test</scope>
		</dependency>
		<!-- oracle driver -->
		<dependency>
			<groupId>com.oracle</groupId>
			<artifactId>ojdbc14</artifactId>
			<version>10.2.0.4.0</version>
		</dependency>
		<!-- oracle driver -->
		<!-- slf4j -->
		<dependency>
			<groupId>org.slf4j</groupId>
			<artifactId>slf4j-api</artifactId>
			<version>1.7.7</version>
		</dependency>
		<!-- slf4j -->
		<!-- logback -->
		<dependency>
			<groupId>ch.qos.logback</groupId>
			<artifactId>logback-core</artifactId>
			<version>1.1.2</version>
		</dependency>
		<dependency>
			<groupId>ch.qos.logback</groupId>
			<artifactId>logback-classic</artifactId>
			<version>1.1.2</version>
		</dependency>
		<!-- logback -->
		<!-- log4j <dependency> <groupId>log4j</groupId> <artifactId>log4j</artifactId> 
			<version>1.2.17</version> </dependency> log4j -->
		<!-- jstl -->
		<dependency>
			<groupId>jstl</groupId>
			<artifactId>jstl</artifactId>
			<version>1.2</version>
		</dependency>
		<!-- jstl -->
		<!-- servlet api -->
		<dependency>
			<groupId>org.mortbay.jetty</groupId>
			<artifactId>servlet-api</artifactId>
			<version>3.0.20100224</version>
			<scope>provided</scope>
		</dependency>
		<!-- servlet api -->
		<!-- spring -->
		<dependency>
			<groupId>javax.inject</groupId>
			<artifactId>javax.inject</artifactId>
			<version>1</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-context</artifactId>
			<version>3.2.9.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-core</artifactId>
			<version>3.2.9.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-web</artifactId>
			<version>3.2.9.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-webmvc</artifactId>
			<version>3.2.9.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-aop</artifactId>
			<version>3.2.9.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-orm</artifactId>
			<version>3.2.9.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-jdbc</artifactId>
			<version>3.2.9.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework</groupId>
			<artifactId>spring-tx</artifactId>
			<version>3.2.9.RELEASE</version>
		</dependency>
		<!-- spring -->
		<!-- spring jpa -->
		<dependency>
			<groupId>org.springframework.data</groupId>
			<artifactId>spring-data-jpa</artifactId>
			<version>1.6.0.RELEASE</version>
		</dependency>
		<!-- spring jpa -->
		<!--spring - security -->
		<dependency>
			<groupId>org.springframework.security</groupId>
			<artifactId>spring-security-core</artifactId>
			<version>3.2.4.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework.security</groupId>
			<artifactId>spring-security-config</artifactId>
			<version>3.2.4.RELEASE</version>
		</dependency>
		<dependency>
			<groupId>org.springframework.security</groupId>
			<artifactId>spring-security-web</artifactId>
			<version>3.2.4.RELEASE</version>
		</dependency>
		<!--spring - security -->
		<!-- -->
		<!-- jackson 2.0 -->
		<dependency>
			<groupId>com.fasterxml.jackson.core</groupId>
			<artifactId>jackson-databind</artifactId>
			<version>2.4.0</version>
		</dependency>
		<dependency>
			<groupId>com.fasterxml.jackson.datatype</groupId>
			<artifactId>jackson-datatype-hibernate4</artifactId>
			<version>2.4.0</version>
		</dependency>
		<!-- jackson 2.0 -->
		<!-- common -->
		<dependency>
			<groupId>commons-io</groupId>
			<artifactId>commons-io</artifactId>
			<version>2.4</version>
		</dependency>
		<dependency>
			<groupId>commons-fileupload</groupId>
			<artifactId>commons-fileupload</artifactId>
			<version>1.3</version>
		</dependency>
		<dependency>
			<groupId>commons-pool</groupId>
			<artifactId>commons-pool</artifactId>
			<version>1.6</version>
		</dependency>
		<dependency>
			<groupId>org.apache.commons</groupId>
			<artifactId>commons-lang3</artifactId>
			<version>3.3.2</version>
		</dependency>
		<dependency>
			<groupId>commons-beanutils</groupId>
			<artifactId>commons-beanutils</artifactId>
			<version>1.9.2</version>
		</dependency>
		<!-- common 工具类 -->
		<!-- common-dbcp -->
		<dependency>
			<groupId>commons-dbcp</groupId>
			<artifactId>commons-dbcp</artifactId>
			<version>1.4</version>
		</dependency>
		<!-- common-dbcp -->
		<!-- hibernate jpa 2.0 -->
		<dependency>
			<groupId>org.hibernate.javax.persistence</groupId>
			<artifactId>hibernate-jpa-2.1-api</artifactId>
			<version>1.0.0.Final</version>
		</dependency>
		<!-- hibernate jpa 2.0 -->
		<!-- hibernate -->
		<dependency>
			<groupId>org.hibernate</groupId>
			<artifactId>hibernate-core</artifactId>
			<version>4.3.5.Final</version>
		</dependency>
		<dependency>
			<groupId>org.hibernate</groupId>
			<artifactId>hibernate-entitymanager</artifactId>
			<version>4.3.5.Final</version>
		</dependency>
		<dependency>
			<groupId>org.hibernate</groupId>
			<artifactId>hibernate-validator</artifactId>
			<version>5.1.1.Final</version>
		</dependency>
		<!-- hibernate -->
	</dependencies>
	<build>
		<finalName>mvc-parent</finalName>
	</build>
</project>
```
web.xml：
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
xmlns="http://java.sun.com/xml/ns/javaee" 
xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd" 
id="WebApp_ID" version="3.0">
 
  <display-name>Archetype Created Web Application</display-name>
  
  
  <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>
<listener> 
	     <listener-class>org.springframework.web.context.request.RequestContextListener</listener-class> 
	</listener> 
	<context-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>classpath:applicationContext*.xml</param-value>
	</context-param>
	
   	<!-- The front controller of this Spring Web application, responsible for handling all application requests -->
	<servlet>
		<servlet-name>springDispatcherServlet</servlet-name>
		<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
		<init-param>
			<param-name>contextConfigLocation</param-name>
			<param-value>classpath:springmvc-servlet.xml</param-value>
		</init-param>
		<load-on-startup>1</load-on-startup>
	</servlet>
	<!-- Map all requests to the DispatcherServlet for handling -->
	<servlet-mapping>
		<servlet-name>springDispatcherServlet</servlet-name>
		<url-pattern>/</url-pattern>
	</servlet-mapping> 
    <welcome-file-list>
        <welcome-file>index.jsp</welcome-file>
    </welcome-file-list>
  
  
  <!-- spring security 	-->
  <filter>
		<filter-name>springSecurityFilterChain</filter-name>
		<filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
	</filter>
  <filter-mapping>
		<filter-name>springSecurityFilterChain</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
  	<filter>
		<filter-name>characterEncodingFilter</filter-name>
		<filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
		<init-param>
			<param-name>encoding</param-name>
			<param-value>UTF-8</param-value>
		</init-param>
	</filter>
	<filter-mapping>
		<filter-name>characterEncodingFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
</web-app>
```
applicationContext-security.xml：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans:beans xmlns="http://www.springframework.org/schema/security"
	xmlns:beans="http://www.springframework.org/schema/beans" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.2.xsd
                        http://www.springframework.org/schema/security http://www.springframework.org/schema/security/spring-security-3.2.xsd">
	<global-method-security pre-post-annotations="enabled" />
	<http pattern="/js/**" security="none"/>
	<http pattern="/images/**" security="none" />
	<http pattern="/widgets/**" security="none" />
	<http pattern="/css/**" security="none" />
	<http pattern="/style/**" security="none" />
	<http pattern="**/*.js" security="none" />
	<http pattern="**/*.swf" security="none" />
	<http pattern="/jsp/login.jsp" security="none" />
	<http pattern="/jsp/403.html" security="none" />
	<http pattern="/jsp/session-timeout.jsp" security="none" />
	<http use-expressions="true" auto-config="true"
		access-denied-page="/jsp/403.html"><!-- 当访问被拒绝时，会转到403.jsp -->
		<intercept-url pattern="/**" access="isAuthenticated()" />
		<form-login login-page='/jsp/login.jsp'
			authentication-success-handler-ref="authSuccessHandler"
			authentication-failure-handler-ref="authFailureHandler" />
		<!-- default-target-url="/secure/index"/> -->
		<logout logout-success-url="/login.jsp" delete-cookies="JSESSIONID" />
		<remember-me />
		<session-management invalid-session-url="/timeout.jsp">
			<!-- <concurrency-control max-sessions="10" error-if-maximum-exceeded="true" 
				/> -->
		</session-management>
	</http>
	<authentication-manager alias="authenticationManager">
		<authentication-provider user-service-ref="userService">
			<password-encoder hash="md5" />
		</authentication-provider>
	</authentication-manager>
</beans:beans>
```
userservice（重写loadUserByUsername方法，登陆后首先进入该方法，
返回的UserDetails将会与登陆界面中获取的用户名及密码锁构成的对象相比较，进而判断登陆成功与否。
若成功登陆：authSuccessHandle；若登陆失败：authFailureHandle；）
```java
package test.service;
import java.util.Collections;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.web.authentication.UsernamePasswordAuthenticationFilter;
import org.springframework.stereotype.Service;
import org.springframework.util.DigestUtils;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletWebRequest;
import org.springframework.web.context.request.ServletRequestAttributes;
@Service
public class UserService implements UserDetailsService {
	@Autowired
	private HttpServletRequest request;
	
	public UserDetails loadUserByUsername(String arg0){
		//HttpServletRequest request = ((ServletRequestAttributes) RequestContextHolder.getRequestAttributes()).getRequest();
		
		if("zhoushun".equals(arg0)){
			String pwd = "";
			try {
				pwd = request.getParameter(UsernamePasswordAuthenticationFilter.SPRING_SECURITY_FORM_PASSWORD_KEY);
			} catch (Exception e) {
				e.printStackTrace();
			}
			System.out.println(pwd);
			
		}
		org.springframework.security.core.userdetails.User tmpUser = 
		new org.springframework.security.core.userdetails.User(arg0, "4f3edd6c1dcd46df67f19b91f977854b", Collections.EMPTY_SET);
		return tmpUser;
	}
	
	public static void main(String[] args){
		System.out.println(DigestUtils.md5DigestAsHex(("zhoushun").getBytes()));
	}
}
```
authSuccessHandler：
```java
package test.service;
import java.io.IOException;
import javax.inject.Named;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.security.core.Authentication;
import org.springframework.security.web.authentication.AuthenticationSuccessHandler;
@Named
public class AuthSuccessHandler implements AuthenticationSuccessHandler {
	public void onAuthenticationSuccess(HttpServletRequest request,
			HttpServletResponse response, Authentication authentication) throws IOException {
			
//		response.setContentType("text/json; charset=UTF-8");
//		response.getWriter().print("{success: true}");
		 response.sendRedirect(request.getContextPath() + "/test");
	}
}
```
authFailureHandler：
```java
package test.service;
import java.io.IOException;
import javax.inject.Named;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.springframework.security.core.AuthenticationException;
import org.springframework.security.web.authentication.AuthenticationFailureHandler;
@Named
public class AuthFailureHandler implements AuthenticationFailureHandler {
	public void onAuthenticationFailure(HttpServletRequest request,
			HttpServletResponse response, AuthenticationException exception) throws IOException {
//		response.setContentType("text/json; charset=UTF-8");
//		response.getWriter().print("{success: false}");
		 response.sendRedirect(request.getContextPath() + "/jsp/login.jsp?login_error=1");
	}
}
```
