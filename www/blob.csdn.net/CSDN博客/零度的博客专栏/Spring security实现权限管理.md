# Spring security实现权限管理 - 零度的博客专栏 - CSDN博客
2016年04月08日 14:53:57[零度anngle](https://me.csdn.net/zmx729618)阅读数：60467
**1、配置文件**
1、POM.xml
```
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
  <modelVersion>4.0.0</modelVersion>
  <groupId>org.nercita</groupId>
  <artifactId>BCP</artifactId>
  <packaging>war</packaging>
  <version>0.0.1-SNAPSHOT</version>
  <name>BCP</name>
  <url>http://maven.apache.org</url>
  
  <properties>
    <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    <spring.version>4.0.5.RELEASE</spring.version>      
    <spring.security.version>3.2.3.RELEASE</spring.security.version>
    <hibernate.version>4.3.5.Final</hibernate.version>  
  </properties>
  <dependencies>  
    <!-- junit -->
    <dependency>
      <groupId>junit</groupId>
      <artifactId>junit</artifactId>
      <version>4.11</version>
      <scope>test</scope> 
    </dependency>
  
    <dependency>
      <groupId>javax.servlet</groupId>
      <artifactId>jstl</artifactId>
      <version>1.2</version>
      <scope>provided</scope>
    </dependency>
    <dependency>
      <groupId>javax.servlet.jsp</groupId>
      <artifactId>jsp-api</artifactId>
      <version>2.1</version>
      <scope>provided</scope>
    </dependency>
    
    <dependency>
      <groupId>org.glassfish</groupId>
      <artifactId>javax.annotation</artifactId>
      <version>3.0.1</version>
    </dependency>
    <dependency>
      <groupId>org.glassfish</groupId>
      <artifactId>javax.ejb</artifactId>
      <version>3.0.1</version>
    </dependency>
    <dependency>
      <groupId>org.jboss.weld</groupId>
      <artifactId>weld-osgi-bundle</artifactId>
      <version>1.0.1-SP3</version>
      <exclusions>             
	        <exclusion>
	            <groupId>javassist</groupId>
	            <artifactId>javassist</artifactId>   
	        </exclusion>
	   </exclusions>
    </dependency>
    <dependency>
      <groupId>org.glassfish</groupId>
      <artifactId>javax.servlet</artifactId>
      <version>3.0.1</version>
    </dependency>
    
    <!-- spring4 -->
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
	
    <!--
    <dependency>
		<groupId>org.springframework</groupId>
		<artifactId>spring-webmvc-portlet</artifactId>
		<version>${spring.version}</version>
	</dependency> 
	-->  
    
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
    
    <!-- 数据库驱动-mysql -->
    <dependency>
	      <groupId>mysql</groupId>
	      <artifactId>mysql-connector-java</artifactId>
	      <version>5.1.26</version>
    </dependency>
    
    <!-- 数据库驱动-oracle -->
	<dependency>         
		  <groupId>com.oracle</groupId>
		  <artifactId>ojdbc6</artifactId>
		  <version>12.1.0.1</version>
	</dependency>       
     
    <!-- 数据库连接池 -->
    <dependency>
		<groupId>com.mchange</groupId>
		<artifactId>c3p0</artifactId>
		<version>0.9.5</version>
	</dependency>
    
    <!-- hibernate4 核心及依赖包  -->
    <dependency>
		<groupId>org.hibernate</groupId>
		<artifactId>hibernate-core</artifactId>
		<version>${hibernate.version}</version>
		<exclusions>             
	        <exclusion>
	            <groupId>org.javassist</groupId>
	            <artifactId>javassist</artifactId>
	        </exclusion>
	        <exclusion>
            	<artifactId>c3p0</artifactId>
            	<groupId>c3p0</groupId>
        	</exclusion>
	    </exclusions>
		
    </dependency>	    
    <!-- 支持JPA规范的core门面 -->
    <dependency>
        <groupId>org.hibernate</groupId>
        <artifactId>hibernate-entitymanager</artifactId>
        <version>${hibernate.version}</version>
        <exclusions>
	        <exclusion>
	            <groupId>org.javassist</groupId>
	            <artifactId>javassist</artifactId>
	        </exclusion>
	    </exclusions>        
    </dependency>	    
    <dependency> 
        <groupId>org.hibernate</groupId>
        <artifactId>hibernate-ehcache</artifactId>
        <version>${hibernate.version}</version>
    </dependency>      
	<!-- JPA注解的实现 -->
    <!--    
    <dependency>
      <groupId>org.hibernate.javax.persistence</groupId>
      <artifactId>hibernate-jpa-2.0-api</artifactId>
      <version>1.0.1.Final</version>
	</dependency>  
	-->	
	<dependency>
	    <groupId>org.hibernate.javax.persistence</groupId>
		<artifactId>hibernate-jpa-2.1-api</artifactId>
		<version>1.0.0.Final</version>
	</dependency>  
		
	<!-- 为了让Hibernate使用代理模式，需要javassist -->
   	<dependency>
		<groupId>org.javassist</groupId>
		<artifactId>javassist</artifactId>
		<version>3.18.1-GA</version>
    </dependency>    
    
    <!-- antlr -->
    <dependency>
        <groupId>antlr</groupId>
        <artifactId>antlr</artifactId>
        <version>2.7.7</version>
    </dependency>
    
    <!-- dom4j -->
    <dependency>
        <groupId>dom4j</groupId>
        <artifactId>dom4j</artifactId>
        <version>1.6.1</version>
    </dependency>
    <!-- apache commons -->
    <dependency>
        <groupId>commons-dbcp</groupId>
        <artifactId>commons-dbcp</artifactId>
        <version>1.4</version>
    </dependency>
    <dependency>
        <groupId>commons-pool</groupId>
        <artifactId>commons-pool</artifactId>
        <version>1.4</version>
    </dependency>  
    <dependency>
        <groupId>commons-collections</groupId>
        <artifactId>commons-collections</artifactId>
        <version>3.1</version>
    </dependency>   
    <dependency>
        <groupId>javax.transaction</groupId>
        <artifactId>jta</artifactId>
        <version>1.1</version>
    </dependency>      
    <dependency>
        <groupId>commons-lang</groupId>
        <artifactId>commons-lang</artifactId>   
        <version>2.6</version>
	</dependency> 
	<dependency> 
		<groupId>commons-beanutils</groupId>
		<artifactId>commons-beanutils</artifactId>
		<version>1.9.0</version>
	</dependency> 	
	 
	<!-- pinyin4j -->
	<dependency>
		<groupId>com.belerweb</groupId>
		<artifactId>pinyin4j</artifactId>
		<version>2.5.0</version>
   </dependency>
	 
    <!-- 其他必选依赖包 -->
    <dependency>
	<groupId>cglib</groupId>
		<artifactId>cglib</artifactId>
		<version>3.1</version>
	</dependency>
	
	<dependency>
		<groupId>asm</groupId>
		<artifactId>asm</artifactId>
		<version>3.3.1</version>
	</dependency>	
	
	<dependency>
		<groupId>aopalliance</groupId>
		<artifactId>aopalliance</artifactId>
		<version>1.0</version>
	</dependency>
    <!--ehcache缓存  -->
    <dependency>
		<groupId>net.sf.ehcache</groupId>
		<artifactId>ehcache</artifactId>
		<version>2.8.3</version>
    </dependency>
        
    <!--其他组件  -->
    <dependency>
		<groupId>org.springframework.osgi</groupId>
		<artifactId>spring-osgi-annotation</artifactId>
		<version>1.2.1</version>
		
    </dependency>
     
    <dependency>
	    <groupId>wsdl4j</groupId>
	    <artifactId>wsdl4j</artifactId>
	    <version>1.6.3</version>
	</dependency>
	
<!----> 	<dependency>
		  <groupId>org.apache</groupId>
		  <artifactId>cxf</artifactId>
		  <version>2.3.0</version>
	</dependency> 
	
	
	
	<dependency>
		<groupId>org.sitemesh</groupId>
		<artifactId>sitemesh</artifactId>
		<version>3.0.0</version>
	</dependency> 
	
	<dependency>   
		<groupId>org.quartz-scheduler</groupId>
		<artifactId>quartz</artifactId>
		<version>2.1.7</version>
		<exclusions>             
	        <exclusion>
	            <groupId>c3p0</groupId>
	            <artifactId>c3p0</artifactId>
	        </exclusion>
	    </exclusions>
	</dependency>			
		
	<dependency>
		<groupId>net.sf.dozer</groupId>
		<artifactId>dozer</artifactId>
		<version>5.2.0</version>
	</dependency>
	  
	<dependency>
		<groupId>javax.validation</groupId>
		<artifactId>validation-api</artifactId>
		<version>1.1.0.Final</version>
	</dependency>
    <dependency>  
		<groupId>org.hibernate</groupId>
		<artifactId>hibernate-validator</artifactId>
		<version>4.3.0.Final</version>
	</dependency> 
	<dependency>
		<groupId>com.fasterxml.jackson.core</groupId>
		<artifactId>jackson-databind</artifactId>
		<version>2.5.1</version>
	</dependency>
	
	<dependency>
		<groupId>com.fasterxml</groupId>
		<artifactId>classmate</artifactId>
		<version>1.1.0</version>
	</dependency> 
	
	<dependency> 
		<groupId>com.thoughtworks.xstream</groupId>
		<artifactId>xstream</artifactId>
		<version>1.4.7</version>
	</dependency>
	
	
	<dependency>
		<groupId>com.sun.mail</groupId>
		<artifactId>javax.mail</artifactId>
		<version>1.5.2</version>
	</dependency> 
     
	
	<dependency>
		<groupId>org.apache.struts</groupId>
		<artifactId>struts2-core</artifactId>
		<version>2.3.12</version>
		<exclusions>             
	        <exclusion>
	            <groupId>javassist</groupId>
	            <artifactId>javassist</artifactId>
	        </exclusion>
	    </exclusions>
	</dependency>
	
	<dependency>
		<groupId>org.apache.struts</groupId>
		<artifactId>struts2-spring-plugin</artifactId>
		<version>2.3.12</version>
	</dependency>
	
	<dependency>
		<groupId>org.dbunit</groupId>
		<artifactId>dbunit</artifactId>
		<version>2.4.7</version>
	</dependency>
	
	<dependency> 
		<groupId>org.easymock</groupId>
		<artifactId>easymock</artifactId>
		<version>3.2</version>
	</dependency>
	
	<dependency>
		<groupId>org.apache.ws.commons.schema</groupId>
		<artifactId>XmlSchema</artifactId>
		<version>1.4.7</version>
	</dependency>
	
	<!-- httpclient -->	
	<dependency>
		<groupId>org.apache.httpcomponents</groupId>
		<artifactId>httpclient</artifactId>
		<version>4.2.1</version>
	</dependency>
	
	<!-- log4j 日志-->
    <dependency>
	      <groupId>log4j</groupId>
	      <artifactId>log4j</artifactId>
	      <version>1.2.17</version>
	</dependency>
	
    <!-- slf4j-api 日志接口 -->
    <dependency>
		<groupId>org.slf4j</groupId>
		<artifactId>slf4j-api</artifactId>
		<version>1.7.7</version>
       </dependency>
        
    <!-- slf4j-log4j12 日志接口桥接 -->
    <dependency>
		<groupId>org.slf4j</groupId>
		<artifactId>slf4j-log4j12</artifactId>
		<version>1.7.7</version>
    </dependency>
		
	<!-- slf4j-nop 日志 -->
    <dependency>
		<groupId>org.slf4j</groupId>
		<artifactId>slf4j-nop</artifactId>
		<version>1.7.7</version>
    </dependency>
    
    <!-- log4j2 日志-->
    <dependency>
		<groupId>org.apache.logging.log4j</groupId>
		<artifactId>log4j-api</artifactId>
		<version>2.1</version>
	</dependency>
	<dependency>
		<groupId>org.apache.logging.log4j</groupId>
		<artifactId>log4j-core</artifactId>
		<version>2.1</version>
	</dependency>
	   
	    
    <!-- 文件上传 -->
    <dependency>
      	 <groupId>commons-fileupload</groupId>
		 <artifactId>commons-fileupload</artifactId>
		 <version>1.3.1</version>
    </dependency>
	    
    <!-- fastJson json数组有关的 -->
    <dependency>
		<groupId>com.alibaba</groupId>
		<artifactId>fastjson</artifactId>
		<version>1.2.1</version>
	</dependency>
		
	<!-- 使用 Jackson 把 Java对象转换成 JSON字符串 -->
    <dependency>
	      <groupId>org.codehaus.jackson</groupId>
	      <artifactId>jackson-mapper-asl</artifactId>
	      <version>1.9.13</version>
    </dependency>
    
    <!-- poi -->   
    <dependency>
		<groupId>org.apache.poi</groupId>
		<artifactId>poi</artifactId>
		<version>3.13</version>
	</dependency>	
	<dependency>
		<groupId>org.apache.poi</groupId>
		<artifactId>poi-ooxml</artifactId>
		<version>3.13</version>
	</dependency>
	
   <!-- 
   <dependency>   
		<groupId>org.apache.poi</groupId>
		<artifactId>openxml4j</artifactId>
		<version>1.0-beta</version>
	</dependency> 
    -->
	
	
	<!-- jxls -->
	<dependency>
		<groupId>net.sf.jxls</groupId>
		<artifactId>jxls-core</artifactId>
		<version>1.0.6</version>
	</dependency>
	<dependency>
		<groupId>net.sf.jxls</groupId>
		<artifactId>jxls-reader</artifactId>
		<version>1.0.6</version>
	</dependency>
	
	<!-- xmlpull xml to java -->	
	<dependency>
		<groupId>xmlpull</groupId>
		<artifactId>xmlpull</artifactId>
		<version>1.1.3.4a</version>
	</dependency>
	<!-- 使用JSONObject,将接收到的结果转成JSON格式 -->
	<dependency>
		<groupId>net.sf.json-lib</groupId>
		<artifactId>json-lib</artifactId>
		<version>2.4</version>
		<classifier>jdk15</classifier><!--指定jdk版本--> 
	</dependency>
	<!-- net.sf.json-lib包依赖以下包:-->
	<dependency>
		<groupId>commons-logging</groupId>
		<artifactId>commons-logging</artifactId>
		<version>1.2</version>
	</dependency>
	<dependency>
		<groupId>net.sf.ezmorph</groupId>
		<artifactId>ezmorph</artifactId>
		<version>1.0.6</version>
	</dependency>
	
	<dependency>
	    <groupId>net.coobird</groupId>
	    <artifactId>thumbnailator</artifactId>
	    <version>0.4.7</version>
	</dependency>
	<!-- 云通讯短信发送API-->
	<dependency>
	  <groupId>com.yuntongxun</groupId>
	  <artifactId>ccp</artifactId>
	  <version>2.6.3</version>
	</dependency>
  
    
  </dependencies>
  
  
  <build>
    <finalName>BCP</finalName>
  </build>
  
</project>
```
2.web.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="3.0"  xmlns="http://java.sun.com/xml/ns/javaee" 
	     xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
	     xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd">
    
    <display-name>SSH-Application</display-name>	
    
	<!--初始化参数-->
	<context-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>/WEB-INF/classes/applicationContext*.xml</param-value>
	</context-param> 
	
	<context-param>  
	 	<param-name>javax.servlet.jsp.jstl.fmt.localizationContext</param-name>  
	 	<param-value>message/message-info</param-value>		
	</context-param>
	
	<!--Spring ContextLoaderListener -->
    <listener>
		<listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
	</listener> 
	
	
    <!-- ETag过滤器,节省带宽 -->
	<filter>
		<filter-name>etagFilter</filter-name>
		<filter-class>org.springframework.web.filter.ShallowEtagHeaderFilter</filter-class>
	</filter>
	<filter-mapping>
		<filter-name>etagFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	
    <!--UTF-8编码 -->
	<filter>
		<filter-name>CharacterEncodingFilter</filter-name>
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
		<filter-name>CharacterEncodingFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	
    <!-- OpenSessionInViewFilter -->
 	<filter>
		<filter-name>OpenSessionInViewFilter</filter-name>
		<filter-class>org.springframework.orm.hibernate4.support.OpenSessionInViewFilter</filter-class>
		<init-param>
			<param-name>singleSession</param-name>
			<param-value>true</param-value>
		</init-param>
	</filter>
	<filter-mapping>
		<filter-name>OpenSessionInViewFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping> 	
   
    <filter>
		<filter-name>springSecurityFilterChain</filter-name>
		<filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
	</filter>
	<filter-mapping>
	    <filter-name>springSecurityFilterChain</filter-name>
	    <url-pattern>/*</url-pattern>
	</filter-mapping> 
		
    <!-- 页面装配-sitemesh -->
    <filter>
		<filter-name>sitemesh</filter-name>
		<filter-class>org.sitemesh.config.ConfigurableSiteMeshFilter</filter-class>
	</filter>
	<filter-mapping>
		<filter-name>sitemesh</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping> 
 
	
    <!-- 清理内存 -->
	<listener>
		<listener-class>org.springframework.web.util.IntrospectorCleanupListener</listener-class>
	</listener>
 
    <!-- WebService-CXF --> 
    <servlet>
		<servlet-name>CXFServlet</servlet-name>
		<servlet-class>org.apache.cxf.transport.servlet.CXFServlet</servlet-class>
	</servlet>
	<servlet-mapping>
		<servlet-name>CXFServlet</servlet-name>
		<url-pattern>/services/*</url-pattern>
	</servlet-mapping> 
    <!-- spring-MVC --> 
	<servlet>
		<servlet-name>springMVC</servlet-name>
		<servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
		<init-param>
			<param-name>contextConfigLocation</param-name>
			<param-value>classpath:config/webmvc-config.xml</param-value>
		</init-param>
		<load-on-startup>1</load-on-startup>
	</servlet>
	<servlet-mapping>
		<servlet-name>springMVC</servlet-name>
		<url-pattern>/</url-pattern>
	</servlet-mapping>
	
	<!-- session过期时长 -->
	<session-config>
		<session-timeout>60</session-timeout>
	</session-config>
	
   <!--默认首页 -->
	<welcome-file-list>
		<welcome-file>/main.jsp</welcome-file>
	</welcome-file-list>
	
	<!-- 异常页面 -->
	<error-page>
		<exception-type>java.lang.Throwable</exception-type>
		<location>/common/500.jsp</location>
	</error-page>
	<error-page>
		<error-code>500</error-code>
		<location>/common/500.jsp</location>
	</error-page>	
	<error-page>
		<error-code>404</error-code>
		<location>/common/404.jsp</location>
	</error-page>
    <error-page>
		<error-code>403</error-code>
		<location>/common/403.jsp</location>
	</error-page>
	
    <!-- 	    
	<servlet>
		<servlet-name>coreServlet</servlet-name>
		<servlet-class>
		org.nercita.bcp.wechat.servlet.CoreServlet
		</servlet-class>
	</servlet> 	
    <servlet-mapping>  
        <servlet-name>coreServlet</servlet-name>  
        <url-pattern>/wx.do</url-pattern>  
    </servlet-mapping>
     -->
</web-app>
```
3.application-security.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<beans:beans xmlns="http://www.springframework.org/schema/security"
             xmlns:beans="http://www.springframework.org/schema/beans"
             xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
             xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.0.xsd
                                 http://www.springframework.org/schema/security http://www.springframework.org/schema/security/spring-security-3.2.xsd"> 
    
    <!--  配置不要过滤的图片等静态资源 -->
    <http pattern="/services*" security="none" />
    <http pattern="/wx.do*" security="none" />
    <http pattern="/api/**" security="none" />
    <http pattern="/services/**" security="none" />
    <http pattern="/common/**" security="none" />
    <http pattern="/images/**" security="none" />
    <http pattern="/styles/**" security="none" />
    <http pattern="/js/**" security="none" />
    <http pattern="/css/**" security="none" />
    <http pattern="/htm/**" security="none" />
    <http pattern="/main.jsp*" security="none" />
    <http pattern="/login.jsp*" security="none" />
    <http pattern="/install.jsp*" security="none" />
    <http pattern="/system/springSecurity/init*" security="none" />
    <http pattern="/system/user/registPage*" security="none" />
    <http pattern="/system/user/regist*" security="none" />
    <http pattern="/mr*" security="none" />
    <http pattern="/system/user/mobile/regist*" security="none" />
    <http pattern="/system/user/activate*" security="none" />
    <http pattern="/system/user/checkImg*" security="none" />
    <http pattern="/system/user/checkName*" security="none" />              
    <http pattern="/system/user/checkValidateCode*" security="none" />              
    <http pattern="/system/user/forgotPassword*" security="none" /> 
    <http pattern="/system/user/resetRequest*" security="none" /> 
    <http pattern="/system/user/resetPasswordPage*" security="none" /> 
    <http pattern="/system/user/resetPassword*" security="none" /> 
    <http pattern="/system/message/save*" security="none" />
    <http pattern="/system/message/introduction" security="none" />  
     
    <http auto-config="true" create-session="always" access-denied-page="/common/403.jsp"
          use-expressions="true"  disable-url-rewriting="true">
        <!-- 配置登录页面  --> 
        <form-login login-page="/login.jsp" login-processing-url="/j_spring_security_check" 
                    authentication-failure-url="/login.jsp?error=true" 
                    default-target-url="/index" 
                    always-use-default-target="true"
                    authentication-success-handler-ref="authenticationSuccess" 
                    authentication-failure-handler-ref="exceptionMappingAuthenticationFailureHandler"/>
        <!-- "记住我"功能，采用持久化策略(将用户的登录信息存放cookie -->             
        <remember-me key="bcp" use-secure-cookie="true" />
        
        <!-- 用户退出的跳转页面 -->  
        <!-- <logout invalidate-session="true" logout-url="/j_spring_security_logout" logout-success-url="/login.jsp"/> -->
        <logout invalidate-session="true" logout-url="/j_spring_security_logout" success-handler-ref="logoutSuccessHandler" />
        
        <!-- 会话管理，设置最多登录异常，error-if-maximum-exceeded = false为第二次登录就会使前一个登录失效 -->
        <session-management invalid-session-url="/login.jsp?expired=true">
            <concurrency-control max-sessions="1" error-if-maximum-exceeded="false" expired-url="/login.jsp?expired=true"/>
        </session-management>
        
        <!-- 添加自定义的过滤器 放在FILTER_SECURITY_INTERCEPTOR之前有效 -->           
        <custom-filter ref="customFilterSecurityInterceptor" before="FILTER_SECURITY_INTERCEPTOR" /> 
        
        <custom-filter ref="switchUserProcessingFilter" after="FILTER_SECURITY_INTERCEPTOR"/> 
        
        <anonymous enabled="false" /> 
    </http>
    
    <!-- 登陆成功跳转 -->
    <beans:bean id="authenticationSuccess" class="org.nercita.bcp.system.service.CustomSavedRequestAwareAuthenticationSuccessHandler">
         <beans:property name="defaultTargetUrl" value="/index"/>
    </beans:bean>
    
    <!-- 注销成功 -->
    <beans:bean id="logoutSuccessHandler" class="org.nercita.bcp.system.service.CustomLogoutSuccessHandler">
         <beans:property name="defaultTargetUrl" value="/login.jsp"></beans:property>
    </beans:bean>
    
    <beans:bean id="customUserDetailsService" class="org.nercita.bcp.system.service.CustomUserDetailsService"/>
  
    <!-- 用户登陆处理 -->
    <authentication-manager alias="authenticationManager">
        <authentication-provider user-service-ref="customUserDetailsService">            
            <password-encoder hash="md5" base64="true" >
                  <salt-source user-property="username"/>
            </password-encoder>             
        </authentication-provider>
    </authentication-manager>
    
	<beans:bean id="exceptionMappingAuthenticationFailureHandler" class="org.springframework.security.web.authentication.ExceptionMappingAuthenticationFailureHandler">
 		<beans:property name="exceptionMappings">
  			<beans:props>
   	  			<beans:prop key="org.springframework.security.authentication.DisabledException">/login.jsp?role=false</beans:prop>
      			<beans:prop key="org.springframework.security.authentication.BadCredentialsException">/login.jsp?error=true</beans:prop>
      			<beans:prop key="org.springframework.security.authentication.LockedException">/login.jsp?locked=true</beans:prop>           
  			</beans:props>
 		</beans:property>
	</beans:bean>
	
	<beans:bean id="switchUserProcessingFilter" class="org.springframework.security.web.authentication.switchuser.SwitchUserFilter">
 			<beans:property name="userDetailsService" ref="customUserDetailsService" />
 			<beans:property name="switchUserUrl" value="/j_spring_security_switch_user" />
   		    <beans:property name="exitUserUrl" value="/j_spring_security_exit_user" />
 			<beans:property name="targetUrl" value="/index" />
	</beans:bean> 
	
</beans:beans>
```
4. applicationContext.xml
```java
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
	<context:component-scan base-package="org.nercita.bcp">
		<context:exclude-filter expression="org.springframework.stereotype.Controller" type="annotation" />
	</context:component-scan>
	
	<!-- 数据源 -->
    <bean id="dataSource" class="com.mchange.v2.c3p0.ComboPooledDataSource" >
        <property name="driverClass" value="${db.driverClass}"/>
        <property name="jdbcUrl" value="${db.url}"/>
        <property name="user" value="${db.username}"/>
        <property name="password" value="${db.password}"/>
    </bean> 
    
    <!-- sessionFactory -->
    <bean id="sessionFactory" class="org.springframework.orm.hibernate4.LocalSessionFactoryBean">
		<property name="dataSource">
			<ref bean="dataSource" />
		</property>
		<property name="hibernateProperties">
			<props>
				<prop key="hibernate.dialect">${hibernate.dialect}</prop>
				<prop key="hibernate.show_sql">${hibernate.show_sql}</prop>
				<prop key="hibernate.format_sql">${hibernate.format_sql}</prop>
				<prop key="hibernate.cache.use_second_level_cache">${hibernate.use_second_level_cache}</prop>
				<prop key="hibernate.cache.use_query_cache">${hibernate.use_query_cache}</prop>
				<prop key="hibernate.cache.region.factory_class">org.hibernate.cache.ehcache.EhCacheRegionFactory</prop> 
				<prop key="hibernate.hbm2ddl.auto">${hibernate.hbm2ddl}</prop>
			</props>
		</property>
		<property name="packagesToScan">
			<list>
			      <value>org.nercita.bcp.**.domain**</value>  
			</list>
		</property>
	</bean>
    <!-- 事务管理器 -->
 	<bean id="transactionManager" class="org.springframework.orm.hibernate4.HibernateTransactionManager">
		<property name="sessionFactory" ref="sessionFactory" />
	</bean> 
	
    <!-- 开启注解事务 -->
	<tx:annotation-driven transaction-manager="transactionManager" />
    <!-- 获取spring上下文 的ApplicationContextAware的实现Bean -->
	<bean  class="org.nercita.core.utils.SpringContextHolder" lazy-init="false" /> 
</beans>
```
5.application.properties
```java
#jdbc settings
#Mysql settings 3306
#db.url=jdbc:mysql://localhost:3306/bcp
#db.driverClass=com.mysql.jdbc.Driver
#db.username=root
#db.password=0729
#hibernate.dialect=org.hibernate.dialect.MySQLDialect
#hibernate settings
hibernate.show_sql=true
hibernate.format_sql=true
hibernate.use_second_level_cache=true
hibernate.use_query_cache=true
hibernate.hbm2ddl=update
```
```
```
6. springMVC-config.xml
```
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:mvc="http://www.springframework.org/schema/mvc" xmlns:context="http://www.springframework.org/schema/context"
	xmlns:p="http://www.springframework.org/schema/p" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.0.xsd     
	http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.0.xsd 
	http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc-4.0.xsd">
    
    <!-- 静态资源 不拦截 -->
	<mvc:resources location="/common/"   mapping="/common/**"/>
	<mvc:resources location="/images/"   mapping="/images/**" />
	<mvc:resources location="/js/"       mapping="/js/**" /> 
	<mvc:resources location="/css/"      mapping="/css/**" />
	<mvc:resources location="/styles/"   mapping="/styles/**" />
	<mvc:resources location="/htm/"      mapping="/htm/**" />
	
    <import resource="view-controller.xml" />
	
	<context:annotation-config/>
	
	<context:component-scan base-package="org.nercita.bcp" use-default-filters="false">
		<context:include-filter expression="org.springframework.stereotype.Controller" type="annotation" />
	</context:component-scan>
		
	<bean id="handlerMapping"  class="org.springframework.web.servlet.mvc.annotation.DefaultAnnotationHandlerMapping"/>  
	<bean id="handlerAdapter"  class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter">	
		<property name="messageConverters">
			<list>
				<bean class="org.nercita.core.web.springmvc.StringHttpMessageConverter" />
				<ref bean="msgConverter"/>
			</list>
		</property>
		<property name="webBindingInitializer">
			<bean class="org.nercita.core.web.springmvc.CustomBindInitializer">		   
				<!-- 
				<property name="validator" ref="validator" />
				<property name="conversionService" ref="conversionService" /> 
				--> 
			</bean> 
		</property>
	</bean>	
	
 	<bean id="msgConverter" class="org.springframework.http.converter.json.MappingJacksonHttpMessageConverter">
        <property name="supportedMediaTypes">
            <list>
                <value>text/html;charset=UTF-8</value>
                <value>text/json;charset=UTF-8</value>
                <value>application/json;charset=UTF-8</value>
            </list>
        </property>
    </bean>  
    
    <!-- 
    <bean id="validator" class="org.springframework.validation.beanvalidation.LocalValidatorFactoryBean"/>
    <bean id="conversionService" class="org.springframework.format.support.FormattingConversionServiceFactoryBean"/> 
    -->  
    
    <bean class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
        <property name="ignoreAcceptHeader" value="true"/>
        <property name="defaultContentType" value="text/html"/>
        <property name="mediaTypes">
            <map>
                <entry key="json" value="application/json"/>
                <entry key="xml" value="application/xml"/>
            </map>
        </property>
        <property name="favorParameter" value="false"/>
        <property name="viewResolvers">
            <list>
                <bean class="org.springframework.web.servlet.view.BeanNameViewResolver"/>
                <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
                    <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>
                    <property name="prefix" value="/WEB-INF/views/"/>
                    <property name="suffix" value=".jsp"/>
                </bean>
            </list>
        </property>
        <property name="defaultViews">
            <list>
                <bean class="org.springframework.web.servlet.view.json.MappingJacksonJsonView"/>
                <bean class="org.springframework.web.servlet.view.xml.MarshallingView">
                    <property name="marshaller">
                        <bean class="org.springframework.oxm.xstream.XStreamMarshaller"/>
                    </property>
                </bean>
            </list>
        </property>
    </bean> 
    
    <bean class="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">
		<property name="defaultErrorView" value="error" />
		<property name="exceptionMappings">
			<props>
				<prop key=".DataAccessException">dataAccessFailure</prop>
				<prop key=".NoSuchRequestHandlingMethodException">resourceNotFound</prop>
				<prop key=".TypeMismatchException">resourceNotFound</prop>
				<prop key=".lang.Exception">uncaughtException</prop>
			</props>
		</property>
	</bean>   
    <!-- 文件上传-->
    <bean id="multipartResolver" class="org.springframework.web.multipart.commons.CommonsMultipartResolver">
        <property name="maxUploadSize" value="104857600"/>
        <property name="maxInMemorySize" value="4096"/>
    </bean>
	
	<!-- 处理国际化资源文件 -->  
    <bean id="messageSource" class="org.springframework.context.support.ResourceBundleMessageSource">    
       <property name="basename" value="message/message-info" />              
       <property name="useCodeAsDefaultMessage" value="true" />       
    </bean>
	
	
 
</beans>
```
**2、实现代码**
1、CustomAccessDecisionManager.java
```
package org.nercita.bcp.system.service;
import java.util.Collection;
import java.util.Iterator;
import org.springframework.security.access.AccessDecisionManager;
import org.springframework.security.access.AccessDeniedException;
import org.springframework.security.access.ConfigAttribute;
import org.springframework.security.access.SecurityConfig;
import org.springframework.security.authentication.InsufficientAuthenticationException;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.stereotype.Service;
/** 
 * 该类为访问决策器，决定某个用户具有的角色，是否有足够的权限去访问某个资源，实现用户和访问权限的对应关。
 * 这个类主要是处理用户在访问某个URL的时候，就会通过访问该类的权限与登录用户所拥有的权限做比较，
 * 如果用户拥有权限，那就可以到访问资源，如果没有权限，那不能访问资源，还会抛一个异常。
 * AccessdecisionManager在Spring security中是很重要的。  
 * 验证部分简略提过所有的Authentication实现需要保存在一个GrantedAuthority对象数组中。 这就是赋予给主体的权限。 
 * GrantedAuthority对象通过AuthenticationManager 保存到 Authentication对象里，然后从AccessDecisionManager读出来，进行授权判断。  
 * Spring Security提供了一些拦截器，来控制对安全对象的访问权限，例如方法调用或web请求。 
 * 一个是否允许执行调用的预调用决定，是由AccessDecisionManager实现的。 
 * 这个 AccessDecisionManager被AbstractSecurityInterceptor调用， 用来作最终访问控制的决定。
 *  
 * 这个AccessDecisionManager接口包含三个方法： 
 * void decide(Authentication authentication, Object secureObject, List<ConfigAttributeDefinition> config); 
 * boolean  supports(ConfigAttribute attribute);  boolean supports(Class clazz);  
 * 第一个方法：AccessDecisionManager使用方法参数传递所有信息，认证评估时进行决定。 
 * 如果访问被拒绝，实现将抛出一个AccessDeniedException异常。   
 * 第二个方法：在启动的时候被 AbstractSecurityInterceptor调用，
 * 来决定AccessDecisionManager 是否可以执行传递ConfigAttribute。
 * 第三个方法：被安全拦截器实现调用， 安全拦截器将显示的AccessDecisionManager支持安全对象的类型。   
 */
@Service("customAccessDecisionManager")
public class CustomAccessDecisionManager implements AccessDecisionManager {
   /**
    * 该方法：需要比较权限和权限配置
    * object参数是一个 URL, 同一个过滤器该url对应的权限配置被传递过来.
    * 查看authentication是否存在权限在configAttributes中
    * 如果没有匹配的权限, 扔出一个拒绝访问的异常
    */
	@Override
	public void decide(Authentication authentication, Object object,
			Collection<ConfigAttribute> configAttributes)
			throws AccessDeniedException, InsufficientAuthenticationException {
		if (configAttributes==null){
            return;
        }        
        Iterator<ConfigAttribute> iter = configAttributes.iterator();        
        while(iter.hasNext()){
            ConfigAttribute ca = iter.next();
            String needRole = ((SecurityConfig) ca).getAttribute();
            //gra 为用户所被赋予的权限，needRole为访问相应的资源应具有的权限
            for (GrantedAuthority gra : authentication.getAuthorities()) {
                if (needRole.trim().equals(gra.getAuthority().trim())) {
                    return;
                }
            }
        }
        throw new AccessDeniedException("Access Denied");
	}
	@Override
	public boolean supports(ConfigAttribute attribute) {
		
		return true;
	}
	@Override
	public boolean supports(Class<?> clazz) {
		
		return true;
	}
}
```
2、CustomFilterInvocationSecurityMetadataSource.java
```
package org.nercita.bcp.system.service;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import javax.annotation.PostConstruct;
import org.nercita.bcp.system.domain.Authority;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.access.ConfigAttribute;
import org.springframework.security.access.SecurityConfig;
import org.springframework.security.web.FilterInvocation;
import org.springframework.security.web.access.intercept.FilterInvocationSecurityMetadataSource;
import org.springframework.stereotype.Service;
/**
 * 该类是资源的访问权限的定义，实现资源和访问权限的对应关系
 * 该类的主要作用是在Spring Security的整个过滤链启动后，
 * 在容器启动的时候，程序就会进入到该类中的init()方法，init调用了loadResourceDefine()方法，
 * 该方法的主要目的是将数据库中的所有资源与权限读取到本地缓存中保存起来！
 * 类中的resourceMap就是保存的所有资源和权限的集合，URL为Key，权限作为Value！
 * 
 * @author zhangwenchao
 *
 */
@Service("customFilterInvocationSecurityMetadataSource")
public class CustomFilterInvocationSecurityMetadataSource implements FilterInvocationSecurityMetadataSource {
	
	@Autowired
	private AuthorityService authorityService;
	
	//使用的是AntUrlPathMatcher这个path matcher来检查URL是否与资源定义匹配
	//private RequestMatcher urlMatcher = null; 
	
	//resourceMap就是保存的所有资源和权限的集合，URL为Key，权限作为Value！ 
	private static HashMap<String, Collection<ConfigAttribute>> resourceMap = null; 
	
    /** 
     * 
     * 自定义方法，这个类放入到Spring容器后，  
     * 指定init为初始化方法，从数据库中读取资源  
     */  
    @PostConstruct	    
    public void init() {  
        loadResourceDefine();  
    }
    
    /**
     * 
     * 程序启动的时候就加载所有资源信息
     * 初始化资源与权限的映射关系
     */
    private void loadResourceDefine() {
        // 在Web服务器启动时，提取系统中的所有权限authority.name.	      
        List<Authority> authorities = authorityService.findAll();
        
        //应当是资源为key， 权限为value。 资源通常为url， 权限就是那些以ROLE_为前缀的角色。 一个资源可以由多个权限来访问。	       
        resourceMap = new HashMap<String, Collection<ConfigAttribute>>();
       
        if(authorities!=null && authorities.size()>0 )
	        for (Authority auth : authorities) {    
	            String authName = auth.getCode();  //获取权限的name 是以 ROLE_为前缀的代码值	       
	            ConfigAttribute ca = new SecurityConfig(authName); //将ROLE_XXX 封装成spring的权限配置属性	            	            
	            //根据权限名获取所有资源
	            String url = auth.getResourceUrl();	            
	            //判断资源文件和权限的对应关系，如果已经存在相关的资源url，则要通过该url为key提取出权限集合，将权限增加到权限集合中。	               
                if (resourceMap.containsKey(url)) { //如果已存在url 加入权限
                    Collection<ConfigAttribute> value = resourceMap.get(url);
                    value.add(ca);
                    resourceMap.put(url, value);
                } else {//如果不存存在url 加入url和权限
                    Collection<ConfigAttribute> atts = new ArrayList<ConfigAttribute>();
                    atts.add(ca);
                    resourceMap.put(url, atts);
                }
	            
	
	        }
    }
    
    /**
     * 根据URL获取该URL权限的配置
     */
	@Override
	public Collection<ConfigAttribute> getAttributes(Object object)
			throws IllegalArgumentException {
		//object是一个URL ,为用户请求URL
        String url = ((FilterInvocation)object).getRequestUrl();     
        int firstQuestionMarkIndex = url.indexOf("?");
        if (firstQuestionMarkIndex != -1) {
            url = url.substring(0, firstQuestionMarkIndex);
        }
        Iterator<String> iter = resourceMap.keySet().iterator();  
        
        String matchUrl=null;//匹配url
        //取到请求的URL后与上面取出来的资源做比较
        while (iter.hasNext()) {
            String resURL = iter.next();
          //  if(urlMatcher.pathMatchesUrl(resURL,url)){
	          if(url.startsWith(resURL)){
//	            	return resourceMap.get(resURL);  //返回权限的集合
	        	  //初次匹配或当前匹配的url更长则更新匹配url
	        	  if(matchUrl==null||matchUrl.length()<resURL.length())
	        		  matchUrl=resURL;
	          }              
                
        }
        if(matchUrl!=null){
        	//如果存在匹配的url则返回需具备的权限
//        	System.out.println(matchUrl+"-------"+resourceMap.get(matchUrl));
        	return resourceMap.get(matchUrl);
        }
        // 当系统中没配资源权限url，用户在访问这个资源的情况下，返回null 表示放行 ，
        // 如果当系统分配了资源url,但是这个用户立属的角色没有则 提示用户无权访问这个页面
		return null;
	}
	/*
     * @return
     * @link org.springframework.security.access.SecurityMetadataSource#getAllConfigAttributes()
     */
	@Override
	public Collection<ConfigAttribute> getAllConfigAttributes() {
		
		return null;
	}
	@Override
	public boolean supports(Class<?> clazz) {
		
		return true;
	}
    
	
}
```
3、CustomFilterSecurityInterceptor.java
```
package org.nercita.bcp.system.service;
import java.io.IOException;
import javax.annotation.Resource;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.nercita.bcp.system.util.LogInfoService;
import org.springframework.security.access.AccessDeniedException;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.security.access.AccessDecisionManager;
import org.springframework.security.access.SecurityMetadataSource;
import org.springframework.security.access.intercept.AbstractSecurityInterceptor;
import org.springframework.security.access.intercept.InterceptorStatusToken;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.web.FilterInvocation;
import org.springframework.security.web.access.intercept.FilterInvocationSecurityMetadataSource;
import org.springframework.stereotype.Service;
@Service("customFilterSecurityInterceptor") 
public class CustomFilterSecurityInterceptor extends
		AbstractSecurityInterceptor implements Filter {
	
	// 注入资源数据定义器
	@Resource  
	@Qualifier("customFilterInvocationSecurityMetadataSource")
    private FilterInvocationSecurityMetadataSource securityMetadataSource;
   
	// 注入访问决策器
	@Resource  
	@Qualifier("customAccessDecisionManager")
    @Override
    public void setAccessDecisionManager(AccessDecisionManager accessDecisionManager){
        super.setAccessDecisionManager(accessDecisionManager);
    }
	// 注入认证管理器
	@Resource  
	@Qualifier("authenticationManager")  
    @Override
    public void setAuthenticationManager(AuthenticationManager newManager) {
        super.setAuthenticationManager(newManager);
    }
	
	@Override
	public void doFilter(ServletRequest request, ServletResponse response,
			FilterChain chain) throws IOException, ServletException {
		
        HttpServletRequest httpRequest = (HttpServletRequest)request;   
        HttpServletResponse httpResponse = (HttpServletResponse)response;
        
        // 用户未登录情况下 通过在地址栏输入有效的url 访问系统 可能造成系统出现问题，所以限制匿名用户登录 自动跳转到登录页面
        if(LogInfoService.getLoginUserName()==null){
        	  httpResponse.sendRedirect(httpRequest.getContextPath()+"/login.jsp"); 
              return;   
          }
             
       FilterInvocation fi = new FilterInvocation(request, response, chain);   
       invoke(fi);   
	}
	
    /**
     * 
     * @param fi 
     * @throws ServletException 
     * @throws IOException 
     */
    private void invoke(FilterInvocation fi) throws IOException, ServletException {
//        InterceptorStatusToken token = super.beforeInvocation(fi);
//        try {
//            fi.getChain().doFilter(fi.getRequest(), fi.getResponse());
//        } finally {
//            super.afterInvocation(token, null);
//        }
    	
    	
    	InterceptorStatusToken token = null;   
	 	try {   
	          token = super.beforeInvocation(fi);
	          
		} catch (Exception e) {   
			
			// 用户登录情况下 系统中存在用户访问的资源url和权限，但是当前用户的角色中没有这个权限 所以提示跳转用户无权访问的页面
			if( e instanceof AccessDeniedException){
//			    HttpServletRequest httpRequest = fi.getRequest();   
//			    HttpServletResponse httpResponse = fi.getResponse();   
//			    			    
//			    String path = httpRequest.getContextPath();
//		    	String basePath = httpRequest.getScheme()+"://"+httpRequest.getServerName()+":"+httpRequest.getServerPort()+path+"/";
//			    httpResponse.setStatus(HttpServletResponse.SC_NOT_ACCEPTABLE);   
			    
//			    RequestDispatcher dispatcher = httpRequest.getRequestDispatcher(basePath+"/common/403.jsp");   
//			    
//			    dispatcher.forward(httpRequest, httpResponse);   
//			    httpResponse.sendRedirect(basePath+"/common/403.jsp"); 
		    	throw new AccessDeniedException("用户无权访问");  
			}
			 return;
		 }   
		         
		       try {   
		           fi.getChain().doFilter(fi.getRequest(), fi.getResponse());   
		      } finally {   
		           super.afterInvocation(token, null);   
		      }   
        
    }
	@Override
	public void init(FilterConfig arg0) throws ServletException {
		
	}
	@Override
	public Class<? extends Object> getSecureObjectClass() {
		
		return FilterInvocation.class;  
	}
	@Override
	public SecurityMetadataSource obtainSecurityMetadataSource() {
		
		return this.securityMetadataSource;
	}
	
	@Override
	public void destroy() {
		
	}
	
	public FilterInvocationSecurityMetadataSource getSecurityMetadataSource() {
		return securityMetadataSource;
	}
	
	public void setSecurityMetadataSource(
			FilterInvocationSecurityMetadataSource securityMetadataSource) {
		this.securityMetadataSource = securityMetadataSource;
	}
}
```
3、核心处理类：CustomUserDetailsService.java
```
package org.nercita.bcp.system.service;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import javax.annotation.Resource;
import org.nercita.bcp.system.dao.AuthorityDao;
import org.nercita.bcp.system.dao.UserDao;
import org.nercita.bcp.system.domain.Authority;
import org.nercita.bcp.system.domain.User;
import org.nercita.bcp.system.domain.reference.UserDetail;
import org.nercita.bcp.system.domain.reference.UserState;
import org.nercita.bcp.system.domain.reference.UserType;
import org.springframework.dao.DataAccessException;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;
/**
 * 该类是用户信息的定义和验证
 * 这个类主要是处理用户登录信息，在用户输入用户名和密码后，
 * spring security会带着用户名调用类里面的loadUserByUsername(usrename)方法，
 * 通过用户名查出用户信息，然后把数据库中查出的用户密码和刚刚用户输入的存储在session中的密码做比较，然后判断该用户是否合法！ 
 *
 */
@Service("customUserDetailsService")
public class CustomUserDetailsService implements UserDetailsService {
	//用户持久类
	@Resource(name="userDao")
	private UserDao userDao;
	@Resource(name="authorityDao")
	private AuthorityDao authorityDao;
	
	@Override
	public UserDetails loadUserByUsername(String userName)
			throws UsernameNotFoundException, DataAccessException {
		//根据登录名获取登陆用户	
		User user = userDao.findByName(userName);
		if(null == user) {  
			 throw new UsernameNotFoundException("用户" + userName + "不存在");  
		}		
		//根据用户的 类型获取用户权限
		if(user.getUserType() == UserType.SysAdmin){ //用户类型为0, 表示是系统平台管理员用户			
			Collection<GrantedAuthority> auths = new ArrayList<GrantedAuthority>();			
			List<Authority> authorities = authorityDao.findAll();  //获取系统中所有权限
			for(Iterator<Authority> iter =authorities.iterator();iter.hasNext();){ 
				auths.add(new SimpleGrantedAuthority(iter.next().getCode())); 	
			}			
			//UserDetail userDetail =  new UserDetail(userName, user.getPassword(), true, true, true, true, auths);	
			//修改为前台系统，不能让系统管理员登陆
			UserDetail userDetail =  new UserDetail(userName, user.getPassword(), false, true, true, true, auths);			
			userDetail.setUserGroupId(user.getUserGroup().getId());
			userDetail.setRealName(user.getRealName());
			userDetail.setUserType(user.getUserType());
			return userDetail;	
		}else if(user.getUserType() == UserType.TeamAdmin){ //团队管理员
			//用户是否可用
			boolean enabled = user.getEnabled();
			//获取团队管理员的所有权限
			Set<Authority> authorities = user.getAuthoritySet();
			Collection<GrantedAuthority> auths = new ArrayList<GrantedAuthority>();
			for(Iterator<Authority> iter=authorities.iterator();iter.hasNext();){
				auths.add(new SimpleGrantedAuthority(iter.next().getCode()));
			}
			//因为spring security包中的User也实现了UserDetails，所以也可以直接返回User 
			UserDetail userDetail =null;
			if(UserState.Enable == user.getUserState()){
				userDetail = new UserDetail(userName, user.getPassword(), enabled, true, true, true, auths);
			}else{
				userDetail = new UserDetail(userName, user.getPassword(), enabled, true, true, false, auths);
				//throw new LockedException("用户" + userName + "被锁定");
			}
			userDetail.setUserGroupId(user.getUserGroup().getId());
			userDetail.setTeamGroupId(user.getUserGroup().getId());
			userDetail.setRealName(user.getRealName());
			userDetail.setUserType(user.getUserType());
			return userDetail;
	
		}else{//团队用户
			//用户是否可用
			boolean enabled = user.getEnabled();			
			Set<Authority> authorities = user.getAuthoritySet();		
			Collection<GrantedAuthority> auths = new ArrayList<GrantedAuthority>();
			for(Iterator<Authority> iter=authorities.iterator();iter.hasNext();){
				auths.add(new SimpleGrantedAuthority(iter.next().getCode()));
			}
			//因为spring security包中的User也实现了UserDetails，所以也可以直接返回User 
			UserDetail userDetail =null;
			
			if(user.getUserGroup().getTeamAdmin().getUserState()==UserState.Disable){//如果团队管理员被禁用，其下属团队用也不能登录
				userDetail = new UserDetail(userName, user.getPassword(), enabled, true, true, false, auths);
				return userDetail;
			}
			
			if(UserState.Enable == user.getUserState()){
				userDetail = new UserDetail(userName, user.getPassword(), enabled, true, true, true, auths);
			}else{
				userDetail = new UserDetail(userName, user.getPassword(), enabled, true, true, false, auths);
				//throw new LockedException("用户" + userName + "被锁定");
			}
			userDetail.setUserGroupId(user.getUserGroup().getId());
			userDetail.setTeamGroupId(user.getUserGroup().getTeamAdmin().getUserGroup().getId());
			userDetail.setRealName(user.getRealName());
			userDetail.setUserType(user.getUserType());
			return userDetail;
			
		}
		
	}
}
```
4.附加类登录成功和退出成功处理
1、CustomSavedRequestAwareAuthenticationSuccessHandler.java
```
package org.nercita.bcp.system.service;
import java.io.IOException;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.nercita.bcp.basic.domain.Crop;
import org.nercita.bcp.system.domain.SystemLog;
import org.nercita.bcp.system.domain.User;
import org.nercita.bcp.system.domain.reference.UserType;
import org.nercita.bcp.system.util.LogInfoService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.Authentication;
import org.springframework.security.web.authentication.SavedRequestAwareAuthenticationSuccessHandler;
import org.springframework.security.web.savedrequest.RequestCache;
public class CustomSavedRequestAwareAuthenticationSuccessHandler extends
		SavedRequestAwareAuthenticationSuccessHandler {
	// 增加登录日志
	private static Logger logger = LogManager.getLogger(CustomSavedRequestAwareAuthenticationSuccessHandler.class);
	
	@Autowired
	private SystemLogService systemLogService;
	
	@Autowired
	private UserService userService;
	
	@Override
	public void onAuthenticationSuccess(HttpServletRequest request,
			HttpServletResponse response, Authentication authentication)
			throws ServletException, IOException {
		
		System.out.println("用户登录成功处理。。。。");
		User u = LogInfoService.getLoginUser();
		HttpSession session =request.getSession();
		if(u.getUserType()==UserType.TeamAdmin ){//用户为团队管理员
			List<Crop> croplist = u.getCropSet();
			for(Iterator<Crop> iter = croplist.iterator();iter.hasNext();){//去除被禁止的作物
				String status = userService.getUserCropStatus(u.getId(),iter.next().getId());
				if(status!=null && !status.equals("1")){
					iter.remove();
				}				
			}
			session.setAttribute("croplist", croplist);  
		}else if(u.getUserType()==UserType.TeamUser){//用户为普通用户
			List<Crop> croplist = u.getCropSet();
			for(Iterator<Crop> iter = croplist.iterator();iter.hasNext();){//去除团队管理员被禁止的作物
				String status = userService.getUserCropStatus(u.getUserGroup().getTeamAdmin().getId(),iter.next().getId());
				if(status!=null && !status.equals("1")){
					iter.remove();
				}				
			}
			session.setAttribute("croplist", croplist);
		}else{//用户为系统管理员
			session.setAttribute("croplist", LogInfoService.getAllCrops());
		} 
		String userName = authentication.getName(); //用户名
		String address =  request.getRemoteAddr();  //远程地址
		//写入日志
		systemLogService.save(new SystemLog(address, "登录", "用户登录系统", userName, new Date()));
		logger.info("用户" + userName + "在地址" + address + "登入系统，时间："+new Date());
		super.onAuthenticationSuccess(request, response, authentication);	
	}
	@Override
	public void setRequestCache(RequestCache requestCache) {
		
		super.setRequestCache(requestCache);
	}
	
	
}
```
2、CustomLogoutSuccessHandler.java
```
package org.nercita.bcp.system.service;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.nercita.bcp.system.domain.SystemLog;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.Authentication;
import org.springframework.security.web.authentication.logout.SimpleUrlLogoutSuccessHandler;
public class CustomLogoutSuccessHandler extends SimpleUrlLogoutSuccessHandler {
	
	private static Logger logger = LogManager.getLogger(CustomLogoutSuccessHandler.class);
	@Autowired
	private SystemLogService systemLogService;
	@Override
	public void onLogoutSuccess(HttpServletRequest request,
			HttpServletResponse response, Authentication authentication)
			throws IOException, ServletException {
		System.out.println("用户退出成功处理。。。。");	
		String userName = authentication.getName(); //用户名
		String address =  request.getRemoteAddr();  //远程地址
		systemLogService.save(new SystemLog(address, "退出", "用户退出系统", userName, new Date()));
		logger.info("日志：ip:"+request.getRemoteAddr() +"host:"+request.getRemoteHost()+"退出时间："+new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()));	
		
		super.onLogoutSuccess(request, response, authentication);
	
		
	}
}
```
####  3、实体Bean
1、User.java
```java
package org.nercita.bcp.system.domain;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import javax.persistence.Cacheable;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToMany;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import org.hibernate.annotations.Cache;
import org.hibernate.annotations.CacheConcurrencyStrategy;
import org.nercita.bcp.basic.domain.Crop;
import org.nercita.bcp.system.domain.reference.ExamineStatus;
import org.nercita.bcp.system.domain.reference.UserState;
import org.nercita.bcp.system.domain.reference.UserType;
import org.nercita.core.orm.BaseUidEntity;
import com.alibaba.fastjson.annotation.JSONField;
/**
 * 用户实体
 * @author zhangwenchao
 */
