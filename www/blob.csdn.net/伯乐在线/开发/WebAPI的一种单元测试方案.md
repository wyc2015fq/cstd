# WebAPI的一种单元测试方案 - 文章 - 伯乐在线
原文出处： [richieyangs](http://www.cnblogs.com/richieyang/p/4658611.html)
大家是如何对webApi写测试的呢？
1.利用Fiddler直接做请求，观察response的内容。
2.利用Httpclient做请求，断言response的内容。
3.直接调用webApi的action,这种方式的测试跟真实的调用还是有一定差距，不够完美。
接下来我介绍一种webApi的in-memory调用方法，也能够达到对webApi的测试，并且由于是in-memory调用，效率也比较高，非常适写单元测试。本文参考了[In memory client, host and integration testing of your Web API service](http://blogs.msdn.com/b/kiranchalla/archive/2012/05/06/in-memory-client-amp-host-and-integration-testing-of-your-web-api-service.aspx)。
一、首先写一个OrderController用来做测试用

C#
```
public class OrderController : ApiController
    {
        // GET api/order
        public Order Get()
        {
            return new Order(){Id = 1,Descriptions = "descriptions",Name = "name"};
        }
        // GET api/order/5
        public string Get(int id)
        {
            return "value";
        }
        // POST api/order
        public Order Post(Order order)
        {
            return order;
        }
        // DELETE api/order/5
        public void Delete(int id)
        {
        }
    }
```
二、WebApi的请求过程
webApi的核心是对消息的管道处理，整个核心是有一系列消息处理器(HttpMessageHandler)首尾连接的双向管道，管道头为HttpServer,管道尾为HttpControllerDispatcher,HttpControllerDispatcher负责对controller的激活和action的执行，然后相应的消息逆向流出管道。
所以我们可以利用HttpMessageInvoker将一个请求消息HttpRequestMessage发送到管道中，最后收到的消息HttpResponseMessage就代表一个真实的请求响应。
三、Get请求的测试

C#
```
[Test]
        public void GetTest()
        {
            string baseAddress = "http://localhost:33203/";
            HttpConfiguration config = new HttpConfiguration();
            WebApiConfig.Register(config);
            config.IncludeErrorDetailPolicy = IncludeErrorDetailPolicy.Always;
            HttpServer server = new HttpServer(config);
            HttpMessageInvoker messageInvoker = new HttpMessageInvoker(server);
            CancellationTokenSource cts = new CancellationTokenSource();
            HttpRequestMessage request = new HttpRequestMessage(HttpMethod.Get, baseAddress + "api/order");
            using (HttpResponseMessage response = messageInvoker.SendAsync(request, cts.Token).Result)
            {
                var content = response.Content.ReadAsStringAsync().Result;
                var result = JsonConvert.DeserializeObject(content);
                result.Name.Should().Be("name");
            }
        }
```
四、Post请求的测试

C#
```
[Test]
        public void PostTest()
        {
            string baseAddress = "http://localhost:33203/";
            HttpConfiguration config = new HttpConfiguration();
            WebApiConfig.Register(config);
            config.IncludeErrorDetailPolicy = IncludeErrorDetailPolicy.Always;
            HttpServer server = new HttpServer(config);
            HttpMessageInvoker messageInvoker = new HttpMessageInvoker(server);
            CancellationTokenSource cts = new CancellationTokenSource();
            HttpRequestMessage request = new HttpRequestMessage(HttpMethod.Post, baseAddress + "api/order");
            var order = new Order() { Id = 1, Name = "orderName", Descriptions = "orderDescriptions" };
            request.Content = new ObjectContent(order, new JsonMediaTypeFormatter());
            using (HttpResponseMessage response = messageInvoker.SendAsync(request, cts.Token).Result)
            {
                var content = JsonConvert.SerializeObject(order, new JsonSerializerSettings() { ContractResolver = new CamelCasePropertyNamesContractResolver() });
                response.Content.ReadAsStringAsync().Result.Should().Be(content);
            }
        }
```
四、重构
可以看到这两个测试大部分的代码是相同的，都是用来发送请求。因此我们提取一个webApiTestBase类，该基类可以提供InvokeGetRequest,InvokePostRequest,InvokePutRequest等方法

C#
```
public abstract class ApiTestBase
    {
        public abstract string GetBaseAddress();
        protected TResult InvokeGetRequest(string api)
        {
            using (var invoker = CreateMessageInvoker())
            {
                using (var cts = new CancellationTokenSource())
                {
                    var request = new HttpRequestMessage(HttpMethod.Get, GetBaseAddress() + api);
                    using (HttpResponseMessage response = invoker.SendAsync(request, cts.Token).Result)
                    {
                        var result = response.Content.ReadAsStringAsync().Result;
                        return JsonConvert.DeserializeObject(result);
                    }
                }
            }
        }
        protected TResult InvokePostRequest(string api, TArguemnt arg)
        {
            var invoker = CreateMessageInvoker();
            using (var cts = new CancellationTokenSource())
            {
                var request = new HttpRequestMessage(HttpMethod.Post, GetBaseAddress() + api);
                request.Content = new ObjectContent(arg, new JsonMediaTypeFormatter());
                using (HttpResponseMessage response = invoker.SendAsync(request, cts.Token).Result)
                {
                    var result = response.Content.ReadAsStringAsync().Result;
                    return JsonConvert.DeserializeObject(result);
                }
            }
        }
        private HttpMessageInvoker CreateMessageInvoker()
        {
            var config = new HttpConfiguration();
            WebApiConfig.Register(config);
            var server = new HttpServer(config);
            var messageInvoker = new HttpMessageInvoker(server);
            return messageInvoker;
        }
    }
```
有了这个基类，我们写测试只需要重写方法GetBaseAddress()，然后直接调用基类方法并进行断言即可

C#
```
[TestFixture]
    public class OrderApiTests:ApiTestBase
    {
        public override string GetBaseAddress()
        {
            return "http://localhost:33203/";
        }
        [Test]
        public void Should_get_order_successfully()
        {
            var result = InvokeGetRequest("api/order");
            result.Name.Should().Be("name");
            result.Descriptions.Should().Be("descriptions");
            result.Id.Should().Be(1);
        }
        [Test]
        public void Should_post_order_successfully()
        {
            var newOrder=new Order(){Name = "newOrder",Id = 100,Descriptions = "new-order-description"};
            var result = InvokePostRequest("api/order", newOrder);
            result.Name.Should().Be("newOrder");
            result.Id.Should().Be(100);
            result.Descriptions.Should().Be("new-order-description");
        }
    }
```
是不是干净多了。
这种in-memory的测试方案有什么优点和缺点呢？
优点：
1.模拟真实调用，需要传入api地址即可得到结果，由于整个调用是in-memory的，所有效率很高，很适合集成测试。
2.整个测试时可以调试的，可以直接从单元测试调试进去，如果你写一个httpClient的测试，需要把webApi启动起来，然后。。。麻烦
缺点：我觉得原文作者说的那些缺点都可以忽略不计。
