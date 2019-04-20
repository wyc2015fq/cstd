# 为什么说特斯拉研发自动驾驶AI芯片应该引起注意？ - 人工智能学家 - CSDN博客
2017年12月09日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：217
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWHzJdSJrKgGQ56fff9dTflcVOJs1xhzuFKUPjNQehS2vjox2p4tyic6dNExRxYQS6fSmrAQ2HHVHw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
*来源：36Kr*
*概要**：对于特斯拉而言，研发这款芯片+配套算法本质上还是对率先将自动驾驶汽车商业化节点的争夺。*
特斯拉Model 3的量产问题仍未彻底解决，CEO Elon Musk又抛出了自研自动驾驶芯片的重磅新闻。
Elon Musk和特斯拉Autopilot负责人Jim Keller在昨天的神经信息处理系统大会（NIPS 2017）上是这么说的：「I wanted to make it clear that Tesla is serious about AI, both on the software and hardware fronts. We are developing
 custom AI hardware chips.Jim is developing specialized AI hardware that we think will be the best in the world.」
（我想明确一点：特斯拉非常重视AI，无论软件还是硬件层面。我们正在开发定制的AI芯片硬件。我们认为Jim带队开发的专用型AI芯片将会是全球最好的芯片。）
在此之前，36氪曾多次撰文阐述特斯拉自研芯片的可能性，这是第一次迎来官方实锤。为什么说特斯拉研发自动驾驶AI芯片应该引起注意？
**先简单科普下Jim Keller。Jim Keller，原AMD首席芯片架构师。**
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWHzJdSJrKgGQ56fff9dTflGrCnplumMJUW3pH8uickvSibK1Q4WbicHibEj4fFL1gyStPykUAv1QWZjQ/0?wx_fmt=png)
1998年，Jim Keller在AMD分别参与设计和主导研发了Athlon和Opteron 64处理器（K7和K8 X86-64架构），帮助AMD攀上了业务发展的顶峰。1999年，Jim Keller离职加盟博通出任首席芯片架构师。
2004年，Jim Keller转投P.A Semi，后者于2008年被苹果收购。Jim Keller出任苹果移动芯片架构师，主导放弃AMD的公版架构，基于AMD的IP深度定制了苹果A4/5芯片，在苹果A系列芯片+iOS「软硬一体化」战略的落地中发挥了关键作用。
2012年，Jim Keller重回AMD，领导开发了Zen架构处理器，带领AMD咸鱼翻身；2015年9月，Jim Keller再次离职，彼时Zen架构处理器已经完成了架构设计，但AMD股价仍应声下跌。
因为辗转多个公司均做出重大贡献的传奇经历，坊间戏称Jim Keller是「Chip God」。（芯片皇帝）
2016年1月加盟特斯拉，带着50人规模的团队，到本月底刚满两年，Jim Keller团队在特斯拉堪称高效。
下一个问题是，搭载英伟达Drive PX 2自动驾驶芯片的特斯拉Autopilot 2.0车型2016年10月才量产，Drive PX 2芯片也是整个自动驾驶产业界最受欢迎的平台之一，特斯拉为什么要自行研发自动驾驶芯片？
L4级别的自动驾驶存在着非常大的算力和数据传输需求，英特尔的一份报告指出，自动驾驶汽车的计算量可以达到4TB/天，这也是当下诸多OEM的自动驾驶汽车后备箱放置着巨大的计算及配套的散热设备的原因。英伟达此前推出了旗舰级Pegasus自动驾驶芯片，这款芯片的算力达到了320万亿次浮点运算/秒，在鲁棒性、多级冗余及撤回机制、车规级ASIL D安全性和数据带宽方面均有更好的支持。唯一不可回避的缺陷是，这款芯片功耗飙升至500W，是前代Xavier芯片的15倍以上。
这款芯片暴露出一个问题：对自动驾驶而言，即便是英伟达这样顶级GPU大厂，在算力和功耗的平衡上也已经触到了天花板。更宏观的说，通用型计算平台很难同时满足自动驾驶要求的巨大算力和超低功耗。
如何解决这一难题？这里举一个题外案例：在Google确立了AI First战略之后，很快发现了一个问题：即使大规模的部署英伟达的GPU，数据中心的功耗仍然飞速上涨。2015年，Google研发数年的AI专用芯片TPU（张量处理单元）开始投入应用，TPU是从芯片架构层面专为机器学习设计和研发的高效能芯片。Google在一篇论文中介绍，TPU比GPU/CPU 快15~30倍、性能功耗比（TOPS/Watt）高出约30~80倍。
具体到自动驾驶领域，专用计算平台能带来多大的能效提升？在特斯拉自研芯片计划的同时，国内也有一家专攻专用型芯片的创业公司地平线宣告成立。地平线创始人兼CEO余凯博士此前介绍，在相对通用型计算平台1/10的功耗下，地平线的BPU会有2~3个数量级（100倍~1000倍）的算力提升。就在刚刚，余凯转发特斯拉确认自研芯片称「在意料之中、战争才刚开始，这是勇敢者的游戏。」
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWHzJdSJrKgGQ56fff9dTflQx2mjZOMEb8AnPrJciaBw2c3V2n0vztC0tHAbNCJgE8v6JQOHUibibyRw/0?wx_fmt=png)
**地平线官网**
回到那个问题，特斯拉为什么要自行研发自动驾驶芯片？Musk说「功耗可以降至当前的1/10」，Keller在演讲中提到「定制硬件可以提升效率」。小鹏汽车副总裁、前特斯拉Autopilot机器学习负责人谷俊丽提到过，特斯拉的自动驾驶策略和其他公司的最大不同，就是整套软硬件解决方案，包括计算平台、传感器和执行机构，全部要求可量产、可商业化。这也是特斯拉坚持不使用激光雷达（成本高昂）和比其他公司更早关注功耗问题的原因。
其实在此之前，特斯拉便有自研芯片的蛛丝马迹流出——在特斯拉Q3财报电话会议上，Elon Musk被要求就英伟达Pegasus芯片比特斯拉Autopilot 2.0上的Drive PX 2性能好十倍，阐述特斯拉下一步的自动驾驶硬件策略。Musk的回应并未针对英伟达，而是围绕特斯拉本身：
「we'll have more to say on the hardware front soon, we're just not ready to say anything now. But I feel very optimistic on that front……we feel confident of the competitiveness of our
 hardware strategy. I would say that, we are certain that our hardware strategy is better than any other option, by a lot.」
