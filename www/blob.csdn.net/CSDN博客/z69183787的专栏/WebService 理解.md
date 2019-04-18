# WebService 理解 - z69183787的专栏 - CSDN博客
2016年12月05日 20:35:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：517
个人分类：[WebService](https://blog.csdn.net/z69183787/article/category/2175701)
从上至下结构
binding
porttype
operation
message
。。。。
转：http://www.cnblogs.com/yongfeng/archive/2013/01/30/2883146.html?utm_source=tuicool&utm_medium=referral
wsdl格式解析：http://blog.csdn.net/z69183787/article/details/53485972
Webservice是使应用程序以与平台和编程语言无关的方式进行相互通信技术。
eg:站点提供访问的数据接口：新浪微博、淘宝。
官方解释：它是一种构建应用程序的普遍模型,可以在任何支持网络通信的操作系统中实施运行;它是一种新的web应用程序分支，是自包含、自描述、模块化的应用，可以发布、定位、通过web调用。WebService是一个应用组件,它逻辑性的为其他应用程序提供数据与服务.各应用程序通过网络协议和规定的一些标准数据格式（Http，XML，Soap)来访问WebService,通过WebService内部执行得到所需结果.Web Service可以执行从简单的请求到复杂商务处理的任何功能。一旦部署以后，其他WebService应用程序可以发现并调用它部署的服务。
![](http://images.cnitblog.com/blog/114370/201301/30153018-26f61ea3127e40b48c4a98e32bcabd89.jpg)
SOAP（Simple Object Access Protocol）：简单对象访问协议是在分散或分布式的环境中交换信息并执行远程过程调用的轻量级协议，是一个基于XML的协议。使用SOAP，不用考虑任何特定的传输协议（最常用的还是HTTP协议），可以允许任何类型的对象或代码，在任何平台上，以任何一种语言相互通信。
WSDL：Web Services Description Language的缩写，是一个用来描述Web服务和说明如何与Web服务通信的XML语言。为用户提供详细的接口说明书。
Axis：Axis本质上就是一个SOAP引擎（Apache Axis is an implementation of the SAOP），提供创建服务名、客户端和网关SOAP操作的基本框架。但是Axis并不完全是一个SOAP引擎，它还包括：
- 是一个独立的SOAP服务器。
- 是一个嵌入Servlet引擎（eg:Tomcat）的服务器。
- 支持WSDL。
- 提供转化WSDL为Java类的工具。
- 提供例子程序。
- 提供TCP/IP数据包监视工具。
Axis有四种Service styles，分别是：
- RPC（Remote Procedure Call Protocol远程访问调用协议，部署时属于默认选项）
- Document
- Wrapped
- Message
WSDD（Web Service Deployment Descriptor）：Web服务分布描述，它定义了Web服务的接口，如服务名、提供的方法、方法的参数信息。
UDDI（Universal Description，Discovery，and Integration）：统一描述、发现和集成，用于集中存放和查找WSDL描述文件，起着目录服务器的作用。
![](http://images.cnitblog.com/blog/114370/201301/30153048-42b48f25f92b4e7a8d67d662ebc6a7b8.png)
# WSDL元素
WSDL元素基于XML语法描述了与服务进行交互的基本元素：
Type（消息类型）：数据类型定义的容器，它使用某种类型系统（如XSD）。
Message（消息）：通信数据的抽象类型化定义，它由一个或者多个part组成。
Part：消息参数
Operation(操作)：对服务所支持的操作进行抽象描述，WSDL定义了四种操作：
- 单向（one-way）：端点接受信息；
- 请求-响应（request-response）：端点接受消息，然后发送相关消息；
- 要求-响应（solicit-response）：端点发送消息，然后接受相关消息；
- 通知（notification）：端点发送消息。
Port Type （端口类型）：特定端口类型的具体协议和数据格式规范。
Binding：特定端口类型的具体协议和数据格式规范
Port ：定义为绑定和网络地址组合的单个端点。
Service：相关端口的集合，包括其关联的接口、操作、消息等。
![](http://images.cnitblog.com/blog/114370/201301/30153125-cb3ad43fadeb4815b6d744033bd8e98e.jpg)
以上类图表达了Service、Port、Binding、Operation、Message之间的依赖、关联、聚合、合成、泛化、实现，这里暂不多说，若感兴趣，请参考该文章
UML类图关系大全：[http://www.cnblogs.com/riky/archive/2007/04/07/704298.html](http://www.cnblogs.com/riky/archive/2007/04/07/704298.html)
# WSDL伪代码
WSDL 文档是利用这些主要的元素来描述某个 web service 的：
元素 定义
web service 执行的操作
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<message> web service 使用的消息
<types> web service 使用的数据类型
<binding> web service 使用的通信协议
一个 WSDL 文档的主要结构是类似这样的：
<definitions>
<types>
definition of types........
</types>
<message>
definition of a message....
</message>
<portType>
definition of a port.......
</portType>
<binding>
definition of a binding....
</binding>
</definitions>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
WSDL 文档可包含其它的元素，比如 extension 元素，以及一个 service 元素，此元素可把若干个 web services 的定义组合在一个单一的 WSDL 文档中
# 实践
为了形成鲜明的对比，客户端用CS架构来创建客户端。
## 实践之一：创建服务端
### 创建ASP.NET Web服务
![](http://images.cnitblog.com/blog/114370/201301/30153228-a58b71cc2cba457da7945476b6133b4b.jpg)
### 代码示例
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
using System;
using System.Linq;
using System.Web;
using System.Web.Services;
using System.Web.Services.Protocols;
using System.Xml.Linq;
using DotNet.Model;
[WebService(Namespace = "http://tempuri.org/")]
[WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)]
// 若要允许使用 ASP.NET AJAX 从脚本中调用此 Web 服务，请取消对下行的注释。
// [System.Web.Script.Services.ScriptService]
public class Service : System.Web.Services.WebService
{
    public Service () {
        //如果使用设计的组件，请取消注释以下行
        //InitializeComponent();
    }
     [WebMethod(Description="获取字符串",MessageName="HelloWorld")]
    public string HelloWorld() {
        return "Hello World";
    }
    [WebMethod(Description="获取用户信息",MessageName="getCustomer")]
    public Customer getCustomer(Customer cus1)
    {
        return cus1;
    }
 
    [WebMethod(Description = "获取用户信息以参数形式", MessageName = "getCustomerFromParams")]
    public Customer getCustomerFromParams(int id, string name, string address)
    {
        Customer cus1 = new Customer();
        cus1.cus_id = id;
        cus1.cus_name = name;
        cus1.cus_address = address;
        return cus1;
    }
   
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
## 实践之二：创建客户端
### 创建ASP.NET WEB客户端（以CS架构）
![](http://images.cnitblog.com/blog/114370/201301/30153456-cec4d872a91c48e1a21432acf02b0db0.jpg)
代码示例
 （其中一种方式通过创建“服务引用”的方式，输入“http://localhost:端口号/XX.asmx?wsdl”方式，获得服务访问接口）
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
      private ServiceReference1.ServiceSoapClient myclient = new ServiceReference1.ServiceSoapClient();
 
        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                /* 第一种方式可以传参、传实体，返回实体 */
                ServiceReference1.ServiceSoapClient myclient = new ServiceReference1.ServiceSoapClient();
                ServiceReference1.Customer tem = new ServiceReference1.Customer();
                tem.cus_id = int.Parse(textBox1.Text);
                tem.cus_name = textBox2.Text;
                tem.cus_address = textBox3.Text;
                string str = JsonHelper.Jso_ToJSON(myclient.getCustomer(tem));
                richTextBox1.Text = str;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
 
        private void button2_Click(object sender, EventArgs e)
        {
            richTextBox1.Text = string.Empty;
            try
            {
                richTextBox1.Text = myclient.HelloWorld();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
 
        private void button4_Click(object sender, EventArgs e)
        {
            try
            {
                /* 第二种方式可以传参，返回实体(不能传入一个对象实体，但是配置动态灵活) */
                string url = "http://localhost:3199/ServicePort/Service.asmx";
                string methodname = "getCustomerFromParams";
                object[] obj = new object[3];
                obj[0] = int.Parse(textBox1.Text);
                obj[1] = textBox2.Text;
                obj[2] = textBox3.Text;
                string str = JsonHelper.Jso_ToJSON(WebServiceHelper.InvokeWebService(url, methodname, obj));
                richTextBox1.Text = str;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
 
        private void button5_Click(object sender, EventArgs e)
        {
            try
            {
                /* 第三种方式可以传参、传实体，返回XmlDom(配置动态灵活，响应处理麻烦了点而已) */
                string url = "http://localhost:3199/ServicePort/Service.asmx";
                string methodname = "getCustomer";
                Hashtable parm = new Hashtable();
                string objectName = "cus1";
                parm["cus_id"] = int.Parse(textBox1.Text);
                parm["cus_name"] = textBox2.Text;
                parm["cus_address"] = textBox3.Text;
                XmlDocument oo = WebServiceXmlHelper.QuerySoapWebServiceByObject(url, methodname, objectName, parm);
                richTextBox1.Text = oo.InnerXml;
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
## 运行效果
### 服务端
![](http://images.cnitblog.com/blog/114370/201301/30153538-dacd4601756f4a2bb669f607213e73e0.jpg)
![](http://images.cnitblog.com/blog/114370/201301/30153602-fc208e4c354049f684f0255b62f64bf1.jpg)
WSDL
![](http://images.cnitblog.com/blog/114370/201301/30153624-5bc2d21f6fff4393b695e1c53e89b525.jpg)
### 客户端（支持多平台，如服务器的Webservice用Java、.Net等）
第1种方式：传参、传实体，URL配置缺少灵活，数据处理灵活
![](http://images.cnitblog.com/blog/114370/201301/30153658-13ddd1b2b2804f68a8a13bebc99537d7.jpg)
第2种方式：传参，不能传实体，URL配置灵活, 数据处理要稍微加工
![](http://images.cnitblog.com/blog/114370/201301/30153706-30973b1af4784c2d8db0208f258bf205.jpg)
第3种方式：传参、传实体、URL配置灵活，数据处理要稍微加工
![](http://images.cnitblog.com/blog/114370/201301/30153716-69d26623c1ff4019ba6e1fa654b4ac69.jpg)
# 小结
- 如果只传递参数，可以用HTTP来传递，Webservice提供的接口如下
HTTP POST
以下是 HTTP POST 请求和响应示例。所显示的占位符需替换为实际值。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
POST /ServicePort/Service.asmx/getCustomerFromParams HTTP/1.1
Host: localhost
Content-Type: application/x-www-form-urlencoded
Content-Length: length
 
id=string&name=string&address=string
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
返回接收串
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
HTTP/1.1 200 OK
Content-Type: text/xml; charset=utf-8
Content-Length: length
<?xml version="1.0" encoding="utf-8"?>
<Customer xmlns="http://tempuri.org/">
  <cus_id>int</cus_id>
  <cus_name>string</cus_name>
  <cus_address>string</cus_address>
</Customer>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
- 如果是传递实体或传递参数，也可以使用SOAP来传递，Webservice提供的接口如下
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
POST /ServicePort/Service.asmx HTTP/1.1
Host: localhost
Content-Type: application/soap+xml; charset=utf-8
Content-Length: length
 
<?xml version="1.0" encoding="utf-8"?>
<soap12:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap12="http://www.w3.org/2003/05/soap-envelope">
  <soap12:Body>
    <getCustomerFromParams xmlns="http://tempuri.org/">
      <id>int</id>
      <name>string</name>
      <address>string</address>
    </getCustomerFromParams>
  </soap12:Body>
</soap12:Envelope>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
返回接收串：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
HTTP/1.1 200 OK
Content-Type: application/soap+xml; charset=utf-8
Content-Length: length
<?xml version="1.0" encoding="utf-8"?>
<soap12:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap12="http://www.w3.org/2003/05/soap-envelope">
  <soap12:Body>
    <getCustomerFromParamsResponse xmlns="http://tempuri.org/">
      <getCustomerFromParamsResult>
        <cus_id>int</cus_id>
        <cus_name>string</cus_name>
        <cus_address>string</cus_address>
      </getCustomerFromParamsResult>
    </getCustomerFromParamsResponse>
  </soap12:Body>
</soap12:Envelope>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 WebService向外发布接口的功能，能够更好的为其它平台提供数据以及现实信息平台一体化。
Word文档下载：[浅淡Webservice、WSDL三种服务访问的方式.doc](http://files.cnblogs.com/yongfeng/0001%E6%B5%85%E6%B7%A1Webservice%E3%80%81WSDL%E4%B8%89%E7%A7%8D%E6%9C%8D%E5%8A%A1%E8%AE%BF%E9%97%AE%E7%9A%84%E6%96%B9%E5%BC%8F.rar)
源代码下载：[http://files.cnblogs.com/yongfeng/001DotNetWebService.rar](http://files.cnblogs.com/yongfeng/001DotNetWebService.rar)
参考网站：[http://www.w3.org/TR/wsdl](http://www.w3.org/TR/wsdl)
