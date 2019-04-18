# 如何为cookie设置HttpOnly - z69183787的专栏 - CSDN博客
2015年05月20日 12:41:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7803
将cookie设置成HttpOnly是为了防止XSS攻击，窃取cookie内容，这样就增加了cookie的安全性，即便是这样，也不要将重要信息存入cookie。
如何在Java中设置cookie是HttpOnly呢？
Servlet 2.5 API 不支持 cookie设置HttpOnly
[http://docs.oracle.com/cd/E17802_01/products/products/servlet/2.5/docs/servlet-2_5-mr2/](http://docs.oracle.com/cd/E17802_01/products/products/servlet/2.5/docs/servlet-2_5-mr2/)
建议升级Tomcat7.0，它已经实现了Servlet3.0
[http://tomcat.apache.org/tomcat-7.0-doc/servletapi/javax/servlet/http/Cookie.html](http://tomcat.apache.org/tomcat-7.0-doc/servletapi/javax/servlet/http/Cookie.html)
但是苦逼的是现实是，老板是不会让你升级的。
那就介绍另外一种办法：
利用HttpResponse的addHeader方法，设置Set-Cookie的值
cookie字符串的格式：key=value; Expires=date; Path=path; Domain=domain; Secure; HttpOnly
//设置cookie
response.addHeader("Set-Cookie", "uid=112; Path=/; HttpOnly");
//设置多个cookie
response.addHeader("Set-Cookie", "uid=112; Path=/; HttpOnly");
response.addHeader("Set-Cookie", "timeout=30; Path=/test; HttpOnly");
//设置https的cookie
response.addHeader("Set-Cookie", "uid=112; Path=/; Secure; HttpOnly");
在实际使用中，我们可以使FireCookie查看我们设置的Cookie 是否是HttpOnly
![如何为cookie设置HttpOnly - 星期五 - 星期五](http://img0.ph.126.net/zMKif5MMEd5CwCqsSSjG7Q==/2052234055297639980.png)
