# 解决XP下iis5.1访问 IIS 元数据库失败 - Big Smile - CSDN博客
2016年11月21日 16:34:31[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1405
        今天永和维护，解决了一个问题，一个困扰了我们一个下午加半个晚上的问题，还是发布的经验不是特别多引起的，借这个机会来分享一下收获的经验。
# 一、遇到的问题
        其实就是访问 IIS 元数据库失败这个主要问题，具体内容如下：
```
访问 IIS 元数据库失败。 
说明: 执行当前 Web 请求期间，出现未处理的异常。请检查堆栈跟踪信息，以了解有关该错误以及代码中导致错误的出处的详细信息。
异常详细信息: System.Web.Hosting.HostingEnvironmentException: 访问 IIS 元数据库失败。
用于运行 ASP.NET 的进程帐户必须具有对 IIS 元数据库(如 IIS://servername/W3SVC)的读访问权。有关如何修改元数据库权限的信息，请参见 http://support.microsoft.com/?kbid=267904。
源错误:
执行当前 Web 请求期间生成了未处理的异常。可以使用下面的异常堆栈跟踪信息确定有关异常原因和发生位置的信息。
堆栈跟踪: 
[HostingEnvironmentException: 访问 IIS 元数据库失败。]
   System.Web.Configuration.MetabaseServerConfig.MapPathCaching(String siteID, VirtualPath path) +690    System.Web.Configuration.MetabaseServerConfig.System.Web.Configuration.IConfigMapPath.MapPath(String siteID, VirtualPath vpath) +9
   System.Web.Hosting.HostingEnvironment.MapPathActual(VirtualPath virtualPath, Boolean permitNull) +174
   System.Web.CachedPathData.GetConfigPathData(String configPath) +398
   System.Web.CachedPathData.GetConfigPathData(String configPath) +263
   System.Web.CachedPathData.GetVirtualPathData(VirtualPath virtualPath, Boolean permitPathsOutsideApp) +166
   System.Web.Configuration.RuntimeConfig.GetLKGRuntimeConfig(VirtualPath path) +187
--------------------------------------------------------------------------------
版本信息: Microsoft .NET Framework 版本:4.0.30319.42; ASP.NET 版本:4.0.30319.42
```
        这个问题让我们一度以为是IIS没有安装好引起的，因为xp系统是不自带IIS的需要从光盘上装，所以就换了好几个版本的IIS然后重装了N多次，但是这个问题依旧，后来由于太晚了，也没有继续弄下去，让客户先去休息了。
# **二、解决**
        第二天，以来客户就发来消息说怎么还是登陆不上，没有界面呢，于是又开始了漫长的查错，调整设置的阶段。最后在一个博客上看到了解决方案，和大家分享一下：
       出现这个问题有两种解决方案，一个是因为ASP.NET没有在IIS上注册的原因，导致了ASP.NET的东西不可用引发了权限的问题，二就只是由于没有充足的权限的原因。
        第一个点还是比较像的，因为在IIS中的浏览器里面，页面是可以出来的，并且只有文字，没有asp.net的控件，这也就说明了是asp.net没有注册的原因，于是需要先注册asp.net 4.0的东西：直接运行——cmd——cd 到.net相应版本的目录下例如这里是cd C:\WINDOWS\Microsoft.NET\Framework\v4.0.30319  （xp系统的特殊路径，win7是不一样的路径），然后要做的就是输入 **aspnet_regiis.exe -i** 回车运行就可以了。
   第二种情况就是在已经注册了asp.net但是还是不行的情况下要做的，给asp.net在iis中提权，方法还是到上一步的目录下面，执行这个：**aspnet_regiis -ga aspnet **就可以了
