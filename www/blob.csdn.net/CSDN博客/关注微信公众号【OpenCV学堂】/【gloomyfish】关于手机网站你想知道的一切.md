# 【gloomyfish】关于手机网站你想知道的一切 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年04月24日 09:26:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：4364








关于手机网站你想知道的一切

-all you want to know about a mobile website

介绍（introduction）

如今越来越多的人打算开发自己的网站尤其是基于移动设备的，这么做的原因很简单。

网站的拥有者希望拥有更多的有明确搜寻的访问者。手机是手持的，人们只要不忘记

他们的钱包，他们也不可能忘记带上手机。



Mobile Web pages (MWP)相比标准的网页稍有不同，支持移动访问网页的内容更少图片更小

一个理想的MWP应该在2秒之内被任何一种移动设备加载完，因此我们应该总是限制MWP

更小。



关键点（Key Point）

网站可以显示在绝大多书的GPRS的移动设备上，PDAs,
 1G,2G和3G移动设备（mobile


devices）。一个真正的挑战是你不可能找出一种解决方案支持所有的移动设备，我已经

Google很多的解决方案，但是让我失望是没有一个可以支持所有设备。这个是导致我写这篇文章的一个因素。



哪些改变是必须的要把一个Normal网页转换成一个Mobile网页

对一个Mobile浏览器来说有两件非常重要的事情，你必须设定它们：



1.文档类型（DOCTYPE）

-DOCTYPE（Document Type Declaration）指导校验那个版本的HTML是你网页正在使用的，你必须在你网页的第一行写入，它帮助验证你网页的标记符号与CSS


1.<!DOCTYPEhtmlPUBLIC
"-//W3C//DTDXHTML1.0
Transitional//EN""http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">


2.内容类型（Content Type）

Mobile网页的内容类型可以为下面任意之一:

a.application/vnd.wap.xhtml+xml

b.application/xhtml+xml

那个是正确的取决于移动设备本身，有些移动设备只支持a，有些移动设备只支持b，更多的移动设备两者都可以支持，最好的方法是开发一个功能来检查请求头信息从而

设定正确的内容类型，我已经展示我开发的功能如下：


1.///*<summary>*

2.///* Function to set the content type dynamically.*

3.///* You need to call this function on each mobile web page.*

4.///*</summary>*

5.

6.publicvoid setContentTypeDynamically(HttpRequest Request, HttpResponse Response)

7.{

8.if(Request.Headers["Accept"].ToString().IndexOf

9.("application/vnd.wap.xhtml+xml") != -1)

10.Response.ContentType =
"application/vnd.wap.xhtml+xml";

11.elseif (Request.Headers["Accept"].ToString().IndexOf

12.("application/xhtml+xml") != -1)

13.Response.ContentType =
"application/xhtml+xml";

14.else

15.Response.ContentType =
"text/html";

16.}


功能相当地容易理解，它读取服务端变量Request. Headers ["Accept"]，根据它设定ContentType值在Response对象中。检查ContentType值的顺序是非常重要的，许多流行的移动浏览器支持以上全面类型。



图像是很特殊的，提供图片要根据移动设备屏幕大小显示

移动设备的通常宽度是255个像素，但是手机设备制造商产生多种模式有不同的屏幕大小，你可以检查X-Wap-Profile
 XML file得到移动设备的屏幕大小，我已经讨论了X-Wap-Profile在下面。



显示图片最好的办法是存储一个255像素的图片在数据库中，根据请求的屏幕高和宽来调整图片的显示大小。



举个例子，Motorola L7屏幕大小是176
 x 200（宽 x
高），黑莓8800是320 x 240，诺基亚2630是128
 x 160，为了完美的显示图片，必须在服务器上先调整大小，然后图像Tag指向正确的URL。



为了处理容易，我在数据库中创建了一张表，它读取第一请求中的X-Wap-Profile中的手机模式名称，X-Wap-Profile的URL，屏幕宽度，高度，支持3gp，MP4等保存在数据库中。



我没有每次读取X-Wap-Profile当web页面Request来到，我只在一个新的手机请求第一次来到时候这么做。在第一次Request时所有的设备信息被存储在数据库表中，在所有随后的请求中，我能直接查找屏幕大小信息在我的数据库中，这将减少每次请求X-Wap-Profile的开销，更快的服务一个web
 page为客户端



同一个域名，两个不同的网站（Single Domain, Two Different
 Web Sites）

有时候，你只有一个注册域名，但是你有两个不同网站指向同样的域名。让我们用一个例子来理解这点



