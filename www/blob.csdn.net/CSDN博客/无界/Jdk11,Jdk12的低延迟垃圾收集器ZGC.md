
# Jdk11,Jdk12的低延迟垃圾收集器ZGC - 无界 - CSDN博客

2019年03月27日 11:49:12[21aspnet](https://me.csdn.net/21aspnet)阅读数：156


[https://wiki.openjdk.java.net/display/zgc/Main](https://wiki.openjdk.java.net/display/zgc/Main)
**Z垃圾收集器**，也称为**ZGC**，是一种可扩展的低延迟垃圾收集器，旨在实现以下目标：
暂停时间不超过10毫秒
暂停时间不会随堆或实时设置大小而增加
处理堆范围从几百M到几T字节大小
一目了然，ZGC是：
同时
基于区域的
压实
NUMA感知
使用彩色指针
使用负载障碍
ZGC的核心是**并发**垃圾收集器，这意味着所有繁重的工作都在**Java线程继续执行时完成**。这极大地限制了垃圾收集对应用程序响应时间的影响。
这个[OpenJDK](http://openjdk.java.net/)项目由[HotSpot Group](http://openjdk.java.net/groups/hotspot/)赞助。
JDK 11
ZGC的初始版本
不支持类卸载（使用-XX：+ ClassUnloading没有效果）
JDK 12
进一步暂停时间缩短
增加了对类卸载的支持
# JVM选项
## 启用ZGC
使用`-XX:+UnlockExperimentalVMOptions -XX:+UseZGC`选项启用ZGC。
## 设置堆大小
ZGC最重要的调整选项是设置最大堆大小（`-Xmx<size>`）。由于ZGC是并发收集器，因此必须选择最大堆大小，以便：1）堆可以容纳应用程序的实时集，以及2）堆中有足够的空间允许在GC处理时分配服务运行。需要多少空间取决于应用程序的分配率和实时设置大小。一般来说，你给ZGC的内存越多越好。但与此同时，浪费内存是不可取的，因此所有关于在内存使用和GC运行频率之间找到平衡点。
## 设置并发GC线程
可能要查看的第二个调优选项是设置并发GC线程数（`-XX:ConcGCThreads=<number>`）。ZGC具有自动选择此编号的启发式方法。这种启发式方法通常运行良好，但根据应用程序的特性，可能需要进行调整。此选项基本上决定了应该给出多少CPU时间。给它太多，GC将从应用程序中窃取过多的CPU时间。给它太少，应用程序可能比GC可以更快地分配垃圾。
**注意！**通常，如果低延迟（即低应用程序响应时间）对您的应用程序很重要，那么*永远不要*过度配置您的系统。理想情况下，您的系统的CPU利用率绝不应超过70％。
## 启用大页面
配置ZGC以使用大页面通常会产生更好的性能（在吞吐量，延迟和启动时间方面）并且没有真正的缺点，除了设置稍微复杂一点。设置过程通常需要root权限，这就是默认情况下未启用的原因。
大页面在Linux / x86上也称为“大页面”，大小为2MB。
假设你想要一个16G的Java堆。这意味着您需要16G / 2M = 8192个大页面。
首先将至少16G（8192页）的内存分配给大页面池。“至少”部分很重要，因为在JVM中启用大页面意味着不仅GC会尝试将这些用于Java堆，而且JVM的其他部分也会尝试将它们用于各种内部数据结构（代码堆，标记位图等）。在此示例中，我们将保留9216页（18G）以允许2G的非Java堆分配使用大页面。

## 启用透明大页面
使用显式大页面（如上所述）的替代方法是使用透明的大页面。对于延迟敏感的应用程序，通常**不**建议使用透明的大页面，因为它往往会导致不必要的延迟峰值。但是，可能值得尝试查看您的工作负载是否/如何受其影响。但请注意，您的里程可能会有所不同。
请注意，使用启用了透明大页面的ZGC需要**Linux内核> = 4.7**。

## 启用NUMA支持
ZGC具有基本的NUMA支持，这意味着它将尽力将Java堆分配引导到NUMA本地内存。**默认情况下启用**此功能。但是，如果JVM检测到它绑定到系统中CPU的子集，它将自动禁用。通常，您不必担心此设置，但如果要显式覆盖JVM的决策，则可以使用`-XX:+UseNUMA`或`-XX:-UseNUMA`选项来执行此操作。
在NUMA计算机（例如多插槽x86计算机）上运行时，启用NUMA支持通常会显着提升性能。
[https://www.boost.org/doc/libs/1_69_0/libs/fiber/doc/html/fiber/numa.html](https://www.boost.org/doc/libs/1_69_0/libs/fiber/doc/html/fiber/numa.html)
## [NUMA](https://www.boost.org/doc/libs/1_69_0/libs/fiber/doc/html/fiber/numa.html)
现代微处理器包含通过通道连接到存储器的集成存储器控制器。访问存储器可以分为两种：
统一存储器访问（UMA）和非统一存储器访问（NUMA）。
与UMA相比，UMA提供了一个集中的内存池（因此在一定数量的处理器之后无法扩展），NUMA架构将内存划分为相对于微处理器的本地和远程内存。
本地内存直接连接到处理器的集成内存控制器。连接到另一个微处理器（多插槽系统）的内存控制器的内存被视为远程内存。如果内存控制器访问远程内存，则必须遍历互连[[8]](https://www.boost.org/doc/libs/1_69_0/libs/fiber/doc/html/fiber/numa.html#ftn.fiber.numa.f0)并连接到远程内存控制器。
因此，访问远程内存会增加本地内存访问的额外延迟开销。由于存储位置不同，NUMA系统经验丰富*非统一*内存访问时间。
因此，通过保持内存访问本地来实现最佳性能。
![](https://img-blog.csdnimg.cn/20190327114759904.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
## [ZGC：低延迟的垃圾回收器 ](http://tang.love/2018/03/06/z-garbage-collector/)
并行化处理阶段
标记(Marking);
重定位（Relocation）/压缩（Compaction）；
重新分配集的选择（Relocation set selection）；
引用处理（Reference processing）；
弱引用的清理（WeakRefs Cleaning）;
字符串常量池（String Table）和符号表（Symbol Table）的清理；
类卸载（Class unloading）。
### ZGC 垃圾回收过程
ZGC 的垃圾回收算法和传统的`Stop-The-World`式的垃圾回收算法不太一样，后者的标记阶段和内存压缩阶段会使得应用线程挂起。ZGC 和`C4（Continuously Concurrent Compacting Collector）`）算法比较类似。
![](https://img-blog.csdnimg.cn/20190327200403289.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
一次完整的 ZGC 回收周期分为以下几个阶段（Phase）：
Pause Mark Start：标记根对象；
Concurrent Mark：并发标记阶段；
Concurrent Relocate：并发重定位；
活动对象被移动到了一个新的Heap RegionB-region 中，之前旧对象所在的Heap RegionA-region 即可复用；如果 B-region 中对象之间的引用关系将会在这一阶段被更新；
在重定位过程中，新旧对象的映射关系（同一对象在不同Region中的映射关系）被记录在了Forwarding Tables中。
Pause Mark Start：这个阶段实际上已经进入了新的ZGC Cycle，同样也是标记根对象；
Concurrent Remap：并发重映射。 这个阶段除了标记根对象直接引用的对象外，还会根据上个ZGC Cycle中生成的Forwarding Tables更新跨Heap Region的引用；
Concurrent Relocate……
从上面的垃圾回收过程可以看到，正是因为 ZGC 回收过程中各个 Phase 的并发性，才使得`GC Pause`不受垃圾回收周期内堆上活动数据数量和需要跟踪与更新的引用数量的影响，将暂停时间保持在较低的水平。
**ZGC 的垃圾回收各阶段也不都是并发执行的，在****Pause Mark Start****阶段进行根对象扫描（Root Scanning）时会出现短暂的暂停。**
ZGC 在内存整理和引用更新上采取了不同的策略，给垃圾回收过程带来了巨大的性能提升。内存整理和引用更新都是并发的，也是交替进行的（其他的垃圾回收算法在更新引用时需要所有的线程到达`safe-point`）。但与此同时，我们也应该看到，并发带来的 GC 吞吐率的下降也是不可忽视的。
当响应时间比吞吐量占有更高的优先级时，ZGC 是个不错的选择。而对那些不能接受长时间暂停的应用程序来说，ZGC 是个理想的选择。而对于那些只是在后台进行密集计算的应用程序，G1 或者 Parallel 垃圾回收器可能具有更好的垃圾回收性能。
ZGC 的介绍 PPT：
## [The Z Garbage Collector: Low Latency GC for OpenJDK](https://www.jfokus.se/jfokus18/preso/ZGC--Low-Latency-GC-for-OpenJDK.pdf)
## 。

