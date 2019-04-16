# 基于RFC6265 (HTTP状态管理协议)实现简单的登录系统 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2014年03月24日 01:24:54[gloomyfish](https://me.csdn.net/jia20003)阅读数：4185








该协议主要是阐述如何利用HTTP Cookie与SetCookie头字段来实现对HTTP Request

状态的跟踪与管理，这个在用户行为分析，登录系统设计等方法有着很重要的应用。对

于大多数现代的浏览器都支持RFC6265.



基本原理:

RFC6265阐述通过设置SetCookie不同值在HTTPResponse中，来告诉浏览器客户端在

接下来的每次请求Request Header中都带上Response中指定的值与行为。直到服务器

配置的session过期为止。通过Tomcat配置session过期时间为30分钟，在web.xml配置

文件中可以重写该属性值。同时当用户关闭浏览器以后，在客户端内存中对于该站点

的cookie内容将会自动销毁。也许这样不太方便用户，于是很多网站提供了记住自己帐

号的原因，其实就是通过将cookie写到本地文件中。



系统访问与重定向到登录页面

首先当浏览器客户端发起一个Request请求访问指定URL或者Web Server时，服务器

端通过检查请求头是否含有Cookie字段，以及Cookie字段中的内容来设别访问者是否

为登录或者未登录用户，如果是未登录页面，则将URL重定向到登录页面即可。用户

登录以后服务器端发送一个HTTP Response + Set-Cookie内容到客户端浏览器，那么

在随后所有发往该Domain的URL都将会带上Set-Cookie中指定的内容，HTTP 

Request + Cookie到服务器端，服务器端通过检查Request 头中Cookie内容实现对

用户的状态追踪。从而将无状态HTTP Request变成一个有状态的HTTP连接加以

管理。登录处理基本的流程图如下：