@Entity
@Table(name = "T_P_User")
@Cacheable
@Cache(usage = CacheConcurrencyStrategy.READ_WRITE)
public class User extends BaseUidEntity {
	private static final long serialVersionUID = -2512321822386351911L;
	// 用户名
	@Column(length = 64, unique = true, nullable = false)
	private String name;
	// 密码
	@Column(length = 64, nullable = false)
	private String password;
	// 真实姓名
	@Column(length = 64, nullable = false)
	private String realName;
	// 手机
	@Column(length = 13)
	private String phone;
	// 公司或单位
	@Column(length = 64)
	private String company;
	// 电子邮件
	@Column(length = 64)
	private String email;
	// 是否可用
	private Boolean enabled;
	// 激活码
	@Column(unique=true,nullable=true)
	private String activationCode;
	
	
	// 重置码
	@Column(unique=true,nullable=true)
	private String resetCode;
	/*
	 * 用户类型: 0：平台管理员 1：团队管理员 2：团队用户
	 */
	private UserType userType;
	/*
	 * 用户状态 0：禁用(未激活) 1：启用（激活）
	 */
	private UserState userState = UserState.Disable;
	
	
	/*
	 * 是否黑名单    0：白名单   1：黑名单
	 */
	@Column(length=1)
	private int abolished=0;
	
	
	//用户代号
	@Column(length=1)
	private String userMark;
	
