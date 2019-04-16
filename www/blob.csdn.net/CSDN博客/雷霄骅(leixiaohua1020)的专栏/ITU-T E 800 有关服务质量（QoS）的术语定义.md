# ITU-T E.800 有关服务质量（QoS）的术语定义 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年11月04日 19:34:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：170
个人分类：[网络](https://blog.csdn.net/leixiaohua1020/article/category/8433201)










**摘要**
 ITU-T E.800建议书为服务质量（QoS）的研究和管理提供了一套通用术语、本建议书列出的与QoS相关的技术和非技术术语旨在代表电信市场所有各方（即用户、服务提供商、制造商和监管机构）的利益。
 术语定义针对服务质量。一些术语在其它语境中可能有不同的定义，所以，应注意在适当语境中使用这些术语。有关QoS的完整术语，请参阅有关具体服务或性能特点的建议书。

**来源**
 ITU-T E.800建议书是ITU-T第2研究组（2005-2008年）按照世界电信标准化全会（WTSA）第1号决议程序于2008年9月23日批准的。

**前言**
 国际电信联盟（ ITU）是从事电信领域工作的联合国专门机构。 ITU-T（国际电信联盟电信标准化部门）是国际电信联盟的常设机构，负责研究技术、操作和资费问题，并且为在世界范围内实现电信标准化，发表有关上述研究项目的建议书。
 每四年一届的世界电信标准化全会（ WTSA）确定 ITU-T 各研究组的研究课题，再由各研究组制定有关这些课题的建议书。
 WTSA 第 1 号决议规定了批准建议书须遵循的程序。
 属 ITU-T 研究范围的某些信息技术领域的必要标准，是与国际标准化组织（ ISO）和国际电工技术委员会（ IEC）合作制定的。


**注**
 本建议书为简明扼要起见而使用的“主管部门”一词，既指电信主管部门，又指经认可的运营机构。
 遵守本建议书的规定是以自愿为基础的，但建议书可能包含某些强制性条款（以确保例如互操作性或适用性等），只有满足所有强制性条款的规定，才能达到遵守建议书的目的。“应该”或“必须”等其它一些强制性用语及其否定形式被用于表达特定要求。使用此类用语不表示要求任何一方遵守本建议书。


**知识产权**
 国际电联提请注意：本建议书的应用或实施可能涉及使用已申报的知识产权。国际电联对无论是其成员还是建议书制定程序之外的其它机构提出的有关已申报的知识产权的证据、有效性或适用性不表示意见。
 至本建议书批准之日止，国际电联尚未收到实施本建议书可能需要的受专利保护的知识产权的通知。但需要提醒实施者注意的是，这可能并非最新信息，因此特大力提倡他们通过下列网址查询电信标准化局（ TSB）的专利数据库：[http://www.itu.int/ITU-T/ipr/](http://www.itu.int/ITU-T/ipr/)。


**1 引言**
 1.1 综述术语标准化必不可少主要基于两个原因：
 – 避免标准使用者因相互矛盾的术语和定义产生混淆；
 – 帮助从事电信标准制定的各个组织实现协调一致。
 为制定服务质量（QoS）和网络性能（NP）等重要领域的标准需要一套一致的术语和定义。本建议书内列举的有关QoS的技术和非技术术语旨在代表电信服务市场中各方利益，包括用户、服务提供商、制造商和监管机构。
 这些术语的定义仅涉及QoS。一些术语在其它文件的不同上下文中可能定义不同。因此，在使用这些术语时应注意适当的上下文。
 本建议书的宗旨是为这些相关概念制定一套完整的术语和定义。这些集合术语和定义可普遍用于所有电信业务及用来提供这些业务的网络安排。
 本建议书将涵盖研究和管理QoS所涉及的通用术语。有关QoS的全套术语，请参阅有关具体业务或性能特性的建议书。
 1.2 有关概念的一般性指南端对端QoS取决于图1中各部分所做贡献。



![](https://img-blog.csdn.net/20131104182255234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 图1 – 端对端QoS贡献图示


 注 1 – 上图配置显示出两端连接用户的传统业务。但是，该配置原理可适用于由服务提供方向另一端用户提供的业务。
 注 2 – 终端设备：对端对端 QoS的贡献取决于终端设备的性能差异。
 注 3 – 接入网络：接入网络对端对端 QoS的贡献取决于接入媒质和某种业务所使用的技术（如，无线、有线、 ADSL等）。
 注 4 – 核心网络：核心网络可能是一个单一的提供商或不同提供商网络的组合。核心网络对端对端性能的QoS的贡献取决于每个网络部分的贡献（无论是单一提供商或多家提供商）、所使用的技术（数字复用、IP等）、传输媒质（空中、光缆或金属）及其它因素。
 为确定端对端QoS，有必要说明支持业务连接（不连接和面向连接）的具体操作条件。受环境因素（如，流量和路由选择）影响的操作条件亦能改变QoS。
 图2显示出QoS和网络性能（NP）之间的关系。QoS包括网络性能和非网络性能。网络性能包括误码率、延迟等，而非网络性能包括提供时间、修复时间、资费范围及投诉解决时间等。一种业务的QoS标准根据业务而不同，其相关性对不同客户亦有差别。


![](https://img-blog.csdn.net/20131104182416781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 图2 – 服务质量包括网络和非网络标准


 在更细的层面上QoS可分为四个视角，如图3所示。该概念在 [ITU-T G.1000] 中有更加详尽的描述。
 QoS的一般性定义源自质量定义，见第2段。QoS中最引人关注的是用户的体验（称为 QoSE或QoSP – 感知的QoS）。QoSE受到所提供的QoS以及影响用户感知的心理因素的影响。了解QoSE对于服务提供商增加收入和优化资源具有至关重要的意义。


![](https://img-blog.csdn.net/20131104182447421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 图3 – 服务质量的四个视角


 [ITU-T E.802] 集中了所有有关QoS的问题，同时还体现了QoS各个方面之间的相互关系。


**2 通用术语**
 2.1 
 质量
 一实体的特性总和，表明其满足明示和暗示需求的能力。
 注 – 这些特性应可观察并 /或可衡量。特性在得到定义后将成为参数，用衡量标准表示（见 2.7至2.13段）。
 2.2 
 服务质量（QoS）
 一种电信业务的特性总和，表明其满足明示和暗示业务用户需求的能力。
 2.3 
 用户/客户QoS要求（QoSR）
 客户/用户或某些客户/用户提出的QoS独特性能要求或需求表述。
 注 – 客户/用户需求可按优选顺序用描述性术语（标准）列出，为每项标准列举优选性能值。服务提供商之后将这些性能值转化为与业务相关的参数和衡量标准（见 [ITU-T E.802]）。
 2.4 
 服务提供商提供/计划的QoS（QoSO）
 服务提供商计划并因此向客户提供的质量水平表述。
 注 – 用有关具体业务的衡量参数目标值（或范围）表述的、服务提供商计划实现（并因此向客户 /用户提供）的 QoS水平。
 2.5 
 服务提供商提供/实现的QoS（QoSD）
 为客户实现或提供的QoS水平表述。注 – 实现或提供的 QoS用说明相关参数的衡量标准表示。
 2.6 
 客户/用户所体验/感受到的QoS（QoSE）
 客户/用户评语，说明他们认为他们所体验到的服务水平注 1 – 可通过一种评分意见来表示客户 /用户所体验 /感受到的服务质量水平。注 2 – 客户/用户所体验 /感受到的服务质量有两项主要的人为因素：量和质。量的因素可受到整个端到端系统因素（网络基础设施）的影响。
 注 3 – 质的因素可受到用户预期、环境条件、心理因素、应用背景等的影响。
 注 4 – 客户/用户所体验 /感受到的服务质量亦可被看作为一个受到影响其对服务理解的相关质化因素的用户所接受和理解的提供商提供 /实现的服务质量。
 2.7 
 特性
 帮助区别一特定人口中不同个人的特征注 – 区别可（通过可变因素体现）为量或（通过特性体现为）质的区别。
 2.8 
 标准
 收集多项特性或单项特性，用以描述一产品或服务对用户的益处。
 2.9 
 参数
 具有特定范围的一服务的可量化的特性。例如：预测“错拨可能性”的参数可表示为：“每100次尝试中错拨号码的次数”。注 – 参数可为客观或主观的。
 2.10 
 客观（量的）参数
 可以（利用工具或观察）衡量的参数以及量化确定的性能值可被列为客观参数。注 – 例如：一个信道的差错率，修理时间。
 2.11 
 主观（质的）参数
 可利用人工评判和理解来表示的参数可被列为主观或质的参数。注 1 – 质的参数通过评分意见表示。注 2 – 例如：一个移动手机的人体工学质量（“感觉好”因素）可以一特定等级上的评分表示。
 2.12 
 衡量单位
 衡量单位为可表示一参数的单位。例如：业务提供的等待时间可以日历日或工作日表示。
 2.13 
 单位（亦称为“指标”）
 利用以衡量单位观察特性计算出的值注 1 – 在表示温度时， 38摄氏度中的38为衡量标准，摄氏为衡量单位。注 2 – 用99.99%（衡量标准）表示的连接链路的可用性可通过将路由器、接入的传输链路和核心网络的可用性（观测到的属性）相加计算得出。
 2.14 
 服务
 一机构向用户提供的一套功能称为服务。
 2.15
 连接
 连接用来在网络两点之间建立通信。连接是“通过呼叫路由选择和连接路由选择建立的承载路径、标签交换路径、虚拟电路和/或虚拟路径”（[ITU-T E.360.1]和[ITU-T E.361]）。
 2.16 
 项目
 可单独考虑的任何部分、设施、子系统、功能单元、设备或系统。注 – 项目可由硬件、软件组成或二者共同构成，还可能包括人，如，电话操作系统中的话务员。
 2.17 
 用户
 用户是一个使用通信设备的实体（如，启动或应答呼叫）（[ITU-T Q.1300]）或借助网络使用通信连接的网络之外的人或实体。
 注 – 连接用来在网络两点之间建立通信。连接是“通过呼叫路由选择和连接路由选择建立的承载路径、标签交换路径、虚拟电路和 /或虚拟路径”（ [ITU-T E.360.1]和[ITU-T E.361]）。
 2.18 
 客户
 负责为服务付费的用户。
 2.19 
 网络性能
 网络或部分网络提供用户之间通信功能的能力。注 1 – 网络性能适用于网络提供商的规划、发展、操作和维护，是 QoSO的主要技术部分。
 注 2 – 网络性能参数对于网络提供商而言意义重大，在适用的各个网络部分可以量化。
 2.20 
 网络提供商
 为传输电信业务载体而拥有电信网络的机构。
 2.21 
 服务提供商
 向用户和客户提供服务的机构。


**3 服务质量术语**

服务质量术语分为三大类：服务、网络和管理。


**3.1 服务类术语**

**3.1.1 一般性术语**
 3.1.1.1
 服务的提供
 3.1.1.1.1
 提供
 从得到签订服务合同要求至客户/用户可以使用服务之时服务提供商在提供服务中开展的所有活动，或服务提供商从得到签订服务合同要求至客户/用户可以使用服务之时开展的所有活动。
 3.1.1.1.2 终止
 从客户与服务提供商执行合同之时至所有与服务相关的硬件和软件无法使用并 /或从客户端撤出期间服务提供商从事的与终止服务相关的所有活动，或从客户与服务提供商执行合同之时到所有与服务相关的硬件和软件无法操作并/或从客户端撤出期间服务提供商为中止服务开展的所有活动。
 3.1.1.2 呼叫建立
 3.1.1.2.1
 呼叫建立时间
 从网络收到建立呼叫所需地址信息（在主叫用户接入线上识别）开始到主叫方收到被叫方忙音、或振铃音或应答信号（即，在主叫用户接入线上识别）结束的时间段。本地、国家和公务呼叫应包含在内，但与其它许可运营商的呼叫不应包含在内，因为指定运营商无法控制其它网络提供的QoS。
 3.1.1.2.2
 业务可接入性性能用户提出要求时，在规定容限和其它给定条件下可获得的服务能力。
 3.1.1.2.3
 平均服务接入延迟
 从用户提出使用服务的最初要求到用户接入服务之时的预期时间段，该服务应在规定容限和其它给定操作条件下获得。
 3.1.1.2.4
 服务用户错误概率用户在尝试使用服务时出现的错误概率。
 3.1.1.2.5
 拨号错误概率电信网络用户在呼叫尝试中出现的拨号错误概率。
 3.1.1.2.6
 呼叫放弃概率
 用户通过电信网络放弃呼叫尝试的概率。注 – 放弃可能是由过高用户错误率、过长服务接入延迟等原因造成的。
 3.1.1.3
 呼叫进展
 3.1.1.3.1
 中断；（服务）中断
 暂时无法提供业务的时间超过给定时限，特点是业务中至少一项重要参数的变化超出规定。注 1 – 服务中断通常为传输中断，特点是功率电平、噪音电平、信号失真和误码率出现非正常值。
 注 2 – 服务中断还可能因用于服务的各项目的不良状态或高服务需求量等外部原因造成。
 3.1.1.3.2
 QoS变量用户可感知到的任何性能变量（拥塞、延迟等）（[ITU-T E.360.1]）。
 3.1.1.3.3
 相对 QoS
 不用绝对术语表述延迟等流量传送的QoS参数限值。它所描述的是一些类型流量的处理之所以不同于其它类型的条件，而不同类型的流量具有不同的QoS水平（[ITU-T Y.2111]）。
 3.1.1.4
 服务恢复/修复
 3.1.1.4.1
 服务恢复
 服务故障后引发的、为成功恢复通信而采用的一套自动或手动方法。相关活动可能涉及网络和/或非网络相关问题。具体而言，服务故障可能导致网元故障、功能故障和/或人为错误，造成服务中断。
 3.1.1.4.2
 修复（故障检修）在发现故障后为使一项目恢复到可行使必要功能状态而进行的维护。
 3.1.1.5
 服务质量特性
 3.1.1.5.1
 速度
 一项性能标准，描述用来履行功能的时间或速度（履行该功能可能达到或可能达不到预期精确度）（[ITU-T I.350]）。
 3.1.1.5.2
 精确度
 描述功能履行准确程度的性能标准（履行功能可能达到或可能达不到预期速度）（[ITU-T I.350]）。
 3.1.1.5.3
 依赖性
 一项性能标准，描述在任何速度或精确度下，在给定观察间隔内履行功能的肯定程度（或保证）（[ITU-T I.350]）。
 3.1.1.5.4
 可用性
 一项目在给定时刻或给定间隔内任何时刻处于可行使必要功能状态的可用性（假设在必要时已提供了外部资源）（[ITU-T E.802]）。
 3.1.1.5.5
 可靠性
 在所述条件下，一项目在给定时间间隔内可行使必要功能的概率。
 3.1.1.5.6
 简便易行用户受益的服务功能的方便性和简便（[ITU-T E.802]）。
 3.1.1.6
 计费和记账
 3.1.1.6.1
 计费
 决定服务使用价格所需要的一套功能（[ITU-T Q.825]）。
 3.1.1.6.2
 错误计费或结算概率
 呼叫尝试受到错误计费或结算处理的概率。
 3.1.1.6.3
 计费不足概率
 因任何原因对呼叫尝试计费不足的概率。
 3.1.1.6.4
 超额计费概率
 因任何原因对呼叫尝试超额计费的概率。
 3.1.1.6.5
 记账
 为服务客户准备账单、提配付费、获取收入并应对客户索赔的行政功能（[ITU-T Q.825]、[ITU-T Q.1703]、[ITU-T Q.1741.2]、[ITU-T Q.1741.3]）。
 3.1.1.6.6
 记账错误概率
 为服务用户记账的错误概率。
 3.1.1.6.7
 完善的记账
 提交用户的记账信息准确反映呼叫尝试类型、目的地和时长的概率。
 3.1.1.7
 通用术语
 3.1.1.7.1
 端对端质量
 与通信系统（包括所有终端设备）性能相关的质量。注 – 对话音业务而言，端对端质量相当于从发话至收听的质量。（ [ITU-T P.10]）
 3.1.1.7.2
 置信区间
 由两项统计数据或一项统计数据确定的随机间隔，使估算参数在该间隔内的概率与给定值相等。
 3.1.1.7.3
 置信系数；置信水平与置信区间或统计容限间隔相关的概率值。
 3.1.1.7.4
 概率
 处于实际原因，可认为，当测试条件可复制时，事件E的发生频次围绕其概率值摆动，随着测试数量的不断增加，发生频次无限地靠近概率值。
 注 – 概率的概念根据是否打算确定信任度或是否被认为是频次限值表现为两种形式中的任意一种。在两种情况下，引用该概念需要谨慎从事，该概念是无法在国际标准中制定的，为此用户应参考专用文献。

**3.1.2 服务专用术语**
 3.1.2.1
 平均服务提供时间
 从潜在用户要求一机构提供服务的必要手段至上述手段已提供之间的预期时长，用取样代表数的算术比均值表示。
 3.1.2.2
 完善的服务性能
 服务一经获得没有明显错误的水平。注 – 应规范可接受的故障水平。
 3.1.2.3
 语音质量
 感知到的以声音形式显示的语音质量，是感知结果及评定过程，在此过程中，评定主体在所感知的特性（即听觉事件）和希望或预期特性之间建立关系（[ITU-T P.10]）。
 3.1.2.4
 语音传输质量
 泛指与通信系统性能相关的语音质量。[ITU-T G.109]根据E-模型的预测（即，传输评定因素R的范围）规定了语音传输质量类别（[ITU-T P.10]）。
 3.1.2.5
 对话质量通信伙伴感知到的双向或多向对话质量（[ITU-T P.10]）。
 3.1.2.6
 对话语音质量双向或多向对话中体验到的语音质量（[ITU-T P.10]）。
 3.1.2.7
 单向话音传输质量
 用户在只听情况下体验到的、通过通信系统传输的、与话音信号相关的语音质量。仅用于单向传输特性（[ITU-T P.10]）。
 3.1.2.8
 意见评分（电话）
 规定尺度上体现主体对用于对话或仅用于听取的语音资料的电话传输系统性能的意见值（[ITU-T P.10]）。
 3.1.2.9
 平均意见分（MOS）
 规定尺度上体现主体对用于对话或仅用于听取的语音资料的电话传输系统性能的意见值平均分（[ITU-T P.800.1]）。注 –还有一些其它类型的 MOS，其定义见 [ITU-T P. 800.1]。
 3.1.2.10
 多媒体
 信息通信中使用的音频、视频、文本、图表、传真和电话等多种形式媒体的组合（[ITU-T J.148]、[ITU-T Q.1702]）。
 3.1.2.11
 多媒体服务
 支持同时使用多种媒体类型（如，话音、数据、视频）的电信服务（[ITU-T E.417]）。
 3.1.2.12
 流媒体（多媒体服务）
 以数据包流形式传送的多媒体数据（通常为话音、文本、视频和音频的组合），软件应用在数据包到达时对其进行解释和成像。注 1 –媒体流是传送多媒体数据的技术。
 注 2 –媒体流可能是实时的，也可能不是实时的。
 3.1.2.13
 下载
 从服务器或主机向个人计算机或设备传送数据或程序。

**3.1.3 安全专用术语**
 3.1.3.1
 安全
 “安全”表示减少资产和资源薄弱环节。资产为任何有价之物。薄弱环节是毁坏系统或系统中信息而可利用的任何弱点（[ITU-T X. 800]）。
 3.1.3.2
 信息安全
 对信息保密性、完整性和可用性的安全保护（[ITU-T X 1051]）。
 3.1.3.3
 数据安全
 对数据完整性和可用性的安全保护。
 3.1.3.4
 隐私
 个人用来控制或影响其哪些信息收集和存储及由谁及向谁披露的权力。
 注 – 由于本术语涉及个人权利，不可能非常准确，应避免使用，除非作为提出安全要求的动机（[ITU-T X. 800]）。
 3.1.3.5
 密码
 通常是由字符串构成的保密认证信息（[ITU-T X. 800]）。
 3.1.3.6
 保密性
 不将信息提供或披露给非授权个人、实体或流程的性质（[ITU-T X. 800]）。
 3.1.3.7
 数据保密性
 用来保护数据不被非授权披露的服务。认证框架支持该数据保密性服务。这种服务可用来防止数据拦截（[ITU-T X. 509]）。
 3.1.3.8
 完整性
 数据未在非授权情况下被改变的性质（[ITU-T H. 235]）。
 3.1.3.9
 数据完整性
 数据未在非授权情况下被改变或毁坏的性质（[ITU-T X.800]）。
 3.1.3.10
 恶意软件
 有意采取可破坏数据或中断系统的行动的软件总称。
 3.1.3.11
 黑客
 该术语泛指多种恶意行为，如摆脱接入控制、拒绝服务、盗取信息或安装恶意软件。
 3.1.3.12
 钓鱼
 复制现有网页，哄骗用户提交个人、财务或密码数据。或发送电子邮件到用户，为骗取用户提交可以用来身份的个人信息谎称为合法企业。该电子邮件指示用户访问一网页，并在此更新个人信息，如合法企业已拥有的密码和信用卡、社会保障和银行账号。然而，该假冒网页的建立仅为用户信息。
 3.1.3.13
 病毒（计算机病毒）
 不经用户允许或知情的情况下可自我复制并感染计算机的计算机程序或重复的程序或代码通过自我插入或将自身附于媒体感染另一个程序、启动扇区、对支持宏的部分或文件予以分区。多数病毒仅仅复制，也有很多病毒造成破坏。
 3.1.3.14
 蠕虫
 自我复制的计算机程序。与病毒不同的是，它无需将自身附于现有程序。蠕虫总是破坏网络（如果仅是消耗带宽），而病毒则总是感染或破坏目标计算机中的文件。或
 从一个磁盘驱动向另一个磁盘驱动复制的程序或通过使用电子邮件或其它传输机制进行自我复制。该程序可破坏计算机安全。它可以JOKE程序或其它类型的软件形式出现。
 3.1.3.15
 特洛伊木马
 貌似良性甚至有用软件，掩盖了其中断系统或盗取信息的真正目的。
 3.1.3.16
 欺诈
 电信中的欺诈是指未经向服务提供商适当支付而获取或购买服务，或通过假冒代表获得不合理优势。以不公平手段获取的优势。
 3.1.3.17
 伪造
 一实体编造信息并宣称该信息是从另一实体收到的或发给另一实体的（[ITU-T M.3016.0]）。
 3.1.3.18
 垃圾信息
 垃圾信息为无用、推介性信息，往往为入侵性互联网广告。
 3.1.3.19
 推广垃圾信息
 推广垃圾信息是将无用重复文章（垃圾信息）公布给大量网络用户的做法。
 3.1.3.20
 加密法
 这种做法包含转换数据的原则、手段和方法，从而隐藏其信息内容，防止暗中修改并/或防止非授权使用。注 –使用密码决定了加密和解密使用的方法。
 3.1.3.21
 加密
 用来将普通文本中信息转化成为密码文本的方法（[ITU-T J. 170]）或扰乱信号以避免非授权获取的程序（[ITU-T J. 93]）。
 3.1.3.22
 拒绝服务
 阻止对资源的授权获取或拖延具有严格时限的操作（[ITU-T X.800]）。
 3.1.3.23
 网络安全
 对与互联网相连接的网络数据和系统的保护。


**3.2 网络类术语**
 一些更通用的网络相关术语 – 网络性能（NP）术语 – 已列在本段内。业务专用NP术语列在其它建议书中，其中包括[ITU-T P.10]、[ITU-T Y.1540]、[ITU-T Y.1541]、[ITU-T Y.1560]、[ITU-T Y.1561]。
 3.2.1
 客户端设备
 网络客户端接口处的电信设备（[ITU-T G. 998]）。
 3.2.2
 网络/用户接口
 网络/用户接口是服务提供商网络与用户或客户的客户端设备之间的物理接口。注 – 在某些情况下，客户端设备可能是由服务提供商提供的。上述定义在此情况下依然有效。
 3.2.3
 互连互通
 “互连互通”相同或不同服务提供商使用的公众通信网络的物理和逻辑连接，以便使一个服务提供商的用户得以与另一服务提供商的用户沟通，或接入另一服务提供商提供的服务。
 3.2.4
 网络可接入性
 服务用户在（向网络）提出请求后收到在规定条件内继续选择信号的概率。注 – 继续选择信号是要求用户选择期望目的地的信号。
 3.2.5
 连接可接入性
 在交换机收到有效代码后，在规定容限和其它给定条件下可建立连接的概率。
 3.2.6
 连接建立错误概率
 在测量阶段内导致连接建立错误的连接建立尝试数量与连接建立尝试总数之比（[ITU-T Y.1560]）。
 3.2.7
 连接建立故障概率
 在测量阶段内未能建立连接的连接建立尝试数量与连接建立尝试总数之比（[ITU-T Y.1560]）。
 3.2.8
 不可接受的传输概率
 正在建立的、语音路径传输质量不可接受的连接概率。
 3.2.9
 无音概率
 交换机收到有效代码后呼叫尝试遇到无侧音情况的概率。
 3.2.10
 路由选择错误概率
 交换机收到有效代码后呼叫尝试被错误路由的概率。
 3.2.11
 流量能力性能
 一项目在给定内部条件下满足给定规模流量需求和其它特性的能力。注 – 给定内部条件用于任何错误或非错误子项目的组合。
 3.2.12
 传输性能
 表示相对于网络入口通信信号在网络出口的性能。性能情况用有关应用或服务的某个相关参数表示。示例1：对于模拟连接之上的话音通信，传输性能应使用损耗、畸变（各种类型）、噪音
 （各种类型）等表示。示例2：对于IP网络话音，有关性能将用延迟（时延）、抖动和包损失表示。
 3.2.13
 比特透明度
 电信系统传输在入口处获得的信号并在给定时间内不经变化将其复制于出口点的能力。
 3.2.14
 比特误码率（BER）
 在给定时间内比特误码数与传输比特总数之比。
 3.2.15
 无错秒比
 在给定时间间隔内收到无错比特的单秒数与单秒间隔总数之比。注 1 – 时间间隔长度须规范。
 注 2 – 该比例通常用百分比表示。
 3.2.16
 传播性能
 一种传播媒介的能力，在此媒介中，波在无人为引导的情况下将信号在给定容限内传送。
 3.2.17
 端对端IP网络
 将IP包从SRC传送至DST的一套EL（交换链路）和NS（网络部分）。约束端对端IP网络的MP（测量点）是SRC（来源主机）和DST（目的地主机）的MP（[ITU-T Y.1540]）。
 3.2.18
 端对端IP网络性能相对于任何给定无方向端对端IP服务而言是可测量的（[ITU-T Y.1540]）。
 3.2.19
 IP服务可用性百分比（PIA）
 IP服务可用性百分比（PIA）使用IP服务可用性函数确定为可用的计划内IP服务总时间百分比（Tav间隔百分比）（[ITU-T Y.1540]）。
 3.2.20
 释放故障概率
 未实现的必要连接释放概率。
 3.2.21
 连接清除故障概率
 在测量时段内连接清除故障与连接释放尝试总数之比（[ITU-T Y.1560]）。
 3.2.22
 连接建立成功比（CESR）
 成功连接建立数除以尝试总数（[ITU-T Y.1562]）。


**3.3 管理类术语**
 3.3.1
 QoS资源管理
 包括服务等级确定、路由选择表、推论、连接准入、带宽划分、带宽保护、带宽预留、优先路由选择和优先排队在内的网络功能（[ITU-T E.360.1]）。或
 在资源分配中，资源管理系统不仅要考虑资源的可用性和资源控制政策，同时还要考虑以QoS参数衡量的应用的QoS要求。
 3.3.2
 服务等级
 服务的一组端对端质量性能值（或范围）被称为“服务等级”。一项服务可能按规定的不同参数的性能限值分为三至五个等级。这些不同等级的服务通常向客户收取不同资费。
 3.3.3
 客户关系管理（CRM）
 确定并解决服务提供商与用户在提供和消费服务合同关系中的问题。注 – 这些问题包括账单的定制、资费选择、多种服务和修复安排的磋商。
 3.3.4
 基准为给未来性能成果制定可比照或评定的标准值评估一个参数或一套参数的性能值。
 3.3.5
 补偿方案
 服务提供商因所提供质量水平的不足（如，拖延服务及修复或未实现承诺服务质量水平）而向客户进行的支付组合。
 3.3.6
 服务水平协议（SLA）
 服务水平协议是一份正式文件，列出了服务提供商将为一项服务或服务组合提供的性能特性和目标值（或范围）。注 – SLA可能包括对性能、资费和记账、服务提供补偿及在存有异议时的升级程序陈述。
 T02 R024_F08
 3.3.7
 服务质量协议（SQA）见图4。

![](https://img-blog.csdn.net/20131104192102953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 图4 – 服务质量协议

3.3.8 
 两次中断之间的时间
 一次中断结束和另一次中断开始之间的时长。
 3.3.9 
 中断时长
 一次中断的时长。
 3.3.10 
 平均中断间隔（MTBI）
 根据有效统计抽样数字计算的预期中断间隔，通常用数学平均值表示。
 3.3.11 
 平均恢复时间（MTTR）；平均修复时间
 从客户报告故障到服务恢复使用期间完成的有效统计修复的预期修复时间，通常用数学平均值表示。
 3.3.12 
 故障覆盖率
 在给定条件下识别一项目故障的比例。
 3.3.13 
 修复范围
 维护机构在给定条件下，利用给定维护政策按需求提供维护一项目必要资源的能力。注 – 给定条件与项目本身相关，符合使用并维护项目的条件。
 3.3.14 
 故障
 一项目不能履行必要功能的情况，不包括因预防维护、缺少外部资源或计划内行动造成的失能。注 – 故障往往是项目的失败造成的，但也可能在没有预先失败的情况下存在。
 3.3.15 
 检修；修复
 发现错误后进行的维护，旨在将项目恢复至可以履行必要功能的状态。
 3.3.16 
 可靠性性能
 一项目在给定条件下，在给定时间内履行必要功能的能力。注 1 – 一般认为，项目在时段开始有能力履行必要功能。
 注 2 – 可靠性一词用来表示可靠性性能单位。对该性能概念，将使用以下小段中的衡量单位：
 3.3.16.1 
 平均首次故障发生时间（MTTFF）
 根据有效统计抽样计算得出的预期首次故障发生时间，通常用数学平均值表示。
 3.3.16.2 
 平均故障时间（MTTF）
 根据有效统计抽样计算得出的预期故障时间，通常用数学平均值表示。
 3.3.16.3 
 平均故障间时间（MTBF）
 根据有效统计抽样计算得出的预期两次故障之间的时间，通常用数学平均值表示。
 3.3.17 
 故障率提高因素
 在所述参考测试条件下提高的测试故障率与故障率之比。注 – 两个故障率针对所测试项目生命周期内的同一时段。
 3.3.18 
 灾害恢复、商业连续性
 灾害后恢复网络服务涉及的所有活动。这种灾害包括火灾、地震、人为破坏、轰炸或软件故障。
 3.3.19 
 投诉
 用户或客户对使用服务中预期与实际得到的利益差距表示不满的声明。注 – 投诉可采取各种形式，包括书面、电子手段或上门投诉。
 3.3.20 
 号码簿服务
 从定义完善的目标目录中搜索和检索信息的服务。上述对象可能包含有关证书、电话号码、接入条件、地址等信息。号码簿服务的提供应符合[ITU-T X.500]（[ITU-T X.843]）。


**附件A**


相关QoS/NP手册和E系列建议书（本附件是本建议书不可分割的一部分）

**参考文献**
 下列ITU-T建议书和其它参考文献的条款，通过在本建议书中的引用而构成本建议书的条款。在出版时，所指出的版本是有效的。所有的建议书和其它参考文献都面临修订，使用本建议书的各方应探讨使用下列建议书和其它参考文献最新版本的可能性。当前有效的 ITUT建议书清单定期出版。本建议书中引用某个独立文件，并非确定该文件具备建议书的地位。
 [ITU-T E.360.1] Recommendation ITU-T E.360.1 (2002), Framework for QoS routing and
 related traffic engineering methods for IP-, ATM-, and TDM-based
 multiservice networks.
 [ITU-T E.361] Recommendation ITU-T E.361 (2003), QoS routing support for interworking of QoS services classes across routing technologies.
 [ITU-T E.417] Recommendation ITU-T E.417 (2005), Framework for the network management of IP-based networks.
 [ITU-T E.470] Recommendation ITU-T E.470 (2005), Operational considerations for QoS of voice over IP-based networks with PSTN-IP-PSTN architecture.
 [ITU-T E.800] Recommendation ITU-T E.800 (1994), Terms and definitions related to quality of service and network performance including dependability.
 [ITU-T E.801] Recommendation ITU-T E.801 (1996) Framework for service quality agreement.
 [ITU-T E.802] Recommendation ITU-T E.802 (2007), Framework and methodologies for the determination and application of QoS parameters.
 [ITU-T E.860] Recommendation ITU-T E.860 (2002), Framework of a service level agreement.
 [ITU-T G.109] Recommendation ITU-T G.109 (1999), Definition of categories of speech transmission quality.
 [ITU-T G.998.1] Recommendation ITU-T G.998.1 (2005), ATM-based multi-pair bonding.
 [ITU-T G.998.2] Recommendation ITU-T G.998.2 (2005), Ethernet-based multi-pair bonding.
 [ITU-T G.998.3] Recommendation ITU-T G.998.3 (2005), Multi-pair bonding using time division inverse multiplexing.
 [ITU-T G.1000] Recommendation ITU-T G.1000 (2001), Communications quality of service: A framework and definitions.
 [ITU-T G.1010] Recommendation ITU-T G.1010 (2001), End-user multimedia QoS categories.
 [ITU-T G.1020] Recommendation ITU-T G.1020 (2006), Performance parameter definitions for quality of speech and other voiceband applications utilizing IP networks.
 [ITU-T G.1030]
 Recommendation ITU-T G.1030 (2005), Estimating end-to-end performance in IP networks for data applications.
 [ITU-T H.235.0]
 Recommendation ITU-T H.235.0 (2005), H.323 security: Framework for security in H-series (H.323 and other H.245-based) multimedia systems.
 [ITU-T I.350]
 Recommendation ITU-T I.350 (1993), General aspects of quality of service and network performance in digital networks, including ISDNs.
 [ITU-T J.93]
 Recommendation ITU-T J.93 (1998), Requirements for conditional access in the secondary distribution of digital television on cable television systems.
 [ITU-T J.148]
 Recommendation ITU-T J.148 (2003), Requirements for an objective perceptual multimedia quality model.
 [ITU-T J.170]
 Recommendation ITU-T J.170 (2005), IPCablecom security specification.
 [ITU-T M.3016.0]
 Recommendation ITU-T M.3016.0 (2005), Security for the management plane: Overview.
 [ITU-T P.10]
 Recommendation ITU-T P.10/G.100 (2006), Vocabulary for performance and quality of service.
 [ITU-T P.800.1]
 Recommendation ITU-T P.800.1 (2006), Mean Opinion Score (MOS) terminology.
 [ITU-T Q.825]
 Recommendation ITU-T Q.825 (1998), Specification of TMN applications at the Q3 interface: Call detail recording.
 [ITU-T Q.1300]
 Recommendation ITU-T Q.1300 (1995), Telecommunication applications for switches and computers (TASC) – General overview.
 [ITU-T Q.1702]
 Recommendation ITU-T Q.1702 (2002), Long-term vision of network aspects for systems beyond IMT-2000.
 [ITU-T Q.1703]
 Recommendation ITU-T Q.1703 (2004), Service and network capabilities framework of network aspects for systems beyond IMT-2000.
 [ITU-T Q.1741.2]
 Recommendation ITU-T Q.1741.2 (2002), IMT-2000 references to release 4 of GSM evolved UMTS core network with UTRAN access network.
 [ITU-T Q.1741.3]
 Recommendation ITU-T Q.1741.3 (2003), IMT-2000 references to release 5 of GSM evolved UMTS core network.
 [ITU-T X.500]
 Recommendation ITU-T X.500 (2001) | ISO/IEC 9594-1:2001, Information technology – Open Systems Interconnection – The Directory: Overview of concepts, models and services.
 [ITU-T X.509]
 Recommendation ITU-T X.509 (2005) | ISO/IEC 9594-8:2005, Information technology – Open Systems Interconnection – The Directory: Public-key and attribute certificate frameworks.
 [ITU-T X.800]
 Recommendation ITU-T X.800 (1991), Security architecture for Open Systems Interconnection for CCITT applications.
 [ITU-T X.843]
 Recommendation ITU-T X.843 (2000) | ISO/IEC 15945:2002, Information technology – Security techniques – Specification of TTP services to support the application of digital signatures.
 [ITU-T X.1051]
 Recommendation ITU-T X.1051 (2008) | ISO/IEC 27011:2008, Information technology – Security techniques – Information security management guidelines for telecommunications organizations based on ISO/IEC 27002.
 [ITU-T Y.1540]
 Recommendation ITU-T Y.1540 (2002), Internet protocol data communication service – IP packet transfer and availability performance parameters.
 [ITU-T Y.1541]
 Recommendation ITU-T Y.1541 (2006), Network performance objectives for IP-based services.
 [ITU-T Y.1560]
 Recommendation ITU-T Y.1560 (2003), Parameters for TCP connection performance in the presence of middleboxes.
 [ITU-T Y.1561]
 Recommendation ITU-T Y.1561 (2004), Performance and availability parameters for MPLS networks.
 [ITU-T Y.1562]
 Recommendation ITU-T Y.1562 (2007), Framework for higher-layer protocol performance parameters and their measurement.
 [ITU-T Y.2111]
 Recommendation ITU-T Y.2111 (2006), Resource and admission control functions in Next Generation Networks.
 [ITU-T Hdbk QoS]
 ITU-T Handbook (2004), Quality of Service and Network Performance.
 NOTE – In this Handbook Chapter 2 (with title "Roadmap to ITU-T Recommendations on QoS") offers two ways to access ITU-T Recommendations on QoS, the first through the concept (clause 2.1), and the second through the series organization with a short description of QoS-related content (clauses 2.2 to 2.9).
 [ETSI ETR 003]
 ETSI ETR 003 ed.2 (1994), Network Aspects (NA); General aspects of Quality of Service (QoS) and Network Performance (NP). <[http://electronics.ihs.com/document/abstract/MQXICAAAAAAAAAAA](http://electronics.ihs.com/document/abstract/MQXICAAAAAAAAAAA)>


**附件 B**


建议缩略语清单
 （本附件是本建议书不可分割的一部分）
|BER|比特误码率|
|----|----|
|CESR|连接建立成功比|
|COS|服务等级|
|CRM|客户关系管理|
|IPER|IP包错误率|
|IPLR|IP包丢失率|
|MOS|平均意见分|
|MTBF|再次故障之间的平均时间|
|MTBI|两次中断之间的平均时间|
|MTTF|出现故障的平均时间|
|MTTFF|首次出现故障的平均时间|
|MTTR|恢复的平均时间|
|PIA|IP服务可用性百分比|
|PTD|包传送延迟|
|QoS|服务质量|
|QoSD|交付的服务质量|
|QoSE|体验到的服务质量|
|QoSO|提供的服务质量|
|QoSR|服务质量要求|
|SLA|服务水平协议|
|SQA|服务质量协议|




**索引**按字母顺序排列的本建议书所含定义清单


**A**
 Accuracy Availability
**B**
 Benchmark Billing Billing error probability Billing integrity (probability) Bit error ratio (BER) Bit transparency Break of service Business continuity
**C**
 Call abandonment probability Call set-up time Cessation Characteristic Charging Class of service Compensation schemes Complaint Computer virus Confidence coefficient Confidence interval Confidence level Confidentiality Connection Connection accessibility Connection clearing failure probability Connection establishment error probability Connection establishment failure probability Connection establishment success ratio (CESR) Conversational quality Conversational speech quality Corrective maintenance Cryptography Criterion Customer Customer premises equipment Customer relationship management (CRM) Cybersecurity
**D**
 Data confidentiality Data integrity Data security Denial of service Dependability Dialling mistake probability Directory services Disaster recovery Download
**E**
 Encryption End-to-end IP network End-to-end IP network performance End-to-end quality Error free seconds ratio (EFSR)
**F**
 Failure rate acceleration factor Fault Fault coverage Forgery Fraud
**H**
 Hacking
**I**
 Incorrect charging Information security Integrity Interconnection Interruption Interruption duration Item
**L**
 Loss ratio
**M**
 Malware Mean opinion score (MOS) Mean service access delay Mean service provisioning time Mean time between failures (MTBF)
 Mean time between interruptions (MTBI) Mean time to failure (MTTF) Mean time to first failure (MTTFF) Mean time to restoration (MTTR) Measure Metric Misrouting probability Multimedia Multimedia service
**N**
 Network accessibility Network/user interface Network performance Network provider No tone probability
**O**
 Objective parameters One-way voice transmission quality Opinion score (in telephony) Overcharging probability
**Q**
 QoS achieved by service provider QoS delivered by service provider QoS experienced by customer QoS experienced by user QoS offered by service provider QoS perceived by customer QoS perceived by user Qos planned by service provider QoS requirements of customer QoS requirements of user QoS resource management QoS variable Quality Quality of service Qualitative parameters Quantitative parameters
**P**
 Parameter Password Percent IP service availability (PIA) Phishing Privacy Probability Propagation performance Provision
**R**
 Relative QoS Release failure probability Reliability Reliability performance Repair Repair coverage
**S**
 Security Service Service accessibility performance Service integrity performance Service level agreement (SLA) Service Provider Service quality agreement (SQA) Service user mistake probability Service restoration Spam Spamming Speech quality Speech transmission quality Speed Streaming Subjective parameters Simplicity
**T**
 Time between interruptions Trafficability performance Transmission performance Trojan horse
**U**
 Unacceptable transmission probability Undercharging probability User
**V**
 Virus
**W**
 Worms



**ITU-T 系列建议书**


|A系列|ITU-T工作的组织|
|----|----|
|D系列|一般资费原则|
|**E系列**|**综合网络运行、电话业务、业务运行和人为因素**|
|F系列|非话电信业务|
|G系列|传输系统和媒质、数字系统和网络|
|H系列|视听及多媒体系统|
|I系列|综合业务数字网|
|J系列|有线网络和电视、声音节目及其他多媒体信号的传输|
|K系列|干扰的防护|
|L系列|电缆和外部设备其它组件的结构、安装和保护|
|M系列|电信管理，包括 TMN和网络维护|
|N系列|维护：国际声音节目和电视传输电路|
|O系列|测量设备的技术规范|
|P系列|电话传输质量、电话设施及本地线路网络|
|Q系列|交换和信令|
|R系列|电报传输|
|S系列|电报业务终端设备|
|T系列|远程信息处理业务的终端设备|
|U系列|电报交换|
|V系列|电话网上的数据通信|
|X系列|数据网、开放系统通信和安全性|
|Y系列|全球信息基础设施、互联网的协议问题和下一代网络|
|Z系列|用于电信系统的语言和一般软件问题|








