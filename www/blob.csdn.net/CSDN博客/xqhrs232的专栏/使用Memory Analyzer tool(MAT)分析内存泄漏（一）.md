# 使用Memory Analyzer tool(MAT)分析内存泄漏（一） - xqhrs232的专栏 - CSDN博客
2011年02月17日 11:35:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1390标签：[generation																[outofmemoryerror																[jvm																[profiler																[reference																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=reference&t=blog)](https://so.csdn.net/so/search/s.do?q=profiler&t=blog)](https://so.csdn.net/so/search/s.do?q=jvm&t=blog)](https://so.csdn.net/so/search/s.do?q=outofmemoryerror&t=blog)](https://so.csdn.net/so/search/s.do?q=generation&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://www.blogjava.net/rosen/archive/2010/05/21/321575.html](http://www.blogjava.net/rosen/archive/2010/05/21/321575.html)
**前言的前言**：本文是自2005年8月以来，首次在一个月之内发布三篇文章。谨以此文献给这么多年始终不济的我。所谓少不入川，而今已非年少。北漂快两年了，何时能回到故乡，回去后又会怎样，也许永远是个未知……
**前言**
在平时工作过程中，有时会遇到OutOfMemoryError，我们知道遇到Error一般表明程序存在着严重问题，可能是灾难性的。所以找出是什么原因造成OutOfMemoryError非常重要。现在向大家引荐Eclipse
 Memory Analyzer tool(MAT)，来化解我们遇到的难题。如未说明，本文均使用Java 5.0 on Windows XP SP3环境。
**为什么用MAT**
之前的观点，我认为使用实时profiling/monitoring之类的工具，用一种非常实时的方式来分析哪里存在内存泄漏是很正确的。年初使用了某profiler工具测试消息中间件中存在的内存泄漏，发现在吞吐量很高的时候profiler工具自己也无法响应，这让人很头痛。后来了解到这样的工具本身就要消耗性能，且在某些条件下还发现不了泄漏。所以，分析离线数据就非常重要了，MAT正是这样一款工具。
**为何会内存溢出**
我们知道JVM根据generation(代)来进行GC，根据下图所示，一共被分为young
 generation(年轻代)、tenured generation(老年代)、permanent
 generation(永久代, perm gen)，perm gen（或称Non-Heap
非堆）是个异类，稍后会讲到。注意，heap空间不包括perm gen。
<!--[if !vml]-->![](http://java.sun.com/docs/hotspot/gc5.0/fig3.gif)
<!--[endif]-->
绝大多数的对象都在young generation被分配，也在young generation被收回，当young
 generation的空间被填满，GC会进行minor collection(次回收)，这次回收不涉及到heap中的其他generation，minor
 collection根据weak generational hypothesis(弱年代假设)来假设young generation中大量的对象都是垃圾需要回收，minor
 collection的过程会非常快。young generation中未被回收的对象被转移到tenured generation，然而tenured
 generation也会被填满，最终触发major collection(主回收)，这次回收针对整个heap，由于涉及到大量对象，所以比minor
 collection慢得多。
JVM有三种垃圾回收器，分别是throughput collector，用来做并行young generation回收，由参数-XX:+UseParallelGC启动；concurrent
 low pause collector，用来做tenured generation并发回收，由参数-XX:+UseConcMarkSweepGC启动；incremental
 low pause collector，可以认为是默认的垃圾回收器。不建议直接使用某种垃圾回收器，最好让JVM自己决断，除非自己有足够的把握。
Heap中各generation空间是如何划分的？通过JVM的-Xmx=n参数可指定最大heap空间，而`-Xms=n``则是指定`最小heap空间。在JVM初始化的时候，如果最小heap空间小于最大heap空间的话，如上图所示JVM会把未用到的空间标注为Virtual。除了这两个参数还有-XX:MinHeapFreeRatio=n和
 -XX:MaxHeapFreeRatio=n来分别控制最大、最小的剩余空间与活动对象之比例。在32位Solaris SPARC操作系统下，默认值如下，在32位windows
 xp下，默认值也差不多。
|参数|默认值|
|----|----|
|MinHeapFreeRatio|40|
|MaxHeapFreeRatio|70|
|-Xms|3670k|
|-Xmx|64m|
由于tenured generation的major collection较慢，所以tenured generation空间小于young
 generation的话，会造成频繁的major collection，影响效率。Server JVM默认的young generation和tenured
 generation空间比例为1:2，也就是说young generation的eden和survivor空间之和是整个heap（当然不包括perm
 gen）的三分之一，该比例可以通过-XX:NewRatio=n参数来控制，而Client JVM默认的-XX:NewRatio是8。至于调整young
 generation空间大小的NewSize=n和MaxNewSize=n参数就不讲了，请参考后面的资料。
young generation中幸存的对象被转移到tenured generation，但不幸的是concurrent collector线程在这里进行major
 collection，而在回收任务结束前空间被耗尽了，这时将会发生Full Collections(Full GC)，整个应用程序都会停止下来直到回收完成。Full GC是高负载生产环境的噩梦……
现在来说说异类perm gen，它是JVM用来存储无法在Java语言级描述的对象，这些对象分别是类和方法数据（与class
 loader有关）以及interned strings(字符串驻留)。一般32位OS下perm
 gen默认64m，可通过参数-XX:MaxPermSize=n指定，[JVM
 Memory Structure](http://www.yourkit.com/docs/kb/sizes.jsp)一文说，对于这块区域，没有更详细的文献了，神秘。
回到问题“为何会内存溢出？”。
要回答这个问题又要引出另外一个话题，既什么样的对象GC才会回收？当然是GC发现通过任何reference chain(引用链)无法访问某个对象的时候，该对象即被回收。名词GC
 Roots正是分析这一过程的起点，例如JVM自己确保了对象的可到达性(那么JVM就是GC
 Roots)，所以GC Roots就是这样在内存中保持对象可到达性的，一旦不可到达，即被回收。通常GC Roots是一个在current thread(当前线程)的call
 stack(调用栈)上的对象（例如方法参数和局部变量），或者是线程自身或者是system class loader(系统类加载器)加载的类以及native
 code(本地代码)保留的活动对象。所以GC Roots是分析对象为何还存活于内存中的利器。知道了什么样的对象GC才会回收后，再来学习下对象引用都包含哪些吧。
从最强到最弱，不同的引用（可到达性）级别反映了对象的生命周期。
<!--[if !supportLists]-->l<!--[endif]-->Strong Ref(强引用)：通常我们编写的代码都是Strong Ref，于此对应的是强可达性，只有去掉强可达，对象才被回收。
<!--[if !supportLists]-->l<!--[endif]-->Soft Ref(软引用)：对应软可达性，只要有足够的内存，就一直保持对象，直到发现内存吃紧且没有Strong Ref时才回收对象。一般可用来实现缓存，通过java.lang.ref.SoftReference类实现。
<!--[if !supportLists]-->l<!--[endif]-->Weak Ref(弱引用)：比Soft Ref更弱，当发现不存在Strong
 Ref时，立刻回收对象而不必等到内存吃紧的时候。通过java.lang.ref.WeakReference和java.util.WeakHashMap类实现。
<!--[if !supportLists]-->l<!--[endif]-->Phantom Ref(虚引用)：根本不会在内存中保持任何对象，你只能使用Phantom Ref本身。一般用于在进入finalize()方法后进行特殊的清理过程，通过java.lang.ref.PhantomReference实现。
有了上面的种种我相信很容易就能把heap和perm gen撑破了吧，是的利用Strong Ref，存储大量数据，直到heap撑破；利用interned
 strings（或者class loader加载大量的类）把perm gen撑破。
**关于shallow size、retained size**
Shallow size就是对象本身占用内存的大小，不包含对其他对象的引用，也就是对象头加成员变量（不是成员变量的值）的总和。在32位系统上，对象头占用8字节，int占用4字节，不管成员变量（对象或数组）是否引用了其他对象（实例）或者赋值为null它始终占用4字节。故此，对于String对象实例来说，它有三个int成员（3*4=12字节）、一个char[]成员（1*4=4字节）以及一个对象头（8字节），总共3*4
 +1*4+8=24字节。根据这一原则，对String a=”rosen jiang”来说，实例a的shallow size也是24字节（很多人对此有争议，请看官甄别并留言给我）。
Retained size是该对象自己的shallow size，加上从该对象能直接或间接访问到对象的shallow size之和。换句话说，retained
 size是该对象被GC之后所能回收到内存的总和。为了更好的理解retained size，不妨看个例子。
把内存中的对象看成下图中的节点，并且对象和对象之间互相引用。这里有一个特殊的节点GC Roots，正解！这就是reference chain的起点。
<!--[if !vml]-->![retained_objects.gif](http://www.yourkit.com/docs/90/help/retained_objects.gif)<!--[endif]--> <!--[if !vml]-->![retained_objects_2.gif](http://www.yourkit.com/docs/90/help/retained_objects_2.gif)<!--[endif]-->
从obj1入手，上图中蓝色节点代表仅仅只有通过obj1才能直接或间接访问的对象。因为可以通过GC Roots访问，所以左图的obj3不是蓝色节点；而在右图却是蓝色，因为它已经被包含在retained集合内。
所以对于左图，obj1的retained size是obj1、obj2、obj4的shallow
 size总和；右图的retained size是obj1、obj2、obj3、obj4的shallow
 size总和。obj2的retained size可以通过相同的方式计算。
**Heap Dump**
heap dump是特定时间点，java进程的内存快照。有不同的格式来存储这些数据，总的来说包含了快照被触发时java对象和类在heap中的情况。由于快照只是一瞬间的事情，所以heap
 dump中无法包含一个对象在何时、何地（哪个方法中）被分配这样的信息。
在不同平台和不同java版本有不同的方式获取heap dump，而MAT需要的是HPROF格式的heap
 dump二进制文件。想无需人工干预的话，要这样配置JVM参数：-XX:-HeapDumpOnOutOfMemoryError，当错误发生时，会自动生成heap
 dump，在生产环境中，只有用这种方式。如果你想自己控制什么时候生成heap dump，在Windows+JDK6环境中可利用JConsole工具，而在Linux或者Mac
 OS X环境下均可使用JDK5、6自带的jmap工具。当然，还可以配置JVM参数：-XX:+HeapDumpOnCtrlBreak，也就是在控制台使用Ctrl+Break键来生成heap
 dump。由于我是windows+JDK5，所以选择了-XX:-HeapDumpOnOutOfMemoryError这种方式，更多配置请参考[MAT
 Wiki](http://wiki.eclipse.org/index.php/MemoryAnalyzer)。
**参考资料**
[MAT Wiki](http://wiki.eclipse.org/index.php/MemoryAnalyzer)
[Interned Strings](http://mindprod.com/jgloss/interned.html)
[Strong,Soft,Weak,Phantom Reference](http://mindprod.com/jgloss/weak.html)
[Tuning Garbage Collection with the 5.0 Java[tm] Virtual Machine](http://java.sun.com/docs/hotspot/gc5.0/gc_tuning_5.html)
[Permanent Generation](http://mindprod.com/jgloss/permgen.html)
[Understanding Weak References译文](http://blog.csdn.net/xtyyumi301/archive/2008/10/04/3015493.aspx)
[Java HotSpot VM Options](http://java.sun.com/javase/technologies/hotspot/vmoptions.jsp)
[Shallow and retained sizes](http://www.yourkit.com/docs/90/help/sizes.jsp)
[JVM Memory Structure](http://www.yourkit.com/docs/kb/sizes.jsp)
[GC roots](http://www.yourkit.com/docs/90/help/gc_roots.jsp)
[**请注意！引用、转贴本文应注明原作者：Rosen Jiang 以及出处：**](http://www.yourkit.com/docs/90/help/gc_roots.jsp)[**http://www.blogjava.net/rosen**](http://writeblog.csdn.net/archive/2010/rosen/archive/2010/rosen/archive/2009/10/archive/2009/09/archive/2009/07/archive/2009/06/archive/2009/04/archive/2009/03/archive/2009/02/archive/2008/12/archive/2008/rosen)

