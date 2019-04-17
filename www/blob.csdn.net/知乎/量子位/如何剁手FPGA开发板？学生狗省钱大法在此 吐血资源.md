# 如何剁手FPGA开发板？学生狗省钱大法在此|吐血资源 - 知乎
# 



> 作者 Joel Williams 
伊瓢 栗子 编译 
量子位 出品 | 公众号 QbitAI

博主Joel Williams在他的主页中分享了一篇购买便宜的FPGA开发板的攻略，量子位编译了本文。

我以前买了安富利（Avnet）49美元的Spartan 3A开发板，但不久之后，就在我决定再需要几十台的时候——它停产了。于是，我谷歌了很多资料，发现了这个省钱大法。
![](https://pic4.zhimg.com/v2-834d500b9a207b726f3a97c5de649953_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='225' height='225'></svg>)
在选择开发板时，需要考虑开发板的元器件和功能，结合自己需要的功能来对比。FPGA非常适合与高速外设一起使用，一般来说，购买包含需要部件的开发板要比后来加一个新板子容易得多，因为你总是不可避免地放弃这种组合形态，然后升级成更强大的电路板。

比如，你可能需要下面这些很难自己添加东西：

> 千兆以太网
HDMI / DVI
PCI / PCI Express
外部非串行内存（DDR / Flash等）

不过这些倒很容易自己加，不用太考虑：

> MMC / SD卡
字符LCD
I2C / SPI
VGA（低色深）

我喜欢有带有多个（至少8个）SPST开关和LED以及瞬时按钮的电路板。与微控制器相比，调制信息从串行端口或通过单个C函数调用的LCD调出相对容易，调试FPGA设计有点困难。LED提供了一种零操作方式来突破内部信号以实现可视化——如果你正在跟踪复杂状态机的进度，则可以在LED达到某一点时点亮LED，而无需添加任何额外的逻辑。

虽然这些很容易自己额外添加，但我发现最好找一块自带这些元件的电路板，这样你就不会浪费宝贵的I/O接口、或者浪费时间来研究你渣渣的焊接技术导致的故障了。
![](https://pic4.zhimg.com/v2-f8c0ff0e1601e473de5cfda7916a7a83_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='3000' height='471'></svg>)
一些制造商推出了附加产品的标准外形尺寸，比如Digilent家，包括各种各样的Pmod和Papilio Wings。

如果你想将高速设备（10-20 MHz以上）连接到FPGA，请确保你的主板有支持这个速度的接口连接器，在信号线、高速连接器（不仅仅是0.1“接头）、PCB走线长度均衡和阻抗控制之间有接地线，即使是便宜的电路板也不能缺了这些东西。

FPGA可能看起来比较难，因此请检查制造商是否提供：

> 电路图；
参考手册，介绍所有板载外设；
如果之前没用过FPGA，就需要看入门指南；
可以运行所有板载外设的参考设计。

参考设计可以是HDL或基于微控制器，但是近年来，大多数制造商似乎都倾向后者。如果没有微控制器和环境的许可证（例如Xilinx EDK / SDK不是免费的），就一定要记住这一点，因为代码很难移植到HDL。

如果是初学者，你可以买带教科书的电路板，专门给配套电路板编写，描述每个外设以及它们的连接方式。一些流行的电路板搞了很大的用户社区，但是这不一定有用，因为社区里的其他用户大部分也是初学者。最流行的主板是赛灵思、Digilent和Avnet。Terasic似乎是最受欢迎的Altera主板。

## **赛灵思ZYNQ**

赛灵思的Zynq器件由其Vivado高级综合设计套件提供支持，包括双核ARM Cortex-A9，USB 2.0和千兆以太网。

![](https://pic3.zhimg.com/v2-83664cb98c4b325c7065a888dc74004a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1130' height='1962'></svg>)
## **Artix-7**

Artix系列在廉价开发板中越来越常见，占据了先前Silinan-6在赛灵思阵容中占据的位置，尽管它们只提供BGA封装。

![](https://pic2.zhimg.com/v2-751c0189004828c3804c7d47fd38c809_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1126' height='1792'></svg>)
## **Spartan-7**
![](https://pic2.zhimg.com/v2-cd1a317a961648e49ae861b30c3b3379_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1126' height='310'></svg>)
## **Spartan-6**
![](https://pic3.zhimg.com/v2-1c9f2498b5df5fcb7b4e8a20ad4ba426_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1130' height='3888'></svg>)
## **Spartan-3**
![](https://pic2.zhimg.com/v2-9d159c38d824b13182c213eee2b44d69_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1124' height='2840'></svg>)
其他：

PLDkit为旧的赛灵思器件提供了许多低成本电路板—CPLD，Spartan 3和Virtex 4和5。

## **Lattice**
![](https://pic3.zhimg.com/v2-e5080478b7a532c76924cc79bb7d545e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1128' height='2776'></svg>)
## **Actel/Microsemi**
![](https://pic1.zhimg.com/v2-e9aef697bf14e4b87b9dc18c62a0554c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1134' height='1352'></svg>)
## **Altera Cyclone V**
![](https://pic3.zhimg.com/v2-c631820270723b89dd420f738d35faba_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1126' height='1348'></svg>)
## **Altera Cyclone IV**
![](https://pic4.zhimg.com/v2-1be9fd782fd39e83395506890199b68f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1138' height='1250'></svg>)
## **Altera Cyclone III**
![](https://pic3.zhimg.com/v2-46c710a8353dd8e827b705bdb776da72_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1132' height='266'></svg>)
## **Max 10，Cyclone II，及其他**
![](https://pic3.zhimg.com/v2-4eb3f4d461e79f06abaeba8796c81532_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1124' height='1810'></svg>)
## **Cypress**

严格意义上讲，并不是FPGA，是一些可重构的混合信号设备，也很有趣。
![](https://pic3.zhimg.com/v2-5ba22bfd6fd89f5e5a37b3369213ab86_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1128' height='556'></svg>)
## **还有什么？**

其实，FPGA-FAQ里，还有一个非常详尽的列表，包含了更多省钱之选。

大家可以前往传送门：
[http://www.fpga-faq.com/FPGA_Boards.shtml](https://link.zhihu.com/?target=http%3A//www.fpga-faq.com/FPGA_Boards.shtml)

另附本文表格传送门 (中有许多直通链接) ：

[https://joelw.id.au/FPGA/CheapFPGADevelopmentBoards](https://link.zhihu.com/?target=https%3A//joelw.id.au/FPGA/CheapFPGADevelopmentBoards)
![](https://pic2.zhimg.com/v2-816abfdcfdeb738c26c3944d57bfe069_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='580' height='385'></svg>)
—**完**—

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


