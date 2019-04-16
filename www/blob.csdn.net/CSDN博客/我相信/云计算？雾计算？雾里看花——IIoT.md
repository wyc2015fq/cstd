# 云计算？雾计算？雾里看花——IIoT - 我相信...... - CSDN博客





2018年03月09日 00:00:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：840
所属专栏：[IoT与智能硬件](https://blog.csdn.net/column/details/19754.html)










工业物联网(IIoT)正在从概念逐渐变成现实。廉价传感器, 从数据到云端, 以及在浏览器上的仪表盘可以看到更细致和更现实的模型，仿若无处不在。 一些文章通过对雾或边缘计算的讨论也探讨了这一趋势[1]。 这是一个很好的开始，但并不完整。 成熟的 IIoT 系统也将包括雾计算，这是一种接近传感器的计算方法, 超出了通常认为的雾计算。 在任何给定的应用程序中, IIoT的部署可能包括这些概念的混合, 包括雾云、雾雾和雾雾云等等。 随着在微控制器、芯片系统和低成本通信能力的不断提高, 薄雾计算将成为数百万解决方案的重要组成部分。

## 丢失的遥控器

有一则电视广告显示, 一家人争先恐后地寻找电视遥控器。 在这一情景下, 一个家人使用亚马逊的 Alexa 选择了正确的频道。 处理请求并将命令发送到 Dish TV Hopper。 没过多久, 这家人就安顿下来看电视了, 遥控器仿佛被遗忘了。

处理电视控制所花的时间可能比找到错误的遥控器所需要的时间要快(特别是如果狗把遥控器带到另一个房间的话)。 这是好事, 但是如果核电站以这种方式控制呢？ 化工厂的空中交通管制系统或紧急警报系统会是这样吗？

这个例子说明了物联网至少分裂成两个主要模型: 消费者模型和工业模型。 消费者模式通常包括数据集中化, 然后在云中进行决策。 这是一个完美的模式, 提供免费或低成本的功能或服务, 在这些服务中收集的大量数据可以通过其他方式实现货币化。 典型的例子是谷歌, 搜索或电子邮件是免费提供的, 但所收集的数据在广告服务中被货币化呈现。 还有些不太明显的例子, 比如智能电表。 计价器在你的房间里, 但是数据是集中收集的, 用户只能通过一个经过精心设计的公用网站才能访问。 多数数据被公用事业用于其他目的, 主要是降低成本、需求响应、故障诊断和系统规划(图1)。

![](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqWicfCFQDbGcou5iaRA1CCgZ7XDwxx2picmbfa5S748BvKJkbjc1VEFHHnkTw8qgENyQhvvtTiaVc5cw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)

图1 | 工业物联网与消费者物联网的对比

## IIoT的不同点

在 IIoT 应用中还有其他的考量: 控制循环中的延迟, 决策过程中涉及的一系列元素, 数据传输和存储成本, 以及敏感操作数据的安全性等等。

为了缓解这些矛盾, ARM、 Cisco、 Intel、 Microsoft 和其他公司已经提出了将边缘计算作为一种替代方案。 在这里, 一个边缘设备或一组边缘设备包含业务逻辑, 可以在本地或区域内作出决策, 而不需要参考或与中心核心合作。 这个概念通常被称为雾计算, 它注意到了分散的性质, 并将它与集中的云服务区分开来。 雾计算可能涉及一个单边设备或多个边缘设备一起操作。 有许多组合, 大多数示例将与云资源一起工作(图2)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqWicfCFQDbGcou5iaRA1CCgZRo6DSicxlvnjBfJdojhkmqboC6aqs1gd3fq02f3qHqYl8aKaHRZSpOQ/640?wx_fmt=png)

图2 | IIoT 架构中雾、薄雾和云计算之间的关系。最常见的无线连接与手持和网络人机界面(HMI)一起被识别出来。




然而, 还有另一个重要的因素。 通过收集所有的传感器数据, 有可能以相关和不相关的数据混合来压垮系统。 系统收到了如此多的数据, 以至于很难弄清楚该如何处理。 这与飞行员在飞机驾驶舱或者医院重症监护室的医务人员所遭受的"惊恐疲劳"问题相似。

更好的方法是从传感器数据中获取智能, 只将情报传送到决策系统(雾或云)。 在理想情况下, 智能是靠近传感器的, 而不是在边缘或云计算的位置。 这个概念被称为薄雾（mist）计算(图3)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqWicfCFQDbGcou5iaRA1CCgZp9ItA1VvP21gOA7ooOx3gia6QoJU87QYvibTH3FctUJaJP0DfpfokT8Q/640?wx_fmt=png)

图3 | 数据, 智能 和洞察力。 传感器数据与雾计算资源共享，派生的智能传递给薄雾计算资源。 只有基本的智能和 / 或数据被发送到云端。

这一想法是使用低成本的微控制器来做更多的事情, 而不仅仅是数据转换和简单的通信。 处理能力被用来观察来自多个传感器的数据流, 并得出结论或复杂的见解。 同时，也可以观察传感器本身的状况。 这种方法可能会使系统进一步了解该地点正在发生的情况或协助维修周期。

## 传感器平台进行助力

幸运的是, 像 cratus / fujitsu BlueBrain 系统这样的传感器平台, 以及像 ARM Cortex 系列这样强大的微控制器家族, 使得这种方法经济而直接。 这样的平台包含了传感器、 i / o、计算资源、通信和开发资源的组合, 使得为个别问题或应用解决方案的原型变得更加容易。 如果所需的体积很小, 传感器平台可以作为最终的解决方案。 如果体积很大, 则可以通过降低平台硬件和软件的成本来精心设计一个自定义设计(图4)。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/DE2dk1GjczqWicfCFQDbGcou5iaRA1CCgZ7TAKtLNJQ69Psrn1zRPzkfibiaOM35wyCnrcpy75jYiccHZPcibtE7gmCQ/640?wx_fmt=png)

