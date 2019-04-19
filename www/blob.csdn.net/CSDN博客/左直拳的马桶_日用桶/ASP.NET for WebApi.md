# ASP.NET for WebApi - 左直拳的马桶_日用桶 - CSDN博客
2016年05月27日 19:43:51[左直拳](https://me.csdn.net/leftfist)阅读数：6327
WebApi，听说过吧？呵呵。
感觉比WebService，WCF要强。尤其是那个啥WCF，啥鬼东西，真难懂。真难搞。真难用。
说比WebService要强，是因为不用在本地先生成个代理。而且XML也显得过于庞大。
用ASP.NET 编写 WebApi 的话，跟MVC差不多。生成的项目也有控制器，视图之类。只不过，其控制器继承的是ApiController。
还有一个不同的是，是不用写action的，只有controller。当然，你硬是要写也可以。但根据RESTful原则，WebApi提供的API方法名称应该纯粹是名词，而没有动词，系统根据请求方式来判断请求者意图，并执行相应操作。什么意思呢？
比如一个API 地址为 
`http://192.168.0.1/api/book/108`
相应的方法为
```
public class BookController : ApiController
    {
        public string Get(string id)
        {//读取book信息
            return "Get " + id;
        }
        public string Post([FromBody]object obj)
        {//新增book
            return "Post";
        }
        public string Put(int id,[FromBody]object obj)
        {//修改book
            return "Put " + id;
        }
        public string Delete(string id)
        {//删除book
            return "Delete " + id;
        }
    }
```
当请求方式为GET时，执行的是方法Get()，Post时，执行的是方法Post()。API地址就一个：
`http://192.168.0.1/api/book/108`
108是ID值。
而不必写成下面那么傻：
```
http://192.168.0.1/api/GetBook/108
http://192.168.0.1/api/DeleteBook/108
```
当然，RESTful原则是这么推荐，但现实往往比较骨感。因为许多网络环境并不支持PUT和DELETE的请求方式，只支持GET和POST。怪不得我调试的时候，GET和POST都可以，到了PUT和DELETE，都报405错误：方法不被允许。错误代码为4开头，说明是浏览器不支持。
幸而看了蒋金楠博客。解决方案如下：
```
//我们定义了如下一个HttpMethodOverrideHandler类型，它继承自DelegatingHandler。我们在重写的SendAsync方法中实现了对“X-HTTP-Method-Override”报头的提取和对HTTP方法的重写，最后调用基类的同名方法将处理后的请求传递给后续的HttpMessageHandler。
        public class HttpMethodOverrideHandler : DelegatingHandler
        {
            protected override Task<HttpResponseMessage> SendAsync(HttpRequestMessage request, CancellationToken cancellationToken)
            {
                IEnumerable<string> methodOverrideHeader;
                if (request.Headers.TryGetValues("X-HTTP-Method-Override", out methodOverrideHeader))
                {
                    request.Method = new HttpMethod(methodOverrideHeader.First());
                }
                return base.SendAsync(request, cancellationToken);
            }
        }
```
在api项目的global.asax.cs里调用这个类：
```
public class WebApiApplication : System.Web.HttpApplication
    {
        protected void Application_Start()
        {          
            AreaRegistration.RegisterAllAreas();
            GlobalConfiguration.Configure(WebApiConfig.Register);
            FilterConfig.RegisterGlobalFilters(GlobalFilters.Filters);
            RouteConfig.RegisterRoutes(RouteTable.Routes);
//调用基类的同名方法将处理后的请求传递给后续的HttpMessageHandler。
            GlobalConfiguration.Configuration.MessageHandlers.Add(new HttpMethodOverrideHandler());
        }
    }
```
这样，请求过来时，读取这些额外设置的报头，系统就能知道请求者的意图了。
前端调用方式如下：
```java
<script type="text/javascript">
    (function(){
        $.ajax({
            type: 'POST',//明地里用支持的HTTP请求方式
            url: 'http://192.168.0.1/api/GetBook/108',
            data: '{"name":"王小波文集"}',
            contentType: "application/json; charset=utf-8",
            dataType: 'json',
            beforeSend: function (xhr) {
            //暗地里写入额外的报头信息
                xhr.setRequestHeader('X-HTTP-Method-Override', 'PUT');
            },
            success: function (results) {
                alert(results);
            },
            error: function (ex) {
                alert("错误：" + ex.statusText);
            }
        })
    })();
</script>
```
服务器端请求也可以，看俺的单元测试代码：
```
[TestClass()]
    public class InterfaceControllerTests
    {
        string _baseurl = "http://192.168.0.1/api/GetBook/108";
        public InterfaceControllerTests()
        {
        }
        [TestMethod()]
        public void GetTest()
        {
            string url = _baseurl + "/1";
            string re;
            using (WebClient webClient = new WebClient())
            {
                webClient.Encoding = Encoding.GetEncoding("utf-8");
                webClient.Headers.Add("Content-Type", "application/x-www-form-urlencoded");//采取POST方式必须加的header，如果改为GET方式的话就去掉这句话即可  
                re = webClient.DownloadString(url);
            }
            Assert.AreEqual("\"Get 1\"", re);
        }
        [TestMethod()]
        public void PostTest()
        {
            string url = _baseurl;
            string postString = @"{""id"":108}";//这里即为传递的参数，可以用工具抓包分析，也可以自己分析，主要是form里面每一个name都要加进来  
            byte[] postData = Encoding.UTF8.GetBytes(postString);//编码，尤其是汉字，事先要看下抓取网页的编码方式  
            string re;
            using (WebClient webClient = new WebClient())
            {
                webClient.Encoding = Encoding.GetEncoding("utf-8");
                webClient.Headers.Add("Content-Type", "application/json");//采取POST方式必须加的header，如果改为GET方式的话就去掉这句话即可  
                byte[] responseData = webClient.UploadData(url, "POST", postData);//得到返回字符流  
                re = Encoding.UTF8.GetString(responseData);//解码  
            }
            Assert.AreNotEqual(-1, re.IndexOf("Post"));
        }
        [TestMethod()]
        public void PutTest()
        {
            string url = _baseurl + "/109";
            string postString = @"{""id"":109}";
            byte[] postData = Encoding.UTF8.GetBytes(postString);
            string re;
            using (WebClient webClient = new WebClient())
            {
                webClient.Encoding = Encoding.GetEncoding("utf-8");
                webClient.Headers.Add("Content-Type", "application/json");
                webClient.Headers.Add("X-HTTP-Method-Override", "PUT");
                byte[] responseData = webClient.UploadData(url, "POST", postData);
                re = Encoding.UTF8.GetString(responseData);
            }
            Assert.AreNotEqual(-1, re.IndexOf("Put"));
        }
        [TestMethod()]
        public void DeleteTest()
        {
            string url = _baseurl + "/1";
            string re;
            using (WebClient webClient = new WebClient())
            {
                webClient.Encoding = Encoding.GetEncoding("utf-8");
                webClient.Headers.Add("Content-Type", "application/x-www-form-urlencoded");
                webClient.Headers.Add("X-HTTP-Method-Override", "DELETE");
                re = webClient.DownloadString(url);
            }
            Assert.AreNotEqual(-1, re.IndexOf("Delete"));
        }
    }
```
//上面这些POST测试通过的前提，是要将数据传送方式改为json:
```
public static class WebApiConfig  
{  
    public static void Register(HttpConfiguration config)  
    {  
        // Web API 路由  
        config.MapHttpAttributeRoutes();  
        config.Routes.MapHttpRoute(  
            name: "DefaultApi",  
            routeTemplate: "api/{controller}/{action}/{id}",  
            defaults: new { id = RouteParameter.Optional }  
        );  
        config.Formatters.JsonFormatter.SupportedMediaTypes.Add(new MediaTypeHeaderValue("text/html"));//改为JSON方式  
    }  
}
```
参考文章： 
[如果调用ASP.NET Web API不能发送PUT/DELETE请求怎么办？](http://www.cnblogs.com/artech/p/x-http-method-override.html)
