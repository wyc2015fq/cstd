# Java虚拟机解析篇之---垃圾回收器 - 零度的博客专栏 - CSDN博客
2017年03月07日 10:46:11[零度anngle](https://me.csdn.net/zmx729618)阅读数：221
上一篇说了虚拟机的内存模型，在说到堆内存的时候我们提到了，堆内存是[Java](http://lib.csdn.net/base/javase)内存中区域最大的一部分，而且垃圾回收器主要就是回收这部分的内容。那么这篇就来介绍一下垃圾回收器的原理以及回收的[算法](http://lib.csdn.net/base/datastructure)。
Java中的垃圾回收器(GC)是Java中比较有特色的一点，不需要我们手动的去管理一个对象，不想C++中的构造函数和析构函数一样，需要程序猿自己去手动的管理，很容易造成内存泄露的问题。当然如果学过O[C语言](http://lib.csdn.net/base/c)的话，我们知道OC语言中有自动释放池的概念，当然我们使用retain/release进行手动管理对象的。所以从这方面我们可以看到[Android](http://lib.csdn.net/base/android)(Java)在这方面和[iOS](http://lib.csdn.net/base/ios)(OC)相比的话，比较卡，因为Java中的垃圾回收器是需要算法计算的，这个可能会有点耗时，但是好处就是不需要人工管理。但是OC是需要手动管理的，这样系统就不需要复杂的算法去进行管理，运行速度就比[Android](http://lib.csdn.net/base/15)流畅(当然IOS比Android流畅，有很多原因的，这个只是一方面内容)
# 一、Java中是如何管理对象的
在堆中存放着Java世界中几乎所有的对象实例，垃圾收集器在对堆进行回收前，第一件事就是要确定这些对象之中哪些还“存活”着，哪些已经“死去”
这里就介绍两种方式来管理对象
## 第一、引用技术算法
这种算法很简单的，而且也是比较常用的一种方式管理对象了
给对象中添加一个引用计数器，每当有一个地方引用它时，计数器的值就加1，当引用失效时，计数器的值就减1，任何时刻计数器为0的对象就是不可能在被使用了，这种算法是很简单的，而且早期很多面向对象语言中都采用这种方式，但是现在主流的Java虚拟机中并没有采用这种方式来管理对象，其原因**最主要的原因是它很难解决对象之间的相互循环引用**。例子：
**[java]**[view
 plain](http://blog.csdn.net/jiangwei0910410003/article/details/40709457#)[copy](http://blog.csdn.net/jiangwei0910410003/article/details/40709457#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/505304/fork)
- publicclass Demo {  
- public Object instance = null;  
- publicstaticvoid main(String[] args){  
-         Demo demo1 = new Demo();  
-         Demo demo2 = new Demo();  
-         demo1.instance = demo2;  
-         demo2.instance = demo1;  
- //现在demo1,demo2设置null,那么demo1,demo2能否被回收呢？
-         demo1 = null;  
-         demo2 = null;  
- //手动的调用垃圾回收器回收
-         System.gc();  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
Demo对象中有一个变量instance，当我们new了两个Demo对象的时候，彼此相关联，当我们将他们设置null,实际上这两个对象已经不可能被访问，但是他们因为互相引用着对方，导致它们的引用计数不为0，于是引用计数算法无法通知GC收集器回收它们。这有点类似于并发中的死锁。
所以解决上面存在的问题，第二种方式就出现了。
## 第二、可达性分析算法
这个算法的基本思路就是通过一系列的称谓“GC Roots"的对象作为起始点，从这些节点开始向下搜索，搜索所有走过的路径为引用链，当一个对象到GC Roots没有任何引用链项链时，则证明此对象时不可用的，下面看一下例子：
![](https://img-blog.csdn.net/20141102164158981?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
上面的这张图，对象object5、object6、object7虽然互相没有关联，但是它们到GC Roots是不可达的，所以它们将会被判定为是可回收的对象
注：Java语言中，可作为GC Roots的对象包括下面几种：
1) 虚拟机栈(栈帧中的本地变量表)中引用的对象
2) 方法区中类静态属性引用的对象
3) 方法区中常量引用的对象
4) 本地方法栈中JNI(即一般说的Native方法)引用的对象
# 二、Java中的引用类型
从JDK1.2之后，Java对引用的概念进行了扩充，将引用分为强引用，软引用，弱引用，虚引用，这四种引用的强度一次逐渐减弱
1) **强引用**就是指在程序代码之中普遍存在的，类似 “Object obj = new Object()” 这类的引用，只要强引用还存在，垃圾回收器永远不会回收掉被引用的对象。
2) **软引用**是用来描述一些还有用但并非需要的对象，对于软引用关联着的对象，在系统将要发生内存异常之前，将会把这些对象列进回收范围之中进行第二次回收，如果这次回收还没有足够的内存，才会抛出内存异常
3) **弱引用**也是用来描述非必需对象的，但是它的强度比软引用更弱一些，被弱引用关联的对象只能生存岛下一次垃圾收集发生之前，当垃圾收集器工作时，无论当前内存释放足够，都会回收掉只被弱引用关联的对象
4) **虚引用**也称为幽灵引用或者幻影引用，它是最弱的一种引用关系，一个对象是否有虚引用的存在，完全不会对其生存时间构成影响，也无法通过虚引用来取得一个对象实例，对一个对象设置虚引用关联的唯一目的就是能在这个对象被收集器回收时收到一个系统通知
# 三、垃圾收集算法
## 1) 标记-清除算法
最基础的收集算法是“标记-清除”（Mark-Sweep）算法，如它的名字一样，算法分为“标记”和“清除”两个阶段：首先标记出所有需要回收的对象，在标记完成后统一回收掉所有被标记的对象，它的标记过程其实在前一节讲述对象标记判定时已经基本介绍过了。之所以说它是最基础的收集算法，是因为后续的收集算法都是基于这种思路并对其缺点进行改进而得到的。它的主要缺点有两个：一个是效率问题，标记和清除过程的效率都不高；另外一个是空间问题，标记清除之后会产生大量不连续的内存碎片，空间碎片太多可能会导致，当程序在以后的运行过程中需要分配较大对象时无法找到足够的连续内存而不得不提前触发另一次垃圾收集动作。
 标记-清除算法的执行过程如图
