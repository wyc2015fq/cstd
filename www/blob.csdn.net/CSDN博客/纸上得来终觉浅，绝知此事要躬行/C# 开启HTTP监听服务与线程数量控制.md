# C# 开启HTTP监听服务与线程数量控制 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年07月14日 16:31:39[boonya](https://me.csdn.net/boonya)阅读数：588








如何开IP和端口的HTTP监听这里就是按照实现这种方式来加以说明，另外线程不要再循环里面不停的new Thread()这样很耗性能。

##### 开启HTTP服务监听

```
/// <summary>
        /// 启动HttpService监听
        /// </summary>
        /// <param name="MyTask"></param>
        /// <returns></returns>
        public bool Start(Task MyTask)
        {
            this.MyTask = MyTask;
            if (!HttpListener.IsSupported)
            {
                SQ.Base.ErrorLog.WriteLog4("GovernmentHttpService::Start() HttpListener not support !", SQ.Base.LOGTYPE.ERRORD);
                return false;
            }
            if (m_http_thread != null)
                return true;

            string http_url = "http://" + MyTask.Config.GovWebIp + ":" + MyTask.Config.GovPort + "/";
            try
            {
                m_http_listener.AuthenticationSchemes = AuthenticationSchemes.Anonymous;
                m_http_listener.Prefixes.Add(http_url);
                m_http_listener.Start();

                m_http_thread = new Thread(HttpServiceThd);
                m_http_thread.Name = "HttpServiceThd";
                m_http_thread.IsBackground = true;
                m_http_thread.Start();

                m_service_thread = new Thread(ServiceThd);
                m_service_thread.Name = "ServiceThd";
                m_service_thread.IsBackground = true;
                m_service_thread.Start();
            }
            catch (Exception ex)
            {
                SQ.Base.ErrorLog.WriteLog4Ex("GovernmentHttpService::Start " + http_url + " ", ex);
            }

            return false;
        }




        /// <summary>
        /// 线程处理http请求方法
        /// </summary>
        protected virtual void HttpServiceThd()
        {
            while (true)
            {
                try
                {
                    HttpListenerContext context = m_http_listener.GetContext();

                    HttpListenerRequest request = context.Request;
                    HttpListenerResponse response = context.Response;

                    var param = request.Url.ToString();

                    // 解码ASCII参数
                    // param = HttpDataParser.ASCII2Str(param);

                    // 解码UTF8参数
                    param = Encoding.UTF8.GetString(Encoding.Default.GetBytes(param.ToString()));

                    Log.WriteLog4("上级平台请求:>>>>>>>>>>>>>>>" + param);

                    int index = param.LastIndexOf("/");
                    param = param.Substring(index, param.Length - index);

                    // 替换"/"
                    param = param.Replace("/", "");

                    // 预解析
                    HttpDataParser.Validator preValidator = new HttpDataParser.Validator(MyTask);

                    if (preValidator.PreParse(param))
                    {
                        //验证通过之后调用接口复用或者建立连接传输流数据
                        //validator.Sim,validator.Channel,validator.DataType;
                        //redis key:车牌号码.车牌颜色.逻辑道通号.音视频标志
                        //实时 real@json; 回放 back@json ;实时控制 realCtrl@json;
                        HttpDataParser.Validator validator = HttpDataParser.Parse(MyTask, param);

                        bool status = validator.GetStatus();
                        if (status)
                        {
                            Service service = new Service();
                            service.Context = context;
                            service.Validator = validator;
                            string key = validator.PlateCode + "." + validator.PlateColor + "." + validator.Channel + "." + validator.DataType;
                            dictionaryService.Add(key, service);
                            dictionaryTime.Add(key, DateTime.Now);
                        }
                        else
                        {
                            // 未通过解析
                            response.StatusCode = 300;
                            response.Close();
                        }
                    }
                    else
                    {
                        // 未通过预解析
                        response.StatusCode = 300;
                        response.Close();
                    }
                }
                catch (Exception ex)
                {
                    SQ.Base.ErrorLog.WriteLog4Ex("上级平台请求HTTP流服务->GovernmentHttpService:HttpServiceThd", ex);
                }
            }
        }
```

启动start方法即可。

