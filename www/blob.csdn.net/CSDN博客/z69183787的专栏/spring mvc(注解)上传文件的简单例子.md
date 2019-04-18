# spring mvc(注解)上传文件的简单例子 - z69183787的专栏 - CSDN博客
2016年09月14日 11:00:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1131
 spring mvc(注解)上传文件的简单例子,这有几个需要注意的地方
1.form的enctype=”multipart/form-data” 这个是上传文件必须的
2.applicationContext.xml中 <bean id=”multipartResolver” class=”org.springframework.web.multipart.commons.CommonsMultipartResolver”/> 关于文件上传的配置不能少
大家可以看具体代码如下:
web.xml
**[html]**[view
 plain](http://blog.csdn.net/cheung1021/article/details/7084673#)[copy](http://blog.csdn.net/cheung1021/article/details/7084673#)
[print](http://blog.csdn.net/cheung1021/article/details/7084673#)[?](http://blog.csdn.net/cheung1021/article/details/7084673#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <web-appxmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns="http://java.sun.com/xml/ns/javaee"xmlns:web="http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"id="WebApp_ID"version="2.5">
- <display-name>webtest</display-name>
- 
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>
-             /WEB-INF/config/applicationContext.xml  
-             /WEB-INF/config/codeifAction.xml  
- </param-value>
- </context-param>
- 
- <servlet>
- <servlet-name>dispatcherServlet</servlet-name>
- <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
- <init-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>/WEB-INF/config/codeifAction.xml</param-value>
- </init-param>
- <load-on-startup>1</load-on-startup>
- </servlet>
- <!-- 拦截所有以do结尾的请求 -->
- <servlet-mapping>
- <servlet-name>dispatcherServlet</servlet-name>
- <url-pattern>*.do</url-pattern>
- </servlet-mapping>
- 
- <welcome-file-list>
- <welcome-file>index.do</welcome-file>
- </welcome-file-list>
- </web-app>
applicationContext.xml
**[html]**[view
 plain](http://blog.csdn.net/cheung1021/article/details/7084673#)[copy](http://blog.csdn.net/cheung1021/article/details/7084673#)
[print](http://blog.csdn.net/cheung1021/article/details/7084673#)[?](http://blog.csdn.net/cheung1021/article/details/7084673#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:p="http://www.springframework.org/schema/p"
- xmlns:context="http://www.springframework.org/schema/context"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
-     http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.0.xsd"  
- default-lazy-init="true">
- 
- <!-- 启动Spring MVC的注解功能，完成请求和注解POJO的映射 -->
- <beanclass="org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter"lazy-init="false"/>
- 
- <!-- 另外最好还要加入DefaultAnnotationHandlerMapping，不然会被 XML或其它的映射覆盖！ -->
- <beanclass="org.springframework.web.servlet.mvc.annotation.DefaultAnnotationHandlerMapping"/>
- 
- <!-- 对模型视图名称的解析，即在模型视图名称添加前后缀 -->
- <beanclass="org.springframework.web.servlet.view.InternalResourceViewResolver"p:prefix="/WEB-INF/jsp/"p:suffix=".jsp"/>
- 
- <!-- 支持上传文件 -->
- <beanid="multipartResolver"class="org.springframework.web.multipart.commons.CommonsMultipartResolver"/>
- 
- </beans>
codeifAction.xml
**[html]**[view
 plain](http://blog.csdn.net/cheung1021/article/details/7084673#)[copy](http://blog.csdn.net/cheung1021/article/details/7084673#)
[print](http://blog.csdn.net/cheung1021/article/details/7084673#)[?](http://blog.csdn.net/cheung1021/article/details/7084673#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:context="http://www.springframework.org/schema/context"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd  
-     http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-3.0.xsd"  
- default-lazy-init="true">
- 
- <beanid="uploadAction"class="com.codeif.action.UploadAction"/>
- 
- </beans>
UploadAction.java
**[java]**[view
 plain](http://blog.csdn.net/cheung1021/article/details/7084673#)[copy](http://blog.csdn.net/cheung1021/article/details/7084673#)
[print](http://blog.csdn.net/cheung1021/article/details/7084673#)[?](http://blog.csdn.net/cheung1021/article/details/7084673#)
- package com.codeif.action;  
- 
- import java.io.File;  
- import java.util.Date;  
- 
- import javax.servlet.http.HttpServletRequest;  
- 
- import org.springframework.stereotype.Controller;  
- import org.springframework.ui.ModelMap;  
- import org.springframework.web.bind.annotation.RequestMapping;  
- import org.springframework.web.bind.annotation.RequestParam;  
- import org.springframework.web.multipart.MultipartFile;  
- 
- @Controller
- publicclass UploadAction {  
- 
- @RequestMapping(value = "/upload.do")  
- public String upload(@RequestParam(value = "file", required = false) MultipartFile file, HttpServletRequest request, ModelMap model) {  
- 
-         System.out.println("开始");  
-         String path = request.getSession().getServletContext().getRealPath("upload");  
-         String fileName = file.getOriginalFilename();  
- //        String fileName = new Date().getTime()+".jpg";
-         System.out.println(path);  
-         File targetFile = new File(path, fileName);  
- if(!targetFile.exists()){  
-             targetFile.mkdirs();  
-         }  
- 
- //保存
- try {  
-             file.transferTo(targetFile);  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-         model.addAttribute("fileUrl", request.getContextPath()+"/upload/"+fileName);  
- 
- return"result";  
-     }  
- 
- }  
index.jsp
**[html]**[view
 plain](http://blog.csdn.net/cheung1021/article/details/7084673#)[copy](http://blog.csdn.net/cheung1021/article/details/7084673#)
[print](http://blog.csdn.net/cheung1021/article/details/7084673#)[?](http://blog.csdn.net/cheung1021/article/details/7084673#)
- <%@ page pageEncoding="utf-8"%>
- <!DOCTYPE html>
- <html>
- <head>
- <metacharset="utf-8">
- <title>上传图片</title>
- </head>
- <body>
- <formaction="upload.do"method="post"enctype="multipart/form-data">
- <inputtype="file"name="file"/><inputtype="submit"value="Submit"/></form>
- </body>
- </html>
WEB-INF/jsp/下的result.jsp
**[html]**[view
 plain](http://blog.csdn.net/cheung1021/article/details/7084673#)[copy](http://blog.csdn.net/cheung1021/article/details/7084673#)
[print](http://blog.csdn.net/cheung1021/article/details/7084673#)[?](http://blog.csdn.net/cheung1021/article/details/7084673#)
- <%@ page pageEncoding="utf-8"%>
- <!DOCTYPE html>
- <html>
- <head>
- <metacharset="utf-8">
- <title>上传结果</title>
- </head>
- <body>
- <imgalt=""src="${fileUrl }"/>
- </body>
- </html>
- 
