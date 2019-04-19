# LabVIEW_百度百科 - xqhrs232的专栏 - CSDN博客
2018年02月13日 14:32:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：125
个人分类：[LabView/测控软件/工控技术](https://blog.csdn.net/xqhrs232/article/category/7457229)
原文地址::[https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin)
相关文章
1、[http://www.ni.com/zh-cn/innovations.html](http://www.ni.com/zh-cn/innovations.html)
# LabVIEW
[锁定](https://baike.baidu.com/view/10812319.htm)
本词条由[“科普中国”百科科学词条编写与应用工作项目](http://www.cast.org.cn/) 审核 。
LabVIEW是一种程序开发环境，由[美国](https://baike.baidu.com/item/%E7%BE%8E%E5%9B%BD)国家仪器（NI）公司研制开发，类似于C和BASIC开发环境，但是LabVIEW与其他计算机语言的显著区别是：其他计算机语言都是采用基于文本的语言产生代码，而LabVIEW使用的是图形化编辑语言G编写程序，产生的程序是框图的形式。
 LabVIEW软件是NI设计平台的核心，也是开发测量或控制系统的理想选择。 LabVIEW开发环境集成了工程师和科学家快速构建各种应用所需的所有工具，旨在帮助工程师和科学家解决问题、提高生产力和不断创新。
- 
中文名
LabVIEW- 
国    家
[美国](https://baike.baidu.com/item/%E7%BE%8E%E5%9B%BD)- 
公    司
[美国](https://baike.baidu.com/item/%E7%BE%8E%E5%9B%BD)国家仪器（NI）公司- 
类    型
软件
## 目录
- 1[解析](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#1)
- 2[特点](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#2)
- 3[虚拟仪器](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#3)
- 4[应用领域](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#4)
- 5[开发系统](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#5)
- 6[附件模块](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#6)
- 7[版本信息](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#7)
- 8[参考书籍](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#8)
## 解析
与 C 和[BASIC](https://baike.baidu.com/item/BASIC) 一样，LabVIEW[1] 也是通用的编程系统，有一个完成任何编程任务的庞大函数库。LabVIEW[2] 的函数库包括数据采集、GPIB、串口控制、[数据分析](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E5%88%86%E6%9E%90)、数据
![LabVIEW标志](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=6357d5187bec54e745ec1d1c89389bfd/18d8bc3eb13533fac0e3cb08a8d3fd1f41345bba.jpg)LabVIEW标志
显示及[数据存储](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E5%AD%98%E5%82%A8)，等等。LabVIEW[2] 也有传统的程序调试工具，如设置[断点](https://baike.baidu.com/item/%E6%96%AD%E7%82%B9)、以动画方式显示数据及其子程序（子VI）的结果、单步执行等等，便于程序的调试。
LabVIEW[1] （Laboratory
 Virtual Instrument Engineering Workbench）是一种用图标代替文本行创建应用程序的图形化编程语言。传统文本编程语言根据语句和指令的先后顺序决定程序执行顺序，而 LabVIEW 则采用数据流编程方式，程序框图中节点之间的数据流向决定了VI及函数的执行顺序。VI指虚拟仪器，是 LabVIEW 的程序模块。
LabVIEW[1] 提供很多外观与传统仪器（如示波器、万用表）类似的控件，可用来方便地创建[用户界面](https://baike.baidu.com/item/%E7%94%A8%E6%88%B7%E7%95%8C%E9%9D%A2)。用户界面在
 LabVIEW 中被称为前面板。使用图标和连线，可以通过编程对前面板上的对象进行控制。这就是图形化源代码，又称G代码。LabVIEW 的图形化源代码在某种程度上类似于流程图，因此又被称作程序框图代码。
## 特点
尽可能采用了通用的硬件，各种仪器的差异主要是[软件](https://baike.baidu.com/item/%E8%BD%AF%E4%BB%B6)。
可充分发挥计算机的能力，有强大的数据处理功能，可以创造出功能更强的[仪器](https://baike.baidu.com/item/%E4%BB%AA%E5%99%A8)。
用户可以根据自己的需要定义和制造各种仪器。
未来
虚拟仪器研究的另一个问题是各种标准仪器的互连及与计算机的连接。目前使用较多的是[IEEE](https://baike.baidu.com/item/IEEE)488 或 [GPIB](https://baike.baidu.com/item/GPIB)协议。未来的仪器也应当是网络化的。[2]
LabVIEW[2] （Laboratory
 Virtual instrument Engineering Workbench）是一种图形化的编程语言的开发环境，它广泛地被工业界、学术界和研究实验室所接受，视为一个标准的[数据采集](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E9%87%87%E9%9B%86)和仪器控制软件。LabVIEW [2] 集成了与满足
 GPIB、VXI、[RS-232](https://baike.baidu.com/item/RS-232)和 RS-485 协议的硬件及数据采集卡通讯的全部功能。它还内置了便于应用[TCP/IP](https://baike.baidu.com/item/TCP%2FIP)、[ActiveX](https://baike.baidu.com/item/ActiveX)等软件标准的库函数。这是一个功能强大且灵活的软件。利用它可以方便地建立自己的虚拟仪器，其图形化的界面使得编程及使用过程都生动有趣。
图形化的程序语言，又称为 “G” 语言。使用这种语言编程时，基本上不写程序代码，取而代之的是[流程图](https://baike.baidu.com/item/%E6%B5%81%E7%A8%8B%E5%9B%BE)或[框图](https://baike.baidu.com/item/%E6%A1%86%E5%9B%BE)。它尽可能利用了技术人员、[科学家](https://baike.baidu.com/item/%E7%A7%91%E5%AD%A6%E5%AE%B6)、[工程师](https://baike.baidu.com/item/%E5%B7%A5%E7%A8%8B%E5%B8%88)所熟悉的术语、图标和概念，因此，LabVIEW[2] 是一个面向最终用户的工具。它可以增强你构建自己的科学和工程系统的能力，提供了实现仪器编程和数据采集系统的便捷途径。使用它进行原理研究、设计、测试并实现仪器系统时，可以大大提高工作效率。
利用 LabVIEW[2] ，可产生独立运行的[可执行文件](https://baike.baidu.com/item/%E5%8F%AF%E6%89%A7%E8%A1%8C%E6%96%87%E4%BB%B6)，它是一个真正的32位/64位[编译器](https://baike.baidu.com/item/%E7%BC%96%E8%AF%91%E5%99%A8)。像许多重要的软件一样，LabVIEW[2] 提供了[Windows](https://baike.baidu.com/item/Windows)、[UNIX](https://baike.baidu.com/item/UNIX)、[Linux](https://baike.baidu.com/item/Linux)、[Macintosh](https://baike.baidu.com/item/Macintosh)的多种版本。
它主要的方便就是，一个硬件的情况下，可以通过改变软件，就可以实现不同的仪器仪表的功能，非常方便，是相当于软件即硬件！现在的图形化主要是上层的系统，国内现在已经开发出图形化的单片机编程系统（支持32位的嵌入式系统，并且可以扩展的），不断完善中(大家可以搜索 CPUVIEW 会有更详细信息；）
## 虚拟仪器
[虚拟仪器](https://baike.baidu.com/item/%E8%99%9A%E6%8B%9F%E4%BB%AA%E5%99%A8)[3] （virtual
 instrument）是基于[计算机](https://baike.baidu.com/item/%E8%AE%A1%E7%AE%97%E6%9C%BA)的仪器。计算机和仪器的密切结合是目前仪器发展的一个重要方向。粗略地说这种结合有两种方式，一种是将计算机装入仪器，其典型的例子就是所谓智能化的仪器。随着计算机功能的日益强大以及其体积的日趋缩小，这类仪器功能也越来越强大，目前已经出现含[嵌入式](https://baike.baidu.com/item/%E5%B5%8C%E5%85%A5%E5%BC%8F)系统的仪器。另一种方式是将仪器装入计算机。以通用的计算机硬件及[操作系统](https://baike.baidu.com/item/%E6%93%8D%E4%BD%9C%E7%B3%BB%E7%BB%9F)为依托，实现各种仪器功能。虚拟仪器主要是指这种方式。[4]
[虚拟仪器](https://baike.baidu.com/item/%E8%99%9A%E6%8B%9F%E4%BB%AA%E5%99%A8)[3] 实际上是一个按照仪器需求组织的[数据采集系统](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E9%87%87%E9%9B%86%E7%B3%BB%E7%BB%9F)。虚拟仪器[3] 的研究中涉及的基础理论主要有计算机数据采集和[数字信号处理](https://baike.baidu.com/item/%E6%95%B0%E5%AD%97%E4%BF%A1%E5%8F%B7%E5%A4%84%E7%90%86)。目前在这一领域内，使用较为广泛的计算机语言是[美国](https://baike.baidu.com/item/%E7%BE%8E%E5%9B%BD)NI
 公司的 LabVIEW[2] 。
虚拟仪器的起源可以追溯到20世纪70年代，那时计算机测控系统在国防、航天等领域已经有了相当的发展。PC机出现以后，仪器级的计算机化成为可能，甚至在 Microsof t公司的 Windows 诞生之前，NI公司已经在Macintosh计算机上推出了LabVIEW2.0 以前的版本。对虚拟仪器和 LabVIEW [2] 长期、系统、有效的研究开发使得该公司成为业界公认的权威。目前LabVIEW
 的最新版本为 LabVIEW2014，LabVIEW 2009 为多线程功能添加了更多特性，这种特性在 1998 年的版本 5 中被初次引入。使用 LabVIEW 软件，用户可以借助于它提供的[软件环境](https://baike.baidu.com/item/%E8%BD%AF%E4%BB%B6%E7%8E%AF%E5%A2%83)，该环境由于其数据流编程特性、LabVIEW Real-Time
 工具对[嵌入式](https://baike.baidu.com/item/%E5%B5%8C%E5%85%A5%E5%BC%8F)平台开发的多核支持，以及自上而下的为多核而设计的软件层次，是进行并行编程的首选。
普通的 PC 有一些不可避免的弱点。用它构建的虚拟仪器[3] 或计算机测试系统性能不可能太高。目前作为计算机化仪器的一个重要发展方向是制定了VXI
 标准，这是一种插卡式的仪器。每一种仪器是一个插卡，为了保证仪器的性能，又采用了较多的硬件，但这些卡式仪器本身都没有面板，其面板仍然用虚拟的方式在计算机屏幕上出现。这些卡插入标准的 VXI 机箱，再与计算机相连，就组成了一个测试系统。VXI 仪器价格昂贵，目前又推出了一种较为便宜的 PXI 标准仪器。
## 应用领域
LABVIEW[1] 有很多优点，尤其是在某些特殊领域其特点尤其突出。
测试测量：LABVIEW[5] 最初就是为测试测量而设计的，因而测试测量也就是现在LABVIEW最广泛的应用领域。经过多年的发展，LABVIEW在测试测量领域获得了广泛的承认。至今，大多数主流的测试仪器、[数据采集](https://baike.baidu.com/item/%E6%95%B0%E6%8D%AE%E9%87%87%E9%9B%86)设备都拥有专门的LabVIEW驱动程序，使用LabVIEW可以非常便捷的控制这些硬件设备。同时，用户也可以十分方便地找到各种适用于测试测量领域的LabVIEW工具包。这些工具包几乎覆盖了用户所需的所有功能，用户在这些工具包的基础上再开发程序就容易多了。有时甚至于只需简单地调用几个工具包中的函数，就可以组成一个完整的测试测量应用程序。
控制：控制与测试是两个相关度非常高的领域，从测试领域起家的LabVIEW自然而然地首先拓展至控制领域。LabVIEW拥有专门用于控制领域的[模块](https://baike.baidu.com/item/%E6%A8%A1%E5%9D%97)----LabVIEWDSC。除此之外，工业控制领域常用的设备、数据线等通常也都带有相应的LabVIEW驱动程序。使用LabVIEW可以非常方便的编制各种控制程序。
仿真：LabVIEW包含了多种多样的数学运算[函数](https://baike.baidu.com/item/%E5%87%BD%E6%95%B0)，特别适合进行模拟、[仿真](https://baike.baidu.com/item/%E4%BB%BF%E7%9C%9F)、原型设计等工作。在设计机电设备之前，可以先在计算机上用LabVIEW搭建[仿真](https://baike.baidu.com/item/%E4%BB%BF%E7%9C%9F)原型，验证设计的合理性，找到潜在的问题。在高等教育领域，有时如果使用LabVIEW进行软件模拟，就可以达到同样的效果，使学生不致失去实践的机会。
儿童教育：由于图形外观漂亮且容易吸引儿童的注意力，同时图形比文本更容易被儿童接受和理解，所以LabVIEW非常受少年儿童的欢迎。对于没有任何计算机知识的儿童而言，可以把LabVIEW理解成是一种特殊的“积木”：把不同的原件搭在一起，就可以实现自己所需的功能。著名的可编程玩具“[乐高积木](https://baike.baidu.com/item/%E4%B9%90%E9%AB%98%E7%A7%AF%E6%9C%A8)”使用的就是LabVIEW编程语言。儿童经过短暂的指导就可以利用[乐高积木](https://baike.baidu.com/item/%E4%B9%90%E9%AB%98%E7%A7%AF%E6%9C%A8)提供的积木搭建成各种车辆模型、机器人等，再使用LabVIEW编写控制其运动和行为的程序。除了应用于玩具，LabVIEW还有专门用于中小学生教学使用的版本。
快速开发：根据笔者参与的一些项目统计，完成一个功能类似的大型应用软件，熟练的LabVIEW程序员所需的开发时间，大概只是熟练的C程序员所需时间的1/5左右。所以，如果项目开发时间紧张，应该优先考虑使用LabVIEW，以缩短开发时间。
跨平台：如果同一个程序需要运行于多个硬件设备之上，也可以优先考虑使用LabVIEW。LabVIEW具有良好的平台一致性。LabVIEW的代码不需任何修改就可以运行在常见的三大台式机操作系统上：Windows、Mac OS 及 Linux。除此之外，LabVIEW还支持各种实时操作系统和嵌入式设备，比如常见的[PDA](https://baike.baidu.com/item/PDA)、[FPGA](https://baike.baidu.com/item/FPGA)以及运行[VxWorks](https://baike.baidu.com/item/VxWorks)和PharLap系统的RT设备。
## 开发系统
每位LabVIEW用户的起点都是开发系统[1] ，这一开发系统也是图形化编程的基础环境。
 以下软件包的功能相辅相成，可帮助用户满足当前和未来的需求。
LabVIEW包括基本版，完整版和专业版。
LabVIEW基本版：
- 
图形化用户界面开发
- 
数据采集
- 
仪器控制
- 
报告生成和文件I/O
LabVIEW完整版：
- 
700多个数学/分析函数
- 
外部代码集成 (.dll)
- 
互联网连接
- 
高级用户界面开发
LabVIEW专业版：
- 
应用发布 (creat.exe)
- 
开发管理
- 
源代码控制
- 
网络通信
## 附件模块
由于LabVIEW的模块化[6] 特性，可通过添加NI和第三方的附加软件来满足顾客的项目需求。
 下方列出的各种LabVIEW函数和高级工具可用于帮助用户开发特定应用并将其部署至终端。
- 
集成部署硬件：结合可编程自动化控制器(PAC)，设计、原型与部署硬件终端，如实时系统和基于现场可编程门阵列(FPGA)的系统。[5]
例如：LabVIEW Real-Time模块，LabVIEW FPGA模块，用于ARM微控制器的NI LabVIEW嵌入式模块，NI LabVIEW Mobile模块，NI LabVIEW触摸屏模块，NI LabVIEW无线传感器网络模块，LabVIEW C代码生成器，NI实时管理程序
- 
信号处理、分析和连接：添加用于声音和振动测量、机器视觉、RF通信、瞬时与短时信号分析等的专用图像和信号处理函数。[6]
例如：LabVIEW视觉应用开发模块，声音和振动测量套件，声音与振动工具包，NI LabVIEW因特网工具包，NI LabVIEW高级信号处理工具包，NI LabVIEW自适应滤波器工具包，NI LabVIEW数字滤波器设计工具包，NI LabVIEW MathScript RT模块，频谱测量工具包，NI LabVIEW调制工具包，NI LabVIEW机器人模块，LabVIEW生物医学工具包，LabVIEW电能套件，ECU测量和校准工具包，用于LabVIEW的GPS仿真工具包，用于固定WiMAX的测量套件，NI
 WLAN测量套件，汽车诊断指令集，LabVIEW GPU分析工具。
- 
控制与仿真：使用高级控制算法、动态仿真与运动控制软件，设计、仿真并执行控制系统。[6]
例如：NI LabVIEW PID和模糊逻辑工具包，NI LabVIEW控制设计与仿真模块，NI LabVIEW系统辨识工具包，NI LabVIEW仿真接口工具包，LabVIEW NI SoftMotion模块。
- 
数据管理、记录与报表生成：快速记录、管理、搜索采集的数据并将其导出至第三方软件工具（如：Microsoft Office和工业标准的数据库）。[6]
例如：NI LabVIEW数据记录与监控模块，NI LabVIEW Microsoft Office报表生成工具包，NI LabVIEW数据库连接工具包，NI LabVIEW DataFinder工具包，NI LabVIEW SignalExpress。
- 
开发工具和验证：用户可利用代码分析仪和单元测试架构，评估图形化代码质量并根据开发需求实现回归测试和验证等操作的自动化。[6]
例如：NI LabVIEW VI分析仪工具包，NI LabVIEW状态图模块，NI LabVIEW桌面执行跟踪工具包，NI需求管理软件，NI Real-Time执行跟踪工具包，NI LabVIEW单元测试架构工具包
- 
应用发布：通过创建可执行程序、安装程序和DLL，将LabVIEW[6] 应用程序发布给用户；或者通过网络或因特网共享用户界面。
例如：NI LabVIEW应用程序生成器 (Windows版)
## 版本信息
简单回顾一下LabVIEW最近的发展历史（也仅限于我能够收集到的版本），从这里也可以间接的体会到LabVIEW的发展速度有多快。从LabVIEW的软件版本来看（我能收集到的），应该有LabVIEW 5系列、LabVIEW 6系列、LabVIEW 7系列和LabVIEW 8系列。发布年份可能有误，以NI为准。
LabVIEW 1.0 发布于：1986年，运行在苹果公司的Macintosh平台上。
LabVIEW 2.0 发布于：1988年，1990年虚拟仪器面板和结构化数据流获两项美国专利。
LabVIEW 3.0 发布于：1994年
LabVIEW 4.0 发布于：1996年
LabVIEW 5.0 发布于：1998年
LabVIEW 5.1.1 发布于：2000年3月
LabVIEW 6.02 发布于：2001年2月
LabVIEW 6.1 发布于：2002年1月
LabVIEW 7.0 发布于：2003年5月
LabVIEW 7.1 发布于：2004年4月
LabVIEW 7.1.1 发布于：2004年11月
LabVIEW 8.0 发布于：2005年10月
LabVIEW 8.0.1 发布于：2006年2月
LabVIEW 8.20 发布于：2006年8月
LabVIEW 8.2.1 发布于：2007年3月
LabVIEW 8.2.1f4 发布于：2007年9月
LabVIEW 8.5 发布于：2007年8月
LabVIEW8.5.1 发布于：2008年4月
LabVIEW8.6 发布于：2008年8月
LabVIEW8.6.1 发布于：2009年2月
LabVIEW 2010 发布于：2010年8月
LabVIEW 2011 发布于：2011年8月
LabVIEW 2012 发布于：2012年8月
LabVIEW 2013 发布于： 2013年8月
LabVIEW 2014 发布于： 2014年8月
从NI的LabVIEW版本号，可以看出：
1、 系列号：5、6、7、8表示新的系列，软件结构或功能可能有重大改进（付费升级）
2、 版本号：5.x、6.x、7.x、8.x表示软件有新的内容或比较大的改进（付费升级）
3、 版本号：5.x.x、6.x.x、7.x.x、8.x.x表示软件较上个版本进行了修补（免费升级）
## 参考书籍
![](https://gss3.bdstatic.com/-Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=8a93adf63adbb6fd215be2243924aba6/a8773912b31bb0512350aa25357adab44aede043.jpg)
书 名：LabVIEW实用工具详解[7]
作　者：[陈树学](https://baike.baidu.com/item/%E9%99%88%E6%A0%91%E5%AD%A6) 编著
出 版 社：[电子工业出版社](https://baike.baidu.com/item/%E7%94%B5%E5%AD%90%E5%B7%A5%E4%B8%9A%E5%87%BA%E7%89%88%E7%A4%BE)
出版时间：2014-9-1
版　次：1
页　数：466
字　数：806000
印刷时间：2014-9-1
本： 16开
纸　张：胶版纸印
印 次：1
包　装：平装
书 名：LabVIEW宝典
![LabVIEW宝典](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=88dc8b594a90f60300b09b450913b370/503d269759ee3d6d57a580a343166d224f4ade72.jpg)LabVIEW宝典
作　者：[陈树学](https://baike.baidu.com/item/%E9%99%88%E6%A0%91%E5%AD%A6)，[刘萱](https://baike.baidu.com/item/%E5%88%98%E8%90%B1)
出 版 社：[电子工业出版社](https://baike.baidu.com/item/%E7%94%B5%E5%AD%90%E5%B7%A5%E4%B8%9A%E5%87%BA%E7%89%88%E7%A4%BE)
出版时间：2011-3-1
版　次：1
页　数：603
字　数：973000
印刷时间：2011-3-1开
本： 16开
纸　张：胶版纸印
印 次：1
包　装：平装
书名：我和LabVIEW--一个NI工程师的十年编程经验
![](https://gss2.bdstatic.com/-fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=f8ec00b50c33874498c5287e610dd937/adaf2edda3cc7cd983d86a3c3901213fb90e9159.jpg)
作　者：阮奇桢 著
丛 书 名：博客藏经阁丛书
出 版 社：[北京航空航天大学出版社](https://baike.baidu.com/item/%E5%8C%97%E4%BA%AC%E8%88%AA%E7%A9%BA%E8%88%AA%E5%A4%A9%E5%A4%A7%E5%AD%A6%E5%87%BA%E7%89%88%E7%A4%BE)
出版时间：2009-09-01
版　次：1
页　数：412
装　帧：平装
开　本：16开
[](https://baike.baidu.com/pic/LabVIEW/4165214/0/c2bce203986178c8d43f7cc1?fr=lemma)
![词条图片](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D235/sign=7ce209afce1b9d168ec79d62c6deb4eb/960a304e251f95cadbec09c6c9177f3e670952a0.jpg)
词条图片(5)
- 
参考资料
- 1.[](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#ref_%5B1%5D_230451)[LabVIEW系统设计软件](https://baike.baidu.com/redirect/2025kyiJ6-4uK2v9rbr9059gKl8nyiBKet9A1JZoaGkEChcvIrj0PgP_xfzc4kj5CafqKAnTgxQTfkg)．NI中国[引用日期2014-02-18]
- 2.[](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#ref_%5B2%5D_230451)[Knowledge
 ME - LabVIEW](https://baike.baidu.com/redirect/48fd3--vYokmU2FKiDd3eWEfY1g-NyCXZdOdeKvY5MoXBkNHVPkAFoasY-7hwXA_KVpfKmSmbq2chOqCcnkXqw)
- 3.[](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#ref_%5B3%5D_230451)[Knowledge
 ME - Virtual Instrument](https://baike.baidu.com/redirect/0853aFBqA6x4VMV4xFSrH3Ybj1riEhHKb27y_fokD1PKcj7BPaZM8-637dbSaSfFbGqBD-sm6yPOyEI)
- 4.[](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#ref_%5B4%5D_230451)[实验室虚拟仪器工程平台
 LabVIEW](https://baike.baidu.com/redirect/ef05RA32HRNj4by1CGnxGdwgNpRlHxY6DEgqaLROXqF0tP3iO1Y-Y_vqG3mJ4NULyHScG5a3q4LA-yEJTw)．开源社区网[引用日期2012-09-15]
- 5.[](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#ref_%5B5%5D_230451)[NI
 LabVIEW应用领域](https://baike.baidu.com/redirect/4c6cmB9NhG5m_3Ey67eNtINmlxhzcbdhwCNZIv226bYl6Jkvtqz7_ybYTUQ3erw-Cq-gI6TdTYXJnyFA6tC4MYi3mhqbYgoX)．NI[引用日期2014-02-18]
- 6.[](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#ref_%5B6%5D_230451)[NI
 LabVIEW产品](https://baike.baidu.com/redirect/bcc1a02XqUdZK5TtllNbM8kOQm-0U2yNsWpFHayDdi9dUPJT4fTeHd_E6QMGHZcHSAwMmhHLKDL5mgSjggs5HyPhfn8)．NI[引用日期2014-02-18]
- 7.[](https://baike.baidu.com/item/LabVIEW/4165214?fr=aladdin#ref_%5B7%5D_230451)陈树学．labVIEW实用工具详解：电子工业出版社，2014-09-01
词条标签：
[中国通信学会](https://baike.baidu.com/wikitag/taglist?tagId=68041)， 书籍