	//PDA imei信息
	@Column(nullable=true)
	private String imei;
	// 用户所在用户组
	@ManyToOne(optional = true, cascade = {CascadeType.PERSIST,CascadeType.MERGE})
	@JoinColumn(name = "userGroup_id")
	private UserGroup userGroup;
	//权限集合
	@ManyToMany(fetch = FetchType.LAZY)
	@JSONField(serialize=false)
	private Set<Authority> authoritySet = new HashSet<Authority>();
	// 用户所属的作物集合
	@ManyToMany(fetch = FetchType.EAGER)
//	@JSONField(serialize=false)
	private List<Crop> cropSet = new ArrayList<Crop>();
	// 用户的菜单包
	@ManyToOne(optional = true, fetch = FetchType.EAGER)
	@JSONField(serialize=false)
	private MenuPackage menuPackage;
	// 用户的团队配置
	@ManyToOne(optional = true, fetch = FetchType.EAGER)
	@JSONField(serialize=false)
	private CollocateTeam collocateTeam;
	
	//注册时间
	private Date registData;
	
	//截止日期
	private Date  expirationDate;
	
	//审核状态（未审核、已上传、已审核）
	private ExamineStatus examineStatus;
	
	//审核材料路径
	private String certificationUrls;
	
	// 记录人
	@Column(updatable = false)
	private String recorder;
	// 记录时间
	@Temporal(TemporalType.TIMESTAMP)
	@Column(updatable = false)
	private Date recordTime;
	// 修改人
	private String modifier;
	// 修改时间
	@Temporal(TemporalType.TIMESTAMP)
	@Column(updatable = true)
	private Date modifyTime;
	@Column(length = 255)
	private String remark;
	
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getRealName() {
		return realName;
	}
	public void setRealName(String realName) {
		this.realName = realName;
	}
	public String getPhone() {
		return phone;
	}
	public void setPhone(String phone) {
		this.phone = phone;
	}
	public String getCompany() {
		return company;
	}
	public void setCompany(String company) {
		this.company = company;
	}
	public String getEmail() {
		return email;
	}
	public void setEmail(String email) {
		this.email = email;
	}
	public String getActivationCode() {
		return activationCode;
	}
	public void setActivationCode(String activationCode) {
		this.activationCode = activationCode;
	}
	public String getResetCode() {
		return resetCode;
	}
	public void setResetCode(String resetCode) {
		this.resetCode = resetCode;
	}
	public Boolean getEnabled() {
		return enabled;
	}
	public void setEnabled(Boolean enabled) {
		this.enabled = enabled;
	}
	public UserType getUserType() {
		return userType;
	}
	public void setUserType(UserType userType) {
		this.userType = userType;
	}
	public String getImei() {
		return imei;
	}
	public void setImei(String imei) {
		this.imei = imei;
	}
	public UserState getUserState() {
		return userState;
	}
	public void setUserState(UserState userState) {
		this.userState = userState;
	}
	
