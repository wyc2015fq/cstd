# ***Spring MVC的多视图解析器配置及与Freemarker的集成 - z69183787的专栏 - CSDN博客
2014年12月01日 22:47:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12433
http://my249645546.iteye.com/blog/1426952
一、从freemarker谈起
Freemarker使用模板技术进行视图的渲染。自从看了Struts标签、Freemarker、JSTL的性能对比后，我毅然决定放弃Struts标签了！效率太差……
Spring本身支持了对Freemarker的集成。只需要配置一个针对Freemarker的视图解析器即可。
二、Spring MVC视图解析器
视图解析器的工作流程大致是这样的：Controller的某个方法执行完成以后，返回一个视图（比如：listUser），视图解析器要做的工作就是找到某个对象来完成视图的渲染，或者跳转到其他的逻辑视图。这里的渲染对象通常就是我们的jsp文件或者我们下面用的Freemarker(例如listUser.jsp或者listUser.ftl)。渲染完成以后，将解析结果发送到客户端浏览器
下面介绍一下本文需要用到的解析器（更多解析器资料，请参考 [http://e-freya.iteye.com/blog/384083](http://e-freya.iteye.com/blog/384083) ）：
InternalResourceViewResolver：这是一个最常用的解析器。通常使用它指定渲染对象为jsp页面
FreeMarkerViewResolver：这就是Spring与Freemarker整合需要用到的解析器
三、配置多视图，支持freemarker
我们通常不希望所有的动态页面请求都使用Freemarker来渲染，那就需要配置多个视图解析器。网上有很多这方面的帖子。我看到很多人的做法是在web.xml中配置两个DispatcherServlet,一个拦截*.do，一个拦截*.ftl;然后再写两个dispatcherServlet.xml,配置两个视图解析器；jsp页面、ftl模板就各司其职。
其实没有那么复杂。
1.Web.xml
Xml代码  ![收藏代码](http://image58.360doc.com/DownloadImg/2013/01/0813/29465106_1.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <web-appversion="2.4"xmlns="http://java.sun.com/xml/ns/j2ee"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://java.sun.com/xml/ns/j2ee   
-     http://java.sun.com/xml/ns/j2ee/web-app_2_4.xsd">
- <welcome-file-list>
- <welcome-file>index.jsp</welcome-file>
- </welcome-file-list>
- 
- <!-- ======================================================== -->
- <!--                  Spring MVC Config Servlet               -->
- <!-- ======================================================== -->
- <!-- JSP DispatcherServlet -->
- <servlet>
- <servlet-name>dispatcher</servlet-name>
- <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
- <init-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>/WEB-INF/classes/applicationContext.xml</param-value>
- </init-param>
- <load-on-startup>1</load-on-startup>
- </servlet>
- 
- <!-- ======================================================== -->
- <!--                  Spring MVC Config Mapping               -->
- <!-- ======================================================== -->
- <servlet-mapping>
- <servlet-name>dispatcher</servlet-name>
- <url-pattern>*.do</url-pattern>
- </servlet-mapping>
- </web-app>
2.dispatcherServlet.xml
Xml代码  ![收藏代码](http://image58.360doc.com/DownloadImg/2013/01/0813/29465106_1.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:aop="http://www.springframework.org/schema/aop"
- xmlns:tx="http://www.springframework.org/schema/tx"
- xmlns:p="http://www.springframework.org/schema/p"
- xmlns:context="http://www.springframework.org/schema/context"
- xmlns:mvc="http://www.springframework.org/schema/mvc"
- xsi:schemaLocation="  
-   http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd  
-   http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.5.xsd  
-   http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd  
-   http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-2.5.xsd  
-   http://www.springframework.org/schema/mvc  http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd">
- 
- 
- <!--通用视图解析器-->
- <beanid="viewResolverCommon"class="org.springframework.web.servlet.view.InternalResourceViewResolver">
- <propertyname="prefix"value="/WEB-INF/page/"/>
- <propertyname="suffix"value=".jsp"/><!--可为空,方便实现自已的依据扩展名来选择视图解释类的逻辑  -->
- <propertyname="viewClass">
- <value>org.springframework.web.servlet.view.InternalResourceView</value>
- </property>
- <propertyname="order"value="1"/>
- </bean>
- 
- <!-- 配置freeMarker视图解析器 -->
- <beanid="viewResolverFtl"class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
- <propertyname="viewClass"value="org.springframework.web.servlet.view.freemarker.FreeMarkerView"/>
- <propertyname="contentType"value="text/html; charset=utf-8"/>
- <propertyname="cache"value="true"/>
- <propertyname="suffix"value=".ftl"/>
- <propertyname="order"value="0"/>
- </bean>
- 
- <!-- 配置freeMarker的模板路径 -->
- <beanid="freemarkerConfig"class="org.springframework.web.servlet.view.freemarker.FreeMarkerConfigurer">
- <propertyname="templateLoaderPath">
- <value>/WEB-INF/ftl/</value>
- </property>
- <propertyname="freemarkerVariables">
- <map>
- <entrykey="xml_escape"value-ref="fmXmlEscape"/>
- </map>
- </property>
- <propertyname="defaultEncoding">
- <value>utf-8</value>
- </property>
- <propertyname="freemarkerSettings">
- <props>
- <propkey="template_update_delay">3600</prop>
- </props>
- </property>
- </bean>
- 
- <beanid="fmXmlEscape"class="freemarker.template.utility.XmlEscape"/>
- 
- <!-- 注解支持 -->
- <mvc:annotation-driven/>
- 
- <!-- 对包中的所有类进行扫描，以完成Bean创建和自动依赖注入的功能 -->
- <context:component-scanbase-package="com.hl.usersmanager">
-         <!-- 允许定义过滤器将基包下的某些类纳入或排除  
- <context:include-filtertype="annotation"expression="org.springframework.stereotype.Controller"/> -->
- </context:component-scan>
- 
- </beans>
3.Controller
Java代码  ![收藏代码](http://image58.360doc.com/DownloadImg/2013/01/0813/29465106_1.png)
- package com.hl.usersmanager.controller;  
- 
- import java.util.List;  
- 
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.stereotype.Controller;  
- import org.springframework.ui.ModelMap;  
- import org.springframework.web.bind.annotation.RequestMapping;  
- 
- import com.hl.usersmanager.model.Users;  
- import com.hl.usersmanager.service.IUserService;  
- 
- @Controller
- publicclass UserController {  
- //  使用注解实现自动装配 不需要再写get set方法以及在context中配置bean
- @Autowired
- private IUserService userService;  
- 
- @RequestMapping(value = "findUserByName.do")  
- public String findUserByName(String name,ModelMap model) {  
-         Users users = userService.findUserByName(name);  
-         model.addAttribute("userPhone",users.getPhone());  
-         System.out.println("userPhone：" + users.getPhone());  
- return"showUser";  
-     }  
- 
- @RequestMapping(value = "findAllUsers.do")  
- public String findAllUsers(ModelMap model) {  
-         List<Users> users = userService.findAllUsers();  
-         model.addAttribute("users",users);  
- return"listUser";  
-     }  
- 
-     ……  
- }  
在视图解析器中有一个<property name="order" value="orderValue"/>的配置，这个配置表示解析器的优先级别。我们将FreeMarkerViewResolver的级别设为0,将InternalResourceViewResolver的级别设为1。这样，解析器就会优先使用 FreeMarkerViewResolver 进行解析，如果找不到相应的模板，就使用InternalResourceViewResolver进行解析，如果还找不到页面，就会产生一个404错误！
在本例中，我们在/WEB-INF/page/下有一个showUser.jsp页面，在/WEB-INF/ftl/下有一个listUser.ftl的模板文件。那么当访问findAllUsers.do的时候，Controller返回一个listUser视图，根据解析器配置，先使用FreeMarkerViewResolver进行解析。它会根据模板路径templateLoaderPath的配置/WEB-INF/ftl/下去找是否有一个listUser并且以后缀配置suffix值.ftl，即listUser.ftl文件，如果找到则使用该模板进行解析。这里我们实现已经创建了这个模板文件，所以user列表成功被显示出来。
当用户访问findUserByName.do的时候，返回showUser视图，毅然先使用FreeMarkerViewResolver进行解析，结果发现在/WEB-INF/ftl/下并没有showUser.ftl这个模板文件，于是使用InternalResourceViewResolver进行解析，于是开始寻找/WEB-INF/page/下是否有showUser.jsp文件。由于我们已经创建了这个文件，于是最终使用showUser.jsp进行渲染。那么如果没有找到showUser.jsp，就会抛出404错误。
这里还要注意的是，如果Controller中返回视图加了后缀jsp或者ftl，在配置中就不要加入suffix配置，否则会找不到页面。
