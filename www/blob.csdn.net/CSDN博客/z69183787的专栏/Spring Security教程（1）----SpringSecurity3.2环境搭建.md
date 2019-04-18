# Spring Security教程（1）----SpringSecurity3.2环境搭建 - z69183787的专栏 - CSDN博客
2014年03月13日 21:19:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：21066
目前Spring官方只提供Meven的下载方式。但在[http://maven.springframework.org](http://maven.springframework.org/)中有SpringSecurity及其他所有Spring产品的下载方式。
[http://maven.springframework.org/release/org/springframework/](http://maven.springframework.org/release/org/springframework/)中有Spring相关的所有下载，但好像直到3.2版的，最新的版本在这个里面找不到
[](http://maven.springframework.org/release/org/springframework/security/spring-security-web/3.2.0.RELEASE/)[http://maven.springframework.org/release/org/springframework/security/spring-security/3.2.0.RELEASE/](http://maven.springframework.org/release/org/springframework/security/spring-security/3.2.0.RELEASE/)这个是SpringSecurity3.2的下载地址
Meven下载地址：
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/17640071#)[copy](http://blog.csdn.net/jaune161/article/details/17640071#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/132526/fork)
- <dependencies>
- <dependency>
- <groupId>org.springframework.security</groupId>
- <artifactId>spring-security-web</artifactId>
- <version>3.2.0.RELEASE</version>
- </dependency>
- <dependency>
- <groupId>org.springframework.security</groupId>
- <artifactId>spring-security-config</artifactId>
- <version>3.2.0.RELEASE</version>
- </dependency>
- </dependencies>
本教程是基于SpringMVC3.2+Hibernate4+JPA2.0+SpringSecurity3.2的环境。SpringMVC3.2+Hibernate4+JPA2.0环境的搭建在这里就不多说了，主要讲下SpringSecurity的环境搭建
web.xml配置
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/17640071#)[copy](http://blog.csdn.net/jaune161/article/details/17640071#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/132526/fork)
- <!-- 加载Spring的配置文件 -->
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>
-         classpath:applicationContext.xml,  
-         classpath:applicationContext-security.xml  
- </param-value>
- </context-param>
- 
- <!-- SpringSecurity 核心过滤器配置 -->
- <filter>
- <filter-name>springSecurityFilterChain</filter-name>
- <filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>springSecurityFilterChain</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
applicationContext-security.xml命名空间配置，官方提供了两种配置方案
第一种、命名空间用beans开头，但是在配置中一直需要用<security:*>来配置。
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/17640071#)[copy](http://blog.csdn.net/jaune161/article/details/17640071#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/132526/fork)
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:security="http://www.springframework.org/schema/security"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-           http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
-           http://www.springframework.org/schema/security  
-           http://www.springframework.org/schema/security/spring-security.xsd">
-     ...  
- </beans>
第二种、命名空间用security开头，在配置中不需要security前缀，但是bean的配置需要用<beans:bean>配置。
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/17640071#)[copy](http://blog.csdn.net/jaune161/article/details/17640071#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/132526/fork)
- <beans:beansxmlns="http://www.springframework.org/schema/security"
- xmlns:beans="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-            http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
-            http://www.springframework.org/schema/security  
-            http://www.springframework.org/schema/security/spring-security.xsd">
-     ...  
- </beans:beans>
到此为止SpringSecurity的环境配置已基本完成
命名空间的配置可在spring的官方文档，第4章 Security Namespace Configuration 中找到，一下附上链接地址
[http://docs.spring.io/spring-security/site/docs/3.2.0.RELEASE/reference/htmlsingle/#ns-config](http://docs.spring.io/spring-security/site/docs/3.2.0.RELEASE/reference/htmlsingle/#ns-config)
