# ASP.NET Web API项目自定义接口路由和HTTP请求方式 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年05月16日 10:07:10[boonya](https://me.csdn.net/boonya)阅读数：1332








在实际编写接口的过程中难免有一些特殊的接口，比如动态参数接口，以“[JTT1078-2016道路运输车辆卫星定位系统视频通信协议(扫描版)](https://download.csdn.net/download/boonya/10179991)”中的HTTP URL服务要求为例，截图如下：




![](https://img-blog.csdn.net/20180516095758356?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180516095819703?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 定义API接口和请求方式
接口定义要满足需求，这里通过四种HTTP 请求方式。



```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;

namespace RTVSApi.Controllers
{
    /// <summary>
    /// 政府音视频监管平台请求接口服务API
    /// </summary>
    public class MonitorController : ApiController
    {
        /// <summary>
        /// 政府音视频监管平台发送音视频连接指令
        /// </summary>
        /// <param name="param">参数字符串：[车牌号码].[车牌颜色].[逻辑通道号].[音视频标志].[时效口令]</param>
        /// <returns></returns>
        [Route("{param}")]
        [HttpGet]
        [HttpPost]
        [HttpPut]
        [HttpDelete]
        public void RealTimeConnection(string param) {

            if (string.IsNullOrEmpty(param))
                return;

            string[] paramArray = param.Split('.');

            // 参数须满足固定长度
            if (paramArray.Length==5)
            {
                string CarNumber = paramArray[0];
                string CarPlateColor = paramArray[1];
                string Channel = paramArray[2];
                string AVType = paramArray[3];
                string TimedPassword = paramArray[4];
            }

        }
    }
}
```

#### 配置指定接口的路由方式 

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;
using System.Web.Routing;

namespace RTVSApi
{
    public class RouteConfig
    {
        public static void RegisterRoutes(RouteCollection routes)
        {
            routes.IgnoreRoute("{resource}.axd/{*pathInfo}");

            routes.MapRoute(
                name: "Default",
                url: "{controller}/{action}/{id}",
                defaults: new { controller = "Home", action = "Index", id = UrlParameter.Optional }
            );

            routes.MapRoute(
               name: "Monitor",
               url: "{param}",
               defaults: new { controller = "Monitor", action = "RealTimeConnection", id = UrlParameter.Optional }
           );
        }
    }
}
```

#### 运行项目查看定好的接口是否可行
查看接口目录列表：
![](https://img-blog.csdn.net/20180516100417372?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

以GET请求方式为例，我们查看参数情况：

![](https://img-blog.csdn.net/20180516100514819?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


经过调试，请求可以进去，但是加了'.'的字符感觉有点问题，总之大的方向是对的。


参考文章：[WebAPI路由分析](https://www.cnblogs.com/landeanfen/p/5501490.html)

#### AcceptVerbs注解实现HTTP多请求方式

```
[AcceptVerbs("Get", "Post")]     //用AcceptVerbs标识即可  
    public List<PUB_HqewIndexResult> GetSalesRankByHqewIndex()  
    {  
        List<PUB_HqewIndexResult> resultList = new List<PUB_HqewIndexResult>();  
        DMSParam dmsParam = new DMSParam();  
        List<PUB_Stock> stockList = new List<PUB_Stock>();  
        IResult stockResult = ExecuteDmsParam(dmsParam, new PUB_HqewIndex(), "PUB.PUB_HqewIndexBLL", "GetSalesRankByHqewIndex");  
        if (stockResult.Complete == DMSComplete.Succeed)  
        {  
            if (stockResult != null && stockResult.Result != null)  
            {  
                resultList = (List<PUB_HqewIndexResult>)stockResult.Result;  
            }  
        }  
        return resultList;  
    }
```




