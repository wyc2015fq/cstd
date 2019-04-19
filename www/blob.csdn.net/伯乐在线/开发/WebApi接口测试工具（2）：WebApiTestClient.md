# WebApi接口测试工具（2）：WebApiTestClient - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/5210356.html)
前言：这两天在整WebApi的服务，由于调用方是Android客户端，Android开发人员也不懂C#语法，API里面的接口也不能直接给他们看，没办法，只有整个详细一点的文档呗。由于接口个数有点多，每个接口都要详细说明接口作用、参数类型、返回值类型等等，写着写着把博主惹毛了，难道这种文档非要自己写不成？难道网上没有这种文档的展示工具吗？带着这两个问题，在网络世界里寻找，网络世界很奇妙，只要你用心，总能找到或多或少的帮助！这不就被博主找到了这个好用的组件：WebApiTestClient。它对于接口的作用、参数作用以及类型、返回值说明等等都能很好展现，并且提供测试调用的功能，输入测试参数发送请求，就能得到相应的返回结果。哇塞，这一系列可视化的效果不正是博主需要的吗，还写什么文档，赶紧用起来！
# 一、WebApiTestClient介绍
1、WebApiTestClient组件作用主要有以下几个：
（1）、将WebApi的接口放到了浏览器里面，以可视化的方式展现出来，比如我们通过http://localhost:8080/Help这个地址就能在浏览器里面看到这个服务里面所有的API接口以及接口的详细说明，省去了我们手写接口文档的麻烦。
（2）、能够详细查看API的类说明、方法说明、参数说明、返回值说明。只需要我们在定义方法时候加上 ///
这种详细注释即可，组件自动读取注释里面的内容展现在界面上面。
（3）、可以修改http请求头文件Head和请求体Body里面的参数，指定发送http请求的特性，比如指定我们最常见的contentType指示参数的类型。
（4）、组件拥有测试接口的功能，用过Soup UI的朋友应该知道，通过Soup UI能够方便测试WebService参数以及返回值。我们的WebApiTestClient也可以实现类似的功能，直接通过页面上的测试按钮，就能测试接口。
2、WebApiTestClient是一个开源组件。开源地址：[https://github.com/yaohuang/WebApiTestClient](https://github.com/yaohuang/WebApiTestClient)
# 二、WebApiTestClient展示
第一印象：接口列表。 ![](http://jbcdn2.b0.upaiyun.com/2016/04/e49b49630351bad4b2d03d69b4a65aab.png)
点击某一个接口查看接口详细。例如本文查看Get请求的无参方法，右下角有按钮可以测试接口。
![](http://jbcdn2.b0.upaiyun.com/2016/04/4ca94a110cdc1c2a6773bd61f9ef0f95.png)
点击“Test API”按钮
![](http://jbcdn2.b0.upaiyun.com/2016/04/09ae6182ef07d44c0e602e40b7f5beef.png)
点击Send发送请求
![](http://jbcdn2.b0.upaiyun.com/2016/04/a5eb14b25cd0f3b4456822589e4feee8.png)
第二个有参数的接口
![](http://jbcdn2.b0.upaiyun.com/2016/04/5cc772596666c894f85dc98306329bac.png)
手动输入参数，得到返回结果
![](http://jbcdn2.b0.upaiyun.com/2016/04/f8cde102c2410771b62076a09b11bfa1.png)
如果参数的类型是对象，可以直接解析class定义上面的 ///
标注，显示如下
![](http://jbcdn2.b0.upaiyun.com/2016/04/c6182043a38c044596ad73bc8803e4bc.png)
由于是post请求，如果需要执行参数长度和类型，可以通过content-length和content-type来指定。并且具体的参数可以指定不同格式显示，比如下图的application/json和application/xml
![](http://jbcdn2.b0.upaiyun.com/2016/04/1c9354b7021c91e0ed38f514ee064f73.png)
![](http://jbcdn2.b0.upaiyun.com/2016/04/53df10c1822616b14bf06ddedfa4db9b.png)
得到返回值
![](http://jbcdn2.b0.upaiyun.com/2016/04/7622ce9a773f4b4c56837471f75afb68.png)
# 三、WebApiTestClient使用
## 1、如何引入组件
首先，我们需要定义一个API项目
![](http://jbcdn2.b0.upaiyun.com/2016/04/468481081319c9ab530607c68db8946f.png)
然后通过Nuget引入组件，如下图。记住选下图中的第三个。
![](http://jbcdn2.b0.upaiyun.com/2016/04/768b70965ab8bd4f99923561765841ce.png)
引入成功后，将向项目里面添加一些主要文件：
- ScriptsWebApiTestClient.js
- AreasHelpPageTestClient.css
- AreasHelpPageViewsHelpDisplayTemplatesTestClientDialogs.cshtml
- AreasHelpPageViewsHelpDisplayTemplatesTestClientReferences.cshtml
## 2、如何使用组件
###  1、修改Api.cshtml文件
通过上述步骤，就能将组件WebAPITestClient引入进来。下面我们只需要做一件事：打开文件 (根据 AreasHelpPageViewsHelp) Api.cshtml 并添加以下内容:
- @Html.DisplayForModel(“TestClientDialogs”)
- @Html.DisplayForModel(“TestClientReferences”)
添加后Api.cshtml文件的代码如下

XHTML
```
@using System.Web.Http
@using WebApiTestClient.Areas.HelpPage.Models
@model HelpPageApiModel
@{
    var description = Model.ApiDescription;
    ViewBag.Title = description.HttpMethod.Method + " " + description.RelativePath;
}
<link type="text/css" href="~/Areas/HelpPage/HelpPage.css" rel="stylesheet" />
<div id="body" class="help-page">
    <section class="featured">
        <div class="content-wrapper">
            <p>
                @Html.ActionLink("Help Page Home", "Index")
            </p>
        </div>
    </section>
    <section class="content-wrapper main-content clear-fix">
        @Html.DisplayForModel()
    </section>
</div>
@Html.DisplayForModel("TestClientDialogs")
@section Scripts{
    <link href="~/Areas/HelpPage/HelpPage.css" rel="stylesheet" />
    @Html.DisplayForModel("TestClientReferences")
}
```
### 2、配置读取注释的xml路径
其实，通过上面的步骤，我们的项目已经可以跑起来了，也可以调用接口测试。但是，还不能读取 ///
注释里面的东西。需要做如下配置才行。
#### （1）配置生成xml的路径。我们在项目上面点右键→属性→生成标签页配置xml的路径
![](http://jbcdn2.b0.upaiyun.com/2016/04/1e477d31808f9deb7cce4bd2912d1fd6.png)
#### （2）在xml的读取路径：在下图的HelpPageConfig.cs里面配置一句话，指定xml的读取路径。
![](http://jbcdn2.b0.upaiyun.com/2016/04/7922639e9c822c4b75c6d2217e1b7cb8.png)
这句代码如下：


```
config.SetDocumentationProvider(new XmlDocumentationProvider(HttpContext.Current.Server.MapPath("~/App_Data/WebApiTestClient.XML")));
```
### 3、测试接口

C#
```
/// 
    /// 测试API Test Client
    /// 
    public class TestChargingDataController : ApiController
    {
        /// 
        /// 得到所有数据
        /// 
        /// 返回数据
        [HttpGet]
        public string GetAllChargingData()
        {
            return "ChargingData";
        }
        /// 
        /// 得到当前Id的所有数据
        /// 
        /// 参数Id
        /// 返回数据
        [HttpGet]
        public string GetAllChargingData(string id)
        {
            return "ChargingData" + id ;
        }
        /// 
        /// Post提交
        /// 
        /// 对象
        /// 提交是否成功
        [HttpPost]
        public bool Post(TB_CHARGING oData)
        {
            return true;
        }
        /// 
        /// Put请求
        /// 
        /// 对象
        /// 提交是否成功
        [HttpPut]
        public bool Put(TB_CHARGING oData)
        {
            return true;
        }
        /// 
        /// delete操作
        /// 
        /// 对象id
        /// 操作是否成功
        [HttpDelete]
        public bool Delete(string id)
        {
            return true;
        }
    }
    /// 
    /// 充电对象实体
    /// 
    public class TB_CHARGING
    {
        /// 
        /// 主键Id
        /// 
        public string ID { get; set; }
        /// 
        /// 充电设备名称
        /// 
        public string NAME { get; set; }
        /// 
        /// 充电设备描述
        /// 
        public string DES { get; set; }
        /// 
        /// 创建时间
        /// 
        public DateTime CREATETIME { get; set; }
    }
```
至此，组件就搭完了，剩下的就是运行了。我们在url里面敲地址http://localhost:8080/Help/Index或者http://localhost:8080/Help就能得到上述效果。还不赶紧试试~~
# 四、总结
上面简单总结了下WebApiTestClient的使用，如果你的WebApi也是一个单独的服务，可以整起来当帮助文档用，省得你自己写。哈哈，不要谢我，请叫我红领巾。
WebAPI系列文章：
- [《WebApi身份认证解决方案（1）：Basic基础认证》](http://blog.jobbole.com/99816/)
- [《WebApi 跨域问题解决方案（3）：CORS》](http://blog.jobbole.com/99855/)
- [《WebApi接口传参不再困惑（4）：传参详解》](http://blog.jobbole.com/99854/)
- [《WebApi异常处理解决方案（5）》](http://blog.jobbole.com/99858/)
