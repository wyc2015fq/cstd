# Maven+SpringMVC+Freemarker入门示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月13日 00:23:52[boonya](https://me.csdn.net/boonya)阅读数：1507








原文地址：[http://blog.csdn.net/haishu_zheng/article/details/51523623](http://blog.csdn.net/haishu_zheng/article/details/51523623)


 FreeMarker是一个用Java语言编写的模板引擎，它基于模板来生成文本输出。官网：[http://freemarker.org/](http://freemarker.org/)


模板 + 数据模型 = 输出 


![](http://static.oschina.net/uploads/space/2013/0108/100909_oCtt_154489.jpg)




1.方式一：参考：[http://blog.csdn.net/haishu_zheng/article/details/51490299](http://blog.csdn.net/haishu_zheng/article/details/51490299)实现


 2.方式二：创建一个名为mavenspringmvcfreemarker的Maven工程，文件目录结构如下图所示

![](https://img-blog.csdn.net/20160528101443183)




3 在pom.xml中添加springmvc和freemarker的依赖包，添加完之后的完整内容为


```
<project xmlns="http://maven.apache.org/POM/4.0.0"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
        xsi:schemaLocation="http://maven.apache.org/POM/4.0.0http://maven.apache.org/xsd/maven-4.0.0.xsd">  
        <modelVersion>4.0.0</modelVersion>  
        <groupId>mavenspringmvcfreemarker</groupId>  
        <artifactId>mavenspringmvcfreemarker</artifactId>  
        <version>0.0.1-SNAPSHOT</version>  
        <packaging>war</packaging>  
        <name>mavenspringmvcfreemarker</name>  
        <description />  
        <properties>  
            <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>  
        </properties>  
        <dependencies>  
            <dependency>  
                <groupId>org.freemarker</groupId>  
                <artifactId>freemarker</artifactId>  
                <version>2.3.20</version>  
            </dependency>  
            <dependency>  
                <groupId>org.springframework</groupId>  
                <artifactId>spring-context-support</artifactId>  
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
                <groupId>javax</groupId>  
                <artifactId>javaee-api</artifactId>  
                <version>7.0</version>  
                <scope>provided</scope>  
            </dependency>  
            <dependency>  
                <groupId>org.glassfish.web</groupId>  
                <artifactId>javax.servlet.jsp.jstl</artifactId>  
                <version>1.2.2</version>  
            </dependency>  
        </dependencies>  
        <build>  
            <plugins>  
                <plugin>  
                    <artifactId>maven-compiler-plugin</artifactId>  
                    <version>2.3.2</version>  
                    <configuration>  
                        <source>1.7</source>  
                        <target>1.7</target>  
                    </configuration>  
                </plugin>  
                <plugin>  
                    <artifactId>maven-war-plugin</artifactId>  
                    <version>2.2</version>  
                    <configuration>  
                        <version>3.1</version>  
                        <failOnMissingWebXml>false</failOnMissingWebXml>  
                    </configuration>  
                </plugin>  
            </plugins>  
        </build>  
    </project>
```
4. web.xml中的完整内容为:




```
<?xml version="1.0"encoding="UTF-8"?>  
    <web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns="http://xmlns.jcp.org/xml/ns/javaee"xsi:schemaLocation="http://xmlns.jcp.org/xml/ns/javaeehttp://xmlns.jcp.org/xml/ns/javaee/web-app_3_1.xsd" id="WebApp_ID" version="3.1">  
      <display-name>mavenspringmvcfreemarker</display-name>  
      <servlet>  
        <servlet-name>spring</servlet-name>  
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>  
        <load-on-startup>1</load-on-startup>  
      </servlet>  
      <servlet-mapping>  
        <servlet-name>spring</servlet-name>  
        <url-pattern>/</url-pattern>  
      </servlet-mapping>  
    </web-app>
```
5. spring-servlet.xml中的完整内容为：





```
<beans xmlns="http://www.springframework.org/schema/beans"  
        xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context"  
        xsi:schemaLocation="http://www.springframework.org/schema/beans  
           http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
            http://www.springframework.org/schema/context  
           http://www.springframework.org/schema/context/spring-context-3.0.xsd">  
      
        <context:component-scan base-package="com.demo" />  
      
        <bean  
            class="org.springframework.web.servlet.mvc.annotation.DefaultAnnotationHandlerMapping" />  
        <bean  
            class="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter" />  
      
        <!-- 配置Freemarker属性文件路径 -->  
        <bean id="freemarkerConfiguration"  
            class="org.springframework.beans.factory.config.PropertiesFactoryBean">  
            <property name="location" value="classpath:conf/freemarker.properties" />  
        </bean>  
        <!-- 配置freeMarker模板加载地址 -->  
        <bean id="freemarkerConfig"  
            class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">  
            <!-- 视图解析器在/WEB-INF/ftl/路径下扫描视图文件 -->  
            <property name="templateLoaderPath" value="/WEB-INF/ftl/" />  
            <property name="freemarkerVariables">  
                <map>  
                    <entry key="xml_escape" value-ref="fmXmlEscape" />  
                </map>  
            </property>  
        </bean>  
        <bean id="fmXmlEscape" class="freemarker.template.utility.XmlEscape" />  
        <!-- 配置freeMarker视图解析器 -->  
        <bean id="freemakerViewResolver"  
            class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">  
            <property name="viewClass"  
                value="org.springframework.web.servlet.view.freemarker.FreeMarkerView" />  
            <!-- 扫描路径內所有以ftl結尾的文件 -->  
            <property name="viewNames">  
                <array>  
                    <value>*.ftl</value>  
                </array>  
            </property>  
            <property name="contentType" value="text/html; charset=UTF-8" />  
            <property name="exposeRequestAttributes" value="true" />  
            <property name="exposeSessionAttributes" value="true" />  
            <property name="exposeSpringMacroHelpers" value="true" />  
            <property name="requestContextAttribute" value="request" />  
            <!-- 给视图解析器配置优先級，你可以给之前jsp视图解析器的值配为2 -->  
            <property name="order" value="1" />  
        </bean>  
      
    </beans>
```






注意：web.xml中，有个<servlet-name>为spring，所以这个文件起名为spring-servlet.xml。这样，程序自动会去加载spring-servlet.xml。也就说<xxx>-servlet.xml中的<xxx>必须与<servlet-name>的值完全对应。

不对应当然也可以，但是要在web.xml中显示指定加载。

比如：把spring-servlet.xml改名为applicationContext.xml，则要在web.xml中的</servlet-class>下方加代码：


```
<init-param>  
        <param-name>contextConfigLocation</param-name>  
        <param-value>/WEB-INF/applicationContext.xml</param-value>  
    </init-param>
```
6. freemarker.properties中的完整内容为




```
tag_syntax=auto_detect  
    template_update_delay=2  
    default_encoding=UTF-8  
    output_encoding=UTF-8  
    locale=zh_CN  
    date_format=yyyy-MM-dd  
    time_format=HH:mm:ss  
    datetime_format=yyyy-MM-dd HH\:mm\:ss
```



7. StudentController.java中的完整代码为：



```java
package com.demo.controller;  
       
    import org.springframework.stereotype.Controller;  
    import org.springframework.web.bind.annotation.RequestMapping;  
    import org.springframework.ui.Model;  
       
    @Controller  
    public class StudentController {  
       
        @RequestMapping("/helloWorld")  
        public String helloWorld(Model model) {  
            String word0 = "Hello ";  
            String word1 = "World!";  
            //将数据添加到视图数据容器中  
            model.addAttribute("word0",word0);  
            model.addAttribute("word1",word1);  
            return "Hello.ftl";  
        }  
    }
```





8 Hello.ftl中的完整代码为：


```
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">  
<html>  
<head>  
<meta http-equiv="Content-Type"content="text/html; charset=UTF-8">  
<title>Insert title here</title>  
</head>  
<body>  
<h2>${word0}${word1}</h2>  
</body>  
</html>
```



9. 将mavenspringmvcfreemarker添加进Tomcat 7中并运行

![](https://img-blog.csdn.net/20160528101714975)




在浏览器中输入

[http://localhost:8080/mavenspringmvcfreemarker/helloWorld](http://localhost:8080/mavenspringmvcfreemarker/helloWorld)



显示结果为

![](https://img-blog.csdn.net/20160528101732452)





10 .源码下载地址

CSDN：[http://download.csdn.net/detail/haishu_zheng/9533679](http://download.csdn.net/detail/haishu_zheng/9533679)

Github：[https://github.com/zhenghaishu/Maven-SpringMVC-Freemarker-Demo](https://github.com/zhenghaishu/Maven-SpringMVC-Freemarker-Demo)



