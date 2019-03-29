# 深入理解Java虚拟机



# （一）：Java内存模型



## 1.Java 虚拟机运行时数据区域

​        Java虚拟机在执行Java程序的过程中会把它所管理的内存划分为若干个不同的数据区域。这些区域都有各自的用途，以及创建和销毁的时间，有的区域随着虚拟机进程的启动而存在，有些区域则依赖用户线程的启动和结束而建立和销毁。

![img](https://img-blog.csdn.net/20180702154400998?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 1.1 程序计数器

​        在Java虚拟机中，程序计数器是线程私有的。程序计数器是一块较少的内存区域，它可以看做是当前线程所执行的字节码的行号指示器。在虚拟机中的概念模型中，字节码解释器的工作就是通过改变这个计数器的值来选取下一条需要执行的字节码指令，分支、循环、跳转、异常处理、线程恢复等基础功能都需要这个依赖这个计数器来完成。

​        由于Java虚拟机的多线程是通过线程轮流切换并分配处理器的执行时间的方式来实现的，在任何一个时刻，一个处理器都只会执行一条线程中的指令。因此为了线程切换后，能够恢复到正确的执行位置，每条线程都需要有一个独立的计数器，各条线程之间互不干扰、互不影响，独立存储。我们称这一类内存存储区域为“线程私有”的内存。

### 1.2 Java虚拟机栈

栈帧是方法运行期的基础数据结构栈容量可由-Xss参数设定

​        与程序计数器一样，Java虚拟机栈也是线程私有的，它的生命周期和线程一样。虚拟机栈的描述的就是Java方法执行的内存模型：每个方法在执行的过程中都会创建一个栈帧用于存储局部变量表、操作数栈、动态链接、方法除开等信息。每一个方法从调用至完成的过程。

​        局部变量表存放了编译器可知的各种基本数据类型（boolean、byte、char、short、int、float、double、long）、对象引用和returnAddress地址信息。局部变量表所存储的内存空间是在编译期间内完成分配的，当进入一个方法时，这个方法需要在帧中分配多大的局部变量空间是完全确定的，在方法运行期间不会改变局部变量表的大小。

​        在Java虚拟机栈中，这个区域规定了两种异常规范：如果线程请求的栈深度大于虚拟机的栈深度，将抛出StackOverFlowError异常；如果虚拟机可以动态扩展，如果扩展时无法申请到足够的内存，就会抛出OutOfMemory异常。

### 1.3 本地方法栈

栈容量可由-Xss参数设定

​        本地方法栈（Native Method Stack）与虚拟机栈所发挥的作用是非常类似的，它们之间的区别就是虚拟机栈是为虚拟机执行Java方法服务，而本地方法栈是为虚拟机执行Native方法服务。

### 1.4 Java堆

可通过参数 -Xms 和-Xmx设置  。

​        Java堆是被所有线程共享的一块内存区域，在虚拟机启动时创建，此内存区域的唯一目的就是存放对象实例，几乎所有的对象实例都在这里分配内存；Java堆是垃圾收集器管理的主要区域，也叫GC堆；java堆可以处于物理上不连续的内存空间中，只要逻辑上是连续的即可。

### 1.5 方法区

参数-XX:MaxPermSize可设置 .

​    方法区与java堆一样，是各个线程共享的内存区域，它用于存储**已被虚拟机加载的类信息**、**常量**、**静态变量**、**即时编译器编译后的代码等数据**。该区域的内存回收目标主要是针对常量池的回收和对类型的卸载。

### 1.6 运行时常量池

可以通过-XX:PermSize和-XX:MaxPermSize设置

​    运行时常量池是方法区的一部分。Class文件中除了有类的版本、字段、方法、接口等描述信息外，**还有一项信息是常量池，用于存放编译期生成的各种字面量和符号引用**，这部分内容将在类加载后进入方法区的运行时常量池中存放。运行时常量池相对于Class文件常量池的另外一个重要特征是具备动态性，java语言并不要求常量一定只有编译器才能产生，也就是并非预置入Class文件中常量池的内容才能进入方法区运行时常量池，运行期间也可能将新的常量放入池中，这种特性被开发人员利用得比较多的便是String类的intern()方法。

### 1.7 直接内存

可通过-XX:MaxDirectMemorySize指定，如果不指定，则默认与Java堆的最大值（-Xmx指定）一样

​    直接内存并不是虚拟机运行时数据区的一部分，也不是java虚拟机规范定义的内存区域。但是这部分内存也被频繁地使用，而且也可能导致OutOfMemoryError异常。在JDK1.4中新加入了NIO(New Input/Output)类，引入了一种基于通道与缓冲区的I/O方式，它可以使用Native函数库直接分配堆外内存，然后通过一个存储在java堆中的DirectByteBuffer对象作为这块内存的引用进行操作，这样能在一些场景中显著提高性能，因为避免了在java堆和Navtive堆中来回复制数据。

## 2. HoptSpot虚拟机对象探秘

### 2.1 对象的创建

 

​        当虚拟机遇到一条new指令时，首先将去检查这个指令的参数是否能在常量池中定位到一个类的符号引用，并且检查这个符号引用代表的类是否已经被加载、解析和初始化过。**如果没有，那必须先执行相应的类加载过程。**

​        \1. **在类加载检查通过后，接下来虚拟机将为新生对象分配内存。**对象所需内存大小在类加载完成后便可确定，为对象分配空间的任务等同于把一块确定大小的内存从java堆中划分出来，其中有“指针碰撞”和“空闲列表”两种方式。除开如何划分可用空间之外，还有另外一个需要考虑的问题就是对象创建在虚拟机中是非常频繁的行为，即使是仅仅修改一个指针所指向的位置，在并发情况下也并不是线程安全的，可能出现在正给对象A分配内存，指针还没来得及修改，对象B又同时使用了原来的指针来分配内存的情况。解决这个问题有两种方案，一种是对分配内存空间的动作进行同步处理--实际上虚拟机采用CAS配上失败重试的方法保证更新操作的原子性；另一种是把内存分配的动作按照线程划分在不同的空间之中进行，即每个线程在java堆中预先分配一小块内存，称为本地线程分配缓冲（TLAB）。哪个线程需要分配内存，就在那个线程的TLAB上分配，只有TLAB用完并分配新的TLAB时，才需要同步锁定。

​      \2.  **内存分配完毕后，虚拟机需要将分配到内存的空间都初始化为零值（不包括对象头），以保证对象实例字段在java代码中可以不赋初值就直接使用。**

​      \3. **接下来就是对对象进行必要的设置，如这个对象是哪个类的实例、如何找到类的元数据、对象的哈希码、独享的GC分代年龄等信息。**

​       4.从虚拟机角度来看，一个新的对象已经产生，但从程序的角度来讲，对象的创建才开始----<init>方法还没开始执行，所有字段都还为0。所以，一般来说，执行new指令后会接着**执行<init>方法把对象按照程序员的意愿进行初始化，**这才算一个真正可用的对象完全产生出来。

### 2.2 对象的内存布局

 

​        在HotSpot虚拟机中，独享在内存中存储的布局可以分为三块区域：对象头（Header）、实例数据（Instance Data）和对齐填充（Padding）。

​        \1. 对象头：HotSpot虚拟机的对象头包括两部分信息，**第一部分用于存储对象自身的运行时数据**，如哈希码、GC分代年龄、锁状态标识、线程持有的锁、偏向线程ID、偏向时间戳等，这部分数据的长度在32位和64位的虚拟机（未开启压缩指针）中分别为32bit和64bit，官方称他为“Mark Word”。**另外一部分是类型指针**，即对象指向它的类元数据的指针，虚拟机通过这个指针来确定这个对象是哪个类的实例。并不是所有的虚拟机实现都必须在对象数据上保留类型指针，换句话说，查找对象的元数据信息并不一定要经过对象本身。

​       \2. 实例数据：实例数据部分是对象真正存储的有效信息，也是在程序代码中所定义的各个类型的字段内容。这部分内容的存储顺序会受到虚拟机分配策略参数和字段在java源代码中定义顺序的影响。HotSpot虚拟机默认的分配策略为longs/doubles 、ints、shorts/chars、bytes/booleans、oops(Ordinary Object Points)，从分配策略可以看出，相同宽度的字段总是被分配到一起。

​       3.填充对齐：这一部分并非必须存在，它仅仅是起占位符的作用，当对象实例数据部分没有对齐时，就需要通过对齐填充来补全8位字节。由于HotSpot VM的自动内存管理系统要求对象起始地址必须是8字节的整数倍，换句话说对象的大小必须是8字节的整数倍，而对象头的部分的大小正好是8字节的倍数，因此，当对象实例数据没有对齐时候，就需要填充对齐来对齐。

### 2.3 对象的访问定位

​        建立对象是为了使用对象，我们的java程序中需要通过栈上的reference数据来操作堆上的具体对象。由于reference类型在java虚拟机中只规范了一个指向对象的引用，并没有去定义这个引用应该以何种方式去定位、访问堆中的对象的具体位置，所以对象访问方式也是取决于虚拟机实现而定的。目前主流的访问方式有使用**句柄**和**直接指针**两种。

​        句柄访问方式：在java堆中将会划分出一块内存来作为句柄池，reference中存储的就是对象的句柄地址，而句柄中包含对象实例数据与类型数据各自的具体地址信息，如图：

![img](https://img-blog.csdn.net/20180702163756652?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

​    直接指针访问：在java堆中的布局中就必须考虑如何放置访问类型数据的相关信息，而reference中存储的就是对象的地址，如图：

![img](https://img-blog.csdn.net/20180702163807219?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 

   这两种对象在访问时各有优势，使用句柄来访问的最大好处就是reference中存储的是稳定的句柄地址，在对象移动（GC中）时只会改变句柄中的实例数据指针，而reference本身不需要修改。使用直接指针访问方式的最大好处就是速度更快，它节省了一次指针对位的时间开销，由于对象的访问在java中非常频繁，因此这类开销积少成多后也是一种非常可观的执行成本。Sun HotSpot中采用第二种。

## 3 OutOfMemoryError 异常

### 3.1 Java堆溢出

​        java堆用于存储对象实例，只要不停地创建对象，病情保证GC Roots到对象之间有可达的路径来避免垃圾回收机制清除这些对象，那么在对象数量到达最大的容量限制后就会产生内存溢出异常。eg:

 

​        java堆内存的OOM异常是实际应用中常见的内存溢出异常情况。当出现java堆内存溢出情况，异常堆栈信息“java.lang.OutOfMemaryError”会跟着进一步提示“Java heap space”。要解决这个区域异常，一般是先通过内存映像分析工具对Dump出来的堆转储快照进行分析，区分是内存泄露还是内存溢出。

​        如果是内存泄露，可进一步通过工具查看泄露对象到GC Roots的引用链。

​        如果是不存在泄露，换句话说内存中的对象确实还活着，那就应当检查虚拟机的堆参数与机器物理内存对比是否还可以调大，从代码上检查是否存在某些对象的生命周期过长、持有状态时间过长的情况，尝试减少程序运行期的内存消耗。

### 3.2 虚拟机栈和本地方法栈溢出

关于虚拟机和本地方法栈，在java虚拟机规范中描述了两种异常：

1）如果把线程请求的栈深度大于虚拟机所允许的最大深度，将抛出StackOutflowError异常。

2）如果虚拟机在扩展栈时无法申请到足够的内存空间，则抛出OutOfMemoryError异常。

​        这里把异常分成两种，看似更加严谨，但却存在着一些相互重叠的地方，当栈空间无法继续分配时，到底是内存太小还是已使用的栈空间太大，其本质上只是对同一件事的两种描述。对于HotSpot虚拟机来说，并不区别本地方法栈和虚拟机栈。
 

### 3.3方法区和运行时常量池溢出

###  3.4本机直接内存溢出







# (二)：垃圾回收机制

2018年12月11日 11:03:12

 

MasterT-J

 

阅读数：278

 

标签： [JVM虚拟机](http://so.csdn.net/so/search/s.do?q=JVM%E8%99%9A%E6%8B%9F%E6%9C%BA&t=blog) 更多

个人分类： [深入理解JVM虚拟机](https://blog.csdn.net/qq_21125183/article/category/7768772)

所属专栏： [JVM虚拟机](https://blog.csdn.net/column/details/31193.html)



谈起GC，应该是让Java程序员最激动的一项技术，我相信每个Java程序员都有探究GC本质的冲动！JVM垃圾回收机制对于了解对象的创建和对象的回收极为重要，是每个Java程序员必须掌握的技能。

本博客围绕三个问题来展开

> 1. 哪些内存需要回收？
> 2. 什么时候回收？
> 3. 如何回收？

前面介绍了Java内存运行时区域的各个部分，其中程序计数器、虚拟机栈、本地方法栈3个区域随着线程而生，随线程而灭；栈中的栈针随着方法的进入和退出而有条不紊的执行着出栈和入栈操作。每一个栈帧分配多少内存基本上是在类结构确定下来就是已知的。因此这几个区域的内存分配和回收都具备确定性，在这几个区域中就不需要过多考虑回收的问题，因为方法结束或者线程结束，内存也就跟着回收了。而Java堆和方法区不一样，一个接口中的多个实现类需要的内存是不一样的，一个方法中的多个分支需要的内存也是不一样的，我们只有在程序处于运行期间才能知道会创建哪些对象，这部分内存的分配和回收是动态的。

# 1.哪些内存需要回收

首先回答第一个问题：不再使用的对象需要进行回收，不使用的类也有可能回收。

那么我们如何判断一个对象不再使用呢？主要有以下两种方法。

------

## 1.1 引用计数算法

定义：给对象添加一个引用计数器，每当有一个地方引用它时，计数器就加1；当引用失效时，计数器就减一；任何时刻计数器为0的对象就是不会被使用的对象。

我们可以看出，引用技术方法实现简单。并且有一些GC中确实使用的是引用计数算法，但是在Java虚拟机中并没有使用这个方法进行内存管理，原因就是一个问题很难被解决—对象之间循环引用。‘

来看一个例子：

```java
class Node { 
    Node next ;
} 

Node a = new Node (); 
Node b = new Node (); 

a.next = b ; 
b.next = a ; 

a = null;
b = null;
123456789101112
```

如上述代码，当我们执行最后两行代码的时候，堆中的对象因为还存在着循环引用，因此引用计数并不是0，导致GC并不会回收这两个对象的内存。

存在的问题：循环引用导致GC不会回收循环引用的对象。

## 2.2 可达性分析算法

基本思想：

> 过一系列的称为“GC Roots”的对象作为起始点，从这些节点开始向下搜索，搜索所走过的路径被称为引用链，当一个对象到“GC Roots”没有任何引用链相连的时候，就证明此对象是不可用的。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211110248192.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

在Java语言中，可作为GC Root对象包括以下几种：

> - 虚拟机栈（栈帧中的本地变量表）中的引用对象。
> - 方法区中的静态属性或常量（final）引用的对象。
> - 本地方法栈中JNI(即一般说的Native方法)引用的对象。

------

# 2. 方法区的回收

Java虚拟机规范中描述可以不要求虚拟机在方法区实现垃圾收集，因此很多人认为方法区中是没有垃圾收集的。

不要求虚拟机对方法区进行垃圾收集的原因主要是性价比比较低，在堆中，尤其是新生代中，进行一次垃圾收集一般会回收70%～95%的空间，但方法区的垃圾收集率远低于此。

即使这样，对方法区进行垃圾收集也并非没有必要，在大量使用反射、动态代理等这类频繁定义ClassLoader的场景都需要虚拟机卸载类的功能，以保证方法区不会溢出。

方法区的垃圾收集主要回收废弃常量与无用的类。

废弃常量的判定与回收比较简单：以“abc”这个常量为例，如果当前系统中没有任何对象引用这个常量，也没有任何其他地方（博主猜测是.class文件中有些地方对此常量的引用）引用这个字面量。此时如果发生内存回收，这个常量就会被清理出常量池。（常量池中其他类、接口、方法、字段的符号引用与此类似）

一个无用的类则需要满足以下三个条件：

> 1. 该类不存在任何实例。
> 2. 加载该类的ClassLoader已经被回收（条件比较苛刻）。
> 3. 该类对应的Class对象没有在任何地方被引用，也就是无法使用反射机制。

虚拟机可以对满足上述三个条件的无用类进行回收。

# 3. Java中的引用

引用分为强引用（Strong Reference）、软引用（Soft Reference）、弱引用（Weak Reference）、虚引用（Phantom Reference）4种，这4种引用强度依次逐渐减弱。

1.强引用就是指在程序代码之中普遍存在的，类似“Object obj = new Object()”这类的引用，只要强引用还存在，垃圾收集器永远不会回收掉被引用的对象。

2.软引用是用来描述一些还有用但并非必需的对象。对于软引用关联着的对象，在系统将要发生内存溢出异常之前，将会把这些对象列进回收范围之中进行第二次回收。如果这次回收还没有足够的内存，才会抛出内存溢出异常。在JDK 1.2之后，提供了SoftReference类来实现软引用。

```java
public class SoftRef {
    public static void main(String args[]) {
        String fk = "do you like me";
        SoftReference<String> soft = new SoftReference<String>(fk);
        System.out.println(soft.get());
    }
}
1234567
```

3.弱引用也是用来描述非必需对象的，但是它的强度比软引用更弱一些，被弱引用关联的对象只能生存到下一次垃圾收集发生之前。当垃圾收集器工作时，无论当前内存是否足够，都会回收掉只被弱引用关联的对象。在JDK 1.2之后，提供了WeakReference类来实现弱引用。

```java
public class WeakRef {
    public static void main(String args[]) {
        String temp = "fk that";
        WeakReference<String> wReference = new WeakReference<String>(temp);
        System.err.println(wReference.get());
    }
}
1234567
```

4.虚引用也称为幽灵引用或者幻影引用，它是最弱的一种引用关系。一个对象是否有虚引用的存在，完全不会对其生存时间构成影响，也无法通过虚引用来取得一个对象实例。为一个对象设置虚引用关联的唯一目的就是能在这个对象被收集器回收时收到一个系统通知。在JDK 1.2之后，提供了PhantomReference类来实现虚引用。

```java
public class PhRef {
    public static void main(String args[]) {
        String temp = "hello china";
        ReferenceQueue<String> queue = new ReferenceQueue<>();
        PhantomReference<String> phReference = new PhantomReference<String>(temp, queue);
        System.out.println(phReference.get());
    }
}
12345678
```

# 4.HotSpot 算法实现

## 4.1 枚举根节点

在可达性分析法中对象能被回收的条件是没有引用来引用它，要做到这点就需要得到所有的GC Roots节点，来从GC Root来遍历。可作为GC Root的主要是全局性引用（例如常量和静态变量），与执行上下文（栈帧中的本地变量表）中。那么如何在这么多的全局变量和栈中的局部变量表中找到栈上的根节点呢？

在栈中只有一部分数据是Reference（引用）类型，那些非Reference的类型的数据对于找到根节点没有什么用处，如果我们对栈全部扫描一遍这是相当浪费时间和资源的事情。

那怎么做可以减少回收时间呢？我们很自然的想到可以用空间来换取时间，我们可以在某个位置把栈上代表引用的位置记录下来，这样在gc发生的时候就不用全部扫描了，在HotSpot中使用的是一种叫做OopMap的数据结构来记录的。对于OopMap可以简单的理解是存放调试信息的对象。

在OopMap的协助下，我们可以快速的完成GC Roots枚举，但我们也不能随时随地都生成OopMap，那样一方面会需要更多的空间来存放这些对象，另一方面效率也会简单低下。所以只会在特定的位置来记录一下，主要是正在：

> 1. 循环的末尾
> 2. 方法临返回前/调用方法的call指令后
> 3. 可能抛异常的位置

这些位置称为安全点。

## 4.2 安全点

我们在做GC的时候需要让jvm停在某个时间点上，如果不是这样我们在分析对象间的引用关系的时候，引用关系还在不断的变化。这样我们的准确性就无法得到保证。 安全点就是所有的线程在要GC的时候停顿的位置。那么如何让所有的线程都到安全点上在停顿下来呢？这里有两种方案可以选择：

- 抢先式中断
- 主动式中断

在抢先式中断：中不需要线程主动配合，在GC发生的时候就让所有线程都中断，如果发现哪个线程中断的地方不在安全点上，那么就恢复线程，然后让它跑到安全点上。

主动式中断是：让GC在需要中断线程的时候不直接对线程操作，设置一个标志，让各个线程主动轮询这个标志，如果中断标志位真时就让自己中断。

------

# 5. 什么时候回收

## finalize方法

通过上面几种算法，虚拟机可以知道此时内存中有哪些需要被回收的对象，但是虚拟机什么时候会对这些对象进行回收呢？我们需要来谈一谈finalize方法。

在JVM中，当一个对象通过可达性分析算法被判定为垃圾的时候，JVM并不能直接对其进行回收，一是垃圾回收机制并不是实时进行，二是真正的回收一个对象之前还会判断是否要运行它的finalize方法。

当一个对象被判定为是垃圾之后，它将会被第一次标记并进行一次筛选，筛选的条件就是此对象是否有必要执行finalize方法。

如何判断一个对象是否有必要执行finalize方法呢？

两种情况下虚拟机会视为“没有必要执行”：

> - 对象没有覆盖finalize方法
> - finalize方法已经被虚拟机调用过（finalize方法只会被调用一次）

如果这个对象被判定为有必要执行finalize方法，那么这个对象会被放置在一个叫做F-Queue的队列之中，并在稍后由一个被虚拟机创建的，低优先级的Finalizer线程去执行该对象的finalize()方法，并且对象在finalize()方法执行中如果出现执行缓慢或者发生死循环，将会导致F-Queue队列中其他对象永久处于等待。甚至导致整个内存回收系统崩溃。之后GC将会对F-Queue之中的对象进行第二次标记。**如果在第二次标记前这些对象在自己的finalize()方法中可以拯救自己(重新与引用链上的任何一个对象建立关联即可)**也是可以成功存活下来并被移除“即将回收”的集合的。 如果此时还没有逃脱，那就真的要被回收了。

注意：finalize()方法的运行代价高昂，不确定性大，无法保证各个对象的调用顺序。博主建议大家完全可以忘掉Java语言中有这个方法的存在。

# 6. 如何回收

JVM虚拟机如何对标记的对象进行垃圾回收，这里主要有四种垃圾回收算法：标记-清除算法、复制-整理算法、标记-整理算法、分代收集算法

## 6.1 标记-清除算法

算法分为标记和清除两个阶段：首先标记出所有需要回收的对象，在标记完成后统一回收所有被标记的对象，它的标记过程就是使用可达性算法进行标记的。

存在的问题：

1. 效率问题，标记和清除两个过程的效率都不高。
2. 空间问题，标记清除之后会产生大量不连续的内存碎片，导致以后分配较大对象时内存不足以至于不得不提前触发另一次垃圾收集动作。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211110237874.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

## 6.2 复制-整理算法

为了解决标记-清除算法的效率问题，JVM虚拟机提供复制-整理算法

> 基本思想：将现有的内存空间分为两块，每次只使用其中一块，在垃圾回收时将正在使用的内存中的存活对象复制到未被使用的内存块中，之后，清除正在使用的内存块中的所有对象，交换两个内存的角色，完成垃圾回收。

如果系统中的垃圾对象很多，复制算法需要复制的存活对象数量并不会太大。因此在真正需要垃圾回收的时刻，复制算法的效率是很高的。又由于对象在垃圾回收过程中统一被复制到新的内存空间中，因此，可确保回收后的内存空间是没有碎片的。该算法的缺点是将系统内存折半。

现在的商业虚拟机都采用这种收集算法回收新生代，同时我们显然不能忍受内存折半的损耗，好在IBM公司研究表明，新生代中98%的对象都是“朝生夕死”，所以并不需要按照1:1的比例来划分内存空间。

通常是将内存分为一块较大的Eden空间两块较小的Survivor空间，每次使用Eden和其中一块Survivor。HotSpot虚拟机默认Eden和Survivor的比例为8:1。

当每次进行回收时，将Eden和Survivor中还存活的对象一次性的复制到另一块Survivor空间上，然后清理掉Eden和刚才使用的Survivor空间。

存在的问题：

1. 内存缩小为原来的一半，内存的利用率太低。
2. 在内存存活率较高时就要进行较多的复制操作，复制的效率会变低

## 6.3 标记-整理算法

复制收集算法在对象存活率较高时就要进行较多的复制操作，效率将会变低。更关键的是，如果不想浪费50%的空间，就需要有额外的空间进行分配担保，以应对被使用的内存中所有对象都100%存活的极端情况，所以在老年代一般不能直接选用这种算法。

> 基本思想：标记过程仍然与“标记-清除”算法一样，但后续步骤不是直接对可回收对象进行清理，而是让所有存活的对象都向一端移动，然后直接清理掉端边界以外的内存，“标记-整理”算法的示意图如下图所示。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211110232972.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

## 6.4 分代收集算法

稍后详细介绍

# 7 JVM堆内存分配模型

Java堆根据对象存活周期的不同将内存划分为新生代与老年代。新生代又被划分为三个区域：Eden、From Survivor、To Survivor。Java堆的内存模型大致为：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211110226720.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

当对象在 Eden ( 包括一个 Survivor 区域，这里假设是 from 区域 ) 出生后，在经过一次 Minor GC 后，如果对象还存活，并且能够被另外一块 Survivor 区域所容纳( 上面已经假设为 from 区域，这里应为 to 区域，即 to 区域有足够的内存空间来存储 Eden 和 from 区域中存活的对象 )，则使用复制算法将这些仍然还存活的对象复制到另外一块 Survivor 区域 ( 即 to 区域 ) 中，然后清理所使用过的 Eden 以及 Survivor 区域 ( 即 from 区域 )，并且将这些对象的年龄设置为1，以后对象在 Survivor 区每熬过一次 Minor GC，就将对象的年龄 + 1，当对象的年龄达到某个值时 ( 默认是 15 岁，可以通过参数 -XX:MaxTenuringThreshold 来设定 )，这些对象就会成为老年代（长期存活的对象进入老年代）。但这也不是一定的，对于一些较大的对象 ( 即需要分配一块较大的连续内存空间 ) 则是直接进入到老年代（PretenureSizeThreshold参数的设定）。

Java堆根据对象存活周期的不同将内存划分为新生代与老年代。那么新生代的对象如何变成老生代的对象的呢？下面有三种情况新生代的对象变成老生带代对象。

## 7.1 大对象直接进入老生代

所谓的大对象是指，需要大量连续内存空间的Java对象，最典型的大对象是那种很长很长的字符串以及数组（笔者列出的Example中的byte[]数组就是典型的大对象）。

## 7.2 长期存活的对象进入老生代

虚拟机给每个对象定义一个对象年龄（Age）计数器。如果对象在Eden出生并经过第一次Minor GC仍然存活，且能够被另外一块 Survivor 区域所容纳( 上面已经假设为 from 区域，这里应为 to 区域，即 to 区域有足够的内存空间来存储 Eden 和 from 区域中存活的对象 )，则使用复制算法将这些仍然还存活的对象复制到另外一块 Survivor 区域 ( 即 to 区域 ) 中，然后清理所使用过的 Eden 以及 Survivor 区域 ( 即 from 区域 )，并且将这些对象的年龄设置为1，以后对象在 Survivor 区每熬过一次 Minor GC，就将对象的年龄 + 1，当对象的年龄达到某个值时 ( 默认是 15 岁，可以通过参数 -XX:MaxTenuringThreshold 来设定 )，这些对象就会成为老年代（长期存活的对象进入老年代）。但这也不是一定的，对于一些较大的对象 ( 即需要分配一块较大的连续内存空间 ) 则是直接进入到老年代（PretenureSizeThreshold参数的设定）。

## 7.3 对象动态年龄判断

为了能更好地适应不同程序的内存状况，虚拟机并不是永远地要求对象的年龄必须达到了MaxTenuringThreshold才能晋升老年代，如果新生代中的Eden与from Survivor空间相同年龄对象的大小之和大于to Survivor空间中的一半，则大于或等于这个年龄的对象则无须等到MaxTenuringThreshold中要求的年龄，即可晋升老年代。

# 8. 分代收集算法

Minor GC（新生代GC）：在新生代中，每次垃圾收集时都发现有大批对象死去，只有少量存活，那就选用复制算法，只需付出少量存活对象的复制成本就可以完成收集。Minor GC非常频繁，并且回收速度也很快。

Full GC/Major GC（老年代GC）：老年代中对象存活率高、没有额外空间对它进行分配担保，就要使用“标记-清理”或“标记-整理”算法进行回收。回收速度比Minor GC慢上很多，发生也不频繁。

## 8.1 分配担保机制

我们之前说过在新生代GC的时候，会将Eden和Survivor中还存活的对象一次性的复制到另一块Survivor空间上，然后清理掉Eden和刚才使用的Survivor空间。

但是会存在另一块Survivor空间不够用的情况，这时就需要分配担保了。

其实在发生Minor GC之前，虚拟机会检查老年代最大可用的连续空间是否**大于新生代所有对象的总大小**，如果大于，则此次Minor GC是安全的。如果小于，则虚拟机会查看HandlePromotionFailure设置值是否允许开启分配担保机制。如果HandlePromotionFailure=true，说明开启了分配担保机制，那么会继续检查老年代最大可用连续空间是否大于历次晋升到老年代对象的平均大小，如果大于，则尝试进行一次Minor GC，但这次Minor GC依然是有风险的；如果小于或者HandlePromotionFailure=false，则改为进行一次Full GC。

上面提到了Minor GC依然会有风险，是因为**新生代采用复制收集算法**，假如大量对象在Minor GC后仍然存活（最极端情况为内存回收后新生代中所有对象均存活），而Survivor空间是比较小的，这时就需要老年代进行分配担保，把Survivor无法容纳的对象放到老年代。老年代要进行空间分配担保，前提是老年代得有足够空间来容纳这些对象，但一共有多少对象在内存回收后存活下来是不可预知的，因此只好取之前每次垃圾回收后晋升到老年代的对象大小的平均值作为参考。使用这个平均值与老年代剩余空间进行比较，来决定是否进行Full GC来让老年代腾出更多空间。

但取平均值仍然是一种概率性的事件，如果某次Minor GC后存活对象陡增，远高于平均值的话，必然导致担保失败。如果出现了分配担保失败，就只能在失败后重新发起一次Full GC。虽然存在发生这种情况的概率，但大部分时候都是能够成功分配担保的，这样就避免了过于频繁执行Full GC。





# (三)：虚拟机性能监控工具

2018年12月11日 15:05:58

 

MasterT-J

 

阅读数：195

更多

所属专栏： [JVM虚拟机](https://blog.csdn.net/column/details/31193.html)



本博客将讲解Java虚拟机性能监控工具的使用以及对Java虚拟机进行性能监控的实验。Java开发人员需要对虚拟机性能监控工具的使用进行掌握，这是很有必要的。

# 1.概述

给一个系统定位问题的时候，知识、经验是关键基础，数据是依据。工具是运用知识处理数据的手段。这里说的数据包括：运行日志、异常堆栈、GC日志、线程快照文件（threaddump/javacore文件）、堆转储快照（heapdump/hprof文件）等。

# 2.JDK的命令行工具

JDK的命令行工具大多数是对jdk/lib/tools.jar类库的一层薄包装而已，它们的主要功能代码是在tools类库中实现的。Linux下的这些工具有的甚至是用shell脚本编写的。

SUN JDK监控和故障处理工具：

| 名称   | 主要作用                                                     |
| ------ | ------------------------------------------------------------ |
| jps    | jvm process status tool,显示指定系统内所有的hotspot虚拟机进程 |
| jstat  | jvm statistics monitoring tool,用于收集hotspot虚拟机各方面的运行数据 |
| jinfo  | configuration info for java，显示虚拟机配置信息              |
| jmap   | memory map for java,生成虚拟机的内存转储快照（heapdump文件） |
| jhat   | jvm heap dump browser，用于分析heapmap文件，它会建立一个http/html服务器让用户可以在浏览器上查看分析结果 |
| jstack | stack trace for java ,显示虚拟机的线程快照                   |

## 2.1 jps：虚拟机进程状况工具

以列出正在运行的虚拟机进程，并显示虚拟机执行主类名称以及这些进程的本地虚拟机唯一ID。

jps命令格式 jps [options] [hostid]

jps可以通过RMI协议开启了RMI服务的远程虚拟机进程状态，hostid为RMI注册表中注册的主机名。

jps常用的选项

| 属性 | 作用                                               |
| ---- | -------------------------------------------------- |
| -p   | 只输出LVMID，省略主类的名称                        |
| -m   | 输出虚拟机进程启动时传递给主类main（）函数的参数   |
| -l   | 输出主类的全名，如果进程执行的是jar包，输出jar路径 |
| -v   | 输出虚拟机进程启动时jvm参数                        |

> 输入命令 jps

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211150458503.png)

## 2.2 jstat：虚拟机统计信息监视工具

jstat是用于监视虚拟机各种运行状态信息的命令行工具。它可以显示本地或者远程虚拟机进程中的类装载、内存、垃圾回收、JIT编译等运行数据，在没有GUI图形界面，只是提供了纯文本控制台环境的服务器上，它将是运行期定位虚拟机性能问题的首选工具。

jstat的命令格式：jstat [option vmid [interval [s|ms] [count]] ]

对于命令格式中的VMID和LVMID，如过是本地虚拟机进程，VMID和LVMID是一致的，如果是远程虚拟机，那VMID的格式应当是：[protocol:] [//] lvmid[@hostname[:port]/servername].

参数interval 和count分别表示查询的间隔和次数，如果省略这两个参数，说明只查询一次。

| 属性              | 作用                                                         |
| ----------------- | ------------------------------------------------------------ |
| -class            | 监视装载类、卸载类、总空间以及类装载所耗费的时间             |
| -gc               | 监视java堆状况，包括eden区、两个survivor区、老年代、永久代等的容量、已用空间、GC时间合计信息 |
| -gccapacity       | 输出主类的全名，如果进程执行的是jar包，输出jar路径           |
| -gcutil           | 监视内容与-gc基本相同，但输出主要关注java堆各个区域使用到最大、最小空间 |
| -gccause          | 监视内容与-gc基本相同，但输出主要关注已使用控件占总空间的百分比 |
| -gcnew            | 与-gcutil功能一样，但是会额外输出导致上一次gc产生的原因      |
| -gcnewcapacity    | 监视内容与-gcnew基本相同，输出主要关注使用到的最大、最小空间 |
| -gcold            | 监视老年代GC情况                                             |
| -gcoldcapacity    | 监视内容与-gcold基本相同，输出主要关注使用到的最大、最小空间 |
| -gcpermcapacity   | 输出永久代使用到的最大、最小空间                             |
| -compiler         | 输出JIT编译过的方法、耗时等信息                              |
| -printcompilation | 输出已经被JIT编译过的方法                                    |

监视进程虚拟机ID39312 的java堆情况

> 输入命令 jstat -gc 39312

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211150505113.png)

## 2.3 jinfo Java配置信息工具

jinfo的作用是实时地查看和调整虚拟机各项参数。
jinfo的命名格式：
jinfo [ option ] pid

> 输入命令 jinfo 39312

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211150511522.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

## 2.4 jmap：java内存映像工具

jmap命令用于生成堆转储快照。jmap的作用并不仅仅为了获取dump文件，它还可以查询finalize执行队列、java堆和永久代的详细信息。如空间使用率、当前用的是哪种收集器等。

和jinfo命令一样，jmap在windows下也受到比较大的限制。除了生成dump文件的-dump选项和用于查看每个类的实例、控件占用统计的-histo选项在所有操作系用都提供之外，其余选项只能在linux/solaris下使用。

jmap格式 jmap [option] vmid

| 选项           | 作用                                                         |
| -------------- | ------------------------------------------------------------ |
| -dump          | 生成java堆转储快照。格式为： -dump:[live,]format=b,file=,其中live子参数说明是否只dump出存活的对象 |
| -finalizerinfo | 显示在F-Queue中等待Finalizer线程执行finalize方法的对象。只在Linux/Solaris平台下有效 |
| -heap          | 显示java堆详细信息，如使用哪种收集器、参数配置、分代情况等，在Linux/Solaris平台下有效 |
| -jisto         | 显示堆中对象统计信息，包含类、实例对象、合集容量             |
| -permstat      | 以ClassLoader为统计口径显示永久代内存状态。只在Linux/Solaris平台下有效 |
| -F             | 当虚拟机进程对-dump选项没有相应时。可使用这个选项强制生成dump快照。只在Linux/Solaris平台下有效 |

生成java堆转储快照 Monitor.bin

> 输入命令 jmap -dump:format=b,file=Monitor.bin 39312

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211150521543.png)

## 2.5 jhat：虚拟机堆转储快照分析工具

Sun JDK提供jhat与jmap搭配使用，来分析dump生成的堆快照。jhat内置了一个微型的HTTP/HTML服务器，生成dump文件的分析结果后，可以在浏览器中查看。

用法举例: jhat test1.bin

test1.bin为生成的dump文件。

屏幕显示“Server is ready.”的提示后，用户在浏览器中键入http://localhost:7000就可以看到分析的结果了。

分析结果默认是以包围单位进行分组显示，分析内存泄漏问题主要会使用到其中的“Heap Histogram”与OQL标签的功能。前者可以找到内存中总容量最大的对象。后者是标准的对象查询语言，使用类似SQL的语法对内存中的对象进行查询统计。

分析java堆转储快照Monitor.bin

> 输入命令 jhat Monitor.bin

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211150526171.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

## 2.6 jstack：java堆栈跟踪工具

jstack命令用于生成虚拟机当前时刻的线程快照（一般称为threaddump或者javacore文件）。线程快照就是当前虚拟机内每一条线程正在执行的方法堆栈集合，生成线程快照的主要目的是定位线程出现长时间停顿的原因，如线程死锁、死循环、请求外部资源导致长时间等待等。

jstack 格式 jstack [option] vmid

option选项的合法值和具体含义

| 选项 | 作用                                         |
| ---- | -------------------------------------------- |
| -F   | 当正常输出的请求不被响应时，强制输出线程堆栈 |
| -l   | 除堆栈外，显示关于锁的附加信息               |
| -m   | 如果调用到本地方法的话，可以显示c/c++的堆栈  |

Tread类新增了一个getAllStackTraces（）方法用于获取虚拟机中所有的线程的StackTraceElement对象。

> 输入命令 jstack 39312

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211150530825.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

## 2.7 HSDIS：JIT生成代码反汇编

Java虚拟机规范中，详细描述了虚拟机指令集中每条指令的执行过程、执行前后对操作数栈、局部变量表的影响等细节。这些细节描述与Sun的早期虚拟机（Sun Classic VM）高度吻合，但随着技术的发展，高性能虚拟机真正的细节实现方式已经渐渐与虚拟机规范所描述的内容产生了越来越大的差距，虚拟机规范中的描述逐渐成了虚拟机实现的“概念模型”——即实现只能保证规范描述等效。基于这个原因，我们分析程序的执行语义问题（虚拟机做了什么）时，在字节码层面上分析完全可行，但分析程序的执行行为问题（虚拟机是怎样做的、性能如何）时，在字节码层面上分析就没有什么意义了，需要通过其他方式解决。

# 3. JDK的可视化工具

JDK中除了提供大量的命令行工具外，还有两个功能强大的可视化工具：JConsole和VisualVM。

# 3.1 JConsole

JConsole工具在JDK/bin目录下，启动JConsole后，将自动搜索本机运行的jvm进程，不需要jps命令来查询指定。双击其中一个jvm进程即可开始监控，也可使用“远程进程”来连接远程服务器。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211150534941.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

# 3.2 VisualVM：多合一故障处理工具

VisualVM是一个集成多个JDK命令行工具的可视化工具。VisualVM基于NetBeans平台开发，它具备了插件扩展功能的特性，通过插件的扩展，可用于显示虚拟机进程及进程的配置和环境信息(jps，jinfo)，监视应用程序的CPU、GC、堆、方法区及线程的信息(jstat、jstack)等。VisualVM在JDK/bin目录下。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211150540686.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

VisualVM的性能分析功能甚至比起JProfiler、YourKit等专业且收费的Profiling工具都不会逊色多少，而且VisualVM还有一个很大的优点：不需要被监视的程序基于特殊Agent运行，因此它对应用程序的实际性能的影响很小，使得它可以直接应用在生产环境中。这个优点是JProfiler、YourKit等工具无法与之媲美的。









# (四)：Class类文件结构(一)

2018年12月11日 16:59:02

 

MasterT-J

 

阅读数：489

更多

所属专栏： [JVM虚拟机](https://blog.csdn.net/column/details/31193.html)



我们都知道Java中的class文件是经过Java编译器对Java类文件进行编译后的产物。我想有不在少数的C程序员在学习Java之后在认知上会粗略的认为C程序在经过编译后产生的.out文件与.class文件在各方面大概相同，我刚开始也这样迷惑自己，但是随着学习的深入，我们必须搞清楚.class文件到底是个什么东西。本博客就带领大家理解Java中Class类文件结构。

# 1. .class文件和.out文件的不同之处

要想明白两个文件的不同，我们首先要了解两个文件的定义。

## 1.1 .class文件

java的编译器在编译java类文件时，会将原有的文本文件（.java）翻译成二进制的字节码，并将这些字节码存储在.class文件。也就是说java类文件中的属性、方法，以及类中的常量信息，都会被分别存储在.class文件中。

从这段话中我们提取出重点：.class文件是二进制的字节码。由JVM识别、分析、执行。

## 1.2 .out文件

C语言源程序（.c文件），经编译器编译，由源代码生成机器指令，并加上描述信息，保存在.out文件（可执行文件）中。可执行文件能被操作系统加载运行，计算机执行该文件中的机器指令。

从这段话中我们提取出重点：.out文件是二进制的机器指令。由操作系统加载运行。

此时两个文件的区别已经非常明显：首先两个文件虽然都是二进制，但存储方式是完全不同的，一个是字节码，一个是机器指令。然后运行平台不同，一个是操作系统，一个是虚拟机。

# 2. 概述

理解了上面那段话，虽然我们从本质上已经知道了两个文件有什么区别，但在使用的时候却依然感觉不到任何差别，两个都是可执行文件啊，字节码和机器指令到底有什么区别呢？

从上有关计算机的第一堂课开始，老师就一直告诉我们，“计算机只认识二进制的数据，在计算机的内部，它运行的本质，就是一串010101010101… …”，这串010101… … 就是机器指令，所以操作系统可以对.out文件进行加载，运行。

那字节码是什么呢？思考这个问题的时候，你可以想一下Java的优势在哪里。有没有记得在Java界流传这样一句话，“一次编写，到处运行”。没错，字节码就是提供平台无关性的基石。

Java程序在各种不同的平台上进行编译却都生成相同的字节码，这些字节码由JVM进行加载，运行。这种统一的程序存储格式，从而实现了Java的跨平台性。使用Java编译器可以将Java代码编译为存储字节码的Class文件，使用JRuby等其他语言的编译器一样可以将程序代码编译成Class文件，虚拟机并不关心Class的来源是何种语言。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211165850245.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

# 3. Class类文件结构

要想了解.class文件里面存储的具体内容，我们首先要对.class文件的整体存储结构有一个全面的认识。当然在这之前，我们先来对.class文件做个详细的定义。

## 3.1 Class文件定义

任何一个类或接口都对应唯一的.class文件，具体来说如下图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211165846109.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

Class文件是一组以字节为基础单位的二进制流，各个数据项目紧凑的排列在Class文件中。并且数据项的存储方式类似于大端模式，不了解大端模式的自行百度。Class文件结构中只含有两种数据类型：无符号数与表。

> - 无符号数就是基本的数据类型，我们以u1，u2，u4，u8分别来表示1个字节，2个字节，4个字节，8个字节。无符号数可以用来描述数字，索引引用，数量值或是UTF-8编码构成的字符串。如果你对上面这句话感到抽象，别急，看到最后再回过头，就会发现疑惑已经自行解决。
> - 表是由多个无符号数或其他表作为数据项构成的复合数据结构，习惯以“_info”结尾。Class文件本质上就是一张表。

Class文件格式：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211165841390.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

首先编写一个Java文件

```java
package com.basic.java.classStructure;
public class TestClass {
    private int m;
    private int c(){
        return m+1;
    }
}
1234567
```

编译成class文件后，用winhex软件打开class字节码

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211165836272.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

## 3.2 魔数

魔数是每个Class文件的头四个字节，作用为确定这个文件是否是一个能被虚拟机接受的Class文件。它的值也非常的好记，充满了浪漫气息：CAFEBABE(咖啡宝贝？)，和Java的logo似乎有某种联系～～

## 3.3 Class文件的版本

后面4个字节存储的就是Class文件的版本号：5、6字节是次版本号，7、8字节是主版本号，这四个字节的作用一般是用来让我们分辨当前的JDK版本，高版本的JDK可以向下兼容低版本，反过来则不可以。

一般来说我们不需要太在意次版本号，版本号转换对应JDK一般步骤如下：

> - 将主版本号转换为10进制
> - 用版本号减去45（JDK版本号从45开始）再加上1

例如上图中我的次版本号为0x00，主版本号为0x34，转换十进制为52，则JDK版本为52-45+1，也就是8，所以我目前的JDK版本为JDK1.8，没有错误。

## 3.4 常量池

要说Class文件中的重要组成部分是什么，我觉得肯定属于常量池（其它项目关联最多数据类型，Class文件空间最大的数据项目之一，表类型数据项目）和属性表，关于属性表的部分我们下次再说。

Java虚拟机运行时方法区中的常量池就是将类加载进内存之后.class文件中的常量池。

### 3.4.1 常量池容量计数

常量池的入口处首先是一项u2类型（无符号，2个字节）的数据，代表常量池容量计数值，设置这个值的原因乃是由于常量池中常量的数量并不固定。从上图中可以看到此Class文件的常量池容量为0x0016。

值得一提的是，常量池中的容量计数是从1开始而不是从0开始，Class文件中也就只有常量池的容量计数是从1开始。这样设计的目的在于满足后面某些指向常量池的索引值的数据在特定情况下需要表达“不引用任何一个常量池项目”的含义。

所以我的Class文件常量池容量转换为10进制为22，也就是只有21项常量，索引值范围1～21。

### 3.4.2 常量池存储项目类型

上面说到常量池容量，然后我们需要分析常量池中的内容，在分析常量池中存储的内容之前，我们需要对常量池的存储类型做一个介绍。

- 常量池中主要存放两大常量：字面量和符号引用。
- 字面量接近于Java语言层面的常量概念，如文本字符串，final常量值等。

符号引用包含下面三类常量（关于全限定名与描述符稍后再说）：

> - 类和接口的全限定名
> - 字段的名称和描述符
> - 方法的名称和描述符

常量池中每一个常量都是一个表。下面我们列出了相应的标志对应的常量池项目类型：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211165827971.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

### 3.4.3 利用javap命令查看常量池

我们可以使用Javap命令借助计算机进行常量表的输出。

我们来看一下用法：

```java
//TestClass 就是我们上面TestClass.java文件编译后产生的Class文件
javap -verbose TestClass
12
```

输出结果：

```
警告: 二进制文件TestClass包含com.basic.java.classStructure.TestClass            
Classfile /C:/Users/MasterTj/Desktop/TestClass.class                            
  Last modified 2018-12-11; size 411 bytes                                       
  MD5 checksum 3deeee1911ec575f2b826c440a51e7a1                                   
  Compiled from "TestClass.java"                                                   
public class com.basic.java.classStructure.TestClass                                    
  minor version: 0                                                                      
  major version: 52                                                                     
  flags: ACC_PUBLIC, ACC_SUPER                                                          
Constant pool:                                                                          
   #1 = Methodref          #4.#18         // java/lang/Object."<init>":()V              
   #2 = Fieldref           #3.#19         // com/basic/java/classStructure/TestClass.m:I
   #3 = Class              #20            // com/basic/java/classStructure/TestClass    
   #4 = Class              #21            // java/lang/Object                           
   #5 = Utf8               m                                                            
   #6 = Utf8               I                                                            
   #7 = Utf8               <init>                                                       
   #8 = Utf8               ()V                                                          
   #9 = Utf8               Code                                                         
  #10 = Utf8               LineNumberTable                                              
  #11 = Utf8               LocalVariableTable                                           
  #12 = Utf8               this                                                         
  #13 = Utf8               Lcom/basic/java/classStructure/TestClass;                    
  #14 = Utf8               c                                                            
  #15 = Utf8               ()I                                                          
  #16 = Utf8               SourceFile                                                   
  #17 = Utf8               TestClass.java                                               
  #18 = NameAndType        #7:#8          // "<init>":()V                               
  #19 = NameAndType        #5:#6          // m:I                                        
  #20 = Utf8               com/basic/java/classStructure/TestClass                      
  #21 = Utf8               java/lang/Object                                             
{                                                                                       
  public com.basic.java.classStructure.TestClass();                                     
    descriptor: ()V                                                                     
    flags: ACC_PUBLIC                                                                   
    Code:                                                                               
      stack=1, locals=1, args_size=1                                                    
         0: aload_0                                                                     
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V   
         4: return                                                                      
      LineNumberTable:                                                                  
        line 7: 0                                                                       
      LocalVariableTable:                                                               
        Start  Length  Slot  Name   Signature                                           
            0       5     0  this   Lcom/basic/java/classStructure/TestClass;           
}                                                                                       
SourceFile: "TestClass.java"                                                    
1234567891011121314151617181920212223242526272829303132333435363738394041424344454647
```

上面的输出结果中有很多信息都与我们刚才分析Class文件所得的结果相符：

#### 次版本号与主版本号：

```
minor version: 0
major version: 52
12
```

#### 2.索引值范围：

```
#1——#21
1
```

#### 3.常量池项目类型分析

```
#1 = Methodref          #4.#18         // java/lang/Object."<init>":()V
1
```

上述代码表示第一项常量指向第4和第15项常量。
至于java/lang/Object.""😦)V这个东西，有关我们之前说的类的全限定名与描述符，等一下再说。

## 3.5 访问标志

常量池结束之后，紧接的两个字节代表访问标志，用于标识一些类或者接口层次的信息，包括：这个Class是类还是接口；是否定义为public类型；是否定义为abstract类型；如果是类，是否被声明为final等。

具体标志位及含义如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211165819218.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

从上面使用Javap命令输出常量表结构之后有这样一行代码：

```
flags: ACC_PUBLIC, ACC_SUPER
1
```

我们查看访问标志可知，这个flags对类的描述是正确的，因此它的access_flags的值应为：0x0001|0x0020=0x0021。如果你对应最上面原始的字节码，会发现这个和在常量池后面所占两个字节所显示的数值相一致。

## 3.6 类索引、父类索引、接口索引集合

访问标志之后，就是类索引和父类索引、接口索引集合了。类索引、父类索引都是一个u2类型的数据，而接口索引集合是一组u2类型的数据集合，Class文件用这三项数据来确定这个类的继承关系。

- 类索引用于确定这个类的全限定名。
- 父类索引用于确定这个类的父类权限定名除外，由于Java语言不允许多重继承，所以父类索引就只有一个，除了java.lang.Object之外，所有的Java类都有父类索引。因此，除了java.lang.Object之外，所有Java类的父类索引都不为0.
- 接口索引集合就是用来描述这个类实现了哪些接口，这些被实现的接口按照implements语句。被实现的接口将按implements语句后的接口顺序从左到右排列在接口索引集合中。

上图中我标记出来的字节码分别为0x0003、0x0004、0x0000，也就是说类索引为常量池中的第3常量、父类索引为常量池中的第4常量。而接口索引有点不同，接口索引的第一项—u2类型的数据为接口计数器，表示索引表的容量。如我刚才所说，可知接口计数器值为0，后面接口的索引表不再占用任何字节。

结合我们刚才Javap命令的输出结果：

```
#3 = Class              #20            // com/basic/java/classStructure/TestClass    
#4 = Class              #21            // java/lang/Object   

#20 = Utf8               com/basic/java/classStructure/TestClass             
#21 = Utf8               java/lang/Object   
12345
```

可以看到第3、4常量又分别指向20、21常量，而它们的值分别为UTF-8格式的TestClass和java/lang/Object。

## 3.7 字段表集合

此表用于描述接口或类中声明的变量。字段包括类级变量（static）和实例级变量，但不包括局部变量。字段包含哪些信息？字段作用域（public、private、protected）、是实例变量还是类变量static、可变性final、并发可见性volatile、可否序列化transient（序列化）、字段数据类型(基本数据类型、字段、数组)、字段名称。

除了字段数据类型、字段名称的字节长度无法固定而需要引用常量池中的内容外，其它修饰符都很适合用标志位来表示。因此字段表主要存储了以下信息：

> 名称索引、描述符索引、访问标志、属性表

- 名称索引就是字段的名称的全限定名
- 描述符索引就是描述字段的数据类型。
- 访问标志就是下面这幅图
- attributes_info用来描述一个字段的额外信息，如：final static int m = 123;，字段表中就会有一个ConstantValue的属性，其值指向常量123。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211165810858.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211165806700.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

在实际情况中，ACC_PUBLIC、ACC_PRIVATE、ACC_PROTECTED三个标志最多只能选择一个，ACC_FINAL、ACC_VOLATILE不能同时选择。接口之中字段必须由ACC_PUBLIC、ACC_STATIC、ACC_FINAL标志，这些都是由Java本身的语言规则定义的。

### 3.7.1 名称索引、全限定名、描述符

名称索引和描述符索引。它们都是对常量池的引用，分别代表着字段的简单名称以及字段和方法的描述符。

#### 简单名称和全限定名

- 简单名称就是指没有类型和参数修饰的方法或字段名称，这个类中的inc()方法和m字段的简单名称分别是“inc”和“m”。
- 全限定名就是 “com/basic/java/classStructure/TestClass;”
- 名称索引就是对简单名称和全限定名在运行时常量池中的索引编号。

#### 描叙符索引

描述符的作用是用来描述字段的数据类型、方法的参数列表（包括数量、类型、顺序）和返回值。根据这些描叙符规则，基本数据类型（byet、char、double、float、int、long、short、boolean）以及代表无返回这的void类型都用一个大写字母来表示，而对象类型则用字符L加对象的全限定名来表示。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211165801177.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

对于数组类型，每一个维度将使用一个前置的“[”字符来描述，例如定义一个“java.lang.String[][]”类型的二维数组，会被记录为：“[[Ljava/lang/String;”，说到这就不得不提全限定名的表示方法了。

#### 描述符的使用

先参数列表、后返回值。

```java
void inc()

java.lang.String.toString()

int indexOf(char[] source, int sourceOffset, int sourceCount, char[] target, int targetOffset, int targetCount, int fromIndex)
12345
```

描述符语言：

```
()V

()Ljava/lang/String;

([CII[CIII)I
12345
```

### 3.8 实例分析

类索引，父类索引、接口索引集合之后便是字段表，它的第一个u2类型数据为容量计数器field_flags。由上面的图可知值为0x0001，也就是说只有一个字段，接下来的u2便是access_flags标志… …依次类推，字段表的固定数据项目分析完成。

注意：最后，字段表集合不会列出从父类中继承而来的字段，但有可能列出原本Java代码里面不存在的字段，譬如在内部类中为了保持对外部类的访问性，会自动添加指向外部类实例的字段。

还有对于字节码来说，如果两个字段的描述符不一致，那字段重名就是合法的。这在Java里面显然是不可能的。

# 4. 方法表

方法表结构：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211165750378.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

方法表与字段表十分相似，在这里我给出方法表的访问标志：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211165745599.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

关于方法表的分析我不在赘述，与前面的分析结果都基本相同。我们来看一下在方法表中需要的注意的几个方面：

> - 方法里面的代码存储在“Code”属性中，我们在下一节中进行讲述。
> - 方法表集合的入口也有一个u2类型的计数器容量的数据。
> - 方法表中也有可能出现编译器自动添加的方法，如 方法。
> - 在Class文件中，描述符不完全一致的方法也可以共存，也就是说在Class文件中就算只有返回值不同，也是一种重载。



# (四)：Class类文件结构(二)

2018年12月11日 17:31:15

 

MasterT-J

 

阅读数：248

更多

所属专栏： [JVM虚拟机](https://blog.csdn.net/column/details/31193.html)



属性表在前面的讲解中出现多次，在Class文件、字段表、方法表都可以携带自己的属性表集合，用于描叙某些场景专有的信息。为了正确解析Class文件，《Java虚拟机规范（第二版）》中预定义了9项虚拟机实现应当识别的属性。然而在最新的《Java虚拟机规范（Java SE7）》中属性表已经增加到了21项。当然我们不用全部记住它们，只需要熟悉其中的几个关键属性。

# 1.属性表的基本结构

本次博客只谈其中的5种属性
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211173035858.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

每个属性的名称都引用自常量池中一个CONSTANT_Utf8_info类型的常量来表示，属性值的结构则完全是自定义，只需要一个u4长度属性去说明属性值所占用的位数即可。

基本属性表的结构图如下所示：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211173042487.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

## 1.1 Code属性

Java方法体中的代码经过Javac编译之后，最终变为字节码存储在Code属性中，Code属性出现在方法表的属性集合之中。**接口或抽象类中的方法并不存在Code属性，因为他们并没有对方法进行具体的实现**。

Code属性表的结构如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211173048197.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

在这里我尝试挑几个概念上比较重要来进行记录。

1. max-stack，要解释清楚它，需要了解操作数栈，栈帧等知识，所以这个属性字段先不谈。
2. max_locals，这个挺有意思。它代表了局部变量所需的存储空间。单位是Slot，虚拟机为局部变量分配内存使用的最小单位。对于byte、char… …这种长度不超过4个字节的数据类型，每个局部变量占用一个Slot，而double、long这两种64位的数据类型则需要两个Slot来存放。方法参数（this）、显式异常处理器参数（try-catch）、方法体中的局部变量都需要使用局部变量表来存放。计算max_locals的值也不是方法中用到了多少个局部变量，就将每个局部变量所占用的Slot算出来最后进行简单求和。事实上局部变量表中的Slot可以重用，当代码执行超出一个局部变量的作用域时，这个局部变量所占用的Slot可以被其他局部变量所使用。Javac编译器会根据变量的作用域来分配Slot给每个变量使用，然后计算出max_locals的大小。
3. code_length不用说代表的是字节码长度。它是一个u4类型的长度值，理论上最大值可以达到2的32次方-1，但Java虚拟机规范限制一个方法不允许超过65535条字节码指令，所以它实际只使用了u2的长度，一旦超过这个长度，编译器会拒绝编译。

Code属性是Class文件中最重要的一个属性，Java程序可以分为代码和元数据，也就是方法体中的代码与字段、类、方法定义等其他信息。因此在Class文件中，只有Code属性用于描述Java方法，其他都用于描述元数据。

我已将常量池部分省略：

```
{                                                                                        
  public com.basic.java.classStructure.TestClass();                                      
    descriptor: ()V                                                                      
    flags: ACC_PUBLIC                                                                    
    Code:                                                                                
      stack=1, locals=1, args_size=1                                                     
         0: aload_0                                                                      
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V    
         4: return                                                                       
      LineNumberTable:                                                                   
        line 7: 0                                                                        
      LocalVariableTable:                                                                
        Start  Length  Slot  Name   Signature                                            
            0       5     0  this   Lcom/basic/java/classStructure/TestClass;            
                                                                                         
  public int c();                                                                        
    descriptor: ()I                                                                      
    flags: ACC_PUBLIC                                                                    
    Code:                                                                                
      stack=2, locals=1, args_size=1                                                     
         0: aload_0                                                                      
         1: getfield      #2                  // Field m:I                               
         4: iconst_1                                                                     
         5: iadd                                                                         
         6: ireturn                                                                      
      LineNumberTable:                                                                   
        line 11: 0                                                                       
      LocalVariableTable:                                                                
        Start  Length  Slot  Name   Signature                                            
            0       7     0  this   Lcom/basic/java/classStructure/TestClass;            
}                                                                                        
SourceFile: "TestClass.java"                                                             
1234567891011121314151617181920212223242526272829303132
```

因为原Java文件中实际上有两个方法，一个实例构造器，一个inc方法。因此第一个public TestClass();代表的是实例构造器。我们可以看到args_size=1，说明这个方法有一个参数，虽然源代码里面两个方法都没有显式参数，但是我们都知道这个参数肯定是this。

当然，这个this只对实例对象有效，也就是说，如果inc方法被声明为static，那么它的args_size就会等于0。

往下看是三条字节码指令，关于字节码指令的内容目前也不进行讨论，所以暂且跳过。

### 异常表

按道理来说，字节码指令之下就是异常表（显式异常处理表），但是此代码并没有生成异常表。所以这个部分对Code来说并不是必须的。

来看一下异常表的结构：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121117305796.png)

异常表包含4个字段，这些字段的含义为：如果字节码从第start_pc到end_pc行之间(不包含第end_pc)行出现了类型为catch_type或其子类的异常(catch_type为指向一个CONSTANT_Class_info型常量的索引)，则转到第handler_pc行继续处理。当catch_type的值为0时，代表任何的异常情况都需要转向到handler_pc行进行处理。

异常表实际上是Java代码的一部分，编译器使用异常表而不是简单的跳转命令来实现Java异常及finally处理机制。注：字节码的“行”是一种形象的描述，指的是字节码相对于方法体开始的偏移量，而不是Java源代码的行号。

## 1.2 Exception属性

属性表结构如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181211173103647.png)

这里的Exceptions属性是在方法表中与Code属性平级的一项属性，而不是Code属性表中的异常属性表。Exceptions属性表的作用是列举出方法中可能抛出的受查异常(Checked Exception)，也就是在方法描述时在throws关键字后面列举的异常。

此属性表中的number_of_exceptions项表示方法可能抛出number_of_exceptions种受检查异常，每一种受检查异常使用一个exception_index_table项表示，指向常量池中CONSTANT_Class_info型常量表的索引，代表了该受检查异常的类型。





# (五)：字节码指令简介

2018年12月11日 17:53:25

 

MasterT-J

 

阅读数：199

更多

所属专栏： [JVM虚拟机](https://blog.csdn.net/column/details/31193.html)



Java 虚拟机的指令由一个字节长度的、代表着某种特定操作含义的数字（称为操作码）以及跟随其后的零至多个代表此操作所需参数（操作数）而构成。由于 Java 虚拟机采用面向操作数栈而不是寄存器的架构，所以大多数的指令都不包含操作数，只有一个操作码。

# 1. 字节码与数据类型

> 在 Java 虚拟机的指令集中，大多数的指令都包含了其操作所对应的数据类型信息。例如，iload 指令用于从局部变量表中加载 int 型的数据到操作数栈中，而 fload 指令加载的则是 float 类型的数据。这两条指令的操作在虚拟机内部可能会是由同一段代码来实现，但在 Class 文件中它们必须拥有各自独立的操作码。

对于大部分与数据类型相关的字节码指令，它们的操作码助记符中都有特殊的字符表明专门为哪种数据类型服务：i 代表对 int 类型的数据操作，l 代表 long，s 代表 short，b 代表 byte，c 代表 char，f 代表 float，d 代表 double，a 代表 reference。也有一些指令的助记符中没有明确的指明操作类型的字母，如 arraylength 指令，它没有代表数据类型的特殊字符，但操作数永远只能是一个数字类型的对象。还有另外一些指令，如无条件跳转指令 goto 则是与数据类型无关的。

# 2. 加载和存储指令

加载和存储指令用于将数据在栈帧中的局部变量表和操作数栈之间来回传输，这类指令包括如下内容：

- 将一个局部变量加载到操作栈：iload、iload_、lload、lload_、fload、fload_、dload、dload_、aload、aload_。
- 将一个数值从操作数栈存储到局部变量表中：istore、istore_、lstore、lstore_、fstore、fstore_、dstore、dstore_、astore、astore_。
- 将一个常量加载到操作数栈：bipush、sipush、ldc、ldc_w、ldc2_w、aconst_null、iconst_m1、iconst_*、lconst_、fconst_、dconst_。*
- 扩充局部变量表的访问索引的指令：wide。

# 3. 运算指令

运算或算术指令用于对两个操作数栈上的值进行某种特定运算，并把结构重新存入到操作栈顶。大体上算术指令可以分为两种：对整形数据进行运算的指令与对浮点型数据进行运算的指令，无论是哪种算术指令，都使用 Java 虚拟机的操作类型，由于没有直接支持 byte、short、char 和 boolean 类型算术指令，对于这些数据的运算，应使用操作 int 类型的指令代替。整数与浮点数的算术指令在溢出和被零除的时候也有各自不同的行为表现，所有的算术指令如下。

- 加法指令：iadd、ladd、fadd、dadd。
- 减法指令：isub、lsub、fsub、dsub。
- 乘法指令：imul、lmul、fmul、dmul。
- 除法指令：idiv、ldiv、fdiv、ddiv。
- 求余指令：irem、lrem、frem、drem。
- 取反指令：ineg、lneg、fneg、dneg。
- 位移指令：ishl、ishr、iushr、lshl、lshr、lushr。
- 按位或指令：ior、lor。
- 按位与指令：iand、land。
- 按位异或指令：ixor、lxor。
- 局部变量自增指令：iinc。
- 比较指令：dcmpg、dcmpl、fcmpg、fcmpl、lcmp。

# 4.类型转换指令

Java 虚拟机直接支持以下数值类型的宽化类型转换（自动转换）：

- int 类型到 long、float 或者 double 类型。
- long 类型到 float、double 类型。
- float 类型到 double 类型。

相对的，处理窄化转换时，必须显式的使用转换指令来完成（即强转）。这些转换指令包括：i2b、i2c、i2s、l2i、f2i、f2l、d2i、d2l 和 d2。

# 5.对象创建与访问指令

虽然类实例和数组都是对象，但 Java 虚拟机对类实例和数组的创建与操作使用了不同的字节码指令。

- 创建类实例的指令：new。
- 创建数组的指令：newarray、anewarray、multianewarray。
- 访问类字段和实例字段的指令：getfield、putfield、getstatic、putstatic。
- 把一个数组元素加载到操作数栈的指令：baload、caload、saload、iaload、laload、faload、daload、aaload。
- 将一个操作数栈的值存储到数组元素中的指令：bastore、castore、sastore、iastore、fastore、dastore、aastore。
- 取数组长度的指令：arraylength。
- 检查类实例类型的指令：instanceof、checkcast。

# 6. 操作数栈管理指令

如同操作一个普通数据结构中的堆栈那样，Java 虚拟机提供了一些用于直接操作数栈的指令，包括：

- 将操作数栈的栈顶一个或两个元素出栈：pop、pop2。
- 复制栈顶一个或两个数值并将复制值或双份的复制值重新压入栈顶：dup、dup2、dup_x1、dup2_x1、dup_x2、dup2_x2。
- 将栈最顶端的两个数值互换：swap。

# 7.控制转义指令

控制转义指令可以让 Java 虚拟机有条件或无条件的从指定的位置指令而不是控制转义指令的下一条指令继续执行程序。

- 条件分支：ifeq、iflt、ifle、ifne、ifgt、ifnull、ifnonnull、if_icmpeq、if_icmpne、if_icmplt、if_icmpgt、if_icmple、if_icmpge、if_acmpeg 和 if_acmpne。
- 复合条件分支：tableswitch、lookupswitch。
- 无条件分支：goto、goto_w、jsr、jsr_w、ret。

# 8. 方法调用和返回指令

这里列出一下 5 条用于方法调用的指令。

- invokevirtual 指令用于调用对象的实例方法，根据对象的实例类型进行分派。
- invokeinterfce 指令用于调用接口方法，它会在运行时搜索一个实现类这个接口方法的对象，找出适合的方法进行调用。
- invokespecial 指令用于调用一些需要特需处理的实例方法，包括实例初始化方法、私有方法和父类方法。
- invokestatic 指令用于调用类方法。
- invokedynamic 指令用于在运行时动态解析出调用点限定符所引用的方法，并执行该方法。

方法调用指令与数据类型无关，而方法返回指令是根据返回值的类型区分的，包括 ireturn（当返回值是 boolean、byte、char、short 和 int 类型时使用）、lreturn、freturn、dreturn 和 areturn。另外还有一条 return 指令供声明为 void 的方法、实例初始化方法以及类和接口的类初始化方法使用。

# 9. 异常处理指令

在 Java 程序显式的抛出异常的操作（throw 语句）都由 athrow 指令来实现。而在 Java 虚拟机中，处理异常（catch 语句）不是有字节码指令来实现的，而是采用异常表来完成的。

# 10. 同步指令

Java 虚拟机可以支持方法级的同步（即同步方法）和方法内部一段指令序列的同步（即同步代码块），这两种同步结构都是使用管程（Monitor）来支持的。

- 方法级的同步是隐式的，即无须通过字节码指令来控制，它实现再方法调用和返回操作之中。虚拟机可以从方法常量池的方法表结构中的 ACC_SYNCHRONIZED 访问标志得知一个方法是否声明为同步方法。当方法调用时，调用指令将会检查方法的 ACC_SYNCHRONIZED 访问标志是否被设置，如果设置了，执行线程就要求先成功持有管程（Monitor），然后才能执行方法，最后当方法完成时释放管城。在方法执行期间，执行线程持有了管程，其他任何线程都无法在获取到同一个管程。如果一个同步方法执行期间抛出了一次，并且在方法内部无法处理此异常，那么这个同步方法所持有的管程将在异常抛出到同步方法之外时自动释放。
- 同步一段指令集序列通常是由 Java 语言中的 synchronized 语句块来表示的，Java 虚拟机的指令集中有 monitorenter 和 monitorexit 两条指令来支持 synchronized 关键字的语义，正确实现 synchronized 关键字需要 Javac 编译器与 Java 虚拟机两者共同协作支持。



# (六)：虚拟机类加载机制

2018年12月12日 16:53:45

 

MasterT-J

 

阅读数：220

更多

所属专栏： [JVM虚拟机](https://blog.csdn.net/column/details/31193.html)



# 1. 概述

虚拟机把描述类的数据从Class文件加载到内存中，并对数据进行校验、转换解析和初始化，最终形成可以被虚拟机直接使用的Java类型，这就是虚拟机的类记载机制。

与那些在编译时需要进行连接工作的语言不同，在Java7语言里面，类型的加载、连接和初始化过程都是在程序运行期间完成的，这种策略虽然会令类加载时稍微增加一些性能开销，但是会为Java程序提高高度的灵活性，在Java里面天生可以动态扩展的语言特性就是依赖运行期动态加载和动态链接这个特点实现的。

# 2. Java虚拟机体系

学习一个东西之前，我们务必要知道，这东西大概是干什么的，有什么作用。为了更清楚的阐释类加载机制到底是干什么的，我先将JVM的结构图贴给大家：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181212165334220.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

如上图，我们要学的类加载机制就是要搞清楚类加载器是如何找到指定的Class文件以及怎样将Class文件装载进内存，以便执行引擎执行Class文件中存在的数据和指令，从而使你的Java程序跑起来。

# 2. 类加载的时机

在我们了解虚拟机类加载机制的开始，我们首先来介绍一下类的生命周期。类从被加载到虚拟机内存中开始，到卸载出内存为止，它的整个生命周期包括：加载、验证、准备、解析、初始化、使用和卸载7个阶段。其中验证、准备、解析3个部分统称为连接。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181212165329984.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

上图中，加载、验证、准备、初始化、卸载这个5个阶段的顺序是确定的，类的加载过程必须按照这种顺序按部就班的开始，而解析阶段则不一定，它在某种情况下可以在初始化阶段之后再开始，这是为了支持Java语言的运行时绑定（也称为动态绑定或者晚绑定）。

现在我们了解了类的加载过程的生命周期，下面我们需要知道什么时候需要类加载过程？Java虚拟机规范中没有进行强制约束，这一点可以交给虚拟机的具体实现来自由把握。但是对于初始化阶段，虚拟机规范则是严格规定了有且只有5种情况必须立即对类进行“初始化”（而加载、验证、准备自然需要在此之前开始）：

> - new一个对象、读取一个类静态字段、调用一个类的静态方法的时候
> - 对类进行反射调用的时候
> - 初始化一个类，发现父类还没有初始化，则先初始化父类
> - 如果一个java.lang.invoke.MethodHandle实例最后的解析结果REF_getStatic、REF_putStatic、REF_invokeStatic的方法句柄，并且这个方法句柄所对应的类没有进行过初始化，则需要先触发其初始化。

对于这五种情况会触发类进行初始化过程，我们称为主动引用。除此之外，所有引用类的方式都不会触发初始化过程，称为被动引用。下面我们来介绍一下下面3种被动引用的情况：

------

## 2.1 被动引用案例1：通过子类引用父类的静态字段，不会导致子类初始化

```java
class SuperClass{
    static {
        System.out.println("SuperClass init");
    }

    public static int  value=123;
}

class SubClass extends SuperClass{
    static {
        System.out.println("SubClass init");
    }
}

public class NotInitialization1 {
    public static void main(String[] args) {
        System.out.println(SubClass.value);
    }
}

运行结果
SuperClass init
123
1234567891011121314151617181920212223
```

对于静态字段，只有直接定义这个字段的类才会被初始化，因此通过其子类引用父类中定义的静态字段、只会触发父类的初始化而不会触发子类的初始化。

## 2.2 被动引用案例2：通过数组定义引用类，不会触发此类的初始化

```java
public class NotInitialization2 {
    public static void main(String[] args) {
        SuperClass[] sca=new SuperClass[20];
    }
}

运行结果

12345678
```

这段代码触发了一个名为“[Lcom.basic.java.classloader.SuperClass”的类初始化阶段，对于用于代码来说，这是并不是一个合法的类名称，它是一个由虚拟机自动生成的、直接继承与java.lang.Object的子类，创建动作由字节码newarray触发。这个类元素代表一个com.basic.java.classloader.SuperClass的一维数组，数组中应用的属性和方法都实现在这个类中。

## 2.3 被动引用案例3：静态常量变量的使用，不会触发类的初始化

```java
class ConstClass{
    static {
        System.out.println("ConstClass init");
    }

    public static final String HELLOWORLD="helloWorld";
}
public class NotInitialization3 {
    public static void main(String[] args) {
        System.out.println(ConstClass.HELLOWORLD);
    }
}

运行结果：
helloWorld
123456789101112131415
```

常量在编译阶段会存入调用类的常量池中，本质上并没有直接引用到定义常量的类，因此不会触发常量的类的初始化。这种调用方式不会触发ConstClass的初始化，因为常量传播优化，常量“hello world”已经被存储到了NotInitialization类的常量池中，以后NotInitialization对常量ConstClass.HELLOWORLD的引用实际上都被转化为NotInitialization对自身常量池的引用。这种做法其实叫做常量的传播优化。

通过本节知识，我们知道了类会在什么时候进行类加载过程，下面我们详细介绍一下类加载机制的具体细节。

# 3.类加载的过程

## 3.1 加载

我们来看在加载阶段，虚拟机需要完成哪些事情：

> - 通过一个类的全限定名来获取定义此类的二进制字节流
> - 将获取到的二进制字节流转化成一种数据结构并放进方法区
> - 在内存中生成一个代表此类的java.lang.Class对象，作为访问方法区中各种数据的接口

过一个类的全限定名来获取定义此类的二进制字节流这一条， 它没有指二进制字节流要从一个Class文件中获取，准确的说是根本没有指明要从哪里获取，怎样获取。虚拟机设计团队在加载阶段搭建了一个非常开放的、广阔的“舞台。我们可以从以下这些地方获得一个类二进制字节流。

> - 在ZIP包中读取，这很常见，最终成为日后JAR、EAR、WAR格式的基础
> - 从网络中获取，这种常见最典型的应用就是Applet
> - 运行时计算生成，这种场景使用得最多的就是动态代理技术，在java.lang.reflect Proxy中，就是用了ProxyGenerator.generateProxyClass来为特定接口生成形式“*$Proxy”的代理类的二进制字节流
> - 由其他文件生成，典型的场景是JSP应用，即由JSP文件生成对应的Class类
> - 从数据库中读取，这种场景相对少见一些，例如有些中间件服务器可以选择把程序安装到数据库中来完成程序代码在集群中的分发

在我们了解了类加载阶段，虚拟机完成了哪些事情之后。下面我们介绍一个特殊的类加载过程：对于数组类价值过程：**数组类本身不通过类加载器创建，它是由Java虚拟机直接创建的**但是数组类与类加载器仍然有很密切的关系，因为数组类的元素类型最终是要靠类加载器去创建，一个数组类创建过程就遵循以下规则：

- 如果数组的组件类型（Component Type，指的是数组去掉一个维度的类型）是引用类型，那就递归采用本节中定义的加载过程去加载这个组件类型，数组C将在加载该组件类型的类加载器的类名称空间上表示
- 如果数组的组件类型不是引用类型（如int[]数组），Java虚拟机将会把数据C标记为与启动类加载器关联
- 数组类的可见性与它的组件类型的可见性一致，如果组件类型不是引用类型，那数组类的可见性将默认为public

加载阶段完成后，虚拟机外部的二进制字节流就按照虚拟机所需的格式存储在方法区之中，方法区中的数据存储格式有虚拟机实现自行定义，虚拟机规范未规定此区域的具体数据结构。然后在内存中实例化一个java.lang.Class类的对象（并没有明确规定是在Java堆中，对于HotSpot虚拟机而言，Class对象比较特殊，它虽然是对象，但是存放在方法区里面），这个对象将作为程序访问方法区中的这些类型数据的外部接口。

## 3.2 验证

从上面类的生命周期一图中我们可以看出，验证是连接的第一步，这一阶段的目的主要是为了确保Class文件的字节流中包含的信息符合当前虚拟机的要求，从而不会危害虚拟机自身安全。也就是说，当加载阶段将字节流加载进方法区之后，JVM需要做的第一件事就是对字节流进行安全校验，以保证格式正确，使自己之后能正确的解析到数据并保证这些数据不会对自身造成危害。

验证阶段主要分成四个子阶段：

> - 文件格式验证
> - 元数据验证
> - 字节码验证
> - 符号引用验证

### 3.2.1 文件格式验证

这一阶段要验证字节流是否符合Class文件格式的规范，并且能被当前版本的虚拟机处理。

### 3.2.2 元数据验证

第二阶段是对字节码描述的信息进行语义分析，以保证其描述的信息符合Java语言规范的要求，这个阶段可能包括的验证点如下：

- 这个类是否有父类（除了java.lang.Object之外，所有的类都应当有父类）
- 这个类的父类是否继承了不允许被继承的类（被final修饰的类）
- 如果这个类不是抽象类，是否实现了其父类或者接口之中要求实现的所有方法
- 类中的字段、方法是否与父类产生矛盾（例如覆盖了父类的final字段，或者出现不符合规则的方法重载，例如方法参数都一致，但返回值类型却不同等）

第二阶段的主要目的是对类的元数据信息进行语义校验，保证不存在不符合Java语言规范的元数据信息。

### 3.2.3 字节码验证

第三阶段是整个验证过程中最复杂的一个阶段，主要目的是通过数据流和控制流分析，确定程序语义是合法的、符合逻辑的。在第二阶段对元数据信息中的数据类型做完校验后，这个阶段将对类的方法体进行校验分析，保证被校验类的方法在运行时不会做出危害虚拟机安全的事件，例如：

- 保证任意时刻操作数栈的数据类型与指令代码序列都能配合工作，例如不会出现类似这样的情况：在操作栈放置了一个int类型的数据，使用时却按照long类型来家载入本地变量表中
- 保证跳转指令不会跳转到方法体以外的字节码指令上
- 保证方法体重的类型转换是有效的，例如可以把一个子类对象赋值给父类数据类型，这是安全的，但是把父类对象赋值给子类数据类型，甚至把对象赋值给与它毫无继承关系，完全不相干的一个数据类型，则是危险和不合法的

### 3.2.4 符号引用验证

最后一个阶段的校验发生在虚拟机将符号引用转换为直接引用的时候，这个转化动作将在连接的第三个阶段——解析阶段发生。符号一弄验证可以看做是对类自身以外（常量池的各种符号引用）的信息进行匹配性校验，统称需要校验下列内容：

- 符号引用中通过字符串描述的全限定名是否能找到对应的类
- 在指定类中是否存在符合方法的字段描述符以及简单名称所描述的方法和字段
- 符号引用中的类、字段、方法的访问性（private、protected、public、default）是否可以被当前类访问

## 3.2.4. 准备阶段（类变量）

1.准备阶段的目的：正式为类变量分配内存并设置类变量初始值的阶段，这些变量所使用的内存将在方法区中分配。

注意我的重点：是类变量（static）不是实例变量，还有，我们又知道了在JVM的方法区中不仅存储着Class字节流（按照运行时方法区的数据结构进行存储，上述的二进制字节流是不严谨的说法，只是为了大家好理解），还有我们的类变量。

2.这里的类变量初始值通常是指数据类型的零值。比如int的零值为0，long为0L，boolean为false… …真正的初始化赋值是在初始化阶段进行的。

额外一点，如果你设置的类变量还具有final字段，如下：

```java
public static final int value = 123;
1
```

那么在准备阶段变量的初始值就会被直接初始化为123，具体原因是由于拥有final字段的变量在它的字段属性表中会出现ConstantValue属性。

## 3.2.5 解析阶段

阶段的目的：虚拟机将常量池内的符号引用替换为直接引用。

常量池：在Class的文件结构中我们就花了大量的篇幅去介绍了常量池，我们再来总结一下：常量池(constant pool)指的是在编译期被确定，并被保存在已编译的.class文件中的一些数据。它包括了关于类、方法、接口等中的常量，也包括字符串常量。

看完上面这句话，你可能会有疑问什么事符号引用，什么是直接引用。

> - 符号引用：符号引用以一组符号来描述所引用的目标，符号可以是任何形式的字面量，只要使用时能无歧义地定位到目标即可。符号引用与虚拟机实现的内存布局无关，引用的目标并不一定已经加载到内存中。各种虚拟机实现的内存布局可以各不相同，但是他们能接受的符号引用必须都是一致的，因为符号引用的字面量形式明确定义在Java虚拟机规范的Class文件格式中。
> - 直接引用：直接引用可以是直接指向目标的指针、相对偏移量或是一个能间接定位到目标的句柄。直接引用是和虚拟机实现的内存布局相关的，同一个符号引用在不同虚拟机实例上翻译出来的直接引用一般不会相同。如果有了直接引用，那引用的目标必定已经存在在内存中。

现在我们对上面那句话进行重新解读：虚拟机将运行时常量池中那些仅代表其他信息的符号引用解析为直接指向所需信息所在地址的指针。

虚拟机规范中并未规定解析阶段发生的具体时间，只要求了在执行anewarray、checkcast、getfield、getstatic、instanceof、invokedynamic、invokeinterface、invokespecial、invokestatic、invokevirtual、ldc、ldc_w、multianewarray、new、putfield和putstatic这16个用于操作符号引用的字节码指令之前，先对他们所使用的符号引用进行解析。所以虚拟机实现可以根据需要来判断到底是在类加载器加载时就对常量池中的符号引用进行解析，还是等到一个符号引用将要被使用前才去解析它。

解析动作主要是有以下执行动作：

- 类或接口的解析（注意数组类和非数组类）
- 字段（简单名称+字段描述符）解析（注意递归搜索）
- 类方法解析（注意递归搜索）
- 接口方法解析（注意递归搜索）

### 在解析阶段还有一个很有意思的东西：动态连接！

它也是上面解析阶段发生时间不确定的直接原因：大部分JVM的实现都是延迟加载或者叫做动态连接。它的意思就是JVM装载某个类A时，如果类A中有引用其他类B，虚拟机并不会将这个类B也同时装载进JVM内存，而是等到执行的时候才去装载。

而这个被引用的B类在引用它的类A中的表现形式主要被登记在了符号表中，而解析的过程就是当需要用到被引用类B的时候，将引用类B在引用类A的符号引用名改为内存里的直接引用。这就是解析发生时间不可预料的原因，而且这个阶段是发生在方法区中的。

## 3.2.6 初始化阶段

类初始化阶段是类加载过程的最后一步，前面的类加载过程中，除了在加载阶段用户应用程序可以通过自定义加载类加载器参与外，其余动作完全由虚拟机主导和控制。到了初始化阶段，财政在开始执行类中定义的Java程序代码。

在准备阶段，类变量（静态变量）已经赋过一次系统要求的初始化值，而在初始化阶段，则根据程序员通过程序制定的主观计划去初始化类变量和其他资源，或者可以从另外一个角度去表达：初始化阶段是执行类构造器<clinit()>方法执行过程中一些可能会影响程序运行行为的特点和细节，这部分相对更贴近于普通的程序开发人员。我们下面来详细讲解<clinit()>方法是怎么生成的。

- clinit()方法是由编译器自动收集类中的所有类变量的赋值动作和静态语句块(static{}块)中的语句合并产生的，编译器收集的顺序是由语句在源文件中出现的顺序所决定的，静态语句块中只能访问到定义在静态语句块之前的变量，定义在它之后的变量，在前面的静态语句块可以赋值，当时不能访问。
- clinit()方法与类的构造器不同，它不需要显示地调用父类构造器，虚拟机会保证在子类的clinit()方法执行之前，父类的clinit()方法已经执行完毕。因此在虚拟机中第一个被执行的clinit()方法的类肯定是java.lang.Object。
- clinit()方法对于类或者接口来说并不是必需的，如果一类中没有静态语句块，也没有对变量的赋值操作，那么编译可以不为这个类生成clinit()方法。
- 接口中不能使用静态语句块，但仍然有变量初始化的赋值操作，因此接口与类一样都会生成clinit()方法。但是接口与类不同的是，执行接口的clinit()方法不需要先执行父接口的clinit()方法。只有当肤疾克中定义的变量使用时，父接口才会初始化。另外，接口的实现类在初始时也一样不会执行接口的clinit()方法。
- 虚拟机会保证一个类的clinit()方法在多线程环境中被正确的加锁、同步，如果多个线程同时去初始化一个类，那么只会有一个线程去执行这个类的clinit()方法，其他线程都需要阻塞等待，知道活动线程执行clinit()方法完毕。如果在一个类的clinit()方法中有耗时很长的操作，就可能造成多个进程阻塞，在实际应用中这种阻塞往往是很隐蔽的。

## 3.3 利用实例分析类加载过程

咱们来看一个例子将上面类加载的过程来串一下吧，加深一下自己的印象

```java
class Lava {

    private int speed = 5;
    void flow() {

    }
}

public class Volcano {

    public static void main(String[] args) {
        Lava lava = new Lava();
        lava.flow();
    }
}
123456789101112131415
```

### 3.3.1 加载

读取一个类的class文件并将其中的二进制字节流组织成正确的数据结构放进运行时方法区中：

> 要运行Volcano程序，首先得以某种“依赖于实现的”方式告诉虚拟机“Volcano”这个名字。之后，虚拟机将找到并读入相应的class文件“Volcano.class”，然后它会从导入的class文件里的二进制数据中提取类型信息并放到方法区中。通过执行保存在方法区中的字节码，虚拟机开始执行main()方法，在执行时，它会一直持有指向当前类（Volcano类）的运行时常量池（方法区中的一个数据结构）的指针。

注意：虚拟机开始执行Volcano类中main()方法的字节码的时候，尽管Lava类还没被装载，但是和大多数（也许所有）虚拟机实现一样，它不会等到把程序中用到的所有类都装载后才开始运行。恰好相反，它只会需要时才装载相应的类。（延迟加载、动态连接）

main()的第一条指令告知虚拟机为列在常量池第一项的类分配足够的内存。所以虚拟机使用指向Volcano常量池的指针找到第一项，发现它是一个对com.basic.java.classloader.Lava类的符号引用，然后它就检查方法区，看Lava类是否已经被加载了。

检查方法区，发现Lava类没有加载，这个时候虚拟机开始进行类加载过程。首先是加载阶段，虚拟机开始查找并装载文件“Lava.class”，并把从读入的二进制数据中提取的类型信息放在方法区中。

### 3.3.2 解析

> 虚拟机以一个直接指向方法区Lava类数据的指针来替换常量池第一项（就是那个字符串“Lava”），以后就可以用这个指针来快速地访问Lava类了。这个替换过程称为常量池解析，即把常量池中的符号引用替换为直接引用。

终于，虚拟机准备为一个新的Lava对象分配内存。此时它又需要方法区中的信息。还记得刚刚放到Volcano类常量池第一项的指针吗？现在虚拟机用它来访问Lava类型信息，找出其中记录的这样一条信息：一个Lava对象需要分配多少堆空间。

AVA虚拟机总能够通过存储在方法区的类型信息来确定一个对象需要多少内存，当JAVA虚拟机确定了一个Lava对象的大小后，它就在堆上分配这么大的空间，并把这个对象实例的变量speed初始化为默认初始值0。

当把新生成的Lava对象的引用压到栈中，main()方法的第一条指令也完成了。接下来的指令通过这个引用调用Java代码（该代码把speed变量初始化为正确初始值5）。另一条指令将用这个引用调用Lava对象引用的flow()方法。

# 4. 类加载器

虚拟机设计团队把类加载阶段中的“通过一个类的全限定名来获取描述此类的二进制字节流”这个动作放到Java机外部来实现，以便让应用程序自己决定如何获取所需的类。实现这个动作的代码模块称为“类加载器”。

## 4.1 类与类加载器

类加载器的命名空间：对于任意一个类，都需要由加载它的类加载器和这个类本身一同确立其在Java虚拟机中的唯一性，每一个类加载器，都拥有一个独立的类命名空间。也就是说，你现在要比较两个类是否相等，只有在这两个类是同一个类加载器加载的前提下才有意义。

我们程序一般使用的都是应用程序类加载器。

## 4.2 双亲委派模型

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181212165308311.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

从Java虚拟机的角度来讲，只存在两种不同的类加载器：一种是启动类加载器（Bootstrap ClassLoader），这个类加载器使用C++语言实现，是虚拟机自身的一部分；另一种就是所有其他的类加载器，这些类加载器都由Java语言实现，独立于虚拟机外部，并且全部继承自抽象类java.lang.ClassLoader。

从Java开发人员的角度来看，类加载器可以划分的更细致一些，绝大部分Java程序都会使用到以下3种系统提供的类加载器。

> - 启动类加载器（Bootstrap ClassLoader）：前面已经介绍过，这个类将存放在\lib目录中的，或者被-Xbootclasspath参数所指定的路径中的，并且是虚拟机识别的（仅按照文件名识别，如rt.jar，名称不符合类库即使放在lib目录中也不会被加载）类库加载到虚拟机内存中。启动类加载器无法被Java程序直接引用，用户在编写自定义类加载器时，如果需要把加载请求委派给引导类加载器，那直接使用null代替即。
> - 扩展类加载器（Extension ClassLoader）：这个加载器由sun.misc.Launcher$ExtClassLoader实现，它负责加载器\lib\ext目录中的，或者被java.ext.dirs系统变量所指定的路径中的所有类库，开发者可以直接使用扩展类加载器。
> - 应用程序类加载器（Application ClassLoader）：这个类加载器由sun.misc.Launcher AppClassLoader实现。由于这个类加载器是ClassLoader中的getSystemClassLoader()方法的返回值，所以一般也称它为系统类加载器。它负责加载用户类路径（ClassPath）上所指定的类库，开发者可以直接使用这个类加载器，如果应用程序中没有自定义过自己的类加载器，一般情况下这个就是程序中的默认类加载器。

我们的应用程序都是由这3种类加载器互相配合进行加载的，如果有必要，还可以加入自己定义的类加载器。

上图中展示的类加载器之间的这种层次关系，称为类加载器的双亲委派模型。双亲委派模型要求除了顶层的启动类加载器外，其余的类加载器都应当有自己的父类加载器。这里类加载器之间的父子关系一般不会以继承的关系来实现，而是都使用组合关系来复用父加载器的代码。

------

**重点**

> 双亲委派模型的工作过程是：如果一个类加载器收到了类加载的请求，它首先不会自己去尝试加载这个类，而是把请求委派给父类加载器去完成，每一个层次的类加载器都是如此，因此所有的加载请求最终都应该传送到顶层的启动类加载器中，只有当父加载器反馈自己无法完成这个加载请求（它的搜索范围中没有找到所需的类）时，子加载器才会尝试自己去加载。



# (七)：虚拟机字节码执行引擎

2018年12月14日 12:17:12

 

MasterT-J

 

阅读数：266

更多

所属专栏： [JVM虚拟机](https://blog.csdn.net/column/details/31193.html)



代码编译的结果就是从本地机器码转变为字节码。我们都知道，编译器将Java源代码转换成字节码？那么字节码是如何被执行的呢？这就涉及到了JVM字节码执行引擎，执行引擎负责具体的代码调用及执行过程。就目前而言，所有的执行引擎的基本一致：

> 1. 输入：字节码文件
> 2. 处理：字节码解析
> 3. 输出：执行结果。

所有的Java虚拟机的执行引擎都是一致的：输入的是字节码执行文件，处理的过程是字节码解析的等效过程，输出的是执行结果。物理机的执行引擎是由硬件实现的，和物理机的执行过程不同的是虚拟机的执行引擎由于自己实现的。

# 1.方法调用

方法调用的主要任务就是确定被调用方法的版本（即调用哪一个方法），该过程不涉及方法具体的运行过程。按照调用方式共分为两类：

1. 解析调用是静态的过程，在编译期间就完全确定目标方法。
2. 分派调用即可能是静态，也可能是动态的，根据分派标准可以分为单分派和多分派。两两组合有形成了静态单分派、静态多分派、动态单分派、动态多分派

Class文件的编译过程不包含传统编译中的连接步骤，一切方法调用在Class文件里面存储的都只是符号引用，而不是方法在实际运行时内存不急的入口地址（相当于说的是直接引用）。

我们知道class文件是源代码经过编译后得到的字节码，如果学过编译原理会知道，这个仅仅完成了一半的工作（词法分析、语法分析、语义分析、中间代码生成），接下来就是实际的运行了。而Java选择的是**动态链接的方式，即用到某个类再加载进内存**，而不是像C++那样使用静态链接：将所有类加载，不论是否使用到。当然了，孰优孰劣不好判断。**静态链接优点在速度，动态链接优点在灵活**。下面我们来详细介绍一下动态链接和静态链接。

# 2. 静态链接

如上面的概念所述，在C/C++中静态链接就是在编译期将所有类加载并找到他们的直接引用，不论是否使用到。而在Java中我们知道，编译Java程序之后，会得到程序中每一个类或者接口的独立的class文件。虽然独立看上去毫无关联，但是他们之间通过接口(harbor)符号互相联系，或者与Java API的class文件相联系。

我们之前也讲述了类加载机制中的一个过程—解析，并在其中提到了解析就是将class文件中的一部分符号引用直接解析为直接引用的过程，但是当时我们并没有详细说明这种解析所发生的条件，现在我给大家进行补充：

> 方法在程序真正运行之前就有一个可确定的调用版本，并且这个方法的调用版本在运行期是不可改变的。可以概括为：编译期可知、运行期不可变。此类方法主要包括静态方法和私有方法两大类，前者与类型直接关联，后者在外部不可访问，因此决定了他们都不可能通过继承或者别的方式重写该方法，符合这两类的方法主要有以下几种：静态方法、私有方法、实例构造器、父类方法。

# 3. 动态链接

如上所述，在Class文件中的常量持中存有大量的符号引用。字节码中的方法调用指令就以常量池中指向方法的符号引用作为参数。这些符号引用一部分在类的加载阶段（解析）或第一次使用的时候就转化为了直接引用（指向数据所存地址的指针或句柄等），这种转化称为静态链接。而相反的，另一部分在运行期间转化为直接引用，就称为动态链接。

与那些在编译时进行链接的语言不同，Java类型的加载和链接过程都是在运行的时候进行的，这样虽然在类加载的时候稍微增加一些性能开销，但是却能为Java应用程序提供高度的灵活性，Java中天生可以动态扩展的语言特性就是依赖动态加载和动态链接这个特点实现的。

# 4. 解析

在Java虚拟机中提高了5中方法调用字节码指令：

> 1. invokestatic：调用静态方法，解析阶段确定唯一方法版本
> 2. invokespecial：调用方法、私有及父类方法，解析阶段确定唯一方法版本
> 3. invokevirtual：调用所有虚方法
> 4. invokeinterface：调用接口方法
> 5. invokedynamic：动态解析出需要调用的方法，然后执行

前四条指令固化在虚拟机内部，方法的调用执行不可认为干预，而invokedynamic指令则支持由用户确定方法版本。

非虚方法：其中invokestatic指令和invokespecial指令调用的方法称为非虚方法，符合这个条件的有静态方法、私有方法、实例构造器、分类方法这4类。Java中的非虚方法除了使用invokestatic指令和invokespecial指令调用的方法之外还有一种，就是final修饰的方法。虽然final方法是使用invokevirtual指令来调用的，但是由于它无法被覆盖没有其他版本，所以也无须对方法接受者进行多态选择，又或者多态选择的结果是唯一的。Java语言规范中明确说明了final方法也是一直用非虚方法。所以对于非虚方法中，Java通过编译阶段，将方法的符号引用转换为直接引用。因为它是编译器可知、运行期不可变得方法。

解析调用一定是一个静态过程，在编译期间就完全确定，在类装载的解析阶段就会把涉及的符号引用全部转换为确定的直接引用，不会延迟到运行期再去完成。而分派调用则可能是静态的也可能是动态的，根据分派依据的宗量数量可以分为单分派和多分派。

# 5. 分派

分派调用更多的体现在多态上。

**宗量**的定义：方法的接受者（亦即方法的调用者）与方法的参数统称为方法的宗量。单分派是根据一个宗量对目标方法进行选择，多分派是根据多于一个宗量对目标方法进行选择。

> - 静态分派：所有依赖静态类型3来定位方法执行版本的分派成为静态分派，发生在编译阶段，典型应用是方法重载。
> - 动态分派：在运行期间根据实际类型4来确定方法执行版本的分派成为动态分派，发生在程序运行期间，典型的应用是方法的重写。
> - 单分派：根据一个宗量对目标方法进行选择。
> - 多分派：根据多于一个宗量对目标方法进行选择。

介绍分派之前我们先来对**静态类型**和**实际类型**进行定义：

```java
Human man = new Man();
1
```

如上代码，Human被称为静态类型，Man被称为实际类型。

```java
//实际类型变化
Human man = new Man();
man = new Woman();

//静态类型变化
StaticDispatch sr = new StaticDispatch();
sr.sayHello((Human) man);
sr.sayHello((Woman) man);
12345678
```

可以看到的静态类型和实际类型都会发生变化，但是有区别：静态类型的变化仅仅在使用时发生，变量本身的静态类型不会被改变，并且最终的静态类型是在编译期可知的，而实际类型变化的结果在运行期才可确定。

## 5.1 静态分派（重载 静态类型）

所有依赖静态类型来定位方法执行版本的分派动作称为静态分派。静态分派的典型应用是方法重载。

我们来看一下下面这个应用程序：

```java
class Human {
}

class Man extends Human {
}

class Woman extends Human {
}

public class StaticDispatch {

    public void sayHello(Human guy) {
        System.out.println("hello, guy!");
    }

    public void sayHello(Man guy){
        System.out.println("hello, gentleman!");
    }

    public void sayHello(Woman guy){
        System.out.println("hello, lady!");
    }

    public static void main(String[] args){
        Human man = new Man();
        Human woman = new Woman();
        StaticDispatch sr = new StaticDispatch();
        sr.sayHello(man);
        sr.sayHello(woman);
    }
}

运行结果：
hello, guy!
hello, guy!
1234567891011121314151617181920212223242526272829303132333435
```

如上代码与运行结果，在调用 sayHello()方法时，方法的调用者都为sr的前提下，使用哪个重载版本，完全取决于传入参数的数量和数据类型。代码中刻意定义了两个静态类型相同、实际类型不同的变量，可见编译器（不是虚拟机，因为如果是根据静态类型做出的判断，那么在编译期就确定了）在重载时是通过参数的静态类型而不是实际类型作为判定依据的。并且静态类型是编译期可知的，所以在编译阶段，javac 编译器就根据参数的静态类型决定使用哪个重载版本。因此，在编译期间，Javac编译器会根据参数的静态类型决定使用哪个重载版本，所以选择了sayHello(Human)作为调用目标，并把这个方法的符号引用写到main()方法的两条invokevirtual指令参数中。

所谓依赖静态类型来定位方法执行版本的分派动作称为静态分派。静态分派的典型应用是方法重载。静态分派发生在编译阶段，因此确定静态分派的动作实际上不是有虚拟机表执行的。

## 5.2 动态分派（重写 实际类型）

动态分派与多态性的另一个重要体现——方法重写有着很紧密的关系。向上转型后调用子类覆写的方法便是一个很好地说明动态分派的例子。这种情况很常见，因此这里不再用示例程序进行分析。很显然，在判断执行父类中的方法还是子类中覆盖的方法时，如果用静态类型来判断，那么无论怎么进行向上转型，都只会调用父类中的方法，但实际情况是，根据对父类实例化的子类的不同，调用的是不同子类中覆写的方法，很明显，这里是要根据变量的实际类型来分派方法的执行版本。而实际类型的确定需要在程序运行时才能确定下来，这种在运行期根据实际类型确定方法执行版本的分派过程称为动态分派。

我们再来看一下下下面应用程序：

```java
/**
 * locate com.basic.java.classExecution
 * Created by MasterTj on 2018/12/14.
 * 方法动态分派演示
 */
public class DynamicDispatch {
    static abstract class Human{
        protected abstract void sayHello();
    }

    static class Man extends Human{

        @Override
        protected void sayHello() {
            System.out.println("man SayHello！！");
        }
    }

    static class Woman extends Human{

        @Override
        protected void sayHello() {
            System.out.println("Woman SayHello！！");
        }
    }

    public static void main(String[] args) {
        Human man=new Man();
        Human woman=new Woman();

        man.sayHello();;
        woman.sayHello();

        man=new Woman();
        man.sayHello();
    }
}

运行结果：
man SayHello！！
Woman SayHello！！
Woman SayHello！！
123456789101112131415161718192021222324252627282930313233343536373839404142
```

对于虚函数的调用，在JVM指令集中是调用invokevirtual指令。下面我们来介绍一下invokevirtual指令的动态查找过程，invokevirtual指令的运行时解析过程大致可以分为以下几个步骤：

1. 找到操作栈顶的第一个元素所指向的对象的实际类型，记做C。
2. 如果在类型C中找到与常量的描述符和简单名称都像符的方法，则进行访问权限校验，如果通过则返回这个方法的直接引用，查找过程结束。如果不通过，则返回java.lang.IllegalAccessError异常。
3. 否则按照继承关系从下往上依次对C的各个父类进行第二步的搜索和验证过程。
4. 否则始终没有找到合适的方法，则抛出java.lang.AbstractMethodError异常

由于invokevirtual指令执行把常量池中的类方法符号引用解析到了不同的直接引用上，这个过程就是Java语言的方法重写的本质。

## 5.3 单分派与多分派

单分派是根据一个宗量对目标方法进行选择，多分派是根据多于一个宗量对目标方法进行选择。

我们再来看一下下面应用程序：

```java
class Eat {
}

class Drink {
}

class Father {
    public void doSomething(Eat arg) {
        System.out.println("爸爸在吃饭");
    }

    public void doSomething(Drink arg) {
        System.out.println("爸爸在喝水");
    }
}

class Child extends Father {
    public void doSomething(Eat arg) {
        System.out.println("儿子在吃饭");
    }

    public void doSomething(Drink arg) {
        System.out.println("儿子在喝水");
    }
}

public class SingleDoublePai {
    public static void main(String[] args) {
        Father father = new Father();
        Father child = new Child();
        father.doSomething(new Eat());
        child.doSomething(new Drink());
    }
}

运行结果：

爸爸在吃饭
儿子在喝水
123456789101112131415161718192021222324252627282930313233343536373839
```

我们首先来看编译阶段编译器的选择过程，即静态分派过程。这时候选择目标方法的依据有两点：一是方法的接受者（即调用者）的静态类型是 Father 还是 Child，二是方法参数类型是 Eat 还是 Drink。因为是根据两个宗量进行选择，所以 **Java 语言的静态分派属于多分派类型**。

再来看运行阶段虚拟机的选择，即动态分派过程。由于编译期已经了确定了目标方法的参数类型（编译期根据参数的静态类型进行静态分派），因此唯一可以影响到虚拟机选择的因素只有此方法的接受者的实际类型是 Father 还是 Child。因为只有一个宗量作为选择依据，所以 **Java 语言的动态分派属于单分派类型**。

**目前的 Java 语言（JDK1.6）是一门静态多分派（方法重载）、动态单分派（方法重写）的语言。**

# 6. 方法的执行

下面我们来探讨虚拟机是如何执行方法中的字节码指令的，上文提到，许多Java虚拟机的执行引擎在执行Java代码的时候都用解释执行（通过解释器执行）和编译执行（通过及时编译器产生本地代码）

## 6.1 解释执行

在jdk 1.0时代，Java虚拟机完全是解释执行的，随着技术的发展，现在主流的虚拟机中大都包含了即时编译器(JIT)。因此，虚拟机在执行代码过程中，到底是解释执行还是编译执行，只有它自己才能准确判断了，但是无论什么虚拟机，其原理基本符合现代经典的编译原理，如下图所示： 大部分程序代码到物理机的目标代码或虚拟机能执行的指令之前，都需要经过以下各个步骤。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121456514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

大多数虚拟机都会遵循这种基于现代经典编译原理的思路，在执行对程序源码进行词法分析和语法分析处理，把源码转换为抽象语法树。对于一门具体语言的实现来说，词法分析、语法分析至后面的优化器和后面的代码生成器都可以选择独立于执行引擎，形成一个完整意义的编译器去实现,这类代表就是C/C++语言。也可以选择一部分步骤（如生成语法树之前的步骤）实现为一个半独立的编译器，这类代表就是Java语言。又或者把这些步骤和执行引擎全部集中在一个封闭的黑匣子里面，如大多数的JavaScript执行器。

Java语言中，Javac编译器完成了程序代码经过词法分析、语法分析到抽象语法树，再遍历语法树生成线性的字节码指令流的过程。这一部分动作是在java虚拟机之外进行的，而解释器（JTI）在虚拟机内部，所以Java程序的编译就是半独立的实现。

## 6.2 基于栈的指令集与基于寄存器的指令集

Java编译器输入的指令流基本上是一种基于栈的指令集架构，指令流中的指令大部分是零地址指令，其执行过程依赖于操作栈。另外一种指令集架构则是基于寄存器的指令集架构，典型的应用是x86的二进制指令集，比如传统的PC以及Android的Davlik虚拟机。两者之间最直接的区别是，基于栈的指令集架构不需要硬件的支持，而基于寄存器的指令集架构则完全依赖硬件，这意味基于寄存器的指令集架构执行效率更高，单可移植性差，而基于栈的指令集架构的移植性更高，但执行效率相对较慢，初次之外，相同的操作，基于栈的指令集往往需要更多的指令，比如同样执行2+3这种逻辑操作，其指令分别如下：

基于栈的指令集运行的就是经过JIT解释器解释执行的指令流，基于寄存器的指令集运行的就是目标机器代码的指令。

基于栈的指令集的优势和缺点：

> - 优点：可以移植性强，寄存器由硬件进行保护，程序直接依赖这些应将寄存器而不可避免地要受到硬件的约束。
> - 缺点：栈架构指令集的代码非常紧凑，但是完成相同功能所需要的指令数量一般会比寄存器的架构多，因为出栈、入栈操作本身就产生了相当多的指令数量。更重要的是，栈实现在内存之中，频繁的栈访问也就意味着频繁的内存访问，相对于处理器来说，内存始终是执行速度的瓶颈。

基于栈的计算流程（以Java虚拟机为例）：

```java
iconst_2  //常量2入栈
istore_1  
iconst_3  //常量3入栈
istore_2
iload_1
iload_2
iadd      //常量2、3出栈，执行相加
istore_0  //结果5入栈
12345678
```

而基于寄存器的计算流程：

```
mov eax,2  //将eax寄存器的值设为1
add eax,3  //使eax寄存器的值加3
12
```

## 6.3 基于栈的代码执行示例

下面我们用简单的案例来解释一下JVM代码执行的过程，代码实例如下：

```java
public class MainTest {
    public  static int add(){
        int result=0;
        int i=2;
        int j=3;
        int c=5;
        return result =(i+j)*c;
    }

    public static void main(String[] args) {
        MainTest.add();
    }
}
12345678910111213
```

使用javap指令查看字节码：

```
{
  public MainTest();
    flags: ACC_PUBLIC
    Code:
      stack=1, locals=1, args_size=1
         0: aload_0
         1: invokespecial #1                  // Method java/lang/Object."<init>":()V
         4: return
      LineNumberTable:
        line 2: 0

  public static int add();
    flags: ACC_PUBLIC, ACC_STATIC
    Code:
      stack=2, locals=4, args_size=0     //栈深度2，局部变量4个，参数0个
         0: iconst_0  //对应result=0,0入栈
         1: istore_0  //取出栈顶元素0，将其存放在第0个局部变量solt中
         2: iconst_2  //对应i=2,2入栈
         3: istore_1  //取出栈顶元素2，将其存放在第1个局部变量solt中
         4: iconst_3  //对应 j=3，3入栈
         5: istore_2  //取出栈顶元素3，将其存放在第2个局部变量solt中
         6: iconst_5  //对应c=5，5入栈
         7: istore_3  //取出栈顶元素，将其存放在第3个局部变量solt中
         8: iload_1   //将局部变量表的第一个slot中的数值2复制到栈顶
         9: iload_2   //将局部变量表中的第二个slot中的数值3复制到栈顶
        10: iadd      //两个栈顶元素2,3出栈，执行相加，将结果5重新入栈
        11: iload_3   //将局部变量表中的第三个slot中的数字5复制到栈顶
        12: imul      //两个栈顶元素出栈5,5出栈，执行相乘，然后入栈
        13: dup       //复制栈顶元素25，并将复制值压入栈顶.
        14: istore_0  //取出栈顶元素25，将其存放在第0个局部变量solt中
        15: ireturn   //将栈顶元素25返回给它的调用者
      LineNumberTable:
        line 4: 0
        line 5: 2
        line 6: 4
        line 7: 6
        line 8: 8

  public static void main(java.lang.String[]);
    flags: ACC_PUBLIC, ACC_STATIC
    Code:
      stack=1, locals=1, args_size=1
         0: invokestatic  #2                  // Method add:()I
         3: pop
         4: return
      LineNumberTable:
        line 12: 0
        line 13: 4
}

1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950
```

执行过程中代码、操作数栈和局部变量表的变化情况如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121505127.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121511150.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121412151959.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121524600.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121530640.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121412153641.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121554992.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121600752.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121609460.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121613608.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121412161893.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121623164.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121628486.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121633958.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181214121639733.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)







# (八)：编译器优化

2018年12月15日 19:27:53

 

MasterT-J

 

阅读数：264

更多

所属专栏： [JVM虚拟机](https://blog.csdn.net/column/details/31193.html)



本博客从编译期源码实现的层次上让我们了解了Java源代码编译为字节码的过程，分析了Java语言中泛型、主动装箱/拆箱、条件编译等多种语法糖的前因后果。

# 1. 概述

java语言的“编译期”其实是一段“不确定”的操作过程，因为它可能是指一个前端编译器（其实叫“编译器的前端”更准确点）把*.java文件转化为*.class文件的过程；也可能是指虚拟机的后端运行期编译器（JIT编译器，Just In Time Compiler）把字节码转化为机器码的过程；还可能是指使用静态提前编译器（AOT编译器，Ahead Of Time Compiler）直接把*.java文件编译成本地机器代码的过程。下面列举了这3类编译过程中一些比较有代表性的编译器。

> 1. 前段编译器：Sun的Javac、Eclipse JDT中的增量式编译器（ECJ）；
> 2. JIT编译器：GNU Compiler for the Java（GCJ）、Excelsior JET；
> 3. AOT编译器：GNU Compiler for the Java（GCJ）、Excelsior JET；

# 2. Javac编译器

Javac编译器不像HotSpot虚拟机那样使用C++语言（包含C少量C语言）实现，它本身就是一个由Java语言编写的程序，这为纯Java的程序员了解它的编译过程带来了很大的便利。

Javac编译器大致上可以分为三个过程，分别是：

> 1. 解析与填充符合表过程；
> 2. 插入式注解处理器的注解处理过程；
> 3. 语义分析与字节码生成过程。

这三个步骤之间的关系与交互顺序如图所示：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215192740868.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

## 2.1 解析与填充符号表

### 2.1.1 词法、语法分析

**词法分析**是将源代码的字符流转变为标记（Token）集合，单个字符是程序编写过程的最小元素，而标记则是编译过程的最小元素，关键字、变量名、字面量、运算符都可以成为标记，如“int a = b + 2”这句代码包含了6个标记，分别是int、a、=、b、+、2，虽然关键字int由3个字符构成，但是它只是一个Token，不可再拆分。在Javac的源码中，词法分析过程由com.sun.tools.javac.parser.Scanner类来实现。

**语法分析**是根据Token序列构造抽象语法树的过程，抽象语法树（Abstract Syntax Tree，AST）是一种用来描述程序代码语法结构的树形表示方式，语法树的每一个阶段都代表着程序代码中的一个语法结构（Construct），例如包、类型、修饰符、运算符、接口、返回值甚至代码注释等都可以是一个语法结构。

### 2.1.2 填充符号表

完成了语法分析和词法分析之后，下一步就是填充符号表的过程。符号表（Symbol Table）是由一组符号地址和符号信息构成的表格，读者可以把它想象成哈希表中K-V值对的形式（实际上符号表不一定是哈希表实现，可以是有序号表、树状符号表、栈结构符号表等）。符号表中所登记的信息在编译的不同阶段都要用到。在语义分析中，符号表所登记的内容将用于语义检查（如检查一个名字的使用和原先的说明是否一致）和产生中间代码。在目标代码生成阶段，当对符号名进行地质分配时，符号表是地址分配的依据。

## 2.2 注解处理器

在JDK1.5之后，Java语言提供了对注解（Annotation）的支持，这些注解与普通的Java代码一样，是在运行期间发挥作用的。在JDK1.6中实现了JSR-269规范，提供了一组插入式注解处理器的标准API在编译期间对注解进行处理，我们可以把它看做是一组编译器的插件，在这些插件里面，可以读取、修改、添加抽象语法树中的任意元素。如果这些插件在处理注解期间对语法树进行了修改，编译器将回到解析及填充符号表的过程重新处理，直到所有插入式注解处理器都没有再对语法树进行修改为止，每一次循环称为一个Round，也就是上图中的回环过程。

有了编译器注解处理的标准API后，我们的代码才有可能干涉编译器的行为，由于语法树中的任意元素，甚至包括代码注释都可以在插件之中访问到，**所以通过插入式注解处理器实现的插件在功能上有很大的发挥空间。只要有足够的创意，程序员可以使用插入式注解处理器来实现许多原本只能在编码中完成的事情，本章最后会给出一个使用插入式注解处理器的简单实战**

## 2.3 语义分析与字节码生成

语法分析之后，编译器获得了程序代码的抽象语法树表示，语法树能表示一个结构正确的源程序的抽象，但无法保证源程序是符合逻辑的。而语义分析的主要任务是对结构上正确的源程序进行上下文有关性质的审查，如进行类型审查。举个例子，假设有如下的3个变量定义语句：

```java
int a = 1;
boolean b = false;
char c = 2;
123
```

后续可能出现的赋值运算：

```java
int d = a + c;
int d = b + c;
char d = a + c;
123
```

后续代码中如果出现了如上3中赋值运算的话，那它们都能构成结构正确的语法树，但是只有第一种的写法在语义上是没有问题的，能够通过编译，其余两种在Java语言中是不合逻辑的，无法编译（是否合乎语义逻辑必须限定在具体的语言与具体的上下文环境之中才有意义。如在C语言中，a、b、c的上下文定义不变，第2、3种写法都是可以正确编译）。所以我们就需要进行语义分析，javac编译器中语义分析包括三个步骤：标注检查、数据及控制流分析、解语法糖。

### 2.3.1 标注检查

Javac的编译过程中，语义分析过程分为标注检查以及数据及控制流分析两个步骤。

标注检查步骤检查的内容包括诸如变量使用前是否已被声明、变量与赋值之间的数据类型是否能够匹配等。在标注检查步骤中，还有一个重要的动作称为常量折叠。

### 2.3.2 数据及控制流分析

数据及控制流分析是对程序上下文逻辑更进一步的验证，它可以检查出诸如程序局部变量在使用前是否有赋值、方法的每条路径是否都有返回值、是否所有的受查异常都被正确处理了等问题。编译时期的数据及控制流分析与类加载时的数据及控制流分析的目的基本上是一致的，但校验范围有所区别，有一些校验项只有在编译期或运行期才能进行。

### 2.3.3 解语法糖

语法糖（System Sugar），也成糖衣语法，是由英国计算机科学家彼得-约翰-兰达（Peter J.Landin）发明的一个术语，指在计算机语言中添加的某种语法，这种语法对语言的功能并没有影响，但是更方便程序员使用。通常来说，使用语法糖能够增加程序的可读性，从而减少程序代码出粗的机会。

Java在现代编程语言之中属于“低糖语言”（相对于C#及许多其他JVM语言来说），尤其是JDK1.5之前的版本，“低糖”语法也是Java语言被怀疑已经“落后”的一个表面理由。Java中最常用的语法糖主要是前面提到过的泛型（泛型并不一定都是语法糖实现，如C#的泛型就是直接由CLP支持的）、变长参数、自动装箱/拆箱等，虚拟机运行时不支持这些语法，它们在编译阶段还原回简单地基础语法结构，这个过程称为解语法糖。Java的这些语法糖被解除后是什么样子，我们将在下一个章节进行介绍。

### 2.3.4 字节码生成

字节码生成是Javac编译过程的最后一个阶段，在Javac源码里面由com.sun.tools.javac.jvm.Gen类来完成。字节码生成阶段不仅仅是把前面各个步骤所生成的信息（语法树、符号表）转化成字节码写到磁盘中，编译器还进行了少量的代码添加和转换工作。

例如，前面章节中多次提到的实例构造器（）方法和类构造器（）方法就是在这个阶段添加到语法树之中的（注意，这里的实例构造器并不是指默认构造函数，如果用户代码中没有提供任何构造函数，那编译器将会添加一个没有参数的、访问性（public、protected或private）与当前类一致的默认构造函数，这个工作在填充符号表阶段就已经完成），这两个构造器的产生过程实际上是一个代码收敛的过程，编译器会把语句块（对于实例构造器而言是“{}”块，对于类构造器而言是“static{}”块）、变量初始化（实例变量和类变量）、调用父类的实例构造器（仅仅是实例构造器，()方法中无须调用父类的()方法，虚拟机会自动保证父类构造器的执行，但在()方法中经常会调用java.lang.Object的()方法的代码）等操作收敛到()和()方法之中，并且保证一定是按先执行父类的实例构造器，然后初始化变量，最后执行语句块的顺序进行，上面所述的动作由Gen.normalizeDefs()方法来实现。

# 3. Java语法糖

Java语法糖主要包括泛型擦除、自动装箱和自动拆箱、以及遍历循环和条件编译。

## 3.1 泛型与类型擦除

泛型是JDK1.5的一项新增特性，它的本质是参数化类型（Parametersized Type）的应用，也就是说所操作的数据类型被指定为一个参数。这种参数类型可以用在类、接口和方法的创建中，分别称为泛型类、泛型接口和泛型方法。

下面一段简单的Java泛型的例子，我们可以看一下它编译后的结果是怎样的。

```java
//10-2  泛型擦除前的例子
public static void main(String []args){
        Map<String, String> map = new HashMap<String, String>();
        map.put("hello", "您好");
        map.put("how are you?", "吃了没？");
        System.out.println(map.get("hello"));
        System.out.println(map.get("how are you?"));
}
12345678
```

这段代码编译成Class文件，然后再用字节码反编译工具进行反编译后，将会发现泛型都不见了，程序又变回了Java泛型出现之前的写法，泛型类型都变回了原生类型。

```java
//代码清单10-3  泛型擦除后的样子
	public static void main(String[] args) {
		Map map = new HashMap();
		map.put("hello", "你好,");
		map.put("how are you!", "吃了么?");
		System.out.println((String)map.get("hello"));
		System.out.println((String)map.get("how are you!"));
	}
12345678
```

我们来看一下方法重载的代码,下面重载的代码方法是不可以编译的，因为由于泛型擦除，变成了一样的原生类型List，擦除动作导致这两种方法的特征签名变得一模一样。

```java
/**
 * 代码清单10-4
 * 当泛型遇到重载 1
 * @author Peter
 *
 */
public class GenericTypes {
 
	public static void method(List<String> list){
		System.out.println("invoke method(List<String> list)");
	}
	
	public static void method(List<Integer> list){
		System.out.println("invoke method(List<Integer> list)");
	}
}
12345678910111213141516
```

下面我们再来看一下方法重载的代码：

```java
/**
 * 代码清单10-5
 * 当泛型遇到重载2
 * @author Peter
 *
 */
public class GenericTypes1 {
 
	public static String method(List<String> lits){
		System.out.println("invoke method(List<String> list)");
		return "";
	}
	
	public static int method(List<Integer> list){
		System.out.println("invoke method(List<Integer> list)");
		return 1;
	}
	
	public static void main(String[] args) {
		method(new ArrayList<String>());
		method(new ArrayList<Integer>());
	}
}
1234567891011121314151617181920212223
```

两个method方法添加了不同的返回值，由于这两个返回值的加入，方法重载居然成功了，即这段代码可以被编译和执行了。

Class文件方法表（method_info）的数据结构时曾经提到过，方法重载要求方法具备不同的特征签名，返回值并不包含在方法的特征签名之中，所以返回值不参与重载选择，但是在Class文件格式之中，只要描述符不是完全一致的两个方法就可以共存。也就是说，两个方法如果有相同的名称和特征签名，但返回值不同，那它们也是可以合法地共存于一个Class文件中的。

## 3.3 自动装箱、自动拆箱与遍历循环

从纯技术的角度来讲，自动装箱、自动拆箱与遍历循环（Foreach循环）这些语法糖，无论是实现上还是思想上都不能和上文介绍的泛型相比，两个的难度和深度都有很大的差距。它们是Java语言里使用得最多的语法糖。

下面我们来通过一个例子来理解自动装箱、自动拆箱与遍历循环。

```java
/**
 * 代码清单10-6
 * 自动装箱、拆箱与遍历循环
 * @author Peter
 *
 */
public class Test03 {
 
	public static void main(String []args){
		
		List<Integer> list = Arrays.asList(1, 2, 3, 4);
		//如果在JDK1.7中，还有另外一种语法糖
		//能让上面这句代码进一步简写成List<Integer> list = [1, 2, 3, 4];
		int sum = 0;
		for(int i : list){
			sum += i;
		}
		System.out.println(sum);
	}
}
1234567891011121314151617181920
```

在经过编译器编译之后，Java语法糖拆解之后的代码：

```java
/**
 * 代码清单10-7
 * 自动装箱、拆箱与遍历循环编译之后
 * @author Peter
 *
 */
public class Test04 {
	
	public static void main(String[] args) {
		List list = Arrays.asList(new Integer[]{
			Integer.valueOf(1),
			Integer.valueOf(2),
			Integer.valueOf(3),
			Integer.valueOf(4)
		});
		int sum = 0;
		for(Iterator localIterator = list.iterator(); localIterator.hasNext();){
			int i = ((Integer) localIterator.next()).intValue();
			sum += i;
		}
		System.out.println(sum);
	}
}
1234567891011121314151617181920212223
```

我们可以看出：

1.自动装箱

```java
Integer a=1;
//编译器优化
Integer a=new Integer(1);
123
```

2.自动拆箱

```java
Integer a=1;
int i = a+2;
//编译器优化
Integer a=new Integer(1);
int i= a.intValue()+2;
12345
```

3.循环编译

```java
List<Integer> list = Arrays.asList(1, 2, 3, 4);
for(int i : list){

}
//编译器优化
List list = Arrays.asList(new Integer[]{
			Integer.valueOf(1),
			Integer.valueOf(2),
			Integer.valueOf(3),
			Integer.valueOf(4)
});
for(Iterator localIterator = list.iterator(); localIterator.hasNext();){

}
1234567891011121314
```

## 3.4 条件编译

许多程序设计语言都提供了条件编译的途径，如C、C++中使用预处理指示符（#ifdef）来完成条件编译。C、C++的预处理器最初的任务是解决编译时的代码依赖关系（如非常常用的#include预处理命令），而在Java语言之中并没有使用预处理器，因为Java语言天然的编译方式（编译器并非一个个地编译Java文件，而是将所有编译单元的语法树顶级节点输入到待处理列表后再进行编译，因此各个文件之间能够互相提供符号信息）无须使用预处理器。

**Java语言当然也可以进行条件编译，方法就是使用条件为常量的if语句**

下面我们来看一下Java条件编译的用法：

```java
/**
 * 代码清单10-9 Java语言的条件编译
 * @author Peter
 *
 */
public class Test06 {
	
	public static void main(String[] args) {
		if(true){
			System.out.println("block 1");
		}else{
			System.out.println("block 2");
		}
	}
}
123456789101112131415
```

Java条件编译优化后的代码：

```java
public class Test06 {
	public static void main(String[] args) {
		System.out.println("block 1");
	}
}
12345
```

Java语言中条件编译的实现，也是Java语言的一颗语法糖，根据布尔常量值的真假，编译器将会把分支中不成立的代码块消除掉，这一工作将在编译器解除语法糖阶段（com.sun.tools.javac.comp.Lower类中）完成。







# (九)：运行期优化与JIT编译器

2018年12月15日 20:26:18

 

MasterT-J

 

阅读数：263

 

标签： [JVM虚拟机](http://so.csdn.net/so/search/s.do?q=JVM%E8%99%9A%E6%8B%9F%E6%9C%BA&t=blog) 更多

个人分类： [深入理解JVM虚拟机](https://blog.csdn.net/qq_21125183/article/category/7768772)

所属专栏： [JVM虚拟机](https://blog.csdn.net/column/details/31193.html)



# 1. JIT编译器的引入

首先我们这篇文章中所说的编译器都是指JVM的组成部分之一—即时编译器（JIT），与生成Java字节码的javac编译器要区分开来。首先我们这篇文章中所说的编译器都是指JVM的组成部分之一—即时编译器（JIT），与生成Java字节码的javac编译器要区分开来。JIT的出现，是为了补强虚拟机边运行边解释的低性能。它会智能地对热点代码进行优化且重复利用，最终将这些代码编译为与本地平台相关的机器码。

# 2. 解释器与编译器并存的架构体系

我们可能会问为什么虚拟机要使用解释器与编译器并存的架构体系？主要是有以下几个原因：

> 1. 当程序需要迅速启动和执行时，解释器可以首先发挥作用，省去编译的时间，立即执行。
> 2. 当程序运行后，随着事件的推移，JIT编译器逐渐发挥作用，把越来越多的代码编译成本地代码之后，可以获取更高的执行效率。
> 3. 当程序运行环境中的内存资源限制较大的时候，可以使用解释器执行节约内存，反之可以使用编译器执行来提高效率。
> 4. 解释器还可以作为编译器优化时的一个“逃生门”，让编译器根据概率选择一些大多数时候都能提升运行速度的优化手段。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215202502117.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

引入及时编译器之后整个JVM的工作流程如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215202508852.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

## 2.1 Client模式和Server模式

在HotSpot中还内置了两个即时编译器，分别是Client Compiler和Server Compiler，也称为C1编译器与C2编译器，在目前的HotSpot JVM中默认采用的是解释器与其中一个编译器直接配合的方式工作。我们可以使用“-client”或“-server”参数去指定解释器与具体的某个编译器配合工作。这也就是Client模式和Server模式的本质—指定了不同的JIT编译器进行工作。

**Client版本（C1编译器）启动快，Server版本（C2编译器）运行快**。至于为什么会产生这样的效果却鲜有人说明，其实就是因为两种编译器之间的差异。为编译器编译本地代码也是需要占用程序运行时间的。

- C1编译器：C1编译器主要是进行简单、可靠的优化，因此Client模式加载速度较快而Server模式运行起来较快。
- C2编译器：C2编译器为了编译出优化程度更高的代码主要是进行一些编译耗时较长的优化，甚至会进行激进优化。
- 分层编译：JVM团队为了在程序启动响应速度和与运行效率之间达到最佳平衡，设计出了分层编译。在JDK1.7的Server模式中，分层编译被作为默认编译策略开启，分层编译根据编译器编译、优化的规模与耗时，划分出不同的编译层次。

## 2.2 编译对象与触发即时编译的条件

在了解了为什么需要引入JIT编译器之后，现在需要讨论的就是哪些代码会被JIT编译器进行编译。

会被JIT编译器编译的“热点代码”有两类：

> - 被多次调用的方法
> - 被多次执行的循环体

OSR（Open Stack Replacement）编译：由于编译是发生在方法执行的过程中，因此会产生“栈上替换”（OSR编译）的行为，也就是方法栈帧还在栈上，方法就被替换了。

**热点探测**：那么判断一段代码是不是热点代码，是不是需要触发即时编译，这样的行为称为热点编译。目前主流的热点探测判断方式主要有两种：

> 1. 基于采样的热点探测: JVM周期性的检查各个线程的栈顶，如发现某个方法经常出现在栈顶，那这个方法就是“热点方法”。这个方法的劣势很明显，如果发生线程阻塞，那将会扰乱热点探测。
> 2. 基于计数器的热点探测: HotSpot虚拟机采用这种方法。它会为每个方法建立计数器，统计方法的执行次数，如果执行次数超过了一定的阀值，就可以认为它是“热点方法”。

“热点探测”技术给我们提供了寻找“热点方法”的途径，而计数器则是这条途径的具体实现。HotSpot虚拟机为每个方法提供了两种计数器，这两个计数器都有一定的阀值，当计数器超过这个阀值溢出了，就会触发JIT编译。

### 2.2.1 方法调用计数器

这个计数器用于统计方法被调用的次数，对应“热点代码”中“被多次调用的方法”。有兴趣的同学可以查查它的默认阀值。阀值可以通过虚拟机参数-XX:CompileThreshold进行设定。

我们重点来看一下方法调用计数器触发即时编译的整个流程。

当一个方法被调用时，会先检查方法是否存在被JIT编译过的版本，如果存在，则优先使用编译后的本地代码来执行。如果不存在已经被编译的版本，则将此方法的调用计数器加1，然后判断方法调用计数器与回边计数器（稍后说明）之和是否超过方法调用计数器的阀值，如果已经超过阀值，那么将会向即时编译器提交一个该方法的代码编译请求。在下次进行方法调用的时候，重复此流程。

具体流程如下图：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215202515804.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

从图中可以看到，在向即时编译器提交编译请求之后，执行引擎并不会进行阻塞，而是继续进入解释器按照解释方式执行字节码，直到提交的请求被编译器编译完成，这样做很明显不会造成程序运行中的阻塞。并且，我们可以判断，即时编译由一个后台线程操作进行。

在方法调用计数器中还有两个特别重要的概念：**方法调用计数器的热度衰减与半衰周期**。

如果不做任何设置，方法调用计数器统计的并不是方法调用的绝对次数，而是一个相对的执行频率。也就是说，如果在一定的时间内，方法调用的次数不足以让它提交给即时编译器编译，那么这个方法的调用计数器就会被减少一半，这个过程就是方法调用计数器的热度衰减。而这段时间，就是此方法统计的半衰周期。

进行热度衰减的动作是在垃圾收集的时候顺便进行的。我们可以通过调节虚拟机参数指定是否进行热度衰减，或者调整它的半衰周期。

### 2.2.2 回边计数器

用于统计一个方法中循环体代码执行的次数。关于回边计数器的阀值不同的模式有不同的计算方法，不在这里进行讨论。

回边计数器触发JIT编译的流程与方法调用计数器极其类似。

当解释器遇到一条**回边指令**（编译原理的相关知识，可以粗略理解为循环）时，会先检查将要执行的代码片段是否存在被JIT编译过的版本，如果存在，则优先使用编译后的本地代码来执行。如果不存在已经被编译的版本，则将此方法的回边计数器加1，然后判断**方法调用计数器与回边计数器之和是否超过回边调用计数器的阀值**，如果已经超过阀值，那么将会向即时编译器提交一个OSR编译请求，并且会把回边计数器的值降低一些，以便继续在解释器中执行循环。在下次进行方法调用的时候，重复此流程。

流程图如下：

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215202521157.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

回边计数器还有另外值得注意的地方：虽然编译动作是由循环体所触发，但是编译器仍然会编译整个方法，因此在回边计数器溢出的时候，它还会把方法计数器的值也调整到溢出状态。在下次进入该方法的时候就会执行标准编译过程。

# 3. 编译优化技术

Java 程序员有一个共识，以编译方式执行本地代码比解释器方式执行更快，之所以会有这样的共识，除去虚拟机介绍执行字节码时额外消耗时间的原因外，还有一个很重要的原因就是虚拟机设计团队几乎把对所有的优化措施都集中在了即时编译器之中，因此一般来说，即时编译器产生的代码会比Javac产生的字节码更加优秀。下面我们将介绍一些HotSpot虚拟机的即时编译器在生成代码时采用的代码优化技术。

## 3.1 优化技术概览

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181215202600408.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121520260769.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

## 3.2 方法内联

即时编译器在将字节码翻译成本地机器码之前，还会对字节码进行一系列的优化，因此JIT编译器产生的本地代码会比javac产生的字节码更加优秀。

JVM设计团队采用的优化手段多不胜数，《深入理解Java虚拟机》一书中列举了方法内联、冗余访问消除、复写传播、无用代码消除、公共子表达式消除、数组边界检查消除、逃逸分析等优化手段。

方法内联的重要性要高于其他优化措施，它的目的有二

> 1. 去除方法调用的成本（建立栈帧）
> 2. 为其他优化建立良好的基础。

优化前的代码：

```java
static class B {
    int value;

    final int get() {
        return value;
    }
}

public void foo() {
    y = b.get();
    // ...do stuff...
    z = b.get();

    sum = y + z;
}
123456789101112131415
```

内联后的代码：

```java
public void foo() {
    y = b.value;
    // ...do stuff...
    z = b.value;

    sum = y + z;
}
1234567
```

方法内联看起来很简单，但按照经典编译原理的优化理论，大多数的Java方法都无法进行内联。

还记得我们在前面讲述的方法解析与分派吗？在Java中，大多数的方法都是虚方法（虚方法的定义可以参见之前博客），这就导致了不到运行期JVM根本不知道实际调用的是哪一个方法版本。那么在JIT编译期（晚期优化还是发生在运行期之前）做内联的时候也就无法确定应该使用的方法版本。

例如如果有ParentB与SubB两个具有继承关系的类，并且子类重写了父类的get方法，那么要执行父类的get方法还是执行子类的get方法，需要到运行期才能确定，JIT编译期是无法得出结论的。

## 3.3 守护内联与内联缓存

为了解决虚方法的内联问题，JVM设计团队引入了一种“**类型继承关系分析（CHA）**”的技术。它用于确定在目前已加载的类中，某个接口是否有多于一种的实现，某个类是否存在子类，子类是否为抽象类等信息。

编译器在进行内联时，如果是**非虚方法**，那么直接进行内联就可以了，这时候的内联是有稳定前提保障的。

如果遇到**虚方法**，则会向CHA查询此方法在当前程序下是否有多个目标版本可供选择，如果查询结果只有一个版本，那也可以进行内联，不过这种内联就属于激进优化，需要预留一个“**逃生门**”（解释器或C1编译器），称为守护内联（Guarded Inlining）。如果程序的后续执行过程中，虚拟机一直没有加载到会令这个方法的接收者的继承关系发生变化的类，那这个内联优化的代码就可以一直使用下去。如果加载了导致继承关系发生变化的新类，那就需要抛弃已经编译的代码，退回到解释状态执行，或者重新进行编译。(类文件可动态加载即类关系可能在运行时被修改)

如果向CHA查询出来的结果是有多个版本的目标方法可供选择，则编译器还将会进行最后一次努力，使用**内联缓存**（Inline Cache）来完成方法内联，这是一个建立在目标方法正常入口之前的缓存，它的工作原理大致是：在未发生方法调用之前，内联缓存状态为空，当第一次调用发生后，缓存记录下方法接收者的版本信息，并且每次进行方法调用时都比较接收者版本，如果以后进来的每次调用的方法接收者版本都是一样的，那这个内联还可以一直用下去。如果发生了方法接收者不一致的情况，就说明程序真正使用了虚方法的多态特性，这时才会取消内联，查找虚方法表进行方法分派（动态分派 方法重写 实际类型）。

所以说，在许多情况下虚拟机进行的内联都是一种激进优化，激进优化的手段在高性能的商用虚拟机中很常见，除了内联之外，对于出现概率很小（通过经验数据或解释器收集到的性能监控信息确定概率大小）的隐式异常、使用概率很小的分支等都可以被激进优化“移除”，如果真的出现了小概率事件，这时才会从“逃生门”回到解释状态重新执行。





# （十）：Java内存模型与多线程

2018年12月21日 20:53:34

 

MasterT-J

 

阅读数：264

更多

所属专栏： [JVM虚拟机](https://blog.csdn.net/column/details/31193.html)



# 1. 硬件的效率与缓存一致性

由于存储设备和处理器运算速度之间的存在巨大的差异，现在计算机系统在内存与处理器之间加入高速缓存来作为处理器与内存之间的缓冲。将处理器需要的数据复制到缓存中，让处理器可以快速的获取数据进行计算，计算结束后再从缓存同步带内存中去，这样处理器无需等待缓慢的内存读写。虽然它很好的解决了处理器与存储的速度矛盾，但是它也为计算机系统带来更高的复杂度以及一个新问题：缓存一致性。

> 在多处理器系统中，每个处理器都有自己的高速缓存，而它们又共享同一主内存，当多个处理器的运算任务都涉及同一块主内存区域时，而它们各自的缓存数据又不一致，那么同步回主内存时以谁的缓存数据为主呢？

为了解决一致性的问题，那怎么才能解决这个问题呢？

> 为了解决缓存一致性的问题，我们的操作系统提出了总线锁定机制以及缓存一致性原则。

- 总线锁定：当CPU要对一个操作数进行操作的时候，其在总线上发出一个LOCK#信号，其他处理器就不能操作缓存了该共享变量内存地址的缓存，也就是阻塞了其他CPU，使该处理器可以独享此共享内存。
- 缓存一致性：当某块CPU对缓存中的数据进行操作了之后，就通知其他CPU放弃储存在它们内部的缓存，或者从主内存中重新读取

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018122120523192.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

上图说明了处理器，高速缓存，主内存之间的交互关系。Java虚拟机规范中试图定义一种**Java内存模型（JMM）**可以用来屏蔽掉各种硬件和操作系统的内存访问差异，以实现让Java程序在各种平台下都能达到一致的内存存储效果。

# 2. Java内存模型

## 2.1 主内存和工作内存

我们已经了解了Java的内存模型是什么以及它有什么用，现在就来谈一谈主内存与工作内存：

> - 主内存：Java内存模型规定了所有变量都存储在主内存中，注意，这里说的变量与平常Java编程中说的变量有所区别，它包括了实例字段，静态字段和构成数组对象的元素，它不包括局部变量与方法参数，因为后者是线程私有的。也就是说，我们可以这样理解，除过线程私有的局部变量和方法参数之外，所有的变量都存在于主内存中。
> - 工作内存：内存可以和计算机中的物理内存进行类比，而工作内存可与高速缓存类比。工作内存是 JMM 的一个抽象概念，并不真实存在。它涵盖了缓存，写缓冲区，寄存器以及其它的硬件和编译器优化。

在我们了解了主内存和工作内存的概念之后，那我们来了解一下什么是Cache缓存和Buffer缓冲区?

> - buffer(缓冲区)：将数据缓冲下来，解决速度慢和快的交接问题；速度快的需要通过缓冲区将数据一点一点传给速度慢的区域。例如：从内存中将数据往硬盘中写入，并不是直接写入，而是缓冲到一定大小之后刷入硬盘中。
> - cache(缓存)：实现数据的重复使用，速度慢的设备需要通过缓存将经常要用到的数据缓存起来，缓存下来的数据可以提供高速的传输速度给速度快的设备。例如：将硬盘中的数据读取出来放在内存的缓存区中，这样以后再次访问同一个资源，速度会快很多。

每个线程都有一个自己的工作内存，该内存空间保存了被该线程使用到的变量的主内存副本，线程对变量的所有操作（读取，赋值等）都必须在工作内存中进行，而不直接读写主内存中的变量。看了这段话也许你会问，那假如线程访问一个10MB的对象，难道也会把这10MB的内存复制一份拷贝出来？这当然是不可能的，它有可能会将对象的引用，对象中某个线程访问到的字段拷贝出来，但绝不会将整个对象拷贝一次。

这个时候你可能会有一个问题：那就是JMM和Java虚拟机运行时的数据区到底有什么区别。

> 这里所讲的主内存，工作内存与Java内存区域中的Java堆，栈，方法区等并不是同一个层次的划分，这两者基本上是没有关系的。如果两者一定要勉强对应起来，那么变量，主内存，工作内存依次对应Java堆中对象实例数据部分，工作内存对应虚拟机栈中的部分区域。从更低层次上来说，主内存直接对应于物理硬件的内存，工作内存优先存储于寄存器以及高速缓存。

## 2.2 内存间交互操作

现在我们再来详细讨论一下一个变量是怎么从主内存拷贝到工作内存的，而工作内存的变量又是怎么同步回主内存的呢？

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181221205237970.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

上图说明了工作内存和主内存之间交互的步骤，还有图上缺少的两种原子性操作分别是lock锁定，unlock解锁。由于这两个操作和内存之间的交互并没有关系，所以分开来说。Java定义了8中操作，虚拟机实现时必须保证下面提到的每一种操作都是原子的、不可再分的。

> - lock(锁定)：作用于主内存的变量，它把一个变量标识为一条线程独占的状态
> - unlock(解锁)：作用于主内存的变量，它把一个处于锁定状态的变量释放出来，释放后的变量才可以被其他线程锁定。
> - read（读取）：作用于主内存变量，把变量的值从主内存传输到线程的工作内存
> - load（载入）：作用于主内存变量，把read操作从主内存中得到的变量值放入工作内存的变量副本中
> - use（使用）：作用于工作内存变量
> - assign（赋值）：作用于工作内存变量
> - store（存储）：作用于工作内存变量，将工作内存中一个变量的值传送回主内存
> - write（写入）：作用于主内存变量，将工作内存中得到的变量值放入主内存的变量中。

下面我们来看看对于这8种原子操作的使用：

> - read与load：从主存复制变量到当前线程工作内存
> - use与assign：执行代码，改变共享变量值
> - store与write：用工作内存数据刷新主存对应变量的值

## 2.3 volatile关键字

现在我们对Java内存模型已经有了一定的认识，这个时候我们再来谈谈volatile这个轻量级同步机制。

当一个变量定义为volatile之后，这个变量它将具备这两个特性：

> 1. 保证变量对所有线程时可见的，这里的“可见性”指的就是一条线程修改了这个变量的值，新值对于其他线程来说是可以立刻得到的

volatile关键字作用：强制从公共堆中取得变量的值，而不是从线程的私有堆栈中取得变量的值。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181221205244242.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

从图中可以看到，volatile保证了变量的新值能立即同步到主内存，以及每次使用之前立即从主内存刷新。因此可以说volatile保证了多线程操作时变量的可见性，而普通变量不能保证这一点。

由于volatile变量只能保证可见性，在不符合以下两条规则的运算场景中，我们仍要通过加锁（使用synchronized关键字或者java.util.concurrent中的原子类）来保证原子性：

1. 运算结果并不依赖变量的当前值，或者能够确保只有单一的线程修改变量的值。
2. 变量不需要与其他状态变量共同参与不变的约束

> 1. 使用volatile变量的第二个语义就是禁止指令重排序。

指令重排序：指CPU采用了允许将多条指令不按程序规定的顺序分开发送给各相应电路单元处理。但并不是说指令任意重排，CPU需要能正确处理指令依赖情况以保证程序能得出正确的执行结果。

## 2.4 原子性、可见性和有序性

ava内存模型是围绕并发过程中如何处理原子性、可见性、有序性3个特征来建立的。

### 2.4.1 原子性

在Java中，对基本数据类型的访问读写都是原子性的（long、double除外）。

Int y = 1是一个原子操作，x++不是原子操作（3个操作：先读取x，x加1，加1后的新值写入x）。Java内存模型提供了lock和unlock这两个操作来满足原子操作需求。在字节码层次是使用monitorenter和monitorexit指令隐式使用这两个操作，在Java代码层次就是同步块synchronize。所以synchronize块之间的操作具有原子性。

### 2.4.2 可见性

可见性是指一个线程修改了共享变量的值，其他线程可以立即得到这个修改。Java中synchronize和final关键字也可以实现可见性。

### 2.4.3 有序性

可以通过volatile关键字来保证一定的“有序性”。另外可以通过synchronized和Lock来保证有序性，很显然，synchronized和Lock保证每个时刻是有一个线程执行同步代码，相当于是让线程顺序执行同步代码，自然就保证了有序性。

Java语言提供了volatile和synchronized两个关键字来保证线程之间操作的有序性，volatile关键字本身就包含了禁止指令重排序的语义，而synchronized则是由“一个变量在同一时刻只允许一条线程对其进行lock操作”这条规则获得的，这个规则决定了持有同一个锁的两个同步块只能串行地进入。

## 2.5 先行发生原则

先行发生是Java内存模型中定义的两项操作之间的偏序关系，如果操作A先行发生于操作B，其实就是说在发生操作B之前，操作A产生的影响能被操作B观察到，“影响”包括修改了内存中共享变量的值/发送了消息/调用了方法等。

```java
  i = 1；//在线程A中执行
    j = i;//在线程B中执行
    i = 2;//在线程C中执行
    //A先于B，但是C与B没有先行关系，B存在读取过期数据风险，不具备多线程安全性
1234
```

下面是Java内存模型下一些“天然的”先行发生关系，无须任何同步器协助就已经存在，可直接在编码中使用。如果两个操作之间的关系不在此列，并且无法从下列规则推倒出来，它们就没有顺序性的保障，虚拟机可以对它们进行随意地重排序。

> 1. 程序次序规则（Program Order Rule）：在一个线程内，按照程序代码顺序，书写在前面的操作先行发生于书写在后面的操作。准确地来说应该是控制流顺序而不是程序代码顺序，因为要考虑分支/循环结构。
> 2. 管程锁定规则（Monitor Lock Rule）：一个unlock操作先行发生于后面对同一锁的lock操作。这里必须强调的是同一锁，而“后面”是指时间上的先后顺序。
> 3. volatile变量规则（Volatile Variable Rule）：对一个volatile变量的写操作先行发生于后面对这个变量的读操作，这里的“后面”是指时间上的先后顺序。
> 4. 线程启动规则（Thread Start Rule）：Thread对象的start()方法先行发生于此线程的每一个动作。
> 5. 线程终止规则（Thread Termination Rule）：线程中的所有操作都先行发生于对此线程的终止检测，我们可以通过Thread.join()方法结束/Thread.isAlive()的返回值等手段检测到县城已经终止执行。
> 6. 线程中断规则（Thread Interruption Rule）：对线程interrupt()方法的调用先行发生于被中断线程的代码检测到中断事件的发生，可以通过Thread.interrupted()方法检测到是否有中断发生。
> 7. 对象终结规则（Finalizer Rule）：一个对象的初始化完成（构造函数执行结束）先行发生于它的finalize()方法的开始。
> 8. 传递性（Transitivity）：如果操作A先行发生于操作B，操作B先行发生于操作C，那么操作A先行发生于操作C。

时间上的先后顺序与先行发生原则之间基本没有太大的关系，所以我们衡量并发安全问题时不要受时间顺序的干扰，一切必须以先行发生原则为准。

# 3. Java与线程

## 3.1 线程的实现

线程是比进程更加轻量级的调度执行单位，线程的引入，可以把一个进程的资源调度和执行调度分开，各个线程既可以共享进程资源（内存地址、文件I/O）又可以独立调度（线程是CPU调度的最小单位）

实现线程主要三种方式：

1. 使用内核线程实现。
2. 使用用户线程实现。
3. 使用用户线程加轻量级进程实现。

### 3.1.1 使用内核线程实现

内核线程（Kernel Thread， KLT）就是直接由操作系统内核（Kernel，下称内核）支持的线程，这种线程由内核来完成线程切换，内核通过操纵调度器（Scheduler）对线程进行调度，并负责将线程的任务映射到各个处理器上。每个内核线程都可以看作是内核的一个分身，这样操作系统就有能力同时处理多件事情，支持多线程的内核就叫多线程内核（Multi-Thread Kernel）。

程序一般不会直接去使用内核线程，而是去使用内核线程的一种高级接口——轻量级进程（Light Weight Process， LWP），轻量级进程就是我们通常意义上所讲的线程，由于每个轻量级进程都由一个内核线程支持，因此只有先支持内核线程，才能有轻量级进程。这种轻量级进程与内核线程之间1：1的关系称为一对一的线程模型。

轻量级进程的局限性：由于是基于内核线程实现的，所以各种进程操作，如创建/析构及同步，都需要进行系统调用。而系统调用的代价相对较高，需要在用户态（User Mode）和内核态（Kernel Mode）中来回切换；每个轻量级进程都需要有一个内核线程的支持，因此轻量级进程需要消耗一定的内核资源（如内核线程的栈空间），因此一个系统支持轻量级进程是有限的。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181221205257386.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

### 3.1.2 使用用户级线程实现

狭义上的用户线程指的是完全建立在用户空间的线程库上，系统内核不能感知到线程存在的实现。用户线程的建立/同步/销毁和调度完全在用户态完成，不需要内核的帮助。如果程序实现得当，

优点：这种线程不需要切换到内核态，因此操作快速且低消耗，也可以支持规模更大的线程数量，部分高性能数据库中的多线程就是由用户线程实现的。这种进程与用户线程之间1：N的关系称为一对多的线程模型。

缺陷：不需要系统内核支援，缺陷也在于没有系统内核的支援，所有的线程操作都需要用户程序自己处理。线程的创建、切换、调度都是需要考虑的问题。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181221205305716.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

### 3.1.3 使用用户线程加轻量级进程混合实现

既存在用户线程，也存在轻量级进程。

1. 用户线程还是完全建立在用户空间中，因此用户线程的创建、切换、析构等操作依然廉价，并且可以支持大规模的用户线程并发
2. 操作系统提供的轻量级进程则作为用户线程和内核线程之间的桥梁，这样可以使用内核提供的线程调度功能以及处理器映射，并且用户线程的系统调用都要通过轻量级线程来完成，大大降低了整个进程被完全阻塞的风险。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181221205311625.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

## 3.1.4 Java线程的实现

对于Sun JDK来说，它使用的都是一对一的线程模型实现的。一条Java线程就映射到一条轻量级线程之中。

# 3.2 Java线程调度

线程调度是指系统为线程分配处理器使用权的过程。主要调度方式两种：

> 1. 使用协同调度的多线程系统，线程执行时间由线程本身控制，线程把自己的工作执行完后，要主动通知系统切换到另外一个线程上去。优点：实现简单。缺点：执行时间不可控制。
> 2. 使用抢占调用的多线程系统，每个线程由系统分配执行时间，线程的切换不由线程本身决定。Java使用的就是这种线程调度方式。

Java提供10个级别的线程优先级设置，不过，线程优先级不靠谱，因为Java线程是被映射到系统的原生线程上实现的，所以线程调度最终还是由操作系统决定。

## 3.2.1 状态转换

Java语言定义了5种进程状态，在任意一个时间点，一个线程只能有且只有其中一种状态：

### 1. 新建（New）

新建（New）：创建尚未启动的线程处于这种状态。

### 2. 运行（Runable）

运行（Runable）：包括操作系统线程状态中的Running和Ready，处于此状态的线程可能正在运行，也可能等待着CPU为它分配执行时间。

### 3. 无限期等待（Waiting）

无限期等待（Waiting）：处于这种状态的线程不会被分配CPU执行时间，它们要等待其他线程显示地唤醒。以下方法会让线程陷入无限期的等待状态：

没有设置Timeout参数的Object.wait()方法。

没有设置Timeout参数的Thread.join()方法。

LockSupport.park()方法。

### 4. 限期等待（Timed Waiting）

限期等待（Timed Waiting）：处于这种状态的线程也不会被分配CPU执行时间，不过无须等待被其他线程显示地唤醒，在一定时间后由系统自动唤醒。以下方法会让线程陷入限期的等待状态：

Thread.sleep()方法。

设置了Timeout参数的Object.wait()方法。

设置了Timeout参数的Thread.join()方法。

LockSupport.parkNanos()方法。

LockSupport.parkUntil()方法。

### 5. 阻塞（Blocked）

阻塞（Blocked）：线程被阻塞了，“阻塞状态”与“等待状态”的区别是：“阻塞状态”在等待获取一个排它锁，这个事件将在另外一个线程放弃这个锁的时候发生；“等待状态”则是在等待一段时间，或者唤醒动作的发生。在程序进入等待进入同步块区域的时候，线程将进入这种状态。

### 6. 结束（Terminated）

结束（Terminated）：已终止线程的线程状态，线程已经结束执行。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181221205321325.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)





# （十一）：线程安全与锁优化

2018年12月21日 21:29:31

 

MasterT-J

 

阅读数：235

更多

所属专栏： [JVM虚拟机](https://blog.csdn.net/column/details/31193.html)



# 1. 线程安全

线程安全的定义：当多个线程访问一个对象时，如果不用考虑这些线程在运行时环境下的调度和交替执行，也不需要进行额外的同步，或者在调用方法进行任何其他的协调操作，调用这个对象的行为都可以获得正确的结果，那么这个对象是线程安全的。

## 1.1 Java语言中的线程安全

这里讨论的线程安全，就限定于多个线程之间存在共享数据访问的这个前提。

按照线程安全的“安全程度”由强到弱排序，可以把Java中各个操作共享的数据分为以下5类：

> 1. 不可变
> 2. 绝对线程安全
> 3. 相对线程安全
> 4. 线程兼容
> 5. 线程对立

### 1. 不可变

不可变（Immutable）的对象一定是线程安全的。

如果共享数据是一个基本数据类型，定义时使用final关键字修饰可保证它不可变。

如果共享数据是一个对象，那就需要保证对象的行为不会对其状态产生任何影响才行。其中最简单的是把对象中带有状态的变量都声明为final，这样在构造函数结束后，它就是不可变的。

Java API中符合不可变要求的类型：java.lang.String/java.lang.Number部分子类等。

### 2. 绝对线程安全

绝对线程安全满足线程安全的定义。Java API中标注自己是线程安全的类，大多数时候都不是绝对的线程安全。

### 3. 相对线程安全

相对线程安全就是我们通常意义上所讲的线程安全，它需要保证对这个对象单独的操作时线程安全的，我们在调用时不需要做额外的保障措施，但是对于一些特定顺序的连续调用，就可能需要在调用端使用额外的同步手段来保证调用的正确定。

Java中，大部分线程安全类都属于这种类型，Vector/HashTable/Collections的synchronizedCollection()方法包装的集合等。

### 4. 线程兼容

线程兼容是指对象本身并不是线程安全的，但是可以通过在调用端正确地使用同步手段来保证对象在并发环境中安全使用。Java API中大部分类都是线程兼容的。

### 5. 线程对立

是指不管调用端是否采取了同步措施，都无法在多线程环境中并发使用的代码。通常是有害的，应当尽量避免。

一个线程对立的例子是Thread类的suspend()和resume()方法，如果两个线程同时持有一个线程对象，一个尝试去中断线程，另一个尝试去恢复线程，如果并发进行的话，无论调用是否进行了同步，目标线程都是存在死锁风险的，如果suspend()中断的线程就是即将要执行resume()的那个线程，那就肯定要产生死锁了。正是因为此原因，此两方法已被JDK废弃了。

## 1.2 线程安全的实现方法

### 1.2.1 互斥同步

互斥同步（Mutual Exclusion & Synchronization）是常见的一种并发正确性保障手段。同步是指在多个线程并发访问共享数据时，保证共享数据在同一个时刻只被一个（或者是一些，使用信号量的时候）线程使用。而互斥是实现同步的一种手段，临界区（Critical Section）、互斥量（MuTex）和信号量（Semaphore）都是主要的互斥实现方式。因此，在这四个字里面，互斥是因，同步是果；互斥是方法，同步是目的。

#### synchronized关键字

在Java中，最基本的互斥同步手段是synchronized关键字

synchronized关键字实现原理：

- synchronized关键字经过编译后，会在同步代码块的前后分别形成monitorenter和monitorexit这两个字节码指令，这两个字节码都需要一个reference类型的参数来指明要锁定和解锁的对象。如果程序中synchronized指明了对象参数，那就是这个对象的reference；如果没有指明，那就根据synchronized修饰的是实例方法还是类方法，去取对应的对象实例或Class对象来作为锁对象。
- 虚拟机规范要求，在执行monitorenter指令时，首先尝试获取对象的锁。如果对象没有被锁定或者当前线程已经拥有了那么对象的锁，把锁的计数器加1，执行monitorexit时，将锁计数减1，当锁计数器为0时，锁被释放。如果获取对象锁失败，当前线程将阻塞等待。

Java中的线程是映射到操作系统的原生线程上的，如果要阻塞或则唤醒一个线程，都需要操作系统来帮忙，这就需要从用户态转换到内核态中，因此状态转换需要耗费絮都CPU处理器的时间。所以说synchronized关键字是一个重量级锁，而虚拟机本身也会进行一些优化，譬如在通知操作系统阻塞线程之前加上一段自旋等待过程，避免频繁地切入到内核态中。

#### 重入锁（ReentrantLock）来实现同步

重入锁（ReentrantLock）代码写法上面会有区别，一个表现为API层面的互斥锁，另一个表现为原生语法层面的互斥锁。

ReentrantLock增加了一些高级功能：

> 1. 等待可中断：当持有的锁的线程长期不释放锁时，正在等待的线程可以选择放弃等待，改为处理其他事情，对处理执行时间长的同步块很有帮助。
> 2. 公平锁：多个线程等待同一锁时，必须按照申请锁的时间顺序来依次获得锁；而非公平锁不保证这一点，在锁被释放时，任何一个等待锁的线程都有机会获得锁。synchronized中的锁是非公平的，ReentrantLock默认也是非公平，但可以通过构造函数要求使用公平锁。
> 3. 绑定多个条件：一个ReentrantLock对象可以同时绑定多个Condition对象，而synchronized中，锁对象的wait()和notify()或notifyAll()可以实现一个隐含的条件，如果要和多于一个的条件关联时，就不得不额外添加一个锁，而ReentrantLock则无须这样，只要多次调用newCondition()即可。

### 1.2.2 非阻塞同步

互斥同步最主要的问题就是进行线程阻塞和唤醒所带来的性能问题，因此这种同步也称为阻塞同步（Blocking Synchronized）。处理问题方式上，互斥同步属于一种悲观的并发策略，总是认为只要不去做正确的同步措施（例如加锁），那肯定会出现问题，无论共享数据是否真的出现竞争，它都要进行加锁（这里讨论的是概念模型，实际上虚拟机会优化很大一部分不必要的加锁）、用户态核心态转换、维护锁计数器和检查是否有阻塞的线程需要等待唤醒等操作。

随着硬件指令集的发展，有了另外一种选择：基于冲突检测的乐观并发策略，就是先进性操作，如果没有其他线程争用共享数据，那操作就成功了；如果共享数据有争，产生了冲突，那就再采取其他的补偿措施（最常见的补偿措施就是不断地重试，直到成功为止），这种乐观的并发策略的许多实现都不需要把线程挂起，因此这种同步称为非阻塞同步（Non-Blocking Synchronization）。

我们需要操作和冲突检测这两个步骤具备原子性，只能靠硬件来完成，硬件保证一个从语义上看起来需要多次操作的行为只通过一条处理器指令就能完成，这类执行常用的有：

> - 测试并设置（Test and Set）。
> - 获取并增加（Fetch and Increment）。
> - 交换（Swap）。
> - 比较并交换（Compare and Swap，以下称CAS）。
> - 加载链接/条件存储（Load Linked/Store Conditional，以下称LL/SC）。

### 1.2.3 无同步方案

要保证线程安全，并不是一定就要进行同步，两者并没有因果关系。同步只是保证共享数据争用时正确性的手段，如果一个方法本来就不涉及共享数据，自然就无须任何同步措施去保证正确定。因此会有一些代码天生就是线程安全的。两类：

> - 可重入代码（Reentrant Code）：这种代码也叫纯代码（Pure Code），可以在代码执行的任何时刻中断它，转而去执行另外一断代码（包括递归调用它本身），而在控制权返回后，原来的程序不会出现任何错误。所有可重入代码都是线程安全的。可重入代码有一些共同特征，例如不依赖存储在堆上的数据和公用的系统资源、用到的状态量都是由参数中传入、不调用非可重入的方法等。判断代码具备可重入的简单原则：如果一个方法，它的返回结果是可以预测的，只要输入了相同的数据，就都能返回相同的结果，就满足可重入性的要求，当然也是线程安全的。

> - 线程本地存储（Thread Local Storage）：如果一段代码中所需要的数据必须与其他代码共享，那就看这些共享数据的代码是否能保证在同一个线程中执行？如能，就把共享数据的可见性范围限制在同一个线程之内，这样，就无须同步也能保证线程之前不出现数据争用问题。

符合这种特点的应用：大部分使用消费队列的架构模式（如“生产者-消费者”模式）都会讲消费过程尽量在一个线程中消费完；经典Web交互模型中的“一个请求对应一个服务线程”（Thread-per-Request）的处理方式，这种处理方式的广泛应用使得很多Web服务端应用都可以使用线程本地存储来解决线程安全问题。

Java中，如果一个变量要被多个线程访问，可以使用volatile关键字声明它为“易变的”；如果一个变量被某个线程独享，可以通过java.lang.ThreadLocal类来实现线程本地存储的功能。每一个线程的Thread对象中都有一个ThreadLcoalMap对象，该对象存储了一组易ThreadLocal.threadLocalHashCode为键，以本地线程变量为值得K-V键值对，ThreadLocal对象就是当前线程的ThreadLocalMap的访问入口，每一个ThreadLocal对象都包含了一个独一无二的threadLocalHashCode值，使用这个值就可以在线程K-V键值对中找回对应的本地线程变量。

# 2. 锁优化

HotSpot虚拟机开发团队在这个版本上花费了大量的精力去实现各种锁优化技术，如适应性自旋、锁消除、锁粗化、轻量级锁、偏向锁。

## 2.1 自旋锁

解决的问题：互斥同步对性能最大的影响是阻塞的实现，挂起线程和恢复线程的操作都需要转入到内核态中完成，这些操作给系统的并发性能带来了很大的压力。

解决方案：为了让线程等待，我们需要让线程执行一个忙循环（自旋），这项技术就是自旋锁

自旋等待不能代替阻塞，且先不说对处理器量的要求，自旋等待本身虽然避免了线程切换的开销，但它要占用处理器的时间，因此，如果锁被占用的时间短，自旋等待效果好，反之，自旋的线程只会白白消耗处理器资源，而不做任何有用工作，带来性能上的浪费。因此，自旋等待的时间必须要有一定的限度，如果自旋超过了限定的次数仍然没有获得锁，就应当使用传统的方式挂起线程。自旋次数的默认值是10次，用户可以使用参数 -XX:PerBlockSpin 来修改。

## 2.2 自适应的自旋锁

解决自旋锁的问题：由于自旋锁每次选择的时间是固定的，对于系统来说是没有必要的。

JDK1.6引入了自适应的自旋锁。自旋的时间是由前一次在同一个锁上的自旋时间及锁的拥有者的状态来决定。如果在同一个锁对象上，自旋等待刚刚成功获得过锁，并且持有锁的线程正在运行中，那么虚拟机就认为这次自旋也很可能成功，进而它将允许自旋等待持续相对更长的时间。如果对于某个锁，自旋很少成功获得，在以后获得这个锁时将可能省略自旋过程，以避免浪费处理器资源。有了自适应自旋，随着程序运行和性能监控信息的不断完善，虚拟机对程序锁的状况预测就会越来越准确，虚拟机就变得越来越“聪明”了。

## 2.3 锁消除

是指虚拟机即时编译器在运行时，对一些代码上要求同步，但是被检测到不可能存在共享数据竞争的锁进行消除。锁消除主要判定依据来源于逃逸分析的数据支持，如果判断在一段代码中，堆上的所有数据都不会逃逸出去，从而被其他线程访问到，那就可以把它们当做栈上数据对待，认为它们是线程私有的，同步加锁自然无须进行。

许多同步措施并不是程序员自己加入的，同步的代码在Java程序中的普遍程度也许超过我们的想象。

```java
public String concatString(String s1, String s2, String s3){
        return s1 + s2 + s3;
    }
123
```

由于String是一个不可变类，对字符串的连接操作总是通过生成新的String对象来进行，因此Javac编译器会对String连接做自动优化。在JDK1.5前，会转换为StringBuffer对象的append()操作，在JDK1.5及以后版本，会转换为StringBuilder对象的连续append()操作。上段代码可能会变成下面的样子。

```java
public String concatString(String s1, String s2, String s3){
        StringBuilder sb = new StringBuilder();
        sb.append(s1);
        sb.append(s2);
        sb.append(s3);
        return sb.toString();
    }
1234567
```

每个StringBuilder.append()中都有一个同步块，锁是sb对象。虚拟机观察变量sb，很快就会发现它的动态作用域被限制在concatString()的内部。也就是sb的所有引用永远不会“逃逸”到concatString()外，其他线程无法访问到它，所以虽然这里有锁，但是可以被安全地消除掉，在即时编译后，这段代码会忽略所有的同步而直接执行。

## 2.4 锁粗化

部分情况下，我们在编写代码时，总是推荐将同步块的作用范围限制得尽量小——只在共享数据的实际作用域才进行同步，如果存在锁竞争，那等待锁的线程也可能尽快拿到锁。

但是如果一系列的连续操作都是对同一对象反复加锁和解锁，甚至加锁操作是出现在循环体中的，那即使没有线程竞争，频繁地进行互斥同步操作也会导致不必要的性能损耗。

上段代码连续的append()就属于这种情况。如果虚拟机探测到有这样一串零碎的操作都对同一个对象加锁，将会把加锁同步的范围扩展（粗化）到整个操作序列的外部，以上段代码为例，就是扩展到第一个append()之前直至最后一个append()之后，这样只需要加锁一次就可以了。

## 2.5 轻量级锁

解决问题：由于传统的重量级锁采用操作系统互斥量来实现同步，每次加锁解锁操作都需要从用户态抓换到内核态，这样会给整个系统带来一定的性能消耗。

轻量级锁原理：
要理解轻量级锁以及偏向锁，则必须从HotSpot虚拟机的对象（对象头）的内存布局开始。HotSpot虚拟机的对象头分为两部分信息，第一部分用于存储对象自身运行时数据，如哈希码（HashCode）、GC分代年龄（GC Age）等，这部分数据的长度在32位和64位的虚拟机中分别为32bit和64bit，官方称为“Mark Word”。另外一部分用于存储指向方法区对象类型数据的指针。在32位的虚拟机中25bit用于存储对象哈希码，4bit存储对象分代年龄，2bit存储锁标志位，其他略。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181221212916930.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

**实现原理:**

> 1. 在代码进入同步块的时候，如果此同步对象没有被锁定（锁标志位为“01”状态），虚拟机首先将在当前线程的栈帧中建立一个名为锁记录（Lock Record）的空间，用于存储锁对象目前的Mark Word的拷贝（官方把这份拷贝加了一个Displaced前缀，即Displaced Mark Word）
> 2. 虚拟机将使用CAS操作尝试将对象的Mark Word更新为指向Lock Record的指针。如果这个更新动作成功了，那么这个线程就拥有了该对象的锁，并且对象Mark Word的锁标志位（Mark Word的最后2bit）将转变为“00”，即表示此对象处于轻量级锁定状态
> 3. 如果这个更新操作失败了，虚拟机首先会检查对象的Mark Word是否指向当前线程的栈帧，如果只说明当前线程已经拥有了这个对象的锁，那就可以直接进入同步块继续执行，否则说明这个锁对象已经被其他线程抢占了。

如果有两条以上的线程争用同一个锁，那轻量级锁就不再有效，要膨胀为重量级锁，锁标志的状态值变为“10”，Mark Word中存储的就是指向重量级锁（互斥量）的指针，后面等待锁的线程也要进入阻塞状态。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181221212909353.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

轻量级锁提升程序性能的依据是“对于绝大多数的锁，在整个同步周期内都是不存在竞争的”，这是一个经验数据。如果没有竞争，轻量级锁使用CAS操作避免了使用互斥量的开销。如果有两条以上的线程争用同一个轻量级锁，那轻量级锁就不再有效，要膨胀为重量级锁，锁标志状态变为"10"。

## 2.6 偏向锁

解决问题：轻量级锁在无竞争情况下使用CAS操作，每次加锁解锁操作都需要使用CAS指令原语。这种操作对于只有一个线程访问对象来说，会造成一定的性能开销。

向锁是JDK1.6引入的锁优化，目的是消除数据在无竞争情况下的同步原语，进一步提高程序的运行性能。如果说轻量级锁是在无竞争的情况下使用CAS操作消除同步使用的互斥量，那偏向锁就是在无竞争的情况下把整个同步都消除掉，连CAS操作都不做了。

**实现原理：**

> 1. 当锁对象第一次被线程获取的时候，虚拟机将会把对象头中的标志位设为“01”，即偏向模式。 同时使用CAS操作把获取到这个锁的线程的ID记录在对象的Mark Word之中，如果CAS操作成功，持有偏向锁的线程以后每次进入这个锁相关的同步块时，虚拟机都可以不再进行任何同步操作
> 2. 当有另外一个线程去尝试获取这个锁时，偏向模式就宣告结束。 根据锁对象目前是否处于被锁定的状态，撤销偏向（Revoke Bias）后恢复到未锁定（标志位为“01”）或轻量级锁定（标志位为“00”）的状态，后续的同步操作就如上面介绍的轻量级锁那样执行

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181221212857424.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxMTI1MTgz,size_16,color_FFFFFF,t_70)

偏向锁可以提高带有同步但无竞争的程序性能。它同样是一个带有效益权衡（Trade Off）性质的优化，它对程序运行不一定有利，如果程序中大多数的锁都总是被多个不同的线程访问，那偏向锁局势多余的。在具体问题具体分析的前提下，有时使用参数 -XX:-UseBiasedLocking 来禁止偏向锁优化反而可以提升性能。





