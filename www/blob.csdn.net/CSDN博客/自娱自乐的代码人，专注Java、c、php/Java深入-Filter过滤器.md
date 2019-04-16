# Java深入 - Filter过滤器 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年07月20日 08:58:01[initphp](https://me.csdn.net/initphp)阅读数：1642
个人分类：[Java深入系列](https://blog.csdn.net/initphp/article/category/2332145)

所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









Java的1.3开始，对servlet2.3规范中加入了过滤器的支持。过滤器能够让我们对目标资源的请求和响应进行截取。

一些filter的特性：

1. Filter是Servlet规范的规定，需要Servlet容器的支持。

2. Filter不能使用Spring框架中的资源对象。

3. Filter只在Servlet前后起作用。




### Filter实现

我们需要实现接口Filter中定义的方法：



```java
/*
 * The contents of this file are subject to the terms
 * of the Common Development and Distribution License
 * (the "License").  You may not use this file except
 * in compliance with the License.
 *
 * You can obtain a copy of the license at
 * glassfish/bootstrap/legal/CDDLv1.0.txt or
 * https://glassfish.dev.java.net/public/CDDLv1.0.html.
 * See the License for the specific language governing
 * permissions and limitations under the License.
 *
 * When distributing Covered Code, include this CDDL
 * HEADER in each file and include the License file at
 * glassfish/bootstrap/legal/CDDLv1.0.txt.  If applicable,
 * add the following below this CDDL HEADER, with the
 * fields enclosed by brackets "[]" replaced with your
 * own identifying information: Portions Copyright [yyyy]
 * [name of copyright owner]
 *
 * Copyright 2005 Sun Microsystems, Inc. All rights reserved.
 *
 * Portions Copyright Apache Software Foundation.
 */

package javax.servlet;

import java.io.IOException;


public interface Filter {

	public void init(FilterConfig filterConfig) throws ServletException;
	
	

    public void doFilter ( ServletRequest request, ServletResponse response, FilterChain chain ) 
throws IOException, ServletException;

	public void destroy();


}
```


自定义一个TestFilter





```java
/**
 * 自定义一个Filter
 * @author zhuli
 * @date 2014-7-20
 */
public class TestFilter implements Filter {

    @Override
    public void init(FilterConfig filterConfig) throws ServletException {
        System.out.println("TestFiltern init"); //初始化容器的时候，这边会运行
    }

    @Override
    public void doFilter(ServletRequest request, ServletResponse response, FilterChain chain) 
throws IOException, ServletException {
        // TODO Auto-generated method stub
        HttpServletRequest request2 = (HttpServletRequest)request;
        String ip = IPUtil.getClientIp(request2);
        System.out.println("TestFiltern doFilter 请求前面就处理了 + ip: " + ip);
        chain.doFilter(request, response); //传递filter链
        System.out.println("业务处理完了之后，会继续调用这个filter，然后调用这边");
    }

    @Override
    public void destroy() {
        // TODO Auto-generated method stub
        System.out.println("TestFiltern destroy"); //销毁容器的时候，这边会运行
    }

}
```



结果：

TestFiltern doFilter 请求前面就处理了 + ip: 127.0.0.1

业务逻辑=========

业务处理完了之后，会继续调用这个filter，然后调用这边




### Filter配置

在web.xml中，配置servlet前面配置Filter即可：

url-pattern可以配置符合哪些请求的需要走这个filter




```
<!-- TEST filter -->
	<filter>
		<filter-name>testFilter</filter-name>
		<filter-class>com.xxx.test.TestFilter</filter-class>
	</filter>
	<filter-mapping>
		<filter-name>testFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
```






### Filter的执行顺序

可以先看一张图：

![](https://img-blog.csdn.net/20140720084644500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

假设我们有两个过滤器，TestFilter和TestFilter1。

当web请求进来

->

我们执行TestFilter过滤器中chain.doFilter之前的代码(例如我们的例子中System.out.println("TestFiltern doFilter 请求前面就处理了");)


-> 


然后执行TestFilter2过滤器中chain.doFilter之前的代码 


-> 


然后Servlet中的service方法 


->

然后处理TestFilter2过滤器中chain.doFilter之后的代码 


-> 


然后处理TestFilter过滤器中chain.doFilter之后的代码（例如我们的例子中System.out.println("业务处理完了之后，会继续调用这个filter，然后调用这边");）




### Filter生命周期

1. init初始化。在web容器启动的时候，对Filter进行初始化。初始化会调用init()方法

2. 过滤。具体过滤是doFilter()方法

3. destroy销毁。在容器关闭的时候会对Filter进行销毁。







