# Web 应用层分类和识别 - 我相信...... - CSDN博客





2014年09月15日 08:08:14[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2847
个人分类：[架构](https://blog.csdn.net/wireless_com/article/category/2540035)









现在，也许很少有人知道Corba吧，那些了解orb的人是很容易理解web service乃至于现在的Web应用。




一个web应用一般可以分为5横1纵，即5个水平层和1切面：

1）运行环境：操作系统，Web 容器，CGI等

2）应用框架：Thinkphp，rails，django等

3）应用本身：论坛，社交，博客等

4）前端框架：AngularJS，jQuery，Bootstrap等

5）第三方的内容混搭：Google AD，mashup 等

一个切面就是各层对应的各种插件体系。




一个web 应用可以从Http头，Html数据 和 特定的URL或文件等诸多方面获得相关的特征。


- Http 头分为请求头信息和相应头信息，请求头信息一般包括Host，User-Agent,Accept,Accept-Language,Accept-Encoding,Accept-Charset,Keep-Alive,Connection,Referer,Cookie等；响应头信息一般包括Cache-Control,Date,Content-Type,Server,X-Powered-By.Set-Cookie,content-Encoding等。
- HTML数据包括：元数据，作者，Powered by，Html注释 <!-- -->，网页的框架，特殊页面等。
- 特定的URL或文件包括：特定的静态文件内容（css、js、html、txt）,特定静态文件的 MD5 （2进制文件等）,特定目录 （或多个目录组合）等

根据特征分类，可以识别出不同的应用类型，发现很多有意思的事情。




有时间看看国外的项目吧：WhatWeb,Blind Elephant,WAFP(Web Application Finger Printing) ,Wapplyzer .......














