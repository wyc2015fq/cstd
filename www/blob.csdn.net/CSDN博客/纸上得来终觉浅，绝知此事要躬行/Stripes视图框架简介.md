# Stripes视图框架简介 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月05日 14:53:24[boonya](https://me.csdn.net/boonya)阅读数：4589








Stripes第一次接触就觉得它很神奇，不禁让喜欢编码的程序员产生好奇心。是什么让他做到了灵活自如的进行页面跳转呢？

**Stripes是一个视图框架用于利用最新的Java技术来构建Web应用程序.****它具有以下特点:**


- 不需要对每一action进行映射配置(ActionBeans将自动被发现,它利用注释进行配置)、支持：action转发、servlet请求、Rest风格请求.
- 强大的绑定引擎用于构建复杂并脱离请求参数(request parameter)的web对象.
- 易于使用并可本地化的验证与类型转换系统.可重复使用ActionBean作为视图帮助类.
- 支持一个form对应多个事件.透明的文件上传能力.支持持续开发(比如在构思你的ActionBean之前可以先构建与测试JSP)等.



**1、Strpes视图控制转发**

Strpes主要的作用就是实现页面与后台数据的一个衔接。我们用它来控制页面的跳转、重定向——控制视图数据流向，方便用户数据展示。

![](https://img-blog.csdn.net/20131105140616156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这幅图片还不足以说明问题，确切的说应该是下面这幅图表达的意思：

![](https://img-blog.csdn.net/20131105141839578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在这里Stripes的作用相当于一组Servlet的处理集合，或者说是Controller层。

**2、ActionBeans过滤（将*ActionBean类映射到上下文）**

  ActionBeans过滤主要是将实现了Stripes的ActionBean接口的实现类加载到应用程序上下文中，下面是Action包的web.xml注册过滤配置：



```
<filter>
		<display-name>Stripes Filter</display-name>
		<filter-name>StripesFilter</filter-name>
		<filter-class>net.sourceforge.stripes.controller.StripesFilter</filter-class>
		<init-param>
			<param-name>ActionResolver.Packages</param-name>
			<param-value>com.boonya.stripes.actions</param-value>
		</init-param>
		<init-param>
			<param-name>Interceptor.Classes</param-name>
			<param-value>
		        net.sourceforge.stripes.integration.spring.SpringInterceptor
		    </param-value>
		</init-param>
	</filter>
	<filter-mapping>
		<filter-name>StripesFilter</filter-name>
		<servlet-name>StripesDispatcher</servlet-name>
		<dispatcher>REQUEST</dispatcher>
	</filter-mapping>
	
	<servlet>
		<servlet-name>StripesDispatcher</servlet-name>
		<servlet-class>net.sourceforge.stripes.controller.DispatcherServlet</servlet-class>
		<load-on-startup>1</load-on-startup>
	</servlet>
	<servlet-mapping>
		<servlet-name>StripesDispatcher</servlet-name>
		<url-pattern>*.action</url-pattern>
	</servlet-mapping>
```
注：在<filter> <ini-param>  下的参数ActionResolver.Packages   设置的值（ param-value），将其配置为你的action类所在的位置，如：com.boonya.stripes.actions。


**3、Stripes支持动态路径映射**

Stripes提供了一个名为：**net.sourceforge.stripes.controller.DynamicMappingFilter**
的动态过滤器类来实现URL的动态配置，此配置既适用于Servlet又实用与Rest风格的请求，Rest动态风格有支持变参如："/home/{username}/{password}"。参考：[http://stripes.sourceforge.net/docs/current/javadoc/net/sourceforge/stripes/controller/DynamicMappingFilter.html](http://stripes.sourceforge.net/docs/current/javadoc/net/sourceforge/stripes/controller/DynamicMappingFilter.html) ，其在web.xml下的配置如下：



```
<!-- 动态配置隐射过滤器 -->
  <filter>
      <description>Dynamically maps URLs to ActionBeans.</description>
      <display-name>Stripes Dynamic Mapping Filter</display-name>
      <filter-name>DynamicMappingFilter</filter-name>
      <filter-class>
          net.sourceforge.stripes.controller.DynamicMappingFilter
      </filter-class>
      <init-param>
          <param-name>ActionResolver.Packages</param-name>
          <param-value>com.boonya.stripes.actions</param-value>
      </init-param>
  </filter>
  
  <filter-mapping>
      <filter-name>DynamicMappingFilter</filter-name>
      <url-pattern>/*</url-pattern>
      <dispatcher>REQUEST</dispatcher>
      <dispatcher>FORWARD</dispatcher>
      <dispatcher>INCLUDE</dispatcher>
  </filter-mapping>
```
注：在<filter> <ini-param>  下的参数ActionResolver.Packages   设置的值（ param-value），将其配置为你的action类所在的位置，如：com.boonya.stripes.actions。


**4、Stripes提供JSP使用的标签库，支持网页布局**

在JSP页面中导入标签库：



```java
<%@ taglib prefix="stripes"
	uri="http://stripes.sourceforge.net/stripes.tld"%>
```


其支持常用的标签有：radio 、form、input 等等，这个导入库页面引用时有相应的自动提示功能。



**5、支持常用框架无缝集成**

如Hibernate、Spring、ibatis等常用的框架都可以很好的兼容。















