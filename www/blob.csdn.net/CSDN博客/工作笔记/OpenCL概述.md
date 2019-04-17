# OpenCL概述 - 工作笔记 - CSDN博客





2015年06月26日 14:02:16[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5704








**OpenCL**是一个为异构平台编写程序的框架，此异构平台可由[CPU](http://baike.baidu.com/view/2089.htm)，[GPU](http://baike.baidu.com/view/1196.htm)或其他类型的处理器组成。OpenCL由一门用于编写kernels
 （在OpenCL设备上运行的函数）的语言（基于[C99](http://baike.baidu.com/view/1967819.htm)）和一组用于定义并控制平台的API组成。OpenCL提供了基于任务分割和[数据分割](http://baike.baidu.com/view/4466818.htm)的[并行计算](http://baike.baidu.com/view/1666.htm)机制。

OpenCL类似于另外两个开放的工业标准[OpenGL](http://baike.baidu.com/view/9222.htm)和[OpenAL](http://baike.baidu.com/view/1355367.htm)，这两个标准分别用于三维图形和计算机音频方面。OpenCL扩展了GPU用于图形生成之外的能力。OpenCL由非盈利性技术组织[Khronos
 Group](http://baike.baidu.com/view/3507329.htm)掌管。

OpenCL最初[苹果公司](http://baike.baidu.com/view/15181.htm)开发，拥有其商标权，并在与[AMD](http://baike.baidu.com/view/810.htm)，IBM，[英特尔](http://baike.baidu.com/view/15281.htm)和[nVIDIA](http://baike.baidu.com/view/16412.htm)技术团队的合作之下初步完善。随后，苹果将这一草案提交至[Khronos
 Group](http://baike.baidu.com/view/3507329.htm)。

2008年6月的WWDC大会上，苹果提出了**OpenCL规范**，旨在提供一个**通用的开放API**，在此基础上开发GPU通用计算软件。随后，Khronos Group宣布成立GPU通用计算开放行业标准工作组，以苹果的提案为基础创立OpenCL行业规范。5个月后的2008年11月18日，该工作组完成了OpenCL 1.0规范的技术细节。2010年6月14日，OpenCL 1.1 发布。2011年11月15日，OpenCL
 1.2 发布。2013年11月19日，OpenCL 2.0发布。

2009年6月NVIDIA首家发布了支持OpenCL 1.0通用计算规范的驱动程序，支持Windows和[Linux操作系统](http://baike.baidu.com/view/46577.htm)。

2009年8月初AMD首次发布了可支持IA处理器(x86和amd64/x64)的OpenCL SDK——ATI Stream SDK v2.0Beta，立即交由业界标准组织KHRONOS进行审核。目前，该SDK更名为AMD APP SDK。

2012年2月，intel发布了The Intel&reg; SDK for OpenCL* Applications 2012，支持OpenCL 1.1基于带HD4000/2500的[显示核心](http://baike.baidu.com/view/1613285.htm)的第三代[酷睿](http://baike.baidu.com/view/63431.htm)CPU（i3,i5,i7).和GPU。

2013年6月，intel发布了第四代酷睿CPU haswell 其内置的HD4600/4400/4200 Iris（锐矩）5000/5100/pro 5200（自带eDRAM缓存）支持OpenCL 1.2（未来可能升级到OpenCL 2.0）

NVIDIA显卡方面 Geforce 8000\9000\100\200\300\400\500\600\700\800\900均支持OpenCL 1.0-1.2

AMD显卡方面 Radeon HD 4000\5000\6000\7000\Rx 200 均支持OpenCL 1.0-1.2，除Radeon HD4000系列外，其余均会支持OpenCL 2.0

移动平台方面目前高通adreno320/330提供了Android上的OpenCL1.1支持，NVIDIA的Tegra K1也提供了OpenCL 支持。



OpenCL工作组的成员包括：3Dlabs、AMD、苹果、ARM、Codeplay、爱立信、飞思卡尔、华为、HSA基金会、Graphic[Remedy](http://baike.baidu.com/view/1773080.htm)、IBM、Imagination Technologies、Intel、诺基亚、NVIDIA、摩托罗拉、QNX、高通，三星、Seaweed、德州仪器、布里斯托尔大学、瑞典Ume大学。[1]

像Intel、NVIDIA和AMD都是这个标准的支持者，不过微软并不在其列。



目前，NVIDIA显卡对OpenCL技术支持得比较到位，所以这里仅用NVIDIA的Geforce（精视）系列显卡作解释。

中国用户可以登录英伟达中文官方网站上下载到最新的[驱动程序](http://baike.baidu.com/view/1048.htm)，只要您下载的驱动是195.62版本或更高，就可以在Geforce（精视）8系列或更高级的显卡中开启OpenCL，在安装好新版本的显卡驱动程序并重新启动后，OpenCL就自动开启了。当有需要使用CPU来完成的工作如转换视频时，GPU就会帮助CPU进行运算，以提高转换速度。但是在3D游戏中应该是不会调用OpenCL的，因为显卡有自己的[硬件加速](http://baike.baidu.com/view/1039397.htm)功能以及物理引擎，所以Geforce（精视）8系列及以上的显卡就不需要CPU辅助进行渲染了。这时候，您就可以一边玩游戏，一边进行消耗CPU的工作了。



当然同样，在NVIDIA的Quadro系列专业显卡中，同样能够使用OpenCL技术。只要您的显卡能够达到CUDA的要求，就能够正常使用OpenCL，以获得优异的CPU运算效率。

在AMD-ATI的Stream技术中（现已经改名为AMD APP并行加速技术），已经为日常使用、办公、游戏等提供物理加速。基于OpenCL标准开发，其中，ATI Radeon HD 4000-5000、AMD Radeon HD 6000系列同时支持ATI Stream和AMD APP（由于Stream基于CAL和Brook+语言开发，更适合VLIW5和VLIW4这样的SIMD架构），AMD Radeon HD7000和Radeon Rx 200系列支持AMD APP，运算效率较老架构提升十分明显。

**OpenCL 1.0**

OpenCL 1.0主要由一个[并行计算](http://baike.baidu.com/view/1666.htm)API和一种针对此类计算的编程语言组成，此外还特别定义了：

1、C99编程语言并行扩展子集;

2、适用于各种类型异构处理器的坐标数据和基于任务并行计算API;

3、基于IEEE 754标准的数字条件;

4、与OpenGL、OpenGL ES和其他图形类API高效互通。

**OpenCL 1.1**

Khronos Group2010年6月15日宣布，OpenCL通用计算标准的1.1版本已经发放，开发者可以免费下载，并依照新标准开始进行编程。

OpenCL 1.1标准向下兼容1.0版，提供了更多的新功能，并对性能进行了改善。主要新特性包括：

- 支持新数据类型，如3维矢量和新增[图像格式](http://baike.baidu.com/view/13674.htm)。

- 支持处理多Host指令以及跨设备Buffer处理。

- Buffer区域操作，包括对1D、2D、3D三角形区域的读、写和拷贝操作。

- 改进驱动和控制指令执行的事件应用。

- 增加OpenCL内建C功能。

- 通过链接OpenCL和OpenGL事件，高效共享图像和Buffer，改进与OpenGL的互操作性。

OpenCL标准由Khronos Group的OpenCL工作组制定，完全开放，任何开发者都可免费使用。OpenCL工作组成员包括（英文首字母排序）：3DLABS、动视暴雪、AMD、苹果、ARM、Broadcom、CodePlay、EA、爱立信、飞思卡尔、富士通、通用电气、Graphic[Remedy](http://baike.baidu.com/view/1773080.htm)、HI、IBM、Intel、Imagination
 Technologies、美国Los Alamos国家实验室、摩托罗拉、Movidia、诺基亚、NVIDIA、Petapath、QNX、高通、RapidMind、三星、Seaweed、S3、意法半导体、Takumi、德州仪器、东芝和Vivante。



OpenCL 2.0

Khronos Group2013年11月19日宣布了OpenCL通用计算标准的2.0版本特性，其中对共享虚拟内存的支持是一大亮点（此前NVIDIA发布了CUDA 6规范也同样支持共享虚拟内存，但目前仅限Kepler和Maxwell架构的N卡。此外，AMD的GCN架构显卡同样支持。AMD的Kaveri APU支持HSA异构计算和hUMA统一物理寻址，较虚拟共享更加先进。）[2]

1、共享虚拟内存

主机和设备内核可以直接共享复杂的、包含指针的数据结构，大大提高编程灵活性，避免冗余的数据转移。

2、动态并行

设备内核可以在无需主机交互的情况下进行内核排队，实现灵活的工作调度，避免数据转移，大大减轻主处理器的负担。

3、通用内存空间

无需指定地址空间名称即可为引数(argument)编写函数，不用再为程序里的每一个地址空间名称编写函数。

4、图像

改进图像支持，包括sRGB、3D，内核可以读写同一图像。

5、C11原子操作

新的C11原子和同步操作子集，分配在同一工作组内

6、Pipes

以FIFO格式组织数据的内存对象，可以直接读写，数据结构可简单编程、高度优化。

7、安卓可安装客户端驱动扩展

安卓系统上可将OpenCL作为共享对象进行载入






**OpenCL平台API**：平台API定义了宿主机程序发现OpenCL设备所用的函数以及这些函数的功能，另外还定义了为OpenCL应用创建上下文的函数。

**OpenCL运行时API**：这个API管理上下文来创建命令队列以及运行时发生的其他操作。例如，将命令提交到命令队列的函数就来自OpenCL运行时API。

**OpenCL编程语言**：这是用来编写内核代码的编程语言。它基于ISO C99标准的一个扩展子集，因此通常称为OpenCL C编程语言。[3]





首先是一个定义上下文的宿主机程序。如上图中中的上下文包含两个OpenCL设备、一个CPU和一个GPU。接下来定义了命令队列。这里有两个队列，一个是面向GPU的有序命令队列，另一个是面向CPU的乱序命令队列。然后宿主机程序定义一个程序对象，这个程序对象编译后将为两个OpenCL设备（CPU和GPU）生成内核。接下来宿主机程序定义程序所需的内存对象，并把它们映射到内核的参数。最后，宿主机程序将命令放入命令队列来执行这些内核。




