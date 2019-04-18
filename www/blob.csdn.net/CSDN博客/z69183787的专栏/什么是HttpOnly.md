# 什么是HttpOnly - z69183787的专栏 - CSDN博客
2015年05月20日 11:58:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2808
**1.什么是HttpOnly?**
如果您在cookie中设置了HttpOnly属性，那么通过js脚本将无法读取到cookie信息，这样能有效的防止XSS攻击
具体一点的介绍请google进行搜索
**2.javaEE的API是否支持?**
目前sun公司还没有公布相关的API，但PHP、C#均有实现。搞javaEE的兄弟们比较郁闷了，别急下文有变通实现
**3.HttpOnly的设置样例**
**javaEE**
Javaee代码 ![复制代码](http://yzd.iteye.com/images/icon_copy.gif)
- response.setHeader("Set-Cookie", "cookiename=value;   
- Path=/;Domain=domainvalue;Max-Age=seconds;HTTPOnly");  
具体参数的含义再次不做阐述，设置完毕后通过js脚本是读不到该cookie的，但使用如下方式可以读取
Java代码 ![复制代码](http://yzd.iteye.com/images/icon_copy.gif)
- Cookie cookies[]=request.getCookies();  
**C#**
C#代码 ![复制代码](http://yzd.iteye.com/images/icon_copy.gif)
- HttpCookie myCookie = **new** HttpCookie("myCookie");   
- myCookie.HttpOnly = true;   
- Response.AppendCookie(myCookie);  
**VB.NET**
Vb.net代码 ![复制代码](http://yzd.iteye.com/images/icon_copy.gif)
- Dim myCookie As HttpCookie = new HttpCookie("myCookie")   
- myCookie.HttpOnly = True   
- Response.AppendCookie(myCookie)  
   但是在 **.NET 1.1** ,中您需要手动添加
Response.Cookies[cookie].Path += ";HTTPOnly";
**PHP4**
Java代码 ![复制代码](http://yzd.iteye.com/images/icon_copy.gif)
- header("Set-Cookie: hidden=value; httpOnly");  
**PHP5**
Java代码 ![复制代码](http://yzd.iteye.com/images/icon_copy.gif)
- setcookie("abc", "test", NULL, NULL, NULL, NULL, TRUE);   
    最后一个参数为HttpOnly属性
