# WebForm与MVC混用 - 左直拳的马桶_日用桶 - CSDN博客
2013年09月12日 11:56:53[左直拳](https://me.csdn.net/leftfist)阅读数：11539
在现有的WebForm项目中加入MVC，可以吗？
西蒙说，可以。
怎么加呢？
我的开发环境是：WIN7 + IIS7.5 + VS2012
一、WebForm项目添加引用：
![](https://img-blog.csdn.net/20130912100659000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我都是选了最高的版本。
二、将MVC项目的部分文件拷贝到WEBFORM项目
拷贝controller和views，也许还有content,scripts。Views似乎是必须的，也不能改名字，拷贝到根目录下。Controller应该拷贝到APP_CODE。
![](https://img-blog.csdn.net/20130912111024265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
三、修改配置文件web.config
```
<compilation debug="true" targetFramework="4.0">
      <assemblies>
        <add assembly="System.Web.Razor, Version=2.0.0.0, Culture=neutral, PublicKeyToken=31BF3856AD364E35"/>
        <add assembly="System.Web.Abstractions, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31BF3856AD364E35"/>
        <add assembly="System.Web.Mvc, Version=4.0.0.0, Culture=neutral, PublicKeyToken=31BF3856AD364E35"/>
        <add assembly="System.Web.WebPages, Version=2.0.0.0, Culture=neutral, PublicKeyToken=31BF3856AD364E35"/>
      </assemblies>
    </compilation>
    <pages>
      <namespaces>
        <add namespace="System.Web.Mvc"/>
        <add namespace="System.Web.Mvc.Ajax"/>
        <add namespace="System.Web.Mvc.Html"/>
        <add namespace="System.Web.Routing"/>
        <add namespace="System.Linq"/>
        <add namespace="System.Collections.Generic"/>
      </namespaces>
    </pages>
```
这里面，有些内容是添加引用后，系统自动写上去的
最重要的，可别忘了这一句：
`<modules runAllManagedModulesForAllRequests="true"/>`
写在 <system.webServer> 里。没有这一句，将无法使用MVC模式。
四、设置路由
添加global.asax文件，设置路由
```
<%@ Application Language="C#" %>
<%@ Import Namespace="System.Web.Routing" %>  
<%@ Import Namespace="System.Web.Mvc" %>  
<script runat="server">
    void Application_Start(object sender, EventArgs e) 
    {
        // 在应用程序启动时运行的代码
        //AreaRegistration.RegisterAllAreas();
        RegisterRoutes(RouteTable.Routes);        
    }
    public static void RegisterRoutes(RouteCollection routes)
    {
        //避免对 Web 资源文件（例如 WebResource.axd 或 ScriptResource.axd）的请求传递给控制器
        routes.IgnoreRoute("{resource}.axd/{*pathInfo}");
        routes.IgnoreRoute("{resource}.aspx/{*pathInfo}");
        routes.MapRoute(
            name: "store" // 路由名称
            , url: "{id}/{controller}/{action}" // 带有参数的 URL。看到没，不一定是{controller}放在前面，顺序可以自己调
            , defaults: new { controller = @"store", action = @"show", id = UrlParameter.Optional }//默认
            , constraints: new { id = @"[1-9]\d{3,8}" }//约束
        );        
        routes.MapRoute(
            "test2", 
            "{controller}/{action}/{id}", 
            new { controller = "Home", action = "Index", id = UrlParameter.Optional } // 参数默认值
        );
    }
```
其中，routes.IgnoreRoute("{resource}.aspx/{*pathInfo}");是照顾现有这些直接访问.aspx的WEBFORM方式。
基本就是这样了。
对于IIS6，可以到微软下载MVC3，MVC4，依次安装好，就可以直接使用了，也没有出现MSDN上说的，要在路由上加上*.MVC的问题。详见拙作：
[我的IIS7.5竟然不支持ASP.NET路由](http://blog.csdn.net/leftfist/article/details/9969797)
[http://blog.csdn.net/leftfist/article/details/9969797](http://blog.csdn.net/leftfist/article/details/9969797)

