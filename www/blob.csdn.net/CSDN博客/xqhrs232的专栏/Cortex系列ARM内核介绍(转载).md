# Cortex系列ARM内核介绍(转载) - xqhrs232的专栏 - CSDN博客
2010年01月28日 15:37:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：955标签：[嵌入式																[compilation																[产品设计																[手机																[nested																[工作](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=nested&t=blog)](https://so.csdn.net/so/search/s.do?q=手机&t=blog)](https://so.csdn.net/so/search/s.do?q=产品设计&t=blog)](https://so.csdn.net/so/search/s.do?q=compilation&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)

**原文地址：[http://www.embedu.org/Column/Column204.htm](http://www.embedu.org/Column/Column204.htm)**
**作者：杨硕,**[华清远见嵌入式学院](http://www.embedu.org/)讲师。
众所周知，英国的ARM公司是嵌入式微处理器世界当中的佼佼者。ARM一直以来都是自己研发微处理器内核架构，然后将这些架构的知识产权授权给各个芯片厂商，精简的CPU架构，高效的处理能力以及成功的商业模式让ARM公司获得了巨大的成功，使他迅速占据了32位嵌入式微处理器的大部分市场份额，甚至现在，ARM芯片在上网本市场的也大有与INTEL的ATOM处理器一较高低的实力。
目前，随着对嵌入式系统的要求越来越高，作为其核心的嵌入式微处理器的综合性能也受到日益严峻的考验，最典型的例子就是伴随3G网络的推广，对手机的本地处理能力要求很高，现在一个高端的智能手机的处理能力几乎可以和几年前的笔记本电脑相当。为了迎合市场的需求，ARM公司也在加紧研发他们最新的ARM架构，Cortex系列就是这样的产品。在Cortex之前，ARM核都是以ARM为前缀命名的，从ARM1一直到ARM11，之后就是Cortex系列了。Cortex在英语中有大脑皮层的意思，而大脑皮层正是人脑最核心的部分，估计ARM公司如此命名正有此含义吧。
一．ARMv7架构特点
下表列出了ARM微处理器核心以及体系结构的发展历史：
![](images/Column204-1.jpg)
表一: ARM微处理器核心以及体系结构的发展历史
我们可以看到，Cortex系列属于ARMv7架构，这是ARM公司最新的指令集架构，而我们比较熟悉的三星的S3C2410芯片是ARMv4架构，ATMEL公司的AT91SAM9261芯片则是ARMv5架构。
ARMv7架构是在ARMv6架构的基础上诞生的。该架构采用了Thumb-2技术，Thumb-2技术是在ARM的Thumb代码压缩技术的基础上发展起来的，并且保持了对现存ARM解决方案的完整的代码兼容性。Thumb-2技术比纯32位代码少使用 31％的内存，减小了系统开销。同时能够提供比已有的基于Thumb技术的解决方案高出38％的性能。ARMv7架构还采用了NEON技术，将DSP和媒体处理能力提高了近4倍，并支持改良的浮点运算，满足下一代3D图形、游戏物理应用以及传统嵌入式控制应用的需求。此外，ARMv7还支持改良的运行环境，以迎合不断增加的JIT(Just
 In Time)和DAC(DynamicAdaptive Compilation)技术的使用。另外，ARMv7架构对于早期的ARM处理器软件也提供很好的兼容性。
ARMv7架构定义了三大分工明确的系列：“A”系列面向尖端的基于虚拟内存的操作系统和用户应用；“R”系列针对实时系统；“M”系列对微控制器和低成本应用提供优化。下图为v5至v7架构比较：
![](images/Column204-2.jpg)
图一：v5至v7架构比较
由于应用领域不同，基于v7架构的Cortex处理器系列所采用的技术也不相同，基于v7A的称为Cortex-A系列，基于v7R的称为Cortex-R系列，基于v7M的称为Cortex-M系列。下面一一介绍。
二．Cortex-A8
Cortex-A8第一款基于ARMv7构架的应用处理器。Cortex-A8是ARM公司有史以来性能最强劲的一款处理器，主频为600MHz到1GHz。A8可以满足各种移动设备的需求，其功耗低于300毫瓦，而性能却高达2000MIPS。
Cortex-A8也是ARM公司第一款超级标量处理器。在该处理器的设计当中，采用了新的技术以提高代码效率和性能，采用了专门针对多媒体和信号处理的NEON技术。同时，还采用了Jazelle RCT技术，可以支持JAVA程序的预编译与实时编译。
针对Cortex-A8，ARM公司专门提供了新的函数库（Artisan Advantage-CE）。新的库函数可以有效的提高异常处理的速度并降低功耗。同时，新的库函数还提供了高级内存泄漏控制机制。
Cortex-A8处理器使用了先进的分支预测技术，并且具有专用的NEON整型和浮点型流水线进行媒体和信号处理。在使用小于4 mm2的硅片及低功耗的65 nm工艺的情况下，Cortex-A8处理器的运行频率将高于600MHz(不包括NEON追踪技术和二级高速缓冲存储器)。在高性能的90 nm和65 nm工艺下，Cortex-A8处理器运行频率最高可达1 GHz，能够满足高性能消费产品设计的需要。
Cortex-A8第一次为低费用、高容量的产品带来了台式机级别的性能。当前最新的IPHONE手机和ANDROID手机里的处理器就是基于Cortex-A8内核的芯片。
Cortex-A8的系统框图如下：
![](images/Column204-3.jpg)
图二：Cortex-A8的系统框图
更详细的框图如下：
![](images/Column204-4.jpg)
图三：Cortex-A8的详细系统框图
三．Cortex-R4
Cortex-R4是ARM开发的超标量结构的ARM内核，主要面向实时控制领域，如汽车刹车控制等，这一领域要求处理器响应中断的实时性高，并且要尽可能的节约成本，而很多客户要求不需过多提高工作频率，就能提高运算性能，因此在Cortex-R4的设计过程中，工作频率的最佳点位是300MHz。再高的话，不仅需要高速内存，而且时钟树的设计也更为复杂。结果，判断合理的做法就是采用超标量结构，增加单位周期所执行的指令平均数。Cortex-R4单位工作频率的运算性能为 1.62MIPS（按Dhrystone换算）/MHz，比ARM9的约1.2MIPS/MHz大幅提高。Cortex-R4电路规模只是比ARM9略有增加，但是可以实现更接近于Cortex-A8的运算性能。
Cortex-R4系统框图如下所示：
![](images/Column204-5.jpg)
图四：Cortex-R3系统框图
四．Cortex-M3
ARM Cortex-M系列则是为那些对开发费用非常敏感同时对性能要求小断增加的嵌入式应用(如微控制器、汽车车身控制系统和各种大型家电)所设计的，主要面向单片机领域，可以说是51单品机的完美替代品。
Cortex-M3系统框图如下所示：
![](images/Column204-6.jpg)
图五：Cortex-M3系统框图
Cortex-M3的速度比ARM7快三分之一，功耗低四分之三，并且能实现更小芯片面积，利于将更多功能整合在更小的芯片尺寸中。Cortex-M3处理器结合了执行Thumb-2指令的32位哈佛微体系结构和系统外设，包括Nested Vec-tored Interrupt Controller和Arbiter总线。该技术方案在测试和实例应用中表现出较高的性能：在台机电180 nm工艺下，芯片性能达1.2 DMIPS／MHz，时钟频率高达100 MHz。
在工控领域，用户要求具有更快的中断速度，Cortex-M3采用了Tail-Chaining中断技术，完全基于硬件进行中断处理，最多可减少12个时钟周期数，在实际应用中可减少 70%中断。
五．总结
ARM Cortex处理器系列都是基于ARMv7架构的产品，从尺寸和性能方而来看，既有少于33000个门电路的Cortex-M系列，也有高性能的Cortex-A系列。其中，Cortex-A系列是针对日益增长的，运行包括Linux、Windows，CE和Symbian操作系统在内的消费娱乐和无线产品设计的；ARM Cortex-R系列针对的是需要运行实时操作系统来进行控制应用的系统，包括汽车电子、网络和影像系统；ARM Cortex-M系列则面向微控制器领域，为那些对开发费用非常敏感同时对性能要求不断增加的嵌入式应用所设计的。可见随着在各种不同领域应用需求的增加，微处理器市场也在趋于多样化。
