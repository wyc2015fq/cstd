# 消息队列（Message Queue）编程 - 深之JohnChen的专栏 - CSDN博客

2010年01月23日 17:12:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2121


**消息队列（Message Queue）简介及其使用**

利用MSMQ（Microsoft Message Queue），应用程序开发人员可以通过发送和接收消息方便地与应用程序进行快速可靠的通信。消息处理为您提供了有保障的消息传递和执行许多业务处理的可靠的防故障方法。

MSMQ与XML Web Services和.Net Remoting一样，是一种分布式开发技术。但是在使用XML Web Services或.Net Remoting组件时，Client端需要和Server端实时交换信息，Server需要保持联机。MSMQ则可以在Server离线的情况下工作，将Message临时保存在Client端的消息队列中，以后联机时再发送到Server端处理。

显然，MSMQ不适合于Client需要Server端及时响应的这种情况，MSMQ以异步的方式和Server端交互，不用担心等待Server端的长时间处理过程。

虽然XML Web Services和.Net Remoting都提供了[OneWay]属性来处理异步调用，用来解决Server端长方法调用长时间阻碍Client端。但是不能解决大量Client负载的问题，此时Server接受的请求快于处理请求。

一般情况下，[OneWay]属性不用于专门的消息服务中。

**1. 基本术语和概念（Basic terms and concepts）**

“消息”是在两台计算机间传送的数据单位。消息可以非常简单，例如只包含文本字符串；也可以更复杂，可能包含嵌入对象。

消息被发送到队列中。“消息队列”是在消息的传输过程中保存消息的容器。消息队列管理器在将消息从它的源中继到它的目标时充当中间人。队列的主要目的是提供路由并保证消息的传递；如果发送消息时接收者不可用，消息队列会保留消息，直到可以成功地传递它。

“消息队列”是 Microsoft 的消息处理技术，它在任何安装了 Microsoft Windows 的计算机组合中，为任何应用程序提供消息处理和消息队列功能，无论这些计算机是否在同一个网络上或者是否同时联机。

“消息队列网络”是能够相互间来回发送消息的任何一组计算机。网络中的不同计算机在确保消息顺利处理的过程中扮演不同的角色。它们中有些提供路由信息以确定如何发送消息，有些保存整个网络的重要信息，而有些只是发送和接收消息。

“消息队列”安装期间，管理员确定哪些服务器可以互相通信，并设置特定服务器的特殊角色。构成此“消息队列”网络的计算机称为“站点”，它们之间通过“站点链接”相互连接。每个站点链接都有一个关联的“开销”，它由管理员确定，指示了经过此站点链接传递消息的频率。

“消息队列”管理员还在网络中设置一台或多台作为“路由服务器”的计算机。路由服务器查看各站点链接的开销，确定经过多个站点传递消息的最快和最有效的方法，以此决定如何传递消息。

**2. 队列类型（Queue Type）**

有两种主要的队列类型：由您或网络中的其他用户创建的队列和系统队列。

**用户创建的队列可能是以下任何一种队列：**

“公共队列”在整个“消息队列”网络中复制，并且有可能由网络连接的所有站点访问。

“专用队列”不在整个网络中发布。相反，它们仅在所驻留的本地计算机上可用。专用队列只能由知道队列的完整路径名或标签的应用程序访问。

“管理队列”包含确认在给定“消息队列”网络中发送的消息回执的消息。指定希望 MessageQueue 组件使用的管理队列（如果有的话）。

“响应队列”包含目标应用程序接收到消息时返回给发送应用程序的响应消息。指定希望 MessageQueue 组件使用的响应队列（如果有的话）。

**系统生成的队列一般分为以下几类：**

“日记队列”可选地存储发送消息的副本和从队列中移除的消息副本。每个“消息队列”客户端上的单个日记队列存储从该计算机发送的消息副本。在服务器上为每个队列创建了一个单独的日记队列。此日记跟踪从该队列中移除的消息。

