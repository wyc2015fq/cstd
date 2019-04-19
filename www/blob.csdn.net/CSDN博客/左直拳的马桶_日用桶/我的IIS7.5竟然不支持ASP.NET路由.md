# 我的IIS7.5竟然不支持ASP.NET路由 - 左直拳的马桶_日用桶 - CSDN博客
2013年08月14日 19:15:58[左直拳](https://me.csdn.net/leftfist)阅读数：18809
MVC模式下那些友好，屏蔽具体物理文件的URL让我眼馋，咱也想在WEB FORM项目用上一用。
按照指引，添加global.asax，写上路由代码什么的：
```
<%@ Application Language="C#" %>
<%@ Import Namespace="System.Web.Routing" %>
<script runat="server">
    void Application_Start(object sender, EventArgs e)
    {
        // 在应用程序启动时运行的代码
        RegisterRoutes(RouteTable.Routes);
    }
    public static void RegisterRoutes(RouteCollection routes)
    {
        routes.MapPageRoute("", "", "~/default.aspx");
        routes.MapPageRoute("test",
            "h/{product}",
            "~/temp/test.aspx");
    }
```
在IIS下一运行，居然报错，Page Not Found！
然后在VS2012里运行却可以，排除是路由写得不对的问题。
有大牛说，IIS7以前的版本，需要指定后缀名，加上.aspx，比如，应当用这种方式
```
routes.MapPageRoute("test",
            "h.aspx/{product}",
            "~/temp/test.aspx");
```
次奥，那这个路由还有什么意义？不过我试了一下，还真可以。但这样就没意思了。
这说明，我这个IIS7.5里面，如果不指定.ASPX，根本没有经过ASP.NET的处理，路由不起作用。
这很奇怪，我的IIS是WIN7下的IIS7.5，应用程序池模式是集成，ASP.NET4.0，一切都符合传说中的“无须任何配置就可以直接使用ASP.NET路由”的条件。
难道是因为我装了微软的URL重写模块？卸载，重启，不行。
重新注册ASP.NET4.0，也是不行。
谁来告诉我？
======================================================================================
2013-09-11
洒家来告诉你。
只要在web.config里，<system.webServer>加上这句就可以鸟：
```
<system.webServer>
<modules runAllManagedModulesForAllRequests="true" />
</system.webServer>
```
应用程序池的模式需要为集成模式。
如果是经典模式，则要使用后缀名了，路由中的路径如果没有特定后缀名的话，是到达不了ASP.NET处理这一层的。当然可以使用.aspx、.ashx、.asmx这样的后缀名，如：
```
routes.MapRoute(
            "test2", // 路由名称
            "{controller}.aspx/{action}/{id}", // 带有参数的 URL
            new { controller = "Home", action = "Index", id = UrlParameter.Optional } // 参数默认值
        );
```
不过容易混淆，如果项目是WEBFORM和MVC混用的话，就更加混乱了。因为为了避免对aspx页面使用MVC路由，通常会在global.asax里声明
```
routes.IgnoreRoute("{resource}.axd/{*pathInfo}");
        routes.IgnoreRoute("{resource}.aspx/{*pathInfo}");
```
这样MVC路由中有.aspx的将会被过滤掉。
可以使用.mvc：
```
routes.MapRoute(
            "test2", // 路由名称
            "{controller}.mvc/{action}/{id}", // 带有参数的 URL
            new { controller = "Home", action = "Index", id = UrlParameter.Optional } // 参数默认值
        );
```
注意.mvc不是ASP.NET保留的后缀名，需要在IIS里映射。有些文章里说装了MVC以后就会自动支持，但我的操作系统是WIN7，IIS为7.5，装了VS2012，从微软站点下载了MVC3、MVC4都装不上，IIS里也不默认支持MVC后缀，没关系，自己映射也一样的：
![](https://img-blog.csdn.net/20130911183048921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
或者直接在web.config里写也是一样的：
```
<system.webServer>
    <handlers>
        <add name="MVC" path="*.mvc" verb="*" modules="IsapiModule" scriptProcessor="C:\Windows\Microsoft.NET\Framework\v4.0.30319\aspnet_isapi.dll" resourceType="Unspecified" preCondition="classicMode,runtimeVersionv4.0,bitness32" />
    </handlers>
```
但是这些似乎都是多此一举。因为如果是IIS7，用集成模式就什么问题都解决了。如果是IIS6，我在两台win2003 sp2 + IIS6上试过，安装了MVC3 + MVC4后，也跟集成模式一样，根本不用什么后缀名。
