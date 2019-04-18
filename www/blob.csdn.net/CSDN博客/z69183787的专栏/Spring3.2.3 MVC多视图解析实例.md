# Spring3.2.3 MVC多视图解析实例 - z69183787的专栏 - CSDN博客
2014年09月10日 19:00:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：841
个人分类：[SpringMvc](https://blog.csdn.net/z69183787/article/category/2176803)
这个架构当中，没有整合进数据库连接及事务管理等的配置，单纯介绍采用ContentNegotiatingViewResolver来做视图解析的一个配置实例，其它的一些配置，相信也难不倒各位亲。**源代码实例见附件。**
好了，我们接下来将按 
项目构建-->配置文件-->后端实现-->前端实现 来讲解。下面先从构建项目开始
**采用Gradle做项目构建**
这里只描述一些基本的要用到的构建语法，基础的应用是很简单的。 
1. 命令行下进入项目的根目录，即build.gradle配置文件所在的目录。 
2. 执行命令：gradle eclipse  自动下载build.gradle配置中的依赖包并生成eclipse的环境配置文件如：.project等等。 
3. 上步完成后，在eclipse导入现成项目即可。 
如果您想进一步了解gradle，可访问博文：[http://yidwo.iteye.com/blog/1848990](http://yidwo.iteye.com/blog/1848990)
或访问其官方网站：[http://gradle.org](http://gradle.org/)
Spring MVC的配置
web.xml:
Xml代码  ![收藏代码](http://yidwo.iteye.com/images/icon_star.png)
- <!-- Spring Web Dispatcher Configuration -->
- <servlet>
- <servlet-name>Dispatcher Servlet</servlet-name>
- <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
- <init-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>
-                 /WEB-INF/spring-web.xml  
- </param-value>
- </init-param>
- <load-on-startup>1</load-on-startup>
- </servlet>
- 
- <servlet-mapping>
- <servlet-name>Dispatcher Servlet</servlet-name>
- <url-pattern>/do/*</url-pattern>
- </servlet-mapping>
重点标注url-pattern部分，url的请求匹配，只要是满足的，都会被Spring mvc 适匹器捕获并处理。
mvc的配置
下面只是列出了主要的ContentNegotiatingViewResolver的相关配置：
Xml代码  ![收藏代码](http://yidwo.iteye.com/images/icon_star.png)
- <bean
- class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
- <propertyname="order"value="1"/>
- <propertyname="favorParameter"value="false"/>
- <propertyname="ignoreAcceptHeader"value="true"/>
- 
- <propertyname="defaultContentType"value="application/json"/>
- 
- <propertyname="mediaTypes">
- <map>
- <entrykey="atom"value="application/atom+xml"/>
- <entrykey="html"value="text/html"/><!-- 真正静态页面，使用htm后缀(文件格式) -->
- <entrykey="json"value="application/json"/>
- <entrykey="xml"value="application/xml"/>
- </map>
- </property>
- <propertyname="viewResolvers">
- <list>
- <beanclass="org.springframework.web.servlet.view.BeanNameViewResolver"/>
- <!-- 页面请求时先找此配置，无的话再找jsp的视图配置 -->
- <bean
- class="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
- <propertyname="cache"value="true"/>
- <propertyname="prefix"value=""/>
- <propertyname="suffix"value=".ftl"/>
- <propertyname="exposeSpringMacroHelpers"value="true"/>
- <propertyname="exposeRequestAttributes"value="true"/>
- <propertyname="exposeSessionAttributes"value="true"/>
- <propertyname="contentType"value="text/html;charset=UTF-8"/>
- </bean>
-     <!--   
- <bean
- class="org.springframework.web.servlet.view.InternalResourceViewResolver">
- <propertyname="viewClass"value="org.springframework.web.servlet.view.JstlView"/>
- <propertyname="prefix"value="/WEB-INF/jsp/"/>
- <propertyname="suffix"value=".jsp"/>
- </bean>
-             -->
- <beanclass="org.springframework.web.servlet.view.InternalResourceViewResolver">
- <propertyname="prefix"value="/WEB-INF/jsp/"/>
- <propertyname="suffix"value=".jsp"/>
- </bean>
- </list>
- </property>
- <!-- view 接口 下的视图解析对象 -->
- <propertyname="defaultViews">
- <list>
- <bean
- class="org.springframework.web.servlet.view.json.MappingJackson2JsonView"/>
-                 <!-- <beanclass="org.springframework.web.servlet.view.json.MappingJacksonJsonView"
- /> -->
- <!-- XML View -->
- <beanclass="org.springframework.web.servlet.view.xml.MarshallingView">
- <constructor-arg>
- <beanclass="org.springframework.oxm.jaxb.Jaxb2Marshaller">
- <propertyname="packagesToScan">
- <list>
- <value>com.sample.model</value>
- </list>
- </property>
- </bean>
- </constructor-arg>
- </bean>
- </list>
- </property>
- </bean>
默认视图配置为json格式 
Xml代码  ![收藏代码](http://yidwo.iteye.com/images/icon_star.png)
- <propertyname="defaultContentType"value="application/json"/>
这么做是因为现今Ajax技术用得比较多。 
查看spring-web.xml配置，里面有较详细的注释。 
后端REST注解实现
后端的实现实例详见SimpleController.java 
text/html
需要mediaType为text/html 的数据时，采用*.htm的uri请求，如：/do/sample.html请求sample.ftl页面或是sample.jsp页面，因为ftl的视图解析配置在jsp的视图解析之前，所以会先查找Freemarker的目录，没发现的话，则会继续查找jsp的目录。
application/json
因为默认配置数据格式为json，所以需要josn格式的数据时，uri可以不加后缀。以下json的请求都是有效的 /do/sample  和  /do/sample.json  请求处理完成后，直接返回客户端需要的数据对象，spring 会自动调用jackson组件将其转换成json格式。不用加
 @ResponseBody注解，是不是更清爽呀。
application/xml
配置采用JAXB做为xml数据的转换组件，只需要在javabean对象的getter方法上加上xml相关注解(详见TestModel.java)，在返回该对象及其包装对象时Spring自动转换成xml格式文档。
前端实现
由于我们在web.xml配置文件的url-pattern配置了/do/*. 所以在Controller的注解uri前面需要加上”/do”. 
更详细的实现见index.jsp 
参考：[http://static.springsource.org/spring/docs/3.2.x/spring-framework-reference/html/mvc.html#mvc-multiple-representations](http://static.springsource.org/spring/docs/3.2.x/spring-framework-reference/html/mvc.html#mvc-multiple-representations)
