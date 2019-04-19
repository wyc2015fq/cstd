# 初识ASP.NET Core 1.0 - 文章 - 伯乐在线
原文出处： [richiezhang](http://www.cnblogs.com/richieyang/p/5281761.html)
本文将对微软下一代ASP.NET框架做个概括性介绍，方便大家进一步熟悉该框架。
在介绍ASP.NET Core 1.0之前有必要澄清一些产品名称及版本号。ASP.NET Core1.0是微软下一代ASP.NET 框架，在这之前ASP.NET版本稳定在ASP.NET  4.6，对应的.NET Framework版本为.net 4.6.2。
曾经一段时间微软将下一代ASP.NET 命名为ASP.NET 5和MVC 6，在ASP.NET 5 is dead – Introducing ASP.NET Core 1.0 and .NET Core 1.0一文中，微软第一次提到ASP.NET 5将会改为ASP.NET Core1.0。
该文还指出了.NET其他产品命名变化
- **ASP.NET 5 现在叫做 ASP.NET Core 1.0 **
- **.NET Core 现在叫做 .NET Core 1.0 **
- **Entity Framework 7 现在叫做 Entity Framework Core 1.0 或者简称 EF Core 1.0**
之所以有这样的改变，微软解释为：下一代的ASP.NET并不是ASP.NET 4.6的简单升级，如果命名为ASP.NET 5则会给开发者一个错误的暗示，开发者会误认为这只是功能上的升级。而事实是微软首先写了一个轻量级跨平台的.NET Core，然后在该平台下重新设计了ASP.NET，新一代的ASP.NET Core 1.0有着众多新的特性，当然最重要的是实现了跨平台。
![](http://jbcdn2.b0.upaiyun.com/2016/03/1c61362b2248b4b2136e7fe6f50e29f9.png)
值得注意的是从ASP.NET 5到ASP.NET Core 1.0这一命名的转变会对开发人员造成一些困惑，因为在一些ASP.NET相关的网站中，仍然可以不时地看到Asp.net 5 或者MVC 6等名称。事实上，由于ASP.NET Core 1.0正式版还没有发布，命名的改变还在进行当中，整个命名的改变过程到ASP.NET Core1.0正式发布之后才会全部结束。
于此同时也产生了一些新的概念：
**DNX**：.NET Execution Environment，即.NET运行时环境，在Windows,Mac和Linux下运行.NET应用程序的环境(有点目前windows环境下的.NET Framework的意思)，当然这东西是跨平台的，这是跟.NET Framework最大的不同之处。
**DNVM**：NX Version Manager，即DNX的版本管理工具，利用DNVM可以管理DNX的不同版本，你可以轻松切换到不同的DNX版本中。
**.NET Core**：可以理解为一个经过精简的、模块化的.NET Framework子集，目的是为了跨平台。.NET Core有一系列的类库组成，叫做”CoreFX”,一个更精简的版本叫做”CoreCLR”。
整个.NET Core所有类库包括之前提到的ASP.NET Core都是通过Nuget来管理的。
> 
**一、安装ASP.NET Core 1.0**
这一过程在Windows、Mac和Linux下各不相同，微软给出了详细的安装文档，以Windows为例又分为两种方式，下载安装或通过命令行安装。
> 
**二、新建ASP.NET Core项目**
安装完毕后，VS2015会增加对应的模板，值得注意的是该模板目前仍旧叫做ASP.NET 5。
![](http://jbcdn2.b0.upaiyun.com/2016/03/99374d98d49f3b8a73185a43439eafe8.png)
> 
**三、项目结构**
![](http://images2015.cnblogs.com/blog/600216/201603/600216-20160318203728521-1127670047.png)
**1、project.json文件：**
dependencies节点：用来管理Nuget依赖，支持智能提示，这一过程等价于在Nuget package Manager中管理依赖项。
commands节点：在DNX环境下可以使用**dnx [command]**命令来执行一组命令，对于：

C#
```
"commands": {
   "web": "Microsoft.AspNet.Server.Kestrel",
   "ef": "EntityFramework.Commands"
 },
```
可以这样使用：

C#
```
dnx web  //这一命令将会启动KestrelHttp 服务器，KestrelHttpServer是微软基于libuv编写的跨平台web 服务器
dnx ef
```
**2、frameworks节点：**该节点定义了DNX环境

C#
```
"frameworks": {
    "dnx451": { },
    "dnxcore50": { }
  },
```
dnx451表示.NET Framework 4.5.1，dnxcore50表示.NET Core5。
![](http://jbcdn2.b0.upaiyun.com/2016/03/3b6983368c1b8543f3958d11dc2f667d.png)
该节点表明此程序可以跑在这两种DNX环境中，在代码中还可以通过下面的方式针对具体的环境编写代码。

C#
```
#if DNX451
  // utilize resource only available with .NET Framework
#endif
```
> 
**四、ASP.NET Core1.0带来的新特性**
**1、采用新的文件系统，不再通过工程文件(.sln和.csproj)来定义项目文件清单。**
以本Demo为例，所有添加在AspnetCore.Practice文件夹下的文件都会被自动添加在项目中，举个例子：
打开AspnetCore.PracticeControllers文件夹：在文件夹内手工添加一个文件ServiceController.cs，并添加如下测试代码：

C#
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNet.Mvc;
namespace AspnetCore.Practice.Controllers
{
    public class ServiceController : Controller
    {
        public IActionResult Index()
        {
            return Content("hello world");
        }
    }
}
```
注意，此时打开VS的项目管理器，可以看到ServiceController已经添加到了项目中。这时若在浏览器中直接输入测试代码的url，即可看到测试结果。也就是说VS在后台监视到了新文件便会自动添加在项目中并**自动完成编译。**
![](http://jbcdn2.b0.upaiyun.com/2016/03/ff926c8ef895c7f8dfd524d04de7676b.png)
**2、Startup类。**
该类可以看作整个ASP.NET Core的启动入口，该类主要存在3个方法：Main函数是入口点，方法ConfigureServices用来向IOC容器中注册组建，方法Configure则用来注册Middleware。
也许你第一次见到这个类会有点不明觉厉，该类中3个方法既没有接口约束，也没有从父类继承。微软在该处采用了这样一种约定：必须要存在一个名叫Startup的类，同时该类必须要存在上面提到的3个方法，该ASP.NET项目才能顺利运行成功。
采用约定而非契约编程的原因在于约定更加灵活。特别是在方法Configure()签名中，参数可以从容器中注入，意味着你可以定义这样的Configure方法：

C#
```
public void Configure(IApplicationBuilder app, IHostingEnvironment env, 
ILoggerFactory loggerFactory)
       {
	//...	
        }
```
也可以定义这样的Configure方法：

C#
```
public void Configure(IApplicationBuilder app, IHostingEnvironment env, 
ILoggerFactory loggerFactory,IUserProvider userProvider)
       {
	//...
        }
```
要保证这样的灵活性对于契约编程很难做到。
即便你对约定这一事实一无所知，一些异常信息也会帮助你朝着正确的方向编写代码。比如当你将Startup类重命名为其他，例如命名为Bootstrapper，你将会得到如下的提示：
![](http://jbcdn2.b0.upaiyun.com/2016/03/963c333f18262be2466032287981d80a.png)
同样的道理，如果方法ConfigureServices或Configure漏写也会得到相似的提示。
该类除了采用约定，大部分代码都是在接口上实现扩展方法的风格，想进一步了解这种代码风格请阅读《再谈扩展方法，从string.IsNullOrEmpty()说起》。
**3、读取Appsetting**
由于已经不再存在web.config文件，所以新的Appsetting也采取了更加通用的设计。在新建项目的时候VS已经帮我们添加了默认的appsettings.json文件。
定义一个键值对：”hello”: “Hello, world”，同时在Startup类的构造函数中将appsettings.json文件添加到了ConfigurationBuilder对象中：

C#
```
public Startup(IHostingEnvironment env)
       {
           // Set up configuration sources.
           var builder = new ConfigurationBuilder()
               .AddJsonFile("appsettings.json")
               .AddJsonFile($"appsettings.{env.EnvironmentName}.json", optional: true);
          //…
           Configuration = builder.Build();
       }
```
同时我们还可以看到一个环境变量env.EnvironmentName，这个设计也有利于我们区分QA，INT，UAT，Production等不同的运行环境。这一变量可以在项目配置中设置：
![](http://jbcdn2.b0.upaiyun.com/2016/03/750222b9a1a8e99d38fcfae1861ed7b1.png)
在代码中可以通过Configuration[“hello”]的方式读取我们之前定义的Appsettings。
**3、默认自带IOC容器，统一依赖注入API**
依赖注入技术从很大程度上使得代码更加模块化，会在一定程度上迫使你写出低耦合，SRP的代码，另外有着良好设计的代码也具备更好的可测试性。
ASP.NET Core自己内置了一个非常轻量级的IOC容器，例如以下代码将组建IEmailSender和ISmsSender分别注册在了容器中。

C#
```
public void ConfigureServices(IServiceCollection services)
        {
            // Add application services.
            services.AddTransient();
            services.AddTransient();
        }
```
当然你可以引入第三方比较成熟的IOC容器，项目Dependency Injection定义了一组抽象，只需要将具体的IOC容器实现该抽象即可整合进ASP.NET Core中。就目前的情况来看ASP.NET Core内置的容器比较适合ASP.NET Core内部的组建使用，而实际业务依赖则可以使用第三方更强大的容器来注册。
另外在ASP.NET Core新的设计中，不光Controller可以进行依赖注入，Filter，View以及ViewModel都可进行注入。这方面的内容比较多，也许会在单独的文章中进行介绍。
**4、Middleware**
这一设计借鉴自OWIN katana 项目的管道设计。什么是Middleware?下面这幅图很好的描述了Middleware是如何在http请求过程中工作的。
![](http://jbcdn2.b0.upaiyun.com/2016/03/ba30266438eb274a56679c404add53ff.png)
在方法Configure中调用的内容都可以理解为Middleware：

C#
```
public void Configure(IApplicationBuilder app, IHostingEnvironment env, ILoggerFactory loggerFactory)  
{
   //…  
    app.UseIISPlatformHandler(); //Middleware
   app.UseStaticFiles(); //Middleware
   app.UseMvc(routes =>
    {
        routes.MapRoute(
            name: "default",
            template: "{controller=Home}/{action=Index}/{id?}");
    });  //Middleware
}
```
注册一个Middleware有两种写法，比如你自定义了一个CustomerMiddleware，
第一种注册方法：
app.UseMiddleware();
第二种则更常用：首先写一个扩展方法

C#
```
public static class MiddlewareExtensions  
{
    public static IApplicationBuilder UseCustomerMiddleware(this IApplicationBuilder builder)
    {
        return builder.UseMiddleware();
    }
}
```
然后就可以这样使用了：app.UseCustomerMiddleware();
自定义Middleware请关注后续文章。
**5、统一MVC和WebAPI**
![](http://jbcdn2.b0.upaiyun.com/2016/03/3416f7c2c862503ed872210850eec955.jpg)
ASP.NET Core统一了MVC和WebAPI，这表现在这两者共用同一套代码，并且在开发过程中不用再继承各自独立的Controller基类了。下面展示了如何在同一个Controller中编写MVC和WebAPI:

C#
```
public class ServiceController : Controller
    {
        private readonly IConfigurationRoot _configurationRoot;
        public ServiceController(IConfigurationRoot configurationRoot)
        {
            _configurationRoot = configurationRoot;
        }
        public IActionResult Index()
        {
            return Content("hello world");
        }
        public User GetUser()
        {
            var user = _configurationRoot.Get("Users");// 读取appsettings.json中的对象
            return user;
        }
    }
```
这一示例采用了构造器注入，为了让IOC容器注入IConfigurationRoot参数，我们需要将该实例注册进IOC容器中：

C#
```
public void ConfigureServices(IServiceCollection services)
        {
           //…          
            services.AddInstance(Configuration);
        }
```
**6、Razor页面中引入新的Tag Helpers**
在之前的Razor页面中，我们可以利用HtmlHelper扩展来完成数据绑定和页面展示，例如：

C#
```
@Html.EditorFor (i => i.Email, new {htmlAttributes = new {@class = "form-control"}})
```
ASP.NET Core设计了新的方案：

C#
```
<input asp-for="Email" class="form-control"/>
```
这种写法更加接近HTML，对纯前段人员更加友好，也更利于结合一些前端的MVVM框架来使用。当然我们还可以根据自己的需求编写自定义的Tag Helpers。使用Tag Helpers需要添加如下Nuget package:

C#
```
"dependencies": {    ...    "Microsoft.AspNet.Mvc.TagHelpers": "6.0.0-rc1-final"}
```
另外一个例子对比：
HtmlHelpers：

C#
```
@using (Html.BeginForm( "Register", "Account", new { ReturnUrl = ViewBag.ReturnUrl }, FormMethod.Post, new { role = "form" })) 
{ 
@Html.AntiForgeryToken() 
@Html.ValidationSummary(true, "", new { @class = "text-danger" })  
}
```
TagHelpers:

C#
```
<form class="form-horizontal" method="post" role="form" asp-controller="Account" asp-action="Register">
<div class="text-danger" asp-validation-summary="ValidationSummary.All"></div>
<!-- other tags or tag helpers -->
</form>
```
**7、其他一些特性如View Components，Caching…**
本文介绍了下一代的ASP.NET版本ASP.NET Core 1.0并且对其新的特性做了概括性的介绍，有助于.NET开发者对ASP.NET Core有一个全面的认识，随着我对ASP.NET Core的进一步了解还会对一些细节进行更进一步的补充，欢迎大家关注。