![](https://img-blog.csdn.net/20140324010801312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




服务器与客户端HTTP Request发送与接受状态：
![](https://img-blog.csdn.net/20140324010834703)



用户退出系统与Request状态终止

当客户端关闭浏览器时候，客户端Cookie将会被自动从内存中丢弃，当客户端再次打

开浏览器请求服务器端资源时候，将被要求再次登录到服务器端建立新的可跟踪的

Request 会话当超过服务器端配置的会话时间时，同样会要求用户再次登录系统。

当用户使用系统退出功能正常退出时，当退出时候通过设置Max-Age ： 0来remove

当前cookie内容实现对客户端状态的清零。只要在HTTP Response中加上Cookie过

期属性同时设置一个过去的时间。例子如下：

![](https://img-blog.csdn.net/20140324010934328)


**RFC6265中关于Cookie与SetCookie的属性与使用详解**
|Cookie|SetCookie|
|----|----|
|包含于HTTP Request Header,用户客户端向服务器端发送验证信息与其它有用信息，主要用来跟踪客户端状态与分析用户行为|在HTTP Response中设置，主要用于服务器端向客户端发送指定的状态信息，建立与客户端的联系。通过设置HTTPOnly属性与Secure属性还可以保护客户端Cookie数据，减少恶意读取用户Cookie信息发生。|

RFC6265中一个简单例子：

== Server -> UserAgent == // 服务器发送到客户端

Set-Cookie:SID=31d4d96e407aad42

== User Agent ->Server == // 每个请求中都会带上SID信息，实现追踪用户状态

Cookie: SID=31d4d96e407aad42

**要求客户端的所有请求路径都要带上SID信息，通过发送Path=/实现**

== Server -> UserAgent ==

Set-Cookie:SID=31d4d96e407aad42; Path=/; Domain=example.com

== User Agent ->Server ==

Cookie: SID=31d4d96e407aad42

**删除客户端Request Cookie中SID信息，取当前时间以前的一个任意时间。**

== Server -> UserAgent ==

Set-Cookie: SID=;Expires=Sun, 06 Nov 1994 08:49:37 GMT

最后来看一看我抓取的CSDN登录以后的Cookie信息：

![](https://img-blog.csdn.net/20140324010916015)

J2EE 当从HTTP Servlet Request中调用获取SessionID方法以后会自动把JSESSIONID

作为Cookie设置到Response头中。所以无需显式再次调用！



根据RFC6265的内容，基于Spring3 MVC我自己也实现了一个简单的登录系统设计

可以帮助大家更好的理解协议。只有两个页面，两个Controller类与一个ServletFilter

各个类的作用大致如下：

ServletFilter类：实现对HTTP Request头的检查，跟踪用户状态

两个Controller类：一个用来管理用户登入登出，一个是简单的获取主页面信息

其中ServletFilter类代码如下：



```java
package com.edinme.exam.filter;

import java.io.IOException;
import java.util.Date;

import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import com.editme.exam.util.FilterUtil;

public class SingleSignOnFilter implements Filter{ 
	@Override
	public void destroy() {
		// TODO Auto-generated method stub
	}

	@Override
	public void doFilter(ServletRequest request, ServletResponse response,
			FilterChain chain) throws IOException, ServletException {
		HttpServletRequest httpRequest = (HttpServletRequest) request;
		HttpServletResponse httpResponse = (HttpServletResponse) response;	
		String ipAddress = httpRequest.getRemoteAddr();
		
		// get URI resource path
		String uriPath = httpRequest.getRequestURI();
		String contextPath = httpRequest.getContextPath();
		String cookie = httpRequest.getHeader("Cookie");
		String sessionId = httpRequest.getSession().getId(); // enable SetCookie header in HTTP Response
		if(FilterUtil.validURLRequest(uriPath, cookie, contextPath, sessionId))
		{
			System.out.println("Request URI : " + uriPath);
			System.out.println("IP "+ipAddress + ", Time " + new Date().toString());
			chain.doFilter(request, response);
		}
		else
		{		
			System.out.println(httpRequest.getProtocol() + httpRequest.getLocalPort() + httpRequest.getContextPath());
			httpResponse.sendRedirect("/exam/user.do");
		}
	}

	@Override
	public void init(FilterConfig config) throws ServletException {
		// TODO Auto-generated method stub
		
	}

}
```
用户登入登出Controller：





```java
package com.edinme.exam.controller;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.servlet.ModelAndView;

import com.edinme.exam.dto.MockUpDataFactory;
import com.edinme.exam.dto.UserDto;
import com.editme.exam.util.FilterUtil;

@Controller
@RequestMapping(value = "/user")
public class UserLoginController {
	
	@RequestMapping(method = RequestMethod.GET)
	public ModelAndView goLoginPage()
	{
		System.out.println("Dispaly SSO Page");
		ModelAndView view = new ModelAndView("user");
		return view;
	}
	
	@RequestMapping(value = "signIn", method = RequestMethod.GET)
	@ResponseBody
	public UserDto login(@RequestParam String userId, @RequestParam String password, /*HttpServletRequest httpRequest,*/ HttpServletResponse response)
	{
		System.out.println("User Name = " + userId);
		MockUpDataFactory dataFactory = new MockUpDataFactory();
		response.addHeader("Set-Cookie", "userId=" + userId + "; Path=" + FilterUtil.CONTEXT_PATH + "; HttpOnly");
		return dataFactory.getUserById(userId);
	}
	
	@RequestMapping(value = "signOut", method = RequestMethod.GET)
	@ResponseBody
	public UserDto logout(@RequestParam String userId, HttpServletRequest httpRequest, HttpServletResponse response)
	{
		MockUpDataFactory dataFactory = new MockUpDataFactory();	
		//	Set-Cookie:JSESSIONID=delete; Expires=Thu, 01-Jan-1970 00:00:10 GMT; Path=/exam/
		//	Set-Cookie:userId=delete; Expires=Thu, 01-Jan-1970 00:00:10 GMT; Path=/exam/
		Cookie[] cs = httpRequest.getCookies();
		for(Cookie c : cs)
		{
			c.setMaxAge(0); // set expire attribute
			c.setValue("delete");
			c.setPath(FilterUtil.CONTEXT_PATH); // set path, must be same as login context path
			response.addCookie(c);			
		}
		response.setHeader("Expires", "Thu, 19 Nov 1981 08:52:00 GMT"); // must be GTM format
		return dataFactory.getUserById(userId);
	}
//	
//	public static void main(String[] args)
//	{
//		SimpleDateFormat sdf = new SimpleDateFormat("E dd MMM yyyy HH:mm:ss zzz", Locale.ENGLISH);
//		sdf.setTimeZone(TimeZone.getTimeZone("GMT"));
//		System.out.println(sdf.format(new Date()));
//	}

}
```
**全部源代码下载点击这里：**
**http://download.csdn.net/detail/jia20003/7087947**

**觉得好请顶一下啊！！，谢谢！！**



