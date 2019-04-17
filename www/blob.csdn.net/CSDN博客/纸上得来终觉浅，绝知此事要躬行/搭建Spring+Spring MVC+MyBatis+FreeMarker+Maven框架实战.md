# 搭建Spring+Spring MVC+MyBatis+FreeMarker+Maven框架实战 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月13日 00:24:48[boonya](https://me.csdn.net/boonya)阅读数：4286








原文地址：[http://xieke90.iteye.com/blog/2302912](http://xieke90.iteye.com/blog/2302912)

本文主要讲的内容是：实战讲解搭建Spring+Spring MVC+MyBatis+FreeMarker+Maven框架。

## 1.项目结构

![](http://dl2.iteye.com/upload/attachment/0117/8605/dfa4026c-aaea-3eff-bb0d-d6719251aa6f.png)

## **2.Maven pom.xml 配置**


```
<project xmlns="http://maven.apache.org/POM/4.0.0"   
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"   
        xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">  
      <modelVersion>4.0.0</modelVersion>  
      <groupId>com.xieke.test</groupId>  
      <artifactId>ssm-freemarker-demo</artifactId>  
      <version>0.0.1-SNAPSHOT</version>  
      <packaging>war</packaging>  
        
       <build>  
            <finalName>ssm-freemarker-demo</finalName>  
       </build>  
      
       <dependencies>  
           <dependency>  
               <groupId>junit</groupId>  
               <artifactId>junit</artifactId>  
               <version>4.11</version>  
               <scope>test</scope>  
           </dependency>  
      
           <!-- spring jar -->  
           <dependency>  
               <groupId>org.springframework</groupId>  
               <artifactId>spring-core</artifactId>  
               <version>3.2.6.RELEASE</version>  
           </dependency>  
           <dependency>  
               <groupId>org.springframework</groupId>  
               <artifactId>spring-context</artifactId>  
               <version>3.2.6.RELEASE</version>  
           </dependency>  
           <dependency>  
               <groupId>org.springframework</groupId>  
               <artifactId>spring-context-support</artifactId>  
               <version>3.2.6.RELEASE</version>  
           </dependency>  
           <dependency>  
               <groupId>org.springframework</groupId>  
               <artifactId>spring-tx</artifactId>  
               <version>3.2.6.RELEASE</version>  
           </dependency>  
           <dependency>  
               <groupId>org.springframework</groupId>  
               <artifactId>spring-aop</artifactId>  
               <version>3.2.6.RELEASE</version>  
           </dependency>  
           <dependency>  
               <groupId>org.springframework</groupId>  
               <artifactId>spring-beans</artifactId>  
               <version>3.2.6.RELEASE</version>  
           </dependency>  
           <dependency>  
               <groupId>org.springframework</groupId>  
               <artifactId>spring-jdbc</artifactId>  
               <version>3.2.6.RELEASE</version>  
           </dependency>  
      
           <!-- spring mvc jar -->  
           <dependency>  
               <groupId>org.springframework</groupId>  
               <artifactId>spring-web</artifactId>  
               <version>3.2.6.RELEASE</version>  
           </dependency>  
           <dependency>  
               <groupId>org.springframework</groupId>  
               <artifactId>spring-webmvc</artifactId>  
               <version>3.2.6.RELEASE</version>  
           </dependency>  
      
           <!-- mybatis jar -->  
           <dependency>  
               <groupId>org.mybatis</groupId>  
               <artifactId>mybatis</artifactId>  
               <version>3.2.4</version>  
           </dependency>  
      
           <!-- mybatis-spring jar -->  
           <dependency>  
               <groupId>org.mybatis</groupId>  
               <artifactId>mybatis-spring</artifactId>  
               <version>1.2.2</version>  
           </dependency>  
      
           <!-- mysql-connector-java jar -->  
           <dependency>  
               <groupId>mysql</groupId>  
               <artifactId>mysql-connector-java</artifactId>  
               <version>5.1.28</version>  
           </dependency>  
           <!-- 数据库连接池 jar -->  
           <dependency>  
               <groupId>commons-dbcp</groupId>  
               <artifactId>commons-dbcp</artifactId>  
               <version>1.4</version>  
           </dependency>  
           <dependency>  
               <groupId>commons-pool</groupId>  
               <artifactId>commons-pool</artifactId>  
               <version>1.6</version>  
           </dependency>  
      
           <!-- log jar -->  
           <dependency>  
               <groupId>log4j</groupId>  
               <artifactId>log4j</artifactId>  
               <version>1.2.17</version>  
           </dependency>  
           <dependency>  
               <groupId>org.slf4j</groupId>  
               <artifactId>slf4j-api</artifactId>  
               <version>1.7.5</version>  
           </dependency>  
           <dependency>  
               <groupId>org.slf4j</groupId>  
               <artifactId>slf4j-log4j12</artifactId>  
               <version>1.7.5</version>  
           </dependency>  
      
           <!-- jsp Template jar -->  
           <dependency>  
               <groupId>org.freemarker</groupId>  
               <artifactId>freemarker</artifactId>  
               <version>2.3.20</version>  
           </dependency>  
      
           <!-- apache servlet api jar -->  
           <dependency>  
               <groupId>org.apache.tomcat</groupId>  
               <artifactId>servlet-api</artifactId>  
               <version>6.0.37</version>  
           </dependency>  
       </dependencies>  
         
    </project>
```


## **3.项目 web.xml配置**


```
<?xml version="1.0" encoding="UTF-8"?>  
    <web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
        xmlns="http://java.sun.com/xml/ns/javaee" xmlns:web="http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"  
        xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_3_0.xsd"  
        version="3.0">  
      
        <!-- 配置log4j日志   -->    
        <context-param>    
         <param-name>log4jConfigLocation</param-name>    
         <param-value>classpath:log4j.properties</param-value>    
        </context-param>    
        <listener>    
         <listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>    
        </listener>    
      
        <!-- 字符集过滤器 -->  
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
         
        <!-- Spring mvc 配置，配置文件名称默认为{servlet-name}-servlet.xml，路径默认在/WEB-INF/下 -->  
        <servlet>  
            <servlet-name>springmvc</servlet-name>  
            <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>  
             <init-param>    
               <param-name>contextConfigLocation</param-name>    
               <param-value>classpath:spring/*.xml</param-value>    
             </init-param>    
            <load-on-startup>1</load-on-startup>  
        </servlet>  
        <servlet-mapping>  
            <servlet-name>springmvc</servlet-name>  
            <url-pattern>/</url-pattern>  
        </servlet-mapping>  
          
    </web-app>
```


## **4.Spring applicationContext.xml配置**

```
<?xml version="1.0" encoding="UTF-8"?>  
    <beans xmlns="http://www.springframework.org/schema/beans"    
           xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"    
           xmlns:p="http://www.springframework.org/schema/p"    
           xmlns:tx="http://www.springframework.org/schema/tx"    
           xmlns:aop="http://www.springframework.org/schema/aop"    
           xmlns:context="http://www.springframework.org/schema/context"    
           xsi:schemaLocation="http://www.springframework.org/schema/beans    
           http://www.springframework.org/schema/beans/spring-beans-3.0.xsd    
           http://www.springframework.org/schema/tx    
           http://www.springframework.org/schema/tx/spring-tx-3.0.xsd    
           http://www.springframework.org/schema/aop    
           http://www.springframework.org/schema/aop/spring-aop-3.0.xsd    
           http://www.springframework.org/schema/context    
           http://www.springframework.org/schema/context/spring-context-2.5.xsd"    
           default-init-method="init">  
                              
        <!-- 引入jdbc配置文件 -->    
        <context:property-placeholder location="classpath:jdbc.properties" />    
            
        <!--创建jdbc数据源 -->    
        <bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource" destroy-method="close">    
            <property name="driverClassName" value="${driver}" />    
            <property name="url" value="${url}" />    
            <property name="username" value="${username}" />    
            <property name="password" value="${password}" />    
        </bean>    
         
        <!-- 创建SqlSessionFactory，同时指定数据源 -->    
        <bean id="sqlSessionFactory" class="org.mybatis.spring.SqlSessionFactoryBean">    
            <property name="dataSource" ref="dataSource" />    
            <!-- 配置sql映射文件所在位置  注意：默认与mapper类位置相同   -->    
            <property name="mapperLocations" value="classpath:sqlmap/*.xml" />    
        </bean>    
         
        <!-- 配置事务管理器，全注解声明式事务  -->    
        <bean id="transactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">    
            <property name="dataSource" ref="dataSource" />    
        </bean>    
        <tx:annotation-driven transaction-manager="transactionManager" />    
           
        <!-- Mapper接口所在包名，Spring会自动查找其下的Mapper -->    
        <bean class="org.mybatis.spring.mapper.MapperScannerConfigurer">    
            <property name="basePackage" value="com.xieke.test.mapper" />    
            <property name="sqlSessionFactoryBeanName" value="sqlSessionFactory" />    
            <!-- 默认情况下会自动注入id=sqlSessionFactory的bean，也可以按上述方式指定sqlSessionFactory -->    
        </bean>    
      
    </beans>
```

## **5.Spring MVC springmvc-servlet.xml配置**


```
<?xml version="1.0" encoding="UTF-8"?>  
    <beans xmlns="http://www.springframework.org/schema/beans"  
        xmlns:aop="http://www.springframework.org/schema/aop" xmlns:context="http://www.springframework.org/schema/context"  
        xmlns:mvc="http://www.springframework.org/schema/mvc" xmlns:tx="http://www.springframework.org/schema/tx"  
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
        xsi:schemaLocation="http://www.springframework.org/schema/aop     
            http://www.springframework.org/schema/aop/spring-aop-3.0.xsd     
            http://www.springframework.org/schema/beans     
            http://www.springframework.org/schema/beans/spring-beans-3.0.xsd     
            http://www.springframework.org/schema/context     
            http://www.springframework.org/schema/context/spring-context-3.0.xsd     
            http://www.springframework.org/schema/mvc     
            http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd     
            http://www.springframework.org/schema/tx     
            http://www.springframework.org/schema/tx/spring-tx-3.0.xsd" >  
      
        <!-- 扫描com.xieke.test包下的所有class文件，配置注解的类全都装入容器中进行管理 -->  
        <context:component-scan base-package="com.xieke.test" />  
        <!-- 添加注解驱动 -->  
        <mvc:annotation-driven />  
        <!-- 静态资源处理 -->  
        <mvc:resources mapping="/**" location="/"/>  
          
        <mvc:view-controller path="/" view-name="redirect:/user/all"/>    
      
        <!-- FreeMarker视图解析器  -->  
        <bean id="freemarkerResolver" class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">  
            <property name="order" value="1" />  
            <property name="suffix" value=".ftl" />  
            <property name="contentType" value="text/html;charset=utf-8" />  
            <property name="viewClass">  
                <value>org.springframework.web.servlet.view.freemarker.FreeMarkerView</value>  
            </property>  
        </bean>  
          
        <!-- freemarker的配置 -->    
        <bean id="freemarkerConfig" class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">  
            <property name="templateLoaderPath">  
                <value>/WEB-INF/views/</value>  
            </property>  
            <property name="freemarkerSettings"><!-- 设置FreeMarker环境属性 -->  
                <props>  
                    <prop key="template_update_delay">5</prop><!--刷新模板的周期，单位为秒 -->  
                    <prop key="default_encoding">UTF-8</prop><!--模板的编码格式 -->  
                    <prop key="locale">UTF-8</prop><!-- 本地化设置 -->  
                    <prop key="datetime_format">yyyy-MM-dd HH:mm:ss</prop>  
                    <prop key="time_format">HH:mm:ss</prop>  
                    <prop key="number_format">0.####</prop>  
                    <prop key="boolean_format">true,false</prop>  
                    <prop key="whitespace_stripping">true</prop>  
                    <prop key="tag_syntax">auto_detect</prop>  
                    <prop key="url_escaping_charset">UTF-8</prop>  
                </props>  
            </property>  
        </bean>  
          
    </beans>
```


至此搭建完毕，以上是主要的配置文件。

## 6.示例代码下载

想详细了解的，可以下载查看源代码，下载地址为：  [http://pan.baidu.com/s/1c7e8eM](http://pan.baidu.com/s/1c7e8eM)



