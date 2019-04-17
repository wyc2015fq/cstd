# 深入分析java web技术内幕----读书笔记（三） - 程序员江湖 - CSDN博客





2017年03月16日 12:57:09[黄小斜](https://me.csdn.net/a724888)阅读数：710标签：[深入java web技术内幕																[java web																[classloader																[jvm																[读书笔记](https://so.csdn.net/so/search/s.do?q=读书笔记&t=blog)
个人分类：[读书笔记](https://blog.csdn.net/a724888/article/category/6784971)





![](https://img-blog.csdn.net/20170316130111806?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)﻿﻿

claasloader读书笔记

claasloader

1、classloader将class加载到jvm中。

2、父优先的等级加载，双亲委派模型。



3、defineclass用于将字节流解析成Class对象，主要用于读取class文件，以及网络文件中的类等。defineclass不会解析类，只会在此对象实例化时才会resolve。

defineclass与findclass常一起使用，findclass是classloader中的方法，覆盖后可以自定义加载规则。

4this.getclass.getclassloader.loadclass可以在运行时加载指定类到程序。（只能在本程序的类加载器中加载）.加载后还没解析，实例化时才resolve。

5、class。forname（）可以加载类并解析。

例子：；

Class.forName("org.java.lang",true,getClass().getClassLoader());参数为 类名、是否初始化、所用类加载器

getsystemclass也可以在自定义类加载器中加载类



6

Classloader是抽象类![](https://img-blog.csdn.net/20170316130004478?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



- 
Bootstrap ClassLoader/启动类加载器

主要负责jdk_home/lib目录下的核心api或-Xbootclasspath选项指定的jar包装入工作。

- 
Extension ClassLoader/扩展类加载器

主要负责jdk_home/lib/ext目录下的jar包或-Djava.ext.dirs指定目录下的jar包装入工作。

- 
System ClassLoader/系统类加载器

主要负责java -classpath/-Djava.class.path所指的目录下的类与jar包装入工作。

- 
User Custom ClassLoader/用户自定义类加载器(java.lang.ClassLoader的子类) 

在程序运行期间,通过java.lang.ClassLoader的子类动态加载class文件,体现java动态实时类装入特性。

![](https://img-blog.csdn.net/20170316130111806?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

自定义的类加载器都继承自URLclassloader或其他子类。

但这些类的父加载器都是appclassloader。

因为它们都调用getsystemclassloader作为父加载器，getsystemclassloader的返回值都是appclassloader。



7、加载一个类的过程

![](https://img-blog.csdn.net/20170316130010899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)









![](https://img-blog.csdn.net/20170316130020781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170316130025884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTcyNDg4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

8、jvm表示一个类是否是同一个类有两个条件：

1、完整类名是否一样

2、加载该类的classloader是否是同一个类加载器的同一个实例

（不仅要类相同，实例也要是同一个）。



9、回收问题：classloader加载的类的字节码（就是。class）会被保存在jvm的永久代，它的对象无引用时也会被jvm回收，但如果大量动态加载类时，会造成永久带溢出。（因为永久代只在full
 gc时清理，若full gc不频繁，会造成永久代内存溢出）



system。gc会触发垃圾回收，但不知道回收情况如何。

一般情况下system。gc会触发full gc，会stop
 the world，full gc一般回收老年代和永久代。](https://so.csdn.net/so/search/s.do?q=jvm&t=blog)](https://so.csdn.net/so/search/s.do?q=classloader&t=blog)](https://so.csdn.net/so/search/s.do?q=java web&t=blog)](https://so.csdn.net/so/search/s.do?q=深入java web技术内幕&t=blog)




