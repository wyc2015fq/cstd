# ASP.NET web API项目添加日志文件输出 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年07月24日 18:32:14[boonya](https://me.csdn.net/boonya)阅读数：768








ASP.NET web API项目添加日志输出与C#日志输出存在一点小小的差异，C#中只需要放配置文件就可以了，而ASP.NET log需要配置并在启动文件中加入启动读取配置文件和相关的日志过滤器。在C#和ASP.NET中使用的日志包是log4net。

### C# 日志配置

只需要编辑一个log4.config文件到bin的对应版本下就可以了。

```
<?xml version="1.0"?>
<configuration>
  <log4net>
    <root>
      <level value="DEBUG" />
      
    </root>
    <logger name="SQ">
      <level value="DEBUG"/>
        <appender-ref ref="RollingFileAppender" />
    </logger>
      <logger name="ErrLog">
          <level value="DEBUG"/>
          <appender-ref ref="LogForError" />
      </logger>
    <appender name="LogFileAppender" type="log4net.Appender.FileAppender" >
      <param name="File" value="LogFile/log-file.txt" />
      <param name="AppendToFile" value="true" />
      <layout type="log4net.Layout.PatternLayout">
        <param name="ConversionPattern" value="%d [%t] %-5p %c [%x]  - %m%n" />
      </layout>
    </appender>
    <appender name="ConsoleAppender" type="log4net.Appender.ConsoleAppender" >
      <layout type="log4net.Layout.PatternLayout">
        <param name="ConversionPattern" value="%d [%t] %-5p %c [%x] - %m%n" />
      </layout>
    </appender>
    <appender name="RollingFileAppender" type="log4net.Appender.RollingFileAppender">
      <!--日志文件名-->
      <file value="LogFile/log-file.txt"/>
      <!--是否在文件中追加-->
      <appendToFile value="true"/>
      <!--按照文件的大小进行变换日志文件-->
      <rollingStyle value="Size"/>
      <!--最大变换数量-->
      <maxSizeRollBackups value="100"/>
      <!--最大文件大小-->
      <maximumFileSize value="10MB"/>
      <!--日志文件名是否为静态-->
      <staticLogFileName value="false"/>
      <layout type="log4net.Layout.PatternLayout">
        <conversionPattern value="%d [%t] %-5p %c [%x]  - %m%n" />
      </layout>
    </appender>
      <appender name="LogForError" type="log4net.Appender.RollingFileAppender">
          <!--日志文件名-->
          <file value="Error_log/"/>
          <datePattern value=""Error_"yyyyMMdd".log""/>
          <!--是否在文件中追加-->
          <appendToFile value="true"/>
          <!--按照文件的大小进行变换日志文件-->
          <rollingStyle value="Composite" />
          <!--最大变换数量-->
          <maxSizeRollBackups value="100"/>
          <!--最大文件大小-->
          <maximumFileSize value="10MB"/>
          <!--日志文件名是否为静态-->
          <staticLogFileName value="false"/>
          <layout type="log4net.Layout.PatternLayout">
              <conversionPattern value="%d [%t] %-5p %c [%x]  - %m%n" />
          </layout>
      </appender>

  </log4net>
</configuration>
```

### ASP.NET 日志相关工具类

在项目中新建一个Logs文件夹，然后添加如下几个类到里面:

LoggerHelper.cs

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace RTVSApiFD.Logs
{
    /// <summary>
    /// 日志帮助类
    /// </summary>
    public class LoggerHelper
    {
        private static readonly log4net.ILog LogInfo = log4net.LogManager.GetLogger("LogInfo");

        private static readonly log4net.ILog LogError = log4net.LogManager.GetLogger("LogError");

        private static readonly log4net.ILog LogMonitor = log4net.LogManager.GetLogger("LogMonitor");

        /// <summary>
        /// 记录Error日志
        /// </summary>
        /// <param name="errorMsg"></param>
        /// <param name="ex"></param>
        public static void Error(string errorMsg, Exception ex = null)
        {
            if (ex != null)
            {
                LogError.Error(errorMsg, ex);
            }
            else
            {
                LogError.Error(errorMsg);
            }
        }

        /// <summary>
        /// 记录Info日志
        /// </summary>
        /// <param name="msg"></param>
        /// <param name="ex"></param>
        public static void Info(string msg, Exception ex = null)
        {
            if (ex != null)
            {
                LogInfo.Info(msg, ex);
            }
            else
            {
                LogInfo.Info(msg);
            }
        }

        /// <summary>
        /// 记录Monitor日志
        /// </summary>
        /// <param name="msg"></param>
        public static void Monitor(string msg)
        {
            LogMonitor.Info(msg);
        }
    }
}
```

MonitorLog.cs

```
using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Diagnostics;
using System.Linq;
using System.Web;

