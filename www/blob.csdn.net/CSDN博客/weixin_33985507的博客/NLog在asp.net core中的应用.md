# NLog在asp.net core中的应用 - weixin_33985507的博客 - CSDN博客
2017年02月02日 20:34:07[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
Asp.net core中，自带的Log是在当selfhost运行时，在控制台中输出，不便于查阅，如果用一个log架框，把日志持久化，便于查询．
NLog是一个免费的日志记录框架，专门为.net平台下的框架提供日志功能，本文主要说明asp.net core下怎么使用NLog。
首先用Nuget安装NLog.Extensions.Logging和NLog.Web.AspNetCore两个类库。
修改project.json，在publishOptions中添加”nlog.config节点”
"publishOptions": {
"include": [
"wwwroot",
"**/*.cshtml",
"appsettings.json",
"web.config",
"nlog.config"
  ]
}
在StartUp.cs中添加
public void ConfigureServices(IServiceCollection services)
      {
          // Add framework services.
          services.AddMvc();
 
          //为NLog.web注入HttpContextAccessor
          services.AddSingleton<IHttpContextAccessor, HttpContextAccessor>();
……
}
public void Configure(IApplicationBuilder app, IHostingEnvironment env, ILoggerFactory loggerFactory)
       {
           //添加NLog到.net core框架中
           loggerFactory.AddNLog();
           //添加NLog的中间件
           app.AddNLogWeb();
           //指定NLog的配置文件
           env.ConfigureNLog("nlog.config");
……
}
HomeController.cs中是自定义日志
static Logger Logger = LogManager.GetCurrentClassLogger();
public IActionResult Index()
{
    Logger.Info("普通信息日志");
    Logger.Debug("调试日志");
    Logger.Error("错误日志");
    Logger.Fatal("异常日志");
    Logger.Warn("警告日志");
    Logger.Trace("跟踪日志");
    Logger.Log(NLog.LogLevel.Warn, "Log日志");
    try
    {
        int i = 0;
        var a = 10 / i;
    }
    catch (Exception exc)
    {
        //异常日志
        Logger.Fatal(exc, exc.Message);
    }
    return View();
}
NLog.config，有两种日志，记录在C:\temp下，一种是全部日志，一种是自己通过NLog函数记录的日志。如果日志库异常，会产生在c:\temp\internal-nlog.txt下，
<?xml version="1.0" encoding="utf-8" ?>
<nlog xmlns="http://www.nlog-project.org/schemas/NLog.xsd"
      xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
      autoReload="true"
      internalLogLevel="Warn"
      internalLogFile="c:\temp\internal-nlog.txt">
  <!--  ASP.NET Core 使用 -->
  <extensions>
    <add assembly="NLog.Web.AspNetCore"/>
  </extensions> 
  <targets>
    <!-- 全部日志文件 -->
    <target xsi:type="File" name="allfile" fileName="c:\temp\nlog-all-${shortdate}.log"
                layout="${longdate}|${event-properties:item=EventId.Id}|${logger}|${uppercase:${level}}|${message} ${exception}" />
    <!-- 自定义记录的日志文件 -->
    <target xsi:type="File" name="ownFile-web" fileName="c:\temp\nlog-own-${shortdate}.log"
            layout="${longdate}|${event-properties:item=EventId.Id}|${logger}|${uppercase:${level}}|  ${message} ${exception}|url: ${aspnet-request-url}|action: ${aspnet-mvc-action}" />
    <target xsi:type="Null" name="blackhole" />
  </targets>
  <rules>
    <!--所括asp.net core下的全部日志-->
    <logger name="*" minlevel="Trace" writeTo="allfile" />
    <!--仅自定义记录的日志文件-->
    <logger name="Microsoft.*" minlevel="Trace" writeTo="blackhole" final="true" />
    <logger name="*" minlevel="Trace" writeTo="ownFile-web" />
  </rules>
</nlog>
Layout中$后的数据就是要写日志的内容，关于这些数据可参考[https://github.com/nlog/nlog/wiki/Layout-Renderers](https://github.com/nlog/nlog/wiki/Layout-Renderers)，可以根据自己的需要，选择要保存的日志数据，如下图是web中的日志数据：
![wKiom1iTJnagAKoAAAC_kDHd2Dw729.png-wh_50](http://s4.51cto.com/wyfs02/M02/8D/3F/wKiom1iTJnagAKoAAAC_kDHd2Dw729.png-wh_500x0-wm_3-wmp_4-s_3044115077.png)
如果想把日志保存到数据库中，可以把NLog.config修改如下：
<?xml version="1.0" encoding="utf-8" ?>
<nlog xmlns="http://www.nlog-project.org/schemas/NLog.xsd"
      xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
      autoReload="true"
      internalLogLevel="Warn"
      internalLogFile="c:\temp\internal-nlog.txt">
 
  <!-- ASP.NET Core 使用  -->
  <extensions>
    <add assembly="NLog.Web.AspNetCore"/>
  </extensions>
 
  <!-- 注意配置连接字符串 -->
  <targets>
    <target name="database" xsi:type="Database" connectionString="server=.;database=nglogdb;uid=SQL用户名;pwd=SQL密码;">
      <commandText>
        INSERT INTO [dbo].[NLog] (
        [MachineName],
        [SiteName],
        [Logged],
        [Level],
        [UserName],
        [Message],
        [Logger],
        [Properties],
        [Host],
        [Controller],
        [Action],
        [Url],
        [CallSite],
        [Exception]
        ) VALUES (
        @machineName,
        @siteName,
        @logged,
        @level,
        @userName,
        @message,
        @logger,
        @properties,
        @host,
        @controller,
        @action,
        @url,
        @callSite,
        @exception
        );
      </commandText>
      <parameter name="@machineName"    layout="${machinename}" />
      <parameter name="@siteName"       layout="${iis-site-name}" />
      <parameter name="@logged"         layout="${date}" />
      <parameter name="@level"          layout="${level}" />
      <parameter name="@username"       layout="${aspnet-user-identity}" />
      <parameter name="@message"        layout="${message}" />
      <parameter name="@logger"         layout="${logger}" />
      <parameter name="@properties"     layout="${all-event-properties:separator=|}" />
      <parameter name="@host"           layout="${aspnet-request-host}" />
      <parameter name="@controller"     layout="${aspnet-MVC-Controller}" />
      <parameter name="@action"         layout="${aspnet-MVC-Action}" />
      <parameter name="@url"            layout="${aspnet-request-url}" />
      <parameter name="@callSite"       layout="${callsite}" />
      <parameter name="@exception"      layout="${exception:tostring}" />
    </target>
  </targets>
  <rules>
    <logger name="*" minlevel="Trace" writeTo="database"/>
  </rules>
</nlog>
数据库数据如下：
[](http://s5.51cto.com/wyfs02/M01/8D/3C/wKioL1iTJwTwrxUJAAC_kDHd2Dw855.png-wh_500x0-wm_3-wmp_4-s_1004401948.png)![wKiom1iTJxWSduw_AAXRGAbx4Ac008.png-wh_50](http://s5.51cto.com/wyfs02/M02/8D/3F/wKiom1iTJxWSduw_AAXRGAbx4Ac008.png-wh_500x0-wm_3-wmp_4-s_862586251.png)
