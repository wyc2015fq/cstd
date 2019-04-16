# Java 学习笔记20：深入Java的Cookie使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月10日 15:19:57[initphp](https://me.csdn.net/initphp)阅读数：2440
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)








- Java的Cookie使用

> - 设置Cookie，设置了一个username的cookie，路径为"/"，有效期为：3600。设置了一个test的cookie。


```java
@RequestMapping(value="/test")
	@ResponseBody
	public String test(HttpServletResponse response, HttpServletRequest request) {
		Cookie cookie = new Cookie("username","zhuli");
		cookie.setMaxAge(3600);
		cookie.setPath("/");
		response.addCookie(cookie);
		
		Cookie cookies = new Cookie("test","zhuli");
		response.addCookie(cookies);
		return "test";
	}
```


在火狐浏览器下，可以看到已经设置了两个cookie了。

![](https://img-my.csdn.net/uploads/201212/10/1355124734_1007.jpg)

- 获取Cookie：


```java
@RequestMapping(value="/testa")
	@ResponseBody
	public String testa(HttpServletResponse response, HttpServletRequest request) {
		Cookie[] cookies = request.getCookies();
		for (Cookie ck : cookies) {
			System.out.print("cookie name:" + ck.getName() + " | ");
			System.out.print("cookie value:" + ck.getValue() + "\r\n");
			ck.getValue();
		}
		return "test";
	}
```


可以看到只输出了username的cookie值，因为上面设置cookie的URL路径是http://xxxx/test/test/，而这个路径是http://xxxx/test/testa/，test这个cookie值的作用于不一样。
![](https://img-my.csdn.net/uploads/201212/10/1355125146_2385.jpg)



- 封装简单的Cookie操作：


```java
package com.xx.www.test.util;

import java.util.HashMap;
import java.util.Map;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;


public class CookieUtil {
	
	/**
	 * 设置cookie
	 * @param response
	 * @param name
	 * @param value
	 * @param path
	 * @param maxExp
	 */
	public static void setCookie(HttpServletResponse response, String name, String value, String path, int maxAge) {
		Cookie cookie = new Cookie(name, value);
		cookie.setPath(path);
		if (maxAge > 0) {
			cookie.setMaxAge(maxAge);
		}
		response.addCookie(cookie);
	}
	
	/**
	 * 删除cookie
	 * @param response
	 * @param name
	 */
	public static void delCookie(HttpServletResponse response, String name) {
		CookieUtil.setCookie(response, name, "", "/", 0);
	}
	
	/**
	 * 获取Cookie
	 * @param request
	 * @param name
	 * @return
	 */
	public static Map<String, Cookie> getCookie(HttpServletRequest request) {
		Map<String, Cookie> map = new HashMap<String, Cookie>();
		Cookie[] cookies = request.getCookies();
		for (Cookie cookie : cookies) {
			map.put(cookie.getName(), cookie);
		}
		return map;
	}
}
```


使用：


```java
@RequestMapping(value="/test")
	@ResponseBody
	public String test(HttpServletResponse response, HttpServletRequest request) {
		//CookieUtil.setCookie(response, "username", "testssssssss", "/", 100);
		//CookieUtil.delCookie(response, "username");
		Map<String, Cookie> map = CookieUtil.getCookie(request);
		System.out.print(map.get("username").getValue());
		return "test";
	}
```


- Cookie原理，实际就是向http头部发送一串cookie的字符串


Set-Cookie：customer=huangxp; path=/foo;domain=.ibm.com; 

expires= Wednesday, 19-OCT-05 23:12:40 GMT; [secure]






