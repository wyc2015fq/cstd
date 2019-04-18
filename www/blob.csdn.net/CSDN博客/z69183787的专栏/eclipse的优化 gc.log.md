# eclipse的优化 gc.log - z69183787的专栏 - CSDN博客
2014年09月08日 13:42:43[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3468
原帖：[http://www.javaeye.com/topic/756538](http://www.javaeye.com/topic/756538)
性能优化从身边做起。
首先建立评估体系，将workspace里所有的项目close掉，关闭eclipse。优化的用例就是启动eclipse，open一个项目，eclipse会自动build这个项目，保证没有感觉到明显的卡，也就是没有full GC。  
开始：  
eclipse.ini里加入打印gc情况的参数：  
-Xloggc:gc.log
这样eclipse在运行过程中会记录gc日志，显示详细的gc情况，并打印在gc.log中，通过分析这个日志寻找eclipse的性能瓶颈和优化方式。  
我最初的参数只是在原版基础上调了堆大小 
-Xmx512m
将堆初始化和最大值设为一样，消除堆大小根据当前堆使用情况而变化带来的影响。  
启动eclipse，发现gc.log里打出了很多full gc的日志 
引用
4.226: [Full GC 4.226: [Tenured: 18470K->19304K(30544K), 0.1159544 secs] 25154K->19304K(44368K), [Perm : 24574K->24554K(24576K)], 0.1160431 secs] [Times: user=0.13 sys=0.00, real=0.13 secs]
在启动的6秒多时间里共出现了8次full gc，所以启动慢，觉得启动时候挺卡的。从日志里可以看出来 FullGC主要是在回收tenured区和Perm区，其中Perm一直都是快满的状态，Perm : 24574K->24554K(24576K)，Perm大小在不断调整，所以需要固定Perm区的大小，保证够用，eclipse.ini里加入 
-XX:MaxPermSize=64m
再启动：发现没有full gc了只有数量比较多的minor gc，挑启动开始到启动完成的第一条和最后一条日志 
引用
0.209: [GC 0.209: [DefNew: 4416K->511K(4928K), 0.0034707 secs] 4416K->614K(15872K), 0.0035239 secs] [Times: user=0.00 sys=0.00, real=0.00 secs]  
….  
6.383: [GC 6.383: [DefNew: 18880K->1985K(21184K), 0.0055311 secs] 46992K->30098K(68040K), 0.0055694 secs]
这6秒中GC日志打了69次， 而内存回收率还是蛮高的 young区18880-1985=16895 jvm 46992-30098=16894 都快接近100%了，可以看出young区是由小到大在不断调整大小，所以不断GC，因此设一个初始值吧，据说设置heap的1/4比较好，那就是 128M，所以eclipse.ini加入 
-Xmn128m
再重启，发现GC日志就四条了，eclipse启动自然快了  
引用
1.292: [GC 1.292: [DefNew: 104960K->10984K(118016K), 0.0334165 secs] 104960K->10984K(511232K), 0.0334603 secs] [Times: user=0.03 sys=0.00, real=0.03 secs]  
2.182: [GC 2.182: [DefNew: 115944K->1852K(118016K), 0.0221714 secs] 115944K->11466K(511232K), 0.0222142 secs] [Times: user=0.00 sys=0.02, real=0.02 secs]  
3.987: [GC 3.987: [DefNew: 106779K->12531K(118016K), 0.0378228 secs] 116393K->22145K(511232K), 0.0378692 secs] [Times: user=0.03 sys=0.00, real=0.03 secs]  
5.377: [GC 5.377: [DefNew: 117491K->9403K(118016K), 0.0513728 secs] 127105K->31364K(511232K), 0.0514133 secs]
但是，启动后open我的多个项目，这些项目互相依赖，eclipse自动build，感觉有点小卡，发现日志里多了4次full GC，所以就卡了… 
引用
67.320: [Full GC (System) 67.320: [Tenured: 88847K->68809K(393216K), 0.2121213 secs] 117385K->68809K(511232K), [Perm : 41915K->41915K(65536K)], 0.2121747 secs] [Times: user=0.20 sys=0.00, real=0.20 secs]  
103.759: [Full GC (System) 103.759: [Tenured: 81882K->66784K(393216K), 0.3287387 secs] 185350K->66784K(511232K), [Perm : 53464K->53414K(65536K)], 0.3287897 secs] [Times: user=0.33 sys=0.00, real=0.33 secs]
这个时候Tenured区和Perm都还没到很接近最大值，但是为什么还有full GC呢，开始以为是JVM悲观认为Tenured区剩余空间不足以应对下一次minor GC 所以进行了full GC调整Tenured空间，索性直接增加了堆最大值到-Xmx728m（工作电脑的内存是3.5G），但重启后full gc还是有4次，而且有几次minor GC用的时间超过了0.1秒，这是因为增加了堆大小，导致GC用时也增加了，不能接受。所以还是改回-Xmx512m。  
再仔细观察日志，发现Full GC (System) 字样，这个意思是eclipse里调用了System.gc()手动触发了系统GC，好吧，哥已经给你分配足够空间了，你就省省吧，在eclipse.ini里加入： 
-XX:+DisableExplicitGC
这样就差不多了，整个过程没有出现full gc，再编码2个小时，中间只出现了一次full gc，在open build某50W行+的代码的时候，eclipse还是卡了…  
最后又稍微调了一下各代的大小，得到目前的参数： 
-XX:+DisableExplicitGC
另外没有去调GC策略，主要是觉得eclipse是客户端程序，默认的client单线程的GC策略应该是比较适合的，以后有时间再试试看吧。
Java 虚拟机默认分配64M内存，如果你的应用比较大，超出64M内 存，Java虚拟机就会抛出outOfMemoryError，并停止运行。不管是什么应用（Web应用、Application等），只需要修改你的机 器上的运行Java命令，在javaxxx命令中添加-Xms(最小使用内存)、-Xmx（最大使用内存）即可解决。当然，这儿的内存容量都是指物理内 存，不能超出你的机器的物理内存的总容量。
对于单独的.class，可以用下面的方法对Test运行时的jvm内存进行设置。
java -Xms64m -Xmx256m Test
-Xms是设置内存初始化的大小
-Xmx是设置最大能够使用内存的大小（最好不要超过物理内存大小）
在eclipse中的设置方法
方 法1：对应在Eclipse中的设置为:窗口->首选项->JAVA->已安装的JRE,在缺 省的VM自变量中增加:-Xmx256M （也可以添加其他参数如-Xms128M -XX:PermSize=64M -XX:MaxPermSize=128M）
方法2：修改eclipse.ini
-vmargs -Xms128M -Xmx512M -XX:PermSize=64M -XX:MaxPermSize=128M
这里有几个问题：
1. 各个参数的含义什么？
2. 为什么有的机器我将-Xmx和-XX:MaxPermSize都设置为512M之后Eclipse可以启动，而有些机器无法启动？
3. 为何将上面的参数写入到eclipse.ini文件Eclipse没有执行对应的设置？
下面我们一一进行回答
1. 各个参数的含义什么？
参数中-vmargs的意思是设置JVM参数，所以后面的其实都是JVM的参数了，我们首先了解一下JVM内存管理的机制，然后再解释每个参数代表的含义。
堆(Heap)和非堆(Non-heap)内存
按 照官方的说法：“Java 虚拟机具有一个堆，堆是运行时数据区域，所有类实例和数组的内存均从此处分配。堆是在 Java 虚拟机启动时创建的。”“在JVM中堆之外的内存称为非堆内存(Non-heap memory)”。可以看出JVM主要管理两种类型的内存：堆和非堆。简单来说堆就是Java代码可及的内存，是留给开发人员使用的；非堆就是JVM留给 自己用的，所以方法区、JVM内部处理或优化所需的内存(如JIT编译后的代码缓存)、每个类结构(如运行时常数池、字段和方法数据)以及方法和构造方法 的代码都在非堆内存中。
堆内存分配
JVM初始分配的内存由-Xms指定，默认是物理内存的1/64；JVM最大分配的内存由-Xmx指 定，默认是物理内存的1/4。默认空余堆内存小于 40%时，JVM就会增大堆直到-Xmx的最大限制；空余堆内存大于70%时，JVM会减少堆直到-Xms的最小限制。因此服务器一般设置-Xms、 -Xmx相等以避免在每次GC 后调整堆的大小。
非堆内存分配
JVM使用-XX:PermSize设置非堆内存初始值，默认是物理内存的1/64；由XX:MaxPermSize设置最大非堆内存的大小，默认是物理内存的1/4。
JVM内存限制(最大值)
首 先JVM内存限制于实际的最大物理内存(废话！呵呵)，假设物理内存无限大的话，JVM内存的最大值跟操作系统有很大的关系。简单的说就32位处理器虽然 可控内存空间有4GB,但是具体的操作系统会给一个限制，这个限制一般是2GB-3GB（一般来说Windows系统下为1.5G-2G，Linux系统 下为2G-3G），而64bit以上的处理器就不会有限制了。
2. 为什么有的机器我将-Xmx和-XX:MaxPermSize都设置为512M之后Eclipse可以启动，而有些机器无法启动？
通过上面对JVM内存管理的介绍我们已经了解到JVM内存包含两种：堆内存和非堆内存，另外JVM最大内存首先取决于实际的物理内存和操作系统。所以说设置VM参数导致程序无法启动主要有以下几种原因：
1) 参数中-Xms的值大于-Xmx，或者-XX:PermSize的值大于-XX:MaxPermSize；
2) -Xmx的值和-XX:MaxPermSize的总和超过了JVM内存的最大限制，比如当前操作系统最大内存限制，或者实际的物理内存等等。说到实际物理 内存这里需要说明一点的是，如果你的内存是1024MB，但实际系统中用到的并不可能是1024MB，因为有一部分被硬件占用了。
3. 为何将上面的参数写入到eclipse.ini文件Eclipse没有执行对应的设置？
那为什么同样的参数在快捷方式或者命令行中有效而在eclipse.ini文件中是无效的呢？这是因为我们没有遵守eclipse.ini文件的设置规则：
参数形如“项 值”这种形式，中间有空格的需要换行书写，如果值中有空格的需要用双引号包括起来。比如我们使用-vm C:\Java\jre1.6.0\bin\javaw.exe参数设置虚拟机，在eclipse.ini文件中要写成这样：
-vm
C:\Java\jre1.6.0\bin\javaw.exe
按照上面所说的，最后参数在eclipse.ini中可以写成这个样子：
-vmargs
-Xms128M
-Xmx512M
-XX:PermSize=64M
-XX:MaxPermSize=128M
实际运行的结果可以通过Eclipse中“Help”-“About Eclipse SDK”窗口里面的“Configuration Details”按钮进行查看。
另一篇：**[一次Java垃圾收集调优实战](http://www.javaeye.com/topic/212967)**
### 1 资料
- [JDK5.0垃圾收集优化之--Don't
 Pause(花钱的年华)![](http://www.javaeye.com/images/icons/linkext7.gif)](http://calvin.javaeye.com/blog/91905)
- [编写对GC友好，又不泄漏的代码(花钱的年华)![](http://www.javaeye.com/images/icons/linkext7.gif)](http://calvin.javaeye.com/blog/91903)
- [JVM调优总结![](http://www.javaeye.com/images/icons/linkext7.gif)](http://pengjiaheng.spaces.live.com/blog/cns%212DAA368B386E6AEA%21770.entry)
- [JDK
 6所有选项及默认值![](http://www.javaeye.com/images/icons/linkext7.gif)](http://www.md.pp.ru/~eu/jdk6options.html)
### 2 GC日志打印
  GC调优是个很实验很伽利略的活儿，GC日志是先决的数据参考和最终验证：
```java
-XX:+PrintGCDetails 
-XX:+PrintGCTimeStamps(GC发生的时间) 
-XX:+PrintGCApplicationStoppedTime(GC消耗了多少时间) 
-XX:+PrintGCApplicationConcurrentTime(GC之间运行了多少时间)
```
### 3 收集器选择
#### CMS收集器：暂停时间优先
   配置参数：-XX:+UseConcMarkSweepGC
   已默认无需配置的参数：-XX:+UseParNewGC(Parallel收集新生代) -XX:+CMSPermGenSweepingEnabled(CMS收集持久代) -XX:UseCMSCompactAtFullCollection(full gc时压缩年老代)
   初始效果：1g堆内存的新生代约60m，minor gc约5-20毫秒，full gc约130毫秒。
#### Parallel收集器：吞吐量优先
    配置参数： -XX:+UseParallelGC -XX:+UseParallelOldGC(Parallel收集年老代，从JDK6.0开始支持)
    已默认无需配置的参数： -XX:+UseAdaptiveSizePolicy(动态调整新生代大小)
    初始效果：1g堆内存的新生代约90-110m(动态调整)，minor gc约5-20毫秒，full gc有无UseParallelOldGC 参数分别为1.3/1.1秒，差别不大。
    另外-XX:MaxGCPauseMillis=100 设置minor gc的期望最大时间，JVM会以此来调整新生代的大小，但在此测试环境中对象死的太快，此参数作用不大。
### 4 调优实战
      Parallel收集高达1秒的暂停时间基本不可忍受，所以选择CMS收集器。
      在被压测的Mule 2.0应用里，每秒都有大约400M的海量短命对象产生：
- 因为默认60M的新生代太小了，频繁发生minor gc，大约0.2秒就进行一次。
- 因为CMS收集器中MaxTenuringThreshold(生代对象撑过过多少次minor gc才进入年老代的设置)默认0，存活的临时对象不经过Survivor区直接进入年老代，不久就占满年老代发生full gc。
     对这两个参数的调优，既要改善上面两种情况，又要避免新生代过大，复制次数过多造成minor gc的暂停时间过长。
- 使用-Xmn调到1/3 总内存。观察后设置-Xmn500M，新生代实际约460m。(用-XX:NewRatio设置无效，只能用 -Xmn)。
- 添加-XX:+PrintTenuringDistribution 参数观察各个Age的对象总大小，观察后设置-XX:MaxTenuringThreshold=5。
      优化后，大约1.1秒才发生一次minor gc，且速度依然保持在15-20ms之间。同时年老代的增长速度大大减缓，很久才发生一次full gc，
      参数定稿：
```java
-server -Xms1024m -Xmx1024m -Xmn500m -XX:+UseConcMarkSweepGC   -XX:MaxTenuringThreshold=5  -XX:+ExplicitGCInvokesConcurrent
```
      最后服务处理速度从1180 tps 上升到1380 tps，调整两个参数提升17%的性能还是笔很划算的买卖。
     另外，JDK6 Update 7自带了一个VisualVM工具，内里就是之前也有用过的Netbean Profiler，类似JConsole一样使用，可以看到线程状态，内存中对象以及方法的CPU时间等调优重要参考依据。免费捆绑啊，Sun 这样搞法，其他做Profiler的公司要关门了。
