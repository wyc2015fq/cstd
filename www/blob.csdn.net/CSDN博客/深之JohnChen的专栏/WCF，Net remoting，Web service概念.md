# WCF，Net remoting，Web service概念 - 深之JohnChen的专栏 - CSDN博客

2010年01月23日 16:54:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1227标签：[wcf																[web																[service																[asp.net																[webservice																[soap](https://so.csdn.net/so/search/s.do?q=soap&t=blog)](https://so.csdn.net/so/search/s.do?q=webservice&t=blog)](https://so.csdn.net/so/search/s.do?q=asp.net&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=web&t=blog)](https://so.csdn.net/so/search/s.do?q=wcf&t=blog)
个人分类：[.Net & C#](https://blog.csdn.net/byxdaz/article/category/628880)


一 WCF
概括地说，WCF具有如下的优势：
    1、统一性
    前面已经叙述，WCF是对于ASMX，.Net Remoting，Enterprise Service，WSE，MSMQ等技术的整合。由于WCF完全是由托管代码编写，因此开发WCF的应用程序与开发其它的.Net应用程序没有太大的区别，我们仍然可以像创建面向对象的应用程序那样，利用WCF来创建面向服务的应用程序。
    2、互操作性
    由于WCF最基本的通信机制是SOAP，这就保证了系统之间的互操作性，即使是运行不同的上下文中。这种通信可以是基于.Net到.Net间的通信。

    可以跨进程、跨机器甚至于跨平台的通信，只要支持标准的Web Service，例如J2EE应用服务器（如WebSphere，WebLogic）。应用程序可以运行在Windows操作系统下，也可以运行在其他的操作系统，如Sun Solaris，HP Unix，Linux等等。

    3、安全与可信赖
WS-Security，WS-Trust和WS-SecureConversation均被添加到SOAP消息中，以用于用户认证，数据完整性验证，数据隐私等多种安全因素。
在SOAP的header中增加了WS-ReliableMessaging允许可信赖的端对端通信。而建立在WS-Coordination和WS-AtomicTransaction之上的基于SOAP格式交换的信息，则支持两阶段的事务提交（two-phase commit transactions）。
    上述的多种WS-Policy在WCF中都给与了支持。对于Messaging而言，SOAP是Web Service的基本协议，它包含了消息头（header）和消息体(body)。在消息头中，定义了WS-Addressing用于定位SOAP消息的地址信息，同时还包含了MTOM（消息传输优化机制，Message Transmission Optimization Mechanism）。

    4、兼容性
    WCF充分的考虑到了与旧有系统的兼容性。安装WCF并不会影响原有的技术如ASMX和.Net Remoting。即使对于WCF和ASMX而言，虽然两者都使用了SOAP，但基于WCF开发的应用程序，仍然可以直接与ASMX进行交互。

二 WebService的运行机理 

首先客户端从服务器的到WebService的WSDL，同时在客户端声称一个代理类(Proxy Class)， 这个代理类负责与WebService服务器进行Request 和Response， 当一个数据（XML格式的）被封装成SOAP格式的数据流发送到服务器端的时候，就会生成一个进程对象并且把接收到这个Request的SOAP包进行解析，然后对事物进行处理，处理结束以后再对这个计算结果进行SOAP包装，然后把这个包作为一个Response发送给客户端的代理类(Proxy Class)，同样地，这个代理类也对这个SOAP包进行解析处理，继而进行后续操作。

三 .net Remoting

是在DCOM等基础上发展起来的一种技术，它的主要目的是实现跨平台、跨语言、穿透企业防火墙，这也是他的基本特点，与WebService有所不同的是，它支持HTTP以及TCP信道，而且它不仅能传输XML格式的SOAP包，也可以传输传统意义上的二进制流，这使得它变得效率更高也更加灵活。而且它不依赖于IIS，用户可以自己开发(Development)并部署(Dispose)自己喜欢的宿主服务器，所以从这些方面上来讲WebService其实上是.netemoting的一种特例。

区别：

1、Remoting可以灵活的定义其所基于的协议，比如http，tcp等，如果定义为HTTP，则与Web Service相同，但是webservice是无状态的，使用remoting一般都喜欢定义为TCP，这样比Web Service稍为高效一些，而且是有状态的。

2、Remoting不是标准，而Web Service是标准。

3、Remoting一般需要通过一个WinForm或是Windows服务进行启动，也可以使用iis部署，而Web Service则必须在IIS进行启动。

4、在VS.net开发环境中，专门对Web Service的调用进行了封装，用起来比Remoting方便。

5 net remoting只能应用于MS 的.net framework之下，需要客户端必须安装framework，但是WebService是平台独立的，跨语言（只要能支持XML的语言都可以） 以及穿透企业防火墙。

来自MSDN：[http://www.microsoft.com/china/MSDN/library/enterprisedevelopment/builddistapp/ASP.NETWebServicesor.NETRemoting-HowtoChoose.mspx?mfr=true](http://www.microsoft.com/china/MSDN/library/enterprisedevelopment/builddistapp/ASP.NETWebServicesor.NETRemoting-HowtoChoose.mspx?mfr=true)

分布式应用程序设计：ASP.NET Web 服务和 .NET Remoting

ASP.NET Web 服务偏向于 XML Schema 类型系统，提供具有广泛使用范围的跨平台支持的简单编程模型。.NET Remoting 偏向于运行时类型系统，提供较为复杂而且使用范围小得多的编程模型。这种本质上的差别是决定使用哪种技术的主要因素。但是，还要考虑很多其他设计因素，包括传输协议、主机进程、安全性、性能、状态管理以及对事务的支持等。

传输协议和主机进程

尽管 SOAP 规范并不要求用 HTTP 作为传输协议，但是客户端只能通过 HTTP 访问使用 ASP.NET Web 服务实现的 Web 服务，因为它是 ASP.NET 支持的唯一一种传输协议。服务是通过 IIS 调用的，并在 ASP.NET 的辅助进程 aspnet_wp.exe 中执行。

.NET Remoting 使您能够在任何类型的应用程序（包括 Windows 窗体、托管的 Windows 服务、控制台应用程序或 ASP.NET 辅助进程）中灵活地托管远程对象。正如前面所述，.NET Remoting 提供两个传输信道——TCP 和 HTTP。这两个信道都能使用套接字提供任意发送和接收进程之间的通信。

它还能将 HTTP 信道与 IIS 和 ASP.NET 辅助进程集成。这一点很重要，原因有以下几点。首先，它是当客户端请求到达时自动启动 .NET Remoting 端点的唯一方法。.NET Remoting 管线不包括启动远程服务器所需的 DCOM 类型的服务控制管理器 (SCM)。如果从任意进程中提供远程对象，则需要确保那些进程正在运行。还必须确保它们是线程安全的，例如，线程 A 不能在线程 B 开始关闭进程之后激活对象。如果从 ASP.NET 提供远程对象，则可以利用 Aspnet_wp.exe 辅助进程，这样既可自动启动又具有线程安全的优势。第二，与 IIS 集成是确保跨进程 .NET Remoting 调用的唯一途径，如下一节所述。

ASP.NET Web 服务和 .NET Remoting 基础结构都是可扩展的。您可以过滤入站和出站消息，从多方面控制类型封送和元数据的生成。使用 .NET Remoting，还能实现您自己的格式化程序和信道。 

安全性

由于 ASP.NET Web 服务依赖于 HTTP，因此它们与标准的 Internet 安全性基础结构相集成。ASP.NET 利用 IIS 的安全性功能，为标准 HTTP 验证方案（包括基本、简要、数字证书，甚至 Microsoft? .NET Passport）提供了强有力的支持。（还可以使用 Windows 集成验证，但只能用于信任域中的客户端。）使用可用的 HTTP 验证方案的一个优势在于，无需在 Web 服务中更改代码，IIS 是在 ASP.NET Web 服务被调用之前执行验证的。ASP.NET 还支持基于 .NET Passport 的验证和其他自定义的验证方案。ASP.NET 支持基于目标 URL 的访问控制，并通过与 .NET 代码访问安全性 (CAS) 基础结构的集成支持访问控制。SSL 可用于确保通信的安全。

尽管这些标准传输技术对于确保 Web 服务相当有效，但它们只能做到这种程度。在涉及到不同信任域中多个 Web 服务的复杂情况下，还得建立自定义的特殊解决方案。Microsoft 和其他公司正致力于创建一套安全性规范，该规范将基于 SOAP 消息的可扩展性提供消息级别的安全性功能。这些规范之一是 XML Web 服务安全性语言（WS-Security），它为消息级别的凭据传输、消息完整性和消息保密定义了框架。

正如上一节所述，一般情况下，.NET Remoting 管线不能确保跨进程调用的安全。使用 ASP.NET 托管于 IIS 中的 .NET Remoting 端点可以利用 ASP.NET Web 服务可用的所有安全性功能，包括对使用 SSL 确保有线通信的安全性的支持。如果您正在使用托管在进程中的 TCP 信道或 HTTP 信道（而不是 aspnet_wp.exe），则必须自己执行身份验证、授权和保密机制。 

另一个要关注的安全性问题是，在不必更改默认安全性策略的情况下，从不完全信任的环境中执行代码的能力。ASP.NET Web 服务客户端代理可以在这些环境中工作，但 .NET Remoting 代理则不能。要从不完全信任的环境中使用 .NET Remoting 代理，需要特殊的序列化权限。默认情况下，该权限不会授予从 Intranet 或 Internet 上下载的代码。如果要在不完全信任的环境中使用 .NET Remoting 客户端，则需要更改从那些区域中加载的代码的默认安全性策略。当您从运行于沙箱（如下载的 Windows 窗体应用程序）中的客户端连接到系统时，ASP.NET Web 服务是较简单的选择，因为不需要更改安全性策略。

状态管理

默认情况下，ASP.NET Web 服务模型采用无状态的服务结构；它并不是本能地与来自同一个用户的多个调用相关。另外，客户端每次调用 ASP.NET Web 服务时，都创建一个新的对象以服务于该请求。方法调用完成后，该对象即被破坏。要维护请求之间的状态，可以使用 ASP.NET 页面使用的相同技术（例如，Session 和 Application 属性包），也可以自己实现自定义的解决方案。 

.NET Remoting 支持许多状态管理选项，并且可能与来自同一个用户的多个调用相关或不相关，这取决于您选择的对象生命周期架构。SingleCall 对象是无状态的（如用于调用 ASP.NET Web 服务的对象），Singleton 对象共享所有客户端的状态，客户端激活的对象在每个客户端的基础上保持状态（带有其产生的所有相关的可升级性和可靠性问题）。

性能

从原始性能方面来讲，使用 TCP 信道和二进制格式化程序时，.NET Remoting 管线能够提供最快的通信。在我们进行的比较 ASP.NET Web 服务和 .NET Remoting 的相对性能的几乎所有的测试中，ASP.NET Web 服务在性能上都超出了使用 HTTP 或 TCP 信道的 SOAP 格式化程序的 .NET Remoting 端点。更有意思的是，使用二进制格式化程序和 HTTP 信道的 ASP.NET 和 .NET Remoting 端点在性能上非常相近。（更多信息，请参见 Performance Comparison:NET Remoting vs. ASP.NET Web Services。）

企业服务

ASP.NET Web 服务或通过 .NET Remoting 提供的对象可以使用本地事务根据单个数据库协调工作。如果需要根据多个资源协调工作，可以使用 .NET 企业服务（又称 COM+）公布的事务（由 COM+ 管线管理的 DTC 分布式事务）。但要注意的是，ASP.NET Web 服务和 .NET Remoting 管线都不能传播公布的事务，因此两种端点都不可能通过跨进程的调用继承公布的事务。

这不一定是件坏事。一般来讲，公布的事务比本地事务代价要高，而要跨进程传播公布的事务，则代价会更高。如果确实需要这一功能，简单的解决方案是在 .NET 企业服务的服务器应用程序中部署一个从 System.EnterpriseServices.ServicedComponent 派生的类（更多信息，请参见 COM+ Integration:How .NET Enterprise Services Can Help You Build Distributed Applications）。对该类对象的跨进程调用将使用 DCOM 进行处理，以确保正确传播事务环境。较难的解决方案是使用底层的 API，手动传播分布的事务。

值得注意的是，传统的分布式事务模型一般不适用于松散耦合的 Web 服务。基于补偿事务的模型（即，撤消其他事务所提交工作的事务）更有意义，因为其隔离约束条件并不是很严格。在包括 Microsoft 的 Web 服务供应商中有一种普遍的说法，即 Web 服务空间需要的事务模型越灵活，该空间中进行的工作越多。等到定义出 Web 服务事务的标准方法时，您就可以根据情况使用本地或公布的事务实现自己的补偿架构了。

小结
虽然 .NET Remoting 基础结构和 ASP.NET Web 服务都可以进行跨进程通信，但每种设计适用于不同的用户。ASP.NET Web 服务提供了简单的编程模型，并具有广泛的使用范围。.NET Remoting 提供了较为复杂的编程模型，而且使用范围窄得多。请务必了解这两种技术的工作原理，并选择适合您应用程序的技术。在任意一种情况下，都要使用 IIS 和 ASP.NET 管理进程生命周期，并提供一般的安全性。


