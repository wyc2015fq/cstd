# Web程序中利用web.config解决无法输出excel页面的问题 - 左直拳的马桶_日用桶 - CSDN博客
2006年05月29日 17:21:00[左直拳](https://me.csdn.net/leftfist)阅读数：1884标签：[excel																[web																[authorization																[globalization																[forms](https://so.csdn.net/so/search/s.do?q=forms&t=blog)](https://so.csdn.net/so/search/s.do?q=globalization&t=blog)](https://so.csdn.net/so/search/s.do?q=authorization&t=blog)](https://so.csdn.net/so/search/s.do?q=web&t=blog)](https://so.csdn.net/so/search/s.do?q=excel&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
Web程序中利用web.config解决无法输出excel页面的问题
左直拳
WEB应用程序中，有时需要将页面以WORD或EXCEL的形式输出。做法是：
//以WORD的形式输出
Response.ContentType = "application/msword";
//以EXCEL的形式输出
Response.ContentType = "application/vnd.ms-excel";
可是，有时候在一些机器上运行，却并不如预期那样输出WORD或EXCEL页面，而是出现了一个登陆的页面（login.aspx）与输出页面（空白）的混合体，显得不伦不类，古灵精怪。
我们的WEB程序采用“Forms”的身份验证模式，有一个默认的登陆页面login.aspx。通常，在还没有身份验证或验证信息丢失的情况下，系统会强制转向登陆页面，然而这个时候访问其他页面却又一切正常，表明验证信息还在。
那么是什么原因呢？难道验证信息对这个输出WORD或EXCEL格式的页面没起作用吗？
因为机器上的文件系统是NTFS，我将该输出文件的权限设成了EVERYONE完全控制，结果还是不行。
真是莫名其妙。
后来在输出文件所在的目录下添加了一个web.config文件，内容如下：
<configuration>
<system.web>
<customErrorsmode="Off"/>
<authorization>
<allowusers="*"/>
</authorization>
<globalizationrequestEncoding="gb2312"responseEncoding="gb2312"fileEncoding="gb2312"/>
</system.web>
</configuration>
问题解决了。
究其原因，原先也许是因为WORD或EXCEL无法访问WEB应用程序而导致吧。
