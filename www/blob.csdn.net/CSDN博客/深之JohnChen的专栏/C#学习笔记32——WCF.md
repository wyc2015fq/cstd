# C#学习笔记32——WCF - 深之JohnChen的专栏 - CSDN博客

2011年09月04日 23:17:21[byxdaz](https://me.csdn.net/byxdaz)阅读数：6113


                Windows Communication Foundation (WCF) 是用于构建面向服务的应用程序的框架。借助 WCF，可以将数据作为异步消息从一个服务终结点发送至另一个服务终结点。服务终结点可以是由 IIS 承载的持续可用的服务的一部分，也可以是应用程序中承载的服务。终结点可以是从服务终结点请求数据的服务客户端。简单消息可以是作为 XML 发送的单个字符或单个单词，复杂消息可以是二进制数据流。一些示例方案包括：- 						处理企业事务的安全服务。

- 						将当前数据提供给其他服务（例如流量报告或其他监视服务）的服务。

- 						使两个人能够实时通信或交换数据的聊天服务。

- 						轮询一个或多个服务以查找数据并将其以逻辑表现形式展示出来的面板应用程序。

- 						将使用 Windows Workflow Foundation 实现的工作流作为 WCF 服务公开。

- 						轮询服务以查找最新数据源的 Silverlight 应用程序。

虽然在 WCF 存在之前就可以创建此类应用程序，但 WCF 使终结点的开发比以前更容易。总之，WCF 设计提供一种用于创建 Web 服务和 Web 服务客户端的可管理的方式。

## WCF 的功能

WCF 包括以下功能集。有关更多信息，请参见 [WCF 功能详细信息](http://msdn.microsoft.com/zh-cn/library/ms733103.aspx).
- **服务导向**

							使用 WS 标准的一个结果是，WCF 允许您创建面向服务的应用程序。面向服务的体系结构 (SOA) 依赖 Web 服务发送和接收数据。这些服务具有松耦合的常规优点，而不是从一个应用程序到另一个应用程序进行硬编码。松耦合关系意味着只要符合基本协定，则在任何平台上创建的任何客户端均可连接到所有服务。

- **互操作性**

WCF 实现了 Web 服务互操作性的现代行业标准。有关支持的标准的更多信息，请参见[互操作性和集成](http://msdn.microsoft.com/zh-cn/library/ms730017.aspx)。

- **多种消息模式**

							采用多种模式之一交换消息。最常用的模式是请求/答复模式，其中一个终结点从另一个终结点请求数据，另一个终结点进行答复。还有其他模式，比如单向消息，其中只有一个终结点发送消息，而不期望得到答复。更复杂的模式是双工交换模式，在该模式下，两个终结点建立连接并来回发送数据，类似于即时消息传递程序。有关如何使用 WCF 实现不同消息交换模式的更多信息，请参见[协定](http://msdn.microsoft.com/zh-cn/library/aa702732.aspx)。

- **服务元数据**

WCF 支持使用行业标准（如 WSDL、XML 架构和 WS-Policy）中指定的格式发布服务元数据。该元数据可用于自动生成并配置客户端，以便访问 WCF 服务。可通过 HTTP 和 HTTPS 来发布元数据，也可使用 Web 服务元数据交换标准来发布元数据。有关更多信息，请参见 [元数据](http://msdn.microsoft.com/zh-cn/library/ms731823.aspx).

- **数据协定**

							由于 WCF 是使用 .NET Framework 生成的，因此它也包括代码友好的方法，用于提供希望强制执行的协定。数据协定就是其中一种通用类型的协定。实质上，当您使用 Visual C# 或 Visual Basic 对服务进行编码时，处理数据的最简单方法是使用属于数据实体的属性创建表示该数据实体的类。WCF 包括一个采用这一简便方式处理数据的综合系统。在创建了表示数据的类之后，服务会自动生成使客户端能够符合所设计数据类型的元数据。有关更多信息，请参见 [使用数据协定](http://msdn.microsoft.com/zh-cn/library/ms733127.aspx)

- **安全性**

							可对消息进加密以保护隐私，而且可以要求用户对其自身进行身份验证，然后才允许接收消息。可使用众所周知的标准（如 SSL 或 WS-SecureConversation）实现安全性。有关更多信息，请参见 [Windows Communication Foundation 安全性](http://msdn.microsoft.com/zh-cn/library/ms732362.aspx).

- **多种传输和编码方式**

							可通过多种内置传输协议和编码中的任意一种发送消息。最常用的协议和编码是使用超文本传输协议 (HTTP) 发送文本编码的 SOAP 消息，以便在万维网上使用。此外，WCF 还允许通过 TCP、命名管道或 MSMQ 发送消息。这些消息可以编码为文本，也可以使用优化的二进制格式。使用 MTOM 标准可有效地发送二进制数据。如果所提供的传输或编码方式都不符合您的需要，您可以创建自己的自定义传输或编码。有关 WCF 支持的传输和编码的更多信息，请参见 [Windows Communication Foundation 中的传输](http://msdn.microsoft.com/zh-cn/library/ms729350.aspx)。

- **可靠的排队消息**

WCF 支持使用通过 WS-ReliableMessaging 实现的可靠会话并使用 MSMQ 进行可靠的消息交换。有关 WCF 中的可靠的排队消息传递支持的更多信息，请参见[队列和可靠会话](http://msdn.microsoft.com/zh-cn/library/ms732355.aspx)。

- **持久性消息**

							持久性消息决不会由于通信中断而丢失。持久性消息模式的消息会始终保存到数据库中。如果发生中断，数据库将允许您在恢复连接后恢复消息交换。此外，也可以使用 Windows Workflow Foundation (WF) 来创建持久性消息。有关更多信息，请参见 [工作流服务](http://msdn.microsoft.com/zh-cn/library/dd456788.aspx).

- **事务**

WCF 还支持使用三个事务模型之一的事务：WS-AtomicTtransactions、[System.Transactions](http://msdn.microsoft.com/zh-cn/library/system.transactions.aspx) 命名空间中的 API 以及 Microsoft 分布式事务协调器。有关 WCF 中的事务支持的更多信息，请参见[事务](http://msdn.microsoft.com/zh-cn/library/ms730266.aspx)。

- **AJAX 和 REST 支持**

REST 是不断发展的 Web 2.0 技术的一个示例。您可以对 WCF 进行配置以处理未包装在 SOAP 信封中的“纯”XML 数据。此外，还可以扩展 WCF 以支持特定 XML 格式，如 ATOM（一种常用的 RSS 标准），甚至支持非 XML 格式，如 JavaScript 对象表示法 (JSON)。

- **可扩展性**

WCF 体系结构具有大量扩展点。如果需要额外功能，它还提供许多入口点，允许您自定义服务的行为。有关可用扩展点的更多信息，请参见[扩展 WCF](http://msdn.microsoft.com/zh-cn/library/ms733848.aspx)。

## WCF 与其他 Microsoft 技术的集成

WCF 是一个灵活的平台。由于这一极强的灵活性，WCF 还在 Microsoft 的一些其他产品中得以利用。如果您也在使用这些产品中的任何产品，通过了解 WCF 的基础知识，您会立即受益。

与 WCF 配对的第一项技术是 Windows Workflow Foundation (WF)。工作流通过将工作流中的各步骤封装为“活动”，从而简化了应用程序开发。在 Windows Workflow Foundation 的第一个版本中，开发人员必须为工作流创建一个主机。Windows Workflow Foundation 的下一版本与 WCF 进行了集成，从而允许在 WCF 服务中轻松承载任何工作流；通过在 Visual Studio 2010 和 Visual Studio 2010 中自动选择 WF/WCF 作为项目类型可实现此目的。

Microsoft BizTalk Server R2 还利用 WCF 作为通信技术。BizTalk 设计用于接收一种标准化格式的数据，然后将其转换为另一种格式。必须将消息传递到其中央消息框，在这里可以使用严格的映射，也可以通过使用其中一种 BizTalk 功能（如其工作流引擎）来转换消息。目前，BizTalk 可使用 WCF 业务线 (LOB) 适配器将消息传递到消息框。

Microsoft Silverlight 是一个用于创建可互操作的、丰富 Web 应用程序的平台，允许开发人员创建媒体密集的网站（如流视频）。从版本 2 开始，Silverlight 加入了 WCF 作为通信技术，以将 Silverlight 应用程序连接到 WCF 终结点。

Microsoft .NET Services 是一项云计算计划，使用 WCF 生成启用 Internet 的应用程序。使用 .NET Services 可创建跨信任边界操作的 WCF 服务。

Windows Server AppFabric 的承载功能 应用程序服务器是针对部署和管理使用 WCF 进行通信的应用程序专门生成的。承载功能 包括丰富的工具和配置选项，专门设计用于启用 WCF 的应用程序。

## WCF 体系结构

![WCF 体系结构](http://i.msdn.microsoft.com/dynimg/IC68535.gif)
### 协定和说明

协定定义消息系统的各个方面。数据协定描述组成某一服务可创建或使用的每则消息的每个参数。消息参数由 XML 架构定义语言 (XSD) 文档定义，这使得任何理解 XML 的系统均可处理该文档。消息协定使用 SOAP 协议定义特定消息部分，当互操作性要求对消息的某些部分进行更精细的控制时，消息协定可实现这种控制。服务协定指定服务的实际方法签名，并以支持的编程语言之一（例如 Visual Basic 或 Visual C#）作为接口进行分发。

策略和绑定规定与某一服务进行通信所需的条件。例如，绑定必须（至少）指定所使用的传输（例如 HTTP 或 TCP）和编码。策略包括安全要求和其他条件，必须满足这些要求和条件才能与服务进行通信。

### 服务运行时

服务运行时层包含仅在服务实际运行期间发生的行为，即该服务的运行时行为。遏制控制处理的消息数，如果对服务的需求增长到预设限制，该消息数则会发生变化。错误行为指定服务出现内部错误时应采取的操作，例如控制传递给客户端的信息（信息过多会向恶意用户提供攻击的机会）。元数据行为控制是否以及如何向外部提供元数据。实例行为指定可运行的服务实例的数目（例如，singleton 指定只能用单一实例来处理所有消息）。通过事务行为，可以在失败时回滚已进行事务处理的操作。调度行为用于控制 WCF 基础结构处理消息的方式。

通过扩展性功能可以自定义运行时进程。例如，消息检查功能用于检查消息的各个部分，使用参数筛选功能可以根据作用于消息头的筛选器来执行预设操作。

### 消息传递

消息传递层由通道组成。通道是以某种方式对消息进行处理（例如通过对消息进行身份验证）的组件。一组通道也称为“通道堆栈”。通道对消息和消息头进行操作。这与服务运行时层不同，服务运行时层主要涉及对消息正文内容的处理。

有两种类型的通道：传输通道和协议通道。

传输通道读取和写入来自网络（或外部的某些其他通信点）的消息。某些传输通道使用编码器来将消息（表示为 XML Infoset）转换为网络所使用的字节流的表示形式，或将字节流表示形式转换为消息。传输通道的示例包括 HTTP、命名管道、TCP 和 MSMQ。编码的示例包括 XML 和优化的二进制文件。

协议通道经常通过读取或写入消息的其他头的方式来实现消息处理协议。此类协议的示例包括 WS-Security 和 WS-Reliability。

消息传递层说明数据的可能格式和交换模式。WS-Security 是对在消息层启用安全性的 WS-Security 规范的实现。通过 WS-Reliable Messaging 通道可以保证消息的传递。编码器提供了大量的编码，可使用这些编码来满足消息的需要。HTTP 通道指定应使用超文本传输协议来传递消息。同理，TCP 通道指定 TCP 协议。事务流通道控制已经过事务处理的消息模式。通过命名管道通道可以进行进程间通信。使用 MSMQ 通道可以与 MSMQ 应用程序进行互操作。

### 承载和激活

服务的最终形式为程序。与其他程序类似，服务必须在可执行文件中运行。这称为“自承载”服务。

某些服务（如 IIS 或 Windows 激活服务 (WAS)）“被承载”，即在外部代理管理的可执行文件中运行。通过 WAS，可以在运行 WAS 的计算机上部署 WCF 应用程序时自动激活该应用程序。还可通过可执行文件（.exe 文件）的形式来手动运行服务。服务也可作为 Windows 服务自动运行。COM+ 组件也可作为 WCF 服务承载。


**WCF技术剖析系列：**

[WCF技术剖析之一：通过一个ASP.NET程序模拟WCF基础架构](http://www.cnblogs.com/artech/archive/2009/06/18/1506163.html)
[WCF技术剖析之二：再谈IIS与ASP.NET管道](http://www.cnblogs.com/artech/archive/2009/06/20/1507165.html)
[WCF技术剖析之三：如何进行基于非HTTP的IIS服务寄宿](http://www.cnblogs.com/artech/archive/2009/06/21/1507945.html)
[WCF技术剖析之四：基于IIS的WCF服务寄宿（Hosting）实现揭秘](http://www.cnblogs.com/artech/archive/2009/06/24/1510497.html)
[WCF技术剖析之五：利用ASP.NET兼容模式创建支持会话（Session）的WCF服务](http://www.cnblogs.com/artech/archive/2009/06/25/1511165.html)
[WCF技术剖析之六：为什么在基于ASP.NET应用寄宿（Hosting）下配置的BaseAddress无效](http://www.cnblogs.com/artech/archive/2009/06/26/1511916.html)
[WCF技术剖析之七：如何实现WCF与EnterLib PIAB、Unity之间的集成](http://www.cnblogs.com/artech/archive/2009/06/29/1513317.html)
[WCF技术剖析之八：ClientBase<T>中对ChannelFactory<T>的缓存机制](http://www.cnblogs.com/artech/archive/2009/07/03/1516573.html)
[WCF技术剖析之九：服务代理不能得到及时关闭会有什么后果?](http://www.cnblogs.com/artech/archive/2009/07/04/1516908.html)
[WCF技术剖析之十：调用WCF服务的客户端应该如何进行异常处
理](http://www.cnblogs.com/artech/archive/2009/07/05/1517257.html)[WCF技术剖析之十一：异步操作在WCF中的应用（上篇）](http://www.cnblogs.com/artech/archive/2009/07/08/1519423.html)
[WCF技术剖析之十一：异步操作在WCF中的应用（下篇）](http://www.cnblogs.com/artech/archive/2009/07/08/1519499.html)
[WCF技术剖析之十二：数据契约（Data Contract）和数据契约序列化器（DataContractSerializer）](http://www.cnblogs.com/artech/archive/2009/07/09/1520121.html)
[WCF技术剖析之十三：序列化过程中的已知类型（Known Type）](http://www.cnblogs.com/artech/archive/2009/07/12/1521994.html)
[WCF技术剖析之十四：泛型数据契约和集合数据契约（上篇）](http://www.cnblogs.com/artech/archive/2009/07/13/1522694.html)
[WCF技术剖析之十四：泛型数据契约和集合数据契约（下篇）](http://www.cnblogs.com/artech/archive/2009/07/14/1523578.html)
[WCF技术剖析之十五：数据契约代理（DataContractSurrogate）在序列化中的作用](http://www.cnblogs.com/artech/archive/2009/07/20/1527283.html)
[WCF技术剖析之十六：数据契约的等效性和版本控制](http://www.cnblogs.com/artech/archive/2009/07/21/1528221.html)

**[《WCF技术内幕》](http://blog.csdn.net/book_frank_xl/article/category/610972)**

**http://blog.csdn.net/book_frank_xl/article/category/610972**

