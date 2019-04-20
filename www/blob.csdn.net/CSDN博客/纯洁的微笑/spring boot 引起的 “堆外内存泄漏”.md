# spring boot 引起的 “堆外内存泄漏” - 纯洁的微笑 - CSDN博客
2018年11月13日 09:09:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：2281
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnpmTjI3DOusKLycUMM10FToIo50kibGKNiafFXYC7adFY7GUFaU0OjTwpicX8d6sLI0y0NJIzPQ2Uycw/640?wx_fmt=jpeg)
**背景**
组内一个项目最近一直报swap区域使用过高异常，笔者被叫去帮忙查看原因。
发现配置的4G堆内内存，但是实际使用的物理内存高达7G，确实有点不正常，JVM参数配置是“-XX:MetaspaceSize=256M -XX:MaxMetaspaceSize=256M -XX:+AlwaysPreTouch -XX:ReservedCodeCacheSize=128m -XX:InitialCodeCacheSize=128m, -Xss512k -Xmx4g -Xms4g,-XX:+UseG1GC -XX:G1HeapRegionSize=4M”，但是使用的虚拟内存和物理内存使用情况如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFcvCfbt9KKF9VZEnOGfYLu72HoGrzc4SJGFPVeM3WZjLRtlAicjVV3Dg/640?wx_fmt=png)
**排查过程**
**步骤一：先使用java层面的工具定位是不是堆内内存、code区域或者使用unsafe.allocateMemory和DirectByteBuffer申请的堆外内存**
笔者在项目中添加“-XX:NativeMemoryTracking=summary ”JVM参数重启项目，查看查到的内存分布如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFEwO2D9nMWPgicFga9Yf5E2e99K7ZACyoGc6xCMKluEIQtfYIIicL8r6Q/640?wx_fmt=png)
发现这个命令显示的committed的远内存小于物理内存。
因为之前就对NativeMemoryTracking有所了解和测试，知道NativeMemoryTracking可以追踪到堆内内存、code区域、通过unsafe.allocateMemory和DirectByteBuffer申请的内存，但是追踪不到其他native code（c代码）申请的堆外内存。这一步也可以使用arthas去查看：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFhnAzmiaqHTgI8nmdYQPPGGOOn0nk3uibfuKH3ALDALMW68sN6IVAAQKg/640?wx_fmt=png)
为了防止误判，笔者适应了pmap查看内存分布，发现大量的64M的地址，而这些地址空间不在NativeMemoryTracking所给出的地址空间里面。基本上就断定就是这些64M的内存导致的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFwcnOHkzrib9V37qfiaicPCiakh37Efud9IzhUHRmUtcqpq5mloAZAfic57A/640?wx_fmt=png)
**步骤二：使用系统层面的工具定位堆外内存**
因为基本上确定是native code引起之后，java层面的工具基本上就失效了，只能使用系统层面的工具去查找问题。首先使用了gperftools去查看，截图如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFFaJe8FNvWzODI36LCL2BZloiaziaaibvgKqbpe8wkYe2Sq4Q2yXhkytMw/640?wx_fmt=png)
上图可以看出，使用malloc申请的的内存最高到3G之后就释放了，之后始终维持在700M-800M。第一反应就是难道native code 中没有使用malloc申请，直接使用mmap/brk申请的？（gperftools原理就使用动态链接的方式替换了操作系统默认的内存分配器（glibc））
直接使用strace对mmap/brk进行追踪发现，并没有申请内存，此时陷入了比较迷茫的状态。
于是想着能不能看看内存里面是啥东西，就用gdb去dump这些64M的内存下来看看，内容如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCF6qzO7hrVvp5RM9bEfoUoAJ3aTw7dRR5Q9atKVibVUrsBO0U6OKJ05Aw/640?wx_fmt=png)
从内容上来看像解压后的jar信息。读取jar信息应该是在项目启动的时候，那么在项目启动之后使用strace作用就不是很大了，于是在项目启动的时候就使用strace，发现确实申请了很多64M内存空间，截图如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFfVUScWlAWGJvibicWTwHq2neB0gA7lClricx0S5VQzlxMYegSVic8kJyaw/640?wx_fmt=png)
使用该mmap申请的地址空间在pmap对应如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCF1BTLQWsu5B8TJpOmmGzMvh22AkI8mkOicLUcfEAYdwjOysm9ic2ShPAQ/640?wx_fmt=png)
根据strace显示的线程Id，去jstack一下java进程，找到线程栈如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFyaqN2uH6CFlO0svf46Qhjic6ibn1iawWYFeQHWczicmib7oaxpDL2oWGuQw/640?wx_fmt=png)
这里基本上就可以看出问题来了，这里使用了Reflections进行扫包，底层使用了spring boot loader去加载了jar。因为需要解压jar肯定需要Inflater类，这个需要用到堆外内存，然后使用btrace去追踪这个方法如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFOXAFGkx6JDQFoKBn1nn3voJCFtz1Pf7556pFoaTeZxQk6aeuq3icMKw/640?wx_fmt=png)
在代码中找到扫包的地方，发现没有配置扫包路径，默认的是扫描所有jar，修改为扫描特定的jar路径。上线测试，内存正常，问题修复。
**步骤三：为什么堆外内存没有释放掉呢**
到步骤二的时候，问题已经解决了，但是有几个疑问：
- 
为什么堆外内存没有释放
- 
为什么内存大小都是64M，jar大小不可能这么大，而且都是一样大
- 
为什么gperftools最终显示使用的的内存大小是700M左右，解压包真的没有使用malloc申请内存吗？
直接看了一下spring boot loader那一块源码，发现spring对jdk的JarFile的进行了包装。他使用Inflater却没有手动去释放，依赖于Inflater中的finalize机制，在gc的时候释放。于是怀疑gc的时候没有调用finalize。
带着这样的怀疑，我把Inflater进行包装在spring loader里面替换成我包装的Inflater，在finalize进行打点监控，发现finalize在young gc 的时候确实被调用了啊。去看了一下Inflater对应的C代码，初始化的使用了malloc 申请内存，调用end的时候调用了free去释放内存了。
于是怀疑free的时候没有真正释放内存。然后想着把spring boot包装JarFile 替换成jdk 自带的 JarFile，发现替换之后内存问题解决。然后再返过来看gperftools的内存分布情况。发现使用spring loader的时候，内存使用一直在增加，突然某个点内存使用下降了好多。
这个点应该就是gc引起的，内存应该释放了。但是操作系统层面没有看到内存变化，怀疑没有释放到操作系统，被内存分配器持有了。
发现和不使用gperftools内存地址分布差别很明显，2.5G地址使用smaps发现他是属于native stack。物理内存地址分布如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFNMdcQ2TQ7udJpUJ38ic9ZNJtwKbQP1uwxaAZo6Tjcdj23sFibQp6JlbQ/640?wx_fmt=png)
到此基本上可以确定是内存分配器在捣鬼，搜索了一下glibc 64M，发现从glibc 从2.11 开始对每个线程引入内存池（64位机器大小就是64M内存），原文如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFtYjVQleoDU5246UzcjEibDY670Yu0IN4h6iayk1AYUiaQFARnNvfjadKw/640?wx_fmt=png)
按照文中所说去修改MALLOC_ARENA_MAX环境变量，发现没什么效果，去查看tcmalloc（gperftools使用的内存分配器）也使用了内存池方式。
因为glibc 内存分配器代码太多，懒得去看，为了验证就自己简单写个内存分配器。使用动态链接替换掉glibc 的内存分配器，代码如下（因为都是从main中分配内存，没有考虑线程安全，realloc，calloc代码类似没截图了）：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFXMHn5cwCPqswkYOCoULNZu2W2OlOFMUMzkGeUHTYCuTzv8ZNbPg8Ag/640?wx_fmt=png)
通过在自定义分配器当中埋点可以发现其实程序启动之后程序实际申请的堆外内存其实始终在700M-800M之前，tcmalloc 也有相关埋点也是在700M-800M左右。但是从操作系统角度来看进程占用的内存差别很大（这里只是监控堆外内存）。
笔者做了一下测试，使用不同分配器进行不同程度的扫包，占用的内存如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFXGZ1MJKCiaI3uUy706GXF3cJT8icFDs3kga0yfDakwibFdia7UqyOtYcVQ/640?wx_fmt=png)
为什么自定义的malloc 申请800M，最终占用的物理内存在1.7G呢？
因为自定义内存采用的是mmap分配内存，mmap分配内存的单位是page，也就是page的整数倍，笔者使用的系统pagesize=4k，也就说如果用户申请了1一个字节，也会分配一个page，存在着巨大的空间浪费，可以通过埋点查看系统申请了多少页。埋点发现最终在536k左右吧。
那实际上向系统申请的内存 = 512k * 4k = 2G，为什么这个数据由大于1.7G内，因为操作系统采取的是延迟加载的方式，也就是说通过mmap向系统申请内存的时候系统仅仅返回地址并没有分配真实的物理地址，只有在使用的时候系统产生一个缺页中断然后在加载这个page到内存当中，这也是使用pmap看到的物理和虚拟内存的区别。
*总结*
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PxbsyCwaqiatN93jdPuBNf4ic6hOnQWtCFOBO3OnXknUP02rls3mLCIZibISPxic8BQhpREhPTVLRKS4lY306ygDZA/640?wx_fmt=png)整个内存分配的流程如上图。在扫描包的时候，spring loader不会主动去释放堆外内存，导致在扫描过程中，堆外内存占用量一直持续飙升。当发生gc 的时候会依赖于finalize机制一并去释放了堆外内存。但是glibc为了性能考虑，并没有真正把内存归返到操作系统，而是留下来当做内存池了，导致应用层以为发生了“内存泄漏”。
补充一下：定位问题用的工具有：top、jstack、arthas、pmap，gperftools，btrace、strace、gdb等。
-END-
**纯洁的微笑**
**一个有故事的程序员**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqjV7GOKB2htgfZjgMjqxftxfmmdrLiaMKpyicTmLLX5fUjb6YxA6Z5Bhcozb3p0uMV7wqdKED89HZA/640?wx_fmt=jpeg)
