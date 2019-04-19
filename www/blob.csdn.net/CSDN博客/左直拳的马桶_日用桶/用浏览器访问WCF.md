# 用浏览器访问WCF - 左直拳的马桶_日用桶 - CSDN博客
2014年08月21日 23:43:10[左直拳](https://me.csdn.net/leftfist)阅读数：9539
在开发的时候，为客户端编写代码访问WCF的方式，我们应该比较熟悉，主要是添加一个代理，然后通过这个代理进行访问。
如果用浏览器访问WCF呢？（其实最终是想在JS里面访问）用浏览器访问、测试Web Service我们常常干， 而WCF整合了Web Service，Remoting，MSMQ，访问起来应当会更加方便吧？
一、代码配置
新建一个WCF服务，系统自动生成契约（即接口文件），代码文件（*.svc）。在契约文件里，要加上必要的特性
```
[OperationContract]
        [WebGet(UriTemplate="/HelloWorld", RequestFormat=WebMessageFormat.Json, ResponseFormat=WebMessageFormat.Json)]  
        string HelloWorld();
```
SVC文件里，类也要加上相关特性：
```
[AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
    public class Attendance : IAttendance
    {
        public string HelloWorld()
        {
            return "Hello World!";
        }
    }
```
类这个特性，貌似也可以写到配置文件里。
二、配置文件配置
1）配置及原理
配置文件要添加一些节点。下面标注为“手动添加”的，即为需要添加的节点。其余则是系统默认提供。
```
<system.serviceModel>
    <behaviors>
      <serviceBehaviors>
        <behavior>
          <!-- 为避免泄漏元数据信息，请在部署前将以下值设置为 false -->
          <serviceMetadata httpGetEnabled="false" httpsGetEnabled="false"/>
          <!-- 要接收故障异常详细信息以进行调试，请将以下值设置为 true。在部署前设置为 false 以避免泄漏异常信息 -->
          <serviceDebug includeExceptionDetailInFaults="false"/>
        </behavior>
      </serviceBehaviors>
      
      <!-- 我们手动添加 -->
      <endpointBehaviors>
        <behavior name="webBehavior">
          <webHttp />
        </behavior>
      </endpointBehaviors>
      
    </behaviors>
    
    <protocolMapping>
        <add binding="basicHttpsBinding" scheme="https" />
    </protocolMapping>    
    <serviceHostingEnvironment aspNetCompatibilityEnabled="true" multipleSiteBindingsEnabled="true" />
    <!-- 我们手动添加 -->
    <services>
      <service name="WCF_Mobile.Attendance">
        <endpoint behaviorConfiguration="webBehavior" binding="webHttpBinding"
          contract="WCF_Mobile.IAttendance" />
      </service>
    </services>
  </system.serviceModel>
```
这里面的节点含义，我目前并不十分了解。大概是：WCF本身提供服务（Services)，外部通过接口，即endpoint（端点）来使用这些服务。然后服务、端点，都有一些约定进行描述，即behavior。
从上述配置文件可以看到，Service下面的endpoint，引用了<endpointBehaviors>的内容。
有关配置文件的一些知识，可以参考
# [我的WCF之旅（2）：Endpoint Overview](http://www.cnblogs.com/artech/archive/2007/02/28/659331.html)
[http://www.cnblogs.com/artech/archive/2007/02/28/659331.html](http://www.cnblogs.com/artech/archive/2007/02/28/659331.html)
## [WCF配置文件详解(一)](http://www.cnblogs.com/weichuo/archive/2008/07/09/1238979.html)
[http://www.cnblogs.com/weichuo/archive/2008/07/09/1238979.html](http://www.cnblogs.com/weichuo/archive/2008/07/09/1238979.html)
2）跨域请求问题
不过，要注意，JS访问WCF，常常有跨域的问题。那么这时候要配置一下WCF的配置文件。详见拙作：
[PhoneGap：JS跨域请求](http://blog.csdn.net/leftfist/article/details/38383049)
[http://blog.csdn.net/leftfist/article/details/38383049](http://blog.csdn.net/leftfist/article/details/38383049)
三、用IIS承载WCF
首先要将WCF发布到一个指定文件夹，然后在IIS里配置此文件夹为网站。
这种发布跟普通asp.net网站发布没什么两样。习惯上，我喜欢发布为文件系统，每个文件一个DLL这种方式。
四、返回值类型用Stream
使用流作为返回值的好处，一是速度似乎比较快，二是如果服务返回string等其他类型，IE浏览器下，会有提问框出现，流就没有这个问题
```
public Stream Delete()
        {
            return GetStream(@"{""data"":""OK!!!!!!""}");
        }
        /// <summary>
        /// 辅助方法，用于输出流
        /// </summary>
        /// <param name="str"></param>
        /// <returns></returns>
        private Stream GetStream(string str)
        {
            MemoryStream ms = new MemoryStream();
            StreamWriter sw = new StreamWriter(ms);
            sw.AutoFlush = true;
            sw.Write(str);
            ms.Position = 0;
            WebOperationContext.Current.OutgoingResponse.ContentType = "text/plain";
            return ms;
        }
```
