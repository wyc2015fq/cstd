# JAVA和JVM运行原理揭秘 - z69183787的专栏 - CSDN博客
2015年01月05日 14:26:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1293

JVM是java的核心和基础，在java编译器和os平台之间的虚拟处理器。它是一种利用软件方法实现的抽象的计算机基于下层的操作系统和硬件平台，可以在上面执行java的字节码程序。
AD：
这里和大家简单分享一下JAVA和[JVM](http://developer.51cto.com/art/201001/176550.htm)运行的原理，Java语言写的源程序通过Java编译器，编译成与平台无关的‘字节码程序’(.class文件，也就是0，1二进制程序)，然后在OS之上的Java解释器中解释执行，而JVM是java的核心和基础，在java编译器和os平台之间的虚拟处理器
**JAVA和JVM运行的原理**
**1.Java语言运行的过程**
Java语言写的源程序通过Java编译器，编译成与平台无关的‘字节码程序’(.class文件，也就是0，1二进制程序)，然后在OS之上的Java解释器中解释执行。
![Java语言运行的过程](http://images.51cto.com/files/uploadimg/20100917/1529230.gif)
也相当与
![Java语言运行的过程](http://images.51cto.com/files/uploadimg/20100917/1529231.gif)
注：JVM（java虚拟机）包括解释器，不同的JDK虚拟机是相同的，解释器不同。
**2.JVM：**
JVM是java的核心和基础，在java编译器和os平台之间的虚拟处理器。它是一种利用软件方法实现的抽象的计算机基于下层的操作系统和硬件平台，可以在上面执行java的字节码程序。
java编译器只要面向JVM，生成JVM能理解的代码或字节码文件。Java源文件经编译成字节码程序，通过JVM将每一条指令翻译成不同平台机器码，通过特定平台运行。
**JVM执行程序的过程 ：**
I.加载。class文件
II.管理并分配内存
III.执行垃圾收集
JRE（java运行时环境）由JVM构造的java程序的运行环境 
![JVM执行程序的过程](http://images.51cto.com/files/uploadimg/20100917/1529232.gif)
