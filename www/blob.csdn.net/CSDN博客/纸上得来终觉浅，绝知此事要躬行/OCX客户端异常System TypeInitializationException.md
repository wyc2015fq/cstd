# OCX客户端异常System.TypeInitializationException - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月05日 19:30:37[boonya](https://me.csdn.net/boonya)阅读数：269








HttpListener是服务端提供HTTP服务的监听，web端安全认证不允许初始化。

**异常信息：**

调试IE查看问题

**![](https://img-blog.csdn.net/20180606100643895)**
`出现System.TypeInitializationException类型初始值设定项引发异常`
**异常原因：**

IE浏览器端不允许HttpListener初始化。


**异常类：**


```
class HttpService
    {
        private HttpListener m_http_listener = new HttpListener();

        private Thread m_http_thread = null;

        private FDWebRtmpService m_web_rtmp_service = null; 

        public bool Start(Task MyTask)
        {
            if (!HttpListener.IsSupported)
            {
                SQ.Base.ErrorLog.WriteLog4("HttpService::Start() HttpListener not support !", SQ.Base.LOGTYPE.ERRORD);
                return false;
            }
            if (m_http_thread != null)
                return true;

            m_web_rtmp_service = new FDWebRtmpService(MyTask);

            try
            {
                m_http_listener.AuthenticationSchemes = AuthenticationSchemes.Anonymous;
                m_http_listener.Prefixes.Add("http://" + MyTask.Config.FDWebIP +":" + MyTask.Config.FDWebPort +"/");
                m_http_listener.Start();

                m_http_thread = new Thread(HttpServiceThd);
                m_http_thread.Name = "HttpServiceThd";
                m_http_thread.IsBackground = true;
                m_http_thread.Start();
            }
            catch(Exception ex)
            {
                SQ.Base.ErrorLog.WriteLog4Ex("HttpService::Start", ex);
            }
            
            return false;
        }

        public void TimeoutCheck()
        {
            if(m_web_rtmp_service != null)
                m_web_rtmp_service.TimeoutCheck();
        }

        /// <summary>
        /// 处理http请求
        /// </summary>
        protected virtual void HttpServiceThd()
        {
       }
}
```

感悟：C#Http服务和IE安全性验证还是了解太少。

**错误代码修正：**

ocx初始化启动里面最好不要初始化

```
/// <summary>
        /// web网页客户端服务
        /// </summary>
        private HttpService m_http_service ;
```

在需要的时候初始化。

```
//启动http服务
  m_http_service = new HttpService();
  m_http_service.Start(this);
```
遗漏之处，谨记！



