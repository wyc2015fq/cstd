# IOT语义互操作性之语义 - 我相信...... - CSDN博客





2018年02月19日 00:00:00[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：2694
所属专栏：[IoT与智能硬件](https://blog.csdn.net/column/details/19754.html)










这个系列文章描述了一个单一的语义数据模型来支持物联网和建筑、企业和消费者的数据转换。 这种模型必须简单可扩展, 以便能够在各行业领域之间实现插件化和互操作性。 对于一个目前从事智能硬件的老码农，觉得这些文字具有积极的参考意义。这一部分定义了数据语义。

## 物联网抽象层和互操作性程度

互操作性, 或计算机系统或软件交换或使用资讯的能力[1] , 是参与如今信息经济所有设备的一项要求。 传统上, 互操作性主要是在网络通信背景下定义的。 但随着从智能家居、智能建筑到智能能源、零售以及医疗保健和交通等行业, 数以百万计的设备正在连接, 因此现在需要一个更广泛的定义, 来考虑互操作性对系统以及系统性能的跨领域影响。

著名的网络互操作性框架是OSI模型。 OSI 模型通过七个不同的抽象层提供了一个互操作性的框架, 这些抽象层通过分享软件中的应用程序数据(第7层)从物理媒体中的底层传输(第1层)。 这些图层及作用的简要描述见图1。

![](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIP5wibKnXaBWDFkYS1x45fL5YepgaicZqoxJqyguqK0ylnibLF3HwCWmXiaA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)

图1 | OSI 模型概述了电信和计算网络互操作性的七个抽象层

虽然 OSI 模型的每一个抽象层都有助于整个网络的互操作性, 但每个抽象层都是弗吉尼亚建模分析和模拟中心（VMASC）定义的互操作性模型(LCIM)[2]中的三类互操作性之一。 这就是技术互操作性、句法互操作性和语义互操作性[3] ：
- 
技术互操作性是网络交换任何形式原始信息的基本能力。 技术互操作性由 OSI 栈(层1-4)的较低层级管理, 这些层次定义了网络上各点之间可靠传输和接收数据的基础设施。

- 
句法互操作性是在两台或多台机器之间交换结构化数据的能力, 通常由 OSI 模型的第5层和第6层处理。 在这里, XML 和 JSON 等标准数据格式提供了语法, 使系统能够识别正在传输或接收的数据类型。

- 
语义互操作性使系统能够以上下文的方式从结构化数据中解释意义，并在 OSI 栈的第7层中实现。


在 OSI 框架中, 每个抽象层的正确实现都有助于实现互操作性, 技术上的互操作性使得句法互操作性得以实现, 进而又可以实现语义互操作性。 技术互操作性目前在多领域通信网络中得到很好的理解和标准化, 这使得句法和语义层成为了真正可互操作的M2M数据通信的关键因素。

## 从句法到语义互操作性的转变

在 OSI 模型的第1至第4层提供了一套基于协议的网络基础设施技术, 而句法和语义互操作性通常依赖于针对特定行业的格式和协议, 并根据现有的系统和数据类型进行优化。 这一事实导致了对现有网络基础设施进行了数十亿美元的投资, 以支持在这些垂直市场上的 M2M 通信[4]。

为了在这些情况下促进广泛的句法互操作性, 工业互联网联盟（IIC）最近发布了"工业互联网连接框架"或 IICF [3]。 IICF 重新定义了传统的 OSI 模型, 将表示层和会话层(第5层和第6层)结合起来, 提供所有必要的机制, 以便"促进数据如何由端点无歧义地结构化和解析"(图2)。 跨行业领域的句法互操作性得到了一套"核心连接标准"(目前为数据分配服务(DDS)、 opc-unify 架构(OPC-UA) , oneM2M, 以及 web services) , 这些标准通过一套拟议的标准化网关进行通信。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPZxvJDwgdnia42icibAUOGoH05PLSMsk6WnNfbic3eOPGVEjUHhiaKfsRozg/640?wx_fmt=jpeg)

图2 | IIC的连接框架层为不同系统和领域的句法互操作性提供了基础

IICF 框架层允许在不同服务质量(QoS)级别上在应用程序之间传输的状态、事件和流。 这样的架构足以满足句法互操作性的要求。

除了 IICF 的句法互操作层之外, 还有信息层(OSI 模型中的应用层) , 这个层面的语义互操作性尚未确定。 在这里发生的分布式数据管理和互操作性依赖于两台或多台机器之间的指定本体, 可以自动而准确地解释交换数据的含义(上下文) , 并将其应用到一个有价值的目标。 正如 IICF 对于句法互操作性的方法所建议的, 本体必须考虑到在不同的系统和环境之间交换的元数据。 它代表了连接系统之间最高级别的互操作性。

一些行业组织努力实施涵盖尽可能广泛的行业和系统的语义数据模型(信息模型)。 它们包括 Object Management Group (OMG)、 IPSO 联盟、开放连接基金会(OCF)、开放组织、 zigbee、 Global Standards 1(GS1)、 Schema.org、 Project Haystack 等。 然而, 它们在实现适用于基础广泛的跨行业用例语义数据方案方面基本上没有成功, 因为它们的经验往往基于一系列狭窄的技术或行业细分。

下面描述了如何利用每种方法的最佳属性来实现跨多个行业和环境的可伸缩语义互操作性。

## 描述数据-数据语义的含义

### 从传感器到执行器的数据

物联网正在改变我们的世界, 影响着我们管理和操作环境的方式, 比如家庭、建筑、商店、医院、工厂和城市。 低成本的传感器, 更强大的控制器, 云服务, 智能设备和新的软件应用使得从设备到供应链的一切事物都得到了新的管理。

智能设备正在大幅增加从环境中获得数据的数量和类型, 而新型的软件应用程序正在创造新的方式从这些数据中获益。 这些进展共同推动了我们如何管理和操作这些环境的根本性转变, 使我们能够从基于简单反馈回路的传统控制战略转变为以数据为导向的方法, 使缔约方能够实时了解智能设备和系统的实际运行情况。

所有这些趋势都有助于提高效率, 并在有效使用的情况下, 推动整体业务成本的节省。 然而, 获取数据是一回事, 让数据具有可操作性是另一回事。 由于现有的数据比以往任何时候都多, 因此产业面临着新的挑战。

毫无疑问, 物联网和自动化系统的广泛应用所面临的最大障碍是互操作性。 麦肯锡公司的一份报告中估算, 实现物联网的互操作性将会为总的可用市场中增加40% 的额外价值。

智能设备的数据以多种不同的格式存储和传递。 它有不一致的、非标准的命名约定, 并提供非常有限的描述符用于理解其含义。 简单地说, 智能设备和自动化系统的数据缺乏信息来描述它自己的意思。 如果没有意义, 就需要进行耗时的规范化努力, 才能有效地利用数据来产生价值。 现状是, 当今设备的数据虽然在技术上是"可用的", 但却难以使用, 从而限制了缔约方充分受益于数据[5]中所承载的价值。

为了充分利用分析应用、远程设备管理和自动化系统等程序中的数据, 我们需要知道数据的含义。 例如, 如果从智能建筑系统中的传感器点获得数据, 它可能包含一个值为77.6。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPva5iahZYzDlC2jAndnhIPz0LnicaWzPbOu8sPDMjNeiaJWaKeFeXGlXZA/640?wx_fmt=jpeg)