你有一个域名比如[http://www.yourwebsitename.com](http://www.yourwebsitename.com/)你将开发两个不同的网站：网站A专注支持于桌面浏览器，网站B是移动页面意味着更小的size设计支持移动浏览器



现在如果[http://www.yourwebsitename.com](http://www.yourwebsitename.com/)被请求从桌面浏览器，你可能显示正常网站A的主页，如果请求来任何GPRS相关的设备，你就显示给它们移动网站B的主页。



为什么你两个网站用同样的域名（Why You Want the Same
 Domain Name for Both of the Websites?）

答案很简单：

1.你将受益于来自两个网站的SEO

2.两个网站接受同样的品牌价值和客户信任

3.市场和广告可以在两个网站间共享

4.你也可以拥有域名[http://www.yourwebsitename.mobi](http://www.yourwebsitename.mobi/)但是更合理的是显示给一个用户移动主页而不是正常主页如果一个请求来自一个移动设备。



代码检测移动浏览器（Code to Detect a Mobile Browser）

下面的代码解决上面的问题，我已经创建一个功能来检查Request中的X-Wap-Profile，如果它被发现，我重定位它们到移动网页否则我定位它们到正常网页。


*// Declare a boolean variable for**mobile** browser*

bool IsMobi =
false;



*// If request header can find the X-Wap-Profile, then it is a**mobile** browser*

if (Request.Headers["X-Wap-Profile"] !=
null)

{

if (Request.Headers["X-Wap-Profile"].ToString().Length >
0)

{

IsMobi = true;

Response.Redirect("index.aspx", 
true);

}

Else

{

Response.Redirect("web/index.aspx", 
true);

}

}




什么是请求头（What are Request. Headers?）

HTTP请求头允许客户端提供自己的信息给服务器，它给出额外信息关于请求种类像类容类型（Content
 Type），接受字符集(Accept-Charset),
接受编码（Accept-Encoding）,
接受语言（Accept-Language）等.



什么是X-Wap-Profile

当你请求一个移动浏览器，它发送更多的信息返回给服务器，其中之一是连接X-Wap-Profile（X-Wap-Profile:
[http://gsm.lge.com/html/gsm/LG-KG220.xml](http://gsm.lge.com/html/gsm/LG-KG220.xml)）



X-Wap-Profile是一个XML文件提供移动设备所有的详细信息，你可以发现移动设备屏幕的大小，支持的音频和视频类型，设备模式名称，其它更多的事情。



手机设备制造商提供这个XML文件目的是在外面使用它。



代码只是用在所有GPRS支持的移动设备上嘛

答案是NO，窍门并没有上面的困难，有些移动设备没有X-Wap-Profile属性相反的他们可能有下面之一：

üX-Wap-Profile


üX-Wap-Profile:


üProfile


üProfile:

而且基于我的经验某些三星和LG的移动设备用不同写法对于同样的属性，它们是下面之一：

üX-Wap-Profile


üx-wap-profile


üX-WAP-PROFILE

最好的方法是在一个方法里面检查所有情况，然后根据值来重定位到相关正确的网页。



一些提示和窍门（Some Tips and Tricks）

1.把移动网页放在父目录，正常网页放在子目录

如果请求来自移动设备这将避免重定向，它将节省你的CPU轮询，也帮助你给出一个快的响应对用户（Request），没有用户能等上两秒为呈现页面，一个理想的移动网页在任何移动设备上呈现应该在两秒之内。

2.iPhone是个特殊的设备，我想我的网站提供一个正常的桌面网页在iPhone上，为此我该做些什么呢？

在这种情况下，你会检查User Agent中值是否为iPhone或者iPod。如果发现，你需要在Metatag中设置ViewPort属性，下面的例子中，我不得不在page
 hearder中写下iPhone字样：


1.<headid="Head1"
runat="server">

2.<asp:literalid="iphone"
runat="server"></asp:literal>

3.</head>


在后台的代码中，我的检查代码显示如下：


1.// If requesting User Agent is an IPhone or IPod than set the iphone label

2.if (Request.UserAgent.ToLower().Contains("iphone"))

3.{

4.*// Set the Viewport attribute.*

5.iphone.Text =
"<meta name=/"viewport/" content=/"width = 

6.device-width height = device-height /" />";

7.}

8.elseif (Request.UserAgent.ToLower().Contains("ipod"))

9.{

10.*// Set the Viewport attribute.*

11.iphone.Text =
"<meta name=/"viewport/" content=/"width = 

12.device-width height = device-height /" />";

13.}




总结Conclusion

个人经验，我已经在200种移动设备上测试过上述代码，它工作良好。

我没有包含一个Dome的项目是因为上述的代码非常简单容易理解和应用，如果你在理解这些代码和概念有任何问题，请随时联系我



作者的E-Mail:[mahesh@indianic.com](mailto:mahesh@indianic.com)

原文地址 -
**http://www.codeproject.com/KB/mobile/All-about-a-mobile-websit.aspx**