	public int getAbolished() {
		return abolished;
	}
	public void setAbolished(int abolished) {
		this.abolished = abolished;
	}
	public UserGroup getUserGroup() {
		return userGroup;
	}
	public void setUserGroup(UserGroup userGroup) {
		this.userGroup = userGroup;
	}
	public Set<Authority> getAuthoritySet() {
		return authoritySet;
	}
	public void setAuthoritySet(Set<Authority> authoritySet) {
		this.authoritySet = authoritySet;
	}
	
	public List<Crop> getCropSet() {
		return cropSet;
	}
	public void setCropSet(List<Crop> cropSet) {
		this.cropSet = cropSet;
	}
	public MenuPackage getMenuPackage() {
		return menuPackage;
	}
	public void setMenuPackage(MenuPackage menuPackage) {
		this.menuPackage = menuPackage;
	}
	public CollocateTeam getCollocateTeam() {
		return collocateTeam;
	}
	public void setCollocateTeam(CollocateTeam collocateTeam) {
		this.collocateTeam = collocateTeam;
	}
	public String getRecorder() {
		return recorder;
	}
	public void setRecorder(String recorder) {
		this.recorder = recorder;
	}
	public Date getRecordTime() {
		return recordTime;
	}
	public void setRecordTime(Date recordTime) {
		this.recordTime = recordTime;
	}
	public String getModifier() {
		return modifier;
	}
	public void setModifier(String modifier) {
		this.modifier = modifier;
	}
	public Date getModifyTime() {
		return modifyTime;
	}
	public void setModifyTime(Date modifyTime) {
		this.modifyTime = modifyTime;
	}
	public String getRemark() {
		return remark;
	}
	public void setRemark(String remark) {
		this.remark = remark;
	}	
	public String getUserMark() {
		return userMark;
	}
	public void setUserMark(String userMark) {
		this.userMark = userMark;
	}
	public User() {
		
	}
	
	
	public Date getRegistData() {
		return registData;
	}
	public void setRegistData(Date registData) {
		this.registData = registData;
	}
	public Date getExpirationDate() {
		return expirationDate;
	}
	public void setExpirationDate(Date expirationDate) {
		this.expirationDate = expirationDate;
	}
	
