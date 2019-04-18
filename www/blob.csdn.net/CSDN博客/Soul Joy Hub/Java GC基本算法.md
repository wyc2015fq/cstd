# Java GC基本算法 - Soul Joy Hub - CSDN博客

2017年03月13日 14:02:50[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：310标签：[jvm																[内存																[java																[gc](https://so.csdn.net/so/search/s.do?q=gc&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=内存&t=blog)](https://so.csdn.net/so/search/s.do?q=jvm&t=blog)
个人分类：[Java](https://blog.csdn.net/u011239443/article/category/6296471)


							http://blog.csdn.net/heyutao007/article/details/38151581

**1、JVM内存组成结构**

JVM内存结构由堆、栈、本地方法栈、方法区等部分组成，结构图如下所示：

![](https://img-blog.csdn.net/20140726230349776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSEVZVVRBTzAwNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

1)堆

所有通过new创建的对象的内存都在堆中分配，其大小可以通过-Xmx和-Xms来控制。堆被划分为新生代和旧生代，新生代又被进一步划分为Eden和Survivor区，Survivor由FromSpace和ToSpace组成，结构图如下所示：

![](https://img-blog.csdn.net/20140726230310906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSEVZVVRBTzAwNw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

新生代。新建的对象都是用新生代分配内存，Eden空间不足的时候，会把存活的对象转移到Survivor中，新生代大小可以由-Xmn来控制，也可以用-XX:SurvivorRatio来控制Eden和Survivor的比例。

旧生代用于存放新生代中经过多次垃圾回收(也即Minor GC)仍然存活的对象

2)栈

每个线程执行每个方法的时候都会在栈中申请一个栈帧，每个栈帧包括局部变量区和操作数栈，用于存放此次方法调用过程中的临时变量、参数和中间结果

3)本地方法栈

用于支持native方法的执行，存储了每个native方法调用的状态

4)方法区

存放了要加载的类信息、静态变量、final类型的常量、属性和方法信息。JVM用持久代(或者叫做永久代，PermanetGeneration)来存放方法区，可通过-XX:PermSize和-XX:MaxPermSize来指定最小值和最大值。

在过去（自定义类加载器还不是很常见的时候），类大多是”static”的，很少被卸载或收集，因此被称为“永久的(Permanent)”。同时，由于类class是JVM实现的一部分，并不是由应用创建的，所以又被认为是“非堆(non-heap)”内存。

在JDK8之前的HotSpot JVM，存放这些”永久的”的区域叫做“永久代(permanent generation)”。永久代是一片连续的堆空间，在JVM启动之前通过在命令行设置参数-XX:MaxPermSize来设定永久代最大可分配的内存空间，默认大小是64M（64位JVM由于指针膨胀，默认是85M）。永久代的垃圾收集是和老年代(old generation)捆绑在一起的，因此无论谁满了，都会触发永久代和老年代的垃圾收集。不过，一个明显的问题是，当JVM加载的类信息容量超过了参数-XX：MaxPermSize设定的值时，应用将会报OOM的错误。

永久代在JDK8中被完全的移除了。所以永久代的参数-XX:PermSize和-XX：MaxPermSize也被移除了。

在JDK8中,classe metadata(the virtual machines internal presentation of Java class),被存储在叫做Metaspace的native memory。一些新的flags被加入：

-XX:MetaspaceSize，class metadata的初始空间配额，以bytes为单位，达到该值就会触发垃圾收集进行类型卸载，同时GC会对该值进行调整：如果释放了大量的空间，就适当的降低该值；如果释放了很少的空间，那么在不超过MaxMetaspaceSize（如果设置了的话），适当的提高该值。

-XX：MaxMetaspaceSize，可以为class metadata分配的最大空间。默认是没有限制的。

-XX：MinMetaspaceFreeRatio,在GC之后，最小的Metaspace剩余空间容量的百分比，减少为class metadata分配空间导致的垃圾收集

-XX:MaxMetaspaceFreeRatio,在GC之后，最大的Metaspace剩余空间容量的百分比，减少为class metadata释放空间导致的垃圾收集

默认情况下，class metadata的分配仅受限于可用的native memory总量。可以使用MaxMetaspaceSize来限制可为class metadata分配的最大内存。当class metadata的使用的内存达到MetaspaceSize(32位clientVM默认12Mbytes,32位ServerVM默认是16Mbytes)时就会对死亡的类加载器和类进行垃圾收集。设置MetaspaceSize为一个较高的值可以推迟垃圾收集的发生。

介绍完了JVM内存组成结构，下面我们再来看一下JVM垃圾回收机制。

**2、Java GC基本算法**

2.1、引用计数（reference counting）

    原理：此对象有一个引用，则+1；删除一个引用，则-1。只用收集计数为0的对象。

    缺点：（1）无法处理循环引用的问题。如：对象A和B分别有字段b、a，令A.b=B和B.a=A，除此之外这2个对象再无任何引用，那实际上这2个对象已经不可能再被访问，但是引用计数算法却无法回收他们。（2）引用计数的方法需要编译器的配合。编译器需要为此对象生成额外的代码。如赋值函数将此对象赋值给一个引用时，需要增加此对象的引用计数。还有就是，当一个引用变量的生命周期结束时，需要更新此对象的引用计数器。

引用计数的方法由于存在显著的缺点，实际上并未被JVM所使用。

2.2、复制（copying）

    原理：把内存空间划分为2个相等的区域，每次只使用一个区域。垃圾回收时，遍历当前使用区域，把正在使用的对象复制到另外一个区域。
 优点：不会出现碎片问题。

    缺点：
（1）暂停整个应用。
（2）需要2倍的内存空间。

2.3、标记-清扫（Mark-and-sweep）

    原理：对于“活”的对象，一定可以追溯到其存活在堆栈、静态存储区之中的引用。这个引用链条可能会穿过数个对象层次,算法基于有向图，采用深度优先搜索。
第一阶段：从GC roots开始遍历所有的引用，对有活的对象进行标记。
第二阶段：对堆进行遍历，把未标记的对象进行清除。
优点：解决了循环引用的问题。

    缺点：
（1）暂停整个应用；
（2）会产生内存碎片。
（3）不管你这个对象是不是可达的，即是不是垃圾，都要在清楚阶段被检查一遍，非常耗时.

sun前期版本就是用这个技术。

2.4、标记-压缩（Mark-Compact）

    第一阶段：同标记-清扫，标记活的对象，
第二阶段：这个阶段将所有做了标记的活动对象整理到堆的底部
  优点：
（1）避免标记扫描的碎片问题；
（2）避免停止复制的空间问题。

2.5、分代（generational collecting）  

原理：基于对象生命周期分析得出的垃圾回收算法。把对象分为年轻代、年老代、持久代，对不同的生命周期使用不同的算法（2-3方法中的一个即4自适应）进行回收。

J2SE1.2以后使用此算法  

JVM分别对新生代和旧生代采用不同的垃圾回收机制

2.5.1、新生代的GC（Minor GC）：

指发生在新生代的垃圾收集动作，因为 Java 对象大多都具备朝生夕灭的特性，所以 Minor GC 非常频繁，一般回收速度也比较快。

新生代通常存活时间较短，因此基于Copying算法来进行回收，所谓Copying算法就是扫描出存活的对象，并复制到一块新的完全未使用的空间中，对应于新生代，就是在Eden和FromSpace或ToSpace之间copy。新生代采用空闲指针的方式来控制GC触发，指针保持最后一个分配的对象在新生代区间的位置，当有新的对象要分配内存时，用于检查空间是否足够，不够就触发GC。当连续分配对象时，对象会逐渐从eden到survivor，最后到旧生代，

2.5.2、旧生代的GC（Major GC  / Full GC）：

指发生在老年代的 GC。旧生代与新生代不同，对象存活的时间比较长，比较稳定，因此采用标记(Mark)算法来进行回收，所谓标记就是扫描出存活的对象，然后再进行回收未被标记的对象，回收后对用空出的空间要么进行合并，要么标记出来便于下次进行分配，总之就是要减少内存碎片带来的效率损耗。

MajorGC 的速度一般会比 Minor GC 慢 10倍以上。

Thinking in java给java gc取了一个罗嗦的称呼：“自适应、分代的、停止-复制、标记-扫描”式的垃圾回收器。

导致Gc的情况：

1、tenured被写满

2、perm被写满

3、System.gc()的显式调用。

4、上一次GC之后heap的各域分配策略动态变化。


