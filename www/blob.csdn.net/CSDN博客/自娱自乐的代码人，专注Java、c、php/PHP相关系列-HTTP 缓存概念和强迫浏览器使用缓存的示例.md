# PHP相关系列 - HTTP 缓存概念和强迫浏览器使用缓存的示例 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年11月16日 15:55:24[initphp](https://me.csdn.net/initphp)阅读数：935
个人分类：[PHP相关系列](https://blog.csdn.net/initphp/article/category/2609095)









对于静态的html页面，浏览器首次访问后会把该网页放入存入缓存。

再次访问该页面的时候，请求头会附上：

If-Modified-Since: Mon, 17 Sep 2012 08:52:31 GMT之样的数据。

发送给服务器，服务器收到后发现这个日期之后没更改，就发送304响应给客户端。客户端就乖乖的显示它自己的缓存了。

如果你按住CTRL+F5，则表示强制刷新，请求头中不包含If-Modified-Since。因此服务器收到后直接发送200响应。

对于用ASP.NET所用到的缓存，也是有同样的效果。比如下面的这个页面代码：

<%@ OutputCache Duration="600" VaryByParam="None"%>

default:<%Response.Write(DateTime.Now.ToString("yyyyMMdd")); %>

此处如果设置Location="ServerAndClient"，那么第二次访问是304响应。访问该页面的时候和访问html相同，第二次访问会有304响应。当然，如果页面更新后，就又会得到200响应。那是因为请求头中会有If-Modified-Since:Tue, 18 Sep 2012 12:32:14 GMT这样的数据。（注：在Chrome尝试，会有304响应。但是在firefox中尝试，请求头没有If-Modified-Since信息，因此没有304，每次都是200响应。原因不明。）

此处如果设置Location="Server"，那么每次都是200响应。这个缓存在Server内存里，因此虽然每次都是200，但是节省了生成页面的开销。
Location="Server"的时候，响应头变成Cache-Control:no-cache，其实即使响应头Cache-Control标记为no-cache，本地还是会有缓存的，只是在没有首次验证内容是否为最新的情况下，不会把cache内容发送给客户端，no-cache更好的名称是“没有验证是否最新就不作为输出的cache”（注：出自《HTTP The Definitive Guide》by David Gourley and
 Brian Totty）

此处如果设置Location="Client"，那么每次都是200响应。这个就是疑问的地方，虽然客户端有缓存了，而且请求头也会有If-Modified-Since，服务器数据也未改变，但还是返回了200。此处推测对于动态页面，如果没有特殊的处理，总是返回200状态。

比如下面的页面，假设是一个php页面，或者aspx页面，即使这个页面的内容没有任何动态代码。

比如： 
- <htmlxmlns="http://www.w3.org/1999/xhtml">
- <head>
- <title></title>
- </head>
- <body>
- <formid="form1">
- <div>
-     不修改  
- </div>
- </form>
- </body>
- </html>
- 

 每次请求也总是得到200响应，从不用缓存。

下面演示一下，服务器端通过发送304状态，欺骗客户端调用自己的缓存，以达到缓解网络压力的目的。

先说明几点测试下来的规则：

response响应头last-modified，缓存到客户端后，即客户端刷新的时候，这个值会成为客户端的request请求头的值。

比如 Response.AddHeader("last-modified", new DateTime(2000,1,1)，那么当刷新或者重新提交这个页面的时候，Request.Headers["If-Modified-Since"]的值就是2001-01-01。因此，If-Modified-Since可以理解为缓存最后的更新时间。

假设实现下面的一个功能，如果客户端请求过某一个页面后，那么10秒之内，客户端再次请求，服务器端都发送304状态码，要求其读取缓存。（客户端如果CTRL+F5没办法读缓存的）

ASPX代码实现如下： 
- <%@ Page Title="主页" Language="C#" AutoEventWireup="true" CodeBehind="Default.aspx.cs"
-     Inherits="WebApplication1._Default" %>  
- <%  
-     Response.Write("If - Modified - Since:" + Request.Headers["If-Modified-Since"] + "<br/>"); //打印读取的请求头中If-Modified-Since的值 
-     Response.Write("Now:" + DateTime.Now.ToString() + "<br/>");//打印当前时间 
-     DateTime If_Modified_Since = new DateTime();  
- if (Request.Headers["If-Modified-Since"] == null || Request.Headers["If-Modified-Since"] == "")  
-     {  
-         If_Modified_Since = new DateTime(1900, 1, 1);//如果读取的请求头中If-Modified-Since没有值，就给它一个默认值为19000101 
-     }  
- else
-     {  
-         If_Modified_Since = DateTime.Parse(Request.Headers["If-Modified-Since"]);  
-     }  
- if (If_Modified_Since.AddSeconds(10) > DateTime.Now)//如果缓存最后的修改时间和当前时间相差10s内 
-     {  
-         Response.StatusCode = 304;//返回304代码，使其读取缓存 
-         Response.End();  
-     }  
- else//否则 
-     {  
- //显示内容 
-         Response.AddHeader("last-modified", DateTime.Now.ToString());  
-         Response.Write("Content Changed;" + DateTime.Now.ToString()+"<br>");  
-     }  
- %>  

 PHP代码如下： 
- <?php  
- date_default_timezone_set('PRC'); //中华人民共和国时间  
- $arr=getallheaders();  
- $If_Modified_Since = new DateTime();  
- if(isset($arr["If-Modified-Since"])==false)  
-     {  
- $If_Modified_Since = new DateTime("1900-1-1");//如果读取的请求头中If-Modified-Since没有值，就给它一个默认值为19000101 
-     }  
- else
-     { //echo "##<br>"; 
- //echo "If-Modified-Since:".$arr["If-Modified-Since"]."<br>"; 
- //echo "##<br>"; 
- $If_Modified_Since = new DateTime($arr["If-Modified-Since"]);  
-     }  
- $now=new DateTime();  
- echo"##<br>";  
- echo$now->format('Y-m-d H:i:s')."---".$If_Modified_Since->format('Y-m-d H:i:s')."<br>";  
- echo"##<br>";  
- $diff=strtotime($now->format('Y-m-d H:i:s'))-strtotime($If_Modified_Since->format('Y-m-d H:i:s'));  
- echo"diff:"."$diff<br>";  
- if ($diff <10)//如果缓存最后的修改时间和当前时间相差10s内 
-     {  
-        header('Etag:',true,304);//返回304代码，使其读取缓存 
- exit();  
-     }  
- else//否则 
-     {  
- $now=new DateTime();  
- //显示内容 
- header ("last-modified:".$now->format('Y-m-d H:i:s'));   
-         print("Content Changed:" .$now->format('Y-m-d H:i:s')."<br>");  
-     }  
- ?>  
- 

 以PHP页面为例：

首次访问的时候，得到响应头为200，请求头中并没有If_Modified_Since

[![clipboard](http://img1.51cto.com/attachment/201209/18/2359144_1347956624wWBl.png)](http://img1.51cto.com/attachment/201209/18/2359144_13479566237fTp.png)

10s内再次访问：

[![clipboard[1]](http://img1.51cto.com/attachment/201209/18/2359144_1347956624oqME.png)](http://img1.51cto.com/attachment/201209/18/2359144_1347956624nhJT.png)

发送的If_Modified_Since头的值为2012-09-18 16:16:30。距离服务器当前时间在10s内，因此服务器发送了304，迫使浏览器调用缓存。




