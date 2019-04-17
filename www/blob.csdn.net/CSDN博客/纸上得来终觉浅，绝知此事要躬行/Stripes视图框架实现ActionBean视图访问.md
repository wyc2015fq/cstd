# Stripes视图框架实现ActionBean视图访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月05日 16:23:02[boonya](https://me.csdn.net/boonya)阅读数：3031








   Stripes最基本的实现是类似于Struts的请求，但没有Struts那么配置繁琐。下面就来看看怎样实现一个web应用的Action访问操作。本篇博文为设计Stripes的三种访问方式：普通Action的映射访问、类Servlet访问方式、Rest变参访问方式。前几天还没将Stripes应用示例打通，今天又本着不解决问题死不罢休的精神继续去弄，终于搞明白了。

**1、编写Stripes的web.xml配置文件**

 该文件定义了第三方的工具包、过滤器类、以及映射关系和访问格式；在此配置文件中有两种Stripes的配置（特定模板的Action过滤配置和动态URL路径配置），它们互不排斥，相互包容，方便了用户不同业务需求下的访问模式。其配置内容如下：



```
<?xml version="1.0" encoding="UTF-8"?>
<web-app xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
	xmlns="http://java.sun.com/xml/ns/javaee" xmlns:web="http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
	xsi:schemaLocation="http://java.sun.com/xml/ns/javaee http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd"
	version="2.5">
	
	<!-- 如果没使用到，此内容可以不添加 -->
	<context-param>
		<param-name>javax.servlet.jsp.jstl.fmt.localizationContext</param-name>
		<param-value>StripesResources</param-value>
	</context-param>
	
	<!-- 基本ActionBean访问过滤配置 -->
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
 
   <!-- 欢迎界面 -->
	<welcome-file-list>
		<welcome-file>index.jsp</welcome-file>
	</welcome-file-list>

</web-app>
```



**2、导入需要的jar包到项目WEB-INF的lib目录下**

此处需要加入的jar包，由于Stripes实现的时候用到了springframework的jar包，所以需要在项目中添加，否则在访问页面时会有异常产生。示例项目目录结构如下图：

