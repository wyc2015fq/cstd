# Web Api 安全 - 左直拳的马桶_日用桶 - CSDN博客
2017年05月24日 05:25:33[左直拳](https://me.csdn.net/leftfist)阅读数：1323
过去，我写过一两个Web Api，没有任何身份校验和安全措施，随便在浏览器中输入地址就能访问，谁都可以。无异于裸奔。
有关Web Api的安全措施，我目前了解到的有以下一些：
**一、使用Basic Authentication验证用户**
客户端在发送Http请求的时候在Header部分提供一个基于Base64编码的用户名和密码，形式为“username：password”，消息接收者（服务器）进行验证，通过后继续处理请求。
![这里写图片描述](https://img-blog.csdn.net/20170523044940575?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
实现方法： 
**1、定义一个安全特性”ApiAuthorizeAttribute”**
```
using System;
using System.Net;
using System.Net.Http;
using System.Security.Principal;
using System.Text;
using System.Threading;
using System.Web.Http.Filters;
namespace WebApi
{
    public class ApiAuthorizeAttribute : AuthorizationFilterAttribute
    {
        public override void OnAuthorization(System.Web.Http.Controllers.HttpActionContext actionContext)
        {
            //forms authentication Case that user is authenticated using forms authentication//so no need to check header for basic authentication.
            if (Thread.CurrentPrincipal.Identity.IsAuthenticated)
            {
                return;
            }
            var authHeader = actionContext.Request.Headers.Authorization;
            if (authHeader != null)
            {
                if (authHeader.Scheme.Equals("basic", StringComparison.OrdinalIgnoreCase) &&
                    !String.IsNullOrWhiteSpace(authHeader.Parameter))
                {
                    var credArray = GetCredentials(authHeader);
                    var userName = credArray[0];
                    var password = credArray[1];
                    //if (IsResourceOwner(userName, actionContext))
                    if(isCheckOK(userName,password))
                    {
                        //You can use Websecurity or asp.net memebrship provider to login, for
                        //for he sake of keeping example simple, we used out own login functionality
                        //if (TheRepository.LoginStudent(userName, password))
                        {
                            var currentPrincipal = new GenericPrincipal(new GenericIdentity(userName), null);
                            Thread.CurrentPrincipal = currentPrincipal;
                            return;
                        }
                    }
                }
            }
            HandleUnauthorizedRequest(actionContext);
        }
        private bool isCheckOK(string username,string pwd)
        {//这里我只是简单做个判断；可以引入数据库操作做一般意义上的校验
            return (username == "chenqu");
        }
        private string[] GetCredentials(System.Net.Http.Headers.AuthenticationHeaderValue authHeader)
        {
            //Base 64 encoded string
            var rawCred = authHeader.Parameter;
            var encoding = Encoding.GetEncoding("iso-8859-1");
            var cred = encoding.GetString(Convert.FromBase64String(rawCred));
            var credArray = cred.Split(':');
            return credArray;
        }
        private bool IsResourceOwner(string userName, System.Web.Http.Controllers.HttpActionContext actionContext)
        {
            var routeData = actionContext.Request.GetRouteData();
            var resourceUserName = routeData.Values["userName"] as string;
            if (resourceUserName == userName)
            {
                return true;
            }
            return false;
        }
        private void HandleUnauthorizedRequest(System.Web.Http.Controllers.HttpActionContext actionContext)
        {
            actionContext.Response = actionContext.Request.CreateResponse(HttpStatusCode.Unauthorized);
            actionContext.Response.Headers.Add("WWW-Authenticate",
                                               "Basic Scheme='eLearning' location='http://localhost:8323/account/login'");
        }
    }
}
```
2、Web Api的controller或action上应用安全特性”ApiAuthorizeAttribute”
```
namespace WebApi.Areas.Test.Controllers
{
    public class Test1Controller : ApiController
    {
        [ApiAuthorize]
        public string Get(int id)
        {
            return "Hello World! Hello Human!";
        }
    }
}
```
**二、https**
由于在Basic Authentication验证用户方式中，用户名和密码仅适用base64编码，因此为了保证安全性，basic authentication通常是基于SSL连接（https）。
将一个网站设置成支持https协议很简单，以IIS为例： 
![这里写图片描述](https://img-blog.csdn.net/20170524045825259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170524045840012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170524045858902?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
一定要绑定一个证书，否则会无法访问。
另外，https这里绑定的端口是443（默认），那么地址 
[https://testweb/webapi/api/test/test1/1](https://testweb/webapi/api/test/test1/1) 相当于 
[https://testweb:443/webapi/api/test/test1/1](https://testweb:443/webapi/api/test/test1/1)。 
我有一次因为创建了一个本地网站，端口是8000，然后支持https，思维定式地认为访问地址是：[https://testweb:8000/](https://testweb:8000/)，结果提示404。https可不是单纯地在http后面加个s而已，是两种不同的协议，分别有自己对应的端口。
有关这个https，证书，服务器端证书，客户端证书，是另外一个主题。
**三、如何访问这些Web Api？**
既然是Web Api，并非是在浏览器地址里输入进行访问的，主要还是在程序里调用。 
以C#为例。客户端：
```
public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        static readonly Encoding UTF8 = Encoding.GetEncoding("utf-8");
        private void button1_Click(object sender, EventArgs e)
        {
            using (WebClient webClient = new WebClient())
            {
                ServicePointManager.ServerCertificateValidationCallback = delegate { return true; };
                webClient.Encoding = UTF8;
                webClient.Credentials = new NetworkCredential("chenqu", "123456");
MessageBox.Show(webClient.DownloadString("https://localhost/webapi/api/test/test1/1"));
            }
        }
    }
```
![这里写图片描述](https://img-blog.csdn.net/20170523050948149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考文章： 
[Web Api的安全性](http://www.aizhengli.com/aspnet-web-api-rest/27/aspnet-web-api-rest-security.html)
