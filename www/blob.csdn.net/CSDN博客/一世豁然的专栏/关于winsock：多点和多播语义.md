# 关于winsock：多点和多播语义 - 一世豁然的专栏 - CSDN博客





2018年10月12日 10:28:42[一世豁然](https://me.csdn.net/Explorer_day)阅读数：47








在考虑如何在Windows套接字2（Winsock）中支持多点和多播语义时，许多现有和提议的方案（包括IP多播，ATM点对多点连接，ST-II，T.120，H.320- MCU）进行了检查。 为了能够对各种方案进行连贯的讨论，首先要创建一个分类，以表征每个方案的基本属性，这是很有价值的。 在本文档中，术语“多点”表示多点和多播。



一、多点分类法

本节中描述的分类法首先将处理自身的控制平面与建立多点会话的方式从处理会话参与者之间的数据传输的数据平面区分开来。

**控制平面中的会话建立**

在控制平面中，有两种不同类型的会话建立：
- 根
- 没有根

在有根控制的情况下，存在一个特殊参与者c_root，它与该多点会话的其余成员不同，每个成员都被称为c_leaf。 c_root必须在多点会话的整个持续时间内保持存在，因为会话在没有c_root的情况下被中断。 c_root通常通过建立与c_leaf或多个c_leaf的连接来启动多点会话。 c_root可能会添加更多c_leaf，或者（在某些情况下）c_leaf可以在以后加入c_root。 有根控制平面的例子可以在ATM和ST-II中找到。

对于非根控制平面，属于多点会话的所有成员都是叶子，也就是说，不存在充当c_root的特殊参与者。 每个c_leaf必须将自己添加到始终可用的预先存在的多点会话中（如在IP多播地址的情况下），或者已通过某些超出Windows范围的带外（OOB）机制设置 套接字规范。

另一种看待这种情况的方法是c_root仍然存在，但可以认为是在网络云中，而不是其中一个参与者。 因为控制根仍然存在，所以也可以认为无根控制平面是隐式根。 这种隐式根多点方案的示例如下：
- 电话会议桥。
- IP组播系统。
- H.320视频会议中的多点控制单元（MCU）。

**数据平面中的数据传输**

在数据平面中，有两种类型的数据传输样式：
- 根
- 没有根

在有根数据平面中，存在一个名为d_root的特殊参与者。 数据传输仅发生在d_root和此多点会话的其余成员之间，每个成员都称为d_leaf。 流量可以是单向的或双向的。 从d_root发出的数据被复制（如果需要）并传递到每个d_leaf，而来自d_leafs的数据仅传递给d_root。 在根数据平面的情况下，d_leaf之间不允许流量。 植根于数据平面的协议的示例是ST-II。

在非根数据平面中，所有参与者都是相等的，即，他们发送的任何数据都被传递给同一多点会话中的所有其他参与者。 同样，每个d_leaf节点都可以从所有其他d_leaf接收数据，在某些情况下，可以从未参与多点会话的其他节点接收数据。 没有特殊的d_root节点。 IP多播在数据平面中是无根的。

请注意，数据单元重复发生的位置，或者共享单个树或多个最短路径树是否用于多点分发的问题是协议问题，与应用程序用于执行多点通信的接口无关。 因此，本附录或Windows套接字界面中未解决这些问题。

下表描述了上述分类法，并指出了现有方案如何适合每个类别。 注意，似乎没有任何现有的方案采用无根控制平面以及有根数据平面。
|Rooted control plane|Nonrooted (implicit rooted) control plane| |
|----|----|----|
|Rooted data plane|ATM, ST-II|No known examples.|
|Nonrooted data plane|T.120|IP-multicast, H.320 (MCU)|





二、用于多点和多播的Windows套接字2接口元素

已合并到Windows套接字2中以利用多点功能的机制可归纳如下：
- WSAPROTOCOL_INFO结构中的三个属性位。
- 为WSASocket的dwFlags参数定义的四个标志。
- 一个函数WSAJoinLeaf，用于将叶节点添加到多点会话中。
- 两个WSAIoctl命令代码，用于控制多点环回和多播传输的范围。

以下部分更详细地描述了这些界面元素：
- 连接多点子叶的语义
- 现有多点协议如何支持这些扩展



一）、WSAPROTOCOL_INFO结构中的属性

为了支持上述分类，WSAPROTOCOL_INFO结构中的三个属性参数分别用于区分控制和数据平面中使用的方案：
- XP1_SUPPORT_MULTIPOINT的值为1表示此协议条目支持多点通信，并且以下两个参数是有意义的。
- XP1_MULTIPOINT_CONTROL_PLANE指示控制平面是否为root（value = 1）或nonrooted（value = 0）。
- XP1_MULTIPOINT_DATA_PLANE指示数据平面是否为root（value = 1）或nonrooted（value = 0）。

请注意，如果多点协议同时支持有根和无根数据平面，则每个条目都存在两个WSAPROTOCOL_INFO条目。

应用程序可以使用WSAEnumProtocols来发现给定协议是否支持多点通信，如果支持，则分别如何支持控制和数据平面。



二）、WSASocket的标志位

在某些情况下，连接到多点会话的套接字可能在点对点套接字的行为方面存在一些差异。 例如，根数据平面方案中的d_leaf套接字只能向d_root参与者发送信息。 这使得应用程序需要能够指示与其创建一致的套接字的预期用途。 这是通过四个标志位完成的，可以在dwFlags参数中设置为WSASocket：
- WSA_FLAG_MULTIPOINT_C_ROOT，用于创建充当c_root的套接字，并且仅在相应的WSAPROTOCOL_INFO条目中指示有根控制平面时才允许。
- WSA_FLAG_MULTIPOINT_C_LEAF，用于创建充当c_leaf的套接字，并且仅在相应的WSAPROTOCOL_INFO条目中指示XP1_SUPPORT_MULTIPOINT时才允许。
- WSA_FLAG_MULTIPOINT_D_ROOT，用于创建充当d_root的套接字，并且仅在相应的WSAPROTOCOL_INFO条目中指示有根数据平面时才允许。
- WSA_FLAG_MULTIPOINT_D_LEAF，用于创建充当d_leaf的套接字，并且仅在相应的WSAPROTOCOL_INFO条目中指示XP1_SUPPORT_MULTIPOINT时才允许。


请注意，在创建多点套接字时，必须在WSASocket的dwFlags参数中设置两个控制平面标志中的一个，以及两个数据平面标志中的一个。 因此，创建多点套接字的四种可能性是：
- "c_root/d_root"
- "c_root/d_leaf"
- "c_leaf/d_root"
- "c_leaf /d_leaf"



三）、SIO_MULTIPOINT_LOOPBACK WSAIoctl的命令代码

当在非根数据数据平面中使用d_leaf套接字时，希望将发出的流量收回到同一套接字上。 WSAIoctl的SIO_MULTIPOINT_LOOPBACK命令代码用于启用或禁用多点流量的环回。



四）、WSIOoctl的SIO_MULTICAST_SCOPE命令代码

当使用多播时，通常需要指定多播应发生的范围。 范围定义为要覆盖的路由网段数。 范围为零表示多播传输不会放在线路上，但可以在本地主机内的套接字上传播。 范围值1（默认值）表示传输将放置在线路上，但不会跨越任何路由器。 较高的范围值决定了可以越过的路由器的数量。 请注意，这对应于IP多播中的生存时间（TTL）参数。

函数WSAJoinLeaf用于将叶节点连接到多点会话。 有关如何使用此功能的讨论，请参阅以下内容。





三、连接多点子叶的语义

在下文中，通常通过在两个平面之一（控制或数据）中定义其角色来描述多点套接字。 应该理解，这个相同的插座在另一个平面中具有作用，但是为了使参考文献保持简短，没有提到这一点。 例如，当引用“c_root套接字”时，这可以是c_root / d_root或c_root / d_leaf套接字。

在有根控制平面方案中，新叶节点以两种不同方式中的一种或两种方式添加到多点会话。 在第一种方法中，root使用WSAJoinLeaf来启动与叶节点的连接并邀请它成为参与者。 在叶节点上，对等应用程序必须已创建c_leaf套接字并使用listen将其设置为侦听模式。 当被邀请加入会话时，叶节点接收FD_ACCEPT指示，并通过调用WSAAccept来表示其加入的意愿。 然后，根应用程序在连接操作完成时接收FD_CONNECT指示。

在第二种方法中，角色基本上是相反的。 根应用程序创建一个c_root套接字并将其设置为侦听模式。 希望加入会话的叶节点创建一个c_leaf套接字并使用WSAJoinLeaf来启动连接并请求准入。 当进入的准入请求到达时，根应用程序接收FD_ACCEPT，并通过调用WSAAccept来接受叶子节点。 叶子节点在被允许时接收FD_CONNECT。

在非根控制平面中，所有节点都是c_leaf，WSAJoinLeaf用于启动将节点包含到现有多点会话中。 当连接已完成且返回的套接字描述符在多点会话中可用时，提供FD_CONNECT指示。 在IP多播的情况下，这将对应于IP_ADD_MEMBERSHIP套接字选项。

（熟悉IP多播使用无连接UDP协议的读者可能会关注这里介绍的面向连接的语义。特别是在UDP套接字上使用WSAJoinLeaf并等待FD_CONNECT指示的概念可能会令人不安。但是， 将面向连接的语义应用于无连接协议的大量先例。例如，允许并且有时在UDP套接字上调用标准连接函数是有用的。将面向连接的语义应用于无连接套接字的一般结果是如何限制 这样的套接字也可以使用，这也是这种情况.WSAJoinLeaf中使用的UDP套接字会有一定的限制，等待FD_CONNECT指示（在这种情况下只是表明已经发送了相应的IGMP消息）是 一个这样的限制。）

因此，有三个实例应用程序将使用WSAJoinLeaf作为：
- 多点root并邀请新叶加入会话
- Leaf向根节点多点会话发出准入请求
- Leaf寻求进入无根多点会话（例如，IP多播）

一）、使用WSAJoinLeaf

如前所述，函数WSAJoinLeaf用于将叶节点连接到多点会话。 WSAJoinLeaf具有与WSAConnect相同的参数和语义，除了它返回套接字描述符（如在WSAAccept中），并且它还有一个额外的dwFlags参数。

dwFlags参数用于指示套接字是仅作为发送方，仅作为接收方，还是两者兼有。 在此功能中，只有多点套接字可用于输入参数。 如果多点套接字处于非阻塞模式，则在收到相应的FD_CONNECT指示之前，返回的套接字描述符不可用。 多点会话中的根应用程序可以调用WSAJoinLeaf一次或多次以便添加多个叶节点，但是一次最多只有一个多点连接请求可能是未完成的。

WSAJoinLeaf返回的套接字描述符根据输入套接字描述符s是c_root还是c_leaf而不同。 当与c_root套接字一起使用时，name参数指定要添加的特定叶节点，并且返回的套接字描述符是与新添加的叶节点对应的c_leaf套接字。 它不是用于交换多点数据，而是用于接收特定c_leaf存在的连接的FD_XXX指示（例如，FD_CLOSE）。 一些多点实现还可以允许该套接字用于根和单个叶节点之间的侧聊。 如果相应的叶节点调用closesocket以退出多点会话，则接收该套接字的FD_CLOSE指示。 对称地，从WSAJoinLeaf返回的c_leaf套接字上调用closesocket会导致相应叶节点中的套接字获得FD_CLOSE通知。

当使用c_leaf套接字调用WSAJoinLeaf时，name参数包含根应用程序的地址（对于有根控制方案）或现有多点会话（无根控制方案），并且返回的套接字描述符与输入套接字描述符相同。 在根控制方案中，根应用程序通过调用listen将其c_root套接字置于侦听模式。 当叶节点请求将自身连接到多点会话时，传递标准FD_ACCEPT通知。 根应用程序使用通常的accept / WSAAccept函数来允许新的叶节点。 从accept或WSAAccept返回的值也是一个c_leaf套接字描述符，就像从WSAJoinLeaf返回的那样。 为了适应允许根启动和叶启动连接的多点方案，可以将已经处于侦听模式的c_root套接字用作WSAJoinLeaf的输入。

多点根应用程序通常负责有序地拆除多点会话。 这样的应用程序可以在c_root套接字上使用shutdown或closesocket来使所有关联的c_leaf套接字（包括从WSAJoinLeaf返回的套接字及其在远程叶节点中的相应c_leaf套接字）获得FD_CLOSE通知。





四、多点套接字与常规套接字之间的语义差异

在控制平面中，c_root套接字和常规的点对点套接字之间存在一些重要的语义差异：
- 可以在WSAJoinLeaf中使用c_root套接字来加入新的叶子。
- 将c_root套接字置于侦听模式（通过调用listen）并不排除在调用WSAJoinLeaf时使用c_root套接字来添加新叶，或者用于发送和接收多点数据。
- 关闭c_root套接字会导致所有关联的c_leaf套接字获得FD_CLOSE通知。

c_leaf套接字和控制平面中的常规套接字之间没有语义差异，除了可以在WSAJoinLeaf中使用c_leaf套接字，并且在listen中使用c_leaf套接字表示只应接受多点连接请求。

在数据平面中，d_root套接字和常规点对点套接字之间的语义差异是：
- 在d_root套接字上发送的数据将传递到同一多点会话中的所有叶子。
- 在d_root套接字上接收的数据可以来自任何叶子。

根数据平面中的d_leaf套接字与常规套接字没有语义差异，但是，在非根数据平面中，在d_leaf套接字上发送的数据将转到所有其他叶节点，并且接收的数据可能来自任何其他叶子节点。 如前所述，有关d_leaf套接字是否位于有根或无根数据平面的信息包含在套接字的相应WSAPROTOCOL_INFO结构中。





五、现有多点协议如何支持这些扩展

以下主题说明如何通过Windows套接字2多点功能访问IP多播和ATM点对多点功能。 我们选择这两个作为例子，因为它们很受欢迎且很容易理解。
- [IP Multicast](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/ip-multicast-2)
- [ATM Point to Multipoint](https://docs.microsoft.com/zh-cn/windows/desktop/WinSock/atm-point-to-multipoint-2)



一）、IP Multicast

IP组播属于非根数据平面和非根控制平面的类别。 所有应用程序都发挥着作用。 目前，大多数IP多播实现使用Steve Deering向Internet工程任务组（IETF）提出的一组套接字选项。 因此提供了五种操作：
- IP_MULTICAST_TTL - 设置生存时间，控制多播会话的范围。
- IP_MULTICAST_IF - 确定用于多播的接口。
- IP_ADD_MEMBERSHI - 加入指定的多播会话。
- IP_DROP_MEMBERSHIP - 丢弃多播会话。
- IP_MULTICAST_LOOP - 控制多播流量的环回。

设置IP多播套接字的生存时间直接映射到使用WSAIoctl的SIO_MULTICAST_SCOPE命令代码。

确定用于多播的IP接口的方法是通过特定于TCP / IP的套接字选项，如Windows套接字2协议特定附件中所述。 其余三个操作都包含在此处描述的Windows套接字2语义中。

应用程序将在WSASocket中打开带有c_leaf / d_leaf标志的套接字。 它将使用WSAJoinLeaf将其自身添加到为多播操作指定的默认接口上的多播组。 如果WSAJoinLeaf中的标志指示此套接字仅是发送方，则加入操作本质上是无操作，并且不需要发送IGMP消息。 否则，将IGMP分组发送到路由器以指示对接收发送到指定多播地址的分组的兴趣。 由于应用程序创建了仅用于执行多播的特殊c_leaf / d_leaf套接字，因此标准的closesocket函数将用于退出多播会话。 WSAIoctl的SIO_MULTIPOINT_LOOPBACK命令代码提供了一种通用控制机制，用于确定是否也可以在同一套接字上接收在非根数多点方案中在d_leaf套接字上发送的数据。

> 
注意

Winsock版本的IP_MULTICAST_LOOP选项在语义上与UNIX版本的IP_MULTICAST_LOOP选项不同：

- 在Winsock中，IP_MULTICAST_LOOP选项仅适用于接收路径。
- 在UNIX版本中，IP_MULTICAST_LOOP选项适用于发送路径。

例如，应用程序ON和OFF（比X和Y更容易跟踪）在同一接口上加入同一组; 应用程序ON将打开IP_MULTICAST_LOOP选项，应用程序OFF将关闭IP_MULTICAST_LOOP选项。 如果ON和OFF是Winsock应用程序，OFF可以发送到ON，但ON不能发送到OFF。 相反，如果ON和OFF是UNIX应用程序，ON可以发送到OFF，但OFF不能发送到ON。



二）、ATM Point to Multipoint

ATM属于有根数据和有根控制平面的范畴。 作为根的应用程序将创建c_root套接字，并且在叶节点上运行的对应项将使用c_leaf套接字。 根应用程序使用WSAJoinLeaf添加新的叶节点。 叶节点上的相应应用程序将其c_leaf套接字设置为侦听模式。 指定了c_root套接字的WSAJoinLeaf将映射到Q.2931 ADDPARTY消息。 ATM UNI 3.1不支持叶启动的连接，但ATM UNI 4.0支持。 因此，指定了c_leaf套接字的WSAJoinLeaf将映射到相应的ATM UNI 4.0消息。

对于ATM点对多点，还需要考虑其他因素：
- 向ATM点对多点会话添加新的子叶仅基于邀请。 root通过调用WSAJoinLeaf函数来邀请子叶 - 它们已经接受了函数调用。
- ATM点对多点会话中的数据流仅来自根到叶;子 叶不能使用相同的会话向根发送信息。
- 每个ATM适配器只允许一个子叶。



