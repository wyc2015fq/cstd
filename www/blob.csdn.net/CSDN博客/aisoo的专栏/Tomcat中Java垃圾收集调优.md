# Tomcat中Java垃圾收集调优 - aisoo的专栏 - CSDN博客
2012年12月06日 10:33:01[aisoo](https://me.csdn.net/aisoo)阅读数：13846
**Tomcat中Java垃圾收集调优**
**1 [JVM内存JAVA_OPTS参数说明](http://lshh83.javaeye.com/blog/339266)**
设置服务器端的JVM参数一般在catalina.bat文件中:JAVA_OPTS="-server -Xms2048m -Xmx2048m-Xss512k"
**JVM中对象的划分及管理介绍**
JVM根据运行于其中的对象的生存时间大致的分为3种。并且将这3种不同的对象分别存放在JVM从系统分配到的不同的内存空间。这种对象存放空间的管理方式叫做Generation管理方式。
1). Young Generation（年轻代）：用于存放“早逝”对象（即瞬时对象）。例如：在创建对象时或者调用方法时使用的临时对象或局部变量。
2). Tenured Generation(年老代)：用于存放“驻留”对象（即较长时间被引用的对象）。往往体现为一个大型程序中的全局对象或长时间被使用的对象。
3). Perm Generation(永久保存区域)：用于存放“永久”对象。这些对象管理着运行于JVM中的类和方法。
------------------------------------------------------------------------------
在命令行下用 java-XmxXXXXM -version 命令来测试java可用最大内存，测试可逐渐增大XXXX的值，如果执行正常就表示指定的内存大小可用，否则会打印错误信息。
通常测试windows系统(32位)最大内存为：1500M,但系统一般到1280M就差不多了
------------------------------------------------------
**关于垃圾收集分类介绍**
在JVM中有两种垃圾方式:
1). 一种叫做Minor（次收集）。Minor在YoungGeneration（年轻代）的空间被对象全部占用后执行，主要是对Young Generation中的对象进行垃圾收集。
2). 一种叫做Major（主收集）。Major是针对于整个Heap size(Xms和Xmx设置为JVM使用的内存，但不包括永久保存区域使用的内存)的垃圾收集。
其中Minor方式的收集经常发生，并且Minor收集所占用的系统时间小。而Major方式的垃圾收集则是一种“昂贵”的垃圾收集方式，因为在Major要对整个Heap size进行垃圾收集,这会使得应用停顿的时间变得较长。
**关于TOMCAT内存占用介绍**
Tomcat运行占用内存= Xmx占用的内存 + Perm Generation(永久保存区域)占用内存 + 所有Java应用创建线程数x 1M
Java应用每创建一个线程，在JVM的内存里也会创建一个Thread对象，但是同时也会在操作系统里创建一个真正的物理线程（参考JVM规范），操作系统会在TOMCAT余下的内存里创建这个物理线程，而不是在JVM的Xmx设置的内存堆里创建。在jdk1.4里头，默认的栈大小是256KB，但是在jdk1.5里头，默认的栈大小为1M每线程。因此，如果系统剩余内存为400M的可用内存，则Java应用最多创建400个可用线程。结论：要想创建更多的线程，必须减少分配给JVM的最大内存。
**参数说明如下**：
-server:一定要作为第一个参数，在多个CPU时性能佳 
-Xms：初始Heap大小，使用的最小内存,cpu性能高时此值应设的大一些 
-Xmx：java heap最大值，使用的最大内存 
上面两个值是分配JVM的最小和最大内存，取决于硬件物理内存的大小，建议均设为物理内存的一半,最大不要超过可用物理内存的80％。 
-Xmn：young generation（年轻代）的heap大小，一般设置为Xmx的3、4分之一（此值对系统性能影响较大，Sun官方推荐配置为整个堆的3/8）
(可使用-XX:NewSize和-XX:MaxNewsize设置年轻代的初始值和最大值)
-Xincgc :启动增量垃圾收集器，缺省是关闭的。增量垃圾收集器能减少偶然发生的长时间的垃圾回收造成的暂停时间。但增量垃圾收集器和应用程序并发执行，因此会占用部分CPU在应用程序上的功能。
-XX:CMSInitiatingOccupancyFraction=70 发现引起promotion failed错误的原因是CMS来不及回收（CMS默认在年老代占到90%左右才会执行），年老代又没有足够的空间供GC把一些活的对象从年轻代移到年老代，所以执行Full GC。CMSInitiatingOccupancyFraction=70表示年老代占到约70%时就开始执行CMS，这样就不会频繁出现Full GC了。
上两个参数设置有很大技巧，基本上满足:
(Xmx-Xmn)*(100- CMSInitiatingOccupancyFraction)/100>=Xmn就不会出现promotion failed。如果在应用中设置Xmx（最大内存）是1500m，Xmn（年轻代）是340m，那么Xmx-Xmn是1160m，也就是年老代有1160 兆，CMSInitiatingOccupancyFraction=70说明年老代到70%满的时候开始执行对年老代的并发垃圾回收（CMS），这时还剩30%的空间是1160*30%=348兆，所以即使Xmn（也就是年轻代共340兆）里所有对象都搬到年老代里，348兆的空间也足够了，所以只要满足上面的公式，就不会出现垃圾回收时的promotionfailed 
-XX:PermSize= xxxm:设定xxx兆内存的永久保存区域
-XX:MaxPermSize=xxxm:设定xxx兆最大内存的永久保存区域 
PermSize和MaxPermSize指明虚拟机为java永久生成对象（Permanate generation）如，class对象、方法对象这些可反射（reflective）对象分配内存限制，这些内存不包括在Heap（堆内存）区之中。上述参数如果不设定，永久保存区域默认大小：-server选项下默认MaxPermSize为64m，-client选项下默认MaxPermSize为32m。运行程序时，jvm会调整永久保存区域的大小以满足需要。每次调整时，jvm会对堆进行一次完全的垃圾收集。
-XX:+UseConcMarkSweepGC ：选择CMS收集器（并发回收，缩短major收集的时间） 
提示：此选项在Heap Size 比较大而且Major收集时间较长的情况下使用更合适。
【-XX:+UseParNewGC ：对年轻代采用多线程并行回收，这样收得快（缩短minor收集的时间，如果设置-XX:+UseConcMarkSweepGC，无须设置-XX:+UseParNewGC，是默认的）】
-XX:MaxTenuringThreshold=5  CMS收集器中，新生代对象撑过过多少次minor gc才进入年老代的。默认为0（或另一说法：一个对象如果在救助空间移动5次还没有被回收就放入年老代）。如果设置为0就是去掉了新生代空间，存活的临时对象不经过Survivor区直接进入年老代，不久就占满年老代发生full gc
-XX:GCTimeRatio=19 表示java可以用5%的时间来做垃圾回收，1/(1+19)=1 /20=5%
-XX:CMSFullGCsBeforeCompaction=N表示执行N次Full GC后执行内存压缩，免得产生内存碎片
             (案例都设置为：-XX:CMSFullGCsBeforeCompaction=0)
