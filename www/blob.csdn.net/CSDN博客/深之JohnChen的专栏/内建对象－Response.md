# 内建对象－Response - 深之JohnChen的专栏 - CSDN博客

2008年11月09日 19:51:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1534


 内建对象－Response

　　Response对象用于控制和管理由服务器发送到浏览器的数据，其功能主要是通过该对象的属性、方法和Cookies集合来实现的。

**一、Response对象的属性：**

**1、Buffer属性**

　　Buffer属性指定ASP页面所创建的输出是否保存在IIS的缓冲区中，其取值为true或false。诺设置为true，则缓存当前页面，直到当前页面的所有服务器脚本处理完毕，或者调用了Flush或End方法，才一次性将其发送给客户端浏览器。若设置为false，则不缓存当前页面，服务器处理多少就发送多少，浏览器也可同时处理接收到的部分页面。
　　对于Buffer属性的设置或修改应放在ASP脚本的所有HTML输出之前，通常应该放在ASP的最开头部分，否则将导致错误，其位置如下：
<%@language="vbscript"
   Option Explicit
   Response.Buffer=true
%>
<html>
....
</html>
　　在ASP3.0中，默认情况下，缓冲区是打开的，而2.0中却是关闭的，可根据需要，利用Buffer属性进行设置。

**2、ContentType属性**

　　用于指定服务器响应的HTTP内容类型。若未指定，则默认为“text/html”，若传送的是GIF格式的图形数据，则应设置其值为“image/gif”，若是JPG的图形格式，则应设置为“image/jpg”。设置该属性也就告诉了浏览器所期望的内容是什么类型。

**3、Charset属性**

　　该属性用于设置网页所采用的字符集。设置该属性后，字符集名称将附加到HTTP Content-type的后面。对于简体中文，字符集应设置为“GB2312”，其设置方法为：
<%
Response.Charset="GB2312"
%>
　　说明：很多ASP程序员忘掉了设置这行语句，从而经常导至网页在运行的时候出现乱码，所以这里请大家一定要养成习惯，如果是简体中文的网页数据，最好在HTML内容输出之前指定如上代码。

**4、Expires属性**

　　以分钟为单位，设置页面距离过期还有多少分钟，即设置该网页在客户端浏览器缓存中的存活时间。其设置方法为：
Response.Expires=页面存活时间
例：若要设置当前页面的存活时间为3分钟，则设置的语句为:
<%Response.Expires=3%>
　　在网页未过期之前，若用户再次返回该页面，则直接从客户端缓冲区中读取该页面并显示，若超过该时间，则网页过期，缓冲区失效，网页会重新从服务器上下载该页面并显示。
　　说明：对于要求信息即时传㼱的网页，或安全性较高的网页，可设置该属性的值为0或负数，让其立即过期，使网页每次都必须从服务器上重新读取，以保证所得到的网页是服务器上最新的网页。但对于数据更新要求不是很即时，可以设置为一个稍长的时间，以减轻服务器负担。该属性必须放在所有HTTP内容输出之前。

**5、ExpireAbsolute**

　 指定网页过期的日期和时间。在未过期之前，若用户再次返回该页面，则显示缓存中的页面，若过期，则重新从服务器上读取网页并显示，在设置时，若未指定时间，则在所指定日期的午夜到期，若未指定日期，则在脚本运行当天的指定时间过期，其设置方法为：
Response.ExpireAbsolute=#日期时间#
例如：
Response.ExpireAbsolute=#20:00:00#

**6、Status属性**

　　该属性用于设置HTTP响应状态。服务器返回的响应状态码由三位数字组成，它代表了服务器对当前请求的响应请况。例如：当访问（请求）一个并不存在的网页是，服务器会自动设置该属性的值为状态码404，浏览器收到该状态码后，就会显示404状态码所对应的信息。如下图所示：

常用的HTTP响应状态码如下表所示： 

|状态码|状态|说明|状态码|状态|说明|
|----|----|----|----|----|----|
|404|Bad Request|错误请求|404|Not Found|文件未找到|
|401|Unauthorized|未授权访问|500|Internet Server Error|内部服务器错误|
|403|Forbidden|禁止访问|503|Server unavailable|服务器无效|

该属性的设置，必须在其它任何HTML输出之前，否则将出错，通常放在网页的开头部分设置，例：
<%
Response.Status="404 Not Found"
%>
　　将如上代码直接保存为test.asp，然后在浏览器中访问该页，就会看到如上图所示的界面了。 

**7、IsClientConnected属性**

　　该属性用于返回客户是否仍然连接到服务器和加载页面。
例：
<%if not Response.IsClientConnected Response.Write("您己经与服务器断开链接。")%>

**二、内建对象－Response（方法）**

**1、Write方法**

　　将指定的字符串写入到当前的HTTP响应流中。

常用该方法实现由服务器向客户端浏览器输出内容，其输出的内容可以包括文本、HTML标记符和脚本。例如：
Response.Write"<a href='http://www.djasp.net'>点击设计</a>"

