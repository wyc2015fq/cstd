# Tomcat jvm启动命令之参数优化 - 零度的博客专栏 - CSDN博客
2017年02月27日 15:55:39[零度anngle](https://me.csdn.net/zmx729618)阅读数：916
                
Xms：set initial Java heap size，设置JVM初始化堆内存大小，设置虚拟机可用内存堆的初始大小，缺省单位为字节，该大小为 1024 的整数倍并且要大于1MB，可用 k(K)或m(M)为单位来设置较大的内存数。初始堆大小为 2MB。 
Xmx：set maximum Java heap size，设置JVM最大的堆内存大小，设置虚拟机内存堆的最大可用大小，缺省单位为字节。该值必须为 1024 整数倍，并且要大于 2MB。可用 k(K)或 m(M)为单位来设置较大的内存数。缺省堆最大值为 64MB。 
Xss：设置JVM栈内存大小，设置线程栈的大小，缺省单位为字节。与-Xmx 类似，也可用 K 或 M 来设置较大的值。通常，操作系统分配给线程栈的缺省大小为 1MB。 另外也可在 java 中创建线程对象时设置栈的大小，构造函数原型为 Thread(ThreadGroup group, Runnable target, String name, long stackSize)。
XX:PermSize：JVM初始分配的非堆内存
XX:MaxPermSize：JVM最大允许分配的非堆内存，按需分配
XX:NewSize：设置年轻代大小(for 1.3/1.4)
XX:MaxNewSize：年轻代最大值(for 1.3/1.4)
XX:SurvivorRatio：Eden区与Survivor区的大小比值
XX:+UseParNewGC：设置年轻代为并行收集
XX:ParallelGCThreads：并行收集器的线程数
XX:MaxTenuringThreshold：垃圾（回收对象）最大年龄
如果设置为0的话,则年轻代对象不经过Survivor区,直接进入年老代. 对于年老代比较多的应用,可以提高效率.如果将此值设置为一个较大值,则年轻代对象会在Survivor区进行多次复制,这样可以增加对象再年轻代的存活 时间,增加在年轻代即被回收的概率
该参数只有在串行GC时才有效.
XX:+DisableExplicitGC：关闭System.gc()
XX:+ScavengeBeforeFullGC：Full GC前调用YGC
XX:SoftRefLRUPolicyMSPerMB：为了避免Perm区满引起的full gc，每兆堆空闲空间中SoftReference的存活时间
XX:-ReduceInitialCardMarks：解决意思GC BUG
XX:+ExplicitGCInvokesConcurrent：当使用CMS收集器时，系统GC将是一件很不幸的事，因为它默认会触发一次Full GC。此参数命令JVM无论什么时候调用系统GC，都执行CMS GC，而不是Full GC；不是触发一个 完全stop-the-world的full GC，而是一次并发GC周期
Xloggc:/data/applogs/heap_trace.txt：设置gclog目录
设置gclog 格式：
-XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+PrintGCApplicationConcurrentTime -XX:+PrintHeapAtGC 
XX:-HeapDumpOnOutOfMemoryError ：表示当JVM发生OOM时，自动生成DUMP文件。
XX:HeapDumpPath：dump目录
XX:-OmitStackTraceInFastThrow：禁止jvm性能优化导致频闭异常日志。
CMS相关参数：
XX:+UseConcMarkSweepGC：使用CMS内存收集
XX:+UseCMSInitiatingOccupancyOnly：使用手动定义初始化定义开始CMS收集，禁止hostspot自行触发CMS GC
XX:+UseCMSCompactAtFullCollection ：在FULL GC的时候， 对年老代的压缩
XX:+CMSParallelRemarkEnabled：降低标记停顿
XX:CMSFullGCsBeforeCompaction：多少次GC后进行内存压缩，由于并发收集器不对内存空间进行压缩,整理,所以运行一段时间以后会产生"碎片",使得运行效率降低.此值设置运行多少次GC以后对内存空间进行压缩,整理.
XX:CMSInitiatingOccupancyFraction：默认CMS是在tenured generation沾满68%的时候开始进行CMS收集，如果你的年老代增长不是那么快，并且希望降低CMS次数的话，可以适当调高此值.
XX:+CMSClassUnloadingEnabled：为了避免Perm区满引起的full gc，建议开启CMS回收Perm区选项
XX:+CMSPermGenSweepingEnabled：为了避免Perm区满引起的full gc，建议开启CMS回收Perm区选项
XX:CMSInitiatingPermOccupancyFraction：设置Perm Gen使用到达多少比率时触发，同XX:CMSInitiatingOccupancyFraction
CATALINA_OPTS="$CATALINA_OPTS -server -Djava.awt.headless=true -Xms2560m -Xmx2560m -Xss512k -XX:PermSize=128m -XX:MaxPermSize=384m 
-XX:NewSize=1024m -XX:MaxNewSize=1024m -XX:SurvivorRatio=22 -XX:+UseParNewGC -XX:ParallelGCThreads=4 -XX:MaxTenuringThreshold=9 
-XX:+UseConcMarkSweepGC -XX:+DisableExplicitGC -XX:+UseCMSInitiatingOccupancyOnly -XX:+ScavengeBeforeFullGC -XX:+UseCMSCompactAtFullCollection 
-XX:+CMSParallelRemarkEnabled -XX:CMSFullGCsBeforeCompaction=9 -XX:CMSInitiatingOccupancyFraction=60 -XX:+CMSClassUnloadingEnabled 
-XX:SoftRefLRUPolicyMSPerMB=0 -XX:-ReduceInitialCardMarks -XX:+CMSPermGenSweepingEnabled -XX:CMSInitiatingPermOccupancyFraction=70 
-XX:+ExplicitGCInvokesConcurrent -Djava.nio.channels.spi.SelectorProvider=sun.nio.ch.EPollSelectorProvider 
-Djava.util.logging.manager=org.apache.juli.ClassLoaderLogManager -Djava.util.logging.config.file="%CATALINA_HOME%\conf\logging.properties" 
-XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+PrintGCApplicationConcurrentTime -XX:+PrintHeapAtGC 
-Xloggc:/data/applogs/heap_trace.txt -XX:-HeapDumpOnOutOfMemoryError 
-XX:HeapDumpPath=/data/applogs/HeapDumpOnOutOfMemoryError -XX:-OmitStackTraceInFastThrow"
            
