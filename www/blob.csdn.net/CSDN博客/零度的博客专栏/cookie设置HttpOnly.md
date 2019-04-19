# cookie设置HttpOnly - 零度的博客专栏 - CSDN博客
2016年05月20日 14:21:20[零度anngle](https://me.csdn.net/zmx729618)阅读数：11186标签：[cookie](https://so.csdn.net/so/search/s.do?q=cookie&t=blog)
个人分类：[Session/Cookie](https://blog.csdn.net/zmx729618/article/category/6221899)
**1.什么是HttpOnly?**
        如果cookie中设置了HttpOnly属性，那么通过js脚本将无法读取到cookie信息，这样能有效的防止XSS攻击，窃取cookie内容，这样就增加了cookie的安全性，即便是这样，也不要将重要信息存入cookie。XSS全称Cross SiteScript，跨站脚本攻击，是Web程序中常见的漏洞，XSS属于被动式且用于客户端的攻击方式，所以容易被忽略其危害性。其原理是攻击者向有XSS漏洞的网站中输入(传入)恶意的HTML代码，当其它用户浏览该网站时，这段HTML代码会自动执行，从而达到攻击的目的。如，盗取用户Cookie、破坏页面结构、重定向到其它网站等。
**2.javaEE的API是否支持?**
       Servlet3.0支持，详细见：[http://tomcat.apache.org/tomcat-7.0-doc/servletapi/javax/servlet/http/Cookie.html](http://tomcat.apache.org/tomcat-7.0-doc/servletapi/javax/servlet/http/Cookie.html)
**3.HttpOnly的设置样例**
        非servlet3.0的JAVAEE项目也可以通过设置Header进行设置，格式如下：
```
response.setHeader("Set-Cookie", "cookiename=value; Path=/;Domain=domainvalue;Max-Age=seconds;HTTPOnly");
```
例如：
//设置cookie
response.addHeader("Set-Cookie", "uid=112; Path=/; HttpOnly");
//设置多个cookie
response.addHeader("Set-Cookie", "uid=112; Path=/; HttpOnly");
response.addHeader("Set-Cookie", "timeout=30; Path=/test; HttpOnly");
//设置https的cookie
response.addHeader("Set-Cookie", "uid=112; Path=/; Secure; HttpOnly");
具体参数的含义再次不做阐述，设置完毕后通过js脚本是读不到该cookie的，但使用如下方式可以读取。
Java代码 ![复制代码](http://yzd.iteye.com/images/icon_copy.gif)
- Cookie cookies[]=request.getCookies();  
