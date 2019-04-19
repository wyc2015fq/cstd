# ASP.NET Core管道深度剖析（1）：采用管道处理HTTP请求 - 文章 - 伯乐在线
原文出处： [蒋金楠](http://www.cnblogs.com/artech/p/asp-net-core-pipeline.html)
之所以称ASP.NET Core是一个Web开发平台，源于它具有一个极具扩展性的请求处理管道，我们可以通过这个管道的定制来满足各种场景下的HTTP处理需求。ASP. NET Core应用的很多特性，比如路由、认证、会话、缓存等，也同时定制消息处理管道来实现的。我们甚至可以通过管道定制在ASP.NET Core平台上创建我们自己的Web框架，实际上MVC和SingalR这两个重要的Web框架也是采用这样的方式创建的。
HTTP协议自身的特性决定了任何一个Web应用的工作方式都是监听、接收并处理HTTP请求，并在最终对请求予以响应，HTTP请求处理是管道式设计典型的应用场景。我们根据HTTP请求的处理流程定制出一个消息处理管道，让接收到的HTTP请求消息想水一样流入这个管道，组成这个管道的各个环节一次对它作相应的处理。
处理的结果同样转变成消息逆向流入这个管道进行处理，并最终转变成回复给客户端的HTTP响应。ASP.NET Core的消息处理管道从设计的角度来讲是非常简单的，但是从具体实现的角度则相对难以理解，为了让读者朋友们通过本章对此具有深刻的理解，我们从简单的部分讲起。
# 一、从Hello World说起
为了使读者朋友们能够以最直观的感受认识ASP.NET Core的消息处理管道，我们来创建一个最简单的Hello World程序。这是一个仅仅由两个类型构成的控制台程序，作为程序入口的Main方法定义在Program类中，Startup则作为初始化类型。这个程序被启动之后将会绑定到默认端口5000进行HTTP请求的监听，任何针对基地址 “http://localhost:5000/” 的请求后，该程序都将响应 “Hello World” 。

C#
```
class Program
 {
     static void Main(string[] args)
     {
           new WebHostBuilder()
             .UseKestrel()
             .UseStartup<Startup>
             .Build()
             .Start();
     }
 }
  
 
 public class Startup
 {
     public void Configure(IApplicationBuilder app)
     {
         app.Run(context=>context.Response.WriteAsync("Hello World");
     }
 }
```
这个程序涉及到一个重要的对象WebHost，它通过WebHostBuilder的Build方法创建。WebHost可以看成是Web应用的宿主，启动Web应用本质上就是启动它的宿主。当我们调用WebHost的Start方法启动应用的时候，用于监听、接收、处理和响应HTTP请求的消息处理管道随之被建立。
那么在这个过程中，通过调用UseStartup方法注册到WebHostBuilder上的初始化类型将用来对这个管道进行定制。总的来说，ASP.NET Core的请求处理管道由WebHost在启动的时候构建，WebHostBuilder则是后者的创建者，右图揭示了三者之间的关系。
![clip_image002](http://jbcdn2.b0.upaiyun.com/2016/05/e312497b5b0936191c4fee7feda33dbf.jpg)
# 二、管道的构成
HTTP请求处理流程始于对请求的监听与接收，终于对请求的响应，这两项工作由同一个对象来完成，我们称之为 “服务器（Server）” ，尽管ASP.NET Core的请求处理管道可以被自由地订制，但是该管道必须有一个服务器，服务器是整个管道的 “龙头” 。
在上面的这个Hello World应用中，在调用WebHostBuilder的Build方法创建一个WebHost之前，我们调用了它的一个扩展方法UseKestrel，这个方法的作用就是为后续构建的管道注册一个名为KestrelServer的服务器。
![7-2](http://jbcdn2.b0.upaiyun.com/2016/05/f8b76ca934d6816477d50d7433242f4a.png)
随着WebHost的Start方法的调用，按照具体需求进行定制的请求处理管道被构建出来，作为第一个节点的服务器会绑定到一个预设的端口（比如KestrelServer默认采用5000作为监听端口）开始监听来自客户端的HTTP请求。一旦请求抵达，服务器会接收请求并将其标准化后向管道后续的节点进行转发，我们将管道中位于服务器之后的请求处理节点成为“中间件（Middleware）”。
每个中间件都具有各自独立的功能，比如我们有专门实现路由功能的中间件，由专门实施用户认证的中间，所谓的对请求处理管道的定制体现在根据具体的需求选择对应的中间件组成最终处理请求的管道。左图揭示了由一个服务器和一组中间件构成的请求处理管道。
![clip_image006](http://jbcdn2.b0.upaiyun.com/2016/05/7ad91f396791dda31bf19b724487962d.jpg)
一个建立在ASP.NET Core之上的应用一般都是根据某个框架开发的，开发框架基本上是建立在某个特殊的中间件上。以ASP.NET Core MVC这个最著名的框架为例，它实际上是利用一个叫做 “路由” 的中间件实现了请求地址与Controller/Action之间的映射，并在此基础实现了激活Controller、执行Action以及呈现View等一系列的功能。所以应用程序可以视为某个中间件的一部分，如果一定要将它独立出来，整个请求处理管道将呈现出如右图所示的结构。
# 三、管道的定制
在演示的Hello World程序中，我们在调用WebHostBuilder的Build方法创建WebHost之前先调用了它的扩展方法UseStartup方法注册了一个类型为Startup的启动类型。从请求处理管道的角度来讲，注册的这个启动类型的目的在于对构建的管道进行定制，说得更加具体一点，我们利用这个类型为管道注册需要的中间件。
一般来说，被注册的启动类型必须具有一个类似于下面代码片断所示的Configure方法，这个方法可以是静态方法和也可以实例方法。这个方法的参数并没有严格的限制，但是第一个参数类型必须是IApplicationBuilder接口。

C#
```
public class Startup
 {
    public void Configure(IApplicationBuilder app);
 }
```
对中间件的注册就是实现在这样一个Configure方法之中。在演示的实例中，我们调用了IApplicationBuilder接口的扩展方法Run注册了一个中间件，它承载的请求处理逻辑很简单，即使直接响应一个“Hello World”字符串。在真实的项目中，我们会根据具体的应用场景在这样一个方法中利用ApplicationBuilder注册相应的中间件进而构建一个适合当前请求处理需求的管道。

C#
```
public class Startup
 {
     public void Configure(IApplicationBuilder app)
     {
        app.UseExceptionHandler("/Home/Error");
         app.UseStaticFiles();
        app.UseIdentity();           
  
         app.UseMvc();
     }
 }
```
比如在一个ASP.NET Core MVC应用中我们除了按照如上的方式调用扩展方法UseMvc注册了支撑MVC框架的中间件（实际上是一个实现路由的中间件）之外，我们还通过调用其它的扩展方法注册了相应的中间件实现了对静态文件的访问（UseStaticFiles）、错误页面的呈现（UseExceptionHandler）以及基于ASP.NET Identity Framework的认证（UseIdentity）。
