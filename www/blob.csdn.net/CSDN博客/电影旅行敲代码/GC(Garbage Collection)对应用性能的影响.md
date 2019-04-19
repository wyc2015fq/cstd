# GC(Garbage Collection)对应用性能的影响 - 电影旅行敲代码 - CSDN博客
2015年12月01日 18:28:04[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：1945
正如我们所了解的，垃圾回收的性能并不是由垃圾对象的数量决定的，而是由可达对象的数量所决定。越多的对象变为垃圾，垃圾回收的速度越快。如果在堆上的对象都是待回收的垃圾对象，垃圾回收周期几乎可以说是瞬时完成的。另外垃圾回收器必须暂停应用程序的执行，以保证对象树的完整性。可达对象数量越多，暂停的时间越长，这会直接影响到系统的响应时间和吞吐量。
**译者注：**现在垃圾回收思想都是通过根集来推导可达对象，并将可达对象中所能索引到的对象加入可达对象集合，这是递归进行的
垃圾回收中暂停程序执行的特性称为**垃圾回收暂停**或者是**“GC暂停”**。在多线程应用中，GC暂停很快地导致扩展性问题。
![这里写图片描述](https://img-blog.csdn.net/20151201162546003)
Figure 2.4: This graphic from an Oracle GC Tuning article illustrates the performance hit of GC suspensions with increasing number of CPUs. (Source: [http://www.oracle.com/technetwork/java/javase/gc-tuning-6-140523.html](http://www.oracle.com/technetwork/java/javase/gc-tuning-6-140523.html))
图2.4向我们展示了GC暂停对多线程应用的吞吐量所造成的直接影响。在32位处理器系统上，如果应用垃圾回收的时间占到了执行时间的1%，会丢失20%的吞吐量。如果将GC时间增加到2%，那么总体吞吐量又会丢失另外的20%。这就是同时暂停32个执行线程所造成的影响。
这里有两种通用的方法用来减少GC暂停时间：
- 通过调整[标记-清除](http://my.oschina.net/winHerson/blog/114391)算法减少暂停时间。
- 限制标记的对象数量。
在应用这些能够提升垃圾回收性能的方法之前，你应该了解一下内存碎片，这些碎片会影响暂停时间和应用性能。
## 关于碎片化内存和寻找足够大空间
当我们在JAVA中创建一个新的对象时，JVM会在堆上自动分配一块足够装下该对象的内存块。
*译者注：[JVM内存模型](http://docs.oracle.com/javase/specs/jvms/se7/html/jvms-2.html#jvms-2.5)*
不断的分配和回收会导致内存碎片化，类似于硬盘中碎片。内存碎片会导致两个问题：
- 减少内存分配速度
- 分配出错
### 减少内存分配速度
JVM会创建一个根据内存块大小组织的空闲内存链表，然后通过该链表追踪空闲内存。为了创建一个新对象，Java会遍历整个链表去挑选并分配一个最佳大小的内存块。碎片化会拖慢分配过程，有效减缓了应用的执行。
### 分配出错
当碎片化严重到挑选不出一块足够大的内存，来容纳新对象时，会导致分配出错。
Java并没有依赖操作系统去解决这些问题，而是自己来解决。Java通过在垃圾回收的结尾执行内存压缩来避免内存碎片(图2.5)。这个过程非常想硬盘上的碎片整理。
![Figure 2.5: When the heap becomes fragmented due to repeated allocations and garbage collections, the JVM executes a compaction step, which aligns all objects neatly and closes all holes.](https://img-blog.csdn.net/20151201164859398)
Figure 2.5: When the heap becomes fragmented due to repeated allocations and garbage collections, the JVM executes a compaction step, which aligns all objects neatly and closes all holes.
**译者注：**标记并压缩的垃圾回收器，通过在堆区内移动可达对象以消除内存碎片(重新定位)。将所有可达对象放在一段连续的位置上可以减少内存空间的碎片，使得更容易存储较大的对象。同时，通过使数据**占用更少的缓存线和内存页，重新定位可以提高程序的时间局部性和空间局部性**，因为几乎同时创建的对象被分配在相邻的存储块中。如果这些相邻的块中的对象一起使用，可以利用数据预取技术
压缩技术简单的将可达对象移动到堆的一端，这样会有效地消除碎片化的空闲内存。对象也能够快速的进行分配，同时能够避免创建大对象时所遇到的问题。
这种技术的不足之处在于，会导致更长的垃圾回收时间，因为在压缩的过程中需要暂停程序的执行，这对程序性能的影响还是很严重的。
注：当前文章翻译自[这里](http://www.dynatrace.com/en/javabook/impact-of-garbage-collection-on-performance.html)
