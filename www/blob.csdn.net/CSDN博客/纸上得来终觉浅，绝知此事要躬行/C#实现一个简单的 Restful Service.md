# C#实现一个简单的 Restful Service - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月09日 11:04:03[boonya](https://me.csdn.net/boonya)阅读数：22953








原文来自：[http://www.cnblogs.com/mingmingruyuedlut/p/3740975.html](http://www.cnblogs.com/mingmingruyuedlut/p/3740975.html)


用 C#  实现一个简单的 Restful Service 供外部调用，大体总结为4点：
- The service contract (the methods it offers).
- How do you know which one to access from the URL given (URL Routing).
- The implementation of the service.
- How you will host the service.



详细的基本步骤如下所示： 

**1）：工程结构（Class Library Project）**

![](http://images.cnitblog.com/i/161932/201405/211432505906634.png)



**2）： IRestDemoService.cs**

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using System.ServiceModel.Web;

namespace EricSunRestService
{
    [ServiceContract(Name = "RestDemoServices")]
    public interface IRestDemoServices
    {
        [OperationContract]
        [WebGet(UriTemplate = Routing.GetClientRoute, BodyStyle = WebMessageBodyStyle.Bare)]
        string GetClientNameById(string Id);
    }


    public static class Routing
    {
        public const string GetClientRoute = "/Client/{id}";
    }
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



**3）：RestDemoService.cs**

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.ServiceModel;
using System.ServiceModel.Activation;

namespace EricSunRestService
{
    [ServiceBehavior(InstanceContextMode = InstanceContextMode.Single, ConcurrencyMode = ConcurrencyMode.Single, IncludeExceptionDetailInFaults = true)]
    [AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
    public class RestDemoServices : IRestDemoServices
    {
        public string GetClientNameById(string Id)
        {
            string ReturnString = "HaHa id is: " + Id;

            return ReturnString;
        }
    }
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



**4）：Host Service 工程结构 （Console Application）**

![](http://images.cnitblog.com/i/161932/201405/211436275746043.png)



**5）：Program.cs**

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using EricSunRestService;
using System.ServiceModel.Web;

namespace EricSunHostService
{
    class Program
    {
        static void Main(string[] args)
        {
            RestDemoServices demoServices = new RestDemoServices();
            WebServiceHost _serviceHost = new WebServiceHost(demoServices, new Uri("http://localhost:8000/DemoService"));
            _serviceHost.Open();
            Console.ReadKey();
            _serviceHost.Close();
        }
    }
}

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```



**6）：运行Host程序，在浏览器中输入对应Service的Url**

![](http://images.cnitblog.com/i/161932/201405/211439152621148.png)