[图3 | 示例传感器值]

首先，在理解值的数据类型之前, 我们不能做任何有效的分析或处理。 这个值是否代表温度、速度、压力或其他数据类型？

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIP44Bn7SheIWmrlKm6KdeZ8GCuAghXCMthJDFldQjkiaZJE7IXqrOx6Nw/640?wx_fmt=jpeg)

[图4 | 数据类型语义的示例值]

如果这个值代表一个温度, 那么我们需要知道它是77.6华氏度还是摄氏度呢？ 度量单位是我们理解和使用数据所需要的另一个基本描述符。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPPRnibticDNCgu1NaW8HpwNff3PTZvAtmmoMO6gVlIdCPodoPslEsFRcw/640?wx_fmt=jpeg)

[图5 | 单元语义的示例值]

继续这个例子, 如果只知道数据类型(温度)和测量单位(华氏度) , 我们仍然不太了解这个值的重要性。 温度值是否代表空气、水或其他环境条件？

如果是建筑物某一楼层的空气温度, 对于住户来说可能有点热。 如果是锅炉的水温, 那就太凉了。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIP5DasSkjSG3DK7UCHNQDvx8LhGfjlUgiaBvjyZdBEhIojBdicy0fOmJDA/640?wx_fmt=jpeg)

[图6 | 对象语义的示例值]