-XX:+UseCMSCompactAtFullCollection：表示执行Full GC后对内存进行整理压缩，免得产生内存碎片
-Xnoclassgc：禁用类垃圾回收，性能会高一点；
-verbose:gc 显示垃圾收集信息 （在虚拟机发生内存回收时在输出设备显示信息）
-Xloggc:gc.log 指定垃圾收集日志文件 
-XX:+DisableExplicitGC禁止System.gc()：免得程序员误调用gc方法影响性能；
-XX:+ExplicitGCInvokesConcurrent：**System.gc()可以与应用程序并发执行。**
（System.gc()来收回不用的内存，是写在程序里的。System.gc()只是“建议”JVM回收内存，不是强制。禁止System.gc()要看实际开发的程序如何处理。因此编程要养成习惯，创建一个对象，不再用时指向null，这样jvm发现它不再使用时，会更早的把它放进回收队列，才能更早的进行回收。
例如：
你要销毁一个对象，可以 
代码: 
String a = "ksadjflasdf";
//do something. 
// 
a=null;  这不是销毁一个对象 仅仅是把对一个对象的引用去掉了
在java中一个对象可以被多个对象引用的只有一个对象不在被引用时才可以被垃圾收集）
-XX:SoftRefLRUPolicyMSPerMB=N 这个参数比较有用的，官方解释是：Soft reference在虚拟机中比在客户集中存活的更长一些。其清除频率可以用命令行参数 -XX:SoftRefLRUPolicyMSPerMB=<N>来控制，这可以指定每兆堆空闲空间的 soft reference 保持存活（一旦它不强可达了）的毫秒数，这意味着每兆堆中的空闲空间中的 soft reference 会（在最后一个强引用被回收之后）存活1秒钟。注意，这是一个近似的值，因为
 soft reference 只会在垃圾回收时才会被清除，而垃圾回收并不总在发生。系统默认为一秒，我觉得没必要等1秒，客户集中不用就立刻清除，改为-XX:SoftRefLRUPolicyMSPerMB=0；
