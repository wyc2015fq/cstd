# JSP与Servlet之间设置Cookie可能导致Cookie无法读取的解决办法 - z69183787的专栏 - CSDN博客
2013年12月18日 15:50:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1486
在jsp中设置了Cookie，可在Servlet中无法读取出相应的Cookie，搜了好多，最终是通过参看Cookie的API文档解决的。其实解决方法很简单，只要设置一下Cookie的path就可以了。另外，如果新建的Cookie不设置maxage的话，关闭浏览器之后该Cookie就失效了。
解决了此问题后，顺便看了下Cookie的原理及相关的Java API文档，整理如下： 
what is Cookie?
Cookie 是在http和https协议中，用来实现会话管理(session management)，在用户终端(比如浏览器)上存储的一小段数据。一般用来存储用户的习惯（user preferences）, 安全性要求较低的自动登录信息, 以及收集用户的信息。它一般保存在客户端用户浏览器的指定的目录下，因此，尽量不要使用Cookie保存敏感的数据，而且Cookie的数据最好加密。
每个Cookie都有个名字和对应的值。Cookie有几个可选的属性，比如注释comment，路径path，域名domain，以及最大生存时间maxage，和版本version。由于现在的浏览器对这些属性的支持存在bug，因此为了最大的互操作行，尽量不要太依靠这些属性。
Cookies是在服务器端被指定的，通过在HTTP响应的header中添加字段来实现。在Java Servlet中，是通过response.addCookie方法来一次添加一个Cookie的。用户浏览器一般被要求对于每个主机支持20个Cookie，每个Cookie最少支持4KB。使用大量的Cookie一般是不鼓励的。当浏览器发出请求时，这些Cookie会被加在HTTP请求header中发到服务器端。在服务器端，可以通过request.getCookies来获得本次请求中的所有的Cookies。位于不同path下可以存在相同名字的Cookie，在特定path下的Cookie只能被该path以及子目录下的读取。
```
1 publicvoid setDomain(String
 pattern);
```
Specifies the domain within which this cookie should be presented. 
The form of the domain name is specified by RFC 2109. A domain name begins with a dot (.foo.com) and means that the cookie is visible to servers in a specified Domain Name System (DNS) zone (for example, [www.foo.com](http://www.foo.com/),
 but not a.b.foo.com). By default, cookies are only returned to the server that sent them.
```
1 publicvoid setMaxAge(int expiry);
```
Sets the maximum age of the cookie in seconds. 
A positive value indicates that the cookie will expire after that many seconds have passed. Note that the value is the maximum age when the cookie will expire, not the cookie's current age. A negative value
 means that the cookie is not stored persistently and will be deleted when the Web browser exits. A zero value causes the cookie to be deleted.
```
1 publicvoid setPath(String
 uri);
```
Specifies a path for the cookie to which the client should return the cookie.
The cookie is visible to all the pages in the directory you specify, and all the pages in that directory's subdirectories. A cookie's path must include the servlet that set the cookie, for example, /catalog, which makes the cookie visible to all directories
 on the server under /catalog. Consult RFC 2109 (available on the Internet) for more information on setting path names for cookies. 
使用方法：
1、添加Cookie：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1 //新建一个Cookie，并设置名字和值。 
2  cookie =new Cookie("cookiename","cookievalue"); 
3  //设置Cookie的有效期为2天，如果不设置此有效期，则用户关闭浏览器后此Cookie失效。 
4  cookie.setMaxAge(60*60*24*2);
5 //设置Cookie的有效路径，“/”即该应用下都可以访问该Cookie; 
6 //如果不设置路径，那么只有设置该Cookie路径及其子路径可以访问. 
7 cookie.setPath("/"); 
8 //将Cookie添加到HTTP响应中去 
9 response.addCookie(cookie);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
2、获得Cookie：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
1 //获取本次请求中的所有Cookies
2 Cookie[] cookies = request.getCookies();
3 if(cookies!=null){
4 for(Cookie cookie : cookies){ 
5 cookie.getName();
6 cookie.getValue();
7 }
8 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
注意：
1 Cookie的名字和值中尽量不要使用特殊字符。关于Cookie的名字，RFC2109中说name只能包含字母或者数字，不能包含逗号、分号、空格及美元符号。而在Cookie的value中建议不用使用逗号，分号，空白字符等。
2 如果jsp和Servlet之间设置和读取Cookie时出现问题的话，一般是因为path的问题，此时通过调用Cookie.setPath来设定Cookie的path。