最后, 如果价值代表一个建筑物内部某层的一个空气温度, 那么当建筑是空置的时候, 它可能是很好的, 但是当它被使用的时候可能就不是这样了。 所以事件的日期和时间也很重要。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIP7NyETYh394G5Jz1sicnWibd55y8CAHHed6ZoKIsTWwGm8FPNSnnVgwVg/640?wx_fmt=jpeg)

[图7 | 示例值与时间戳] 

让我们假设值为77.6的传感器由名称(即标识符)"zn3-wwfl4"来识别。 如果我对安装时使用的建筑系统和命名约定非常熟悉, 我也许能够确定这意味着西翼第3区, 第4层等这些信息来解决问题。如果我对这座建筑了如指掌, 我也许还能看出,"zn3-wwfl4"标识符是按照第1号占用调度时间表(上午7时30分至下午6时30分)运作的楼层空气温度。

有了这些额外的信息, 可以确定一个工作日上午9点的值为77.6是不合适的, 因为天气太热, 会导致乘客投诉。 然而, 使我能够作出这一决定的是有关特定传感器含义的大量信息。 我之所以碰巧掌握这些信息, 是因为我个人对建筑物的了解, 但是在控制系统(或任何一个数据存储器中没有记录到的信息) 中, 可能不能够以任何一致的"机器可读"格式提供。

这就是利用现有系统和设备产生丰富数据的挑战——数据的表示、交流和解释能力。 这种"关于数据的数据表示"通常被称为元数据或数据语义。

有关传感器点的适当元数据将使我们能够了解目前77.6的价值所产生的影响, 而不需要依靠个人对系统的了解。 如上所述, 如果我们知道与传感器相关位置的相关时间, 我们就可以确定它在被占用的时间段温度过高, 而且用户很可能会感到不舒服。

然而, 如果没有必要的元数据, 我们无法确定当前值的影响及其与相关运行系统的关系。因此, 为了有效地使用数据, 我们需要将元数据和传感器值结合起来。 当人工完成时, 这个过程被称为映射或数据规范化。对于如数据分析、远程设备管理和自动化系统这些新实施的软件应用程序，时间序列数据处理是一个耗费时间的过程。

有趣的是, 在过去的十年里, 标准的通信协议几乎没有为大多数自动化系统提供任何能力来捕捉他们所包含数据的语义信息。 没有一种标准化的方法来表达它们所产生或包含的数据含义。 该系统提供带有标识符的传感器点(zn3-wwfl4) , 一个值(77.6)和单位(华氏度) , 但是其他信息很少。 结果是需要一个劳动密集的过程来"映射"数据(数据映射) , 然后才能开始有效使用传感器数据。 显然, 这对有效使用智能设备中的数据造成了重大障碍。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPGia7iaKu6WuwjOoXLWNFKNalIymtGrYaoWTFxL4BGudhzUP5GoDpQxqQ/640?wx_fmt=jpeg)