![](https://img-blog.csdn.net/20131105155708906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20131105155718593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


注：上面所示的Jar有个别不需要，如果需要使用Spring整合建议保留；另外，根目录下下还需要一个消息配置文件**StripesResources.properties**，缺少它在数据验证的时候无法处理消息信息。

3、编写抽象的ActionBean，AbstractActionBean

因为在每个ActionBean中都使用到了上下文对象设置，这部分属于公共行为，所以将其抽象出来。



```java
package com.boonya.stripes.actions;

import java.io.Serializable;

import net.sourceforge.stripes.action.ActionBean;
import net.sourceforge.stripes.action.ActionBeanContext;
import net.sourceforge.stripes.action.SimpleMessage;

public abstract class AbstractActionBean implements ActionBean, Serializable {

  private static final long serialVersionUID = -1767714708233127983L;

  protected static final String ERROR = "/WEB-INF/Error.jsp";

  protected transient ActionBeanContext context;

  protected void setMessage(String value) {
    context.getMessages().add(new SimpleMessage(value));
  }
  
  public ActionBeanContext getContext() {
    return context;
  }

  public void setContext(ActionBeanContext context) {
    this.context = context;
  }

}
```
注：系统中公用的页面也可以设置在这里，应为这是所有对象的基本行为方式和处理逻辑。


**4、编写业务需要的ActionBean**

方式一：基本Action映射访问



```java
package com.boonya.stripes.actions;

import net.sourceforge.stripes.action.DefaultHandler;
import net.sourceforge.stripes.action.ForwardResolution;
import net.sourceforge.stripes.action.HandlesEvent;
import net.sourceforge.stripes.action.SessionScope;

@SessionScope
public class HelloActionBean extends AbstractActionBean {

	private static final long serialVersionUID = -8946154817549504368L;
	
	protected static final String HELLO ="/WEB-INF/Hello.jsp";
	
	protected static final String BLOG ="/WEB-INF/Blog.jsp";
	// 传递到指定的页面的参数
	private String name="boonya-only-hellos";

	public void setName(String name) {
		this.name = name;
	}

	public String getName() {
		return name;
	}

	/**
	 * 默认跳转页面(@DefaultHandler)
	 * @return
	 */
	@DefaultHandler
	public ForwardResolution view() {
		return new ForwardResolution(HELLO);
	}
	
	/**
	 * 指定跳转页面(@HandlesEvent)处理,可以不添加该注解
	 * @return
	 */
	@HandlesEvent("viewToBlog")
    public ForwardResolution viewToBlog() {
    	this.name=this.getClass().getName();
		return new ForwardResolution(BLOG);
    }

}
```



方式二：类Servlet访问方式



```java
package com.boonya.stripes.actions;

import net.sourceforge.stripes.action.ActionBeanContext;
import net.sourceforge.stripes.action.DefaultHandler;
import net.sourceforge.stripes.action.ForwardResolution;
import net.sourceforge.stripes.action.HandlesEvent;
import net.sourceforge.stripes.action.UrlBinding;
// URL 隐射路径
@UrlBinding("/hellos")
public class WorldActionBean  extends AbstractActionBean {
	
	private static final long serialVersionUID = 3073270783412011577L;

	protected static final String WORLD ="/WEB-INF/World.jsp";
	
	protected static final String BLOG ="/WEB-INF/Blog.jsp";

	// 传递到指定的页面的参数
	private String name="boonya-page";
	
	protected transient ActionBeanContext context;
	
	@Override
	public ActionBeanContext getContext() {
		    return context;
	}
	 @Override
    public void setContext(ActionBeanContext context) {
		    this.context = context;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getName() {
		return name;
	}

	/**
	 * 默认跳转页面(@DefaultHandler)
	 * @return
	 */
	@DefaultHandler
	public ForwardResolution view() {
		return new ForwardResolution(WORLD);
	}
	
	/**
	 * 指定跳转页面(@HandlesEvent)处理,可以不添加该注解
	 * @return
	 */
	@HandlesEvent("viewToBlog")
    public ForwardResolution viewToBlog() {
    	this.name=this.getClass().getName();
		return new ForwardResolution(BLOG);
    }

}
```



方式三：Rest风格变参访问



```java
package com.boonya.stripes.actions;

import net.sourceforge.stripes.action.DefaultHandler;
import net.sourceforge.stripes.action.ForwardResolution;
import net.sourceforge.stripes.action.HandlesEvent;
import net.sourceforge.stripes.action.UrlBinding;
// URL 隐射路径
@UrlBinding("/helloworld/{name}")
public class HelloWorldActionBean  extends AbstractActionBean {
	
	private static final long serialVersionUID = 2276458379123668500L;

	protected static final String WORLD ="/WEB-INF/HelloWorld.jsp";
	
	protected static final String BLOG ="/WEB-INF/Blog.jsp";

	// 传递到指定的页面的参数
	private String name;

	public void setName(String name) {
		this.name = name;
	}

	public String getName() {
		return name;
	}

	/**
	 * 默认跳转页面(@DefaultHandler)
	 * @return
	 */
	@DefaultHandler
	public ForwardResolution view() {
		return new ForwardResolution(WORLD);
	}
	
	/**
	 * 指定跳转页面(@HandlesEvent)处理,可以不添加该注解
	 * @return
	 */
	@HandlesEvent("viewToBlog")
    public ForwardResolution viewToBlog() {
    	this.name=this.getClass().getName();
		return new ForwardResolution(BLOG);
    }

}
```



**5、编写测试JSP页面**

index.jsp



```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Insert title here</title>
<style type="text/css">
   body{
      text-decoration: underline;
      color: blue;
      font-size: 12px;
      font-family: '仿宋体';
   }
</style>
</head>
<body>
 <h3> <a href="actions/Hello.action">forward风格测试</a></h3>
 <h3> <a href="hellos">Servlet风格测试</a></h3>
 <h3> <a href="helloworld/boonya-test-rest">Rest风格的测试</a></h3>
</body>
</html>
```



Hello.jsp




```
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="stripes"
	uri="http://stripes.sourceforge.net/stripes.tld"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<title>Hello-page</title>
<body>
	Hello ${actionBean.name}
	<br />
	<br />
	<stripes:link beanclass="com.boonya.stripes.actions.HelloActionBean">
		<stripes:param name="name" value="boonya-hello" />Try again</stripes:link>
	<br>
	<stripes:link beanclass="com.boonya.stripes.actions.HelloActionBean" event="viewToBlog">
	from Hello-page to visit boonya's blog
	</stripes:link>
	<br>
</body>
</html>
```

World.jsp





```
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="stripes"
	uri="http://stripes.sourceforge.net/stripes.tld"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<title>World-page</title>
<body>
	Hello ${actionBean.name}
	<br />
	<br />
	<stripes:link beanclass="com.boonya.stripes.actions.WorldActionBean">
		<stripes:param name="name" value="boonya-world" />Try again</stripes:link>
	<br>
	<stripes:link beanclass="com.boonya.stripes.actions.WorldActionBean" event="viewToBlog">
	from World-page to visit boonya's blog
	</stripes:link>
	<br>
</body>
</html>
```

HelloWorld.jsp





```
<%@ page language="java" contentType="text/html; charset=UTF-8"
	pageEncoding="UTF-8"%>
<%@ taglib prefix="stripes"
	uri="http://stripes.sourceforge.net/stripes.tld"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<title>HelloWorld-page</title>
<body>
	Hello ${actionBean.name}
	<br />
	<br />
	<stripes:link beanclass="com.boonya.stripes.actions.HelloWorldActionBean">
		<stripes:param name="name" value="boonya-world" />Try again</stripes:link>
	<br>
	<stripes:link beanclass="com.boonya.stripes.actions.HelloWorldActionBean" event="viewToBlog">
	from HelloWorld-page to visit boonya's blog
	</stripes:link>
	<br>
</body>
</html>
```

Blog.jsp





```
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<title>Blog-Page</title>
</head>
<body>
 <h3>welcome to  boonya's blog.</h3>
 
 <h2 style="color: blue;">from action's class name: <font color="red">${actionBean.name}</font> </h2>
</body>
</html>
```

Error.jsp没什么特别的内容故不在此处写出。



**6、项目部署测试访问**

以下是测试访问的效果图：

**系统首页显示：**

![](https://img-blog.csdn.net/20131105161840421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**forward显示：**

![](https://img-blog.csdn.net/20131105161908421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20131105161919546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**servlet显示：**

![](https://img-blog.csdn.net/20131105161933734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20131105161952171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**rest显示：**

![](https://img-blog.csdn.net/20131105162010078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20131105162020796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


示例程序下载地址：[http://download.csdn.net/detail/boonya/6506145](http://download.csdn.net/detail/boonya/6506145)



