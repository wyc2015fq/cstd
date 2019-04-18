# JVM加载class文件的原理机制 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月06日 16:20:45[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1075
个人分类：[Java](https://blog.csdn.net/fjssharpsword/article/category/6480440)









先引入JVM图：

![](https://img-blog.csdn.net/20170706161042381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


1）Class Loader 类加载器：类加载器的作用是加载类文件到内存。

2）Execution Engine 执行引擎：执行引擎也叫做解释器(Interpreter) ，负责解释命令，提交操作系统执行。

3）Native Interface 本地接口：本地接口的作用是融合不同的编程语言为Java 所用，如融合C/C++ 程序。

在内存中专门开辟了一块区域处理标记为native 的代码，它的具体做法是Native Method Stack 中登记native 方法，在Execution Engine 执行时加载native libraies 。主要是与硬件有关的应用，比如通过Java 程序驱动打印机、Java 系统管理生产设备。

4）Runtime data area 运行数据区：运行数据区是整个JVM 的重点。




主要步骤就是：

1）装载：查找和导入class文件；

2）连接：检查载入的class文件数据的正确性，为类的静态变量分配存储空间，将符号引用转换成直接引用(这一步是可选的)。

3）初始化:初始化静态变量，静态代码块。





JVM加载class文件的几点原理：



1.Java中的所有类，装载到jvm中运行，这个装载工作是由jvm中的类装载器完成的,类装载器所做的工作实质是把类文件从硬盘读取到内存中。



2.java中的类大致分为三种： 

    1)系统类 

    2)扩展类 

    3)由程序员自定义的类 



3.类装载方式，有两种 

    1.隐式装载， 程序在运行过程中当碰到通过new 等方式生成对象时，隐式调用类装载器加载对应的类到jvm中； 

    2.显式装载， 通过class.forname()等方法，显式加载需要的类。 



4.类加载的动态性体现 

   一个应用程序总是由n多个类组成，Java程序启动时，并不是一次把所有的类全部加载后再运行；

  它总是先把保证程序运行的基础类一次性加载到jvm中，其它类等到jvm用到的时候再加载，节省了内存的开销。



5．java类装载器 

    Java中的类装载器实质上也是类，功能是把类载入jvm中，值得注意的是jvm的类装载器并不是一个，而是三个，层次结构如下： 

      Bootstrap Loader  - 负责加载系统类 

            | 

          - - ExtClassLoader  - 负责加载扩展类 

                          | 

                      - - AppClassLoader  - 负责加载应用类 

    三个类加载器，一方面是分工，各自负责各自的区块，另一方面为了实现委托模型。 

    委托模型机制：类装载器有载入类的需求时，会先请示其Parent使用其搜索路径帮忙载入，如果Parent 找不到，那么才由自己依照自己的搜索路径搜索类。




