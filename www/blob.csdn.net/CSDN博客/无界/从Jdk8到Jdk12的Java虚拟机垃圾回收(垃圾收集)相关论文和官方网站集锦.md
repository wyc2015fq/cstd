
# 从Jdk8到Jdk12的Java虚拟机垃圾回收(垃圾收集)相关论文和官方网站集锦 - 无界 - CSDN博客

置顶2019年03月24日 03:00:23[21aspnet](https://me.csdn.net/21aspnet)阅读数：662



## JVM架构图
《[Java Program Execution Process in Detail | Working of JUST-IT-TIME Compiler (JIT) in Detail](https://www.youtube.com/watch?v=GXUiEouK7DM)》
![](https://img-blog.csdnimg.cn/20190328201024529.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
----
这张图微观画的很好：[https://www.javainterviewpoint.com/java-virtual-machine-architecture-in-java/](https://www.javainterviewpoint.com/java-virtual-machine-architecture-in-java/)
![](https://img-blog.csdnimg.cn/20190327105003158.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
这张图更宏观一点：[http://sunjava4all.blogspot.com/2012/01/jvm-architecture.html](http://sunjava4all.blogspot.com/2012/01/jvm-architecture.html)
![](https://img-blog.csdnimg.cn/20190327105014178.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
这张图综合了上面：[https://www.quora.com/What-is-the-Java-virtual-machine-JVM](https://www.quora.com/What-is-the-Java-virtual-machine-JVM)
![](https://img-blog.csdnimg.cn/2019032711020211.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
有个老外的视频感兴趣可以看看：[https://www.youtube.com/watch?v=dncpVFP1JeQ](https://www.youtube.com/watch?v=dncpVFP1JeQ)
![](https://img-blog.csdnimg.cn/20190327105228290.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
## JVM分代这张图画的很清晰
出自:[http://jprante.github.io/2012/11/28/Elasticsearch-Java-Virtual-Machine-settings-explained.html](http://jprante.github.io/2012/11/28/Elasticsearch-Java-Virtual-Machine-settings-explained.html)
![](https://img-blog.csdnimg.cn/20190325173521122.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

## JDK12的JVM选项
## Java虚拟机技术概述
本章介绍Java虚拟机（JVM）的实现以及Java HotSpot技术的主要功能：
自适应编译器：标准解释器用于启动应用程序。应用程序运行时，将分析代码以检测性能瓶颈或热点。Java HotSpot VM编译代码的性能关键部分以提高性能，但不编译很少使用的代码（大多数应用程序）。Java HotSpot VM使用自适应编译器来决定如何使用内联等技术优化编译代码。
快速内存分配和垃圾收集：Java HotSpot技术为对象和快速，高效，最先进的垃圾收集器提供快速内存分配。
线程同步：Java HotSpot技术提供了一种线程处理功能，旨在扩展以用于大型共享内存多处理器服务器。
在Oracle Java Runtime Environment（JRE）8及更早版本中，JVM的不同实现（客户端VM，服务器VM和最小VM）支持常用作客户端，服务器和嵌入式系统的配置。由于大多数系统现在可以利用服务器VM，因此Oracle Java运行时环境（JRE）9仅提供该VM实现。
## [Java虚拟机指南](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSJVM-GUID-982B244A-9B01-479A-8651-CB6475019281)
![](https://img-blog.csdnimg.cn/20190325144714754.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
## [垃圾收集调整](https://www.oracle.com/pls/topic/lookup?ctx=javase12&id=JSGCT-GUID-326EB4CF-8C8C-4267-8355-21AB04F0D304)
## 垃圾收集
Oracle的HotSpot VM包含几个垃圾收集器，可用于帮助优化应用程序的性能。如果您的应用程序处理大量数据（多个千兆字节），具有多个线程并具有高事务率，则垃圾收集器尤其有用。
有关可用垃圾收集器的说明，请参阅Java平台标准版HotSpot虚拟机垃圾收集调整指南中的[垃圾收集实现](https://www.oracle.com/pls/topic/lookup?ctx=en/java/javase/12/vm&id=JSGCT-GUID-23844E39-7499-400C-A579-032B68E53073)。
![](https://img-blog.csdnimg.cn/20190325144018931.PNG)
### G1与其他收集器(CMS)的比较
这是G1和其他收集器之间主要差异的总结：
并行GC只能作为一个整体压缩和回收旧一代的空间。G1逐步将这项工作分散到多个更短的集合中。这大大缩短了暂停时间，可能会降低吞吐量。
与CMS类似，G1同时执行旧一代空间回收的一部分。但是，CMS无法对旧一代堆进行碎片整理，最终会遇到长的Full GC。
G1可能表现出比上述收集器更高的开销，由于其并发性质而影响吞吐量。
ZGC针对非常大的堆，旨在以更高的吞吐量成本提供显着更小的暂停时间。
**已经不推荐使用CMS推荐G1了**：
![](https://img-blog.csdnimg.cn/20190325144026990.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

## [Java虚拟机规范，Java SE 12 Edition](https://docs.oracle.com/javase/specs/jvms/se12/html/index.html)
![](https://img-blog.csdnimg.cn/20190325144321479.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

## JDK8的JVM选项
[Java虚拟机规范，Java SE 8 Edition](https://docs.oracle.com/javase/specs/jvms/se8/html/index.html)
![](https://img-blog.csdnimg.cn/20190325144452495.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[https://docs.oracle.com/javase/8/docs/technotes/tools/windows/toc.html](https://docs.oracle.com/javase/8/docs/technotes/tools/windows/toc.html)
![](https://img-blog.csdnimg.cn/20190325140430607.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
**JVM按以下顺序搜索并加载类：**
**Bootstrap类**，它们是构成Java平台的类，包括类rt.jar和其他几个重要的JAR文件。
**扩展类**，它使用Java扩展机制。这些类捆绑为JAR文件并位于extensions目录中。
**用户类**，是由开发人员和第三方定义的类，不利用扩展机制。您-classpath可以使用命令行（首选）上的选项或CLASSPATH环境变量来标识这些类的位置。请参阅设置类路径。
----------------------
![](https://img-blog.csdnimg.cn/20190325143701498.PNG)
[https://docs.oracle.com/javase/8/docs/technotes/tools/windows/java.html\#CBBIJCHG](https://docs.oracle.com/javase/8/docs/technotes/tools/windows/java.html#CBBIJCHG)
![](https://img-blog.csdnimg.cn/20190325140200664.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
下面选择一些**常用选项**，更多完整参数建议看官方链接：
**-XX：MaxGCPauseMillis =****时间**
设置**最大GC暂停时间**的目标（以毫秒为单位）。这是一个**软目标**，JVM将尽最大努力实现它。默认情况下，没有最大暂停时间值。
以下示例显示如何将最大目标暂停时间设置为500毫秒：
```python
-XX：MaxGCPauseMillis = 500
```
**-XX：MaxMetaspaceSize =****size**
设置可以为类**元数据分配的最大本机内存量**。**默认情况下，大小不受限制**。应用程序的元数据量取决于应用程序本身，其他正在运行的应用程序以及系统上可用的内存量。
以下示例显示如何将最大类元数据大小设置为256 MB：
```python
-XX：MaxMetaspaceSize =256M
```
**-XX：ParallelGCThreads =****threads**
设置在年轻和老一代中用于**并行垃圾收集的线程数**。默认值**取决于JVM可用的CPU数量**。
例如，要将并行GC的线程数设置为2，请指定以下选项：
```python
-XX：ParallelGCThreads = 2
```
**-XX：+ ScavengeBeforeFullGC**
在每个**完整GC之前启用年轻代的GC**。默认情况下启用此选项。Oracle建议您*不要*禁用它，因为在**完整GC之前清除年轻代可以减少从旧代空间到年轻代空间可到达的对象数**。要在每个完整GC之前禁用年轻代的GC，请指定`-XX:-ScavengeBeforeFullGC`。
**-XX：+ UseTLAB**
允许在**年轻代空间中使用线程局部分配块（TLAB）**。默认情况下启用此选项。要禁用TLAB，请指定`-XX:-UseTLAB`。
**-XX：TLABSize =****size**
设置**线程局部分配缓冲区（TLAB）的初始大小**（以字节为单位）。附加字母`k`或`K`表示千字节，`m`或`M`指示兆字节，`g`或`G`指示千兆字节。如果此选项设置为0，则JVM会自动选择初始大小。
以下示例显示如何将初始TLAB大小设置为512 KB：
```python
-XX：TLABSize = 512K
```
**-XX：+ UseG1GC**
**允许使用垃圾优先（G1）垃圾收集器**。它是一个服务器式垃圾收集器，针对具有大量RAM的多处理器计算机。它以高概率满足GC暂停时间目标，同时保持良好的吞吐量。G1收集器推荐用于需要大堆（大小约为6 GB或更大）且GC延迟要求有限的应用（稳定且可预测的暂停时间低于0.5秒）。
默认情况下，禁用此选项，并根据计算机的配置和JVM的类型自动选择收集器。
**XX：+ UseConcMarkSweepGC**
允许为**旧一代使用CMS垃圾收集器**。Oracle建议您在spam（`-XX:+UseParallelGC`）垃圾收集器无法满足应用程序延迟要求时使用CMS垃圾收集器。G1垃圾收集器（`-XX:+UseG1GC`）是另一种选择。
默认情况下，禁用此选项，并根据计算机的配置和JVM的类型自动选择收集器。启用此选项后，将`-XX:+UseParNewGC`自动设置该选项，您不应禁用该选项，因为JDK 8中已弃用以下选项组合：`-XX:+UseConcMarkSweepGC -XX:-UseParNewGC`。

**-XX：+ PrintGC**
允许在每个GC上**打印消息**。默认情况下，禁用此选项。
======================
## JDK7的JVM选项
[https://www.oracle.com/technetwork/articles/java/vmoptions-jsp-140102.html](https://www.oracle.com/technetwork/articles/java/vmoptions-jsp-140102.html)
![](https://img-blog.csdnimg.cn/20190325135500556.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
-------------------------------
[Java垃圾收集基础知识](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/gc01/index.html)
![](https://img-blog.csdnimg.cn/20190324161113880.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[G1垃圾收集器入门](https://www.oracle.com/webfolder/technetwork/tutorials/obe/java/G1GettingStarted/index.html)
![](https://img-blog.csdnimg.cn/20190324164724298.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
------------------------------
如果还看不明白这个说的比较清晰准确可以入门看看：
[JVM 垃圾回收机制](https://my.oschina.net/hosee/blog/644085)
![](https://img-blog.csdnimg.cn/20190324164024564.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190324164129264.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190324164137643.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[JVM GC参数以及GC算法的应用 ](https://my.oschina.net/hosee/blog/644618#OSC_h2_6)
![](https://img-blog.csdnimg.cn/20190324164256524.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190324164330956.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
上文同《[JVM中的G1垃圾回收器](http://www.importnew.com/15311.html)》
G1虽然保留了CMS关于代的概念，但是代已经不是物理上连续区域，而是一个逻辑的概念。在标记过程中，每个区域的对象活性都被计算，在回收时候，就可以根据用户设置的停顿时间，选择活性较低的区域收集，这样既能保证垃圾回收，又能保证停顿时间，而且也不会降低太多的吞吐量。Remark阶段新算法的运用，以及收集过程中的压缩，都弥补了CMS不足。引用Oracle官网的一句话：“G1 is planned as the long term replacement for the Concurrent Mark-Sweep Collector (CMS)”。
-----
[什么时候会发生FullGC](https://my.oschina.net/hosee/blog/674261)
![](https://img-blog.csdnimg.cn/20190324182514887.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
----------------------------
## [G1收集器与CMS收集器的对比与实战](http://blog.chriscs.com/2017/06/20/g1-vs-cms/)
CMS（Concurrent Mark and Sweep）是以牺牲吞吐量为代价来获得最短停顿时间的垃圾回收器，主要适用于对响应时间的侧重性大于吞吐量的场景。**仅针对老年代（Tenured Generation）的回收**。
为求达到该目标主要是因为以下两个原因：
没有采取compact操作，而是简单的mark and sweep，同时维护了一个free list来管理内存空间，所以也产生了大量的内存碎片。
mark and sweep分为多个阶段，其中大部分的阶段的GC线程是和用户线程并发执行，默认的GC线程数为物理CPU核心数的1/4。
因为是并发地进行清理，所以必须**预留**部分堆空间给正在运行的应用程序，默认情况下在老年代使用了68%及以上的内存的时候就开始CMS。
### Promotion Failed
由于CMS没有任何的碎片整理机制，所以会产生大量的堆碎片。因此可能会发生即使堆的大小没有耗尽，但是从新生代晋升至老年代却失败的情况。此时会fallback为Serial Old从而引起一次full GC（会进行碎片整理）。可以增加老年代的大小和Survivor区的大小以减少full GC的发生。
### Concurrent Mode Failed
如果对象分配率高于CMS回收的效率，将导致在CMS完成之前老年代就被填满，这种状况成为“并发模式失败”，同样也会引起full GC。可以调节-XX:CMSInitiatingOccupancyFraction和新生代的堆大小。
[JVM: CMS过程中的promotion failure和concurrent mode failure有何区别，如何应对](https://segmentfault.com/q/1010000013977499)
## Garbage First - G1垃圾收集器
G1收集器（或者垃圾优先收集器）的设计初衷是为了尽量缩短处理超大堆时产生的停顿。在回收的时候将对象从一个小堆区复制到另一个小堆区，这意味着G1在回收垃圾的时候同时完成了堆的部分内存压缩，相对于CMS的优势而言就是内存碎片的产生率大大降低。
![](https://img-blog.csdnimg.cn/20190326101418327.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
heap被划分为一系列大小相等的“小堆区”，也称为region。每个小堆区（region）的大小为1~32MB，整个堆**大致**要划分出2048个小堆区。
与上一代的垃圾收集器一样在逻辑上被划分Eden、Survivor和老年代，但是各种角色的region个数都不是固定的。
### young GC
young GC主要是对Eden区进行GC，它在Eden空间耗尽时会被触发。在这种情况下，Eden空间存活的对象会被**撤离**（代表复制或者移动）到另外**一个或是多个**Survivor小堆区，如果Survivor空间不够，Eden空间的部分数据会直接晋升到年老代空间。Survivor区的数据移动到新的Survivor区中，也有部分数据晋升到老年代空间中。最终Eden空间的数据为空，GC停止工作，应用线程继续执行。
![](https://img-blog.csdnimg.cn/20190326101525411.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
本阶段如同老一代的垃圾收集器一般，会有Stop The World暂停。同时计算出Eden的大小和Survivor的大小，为下一次young GC做准备。Accounting信息会被保存用于计算大小。如目标暂停时间的东西会被纳入考虑范围当中。这种方法使得resize小堆区更加容易，使小堆区可以根据需要变得更大或者更小。
最后，存活的对象会被撤离至Survivor小堆区和老年代小堆区，如下图所示。
![](https://img-blog.csdnimg.cn/20190326101718617.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
最近被晋升至老年代的对象用深蓝色表示，Survivor小堆区用绿色表示。
简而言之，G1的young GC规范如下：
堆从一个单一的内存空间被划分为众多的小堆区（region）。
新生代的内存由一系列不连续的小堆区所组成。这使得在需要的时候更加容易进行resize。
young GC是一个STW事件，所有应用程序线程都会被暂停。
young GC会使用多线程并行执行。
存活的对象将会复制到新的Survivor小堆区或者老年代小堆区。
总结一下G1收集器对老年代的收集
并发标记阶段在应用程序运行时并发地计算活跃度信息
活跃度信息甄别出哪个小堆区是在撤离暂停时最适合回收的
重新标记阶段使用Snapshot-at-the-Beginning (SATB) 算法，这个算法比CMS所使用的要快得多
回收空的小堆区
复制/清除阶段新生代和老年代同时被回收
老年代的小堆区会根据活跃度而进行部分的选定

## 总结
如果是使用旧的老年代收集器，在面临超大堆的时候会显得力不从心，通常一次full GC就会暂停3s以上。CMS的出现真的是救星，如今G1也在蓬勃发展，在JDK9中成为默认的垃圾收集器。我们在上面对老年代收集进行测试时，的确发现了G1收集器相对于CMS是有优势的。但是如果我们并不是运行超大堆的Java程序，或者线上的CMS收集器已经运行得很好，我们就不必再迁移到G1了。
------------------------------
# [深入理解 Java G1 垃圾收集器](http://blog.jobbole.com/109170/)
# （优秀）
![](https://img-blog.csdnimg.cn/20190326102659462.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
第四阶段，G1（并发）收集器
G1收集器（或者垃圾优先收集器）的设计初衷是为了尽量缩短处理超大堆（大于4GB）时产生的停顿。相对于CMS的优势而言是内存碎片的产生率大大降低。
![](https://img-blog.csdnimg.cn/20190326102849736.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
**其次，G1将新生代，老年代的物理空间划分取消了。**
这样我们再也不用单独的空间对每个代进行设置了，不用担心每个代内存是否足够。
取而代之的是，G1算法将堆划分为若干个区域（Region），它仍然属于分代收集器。不过，这些区域的一部分包含新生代，新生代的垃圾收集依然采用暂停所有应用线程的方式，将存活对象拷贝到老年代或者Survivor空间。老年代也分成很多区域，G1收集器通过将对象从一个区域复制到另外一个区域，完成了清理工作。这就意味着，在正常的处理过程中，G1完成了堆的压缩（至少是部分堆的压缩），这样也就不会有cms内存碎片问题的存在了。
![](https://img-blog.csdnimg.cn/20190326103028413.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326103153439.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326103320278.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326103616896.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
需要注意的是，如果引用的对象很多，赋值器需要对每个引用做处理，赋值器开销会很大，为了解决赋值器开销这个问题，在G1 中又引入了另外一个概念，**卡表**（Card Table）。一个Card Table将一个分区在逻辑上划分为固定大小的连续区域，每个区域称之为卡。卡通常较小，介于128到512字节之间。Card Table通常为字节数组，由Card的索引（即数组下标）来标识每个分区的空间地址。默认情况下，每个卡都未被引用。当一个地址空间被引用时，这个地址空间对应的数组索引的值被标记为”0″，即标记为脏被引用，此外RSet也将这个数组下标记录下来。一般情况下，这个RSet其实是一个Hash Table，Key是别的Region的起始地址，Value是一个集合，里面的元素是Card Table的Index。
Young GC 阶段：
阶段1：根扫描
静态和本地对象被扫描
阶段2：更新RS
处理dirty card队列更新RS
阶段3：处理RS
检测从年轻代指向年老代的对象
阶段4：对象拷贝
拷贝存活的对象到survivor/old区域
阶段5：处理引用队列
软引用，弱引用，虚引用处理
**G1 Mix GC(新生代垃圾收集+回收老年代分区)**
Mix GC不仅进行正常的新生代垃圾收集，**同时**也回收部分后台扫描线程标记的老年代分区。
它的GC步骤分2步：
全局并发标记（global concurrent marking）
拷贝存活对象（evacuation）
在进行Mix GC之前，会先进行global concurrent marking（全局并发标记）。 global concurrent marking的执行过程是怎样的呢？
在G1 GC中，它主要是为Mixed GC提供标记服务的，并不是一次GC过程的一个必须环节。global concurrent marking的执行过程分为五个步骤：
初始标记（initial mark，STW）
在此阶段，G1 GC 对根进行标记。该阶段与常规的 (STW) 年轻代垃圾回收密切相关。
根区域扫描（root region scan）
G1 GC 在初始标记的存活区扫描对老年代的引用，并标记被引用的对象。该阶段与应用程序（非 STW）同时运行，并且只有完成该阶段后，才能开始下一次 STW 年轻代垃圾回收。
并发标记（Concurrent Marking）
G1 GC 在整个堆中查找可访问的（存活的）对象。该阶段与应用程序同时运行，可以被 STW 年轻代垃圾回收中断
最终标记（Remark，STW）
该阶段是 STW 回收，帮助完成标记周期。G1 GC 清空 SATB 缓冲区，跟踪未被访问的存活对象，并执行引用处理。
清除垃圾（Cleanup，STW）
在这个最后阶段，G1 GC 执行统计和 RSet 净化的 STW 操作。在统计期间，G1 GC 会识别完全空闲的区域和可供进行混合垃圾回收的区域。清理阶段在将空白区域重置并返回到空闲列表时为部分并发。
![](https://img-blog.csdnimg.cn/20190326104403157.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
------------------------------
## [使用CMS垃圾收集器产生的问题和解决方案](https://my.oschina.net/hosee/blog/674181)
首先我们经常遇到promotion failed问题，这也确实是个很头痛的问题，一般是进行Minor GC的时候，发现救助空间不够，所以，需要移动一些新生带的对象到老年带，然而，有些时候尽管老年带有足够的空间，但是由于CMS采用标记清除算法，默认并不使用标记整理算法，可能会产生很多碎片，因此，这些碎片无法完成大对象向老年带转移，因此需要进行CMS在老年带的Full GC来合并碎片。
这个问题的直接影响就是它会导致提前进行CMS Full GC, 尽管这个时候CMS的老年带并没有填满，只不过有过多的碎片而已，但是Full GC导致的stop-the-wold是难以接受的。
解决这个问题的办法就是可以让CMS在进行一定次数的Full GC（标记清除）的时候进行一次标记整理算法，CMS提供了以下参数来控制：
`-XX:UseCMSCompactAtFullCollection -XX:CMSFullGCBeforeCompaction=5`也就是CMS在进行5次Full GC（标记清除）之后进行一次标记整理算法，从而可以控制老年带的碎片在一定的数量以内，甚至可以配置CMS在每次Full GC的时候都进行内存的整理。
另外，有些应用存在比较大的对象朝生熄灭，这些对象在救助空间无法容纳，因此，会提早进入老年带，老年带如果有碎片，也会产生promotion failed, 因此我们应该控制这样的对象在新生代，然后在下次Minor GC的时候就被回收掉，这样避免了过早的进行CMS Full GC操作，下面的一个配置样例就通过增加救助空间的大小来解决这个问题：
`-Xmx4000M -Xms4000M -Xmn600M -XXmSize=500M -XX:MaxPermSize=500M -Xss256K -XX:+DisableExplicitGC -XX:SurvivorRatio=1 -XX:+UseConcMarkSweepGC -XX:+UseParNewGC -XX:+CMSParallelRemarkEnabled eCMSCompactAtFullCollection -XX:CMSFullGCsBeforeCompaction=0 -XX:+CMSClassUnloadingEnabled -XX:LargePageSizeInBytes=128M -XX:+UseFastAccessorMethods -XX:+UseCMSInitiatingOccupancyOnly -XX:CMSInitiatingOccupancyFraction=80 -XX:SoftRefLRUPolicyMSPerMB=0 -XX:+PrintClassHistogram -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+PrintHeapAtGC -Xloggc:log/gc.log`上面讨论了promotion failed引起的原因以及解决方案，除了promotion failed还有一个情况会引起CMS回收失败，从而退回到Serial Old收集器进行回收，我们在线上尤其要注意的是concurrent mode failure出现的频率，这可以通过-XX:+PrintGCDetails来观察，当出现concurrent mode failure的现象时，就意味着此时JVM将继续采用Stop-The-World的方式来进行Full GC，这种情况下，CMS就没什么意义了，造成concurrent mode failure的原因是当minor GC进行时，旧生代所剩下的空间小于Eden区域+From区域的空间，或者在CMS执行老年带的回收时有业务线程试图将大的对象放入老年带，导致CMS在老年带的回收慢于业务对象对老年带内存的分配。
解决这个问题的通用方法是调低触发CMS GC执行的阀值，CMS GC触发主要由CMSInitiatingOccupancyFraction值决定，默认情况是当旧生代已用空间为68%时，即触发CMS GC，在出现concurrent mode failure的情况下，可考虑调小这个值，提前CMS GC的触发，以保证旧生代有足够的空间。
**总结：**
1. promotion failed – concurrent mode failure
Minor GC后， 救助空间容纳不了剩余对象，将要放入老年带，老年带有碎片或者不能容纳这些对象，就产生了concurrent mode failure, 然后进行stop-the-world的Serial Old收集器。
解决办法：-XX:UseCMSCompactAtFullCollection -XX:CMSFullGCBeforeCompaction=5 或者 调大新生代或者救助空间
2. concurrent mode failure
CMS是和业务线程并发运行的，在执行CMS的过程中有业务对象需要在老年带直接分配，例如大对象，但是老年带没有足够的空间来分配，所以导致concurrent mode failure, 然后需要进行stop-the-world的Serial Old收集器。
解决办法：+XX:CMSInitiatingOccupancyFraction，调大老年带的空间，+XX:CMSMaxAbortablePrecleanTime
总结一句话：使用标记整理清除碎片和提早进行CMS操作。
----------------------------------------------------------------
[JVM性能优化，第1部分：JVM技术入门](https://www.javaworld.com/article/2078623/core-java-jvm-performance-optimization-part-1-a-jvm-technology-primer.html?page=2)
**[JVM性能优化，第2部分：编译器](https://www.javaworld.com/article/2078635/jvm-performance-optimization-part-2-compilers.html)**
**[JVM性能优化，第4部分：针对低延迟的C4垃圾收集......](https://www.javaworld.com/article/2078661/jvm-performance-optimization--part-4--c4-garbage-collection-for-low-latency-java-ap.html)**
**[JVM性能优化，第3部分：垃圾收集](https://www.javaworld.com/article/2078645/jvm-performance-optimization-part-3-garbage-collection.html)**
[http://ssw.jku.at/General/Staff/TW/igv.html](http://ssw.jku.at/General/Staff/TW/igv.html)
[IdealGraphVisualizer](http://ssw.jku.at/General/Staff/TW/igv.html)研究C2的实用工具之一
![](https://img-blog.csdnimg.cn/20190324022815781.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[使用 Ideal Graph Visualizer 分析编译代码过程 ](https://www.jianshu.com/p/2aa706b0fb66)
-------------------
[https://www.cs.kent.ac.uk/people/staff/rej/gcbib/](https://www.cs.kent.ac.uk/people/staff/rej/gcbib/)
![](https://img-blog.csdnimg.cn/20190324153936719.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
-----------------
[https://hllvm-group.iteye.com/group/topic/21468](https://hllvm-group.iteye.com/group/topic/21468)
![RednaxelaFX的博客](https://www.iteye.com/upload/logo/user/179842/e7f2a3da-4ed0-3627-ba26-c63a7a4f5f13-thumb.jpg?1260702218)
|[RednaxelaFX](https://rednaxelafx.iteye.com/)|2010-09-11
|
[关于VM的帖的目录](https://rednaxelafx.iteye.com/blog/362738)
------
Vladimir Ivanov讲解JIT编译器：
[JIT-compiler in JVM seen by a Java developer](http://www.slideshare.net/iwanowww/jitcompiler-in-jvm-by), Vladimir Ivanov, JavaOne 2013 Moscow, 2013
[https://wiki.openjdk.java.net/display/HotSpot/Garbage+Collection](https://wiki.openjdk.java.net/display/HotSpot/Garbage+Collection)
[[HotSpot VM] 请教G1算法的原理](https://hllvm-group.iteye.com/group/topic/44381)
至于G1的算法⋯大体概念其实还挺直观的？到底是哪里没明白？
从最高层看，G1的collector一侧其实就是两个大部分：
* 全局并发标记（global concurrent marking）
* 拷贝存活对象（evacuation）
而这两部分可以相对独立的执行。
Global concurrent marking基于SATB形式的并发标记。它具体分为下面几个阶段：
1、**初始标记（initial marking）**：暂停阶段。扫描根集合，标记所有从根集合可直接到达的对象并将它们的字段压入扫描栈（marking stack）中等到后续扫描。G1使用外部的bitmap来记录mark信息，而不使用对象头的mark word里的mark bit。在分代式G1模式中，初始标记阶段借用young GC的暂停，因而没有额外的、单独的暂停阶段。
2、**并发标记（concurrent marking）**：并发阶段。不断从扫描栈取出引用递归扫描整个堆里的对象图。每扫描到一个对象就会对其标记，并将其字段压入扫描栈。重复扫描过程直到扫描栈清空。过程中还会扫描SATB write barrier所记录下的引用。
3、**最终标记（final marking，在实现中也叫remarking）**：暂停阶段。在完成并发标记后，每个Java线程还会有一些剩下的SATB write barrier记录的引用尚未处理。这个阶段就负责把剩下的引用处理完。同时这个阶段也进行弱引用处理（reference processing）。
注意这个暂停与CMS的remark有一个本质上的区别，那就是这个暂停只需要扫描SATB buffer，而CMS的remark需要重新扫描mod-union table里的dirty card外加整个根集合，而此时整个young gen（不管对象死活）都会被当作根集合的一部分，因而CMS remark有可能会非常慢。
4、**清理（cleanup）**：暂停阶段。清点和重置标记状态。这个阶段有点像mark-sweep中的sweep阶段，不过不是在堆上sweep实际对象，而是在marking bitmap里统计每个region被标记为活的对象有多少。这个阶段如果发现完全没有活对象的region就会将其整体回收到可分配region列表中。

G1需要暂停来拷贝对象，而CMS在暂停中只需要扫描（mark）对象，那算法上G1的暂停时间会比CMS短么？
其实**CMS在较小的堆、合适的workload的条件下暂停时间可以很轻松的短于G1**。在**2011年**的时候Ramki告诉我堆大小的**分水岭大概在10GB～15GB左右：以下的-Xmx更适合CMS**，**以上的才适合试用G1**。现在到了**2014年，G1的实现经过一定调优**，大概在**6GB～8GB也可以跟CMS有一比**，我之前见过有在-Xmx**4g的环境里G1比CMS的暂停时间更短的案例。**
合适的workload：CMS最严重的暂停通常发生在remark阶段，因为它要扫描整个根集合，其中包括整个young gen。如果在CMS的并发标记阶段，mutator仍然在高速分配内存使得young gen里有很多对象的话，那remark阶段就可能会有很长时间的暂停。Young gen越大，CMS remark暂停时间就有可能越长。所以这是不适合CMS的workload。相反，如果mutator的分配速率比较温和，然后给足时间让并发的precleaning做好remark的前期工作，这样CMS就只需要较短的remark暂停，这种条件下G1的暂停时间很难低于CMS。
要在拷贝对象的前提下实现真正的**低延迟就需要做并发拷贝**（concurrent compaction）。但是现在已知的实现concurrent compaction的GC算法无一例外需要使用某种形式的**read barrier**，例如**Azul的C4**和Red Hat的Shenendoah。不用read barrier的话，没办法安全的实现一边移动对象一边修正指向这些对象的引用，因为mutator也可以会并发的访问到这些引用。
而G1则坚持只用write barrier不用read barrier，所以无法实现concurrent compaction。
[[资料] ](https://hllvm-group.iteye.com/group/forum?tag_id=689)[名词链接帖[占位]](https://hllvm-group.iteye.com/group/topic/21468)[https://rednaxelafx.iteye.com/](https://rednaxelafx.iteye.com/)
[新生代回收调试的一些心得](https://hllvm-group.iteye.com/group/topic/35798)[了解CMS GC日志](https://blogs.oracle.com/poonam/understanding-cms-gc-logs)
[输出Java程序| 第10集（垃圾收集）](https://www.geeksforgeeks.org/output-of-java-programs-set-10-garbage-collection/)
[如何使对象符合Java中的垃圾收集条件？](https://www.geeksforgeeks.org/how-to-make-object-eligible-for-garbage-collection/)
[如何防止Java中的垃圾收集类的对象](https://www.geeksforgeeks.org/how-to-prevent-objects-of-a-class-from-garbage-collection-in-java/)
[Mark-and-Sweep：垃圾收集算法](https://www.geeksforgeeks.org/mark-and-sweep-garbage-collection-algorithm/)
[Iterator vs Java中的集合](https://www.geeksforgeeks.org/iterator-vs-collection-in-java/)

## [选择合适的GC](https://dzone.com/articles/choosing-the-right-gc)
## G1垃圾收集器
Java生态系统的好消息是，从JDK 9开始，默认情况下启用现代缩小的G1垃圾收集器。如果使用较低版本的JDK，则可以使用该`-XX:+UseG1GC`参数启用G1  。
G1的主要优势之一是能够在没有冗长的暂停时间的情况下压缩可用内存空间。它也可以取消使用未使用的堆。我们发现此GC是在OpenJDK或HotSpot JDK上运行的Java应用程序的垂直扩展的最佳选择。
[垃圾收集器 - 串行与并行与CMS与G1（以及Java 8中的新功能） ](https://blog.overops.com/garbage-collectors-serial-vs-parallel-vs-cms-vs-the-g1-and-whats-new-in-java-8/)
### Java 8和PermGen
Java 8中最大的变化之一是[删除](http://java.dzone.com/articles/java-8-permgen-metaspace)传统上为类元数据，实体字符串和静态变量分配的堆的permgen部分。传统上，这需要开发人员使用能够加载大量类的应用程序（使用企业容器的应用程序常见的东西）来专门优化和调整堆的这一部分。多年来，这已成为许多OutOfMemory异常的来源，因此如果JVM（大多数情况下）是一个非常好的补充，那么让它（大多数情况下）要小心。即便如此，这本身可能不会减少开发人员将他们的应用程序分离到多个JVM的浪潮。
---------------
这个老外讲的很好《[Java Generics](https://www.youtube.com/watch?v=4ZO7uVon-kI)》
《[Java Stack + Heap with Reference & Instance Variables](https://www.youtube.com/watch?v=UcPuWY0wn3w)》
![](https://img-blog.csdnimg.cn/20190328195019243.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
垃圾收集器 - 串行与并行与CMS垃圾收集器 - 串行与并行与CMS与G1（以及Java 8中的新功能）与G1（以及Java 8中的新
![](https://img-blog.csdnimg.cn/20190325102946197.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190328185256578.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190328185416233.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[JVM GC 之「AdaptiveSizePolicy」实战](https://segmentfault.com/a/1190000016427465)
AdaptiveSizePolicy(自适应大小策略) 是 JVM GC Ergonomics(自适应调节策略) 的一部分。
如果开启 AdaptiveSizePolicy，则每次 GC 后会重新计算 Eden、From 和 To 区的大小，计算依据是 GC 过程中统计的**GC 时间、吞吐量、内存占用量**。
![](https://img-blog.csdnimg.cn/20190326094811561.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
使用 jinfo -flags pid 即可查看默认配置的 GC 算法。
**上文提到，该算法默认开启 AdaptiveSizePolicy。**
即使 SurvivorRatio 的默认值是 8，但年轻代三个区域之间的比例仍会变动。
这个问题，可以参考来自R大的回答
## [vivorRatio没用？](https://hllvm-group.iteye.com/group/topic/35468)
![](https://img-blog.csdnimg.cn/20190326104800316.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[ 用于JVM性能调整的Java命令行选项](https://www.techpaste.com/2012/02/java-command-line-options-jvm-performance-improvement/)
### -XX：SurvivorRatio = <N>
每个幸存者空间的大小与伊甸园空间大小的比率，其中<n>是比率。以下等式可用于确定使用-XX指定的比率的幸存者空间大小：SurvivorRatio = <n>：幸存者大小= -Xmn <n> /（ - XX：SurvivorRatio = <n> + 2）其中-Xmn <n>是年轻代空间的大小，-XX：SurvivorRatio = <n>是指定为比率的值。等式中+ 2的原因是存在两个幸存者空间。指定为比率的值越大，幸存者空间大小越小。-XX：当你想要使用并发垃圾收集器显式调整幸存者空间大小来操纵对象老化时，应该使用SurvivorRatio = <n>
-XX：SurvivorRatio = <n>不应与启用了自适应大小调整的吞吐量收集器一起使用。默认情况下，通过-XX：+ UseParallelGC或-XX：+ UseParallelOldGC使用吞吐量垃圾收集器启用自适应大小调整。如果需要初始幸存者比率来开始吞吐量垃圾收集器的自适应大小调整，则应使用-XX：InitialSurvivorRatio = <n>。
----
参考官方：[https://docs.oracle.com/en/java/javase/12/gctuning/factors-affecting-garbage-collection-performance.html\#GUID-4ADBEDE9-5D52-4FBF-ADB2-431C3EB089C5](https://docs.oracle.com/en/java/javase/12/gctuning/factors-affecting-garbage-collection-performance.html#GUID-4ADBEDE9-5D52-4FBF-ADB2-431C3EB089C5)
![](https://img-blog.csdnimg.cn/20190326095023170.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
=================
经典著作《深入java虚拟机第二版》
![](https://img-blog.csdnimg.cn/20190326155757267.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
Java虚拟机内部的第5章
## [数据类型](https://www.artima.com/insidejvm/ed2/jvm3.html)
Java虚拟机通过对某些类型的数据执行操作来计算。数据类型和操作都由Java虚拟机规范严格定义。数据类型可以分为一组*基本类型*和*引用类型*。基本类型的变量包含*原始值*，引用类型的变量保存*参考值*。引用值是指对象，但不是对象本身。相反，原始值不是指任何东西。它们本身就是实际数据。您可以在图5-4中看到Java虚拟机的数据类型系列的图形描述。
![](https://img-blog.csdnimg.cn/2019032615392555.gif)
Java编程语言的所有原始类型都是Java虚拟机的原始类型。尽管**boolean**有资格作为Java虚拟机的原始类型，但指令集对它的支持非常有限。当**编译器将Java源代码转换为字节码**时，它**使用****int****s或****byte****s来表示****boolean****s**。在Java虚拟机中，**false****由整数零和****true****任何非零整数表示**。涉及`boolean`值的操作使用`int`s。数组`boolean`作为数组访问`byte`，但它们可以在堆上表示为数组`byte`或作为位字段。
Java编程语言的原始类型，而不是Java虚拟机`boolean`的*数字类型*。数字类型之间划分*整数类型*：`byte`，`short`，`int`，`long`，和`char`，以及*浮点类型*：`float`和`double`。与Java编程语言一样，Java虚拟机的原始类型在任何地方都具有相同的范围。`long`Java虚拟机中的A 总是充当64位带符号的二进制补码数，独立于底层主机平台。
Java虚拟机使用Java程序员不可用的另一种基本类型：`returnAddress`类型。该原始类型用于实现`finally`Java程序的子句。`returnAddress`第18章“最后条款”中详细描述了该类型的使用 。
Java虚拟机的引用类型巧妙地命名`reference`。类型的值有`reference`三种形式：*类类型*，*接口类型*和*数组类型*。所有三种类型都具有对动态创建的对象的引用的值。类类型的值是对类实例的引用。数组类型的值是对数组的引用，数组是Java虚拟机中的完整对象。接口类型的值是对实现接口的类实例的引用。另一个参考值是`null`值，表示`reference`变量不引用任何对象。
Java虚拟机规范定义了每种数据类型的值范围，但未定义其大小。用于存储每个数据类型值的比特数是各个实现的设计者的决定。Java虚拟机数据类型的范围如表5-1所示。有关浮点范围的更多信息，请参见第14章“浮点运算”。
![](https://img-blog.csdnimg.cn/20190326154104332.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[类加载器子系统 ](https://www.artima.com/insidejvm/ed2/jvm4.html)
类加载器子系统涉及Java虚拟机的许多其他部分以及`java.lang`库中的几个类。例如，用户定义的类加载器是其类来自的常规Java对象`java.lang.ClassLoader`。类的方法`ClassLoader`允许Java应用程序访问虚拟机的类加载机制。此外，对于Java虚拟机加载的每种类型，它都会创建一个类的实例`java.lang.Class`来表示该类型。与所有对象一样，用户定义的**类加载器和类实例****Class****驻留在堆上**。**加载类型的数据**驻留在**方法区**中。
## [方法区](https://www.artima.com/insidejvm/ed2/jvm5.html)
有关已加载类型的信息存储在称为方法区域的内存的逻辑区域中。当Java虚拟机加载类型时，它使用类加载器来定位相应的类文件。类加载器读入类文件 - 二进制数据的线性流 - 并将其传递给虚拟机。虚拟机从二进制数据中提取有关类型的信息，并将信息存储在方法区域中。类中声明的类（静态）变量的内存也取自方法区域。
## [堆](https://www.artima.com/insidejvm/ed2/jvm6.html)
只要在正在运行的Java应用程序中创建类实例或数组，就会从单个堆中分配新对象的内存。由于Java虚拟机实例中只有一个堆，因此所有线程都共享它。因为Java应用程序在其“自己的”独占Java虚拟机实例中运行，所以每个运行的应用程序都有一个单独的堆。两个不同的Java应用程序无法在彼此的堆数据上进行践踏。但是，同一应用程序的两个不同线程可以互相压缩彼此的堆数据。这就是您必须关注Java程序中对对象（堆数据）的多线程访问的正确同步的原因。
Java虚拟机有一条指令，用于在堆上为新对象分配内存，但没有释放该内存的指令。正如您无法在Java源代码中显式释放对象一样，您无法在Java字节码中显式释放对象。虚拟机本身负责决定是否以及何时释放正在运行的应用程序不再引用的对象占用的内存。通常，Java虚拟机实现使用*垃圾收集器*来管理堆。

### 对象表示
Java虚拟机规范没有说明如何在堆上表示对象。对象表示 - 堆和垃圾收集器的整体设计的一个不可或缺的方面 - 是实现设计者的决定
必须以某种方式为每个对象表示的主要数据是在对象的类及其所有超类中声明的实例变量。给定对象引用，虚拟机必须能够快速定位对象的实例数据。此外，在给定对象的引用的情况下，必须有某种方法来访问对象的类数据（存储在方法区域中）。因此，为对象分配的内存通常包括某种指向方法区域的指针。
一种可能的堆设计将堆分为两部分：句柄池和对象池。对象引用是指向句柄池条目的本机指针。句柄池条目有两个组件：指向对象池中实例数据的指针和指向方法区域中类数据的指针。这种方案的优点是它使虚拟机可以轻松地对抗堆碎片。当虚拟机移动对象池中的对象时，它只需要使用对象的新地址更新一个指针：句柄池中的相关指针。这种方法的缺点是每次访问对象的实例数据都需要解除引用两个指针。这种对象表示方法如图5-5所示。HeapOfFish applet以交互方式演示这种堆，
![](https://img-blog.csdnimg.cn/20190326154706630.gif)
另一种设计使对象引用指向包含对象实例数据和指向对象类数据的指针的数据包的本机指针。此方法只需要解除引用一个指针来访问对象的实例数据，但会使移动对象更复杂。当虚拟机移动对象以对抗此类堆的碎片时，它必须更新运行时数据区域中任何位置的对该对象的每个引用。这种对象表示方法如图5-6所示。
![](https://img-blog.csdnimg.cn/20190326154802149.gif)
## [Java堆栈](https://www.artima.com/insidejvm/ed2/jvm8.html)
启动新线程时，Java虚拟机会为该线程创建新的Java堆栈。如前所述，Java堆栈将线程的状态存储在离散帧中。Java虚拟机只在Java Stacks上直接执行两个操作：它推送和弹出帧。
线程当前正在执行的*方法*是线程的*当前方法*。当前方法的堆栈帧是*当前帧*。定义当前方法的*类*称为*当前类*，当前类的常量池是*当前常量池*。在执行方法时，Java虚拟机会跟踪当前类和当前常量池。当虚拟机遇到对存储在堆栈帧中的数据进行操作的指令时，它会在当前帧上执行这些操作。
当线程调用Java方法时，虚拟机会创建一个新帧并将其推送到线程的Java堆栈上。然后，这个新帧成为当前帧。当该方法执行时，它使用该帧来存储参数，局部变量，中间计算和其他数据。
方法可以以两种方式之一完成。如果方法通过返回完成，则称其具有*正常完成*。如果它通过抛出异常完成，则说它*突然完成*。当方法完成时，无论是正常还是突然，Java虚拟机都会弹出并丢弃方法的堆栈帧。然后，先前方法的帧成为当前帧。
线程的Java堆栈上的所有数据都是该线程专用的。线程无法访问或更改另一个线程的Java堆栈。因此，您无需担心在Java程序中同步多线程访问本地变量。当线程调用方法时，方法的局部变量存储在调用线程的Java堆栈的框架中。只有一个线程可以访问这些局部变量：调用该方法的线程。
与方法区域和堆一样，Java堆栈和堆栈帧在内存中不需要是连续的。帧可以在连续堆栈上分配，也可以在堆上分配，或者两者的某种组合。用于表示Java堆栈和堆栈帧的实际数据结构是实现设计者的决定。实现可以允许用户或程序员指定Java堆栈的初始大小，以及最大或最小大小。
## [本机方法堆栈](https://www.artima.com/insidejvm/ed2/jvm9.html)
除了由Java虚拟机规范定义并且如前所述的所有运行时数据区域之外，正在运行的Java应用程序可以使用由本机方法创建的或用于本机方法的其他数据区域。当线程调用本机方法时，它进入一个新的世界，其中Java虚拟机的结构和安全限制不再妨碍其自由。本机方法可能会访问虚拟机的运行时数据区域（它取决于本机方法接口），但也可以执行其他任何需要的操作。它可以使用本机处理器内的寄存器，在任意数量的本机堆上分配内存，或使用任何类型的堆栈。
本机方法本质上依赖于实现。实现设计人员可以自由决定使用哪种机制来使运行在其实现上的Java应用程序调用本机方法。
任何本机方法接口都将使用某种本机方法堆栈。当线程调用Java方法时，虚拟机会创建一个新帧并将其推送到Java堆栈。但是，当线程调用本机方法时，该线程将留下Java堆栈。Java虚拟机不是将新帧推送到线程的Java堆栈上，而是简单地动态链接到并直接调用本机方法。考虑它的一种方法是Java虚拟机使用本机代码动态扩展自身。就好像Java虚拟机实现只是在运行Java程序的命令下调用其自身内的另一个（动态链接）方法。
如果实现的本机方法接口使用C链接模型，则本机方法堆栈是C堆栈。当C程序调用C函数时，堆栈以某种方式运行。函数的参数按特定顺序压入堆栈。返回值以某种方式传递回调用函数。这将是该实现中本机方法堆栈的行为。
本机方法接口可能（再次由设计人员决定）能够回调到Java虚拟机并调用Java方法。在这种情况下，线程离开本机方法堆栈并进入另一个Java堆栈。
图5-13显示了一个线程的图形描述，该线程调用一个本机方法，该方法回调到虚拟机以调用另一个Java方法。该图显示了线程在Java虚拟机中可以期待的全貌。线程可能会花费整个生命周期来执行Java方法，并在其Java堆栈上使用框架。或者，它可能在Java堆栈和本机方法堆栈之间来回跳转。
![](https://img-blog.csdnimg.cn/2019032615533073.gif)
如图5-13所示，线程首先调用两个Java方法，第二个调用本机方法。此操作导致虚拟机使用本机方法堆栈。在此图中，本机方法堆栈显示为有限数量的连续内存空间。假设它是一个C堆栈。每个C-连接函数使用的堆栈区域以灰色显示并由虚线界定。第一个C-linkage函数，作为本机方法调用，调用另一个C-linkage函数。第二个C-linkage函数通过本机方法接口调用Java方法。这个Java方法调用了另一个Java方法，它是图中所示的当前方法。
与其他运行时内存区域一样，它们由本机方法堆栈占用的内存不需要具有固定大小。它可以根据运行的应用程序的需要进行扩展和收缩。实现可以允许用户或程序员指定方法区域的初始大小，以及最大或最小大小。
[本机方法接口](https://www.artima.com/insidejvm/ed2/jvm12.html)
---------
[垃圾收集的第9章](https://www.artima.com/insidejvm/ed2/gc.html)
# [垃圾收集算法](https://www.artima.com/insidejvm/ed2/gc2.html)
![](https://img-blog.csdnimg.cn/2019032614500771.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
# 分代是收集器
简单的停止和复制收集器的一个缺点是必须在每个集合中复制*所有*活动对象。通过考虑在多种语言的大多数程序中凭经验观察到的两个事实，可以改进复制算法的这一方面：
大多数程序创建的大多数对象的寿命很短。
大多数程序都会创建一些具有很长生命周期的对象。简单复制收集器的低效率的主要原因是它们花费大量时间一次又一次地复制相同的长寿命对象。
分代收集器通过按年龄分组对象以及比旧对象更频繁地收集较年轻对象的垃圾来解决这种低效问题。在这种方法中，堆被分成两个或更多个子堆，每个子堆服务于一个“生成”对象。最年轻的一代最常收集垃圾。由于大多数物体是短暂的，只有一小部分年轻物体可能在第一次收集时存活下来。一旦一个对象作为最年轻一代的成员幸存了几个垃圾收集，该对象就会被提升到下一代：它被移动到另一个子堆。逐渐老一代的垃圾收集频率低于下一代年轻一代。在当前一代中，对象“成熟”（在多个垃圾收集中存活），
世代收集技术可以应用于标记和扫描算法以及复制算法。在任何一种情况下，将堆分成几代对象都有助于提高基本底层垃圾收集算法的效率。
# [Finalization](https://www.artima.com/insidejvm/ed2/gc13.html)
在Java中，对象可能具有终结器(finalizer)：垃圾收集器必须在释放对象之前在对象上运行的方法。
终结器的潜在存在使Java虚拟机中任何垃圾收集器的工作变得复杂。
要将终结器添加到类中，只需在该类中声明一个方法，如下所示：
```python
//在文件gc / ex2 / Example2.java中的CD-ROM上
类Example2 { 
    protected void finalize（）throws Throwable { 
        // ... 
        super.finalize（）; 
    } 
    // ... 
}

垃圾收集器必须检查它发现的所有未引用的对象，以查看是否包含finalize()方法。
```
由于终结器，Java虚拟机中的垃圾收集器每次垃圾收集时都必须执行一些额外的步骤。首先，垃圾收集器必须以某种方式检测未引用的对象（称为Pass I）。然后，它必须检查它检测到的未引用对象，看是否有任何声明终结器。如果它有足够的时间，那么在垃圾收集过程中此时可能会最终确定所有声明终结器的未引用对象。
在执行所有终结器之后，垃圾收集器必须再次检测以根节点开始的未引用对象（称为Pass II）。此步骤是必需的，因为**终结器可以“复活”未引用的对象并再次引用它们**。最后，垃圾收集器可以释放在通道I和II中发现未引用的所有对象。
为了减少释放内存所需的时间，垃圾收集器可以选择在检测具有终结器的未引用对象和运行这些终结器之间插入一个步骤。一旦垃圾收集器执行了Pass I并找到了需要最终确定的未引用对象，它就可以运行一个微型跟踪，而不是从根节点开始，而是等待最终确定的对象。任何终结器都不能复制（1）从根节点无法访问的任何对象（在通过I期间检测到的对象）和（2）从等待完成的对象无法到达的对象。可以立即释放这些对象。
如果具有终结器的对象变为未引用，并且其终结器已运行，则垃圾收集器必须以某种方式确保它永远不会再次在该对象上运行终结器。如果该对象由其自己的终结器或其他对象的终结器复活，并且稍后再次被取消引用，则垃圾收集器必须将其视为没有终结器的对象。
在Java中编程时，必须记住，它是**在对象上运行终结器的垃圾收集器**。由于通常无法准确预测未引用的对象何时被垃圾收集，因此**无法预测何时运行对象终结器**。如第2章“平台独立性”中所述，您**应该避免编写正确性取决于对象及时完成的​​程序**。
例如，如果 未引用对象的终结器 释放 程序稍后 再次需要的资源，则在垃圾收集器 运行对象终结器 之后，该资源将不可用。
如果程序 在垃圾收集器 到达最终确定未引用对象之前 需要资源，则程序运气不佳。
*我的理解：**终结器需要的资源以及被提前释放会出问题。*
## [对象的可达性生命周期](https://www.artima.com/insidejvm/ed2/gc14.html)
在**1.2之前**的版本中，从垃圾收集器的角度来看，堆上的每个对象都处于三种状态之一：**可达，可复活或无法访问**。如果垃圾收集器可以通过从根节点开始描绘出对象引用的图形来“到达”对象，则对象处于*可达*状态。每个对象在可达状态下开始其生命，并且只要程序至少保持一个对象的可到达引用，就保持可达。但是，只要程序释放对对象的所有引用，对象就会变得可复原。
如果对象处于可*复活*状态，如果当前无法通过跟踪以根节点开始的引用图来访问它，但是当垃圾收集器执行某些终结器时可能会再次使其可达。所有对象，而不仅仅是声明`finalize()`方法的对象，都会通过可复活的状态。如前一节所述，对象的终结器可以通过使对象再次可达而“复活”自身或任何其他可复活对象。因为可复活状态中的任何对象都可能被其自身或其他对象再次访问`finalize()`方法，垃圾收集器在确定通过执行终结器不会使对象恢复生命之前，不能回收可复原对象占用的内存。通过运行声明`finalize()`方法的所有可复活对象的终结器，垃圾收集器将转换所有可复原对象的状态，或者返回到可达状态（对于复活的对象），或转发到无法到达状态。
该*可达*状态不仅表示一个对象不再可达，而且该对象无法通过某些终结的执行进行再次可达。无法访问的对象不再对正在运行的程序产生任何影响。因此，垃圾收集器可以自由地回收它们占用的内存。
在**版本1.2中**，三个原始可达性状态 - 可达，可恢复和不可达 - 由**三个新状态**增加：**软引用，弱引用和幻像可达**。因为这三种新状态代表三种新的（逐渐变弱的）可达性，所以在1.2之前的版本中简称为“可达”的状态称为“强可达”，从1.2开始。直接从根节点引用的任何对象（例如局部变量）都可以很容易地访问。同样，直接从强可达对象的实例变量引用的任何对象都是强可访问的。
## [Reference Objects](https://www.artima.com/insidejvm/ed2/gc15.html)
较弱形式的可达性涉及在1.2版中首次引入的实体：引用对象(reference object)。一个引用对象封装了一个引用一些其它对象，称为*所引用*。所有引用对象都是抽象`java.lang.ref.Reference`类的子类的实例。`Reference`类，这是在图9-3中所示，包括三个直接子类：`SoftReference`，`WeakReference`，和`PhantomReference`。
`SoftReference`对象封装“软引用”到所指对象;
`WeakReference`对象封装了“弱引用”到所指对象;
`PhantomReference`，将“幻像引用”封装到引用对象。
强引用及其三个逐渐弱化的表兄弟 - 软，弱和幻像引用 - 之间的**根本区别**在于，**强引用阻止其引用被垃圾收集，软，弱和幻像引用则不然。**
![](https://img-blog.csdnimg.cn/20190326152045928.gif)
要创建soft，weak或phantom引用，只需将强引用传递给相应类型的引用对象的构造函数即可。例如，要创建对特定`Cow`对象的软引用，可以向新`SoftReference`对象的构造函数传递引用该对象的强引用`Cow`。通过维护对`SoftReference`对象的强引用，可以保持对对象的软引用`Cow`。
图9-4显示了这样一个`SoftReference`对象，它封装了对象的软引用`Cow`。该`SoftReference`对象是从局部变量强引用的，该局部变量与所有局部变量一样，充当垃圾收集器的根节点。如前所述，垃圾收集根节点和强可访问对象的实例变量中包含的引用是强引用。由于`SoftReference`强引用引用了图9-4中所示的`SoftReference`对象，因此该对象可以很容易地访问。假设此`SoftReference`对象包含对象的唯一引用`Cow`，则`Cow`可以轻柔地访问该对象。`Cow`可以轻松到达的原因是垃圾收集器只能到达`Cow`通过遍历软引用从根节点对象。
![](https://img-blog.csdnimg.cn/20190326152136313.gif)
一旦创建了一个引用对象，它将继续保持其对象的soft，weak或phantom引用，直到它被程序或垃圾收集器*清除*。要清除引用对象，程序或垃圾收集器只需要在引用对象上调用`clear()`类中定义的方法`Reference`。清除引用对象会使引用对象中包含的soft，weak或phantom引用无效。例如，如果程序或垃圾收集器要调用图9-4中所示`clear()`的`SoftReference`对象，则对象的软引用`Cow`将无效，并且该`Cow`对象将不再可以软引用地访问。
## [可达性状态变化](https://www.artima.com/insidejvm/ed2/gc16.html)
如前所述，引用对象的目的是使您能够保存对垃圾收集器可以自由收集的对象的引用。换句话说，允许垃圾收集器更改任何不可强制访问的对象的可达性状态。因为在保存软，弱或幻像引用时跟踪垃圾收集器带来的可达性状态变化通常很重要，所以您可以安排在发生此类更改时收到通知。要注册对可达性状态更改的兴趣，请将引用对象与引用队列相关联。一个*reference queue*是类的一个实例`java.lang.ref.ReferenceQueue`垃圾收集器将附加（或“入队”）参与对象的可达性状态更改。通过设置和监视引用队列，您可以了解垃圾收集器异步执行的有趣的可达性状态更改。
要将引用对象与引用队列相关联，只需在创建引用对象时将引用作为构造函数参数传递给引用队列。如此创建的引用对象除了保存对引用的引用外，还将保存对引用队列的引用。当垃圾收集器对引用对象的可达性状态进行相关更改时，它会将引用对象附加到其关联的引用队列。
例如，当`WeakReference`创建图9-5中所示的对象时，会将两个引用传递给构造函数：`Fox`对象的引用和对象的引用`ReferenceQueue`。当垃圾收集器决定收集弱可达`Fox`对象时，它将清除该`WeakReference`对象，并在此时或稍后添加`WeakReference`对象到它的引用队列。
![](https://img-blog.csdnimg.cn/20190326152444417.gif)
**图9-5。与引用队列关联的引用对象。**
要将引用对象附加到其关联队列的末尾，垃圾收集器`enqueue()`将在引用对象上调用。该`enqueue()`方法在超类中定义，`Reference`仅当对象在创建时与队列关联时才将引用对象附加到引用队列，并且仅`enqueue()`在对象上调用第一次。程序可以通过两种方式监视引用队列，方法是使用`poll()`方法轮询或使用`remove()`方法阻塞。如果引用对象在队列对象上调用`poll()`或`remove()`在队列对象上调用时，该方法将从引用队列中删除该对象并将其返回。如果没有参考对象在排队等候，然而，`poll()`将立即返回`null`，但`remove()`将阻塞，直到下一个参考对象入队。一旦引用对象到达队列，`remove()`将删除并返回它。
垃圾收集器在不同情况下将软，弱和幻像引用对象排入队列，以指示三种不同类型的可达性状态更改。六个可达性状态的含义以及状态变化发生的环境如下：
强可达 - 可以从根到达对象而不遍历任何引用对象。对象在强可达状态下开始其生命周期，并且只要可通过根节点或另一个强可访问对象访问，就保持强可访问性。垃圾收集器不会尝试回收强可达对象占用的内存。
软到达 - 对象不可强到达，但可以通过一个或多个（未清除的）软引用对象从根目录到达。垃圾收集器可以回收软可访问对象占用的内存。如果它这样做，它将清除对该可轻松访问的对象的所有软引用。当垃圾收集器清除与引用队列关联的软引用对象时，它会将该引用对象排入队列。
弱可达 - 对象既不强也不可达，但可以通过一个或多个（未清除的）弱引用对象从根到达。垃圾收集器必须回收弱可达对象占用的内存。当它这样做时，它会清除对该弱可达对象的所有弱引用。当垃圾收集器清除与引用队列关联的弱引用对象时，它会将该引用对象排入队列。
resurrectable - 一个对象既不强、不软，不弱，也可以通过执行某些终结器复活回其中一个状态。
幻像可达 - 一个对象没有强烈，柔和，也没有弱可达性，已被确定为不能被任何终结器复活（如果它声明了一个finalize()方法本身，那么它的终结器将被运行），并且可以通过一个从根本位置到达或更多（未清除）幻像参考对象。只要幻像引用对象引用的对象变为幻像可达，垃圾收集器就会将其排入队列。垃圾收集器永远不会清除幻像引用。程序必须明确清除所有幻像引用。
无法到达 - 一个物体既不强烈，柔和，微弱，也不是幻影可达，并且不可复原。无法访问的对象已准备好进行回收。
请注意，当垃圾收集器的对象离开相关的可达性状态时，垃圾收集器会将软和弱引用对象排入队列，而当对象进入相关状态时，它会将幻像引用排入队列。您还可以看到这种差异，垃圾收集器在排队之前清除软和弱引用对象，而不是幻像引用对象。因此，垃圾收集器将软引用对象排队以指示它们的指示对象刚刚离开可轻松到达的状态。同样，垃圾收集器将弱引用对象排入队列以指示它们的指示对象刚刚离开弱可达状态。但是垃圾收集器将幻像引用对象排入队列以指示它们的引用已进入幻像可达状态。
## [缓存，规范化映射和预先清理](https://www.artima.com/insidejvm/ed2/gc17.html)
垃圾收集器以不同方式处理软，弱和幻像对象，因为每个对象都旨在为程序提供不同类型的服务。
## 软引用使您可以创建对程序的整体内存需求敏感的内存缓存。
## 弱引用使您能够创建规范化映射，例如哈希表，如果它们在程序中未被引用，则其键和值将从映射中删除。
## 幻影引用使您能够建立比终结器更灵活的预先清理策略。
要使用软引用或弱引用的引用，请`get()`在引用对象上调用。如果引用尚未清除，您将获得对引用的强引用，然后您可以按常规方式使用引用。如果参考已被清除，您将`null`回来。`get()`但是，如果调用幻像引用对象，`null`即使尚未清除引用对象，也总是会返回。因为幻像可达状态仅在对象通过可复活状态之后才获得，所以幻像参考对象无法访问其指示对象。调用`get()`幻像引用对象始终返回`null`，即使幻象参考尚未被清除，因为如果它返回对幻像可到达对象的强引用，它实际上会复活该对象。因此，一旦物体达到幻像可达性，它就不能复活。
虚拟机实现需要在抛出之前清除软引用`OutOfMemoryError`，否则可以自由决定何时或是否清除它们。但是，鼓励实现仅在程序对内存的需求超过供应时清除软引用，在较新的软引用之前清除较旧的软引用，并清除最近在最近使用的软引用之前未使用的软引用。
软引用使您可以缓存内存数据，您可以从外部源（如文件，数据库或网络）中更慢地检索这些数据。只要虚拟机有足够的内存来容纳堆上的软引用数据以及所有强引用数据，软引用通常就足够强大，以便在堆上保留软引用数据。但是，如果内存变得稀缺，则垃圾收集器可能决定清除软引用并回收软引用数据占用的空间。下次程序需要使用该数据时，必须从外部源重新加载。同时，虚拟机有更多的空间来容纳程序的强烈（和其他轻微）引用的内存需求。
弱引用类似于软引用，除了垃圾收集器可以自由决定是否清除对软可访问对象的软引用，它必须在确定弱可达对象时立即清除对弱可到达对象的弱引用。弱引用使您可以创建从键到值的规范化映射。本`java.util.WeakHashMap`类使用弱引用只是提供这样的映射进行规范化。您可以`WeakHashMap`通过该`put()`方法向实例添加键值对，就像您可以实现任何类的实例一样`java.util.Map`。但在里面`WeakHashMap`，关键对象通过与引用队列关联的弱引用对象来保存。如果垃圾收集器确定密钥对象是弱可达的，则它将清除并排队引用密钥对象的任何弱引用对象。下次`WeakHashMap`访问时，它将轮询引用队列并提取垃圾收集器放在那里的所有弱引用对象。所述`WeakHashMap`然后将从其映射除去任何键-值对键，其弱引用对象在队列中出现了。因此，如果向a添加键值对`WeakHashMap`，只要程序没有使用该`remove()`方法显式删除它，它就会保留在那里，并且垃圾收集器不会确定密钥对象是弱可达的。
**幻像可达性表示对象已准备好进行回收**。当垃圾收集器确定幻像引用对象的引用对象是幻像可达时，它会**将幻像引用对象附加到其关联的引用队列**。（**与软和弱引用对象不同，可以选择创建它们而不将它们与引用队列相关联**），如果**不将引用对象与引用队列相关联，则无法实例化幻像引用对象**。）您可以在引用中使用幻像引用的到达排队以触发您希望在对象生命周期结束时执行的某些操作。因为您无法获得对幻像可到达对象的强引用（该`get()`方法始终返回`null`），您将无法采取任何需要您访问目标实例变量的操作。完成幻像可到达对象的预先清理操作后，必须调用`clear()`引用它的幻像引用对象。调用`clear()`幻象参考对象是其指示物的政变，将指示物从幻象可达状态发送到其最终静止位置：不可接近。
[有关JVM内容的书籍中](https://www.artima.com/jvm/booklist.html)提供了涵盖Java虚拟机内部主题的[推荐书籍列表](https://www.artima.com/insidejvm/resources/)
## [线程同步第20章](https://www.artima.com/insidejvm/ed2/threadsynch.html)
Java编程语言的优势之一是它支持语言级的多线程。大部分支持都集中在*同步：*协调多个线程之间的活动和数据访问。Java用于支持同步的机制是*监视器*。
Java的监视器支持两种线程同步：*互斥*和*合作*。通过对象锁在Java虚拟机中支持的相互排除使多个线程能够独立地处理共享数据而不会相互干扰。通过类的wait和notify方法在Java虚拟机中支持的协作`Object`使线程能够朝着共同的目标一起工作。
监视器就像一个包含一个特殊房间的建筑物，一次只能由一个线程占用。房间通常包含一些数据。从一个线程进入这个房间到它离开的时间，它可以独占访问房间里的任何数据。进入监视器建筑物称为“进入监视器”。进入大楼内的特殊房间称为“获取监视器”。占据房间被称为“拥有监视器”，离开房间被称为“释放监视器”。离开整个建筑被称为“退出监视器”。
除了与一些数据相关联之外，监视器还与一个或多个代码位相关联，在本书中将其称为*监视区域*。监视区域是需要作为关于特定监视器的一个不可分割操作来执行的代码。换句话说，一个线程必须能够从头到尾执行监视区域，而没有另一个线程同时执行同一监视器的监视区域。监视器强制执行其监视区域的一次一个线程执行。线程进入监视器的唯一方法是到达与该监视器关联的监视区域之一的开头。线程可以向前移动并执行监视区域的唯一方法是获取监视器。
当线程到达监视区域的开头时，它将被放入相关监视器的*条目集中*。入口设置就像监视器建筑的前走廊。如果没有其他线程在条目集中等待，并且没有其他线程当前拥有监视器，则线程获取监视器并继续执行监视区域。当线程完成执行监视区域时，它将退出（并释放）监视器。
如果线程到达受另一个线程已拥有的监视器保护的监视区域的开头，则新到达的线程必须在条目集中等待。当前所有者退出监视器时，新到达的线程必须与也在条目集中等待的任何其他线程竞争。只有一个线程将赢得竞争并获得监视器。
上面列出的第一种同步，互斥，是指多线程互斥执行监控区域。在任何时候，只有一个线程可以执行特定监视器的监视区域。通常，只有当多个线程共享数据或某些其他资源时，互斥才是重要的。如果两个线程不使用任何公共数据或资源，它们通常不会相互干扰，也不需要以互斥的方式执行。但是，在没有时间片的Java虚拟机实现上，永不阻塞的优先级较高的线程会干扰任何优先级较低的线程，即使这些线程都没有共享数据。优先级较高的线程将以较低优先级线程为代价来独占CPU。较低优先级的线程永远不会获得任何CPU时间。在这种情况下，可以使用不保护数据的监视器来编排这些线程，以确保所有线程都获得一些CPU时间。然而，在大多数情况下，监视器保护通过监视区域代码访问的数据。如果只能通过监视区域访问数据，则监视器会强制对该数据进行互斥访问。
上面列出的监视器支持的另一种同步是合作。虽然互斥有助于防止线程在共享数据时彼此干扰，但合作有助于线程协同工作以实现某些共同目标。
当一个线程需要某些数据处于特定状态而另一个线程负责将数据导入该状态时，合作很重要。例如，一个线程，即“读取线程”，可能正在从另一个线程“写入线程”填充的缓冲区中读取数据。读取线程需要缓冲区处于“非空”状态，然后才能从缓冲区中读取任何数据。如果读取线程发现缓冲区为空，则必须等待。写线程负责用数据填充缓冲区。一旦写入线程完成了更多的写入，读取线程就可以进行更多的读取。
Java虚拟机使用的监视器形式称为“等待和通知”监视器。（它有时也被称为“信号和继续”监视器。）在这种监视器中，当前拥有监视器的线程可以通过执行*wait命令*将其自身暂停在监视器内。当线程执行等待时，它会释放监视器并进入*等待集*。线程将在等待集中保持挂起，直到另一个线程执行*notify命令*之后的某个时间在显示器内。当线程执行通知时，它继续拥有监视器，直到它自动释放监视器，执行等待或完成监视区域。通知线程释放监视器后，等待的线程将被复活并重新获取监视器。
Java虚拟机中使用的监视器类型有时称为“信号和继续”监视器，因为在线程执行通知（信号）后，它将保留监视器的所有权并继续执行监视区域（继续）。稍后，通知线程释放监视器并且等待线程被复活。据推测，等待线程自行暂停，因为受监视器保护的数据不处于允许线程继续执行有用工作的状态。此外，通知线程可能在将由监视器保护的数据置于等待线程所需的状态之后执行通知命令。但是因为通知线程继续，它可能在通知之后改变了状态，使得等待线程仍然不能做有用的工作。或者，第三个线程可能在通知线程释放它之后但在等待线程获取它之前获取了监视器，并且第三个线程可能已经改变了受保护数据的状态。因此，通常必须通过等待线程仅将其视为所需状态的提示来考虑通知*可能*存在。每次等待线程复活时，可能需要再次检查状态以确定它是否可以向前移动并执行有用的工作。如果它发现数据仍未处于所需状态，则线程可以执行另一次等待或放弃并退出监视器。
作为示例，再次考虑上述涉及缓冲区，读取线程和写入线程的场景。假设缓冲区受监视器保护。当读取线程进入保护缓冲区的监视器时，它会检查缓冲区是否为空。如果缓冲区不为空，则读取线程从缓冲区读取（并删除）一些数据。满意，它退出显示器。另一方面，如果缓冲区为空，则读取线程执行等待命令。一旦执行等待，读取线程就会暂停并放入监视器的等待集中。在此过程中，读取线程释放监视器，该监视器可供其他线程使用。稍后，写入线程进入监视器，将一些数据写入缓冲区，执行通知并退出监视器。当写线程执行通知时，读取线程被标记为最终复活。写入线程退出监视器后，读取线程将作为监视器的所有者复活。如果某个其他线程有可能出现并消耗了写线程留下的数据，则读取线程必须显式检查以确保缓冲区不为空。如果任何其他线程没有机会消耗数据，则读取线程可以假设数据存在。读线程从缓冲区读取一些数据并退出监视器。读取线程必须显式检查以确保缓冲区不为空。如果任何其他线程没有机会消耗数据，则读取线程可以假设数据存在。读线程从缓冲区读取一些数据并退出监视器。读取线程必须显式检查以确保缓冲区不为空。如果任何其他线程没有机会消耗数据，则读取线程可以假设数据存在。读线程从缓冲区读取一些数据并退出监视器。
图20-1显示了Java虚拟机使用的监视器类型的图形描述。该图将监视器显示为三个矩形。在中心，一个大矩形包含一个线程，即监视器的所有者。在左侧，一个小矩形包含条目集。在右侧，另一个小矩形包含等待集。活动线程显示为深灰色圆圈。悬浮的线显示为浅灰色圆圈。
![](https://img-blog.csdnimg.cn/20190326160832156.gif)
图20-1还显示了几个编号的门，线程必须“穿过”以与监视器交互。当线程到达监视区域的开头时，它通过最左边的门（门号1）进入监视器，并发现自己位于容纳入口集的矩形中。如果没有线程当前拥有监视器且没有其他线程在条目集中等待，则线程立即通过隔壁的第二个门，并成为监视器的所有者。作为监视所有者，线程继续执行监视区域。另一方面，如果另一个线程当前声称拥有监视器的所有权，则新到达的线程必须在条目集中等待，可能还有已经在那里等待的其他线程。新到的线程被阻止，因此没有
图20-1显示了在入口集中挂起的三个线程和在等待集中挂起的四个线程。这些线程将保留在它们所在的位置，直到监视器的当前所有者（活动线程）释放监视器。活动线程可以通过以下两种方式之一释放监视器：它可以完成正在执行的监视区域，也可以执行wait命令。如果它完成了显示器区域，它将通过中央矩形底部的门5号门离开显示器。如果它执行了一个等待命令，它会在显示器通过三号门（等待设置的门）时释放显示器。
如果前所有者在释放监视器之前没有执行通知（并且之前没有通知任何等待的线程并且等待复活），那么只有条目集中的线程才会竞争获取监视器。如果前所有者确实执行了通知，那么条目集线程将必须与来自等待集的一个或多个线程竞争。如果来自条目集的线程赢得竞争，它将通过第二门，并成为监视器的新所有者。如果来自等待集的线程赢得竞争，则它退出等待集并在通过第4门时重新获取监视器。请注意，第3和第4门是线程进入或退出等待集的唯一方法。一个线程只能执行一个等待命令，如果它当前拥有该监视器，它可以'
在Java虚拟机中，线程可以选择在执行wait命令时指定超时。如果线程确实指定了超时，并且在超时到期之前没有其他线程执行通知，则等待线程实际上会从虚拟机接收自动通知。超时到期后，即使没有其他线程执行显式通知，等待线程也将被复活。
Java虚拟机提供两种通知命令：“notify”和“notify all”。notify命令从等待集中任意选择一个线程并将其标记为最终复活。notify all命令标记当前在等待集中的所有线程，以便最终复活。
在很大程度上，Java虚拟机实现从等待或条目集中选择下一个线程的方式是各个实现设计者的决定。例如，实现设计者可以决定如何选择：给定一个通知命令的等待集中的线程o给定一个notify all命令从等待集中恢复线程的顺序o允许来自条目集的线程获取监视器的命令o如何在等待集中挂起的线程与在notify命令之后的条目集之间进行选择你可能认为将入口集和等待集实现为先进先出（FIFO）队列是有意义的，这样线程等待时间最长的将是第一个选择购买显示器的人。或者，有10个FIFO队列可能是有意义的，一个线程可以在Java虚拟机中拥有一个优先级。然后，虚拟机可以选择在包含任何等待线程的最高优先级队列中等待时间最长的线程。实现可能采取这些方法，但您不能依赖它。实现可以自由地将入口和等待集实现为后进先出（LIFO）队列，在较高优先级的线程之前选择较低优先级的线程，或者执行其他可能看起来没有意义的事情。简而言之，实现可以以任意方式自由选择线程，从而无法进行分析并产生令人惊讶的排序。实现可能采取这些方法，但您不能依赖它。实现可以自由地将入口和等待集实现为后进先出（LIFO）队列，在较高优先级的线程之前选择较低优先级的线程，或者执行其他可能看起来没有意义的事情。简而言之，实现可以以任意方式自由选择线程，从而无法进行分析并产生令人惊讶的排序。实现可能采取这些方法，但您不能依赖它。实现可以自由地将入口和等待集实现为后进先出（LIFO）队列，在较高优先级的线程之前选择较低优先级的线程，或者执行其他可能看起来没有意义的事情。简而言之，实现可以以任意方式自由选择线程，从而无法进行分析并产生令人惊讶的排序。
作为程序员，您不能依赖任何特定的选择算法或优先级处理，至少在您尝试编写与平台无关的Java程序时。例如，因为您不知道通过notify命令选择等待集中的哪些顺序线程来复活，所以只有当您绝对确定只有一个线程被挂起时，您应该使用notify（而不是通知all）在等待集中。如果在任何时候都有可能在等待集中挂起多个线程，则应该使用notify all。否则，在某些Java虚拟机实现上，特定线程可能会在等待集中停留很长时间。如果通知始终从等待集中选择最近到达，并且等待集始终包含多个线程，
# [对象锁定](https://www.artima.com/insidejvm/ed2/threadsynch2.html)
一些Java虚拟机的运行时数据区域由所有线程共享，其他线程对于各个线程是私有的。因为堆和方法区域是由所有线程共享的，所以Java程序需要协调对两种数据的多线程访问：o实例变量，它们存储在堆上的o类变量中，这些变量存储在方法区域中需要协调对驻留在Java堆栈上的本地变量的访问，因为Java堆栈上的数据对于Java堆栈所属的线程是私有的。
在Java虚拟机中，每个对象和类在逻辑上与监视器关联。对于对象，关联的监视器保护对象的实例变量。对于类，监视器保护类的类变量。如果对象没有实例变量，或者类没有类变量，则关联的监视器不保护数据。
为了实现监视器的互斥功能，Java虚拟机将锁（有时称为互斥锁）与每个对象和类相关联。锁就像一个特权，任何时候只有一个线程可以“拥有”。线程无需获取锁来访问实例或类变量。但是，如果一个线程确实获得了一个锁，那么在拥有该锁的线程释放它之前，没有其他线程可以获得对相同数据的锁定。（“锁定对象”是获取与该对象关联的监视器。）
类锁实际上实现为对象锁。如前面章节中所述，当Java虚拟机加载类文件时，它会创建一个类实例`java.lang.Class`。锁定类时，实际上是锁定该类的`Class`对象。
允许单个线程多次锁定同一对象。对于每个对象，Java虚拟机维护对象被锁定的次数。解锁对象的计数为零。当线程第一次获得锁定时，计数再次增加到1。每次线程获取同一对象的锁定时，计数再次递增。（只有已经拥有对象锁的线程才允许再次锁定它。如前所述，在拥有线程释放锁之前，没有其他线程可以锁定对象。）每次线程释放锁时，计数都会递减。当计数达到零时，锁被释放并可供其他线程使用。
Java虚拟机中的线程在到达监视区域的开头时请求锁定。在Java中，有两种监视区域：synchronized语句和synchronized方法。（这些将在本章后面详细介绍。）Java程序中的每个监视区域都与对象引用相关联。当线程到达监视区域中的第一条指令时，线程必须获取对引用对象的锁定。在获得锁之前，不允许该线程执行代码。一旦获得锁定，线程就会进入受保护代码块。当线程离开块时，无论它如何离开块，它都会释放相关对象的锁定。
请注意，作为Java程序员，您永远不会显式锁定对象。对象锁是Java虚拟机的内部对象。在Java程序中，您可以通过编写同步语句和方法来识别程序的监视区域。当Java虚拟机运行程序时，它会在每次遇到监视区域时自动锁定对象或类。
## [同步语句synchronized](https://www.artima.com/insidejvm/ed2/threadsynch3.html)
当`monitorenter`Java虚拟机遇到它时，它获取堆栈上objectref引用的对象的锁。如果线程已拥有该对象的锁，则与该锁相关联的计数将递增。每次`monitorexit`对对象上的线程执行时，计数递减。当计数达到零时，监视器被释放。
同步类（静态）方法的操作方式与上例中显示的同步实例方法相同。一个区别是，线程必须获取对相应实例的锁定，而不是获取锁定`this`（因为`this`类方法中没有）`Class`。
类`Object`声明了五种方法，使程序员能够访问Java虚拟机对同步协调方面的支持。这些方法声明为public和final，因此它们由所有类继承。它们只能在同步方法或语句中调用。换句话说，必须在调用任何这些方法之前获取与对象关联的锁。表20-2列出了这些方法。表20-2。class的wait和notify方法`Object`
![](https://img-blog.csdnimg.cn/20190326161605487.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
=================
《[[JCConf 2015] 阿里 JVM 的工作方向 by 趙海平（福貝） - R0 Day2-1](https://www.youtube.com/watch?v=dODJT_cINxk)》
![](https://img-blog.csdnimg.cn/20190326201439397.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326201600152.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
基于G1 不同业务打标记。
![](https://img-blog.csdnimg.cn/20190326200352475.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
Jvm没有考虑到很多公司是webapplication，也就是一个请求很快返回，sesssion end所以这些object就不要了.
java sesssion end的时候需要一个个去查有无继续使用， 可以类似G1打标签请求结束一次性处理。
静态对象需要区分全局的静态对象还是单次请求的非静态的对象，单次的可以请求结束就释放。
可以把变量硬性分为静态和非静态的。可以把全部复制出去。
这样ygc<1ms，但是目前ygc需要3-10ms。
没有把FGC跑的时候需要做的事情再去做，只是提前做一些事情。缩短FGC时间，碎片化。不会造成大的停顿。
提前把这个变量拷贝到年老代了碎片化，每个web请求都做自己的，这样YGC清理起来就非常快。
避免碎片化，FGC很多时候是在做整理。
允许有内部的碎片化，没有外部的碎片化。
![](https://img-blog.csdnimg.cn/20190326200403158.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
每次调用用IO，有等待，用线程池就是想避免这个，因为有主线程+子线程。
![](https://img-blog.csdnimg.cn/20190326200415642.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
在大规模的系统里线程池不是一个很好的模式，更应该思考异步化的东西。
只有异步化才可以最大化IO操作，没有线程的切换，因为底层要做很大的上下文切换，对系统资源消耗很大。
异步化等于是在用户态做。
协程就是异步化的一个组件。
yield让出线程，这个有个engin在监听IO是否回来了，只要一回来就又回到之前yield之后的地方。
![](https://img-blog.csdnimg.cn/20190326200425565.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326200434973.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326200444873.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326200454334.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190326200504265.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
希望做到像 Zing的ReadyNow一样，但是ReadyNow不开源又有自己的设计哲学。
1.没有改动的代码加载之前解析过的；
2.打赌预测 加载A类的时候也会加载B
3.开发人员加注解，说明是核心方法，需要预热。
![](https://img-blog.csdnimg.cn/20190326200914348.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
必须关注线上的每一个细节。需要可以连接上线上JVM里。
![](https://img-blog.csdnimg.cn/20190326201059671.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
每一段代码消耗多少系统资源，需要指标分析然后优化。
某个函数CPU消耗大做优化，比去猜想要准。
不断的收集线上，实时的掌握集群情况。
每一个函数都要去测。
需要非常低损耗的去收集数据。
然后做局部优化。
如果每个函数都调用已经不多，那么这时需要做架构上的调整，减少调用次数。
所以需要看到函数调用次数。
=================
# [使用Java创建内存泄漏](https://stackoverflow.com/questions/6470651/creating-a-memory-leak-with-java)
![](https://img-blog.csdnimg.cn/20190325162205513.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190325164516813.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/2019032516460388.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[https://groups.google.com/a/jclarity.com/forum/\#!search/jvm](https://groups.google.com/a/jclarity.com/forum/#!search/jvm)
![](https://img-blog.csdnimg.cn/20190325164408839.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)[https://bugs.openjdk.java.net/projects/JDK/issues/JDK-8213225?filter=allopenissues](https://bugs.openjdk.java.net/projects/JDK/issues/JDK-8213225?filter=allopenissues)
![](https://img-blog.csdnimg.cn/20190325175620945.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[https://www.quora.com/search?q=jvm](https://www.quora.com/search?q=jvm)
![](https://img-blog.csdnimg.cn/2019032711545363.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