namespace RTVSApiFD.Logs
{
    /// <summary>
    /// 监控日志对象
    /// </summary>
    public class MonitorLog
    {
        /// <summary>
        /// 构造函数
        /// </summary>
        public MonitorLog()
        {
            this.Watch = new Stopwatch();
            this.Watch.Start();
        }

        /// <summary>
        /// 监控类型
        /// </summary>
        public enum MonitorType
        {
            /// <summary>
            /// Action
            /// </summary>
            Action = 1,

            /// <summary>
            /// 视图
            /// </summary>
            View = 2
        }

        /// <summary>
        /// 
        /// </summary>
        public string ControllerName { get; set; }

        /// <summary>
        /// 
        /// </summary>
        public string ActionName { get; set; }

        /// <summary>
        /// 
        /// </summary>
        public Stopwatch Watch { get; set; }

        /// <summary>
        /// 
        /// </summary>
        public DateTime ExecuteStartTime { get; set; }

        /// <summary>
        /// 
        /// </summary>
        public DateTime ExecuteEndTime { get; set; }

        /// <summary>
        /// Form 表单数据
        /// </summary>
        public NameValueCollection FormCollections { get; set; }

        /// <summary>
        /// URL 参数
        /// </summary>
        public NameValueCollection QueryCollections { get; set; }

        /// <summary>
        /// 文本流
        /// </summary>
        public string Raw { get; set; }

        /// <summary>
        /// 获取监控指标日志
        /// </summary>
        /// <param name="mtype"></param>
        /// <returns></returns>
        public string GetLogInfo(MonitorType mtype = MonitorType.Action)
        {
            this.Watch.Stop();
            string actionView = "Action执行时间监控：";
            string action = "Action";
            if (mtype == MonitorType.View)
            {
                actionView = "View视图生成时间监控：";
                action = "View";
            }
            string msgContent = string.Format(@"{0}ControllerName：{1}Controller {2}Name:{3} 开始时间：{4}  结束时间：{5} 总 时 间：{6}秒",
                actionView,
                this.ControllerName,
                action,
                this.ActionName,
                this.ExecuteStartTime,
                this.ExecuteEndTime,
                this.Watch.ElapsedMilliseconds);

            if (!string.IsNullOrEmpty(this.Raw))
            {
                msgContent += @"
        Raw：" + this.Raw;
            }
            else if (this.FormCollections != null)
            {
                msgContent += @"
        Form：" + this.GetCollections(this.FormCollections);
            }
            else if (this.QueryCollections != null)
            {
                msgContent += @"
        Query：" + this.GetCollections(this.QueryCollections);
            }

            return msgContent;
        }

        /// <summary>
        /// 获取Post 或Get 参数
        /// </summary>
        /// <param name="collections"></param>
        /// <returns></returns>
        public string GetCollections(NameValueCollection collections)
        {
            string parameters = string.Empty;
            if (collections == null || collections.Count == 0)
            {
                return parameters;
            }
            parameters = collections.Keys.Cast<string>()
                .Aggregate(parameters, (current, key) => current + string.Format("{0}={1}&", key, collections[key]));
            if (!string.IsNullOrWhiteSpace(parameters) && parameters.EndsWith("&"))
            {
                parameters = parameters.Substring(0, parameters.Length - 1);
            }
            return parameters;
        }
    }
   }
```

TrackerFilter.cs

```
using System;
using System.Globalization;
using System.Web.Mvc;

namespace RTVSApiFD.Logs
{
    
