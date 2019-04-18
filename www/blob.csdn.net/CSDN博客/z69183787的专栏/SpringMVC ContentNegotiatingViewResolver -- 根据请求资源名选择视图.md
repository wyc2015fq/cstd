# SpringMVC ContentNegotiatingViewResolver -- 根据请求资源名选择视图 - z69183787的专栏 - CSDN博客
2016年10月11日 11:54:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：928

前一篇文章介绍了如何利用@ResponseBody和HTTP Request Header中的Accept属性实现返回结果中自动选择XML或者JSON。
这里介绍另外一种办法，利用使用ContentNegotiatingViewResolver，这个视图解析器允许你用同样的内容数据来呈现不同的view。
它支持如下面描述的三种方式:
1）使用扩展名
http://localhost:8080/employees/nego/Jack.xml  返回结果为XML
http://localhost:8080/employees/nego/Jack.json 返回结果为JSON
http://localhost:8080/employees/nego/Jack  使用默认view呈现，比如JSP
2) HTTP Request Header中的Accept
3) 使用参数
http://localhost:8080/employees/nego/Jack?format=xml  返回结果为XML
http://localhost:8080/employees/nego/Jack?format=json 返回结果为JSON
我们使用方式1来实现。
1）添加Controller方法
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48315523#)[copy](http://blog.csdn.net/fw0124/article/details/48315523#)
- /**
-  * Jack.xml -- 返回XML
-  * Jack.json -- 返回JSON
-  * Jack -- 返回JSP:employee.jsp
-  */
- @RequestMapping(value="/nego/{name}", method=RequestMethod.GET)  
- public String getEmployeeNegotiating(@PathVariable String name, ModelMap model) {  
-     model.addAttribute("emp", new EmployeeX(name, 16000));  
- return"employee";  
- }  
employee.jsp如下
**[java]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48315523#)[copy](http://blog.csdn.net/fw0124/article/details/48315523#)
- <%@ page language="java" contentType="text/html; charset=UTF-8" pageEncoding="UTF-8"%>  
- <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN""http://www.w3.org/TR/html4/loose.dtd">  
- <html>  
- <head>  
- <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">  
- <title>Employee</title>  
- </head>  
- <body>  
- Name : ${emp.name} <br/>  
- Salary : ${emp.salary}  
- </body>  
- </html>  
2）在spring-mvc.xml中添加如下配置
**[html]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48315523#)[copy](http://blog.csdn.net/fw0124/article/details/48315523#)
- <!-- ViewResolver -->
- <beanclass="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
- <propertyname="order"value="1"/>
- <propertyname="favorParameter"value="false"/>
- <propertyname="ignoreAcceptHeader"value="true"/>
- 
- <propertyname="mediaTypes">
- <map>
- <entrykey="json"value="application/json"/>
- <entrykey="xml"value="application/xml"/>
- </map>
- </property>
- 
- <propertyname="defaultViews">
- <list>
- <!-- JSON View -->
- <beanclass="org.springframework.web.servlet.view.json.MappingJackson2JsonView"/>
- <!-- JAXB XML View -->
- <beanclass="org.springframework.web.servlet.view.xml.MarshallingView">
- <constructor-arg>
- <beanclass="org.springframework.oxm.jaxb.Jaxb2Marshaller">
- <propertyname="classesToBeBound">
- <list>
- <value>learning.webapp.model.EmployeeX</value>
- </list>
- </property>
- </bean>
- </constructor-arg>
- </bean>
- </list>
- </property>
- </bean>
- 
- <!-- If no extension matched, use JSP view -->
- <beanclass="org.springframework.web.servlet.view.InternalResourceViewResolver">
- <propertyname="order"value="2"/>
- <propertyname="viewClass"value="org.springframework.web.servlet.view.JstlView"/>
- <propertyname="prefix"value="/WEB-INF/views/jsp/"/>
- <propertyname="suffix"value=".jsp"/>
- </bean>
有几个重要的参数：
<property name="favorPathExtension" value="true"></property> 是否启用扩展名支持，默认是true
<property name="favorParameter" value="false"></property> 是否启用参数支持，默认是true
<property name="ignoreAcceptHeader" value="true"></property> 是否忽略掉accept header，默认就是false
我们使用扩展名，所以关掉后面两个。
3）在POM添加添加下面2个依赖
**[html]**[view
 plain](http://blog.csdn.net/fw0124/article/details/48315523#)[copy](http://blog.csdn.net/fw0124/article/details/48315523#)
- <dependency>
- <groupId>com.fasterxml.jackson.core</groupId>
- <artifactId>jackson-databind</artifactId>
- <version>2.6.1</version>
- </dependency>
- <dependency>
- <groupId>org.springframework</groupId>
- <artifactId>spring-oxm</artifactId>
- <version>${spring.version}</version>
- </dependency>
打开浏览器，访问如下URL:
![](https://img-blog.csdn.net/20150909144538622)
**[参考资料]**
1）http://www.cnblogs.com/zhaoyang/archive/2012/01/07/2315428.html
2）http://www.mkyong.com/[spring](http://lib.csdn.net/base/javaee)-mvc/spring-3-mvc-contentnegotiatingviewresolver-example/
资料1中的ContentNegotiatingViewResolver处理流程图
![](https://img-blog.csdn.net/20150909144819505)

