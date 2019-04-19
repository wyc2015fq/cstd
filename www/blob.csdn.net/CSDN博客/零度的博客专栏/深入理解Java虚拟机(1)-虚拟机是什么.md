# 深入理解Java虚拟机(1)-虚拟机是什么 - 零度的博客专栏 - CSDN博客
2017年08月10日 10:49:15[零度anngle](https://me.csdn.net/zmx729618)阅读数：265标签：[java																[虚拟机](https://so.csdn.net/so/search/s.do?q=虚拟机&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[JVM调优](https://blog.csdn.net/zmx729618/article/category/6129208)
# 什么是Java虚拟机
# 作为一个Java程序员，我们每天都在写Java代码，我们写的代码都是在一个叫做Java虚拟机的东西上执行的。但是如果要问什么是虚拟机，恐怕很多人就会模棱两可了。在本文中，我会写下我对虚拟机的理解。因为能力所限，可能有些地方描述的不够欠当。如果你有不同的理解，欢迎交流。
我们都知道java程序必须在虚拟机上运行。那么虚拟机到底是什么呢？先看网上搜索到的比较靠谱的解释：
虚拟机是一种抽象化的[计算机](http://baike.baidu.com/view/3314.htm)，通过在实际的计算机上仿真模拟各种计算机功能来实现的。Java虚拟机有自己完善的[硬体](http://baike.baidu.com/view/2871257.htm)架构，如[处理器](http://baike.baidu.com/view/50152.htm)、[堆栈](http://baike.baidu.com/view/93201.htm)、[寄存器](http://baike.baidu.com/view/6159.htm)等，还具有相应的[指令](http://baike.baidu.com/view/178461.htm)系统。JVM屏蔽了与具体[操作系统](http://baike.baidu.com/view/880.htm)平台相关的信息，使得Java[程序](http://baike.baidu.com/view/17674.htm)只需生成在Java虚拟机上运行的目标代码（[字节码](http://baike.baidu.com/view/560330.htm)），就可以在多种平台上不加修改地运行。
这种解释应该算是正确的，但是只描述了虚拟机的外部行为和功能，并没有针对内部原理做出说明。一般情况下我们不需要知道虚拟机的运行原理，只要专注写java代码就可以了，这也正是虚拟机之所以存在的原因--屏蔽底层操作系统平台的不同并且减少基于原生语言开发的复杂性，使java这门语言能够跨各种平台（只要虚拟机厂商在特定平台上实现了虚拟机），并且简单易用。这些都是虚拟机的外部特性，但是从这些信息来解释虚拟机，未免太笼统了，无法让我们知道内部原理。
# 从进程的角度解释JVM
让我们尝试从操作系统的层面来理解虚拟机。我们知道，虚拟机是运行在操作系统之中的，那么什么东西才能在操作系统中运行呢？当然是进程，因为进程是操作系统中的执行单位。可以这样理解，当它在运行的时候，它就是一个操作系统中的进程实例，当它没有在运行时（作为可执行文件存放于文件系统中），可以把它叫做程序。
对命令行比较熟悉的同学，都知道其实一个命令对应一个可执行的二进制文件，当敲下这个命令并且回车后，就会创建一个进程，加载对应的可执行文件到进程的地址空间中，并且执行其中的指令。下面对比C语言和Java语言的HelloWorld程序来说明问题。
首先编写C语言版的HelloWorld程序。
**[cpp]**[view plain](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[copy](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[print](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[?](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)
- #include <stdio.h>
- #include <stdlib.h>
- 
- int main(void) {  
-     printf("hello world\n");  
- return 0;  
- }  
```cpp
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	printf("hello world\n");
	return 0;
}
```
编译C语言版的HelloWorld程序：
**[plain]**[view plain](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[copy](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[print](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[?](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)
- gcc HelloWorld.c -o HelloWorld  
`gcc HelloWorld.c -o HelloWorld`
运行C语言版的HelloWorld程序：
**[plain]**[view plain](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[copy](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[print](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[?](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)
- zhangjg@linux:/deve/workspace/HelloWorld/src$ ./HelloWorld   
- hello world  
```
zhangjg@linux:/deve/workspace/HelloWorld/src$ ./HelloWorld 
hello world
```
gcc编译器编译后的文件直接就是可被操作系统识别的二进制可执行文件，当我们在命令行中敲下 ./HelloWorld这条命令的时候， 直接创建一个进程， 并且将可执行文件加载到进程的地址空间中， 执行文件中的指令。
作为对比， 我们看一下Java版HelloWord程序的编译和执行形式。
首先编写源文件HelloWord.java ：
**[java]**[view plain](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[copy](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[print](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[?](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)
- publicclass HelloWorld {  
- 
- publicstaticvoid main(String[] args) {  
-         System.out.println("HelloWorld");  
-     }  
- }  
```java
public class HelloWorld {
	public static void main(String[] args) {
		System.out.println("HelloWorld");
	}
}
```
编译Java版的HelloWorld程序：
**[java]**[view plain](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[copy](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[print](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[?](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)
- zhangjg@linux:/deve/workspace/HelloJava/src$ javac HelloWorld.java   
- zhangjg@linux:/deve/workspace/HelloJava/src$ ls  
- HelloWorld.class  HelloWorld.java  
```java
zhangjg@linux:/deve/workspace/HelloJava/src$ javac HelloWorld.java 
zhangjg@linux:/deve/workspace/HelloJava/src$ ls
HelloWorld.class  HelloWorld.java
```
运行Java版的HelloWorld程序：
**[plain]**[view plain](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[copy](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[print](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)[?](http://blog.csdn.net/zhangjg_blog/article/details/20380971#)
- zhangjg@linux:/deve/workspace/HelloJava/src$ java -classpath . HelloWorld   
- HelloWorld  
```
zhangjg@linux:/deve/workspace/HelloJava/src$ java -classpath . HelloWorld 
HelloWorld
```
从上面的过程可以看到， 我们在运行Java版的HelloWorld程序的时候， 敲入的命令并不是 ./HelloWorld.class 。 因为class文件并不是可以直接被操作系统识别的二进制可执行文件 。 我们敲入的是java这个命令。 这个命令说明， 我们首先启动的是一个叫做java的程序， 这个java程序在运行起来之后就是一个JVM进程实例。 
上面的命令执行流程是这样的：
java命令首先启动虚拟机进程，虚拟机进程成功启动后，读取参数“HelloWorld”，把他作为初始类加载到内存，对这个类进行初始化和动态链接（关于类的初始化和动态链接会在后面的博客中介绍），然后从这个类的main方法开始执行。也就是说我们的.class文件不是直接被系统加载后直接在cpu上执行的，而是被一个叫做虚拟机的进程托管的。首先必须虚拟机进程启动就绪，然后由虚拟机中的类加载器加载必要的class文件，包括jdk中的基础类（如String和Object等），然后由虚拟机进程解释class字节码指令，把这些字节码指令翻译成本机cpu能够识别的指令，才能在cpu上运行。
从这个层面上来看，在执行一个所谓的java程序的时候，真真正正在执行的是一个叫做Java虚拟机的进程，而不是我们写的一个个的class文件。这个叫做虚拟机的进程处理一些底层的操作，比如内存的分配和释放等等。我们编写的class文件只是虚拟机进程执行时需要的“原料”。这些“原料”在运行时被加载到虚拟机中，被虚拟机解释执行，以控制虚拟机实现我们java代码中所定义的一些相对高层的操作，比如创建一个文件等，可以将class文件中的信息看做对虚拟机的控制信息，也就是一种虚拟指令。
编程语言也有自己的原理， 学习一门语言， 主要是把它的原理搞明白。 看似一个简单的HelloWorld程序， 也有很多深入的内容值得剖析。
# JVM体系结构简介
为了展示虚拟机进程和class文件的关系，特意画了下面一张图：
![](https://img-blog.csdn.net/20140317234757765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqZ19ibG9n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
根据上图表达的内容，我们编译之后的class文件是作为Java虚拟机的原料被输入到Java虚拟机的内部的，那么具体由谁来做这一部分工作呢？其实在Java虚拟机内部，有一个叫做类加载器的子系统，这个子系统用来在运行时根据需要加载类。注意上面一句话中的“根据需要”四个字。在Java虚拟机执行过程中，只有他需要一个类的时候，才会调用类加载器来加载这个类，并不会在开始运行时加载所有的类。就像一个人，只有饿的时候才去吃饭，而不是一次把一年的饭都吃到肚子里。一般来说，虚拟机加载类的时机，在第一次使用一个新的类的时候。本专栏后面的文章会具体讨论Java中的类加载器。
由虚拟机加载的类，被加载到Java虚拟机内存中之后，虚拟机会读取并执行它里面存在的字节码指令。虚拟机中执行字节码指令的部分叫做执行引擎。就像一个人，不是把饭吃下去就完事了，还要进行消化，执行引擎就相当于人的肠胃系统。在执行的过程中还会把各个class文件动态的连接起来。关于执行引擎的具体行为和动态链接相关的内容也会在本专栏后续的文章中进行讨论。
我们知道，Java虚拟机会进行自动内存管理。具体说来就是自动释放没有用的对象，而不需要程序员编写代码来释放分配的内存。这部分工作由垃圾收集子系统负责。
从上面的论述可以知道， 一个Java虚拟机实例在运行过程中有三个子系统来保障它的正常运行，分别是类加载器子系统， 执行引擎子系统和垃圾收集子系统。 如下图所示：
![](https://img-blog.csdn.net/20140317235203656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqZ19ibG9n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
虚拟机的运行，必须加载class文件，并且执行class文件中的字节码指令。它做这么多事情，必须需要自己的空间。就像人吃下去的东西首先要放在胃中。虚拟机也需要空间来存放个中数据。首先，加载的字节码，需要一个单独的内存空间来存放；一个线程的执行，也需要内存空间来维护方法的调用关系，存放方法中的数据和中间计算结果；在执行的过程中，无法避免的要创建对象，创建的对象需要一个专门的内存空间来存放。关于虚拟机运行时数据区的内容，也会出现在本专栏后续的文章中。虚拟机的运行时内存区大概可以分成下图所示的几个部分。(这里只是大概划分，
 并没有划分的很精细)
![](https://img-blog.csdn.net/20140317235550250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdqZ19ibG9n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 总结
写到这里，基本上关于我对java虚拟机的理解就写完了。这篇文章的主题虽然是深入理解Java虚拟机，但是你可能感觉一点也不“深入”，也只是泛泛而谈。我也有这样的感觉。限于自己水平有限，也只能这样了，要是想深入理解java虚拟机，强烈建议读一下三本书：
《深入Java虚拟机》
《深入理解Java虚拟机JVM高级特性与最佳实践》
《Java虚拟机规范》
其实我也读过这几本书，但是它们对虚拟机的解释也是基于一个外部模型，而没有深入剖析虚拟机内部的实现原理。虚拟机是一个大而复杂的东西，实现虚拟机的人都是大牛级别的，如果不是参与过虚拟机的实现，应该很少有人能把它参透。本专栏后面的一些文章也参考了这三本书， 虽然讲解Java语法的书不计其数， 但是深入讲解虚拟机的书， 目前为止我就见过这三本，并且网上的资料也不是很多。
最后做一个总结：
1 虚拟机并不神秘，在操作系统的角度看来，它只是一个普通进程。
2 这个叫做虚拟机的进程比较特殊，它能够加载我们编写的class文件。如果把JVM比作一个人，那么class文件就是我们吃的食物。
3 加载class文件的是一个叫做类加载器的子系统。就好比我们的嘴巴，把食物吃到肚子里。
4 虚拟机中的执行引擎用来执行class文件中的字节码指令。就好比我们的肠胃，对吃进去的食物进行消化。
5 虚拟机在执行过程中，要分配内存创建对象。当这些对象过时无用了，必须要自动清理这些无用的对象。清理对象回收内存的任务由垃圾收集器负责。就好比人吃进去的食物，在消化之后，必须把废物排出体外，腾出空间可以在下次饿的时候吃饭并消化食物。
