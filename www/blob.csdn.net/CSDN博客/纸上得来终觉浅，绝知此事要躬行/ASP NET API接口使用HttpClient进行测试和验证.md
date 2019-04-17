# ASP.NET API接口使用HttpClient进行测试和验证 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年05月24日 14:14:26[boonya](https://me.csdn.net/boonya)阅读数：1140








ASP.NET API接口使用HttpClient进行测试和验证，以保证我们的接口的可用性。

参考Github开源项目：[https://github.com/MikeWasson/HttpClientSample](https://github.com/MikeWasson/HttpClientSample)

#### 编写一个Model实例

```
namespace HttpClientSample
{
    public class Product
    {
        public string Id { get; set; }
        public string Name { get; set; }
        public decimal Price { get; set; }
        public string Category { get; set; }
    }
}
```

#### 编写API Controller接口

```
using ProductApi.Models;
using System;
using System.Collections.Concurrent;
using System.Net;
using System.Web.Http;
using System.Web.Http.Results;

namespace ProductApi.Controllers
{
    [RoutePrefix("api/products")]
    public class ProductsController : ApiController
    {
        private static ConcurrentDictionary<string, Product> _products = new ConcurrentDictionary<string, Product>();

        [Route("{id}", Name = "GetById")]
        public IHttpActionResult Get(string id)
        {
            Product product = null;
            if (_products.TryGetValue(id, out product))
            {
                return Ok(product);
            }
            else
            {
                return NotFound();
            }
        }

        [HttpPost]
        [Route("")]
        public IHttpActionResult Post(Product product)
        {
            if (product == null)
            {
                return BadRequest("Product cannot be null");
            }
            if (!ModelState.IsValid)
            {
                return BadRequest(ModelState);
            }

            product.Id = Guid.NewGuid().ToString();
            _products[product.Id] = product;
            return CreatedAtRoute("GetById", new { id = product.Id }, product);
        }

        [HttpPut]
        [Route("{id}")]
        public IHttpActionResult Put(string id, Product product)
        {
            if (product == null)
            {
                return BadRequest("Product cannot be null");
            }
            if (!ModelState.IsValid)
            {
                return BadRequest(ModelState);
            }
            if (product.Id != id)
            {
                return BadRequest("product.id does not match id parameter");
            }
            
            if (!_products.Keys.Contains(id))
            {
                return NotFound();
            }

            _products[id] = product;
            return new StatusCodeResult(HttpStatusCode.NoContent, this);
        }

        [HttpDelete]
        [Route("{id}")]
        public IHttpActionResult Delete(string id)
        {
            Product product = null;
            _products.TryRemove(id, out product);
            return new StatusCodeResult(HttpStatusCode.NoContent, this);
        }
    }
}
```

#### 编写HttpClient测试

```
using System;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Threading.Tasks;

namespace HttpClientSample
{
    class Program
    {
        static HttpClient client = new HttpClient();

        static void ShowProduct(Product product)
        {
            Console.WriteLine($"Name: {product.Name}\tPrice: {product.Price}\tCategory: {product.Category}");
        }

        static async Task<Uri> CreateProductAsync(Product product)
        {
            HttpResponseMessage response = await client.PostAsJsonAsync("api/products", product);
            response.EnsureSuccessStatusCode();

            // return URI of the created resource.
            return response.Headers.Location;
        }

        static async Task<Product> GetProductAsync(string path)
        {
            Product product = null;
            HttpResponseMessage response = await client.GetAsync(path);
            if (response.IsSuccessStatusCode)
            {
                product = await response.Content.ReadAsAsync<Product>();
            }
            return product;
        }

        static async Task<Product> UpdateProductAsync(Product product)
        {
            HttpResponseMessage response = await client.PutAsJsonAsync($"api/products/{product.Id}", product);
            response.EnsureSuccessStatusCode();

            // Deserialize the updated product from the response body.
            product = await response.Content.ReadAsAsync<Product>();
            return product;
        }

        static async Task<HttpStatusCode> DeleteProductAsync(string id)
        {
            HttpResponseMessage response = await client.DeleteAsync($"api/products/{id}");
            return response.StatusCode;
        }

        static void Main()
        {
            RunAsync().Wait();
        }

        static async Task RunAsync()
        {
            client.BaseAddress = new Uri("http://localhost:55268/");
            client.DefaultRequestHeaders.Accept.Clear();
            client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));

            try
            {
                // Create a new product
                Product product = new Product { Name = "Gizmo", Price = 100, Category = "Widgets" };

                var url = await CreateProductAsync(product);
                Console.WriteLine($"Created at {url}");

                // Get the product
                product = await GetProductAsync(url.PathAndQuery);
                ShowProduct(product);

                // Update the product
                Console.WriteLine("Updating price...");
                product.Price = 80;
                await UpdateProductAsync(product);

                // Get the updated product
                product = await GetProductAsync(url.PathAndQuery);
                ShowProduct(product);

                // Delete the product
                var statusCode = await DeleteProductAsync(product.Id);
                Console.WriteLine($"Deleted (HTTP Status = {(int)statusCode})");

            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }

            Console.ReadLine();
        }

    }
}
```

#### 封装一个简单的HttpClientUtil工具类

```
using Newtonsoft.Json;
using Newtonsoft.Json.Converters;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Threading.Tasks;
using System.Web;
using System.Xml.Serialization;

namespace RTVSApi.Utils
{
    /// <summary>
    /// HttpClient工具类
    /// </summary>
    public class HttpClientUtil
    {
        /// <summary>  
        /// post请求  
        /// </summary>  
        /// <param name="url"></param>  
        /// <param name="postData">post数据</param>  
        /// <returns></returns>  
        public static string PostResponse(string url, string postData)
        {
            if (url.StartsWith("https"))
                System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;

            HttpContent httpContent = new StringContent(postData);
            httpContent.Headers.ContentType = new MediaTypeHeaderValue("application/json");
            HttpClient httpClient = new HttpClient();

            HttpResponseMessage response = httpClient.PostAsync(url, httpContent).Result;

            if (response.IsSuccessStatusCode)
            {
                string result = response.Content.ReadAsStringAsync().Result;
                return result;
            }
            return null;
        }

        /// <summary>  
        /// 发起post请求  
        /// </summary>  
        /// <typeparam name="T"></typeparam>  
        /// <param name="url">url</param>  
        /// <param name="postData">post数据</param>  
        /// <returns></returns>  
        public static string PostResponse<T>(string url, T postData) where T : class, new()
        {
            string result = "0";
            var format = new IsoDateTimeConverter();
            format.DateTimeFormat = "yyyyMMddHHmmssSSS";
            var dataJson = JsonConvert.SerializeObject(postData, Newtonsoft.Json.Formatting.Indented, format);
            var content = new StringContent(dataJson, Encoding.UTF8, "application/json");
            HttpClient httpClient = new HttpClient();
            HttpResponseMessage response = httpClient.PostAsync(url, content).Result;
            if (response.IsSuccessStatusCode)
            {
                //Task<string> t = response.Content.ReadAsStringAsync();  
                //string s = t.Result;  
                //T ss = JsonConvert.DeserializeObject<T>(s);  
                result = "1";
            }
            return result;
        }

        /// <summary>  
        /// 发起post请求主键返回  
        /// </summary>  
        /// <typeparam name="T"></typeparam>  
        /// <param name="url">url</param>  
        /// <param name="postData">post数据</param>  
        /// <returns>主键</returns>  
        public static string PostResponseKey<T>(string url, T postData) where T : class, new()
        {
            string ret = "0";
            var format = new IsoDateTimeConverter();
            format.DateTimeFormat = "yyyyMMddHHmmssSSS";
            var dataJson = JsonConvert.SerializeObject(postData, Newtonsoft.Json.Formatting.Indented, format);
            var content = new StringContent(dataJson, Encoding.UTF8, "application/json");
            HttpClient httpClient = new HttpClient();
            HttpResponseMessage response = httpClient.PostAsync(url, content).Result;
            if (response.IsSuccessStatusCode)
            {
                Task<string> t = response.Content.ReadAsStringAsync();
                ret = t.Result;
            }
            return ret;
        }


        /// <summary>  
        /// 发起post请求  
        /// </summary>  
        /// <typeparam name="T"></typeparam>  
        /// <param name="url">url</param>  
        /// <param name="postData">post数据</param>  
        /// <returns></returns>  
        public static T PostResponse<T>(string url, string postData) where T : class, new()
        {
            if (url.StartsWith("https"))
                System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;
            HttpContent httpContent = new StringContent(postData);
            httpContent.Headers.ContentType = new MediaTypeHeaderValue("application/json");
            HttpClient httpClient = new HttpClient();

            T result = default(T);

            HttpResponseMessage response = httpClient.PostAsync(url, httpContent).Result;

            if (response.IsSuccessStatusCode)
            {
                Task<string> t = response.Content.ReadAsStringAsync();
                string s = t.Result;

                result = JsonConvert.DeserializeObject<T>(s);
            }
            return result;
        }

        /// <summary>  
        /// V3接口全部为Xml形式，故有此方法  
        /// </summary>  
        /// <typeparam name="T"></typeparam>  
        /// <param name="url"></param>  
        /// <param name="xmlString"></param>  
        /// <returns></returns>  
        public static T PostXmlResponse<T>(string url, string xmlString) where T : class, new()
        {
            if (url.StartsWith("https"))
                System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;

            HttpContent httpContent = new StringContent(xmlString);
            httpContent.Headers.ContentType = new MediaTypeHeaderValue("application/json");
            HttpClient httpClient = new HttpClient();

            T result = default(T);

            HttpResponseMessage response = httpClient.PostAsync(url, httpContent).Result;

            if (response.IsSuccessStatusCode)
            {
                Task<string> t = response.Content.ReadAsStringAsync();
                string s = t.Result;

                result = XmlDeserialize<T>(s);
            }
            return result;
        }

        /// <summary>  
        /// 反序列化Xml  
        /// </summary>  
        /// <typeparam name="T"></typeparam>  
        /// <param name="xmlString"></param>  
        /// <returns></returns>  
        public static T XmlDeserialize<T>(string xmlString) where T : class, new()
        {
            try
            {
                XmlSerializer ser = new XmlSerializer(typeof(T));
                using (StringReader reader = new StringReader(xmlString))
                {
                    return (T)ser.Deserialize(reader);
                }
            }
            catch (Exception ex)
            {
                throw new Exception("XmlDeserialize发生异常：xmlString:" + xmlString + "异常信息：" + ex.Message);
            }

        }


        /// <summary>  
        /// get请求  
        /// </summary>  
        /// <param name="url"></param>  
        /// <returns></returns>  
        public static string GetResponse(string url)
        {
            if (url.StartsWith("https"))
                System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;

            HttpClient httpClient = new HttpClient();
            httpClient.DefaultRequestHeaders.Accept.Add(
               new MediaTypeWithQualityHeaderValue("application/json"));
            HttpResponseMessage response = httpClient.GetAsync(url).Result;

            if (response.IsSuccessStatusCode)
            {
                string result = response.Content.ReadAsStringAsync().Result;
                return result;
            }
            return null;
        }

        /// <summary>  
        /// get请求  
        /// </summary>  
        /// <typeparam name="T"></typeparam>  
        /// <param name="url"></param>  
        /// <returns></returns>  
        public static T GetResponse<T>(string url) where T : class, new()
        {
            if (url.StartsWith("https"))
                System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;
            HttpClient httpClient = new HttpClient();
            httpClient.Timeout = TimeSpan.FromMilliseconds(3000);
            httpClient.DefaultRequestHeaders.Accept.Add(
               new MediaTypeWithQualityHeaderValue("application/json"));
            HttpResponseMessage response = httpClient.GetAsync(url).Result;

            T result = default(T);

            if (response.IsSuccessStatusCode)
            {
                Task<string> t = response.Content.ReadAsStringAsync();
                string s = t.Result;

                result = JsonConvert.DeserializeObject<T>(s);
            }
            return result;
        }

        /// <summary>  
        /// Get请求返回List集合  
        /// </summary>  
        /// <typeparam name="T"></typeparam>  
        /// <param name="url"></param>  
        /// <returns>List<T></returns>  
        public static List<T> GetResponseList<T>(string url) where T : class, new()
        {
            if (url.StartsWith("https"))
            {
                System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;
            }
            HttpClient httpClient = new HttpClient();
            httpClient.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
            HttpResponseMessage response = httpClient.GetAsync(url).Result;

            List<T> result = default(List<T>);

            if (response.IsSuccessStatusCode)
            {
                Task<string> t = response.Content.ReadAsStringAsync();
                string s = t.Result;
                if (s != null && !s.StartsWith("["))
                {
                    s = "[" + s + "]";
                }
                result = JsonConvert.DeserializeObject<List<T>>(s);
            }
            return result;
        }


        /// <summary>  
        /// Delete请求  
        /// </summary>  
        /// <param name="url"></param>  
        /// <param name="postData">post数据</param>  
        /// <returns></returns>  
        public static string DeleteResponse(string url)
        {
            if (url.StartsWith("https"))
                System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;
            HttpClient httpClient = new HttpClient();
            HttpResponseMessage response = httpClient.DeleteAsync(url).Result;
            if (response.IsSuccessStatusCode)
            {
                string result = response.Content.ReadAsStringAsync().Result;
                return "1";
            }
            return "0";
        }

        /// <summary>  
        /// 发起put请求 List格式数据  
        /// </summary>  
        /// <typeparam name="T"></typeparam>  
        /// <param name="url">url</param>  
        /// <param name="postData">put数据</param>  
        /// <returns></returns>  
        public static string PutListDataResponse<T>(string url, List<T> postData) where T : class, new()
        {
            string result = "0";
            if (postData != null && postData.Count > 0)
            {
                if (url.StartsWith("https"))
                    System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;
                HttpClient httpClient = new HttpClient();
                var format = new IsoDateTimeConverter();
                format.DateTimeFormat = "yyyyMMddHHmmssSSS";
                var dataJson = JsonConvert.SerializeObject(postData[0], Newtonsoft.Json.Formatting.Indented, format);
                var content = new StringContent(dataJson, Encoding.UTF8, "application/json");

                HttpResponseMessage response = httpClient.PutAsync(url, content).Result;
                if (response.IsSuccessStatusCode)
                {
                    result = "1";
                }
            }
            else
            {
                HttpClient httpClient = new HttpClient();
                HttpResponseMessage response = httpClient.PutAsync(url, null).Result;
                if (response.IsSuccessStatusCode)
                {
                    result = "1";
                }
            }

            return result;
        }

        /// <summary>  
        /// 发起put请求 (修改时候用)  
        /// </summary>  
        /// <typeparam name="T"></typeparam>  
        /// <param name="url">url</param>  
        /// <param name="postData">put数据</param>  
        /// <returns></returns>  
        public static string PutDataResponse<T>(string url, T postData) where T : class, new()
        {
            string result = "0";
            if (postData != null)
            {
                if (url.StartsWith("https"))
                    System.Net.ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls;
                HttpClient httpClient = new HttpClient();
                var format = new IsoDateTimeConverter();
                format.DateTimeFormat = "yyyyMMddHHmmssSSS";
                var dataJson = JsonConvert.SerializeObject(postData, Newtonsoft.Json.Formatting.Indented, format);
                var content = new StringContent(dataJson, Encoding.UTF8, "application/json");
                HttpResponseMessage response = httpClient.PutAsync(url, content).Result;
                if (response.IsSuccessStatusCode)
                {
                    result = "1";
                }
            }
            else
            {
                HttpClient httpClient = new HttpClient();
                HttpResponseMessage response = httpClient.PutAsync(url, null).Result;
                if (response.IsSuccessStatusCode)
                {
                    result = "1";
                }
            }
            return result;
        }
    }
}
```











