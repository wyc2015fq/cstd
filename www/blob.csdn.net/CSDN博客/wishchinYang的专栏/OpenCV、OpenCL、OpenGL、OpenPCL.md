# OpenCV、OpenCL、OpenGL、OpenPCL - wishchinYang的专栏 - CSDN博客
2013年11月11日 14:46:30[wishchin](https://me.csdn.net/wishchin)阅读数：3395
        对于几个开源库的总结，作为标记，以前看过，现在开始重视起来！更详细资料请移步 开源中国社区！
涉及：OpenCV，OpenCL，OpenGL，OpenPCL
截止到目前：
        OpenGL的最新版本为4.4，NVIDIA显卡系列已提供了基于4.4版本的驱动；
        OpenGL ES的最新版本为3.0，高通处理器图形芯片已经可以支持；
        OpenCV的最新发行版本为2.4.6,  另外3,0版本在14年2月发布，有兴趣可以查看OpenCV官方里程碑图；
        OpenPCL的最新发行版为1.7.1的源代码，不过编译成功的人是少数，总之我是一个失败的案例！
        OpenCL的最新版本为2.0，OpenCL全称**Open Computing Language**，是**第一个面向异构系统通用目的并行编程的开放式、免费标准**，也是一个统一的编程环境，便于软件开发人员为**高性能计算服务器、桌面计算系统、手持设备**编写高效轻便的代码，而且**广泛适用于多核心处理器(CPU)、图形处理器(GPU)、Cell类型架构以及数字信号处理器(DSP)等其他并行处理器**，在游戏、娱乐、科研、医疗等各种领域都有广阔的发展前景。
一、[OpenCL框架组成 （转自于华章IT官方博客）](http://blog.csdn.net/hzbooks/article/details/8206571)
原文链接：[http://blog.csdn.net/hzbooks/article/details/8206571](http://blog.csdn.net/hzbooks/article/details/8206571)
下面是Copy：
OpenCL框架组成
本文主要讨论OpenCL框架，其组成可划分为以下三个部分：
- **OpenCL平台API**：平台API定义了宿主机程序发现OpenCL设备所用的函数以及这些函数的功能，另外还定义了为OpenCL应用创建上下文的函数。
- **OpenCL运行时API**：这个API管理上下文来创建命令队列以及运行时发生的其他操作。例如，将命令提交到命令队列的函数就来自OpenCL运行时API。
- **OpenCL编程语言**：这是用来编写内核代码的编程语言。它基于ISO C99标准的一个扩展子集，因此通常称为OpenCL C编程语言。
在后面的小节中，我们将为以上各个部分提供一个高层的概述。详细内容留待本书后面介绍，不过使用OpenCL时先有一个高层认识会很有帮助。
1.　平台API
平台（platform）一词在OpenCL中有非常特定的含义。它表示宿主机、OpenCL设备和OpenCL框架的组合。一个异构计算机上可以同时存在多个OpenCL平台。例如，CPU开发商和GPU开发商可以在一个系统上分别定义自己的OpenCL框架。程序员需要一种方法查询系统中可用的OpenCL框架。他们需要查找哪些OpenCL设备可用，这些OpenCL设备有什么特性。另外，他们还需要控制这些框架和设备的哪个子集构成给定OpenCL应用中使用的平台。
这些功能由OpenCL平台API中的函数解决。在后面的章节中将会看到，我们重点讨论OpenCL程序员为宿主机程序编写代码时，每个OpenCL应用程序都以类似的方式打开，调用平台API的函数为OpenCL计算定义上下文。
2.　运行时API
平台API中的函数为OpenCL应用定义上下文。运行时API则强调使用这个上下文满足应用需求的函数。这是一个庞大而且确实相当复杂的函数集。
运行时API的第一个任务是建立命令队列。可以将命令队列关联到一个设备，不过一个上下文中可以同时有多个活动的命令队列。
有了命令队列，就可以使用运行时API来定义内存对象和管理内存对象所需要的所有其他对象（如对于图像对象还需要采样器对象）。管理内存对象是一个很重要的任务。为了支持垃圾回收，OpenCL会跟踪多少个内核实例使用这些对象（也就是说，持有一个内存对象），以及内核何时用完一个内存对象（即释放一个内存对象）。
运行时API管理的另一个任务是创建构建动态库所用的程序对象，内核就由这些动态库定义。程序对象、编译程序对象的编译器以及内核定义都在运行时层处理。
最后，与命令队列交互的命令都由运行时层的函数发出。管理数据共享和对内核执行施加约束的同步点也由运行时API处理。
可以看到，运行时API函数完成了宿主机程序的大部分具体工作。要想一次掌握运行时API，从第一个函数开始学完所有函数，这是很有压力的。我们发现，更好的做法是使用一种实用的方法。掌握真正要使用的函数。过一段时间，你就会把它们全面覆盖到，并完全掌握，不过要根据OpenCL应用的具体需要来学习这些函数。
3.　内核编程语言
宿主机程序非常重要，不过完成OpenCL中实际工作的是内核。有些OpenCL实现允许你与非OpenCL编写的原生内核交互，不过，大多数情况下都需要编写内核来完成应用中的特定工作。
OpenCL中的内核编程语言称为OpenCL C编程语言，因为我们希望过一段时间后可以定义符合规范的其他语言。它由ISO C99语言派生而来。
在OpenCL中，要对支持可移植性特别当心。这要求我们标准化不同类的OpenCL设备之间的最小公共子集。由于C99中有些特性只有CPU能够支持，所以在定义OpenCL C编程语言时，我们去掉了C99的一些语言特性。删除的主要语言特性包括：
- 递归函数。
- 函数指针。
- 位域。
另外，我们不支持完整的标准库集合。OpenCL编程语言中不支持的标准头文件很多，不过程序员最有可能遗漏的是stdio.h和stdlib.h。再次说明，一旦不再将通用处理器作为OpenCL设备，这些库将很难获得支持。
由于需要保持OpenCL核心抽象的真实性，所以会带来另外一些限制。例如，OpenCL定义了一组内存地址空间。联合（union）或结构（structure）不能混合类型。另外，OpenCL还定义了一些不透明的类型，例如，支持图像的内存对象。OpenCL C编程语言除了允许将这些类型作为参数传递给函数外，不允许对它们做任何其他处理。
我们将OpenCL C编程语言限制为只满足用于OpenCL的关键OpenCL设备的需求。出于同样的原因，促使我们扩展语言以及以下方面：
- 矢量类型和这些类型实例上的操作。
- 地址空间限定符，支持OpenCL对多个地址空间的控制。
- 一组丰富的内置函数，支持OpenCL应用中通常需要的功能。
- 全局和局部内存中处理无符号整数和单精度标量变量的原子函数。
大多数编程语言忽略浮点算术系统的特定细节。它们只是从硬件导入算术系统，从而完全避开这个问题。由于所有主流CPU都支持IEEE 754和IEEE 854标准，所以这个策略是可行的。实际上，通过集中研究这些浮点标准，硬件开发商在为语言开发商解决浮点定义的有关问题。
不过，在异构世界中，如果脱离CPU，那么对浮点算术运算的支持会有更多的选择。过去通过与硬件开发商的紧密合作，我们希望大力推动他们完善对IEEE浮点标准的支持。与此同时，我们不希望对这些开发商过于苛刻，所以赋予他们一定的灵活性可以避开IEEE标准中一些不常使用但实现很困难的特性。后面会详细讨论有关细节，不过从高层可以总结为OpenCL需要以下特性：
- 对IEEE 754格式的全面支持。双精度是可选的，不过如果提供双精度，也必须符合IEEE 754格式。
- 支持默认的IEEE 754舍入模式，即“舍入为最近整数”。其他舍入模式尽管值得推荐（因为数值分析学者需要这些模式），但它们是可选的。
- 尽管IEEE规范要求动态改变舍入模式，但OpenCL中的舍入模式是静态设置的。
- 必须支持特殊值INF（无穷大）和NaN（非数字），不过不要求提示NaN（通常反映并发系统中的问题）。
- 非规格化数（小于1的数乘以所支持的最大负指数）可以化简为0。如果你还不了解为什么这很重要，不用担心，很多人都与你一样。这也是数值分析学者很依赖但很少有程序员了解的另一个特性。
关于浮点数异常还有很多其他规则，不过它们对大多数人来说都过于复杂、过于深奥，没有必要在这里多做说明。关键是要了解我们已努力满足IEEE 754的大多数内容，同时省略了很少使用而且（在配有矢量单元的异构平台上）难以支持的一些特性。
OpenCL规范并不仅限于IEEE标准。在OpenCL规范中，还有一些表格详尽地定义了数学函数中允许的相对误差。要想了解所有这些错误确实难度很大，不过对于编写详细数值代码的程序员来说，定义这些错误是至关重要的。
综合以上浮点数需求、限制和扩展，就得到了一个非常适合当前异构平台的编程语言，随着这些平台中使用的处理器继续发展，并变得更为通用，OpenCL C编程语言也会随之发展。
4.　小结
我们已经介绍了核心OpenCL框架的基本组成。要单独地了解这些组成部分（我们介绍时就主要采用了这种方式）很重要。为了把这些单独的部分汇集起来，形成OpenCL的一个全景图，下面对应用在OpenCL框架中的基本工作流做个小结，如图1-9所示。
![](https://img-my.csdn.net/uploads/201211/21/1353463744_7356.jpg)
图1-9　这个模块图总结了OpenCL的组成以及一个OpenCL应用执行期间在宿主机上发生的动作
首先是一个定义上下文的宿主机程序。图1-9中的上下文包含两个OpenCL设备、一个CPU和一个GPU。接下来定义了命令队列。这里有两个队列，一个是面向GPU的有序命令队列，另一个是面向CPU的乱序命令队列。然后宿主机程序定义一个程序对象，这个程序对象编译后将为两个OpenCL设备（CPU和GPU）生成内核。接下来宿主机程序定义程序所需的内存对象，并把它们映射到内核的参数。最后，宿主机程序将命令放入命令队列来执行这些内核。
-----------------------------
![](https://img-my.csdn.net/uploads/201211/21/1353464816_8549.jpg)本文节选自《OpenCL编程指南》
原书名：OpenCL Programming Guide
作者：Aaftab Munshi / Benedict R. Gaster / Timothy G. Mattson / James Fung / Dan Ginsburg
本书由五位OpenCL核心设计人员携手打造，不仅详细而完整地解读了枯燥的OpenCL规范，而且通过一些重要的案例展示了如何利用OpenCL实现大量并行算法、编写复杂的并行程序、对OpenCL进行性能优化，以及探查硬件和针对硬件进行调整，是系统学习OpenCL的经典参考书。
阅读本书PDF样章，请访问[http://download.csdn.net/detail/hzbooks/4791805](http://download.csdn.net/detail/hzbooks/4791805)
豆瓣收藏本书，请访问[http://book.douban.com/subject/20385439/](http://book.douban.com/subject/20385439/)
查看评论
- 
2楼 [abcdef_abcd](http://blog.csdn.net/abcdef_abcd)2012-12-20 16:27发表[[回复]](http://blog.csdn.net/hzbooks/article/details/8206571#reply)
![](http://avatar.csdn.net/A/4/F/3_abcdef_abcd.jpg)不错不错，终于见到一篇文章可以对opencl在整体架构的高度上进行比较通俗的介绍- 
1楼 [choovin](http://blog.csdn.net/choovin)2012-11-25 22:38发表[[回复]](http://blog.csdn.net/hzbooks/article/details/8206571#reply)
![](http://avatar.csdn.net/B/F/6/3_choovin.jpg)本书由五位OpenCL核心设计人员携手打造，不仅详细而完整地解读了枯燥的OpenCL规范，而且通过一些重要的案例展示了如何利用OpenCL实现大量并行算法、编写复杂的并行程序、对OpenCL进行性能优化，以及探查硬件和针对硬件进行调整，是系统学习OpenCL的经典参考书。权威啊
二、OpenGL简介:
原文链接：[http://blog.csdn.net/barnett_zhubo/article/details/6460789](http://blog.csdn.net/barnett_zhubo/article/details/6460789)
OpenGL(Open Graphics Library)定义了一个跨编程语言、跨平台的编程接口的规格，是一个性能卓越的三维图形标准。OpenGL是一个专业的图形程序接口，是一个功能强大、调用方便的底层图形库。OpenGL的前身是SGI公司为其图形工作站开发的IRIS GL。IRIS GL是一个工业标准的3D图形软件接口，SGI公司便在IRIS GL的基础上开发了OpenGL。
OpenGL与OpenGL ES的区别
OpenGL ES是专为内嵌和移动设备设计的一个2D/3D轻量图形库，它是基于OpenGL API设计的，是OpenGL 三维图形API的子集，针对手机、PDA和游戏主机等嵌入式设备而设计。该API由Khronos集团定义、推广，Khronos是一个图形软硬件行业协会。
OpenGL ES的版本
OpenGL ES现在主要有两个版本：OpenGL 1.x 针对固定管线硬件，OpenGL 2.x针对可编程管线硬件。
OpenGL ES 是从 OpenGL 裁剪定制而来的，去除了 glBegin/glEnd，四边形（GL_QUADS）、多边形（GL_POLYGONS）等复杂图元等许多非绝对必要的特性。经过多年发展，现在主要有两个版本，OpenGL ES 1.x 针对固定管线硬件的，OpenGL ES 2.x 针对可编程管线硬件。OpenGL ES 1.0 是以 OpenGL 1.3 规范为基础的，OpenGL ES 1.1 是以 OpenGL 1.5 规范为基础的，它们分别又支持 common 和 common lite
 两种profile。lite profile只支持定点定点实数，而common profile既支持定点数又支持浮点数。 OpenGL ES 2.0 则是参照 OpenGL 2.0 规范定义的，common profile发布于2005-8，引入了对可编程管线的支持。
下面是我们列举的一些被Opengl ES裁剪掉的Opengl功能,因此大家在使用api时需要注意,不能使用以下这些api：
1. glBegin/glEnd 
2. glArrayElement 
3. 显示列表 
4. 求值器 
5. 索引色模式 
6. 自定义裁剪平面 
7. glRect 
8. 图像处理(这个一般显卡也没有，FireGL/Quadro显卡有) 
9. 反馈缓冲 
10. 选择缓冲 
11. 累积缓冲 
12. 边界标志 
13. glPolygonMode 
14. GL_QUADS、GL_QUAD_STRIP、GL_POLYGON
15. glPushAttrib、PopAttrib、glPushClientAttrib、glPopClientAttrib 
16. TEXTURE_1D、TEXTURE_3D、TEXTURE_RECT、TEXTURE_CUBE_MAP 
17. GL_COMBINE 
18. 自动纹理坐标生成 
19. 纹理边界 
20. GL_CLAMP、GL_CLAMP_TO_BORDER 
21. 消失纹理代表 
22. 纹理LOD限定 
23. 纹理偏好限定 
24. 纹理自动压缩、解压缩 
25. glDrawPixels、glPixelTransfer、glPixelZoom 
26. glReadBuffer、glDrawBuffer、glCopyPixels
由于篇幅关系,这里我们列举的可能不是全部,更多详细内容大家可以参考[www.khronos.org/opengles/](http://www.khronos.org/opengles/)。
三、[Android中的OpenGL ES简介](http://blog.csdn.net/kaitiren/article/details/8537040)
原文链接：[http://blog.csdn.net/jusang486/article/details/8978112](http://blog.csdn.net/jusang486/article/details/8978112)
OpenGL ES软件层次栈，本章介绍了Android系统中OpenGL ES的调用层次栈，详细分析了库之间如何通过钩子（hook）实现API调用关联，让我们明白各个库之间的依赖关系。本节为Android中的OpenGL ES简介。
Android中的OpenGL ES简介
Android支持使用OpenGL（Open Graphics Library）API进行2D和3D图形的绘制 ，尤其使用OpenGL ES（Embedded System） API。OpenGL是一个跨平台的图形API规范，它为3D图形处理硬件定义了一套标准的软件接口。OpenGL ES是在OpenGL API的基础上专为嵌入式设备选取的一套API规范。自早期的版本Android 1.0就开始支持OpenGL ES 1.0和1.1规范，自Android
 2.2版本开始支持OpenGL ES 2.0规范。Android提供的OpenGL ES API类似于J2ME JSR239标准，但不同。
我们既可通过Android Framework中提供的API，也可以通过Native层（Native Development Kit ，NDK）的API去使用OpenGL。开发者可以使用Framework中两个基本的类GLSurfaceView和GLSurfaceView。Renderer在Android应用程序中进行图形绘制操作，也就是在Renderer中使用OpenGL API在GLSurfaceView上进行绘制，具体如何使用它们，详见官方文档
 、 。
在使用GLSurfaceView和GLSurfaceView.Renderer进行绘制时，可以使用下面这些OpenGL ES API包。
（1）包javax.microedition.khronos.opengles提供OpenGL ES 1.0/1.1标准实现，可以使用的API类包括：GL10、 GL10Ext、 GL11、 GL11Ext和GL11ExtensionPack。
（2）包android.opengl，提供了一套静态函数接口，包括OpenGL ES 1.0/1.1和2.0，其性能优于包javax.microedition.khronos.opengles。支持OpenGL ES 1.0/1.1的类包括：GLES10、 GLES10Ext、 GLES11和GLES10Ext；支持OpenGL ES 2.0的API类是：android.opengl.GLES20 （自Android 2.2开始）。
由于Android支持OpenGL ES 1.0/1.1、2.0，且由于二者有显著差异。那么该选择哪个版本呢？在本节参考的Google文档里的"Choosing an OpenGL API Version"部分列出了四项因素，介绍如下。
（1）性能 ：通常，OpenGL ES 2.0能比ES 1.0/1.1提供较快的性能。但是，这最终依赖于Android设备，不同的设备平台OpenGL的实现不同。
（2）设备兼容性：开发者开发的应用程序需要考虑Android设备类型，不同的Android版本对OpenGL版本支持不同，如OpenGL ES 2.0自Android 2.2才开始支持。
（3）编码便利性：OpenGL ES 1.0/1.1 API编程较为方便，2.0版本相对复杂些。
（4）图形控制：通过使用shaders，OpenGL ES 2.0对图形绘制能提供较多的控制，可以创建更好的效果，在1.0/1.1版本上则很难达到。
