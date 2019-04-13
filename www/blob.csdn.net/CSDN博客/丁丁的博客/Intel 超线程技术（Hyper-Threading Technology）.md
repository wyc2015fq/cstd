
# Intel 超线程技术（Hyper-Threading Technology） - 丁丁的博客 - CSDN博客


2012年08月01日 02:08:20[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：4175


我们经常会提到两种“线程”（thread）。
一种线程存在于操作系统（OS）中。对OS而言，进程是资源分配的基本单位，每个程序在一个进程中分配相应的内存等系统资源。而每个进程可以包含多个“线程”。这些线程共享进程分配的资源，但是独立地被OS调度执。我们可以称这种线程为“软线程”。
另一种线程则是我们将要介绍的，因为它与处理器密切相关，我们不妨称之为“硬线程”。
可以认为，这两种线程除了名称相同外，没有任何关系，因此注意不要混淆。我们以下主要讨论“硬线程”。
Intel的超线程技术（Hyper-Threading Technology, HT）最早出现在2002年3月发布的Xeon处理器上，并于同年底应用在Pentium 4处理器中。之后的Itanium、Atom和Core 'i'系列中都利用了这项技术。
在支持超线程技术的处理器中，每个核心（此处我们也称为一个CPU）的指令流水线同时维护2条指令流。流水线按照一定的规则，反复地在两条指令流上切换。这一过程对OS是完全透明的。在OS看来，仿佛真的存在着两个执行核心（逻辑核心或虚拟核心）。这就使得可供OS调度的计算资源增加了一倍，从而提升系统整体性能。
超线程中的各个硬线程之间是独立被OS调度的。如果我们有一个程序启动了2个软线程，那么它们可以同时在运行在2个不同硬线程上，而这2个硬线程可能来自不同的物理核心。硬线程对程序是完全透明的，我们不能够指定将某人程序或软线程运行在某个硬线程上，这些工作由OS来完成。但我们可以通过工具（如Intel® VTune™ Performance Analyzer和Intel® Thread
 Profiler等）来查看硬线程的运行状况，并通过优化多线程程序来提高硬线程的利用率。
超线程技术需要BIOS、芯片组和OS（需支持SMP）的支持，所幸通常这些都不构成问题。在有些平台下，用户能够通过BIOS选项，手动设置是否启用超线程。而通常超线程都是默认启动的。在超线程启用的情况下，我们会看到2倍于CPU实际物理核心数目的CPU。例如，对一个4核心的i7
 处理器，资源管理器中会显示有8有个CPU。即有8个硬线程运行在4个物理核心上，每个核心运行2个线程。（在Windows下可以通过资源管理器，在Linux可以通过/proc/cpuinfo 查看CPU的信息。）
![](https://img-my.csdn.net/uploads/201208/01/1343752089_5050.jpg)![](https://img-my.csdn.net/uploads/201208/01/1343752099_5538.jpg)
在禁用（左）和启用（右）多线程的配置下，Windows资源管理器的显示情况
从宏观角度看，超线程技术有助于提高处理器并行性，OS有了更多的计算核心可供调度。从微观看，多线程技术增加了指令的吞吐量。当物理核心的一条指令流执行到一条延迟较大的指令时，可以切换到另一个指令流进行执行，从而提高了处理器的占用率。按照Intel公司的数据，最早的超线程实现上，仅比同时期的非超线程的处理器多占用了5%的基座面积，而性能提升了15~30%。
参考文献
Wikipedia:[
Hyper-Threading](http://en.wikipedia.org/wiki/Hyper-threading).
Shannon Cepeda:[
Intel® Hyper-Threading Technology: Your Questions Answered](http://software.intel.com/en-us/blogs/2009/06/02/intel-hyper-threading-technology-your-questions-answered/).