![](https://img-blog.csdn.net/20141102170033015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 2) 复制算法
为了解决效率问题，一种称为“复制”（Copying）的收集算法出现了，它将可用内存按容量划分为大小相等的两块，每次只使用其中的一块。当这一块的内存用完了，就将还存活着的对象复制到另外一块上面，然后再把已使用过的内存空间一次清理掉。这样使得每次都是对其中的一块进行内存回收，内存分配时也就不用考虑内存碎片等复杂情况，只要移动堆顶指针，按顺序分配内存即可，实现简单，运行高效。只是这种算法的代价是将内存缩小为原来的一半，未免太高了一点。复制算法的执行过程如图
![](https://img-blog.csdn.net/20141102170015242?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 3) 标记-整理算法
复制收集算法在对象存活率较高时就要执行较多的复制操作，效率将会变低。更关键的是，如果不想浪费50%的空间，就需要有额外的空间进行分配担保，以应对被使用的内存中所有对象都100%存活的极端情况，所以在老年代一般不能直接选用这种算法。
根据老年代的特点，有人提出了另外一种“标记-整理”（Mark-Compact）算法，标记过程仍然与“标记-清除”算法一样，但后续步骤不是直接对可回收对象进行清理，而是让所有存活的对象都向一端移动，然后直接清理掉端边界以外的内存，“标记-整理”算法的示意图如图
![](https://img-blog.csdn.net/20141102170022215?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 4) 分代收集算法
当前商业虚拟机的垃圾收集都采用“分代收集”（Generational Collection）算法，这种算法并没有什么新的思想，只是根据对象的存活周期的不同将内存划分为几块。一般是把Java堆分为新生代和老年代，这样就可以根据各个年代的特点采用最适当的收集算法。在新生代中，每次垃圾收集时都发现有大批对象死去，只有少量存活，那就选用复制算法，只需要付出少量存活对象的复制成本就可以完成收集。而老年代中因为对象存活率高、没有额外空间对它进行分配担保，就必须使用“标记-清理”或“标记-整理”算法来进行回收
# 四、Java虚拟机对堆的内存细化的几个区域，并且这些区域都是采用哪些收集算法。
![](https://img-blog.csdn.net/20141102173507268?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhbmd3ZWkwOTEwNDEwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
JVM内存模型中分两大块，一块是**New Generation**, 另一块是**Old Generation**. 在New Generation中，有一个叫Eden的空间，主要是用来存放新生的对象，还有两个Survivor Spaces（from,to）, 它们用来存放每次垃圾回收后存活下来的对象。在Old Generation中，主要存放应用程序中生命周期长的内存对象，还有个**Permanent Generation**，主要用来放JVM自己的反射对象，比如类对象和方法对象等。
1) 在New Generation块中，垃圾回收一般用复制算法，速度快。每次GC的时候，存活下来的对象首先由Eden拷贝到某个Survivor Space, 当Survivor Space空间满了后, 剩下的live对象就被直接拷贝到Old Generation中去。因此，每次GC后，Eden内存块会被清空
2) 在Old Generation块中，垃圾回收一般用标记整理的算法，速度慢些，但减少内存要求.
垃圾回收分多级，0级为全部(Full)的垃圾回收，会回收Old段中的垃圾；1级或以上为部分垃圾回收，只会回收New中的垃圾，内存溢出通常发生于Old段或Perm段垃圾回收后，仍然无内存空间容纳新的Java对象的情况。
Out Of Memory 只发生在jvm对old和perm generation 回收后还不能获足够内存的情况.
当生成一个新对象时，内存申请过程如下：
A. JVM会试图为相关Java对象在Eden中初始化一块内存区域
B. 当Eden空间足够时，内存申请结束。否则到下一步
C. JVM试图释放在Eden中所有不活跃的对象（这属于1或更高级的垃圾回收）, 释放后若Eden空间仍然不足以放入新对象，则试图将部分Eden中活跃对象放入Survivor区
D. Survivor区被用来作为Eden及Old的中间交换区域，当Old区空间足够时，Survivor区的对象会被移到Old区，否则会被保留在Survivor区
E. 当Old区空间不够时，JVM会在Old区进行完全的垃圾收集（0级）
F. 完全垃圾收集后，若Survivor及Old区仍然无法存放从Eden复制过来的部分对象，导致JVM无法在Eden区为新对象创建内存区域，则出现”out of memory错误”
**造成full gc的原因**
new了很多对象,没有即时在主动释放掉->Eden内存不够用->不断把对象往old迁移->old满了->full gc
总结：上面的内容就介绍了Java虚拟机如何管理对象的，我们也看到了上面主要就是收集算法和堆空间的从新划分，这样做的目的都是在于垃圾回收的高效执行，但是总归看来，如果对象交给系统来管理，在系统运行的过程效率肯定会有影响的，但是这有一点比较好，就是不需要手动管理，给程序猿带来方便。
