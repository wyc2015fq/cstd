# Asp.net WebPages框架运行原理浅析{转} - weixin_33985507的博客 - CSDN博客
2014年04月02日 08:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
在Asp.net4和4.5中，新增了WebPages Framework，编写页面代码使用了新的Razor语法，代码更加的简洁和符合Web标准，编写方式更接近于PHP和以前的Asp，和使用WebForms这种模仿Windows Form编程方式有了很大不同，不再有大量控件和控件生成的大量不够灵活的代码，但是同样可以使用Asp.net提供的大量类库和功能，可以说WebPages框架融合了Asp、PHP和Asp.net的全部优点，又可使用C#和VB编程语言。一看到WebPages框架，我就马上有了深入学习的兴趣，因为它和WebForms相比立刻就会让有完美主义情结的程序员们倾心。
但WebPages框架却并没有绑定Razor语法，它可以使用第三方的视图引擎。WebPages和Razor也并没有和Asp.net MVC具有必然的联系。在VS2012中默认的网站模板里面多了”Asp.net网站(Razor v2)“，可以根据Razor语法创建WebPage。下图说明了WebPages框架在Asp.net中的位置。
![](https://images0.cnblogs.com/i/36866/201404/020805031568766.jpg)
**WebPages网站简介**
WebPages网站包含多个cshtml或vbhtml页面，这些页面中使用Razor模板语法，整个网站的文件都在一个文件夹中，bin目录中有各种要用到的dll，没有解决方案文件，解决方案文件在另外一个和网站同时创建的项目中，其中有packages目录以管理WebPages网站需要用到的包。一个普通的cshtml页代码如下：
```
@{
    var db = Database.Open("StarterSite");
    var users = db.Query("Select * From UserProfile");
    var grid = new WebGrid(users);
}
<!DOCTYPE html>
<html>
    <head>
        <title></title>
    </head>
    <body>
        @grid.GetHtml()
    </body>
</html>
```
从中可以看到，这种编写方式和PHP、Asp很相似，但WebPages身后却是庞大的Asp.net类库。
**WebPages框架相关配置**
在WebPages网站的web.config中并没有什么特殊配置，在.net framework 4.0中的web.config中相关的配置如下：
```
<compilation>
    <assemblies>
        <remove assembly="System.Web.WebPages.Deployment, Version=2.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35" />
        <add assembly="System.Web.WebPages.Deployment, Version=2.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35" />
    <add assembly="System.Web.WebPages.Deployment, Version=1.0.0.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35" />
     </assemblies>
</compilation>
<httpHandlers>
<add path="*.cshtm" verb="*" type="System.Web.HttpForbiddenHandler" validate="True"/>
<add path="*.cshtml" verb="*" type="System.Web.HttpForbiddenHandler" validate="True"/>
<add path="*.vbhtm" verb="*" type="System.Web.HttpForbiddenHandler" validate="True"/>
<add path="*.vbhtml" verb="*" type="System.Web.HttpForbiddenHandler" validate="True"/>
</httpHandlers>
```
其中没有相关buildProviders的配置也没有相关httpModules的配置，httpHandlers的配置还将其映射到了HttpForbiddenHandler禁止访问。在IIS或IIS Express中的配置中也只有Asp.net4.0的ISAPI的配置而没有相关的httpModule。而WebForms在web.config中配置有相应的httpHandler和buildProvider。那么WebPages框架是如何运行的，其和WebForms在哪里产生了不同，WebPages框架是如何在Asp.net框架下实现的，这就需要进入WebPages框架进行探索。
**WebPages框架自动运行过程**
从web.config中的System.Web.WebPages.Deployment程序集开始，这个程序集dll有一个Asp.net4.0新增的特性PreApplicationStartMethodAttribute，这个特性配置了一个静态方法以在程序启动之前自动执行，特性如下：[assembly:[PreApplicationStartMethod](http://www.aisto.com/roeder/dotnet/Default.aspx?Target=code://System.Web:4.0.0.0:b03f5f7f11d50a3a/System.Web.PreApplicationStartMethodAttribute/.ctor(System.Type,String))(typeof([PreApplicationStartCode](http://www.aisto.com/roeder/dotnet/Default.aspx?Target=code://System.Web.WebPages.Deployment:2.0.0.0:31bf3856ad364e35/System.Web.WebPages.Deployment.PreApplicationStartCode)), "Start")]，查看PreApplicationStartCode类的Start方法，其调用了StartCore方法，StartCore又调用了LoadWebPages方法，其中实现的功能主要是获取所有和WebPages框架相关的dll并得到这些dll上配置的PreApplicationStartMethodAttribute特性对应的启动方法并全部执行，具有这个特性的dll有System.Web.WebPages、System.Web.WebPages.Razor和WebMatrix.WebData，我们主要关注前两个。
```
private static void LoadWebPages(Version version)
{
    IEnumerable<Assembly> assemblies = Enumerable.Select<AssemblyName, Assembly>(AssemblyUtils.GetAssembliesForVersion(version), new Func<AssemblyName, Assembly>(null, (IntPtr) LoadAssembly));
    foreach (Assembly assembly in assemblies)
    {
        BuildManager.AddReferencedAssembly(assembly);
    }
    foreach (MethodInfo info in GetPreStartInitMethodsFromAssemblyCollection(assemblies))
    {
        info.Invoke(null, null);
    }
}
```
在System.Web.WebPages.Razor程序集上的启动方法代码如下：
```
public static class PreApplicationStartCode
{
    // Fields
    private static bool _startWasCalled;
    // Methods
    public static void Start()
    {
        if (!_startWasCalled)
        {
            _startWasCalled = true;
            BuildProvider.RegisterBuildProvider(".cshtml", typeof(RazorBuildProvider));
            BuildProvider.RegisterBuildProvider(".vbhtml", typeof(RazorBuildProvider));
        }
    }
}
```
其注册了cshtml和vbhtml文件对应的BuildProvider为RazorBuildProvider，即编译Razor语法文件的提供程序。
在System.Web.WebPages程序集上的启动方法代码如下，这些启动类的名字和方法是一样的
```
public static class PreApplicationStartCode
{
    // Fields
    private static bool _startWasCalled;
    // Methods
    public static void Start()
    {
        if (!_startWasCalled)
        {
            _startWasCalled = true;
            WebPageHttpHandler.RegisterExtension("cshtml");
            WebPageHttpHandler.RegisterExtension("vbhtml");
            PageParser.EnableLongStringsAsResources = false;
　　　　　　　　//此行代码注册了WebPageHttpModule            
　　　　　　　**DynamicModuleUtility.RegisterModule(typeof(WebPageHttpModule));**
            ScopeStorage.CurrentProvider = new AspNetRequestScopeStorageProvider();
        }
    }
}
```
其中最重要的功能就是自动注册了一个HttpModule,到此我们就可以知道WebPages页面的编译和处理已经有了着落了。接着查看WebPageHttpModule的代码，这个httpmodule注册处理了HttpApplication的PostResolveRequestCache,BeginRequest和EndRequest事件，这些代码会在用户请求cshtml和vbhtml页面时触发执行，在这个过程中WebPageHttpModule还会在WebPages网站首次启动的时候调用System.Web.WebPages.ApplicationStartPage.ExecuteStartPage方法，在PostResolveRequestCache事件处理代码中调用了WebPageRoute的方法，其中创建了处理页面的WebPageHttpHandler类。
```
internal static void OnApplicationPostResolveRequestCache(object sender, EventArgs e)
{
    HttpContextBase context = new HttpContextWrapper(((HttpApplication) sender).Context);
    new WebPageRoute().DoPostResolveRequestCache(context);
}
```
```
internal void DoPostResolveRequestCache(HttpContextBase context)
{
    if (!this.IsExplicitlyDisabled)
    {
        string pathValue = context.Request.AppRelativeCurrentExecutionFilePath.Substring(2) + context.Request.PathInfo;
        ReadOnlyCollection<string> registeredExtensions = WebPageHttpHandler.GetRegisteredExtensions();
        WebPageMatch match = MatchRequest(pathValue, registeredExtensions, this.VirtualPathFactory, context, DisplayModeProvider.Instance);
        if (match != null)
        {
            context.Items[typeof(WebPageMatch)] = match;
            string path = "~/" + match.MatchedPath;
　　　　　　　　//是否在WebPages网站的web.config中明确配置了webpages:Enabled
            if (!WebPagesDeployment.IsExplicitlyDisabled(path))
            {
                //创建WebPageHttpHandler
                IHttpHandler handler = WebPageHttpHandler.CreateFromVirtualPath(path);
                if (handler != null)
                {
                    SessionStateUtil.SetUpSessionState(context, handler);
　　　　　　　　　　　　//替换web.config中配置的HttpForbiddenHandler
                    context.RemapHandler(handler);
                }
            }
        }
        else
        {
            string extension = PathUtil.GetExtension(pathValue);
            foreach (string str4 in registeredExtensions)
            {
                if (string.Equals("." + str4, extension, StringComparison.OrdinalIgnoreCase))
                {
                    throw new HttpException(0x194, null);
                }
            }
        }
    }
}
```
WebPageRoute还实现了WebPages网站页面地址更加友好的功能，如不必带cshtml和vbhtml后缀即可访问相应页面，可采用filename/category/id之类的地址访问，通过添加xx.Mobile.cshtml即可自动实现切换到移动页的功能等。到此HttpHandler创建完毕之后，就开始执行页面，WebPages页面的基类是System.Web.WebPages.WebPage。
如果我们在WebPages网站的web.config里面配置了webpages:Enabled为false，那么再次访问cshtml或vbhtml时发现处理它们的是HttpForbiddenHandler。
```
<appSettings>
    <add key="webpages:Enabled" value="false"/>
</appSettings>
```
```
<!DOCTYPE html>
<html>
    <head>
        <title>无法提供此类型的页。</title>
        <meta name="viewport" content="width=device-width" />
        <style>
         body {font-family:"Verdana";font-weight:normal;font-size: .7em;color:black;} 
         p {font-family:"Verdana";font-weight:normal;color:black;margin-top: -5px}
         b {font-family:"Verdana";font-weight:bold;color:black;margin-top: -5px}
         H1 { font-family:"Verdana";font-weight:normal;font-size:18pt;color:red }
         H2 { font-family:"Verdana";font-weight:normal;font-size:14pt;color:maroon }
         pre {font-family:"Consolas","Lucida Console",Monospace;font-size:11pt;margin:0;padding:0.5em;line-height:14pt}
         .marker {font-weight: bold; color: black;text-decoration: none;}
         .version {color: gray;}
         .error {margin-bottom: 10px;}
         .expandable { text-decoration:underline; font-weight:bold; color:navy; cursor:hand; }
         @media screen and (max-width: 639px) {
          pre { width: 440px; overflow: auto; white-space: pre-wrap; word-wrap: break-word; }
         }
         @media screen and (max-width: 479px) {
          pre { width: 280px; }
         }
        </style>
    </head>
    <body bgcolor="white">
            <span><H1>“/”应用程序中的服务器错误。<hr width=100% size=1 color=silver></H1>
            <h2> <i>无法提供此类型的页。</i> </h2></span>
            <font face="Arial, Helvetica, Geneva, SunSans-Regular, sans-serif ">
            <b> 说明: </b>由于已明确禁止所请求的页类型，无法对该类型的页提供服务。扩展名“.cshtml”可能不正确。   请检查以下的 URL 并确保其拼写正确。
            <br><br>
            <b> 请求的 URL: </b>/s.cshtml<br><br>
            <hr width=100% size=1 color=silver>
            <b>版本信息:</b> Microsoft .NET Framework 版本:4.0.30319; ASP.NET 版本:4.0.30319.17929
            </font>
    </body>
</html>
<!-- 
[HttpException]: 禁止路径“/s.cshtml”。
   在 System.Web.HttpForbiddenHandler.ProcessRequest(HttpContext context)
   在 System.Web.HttpApplication.CallHandlerExecutionStep.System.Web.HttpApplication.IExecutionStep.Execute()
   在 System.Web.HttpApplication.ExecuteStep(IExecutionStep step, Boolean& completedSynchronously)
-->
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html>
<html>
    <head>
        <title>无法提供此类型的页。</title>
        <meta name="viewport" content="width=device-width" />
        <style>
         body {font-family:"Verdana";font-weight:normal;font-size: .7em;color:black;} 
         p {font-family:"Verdana";font-weight:normal;color:black;margin-top: -5px}
         b {font-family:"Verdana";font-weight:bold;color:black;margin-top: -5px}
         H1 { font-family:"Verdana";font-weight:normal;font-size:18pt;color:red }
         H2 { font-family:"Verdana";font-weight:normal;font-size:14pt;color:maroon }
         pre {font-family:"Consolas","Lucida Console",Monospace;font-size:11pt;margin:0;padding:0.5em;line-height:14pt}
         .marker {font-weight: bold; color: black;text-decoration: none;}
         .version {color: gray;}
         .error {margin-bottom: 10px;}
         .expandable { text-decoration:underline; font-weight:bold; color:navy; cursor:hand; }
         @media screen and (max-width: 639px) {
          pre { width: 440px; overflow: auto; white-space: pre-wrap; word-wrap: break-word; }
         }
         @media screen and (max-width: 479px) {
          pre { width: 280px; }
         }
        </style>
    </head>
    <body bgcolor="white">
            <span><H1>“/”应用程序中的服务器错误。<hr width=100% size=1 color=silver></H1>
            <h2> <i>无法提供此类型的页。</i> </h2></span>
            <font face="Arial, Helvetica, Geneva, SunSans-Regular, sans-serif ">
            <b> 说明: </b>由于已明确禁止所请求的页类型，无法对该类型的页提供服务。扩展名“.cshtml”可能不正确。   请检查以下的 URL 并确保其拼写正确。
            <br><br>
            <b> 请求的 URL: </b>/s.cshtml<br><br>
            <hr width=100% size=1 color=silver>
            <b>版本信息:</b> Microsoft .NET Framework 版本:4.0.30319; ASP.NET 版本:4.0.30319.17929
            </font>
    </body>
</html>
<!-- 
[HttpException]: 禁止路径“/s.cshtml”。
   在 System.Web.HttpForbiddenHandler.ProcessRequest(HttpContext context)
   在 System.Web.HttpApplication.CallHandlerExecutionStep.System.Web.HttpApplication.IExecutionStep.Execute()
   在 System.Web.HttpApplication.ExecuteStep(IExecutionStep step, Boolean& completedSynchronously)
-->
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
至此，我们对WebPages框架的相关运行原理有了一个大概的了解。WebPages框架通过Asp.net4.0提供的PreApplicationStartMethodAttribute特性实现了HttpModule和BuildProvider注册，而不是通常的web.config配置文件，本文介绍的过程并不是绝对的，因为这个特性是可以被Asp.net Runtime自动识别和运行的，例如只要你引用了System.Web.WebPages就会执行其上配置的注册WebPageHttpModule的方法，其这些程序集上的启动方法都有判断机制防止重复执行。
**和Asp.net MVC中的Razor视图引擎的关系**
Razor并没有和MVC紧密耦合，其可以脱离MVC，也可以脱离WebPage框架。System.Web.MVC程序集上也有PreApplicationStartMethodAttribute特性，其中分别直接调用了System.Web.WebPages和System.Web.WebPages.Razor中的启动类以注册WebPageHttpModule和RazorBuildProvider，但MVC之中又继承了System.Web.Razor中的WebPageRazorHost类实现了自己特有的MvcWebPageRazorHost，相当于增加了新的功能以和MVC配合。
正如MVC中有多种不同的视图引擎一样，WebPages框架也不一定要使用Razor引擎，我们可以实现自己的BuildProvider来定义WebPage语法和解析生成.Net程序集。
**结语**
个人认为WebPages框架在人们越来越注重Web标准和前端UI的情况下，抛弃了WebForms有些笨重和不透明的编程方式，具有极大的灵活性同时又可以利用Asp.net强大的类库，后端的框架模型并没有变，但却让Asp.net程序员有了和PHP、Asp类似的快速编程体验。受够了WebForms输出代码的臃肿的程序员们，赶快学习WebPages吧:)