##### 控制线程数量

控制线程的数量可以有效提高系统性能，最简单的方式就是开启一个线程来处理一类业务，将自己的需要处理的加到一个集合中用线程去处理：

```
Service service = new Service();
                            service.Context = context;
                            service.Validator = validator;
                            string key = validator.PlateCode + "." + validator.PlateColor + "." + validator.Channel + "." + validator.DataType;
                            dictionaryService.Add(key, service);
                            dictionaryTime.Add(key, DateTime.Now);
```

执行业务处理：

```
class Service
        {
            public HttpListenerContext Context { set; get; }

            public HttpDataParser.Validator Validator { set; get; }

        }

        /// <summary>
        /// 业务线程执行方法
        /// </summary>
        void ServiceThd()
        {
            while (true)
            {
                try
                {
                    TimeSpan timeSpan;
                    foreach (var item in dictionaryService.ToList())
                    {
                        DateTime startTime;
                        dictionaryTime.TryGetValue(item.Key, out startTime);
                        timeSpan = DateTime.Now - startTime;
                        if (timeSpan.Seconds < 5)
                        {
                            bool flag = ServiceProcess(item.Value.Context, item.Value.Validator);
                            if (flag)
                            {
                                dictionaryService.Remove(item.Key);
                                dictionaryTime.Remove(item.Key);
                            }
                        }
                        else
                        {
                            dictionaryService.Remove(item.Key);
                            dictionaryTime.Remove(item.Key);
                        }
                    }
                    Thread.Sleep(1000);
                }
                catch (Exception ex)
                {
                    SQ.Base.ErrorLog.WriteLog4Ex("上级平台业务处理线程执行GovernmentHttpService:ServiceThd",ex);
                }
            }
        }

        /// <summary>
        /// 执行业务处理方法
        /// </summary>
        /// <param name="context"></param>
        /// <param name="validator"></param>
        bool ServiceProcess(HttpListenerContext context,HttpDataParser.Validator validator)
        {
            try
            {
                string key = validator.PlateCode + "." + validator.PlateColor + "." + validator.Channel + "." + validator.DataType;

                string result = HttpDataParser.GetRedisData(key);

                if (!string.IsNullOrEmpty(result) && result.Contains("@"))
                {
                    SQ.Base.Log.WriteLog4("上级平台业务处理Get data from redis:result=" + result);
                    string[] strs = result.Split('@');

                    var type = strs[0];
                    var json = strs[1];

                    if ("real".Equals(type))
                    {
                        // 实时视频
                        JTSDownRealVideoRequest jtSDownRealVideoRequest = json.ParseJSON<JTSDownRealVideoRequest>();
                        GovernmentWebClientManage.AcceptRealConnect(context, key, validator.Sim, (byte)validator.Channel, (byte)validator.DataType);
                        return true;
                    }
                    else if ("back".Equals(type))
                    {
                        // 回放视频
                        JTRTDownPlayBackMsgStartUp jtRTDownPlayBackMsgStartUp = json.ParseJSON<JTRTDownPlayBackMsgStartUp>();
                        GovernmentWebClientManage.AcceptBackConnect(context, key, validator.Sim, (byte)validator.Channel, jtRTDownPlayBackMsgStartUp.PLAYBACK_STARTTIME.UNIXtoDateTime(), (long)jtRTDownPlayBackMsgStartUp.PLAYBACK_ENDTIME, (AudioVideoFlag)validator.DataType);
                        return true;
                    }
                }
                else
                {
                    SQ.Base.Log.WriteLog4("上级平台业务处理Get data from redis:result="+result);
                }
            }
            catch (Exception ex)
            {
                SQ.Base.ErrorLog.WriteLog4Ex("上级平台业务处理ServiceProcess read data from redis:", ex);
            }
            return false;
        }
```

这里我们应该注意到，我们会启动这个线程，前面HTTP服务启动中有，代码如下：

```
m_service_thread = new Thread(ServiceThd);
                m_service_thread.Name = "ServiceThd";
                m_service_thread.IsBackground = true;
                m_service_thread.Start();
```
注意：代码的规范性和简洁，才能保证优质的服务。

** 要记得操作List和Dictionary加锁，以下是示例代码：**

