# 【java】JVM 类加载机制详解 - zkq_1986的博客 - CSDN博客





2018年02月06日 15:21:31[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：65
个人分类：[java](https://blog.csdn.net/zkq_1986/article/category/6317924)









这几种类加载器的层次关系如下图所示：

![wpsC239.tmp](https://images2015.cnblogs.com/blog/331425/201606/331425-20160621125944459-1013316302.jpg)

注意：这里父类加载器并不是通过继承关系来实现的，而是采用组合实现的。

站在Java虚拟机的角度来讲，只存在两种不同的类加载器：启动类加载器：它使用C++实现（这里仅限于Hotspot，也就是JDK1.5之后默认的虚拟机，有很多其他的虚拟机是用Java语言实现的），是虚拟机自身的一部分；所有其他的类加载器：这些类加载器都由Java语言实现，独立于虚拟机之外，并且全部继承自抽象类java.lang.ClassLoader，这些类加载器需要由启动类加载器加载到内存中之后才能去加载其他的类。

站在Java开发人员的角度来看，类加载器可以大致划分为以下三类：

启动类加载器：Bootstrap ClassLoader，负责加载存放在JDK\jre\lib(JDK代表JDK的安装目录，下同)下，或被-Xbootclasspath参数指定的路径中的，并且能被虚拟机识别的类库（如rt.jar，所有的java.*开头的类均被Bootstrap ClassLoader加载）。启动类加载器是无法被Java程序直接引用的。

扩展类加载器：Extension ClassLoader，该加载器由sun.misc.Launcher$ExtClassLoader实现，它负责加载jdk\jre\lib\ext目录中，或者由java.ext.dirs系统变量指定的路径中的所有类库（如javax.*开头的类），开发者可以直接使用扩展类加载器。

应用程序类加载器：Application ClassLoader，该类加载器由sun.misc.Launcher$AppClassLoader来实现，它负责加载用户类路径（ClassPath）所指定的类，开发者可以直接使用该类加载器，如果应用程序中没有自定义过自己的类加载器，一般情况下这个就是程序中默认的类加载器。

应用程序都是由这三种类加载器互相配合进行加载的，如果有必要，我们还可以加入自定义的类加载器。因为JVM自带的ClassLoader只是懂得从本地文件系统加载标准的java class文件，因此如果编写了自己的ClassLoader，便可以做到如下几点：

1）在执行非置信代码之前，自动验证数字签名。

2）动态地创建符合用户特定需要的定制化构建类。

3）从特定的场所取得java class，例如数据库中和网络中。



JVM类加载机制

•全盘负责，当一个类加载器负责加载某个Class时，该Class所依赖的和引用的其他Class也将由该类加载器负责载入，除非显示使用另外一个类加载器来载入

•父类委托，先让父类加载器试图加载该类，只有在父类加载器无法加载该类时才尝试从自己的类路径中加载该类

•缓存机制，缓存机制将会保证所有加载过的Class都会被缓存，当程序中需要使用某个Class时，类加载器先从缓存区寻找该Class，只有缓存区不存在，系统才会读取该类对应的二进制数据，并将其转换成Class对象，存入缓存区。这就是为什么修改了Class后，必须重启JVM，程序的修改才会生效



4、类的加载

类加载有三种方式：

1、命令行启动应用时候由JVM初始化加载

2、通过Class.forName()方法动态加载

3、通过ClassLoader.loadClass()方法动态加载

例子：


![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.neo.classloader;publicclass loaderTest {         publicstaticvoid main(String[] args) throws ClassNotFoundException {                 ClassLoader loader = HelloWorld.class.getClassLoader();                 System.out.println(loader);                 //使用ClassLoader.loadClass()来加载类，不会执行初始化块                 loader.loadClass("Test2");                 //使用Class.forName()来加载类，默认会执行初始化块 //                Class.forName("Test2");                 //使用Class.forName()来加载类，并指定ClassLoader，初始化时不执行静态块 //                Class.forName("Test2", false, loader);         } }
![复制代码](https://common.cnblogs.com/images/copycode.gif)


demo类


![复制代码](https://common.cnblogs.com/images/copycode.gif)
publicclass Test2 {         static {                 System.out.println("静态初始化块执行了！");         } }
![复制代码](https://common.cnblogs.com/images/copycode.gif)


分别切换加载方式，会有不同的输出结果。



Class.forName()和ClassLoader.loadClass()区别

Class.forName()：将类的.class文件加载到jvm中之外，还会对类进行解释，执行类中的static块；

ClassLoader.loadClass()：只干一件事情，就是将.class文件加载到jvm中，不会执行static中的内容,只有在newInstance才会去执行static块。

注：

Class.forName(name, initialize, loader)带参函数也可控制是否加载static块。并且只有调用了newInstance()方法采用调用构造函数，创建类的对象 。




转载自：https://www.cnblogs.com/ityouknow/p/5603287.html



