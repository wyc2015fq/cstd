# Java深入 - 触发Full GC执行的情况 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年05月04日 17:37:04[initphp](https://me.csdn.net/initphp)阅读数：1610








触发FULL GC的四种情况

#### 1. ****System.gc()方法调用

一般情况下禁止使用此方法，让JVM自己决定FULL GC即可。


#### 2.老生代空间不足

老生代空间只有在新生代对象转入及创建为大对象、大数组时才会出现不足的现象，会出现FULL GC情况。


当执行Full GC后空间仍然不足，则抛出如下错误：

java.lang.OutOfMemoryError: Java heap space 

为避免以上两种状况引起的FullGC，调优时应尽量做到让对象在新生代Minor GC阶段被回收、让对象在新生代多存活一段时间及不要创建过大的对象及数组。




#### 3. Permanet Generation 永久代空间满

PermanetGeneration永久代（方法区）中存放的为一些class的信息等，当系统中要加载的类、反射的类和调用的方法较多时，Permanet Generation可能会被占满，在未配置为采用CMS GC的情况下会执行Full GC。如果经过Full GC仍然回收不了，那么JVM会抛出如下错误信息：

java.lang.OutOfMemoryError: PermGen space 

为避免Perm Gen占满造成Full GC现象，可采用的方法为增大Perm Gen空间或转为使用CMS GC。




#### 4. CMS GC时出现promotion failed和concurrent mode failure

对于采用CMS进行老生代GC的程序而言，尤其要注意GC日志中是否有promotion failed和concurrent mode failure两种状况，当这两种状况出现时可能会触发Full GC。

promotionfailed是在进行Minor GC时，survivor space放不下、对象只能放入老生代，而此时老生代也放不下造成的；

concurrent mode failure是在执行CMS GC的过程中同时有对象要放入老生代，而此时老生代空间不足造成的。

应对措施为：增大survivorspace、老生代空间或调低触发并发GC的比率，但在JDK 5.0+、6.0+的版本中有可能会由于JDK的bug29导致CMS在remark完毕后很久才触发sweeping动作。对于这种状况，可通过设置-XX:CMSMaxAbortablePrecleanTime=5（单位为ms）来避免。




#### 4.统计得到的Minor GC晋升到老生代的平均大小大于老生代的剩余空间

这是一个较为复杂的触发情况，Hotspot为了避免由于新生代对象晋升到老生代导致旧生代空间不足的现象，在进行Minor GC时，做了一个判断，如果之前统计所得到的Minor GC晋升到老生代的平均大小大于老生代的剩余空间，那么就直接触发Full GC。

例如程序第一次触发MinorGC后，有6MB的对象晋升到旧生代，那么当下一次Minor GC发生时，首先检查老生代的剩余空间是否大于6MB，如果小于6MB，则执行Full GC。




### 尽量避免FULLGC的方法

#### 1. 增大old老年代内存空间

#### 2. 尽可能减少young年轻代到老年代。调整晋升年龄-XX:PretenureSizeThreshold 和Survrvor的大小。

#### 3. 优化系统性能



