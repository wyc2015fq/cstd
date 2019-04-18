# JVMGC——并发收集器（CMS） - z69183787的专栏 - CSDN博客
2018年12月07日 14:01:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：37
个人分类：[Jvm-Gc&垃圾回收](https://blog.csdn.net/z69183787/article/category/2917447)
# CMS（Concurrent Mark Sweep）收集器
C :  Concurrent
M :  标记（marking）对象 ：GC必须记住哪些对象可达，以便删除不可达的对象 
S :  清除（sweeping） ： 删除未标记的对象并释放它们的内存
CMS是一种以最短停顿时间为目标的收集器，使用CMS并不能达到GC效率最高，但它尽可能降低GC时服务的停顿时间。
使用标记-清除算法（Mark Sweep），在运行时会产生内存碎片
虚拟机提供了参数开启CMS收集结束后再进行一次内存压缩。
-XX:+UseConcMarkSweepGC
激活CMS收集器。默认HotSpot JVM使用的是并行收集器
|-XX:+UseCMSCompactAtFullCollection||
设置在垃圾收集器后是否需要一次内存碎片整理过程，仅在CMS收集器时有效
-XX:+CMSFullGCBeforeCompaction
设置CMS收集器在进行若干次垃圾收集后再进行一次内存碎片整理过程
通常与UseCMSCompactAtFullCollection参数一起使用
CMS垃圾回收整个过程分为六个步骤：
1. 初始标记 (CMS initial mark)  会STW(Stop The World)
为了收集应用程序的对象引用需要暂停应用程序线程
该阶段完成后，应用程序线程再次启动
2. 并发标记 (CMS concurrent mark)
从第一阶段收集到的对象引用开始，遍历所有其他的对象引用
3. 并发预清理(CMS-concurrent-preclean)
改变当运行第二阶段时，由应用程序线程产生的对象引用，以更新第二阶段的结果
4. 重新标记 (CMS remark)  会STW
由于第三阶段是并发的，对象引用可能会发生进一步改变，因此应用程序线程会再一次被暂停以更新这些变化
并且在进行实际的清理之前确保一个正确的对象引用视图
5. 并发清理 (CMS concurrent sweep)  
所有不再被引用的对象将从堆里清除掉
6. 并发重置：
收集器做一些收尾的工作，以便下一次GC周期能有一个干净的状态
[https://blog.csdn.net/hqq2023623/article/details/50993165](https://blog.csdn.net/hqq2023623/article/details/50993165)
CMS的两个挑战：
1. 堆碎片
CMS收集器默认并没有任何碎片整理的机制。所以可能会出现这样的情形：
即使总的堆大小远没有耗尽但却不能分配对象，仅仅是因为没有足够连续的空间完全容纳对象。
当这种事发生后，JVM会触发Full GC
2. 对象分配率高
获取对象实例的频率高于收集器清除堆里死对象的频率
并发模式失败： 老年代没有足够的可用空间来容纳一个从年轻代提升过来的对象
此时JVM会执行堆碎片整理：触发Full GC
当这些情形之一出现的时候，经常被证实是老年代有大量不必要的对象。
一个可行的办法： 增加年轻代的堆大小（增加Survior区的个数），以防止年轻代生命周期短的对象提前进入老年代。
其他方法： 利用jmap和jhat分析过度的对象分配，找出这些对象，最终减少这些对象的申请。