-Xss 15120 这使得JBoss每增加一个线程（thread)就会立即消耗15M内存，而最佳值应该是128K,默认值好像是512k.
+XX:AggressiveHeap 会使得 Xms没有意义。这个参数让jvm忽略Xmx参数,疯狂地吃完一个G物理内存,再吃尽一个G的swap。
-Xss：每个线程的Stack大小 
显示日志参数
-verbose:gc在虚拟机发生内存回收时在输出设备显示信息，格式如下：
[Full GC268K->168K(1984K), 0.0187390 secs]
该参数用来监视虚拟机内存回收的情况。-XX:+PrintGCDetails
-XX:+PrintGCTimeStamps(GC发生的时间) 
-XX:+PrintGCApplicationStoppedTime(GC消耗了多少时间) 
-XX:+PrintGCApplicationConcurrentTime(GC之间运行了多少时间)
-XX:+PrintTenuringDistribution 参数观察各个Age的对象总大小
**2GC日志打印**
  GC调优是个很实验很伽利略的活儿，GC日志是先决的数据参考和最终验证：
-XX:+PrintGCDetails -XX:+PrintGCTimeStamps(GC发生的时间)-XX:+PrintGCApplicationStoppedTime(GC消耗了多少时间)-XX:+PrintGCApplicationConcurrentTime(GC之间运行了多少时间)
**3 收集器选择**
**CMS收集器（并发回收）**：暂停时间优先
   配置参数：-XX:+UseConcMarkSweepGC
   已默认无需配置的参数：-XX:+UseParNewGC(Parallel收集新生代)             
-XX:+CMSPermGenSweepingEnabled(CMS收集持久代)-XX:UseCMSCompactAtFullCollection(full gc时压缩年老代)
   初始效果：1g堆内存的新生代约60m，minorgc约5-20毫秒，full gc约130毫秒。
**Parallel收集器（并行回收）**：吞吐量优先
    配置参数： -XX:+UseParallelGC-XX:+UseParallelOldGC(Parallel收集年老代，从JDK6.0开始支持)
    已默认无需配置的参数： -XX:+UseAdaptiveSizePolicy(动态调整新生代大小)
    初始效果：1g堆内存的新生代约90-110m(动态调整)，minor gc约5-20毫秒，fullgc有无UseParallelOldGC 参数分别为1.3/1.1秒，差别不大。
另外-XX:MaxGCPauseMillis=100设置minor gc的期望最大时间，JVM会以此来调整新生代的大小，但在此测试环境中对象死的太快，此参数作用不大。
**4 调优实战**
      Parallel收集高达1秒的暂停时间基本不可忍受，所以选择CMS收集器。 
      在被压测的Mule 2.0应用里，每秒都有大约400M的海量短命对象产生：
因为默认60M的新生代太小了，频繁发生minor gc，大约0.2秒就进行一次。
因为CMS收集器中MaxTenuringThreshold(新生代对象撑过过多少次minor gc才进入年老代的设置)默认0，存活的临时对象不经过Survivor区直接进入年老代，不久就占满年老代发生full gc。
     对这两个参数的调优，既要改善上面两种情况，又要避免新生代过大，复制次数过多造成minorgc的暂停时间过长。
使用-Xmn调到1/3 总内存。观察后设置-Xmn500M，新生代实际约460m。(用-XX:NewRatio设置无效，只能用 -Xmn)。
添加-XX:+PrintTenuringDistribution 参数观察各个Age的对象总大小，观察后设置-XX:MaxTenuringThreshold=5。
      优化后，大约1.1秒才发生一次minorgc，且速度依然保持在15-20ms之间。同时年老代的增长速度大大减缓，很久才发生一次full gc，
      参数定稿：
 -server -Xms1536m -Xmx1536m-Xmn512m 
-XX:+UseConcMarkSweepGC   -XX:MaxTenuringThreshold=5  -XX:+ExplicitGCInvokesConcurrent-XX:GCTimeRatio=19 -XX:CMSInitiatingOccupancyFraction=70  -XX:CMSFullGCsBeforeCompaction=0 -Xnoclassg
      最后服务处理速度从1180 tps 上升到1380 tps，调整两个参数提升17%的性能还是笔很划算的买卖。
     另外，JDK6 Update 7自带了一个VisualVM工具，内里就是之前也有用过的Netbean Profiler，类似JConsole一样使用，可以看到线程状态，内存中对象以及方法的CPU时间等调优重要参考依据。免费捆绑啊，Sun 这样搞法，其他做Profiler的公司要关门了。
标准参数设置参考样本（tomcat可用8G内存案例）
set JAVA_OPTS=%JAVA_OPTS%  -server –Xms8192m –Xmx8192m–Xmn1890m -verbose:gc -XX:+UseConcMarkSweepGC  -XX:MaxTenuringThreshold=5 -XX:+ExplicitGCInvokesConcurrent -XX:GCTimeRatio=19 -XX:CMSInitiatingOccupancyFraction=70-XX:CMSFullGCsBeforeCompaction=0
 –Xnoclassgc -XX:SoftRefLRUPolicyMSPerMB=0
