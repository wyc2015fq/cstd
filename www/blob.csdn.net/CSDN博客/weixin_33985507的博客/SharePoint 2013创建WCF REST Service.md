# SharePoint 2013创建WCF REST Service - weixin_33985507的博客 - CSDN博客
2013年12月19日 21:50:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
> 
**SharePoint 2013为****开发者提供了丰富的REST API，方便了我们在客户端操作List中的数据。当然我们也可以在SharePoint 2013中创建自定义的REST Service，比如通过REST Service去操作数据库。本篇博客将介绍怎样在SharePoint 2013创建WCF REST Service。**
### SharePoint 中 创建WCF Service
因为无法在SharePoint 2013 Project中添加WCF Service Template，所以预先创建一个WCF Service Application ， 在把契约接口和svc服务拖到SharePoint Project中。所以你需要以下步骤：
- **1.创建 WCF Service Application**
- **2.在SharePoint Project中创建SharePoint Mapped Folder ISAPI**，因为SharePoint 2013中能访问的服务（.svc）存在：C:\Program Files\Common Files\Microsoft Shared\Web Server Extensions\15\isapi 文件夹中，对应于IIS中虚拟目录_vti_bin。
- **3.把WCF Service Application的svc拖到 ISAPI文件夹中，如下所示：**
![](https://images0.cnblogs.com/blog/299214/201312/19181750-f66022788fce4f898cdb7356d1dcf55a.png)
- **4.修改Namespace，并添加程序集引用，如下所示：**
![](https://images0.cnblogs.com/blog/299214/201312/19181750-f02fe3100e33438e85cd2f2bc3d66693.png)
- **5.因为SharePoint Project需要强名称Key File，所以引用此程序集需要提供PublicToken(sn -T assembly.dll**，使用方法参考：[http://www.cnblogs.com/OceanEyes/p/custom-provider-in-sharepoint-2013-fba-authentication.html](http://www.cnblogs.com/OceanEyes/p/custom-provider-in-sharepoint-2013-fba-authentication.html))
![](https://images0.cnblogs.com/blog/299214/201312/19181750-c5186d30efed4d40ab8ec9cee04b0e03.png)
- 6.修改svc
```
<%@ ServiceHost Language="C#" Debug="true" Service="Eyes.CustomRestService.Service1,Eyes.CustomRestService,Version=1.0.0.0,Culture=neutral,PublicKeyToken= bf65dbaa17f24124" CodeBehind="Service1.svc.cs" %>
```
- **7.为了测试WCF Service是否成功部署，需要实现契约接口：**
 创建用于测试的契约接口：
```
[ServiceContract]
    public interface IService1
    {
        [OperationContract]
        [WebGet(ResponseFormat=WebMessageFormat.Json,BodyStyle=WebMessageBodyStyle.Wrapped,UriTemplate="GetData/{value}")]
        string GetData(string value);
        // TODO: Add your service operations here
    }
```
 接着实现契约接口，也就是我们的服务：
```
[AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
    public class Service1 : IService1
    {
        public string GetData(string value)
        {
            return string.Format("You entered: {0}", value);
        }
    }
```
- **8.最后，修改Config文件**
```
<system.serviceModel>
       <serviceHostingEnvironment aspNetCompatibilityEnabled="true" multipleSiteBindingsEnabled="true" />
       <behaviors>
        <serviceBehaviors>
            <behavior name="Service1ServiceBehavior">
                <serviceMetadata httpGetEnabled="true" httpsGetEnabled="true" />
                <serviceDebug includeExceptionDetailInFaults="true" />
            </behavior>
        </serviceBehaviors>
        <endpointBehaviors>
            <behavior name="jsonBehavior">
                <webHttp />
            </behavior>
        </endpointBehaviors>
        </behaviors>
        <services>
            <service name="Eyes.CustomRestService.Service1" behaviorConfiguration="Service1ServiceBehavior">
                <endpoint address="" binding="webHttpBinding" behaviorConfiguration="jsonBehavior" contract="Eyes.CustomRestService.IService1" />
                <endpoint address="mex" binding="mexHttpBinding" contract="IMetadataExchange" />
            </service>
        </services>
  </system.serviceModel>
```
- **9.客户端访问REST Service**
![](https://images0.cnblogs.com/blog/299214/201312/19182431-1d373d51257a46d6a66815c7ca494df7.jpg)
### 小结
> 
SharePoint 2013的REST API 十分强大，有时间再分享SharePoint 2013 REST API方面的知识。