“死信队列”存储无法传递或已过期的消息的副本。如果过期或无法传递的消息是事务性消息，则被存储在一种特殊的死信队列中，称为“事务性死信队列”。死信存储在过期消息所在的计算机上。有关超时期限和过期消息的更多信息，请参见默认消息属性。

“报告队列”包含指示消息到达目标所经过的路由的消息，还可以包含测试消息。每台计算机上只能有一个报告队列。

“专用系统队列”是一系列存储系统执行消息处理操作所需的管理和通知消息的专用队列。

在应用程序中进行的大多数工作都涉及访问公共队列及其消息。但是，根据应用程序的日记记录、确认和其他特殊处理需要，在日常操作中很可能要使用几种不同的系统队列。

**3. 同步和异步通信（Synchronous VS. Asynchronous Communication）**

队列通信天生就是异步的，因为将消息发送到队列和从队列中接收消息是在不同的进程中完成的。另外，可以异步执行接收操作，因为要接收消息的人可以对任何给定的队列调用 BeginReceive 方法，然后立即继续其他任务而不用等待答复。这与人们所了解的“同步通信”截然不同。在同步通信中，请求的发送方在执行其他任务前，必须等待来自预定接收方的响应。发送方等待的时间完全取决于接收方处理请求和发送响应所用的时间。

(1)同步接收消息

  接收消息的代码很简单：

 Mq.Receive();
        Mq.Receive(TimeSpan timeout); //设定超时时间
 Mq.ReceiveById(ID);
        Mq.Peek(); 

 通过Receive方法接收消息同时永久性地从队列中删除消息；
 通过Peek方法从队列中取出消息而不从队列中移除该消息。
 如果知道消息的标识符（ID），还可以通过ReceiveById方法和PeekById方法完成相应的操作。

(2)异步接受消息

   利用委托机制：

 MessQueue.ReceiveCompleted +=new ReceiveCompletedEventHandler(mq_ReceiveCompleted);

(3)消息阅读

在应用程序能够阅读的消息和消息队列中的消息格式不同，应用程序发送出去的消息经过序列化以后才发送给了消息队列
而在接受端必须反序列化，利用下面的代码可以实现：

 public void mq_ReceiveCompleted(object sender, System.Messaging.ReceiveCompletedEventArgs e)
  {
   System.Messaging.Message m = MessQueue.EndReceive(e.AsyncResult);
   m.Formatter = new System.Messaging.XmlMessageFormatter(new string[]{"System.String,mscorlib"});
   Console.WriteLine("Message: " + (string)m.Body);
   MessQueue.BeginReceive() ;

  }

反序列化还有另一种写法：
 m.Formatter = new XmlMessageFormatter ( new Type [] { typeof (string) } );

