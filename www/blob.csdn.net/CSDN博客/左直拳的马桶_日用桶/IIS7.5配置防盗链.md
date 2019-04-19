# IIS7.5配置防盗链 - 左直拳的马桶_日用桶 - CSDN博客
2012年03月22日 17:58:57[左直拳](https://me.csdn.net/leftfist)阅读数：3803标签：[iis																[input																[url																[微软																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=微软&t=blog)](https://so.csdn.net/so/search/s.do?q=url&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=iis&t=blog)
个人分类：[IIS																[.NET																[Web](https://blog.csdn.net/leftfist/article/category/674421)](https://blog.csdn.net/leftfist/article/category/94497)](https://blog.csdn.net/leftfist/article/category/2459577)
首先，要下载、安装一个IIS 重写模块。是到微软站点下载的，可以放心了。（靠，之前以为IIS7是内置了的，想不到还是要另外安装东西）
64位：
[http://www.microsoft.com/downloads/zh-cn/details.aspx?familyid=1b8c7bd8-8824-4408-b8fc-49dc7f951a00](http://www.microsoft.com/downloads/zh-cn/details.aspx?familyid=1b8c7bd8-8824-4408-b8fc-49dc7f951a00)
32位：
[http://www.microsoft.com/zh-cn/download/details.aspx?id=5747](http://www.microsoft.com/zh-cn/download/details.aspx?id=5747)
安装完以后，修改网站的web.config，加入
```
<system.webServer>
<rewrite>
<rules>
<rule name="Prevent hotlinking">
<match url="^.*\.(rar|zip)$" ignoreCase="true" />
<conditions>
<add input="{HTTP_REFERER}" pattern="http://www.fuqi800.com/.*" negate="true" />
<add input="{HTTP_REFERER}" pattern="http://sims.fuqi800.com/.*" negate="true" />
<add input="{HTTP_REFERER}" pattern="http://www.hudie.la/.*" negate="true" />
</conditions>
<action type="Rewrite" url="/404.htm" />
</rule>
</rules>
</rewrite>
</system.webServer>
```
文件中设置了只允许http://www.fuqi800.com、http://sims.fuqi800.com、http://www.hudie.la调用网站的rar、zip类型的文件。