	public ExamineStatus getExamineStatus() {
		return examineStatus;
	}
	public void setExamineStatus(ExamineStatus examineStatus) {
		this.examineStatus = examineStatus;
	}
	
	public String getCertificationUrls() {
		return certificationUrls;
	}
	public void setCertificationUrls(String certificationUrls) {
		this.certificationUrls = certificationUrls;
	}
	public User(String name, String password, String realName, Boolean enabled,
			UserType userType, UserState userState) {
		super();
		this.name = name;
		this.password = password;
		this.realName = realName;
		this.enabled = enabled;
		this.userType = userType;
		this.userState = userState;
	}
}
```
2.Authority.java
```java
package org.nercita.bcp.system.domain;
import java.util.Date;
import javax.persistence.Cacheable;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import org.hibernate.annotations.Cache;
import org.hibernate.annotations.CacheConcurrencyStrategy;
import org.hibernate.annotations.Index;
import org.nercita.core.orm.BaseUidEntity;
/**
 * 用户权限
 * @author zhangwenchao
 *
 */
@SuppressWarnings("deprecation")
@Entity
@Table(name="T_P_Authority")
@Cacheable  
@Cache(usage=CacheConcurrencyStrategy.READ_WRITE)
public class Authority extends BaseUidEntity{
	private static final long serialVersionUID = 1558957611918721365L;
	
