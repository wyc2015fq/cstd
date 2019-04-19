# ASP.NET Core WebAPI 开发-新建WebAPI项目 - 文章 - 伯乐在线
原文出处： [LineZero](http://www.cnblogs.com/linezero/p/5497472.html)
ASP.NET Core WebAPI 开发-新建WebAPI项目，
ASP.NET Core 1.0 RC2 即将发布，我们现在来学习一下 ASP.NET Core WebAPI开发。
网上已经有泄露的VS2015 Tooling，需要VS2015 Update 2。
> 
.NET Core 1.0.0 RC2 SDK Preview1 win64 http://download.microsoft.com/download/2/1/0/2107669A-0DF9-4A91-A275-74735D433045/dotnet-dev-win-x64.1.0.0-preview1-002702.zip
WinSvr Hosting https://download.microsoft.com/download/4/6/1/46116DFF-29F9-4FF8-94BF-F9BE05BE263B/DotNetCore.1.0.0.RC2-WindowsHosting.exe
VS2015 Tooling https://download.microsoft.com/download/4/6/1/46116DFF-29F9-4FF8-94BF-F9BE05BE263B/DotNetCore.1.0.0.RC2-VS2015Tools.Preview1.exe
我也在第一时间安装上了，终于可以使用VS 开发 .NET Core 应用程序了。
安装好VS2015 Tooling 以后，我们在新建项目里就可以选择 .NET Core 。
## 新建WebAPI项目
新建项目
![](http://jbcdn2.b0.upaiyun.com/2016/06/ba7d53b2183ce2241a5dfed7581bc0a7.png)
![](http://jbcdn2.b0.upaiyun.com/2016/06/fa8996143e1baad23c8c5252da5739eb.png)
![](http://jbcdn2.b0.upaiyun.com/2016/06/3f8d89bc2f231920a5df0fa6fd872247.png)
这里就新建好了webapi 项目。
ASP.NET Core 应用程序运行，可以选择 IIS Express 也可以选择 自己运行 ，采用的Server是 Kestrel。
这里我们采用第二种 Kestrel。
![](http://jbcdn2.b0.upaiyun.com/2016/06/5e31968d3ccefedf953d127e883c964d.png)
F5 程序也就运行起来了，VS会自动打开浏览器。
这里有时候可能会浏览器打开，但是无法访问，因为程序还没有跑起来，待程序运行起来刷新既可。
## 添加API
在 Controllers 文件夹右键，添加-》新建项  选择 Web API 控制器类。
我们添加一个**UsersController**
![](http://jbcdn2.b0.upaiyun.com/2016/06/614e907ea25c53e73702b09a224f23e8.png)

C#
```
[Route("api/[controller]")]
    public class UsersController : Controller
    {
        // GET: api/values
        [HttpGet]
        public IEnumerablestring> Get()
        {
            return new string[] { "linezero", "cnblogs" };
        }
        // GET api/values/5
        [HttpGet("{id}")]
        public string Get(int id)
        {
            return $"linezero-{id}";
        }
    }
```
这里的路由是直接写在Controller 里。


C#
```
[Route("api/[controller]")]
```
程序运行起来访问：http://localhost:5000/api/users
![](http://jbcdn2.b0.upaiyun.com/2016/06/069b1c4aeb79e51ac34886f85d0cf10b.png)
http://localhost:5000/api/users/1
![](http://jbcdn2.b0.upaiyun.com/2016/06/0618ef5430a79738f91ec42d02ade81a.png)
开发 ASP.NET Core 应用程序，还是VS好用。之前用VS Code 总有些不适应。