图4 | cratus/fujitsu BlueBrain传感器平台包含了一系列传感器和执行器, 以及 MCU、通信和固件框架，被用来为物联网应用程序创建一个快速的概念验证。

在去年（(2017年6月)的传感器博览会上 , CRATUS 和富士通通过将两个BlueBrian传感器平台和 Microsoft HoloLens 耳机, 展示了一个薄雾计算的例子。 这种扩增实境(AR)的应用使用了来自 BlueBrain 平台的传感器数据, 直接覆盖在透过全息透镜所看到的视觉上, 并在 Cortex M4处理器上实现, 没有额外的边缘、手持或云计算。 这个例子说明了在工业环境中如何提供反馈和控制。在这种环境中, 视野的复杂性使得很难区分因果关系。 这种系统直接支持人与机器的合作, 并减少危险环境中的不安因素。

## 自寻烦恼么？

云平台随处可见, 所以人们可能会问:"何苦自寻烦恼呢?" 。 最近的研究提供了一些佐证: 在通讯、电力以及隐式存储成本等方面都有节约。 在 IEEE 最近的一篇文章[5]中, Markakis 等人比较了从纯云到纯边缘的三种不同方法, 证明了这一方式可以节省大量的资金。

使用靠近传感器的计算资源可以获得类似的好处。 在这种情况下, 通信通常是无线的, 因此除了带宽的好处之外, 还将减少预计部署的数十亿装置的无线电频率噪声和干扰。

### 为什么不只是云？

主要原因如下:
- 
Latency 网络延迟

- 
Privacy 隐私

- 
Security 安全

- 
Reliability 可靠性

- 
Critical infrastructure 关键的基础设施

- 
Persistent connection 持久的连接

- 
Storage and retrieval cost 存储和检索成本


网络延迟是从传感器到云的往返时间。 由于通信系统存在着不确定性，那么数据中心离云服务器多远合适？ 云服务器是否负载过多？ 对于时间紧迫的情况来说, 纯粹的云服务或许并不是最理想的。

数据隐私是另一个很大的问题。 敏感的商业信息可能嵌入到数据中, 竞争者可以通过访问这些数据获得优势。 如果一个掠夺者在股票市场上获得洞察力并使用它会是怎样情形？

任何时候, 安全都是永恒的话题。 如果数据被发送到一个远程位置进行分析和存储, 那么系统输入就会比边缘设置多得多。 如果数据涉及关键的操作信息, 操作或企业的整体安全性可能会受到影响。

云解决方案的可靠性较低。 在外界条件相同的情况下, 更多的设备意味着更低的可靠性。

目前需要评估关键行业基础设施的脆弱性，这可能是政府必须做的事情, 并且应该有相应的罚款和其他的处罚措施。

持久连接可能并不总是可用的。 当云在某段时间100%不可用时会发生什么？

发送到云端的不必要数据和信息在电力和通信方面有成本, 而且存储和检索(通信及访问)也是需要成本的。

## 薄雾计算

当前多数的 IoT 和 IIoT 解决方案是一维的, 通常用于处理一个需求或用户用例。 物联网技术的真正好处在于多个系统的合作（要点是互操作性）, 以获得更大的前景。 对于全部的使用场景而言, 难以在云中全部完成。 雾技术和薄雾计算装置需要更多的灵活性，必须对在初始部署后很长时间内提供对附加功能的开放。

微控制器的能力日益强大, 以及在软件定义传感器方面的最新进展[6]，可能暂时难以把握当前市场上的薄雾计算能力, 就像在早期的手机市场上很难把握智能手机一样。

理解未来体系结构的一个较好例子是目前推出的车对车和车对基础设施(V2I)系统。 这个基础设施看起来像云, 但是车辆之间会相互通信, 并且在没有基础设施参与的情况下做出决策，这些都是雾计算装置。 进一步，如果汽车中的单个 ECU 也能在不考虑整个汽车系统的情况下做出决策，决定它们自己的子系统动作，这就是薄雾计算了。

### References
- 
“Internet of Things Leaders Create Open Fog Consortium to Help Enable End-to-End Technology Scenarios for the Internet of Things.” (2015, November 19). https://iotbusinessnews.com/2015/11/19/80306-internet-of-things-leaders-create-openfog-consortium-to-help-enable-end-to-end-technology-scenarios-for-the-internet-of-things/.

- 
McMillin, Bruce et al. (2017, February). “Fog Computing for Smart Living.” IEEE Computer Magazine, Vol 50, No 2, page 5.

- 
Wald, Matthew L. (2010, July 31). “For No Signs of Trouble, Kill the Alarm.” New York Times. http://www.nytimes.com/2010/08/01/weekinreview/01wald.html.

- 
“FUJITSU Component Sensor-Based System BlueBrain(r) Interface Board.” (2017, August 2). http://www.fujitsu.com/downloads/MICRO/fcai/wireless-modules/bluebrain-interface-board.pdf.

- 
Markakis, Evangelos K. et al. (2017, July). “EXEGESIS: Extreme Edge Resource Harvesting for a Virtualized Fog Environment.” IEEE Communications Magazine, pp 173-179.

- 
Gunay, Z. (2017, October, 31) “Software Defined Sensors for Industrial IoT and Industrie 4.0.” www.cratustech.com/downloads/.


编译自 http://www.embedded-computing.com/automotive/getting-misty-eyed-over-industrial-iot