	//ROLE_"为前缀的代码
	@Column(length=64, nullable=false, unique=true)
    @Index(name="Index_Authority_code")
	private String  code;
	//权限名称
	@Column(length=64,nullable=false)
    private String name;
	
	//父权限
	@ManyToOne
	private Authority parentAuthority;
	
	//资源路径
	private String resourceUrl;
	
	//是否为父权限
	private boolean isParent;
	
	//权限树形深度
    private Integer deepLevel = 0;
    
    
    //权限是否可用
    private boolean enabled=true;
    
    //所属菜单
    @ManyToOne(optional = true, cascade = {CascadeType.PERSIST})
    @JoinColumn(name="menu_id")
    private Menu menu; 
    
    
	//记录人
	@Column(updatable = false)
	private String recorder;
	
	
	private Integer orderNum = 0;
    
    
	//记录时间
    @Temporal(TemporalType.TIMESTAMP)
    @Column(updatable = false)
	private Date  recordTime;
    
    //修改人
    private String modifier;
    
    
    //修改时间
    @Temporal(TemporalType.TIMESTAMP)
    @Column(updatable = true)
    private Date modifyTime;
    
    //备注
    @Column(length=255)
    private String remark;
    
	public Integer getOrderNum() {
		return orderNum;
	}
	public void setOrderNum(Integer orderNum) {
		this.orderNum = orderNum;
	}
	public String getCode() {
		return code;
	}
	public void setCode(String code) {
		this.code = code;
	}
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public Authority getParentAuthority() {
		return parentAuthority;
	}
	public void setParentAuthority(Authority parentAuthority) {
		this.parentAuthority = parentAuthority;
	}
	public String getResourceUrl() {
		return resourceUrl;
	}
	public void setResourceUrl(String resourceUrl) {
		this.resourceUrl = resourceUrl;
	}
	public boolean isParent() {
		return isParent;
	}
	public void setParent(boolean isParent) {
		this.isParent = isParent;
	}
	public Integer getDeepLevel() {
		return deepLevel;
	}
	public void setDeepLevel(Integer deepLevel) {
		this.deepLevel = deepLevel;
	}
	public boolean isEnabled() {
		return enabled;
	}
	public void setEnabled(boolean enabled) {
		this.enabled = enabled;
	}
	public Menu getMenu() {
		return menu;
	}
	public void setMenu(Menu menu) {
		this.menu = menu;
	}
	public String getRecorder() {
		return recorder;
	}
	public void setRecorder(String recorder) {
		this.recorder = recorder;
	}
	public Date getRecordTime() {
		return recordTime;
	}
	public void setRecordTime(Date recordTime) {
		this.recordTime = recordTime;
	}
	public String getModifier() {
		return modifier;
	}
	public void setModifier(String modifier) {
		this.modifier = modifier;
	}
	public Date getModifyTime() {
		return modifyTime;
	}
	public void setModifyTime(Date modifyTime) {
		this.modifyTime = modifyTime;
	}
	public String getRemark() {
		return remark;
	}
	public void setRemark(String remark) {
		this.remark = remark;
	}
	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + ((code == null) ? 0 : code.hashCode());
		return result;
	}
	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		Authority other = (Authority) obj;
		if (code == null) {
			if (other.code != null)
				return false;
		} else if (!code.equals(other.code))
			return false;
		return true;
	}
	
	
    
  
	
}
```
3. UserGroup.java
```java
package org.nercita.bcp.system.domain;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import javax.persistence.Cacheable;
import javax.persistence.CascadeType;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.FetchType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.Table;
import javax.persistence.Temporal;
import javax.persistence.TemporalType;
import org.hibernate.annotations.Cache;
import org.hibernate.annotations.CacheConcurrencyStrategy;
import org.nercita.core.orm.BaseUidEntity;
import com.alibaba.fastjson.annotation.JSONField;
@Entity
@Table(name="T_P_UserGroup")
@Cacheable  
@Cache(usage=CacheConcurrencyStrategy.READ_WRITE)
public class UserGroup extends BaseUidEntity{
	
