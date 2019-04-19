# ASP.NET Core 开发：中间件 - 文章 - 伯乐在线
原文出处： [LineZero](http://www.cnblogs.com/linezero/p/5529767.html)
ASP.NET Core开发，开发并使用中间件(Middleware)。中间件是被组装成一个应用程序管道来处理请求和响应的软件组件。每个组件选择是否传递给管道中的下一个组件的请求，并能之前和下一组分在管道中调用之后执行特定操作。
具体如图：
![](http://jbcdn2.b0.upaiyun.com/2016/05/390775a5883853eb189f2fef62441ef6.png)
## 开发中间件(Middleware)
今天我们来实现一个记录ip 的中间件。
1.新建一个asp.net core项目，选择空的模板。
![](http://jbcdn2.b0.upaiyun.com/2016/05/d8af1edaa3993e19e8457992e5016299.png)
![](http://jbcdn2.b0.upaiyun.com/2016/05/da4e7bbcfed15172af8148061173f046.png)
然后为项目添加一个 Microsoft.Extensions.Logging.Console
NuGet 命令行 ，请使用官方源。
Install-Package Microsoft.Extensions.Logging.Console -Pre
2.新建一个类： RequestIPMiddleware.cs


C#
```
public class RequestIPMiddleware
    {
        private readonly RequestDelegate _next;
        private readonly ILogger _logger;
        public RequestIPMiddleware(RequestDelegate next, ILoggerFactory loggerFactory)
        {
            _next = next;
            _logger = loggerFactory.CreateLogger();
        }
        public async Task Invoke(HttpContext context)
        {            
            _logger.LogInformation("User IP: " + context.Connection.RemoteIpAddress.ToString());
            await _next.Invoke(context);
        }
    }
```
3.再新建一个：RequestIPExtensions.cs


C#
```
public static class RequestIPExtensions
    {
        public static IApplicationBuilder UseRequestIP(this IApplicationBuilder builder)
        {
            return builder.UseMiddleware();
        }
    }
```
这样我们就编写好了一个中间件。
## 使用中间件(Middleware)
1.使用
在 Startup.cs 添加 app.UseRequestIP()


C#
```
public void Configure(IApplicationBuilder app, ILoggerFactory loggerfactory)
        {
            loggerfactory.AddConsole(minLevel: LogLevel.Information);
            app.UseRequestIP();//使用中间件
            app.Run(async (context) =>
            {
                await context.Response.WriteAsync("Hello World!");
            });
        }
```
然后运行程序，我选择使用Kestrel 。
访问：http://localhost:5000/
![](http://jbcdn2.b0.upaiyun.com/2016/05/e5f1cbc73ce2c61fa79cbe99a5a7ee36.png)
成功运行。
这里我们还可以对这个中间件进行进一步改进，增加更多的功能，如限制访问等。