    /// <summary>
    /// 跟踪过滤器
    /// </summary>
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Method, AllowMultiple = false)]
    public class TrackerFilter : System.Web.Mvc.ActionFilterAttribute,System.Web.Mvc.IActionFilter,System.Web.Mvc.IResultFilter,System.Web.Mvc.IExceptionFilter
    {
        private readonly string key = "_thisOnActionMonitorLog_";

        #region Action时间监控
        /// <summary>
        /// OnActionExecuting
        /// </summary>
        /// <param name="filterContext"></param>
        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            MonitorLog monLog = new MonitorLog();
            monLog.ExecuteStartTime = Convert.ToDateTime(DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss.ffff", DateTimeFormatInfo.InvariantInfo));
            monLog.ControllerName = filterContext.RouteData.Values["controller"] as string;
            monLog.ActionName = filterContext.RouteData.Values["action"] as string;
            filterContext.Controller.ViewData[this.key] = monLog;
        }

        /// <summary>
        /// OnActionExecuted
        /// </summary>
        /// <param name="filterContext"></param>
        public override void OnActionExecuted(ActionExecutedContext filterContext)
        {
            MonitorLog monLog = filterContext.Controller.ViewData[this.key] as MonitorLog;
            monLog.ExecuteEndTime = DateTime.Now;
            monLog.FormCollections = filterContext.HttpContext.Request.Form;//form表单提交的数据
            monLog.QueryCollections = filterContext.HttpContext.Request.QueryString;//Url 参数
            LoggerHelper.Monitor(monLog.GetLogInfo());
        }
        #endregion

        #region View 视图生成时间监控
        /// <summary>
        /// OnResultExecuting
        /// </summary>
        /// <param name="filterContext"></param>
        public override void OnResultExecuting(ResultExecutingContext filterContext)
        {
            MonitorLog monLog = filterContext.Controller.ViewData[this.key] as MonitorLog;
            monLog.ExecuteStartTime = DateTime.Now;
        }

        /// <summary>
        /// OnResultExecuted
        /// </summary>
        /// <param name="filterContext"></param>
        public override void OnResultExecuted(ResultExecutedContext filterContext)
        {
            MonitorLog monLog = filterContext.Controller.ViewData[this.key] as MonitorLog;
            monLog.ExecuteEndTime = DateTime.Now;
            LoggerHelper.Monitor(monLog.GetLogInfo(MonitorLog.MonitorType.View));
            filterContext.Controller.ViewData.Remove(this.key);
        }
        #endregion

        #region 错误日志
        /// <summary>
        /// OnException
        /// </summary>
        /// <param name="filterContext"></param>
        public void OnException(ExceptionContext filterContext)
        {
            if (!filterContext.ExceptionHandled)
            {
                string controllerName = string.Format("{0}Controller", filterContext.RouteData.Values["controller"] as string);
                string actionName = filterContext.RouteData.Values["action"] as string;
                string errorMsg = string.Format("在执行 controller[{0}] 的 action[{1}] 时产生异常", controllerName, actionName);
                LoggerHelper.Error(errorMsg, filterContext.Exception);
            }
        }

        /// <summary>
        /// OnAuthorization
        /// </summary>
        /// <param name="filterContext"></param>
        public void OnAuthorization(AuthorizationContext filterContext)
        {
            ////这个方法是在Action执行之前调用
            //var user = filterContext.HttpContext.Session["userName"];
            //if (user == null)
            //{
            //    //filterConetext.HttpContext.Response.Redirect("/Login/index");
            //    var url = new UrlHelper(filterContext.RequestContext);
            //    var urls = url.Action("Index", "Login");
            //    filterContext.Result = new RedirectResult(urls);
            //}
        }
        #endregion
    }
}
```

ApiTrackerFilter.cs

```
using System;
using System.Globalization;
using System.Web.Mvc;

namespace RTVSApiFD.Logs
{
    /// <summary>
    /// API过滤器
    /// </summary>
    [AttributeUsage(AttributeTargets.Class | AttributeTargets.Method, AllowMultiple = false)]
    public class ApiTrackerFilter : System.Web.Mvc.ActionFilterAttribute, System.Web.Mvc.IActionFilter, System.Web.Mvc.IResultFilter, System.Web.Http.Filters.IFilter
    {
        private readonly string key = "_thisOnApiActionMonitorLog_";


        #region Action时间监控
        /// <summary>
        /// OnActionExecuting
        /// </summary>
        /// <param name="filterContext"></param>
        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            MonitorLog monLog = new MonitorLog();
            monLog.ExecuteStartTime = Convert.ToDateTime(DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss.ffff", DateTimeFormatInfo.InvariantInfo));
            monLog.ControllerName = filterContext.RouteData.Values["controller"] as string;
            monLog.ActionName = filterContext.RouteData.Values["action"] as string;
            filterContext.Controller.ViewData[this.key] = monLog;
        }

        /// <summary>
        /// OnActionExecuted
        /// </summary>
        /// <param name="filterContext"></param>
        public override void OnActionExecuted(ActionExecutedContext filterContext)
        {
            MonitorLog monLog = filterContext.Controller.ViewData[this.key] as MonitorLog;
            monLog.ExecuteEndTime = DateTime.Now;
            monLog.FormCollections = filterContext.HttpContext.Request.Form;//form表单提交的数据
            monLog.QueryCollections = filterContext.HttpContext.Request.QueryString;//Url 参数
            LoggerHelper.Monitor(monLog.GetLogInfo());
        }
        #endregion