	private static final long serialVersionUID = -2723021723072060418L;
	
	//用户组名
	private String name;
	
	//父用户组 ---允许为空
	@ManyToOne(optional=true,cascade=CascadeType.PERSIST,fetch=FetchType.EAGER)
	private UserGroup parentUserGroup;
	
	
	//子用户组
    @OneToMany(mappedBy="parentUserGroup")
    @JSONField(serialize=false)
    private Set<UserGroup> childrenUserGroups = new HashSet<UserGroup>();
    
    //该用户组的团队管理员
    @ManyToOne(optional=true, cascade=CascadeType.PERSIST)
    @JoinColumn(name="teamAdmin_id")
    private User teamAdmin;
    
    
    //组内用户列表
    @OneToMany(mappedBy="userGroup")
    @JSONField(serialize=false)
    private List<User> userList = new ArrayList<User>();
    
    //用户组深度级别 ：0：团队管理员，  1...N：子用户
    @Column(nullable=false)
    private Integer deepLevel;
    
	//记录人
	@Column(updatable = false)
	private String recorder;
	
	//记录时间
    @Temporal(TemporalType.TIMESTAMP)
    @Column(updatable = false)
	private Date  recordTime;
    
    //修改人
    private String modifier;
    
    
    //修改时间
    @Temporal(TemporalType.TIMESTAMP)
    @Column(updatable = true)
    private Date modifyTime;
    
