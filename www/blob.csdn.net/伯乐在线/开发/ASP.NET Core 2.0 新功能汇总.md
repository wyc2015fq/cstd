# ASP.NET Core 2.0 新功能汇总 - 文章 - 伯乐在线
原文出处： [Savorboard](http://www.cnblogs.com/savorboard/p/aspnetcore2-feature.html)
### 前言
ASP.NET Core 的变化和发展速度是飞快的，当你发现你还没有掌握 ASP.NET Core 1.0 的时候， 2.0 已经快要发布了，目前 2.0 处于 Preview 1 版本，意味着功能已经基本确定，还没有学习过 ASP.NET Core 的同学可以直接从 2.0 开始学起，但是如果你已经掌握了 1.0 的话，那么你只需要了解在 2.0 中增加和修改的一些功能即可。
每一次大版本的发布和升级，总会带给开发人员一些惊喜和令人兴奋的特性，有关 ASP.NET Core 本次的 2.0 版本的新特性，主要集中在几个部分上。
### SDK 的变化
PS: 目前如果你想在VS中体验 ASP.NET Core 2.0 全部特性的话，你需要 VS 2017.3 预览版本。当然你可以使用 VS Core 来快速了解。
.NET Core 2.0 Priview 的下载地址：
[https://www.microsoft.com/net/core/preview#windowscmd](https://www.microsoft.com/net/core/preview#windowscmd)
完成之后可以在 cmd 中使用以下命令查看版本。
![](http://jbcdn2.b0.upaiyun.com/2017/06/ab55900c39118415725f32ac7906f437.png)
**变化1**：添加了如下图箭头所指新命令。
![](http://jbcdn2.b0.upaiyun.com/2017/06/c1586e500a04c565a23bbe72f2cf2f0d.png)


```
dotnet new razor
dotnet new nugetconfig
dotnet new page
dotnet new viewimports
dotnet new viewstart
```
添加了这些新的cli命令。 其中 `viewimports`,`viewstart` 即为Razor视图中的_xxx.cshtml那两个文件.
**变化2**： `dotnet new xxx` 将会自动还原 NuGet 包，不需要你再次进行 `dotnet restore` 命令了。

```
G:SampleASPNETCore2 > dotnet new mvc
The template "ASP.NET Core Web App (Model-View-Controller)" was created successfully.
This template contains technologies from parties other than Microsoft, see https://aka.ms/template-3pn for details.
Processing post-creation actions...
Running 'dotnet restore' on G:SampleASPNETCore2ASPNETCore2.csproj...
Restore succeeded.
```
### *.csproj 项目文件
在 2.0 中，当创建一个 MVC 项目的时候，生成的 csporj 项目文件如下：
![](http://jbcdn2.b0.upaiyun.com/2017/06/97fdfcb1ba546862f303e979c9e1abf6.png)
其中，红色箭头部分为新增内容，我们依次来看一下：
**MvcRazorCompileOnPublish**：
在 1.0 版本中，如果我们需要在发布的时候编译 MVC 中的 Views 文件夹为DLL的话，需要引用
`Microsoft.AspNetCore.Mvc.Razor.ViewCompilation` 这个 NuGet 包，而现在已经不需要了，这个功能已经默认的集成在了SDK中，只需要在csporj添加配置即可，在发布的时候将会自动打包 Views 文件夹中的 *.cshtml 文件为 DLL 程序集。
**PackageTargetFallback**
这个配置项是用来配置当前程序集支持的目标框架。
**UserSecretsId**
这个是用来存储程序中使用的机密，以前是存储在 `project.json` 文件中，现在你可以在这里进行配置了。
有关 UserSecrets 的更多信息，可以查看我的[这篇博客文章](http://www.cnblogs.com/savorboard/p/dotnetcore-user-secrets.html)。
**MVC 相关包**


```
<packagereference include="Microsoft.AspNetCore.All" version="2.0.0-preview1-final"/>
```
在 Core MVC 2.0 中，所有MVC相关的NuGet 包都被集成到了这个`Microsoft.AspNetCore.All`包中，它是一个元数据包，包含了大量的东西，其中包括：Authorization, Authentication, Identity, CORS, Localization, Logging, Razor, Kestrel 等，除了这些它还附加了 EntityFramework, SqlServer, Sqlite 等包。
有些同学可能会觉得这样会引用了很多项目中使用不到的程序集，导致发布后的程序变得很庞大，不过我要告诉你不必担心，发布后的程序集不但不会变得很大，反而会小很多，因为 Microsoft 把所有的这些依赖全部都集成到了sdk中，也就是说当你安装sdk的之后，MVC相关的包就已经安装到了你的系统上。
这样的好处是你不用担心更新Nuget包或者删除的时候，因为大量的版本不一致问题导致隐藏的冲突问题，另外一个好处就是，这样对于很多新手的话就很友好 2333，他们不需要知道他们什么情况下会从那个NuGet 包中获取自己需要的信息。
**现在**，发布后的文件夹是如此简洁： 大小 4.3M
![](http://jbcdn2.b0.upaiyun.com/2017/06/45fbe5a1339d55457e306c99537e2275.png)
再贴个**以前的** 发布后的文件夹你们感受一下: 大小 16.5M
![](http://jbcdn2.b0.upaiyun.com/2017/06/4ed198a340b62b14d8b07293a2729a09.png)
有些同学可能好奇他们把那些引用的 MVC 包放到哪里了，默认情况下他们位于这个目录：
`C:Program Filesdotnetstorex64netcoreapp2.0`
### 新的 Program.cs 和 Startup.cs
现在，当创建一个 ASP.NET Core 2.0 MVC 程序的时候，Program 和 Startup 已经发生了变化，他们已经变成了这样：
*Program.cs*

```
public class Program
{
    public static void Main(string[] args)
    {
        BuildWebHost(args).Run();
    }
    public static IWebHost BuildWebHost(string[] args) =>
        WebHost.CreateDefaultBuilder(args)
            .UseStartup<Startup>()
            .Build();
}
```
*Startup.cs*


```
public class Startup
{
    public Startup(IConfiguration configuration)
    {
        Configuration = configuration;
    }
    
    public IConfiguration Configuration { get; }
    
    public void ConfigureServices(IServiceCollection services)
    {
        services.AddMvc();
    }
    public void Configure(IApplicationBuilder app, IHostingEnvironment env)
    {
        if (env.IsDevelopment())
        {
            app.UseDeveloperExceptionPage();
        }
        else
        {
            app.UseExceptionHandler("/Home/Error");
        }
        app.UseStaticFiles();
        app.UseMvc(routes =>
        {
            routes.MapRoute(
                name: "default",
                template: "{controller=Home}/{action=Index}/{id?}");
        });
    }
}
```
可以发现，新的 Program.cs 中和 Startup.cs 中的内容已经变得很简单了，少了很多比如 appsetting.json 文件的添加，日志中间件， Kertrel ， HostingEnvironment 等，那么是怎么回事呢？ 其他他们已经被集成到了 `WebHost.CreateDefaultBuilder` 这个函数中，那么我们跟进源码来看一下内部是怎么做的。
#### WebHost.CreateDefaultBuilder
下面是 `WebHost.CreateDefaultBuilder` 这个函数的源码：

```
public static IWebHostBuilder CreateDefaultBuilder(string[] args)
{
    var builder = new WebHostBuilder()
        .UseKestrel()
        .UseContentRoot(Directory.GetCurrentDirectory())
        .ConfigureAppConfiguration((hostingContext, config) =>
        {
            var env = hostingContext.HostingEnvironment;
            config.AddJsonFile("appsettings.json", optional: true, reloadOnChange: true)
                  .AddJsonFile($"appsettings.{env.EnvironmentName}.json", optional: true, reloadOnChange: true);
            if (env.IsDevelopment())
            {
                var appAssembly = Assembly.Load(new AssemblyName(env.ApplicationName));
                if (appAssembly != null)
                {
                    config.AddUserSecrets(appAssembly, optional: true);
                }
            }
            config.AddEnvironmentVariables();
            if (args != null)
            {
                config.AddCommandLine(args);
            }
        })
        .ConfigureLogging((hostingContext, logging) =>
        {
            logging.UseConfiguration(hostingContext.Configuration.GetSection("Logging"));
            logging.AddConsole();
            logging.AddDebug();
        })
        .UseIISIntegration()
        .UseDefaultServiceProvider((context, options) =>
        {
            options.ValidateScopes = context.HostingEnvironment.IsDevelopment();
        })
        .ConfigureServices(services =>
        {
            services.AddTransient<IConfigureOptions<KestrelServerOptions>, KestrelServerOptionsSetup>();
        });
    return builder;
}
```
可看到，新的方式已经隐藏了很多细节，帮助我们完成了大部分的配置工作。但是你知道怎么样来自定义这些中间件或者配置也是必要的技能之一。
### appsettings.json 的变化
在 appsettings.json 中，我们可以定义 Kestrel 相关的配置，应用程序会在启动的时候使用该配置进行Kerstrel的启动。

```
{
    "Kestrel": {
        "Endpoints": {
            "Localhost": {
                "Address": "127.0.0.1",
                "Port": "9000"
            },
            "LocalhostHttps": {
                "Address": "127.0.0.1",
                "Port": "9001",
                "Certificate": "Https"
            }
        }
    },
    "Certificate": {
        "HTTPS": {
            "Source": "Store",
            "StoreLocation": "LocalMachine",
            "StoreName": "MyName",
            "Subject": "CN=localhost",
            "AllowInvalid": true
        }
    },
    "Logging": {
        "IncludeScopes": false,
        "LogLevel": {
            "Default": "Warning"
        }
    }
}
```
以上配置内容配置了 Kertrel 启动的时候使用的本地地址和端口，以及在生产环境需要使用的 HTTPS 的配置项，通常情况下关于 HTTPS 的节点配置部分应该位于 appsettings.Production.json 文件中。
现在，`dotnet run`在启动的时候将同时监听 9000, 和 9001 端口。
### 日志的变化
在 ASP.NET Core 2.0 中关于日志的变化是非常令人欣慰的，因为它现在不是作为MVC中间件配置的一部分了，而是 Host 的一部分，这句话好像有点别扭，囧~。 这意味着你可以记录到更加底层产生的一些错误信息了。
现在你可以这样来扩展日志配置。


```
public static IWebHost BuildWebHost(string[] args) =>
        WebHost.CreateDefaultBuilder(args)
        .UseStartup<Startup>()
        .ConfigureLogging(factory=>{你的配置})
        .Build();
```
### 全新的 Razor Pages
ASP.NET Core 2.0 引入的另外一个令人兴奋的特性就是 Razor Pages。提供了另外一种方式可以让你在做Web 页面开发的时候更加的沉浸式编程，或者叫 page-focused 。额…它有点像以前 Web Form Page，它隶属于 MVC 框架的一部分，但是他们没有 Controller。
你可以通过 `dotnet new razor` 命令来新建一个 Razor Pages 类型的**应用程序**。
Razor Pages 的 cshtml 页面代码可能看起来是这样的：

```
@ page
@{
    var message = "Hello, World!";
}
<html>
<body>
    <p>@ message</p>
</body>
</html>
```
Razor Pages 的页面必须具有 `@page` 标记。他们可能还会有一个 `*.cshtml.cs` 的 class 文件，对应的页面相关的一些代码，是不是很像 Web Form 呢？
有同学可能会问了，没有 Controller 是怎么路由的呢？ 实际上，他们是通过文件夹物理路径的方式进行导航，比如：
|文件名和路径|匹配的URL|
|----|----|
|/Pages/Index.cshtml|`/` or `/Index`|
|/Pages/Contact.cshtml|`/Contact`|
|/Pages/Store/Contact.cshtml|`/Store/Contact`|
有关 Razor Pages的更多信息可以看这里：
[https://docs.microsoft.com/en-us/aspnet/core/razor-pages](https://docs.microsoft.com/en-us/aspnet/core/razor-pages/)
### 总结
可以看到，在 ASP.NET Core 2.0 中，给我们的开发过程带来了很多便利和帮助，他们包括 Program 等的改进，包括 MVC 相关 NuGet 包的集成，包括appsetting.json的服务器配置，以及令人惊讶的Razor Page，是不是已经迫不及待的期待正式版的发布呢？
