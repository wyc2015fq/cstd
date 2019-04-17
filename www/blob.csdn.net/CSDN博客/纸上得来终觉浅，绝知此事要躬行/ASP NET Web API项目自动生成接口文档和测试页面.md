# ASP.NET Web API项目自动生成接口文档和测试页面 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年05月16日 09:49:03[boonya](https://me.csdn.net/boonya)阅读数：2686








在开发接口的时候，写接口文档已是一件不可忽视的事情，有了更新也要同步更新很麻烦。ASP.NET 创建的Web API项目可以自己配置接口文档的XML显示，这样接口更新和注释更新了重新发布就有了，确实方便不少，下来就介绍下怎么配置生成API接口注释文档。另外，如果在接口生成的同时能够一并生成测试页面也是不错的选择，能节省不少开发时间和人力成本。

#### 创建Web API项目

请参考：[ASP.NET 使用Swagger开发Web API接口项目](https://blog.csdn.net/boonya/article/details/80321229)这里我们以WebAPI项目为例。

![](https://img-blog.csdn.net/2018051609273982?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

#### 修改默认的API路由配置

为什么要修改呢？因为每一个Controller的默认路由都是指定到"api/{controller}/{id}"的，所以默认生成的控制器类似下面：

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;

namespace WebAPI.Controllers
{
    public class ValuesController : ApiController
    {
        // GET api/<controller>
        public IEnumerable<string> Get()
        {
            return new string[] { "value1", "value2" };
        }

        // GET api/<controller>/5
        public string Get(int id)
        {
            return "value";
        }

        // POST api/<controller>
        public void Post([FromBody]string value)
        {
        }

        // PUT api/<controller>/5
        public void Put(int id, [FromBody]string value)
        {
        }

        // DELETE api/<controller>/5
        public void Delete(int id)
        {
        }
    }
}
```

我们需要一个方法提供多种HTTP访问方式，所以应该将action的配置加入，如："api/{controller}/{action}/{id}"。

修改WebApiConfig.cs文件：

![](https://img-blog.csdn.net/20180516093540157?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


代码如下：

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web.Http;

namespace WebAPI
{
    public static class WebApiConfig
    {
        public static void Register(HttpConfiguration config)
        {
            // Web API 配置和服务

            // Web API 路由
            config.MapHttpAttributeRoutes();

            config.Routes.MapHttpRoute(
                name: "DefaultApi",
                routeTemplate: "api/{controller}/{action}/{id}",
                defaults: new { id = RouteParameter.Optional }
            );
        }
    }
}
```

#### 修改HelpPage配置启动

![](https://img-blog.csdn.net/20180516093651447?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改HelpPageConfig.cs文件，代码如下：

```
// Uncomment the following to provide samples for PageResult<T>. Must also add the Microsoft.AspNet.WebApi.OData
// package to your project.
////#define Handle_PageResultOfT

using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using System.Net.Http.Headers;
using System.Reflection;
using System.Web;
using System.Web.Http;
using WebAPI.Utils;

namespace WebAPI.Areas.HelpPage
{
    /// <summary>
    /// Use this class to customize the Help Page.
    /// For example you can set a custom <see cref="System.Web.Http.Description.IDocumentationProvider"/> to supply the documentation
    /// or you can provide the samples for the requests/responses.
    /// </summary>
    public static class HelpPageConfig
    {
        [SuppressMessage("Microsoft.Globalization", "CA1303:Do not pass literals as localized parameters",
            MessageId = "WebAPI.Areas.HelpPage.TextSample.#ctor(System.String)",
            Justification = "End users may choose to merge this string with existing localized resources.")]
        [SuppressMessage("Microsoft.Naming", "CA2204:Literals should be spelled correctly",
            MessageId = "bsonspec",
            Justification = "Part of a URI.")]
        public static void Register(HttpConfiguration config)
        {
            //// Uncomment the following to use the documentation from XML documentation file.
            //config.SetDocumentationProvider(new XmlDocumentationProvider(HttpContext.Current.Server.MapPath("~/App_Data/XmlDocument.xml")));
            #region *************************XML显示接口参数和注释***************************************
            // 添加本项目或其他引用项目的XML配置:可加多个
            config.SetDocumentationProvider(new MultiXmlDocumentationProvider(HttpContext.Current.Server.MapPath("~/bin/WebAPI.XML")));
            #endregion
            //// Uncomment the following to use "sample string" as the sample for all actions that have string as the body parameter or return type.
            //// Also, the string arrays will be used for IEnumerable<string>. The sample objects will be serialized into different media type 
            //// formats by the available formatters.
            //config.SetSampleObjects(new Dictionary<Type, object>
            //{
            //    {typeof(string), "sample string"},
            //    {typeof(IEnumerable<string>), new string[]{"sample 1", "sample 2"}}
            //});

            // Extend the following to provide factories for types not handled automatically (those lacking parameterless
            // constructors) or for which you prefer to use non-default property values. Line below provides a fallback
            // since automatic handling will fail and GeneratePageResult handles only a single type.
#if Handle_PageResultOfT
            config.GetHelpPageSampleGenerator().SampleObjectFactories.Add(GeneratePageResult);
#endif

            // Extend the following to use a preset object directly as the sample for all actions that support a media
            // type, regardless of the body parameter or return type. The lines below avoid display of binary content.
            // The BsonMediaTypeFormatter (if available) is not used to serialize the TextSample object.

            #region *************************默认JSON显示接口***************************************
            //config.SetSampleForMediaType(
            //    new TextSample("Binary JSON content. See http://bsonspec.org for details."),
            //    new MediaTypeHeaderValue("application/bson"));
            #endregion

            //// Uncomment the following to use "[0]=foo&[1]=bar" directly as the sample for all actions that support form URL encoded format
            //// and have IEnumerable<string> as the body parameter or return type.
            //config.SetSampleForType("[0]=foo&[1]=bar", new MediaTypeHeaderValue("application/x-www-form-urlencoded"), typeof(IEnumerable<string>));

            //// Uncomment the following to use "1234" directly as the request sample for media type "text/plain" on the controller named "Values"
            //// and action named "Put".
            //config.SetSampleRequest("1234", new MediaTypeHeaderValue("text/plain"), "Values", "Put");

            //// Uncomment the following to use the image on "../images/aspNetHome.png" directly as the response sample for media type "image/png"
            //// on the controller named "Values" and action named "Get" with parameter "id".
            //config.SetSampleResponse(new ImageSample("../images/aspNetHome.png"), new MediaTypeHeaderValue("image/png"), "Values", "Get", "id");

            //// Uncomment the following to correct the sample request when the action expects an HttpRequestMessage with ObjectContent<string>.
            //// The sample will be generated as if the controller named "Values" and action named "Get" were having string as the body parameter.
            //config.SetActualRequestType(typeof(string), "Values", "Get");

            //// Uncomment the following to correct the sample response when the action returns an HttpResponseMessage with ObjectContent<string>.
            //// The sample will be generated as if the controller named "Values" and action named "Post" were returning a string.
            //config.SetActualResponseType(typeof(string), "Values", "Post");
        }

#if Handle_PageResultOfT
        private static object GeneratePageResult(HelpPageSampleGenerator sampleGenerator, Type type)
        {
            if (type.IsGenericType)
            {
                Type openGenericType = type.GetGenericTypeDefinition();
                if (openGenericType == typeof(PageResult<>))
                {
                    // Get the T in PageResult<T>
                    Type[] typeParameters = type.GetGenericArguments();
                    Debug.Assert(typeParameters.Length == 1);

                    // Create an enumeration to pass as the first parameter to the PageResult<T> constuctor
                    Type itemsType = typeof(List<>).MakeGenericType(typeParameters);
                    object items = sampleGenerator.GetSampleObject(itemsType);

                    // Fill in the other information needed to invoke the PageResult<T> constuctor
                    Type[] parameterTypes = new Type[] { itemsType, typeof(Uri), typeof(long?), };
                    object[] parameters = new object[] { items, null, (long)ObjectGenerator.DefaultCollectionSize, };

                    // Call PageResult(IEnumerable<T> items, Uri nextPageLink, long? count) constructor
                    ConstructorInfo constructor = type.GetConstructor(parameterTypes);
                    return constructor.Invoke(parameters);
                }
            }

            return null;
        }
#endif
    }
}
```

这里修改支持了多个XML文档方法，扩展类如下：

```
using System;
using System.Linq;
using System.Reflection;
using System.Web.Http.Controllers;
using System.Web.Http.Description;
using WebAPI.Areas.HelpPage;
using WebAPI.Areas.HelpPage.ModelDescriptions;

namespace WebAPI.Utils
{
    /// <summary>A custom <see cref='IDocumentationProvider'/> that reads the API documentation from a collection of XML documentation files.</summary>
    public class MultiXmlDocumentationProvider : IDocumentationProvider, IModelDocumentationProvider
    {
        /*********
        ** Properties
        *********/
        /// <summary>The internal documentation providers for specific files.</summary>
        private readonly XmlDocumentationProvider[] Providers;


        /*********
        ** Public methods
        *********/
        /// <summary>Construct an instance.</summary>
        /// <param name='paths'>The physical paths to the XML documents.</param>
        public MultiXmlDocumentationProvider(params string[] paths)
        {
            this.Providers = paths.Select(p => new XmlDocumentationProvider(p)).ToArray();
        }

        /// <summary>Gets the documentation for a subject.</summary>
        /// <param name='subject'>The subject to document.</param>
        public string GetDocumentation(MemberInfo subject)
        {
            return this.GetFirstMatch(p => p.GetDocumentation(subject));
        }

        /// <summary>Gets the documentation for a subject.</summary>
        /// <param name='subject'>The subject to document.</param>
        public string GetDocumentation(Type subject)
        {
            return this.GetFirstMatch(p => p.GetDocumentation(subject));
        }

        /// <summary>Gets the documentation for a subject.</summary>
        /// <param name='subject'>The subject to document.</param>
        public string GetDocumentation(HttpControllerDescriptor subject)
        {
            return this.GetFirstMatch(p => p.GetDocumentation(subject));
        }

        /// <summary>Gets the documentation for a subject.</summary>
        /// <param name='subject'>The subject to document.</param>
        public string GetDocumentation(HttpActionDescriptor subject)
        {
            return this.GetFirstMatch(p => p.GetDocumentation(subject));
        }

        /// <summary>Gets the documentation for a subject.</summary>
        /// <param name='subject'>The subject to document.</param>
        public string GetDocumentation(HttpParameterDescriptor subject)
        {
            return this.GetFirstMatch(p => p.GetDocumentation(subject));
        }

        /// <summary>Gets the documentation for a subject.</summary>
        /// <param name='subject'>The subject to document.</param>
        public string GetResponseDocumentation(HttpActionDescriptor subject)
        {
            return this.GetFirstMatch(p => p.GetDocumentation(subject));
        }


        /*********
        ** Private methods
        *********/
        /// <summary>Get the first valid result from the collection of XML documentation providers.</summary>
        /// <param name='expr'>The method to invoke.</param>
        private string GetFirstMatch(Func<XmlDocumentationProvider, string> expr)
        {
            return this.Providers
                .Select(expr)
                .FirstOrDefault(p => !String.IsNullOrWhiteSpace(p));
        }
    }
}
```

注意：开启XML文档注释的时候HelpPageConfig.cs需要注释掉如下代码

```
#region *************************默认JSON显示接口***************************************
            //config.SetSampleForMediaType(
            //    new TextSample("Binary JSON content. See http://bsonspec.org for details."),
            //    new MediaTypeHeaderValue("application/bson"));
            #endregion
```

#### 编写示例程序验证API文档更新情况

PersonController.cs示例：

```
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Web.Http;
using WebAPI.Utils;

namespace WebAPI.Controllers
{
    /// <summary>
    /// 人物
    /// </summary>
    public class Person
    {
        /// <summary>
        /// 唯一标识记录ID
        /// </summary>
        public long Id { set; get; }
        /// <summary>
        /// 姓名
        /// </summary>
        public string Name { set; get; }
        /// <summary>
        /// 年龄
        /// </summary>
        public int Age { set; get; }
        /// <summary>
        /// 人物构造函数
        /// </summary>
        /// <param name="Id"></param>
        /// <param name="Name"></param>
        /// <param name="Age"></param>
        public Person(long Id, string Name, int Age)
        {
            this.Id = Id;
            this.Name = Name;
            this.Age = Age;
        }
    }

    /// <summary>
    /// 结果对象
    /// </summary>
    public class Result
    {
        /// <summary>
        /// 状态：0失败，1成功
        /// </summary>
        public int Status { set; get; }
        /// <summary>
        /// 数据
        /// </summary>
        public string Data { set; get; }
        /// <summary>
        /// 消息
        /// </summary>
        public string Msg { set; get; }
        /// <summary>
        /// 结果构造函数
        /// </summary>
        /// <param name="Status"></param>
        /// <param name="Data"></param>
        /// <param name="Msg"></param>
        public Result(int Status, string Data, string Msg)
        {
            this.Status = Status;
            this.Data = Data;
            this.Msg = Msg;
        }
    }
    /// <summary>
    /// 人物视图控制器
    /// </summary>
    public class PersonController : ApiController
    {
        /// <summary>
        /// 获取所有人物
        /// </summary>
        /// <returns></returns>
        public List<Person> GetPersons()
        {
            List<Person> perons = new List<Person>();
            Person person = null;
            for (var i=1;i<=10;i++)
            {
                person = new Person(i,"boonya"+1,new Random(100).Next());
                perons.Add(person);
            }
            return perons;
        }

        /// <summary>
        /// 获取人物
        /// </summary>
        /// <param name="Id">唯一标识记录ID</param>
        /// <returns></returns>
        public Person GetPerson(long Id)
        {
            return new Person(Id, "boonya" + Id, new Random(100).Next());
        }

        /// <summary>
        /// 删除人物
        /// </summary>
        /// <param name="Id">唯一标识记录ID</param>
        /// <returns></returns>
        public Result DeletePerson(long Id)
        {
            return new Result(1,"","Id="+Id+"删除成功");
        }

        /// <summary>
        /// 添加人物
        /// </summary>
        /// <param name="Name">姓名</param>
        /// <param name="Age">年龄</param>
        /// <returns></returns>
        public Result AddPerson(string Name,int Age)
        {
            Person person = new Person(new Random(100).Next(), Name, Age);
            return new Result(1, JsonConvert.SerializeObject(person), "添加成功");
        }
    }
}
```
启动项目运行，查看API链接：

![](https://img-blog.csdn.net/20180516094631472)

详细接口说明类似下面：

![](https://img-blog.csdn.net/20180516094702144)

至此，我们看到接口的参数说明和返回值等，不用手动再去写文档了。

注意：参考博客内容里面有怎么生成项目的XML文件。

#### 配置自动生成接口测试页面

添加引用NuGet安装包：

![](https://img-blog.csdn.net/20180516175532401?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

安装完成后运行就可以了：

![](https://img-blog.csdn.net/20180516175823946?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击“Test API”按钮就可以测试了：

![](https://img-blog.csdn.net/20180516175924648?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

使用此安装包不需要修改如下文件：

![](https://img-blog.csdn.net/20180516180020471?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

示例程序包下载：[https://download.csdn.net/download/boonya/10418822](https://download.csdn.net/download/boonya/10418822)




