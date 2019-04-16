# Rethinking IoT - 我相信...... - CSDN博客





2018年03月27日 00:00:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1087标签：[IoT																[协议																[架构](https://so.csdn.net/so/search/s.do?q=架构&t=blog)
个人分类：[IoT																[架构](https://blog.csdn.net/wireless_com/article/category/2540035)](https://blog.csdn.net/wireless_com/article/category/7459858)

所属专栏：[IoT与智能硬件](https://blog.csdn.net/column/details/19754.html)






在地铁阅读时光里读完了《重构物联网的未来》一书，稍感失望。该书的英文原著书名为：《Rethinking The Internet of Things-- A Scalable Approach to Connecting Everything 》，译为“物联网的反思”仿佛更为贴近书中的内容，自己的很多问题并没有从书中得到答案。但是，该书既然获得了 Jolt Award的提名，还是有些有价值的东西的。

![](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrsibsoiaFXicpvNyKSQQ5wO5wlz6VZUyyNbTb9vAYAPFILN0epNKxtfDcEiajqFelT730ib5vAFJ0pfUg/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)

（自己拍照太烂，图片来自百度百科）

## 一个假设

作者的一个假设是：边缘网络的连接质量没法保证，经常出现间歇性和不稳定，是小数据量、低频传输且高时延的网络。

也就是说，边缘网络不再提供设备性能的保护，不能提供超配置容量、界定端到端的连接以及管理基础设施。既然物联网具备边缘通信技术的特征，那么，其架构也因此需要更为有机的组网策略，轻量级设备需要在边缘网络中自力更生。

## 物联网分层架构

书中给出的物联网体系结构并没有标新立异的地方，从这一意义上说，确实有些重构的味道。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrsibsoiaFXicpvNyKSQQ5wO5wDGNfBDDL5ibpdjMd8AhiaI3iaPaw94nosgbereV1XkFegYia1njnibW9UJQ/640?wx_fmt=jpeg)

### 终端设备：物联网边缘的海量终端设备

物联网设备实际上可以是任何依靠电信号(或者通过热能、运动、光能等转化为电信号）运行的物质。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrsibsoiaFXicpvNyKSQQ5wO5wfn7ALTkCu1c4psoC553q0uib2xenkpdDB6iciaibePclJUIqIC3f5ZJ3iaA/640?wx_fmt=jpeg)

### 转发单元：提供传统互联网的数据传输及网关功能

转发节点堪称物联网架构的核心模块，必须为任务紧急的远程系统提供最终的控制决策权。每一个独立节点都有能力通过与其他节点协商，从而创建一个无环结构化路径。转发节点通过邻区信息共享建立有效的路由机制，对潜在的传输链路进行决策抽象。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrsibsoiaFXicpvNyKSQQ5wO5wlwkgVLdwf9AXnFCt2kKBFwJXiaFGibiaNo7eZBGPjV9RVvXJchibWUZ8bA/640?wx_fmt=jpeg)

### 汇聚单元：提供数据分析、控制以及物联网人机入口

汇聚单元指导内置智能发布体的转发节点进行智能化管理，具有广泛的架构定义和丰富的应用功能，存在着不同种类的社区。汇聚单元成为一个将数据转化为人类所理解信息的重要接口，用于生成现场状态的分析报告，或者估算某些事件的阈值，并发出告警等。拥有社交网络的汇聚单元更具备更广泛的事件和趋势认知能力。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrsibsoiaFXicpvNyKSQQ5wO5wr1PJrt0SibRicIHSXP9icmaJ1m6PhhJgTvIpukgbhTMcj6ptjDUITN8Sg/640?wx_fmt=jpeg)

## 协议创新

作者认为IPv6的思路可能混淆了地址空间和寻址能力的区别。IPv6需将协议栈置于设备存储器，在内存中执行，并由处理器驱动，IPv6 的边际成本可能微乎其微。廉价的设备难以承载传统协议，IPV6 支持1280字节的MTU，但是利用率在物联网场景会很低。

基于此，作者给出了物联网架构和协议设计的一些原则：
- 
限定尽可能少，从而建立更加开放的创新空间；

- 
系统容错设计，不要企图消除一切差错，而是学会包容；

- 
关于分层的组网功能与复杂度，一切都以需求为前提；

- 
利用自然现象，以简单的理念构建复杂的系统；

- 
从实时数据中提取有价值的信息；


简单的说，作者提出的设计准则是以非常低的成本和复杂度提供足够的通信连接，并采用“发布/订阅”模型。

进而，作者创造性的提出了极简边缘协议——小数据包的chirp协议。该协议仅包含：
- 
最小的开销负载，传输指向，简单的“非唯一”地址及适度的校验和；

- 
是一种独立的非紧急设计模式

- 
不包含重传或应答协议


这是一个轻量级协议，一个chirp消息仅包含一个地址、短数据段和校验和。这些设备近用于广播和收听，本地相关性才是最重要的。chirp信号通过外部标签进行自分类，而私有标签可定制可扩展，可用于寻址。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrsibsoiaFXicpvNyKSQQ5wO5wFHhIOgxBMncvny2xoGNLtSedia6ficjU42WluTUbGTAVVjX3jiawPFXCg/640?wx_fmt=jpeg)

## 物联网实施的要点

与传统物联网的趋势相反，物联网设备产生的有价值数据会越来越少，物联网并不等同于P2P的网络。

参照自然界群体是一个高度优化的系统，其中包括自然界原始的大数据，个体自制，兴趣社区，以及面向接受者的通信方式。

物联网系统是无人值守的，支持临时的自组织设备以应对不确定性边缘，通过数量增加可靠性。为物联网选择了树状网络结构，具备可扩展性且有效的自组织网络架构。下图是个新零售的示例：

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrsibsoiaFXicpvNyKSQQ5wO5waYibkWVSNu56jchbLsppWYoYsg6KIOTkzFBN8iaN8EsoRbaCwibUJH1Og/640?wx_fmt=jpeg)

物联网在部署的时候应具备：
- 
最小化终端设备组网需求

- 
提供本地自主决策能力

- 
分布式智能体为终端设备和汇聚单元减负

- 
灵活的“发布/订阅”模型创建信息社区


作者还建议基于开源软件实现，例如openwrt， 它基于linux内核，为嵌入式设备提供给各种网络路由业务。

## 一点儿反思

感到遗憾的是，作者并没有对物联网给出相对明确的定义和分类。比如消费着物联网和工业物联网的差异是什么，在体系结构上有哪些具体的不同？同样， 也没有对端设备按照资源和场景进行分类，例如参照Java的用法（J2EE，J2SE，J2ME，Java card 等）对物联网的物进行分类。其创新协议可能更适合智能颗粒或智能尘埃之类的感知类终端，或者说是传感器网络的终端。

投身于物联网，可能需要一个更为全面的全局性视图，才可能建立物联网的系统性架构体系，进而构建一个面向物联网的全栈式知识体系，最终让物联网真正地服务于人们的生产和生活。

士不可以不弘毅，任重而道远。同时，纸上得来终觉浅，绝知此事要躬行。




（图片来自百度百科）](https://so.csdn.net/so/search/s.do?q=协议&t=blog)](https://so.csdn.net/so/search/s.do?q=IoT&t=blog)




