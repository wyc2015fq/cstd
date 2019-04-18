# JVM内存参数详解以及配置调优 - z69183787的专栏 - CSDN博客
2014年03月22日 20:43:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1632
基本概念：
PermGen space：全称是Permanent Generation space。就是说是永久保存的区域,用于存放Class和Meta信息，Class在被Load的时候被放入该区域
Heap space：存放Instance。GC(Garbage Collection)应该不会对PermGen space进行清理
所以如果你的APP会LOAD很多CLASS的话，就很可能出现PermGen space错误
Java Heap分为3个区，Young，Old和Permanent。Young保存刚实例化的对象。当该区被填满时，GC会将对象移到Old区。Permanent区则负责保存反射对象。
JVM有2个GC线程。
第一个线程负责回收Heap的Young区。
第二个线程在Heap不足时，遍历Heap，将Young 区升级为Older区。Older区的大小等于-Xmx减去-Xmn，不能将-Xms的值设的过大，因为第二个线程被迫运行会降低JVM的性能。
为什么一些程序频繁发生GC？有如下原因：
程序内调用了System.gc()或Runtime.gc()。
一些中间件软件调用自己的GC方法，此时需要设置参数禁止这些GC。
Java的Heap太小，一般默认的Heap值都很小。
频繁实例化对象，Release对象。此时尽量保存并重用对象，例如使用StringBuffer()和String()。
如果你发现每次GC后，Heap的剩余空间会是总空间的50%，这表示你的Heap处于健康状态。许多Server端的Java程序每次GC后最好能有65%的剩余空间。
建议Server端JVM最好将-Xms和-Xmx设为相同值。为了优化GC，最好让-Xmn值约等于-Xmx的1/3。一个GUI程序最好是每10到20秒间运行一次GC，每次在半秒之内完成。
增加Heap的大小虽然会降低GC的频率，但也增加了每次GC的时间。并且GC运行时，所有的用户线程将暂停，也就是GC期间，Java应用程序不做任何工作。
Heap大小并不决定进程的内存使用量。进程的内存使用量要大于-Xmx定义的值，因为Java为其他任务分配内存，例如每个线程的Stack等。
Stack的设定
每个线程都有他自己的Stack。
-Xss 每个线程的Stack大小 
Stack的大小限制着线程的数量。如果Stack过大就会导致内存溢漏。-Xss参数决定Stack大小，例如-Xss1024K。如果Stack太小，也会导致Stack溢漏。
硬件环境也影响GC的效率，例如机器的种类，内存，swap空间，和CPU的数量。
如果你的程序需要频繁创建很多transient对象，会导致JVM频繁GC。这种情况你可以增加机器的内存，来减少Swap空间的使用。
4种GC
1、第一种为单线程GC，也是默认的GC。，该GC适用于单CPU机器。
2、第二种为Throughput GC，是多线程的GC，适用于多CPU，使用大量线程的程序。第二种GC与第一种GC相似，不同在于GC在收集Young区是多线程的，但在Old区和第一种一样，仍然采用单线程。-XX:+UseParallelGC参数启动该GC。
3、第三种为Concurrent Low Pause GC，类似于第一种，适用于多CPU，并要求缩短因GC造成程序停滞的时间。这种GC可以在Old区的回收同时，运行应用程序。-XX:+UseConcMarkSweepGC参数启动该GC。
4、第四种为Incremental Low Pause GC，适用于要求缩短因GC造成程序停滞的时间。这种GC可以在Young区回收的同时，回收一部分Old区对象。-Xincgc参数启动该GC。
JVM参数配置
1: heap size 
a: -Xmx<n> 
指定 jvm 的最大 heap 大小 , 如 :-Xmx=2g 
b: -Xms<n> 
指定 jvm 的最小 heap 大小 , 如 :-Xms=2g ， 高并发应用， 建议和-Xmx一样， 防止因为内存收缩／突然增大带来的性能影响。 
c: -Xmn<n> 
指定 jvm 中 New Generation 的大小 , 如 :-Xmn256m。 这个参数很影响性能， 如果你的程序需要比较多的临时内存， 建议设置到512M， 如果用的少， 尽量降低这个数值， 一般来说128／256足以使用了。 
d: -XX:PermSize=<n> 
指定 jvm 中 Perm Generation 的最小值 , 如 :-XX:PermSize=32m。 这个参数需要看你的实际情况，。 可以通过jmap 命令看看到底需要多少。 
e: -XX:MaxPermSize=<n> 
指定 Perm Generation 的最大值 , 如 :-XX:MaxPermSize=64m 
f: -Xss<n> 
指定线程桟大小 , 如 :-Xss128k， 一般来说，webx框架下的应用需要256K。 如果你的程序有大规模的递归行为，请考虑设置到512K／1M。 这个需要全面的测试才能知道。 不过，256K已经很大了。 这个参数对性能的影响比较大的。 
g: -XX:NewRatio=<n> 
指定 jvm 中 Old Generation heap size 与 New Generation 的比例 , 在使用 CMS GC 的情况下此参数失效 , 如 :-XX:NewRatio=2 
h: -XX:SurvivorRatio=<n> 
指 定 New Generation 中 Eden Space 与一个 Survivor Space 的 heap size 比例 ,-XX:SurvivorRatio=8, 那么在总共 New Generation 为 10m 的情况下 ,Eden Space 为 8m 
i: -XX:MinHeapFreeRatio=<n> 
指定 jvm heap 在使用率小于 n 的情况下 ,heap 进行收缩 ,Xmx==Xms 的情况下无效 , 如 :-XX:MinHeapFreeRatio=30 
j: -XX:MaxHeapFreeRatio=<n> 
指定 jvm heap 在使用率大于 n 的情况下 ,heap 进行扩张 ,Xmx==Xms 的情况下无效 , 如 :-XX:MaxHeapFreeRatio=70 
k: -XX:LargePageSizeInBytes=<n> 
指定 Java heap 的分页页面大小 , 如 :-XX:LargePageSizeInBytes=128m 
2: garbage collector 
a: -XX:+UseParallelGC 
指 定在 New Generation 使用 parallel collector, 并行收集 , 暂停 app threads, 同时启动多个垃圾回收 thread, 不能和 CMS gc 一起使用 . 系统吨吐量优先 , 但是会有较长长时间的 app pause, 后台系统任务可以使用此 gc 
b: -XX:ParallelGCThreads=<n> 
指定 parallel collection 时启动的 thread 个数 , 默认是物理 processor 的个数 , 
c: -XX:+UseParallelOldGC 
指定在 Old Generation 使用 parallel collector 
d: -XX:+UseParNewGC 
指定在 New Generation 使用 parallel collector, 是 UseParallelGC 的 gc 的升级版本 , 有更好的性能或者优点 , 可以和 CMS gc 一起使用
e: -XX:+CMSParallelRemarkEnabled 
在使用 UseParNewGC 的情况下 , 尽量减少 mark 的时间 
f: -XX:+UseConcMarkSweepGC 
指 定在 Old Generation 使用 concurrent cmark sweep gc,gc thread 和 app thread 并行 ( 在 init-mark 和 remark 时 pause app thread). app pause 时间较短 , 适合交互性强的系统 , 如 web server 
g: -XX:+UseCMSCompactAtFullCollection 
在使用 concurrent gc 的情况下 , 防止 memory fragmention, 对 live object 进行整理 , 使 memory 碎片减少 
h: -XX:CMSInitiatingOccupancyFraction=<n> 
指示在 old generation 在使用了 n% 的比例后 , 启动 concurrent collector, 默认值是 68, 如 :-XX:CMSInitiatingOccupancyFraction=70 
i: -XX:+UseCMSInitiatingOccupancyOnly 
指示只有在 old generation 在使用了初始化的比例后 concurrent collector 启动收集 
3:others 
a: -XX:MaxTenuringThreshold=<n> 
指 定一个 object 在经历了 n 次 young gc 后转移到 old generation 区 , 在 linux64 的 java6 下默认值是 15, 此参数对于 throughput collector 无效 , 如 :-XX:MaxTenuringThreshold=31 
b: -XX:+DisableExplicitGC 
禁止 java 程序中的 full gc, 如 System.gc() 的调用. 最好加上么， 防止程序在代码里误用了。对性能造成冲击。 
c: -XX:+UseFastAccessorMethods 
get,set 方法转成本地代码 
d: -XX:+PrintGCDetails 
打应垃圾收集的情况如 : 
[GC 15610.466: [ParNew: 229689K->20221K(235968K), 0.0194460 secs] 1159829K->953935K(2070976K), 0.0196420 secs] 
e: -XX:+PrintGCTimeStamps 
打应垃圾收集的时间情况 , 如 : 
[Times: user=0.09 sys=0.00, real=0.02 secs] 
f: -XX:+PrintGCApplicationStoppedTime 
打应垃圾收集时 , 系统的停顿时间 , 如 : 
Total time for which application threads were stopped: 0.0225920 seconds 
JVM参数设置大全 
[http://java.sun.com/javase/technologies/hotspot/vmoptions.jsp](http://java.sun.com/javase/technologies/hotspot/vmoptions.jsp)
JVM参数设置大全压缩版 
[http://blogs.sun.com/watt/resource/jvm-options-list.html](http://blogs.sun.com/watt/resource/jvm-options-list.html)
