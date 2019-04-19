# Java性能优化哪些事 - 零度的博客专栏 - CSDN博客
2015年08月26日 11:12:41[零度anngle](https://me.csdn.net/zmx729618)阅读数：1159
性能优化主要目的：
    性能优化主要用于改善应用的“吞吐量”和“延迟”。
java性能优化的步骤：
    1、性能监控：通常是指一种在生产、质量评估、开发环境中实施的带有预防或主动性的非侵入活动。
    2、性能分析：是一种以侵入方式收集运行性能数据的活动。
    3、性能调优：是一种为改善应用响应性或吞度量而更改参数、源代码、或者属性配置活动。
名词解释：
    用户态CPU使用率：执行应用程序代码的时间占总CPU时间的百分比，一个方法运行于操作系统内核之外所消耗的CPU时间
    系统态CPU使用率：应用执行操作系统调用时间占总CPU时间的百分比，操作系统代表该方法运行所消耗的时间，提高应用性能和扩展性的目标是尽可能降级系统态CPU使用率
    CUP指令周期：运行一个指令的时间
    页面交换：应用运行所需的内存超过可用物理内存时，操作系统会将应用的一部分置换到磁盘上。
    MinorGC：新生代垃圾收集，收集新生代。
    FullGC： 老年代垃圾收集，收集整个JVM堆内存，包括新生代、老年代、永久代。FullGC之后新生代Eden为空、老年代和永久代也已压缩整理并且只有活跃对象。老年代或永久代的占用量接近其容量时都会触发FullGC
   内存泄露：如果一个永远不再使用的对象由于等待一个或者多个Java对象对它的引用而不能被垃圾收集，就称之为发生了内存泄露。
   基准测试：为了测量java应用程序运行期间一个或几个方面的性能所开发的java程序。
一、 性能监控
  1、Windows性能监控命令
    1、CPU使用效率
        使用typeperf监控 Privileged Time， User Time， Processor Time
        C:\Users\zhangw>typeperf "\Processor(_Total)\% Privileged Time" "\Processor(_Total)\% User Time" "\Processor(_Total)\% Processor Time"
    2、运行队列长度：通常不要长期超过虚拟处理器数量的3-4倍
       C:\Users\zhangw>typeperf "\System\Processor Queue Length"
    3、内存利用率--可用内存/页面交换、加锁、线程迁移
       C:\Users\zhangw>typeperf -si 5 "\Memory\Available Mbytes" "\Memory\Pages/sec"   --监控可用内存和每秒页面交换
    4、锁竞争
       需要借助外部工具如Intel VTune或AMD CodeAnalyst
  2、Linux性能监控命令
     1、CPU使用效率
        使用gnome-system-monitor监控
        使用命令： vmstat   其中：us sy id分别对应 User Time，Privileged Time，Processor Time
        使用命令： top
     2、运行队列长度
        使用命令： vmstat   其中：r代表队列长度
     3、内存利用率
        使用命令： vmstat   其中：free代表可用的空闲内存，si和so分别代表内存页面换入和换出
    4、锁竞争（多线程）---让步时钟周期占用超过5%的时钟周期，说明java应用正面临锁竞争
       使用命令： pidstat -w -I -p 9391 5   每5秒监控进程id为9391的java应用的让步上下文切换
       让步时钟周期= cswch/s * 80000/处理器内核数/处理器时钟频率
    5、磁盘I/O使用率
       使用命令：iostat
  3、主要性能参数：
      -client/-server   启动哪个JIT编译器 
      -XX: +UseCompressOops    开启压缩指针
      -XX: +PrintCommandLineFlags -version  打印JVM默认设置的的优化值
      -Xmn、-XX:NewSize、-XX:MaxNewSize、-XX:NewRatio、 -XX:SurvivorRatio  调整新生代参数
      -verbose:gc -XX: +PrintGCDetails -XX: +PrintGCTimeStamps  打印GC信息
      -XX:PermSize  -XX:MaxPermSize    调整永久代参数
   4、JVM性能监控：
      1、JVM监控范围：垃圾收集、JIT编译、类加载。
      2、垃圾收集
         2.1  垃圾收集数据包括：
              1、当前使用的垃圾收集器
              2、Java堆的大小
              3、新生代和老年代的大小
              4、永久代的大小
              5、MinorGC的持续时间 
              6、MinorGC的频率
              7、MinorGC的空间回收量
              8、FullGC的持续时间
              9、FullGC的频率
              10、每个并发垃圾收集周期内的空间回收量
              11、垃圾收集前后新生代和老年代的占用量
              12、垃圾收集前后永久代的占用量
              13、是否老年代或永久代的占用触发了FullGC
              14、应用是否显式调用了System.gc()
         2.2  监控垃圾收集的推荐选项
              -XX: +PrintGCDetails
              -XX: +PrintGCTimeStamps
         2.3  主要监控工具
              Java VisualVM  JConsole  VisualGC (均适用于Windows,Linux)
     3、JIT编译
         3.1  JIT编译数据包括
              1、CPU周期
              2、内存
              3、编译时间
         3.2  监控JIT编译的推荐选项
              -XX: +PrintCompilation
         3.3  主要监控工具
              Java VisualVM  JConsole  VisualGC (均适用于Windows,Linux)
     4、类加载
        4.1  类加载数据包括
             1、类加载数量
             2、类卸载数量
        4.2  监控类加载的推荐选项
        4.3  主要监控工具
             Java VisualVM    VisualGC (均适用于Windows,Linux)
二、JVM性能分析：
      分析指标：用户态CPU时间、系统态CPU时间、锁竞争
三、JVM性能调优
    （1）性能需求：可用性、可管理性、启动时间、内存消耗、吞吐量、响应时间    
         1、可用性：应用程序处于可操作、可使用状态的度量。
         2、可管理性：是对由运行、监控应用程序而产生的操作性开销的度量同时也包括配置应用程序的难易程度。
         3、吞吐量：单位时间内处理工作量的度量。
         4、延迟及响应性：应用程序收到指令开始工作直到完成工作所消耗的时间。
         5、内存消耗：内存占有的度量。
         6、启动时间：应用程序初始化所消耗的时间。
   （2）由于各个指标都是互斥的，因此性能调优过程的第一步是划分应用程序的系统需求优先级。
   （3）优化过程：
        1、选择JVM部署模式：
           （1） 单JVM部署模式：降低管理成本、减少内存开销、存在单点故障无法保证程序可用性。
           （2） 多JVM部署模式： 更高可用性、降低延迟、提高吞吐量、增加了管理代价。
        2、选择JVM启动模式：
           （1） Client模式：启动速度快、占用内存少、JIT编译速度快
           （2）Server模式：提供了更复杂的生成码优化功能，
        3、32/64位JVM：
           （1）HotSpot默认使用32位，使用32位还是64位JVM是由应用程序的内存占用决定的，同时考虑第三方库是否支持64位。
           （2）在64位JVM中，使用JNI的所有本地组件都必须用64位模式编译。
        4、选择初始垃圾收集器：JVM提供多个垃圾收集器如:Serial收集器、Throughput收集器、Mostly-Concurrent收集器、G1收集器
           （1）使用 -XX:+UseParallelOldGC 或 -XX:+UseParallelGC命令启动Throughput收集器。两个命令的区别：-XX:+UseParallelOldGC同时启用多线程新生代垃圾收集器和多线程老年代垃圾收集器；-XX:+UseParallelGC仅仅启用多线程新生代垃圾收集器，老年代仍采用单线程。
           （2）使用 -XX:+
        5、垃圾收集调优
            1、性能属性： 吞吐量、延迟、内存占用
               吞吐量: （不考虑垃圾收集引起的停顿时间或内存消耗）垃圾收集器能支撑应用程序达到的最高性能指标
               延迟: 垃圾收集所引起的停顿。
               内存占用：垃圾收集器流畅运行所需要的内存数量。
            2、调优原则：MinorGC回收原则、GC内存最大化原则、GC调优的3选2原则
            3、GC日志相关的命令行选项
               -XX:+PrintGCTimeStamps -XX:+PrintGCDetails -Xloggc:<filename>
            4、 获得应用程序由于执行VM安全点操作而阻塞的时间命令选项
                -XX:+PrintGCApplicationStoppedTime
                -XX:+PrintGCApplicationConcurrentTime
               安全点操作：使用JVM进入到一种状态，即所有Java应用线程都被阻塞且不能修改java堆，执行本地代码的线程都被禁止返回VM执行Java代码
            5、确定内存占用
               活跃数据的大小: 应用程序稳定运行时长期存活对象所占用的java堆内存量，即FullGC之后java堆所占用的空间大小
               -Xms   :堆的初始值和最小值
               -Xmx   :堆区的最大值
               如果关注吞吐量和延迟的应用程序应该将-Xms和-Xmx设定为相同，因为堆区动态扩展会进行FullGC，设定为同一个值尽量减少堆区动态扩展。 如果-Xms和-Xmx设定为相同，可以通过-Xmn设置不能动态扩展的新生代空间。    
            6、新生代    理想设置：应该设置新生代大小为老年代活跃数据的1-1.5倍 
               -XX:Newsize=<n>[g|m|k]      :设定新生代空间初始值大小
               -XX:MaxNewsize=<n>[g|m|k]   :设定新生代空间最大值
            7、老年代  　理想设置：应该设置老年代大小为老年代活跃数据的大小的3－4倍  
               老年代最小值 = -Xmx的值 - -XX:MaxNewsize
               老年代最大值 = -Xmx的值 - -XX:Newsize
            8、永久代    理想设置：应该设置永久代大小为永久代活跃数据的大小的1.5倍   
               -XX:PermSize=<n>[g|m|k]       :永久代初始值和最小值
               -XX:MaxPermSize=<n>[g|m|k]    :永久代最大值
            9、何时触发垃圾收集
               新生代、老年代、永久代这三个空间中的任何一个不能满足内存分配请求时，就会发生垃圾收集。
            10、活跃数据大小
                应用程序运行稳定态时，活跃数据大小=老年代占用大小 + 永久代占用大小，可以使用JVM监控工具VisualVM或JConsole等触发一次FullGC进行计算，（明智的做法多进行几次FullGC求解平均值）
            11、整个JVM堆区    理想设置： 整个JVM堆区的初始值和最大值大小应该设置为为活跃数据大小的3-4倍
            12、其他考量：Java堆区大小并不等于java应用程序占用的物理机的总内存，java应用程序的内存使用情况使用操作系统工具：prstat/top/任务管理器。 线程栈也可能需要较多的内存，第三方库分配内存、I/O缓存都可以导致使用较多的内存
       6、应用程序延迟/响应调优
            1、响应调优需求    
               1、应用程序可接受的平均停滞时间；
               2、可接受的MinorGC频率； 
               3、用户可接受的应用程序最大停顿时间；
               4、用户可接受的最大停顿发生的频率；
            2、新生代
               MinorGC的持续时间和频率可以确定新生代空间的大小，如果MinorGC的频率大于应用程序的延迟频率（发生的太频繁）增大新生代空间；如果MinorGC持续时间太长，可以适当减小新生代空间
            3、老年代
               FullGC持续的时间和频率可以确定老年代空间的大小
            4、Survivor空间大小调优
               Survivor空间大小  =  -Xmn<value> / （-XX:SurvivorRatio=<ratio> + 2）
               -XX:MaxTenuringThreshold=<n>  设置新生代到老年代的晋升阈值
               如果Survivor空间大小小于总的存活对象大小或者观察到新晋升阈值持续小于最大晋升阈值 都需要增大器容量，增大Survivor空间需要保持Eden空间不变，因此需要增大整个新生代区间大小。
           5、CMS收集周期调优
              成功的CMS收集器调优要能以对象从新生代提升到老年代的同等速度对老年代中的对象进行垃圾收集。
