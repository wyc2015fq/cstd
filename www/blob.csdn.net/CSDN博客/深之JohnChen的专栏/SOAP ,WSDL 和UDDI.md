# SOAP ,WSDL 和UDDI  - 深之JohnChen的专栏 - CSDN博客

2006年05月23日 14:39:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1909


                什么是Web Services 一直没有一个明确的答案,这里给出的看法是:Web Services 是一种基于组件的软件平台,是面向服务的Internet 应用。Web Services 是应用于Internet 的,而不是限于局域网或试验环境。这要求提出的Web Services 框架必须适用于现有的Internet 软件和硬件环境,即服务的提供者所提供的服务必须具有跨平台、跨语言的特性。其次,Web Services 所提供的服务不仅是向人,更需服务于其它应用系统。现有的Web网站也可以认为是面向服务的,但这种服务仅仅可以提供给人使用(只有人类才可以读懂浏览器下载的页面) 。而新一代的Web Services 所提供的服务应能被机器所读懂,例如其它应用程序及移动设备中的软件系统。这样,我们可以看出,Web Services 的发展方向实际上是构造一个在现有Internet 技术上的分布计算系统。

　　Web Services 框架的核心技术包括SOAP ,WSDL 和UDDI ,它们都是以标准的XML 文档的形式表的。SOAP 是Web services 的通信协议。SOAP是一种简单的、轻量级的基于XML 的机制，用于在网络应用程序之间进行结构化数据交换。SOAP包括三部分:一个定义描述消息内容的框架的信封，一组表示应用程序定义的数据类型实例的编码规则，以及表示远程过程调用和响应的约定。

　　WSDL表示WEB服务说明语言。WSDL文件是一个XML 文档，用于说明一组SOAP消息以及如何交换这些消息。

　　UDDI(统一描述发现和集成) 提供一种发布和查找服务描述的方法。UDDI 数据实体提供对定义业务和服务信息的支持。WSDL 中定义的服务描述信息是UDDI注册中心信息的补充。

　　Web Services 的体系架构如图1 所示

![](http://cio.csai.cn/56zg/images/no00060_01.JPG)

　　Web Services 服务提供方通过WSDL(Web Services Description Language) 描述所提供的服务,并将这一描述告知Web Services 注册服务器。注册服务器依据WSDL 的描述,依照UDDI (Universal Description Discovery and Integration) 的协定更新服务目录并在Internet 上发布。用户在使用Web Services 前先向注册服务器发出请求,获得Web Services 提供者的地址和服务接口信息,之后使用SOAP 协议(Simple Object Access Protocol) 与Web Services 提供者建立连接,进行通信。Web Services 的技术主要建立在XML 的规范之上,这保证了这一体系结构的平台无关性、语言无关性和人机交互性能。

　电子商务和ERP的集成框架。传统的应用系统大都不太灵活，它们都建立在点到点的解决方案的基础上，需要程序员大量的工作和一个比较长的开发周期。造成大量昂贵硬编码的单独应用，这些应用不能很好的集成也不能灵活的适应业务流程的改变。通过第2部分的分析，可以知道，功能组件化并具有良好集成性的Web services可解决这些问题。通过对Web服务平台的研究并结合电子商务和ERP的具体特点，本文提出的框架整体结构如图2所示:

![](http://cio.csai.cn/56zg/images/no00060_02.JPG)

　　在该模式中我们不使用传统点到点的集成，而是在原有集成模式的基础上结合Web服务新技术，提出的一种新模式。该集成框架将企业后台的ERP系统和前台的电子商务系统集成到框架中。

　　SOAP消息代理是整个框架的信息处理中心，它根据预先定义好的应用消息转换机制将消息转换成目标应用需要的数据并存储到应用消息仓库中。除了保存和转发应用消息之外，消息代理还负责将不同的企业应用所保存的应用数据通过企业模型转化为一致的企业数据，保存到数据库中。

　　客户端通过UDDI的标准和机制来搜寻需要的web服务，绑定找到的web服务并使用它提供的服务。另外，框架为企业进一步发展电子商务提供了接口，建立在UDDI基础上的Web services可以完成这一功能。

SOAP:
SOAP协议初级指南 :http://kb.csdn.net/java/Articles/200310/96246973-2b5c-4fd6-aafa-8cb79d780381.html
SOAP协议规范:http://dotnet.chinaitlab.com/XMLWebService/378796_4.html
[http://www.microsoft.com/china/msdn/Archives/xml/general/soapinteropbkgnd.asp](http://www.microsoft.com/china/msdn/Archives/xml/general/soapinteropbkgnd.asp)

UDDI
[http://www.microsoft.com/china/windowsserver2003/technologies/webapp/uddi/default.mspx](http://www.microsoft.com/china/windowsserver2003/technologies/webapp/uddi/default.mspx)

WSDL
[http://www.microsoft.com/China/msdn/Archives/msdnonline/features/articles/wsdl.asp](http://www.microsoft.com/China/msdn/Archives/msdnonline/features/articles/wsdl.asp)

