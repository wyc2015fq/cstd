# WebService与共享COOKIE - 左直拳的马桶_日用桶 - CSDN博客
2008年01月24日 18:15:00[左直拳](https://me.csdn.net/leftfist)阅读数：1842标签：[webservice																[domain																[浏览器](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=domain&t=blog)](https://so.csdn.net/so/search/s.do?q=webservice&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
WebService与共享COOKIE
左直拳
现在有两个网站：[www.abc.com](http://www.aaa.com/)，check.abc.com。要求用户打开邮件时，访问check.abc.com，保存COOKIE。以后用户访问[www.abc.com](http://www.aaa.com/)，则检查COOKIE。如果发现在check.abc.com上有COOKIE，则自动登陆。
这大概跟单点登陆有点类似。我刚开始的设想是：在check.abc.com上提供一个WebService，供[www.abc.com](http://www.aaa.com/)调用来检查COOKIE。
但是，在本机上直接用浏览器访问WebService检查，可以准确读取check.abc.com 保存的COOKIE；而运行同样放在本机上的[www.abc.com](http://www.abc.com/)，通过[www.abc.com](http://www.abc.com/)上的页面调用WebService，却怎么也读不出COOKIE。调试了很久，查阅了许多资料都没有结果。
后来有篇文章介绍说：COOKIE可以自动由子级域名分享，无须做什么特别设置。对网站[www.abc.com](http://www.aaa.com/)，check.abc.com来说，它们都是二级域名，一级域名是abc.com。所以，保存COOKIE的时候，将Cookie的Domain设置成“.abc.com”，无论[www.abc.com](http://www.aaa.com/)还是check.abc.com，应该都可以顺利读取。代码如下
publicvoid WriteToCookie(int userId)
{
HttpCookie cookie = newHttpCookie(“UserInfo”);
cookie.Values.Add(“UserId”, userId.ToString());
cookie.Expires = DateTime.Now.AddHours(1);
cookie.Secure = false;
cookie.Domain = ".abc.com";
HttpContext.Current.Response.Cookies.Add(cookie);
}
可是照样不行！
老子一生气，将WebService去掉了，在[www.abc.com](http://www.aaa.com/)里直接读取，一读就读到了。
去他娘的WebService。
