# Tomcat 性能调优之 JVM 调优 - 纯洁的微笑 - CSDN博客
2019年01月22日 08:41:21[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：99
Tomcat、Jetty、GlassFish 等等这系列 Web容器/应用服务器，虽然做为容器，提供的是一个 Java Web 的运行时环境，以支持Servlet/JSP 等等这些内容的运行，但我们都很清楚，其本质上还是一个 Java 应用程序。 每次对于 容器的启动运行，都是把这个 Java 程序跑起来，来实现 Web 容器的能力。
做为一类“**特殊**”的 Java 应用程序，和任务其他的 Java 应用一样，需要使用到JVM，会有堆，会使用到垃圾回收，会涉及到不同的堆分区比例...  
因此在对Web 容器( 应用服务器) 的调优中必不可少的是对于 JVM 的调优。
对于 JVM 的调优，主要有两个方面考虑：
- 
内存大小配置
- 
垃圾回收算法选择
当然，确切的说，以上两点并不互相独立，内存的大小配置也会影响垃圾回收的执行效率。
其中内存大小配置，最主要做的有
- 
确定内存占用的总大小
- 
确定内存中各个代(Gen) 的大小划分
**内存大小配置**
所谓内存大小的占用，是指应用程序启动后稳定运行一小段时间时，观察到的内存占用情况。
以 HotSpot 虚拟机为例，Java 堆主要有三个空间：
新生代、老年代和永久代。 
根据不同应用的特别，观察应用对于内存的占用，如果有大量的临时对象，不会重复使用，则可以调整 New Gen， 这样这些临时对象就在新生代创建完成，并在 Minor GC 产生时被回收，这样较短生存活的对象不会晋升到老年代，从而可以避免垃圾堆集产生 Full GC。 
理想状态下，短期存活的对象都只在新生代完成生命周期，被费时劲少的 
Minor GC  回收完成， 而长期存活，将会多次使用的在多次回收之后晋升到老年代， 最终经过 Full GC 完成生命周期。
这里涉及到关于内存大小的调整参数有：
- 
-Xms
- 
-Xmx
这两个参数用于配置 **heap** 的起始大小和最大值。这里需要经过观察，找一个合适的值，设置太大会导致内存浪费，同时也会导致垃圾回收耗时太长。对于 Tomcat 来说，一般都会将初始值和最大值设置为相同值，这样就避免在初始内存不足时触发 Full GC 来进行扩展内存。
设定 heap 大小之后，要根据对象生命周期的特征，来调整**新生代**与**老年代**的大小比例。
涉及到的参数有：
- 
-XX:NewSize
- 
-XX:NewRatio
- 
-XX:MaxNewSize
- 
-Xmn
第一个是直接设置新生代初始大小，第二个是设置比例(Ratio)。太高或太低都会导致 GC 不能高效的工作。毕竟 Minor GC 也是要耗时的。最后一个设置新生代的初始值和最大值相同，堆空间的变化不影响其值。
对于使用了大量第三方类库的应用来说，会加载许多框架依赖的类，使用过程中可能会遇到因为Perm Gen 不足产生的 OOM，这种情况可以通过观察稳定状态下 Perm 区的占用，再通过参数设置。
- 
-XX:PermSize
- 
-XX:MaxPermSize
- 
-XX:MaxMetaspaceSize
第一个会设置Perm区的初始大小，第二个用于设置Perm 区的最大值。在Java 8的时候， Perm 区被移除，改为Metaspace，不过如果遇到类似的OOM,依然可以调整其大小。
此外，对于使用大量线程的应用，也可以配置 -Xss，主要用于**设置单个线程的stack 大小**。注意，是单个的大小，因此设置值越大，会占用越大，可用的线程数也就越少。
这里的配置一般对于-X开始的可以直接在后面用数字加单位，而-XX的则需要等号后数字再加单位，例如：
java -Xms100m -Xmx200m -XX:PermSize=300m
这里数字后的单可以是m,g,k代表计算机中的不同单位。
那我们前面一直在说根据不同的应用，观察分析设置堆的大小，堆的各个代的大小，那具体观察什么呢？
我们一般在JVM的配置中增加一些打印 GC 日志的选项，配置方式和上面的类似，这样在 GC 产生时，会打印出各个代占用的大小，具体触发时间等。推荐的配置有以下几个：
- 
-XX:+PrintGCTimeStamps
- 
-XX:+PrintGCDetails
- 
-Xloggc:<文件名>
- 
-XX:PrintGCDateStamps
第一个和第四个选项可以任选一个，第一个打印自JVM启动以来的时间，一般也称为uptime, 第四个打印的是系统当前日期和时间。
根据 GC 日志产生的内容，来观察具体的大小，开始使用上述的配置参数进行调整。当然，也可以用 JConsole, JVisual VM 这些工具可视化的进行了解再调整。
**垃圾回收算法**
不同的垃圾回收算法，对于应用的影响很大。一方面可能在一个服务器上却使用了单线程的回收算法，也可能应用对于响应要求很高，但却使用了一个吞吐量优先的算法，导致响应太慢。
所以对于垃圾回收算法的选择，一般都是根据应用的特点，是要低延迟还是高吞吐量，选择合适的算法。我们前面也提到，垃圾回收算法和内存的大小配置并非独立的，内存设置大是回收的频率会降低，但每次的执行时间也会变长。所以这里也是一个需要权衡的地方。
- 
延迟、吞吐量调优
- 
其他 JVM 配置
垃圾回收算法对应到的就是不同的垃圾收集器，具体到在 JVM 中的配置，是使用 -XX:+UseParallelOldGC 或者 -XX:+UseConcMarkSweepGC 这种不同的收集器来达到选择算法的目的。
其中 ParallelGC 也称为 吞吐量优先收集器，可以提升应用的吞吐量，但在老年代大小调整之，进行几次垃圾回收后，不能满足应用的低延迟要求。
一般常用到ConcMarkSweepGC， 也称之为 CMS GC，其可以做到老年代的垃圾回收与应用程序的纯种并行执行，所以可以实现低延迟。
这里注意，由于 CMS GC 和其他GC回收算法使用的框架不同，因此不能混用，在使用CMS 进行老年代回收时，新生代默认使用了单线程回收算法，此时可以通过配置 -XX:+UseParNewGC来使用 新生代并行回收。
由于CMS是垃圾回收和应用线程并行，因此需要额外的CPU处理资源，如果只有一个CPU的机器，或者有多个忙碌的CPU，又想要使用低延迟的收集器，此时可以通过配置 CMS 收集器的增量模式来进行回收，通过指定 -XX:+CMSIncrementalMode 来开启增量模式。此时交替运行垃圾收集器应用线程。通过配置 
-XX:CMSIncrementalSafetyFactor=X， -XX:CMSIncrementalDutyCycleMin=Y,
-XX:CMSIncrementalPacing 可以控制垃圾收集后台线程为应用线程让出多少CPU周期。
参数-XX:+CMSParallelRemarkEnabled  用来降低标记停顿，另外由于CMS 回收后的老年代内存空间并不是连续的，因此通过参数
-XX:+UseCMSCompactAtFullCollection 在Full GC的时候对年老代的压缩。
在JDK1.7 的时候引入了 G1 收集器，可以通过配置-XX:+UseG1GC  来开启。这一方面的实战经验不多，有相关使用经验的朋友欢迎分享。
此外，还可以对新生代进行更细致的配置，比如设置Eden 和 Suvivor 区的比例等，和Newxx类似，可以通过SuvivorRation设置比例。
其他 JVM 配置
可以使用 -XX:+DisableExplicitGC 选项来禁止显式的 System.gc 的调用。这个使用时需要评估后再使用。
**所谓调优，就是一个不断调整和优化的过程，需要观察、配置、测试再如此重复**。有相关经验的朋友欢迎留言补充！
说到底，那上面的这些选项是要配置在哪里呢？ 我们前面提到 Tomcat 本质也是个普通的 Java 应用，因此和一般的 Java 启动方式类似，也是类似 
java -Xms100m -XX:+UseParallelOldGC  应用主类
通过这种形式来启动，区别只是 Tomcat 将上述命令放到了文件中，对应到不同的操作系统，Windows下使用 bat文件， Linux下使用 sh 文件。
所以我们的配置项也是加到这些文件中。
我们来看catalina.sh中实际启动时执行的命令：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/8yRv8Dibia2sreIK9TDa1MOMzH6lMF7rkiaXzK3vatibSQ91zgziaKdca61CAN7NfeDwnGTYO49FiaicyP55vZicJ4MYlw/640?wx_fmt=png)
所以我们的选项可以加到 
JAVA_OPTS
CATALINA_OPTS
这些可选项中。
配置比较简单，例如下面这样：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/8yRv8Dibia2sreIK9TDa1MOMzH6lMF7rkiaRG86XKAmAfHXgY3j9mia7h0icGpfPic4nibrRKRZcNGYG69HlgYs4fzeXQ/640?wx_fmt=png)
配置的时候需要特别注意的是，不要把前面已经有的配置冲掉，比如
在配置JAVA_OPTS的时候，要把前面已经配置的加上，写起来是这样：
JAVA_OPTS="**$JAVA_OPTS**  新增的内容"
-END-
**纯洁的微笑**
**一个有故事的程序员**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqjV7GOKB2htgfZjgMjqxftxfmmdrLiaMKpyicTmLLX5fUjb6YxA6Z5Bhcozb3p0uMV7wqdKED89HZA/640?wx_fmt=jpeg)
