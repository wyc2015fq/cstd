# 《计算机网络》第一章：基础知识Part II - Koma Hub - CSDN博客
2018年06月05日 13:38:19[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：344
个人分类：[计算机网络](https://blog.csdn.net/Rong_Toa/article/category/7569489)
**内容概要**
计算机网络在信息时代的作用
计算机网络与因特网
计算机网络的发展历史
计算机网络(因特网)的组成
计算机网络的分类
几种常用网络
计算机网络的性能
计算机网络的体系结构
-分层
-计算机网络体系结构的内容
-实体、协议、服务
-层次设计问题
体系结构模型
-关于开放系统互连参考模型OSIRM
-TCP/IP参考模型
-原理模型
网络标准化
**一.计算机网络的体系结构**
**1.复杂的计算机网络及分层**
相互通信的两个计算机系统必须高度协调工作才行，而这种“协调”是相当复杂的。
“分层”可将庞大而复杂的问题，转化为若干较小的局部问题，而这些较小的局部问题就比较易于研究和处理。
WSN例子: HowComplexthe Network is!
![](https://img-blog.csdn.net/20180605132615749)
划分层次的概念举例
![](https://img-blog.csdn.net/20180605132626325)
分层的好处：各层之间是独立的。灵活性好。结构上可分割开。易于实现和维护。能促进标准化工作。
**2.计算机网络的体系结构**
计算机网络的体系结构(architecture)是计算机网络的各层及其协议的集合。
体系结构就是这个计算机网络及其部件所应完成的功能的精确定义。
实现(implementation)是遵循这种体系结构的前提下用何种硬件或软件完成这些功能的问题。
体系结构是抽象的，而实现则是具体的，是真正在运行的计算机硬件和软件。
**3.计算机网络的体系结构主要内容**
Protocol hierarchies协议分层
Design issues for the layers 各层的设计问题
Interfaces and services接口与服务
Connection-oriented and connectionless services面向连接的服务和无连接的服务
Service primitives服务原语
The relationship of services to protocol服务与协议的关系
（1）层数多少要适当
若层数太少，就会使每一层的协议太复杂。层数太多又会在描述和综合各层功能的系统工程任务时遇到较多的困难。
**4.网络协议**
计算机网络中的数据交换必须遵守事先约定好的规则。
这些规则明确规定了所交换的数据的格式以及有关的同步问题（同步含有时序的意思）。
网络协议(network protocol)，简称为协议，是为进行网络中的数据交换而建立的规则、标准或约定。
**5.实体、协议、服务**
实体(entity) 表示任何可发送或接收信息的硬件或软件进程。
协议是控制两个对等实体进行通信的规则的集合。
在协议的控制下，两个对等实体间的通信使得本层能够向上一层提供服务。
要实现本层协议，还需要使用下层所提供的服务。
本层的服务用户只能看见下一层的服务而无法看见下面的协议。
下面的协议对上面的服务用户是透明的。
协议是“水平的”，即协议是控制对等实体之间通信的规则。
服务是“垂直的”，即服务是由下层向上层通过层间接口提供的。
Multilayer Communication
![](https://img-blog.csdn.net/20180605132646146)
Five-layer network，The entities comprising the corresponding layers on different machines.
位于不同系统内同一层次的实体。协议作用在对等实体之间
多层通信的实质：对等层实体之间虚拟通信(非直接数据交换)；下层向上层提供服务；实际通信在最底层完成；
**6.网络协议的组成要素**
语法数据与控制信息的结构或格式。
语义需要发出何种控制信息，完成何种动作以及做出何种响应。
同步事件实现顺序的详细说明。
**7.层次设计问题**
可靠性设计：需要发现并修复错误，如检错（error detection）+retransmissions, 纠错（error correction)
如何处理网络演进：识别发送方与接收方，网络互联，scalable
资源分配：动态共享网络带宽，流量控制（如接收方较慢），拥塞控制，服务质量（QoS）
安全性：如加密，认证等
**8.服务原语（Service Primitives）**
一种服务由其向服务使用者所提供的服务原语（操作）描述。原语告知服务的动作，或向服务用户报告其对等实体的动作。
**二.体系结构参考模型**
**1.两种国际标准**
法律上的(de jure)国际标准OSI 并没有得到市场的认可。
是非国际标准TCP/IP 现在获得了最广泛的应用。
-**TCP/IP 常被称为事实上的(de facto) 国际标准**。
**2.关于开放系统互连参考模型OSI/RM**
PDU: Protocol data unit 协议数据单元
![](https://img-blog.csdn.net/20180605132707381)
**3.物理层Physical Layer**
主要功能——transmitting raw bits over a communication channel.为数据链路层提供一个物理连接，保证信息进入信道并在接收方取下，实现透明地传送比特流。
数据单位——比特。
注意——是OSI的最低层，是网络物理设备之间的接口，但不包括传输介质，而只包括物理设备：包括数据终端设备DTE（如：显示器、PC、工作站等）和数据电路端连接设备DCE（如调制解调器）。
**4.数据链路层Data Link Layer**
主要功能——负责在两个相邻结点间建立、维护和拆除链路，并通过差错控制、流量控制将不太可靠的物理链路改造成无差错的数据链路。提高链路的可靠性，为网络通信提供一条在逻辑上无差错的数据链路。
数据单位——帧。每一帧包括一定数量的数据和一些必要的控制信息。
**5.网络层Network Layer**
主要功能——determining how packets are routed from source to destination. 从源端到目的端中间经过多个中间结点，有很多种走法，网络层负责从中选择出一条最优的路径。即确定分组从源端到目的端如何选择路由。
数据单位——分组或包。
网络层所要处理的问题：
路由选择——要解决的主要问题，考虑因素：最佳性、公平性、简单性、自适应性和稳定性；
网络寻址——路由器用来连接两个或多个不同逻辑网络；
网络互联——当分组要跨越一个网络以到达目的地时，两个网络的寻址方法、分组长度限制以及使用协议等有所不同，需解决异种网络互联问题。
**6.运输层Transport Layer**
为源主机和目的主机的会话层之间建立一条传输通道，用以透明的传送报文。
两种协议：
**TCP **(传输控制协议)：面向连接，数据单位是报文段，提供可靠的交付；
**UDP** (用户数据报协议)：无连接，数据单位是用户数据报，不保证提供可靠的交付。
**7.会话层Session Layer**
主要功能——manage dialogue control, 用户（进程）的入网接口。会话层虽然不参与具体的数据传输，但它却对数据传输进行管理。会话层在两个互相通信的应用进程之间建立、组织和协调起交互活动（即会话）。
会话层起连接作用，在运输服务的基础上增加一些协调管理对话的功能，为上层的表示实体提供更完善的服务。
会话层以下各层都是面向通信的，为上层两个会话实体提供透明、可靠的数据传输，使会话实体在进行会话时，不再考虑通信问题。
具体功能：
会话连接的建立与释放——会话双方必须经过批准，有权参加会话；并确定通信方式；
会话同步服务——为保证当运输连接一旦出故障时，整个会话活动不需要全部重复一遍，允许会话用户在传送的数据中，自由设置同步点，并给同步点赋予同步序号。
会话交互管理——协调和控制多个用户的交互，保证交互动作按序进行，不出现混乱；
**8.表示层Presentation Layer**
主要功能——所处理的是通信双方之间的数据表示问题，包括数据加密与解密、数据压缩和数据转换问题。
**9.应用层Application Layer**
主要功能——是直接面向用户的一层，通过向应用进程提供服务
常用的网络服务——支持万维网的http协议，支持电子邮件的SMTP协议、支持文件传输的FTP协议等。
**三.TCP/IP协议**
TCP/IP 是四层的体系结构：应用层、传输层、互联网层和链路层。
![](https://img-blog.csdn.net/20180605132726227)
**1.TCP/IP参考模型**
Application layer (应用层)：DNS、TELNET、SMTP、FTP、HTTP.
Transport layer (传输层)：与OSI运输层功能相同，TCP（面向连接）和UDP（无连接）
Internet layer (互联网层)：与OSI网络层功能相似，无连接服务，IP为主要协议
Link layer (链路层): 描述了链路必须满足什么功能才能满足上一层的需求。不是真正的一个层，是主机与传输线路的接口。
![](https://img-blog.csdn.net/20180605132733755)
**2.开放系统互连参考模型OSI/RM评价**
OSI/RM是七层的体系结构，明确区分了服务、接口、协议的概念。在市场化方面OSI 却失败了。
糟糕的时机，较TCP/IP晚进入学术市场, 按OSI 标准生产的设备无法及时进入市场
糟糕的技术，7层因政策上的考虑，非技术因素决定，如两层几乎是空的；有些功能在多个层次中重复出现。
糟糕的实现，OSI 的协议实现起来过分复杂，且运行效率很低；
糟糕的政策，政府的强制
![](https://img-blog.csdn.net/20180605132741409)
**3.TCP/IP参考模型评价**
The model does not distinguish the concepts of service, interface, and protocol;
该模型不区分服务，接口和协议的概念;
TCP/IP model is not at all general and is poorly suited to describing any protocol stack other than TCP/IP;
TCP / IP模型根本不是一般的，不太适合描述除TCP / IP以外的任何协议栈;
The link layer is not really a layer at all in the normal sense that the term is used in the context of layered protocols；it is an interface;
在通常的意义上，链接层实际上不是一个层，该术语在分层协议的上下文中使用;它是一个接口;
TCP/IPmodel does not distinguish the physical and data link layers，which are completely different.
TCP / IP模型不能区分完全不同的物理层和数据链路层。
**四.体系结构原理模型**
**1.五层协议的体系结构**
![](https://img-blog.csdn.net/20180605132755696)
**2.主机1向主机2发送数据**
![](https://img-blog.csdn.net/20180605132802612)
**四.网络标准化**
**1.电信领域的标准化组织**
国际电信联盟ITU——标准化国际电信(International telecommunication union)
ITUhas three main sectors：
Radiocommunications Sector 无线通信部门(ITU-R)，协调全球无线电频率利益集团之间的竞争使用；
Telecommunications standardization Sector 电信标准化部门(ITU-T)，主要关注电话、电报和数据通信系统
Development Sector 发展部门(ITU-D)，促进信息和通信技术的发展。
**2.国际标准领域有影响力的组织**
国际标准化组织ISO(International standards organization)
电器和电子工程师协会IEEE(Institute of electrical and electronics engineers)世界上最大的专业组织
**3.Internet的标准化组织**
InternetEngineeringTaskForce（IETF）：IETF负责Internet协议的研发和改进。IETF被分为很多个工作组（workinggroups），他们提交的文档称为RFC（RequestForComments）。
IRTF（InternetResearchTaskForce）：IRTF由一些专注于某个领域长期发展的研究小组组成。
InternetArchitectureBoard（IAB）：IAB负责定义Internet的整体框架，为IETF提供大方向上的指导。
InternetEngineeringSteeringGroup（IESG）：IESG在技术方面管理IETF的活动，负责Internet标准的制定过程。
**4.Internet标准的制定过程**
所有的标准以RFC的形式发布出来，可以从www.ietf.org免费获得。但不是所有的RFC都是Internet标准。
标准形成的一般步骤是：
-InternetDrafts
-RFCs
-ProposedStandard
-DraftStandard（需要两个可以工作的实现）
-InternetStandard（由IAB发布）
此文章为笔记，为了学习交流，如有侵权，请联系删除。