**2、BinaryWrite方法**

　　将指定的数据写入到当前的HTTP响应流中，而不进行转换。常用于向客户端输出二进制数据，如图像或声音文件等。其用法为：
Response.BinaryWrite data
　　其中的data为要输出的数据。

**3、Redirect方法**

　　该方法使浏览器立即重新定位到指定的URL，在功能上类似于HTML的<a>标记。

该方法必须写在HTML标记之前或任何内容输出之前调用，否则将出错。其用法为：
Response.Redirect URL
例：
若isLogin变量的值不是1，则让其返回到index.asp页面，实现代码为：
<%if not isLogin then Response.Redirect "index.asp"%>

**4、End方法**

　　结束服务器对脚本的处理并将己处理结果传送给浏览器。若Buffer属性设置为True，还可以将缓冲区内容一并传送给浏览器。其用法为：
Response.End

**5、Flush方法**

　　将缓冲区中的内容发送给浏览器。调用该方法时，Buffer属性必须设置为True，否则将出错。

其用法为：
Response.Flush

**6、Clear方法**

　　清除缓冲区中的内容。只有Buffer属性设置为True时，才能调用，否则将出错，其用法为：
Response.Clear

**7、AppendToLog方法**

　　将指定的信息添加到IIS的日记文件中，添加的信息最多可为80个字符，其用法为：
Response.AppendToLog "要添加的信息"
例如，若要将当前登录的日期和时间添加到日志文件中，则实现代码为：
<%Response.AppendToLog Now%>

三、**内建对象－Response（Cookies集合）**

Cookies是一种标记，由WEB服务器嵌入到用户浏览器中，以便识别用户，下次同一浏览器请求网页时，将发送从WEB服务器收到的Cookies。

利用Cookies可存储与用户相关的信息。利用Response和Request对象的Cookies集合，可实现设置或获得Cookies的值。 

　　利用Response对象的Cookies集合，设置Cookies的用法为：

Response.Cookies(CookieName)[(Key)|.Attribute]=value

**参数说明：**（1）、CookieName：代表所要设置的Cookie的名字，由用户自行命名。
（2）、Key：为Cookie指定关键字。通过给Cookie指定关键字，可使Cookie拥有多个值，这种Cookie称为带索引的Cookie。
（3）、Attribute：该参数代表Cookies集合对象的属性，其最常用的属性主要是Expires（用于设置Cookies的过期日期）、Domain（用于设置Cookies的有效作用域名范围）。

**例如：**若要设置一个名为VisitNum的Cookie，用以储存该用户访问本网站的次数，并初始化其值为1，则实现代码为：

<%Response.Cookies("VisitNum")=1%>

　　该语句必须位于网页的<HTML>标记之前。若指定的Cookies己经存在，则将重写Cookies的值，若不存在，建创建该Cookies。
　　利用该方法所设置的Cookies仅在当前会话中有效，若要在用户已经终止或重新启动浏览器之后仍有效，就必须强制浏览器将Cookie保存到用户的硬盘上，为此，可以通过设置Cookies集合对象的Expires属性为此后的某一段时间来实现。

例如，若要设置名为VisitNum的Cookies在3天后过期，则设置方法为： 

<%
Response.Cookies("VisitNum")=1
Response.Cookies("VisitNum").Expires=DateAdd("d",3,Date)
%>

**例：**试用Cookie保存用户访问网站的次数和最近一次访问的日期和时间，并要求该Cookie在一周后过期。
分析：要保存的信息有2个，而且这2个信息有一定的联系，因此，可用带索引的Cookies来实现。另外，也可以用两个不同的Cookie来分别保存。
　　为了实现累加访问次数，可利用Request对象的Cookie集合对象来获得Cookie的值，递增后，再赋回给该Cookie。Request对Cookie操作的方法与Response相似，只不过Request是用来获取Cookies的值，而Response是用来设置Cookies的值。

**（1）、带索引的Cookies的实现方法为:**
<%
if Request.Cookies("UserVisit")("num")="" then   '若该Cookie不存在
    Response.Cookies("UserVisit")("num")=1        '则创建该Cookie并赋初值
else
    Response.Cookies("UserVisit")("num")=Request.Cookies("UserVisit")("num")+1
end if
Response.Cookies("UserVisit")("LastVisit")=now
Response.Cookies("UserVisit").Expires=DateAdd("ww",1,Date)
%>

　　为了测试是否正确设置了Cookie,可用以下代码通过输出相应Cookie的值来实现:

<%="访问人数为:"&Request.Cookies("UserVisit")("num") &"<BR>最近访问时间为:" & Request.Cookies("UserVisit")("LastVisit")%>

**（2）、用两个不同Cookie的实现方法(不带索引):**

<%
Response.Cookies("VisitNum")=1
Resposne.Cookies("VisitNum").Expires=DateAdd("ww",1,Date)
Response.Cookies("LastVisit")=now
Response.Cookies("LastVisit").Expires=DateAdd("ww",1,Date)
%>