.Net下的MSMQ（微软消息队列）的同步异步调用 :[http://www.cnblogs.com/goody9807/archive/2007/05/18/546572.html](http://www.cnblogs.com/goody9807/archive/2007/05/18/546572.html)

**4. 同消息队列交互（Interacting with Message Queues）**

消息处理和消息为基于服务器的应用程序组件之间的进程间通信提供了强大灵活的机制。同组件间的直接调用相比，它们具有若干优点，其中包括：
- 稳定性 — 组件失败对消息的影响程度远远小于组件间的直接调用，因为消息存储在队列中并一直留在那里，直到被适当地处理。消息处理同事务处理相似，因为消息处理是有保证的。
- 消息优先级 — 更紧急或更重要的消息可在相对不重要的消息之前接收，因此可以为关键的应用程序保证足够的响应时间。
- 脱机能力 — 发送消息时，它们可被发送到临时队列中并一直留在那里，直到被成功地传递。当因任何原因对所需队列的访问不可用时，用户可以继续执行操作。同时，其他操作可以继续进行，如同消息已经得到了处理一样，这是因为网络连接恢复时消息传递是有保证的。
- 事务性消息处理 — 将多个相关消息耦合为单个事务，确保消息按顺序传递、只传递一次并且可以从它们的目标队列中被成功地检索。如果出现任何错误，将取消整个事务。
- 安全性 — MessageQueue 组件基于的消息队列技术使用 Windows 安全来保护访问控制，提供审核，并对组件发送和接收的消息进行加密和验证。

**5. 在.Net环境下编写简单的Message Queue程序**

（1）先安装Message Queuing Services

通过Control Panel，“Add/Remove Programs” – “Add/Remove Windows Components”步骤安装MSMQ。

MSMQ可以安装为工作组模式或域模式。如果安装程序没有找到一台运行提供目录服务的消息队列的服务器，则只可以安装为工作组模式，此计算机上的“消息队列”只支持创建专用队列和创建与其他运行“消息队列”的计算机的直接连接。

（2）配置MSMQ

打开Computer Management – Message Queuing，在Private Queues下创建MSMQDemo队列

（3）编写代码－简单演示MSMQ对象

MessageQueue 类是“消息队列”周围的包装。MessageQueue 类提供对“消息队列”队列的引用。可以在 MessageQueue 构造函数中指定一个连接到现有资源的路径，或者可在服务器上创建新队列。在调用 Send、Peek 或 Receive 之前，必须将 MessageQueue 类的新实例与某个现有队列关联。

MessageQueue 支持两种类型的消息检索：同步和异步。同步的 Peek 和 Receive 方法使进程线程用指定的间隔时间等待新消息到达队列。异步的 BeginPeek 和 BeginReceive 方法允许主应用程序任务在消息到达队列之前，在单独的线程中继续执行。这些方法通过使用回调对象和状态对象进行工作，以便在线程之间进行信息通讯。

// Send Message

privatevoidbtnSendMessage_Click(objectsender, System.EventArgse)

{

// Open queue

System.Messaging.MessageQueuequeue = newSystem.Messaging.MessageQueue(".//Private$//MSMQDemo");

// Create message

System.Messaging.Messagemessage = newSystem.Messaging.Message();

message.Body = txtMessage.Text.Trim();

message.Formatter = newSystem.Messaging.XmlMessageFormatter(newType[] {typeof(string)});

// Put message into queue

queue.Send(message);

}

// Receive Message

privatevoidbtnReceiveMessage_Click(objectsender, System.EventArgse)

{

// Open queue

System.Messaging.MessageQueuequeue = newSystem.Messaging.MessageQueue(".//Private$//MSMQDemo");

// Receive message, 同步的Receive方法阻塞当前执行线程，直到一个message可以得到

System.Messaging.Messagemessage = queue.Receive(); 

message.Formatter = newSystem.Messaging.XmlMessageFormatter(newType[] {typeof(string)});

txtReceiveMessage.Text = message.Body.ToString();

}

Demo界面：

![](http://images.cnblogs.com/cnblogs_com/rickie/MSMQ_Demo.GIF)

******

关于MSMQ消息队列介绍文字来自于MSDN.


**. Net环境下消息队列（MSMQ）对象的应用**

关于消息对象（MSMQ）的一些基本概念可以从《消息队列（Message Queue）简介及其使用》查阅，这里归纳在.Net 环境下应用消息队列（MSMQ）开发的一些基本对象和方法。

队列类型及其相应的路径格式：

Public:[MachineName]/[QueueName]

Private:[MachineName]/Private$/[QueueName]

Journal:[MachineName]/[QueueName]/Journal$

Machine journal:[MachineName]/Journal$

Machine dead-letter:[MachineName]/DeadLetter$

Machine transactional dead-letter:[MachineName]/XactDeadLetter$

The first portion of the path indicates a computer or domain name or uses a period (.) to indicate the current computer.

**1. 创建消息队列**

可以手动的方式通过Windows提供的工具创建，或者通过程序的方式创建：

if(MessageQueue.Exists(".//Private$//MSMQDemo"))

queue = newMessageQueue(".//Private$//MSMQDemo");

else

queue = MessageQueue.Create(".//Private$//MSMQDemo");

**2. 发送消息**

缺省情况下，消息序列化XML格式，也可设置为MessageQueue对象的Formatter属性为BinaryMessageFormatter，以二进制格式序列化。

设置消息序列化格式：

if(rdoXMLFormatter.Checked)

queue.Formatter = newXmlMessageFormatter();

else

queue.Formatter = newBinaryMessageFormatter();

发送简单的文本消息：

stringstrMessage = "Hello, I am Rickie.";

queue.Send(strMessage, "Simple text message");

消息队列可以传送简单的文本消息，也可以传送对象消息，但需要满足如下条件：

（1）class必须有一个无参数的公共构造函数，.Net使用这个构造函数在接收端重建对象。

（2）class必须标示为serializable（序列化）。

（3）所有的class属性必须可读写，因为.Net在重建对象时不能够恢复只读属性的属性值，因此只读属性不能够序列化。

发送对象消息（CustomerInfo class需要满足上述条件）：

CustomerInfotheCustomer = newCustomerInfo("0001", "Rickie Lee", "Rickieleemail@yahoo.com");

queue.Send(theCustomer, "Object message");

**3. 读/显示消息**

当消息接受后，消息将从队列中删除。可以通过使用MessageQueue.Peek方法来检索消息队列中的第一个消息的复制，保留消息在队列中。不过，这样只能获取的相同的消息。更好的办法是通过foreach来读消息队列中的消息，但不删除队列中的消息。

foreach(System.Messaging.Message message in queue)

{

txtResults.Text += message.Label + Environment.NewLine;

}

**4. 接收消息**

一般而言，可以通过Receive方法来读取队列中的消息，对于非事务性的队列，优先读取高优先级的消息。如果队列中有多个相同优先级的消息，则以先进先去的方式进行读取消息。对于事务性的队列，则完全以先进先去的方式进行读取消息，忽略消息的优先级。

System.Messaging.MessagereceivedMessage;

receivedMessage = queue.Receive(TimeSpan.FromSeconds(5));

上面采用同步调用，并且一直等到队列中有可用消息或超时过期。

Demo界面(不在提供DEMO程序)：

![](http://images.cnblogs.com/cnblogs_com/rickie/MSMQ_Demo2.gif)

其他相关事项：
- 关于消息的加密、路由等等特性，需要有配置Active Directory的消息队列服务器。
- 为了避免存放消息队列的计算机重新启动而丢失消息，可以通过设置消息对象的Recoverable属性为true，在消息传递过程中将消息保存到磁盘上来保证消息的传递，默认为false。
- 消息发送方和消息接收方需采用相同的序列化格式，如XML或Binary。
- 建议每一个消息队列存放相同类型的消息对象，这样可以省掉获取消息对象后，进行类型判别的麻烦。

**5.消息队列在分布式系统中的应用**

消息队列MSMQ和数据库不一样，消息队列缺乏足够的错误检查能力，并且MSMQ由于需要束缚在windows平台，这些是MSMQ的不足之处。另外，在Production环境中，需要编写大量的代码来进行错误检测和响应。还有大量的死信队列、响应队列和日记队列可能部分在企业不同的计算机上，使得跟踪这些问题或进行诊断变得比较困难。

但是，MSMQ作为组件内部连接比较有用。例如，你可以创建一个XML Web Services使用MSMQ来转发对另一个Server端组件的请求，这种设计巧妙回避了其他异步调用的方法，并且确保可扩展性和性能。

**References:**

1, Matthew MacDonald, Microsoft® .NET Distributed Applications: Integrating XML Web Services and .NET Remoting

2, Rickie, [消息队列（Message Queue）简介及其使用](http://www.cnblogs.com/rickie/archive/2004/11/16/64345.html)