        #region View 视图生成时间监控
        /// <summary>
        /// OnResultExecuting
        /// </summary>
        /// <param name="filterContext"></param>
        public override void OnResultExecuting(ResultExecutingContext filterContext)
        {
            MonitorLog monLog = filterContext.Controller.ViewData[this.key] as MonitorLog;
            monLog.ExecuteStartTime = DateTime.Now;
        }

        /// <summary>
        /// OnResultExecuted
        /// </summary>
        /// <param name="filterContext"></param>
        public override void OnResultExecuted(ResultExecutedContext filterContext)
        {
            MonitorLog monLog = filterContext.Controller.ViewData[this.key] as MonitorLog;
            monLog.ExecuteEndTime = DateTime.Now;
            LoggerHelper.Monitor(monLog.GetLogInfo(MonitorLog.MonitorType.View));
            filterContext.Controller.ViewData.Remove(this.key);
        }
        #endregion

        #region 错误日志
        /// <summary>
        /// OnException
        /// </summary>
        /// <param name="filterContext"></param>
        public void OnException(ExceptionContext filterContext)
        {
            if (!filterContext.ExceptionHandled)
            {
                string controllerName = string.Format("{0}Controller", filterContext.RouteData.Values["controller"] as string);
                string actionName = filterContext.RouteData.Values["action"] as string;
                string errorMsg = string.Format("在执行 controller[{0}] 的 action[{1}] 时产生异常", controllerName, actionName);
                LoggerHelper.Error(errorMsg, filterContext.Exception);
            }
        }

        /// <summary>
        /// OnAuthorization
        /// </summary>
        /// <param name="filterContext"></param>
        public void OnAuthorization(AuthorizationContext filterContext)
        {
            ////这个方法是在Action执行之前调用
            //var user = filterContext.HttpContext.Session["userName"];
            //if (user == null)
            //{
            //    //filterConetext.HttpContext.Response.Redirect("/Login/index");
            //    var url = new UrlHelper(filterContext.RequestContext);
            //    var urls = url.Action("Index", "Login");
            //    filterContext.Result = new RedirectResult(urls);
            //}
        }
        #endregion
    }
}
```

### 测试ASP..NET相关设置

在Global.asax文件中添加读取日志的配置

```
using JT1078Server;
using log4net.Config;
using RedisHelp;
using System.Web.Http;
using System.Web.Mvc;
using System.Web.Optimization;
using System.Web.Routing;

namespace RTVSApiFD
{
    /// <summary>
    /// 全局启动类
    /// </summary>
    public class WebApiApplication : System.Web.HttpApplication
    {

        /// <summary>
        /// Redis帮助类初始化
        /// </summary>
        public static RedisHelper RedisHelper = new RedisHelper();

        /// <summary>
        /// 音视频服务
        /// </summary>
        public static Task TaskService = new Task();

        /// <summary>
        /// 启动web程序
        /// </summary>
        protected void Application_Start()
        {
            AreaRegistration.RegisterAllAreas();
            GlobalConfiguration.Configure(WebApiConfig.Register);
            FilterConfig.RegisterGlobalFilters(GlobalFilters.Filters);
            RouteConfig.RegisterRoutes(RouteTable.Routes);
            BundleConfig.RegisterBundles(BundleTable.Bundles);
            //应用程序启动时，自动加载配置log4Net
            log4net.Config.XmlConfigurator.Configure();
            GlobalConfiguration.Configuration.Filters.Add(new Logs.ApiTrackerFilter());
        }
    }
}
```

此时还需要修改FilterConfig类，添加注册过滤器：

```
using System.Web;
using System.Web.Mvc;

