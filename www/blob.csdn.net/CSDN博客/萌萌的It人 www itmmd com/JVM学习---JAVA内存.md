
# JVM学习---JAVA内存 - 萌萌的It人 www.itmmd.com - CSDN博客


2013年12月04日 13:26:12[Jlins](https://me.csdn.net/dyllove98)阅读数：1411


一、JAVA运行时数据区域：JAVA中的运行时内存区域有的随着虚拟机进程的启动而存在，有的区域则是依赖用户线程的启动和结束而建立和销毁的。包括以下的几个区域。
![](http://images.cnblogs.com/cnblogs_com/Cratical/201208/201208212311249241.png)
图. JAVA虚拟机运行时数据区
1.程序计数器：线程私有的内存。作用是当前线程所执行的字节码的行号指示器。多线程是通过线程轮流切换并分配处理器执行时间的方式实现，通过程序计数器为线程切换后能恢复到正确的执行位置。每条线程都需要有一个独立的程序计数器，各线程之间的计数器互不影响，独立存储。
2.JAVA虚拟机栈：线程私有的内存。作用是描述JAVA方法执行的内存模型，每个方法被执行的时候都会创建一个栈帧（Stack Frame）用于存储局部变量表、操作数栈、动态链接、方法出口等信息。每个方法的调用直到执行完成的过程，就对应着一个栈帧在虚拟机栈中从入栈到出栈的过程。
局部变量表：存放了编译器可知的各种基本数据类型（int、boolean、long、float等）、对象引用（reference类型，可能是一个指针或是句柄）和returnAddress类型。
可能抛出StackOverFlowError异常、OutOfMemoryError异常。
3.本地方法栈（Native Method Stacks）：为虚拟机使用到的Native方法服务。
可能抛出StackOverFlowError异常、OutOfMemoryError异常。
4.JAVA堆：存放对象实例。Java堆是垃圾收集器管理的主要区域。
可能抛出OutOfMemoryError异常。
5.方法区：用于存储已被虚拟机加载的类信息、常量、静态变量、即时编译器编译后的代码等数据。
可能抛出OutOfMemoryError异常。
5.1运行时常量池：是方法区的一部分。用于存放编译期生成的各种字面量和符号引用。
可能抛出OutOfMemoryError异常。
二、对象访问
|1
|Object
 obj =|new|Object();
|

对于上述代码，至少会涉及到Java栈、Java堆、方法区三个内存区域。
Object obj存储在Java栈的变量表，作为一个reference类型数据。new Object（）存储在Java堆中，形成一块实例数据。在Java堆中还必须包含能查找到此对象类型数据的地址信息，这些类型数据存储到方法区中。
Java堆中对象访问的方式有两种：使用句柄和直接指针。
（1）使用句柄访问方式：优点是reference中存放了稳定的句柄地址，在对象被移动是只会改变句柄中的实例数据指针，而reference本身不需要被修改
![](https://img-my.csdn.net/uploads/201209/26/1348659242_7055.jpg)
（2）直接指针访问：优点是速度更快，能够节省一次指针定位的时间开销。HotSpot虚拟机采用这种方式进行对象访问。
![](https://img-my.csdn.net/uploads/201209/26/1348658605_5211.jpg)
参考资料：深入理解JAVA虚拟机
[http://chenzhou123520.iteye.com/blog/1585224](http://chenzhou123520.iteye.com/blog/1585224)
[http://blog.csdn.net/java2000_wl/article/details/8015105](http://blog.csdn.net/java2000_wl/article/details/8015105)