[图8 | 数据交换和规范化]

分析人士预测, 到2020年, 物联网将有超过250亿个连接设备。 总之, 这些设备将产生前所未有的数据量, 为了创造价值, 必须有效地索引、共享、存储、查询和分析。

越来越多的数据被规范化为时间序列数据——标有时间戳的数据——并以定期(基于间隔)或状态(或值)更改(基于事件)进行传输。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPX3Cvt50ymBu5VwW7hf1zgiaAyOOAStVDic5Y1DYqwyc8f6XjnUPTRWDg/640?wx_fmt=jpeg)

[图9 | 时间序列和事件]

虽然应用分析程序只需要时间序列数据的单向流动, 但自动化系统需要双向数据流, 以便将测量数据从传感器和指令消息传递给执行器。


![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPVnLBSElvkQ5TVwbbSMY6Kqq7MCaoga3T3S0tlA4G6hNnEAKdabk4rA/640?wx_fmt=jpeg)

[图10 | 双向数据流]

## 元数据挑战

那么我们如何获取所有这些信息, 分享它, 并将它与我们的自动化系统和智能设备中的数据元素联系起来呢？ 不能仅仅通过使用标准化的点标识符来做到这一点。 即使在这个的简单例子中, 我们有更多的元数据才能有效地捕捉到一个点标识符。 除此之外, 我们可能希望随着时间推移增加许多其他数据元素, 这远远超出了这个简单的例子, 显然我们需要另一种方法。 有效的解决办法必须具有以下特点:
- 
点标识符与相关元数据的解耦合。事实上, 我们有成千上万个系统中的数百万个点和它们的点标识符且无法改变。我们需要的是一个标准化的模型来联系点标识符和元数据。

- 
应该利用一个点标识符元数据的标准化库来提供元数据的一致性。这将使软件应用程序能够解释数据意义, 而不需要数据的规范化。随着新应用的涌现, 标准库需要由行业组织维护，使元数据具有良好的扩展性。


## 跨行业领域的用例

语义互操作性面临的一个关键挑战是, 能否在不同行业领域之间实现互操作性, 每个行业都有自己的环境和互操作性的应用场景。 在本系列接下来的部分中, 讨论了五个相互关联的行业——住宅与建筑、能源、零售、医疗和运输及物流。

![](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/DE2dk1GjczrJr3Od3iaTYLtsMjZzmyNIPhMMTgic2q18ic1Hpr2mc9dnx9K46PKLB6Vh7ccGQtX9M7wO5T0Q9qJYQ/640?wx_fmt=jpeg)

[图11 | 工业用例受益于语义互操作性]

### References:
- 
“Oxford Dictionaries – Dictionary, Thesaurus, & Grammar.” Oxford Dictionaries | English. Accessed June 20, 2017. https://en.oxforddictionaries.com/.

- 
Andreas, T., et al. “Applying the Levels of Conceptual Interoperability Model in Support of Integratability, Interoperability, and Composability for System-of-Systems Engineering.” Virginia Modeling Analyses & Simulation Center, Old Dominion University. Accessed June 20, 2017. http://www.iiisci.org/journal/cv$/sci/pdfs/p468106.pdf

- 
“The Industrial Internet of Things Volume G5: Connectivity …” Accessed June 20, 2017. https://www.iiconsortium.org/pdf/IICPUBG5V1.0PB_20170228.pdf

- 
ReportsnReports. “M2M, IoT & Wearable Technology Market – 30% CAGR for Installed Base Connections by 2020.” PR Newswire: news distribution, targeting and monitoring. June 21, 2016. Accessed June 20, 2017. http://www.prnewswire.com/news-releases/m2m-iot–wearable-technology-market—30-cagr-for-installed-base-connections-by-2020-583815741.html.

- 
Petze, John. “Describing the Meaning of Data—An Introduction to Data Semantics and Tagging”, Project Haystack Connections, Issue 1, Spring 2016.