（我们很快就会有一些硬件层面的新进展宣布，只是还没有万事俱备，但我对此非常乐观……我们对特斯拉硬件战略的竞争力充满信心，我想说的是，我们确信特斯拉的硬件战略比任何解决方案都好不止一个量级。）
什么方案可以比哪怕号称全球首款L5级自动驾驶芯片英伟达Pegasus还要“好不止一个量级”？唯有重构芯片架构的专用型芯片可以带来这样的突破。
关于这款神秘芯片的其他进展还包括：该芯片基于AMD的IP打造；目前已经走到了设计完成、测试验证的阶段；特斯拉已经收到了首批芯片样品，目前正在进行相关测试；代工方可能是格罗方德和三星电子等。
尽管短期来看，英特尔、英伟达等传统芯片大厂的自动驾驶芯片仍然有着广阔的市场空间，但毋庸置疑的是，率先将专用型自动驾驶芯片商业化的企业将在未来的市场竞争中占据更加主动的地位。对于特斯拉而言，研发这款芯片+配套算法本质上还是对率先将自动驾驶汽车商业化节点的争夺。摆在其他自动驾驶企业面前的问题是，跟还是不跟？
未来智能实验室致力于研究互联网与人工智能未来发展趋势，观察评估人工智能发展水平，由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎支持和加入我们。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXoz49H9w8lfGXmBjcGga9W15bmYAvZDqbbI8p0liaLcxEh1iao7YXIgJvp6MLHpskd7jHq8ibX0QzDQ/640?wx_fmt=png)
