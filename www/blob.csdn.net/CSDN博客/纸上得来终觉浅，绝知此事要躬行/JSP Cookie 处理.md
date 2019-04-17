# JSP Cookie 处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月09日 17:02:21[boonya](https://me.csdn.net/boonya)阅读数：531标签：[jsp																[cookie](https://so.csdn.net/so/search/s.do?q=cookie&t=blog)](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)
个人分类：[Servlet/JSP](https://blog.csdn.net/boonya/article/category/2864195)








文章来源：[http://www.runoob.com/jsp/jsp-cookies.html](http://www.runoob.com/jsp/jsp-cookies.html)


Cookie是存储在客户机的文本文件，它们保存了大量轨迹信息。在servlet技术基础上，JSP显然能够提供对HTTP cookie的支持。

通常有三个步骤来识别回头客：
- 服务器脚本发送一系列cookie至浏览器。比如名字，年龄，ID号码等等。
- 浏览器在本地机中存储这些信息，以备不时之需。
- 当下一次浏览器发送任何请求至服务器时，它会同时将这些cookie信息发送给服务器，然后服务器使用这些信息来识别用户或者干些其它事情。

本章节将会传授您如何去设置或重设cookie的方法，还有如何访问它们及如何删除它们。

> 
JSP Cookie 处理需要对中文进行编码与解码，方法如下：
String   str   =   java.net.URLEncoder.encode("中文");//编码String   str   =   java.net.URLDecoder.decode("编码后的字符串");// 解码

## Cookie 剖析

Cookie通常在HTTP信息头中设置（虽然JavaScript能够直接在浏览器中设置cookie）。在JSP中，设置一个cookie需要发送如下的信息头给服务器：
HTTP/1.1200 OK
Date:Fri,04Feb201521:03:38 GMT
Server:Apache/1.3.9(UNIX) PHP/4.0b3Set-Cookie: name=runoob; expires=Friday,04-Feb-0722:03:38 GMT; 
                 path=/; domain=runoob.com
Connection: close
Content-Type: text/html
正如您所见，Set-Cookie信息头包含一个键值对，一个GMT（格林尼治标准）时间，一个路径，一个域名。键值对会被编码为URL。有效期域是个指令，告诉浏览器在什么时候之后就可以清除这个cookie。

如果浏览器被配置成可存储cookie，那么它将会保存这些信息直到过期。如果用户访问的任何页面匹配了cookie中的路径和域名，那么浏览器将会重新将这个cookie发回给服务器。浏览器端的信息头长得就像下面这样：
GET / HTTP/1.0Connection:Keep-AliveUser-Agent:Mozilla/4.6(X11; I;Linux2.2.6-15apmac ppc)Host: zink.demon.co.uk:1126Accept: image/gif,*/*
Accept-Encoding: gzip
Accept-Language: en
Accept-Charset: iso-8859-1,*,utf-8
Cookie: name=xyz
JSP脚本通过request对象中的getCookies()方法来访问这些cookie，这个方法会返回一个Cookie对象的数组。

## Servlet Cookie 方法

下表列出了Cookie对象中常用的方法：
|**序号**|**方法 & 描述**|
|----|----|
|1|**public void setDomain(String pattern)**设置cookie的域名，比如w3cschool.cc|
|2|**public String getDomain()**获取cookie的域名，比如w3cschool.cc|
|3|**public void setMaxAge(int expiry)**设置cookie有效期，以秒为单位，默认有效期为当前session的存活时间|
|4|**public int getMaxAge()**获取cookie有效期，以秒为单位，默认为-1 ，表明cookie会活到浏览器关闭为止|
|5|**public String getName()**返回 cookie的名称，名称创建后将不能被修改|
|6|**public void setValue(String newValue)**设置 cookie的值|
|7|**public String getValue()**获取cookie的值|
|8|**public void setPath(String uri)**设置cookie 的路径，默认为当前页面目录下的所有URL，还有此目录下的所有子目录|
|9|**public String getPath()**获取cookie 的路径|
|10|**public void setSecure(boolean flag)**指明cookie是否要加密传输|
|11|**public void setComment(String purpose)**设置注释描述 cookie的目的。当浏览器将cookie展现给用户时，注释将会变得非常有用|
|12|**public String getComment()**返回描述cookie目的的注释，若没有则返回null|

## 使用JSP设置Cookie

使用JSP设置cookie包含三个步骤：

**(1)创建一个Cookie对象： **调用Cookie的构造函数，使用一个cookie名称和值做参数，它们都是字符串。
Cookie cookie =newCookie("key","value");
请务必牢记，名称和值中都不能包含空格或者如下的字符：
[]()=," / ? @ : ;
**(2) 设置有效期：**调用setMaxAge()函数表明cookie在多长时间（以秒为单位）内有效。下面的操作将有效期设为了24小时。
cookie.setMaxAge(60*60*24);
**(3) 将cookie发送至HTTP响应头中：**调用response.addCookie()函数来向HTTP响应头中添加cookie。
response.addCookie(cookie);
### 实例演示

main.jsp 文件代码如下所示：
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.net.*" %>
<%// 编码，解决中文乱码   String str =URLEncoder.encode(request.getParameter("name"),"utf-8");// 设置 name 和 url cookie Cookie name =newCookie("name",
		   str);Cookie url =newCookie("url",
			  request.getParameter("url"));// 设置cookie过期时间为24小时。
   name.setMaxAge(60*60*24); 
   url.setMaxAge(60*60*24);// 在响应头部添加cookie
   response.addCookie( name );
   response.addCookie( url );
%>
<html><head><title>设置 Cookie</title></head><body><h1>设置 Cookie</h1><ul><li><p><b>网站名:</b><%= request.getParameter("name")%>
</p></li><li><p><b>网址:</b><%= request.getParameter("url")%>
</p></li></ul></body></html>
以下是一个简单的 HTML 表单通过GET方法将客户端数据提交到 main.jsp 文件中,并设置 cookie：
<!DOCTYPE html><html><head><metacharset="utf-8"><title>菜鸟教程(runoob.com)</title></head><body><formaction="main.jsp"method=GET>
站点名: <inputtype="text"name="name"><br/>
网址: <inputtype="text"name="url"/><inputtype="submit"value="提交"/></form></body></html>
将以上HTML代码保存到test.htm文件中。 

将该文件放置于当前jsp项目的 WebContent 目录下（与 main.jsp 同一个目录）。

通过访问 http://localhost:8080/testjsp/test.html 提交表单数据到 main.jsp 文件，演示 Gif 图如下所示：

![](http://www.runoob.com/wp-content/uploads/2014/01/jsp6.gif)

试着输入 "站点名" 和 "网址"，然后点击提交按钮，它将会在您的屏幕中显示 "站点名" 和 "网址"，并且设置 "站点名" 和 "网址" 的两个 cookie。

## 使用 JSP 读取 Cookie

想要读取cookie，您就需要调用request.getCookies()方法来获得一个javax.servlet.http.Cookie对象的数组，然后遍历这个数组，使用getName()方法和getValue()方法来获取每一个cookie的名称和值。

让我们来读取上个例子中的cookie, 以下为 cookie.jsp 文件代码：
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.net.*" %>
<!DOCTYPE html><html><head><metahttp-equiv="Content-Type"content="text/html; charset=UTF-8"><title>获取 Cookie</title></head><body><%Cookie cookie =null;Cookie[] cookies =null;// 获取cookies的数据,是一个数组
   cookies = request.getCookies();if( cookies !=null){out.println("<h2> 查找 Cookie 名与值</h2>");for(int i =0; i < cookies.length; i++){
         cookie = cookies[i];out.print("参数名 : "+ cookie.getName());out.print("<br>");out.print("参数值: "+URLDecoder.decode(cookie.getValue(),"utf-8")+" <br>");out.print("------------------------------------<br>");}}else{out.println("<h2>没有发现 Cookie</h2>");}
%>
</body></html>
浏览器访问后，输出结果为：

![](http://www.runoob.com/wp-content/uploads/2014/01/C6A7341F-029A-4244-8B38-BE010E391091.jpg)

## 使用JSP删除Cookie

删除cookie非常简单。如果您想要删除一个cookie，按照下面给的步骤来做就行了：
- 获取一个已经存在的cookie然后存储在Cookie对象中。
- 将cookie的有效期设置为0。
- 将这个cookie重新添加进响应头中。

### 实例演示

下面的程序删除一个名为"name"的cookie，当您第二次运行cookie.jsp时，name 将会为 null。
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ page import="java.net.*" %>
<!DOCTYPE html><html><head><metahttp-equiv="Content-Type"content="text/html; charset=UTF-8"><title>获取 Cookie</title></head><body><%Cookie cookie =null;Cookie[] cookies =null;// 获取当前域名下的cookies，是一个数组
   cookies = request.getCookies();if( cookies !=null){out.println("<h2> 查找 Cookie 名与值</h2>");for(int i =0; i < cookies.length; i++){
         cookie = cookies[i];if((cookie.getName()).compareTo("name")==0){
            cookie.setMaxAge(0);
            response.addCookie(cookie);out.print("删除 Cookie: "+ 
            cookie.getName()+"<br/>");}out.print("参数名 : "+ cookie.getName());out.print("<br>");out.print("参数值: "+URLDecoder.decode(cookie.getValue(),"utf-8")+" <br>");out.print("------------------------------------<br>");}}else{out.println("<h2>没有发现 Cookie</h2>");}
%>
</body></html>
通过浏览器访问，输出结果为：

![](http://www.runoob.com/wp-content/uploads/2014/01/C6A7341F-029A-4244-8B38-BE010E391091.jpg)

再次访问 **http://localhost:8080/testjsp/cookie.jsp**，将会得到如下结果：

![](http://www.runoob.com/wp-content/uploads/2014/01/7BF1C669-F7A4-4245-B7B0-B79BBB272604.jpg)

可以看到名为 width="70%" "name" 的 cookie 已经不见了。

您也可以手动在浏览器中删除 cookie。IE 浏览器通过点击Tools菜单项，然后选择Internet Options，点击 Delete Cookies，就能删除所有 cookie 。




