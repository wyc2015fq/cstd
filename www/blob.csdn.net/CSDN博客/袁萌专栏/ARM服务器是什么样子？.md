# ARM服务器是什么样子？ - 袁萌专栏 - CSDN博客
2011年10月29日 07:21:30[yuanmeng001](https://me.csdn.net/yuanmeng001)阅读数：1247标签：[服务器																[ubuntu																[dell																[server																[互联网																[rack](https://so.csdn.net/so/search/s.do?q=rack&t=blog)](https://so.csdn.net/so/search/s.do?q=互联网&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=dell&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)
         10月25~27日，在美国加州SantaClara市举办“ARM
 Techcon 2011”年会，吸引了全球业界人士的目光。预装Ubuntu11.10（服务器版）的ARM服务器在展示现场“亮相”。ARM服务器是个什么样子呢？为什么惠普、Dell这些服务器世界巨头对其都“眼红不已“呢？
折腾了二十多年，ARM处理器的基本部件“核心”（所谓“Core”）还是老样子，基本上没有什么大的变化，只是加工工艺更加精细而已，每个“核心”其中有3万多个晶体管“颗粒”，每个“核心”里面只有16个寄存器（构成32位系统），每个”核心“耗电不足0.5瓦。后来，有人把4个这种“核心”放在一起，组成所谓“4核”的ARM处理器（也叫SoC，单芯片系统），将其作为一个基本处理单元，叫做ARM“结点”（”Node”）。每个“结点”的DRAM（内存）用光纤连接起来，由数十个“结点”放在一个机架（Rack）里面就成了一个ARM处理器（SoC）构成的所谓“ARM服务器”了，其外观大致如下：
![](http://www.linuxfordevices.com/images/stories/ztsystems_r1801e_sm.jpg)
**ZT Systems' R1801e**
总之，严格地说，ARM服务器是一种“模块组成的计算机”（COM，即“Computor
 on Modules”），好似由许多小智能手机主板堆砌起来的”物理组合物“（相互之间有光纤连接）。比如，上图就是ZT系统公司于去年年底制造的基于Ubuntu操作系统的ARM服务器，除了固态存储器（SSD）之外，由8个ARMCortex-A9“模块”组成，共计具有16个ARM“核心”（600MHz），耗电小于80瓦。
大家已经看到，ARM服务器就像一个“蜂窝”（”Cell”），有许多“核心”在同时协同计算（动作），有哪一款操作系统能够制服这些“小精灵”？让我们回到2008年，Ubuntu社区率先采取了一个“Wisemove”(聪明的步骤)把Ubuntu操作系统“Port”（“停靠”）到ARM处理器上，实乃英明远见也。实际上，Ubuntu11.10是世界上第一款完备支持ARM服务器的操作系统。本月25～27日，在“ARM
 Techcon 2011”展示会上，Ubuntu现场展出的新款ARM服务器大出“风头"，夺人眼球。
有人算过一笔“帐”，ARM处理器模块耗电只有5瓦，而相应的x86处理器模块的一般耗电量却有85～135瓦（其“低功耗”模式电耗也有20W瓦），两者平均相差一个数量级。近日，据国外媒体有关报道，惠普将在11月1日发布其首款ARM服务器。大家知道，在前不久，Canonical公司（即Ubuntu首要支持者）宣布将与惠普共建“公有云”，提供“云服务”。十分明显的事实是，ARM服务器将给互联网“云中心”带来“福音”（即“节电”），也给惠普等服务器厂商带来丰厚的利润。
说明：“HP could save about $200 per ARM-based
 server”，惠普出售ARM服务器，此举将会使其增加收入：“add
 about $220 million in operating income in 2012”，对此，连Dell都“眼红”了。在此，我顺便说一句，ARM服务器潮流是“大局”，目前，借助ARM服务器兴起的东风，Ubuntu国际社区正在杨帆启航，得意非凡也。