```
using JT1078Server.Client.GovWebClient;
using JX;
using SQ.Base;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;

namespace JT1078Server.Client.GovWeb
{
    /// <summary>
    /// 上级政府平台HTTP请求流监听服务
    /// </summary>
    public class GovernmentHttpService
    {
        private HttpListener m_http_listener = new HttpListener();

        private Thread m_http_thread = null;

        private Thread m_service_thread = null;

        private Dictionary<string, DateTime> dictionaryTime = new Dictionary<string, DateTime>();

        private Dictionary<string, Service> dictionaryService = new Dictionary<string, Service>();

        Task MyTask=null;

        object obj = new object();

        /// <summary>
        /// 启动HttpService监听
        /// </summary>
        /// <param name="MyTask"></param>
        /// <returns></returns>
        public bool Start(Task MyTask)
        {
            this.MyTask = MyTask;
            if (!HttpListener.IsSupported)
            {
                SQ.Base.ErrorLog.WriteLog4("GovernmentHttpService::Start() HttpListener not support !", SQ.Base.LOGTYPE.ERRORD);
                return false;
            }
            if (m_http_thread != null)
                return true;

            string http_url = "http://" + MyTask.Config.GovWebIp + ":" + MyTask.Config.GovPort + "/";
            try
            {
                m_http_listener.AuthenticationSchemes = AuthenticationSchemes.Anonymous;
                m_http_listener.Prefixes.Add(http_url);
                m_http_listener.Start();

                m_http_thread = new Thread(HttpServiceThd);
                m_http_thread.Name = "HttpServiceThd";
                m_http_thread.IsBackground = true;
                m_http_thread.Start();

                m_service_thread = new Thread(ServiceThd);
                m_service_thread.Name = "ServiceThd";
                m_service_thread.IsBackground = true;
                m_service_thread.Start();
            }
            catch (Exception ex)
            {
                SQ.Base.ErrorLog.WriteLog4Ex("GovernmentHttpService::Start " + http_url + " ", ex);
            }

            return false;
        }




        /// <summary>
        /// 线程处理http请求方法
        /// </summary>
        protected virtual void HttpServiceThd()
        {
            while (true)
            {
                try
                {
                    HttpListenerContext context = m_http_listener.GetContext();

                    HttpListenerRequest request = context.Request;
                    HttpListenerResponse response = context.Response;

                    var param = request.Url.ToString();
                    Log.WriteLog4("上级平台原始请求:>>>>>>>>>>>>>>>" + param);
                    param= System.Web.HttpUtility.UrlDecode(param, Encoding.UTF8);
                    Log.WriteLog4("上级平台解码请求:>>>>>>>>>>>>>>>" + param);

                    // 解码ASCII参数
                    // param = HttpDataParser.ASCII2Str(param);
                    //if(true)
                    //{
                    //    GovernmentWebClientManage.AcceptRealConnect(context, "123", "013777882100", 1, 0);
                    //}
                    //continue;

                    // 解码ASCII参数
                    // param = HttpDataParser.ASCII2Str(param);

                    //// 解码UTF8参数
                    //param = Encoding.UTF8.GetString(Encoding.Default.GetBytes(param.ToString()));
                    

                    int index = param.LastIndexOf("/");
                    param = param.Substring(index, param.Length - index);

                    // 替换"/"
                    param = param.Replace("/", "");

                    // 预解析
                    HttpDataParser.Validator preValidator = new HttpDataParser.Validator(MyTask);

                    if (preValidator.PreParse(param))
                    {
                        //验证通过之后调用接口复用或者建立连接传输流数据
                        //validator.Sim,validator.Channel,validator.DataType;
                        //redis key:车牌号码.车牌颜色.逻辑道通号.音视频标志
                        //实时 real@json; 回放 back@json ;实时控制 realCtrl@json;
                        HttpDataParser.Validator validator = HttpDataParser.Parse(MyTask, param);

                        bool status = validator.GetStatus();
                        if (status)
                        {
                            Service service = new Service();
                            service.Context = context;
                            service.Validator = validator;
                            string key = validator.PlateCode + "." + validator.PlateColor + "." + validator.Channel + "." + validator.DataType;
                            // 添加到处理列表
                            lock (obj)
                            {
                                if (dictionaryService.ContainsKey(key))
                                {
                                    dictionaryService[key] = service;
                                    dictionaryTime[key] = DateTime.Now;
                                }
                                else
                                {
                                    dictionaryService.Add(key, service);
                                    dictionaryTime.Add(key, DateTime.Now);
                                }
                            }
                           
                        }
                        else
                        {
                            // 未通过解析
                            response.StatusCode = 300;
                            response.Close();
                        }
                    }
                    else
                    {
                        // 未通过预解析
                        response.StatusCode = 300;
                        response.Close();
                    }
                }
                catch (Exception ex)
                {
                    SQ.Base.ErrorLog.WriteLog4Ex("上级平台请求HTTP流服务->GovernmentHttpService:HttpServiceThd", ex);
                }
            }
        }

        class Service
        {
            public HttpListenerContext Context { set; get; }

            public HttpDataParser.Validator Validator { set; get; }

        }

        /// <summary>
        /// 业务线程执行方法
        /// </summary>
        void ServiceThd()
        {
            while (true)
            {
                try
                {
                    TimeSpan timeSpan;
                    foreach (var item in dictionaryService.ToList())
                    {
                        DateTime startTime;
                        dictionaryTime.TryGetValue(item.Key, out startTime);
                        timeSpan = DateTime.Now - startTime;
                        if (timeSpan.Seconds < 5)
                        {
                            bool flag = ServiceProcess(item.Value.Context, item.Value.Validator);
                            if (flag)
                            {
                                lock(obj)
                                {
                                    dictionaryService.Remove(item.Key);
                                    dictionaryTime.Remove(item.Key);
                                }
                            }
                        }
                        else
                        {
                            lock(obj)
                            {
                                dictionaryService.Remove(item.Key);
                                dictionaryTime.Remove(item.Key);
                            }
                        }
                    }
                    Thread.Sleep(1000);
                }
                catch (Exception ex)
                {
                    SQ.Base.ErrorLog.WriteLog4Ex("上级平台业务处理线程执行GovernmentHttpService:ServiceThd",ex);
                }
            }
        }

        /// <summary>
        /// 执行业务处理方法
        /// </summary>
        /// <param name="context"></param>
        /// <param name="validator"></param>
        bool ServiceProcess(HttpListenerContext context,HttpDataParser.Validator validator)
        {
            try
            {
                string key = validator.PlateCode + "." + validator.PlateColor + "." + validator.Channel + "." + validator.DataType;

                string result = HttpDataParser.GetRedisData(key);

                if (!string.IsNullOrEmpty(result) && result.Contains("@"))
                {
                    SQ.Base.Log.WriteLog4("上级平台业务处理Get data from redis:result=" + result);
                    string[] strs = result.Split('@');

                    var type = strs[0];
                    var json = strs[1];

                    if ("real".Equals(type))
                    {
                        // 实时视频
                        JTSDownRealVideoRequest jtSDownRealVideoRequest = json.ParseJSON<JTSDownRealVideoRequest>();
                        GovernmentWebClientManage.AcceptRealConnect(context, key, validator.Sim, (byte)validator.Channel, (byte)validator.DataType);
                        return true;
                    }
                    else if ("back".Equals(type))
                    {
                        // 回放视频
                        JTRTDownPlayBackMsgStartUp jtRTDownPlayBackMsgStartUp = json.ParseJSON<JTRTDownPlayBackMsgStartUp>();
                        GovernmentWebClientManage.AcceptBackConnect(context, key, validator.Sim, (byte)validator.Channel, jtRTDownPlayBackMsgStartUp.PLAYBACK_STARTTIME.UNIXtoDateTime(), (long)jtRTDownPlayBackMsgStartUp.PLAYBACK_ENDTIME, (AudioVideoFlag)validator.DataType);
                        return true;
                    }
                }
                else
                {
                    SQ.Base.Log.WriteLog4("上级平台业务处理Get data from redis:result="+result);
                }
            }
            catch (Exception ex)
            {
                SQ.Base.ErrorLog.WriteLog4Ex("上级平台业务处理ServiceProcess read data from redis:", ex);
            }
            return false;
        }
    }
}
```










