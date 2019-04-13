
# Java内存管理机制 - Wesley's Blog - CSDN博客


2018年05月08日 21:17:13[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：239


## 内存划分区域
方法区(Method Area), 虚拟机栈(VM Stack), 本地方法栈(Native Method Stack), 堆(Heap), 程序计数器(Program Counter Register)
1. 程序计数器(Program Counter Register)
线程私有的．当前线程所执行的字节码的行号指示器．通过改变这个计数器的值来选取下一条需要执行的字节码指令．
2. 虚拟机栈(VM Stack)
线程私有的，生命周期与线程相同．每个方法被执行的时候都会创建一个栈帧(Stack Frame)用于存储局部变量，操作栈，动态链接，方法出口等信息．每一个方法被调用直至执行完成的过程，对应着一个栈帧在虚拟机栈中从入栈到出栈的过程．
3. 本地方法栈(Native Method Stacks)
与虚拟机栈发挥的作用非常相似．区别是虚拟机栈执行Java方法（也就是字节码）服务，而本地方法栈则是为虚拟机使用到的Native方法服务．
4. 堆(Heap)
最大的一块，所有线程共享的一块内存，虚拟机启动时创建．唯一目的就是存放对象实例．垃圾收集器管理的主要区域，很多时候也被称作＂GC堆＂(Garbage Collected Heap)
5. 方法区(Method Area)
各个线程共享区域，存储已被虚拟机加载的类信息，常量，静态变量，即时编译器编译后的代码等数据．运行时常量池(Runtime Constant Pool)是方法区的一部分，存放编译期生成的各种字面量和符号引用
## 对象访问
`Object obj = new Object();`Object obj将会反映到Java栈的本地变量表中，作为一个reference类型数据出现．而”new Object()”这部分则会反映到Java堆中，形成了一块存储了Object类型所有实例数据的结构化内存．而reference类型访问对象的方式有两种:
- 使用句柄，Java堆中将会划分出一块内存作为句柄池，reference存储句柄地址
- 使用指针，reference中直接存储的就是对象地址．主要虚拟机(Sun HotSpot）使用的这一种
## 垃圾回收(GC)
## 对象已死
## 引用计数法(Reference Counting)
## 引用时，计数器加１，失效时减１．简单高效，但是无法解决相互循环引用的问题．
## 根搜索算法(GC Roots Tracing)
## 主流的方法．通过一系列名为”GC Roots”的对象作为起始点，从这些节点开始向下搜索，搜索所走过的路径称为引用链(Reference Chain)，当一个对象没有任何引用链相连时，则证明对象是不可用的．可作为GC Roots的对象有以下几种:
## 虚拟机栈（栈帧中的本地变量表）中的引用的对象
## 方法区中的类静态属性引用的对象
## 方法区中的常量引用的对象
## 本地方法栈中JNI的引用的对象（类似于第一种）
![这里写图片描述](https://img-blog.csdn.net/2018050821152871?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 何时回收
## 一个对象死亡，至少要经历两次标记过程:第一次判断是否有与GC Roots相连接的引用链，如果没有被执行过finalize指令，则会执行，被放置在一个F-Queue中，由虚拟机建立的Finalizer线程执行．GC将对F-Queue中的对象进行得二次小规模标记．
## 回收方法区
## 方法区（或者HotSpot虚拟机中的永久代）的回收的＂性价比＂一般比较低．主要回收废弃常量和无用的类
## 垃圾收集算法
## 标记-清除(Mark-Sweep)算法
## 分为标记，清除两个阶段．首先标记出所有需要回收的对象，在标记完成后统一回收所有被标记的对象．
## 有两个缺点：(1)效率　(2)空间，容易造成碎片

## 复制(Copying)算法
## 将可用内存按照容量划分为大小相等的两块，每次只使用其中的一块，当这一块的内存用完了，就将存活着的对象复制到另一块上面，然后再把已使用过的内存空间一次清理掉．这样实现简单，但是代价是内存缩小到原来的一般，空间代价较高．
![这里写图片描述](https://img-blog.csdn.net/20180508211541545?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 标记-整理算法
## 复制收集算法在对象存活率较高的时候就要执行较多的复制操作，效率变低，而且空间浪费高．标记整理算法是让所有的存活对象都向一端移动，然后直接清理掉端边界外的内存．
![这里写图片描述](https://img-blog.csdn.net/20180508211600627?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 分代收集算法
## 根据对象的存货周期划分为几块．一般把堆分为新生代(Young Generation)和老年代(Old Generation)．根据各个年代的特点采用最适当的收集算法．在新生代如果有大批对象死去，少量存活，就采用复制算法，只需要少量的复制就可以完成收集．而老年代中则采取＂标记－清理＂或者＂标记－整理＂算法来回收
## 垃圾收集器
## 垃圾收集算法的具体实现
![这里写图片描述](https://img-blog.csdn.net/20180508211626581?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 对象分配回收策略
## 对象优先在新生代Eden分配．当Eden区没有足够的空间进行分配时，虚拟机将发起一次Minor GC．（新生代GC为MinorGC; 老年代GC为Major GC/Full GC）
## 大对象直接进入老年代．避免在Eden区以及两个Survivor区之间发生大量的内存拷贝．
## 长期存活的对象进入老年区．给每个对象定义一个对象年龄(Age)计数器，年龄增加到一定程度时(默认为１５岁)，就晋升到老年代
## 动态对象年龄判定．如果Survivor空间中相同年龄所有对象大小的总和大于Survivor空间的一半,年龄大于或等于该年龄的对象就可以直接进入老年代.
## 空间分配担保．如果晋升到老年代的平均大小大于老年代的剩余空间大小，则改为直接进行一次Full GC.

