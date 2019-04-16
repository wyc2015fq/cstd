# Java深入 - Java 内存分配和回收机制 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年06月13日 14:55:45[initphp](https://me.csdn.net/initphp)阅读数：29536
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









Java的GC机制是自动进行的，和c语言有些区别需要程序员自己保证内存的使用和回收。

Java的内存分配和回收也主要在Java的堆上进行的，Java的堆中存储了大量的对象实例，所以Java的堆也叫GC堆。

Java在垃圾收集的过程中，主要用到了**分代收集算法**，我会先讲一下常用垃圾收集算法。




### 常用垃圾收集算法

#### **1. 标记－清除算法**

这种垃圾收集算法思路非常简单，主要是首先标记出所有需要回收的对象，然后回收所有需要回收的对象。

但是有一个明显的缺点，采用这种算法之后会发现内存块回收之后就不连续了，这就导致了在下一次想分配一个大内存块的时候无法分配。

![](https://img-blog.csdn.net/20140613151809718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




#### **2. 标记-清除-压缩**

这种垃圾收集算法主要是对上面的算法进行了优化，内存回收了对内存进行了一次优化压缩。这样回收后内存块的连续性又比较强了。

但是这种算法会涉及到不停的内存间的拷贝和复制，性能会非常差。

![](https://img-blog.csdn.net/20140613152506046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




#### **3.标记-清除-复制**

这种算法会将内存空间分配成两块相同的区域A和B。当内存回收的时候，将A中的内存块拷贝到B中，然后一次性清空A。

但是这种算法会对内存要求比较大一些，并且长期复制拷贝性能上也会受影响。

![](https://img-blog.csdn.net/20140613152930906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




### Java分代收集算法

Java主要采用了分代收集算法。分代收集算法主要将对象存活期的长短将内存进行划分。

Java主要将内存划分为两部分：**新生代**和**老生代**

Java的新生代中，对象的存活率低，存活期期会相对会比较短一些，所以可以选用**复制算法**来进行内存回收。

Java的老生代中，对象的存活率比较高，并且相对存活期比较长一些，可以采用**标记-清除-压缩**的算法来进行内存回收。

可以看图：

![](https://img-blog.csdn.net/20140613154319140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

通常新生代分为**Eden**和两个**Survivor**，其中可以通过**-XX:SurvivorRatio=1**来设置（这里要考虑两个Survivor，意味着二个S的大小是整个新生代的2/3）


前面已经说了，Java的内存分配和内存回收主要在Java的堆上进行的。而Java的方法区间和常量池我们一般称为**永久代**。永久代可以通过**-XX:PermSize=512M -XX:MaxPermSize=512M**设置

Java堆内存设置参数：**-Xmx20m -Xms20m**

Java堆新生代内存分配设置：**-Xmn10m** 新生代分配了10M的内存，那么剩下的10M就是老生代上面分配了。也可以设置：**-XX:NewRatio=4**

通过设置参数，我们就可以在控制台中看到Java虚拟机在执行GC时候的日志：**-XX:+PrintGCDetails  **

也可以指定日志的位置：**-Xloggc:gc.log   **

永久代一般是指方法区和常量池，一般情况下永久代在虚拟机运行时就能确定大小的，但是一些框架可能动态生成一些类信息就会导致永久代越来越大。****






### Java内存分配策略

使用的ParNew+Serial Old收集器组合


#### **1. 优先在Eden上分配。**



Java的对象优先会在新生代的Eden上分配。

我们可以看一个例子：

我设置了这些参数：-XX:+PrintGCDetails -Xms20m -Xmx20m -Xmn10m，堆内存分配20M，新生代10M，老生代10M，默认情况下Survivor区为8:1，所以Eden区域为8M

我运行这段代码：



```java
public class JavaTest {

    static int m = 1024 * 1024;

    public static void main(String[] args) {
        //分配2兆
        byte[] a1 = new byte[2 * m];
        System.out.println("a1 ok");
        //分配2兆
        byte[] a2 = new byte[2 * m];
        System.out.println("a2 ok");
    }
}
```


控制台日志：





```
a1 ok
a2 ok
Heap
 def new generation   total 9216K, used 4603K [0x331d0000, 0x33bd0000, 0x33bd0000)
  eden space 8192K,  56% used [0x331d0000, 0x3364ef50, 0x339d0000)
  from space 1024K,   0% used [0x339d0000, 0x339d0000, 0x33ad0000)
  to   space 1024K,   0% used [0x33ad0000, 0x33ad0000, 0x33bd0000)
 tenured generation   total 10240K, used 0K [0x33bd0000, 0x345d0000, 0x345d0000)
   the space 10240K,   0% used [0x33bd0000, 0x33bd0000, 0x33bd0200, 0x345d0000)
 compacting perm gen  total 12288K, used 381K [0x345d0000, 0x351d0000, 0x385d0000)
   the space 12288K,   3% used [0x345d0000, 0x3462f4d0, 0x3462f600, 0x351d0000)
    ro space 10240K,  55% used [0x385d0000, 0x38b51140, 0x38b51200, 0x38fd0000)
    rw space 12288K,  55% used [0x38fd0000, 0x396744c8, 0x39674600, 0x39bd0000)
```


日志中非常清晰的可以看到，我们分配了一个4M内存大小，直接是分配在了eden space里面。






#### **2. 大对象直接进入老生代。**



参数：-XX:PretenureSizeThreshold（该设置只对Serial和ParNew收集器生效） 可以设置进入老生代的大小限制，我们设置为3M，则大于3M的大对象就直接进入老生代

测试代码：



```java
public class JavaTest {

    static int m = 1024 * 1024;

    public static void main(String[] args) {
        //分配2兆
        byte[] a1 = new byte[2 * m];
        System.out.println("a1 ok");
        byte[] a3 = new byte[4 * m];
        System.out.println("a2 ok");
    }
}
```


控制台日志：





```
a1 ok
a2 ok
Heap
 def new generation   total 9216K, used 2555K [0x331d0000, 0x33bd0000, 0x33bd0000)
  eden space 8192K,  31% used [0x331d0000, 0x3344ef40, 0x339d0000)
  from space 1024K,   0% used [0x339d0000, 0x339d0000, 0x33ad0000)
  to   space 1024K,   0% used [0x33ad0000, 0x33ad0000, 0x33bd0000)
 tenured generation   total 10240K, used 4096K [0x33bd0000, 0x345d0000, 0x345d0000)
   the space 10240K,  40% used [0x33bd0000, 0x33fd0010, 0x33fd0200, 0x345d0000)
 compacting perm gen  total 12288K, used 381K [0x345d0000, 0x351d0000, 0x385d0000)
   the space 12288K,   3% used [0x345d0000, 0x3462f4d0, 0x3462f600, 0x351d0000)
    ro space 10240K,  55% used [0x385d0000, 0x38b51140, 0x38b51200, 0x38fd0000)
    rw space 12288K,  55% used [0x38fd0000, 0x396744c8, 0x39674600, 0x39bd0000)
```


上面的日志中，可以清洗看到第一次分配的2M留存在了eden space中，而4M超过了大对象设置的值3M，所以直接进入了老生代tenured generation






#### 3. 长期存活的对象进入老年代

为了演示方便，我们设置-XX:MaxTenuringThreshold=1（默认15），当在新生代中年龄为1的对象进入老年代。

测试代码：



```java
public class JavaTest {

    static int m = 1024 * 1024;

    public static void main(String[] args) {
        //分配2兆
        byte[] a1 = new byte[1 * m / 4];
        System.out.println("a1 ok");
        byte[] a2 = new byte[7 * m];
        System.out.println("a2 ok");
        byte[] a3 = new byte[3 * m]; //GC
        System.out.println("a3 ok");
    }
}
```


控制台日志：





```
a1 ok
a2 ok
[GC [DefNew: 7767K->403K(9216K), 0.0062209 secs] 7767K->7571K(19456K), 0.0062482 secs] 
[Times: user=0.00 sys=0.00, real=0.01 secs] 
a3 ok
Heap
 def new generation   total 9216K, used 3639K [0x331d0000, 0x33bd0000, 0x33bd0000)
  eden space 8192K,  39% used [0x331d0000, 0x334f9040, 0x339d0000)
  from space 1024K,  39% used [0x33ad0000, 0x33b34de8, 0x33bd0000)
  to   space 1024K,   0% used [0x339d0000, 0x339d0000, 0x33ad0000)
 tenured generation   total 10240K, used 7168K [0x33bd0000, 0x345d0000, 0x345d0000)
   the space 10240K,  70% used [0x33bd0000, 0x342d0010, 0x342d0200, 0x345d0000)
 compacting perm gen  total 12288K, used 381K [0x345d0000, 0x351d0000, 0x385d0000)
   the space 12288K,   3% used [0x345d0000, 0x3462f548, 0x3462f600, 0x351d0000)
    ro space 10240K,  55% used [0x385d0000, 0x38b51140, 0x38b51200, 0x38fd0000)
    rw space 12288K,  55% used [0x38fd0000, 0x396744c8, 0x39674600, 0x39bd0000)
```


我们可以看到在A3处有一次GC，并且a2的7M已经满足-XX:MaxTenuringThreshold=1的要求，所以a2进入老年代，而空出来的空间a3就进入新生代






#### 4. 动态对象年龄判定



为了使内存分配更加灵活，虚拟机并不要求对象年龄达到MaxTenuringThreshold才晋升老年代

如果Survivor区中相同年龄所有对象大小的总和大于Survivor区空间的一半，年龄大于或等于该年龄的对象在Minor GC时将复制至老年代




#### 5. 空间分配担保

新生代使用复制算法，当Minor GC时如果存活对象过多，无法完全放入Survivor区，就会向老年代借用内存存放对象，以完成Minor GC。

在触发Minor GC时，虚拟机会先检测之前GC时租借的老年代内存的平均大小是否大于老年代的剩余内存，如果大于，则将Minor GC变为一次Full GC，如果小于，则查看虚拟机是否允许担保失败，如果允许担保失败，则只执行一次Minor GC，否则也要将Minor GC变为一次Full GC。

说白了，新生代放不下就会借用老年代的空间来进行GC






### Java垃圾收集器：



首先我们可以看一张图，下面这张图中列出来新生代和老生代可以用到的垃圾收集器。

![](https://img-blog.csdn.net/20140614080656937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




#### 1. Serial 收集器 串行

单线程的串行收集器。它在垃圾收集的时候会暂停其它所有工作线程。直到收集结束。一般在客户端模式下使用。




#### 2. ParNew收集器 并行

ParNew收集器是Serial的多线程版本。一般运行在Server模式下首先的新生代收集器。如果老年代使用CMS收集器，基本也只能和它进行合作。参数：-XX:+UseConcMarkSweepGC，比较适合web服务的收集器。

**一般ParNew和CMS组合**





#### 3. Parallel Scavenge收集器 并行



它使用复制算法的收集器，并且是多线程的。该收集器主要目的就是达到一个可控制的吞吐量，说白了就是CPU的利用率。于是该收集器比较适合后端运算比较多的服务。

**-XX:MaxGCPauseMillis**每次年轻代垃圾回收的最长时间(最大暂停时间)，收集器尽量保证内存回收时间不大于这个值，应该设置一个合理的值。

**-XX:GCTimeRatio**设置垃圾回收时间占程序运行时间的百分比

**-XX:+UseAdaptiveSizePolicy **设置此选项后,并行收集器会自动选择年轻代区大小和相应的Survivor区比例,以达到目标系统规定的最低相应时间或者收集频率等,此值建议使用并行收集器时,一直打开.




#### 4.Serial Old收集器 串行



单线程串行的老生代收集器。




#### 5. Parallel Old 收集器 并行

使用“标记-整理”的算法。该收集器比较适合和Parallel Scavenge收集器进行组合。**-XX:+UseParallelOldGC**




#### 6. CMS收集器

CMS(Concurrent Mark Sweep)收集器是一种以获取最短回收停顿时间为目标的收集器，目前大部分的B/S系统都使用CMS的收集器。一般CMS是老生代收集器，新生代就和PerNew进行组合。

CMS收集器基于“标记-清除”的算法。分四个阶段：初始标记，并发标记，重新标记，并发清除

CMS收集器的优点：并发收集、低停顿

CMS缺点：

**1. CMS收集器对CPU资源非常敏感。**在并发阶段，虽然不会导致用户线程停顿，但是会占用CPU资源而导致引用程序变慢，总吞吐量下降。CMS默认启动的回收线程数是：(CPU数量+3) / 4。

**2. CMS收集器无法处理浮动垃圾**，可能出现“Concurrent Mode Failure“，失败后而导致另一次Full  GC的产生。由于CMS并发清理阶段用户线程还在运行，伴随程序的运行自热会有新的垃圾不断产生，这一部分垃圾出现在标记过程之后，CMS无法在本次收集中处理它们，只好留待下一次GC时将其清理掉。这一部分垃圾称为“浮动垃圾”。也是由于在垃圾收集阶段用户线程还需要运行，即需要预留足够的内存空间给用户线程使用，因此CMS收集器不能像其他收集器那样等到老年代几乎完全被填满了再进行收集，需要预留一部分内存空间提供并发收集时的程序运作使用。在默认设置下，CMS收集器在老年代使用了68%的空间时就会被激活，也可以通过参数-XX:CMSInitiatingOccupancyFraction的值来提供触发百分比，以降低内存回收次数提高性能。要是CMS运行期间预留的内存无法满足程序其他线程需要，就会出现“Concurrent
 Mode Failure”失败，这时候虚拟机将启动后备预案：临时启用Serial Old收集器来重新进行老年代的垃圾收集，这样停顿时间就很长了。所以说参数**-XX:CMSInitiatingOccupancyFraction**设置的过高将会很容易导致“Concurrent Mode Failure”失败，性能反而降低。

**3. CMS是基于“标记-清除”算法实现的收集器，使用“标记-清除”算法收集后，会产生大量碎片**。空间碎片太多时，将会给对象分配带来很多麻烦，比如说大对象，内存空间找不到连续的空间来分配不得不提前触发一次Full  GC。为了解决这个问题，CMS收集器提供了一个**-XX:UseCMSCompactAtFullCollection**开关参数，用于在Full  GC之后增加一个碎片整理过程，还可通过**-XX:CMSFullGCBeforeCompaction**参数设置执行多少次不压缩的Full
  GC之后，跟着来一次碎片整理过程。







Client、Server模式默认GC




||新生代GC方式|老年代和持久**代**GC方式|
|----|----|----|
|Client|Serial 串行GC|Serial Old 串行GC|
|Server|Parallel Scavenge  并行回收GC|Parallel Old 并行GC|




Sun/oracle JDK GC组合方式


||新生代GC方式|老年代和持久**代**GC方式|
|----|----|----|
|-XX:+UseSerialGC|Serial 串行GC|Serial Old 串行GC|
|-XX:+UseParallelGC|Parallel Scavenge  并行回收GC|Parallel Old 并行GC|
|-XX:+UseConcMarkSweepGC|ParNew 并行GC|CMS 并发GC 当出现“Concurrent Mode Failure”时采用Serial Old 串行GC|
|-XX:+UseParNewGC|ParNew 并行GC|Serial Old 串行GC|
|-XX:+UseParallelOldGC|Parallel Scavenge  并行回收GC|Parallel Old 并行GC|
|-XX:+UseConcMarkSweepGC-XX:+UseParNewGC|Serial 串行GC|CMS 并发GC 当出现“Concurrent Mode Failure”时采用Serial Old 串行GC|


### 附表：
|**参数名称**|**含义**|**默认值**||
|----|----|----|----|
|-Xms|初始堆大小|物理内存的1/64(<1GB)|默认(MinHeapFreeRatio参数可以调整)空余堆内存小于40%时，JVM就会增大堆直到-Xmx的最大限制.|
|-Xmx|最大堆大小|物理内存的1/4(<1GB)|默认(MaxHeapFreeRatio参数可以调整)空余堆内存大于70%时，JVM会减少堆直到 -Xms的最小限制|
|-Xmn|年轻代大小(1.4or lator)||**注意**：此处的大小是（eden+ 2 survivor space).与jmap -heap中显示的New gen是不同的。整个堆大小=年轻代大小 + 年老代大小 + 持久代大小.增大年轻代后,将会减小年老代大小.此值对系统性能影响较大,Sun官方推荐配置为整个堆的3/8|
|-XX:NewSize|设置年轻代大小(for 1.3/1.4)|||
|-XX:MaxNewSize|年轻代最大值(for 1.3/1.4)|||
|-XX:PermSize|设置持久代(perm gen)初始值|物理内存的1/64||
|-XX:MaxPermSize|设置持久代最大值|物理内存的1/4||
|-Xss|每个线程的堆栈大小||JDK5.0以后每个线程堆栈大小为1M,以前每个线程堆栈大小为256K.更具应用的线程所需内存大小进行 调整.在相同物理内存下,减小这个值能生成更多的线程.但是操作系统对一个进程内的线程数还是有限制的,不能无限生成,经验值在3000~5000左右一般小的应用， 如果栈不是很深， 应该是128k够用的 大的应用建议使用256k。这个选项对性能影响比较大，需要严格的[测试](http://softtest.chinaitlab.com)。（校长）和threadstacksize选项解释很类似,官方文档似乎没有解释,在论坛中有这样一句话:"”-Xss is translated in a VM flag named ThreadStackSize”一般设置这个值就可以了。|
|-*XX:ThreadStackSize*|Thread Stack Size||(0 means use default stack size) [Sparc: 512; Solaris x86: 320 (was 256 prior in 5.0 and earlier); Sparc 64 bit: 1024; Linux amd64: 1024 (was 0 in 5.0 and earlier); all others 0.]|
|-XX:NewRatio|年轻代(包括Eden和两个Survivor区)与年老代的比值(除去持久代)||-XX:NewRatio=4表示年轻代与年老代所占比值为1:4,年轻代占整个堆栈的1/5Xms=Xmx并且设置了Xmn的情况下，该参数不需要进行设置。|
|-XX:SurvivorRatio|Eden区与Survivor区的大小比值||设置为8,则两个Survivor区与一个Eden区的比值为2:8,一个Survivor区占整个年轻代的1/10|
|-XX:LargePageSizeInBytes|内存页的大小不可设置过大， 会影响Perm的大小||=128m|
|-XX:+UseFastAccessorMethods|原始类型的快速优化|||
|-XX:+DisableExplicitGC|关闭System.gc()||这个参数需要严格的[测试](http://softtest.chinaitlab.com)|
|-XX:MaxTenuringThreshold|垃圾最大年龄||如果设置为0的话,则年轻代对象不经过Survivor区,直接进入年老代. 对于年老代比较多的应用,可以提高效率.如果将此值设置为一个较大值,则年轻代对象会在Survivor区进行多次复制,这样可以增加对象再年轻代的存活 时间,增加在年轻代即被回收的概率该参数只有在串行GC时才有效.|
|-XX:+AggressiveOpts|加快编译|||
|-XX:+UseBiasedLocking|锁机制的性能改善|||
|-Xnoclassgc|禁用垃圾回收|||
|-XX:SoftRefLRUPolicyMSPerMB|每兆堆空闲空间中SoftReference的存活时间|1s|softly reachable objects will remain alive for some amount of time after the last time they were referenced. The default value is one second of lifetime per free megabyte in the heap|
|-XX:PretenureSizeThreshold|对象超过多大是直接在旧生代分配|0|单位字节 新生代采用Parallel Scavenge GC时无效另一种直接在旧生代分配的情况是大的数组对象,且数组中无外部引用对象.|
|-XX:TLABWasteTargetPercent|TLAB占eden区的百分比|1%||
|-XX:+*CollectGen0First*|FullGC时是否先YGC|false||





并行收集器相关参数
|-XX:+UseParallelGC|Full GC采用parallel MSC(此项待验证)||选择垃圾收集器为并行收集器.此配置仅对年轻代有效.即上述配置下,年轻代使用并发收集,而年老代仍旧使用串行收集.(此项待验证)|
|----|----|----|----|
|-XX:+UseParNewGC|设置年轻代为并行收集||可与CMS收集同时使用JDK5.0以上,JVM会根据系统配置自行设置,所以无需再设置此值|
|-XX:ParallelGCThreads|并行收集器的线程数||此值最好配置与处理器数目相等 同样适用于CMS|
|-XX:+UseParallelOldGC|年老代垃圾收集方式为并行收集(Parallel Compacting)||这个是JAVA 6出现的参数选项|
|-XX:MaxGCPauseMillis|每次年轻代垃圾回收的最长时间(最大暂停时间)||如果无法满足此时间,JVM会自动调整年轻代大小,以满足此值.|
|-XX:+UseAdaptiveSizePolicy|自动选择年轻代区大小和相应的Survivor区比例||设置此选项后,并行收集器会自动选择年轻代区大小和相应的Survivor区比例,以达到目标系统规定的最低相应时间或者收集频率等,此值建议使用并行收集器时,一直打开.|
|-XX:GCTimeRatio|设置垃圾回收时间占程序运行时间的百分比||公式为1/(1+n)|
|-XX:+*ScavengeBeforeFullGC*|Full GC前调用YGC|true|Do young generation GC prior to a full GC. (Introduced in 1.4.1.)|


CMS相关参数
|-XX:+UseConcMarkSweepGC|使用CMS内存收集||测试中配置这个以后,-XX:NewRatio=4的配置失效了,原因不明.所以,此时年轻代大小最好用-Xmn设置.???|
|----|----|----|----|
|-XX:+AggressiveHeap|||试图是使用大量的物理内存长时间大内存使用的优化，能检查计算资源（内存， 处理器数量）至少需要256MB内存大量的CPU／内存， （在1.4.1在4CPU的机器上已经显示有提升）|
|-XX:CMSFullGCsBeforeCompaction|多少次后进行内存压缩||由于并发收集器不对内存空间进行压缩,整理,所以运行一段时间以后会产生"碎片",使得运行效率降低.此值设置运行多少次GC以后对内存空间进行压缩,整理.|
|-XX:+CMSParallelRemarkEnabled|降低标记停顿|||
|-XX+UseCMSCompactAtFullCollection|在FULL GC的时候， 对年老代的压缩||CMS是不会移动内存的， 因此， 这个非常容易产生碎片， 导致内存不够用， 因此， 内存的压缩这个时候就会被启用。 增加这个参数是个好习惯。可能会影响性能,但是可以消除碎片|
|-XX:+UseCMSInitiatingOccupancyOnly|使用手动定义初始化定义开始CMS收集||禁止hostspot自行触发CMS GC|
|-XX:CMSInitiatingOccupancyFraction=70|使用cms作为垃圾回收使用70％后开始CMS收集|92|为了保证不出现promotion failed(见下面介绍)错误,该值的设置需要满足以下公式**[CMSInitiatingOccupancyFraction计算公式](http://developer.51cto.com/art/201201/312018.htm#CMSInitiatingOccupancyFraction_value)**|
|-XX:CMSInitiatingPermOccupancyFraction|设置Perm Gen使用到达多少比率时触发|92||
|-XX:+CMSIncrementalMode|设置为增量模式||用于单CPU情况|
|-XX:+CMSClassUnloadingEnabled||||






辅助信息


|-XX:+PrintGC|||输出形式:[GC 118250K->113543K(130112K), 0.0094143 secs][Full GC 121376K->10414K(130112K), 0.0650971 secs]|
|----|----|----|----|
|-XX:+PrintGCDetails|||输出形式:[GC [DefNew: 8614K->781K(9088K), 0.0123035 secs] 118250K->113543K(130112K), 0.0124633 secs][GC [DefNew: 8614K->8614K(9088K), 0.0000665 secs][Tenured: 112761K->10414K(121024K), 0.0433488 secs] 121376K->10414K(130112K), 0.0436268 secs]|
|-XX:+PrintGCTimeStamps||||
|-XX:+PrintGC:PrintGCTimeStamps|||可与-XX:+PrintGC -XX:+PrintGCDetails混合使用输出形式:11.851: [GC 98328K->93620K(130112K), 0.0082960 secs]|
|-XX:+PrintGCApplicationStoppedTime|打印垃圾回收期间程序暂停的时间.可与上面混合使用||输出形式:Total time for which application threads were stopped: 0.0468229 seconds|
|-XX:+PrintGCApplicationConcurrentTime|打印每次垃圾回收前,程序未中断的执行时间.可与上面混合使用||输出形式:Application time: 0.5291524 seconds|
|-XX:+PrintHeapAtGC|打印GC前后的详细堆栈信息|||
|-Xloggc:filename|把相关日志信息记录到文件以便分析.与上面几个配合使用|||
|-XX:+PrintClassHistogram|garbage collects before printing the histogram.|||
|-XX:+PrintTLAB|查看TLAB空间的使用情况|||
|XX:+PrintTenuringDistribution|查看每次minor GC后新的存活周期的阈值||Desired survivor size 1048576 bytes, new threshold 7 (max 15)new threshold 7即标识新的存活周期的阈值为7。|









