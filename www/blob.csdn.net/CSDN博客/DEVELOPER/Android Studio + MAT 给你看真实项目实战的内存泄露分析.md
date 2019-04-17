# Android Studio + MAT 给你看真实项目实战的内存泄露分析 - DEVELOPER - CSDN博客





2018年03月28日 18:00:06[学术袁](https://me.csdn.net/u012827205)阅读数：5190








![这里写图片描述](https://img-blog.csdn.net/20180328112534464?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180328112641657?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180328112650958?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

先贴出我要进行实战的项目背景；项目已经出炉快半年时间了，现在要对它进行性能上的优化，这时候就要使用到 MAT 。然后随便记录一下我的分析历程。
***首先要了解两个概念：内存溢出和内存泄漏***
**内存溢出**(`out of memory`)，是指程序在申请内存时，没有足够的内存空间供其使用，出现out of memory；比如申请了一个integer,但给它存了long才能存下的数，那就是内存溢出。 
**内存泄露**(`memory leak`)，是指程序在申请内存后，完事儿之后，仍被其他对象持有其引用，而无法释放已申请的内存空间，一次内存泄露危害可以忽略，但内存泄露堆积后果很严重，最终会导致内存溢出。
**分析内存溢出问题，首先我们要非常清楚怎么去查找导致内存空间溢出的问题的操作！！！**

就使用`MAT` 来讲，如果你从A页面进入`B页面`在进入`C页面`。然后你会在`Monitors`中看到内存递增，觉得不正常。下意识中想要分析一下`B和C页面`的一些代码逻辑是否出现了内存溢出。此时你却在`C页面`就一直疯狂的`GC`，然后再拿到`.hprof`文件进行分析。这种操作是错误的。为什么？因为你要分析的是应用的`B和C页面`可能出现的内存泄漏，真正的分析内存泄漏的关键操作是，你至少要逐次关闭了`C和B页面`。然后`GC`，再然后拿到`.hprof`文件去进行分析。 
**`总结语：也就是说，查找内存泄露是要在一个对象在你认为中已经被释放了，并且你也这样操作了（比如关闭了页面，停止了线程）。但是在分析时却发现，这个狗日的对象还活着，且被别的对象持有者引用。这个时候的分析才算是到了时机。`**
先挑选两个页面进入并频刷，得内存分析，看图 
![这里写图片描述](https://img-blog.csdn.net/20180328123237513?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**操作过程**
![这里写图片描述](https://img-blog.csdn.net/20180328123835511?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

此时，我就打开应用，在首页和由首页进入的第二级页面进行频刷，然后关闭回到首页面。看到内存呈现阶梯状增涨，并伴有内存抖动现象。然后开始操作，点击`Initate GC 按钮` 释放内存，经过3秒钟左右看下内存状态显示，再次`GC` 然后点击`Dimp Java Heap 按钮`。然后，就会看到这样的效果： 
***注意：***一定要手动点击 `Initiate GC`按钮手动触发`GC`，这样抓到的内存使用情况才是不包括`Unreachable`对象的。`Unreachable`指的是可以被垃圾回收器回收的对象，但是由于没有`GC`发生，没有释放，这时抓的`Unreachable` 就存在正在使用的内存中。 
![这里写图片描述](https://img-blog.csdn.net/20180328125418929?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

于是就会看到 `Java Heap` 文件，点击`Analyzer Tasks` 按钮，呈现分析窗口。并执行`Perform Analysis`就能对我们刚才进行`GC` 的那一段内存状态进行分析，并呈现出分析的结果。 
![这里写图片描述](https://img-blog.csdn.net/20180328125934937?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

由图片我们看到了分析结果`Analysis Results` 出现`Leaked Activities` 内存泄漏的根源竟然是启动页面 `SplashAty` 这个Activity页面。 

接下来还要继续深入分析内存泄漏的原因 

在`Captures` 窗口右击刚`Dump Java Heap` 导出标准的`.hprof` 文件，导出到任意位置并使用基于`Eclipse` 的 [MAT](https://www.eclipse.org/mat/downloads.php) 工具打开。 然后可以看见初始打开的页面结果 
![这里写图片描述](https://img-blog.csdn.net/20180328140947340?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在`Eclipase` 的 `MAT` 显示的页面点击 `Histogram` 显示 
![这里写图片描述](https://img-blog.csdn.net/20180328153747876?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后输入您需要定位的`Activity`页面的名字，查看该`Activity`所发生的内存泄漏的源头页面 
![这里写图片描述](https://img-blog.csdn.net/20180328153927388?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

或者点击`QQL`图标 输入 `select * from instanceof android.app.Activity` ，然后点击`红色叹号`执行，结果是这样的 
![这里写图片描述](https://img-blog.csdn.net/20180328171641685?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

选中ta并作这样的操作，就能够看到具体发生内存泄漏的位置 
![这里写图片描述](https://img-blog.csdn.net/20180328154126373?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最后得到这样的结果 
![这里写图片描述](https://img-blog.csdn.net/20180328154841776?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后结合代码分析并解决问题！！至于还有很多可做的一些操作，那就需要自己去发现。 

接下来，做一些概念上的介绍。毕竟只会操作还是远远不够的，逻辑会很复杂，泄露也会频发，查找也会变得千奇百怪。只有全方位的了解了`MAT` 才能做到有备无患，仗剑走天涯！
**Histogram**
- 列出内存中的对象，对象的个数以及大小。

**Dominator Tree**
- 列出最大的对象以及其依赖存活的Object （大小是以Retained Heap为标准排序的）。

**Top Consumers**
- 通过图形列出最大的object。

**Shallow heap**
- 无序列表1 Shallow size就是对象本身占用内存的大小，不包含其引用的对象。针对非数组类型的对象，它的大小就是对象与它所有的成员变量大小的总和。当然这里面还会包括一些java语言特性的数据存储单元。针对数组类型的对象，它的大小是数组元素对象的大小总和。

**Retained Heap**
- 它表示如果一个对象被释放掉，那会因为该对象的释放而减少引用进而被释放的所有的对象（包括被递归释放的）所占用的heap大小。(间接引用的含义：A->B->C, C就是间接引用。不过，释放的时候还要排除被GC Roots直接或间接引用的对象。他们暂时不会被被当做Garbage）

资料参考： 
[http://ju.outofmemory.cn/entry/172684](http://ju.outofmemory.cn/entry/172684)
[http://ju.outofmemory.cn/entry/204399](http://ju.outofmemory.cn/entry/204399)
[https://blog.csdn.net/itachi85/article/details/77075455](https://blog.csdn.net/itachi85/article/details/77075455)
[https://blog.csdn.net/yxz329130952/article/details/50288145](https://blog.csdn.net/yxz329130952/article/details/50288145)








