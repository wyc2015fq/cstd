# Java深入 - Java内存区域详解 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年06月08日 09:14:07[initphp](https://me.csdn.net/initphp)阅读数：1752
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









Java和c c++不一样，c c++都是直接通过手动的方式来申请内存，释放内容。而java拥有良好的内存自动管理机制。所以在我们开发java程序代码的时候，一般情况下不需要关心java的内存问题。

Java的内存区域图：

![](https://img-blog.csdn.net/20140608091331750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上面的图就是Java运行时的内存数据区域。分成方法区，虚拟机栈，本地方法栈，堆，程序计数器。




### 程序计数器

作用：程序计数器是一块很小的内存空间，主要是线程所执行的字节码的行号指示器。每个线程都会有独立的程序计数器。




### 虚拟机栈

Java的虚拟机栈也是线程私有的，生命周期和线程相同。虚拟机栈描述的是Java方法执行的内存模型：每个方法被执行的时候都会同时创建一个栈帧（Stack Frame）用于存储局部变量表、操作数栈、动态链接、方法出口等信息。每个方法被调用直至执行完成的过程，就是对应着一个栈帧在VM Stack中从入栈到出栈的过程。

本地方法栈主要为虚拟机使用的Native方法服务，也是栈的一部分。


虚拟机栈会抛两种异常：

1. StackOverflowError异常。通常主要是线程请求的栈深度大于虚拟机所允许的深度。比如递归层次太深就有可能导致这个异常出现，一般1000-2000层之间。减少递归层次可以减小这个防范。也可以通过参数**-Xss128k** 可以设置每个线程的栈大小。

2. OutOfMemoryError异常。主要是内存不够。




### Java堆

Java内存中最大的块就是堆。所有的对象实例和数组都会分配在java的堆上面。Java堆也是内存GC的重要区域。

Java堆异常：

1. OutOfMemoryError异常。Java堆内存溢出可以通过设置参数**-Xmx3550m -Xms3550m**来控制堆内存的大小。-Xms是最小值，-Xmx是最大值。

一般堆溢出：java.lang.OutOfMemoryError: Java heap space

异常提示中都有Java heap space 说明是堆溢出。

一般解决堆溢出思路有三种：

1. 通过内存影像分析工具（Eclipse Memory Analyzer）来定位堆溢出问题

2. 分析是否有内存泄露

3. 调整堆参数。




### 方法区

方法区也是各线程共享的内存区域，主要存放虚拟机加载的类信息、常量、静态变量等信息。

运行时常量池也是方法区的一部分，作用是存储编译期生成的各种字面量和符号引用。

1. OutOfMemoryError异常。发生异常时，主要抛下面类似的异常信息：



```java
Exception in thread "main" java.lang.OutOfMemoryError: PermGen space  
    at java.lang.String.intern(Native Method)  
    at org.fenixsoft.oom.RuntimeConstantPoolOOM.main(RuntimeConstantPoolOOM.java:18)
```


可以看到有PermGen space的提示则表示方法区溢出。



方法区溢出，可以通过限制方法区的内存大小来限制：-XX:PermSize 和 -XX:MaxPermSize