    @Column(length=255)
    private String remark;
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public UserGroup getParentUserGroup() {
		return parentUserGroup;
	}
	public void setParentUserGroup(UserGroup parentUserGroup) {
		this.parentUserGroup = parentUserGroup;
	}
	public Set<UserGroup> getChildrenUserGroups() {
		return childrenUserGroups;
	}
	public void setChildrenUserGroups(Set<UserGroup> childrenUserGroups) {
		this.childrenUserGroups = childrenUserGroups;
	}
	public User getTeamAdmin() {
		return teamAdmin;
	}
	public void setTeamAdmin(User teamAdmin) {
		this.teamAdmin = teamAdmin;
	}
	public List<User> getUserList() {
		return userList;
	}
	public void setUserList(List<User> userList) {
		this.userList = userList;
	}
	public Integer getDeepLevel() {
		return deepLevel;
	}
	public void setDeepLevel(Integer deepLevel) {
		this.deepLevel = deepLevel;
	}
	public String getRecorder() {
		return recorder;
	}
	public void setRecorder(String recorder) {
		this.recorder = recorder;
	}
	public Date getRecordTime() {
		return recordTime;
	}
	public void setRecordTime(Date recordTime) {
		this.recordTime = recordTime;
	}
	public String getModifier() {
		return modifier;
	}
	public void setModifier(String modifier) {
		this.modifier = modifier;
	}
	public Date getModifyTime() {
		return modifyTime;
	}
	public void setModifyTime(Date modifyTime) {
		this.modifyTime = modifyTime;
	}
	public String getRemark() {
		return remark;
	}
	public void setRemark(String remark) {
		this.remark = remark;
	}
    
    
    
    
    
    
    
    
    
}
```
 4.BaseUidEntity.java
```java
@SuppressWarnings("serial")
@MappedSuperclass
public class BaseUidEntity implements Serializable,Cloneable{
	@Id
	@GenericGenerator(name="uu_id", strategy="uuid")
	@GeneratedValue(generator="uu_id")
	private String id;
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public Object clone()
    {
        Object o=null;
        try
        {
            o=(Object)super.clone();
        }
        catch(CloneNotSupportedException e)
        {
            System.out.println(e.toString());
        }
        return o;
    }
		
	
}
```
5. UserDetail.java
```java
package org.nercita.bcp.system.domain.reference;
import java.util.Collection;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.userdetails.User;
/**
 * 登录用户详细信息
 * @author zhangwenchao
 *
 */
public class UserDetail extends User{
	
	private static final long serialVersionUID = -6889247576063361649L;
    private String userId;        //用户名（账号）
    private String realName;      //真实姓名
    
    public UserDetail(String username, String password,Collection<? extends GrantedAuthority> authorities) {
		super(username, password, authorities);
	}
	public UserDetail(String username, String password, boolean enabled,
			boolean accountNonExpired, boolean credentialsNonExpired,
			boolean accountNonLocked,
			Collection<? extends GrantedAuthority> authorities) {
		super(username, password, enabled, accountNonExpired, credentialsNonExpired,
				accountNonLocked, authorities);
	}
	public String getUserId() {
		return userId;
	}
	public void setUserId(String userId) {
		this.userId = userId;
	}
	public String getRealName() {
		return realName;
	}
	public void setRealName(String realName) {
		this.realName = realName;
	}
}
```
6.UserState.java
```java
package org.nercita.bcp.system.domain.reference;
public enum UserState {
    /**
     * 禁用(0)
     */
    Disable {
        @Override
        public String getTitle() {
            return "Disable";
        }
    },
    
    /**
     * 启用(1)
     */
    Enable {
        @Override
        public String getTitle() {
            return "Enable";
        }
    };
    @Override
    public String toString() {
        return this.getTitle();
    }
    public abstract String getTitle();
    public int getValue() {
        return this.ordinal();
    }
}
```

