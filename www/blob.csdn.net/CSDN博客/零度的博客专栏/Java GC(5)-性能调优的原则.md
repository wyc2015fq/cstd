# Java GC(5)-性能调优的原则 - 零度的博客专栏 - CSDN博客
2016年05月19日 11:40:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：1104
       本篇为JavaGC的第五篇，本文将讲解Java程序性能调优的原则，尤其是在这个过程中必要的知识以及判断你的程序是否需要调优。还会介绍调优过程中你可能遇到的问题。本文最后会给出一些建议，依据这些你能在对Java程序调优时做出更好的决策。
## 1、概述
        并不是每个程序都需要调优。如果一个程序性能表现和预期一样，你不必付出额外的精力去提高它的性能。然而，在程序调试完成之后，很难马上就满足它的性能需求，于是就有了调优这项工作。无论哪种编程语言，对应用程序进行调优都需要丰富的技术知识并且注意力高度集中。另外，你也不应该用相同的方式对两个程序调优，因为每个程序都有它自己独特的运作方式和不同的资源使用方式。正因如此，调优比写程序需要更多基础知识。例如，你需要熟悉虚拟机、操作系统和计算机架构。而当你面对在这些知识基础上编写的程序时，就能成功地对它进行调优。
有时调优Java程序只需要修改JVM参数，比如GC的参数。但也有些时候需要修改程序代码。无论那种方法，你首先都需要监控执行Java程序的进程。因此本文会讲解下面几个问题：
- **怎样监控Java程序？**
- **应该给JVM设置怎样的参数？**
- **如何确定是否需要修改代码？**
## 2、对Java程序进行调优的必要知识
        Java程序在Java虚拟机中运行。因此为了进行调优，你需要理解JVM的工作流程。有一篇博文[Understanding
 JVM Internals](http://www.cubrid.org/blog/dev-platform/understanding-jvm-internals/)，将让你对JVM有深入的了解。本文中有关JVM运作过程的知识主要关于GC和Hotspot。尽管只有这两方面的知识可能无法对所有的Java程序进行调优，但是这两个因素在大多数情况下都影响着Java程序的性能。
        值得注意的是，从操作系统的角度来看，JVM也是一个应用程序进程。为了给JVM创造良好的运行环境，你还需要对操作系统分配资源的过程有所了解。这意味着，想要调优Java程序，除了JVM你也应该理解操作系统或者硬件的工作方式。
        需要具有的知识还有Java这门语言本身。另外理解锁和并发、类加载和对象创建都是非常重要的。
        当开始调优Java程序时，你应该整合以上各方面的知识来完成工作。
**3、Java程序性能调优的过程**
图1是一张Java程序性能调优的流程图，摘自由Charlie Hunt和Binu John所著的*Java Performance*。
![](http://incdn1.b0.upaiyun.com/2014/12/c0eebd5d5287749ea99a2e1ea4dba577.png)
图1：Java程序性能调优的过程
### 4、JVM分布式模型
**       JVM分布式模型**用于决定是在一个JVM还是多个JVM上执行Java程序。你可以根据其有效性、响应能力和可维护性来进行选择。当在多台服务器上运行JVM时，你也可以选择将多个JVM运行于一台服务器或者每台服务器运行一个JVM。例如，对于每台服务器，你可以运行一个使用8GB堆内存的JVM，也可以运行4个使用2GB的JVM。你理应根据处理器内核的个数还有程序的特性来决定这个数量。当优先考虑响应能力时， 使用2GB的堆内存会优于8GB的，原因是这样能在更短的时间内完成Full
 GC。当然，8GB的堆内存可以降低Full GC的频率。如果你的程序使用了内部缓存，还可以通过增加缓存命中率来提高响应能力。综上所述，选择合适的模型需要考虑应用程序的特性，然后在各种模型中 选定一个能够扬长避短的。
**5、JVM架构**
         选择JVM其实就是决定使用32位还是64位的JVM。在相同的条件下，你最好用32位的。因为32位的JVM比64位性能更好。然而，32位 JVM最大支持的堆内存是4GB（无论在32位操作系统还是64位的上，实际可分配的大小都只有2-3GB）。如果需要更大的堆内存，还是用64位的 JVM比较合适。
**表1：性能比较（[数据来源](http://readwrite.com/2011/06/06/cpp-go-java-scala-performance-benchmark)）**
|测试基准|时间（秒）|系数|
|----|----|----|
|C++ Opt|23|1.0x|
|C++ Dbg|197|8.6x|
|Java 64-bit|134|5.8x|
|Java 32-bit|290|12.6x|
|Java 32-bit GC*|106|4.6x|
|Java 32-bit SPEC GC*|89|3.7x|
|Scala|82|3.6x|
|Scala low-level*|67|2.9x|
|Scala low-level GC*|58|2.5x|
|Go 6g|161|7.0x|
|Go Pro*|126|5.5x|
        下一步就是运行程序来测试它的性能。这个过程包括GC调优、改变操作系统设置和修改代码。对于这些工作，你可以使用系统监视工具或者性能分析工具。
**        注意：**针对响应能力的调优和针对吞吐量的调优可能使用不同的方法。如果经常性地发生[stop-the-word](http://www.cubrid.org/blog/dev-platform/understanding-java-garbage-collection/)（串行GC暂时中断程序执行），程序的响应能力就会被降低。比如在高吞吐量时执行Full
 GC。不要忘记，在调优时往往有得有失。这样需要折衷处理的事情不仅发生在响应能力和吞吐量之间。例如使用更多的CPU资源来降低内存的使用，或者不得不忍受响应能力和吞吐量其中一个性能指标的下降。相反的情况同样可能发生，实际的调优应该根据各指标的优先级来执行。
        上面**图1**中的流程展示了几乎可用于所有Java程序的性能调优过程，包括Swing应用。然而，对于我们公司[NHN](http://www.cubrid.org/blog/tags/NHN/)用于提供网络服务的服务器端程序来说，这个方法多少有些不合适。下面**图2**中的流程是根据**图1**修改而来，它更简单，也更适合NHN。
![](http://incdn1.b0.upaiyun.com/2014/12/55184b2a8def7242e1a284d50cb49b32.png)
图2：对HNH的Java程序的调优过程
        其中，**Select JVM**表示尽可能使用32位的JVM，除非你需要用64位的JVM来维护一个数GB的缓存。
        现在，跟随**图2**中的流程，你会了解到每一步具体的工作。
## 6、JVM参数
         我们会主要讲解如何为Web服务端程序设置合适的JVM参数。尽管不一定适合所有的案例，但是**最好的GC算法**是[Concurrent
 Mark Sweep](http://www.cubrid.org/blog/dev-platform/understanding-java-garbage-collection/)（CMS垃圾回收），特别是对于Web服务端程序。因为**低延迟**是非常重要的。当然，在使用CMS时，由于新生代空间（New Area）的分配，可能发生较长时间的stop-the-world现象，不过调整新生代空间的大小或者它和整个堆空间的比例可能解决这个问题。
         指定新生代空间的大小和指定整个对堆内存的大小同样重要。你最好使用`–XX:NewRatio`来指定新生代和整个堆的大小比例，或者直接用`–XX:NewSize`来指定所需的新生代空间。这个配置是非常必要的，因为大部分对象都不会存活很久。在Web程序中，除了缓存数据，其他多数对象都只在`HttpRequest`到`HttpResponse`期间创建。这个时间几乎不会超过1秒，表示这些对象的存活时间也不会超过1秒。如果新生代空间不够大，对象会被转移到老年代空间，以便腾出地方给新对象使用。老年代空间（Old
 Area）垃圾回收的代价是比新生代空间大的多的，因此很需要设置一个充足的新生代空间。
          然而，当新生代空间的大小超过一个特定的水平，程序的响应能力会被降低。因为新生代空间的垃圾回收过程，基本上是将数据从一个Survivor Area复制到另外一个（From Space和To Space）。另外，stop-the-world的现象在新生代空间和老年代空间执行垃圾回收时都会发生。如果新生代空间变大，那么Survivor Area的空间也会更大，于是每次复制的数据就更多。基于这样一种特性，我们应该通过指定不同操作系统中HotSpot
 JVM的`NewRatio`参数来分配合适大小的新生代空间。
**表2：不同操作系统和配置下`NewRatio`的默认值**
|操作系统及参数|默认-XX:NewRatio|
|----|----|
|Sparc -server|2|
|Sparc -client|8|
|x86 -server|8|
|x86 -client|12|
        如果设置了`NewRatio`，那么整个堆空间的`1/(NewRatio +1)`就是新生代空间的大小。上表可以看出**Sparc -server**的NewRatio默认值很小，因为相比**x86**的操作系统，Sparc以前更多用于高端应用，这个值就是为它们设置的。但现在x86操作系统的性能有很大提升，使用它们作为服务器已经很普遍了。因此指定NewRatio为2或者3是更好的选择，就和**Sparc -server**上的配置一样。
        另外，你还可以通过指定`NewSize`和`MaxNewSize`来代替NewRatio。那么新生代空间创建时的大小就是指定的NewSize，随后可以一直增长到MaxNewSize的值。Eden（新创建对象存放的区域）和Survivor Area两个区域会随比例增加。就和你为-Xms（**译者注：**原文是-Xs，应该是笔误）和-Xmx设置相同的值一样，将MaxSize和
 MaxNewSize设置为相同的也是一个好选择。
         如果同时指定了NewRatio和NewSize，你应该使用更大的那个。于是，当堆空间被创建时，你可以用过下面的表达式计算初始新生代空间的大小：
```java
min(MaxNewSize, max(NewSize, heap/(NewRatio+1)))
```
         无论如何，仅通过一次尝试就找到合适的堆空间和新生代空间大小是不可能的。根据我在NHN运行Web服务器的经验，建议使用下面的JVM参数来运行Java程序。监控在这些参数的条件下程序的性能表现之后，你就能够选择更合适的GC算法或者配置。
**表3：推荐的JVM参数**
|类型|参数|
|----|----|
|运行模式|-sever|
|整个堆内存大小|为-Xms和-Xmx设置相同的值。|
|新生代空间大小|-XX:NewRatio: 2到4. -XX:NewSize=? –XX:MaxNewSize=?. 使用NewSize代替NewRatio也是可以的。|
|持久代空间大小|-XX:PermSize=256m -XX:MaxPermSize=256m. 设置一个在运行中不会出现问题的值即可，这个参数不影响性能。|
|GC日志|-Xloggc:$CATALINA_BASE/logs/gc.log -XX:+PrintGCDetails -XX:+PrintGCDateStamps. 记录GC日志并不会特别地影响Java程序性能，推荐你尽可能记录日志。|
|GC算法|-XX:+UseParNewGC -XX:+CMSParallelRemarkEnabled -XX:+UseConcMarkSweepGC -XX:CMSInitiatingOccupancyFraction=75. 一般来说推荐使用这些配置，但是根据程序不同的特性，其他的也有可能更好。|
|发生OOM时创建堆内存转储文件|-XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=$CATALINA_BASE/logs|
|发生OOM后的操作|-XX:OnOutOfMemoryError=$CATALINA_HOME/bin/stop.sh 或 -XX:OnOutOfMemoryError=$CATALINA_HOME/bin/restart.sh. 记录内存转储文件后，为了管理的需要执行一个合适的操作。|
## 7、测定程序的性能
       为了得到程序的性能表现，需要以下这些信息：
- **系统吞吐量（TPS、OPS）**：从整体概念上理解程序的性能。
- **每秒请求数（Request Per Second – RPS）**：严格来说，RPS和单纯的响应能力是不同的，但是你可以把它理解为响应能力。通过这个指标，你能够了解到用户需要多长时间才能得到请求的结果。
- **RPS的标准差**：如果可能的话，还有必要包括事件的RPS。一旦出现了偏差，你应该检查GC或者网络系统
       为了得到更准确的性能表现，你应该等到程序彻底启动完成后再进行测量，因为字节码随后会被HotSpot JIT编译为本地机器码。总体来说，需要在程序加载完指定功能后，用[nGrinder](http://www.nhnopensource.org/ngrinder/)等工具测试至少10分钟。
### 8、切实地调优
        如果nGrinder测试的结果满足了预期，那么你不需要对程序进行性能调优。如果没有达到预期结果，你就应该执行调优来解决问题。接下来会通过实例讲解方法。
###         stop-the-world耗时过长
        stop-the-world耗时过长可能是由于GC参数不合理或者代码实现不正确。你可以通过分析工具或堆内存转储文件（Heap dump）来定位问题，比如检查堆内存中对象的类型和数量。如果在其中找到了很多不必要的对象，那么最好去改进代码。如果没有发现创建对象的过程中有特别的问题，那么最好单纯地修改GC参数。
      为了适当地调整GC参数，你需要获取一段足够长时间的GC日志，还必须知道哪些情况会导致长时间的stop-the-world。想了解更多关于如何选择合适的GC参数，可以阅读我同事的一篇博文：[How
 to Monitor Java Garbage Collection](http://www.cubrid.org/blog/dev-platform/how-to-monitor-java-garbage-collection/)。
###         CPU使用率过低
        当系统发生阻塞，吞吐量和CPU使用率都会降低。这可能是由于网络系统或者并发的问题。为了解决这个问题，你可以分析线程转储信息（Thread dump）或者使用分析工具。阅读这篇文章可以获得更多关于线程转储分析的知识：[How
 to Analyze Java Thread Dumps](http://www.cubrid.org/blog/dev-platform/how-to-analyze-java-thread-dumps/)。
你可以使用商业的分析工具对线程锁进行精确的分析，不过大部分时候，只需使用**JVisualVM**中的CPU分析器，就能获得足够的信息。
###        CPU使用率过高
        如果吞吐量很低但是CPU使用率却很高，很可能是低效率代码导致的。这种情况下，你应该使用分析工具定位代码中性能的瓶颈。可使用的工具有：**JVisualVM**、[Eclipse](http://res.importnew.com/eclipse)**TPTP**或者**JProbe**。
## 9、调优方法
        建议你使用如下方法对程序进行调优。
       首先，检查性能调优是否必要。测量性能不是一件简单的工作，你也不能保证每次都获得满意的结果。因此如果程序已经满足预期性能需求，不必在调优上增加额外的投入了。
       问题只出在一个地方，你要做的就是去解决掉它。二八定律（[Pareto principle](http://en.wikipedia.org/wiki/Pareto_principle)）对性能调优同样适用。这不是说某个模块的低性能一定只源于一个问题，而是强调我们应该在调优时把注意力放在影响最大的那个问题上。在处理好了最重要的之后，你才应该去解决剩下其他的。也就是建议一次只对一个问题进行修复。
       另外需要考虑到气球效应（[Balloon effect](http://en.wikipedia.org/wiki/Balloon_effect)），有得必有失。你可以通过使用缓存来提高响应能力，但是当缓存逐渐增大，执行一次Full
 GC的时间也会更长。一般而言，如果你希望内存使用率比较低，那么吞吐量和响应能力可能都会恶化。因此，要知道什么对自己程序来说最重要的，而哪些又是次要的。
       到此为止，你应该已经了解了如何对Java程序进行性能调优。为了介绍性能测定的具体过程，我不得不省略其中一些细节，不过我认为这些也足够应对大多数Java Web服务端程序了。
