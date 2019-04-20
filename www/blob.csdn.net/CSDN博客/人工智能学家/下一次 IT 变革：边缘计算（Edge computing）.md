# 下一次 IT 变革：边缘计算（Edge computing） - 人工智能学家 - CSDN博客
2018年10月03日 20:44:34[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：3587
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhUYp6pLJujU36QCtg987bbYB4j13OtS2NtR08uN74KlYVo5zLSwibLrg/640?wx_fmt=jpeg)
来源：云头条
摘要：外媒 ZDnet 发布了此篇边缘计算领域的重要文章，详细分析了各机构不同的定义、市场状况、前景等，经编译，供各位参考
数十亿物联网设备和5G网络这两股力量必将推动计算工作负载的部署方式发生深远而重大的变化。
近年来，计算工作负载一直在迁移：先是从本地数据中心迁移到云，现在日益从云数据中心迁移到更靠近所处理的数据源的“边缘”位置。旨在缩短数据的传输距离，从而消除带宽和延迟问题，最终提升应用和服务的性能和可靠性，并降低运行成本。
这倒不是说本地环境或云中心已死亡；一些数据总是需要存储在集中的地方并加以处理。但数字基础设施无疑在发生变化。比如据Gartner声称，到2025年，80%的企业将关闭传统数据中心，而2018年这个比例只有10%。工作负载的布置是这次基础设施演变的关键驱动因素，工作负载的布置有赖于各种业务需求，这家调研公司称：
随着最近业务驱动的IT项目不断增加（通常超出传统IT预算的范围），实施物联网解决方案、边缘计算环境和“非传统”IT的现象在迅速增多。此外日益关注面向外部的应用的客户体验，并日益关注糟糕的客户体验给企业声誉带来的直接影响。关注外部的这种形势导致许多企业组织基于网络延迟、客户群体和地缘政治限制因素（比如欧盟的《通用数据保护条例》即GDPR或监管方面的限制），重新考虑某些应用的布置。
当然，边缘计算面临挑战，尤其是连接方面（连接断断续续），具体表现为网络边缘处带宽低及/或延迟高。如果大量智能边缘设备运行的软件（比如机器学习应用）需要与中央云服务器或“雾计算”中的节点进行联系，这就会出现问题。然而，解决方案正在开发中。
由于边缘计算处于Gartner 2018年云计算成熟度曲线的高峰期，在标准和最佳实践得以确定之前，还要经历一段相当长的幻灭期，之后才会得到主流界的采用。本文旨在评估边缘计算的现状。
**不同机构的定义**
边缘计算是个比较新的概念，已经与另一个术语“雾计算”联系起来，这可能会在非专业观察人士当中造成混淆。下面是几个定义，但愿可以澄清一下。
•Futurum Research：不像云计算依赖数据中心和通信带宽来处理和分析数据，边缘计算在网络边缘附近处理和分析数据，数据最初在边缘附近加以收集。边缘计算（专注于在网络节点层面处理和分析的一类雾计算）......应被视为雾计算的一个事实上的要素。
•State of the Edge 2018：将计算能力提供给网络的逻辑末端，以便改善应用和服务的性能、运营成本和可靠性。通过缩短设备与提供给设备的云资源之间的距离，另外减少网络跳数，边缘计算消除了当今互联网在延迟和带宽方面的限制，从而带来全新的应用类别。实际上，这意味着沿着今天的集中式数据中心和越来越多的现场设备之间的路径，分配新的资源和软件堆栈，而这条路径邻近最后一英里网络，连接基础设施和设备两端。
•451 Research/开放雾联盟：[雾]的一端始于边缘设备（我们在这里定义的边缘设备是生成传感器数据的那些设备，比如车辆、制造设备和“智能”医疗设备），这些设备拥有必要的计算硬件、操作系统、应用软件和连接功能，以便参与分布式分析雾。它从边缘延伸到“近边缘”功能，比如本地数据中心及其他计算资产、企业或运营商无线接入网络内的多接入边缘（MEC）功能、托管服务提供商内的中间计算和存储功能以及互连和托管设施，最终延伸到云服务提供商。这些位置有集成或主机“雾节点”，这些其实是能够参与整个分布式分析系统的设备。
•David Linthicum（德勤咨询公司的首席云战略官）：“就边缘计算而言，计算系统和存储系统也位于边缘，尽可能接近生成所处理的数据的部件、设备、应用或人。目的是消除处理延迟，因为数据不必从网络边缘发送到中央处理系统，然后发回到边缘......思科首创的“雾计算”这个术语还指将计算扩展到网络边缘。思科在2014年1月推出了其雾计算，以此将云计算功能引入到网络边缘......实际上，雾是标准，边缘是概念。雾实现了边缘计算概念中的可重复结构，那样企业就可以将计算推送到集中系统或云之外的地方，以获得更好的性能和可扩展性。”
开放雾联盟在下面直观地显示了网络边缘处生成数据的“物件”、核心处的云数据中心以及介于两者之间的雾基础设施三者之间的关系：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhZKUcTvRQLgXBqQeSPrpro0FoFr5ricwVQibYIfsOb5LSkpOiad0KknCDw/640?wx_fmt=png)
图片来源：开放雾联盟
**市场估计**
据B2B调研公司MarketsandMarkets声称，到2022年，边缘计算市场的价值将达到67.2亿美元，比2017年的14.7亿美元大幅增多，年复合增长率（CAGR）高达35.4%。几大驱动因素是物联网和5G网络的出现、“智能”应用的数量增加以及云基础设施上的负载越来越大。
**边缘计算市场状况**
|驱动因素|- 云基础设施上的负载越来越大- 不同行业有种类广泛的应用- 智能应用的数量增加|
|----|----|
|制约因素|- 与边缘计算有关的安全和隐私问题|
|机会|- 5G网络出现|
|挑战|- 不够到位的行业标准和互操作性问题|
数据来源：MarketsandMarkets
在MarketsandMarkets考虑的几个垂直细分领域中，预计电信IT在2017年至2022年预测期内拥有最大的市场份额。这是由于企业面临繁重的网络负载，对带宽需求又越来越高，因而需要优化和扩展无线接入网络（RAN），以便为其应用和服务提供高效的移动（或多接入）边缘计算（MEC）环境。
MarketsandMarkets表示，在预测期内，边缘计算市场增长最快的细分领域很可能是零售：与智能应用相连接的物联网传感器、摄像头和信标生成大量数据，这些数据可以更有效地在网络边缘而不是在云或本地数据中心加以收集、存储和处理。
Grand View Research的观点较为保守，估计到2025年边缘计算市场的价值将达到32.4亿美元，不过在2017年至2025年预测期内，CAGR仍达到“惊人”的41%。这家研究公司表示，从地区来看，由于物联网设备在美国和加拿大的普及率日增，北美将领跑全球市场，CAGR最高的垂直细分领域将是医疗保健和生命科学，这归因于“边缘计算解决方案提供了存储能力和实时计算”。Grand View Research表示，由于边缘计算解决方案能够降低运营成本，中小企业在预测期内的CAGR将最高（46.5%）。
增长形势最乐观的预测来自451 Research，2017年10月开放雾联盟委托这家调研公司搞了一份研究，名为《雾计算市场的规模和影响》。这项广泛的研究估计到2022年，雾计算的市场规模有望达到182亿美元，远高于2018年的10.3亿美元和2019年的37亿美元，2018年至2022年的CAGR高达104.9%。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhia4BjyjrJY5pfib39PHlVPJ4fBZSItzA6GGtslW4HA5rqbpicbjS1wO3g/640?wx_fmt=png)
数据来源：451 Research和开放雾联盟
据451 Research声称，就市场份额而言，2022年雾计算领先的垂直细分领域将是公用事业、交通运输、医疗保健、工业和农业。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhzobmy2Fn1u80YmhkLY7csGRJS32OvibJow5lWOgIVo9WL3e2lZCvUiaQ/640?wx_fmt=png)
图片来源：451 Research和开放雾联盟
说到2022年的雾计算生态系统，451 Research分成了这几大部分：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhZTsGzf8nvlyFdWbIqvR9icHbSrKLxPdo53EwDCQicicDdyDsaeZPqsfkQ/640?wx_fmt=png)
数据来源：451 Research和开放雾联盟
硬件这部分遥遥领先，在2022年饼形图中占有42.1%的份额，其次是雾应用/平台（21.5%）和雾服务（20.4%）。难怪硬件供应商和云应用/服务提供商在竞相涌入迅速发展的边缘/雾市场。
尽管侧重点各不相同，但这些预测清楚地表明，两股合力正形成边缘计算的“完美风暴”：数量快速增加的联网设备和即将出现的高带宽低延迟5G网络。爱立信2018年6月的《移动报告》总结了这些领域的预期发展动向。
虽然PC、笔记本电脑、平板电脑以及手机在2017年至2023年期间增长平缓，但物联网设备增长迅猛：拥有广域连接的设备其CAGR将达到30%，短距离物联网设备的增长势头要弱一些（CAGR为17%）。这导致2017年（175亿个）至2023年（314亿个）的联网设备数量增长近80%（79.4%）：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhN3mHt9n30NJR94aFneqGkDVQwhNaiaOSzxnfmwKSkGbla0MjO2V7xsA/640?wx_fmt=png)
*蜂窝物联网设备是广域物联网设备的子集
数据来源：爱立信《移动报告》2018年6月
至于5G，爱立信预计2018年下半年会出现首批纯数据设备，2019年出现首批5G智能手机。继第三代芯片组在2020年问世后，到2023年，该公司预计全球会有10亿部联网的5G设备。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhKRk0e43lJXKcedJTibwMfKqiaZd2Vl7mDdLzPo0t5Rou2iclibwTE0ZNRw/640?wx_fmt=png)
CPE / FWT：客户提供的设备/固定无线终端
图片来源：爱立信《移动报告》2018年6月
爱立信表示，预计2020年会出现首批基于模块的5G物联网设备，支持面向工业过程监控的超低延迟通信。
**标准和组织**
任何新的IT项目都需要标准和最佳实践，而早期阶段的特点常常是多家组织和联盟林立，它们的工作议程各不相同（尽管成员常常存在很大的重叠）。边缘/雾计算也不例外。
雾计算是思科杜撰的一个术语，得到了开放雾联盟的支持，该联盟由Arm、思科、戴尔、英特尔、微软和普林斯顿大学边缘实验室于2015年共同成立。其使命内容如下（部分）：
我们致力于定义涵盖分布式计算、网络、存储、控制和资源的一种架构，以支持物联网边缘处的智能，包括自我感知的自主机器、物件、设备和智能对象。开放雾联盟的成员还将确定和开发新的运营模式。最终，我们的工作将有助于实现和推动下一代物联网。
边缘计算由Linux基金会主持的开源项目EdgeX Foundry负责推广。EdgeX Foundry的目标包括：构建和推广EdgeX成为统一物联网边缘计算的通用平台，认证EdgeX组件以确保互操作性和兼容性，提供快速创建基于EdgeX的物联网边缘解决方案的工具，并与相关的开源项目、标准组织和行业联盟合作。
据EdgeX Foundry声称：“该项目注重的是边缘节点，比如嵌入式PC、集线器、网关、路由器和本地服务器，以克服分布式物联网雾架构中互操作性方面的重大挑战。”
EdgeX Foundry的技术指导委员会包括来自IOTech、ADI、Mainflux、戴尔、Linux基金会、三星电子、VMware和Canonical的代表。
这个领域还有另外两大行业机构：一个是注重日本市场的EdgeCross联盟，由欧姆龙公司、Advantech、NEC、IBM日本、Oracle日本和三菱电机于2017年11月共同成立；另一个是工业互联网联盟，由AT&T、思科、通用电气、英特尔和IBM于2014年共同成立。
**调查说明了什么？**
Futurum Research在2017年底调查了500多家北美公司（员工数量从500人到50000人不等），以了解它们在边缘计算方面的状况：采用、部署以及投资意向等。Futurum表示，所有调查对象都对边缘计算的投资决策有影响，其中41.8%是“运营人员”，25.6%是“董事、经理和团队负责人”，不过只有8.6%属于“主管、高管、老板和合作伙伴”。
Futurum声称，近四分之三（72.7%）的公司已经实施了边缘计算战略，或正在实施这一战略。此外，几乎所有（93.3%）的公司打算在今后12个月内投入于边缘计算：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhsmP8xqcWTxunSUHlz4lLVgCgU9CdHSy7vgY3x5uuQkuO9U2KAyME9w/640?wx_fmt=png)
数据来源：Futurum Research
Futurum还编制了一份通用的数字化转型指数，该指数在2018年将68%的公司列入“领导者”和“采用者”这两个类别。因此，72.7%的调查对象已经投入于边缘计算，这表明边缘计算对于精通技术的企业来说是个热门话题。然而Futurum也特别指出，“93.3%的企业在今后12个月内投入于边缘计算的热情说明不了它们的投入规模。”
被问及边缘计算数据流在业务流程中的重要性时，Futurum的调查对象继续给出了积极的看法，71.8%的调查对象称这种数据流“至关重要”（22.2%）或“非常重要”（49.6%）：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhhqCGdyHXxxkfjuy5TaUYeHvdFPvOA4Ab6ySbCasWZvGgTEu5MFzLDg/640?wx_fmt=png)
数据来源：Futurum Research
哪些关键因素促使企业对边缘计算有这么高涨的热情？对于Futurum的调查对象来说，最主要的因素是“提升应用性能”，其次是“实时分析/数据流”：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqh077F7pyRnGF2WQK4IE64iaPz9gIibbld6TjIzrGZQY49Nf1jvJec0aTA/640?wx_fmt=png)
数据来源：Futurum Research
这家调研公司解释，这些优先事项体现了企业需要提升运营效率，表明物联网战略比较低的排名在“未来几年可能会上升”，物联网战略常常被认为是边缘计算的一种典型的使用场景。
这家研究公司表示，只有15.6%的调查对象旨在将边缘计算和云计算分开来，这个决定常常有赖于数据和系统安全方面的问题以及注重划分开来的运营。因而，近64%（63.9%）的调查对象已经部署（28.3%）或正在物色（35.6%）综合的边缘/数据中心分析解决方案，另外20.5%的调查对象不确定将这些功能组合起来还是分开来：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhDRB7YoHWjlJw3u4PaxX5QInOOusQCEDy7tH7ic2UbUZKvWAVYT7ZQuQ/640?wx_fmt=png)
数据来源：Futurum Research
回答“不确定”和“正在物色”的占调查样本总数的56.1%，这显然表明边缘计算提供商大有机会。
2017年12月，开放雾联盟就雾计算行情调查了其61家成员组织，发现多达70%的CEO意识到雾计算项目已上了工作议程。
2018年雾计算预算普遍增加（40%）或保持不变（51%），只有5%的调查对象声称预算减少。这类项目主要归属研发部门（51%），绝大多数将物联网应用视作主要的关注领域（70%）。
安全是调查对象当中最关注的问题（32%），其次是担心早期/未经证实的技术、互操作性和不明确的投资回报率。促使企业对雾计算备感兴趣的主要因素是延迟和带宽问题。调查对象预计制造、智慧城市和交通运输将是采用雾计算的几大行业领域，其次是能源、医疗保健和智能家居。
**主要供应商**
边缘/雾计算可以将工作负载从云数据中心提取出来，因此看到云巨头采取措施、防止那些工作负载逃离其轨道也就不足为奇了。
- 
**亚马逊**
AWS Greengrass在亚马逊的2016年re：Invent开发者大会上推出，立足于该公司现有的物联网和Lambda（Serverless计算）产品，旨在将AWS扩展到间歇连接的边缘设备。
亚马逊称：“借助AWS Greengrass，开发人员可以径直从AWS管理控制台，将AWS Lambda函数添加到联网设备，而设备在本地执行代码，以便设备可以响应事件，并近乎实时地执行操作。AWS Greengrass还包括AWS物联网消息传递和同步功能，因此设备可以在不连回到云的情况下向其他设备发送消息。AWS Greengrass让客户可以灵活地让设备在必要时依赖云，在必要时自行执行任务，并在必要时相互联系，这一切都在一个无缝的环境中进行。”
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhSibHmDNw4hs01TExoAmxLian0lOPj7phRB937YwsNibkCSYM2au0G1wMA/640?wx_fmt=png)
图片来源：AWS
当然，这些是“智能”边缘设备：Greengrass需要至少1GHz的计算芯片（Arm或x86）、128MB内存，还有操作系统、消息吞吐量和AWS Lambda执行所需的额外资源。据亚马逊声称：“Greengrass Core可以在从Raspberry Pi到服务器级设备的多种设备上运行。”
- 
**微软**
Azure IoT Edge在微软的BUILD 2017开发者大会上推出，2018年6月以后开始面市，它让云工作负载可实现容器化，并在从Raspberry Pi到工业网关的多种智能设备上本地运行。
Azure IoT Edge包含三个部分：IoT Edge模块、IoT Edge运行时环境和IoT中心。IoT Edge模块是运行Azure服务、第三方服务或自定义代码的容器，它们部署到IoT Edge设备上，并在本地执行。IoT Edge运行时环境在每个IoT Edge设备上运行，管理已部署的模块。而IoT中心是基于云的界面，用于远程监控和管理IoT Edge设备。
下图显示了Azure IoT Edge的不同部分如何组合在一起：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhpPjqf807FKn3karYaETSBIuiat4jpNhOvIJhtic9ACldN70eRZW2xa0A/640?wx_fmt=png)
图片来源：微软
推出正式版后，微软为Azure IoT Edge添加了新功能，包括支持开源、设备配置、安全和管理服务以及简化的开发者体验。
- 
**谷歌**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhyhFpsXoicchrz5TOHgBgiaEXyF0Soutp8HU3wpoRtaZsEe1kBJvMWZhQ/640?wx_fmt=jpeg)
谷歌的Edge TPU ASIC与1美分硬币相比
2018年7月，谷歌宣布推出两款大规模开发和部署智能联网设备的产品：Edge TPU和Cloud IoT Edge。Edge TPU是一种专用的小型ASIC芯片，旨在在边缘设备上运行TensorFlow Lite机器学习模型。Cloud IoT Edge是软件堆栈，负责将谷歌的云服务扩展到物联网网关和边缘设备。
Cloud IoT Edge有三个主要组件：便于网关级设备（至少有一个CPU）存储、转换和处理边缘数据，并从中提取信息的运行时环境，同时与谷歌云IoT平台的其余组件协同操作； Edge IoT Core运行时环境，可将边缘设备安全地连接到云；以及 Edge ML运行时环境，它基于TensorFlow Lite，使用预先训练的模型执行机器学习推理。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/tJ5NYyM9MHCjRbbF9qtReXsWskvBODqhEicQqRsAAHDTLeKvdkvRAKcePYDoLsk4NMzgp7skv9dYOxpTWywHNJA/640?wx_fmt=png)
图片来源：谷歌
撰写本文时（2018年9月），Edge TPU和Cloud IoT Edge都处于alpha测试阶段。
**前景展望**
向边缘/雾计算转变是IT界定期出现的一种模式转变，比如就像从大型机转向台式机、转向本地数据中心，再转向云数据中心。现在，我们看到众多的现有组件连同数十亿个智能物联网设备由网关和节点组成的“雾”连接起来。设备连接一向是阻碍这种转变的瓶颈，但随着5G移动网络问世，这种情况将得到极大的改善。
凡是得益于及时分析物联网数据流的行业部门（几乎各行各业）都会对边缘/雾计算感兴趣。这就是为什么技术堆栈所有层面的供应商都大有机会：标准、网络、计算、存储、应用和服务。
由于越来越多的数据在更多的地方生成、处理和存储，基础设施管理以及数据安全、隐私和治理方面的问题会变得比现在更重要。但愿那些问题尽快得到解决。
原文链接：https://www.zdnet.com/article/edge-computing-the-state-of-the-next-it-transformation/
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
