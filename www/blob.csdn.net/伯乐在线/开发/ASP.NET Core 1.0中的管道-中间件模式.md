# ASP.NET Core 1.0中的管道-中间件模式 - 文章 - 伯乐在线
原文出处： [richieyangs](http://www.cnblogs.com/richieyang/p/5315390.html#top)
ASP.NET Core 1.0借鉴了Katana项目的管道设计(Pipeline)。日志记录、用户认证、MVC等模块都以中间件(Middleware)的方式注册在管道中。显而易见这样的设计非常松耦合并且非常灵活，你可以自己定义任意功能的Middleware注册在管道中。这一设计非常适用于“请求-响应”这样的场景——消息从管道头流入最后反向流出。
在本文中暂且为这种模式起名叫做“管道-中间件（Pipeline-Middleware)”模式吧。
本文将描述”管道-中间件模式”的“契约式”设计和“函数式”设计两种方案。
一、什么是管道-中间件模式？
![](http://jbcdn2.b0.upaiyun.com/2016/06/b66f16c9970177ae6960f98de04f758d.png)
在此模式中抽象了一个类似管道的概念，所有的组件均以中间件的方式注册在此管道中，当请求进入管道后：中间件依次对请求作出处理，然后从最后一个中间件开始处理响应内容，最终反向流出管道。
二、契约式设计
契约式设计是从面向对象的角度来思考问题，根据管道-中间件的理解，中间件(Middleware)有两个职责：

C#
```
public interface IMiddleware
    {
        Request ProcessRequest(Request request);
        Response ProcessResponse(Response response);
    }
```
管道(Pipeline)抽象应该能够注册中间件(Middleware)：

C#
```
public interface IApplicationBuilder
    {
  
        void Use(IMiddleware middleware);
        void UseArrange(List middlewares);
        Context Run(Context context);
    }
```
实现IApplicationBuilder：

C#
```
public class ApplicationBuilder : IApplicationBuilder
    {
        public IWindsorContainer Container { get; private set; }
        private readonly List _middlewares;
        public ApplicationBuilder(IWindsorContainer container)
        {
            Contract.Requires(container!=null,"container!=null");
            _middlewares=new List();
            Container = container;
        }
        public void Use(IMiddleware middleware)
        {
            Contract.Requires(middleware != null, "middleware!=null");
            _middlewares.Add(middleware);
        }
        public void UseArrange(List middlewares)
        {
            Contract.Requires(middlewares != null, "middlewares!=null");
            _middlewares.AddRange(middlewares);
        }
        public Context Run(Context context)
        {
            Contract.Requires(context!=null,"context!=null");
            var request=context.Request;
            var response=context.Response;
            foreach (var middleware in _middlewares)
            {
                request = middleware.ProcessRequest(request);
            }
            _middlewares.Reverse();
            foreach (var middleware in _middlewares)
            {
                response = middleware.ProcessResponse(response);
            }
            return new Context(request,response);
        }
    }
```
Run()方法将依次枚举Middleware并对消息的请求和响应进行处理，最后返回最终处理过的消息。
接下来需要实现一个Middleware：

C#
```
public class DefaultMiddleware:IMiddleware
    {
        public Request ProcessRequest(Request request)
        {
            request.Process("default request", "processed by defaultMiddleware");
            return request;
        }
        public Response ProcessResponse(Response response)
        {
            response.Process("default response", "processed by defaultMiddleware");
            return response;
        }
    }
```
为了将Middleware注册进管道，我们还可以写一个扩展方法增加代码的可读性：

C#
```
public static void UseDefaultMiddleware(this IApplicationBuilder applicationBuilder)
        {
            applicationBuilder.Use();
        }
        public static void Use(this IApplicationBuilder applicationBuilder)
            where TMiddleware:IMiddleware
        {
            var middleware = applicationBuilder.Container.Resolve();
            applicationBuilder.Use(middleware);
        }
```
写个测试看看吧：
![](http://jbcdn2.b0.upaiyun.com/2016/06/97c4f971f9908b7e3e5af42dccaa0a95.png)
写第二个Middleware：

C#
```
public class GreetingMiddleware:IMiddleware
    {
        public Request ProcessRequest(Request request)
        {
            request.Process("hello, request","processed by greetingMiddleware");
            return request;
        }
        public Response ProcessResponse(Response response)
        {
            response.Process("hello, request", "processed by greetingMiddleware");
            return response;
        }
    }
```
编写测试：
![](http://jbcdn2.b0.upaiyun.com/2016/06/423c66966d0abb316440d1fc551ec7f9.png)
三、函数式设计方案
此方案也是Owin和ASP.NET Core采用的方案，如果站在面向对象的角度，第一个方案是非常清晰的，管道最终通过枚举所有Middleware来依次处理请求。
站在函数式的角度来看，Middleware可以用Func来表示，再来看看这张图：
![](http://jbcdn2.b0.upaiyun.com/2016/06/e9ff34727cd33a6d0354badac0e79b3c.png)
一个Middleware的逻辑可以用Func, Func>来表示，整个Middleware的逻辑可以用下面的代码描述：

C#
```
public Func, Func> Process()
        {
            Func, Func> middleware = next =>
            {
                Func process = context =>
                {
                    /*process request*/
                  
                    next(context);
                    /*process response*/
                    return context;
                };
                return process;
            };
            return middleware;
        }
```
这一过程是理解函数式方案的关键，所有Middleware可以聚合为一个Func,为了易于阅读，我们可以定义一个委托：

C#
```
public delegate Context RequestDelegate(Context context);
```
给定初始RequestDelegate，聚合所有Middleware：

C#
```
public IApplication Build()
        {
            RequestDelegate request = context => context;
            _middlewares.Reverse();
            foreach (var middleware in _middlewares)
            {
                request = middleware(request);
            }
            return new Application(request);
        }
```
自定义一个函数式Middleware：

C#
```
public class DefaultMiddleware:IMiddleware
    {
        public Func Request()
        {
            Func request = next =>
            {
                return context =>
                {
                    context.Request.Process("default request", "processed by defaultMiddleware");
                    next(context);
                    context.Response.Process("default response", "processed by defaultMiddleware");
                    return context;
                };
            };
            return request;
        }
    }
```
所有代码提供下载：[https://git.oschina.net/richieyangs/Pipeline.Middleware.git](https://git.oschina.net/richieyangs/Pipeline.Middleware.git)
