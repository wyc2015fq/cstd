# 面试必备：CMS垃圾收集器与G1收集器 - Spark高级玩法 - CSDN博客
2018年03月27日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：2567
**1、CMS收集器**
  CMS收集器是一种以获取最短回收停顿时间为目标的收集器。基于“标记-清除”算法实现，它的运作过程如下：
1）初始标记
2）并发标记
3）重新标记
4）并发清除
  初始标记、从新标记这两个步骤仍然需要“stop the world”，初始标记仅仅只是标记一下GC Roots能直接关联到的对象，熟读很快，并发标记阶段就是进行GC Roots Tracing，而重新标记阶段则是为了修正并发标记期间因用户程序继续运作而导致标记产生表动的那一部分对象的标记记录，这个阶段的停顿时间一般会比初始标记阶段稍长点，但远比并发标记的时间短。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CTNjTuHibrZep15B7jCbXNm1wdJQ218aGNtnUdzp3xS0jAeAmdeB1lAiaw/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
  CMS是一款优秀的收集器，主要优点：并发收集、低停顿。
缺点：
1）CMS收集器对CPU资源非常敏感。在并发阶段，它虽然不会导致用户线程停顿，但是会因为占用了一部分线程而导致应用程序变慢，总吞吐量会降低。
2）CMS收集器无法处理浮动垃圾，可能会出现“Concurrent Mode Failure（并发模式故障）”失败而导致Full GC产生。
浮动垃圾：由于CMS并发清理阶段用户线程还在运行着，伴随着程序运行自然就会有新的垃圾不断产生，这部分垃圾出现的标记过程之后，CMS无法在当次收集中处理掉它们，只好留待下一次GC中再清理。这些垃圾就是“浮动垃圾”。
3）CMS是一款“标记--清除”算法实现的收集器，容易出现大量空间碎片。当空间碎片过多，将会给大对象分配带来很大的麻烦，往往会出现老年代还有很大空间剩余，但是无法找到足够大的连续空间来分配当前对象，不得不提前触发一次Full GC。
### **2、G1收集器**
G1是一款面向服务端应用的垃圾收集器。G1具备如下特点：
1、并行于并发：G1能充分利用CPU、多核环境下的硬件优势，使用多个CPU（CPU或者CPU核心）来缩短stop-The-World停顿时间。部分其他收集器原本需要停顿Java线程执行的GC动作，G1收集器仍然可以通过并发的方式让java程序继续执行。
2、分代收集：虽然G1可以不需要其他收集器配合就能独立管理整个GC堆，但是还是保留了分代的概念。它能够采用不同的方式去处理新创建的对象和已经存活了一段时间，熬过多次GC的旧对象以获取更好的收集效果。
3、空间整合：与CMS的“标记--清理”算法不同，G1从整体来看是基于“标记整理”算法实现的收集器；从局部上来看是基于“复制”算法实现的。
4、可预测的停顿：这是G1相对于CMS的另一个大优势，降低停顿时间是G1和ＣＭＳ共同的关注点，但Ｇ１除了追求低停顿外，还能建立可预测的停顿时间模型，能让使用者明确指定在一个长度为M毫秒的时间片段内，
5、G1运作步骤：
1、初始标记；2、并发标记；3、最终标记；4、筛选回收
上面几个步骤的运作过程和CMS有很多相似之处。初始标记阶段仅仅只是标记一下GC Roots能直接关联到的对象，并且修改TAMS的值，让下一个阶段用户程序并发运行时，能在正确可用的Region中创建新对象，这一阶段需要停顿线程，但是耗时很短，并发标记阶段是从GC Root开始对堆中对象进行可达性分析，找出存活的对象，这阶段时耗时较长，但可与用户程序并发执行。而最终标记阶段则是为了修正在并发标记期间因用户程序继续运作而导致标记产生变动的那一部分标记记录，虚拟机将这段时间对象变化记录在线程Remenbered Set Logs里面，最终标记阶段需要把Remembered Set Logs的数据合并到Remembered Set Logs里面，最终标记阶段需要把Remembered Set Logs的数据合并到Remembered Set中，这一阶段需要停顿线程，但是可并行执行。最后在筛选回收阶段首先对各个Region的回收价值和成本进行排序，根据用户所期望的GC停顿时间来制定回收计划。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVacn50auMODzWclnwQZ3CT6s3ic7AKaz9haicz5u528iaCzlzsGUCarGxcaZ4laHu5qzePRU3ricxZNA/640?wx_fmt=png)
尾部介绍几个名词：
**1，并行（Parallel）**：多个垃圾收集线程并行工作，此时用户线程处于等待状态 
**2，并发（Concurrent）**：用户线程和垃圾收集线程同时执行 
**3，吞吐量：**运行用户代码时间／（运行用户代码时间＋垃圾回收时间）
面试题：
吞吐量优先和响应优先的垃圾收集器如何选择？
**(1) 吞吐量优先的并行收集器 **
参数配置： 
1, -Xmx4g -Xms4g -Xmn2g -Xss200k -XX:+UseParallelGC -XX:ParallelGCThreads=8 
说明：选择Parallel Scavenge收集器，然后配置多少个线程进行回收，最好与处理器数目相等。
2，-Xmx4g -Xms4g -Xmn2g -Xss200k -XX:+UseParallelGC -XX:ParallelGCThreads=8 -XX:+UseParallelOldGC 
说明：配置老年代使用Parallel Old
3，-Xmx4g -Xms4g -Xmn2g -Xss200k -XX:+UseParallelGC -XX:MaxGCPauseMills=100 
说明：设置每次年轻代垃圾回收的最长时间。如何不能满足，那么就会调整年轻代大小，满足这个设置
4，-Xmx4g -Xms4g -Xmn2g -Xss200k -XX:+UseParallelGC -XX:MaxGCPauseMills=100 -XX:+UseAdaptiveSizePolicy 
说明：并行收集器会自动选择年轻代区大小和Survivor区的比例。
**(2)响应时间优先的并发收集器 **
1， -Xmx4g -Xms4g -Xmn2g -Xss200k -XX:+UseConcMarkSweepGC -XX:+UseParNewGC 
说明：设置老年代的收集器是CMS，年轻代是ParNew
2，-Xmx4g -Xms4g -Xmn2g -Xss200k -XX:+UseConcMarkSweepGC -XX:CMSFullGCsBeforeCompaction=5 -XX:+UseCMSCompactAtFullCollection 
说明：首先设置运行多少次GC后对内存空间进行压缩，整理。同时打开对年老代的压缩（会影响性能）
spark面试顺便问道，Spark Streaming应该选择何种垃圾收集器？
--driver-java-options和 spark.executor.extraJavaOptions这两个参数将driver和Executor垃圾回收器设置为cms，以提高响应速度。
**本文参考与网络，如有侵权请联系作者删除。**
http://blog.csdn.net/linhu007/article/details/48897597
http://blog.csdn.net/KilluaZoldyck/article/details/75081875
**推荐阅读：**
1，[浅析Java中的final关键字](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484460&idx=1&sn=3a6920291100ea83c5ccf1867ff5349a&chksm=9f38e704a84f6e12ec015f8f7ded1c2c9a25e495c07b8227f347287ab241f027f4beabed8e50&scene=21#wechat_redirect)
2，[Java多线程和并发基础面试问答](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484490&idx=1&sn=9ea4371525ad4b91107a950f5be766c6&chksm=9f38e762a84f6e740be8c2265f6245857eb437fe09a098d9766d4460653e7a4c5a042dcd3c20&scene=21#wechat_redirect)
3，[Java反射机制深入详解](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484454&idx=1&sn=f5e1d27d311011df0663bb0dd16a7ec2&chksm=9f38e70ea84f6e18d0be5a310e45bab759b0681fe990aa7f616f3531e6c70c4354f5d60449bd&scene=21#wechat_redirect)
4，[spark调优系列之内存和GC调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483782&idx=1&sn=52059883b33cb97005186b0cba4e5f5a&chksm=9f38e2aea84f6bb8230189ee4e3de9cc7e11497f20f6cd498e540ef7f713478481c830106f51&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
