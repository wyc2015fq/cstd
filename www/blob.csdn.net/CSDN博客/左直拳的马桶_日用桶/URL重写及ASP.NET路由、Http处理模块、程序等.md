# URL重写及ASP.NET路由、Http处理模块、程序等 - 左直拳的马桶_日用桶 - CSDN博客
2013年08月30日 19:39:17[左直拳](https://me.csdn.net/leftfist)阅读数：7705
这段时间在学习ASP.NET路由、HTTP处理等内容，了解了一些，但又未完全弄懂，似是而非，不管如何，作一总结，供日后借鉴和修改。
一、IIS6和IIS7经典模式和集成模式
在IIS6及IIS7经典模式下，对ASP.NET的处理部分是独立出来的，靠aspnet_isapi.dll来处理完成。一个请求从浏览器发到IIS，IIS先判断请求的类型，如果是ASP.NET部分，就转发给aspnet_isapi.dll来处理，处理完了再返回给IIS，再由IIS返回到浏览器。
![](https://img-blog.csdn.net/20130830190856828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
而在IIS7集成模式中，不管什么资源，什么类型，都在同一个管道中完成。
![](https://img-blog.csdn.net/20130830190928593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考文章：
**深入解读IIS 7中应用程序池的管道模式**[http://tech.techweb.com.cn/redirect.php?fid=2&tid=496369&goto=nextnewset](http://tech.techweb.com.cn/redirect.php?fid=2&tid=496369&goto=nextnewset)
Asp.Net生命周期和Http管道技术
[http://www.cnblogs.com/refactor/archive/2012/07/06/2568742.html](http://www.cnblogs.com/refactor/archive/2012/07/06/2568742.html)
也就是说，我写一个HTTP处理模块（module)、程序(handler)，如果在IIS6和经典模式下，对静态页面之类的资源，是不起作用的，但IIS7的集成模式下，就可以。另外，如果我想写一个HTTP  Handler 来实现什么路由：
http://www.test.com/a.action/id/1 ==》http://www.test.com/a.aspx?id=1，则
在IIS6下，需要映射后缀名为.action的文件到aspnet_isapi.dll
在IIS7经典模式下，需要在web.config中注册和声明
在IIS7集成模式下，最简单，仅需少许配置
app_code/myhandler.cs
```
/// <summary>
/// handler 的摘要说明
/// </summary>
public class MyHandler : IHttpHandler
{
    public void ProcessRequest(HttpContext context)
    {
        context.Response.Write("<H1>Hello, I'm an HTTP handler</H1>");
        //HttpRequest request = context.Request;
        //context.Response.Redirect("/default2.aspx");
    }
    public bool IsReusable
    {
        get
        {
            return false;
        }
    }
}
```
IIS7经典模式下的web.config
```
<!--
  有关如何配置 ASP.NET 应用程序的详细信息，请访问
  http://go.microsoft.com/fwlink/?LinkId=169433
  -->
<configuration>
  <system.web>
    <httpHandlers>
      <add verb="*" path="*.action" 
        type="MyHandler" />
    </httpHandlers>
  </system.web>
  <system.webServer>
      <handlers>
        <add name="MyHandler" verb="*" path="*.action" 
          modules="IsapiModule" 
          scriptProcessor="C:\Windows\Microsoft.NET\Framework\v4.0.30319\aspnet_isapi.dll"
          resourceType="Unspecified" />
      </handlers>
  </system.webServer>
</configuration>
```
IIS7集成模式下
```
<?xml version="1.0"?>
<!--
  有关如何配置 ASP.NET 应用程序的详细信息，请访问
  http://go.microsoft.com/fwlink/?LinkId=169433
  -->
<configuration>
  <system.webServer>
      <handlers>
          <add name="SampleHandler" verb="*"
            path="*.action"
            type="MyHandler"
            resourceType="Unspecified" />
      </handlers>
  </system.webServer>
</configuration>
```
参考文章：
# HTTP 处理程序和 HTTP 模块概述
[http://msdn.microsoft.com/zh-cn/library/bb398986(v=vs.100).aspx](http://msdn.microsoft.com/zh-cn/library/bb398986(v=vs.100).aspx)
二、ASP.NET路由
路由不是MVC特有的功能，WEB FORM程序也完全可以采用。问题是，IIS6和IIS7经典模式下，因为上述原因，路径中一定要加后缀名，比如http://www.test.com/a.aspx/id/1，
否则不起作用，URL的优美、友好性不免打上折扣。
还有一个问题，就是采用路由之后，如何读取参数。
习惯了传统URL下，QueryString的方式。但采用路由，是不能像常见的URL重写那样，将参数附在目标页面后面比如
```
routes.MapPageRoute("productinfo",
            "pid.ashx/{id}",
            "~/handler.ashx?pid=");
```
我曾经以为可以将这个{id}，附在目标地址后面，写成这个样子，hanler.ashx?pid=$1，结果没门。
参数是怎么提取的呢？是在目标页面里，用 Page.RouteData.Values["..."]的方式提取。并且这个页面要是aspx页面，ashx还不行。
# ASP.NET 路由
[http://msdn.microsoft.com/zh-cn/library/cc668201(v=vs.100).aspx](http://msdn.microsoft.com/zh-cn/library/cc668201(v=vs.100).aspx)
三、URL重写过时了吗
又有路由，又有自定义的HTTP处理模块、程序，还要URL重写干嘛？微软有一个给IIS7的重写模块外，其他都是杂牌军，总有点怪怪的感觉。
（详见拙作[http://blog.csdn.net/leftfist/article/details/9283141](http://blog.csdn.net/leftfist/article/details/9283141)）
但是，对于IIS6和IIS7经典模式来讲，URL重写可以处理静态资源类地址，证明在IIS处理过程中，先于ASP.NET一步，我相信它的性能要比路由和自定义HTTP处理要高。并且各种正则表达式，复杂是复杂了一点，但毕竟能实现得相当好。
![](https://img-blog.csdn.net/20130830193615171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20130830193644359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20130830193703375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20130830193721734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这些不同WEB服务器下的特点，是我们想实现优美、友好的URL所要考虑的。

