# Struts2 使用SiteMesh做网页布局 - z69183787的专栏 - CSDN博客
2015年07月18日 20:42:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：877
简单来讲SiteMesh就是用来做网页母版 Layout分层的。
SiteMesh[官方网站](http://www.opensymphony.com/sitemesh/)给出的最新版本是SiteMesh Version 2.4.1，但是下载不下来！！下载页面NOT FOUND！很囧有木有！无奈只能在[findjar.com](http://www.findjar.com/jar/opensymphony/sitemesh/2.4.1/sitemesh-2.4.1.jar.html;jsessionid=73C7DB5D1BE64328D6494D645211BFFB)下载2.4.1的jar包。
 或者使用Struts2的sitemesh包2.4.2，这个包是插件struts2-sitemesh-plugin-2.2.1.1.jar所依赖的。
安装使用过程记录：
1.把包扔到WEB-INF/lib下面。
2.修改web.xml文档：
    之前：注意filter-class是StrutsPrepareAndExecuteFilter
**[html]**[view
 plain](http://blog.csdn.net/zavens/article/details/7940008#)[copy](http://blog.csdn.net/zavens/article/details/7940008#)
- <filter>
- <filter-name>struts2</filter-name>
- <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter</filter-class>
- </filter>
- <filter-mapping>
- <filter-name>struts2</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
之后：
**[html]**[view
 plain](http://blog.csdn.net/zavens/article/details/7940008#)[copy](http://blog.csdn.net/zavens/article/details/7940008#)
- <filter>
- <filter-name>struts-prepare</filter-name>
- <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareFilter</filter-class>
- </filter>
- <filter>
- <filter-name>struts-execute</filter-name>
- <filter-class>org.apache.struts2.dispatcher.ng.filter.StrutsExecuteFilter</filter-class>
- </filter>
- <filter>
- <filter-name>sitemesh</filter-name>
- <filter-class>com.opensymphony.sitemesh.webapp.SiteMeshFilter</filter-class>
- </filter>
- <!--sitemesh in the middle and after struts-prepare -->
- <filter-mapping>
- <filter-name>struts-prepare</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- <filter-mapping>
- <filter-name>sitemesh</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
- <filter-mapping>
- <filter-name>struts-execute</filter-name>
- <url-pattern>/*</url-pattern>
- </filter-mapping>
注释下面的代码顺序很重要sitemesh的过滤器要在prepare和execute之间。
3. 在WEB-INF下添加decorators.xml文档：
**[html]**[view
 plain](http://blog.csdn.net/zavens/article/details/7940008#)[copy](http://blog.csdn.net/zavens/article/details/7940008#)
- <?xmlversion="1.0"encoding="utf-8"?>
- <!-- defaultdir指定装饰器文件所在的路径 -->
- <decoratorsdefaultdir="/layout">
- <!--excludes结点则指定了哪些路径的请求不使用任何模板 -->
- <excludes>
- <pattern>/entpLogin.jsp</pattern>
- <pattern>/entpRegister.jsp*</pattern>
- <pattern>/messageDiv*</pattern>
- <pattern>/registerServiceDetail*</pattern>
- </excludes>
- <!--decorator结点指定了模板的位置和文件名，通过pattern来指定哪些路径引用哪个模板 -->
- <decoratorname="main"page="mode.jsp">
- <pattern>/*</pattern>
- </decorator>
- </decorators
decorator标签的page指定的是装饰母版页。至于<pattern>/messageDiv*</pattern>和<pattern>/registerServiceDetail*</pattern> 是我在页面中使用Struts
 Ajax主题是指定的action（用来局部刷新的），这样sitemesh就不对这些ajax返回的response进行装饰
4.编写母版mode.jsp
**[html]**[view
 plain](http://blog.csdn.net/zavens/article/details/7940008#)[copy](http://blog.csdn.net/zavens/article/details/7940008#)
- <%@ page language="java"contentType="text/html; charset=UTF-8"
- pageEncoding="UTF-8"%>
- <%@ taglib uri="http://www.opensymphony.com/sitemesh/decorator"
- prefix="decorator"%>
- <%@ taglib uri="http://www.opensymphony.com/sitemesh/page"prefix="page"%>
- <%@ taglib prefix="s"uri="/struts-tags"%>
- <%@ taglib prefix="sx"uri="/struts-dojo-tags"%>
- <!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html; charset=UTF-8">
- <linkrel="stylesheet"type="text/css"href="css/style.css"
- media="screen">
- <title>
- <decorator:titledefault="Welcome"/><!--被装饰页面的Title-->
- </title>
- <sx:head/>
- <decorator:head/><!--被装饰页面的head-->
- </head>
- <body>
- <decorator:body/><!--被装饰页面的body-->
- </body>
- </html>
Ok啦 这样除了在decorator.xml中exclude的请求之外，所有的请求页面都会被装饰了，可以在<decorator:body/> 之前增加header，导航条，之后增加footer之类的东西啦！
> 
注意：如果在页面中增加了ajax主题的标签（局部刷新），则一定要在exclude pattern中添加请求的action名，否则局部刷新得到的innerHtml也会被添加上header导航条footer等装饰元素
