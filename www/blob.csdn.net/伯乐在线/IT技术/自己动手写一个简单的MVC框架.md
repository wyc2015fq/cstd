# 自己动手写一个简单的MVC框架 - 文章 - 伯乐在线
原文出处： [周旭龙（@周旭龙EdisonChou）](http://www.cnblogs.com/edisonchou/p/5256517.html)
# 一、ASP.NET MVC核心机制回顾
在ASP.NET MVC中，最核心的当属“**路由系统**”，而路由系统的核心则源于一个强大的**System.Web.Routing.dll**组件。
![](http://jbcdn2.b0.upaiyun.com/2016/03/a436c0dede5123d16dbb6db2960051ac.jpg)
在这个System.Web.Routing.dll中，有一个最重要的类叫做**UrlRoutingModule**，它是一个实现了**IHttpModule**接口的类，在请求处理管道中专门针对ASP.NET MVC请求进行处理。首先，我们要了解一下UrlRoutingModule是如何起作用的。
（1）IIS网站的配置可以分为两个块：全局 Web.config 和本站 Web.config。Asp.Net Routing属于全局性的，所以它配置在全局Web.Config 中，我们可以在如下路径中找到：“$WindowsMicrosoft.NETFramework版本号ConfigWeb.config“


```
<?xml version="1.0" encoding="utf-8"?>
 <!-- the root web configuration file -->
 <configuration>
     <system.web>
         <httpModules>
             <add name="UrlRoutingModule-4.0" type="System.Web.Routing.UrlRoutingModule" />
         </httpModules>
    </system.web>
 </configuration>
```
（2）通过在全局Web.Config中注册** System.Web.Routing.UrlRoutingModule**，IIS请求处理管道接到请求后，就会加载 UrlRoutingModule类型的**Init()**方法。
**PS :** 在UrlRoutingModule中为请求处理管道中的第七个事件PostResolveRequestCache注册了一个事件处理方法：OnApplicationPostResolveRequestCache。从这里可以看出：**ASP.NET MVC的入口在UrlRoutingModule，即订阅了HttpApplication的第7个管道事件PostResolveRequestCahce。换句话说，是在HtttpApplication的第7个管道事件处对请求进行了拦截。**
![](http://jbcdn2.b0.upaiyun.com/2016/03/111e320b1b1efa715af519507ffbf318.png)
现在我们将ASP.NET MVC的请求处理分为**两个重要阶段**来看看：
## ①在第七个事件中创建实现了IHttpHandler接口的MvcHandler
当请求到达UrlRoutingModule的时候，UrlRoutingModule取出请求中的Controller、Action等RouteData信息，与路由表中的所有规则进行匹配，若匹配，把请求交给IRouteHandler，即**MVCRouteHandler**。我们可以看下UrlRoutingModule的源码来看看，以下是几句核心的代码：

C#
```
public virtual void PostResolveRequestCache(HttpContextBase context)
{
    // 通过RouteCollection的静态方法GetRouteData获取到封装路由信息的RouteData实例
    RouteData routeData = this.RouteCollection.GetRouteData(context);
    if (routeData != null)
    {
        // 再从RouteData中获取MVCRouteHandler
        IRouteHandler routeHandler = routeData.RouteHandler;
        ......
        if (!(routeHandler is StopRoutingHandler))
        {
            ......
            // 调用 IRouteHandler.GetHttpHandler()，获取的IHttpHandler 类型实例，它是由 IRouteHandler.GetHttpHandler获取的，这个得去MVC的源码里看
            IHttpHandler httpHandler = routeHandler.GetHttpHandler(requestContext);
            ......
            // 合适条件下，把之前将获取的IHttpHandler 类型实例 映射到IIS HTTP处理管道中
            context.RemapHandler(httpHandler);
        }
    }
}
```
从源码片段中可以看出，最后将请求转移给了实现了IHttpHandler接口的处理程序进行后续的处理。在ASP.NET MVC的实现中，是将请求交给了MvcHandler这个类，通过执行其ProcessRequest方法来进行后续的处理。
![](http://jbcdn2.b0.upaiyun.com/2016/03/091944345cb025c70801038626ad4c73.png)
## ②在第十一个事件与第十二个事件之间调用MvcHandler的ProcessRequest()方法
（1）在WebForm中，此阶段会调用Page类对象的ProcessRequest()方法。在ASP.NET MVC中，会调用MvcHandler的ProcessRequest()方法，此方法会**激活具体请求的Controller类对象，触发Action方法，返回ActionResult实例**。
（2）如果ActionResult是非ViewResult，比如JsonResult, ContentResult，这些内容将直接被输送到Response响应流中，显示给客户端；如果是ViewResult，就会进入下一个渲染视图环节。
（3）在渲染视图环节，ViewEngine找到需要被渲染的视图，View被加载成WebViewPage类型，并渲染生成Html，最终返回Html。
![](http://jbcdn2.b0.upaiyun.com/2016/03/a366cc3a18e6b8e9e850feaa089b2384.jpg)
# 二、我的MVC框架核心部分介绍
## 2.1 解决方案概览
![](http://jbcdn2.b0.upaiyun.com/2016/03/b9b1994967447efad76b1b77ff2e1471.jpg)
在该解决方案中，一共有两个项目：
一个是App，它是一个由最小化的引用环境（只引用了System和System.Web，以及Mvc.Lib）搭建起来的一个Web应用项目，借助MVC核心类库（Mvc.Lib）实现了MVC模式。
一个是Lib，它是一个模拟ASP.NET MVC框架的最小化、轻量级的迷你MVC框架，其中Mvc文件夹模拟System.Web.Mvc，Routing文件夹模拟System.Web.Routing，而View则简单地借助NVelocity模板引擎提供View视图服务。
## 2.2 MVC核心类库
（1）Routing
![](http://jbcdn2.b0.upaiyun.com/2016/03/cdf55a74e546358709ab87bb780dd90e.jpg)
从第一部分我们可以知道，ASP.NET MVC的入口在于UrlRoutingModule，因此这里我们便模拟实现了一个UrlRoutingModule.

C#
```
/// 
    /// 解析请求中的路由数据，并分发请求到Handler
    /// 
    public class UrlRoutingModule : IHttpModule
    {
        public void Init(HttpApplication application)
        {
            // 注册ASP.NET请求处理管道的第七个事件
            application.PostResolveRequestCache += Application_PostResolveRequestCache;
        }
        // 假设请求 http://www.edisonchou.cn/home/index
        private void Application_PostResolveRequestCache(object sender, EventArgs e)
        {
            var application = sender as HttpApplication;
            var context = application.Context;
            // 根据全局路由表解析当前请求的路径
            var requestUrl = context.Request.AppRelativeCurrentExecutionFilePath.Substring(2);
            // 遍历全局路由表中的路由规则解析数据
            IDictionarystring, object> routeData;
            var route = RouteTable.MatchRoutes(requestUrl, out routeData);
            if (route == null)
            {
                // 404 Not Found
                throw new HttpException(404, "Not Found!");
            }
            // 获取处理请求的Handler处理程序
            if (!routeData.ContainsKey("controller"))
            {
                // 404 Not Found
                throw new HttpException(404, "Not Found!");
            }
            var handler = route.GetRouteHandler(routeData);
            // 为当前请求指定Handler处理程序
            context.RemapHandler(handler);
        }
        public void Dispose()
        {
            this.Dispose();
        }
    }
```
该UrlRoutingModule通过注册ASP.NET请求处理管道的第七个事件，来实现对URL地址进行路由规则的处理，并将最后生成的路由数据交给MvcHandler进行后续处理。这里我省略了ASP.NET MVC源码中MvcRouteHandler生成MvcHandler的步骤，直接丢给MvcHandler处理。
核心部分有两点，一是路由规则的匹配，二是为请求指定handler。
在路由规则的匹配中，通过设置路由数据键值对（Dictionary），并将设置好的路有数据传递给MvcHandler。具体的流程如下图所示，这里就不再展示源码，请自行下载DEMO查看：
![](http://jbcdn2.b0.upaiyun.com/2016/03/ca569878f405d101e8a053b2fc26ca34.jpg)
（2）Mvc
![](http://jbcdn2.b0.upaiyun.com/2016/03/b7f7c81d7d2ff8158b47e7a1aa25e6bf.jpg)
在此文件夹中，实现了三个核心的部分：
① 最核心的处理者 ： **MvcHandler**

C#
```
public class MvcHandler : IHttpHandler
    {
        private IDictionarystring, object> routeData;
        public MvcHandler(IDictionarystring, object> routeData)
        {
            this.routeData = routeData;
        }
        public void ProcessRequest(HttpContext context)
        {
            var controllerName = routeData["controller"].ToString();
            // 借助控制器工厂创建具体控制器实例
            IController controller = DefaultControllerFactory.CreateController(controllerName);
            // 确保有找到一个Controller处理请求
            if (controller == null)
            {
                // 404 Not Found!
                throw new HttpException(404, "Not Found");
            }
            // 封装请求
            var requestContext = new RequestContext { HttpContext = context, RouteData = routeData };
            // 开始执行
            var result = controller.Execute(requestContext);
            result.Execute(requestContext);
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
② 花样的返回类型 ： **ActionResult** 以及它的子类们
![](http://jbcdn2.b0.upaiyun.com/2016/03/c80496bf67dddc35da7d23c8d827aa57.jpg)
在以往的ASP.NET MVC开发中，我们在Action方法的编写中，总会看到它们的返回类型都是以ActionResult为基类的各种Result类型。

C#
```
/// <summary>
    /// Action统一的返回类型
    /// </summary>
    public abstract class ActionResult
    {
        public abstract void Execute(RequestContext context);
    }
```
因此，这里也实现了ActionResult这个抽象类，并以此为基础实现了ContentResult、JsonResult以及ViewResult。它们的区别就在于是不同的返回类型，因此有不同的处理。
这里以ContentResult 和 JsonResult 为例，来看看具体做了什么处理。
[ContentResult]

C#
```
public class ContentResult : ActionResult
    {
        private string content;
        private string contentType;
        public ContentResult(string content, string contentType)
        {
            this.content = content;
            this.contentType = contentType;
        }
        public override void Execute(RequestContext context)
        {
            context.HttpContext.Response.Write(content);
            context.HttpContext.Response.ContentType = contentType;
        }
    }
[JsonResult]
    public class JsonResult : ActionResult
    {
        private object paraObj;
        public JsonResult(object paraObj)
        {
            this.paraObj = paraObj;
        }
        public override void Execute(RequestContext context)
        {
            JavaScriptSerializer jss = new JavaScriptSerializer();
            var json = jss.Serialize(paraObj);
            context.HttpContext.Response.Write(json);
            context.HttpContext.Response.ContentType = "application/json";
        }
    }
```
相信有经验的读者一眼就看穿了，因此这里也就不再多说了。
③ 路由的扩展者 ： **RouteExtend**
在以往的ASP.NET MVC开发中，我们会在Global全局应用处理文件中为项目注册路由规则，但却不知道其实我们常用的MapRoute方法其实是一个扩展方法，它并不位于System.Web.Routing这个类库之中，而是位于System.Web.Mvc这个类库之中。
![](http://jbcdn2.b0.upaiyun.com/2016/03/cde60340b4a6e74bde29273f7fa70056.jpg)
因此，我们也在Mvc文件夹中实现了一个RouteExtend类，它为RouteTable类的Route集合实现了一个扩展方法：

C#
```
/// 
    /// Route 的扩展方法所在类
    /// 
    public static class RouteExtend
    {
        /// 
        /// 指定MvcHandler来处理
        /// 
        public static void MapRoute(this IList source, string urlTemplate, object defaults)
        {
            MapRoute(source, urlTemplate, defaults, routeData => new MvcHandler(routeData));
        }
        /// 
        /// 通过指定实现了IHttpHandler的处理程序来处理
        /// 
        public static void MapRoute(this IList source, string urlTemplate, object defaults, Funcstring, object>, IHttpHandler> handler)
        {
            source.Add(new Route(urlTemplate, defaults, handler));
        }
    }
```
可以看出，MvcHandler是在这里传入的（Mvc与Routing是单向依赖）。那么，为什么还要提供一个可传入自定义Handler的接口呢？因为，不同的路由规则有可能需要不同的实现IHttpHandler的处理程序来处理，也不一定就非得是MvcHandler。
（3）View
在ASP.NET MVC中提供了aspx与Razor等模板引擎，这里我偷了懒，直接借助了NVelocity模板引擎来实现。因此，这个文件夹中只有一个VelocityHelper类（我直接从网上搜索的），该类可以帮助我们找到指定的HTML并绑定Model实体。


C#
```
/// 
    /// NVelocity模板工具类 VelocityHelper
    /// 
    public class VelocityHelper
    {
        private VelocityEngine velocity = null;
        private IContext context = null;
        public object YZControl { get; private set; }
        /// 
        /// 构造函数
        /// 
        /// 模板文件夹路径
        public VelocityHelper(string templatDir)
        {
            Init(templatDir);
        }
        /// 
        /// 无参数构造函数
        /// 
        public VelocityHelper() { }
        /// 
        /// 初始话NVelocity模块
        /// 
        public void Init(string templatDir)
        {
            // 创建VelocityEngine实例对象
            velocity = new VelocityEngine();
            // 使用设置初始化VelocityEngine
            ExtendedProperties props = new ExtendedProperties();
            props.AddProperty(RuntimeConstants.RESOURCE_LOADER, "file");
            props.AddProperty(RuntimeConstants.FILE_RESOURCE_LOADER_PATH, HttpContext.Current.Server.MapPath(templatDir));
            //props.AddProperty(RuntimeConstants.FILE_RESOURCE_LOADER_PATH, Path.GetDirectoryName(HttpContext.Current.Request.PhysicalPath));
            props.AddProperty(RuntimeConstants.INPUT_ENCODING, "utf-8");
            props.AddProperty(RuntimeConstants.OUTPUT_ENCODING, "utf-8");
            // 模板的缓存设置
            props.AddProperty(RuntimeConstants.FILE_RESOURCE_LOADER_CACHE, true);              //是否缓存
            props.AddProperty("file.resource.loader.modificationCheckInterval", (Int64)30);    //缓存时间(秒)
            velocity.Init(props);
            // 为模板变量赋值
            context = new VelocityContext();
        }
        /// 
        /// 给模板变量赋值
        /// 
        /// 模板变量
        /// 模板变量值
        public void Put(string key, object value)
        {
            if (context == null)
            {
                context = new VelocityContext();
            }
            context.Put(key, value);
        }
        /// 
        /// 显示模板
        /// 
        /// 模板文件名
        public void Display(string templatFileName)
        {
            // 从文件中读取模板
            Template template = velocity.GetTemplate(templatFileName);
            // 合并模板
            StringWriter writer = new StringWriter();
            template.Merge(context, writer);
            // 输出
            HttpContext.Current.Response.Clear();
            HttpContext.Current.Response.Write(writer.ToString());
            HttpContext.Current.Response.Flush();
            HttpContext.Current.Response.End();
        }
        /// 
        /// 根据模板生成静态页面
        /// 
        /// 
        /// 
        public void CreateHtml(string templatFileName, string htmlpath)
        {
            // 从文件中读取模板
            Template template = velocity.GetTemplate(templatFileName);
            // 合并模板
            StringWriter writer = new StringWriter();
            template.Merge(context, writer);
            using (StreamWriter write2 = new StreamWriter(HttpContext.Current.Server.MapPath(htmlpath), false, Encoding.UTF8, 200))
            {
                write2.Write(writer);
                write2.Flush();
                write2.Close();
            }
        }
        /// 
        /// 根据模板生成静态页面
        /// 
        /// 
        /// 
        //public void CreateJS(string templatFileName, string htmlpath)
        //{
        //    //从文件中读取模板
        //    Template template = velocity.GetTemplate(templatFileName);
        //    //合并模板
        //    StringWriter writer = new StringWriter();
        //    template.Merge(context, writer);
        //    using (StreamWriter write2 = new StreamWriter(HttpContext.Current.Server.MapPath(htmlpath), false, Encoding.UTF8, 200))
        //    {
        //        write2.Write(YZControl.Strings.Html2Js(YZControl.Strings.ZipHtml(writer.ToString())));
        //        write2.Flush();
        //        write2.Close();
        //    }
        //}
    }
```
# 三、我的MVC框架应用实例
## 3.1 MVC 应用DEMO介绍
![](http://jbcdn2.b0.upaiyun.com/2016/03/6b7c4c8931b2d9255cb69f06fb4430e5.jpg)
这是一个ASP.NET 空Web应用项目搭建起来的MVC Web应用项目，它移除了自带的所有引用项目，仅仅保留了System和System.Web，做到了尽可能地“纯净”。通过引入Mvc.Lib核心类库，建立Controller、Model和View文件夹以及对应的类和HTML来实现MVC模式。
（1）引入Mvc.Lib核心类库之后，需要配置一下Web.config，使UrlRoutingModule能够正常工作：


```
<system.web>
    <compilation debug="true" targetFramework="4.5"/>
    <httpRuntime targetFramework="4.5"/>
    <!-- HttpModule配置（IIS6版本） -->
    <httpModules>
      <add name="UrlRoutingModule" type="Manulife.Web.Mvc.Lib.Routing.UrlRoutingModule"/>
    </httpModules>
  </system.web>
  <system.webServer>
    <!-- 配置不去校验是否是集成模式 -->
    <validation validateIntegratedModeConfiguration="false"/>
    <!-- HttpModule配置（IIS7及以上版本） -->
    <modules>
      <add name="UrlRoutingModule" type="Manulife.Web.Mvc.Lib.Routing.UrlRoutingModule"/>
    </modules>
  </system.webServer>
```
（2）新建Global全局处理配置，在Application_Start事件中为项目添加路由规则：

C#
```
public class Global : System.Web.HttpApplication
    {
        protected void Application_Start(object sender, EventArgs e)
        {
            // 注册路由规则1
            RouteTable.Routes.MapRoute(
                urlTemplate: "{controller}/{action}/{id}",
                defaults: new { controller = "Home", action = "Index" }
                );
            // 注册路由规则2
            RouteTable.Routes.MapRoute(
                urlTemplate: "{controller}/{action}",
                defaults: new { controller = "Home", action = "Index" }
                );
            // 注册路由规则3
            RouteTable.Routes.MapRoute(
                urlTemplate: "{controller}",
                defaults: new { controller = "Home", action = "Index" }
                );
        }
    }
```
（3）看看Controller是怎么写的？是不是很熟悉？

C#
```
public class HomeController : ControllerBase
    {
        public ActionResult Index(int id, string controller, string action)
        {
            return new ContentResult(string.Format("<h1>Controller : {0}, Action : {1}, Id : {2}</h1>", controller, action, id), "text/html");
        }
        public ActionResult View()
        {
            return new ViewResult(new { Id = 1, Name = "Edison Chou", Age = 27, Gender = true });
        }
    }
```
（4）看看View中的HTML呢？这里使用NVelocity模板引擎提供的语法，操作Model实体对象。

XHTML
```
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>Index - View</title>
    <meta charset="utf-8" />
</head>
<body>
    <h1>User Name : $model.Name</h1>
    <h1>User Age : $model.Age</h1>
</body>
</html>
```
## 3.2 MVC 应用DEMO演示
（1）默认路由 : home/index -> ContentResult
![](http://jbcdn2.b0.upaiyun.com/2016/03/0026be1dcd49ac488a4a74c3ea105b36.jpg)
（2）请求JsonResult
![](http://jbcdn2.b0.upaiyun.com/2016/03/18ef12d3663cf088e9eeebdfac9de0a5.jpg)
（3）请求ViewResult
![](http://jbcdn2.b0.upaiyun.com/2016/03/62f74612153ec08c21f04066d34f5cac.jpg)
# 附件下载
**Manulife.Web.Mvc** : [点我下载](http://pan.baidu.com/s/1pKjiYL1)