namespace RTVSApiFD
{
    /// <summary>
    /// 过滤器配置
    /// </summary>
    public class FilterConfig
    {
        /// <summary>
        /// 注册全局过滤器
        /// </summary>
        /// <param name="filters"></param>
        public static void RegisterGlobalFilters(GlobalFilterCollection filters)
        {
            //API日志
            filters.Add(new Logs.ApiTrackerFilter());
            //监控日志
            filters.Add(new Logs.TrackerFilter());

            filters.Add(new HandleErrorAttribute());
        }
    }
}
```

修改web.config文件加入日志配置：

```
<configuration>
  <configSections>
    <section name="log4net" type="log4net.Config.Log4NetConfigurationSectionHandler,log4net" />
  </configSections>

  <log4net>
    <!--错误日志-->
    <appender name="RollingLogFileAppender" type="log4net.Appender.RollingFileAppender">
      <file value="log\\LogError\\" />
      <appendToFile value="true" />
      <rollingStyle value="Date" />
      <datePattern value="yyyy\\yyyyMM\\yyyyMMdd'.txt'" />
      <staticLogFileName value="false" />
      <param name="MaxSizeRollBackups" value="100" />
      <layout type="log4net.Layout.PatternLayout">
        <!--每条日志末尾的文字说明-->
        <!--输出格式-->
        <!--样例：2008-03-26 13:42:32,111 [10] INFO  Log4NetDemo.MainClass [(null)] - info-->
        <conversionPattern value="%newline %n记录时间：%date %n线程ID:[%thread] %n日志级别：  %-5level %n错误描述：%message%newline %n" />
      </layout>
    </appender>

    <!--Info日志-->
    <appender name="InfoAppender" type="log4net.Appender.RollingFileAppender">
      <param name="File" value="Log\\LogInfo\\" />
      <param name="AppendToFile" value="true" />
      <param name="MaxFileSize" value="10240" />
      <param name="MaxSizeRollBackups" value="100" />
      <param name="StaticLogFileName" value="false" />
      <param name="DatePattern" value="yyyy\\yyyyMM\\yyyyMMdd'.txt'" />
      <param name="RollingStyle" value="Date" />
      <layout type="log4net.Layout.PatternLayout">
        <conversionPattern value="%newline %n记录时间：%date %n线程ID:[%thread] %n日志级别：  %-5level %n日志描述：%message%newline %n" />
      </layout>
    </appender>

    <!--监控日志-->
    <appender name="MonitorAppender" type="log4net.Appender.RollingFileAppender">
      <param name="File" value="Log\\LogMonitor\\" />
      <param name="AppendToFile" value="true" />
      <param name="MaxFileSize" value="10240" />
      <param name="MaxSizeRollBackups" value="100" />
      <param name="StaticLogFileName" value="false" />
      <param name="DatePattern" value="yyyy\\yyyyMM\\yyyyMMdd'.txt'" />
      <param name="RollingStyle" value="Date" />
      <layout type="log4net.Layout.PatternLayout">
        <conversionPattern value="%newline %n记录时间：%date %n线程ID:[%thread] %n日志级别：  %-5level %n跟踪描述：%message%newline %n" />
      </layout>
    </appender>

    <!--Error日志-->
    <logger name="LogError">
      <level value="ERROR" />
      <appender-ref ref="RollingLogFileAppender" />
    </logger>

    <!--Info日志-->
    <logger name="LogInfo">
      <level value="INFO" />
      <appender-ref ref="InfoAppender" />
    </logger>

    <!--监控日志-->
    <logger name="LogMonitor">
      <level value="Monitor" />
      <appender-ref ref="MonitorAppender" />
    </logger>
  </log4net>

<configuration>
```



### 测试ASP..NET日志输出

编写一个简单的API测试，下面是API的一个方法：

```
[HttpPost]
        public Result MultiSIMAlarmForJsonParam(FileSourceMultiRequest fileSourceMultiRequest)
        {
            Result result = null;
            try
            {
                if (fileSourceMultiRequest != null)
                {
                    LoggerHelper.Monitor("MultiSIMAlarmForJsonParam:" + fileSourceMultiRequest.ToJson());
                    LoggerHelper.Info("MultiSIMAlarmForJsonParam:" + fileSourceMultiRequest.ToJson());
                    List<AVInfo> list = cache.QueryMulti(fileSourceMultiRequest.Sims, fileSourceMultiRequest.Channel, fileSourceMultiRequest.Alarms, fileSourceMultiRequest.StorageType, fileSourceMultiRequest.StreamType, fileSourceMultiRequest.StartTime.DateTimeToUNIX_long(), fileSourceMultiRequest.EndTime.DateTimeToUNIX_long(),fileSourceMultiRequest.DataType);
                    LoggerHelper.Info("MultiSIMAlarmForJsonParam:list.size=" + (list == null ? 0 : list.Count));
                    if (list != null)
                    {
                        result = new Result(1, "查询成功!", list);
                    }
                    else
                    {
                        result = new Result(0, "没有查询到结果!", new List<AVInfo>());
                    }
                    LoggerHelper.Info("Result=" + result.ToJson());
                }
            }
            catch (Exception ex)
            {
                result = new Result(2, "异常:" + ex.Message, new List<AVInfo>());
                LoggerHelper.Error("MultiSIMAlarmForJsonParam异常:" + result.ToJson());
            }
            return result;
        }
```

查看日志（运行的时候回自动创建日志路径）：

![](https://img-blog.csdn.net/20180724182849593?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

自此，日志就配置完成了! 

参考文章：[https://www.cnblogs.com/huanghzm/p/4754890.html](https://www.cnblogs.com/huanghzm/p/4754890.html)



