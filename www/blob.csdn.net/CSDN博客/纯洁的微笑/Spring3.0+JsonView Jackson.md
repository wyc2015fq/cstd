# Spring3.0+JsonView/Jackson - 纯洁的微笑 - CSDN博客
2013年10月25日 11:20:05[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：6877
  一、web.xml 配置：
```
<!DOCTYPE web-app PUBLIC
 "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN"
 "http://java.sun.com/dtd/web-app_2_3.dtd" >
<web-app>
	<display-name>Archetype Created Web Application</display-name>
     
    <context-param>
	    <param-name>contextConfigLocation</param-name>
	    <param-value>
	    	classpath*:spring/*.xml
	    </param-value>
    </context-param>
     <!-- Filter encoding -->
	<filter>
		<filter-name>CharacterEncodingFilter</filter-name>
		<filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class>
		<init-param>
			<param-name>encoding</param-name>
			<param-value>UTF-8</param-value>
		</init-param>
	</filter>
     <filter-mapping>
		<filter-name>CharacterEncodingFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
    <listener>
	    <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>
  
	
   	<servlet>
	<servlet-name>spring</servlet-name>
	<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
	<init-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>
			classpath*:spring/spring-p2p-console.xml
		</param-value>
	</init-param>
	<load-on-startup>1</load-on-startup>
	</servlet>
	<servlet-mapping>
		<servlet-name>spring</servlet-name>
		<url-pattern>*.con</url-pattern>
	</servlet-mapping>
	
</web-app>
```
二、spring-servlet.xml 配置
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:context="http://www.springframework.org/schema/context"
	xmlns:mvc="http://www.springframework.org/schema/mvc" 
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns:tx="http://www.springframework.org/schema/tx"
	xsi:schemaLocation="
        http://www.springframework.org/schema/beans     
        http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
        http://www.springframework.org/schema/context 
        http://www.springframework.org/schema/context/spring-context-3.0.xsd
        http://www.springframework.org/schema/mvc
        http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd
        http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.0.xsd">
        
  <context:annotation-config />
	<tx:annotation-driven transaction-manager="transactionManager"/>
	<context:component-scan base-package="com.icardpay.business.p2p.console.controller" /> 
	
	<bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource"
		destroy-method="close">
		<property name="driverClassName" value="oracle.jdbc.driver.OracleDriver" />
		<property name="url" value="jdbc:oracle:thin:@192.168.1.8:1521:ORCL" />
		<property name="username" value="root" />
		<property name="password" value="root" />
	</bean>
	
	<bean id="propertyConfigurer" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer">
        <property name="locations">
            <list>
                <value>classpath:project.properties</value>
            </list>
        </property>
    </bean>
    
 <!--     <bean class="org.springframework.web.servlet.view.BeanNameViewResolver">
        <property name="order" value="1"/>
    </bean>
    
    <bean name="jsonView" class="org.springframework.web.servlet.view.json.JsonView">
		<property name="jsonWriter">
			<ref bean="jsonWriter" />
		</property>
	</bean>
	<bean name="jsonWriter" class="org.springframework.web.servlet.view.json.writer.sojo.SojoJsonStringWriter">
		<property name="convertAllMapValues">
			<value>true</value>
		</property>
	</bean>  -->//注释掉为JsonView
	
	<bean class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter">      
        <property name="messageConverters">      
            <list >      
                <ref bean="mappingJacksonHttpMessageConverter" />      
            </list>      
        </property>      
    </bean> 
    <bean id="mappingJacksonHttpMessageConverter" class="org.springframework.http.converter.json.MappingJacksonHttpMessageConverter">    
	    <property name="supportedMediaTypes">    
	        <list>    
	            <value>application/json;charset=UTF-8</value>    
	       </list>    
	    </property>  
	</bean>  
    
</beans>
```
三、pom.xml
```
<!-- json -->
		<dependency>
			<groupId>org.codehaus.jackson</groupId>
			<artifactId>jackson-core-lgpl</artifactId>
		</dependency>
		<dependency>
			<groupId>org.codehaus.jackson</groupId>
			<artifactId>jackson-mapper-lgpl</artifactId>
		</dependency>
```

四、Controller代码
```java
@Controller
@RequestMapping("/xxx")
public class aaaaaController {
	
	private static Logger logger = LoggerFactory.getLogger(aaaaaController.class);
	@Resource
	private ffffBiz xxxxeeee;
	
    @RequestMapping("/test_json")
    @ResponseBody  
    public Map<String,Object>  testJson3(Param p){
    	System.out.println("enter....");
    	System.out.println("value....=========="+p.getId());
    	 Map<String,Object> map = new HashMap<String,Object>();  
         map.put("code", true);  
    	return map;
    }
	
}
```

