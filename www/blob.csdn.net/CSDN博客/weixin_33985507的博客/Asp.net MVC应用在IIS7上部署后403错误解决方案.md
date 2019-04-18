# Asp.net MVC应用在IIS7上部署后403错误解决方案 - weixin_33985507的博客 - CSDN博客
2014年03月03日 22:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
1、检查服务器上是否安装了“HTTP重定向”功能和“静态内容压缩”功能（在添加/删除程序或增加角色处安装）
2、应用程序池要被配置为“集成”
3、如果IIS在.Net Framework之后安装的，需要将.Net Framework重新注册到IIS，使用aspnet_regiis -i或者aspnet_regiis -ir命令
切换到cd \Windows\Microsoft .NET\Framework\v4.xxx.xxx或者cd \Windows\Microsoft .NET\Framework64\v4.xxx.xxx目录执行
4、确保自己的web.config里有
<modulesrunAllManagedModulesForAllRequests="true"/>
5、 在应用程序池高级设置里面允许32位应用程序运行
今天刚好碰到这个问题，参考第三条解决的，写下这篇文章备忘
参考
[http://stackoverflow.com/questions/1741439/asp-mvc-in-iis-7-results-in-http-error-403-14-forbidden](http://stackoverflow.com/questions/1741439/asp-mvc-in-iis-7-results-in-http-error-403-14-forbidden)
