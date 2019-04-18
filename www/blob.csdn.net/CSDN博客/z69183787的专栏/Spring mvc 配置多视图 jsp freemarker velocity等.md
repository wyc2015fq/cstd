# Spring mvc 配置多视图 jsp freemarker velocity等 - z69183787的专栏 - CSDN博客
2014年09月10日 19:11:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3486
spring-servlet：注（freemark 的 viewresolver 一定要放在 jsp 的上面，不然不能匹配ftl ，不知道为什么）
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context"
	xmlns:mvc="http://www.springframework.org/schema/mvc"
	xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.2.xsd
        http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc-3.2.xsd">
	<context:component-scan base-package="test.**.controller"
		use-default-filters="false">
		<context:include-filter type="annotation"
			expression="org.springframework.stereotype.Controller" />
	</context:component-scan>
	<mvc:resources location="/resources/" mapping="/resources/**" />
	<mvc:annotation-driven>
		<!-- <mvc:argument-resolvers> <bean class="org.springframework.data.web.PagedResourcesAssemblerArgumentResolver" 
			/> </mvc:argument-resolvers> -->
		<mvc:message-converters>
			<bean class="org.springframework.http.converter.StringHttpMessageConverter">
				<property name="supportedMediaTypes">
					<list>
						<value>text/plain;charset=UTF-8</value>
						<value>text/html;charset=UTF-8</value>
					</list>
				</property>
			</bean>
			<bean
				class="org.springframework.http.converter.json.MappingJackson2HttpMessageConverter">
				<property name="supportedMediaTypes">
					<list>
						<value>text/html;charset=UTF-8</value>
						<value>application/json;charset=UTF-8</value>
					</list>
				</property>
			</bean>
		</mvc:message-converters>
	</mvc:annotation-driven>
	<bean id="multipartResolver"
		class="org.springframework.web.multipart.commons.CommonsMultipartResolver" />
	<bean id="freemarkerConfig"
		class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">
		<property name="templateLoaderPath">  
            <value>/ftl/</value>  
        </property>  
		<property name="defaultEncoding" value="UTF-8" />
		<property name="freemarkerVariables">
			<map>
				<entry key="xml_escape" value-ref="fmXmlEscape" />
			</map>
		</property>
		<property name="freemarkerSettings">
			<props>
				<!--<prop key="template_update_delay">3600</prop> -->
				<prop key="tag_syntax">auto_detect</prop>
				<!-- 设置标签类型 两种：[] 和 <> 。[] 这种标记解析要快些 -->
				<prop key="default_encoding">UTF-8</prop>
				<prop key="output_encoding">UTF-8</prop>
				<prop key="locale">zh_CN</prop>
				<prop key="date_format">yyyy-MM-dd</prop>
				<prop key="time_format">HH:mm:ss</prop>
				<prop key="datetime_format">yyyy-MM-dd HH:mm:ss</prop>
				<prop key="number_format">#.##</prop>
				<!-- 设置数字格式 以免出现 000.00 -->
				<prop key="classic_compatible">true</prop>
				<!-- 可以满足一般需要。默认情况变量为null则替换为空字符串，如果需要自定义，写上${empty!"EmptyValue of fbysss"}的形式即可 -->
				<prop key="template_exception_handler">html_debug</prop>
				<!-- ignore,debug,html_debug,rethrow -->
			</props>
		</property>
	</bean>
	<bean id="fmXmlEscape" class="freemarker.template.utility.XmlEscape" />
	<!-- FreeMarker视图解析 如返回student。。在这里配置后缀名ftl和视图解析器。。
	<bean id="viewResolver"
		class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
		<property name="viewClass"
			value="org.springframework.web.servlet.view.freemarker.FreeMarkerView"></property>
		<property name="suffix" value=".ftl" />
		<property name="contentType" value="text/html;charset=utf-8" />
		<property name="exposeRequestAttributes" value="true" />
		<property name="exposeSessionAttributes" value="true" />
		<property name="exposeSpringMacroHelpers" value="true" />
		<property name="order" value="2" />
	</bean> 
	<bean id="JSPviewResolver"
		class="org.springframework.web.servlet.view.InternalResourceViewResolver">
		<property name="viewClass"
			value="org.springframework.web.servlet.view.JstlView" />
		<property name="prefix" value="/jsp/" />
		<property name="suffix" value=".jsp" />
	</bean>
	<bean id="beanNameViewResolver"
		class="org.springframework.web.servlet.view.BeanNameViewResolver" />-->
	<!-- Velocity ViewResolver Configuration 
	<bean id="velocityViewResolver"
		class="org.springframework.web.servlet.view.velocity.VelocityViewResolver">
		<property name="order" value="0" />
		<property name="contentType" value="text/html;charset=UTF-8" />
		<property name="requestContextAttribute" value="req" />
	</bean>-->
	<!-- Velocity Configuration
	<bean id="velocityConfig"
		class="org.springframework.web.servlet.view.velocity.VelocityConfigurer">
		<property name="configLocation" value="/WEB-INF/classes/velocity.properties" />
		<property name="resourceLoaderPath" value="/" />
	</bean> -->
 	<bean id="viewResolver"
		class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
		<property name="defaultContentType" value="application/json" />
		<property name="mediaTypes">
			<map>
				<entry key="html" value="text/html" />
				<entry key="json" value="application/json" />
				<entry key="xml" value="application/xml" />
				<entry key="excel" value="application/excel" />
				<entry key="pdf" value="application/pdf" />
			</map>
		</property>
		<property name="viewResolvers">
			<list>
			<bean
					class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
					<property name="viewClass"
						value="org.springframework.web.servlet.view.freemarker.FreeMarkerView"></property>
					<property name="suffix" value=".ftl" />
					<property name="contentType" value="text/html;charset=utf-8" />
					<property name="exposeRequestAttributes" value="true" />
					<property name="exposeSessionAttributes" value="true" />
					<property name="exposeSpringMacroHelpers" value="true" />
				</bean>
				<bean
					class="org.springframework.web.servlet.view.InternalResourceViewResolver">
					<property name="viewClass"
						value="org.springframework.web.servlet.view.JstlView" />
					<property name="prefix" value="/jsp/" />
					<property name="suffix" value=".jsp" />
				</bean>
				
			</list>
		</property>
		<property name="defaultViews">
			<list>
				<bean
					class="org.springframework.web.servlet.view.json.MappingJackson2JsonView">
					<property name="prettyPrint" value="true" />
					<property name="contentType" value="text/plain" />
				</bean>
			</list>
		</property>
	</bean>
	<!--定义异常处理页面 -->
	<bean id="exceptionResolver"
		class="org.springframework.web.servlet.handler.SimpleMappingExceptionResolver">
		<property name="exceptionMappings">
			<props>
				<prop key="java.lang.Exception">outException</prop>
			</props>
		</property>
	</bean>
	<mvc:default-servlet-handler />
</beans>
```
pom.xml:
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
			<artifactId>spring-context-support</artifactId>
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
		<!-- freemarker -->
		<dependency>
			<groupId>org.freemarker</groupId>
			<artifactId>freemarker</artifactId>
			<version>2.3.20</version>
		</dependency>
		<!-- velocity -->
		<dependency>
			<groupId>org.apache.velocity</groupId>
			<artifactId>velocity</artifactId>
			<version>1.7</version>
		</dependency>
	</dependencies>
	<build>
		<finalName>mvc-parent</finalName>
	</build>
</project>
```
