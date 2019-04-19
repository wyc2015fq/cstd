# JAVA虚拟机JVM工作原理 - 左直拳的马桶_日用桶 - CSDN博客
2018年12月20日 19:39:40[左直拳](https://me.csdn.net/leftfist)阅读数：4057
提起JAVA，都知道它有个虚拟机，因此可以跨平台，一次编写，到处运行。但具体原理是咋样的？身为JAVA小白，岂可不知！
```
一、原理
二、结构
1、类加载器
2、执行引擎
3、运行时数据区域
```
**一、原理**
JVM实现了JAVA语言最重要的特征：平台无关性。其原理：java程序并不直接在操作系统上执行，而是由JVM执行。JVM屏蔽了与具体平台相关的信息，使得JAVA程序只需编译成适应JVM，可以在JVM上运行的目标代码（.class）就行，JVM再负责解释成具体平台的机器指令执行。同时，JVM还会进行安全检查，是java程序的安全检验引擎。
**二、结构**
JVM由三大部分组成：
```
1、类加载器
2、执行引擎
3、运行时数据区域
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018122018211859.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**1、类加载器**
具体作用就是将`.class`文件加载到jvm虚拟机中去，程序就可以正确运行了。但是，jvm启动的时候，并不会一次性加载所有的`.class`文件，而是根据需要去动态加载。
类的加载过程采用双亲委派模型：
```
1）当前类加载器首先检查自己的加载清单，如果存在该类，则返回之；
2）如果没找到，委托父类加载器去加载；父类加载器重复动作1，递归至祖宗bootstrap ClassLoder（我靠，bootstrap，还以为是前端框架呢。bootstrap，引导之意）；
3）都没有，仍然由当前类加载器加载，并纳入自己缓存中。
```
采用双亲委派模型，目的在于更好的保证Java平台的安全。所谓的安全，是指java的核心类不被用户自己编写的类所替换。由于核心类在虚拟机初始化时就已加载，而每次加载类前先检查是否存在，所以可以避免核心类被替换。
各种类加载器：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181220185427170.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**Bootstrap class loader**： 父类
当运行 java 虚拟机时，这个类加载器被创建，它负责加载虚拟机的核心类库，如 java.lang.* 等。例如 java.lang.Object 就是由根类加载器加载的。需要注意的是，这个类加载器不是用 java 语言写的，而是用 C/C++ 写的。
**Extension class loader**:
这个加载器加载出了基本 API 之外的一些拓展类。
**AppClass Loader（SystemAppClass）**:
加载应用程序和程序员自定义的类。
**2、执行引擎**
作用： 执行java编译好的字节码，或者执行本地方法
**3、运行时数据区域**
JVM 运行时数据区域 (JVM Runtime Area) 其实就是指 JVM 在运行期间，其对JVM内存空间的划分和分配。JVM在运行时将数据划分为6个区域来存储：
```
Program Counter (PC) Register：程序计数器
Java Virtual Machine Stacks：Java虚拟机栈
Heap Memory：堆内存
Method Area：方法区
Run-time Constant Pool：运行时常量池
Native Method Stacks：本地方法栈
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181221114535617.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
我们写的所有程序都被加载到这里，按不同类别存放在
```
堆内存
Java虚拟机栈
本地方法栈
程序计数器
方法区
```
1）从**线程的角度**理解Java Runtime Area
a.线程私有：
`程序计数器 Java虚拟机栈 本地方法栈`，生命周期与线程一致；
这3个内存区域是不需要进行垃圾回收的。因为他们的生命周期是和线程同步的，随着线程的销毁，他们占用的内存会自动释放。
b.线程共享：
`堆内存，方法区，运行时常量池`，生命周期与JVM一致。
`方法区`和`堆内存`需要进行垃圾回收，回收的对象就是那些不存在任何引用的对象。
2）从**存储内容**理解Java Runtime Area
`方法区`和`常量池`存储类的信息
`堆内存`存储对象信息
`程序计数器`，`Java虚拟机栈`，`本地方法栈`存储线程的信息
**3）堆内存、方法区和运行时常量**
方法区的作用是存储 Java 类的结构信息，当我们创建对象实例后，对象的类型信息存储在方法区之中，实例数据存放在堆内存中；
运行时常量池是方法区的一部分，所以也是全局共享的。其作用是存储 Java 类文件常量池中的符号信息。
*class 文件中存在常量池(非运行时常量池)，其在编译阶段就已经确定；JVM 规范对 class 文件结构有着严格的规范，必须符合此规范的 class 文件才会被 JVM 认可和装载。运行时常量池 中保存着一些 class 文件中描述的符号引用，同时还会将这些符号引用所翻译出来的直接引用存储在 运行时常量池 中。运行时常量池相对于 class 常量池一大特征就是其具有动态性，Java 规范并不要求常量只能在运行时才产生，也就是说运行时常量池中的内容并不全部来自 class 常量池，class 常量池并非运行时常量池的唯一数据输入口；在运行时可以通过代码生成常量并将其放入运行时常量池中。同方法区一样，当运行时常量池无法申请到新的内存时，将抛出 OutOfMemoryError 异常。*
**4）直接内存（Direct Memory）**
直接内存并不是JVM管理的内存，而是JVM以外的机器内存。比如，你有4G的内存，JVM占用了1G，则其余的3G就是直接内存。
JDK中有一种基于通道（Channel）和缓冲区（Buffer）的内存分配方式，将由C语言实现的native函数库分配在直接内存中，用存储在JVM堆中的DirectByteBuffer来引用。由于直接内存收到本机器内存的限制，所以也可能出现OutOfMemoryError的异常。
