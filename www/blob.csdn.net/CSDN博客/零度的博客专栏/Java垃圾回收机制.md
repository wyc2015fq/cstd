# Java垃圾回收机制 - 零度的博客专栏 - CSDN博客
2016年09月22日 16:08:40[零度anngle](https://me.csdn.net/zmx729618)阅读数：249标签：[垃圾回收机制](https://so.csdn.net/so/search/s.do?q=垃圾回收机制&t=blog)
个人分类：[Java GC](https://blog.csdn.net/zmx729618/article/category/6422060)
                
说到垃圾回收（Garbage Collection，GC），很多人就会自然而然地把它和Java联系起来。在Java中，程序员不需要去关心内存动态分配和垃圾回收的问题，这一切都交给了JVM来处理。顾名思义，垃圾回收就是释放垃圾占用的空间，那么在Java中，什么样的对象会被认定为“垃圾”？那么当一些对象被确定为垃圾之后，采用什么样的策略来进行回收（释放空间）？在目前的商业虚拟机中，有哪些典型的垃圾收集器？下面我们就来逐一探讨这些问题。以下是本文的目录大纲：
　　一.如何确定某个对象是“垃圾”？
　　二.典型的垃圾收集算法
　　三.典型的垃圾收集器
　　如果有不正之处，希望谅解和批评指正，不胜感激。
　　请尊重作者劳动成果，转载请标明原文链接：
[http://www.cnblogs.com/dolphin0520/p/3783345.html](http://www.cnblogs.com/dolphin0520/p/3783345.html)
## 一.如何确定某个对象是“垃圾”？
　　在这一小节我们先了解一个最基本的问题：如果确定某个对象是“垃圾”？既然垃圾收集器的任务是回收垃圾对象所占的空间供新的对象使用，那么垃圾收集器如何确定某个对象是“垃圾”？—即通过什么方法判断一个对象可以被回收了。
　　在java中是通过引用来和对象进行关联的，也就是说如果要操作对象，必须通过引用来进行。那么很显然一个简单的办法就是通过引用计数来判断一个对象是否可以被回收。不失一般性，如果一个对象没有任何引用与之关联，则说明该对象基本不太可能在其他地方被使用到，那么这个对象就成为可被回收的对象了。这种方式成为引用计数法。
　　这种方式的特点是实现简单，而且效率较高，但是它无法解决循环引用的问题，因此在Java中并没有采用这种方式（[Python](http://lib.csdn.net/base/11)采用的是引用计数法）。看下面这段代码：
```
```java
public
```
```java
class
```
```java
Main
 {
```
```java
```
```java
public
```
```java
static
```
```java
void
```
```java
main(String[]
 args) {
```
```java
```
```java
MyObject
 object1 =
```
```java
new
```
```java
MyObject();
```
```java
```
```java
MyObject
 object2 =
```
```java
new
```
```java
MyObject();
```
```java
```
```java
```
```java
object1.object
 = object2;
```
```java
```
```java
object2.object
 = object1;
```
```java
```
```java
```
```java
object1
 =
```
```java
null
```
```java
;
```
```java
```
```java
object2
 =
```
```java
null
```
```java
;
```
```java
```
```java
}
```
```java
}
```
```java
class
```
```java
MyObject{
```
```java
```
```java
public
```
```java
Object
 object =
```
```java
null
```
```java
;
```
```java
}
```
```
　　最后面两句将object1和object2赋值为null，也就是说object1和object2指向的对象已经不可能再被访问，但是由于它们互相引用对方，导致它们的引用计数都不为0，那么垃圾收集器就永远不会回收它们。
　　为了解决这个问题，在Java中采取了 可达性分析法。该方法的基本思想是通过一系列的“GC Roots”对象作为起点进行搜索，如果在“GC Roots”和一个对象之间没有可达路径，则称该对象是不可达的，不过要注意的是被判定为不可达的对象不一定就会成为可回收对象。被判定为不可达的对象要成为可回收对象必须至少经历两次标记过程，如果在这两次标记过程中仍然没有逃脱成为可回收对象的可能性，则基本上就真的成为可回收对象了。
至于可达性分析法具体是如何操作的我暂时也没有看得很明白，如果有哪位朋友比较清楚的话请不吝指教。
下面来看个例子：
```
```java
Object
 aobj =
```
```java
new
```
```java
Object
 ( ) ;
```
```java
Object
 bobj =
```
```java
new
```
```java
Object
 ( ) ;
```
```java
Object
 cobj =
```
```java
new
```
```java
Object
 ( ) ;
```
```java
aobj
 = bobj;
```
```java
aobj
 = cobj;
```
```java
cobj
 =
```
```java
null
```
```java
;
```
```java
aobj
 =
```
```java
null
```
```java
;
```
```
 　　第几行有可能会使得某个对象成为可回收对象？第7行的代码会导致有对象会成为可回收对象。至于为什么留给读者自己思考。
　　再看一个例子：
```
```java
String
 str =
```
```java
new
```
```java
String(
```
```java
"hello"
```
```java
);
```
```java
SoftReference<String>
 sr =
```
```java
new
```
```java
SoftReference<String>(
```
```java
new
```
```java
String(
```
```java
"java"
```
```java
));
```
```java
WeakReference<String>
 wr =
```
```java
new
```
```java
WeakReference<String>(
```
```java
new
```
```java
String(
```
```java
"world"
```
```java
));
```
```
　　这三句哪句会使得String对象成为可回收对象？第2句和第3句，第2句在内存不足的情况下会将String对象判定为可回收对象，第3句无论什么情况下String对象都会被判定为可回收对象。
　　最后总结一下平常遇到的比较常见的将对象判定为可回收对象的情况：
　　1）显示地将某个引用赋值为null或者将已经指向某个对象的引用指向新的对象，比如下面的代码：
```
```java
Object
 obj =
```
```java
new
```
```java
Object();
```
```java
obj
 =
```
```java
null
```
```java
;
```
```java
Object
 obj1 =
```
```java
new
```
```java
Object();
```
```java
Object
 obj2 =
```
```java
new
```
```java
Object();
```
```java
obj1
 = obj2;
```
```
　　　　2）局部引用所指向的对象，比如下面这段代码：
```
```java
void
```
```java
fun()
 {
```
```java
.....
```
```java
```
```java
for
```
```java
(
```
```java
int
```
```java
i=
```
```java
0
```
```java
;i<
```
```java
10
```
```java
;i++)
 {
```
```java
```
```java
Object
 obj =
```
```java
new
```
```java
Object();
```
```java
```
```java
System.out.println(obj.getClass());
```
```java
```
```java
}
```
```java
}
```
```
 　　循环每执行完一次，生成的Object对象都会成为可回收的对象。
　　3）只有弱引用与其关联的对象，比如：
```
```java
WeakReference<String>
 wr =
```
```java
new
```
```java
WeakReference<String>(
```
```java
new
```
```java
String(
```
```java
"world"
```
```java
));
```
```
## 二.典型的垃圾收集算法
　　在确定了哪些垃圾可以被回收后，垃圾收集器要做的事情就是开始进行垃圾回收，但是这里面涉及到一个问题是：如何高效地进行垃圾回收。由于Java虚拟机规范并没有对如何实现垃圾收集器做出明确的规定，因此各个厂商的虚拟机可以采用不同的方式来实现垃圾收集器，所以在此只讨论几种常见的垃圾收集算法的核心思想。
　　1.Mark-Sweep（标记-清除）算法
　　这是最基础的垃圾回收算法，之所以说它是最基础的是因为它最容易实现，思想也是最简单的。标记-清除算法分为两个阶段：标记阶段和清除阶段。标记阶段的任务是标记出所有需要被回收的对象，清除阶段就是回收被标记的对象所占用的空间。具体过程如下图所示：
![](http://images.cnitblog.com/i/288799/201406/181024382398115.jpg)
　　从图中可以很容易看出标记-清除算法实现起来比较容易，但是有一个比较严重的问题就是容易产生内存碎片，碎片太多可能会导致后续过程中需要为大对象分配空间时无法找到足够的空间而提前触发新的一次垃圾收集动作。
　　2.Copying（复制）算法
　　为了解决Mark-Sweep算法的缺陷，Copying算法就被提了出来。它将可用内存按容量划分为大小相等的两块，每次只使用其中的一块。当这一块的内存用完了，就将还存活着的对象复制到另外一块上面，然后再把已使用的内存空间一次清理掉，这样一来就不容易出现内存碎片的问题。具体过程如下图所示：
![](http://images.cnitblog.com/i/288799/201406/181041528488728.jpg)
　　这种算法虽然实现简单，运行高效且不容易产生内存碎片，但是却对内存空间的使用做出了高昂的代价，因为能够使用的内存缩减到原来的一半。
　　很显然，Copying算法的效率跟存活对象的数目多少有很大的关系，如果存活对象很多，那么Copying算法的效率将会大大降低。
　　3.Mark-Compact（标记-整理）算法
　　为了解决Copying算法的缺陷，充分利用内存空间，提出了Mark-Compact算法。该算法标记阶段和Mark-Sweep一样，但是在完成标记之后，它不是直接清理可回收对象，而是将存活对象都向一端移动，然后清理掉端边界以外的内存。具体过程如下图所示：
![](http://images.cnitblog.com/i/288799/201406/181100129575916.jpg)
　　4.Generational Collection（分代收集）算法
　　分代收集算法是目前大部分JVM的垃圾收集器采用的算法。它的核心思想是根据对象存活的生命周期将内存划分为若干个不同的区域。一般情况下将堆区划分为老年代（Tenured Generation）和新生代（Young Generation），老年代的特点是每次垃圾收集时只有少量对象需要被回收，而新生代的特点是每次垃圾回收时都有大量的对象需要被回收，那么就可以根据不同代的特点采取最适合的收集算法。
　　目前大部分垃圾收集器对于新生代都采取Copying算法，因为新生代中每次垃圾回收都要回收大部分对象，也就是说需要复制的操作次数较少，但是实际中并不是按照1：1的比例来划分新生代的空间的，一般来说是将新生代划分为一块较大的Eden空间和两块较小的Survivor空间，每次使用Eden空间和其中的一块Survivor空间，当进行回收时，将Eden和Survivor中还存活的对象复制到另一块Survivor空间中，然后清理掉Eden和刚才使用过的Survivor空间。
　　而由于老年代的特点是每次回收都只回收少量对象，一般使用的是Mark-Compact算法。
　　注意，在堆区之外还有一个代就是永久代（Permanet Generation），它用来存储class类、常量、方法描述等。对永久代的回收主要回收两部分内容：废弃常量和无用的类。
## 三.典型的垃圾收集器
　　垃圾收集算法是 内存回收的理论基础，而垃圾收集器就是内存回收的具体实现。下面介绍一下HotSpot（JDK 7)虚拟机提供的几种垃圾收集器，用户可以根据自己的需求组合出各个年代使用的收集器。
　　1.Serial/Serial Old
　　Serial/Serial Old收集器是最基本最古老的收集器，它是一个单线程收集器，并且在它进行垃圾收集时，必须暂停所有用户线程。Serial收集器是针对新生代的收集器，采用的是Copying算法，Serial Old收集器是针对老年代的收集器，采用的是Mark-Compact算法。它的优点是实现简单高效，但是缺点是会给用户带来停顿。
　　2.ParNew
　　ParNew收集器是Serial收集器的多线程版本，使用多个线程进行垃圾收集。
　　3.Parallel Scavenge
　　Parallel Scavenge收集器是一个新生代的多线程收集器（并行收集器），它在回收期间不需要暂停其他用户线程，其采用的是Copying算法，该收集器与前两个收集器有所不同，它主要是为了达到一个可控的吞吐量。
　　4.Parallel Old
　　Parallel Old是Parallel Scavenge收集器的老年代版本（并行收集器），使用多线程和Mark-Compact算法。
　　5.CMS
　　CMS（Current Mark Sweep）收集器是一种以获取最短回收停顿时间为目标的收集器，它是一种并发收集器，采用的是Mark-Sweep算法。
　　6.G1
　　G1收集器是当今收集器技术发展最前沿的成果，它是一款面向服务端应用的收集器，它能充分利用多CPU、多核环境。因此它是一款并行与并发收集器，并且它能建立可预测的停顿时间模型。
　　下面补充一下关于内存分配方面的东西：
![](http://images.cnitblog.com/i/288799/201406/181512325519249.jpg)
　　对象的内存分配，往大方向上讲就是在堆上分配，对象主要分配在新生代的Eden Space和From Space，少数情况下会直接分配在老年代。如果新生代的Eden Space和From Space的空间不足，则会发起一次GC，如果进行了GC之后，Eden Space和From Space能够容纳该对象就放在Eden Space和From Space。在GC的过程中，会将Eden Space和From  Space中的存活对象移动到To Space，然后将Eden Space和From Space进行清理。如果在清理的过程中，To
 Space无法足够来存储某个对象，就会将该对象移动到老年代中。在进行了GC之后，使用的便是Eden space和To Space了，下次GC时会将存活对象复制到From Space，如此反复循环。当对象在Survivor区躲过一次GC的话，其对象年龄便会加1，默认情况下，如果对象年龄达到15岁，就会移动到老年代中。
　　一般来说，大对象会被直接分配到老年代，所谓的大对象是指需要大量连续存储空间的对象，最常见的一种大对象就是大数组，比如：
　　byte[] data = new byte[4*1024*1024]
　　这种一般会直接在老年代分配存储空间。
　　当然分配的规则并不是百分之百固定的，这要取决于当前使用的是哪种垃圾收集器组合和JVM的相关参数。
