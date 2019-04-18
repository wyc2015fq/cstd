# 使用Eclipse Memory Analyzer进行内存泄漏分析三部曲 - z69183787的专栏 - CSDN博客
2015年09月22日 14:16:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1621
一、准备工作 
分析较大的dump文件（根据我自己的经验2G以上的dump文件就需要使用以下介绍的方法，不然mat会出现oom）需要调整虚拟机参数 
找个64位的系统在MemoryAnalyzer.ini设置-Xmx2g 
如果是32位的xp可以使用下面的方法进行尝试：
- 安装jrockit 6.0的JDK
- mat使用jrockit的jdk来启动
	
Java代码  ![收藏代码](http://seanhe.iteye.com/images/icon_star.png)
- -vm  
- D:/Program Files/Java/jrockit-R28.0.0-jre1.6.0_17/bin/jrockit/jvm.dll  
- -vmargs  
- -Xmx1700m  
	二、开始使用MAT进行OOM分析 
	第一步，启动mat ,选择File->Open Heap Dump 选择你的dump文件。下面开始等待,mat解析dump文件需要花一些时间，在解析的同时会在硬盘上写入一些解析结果文件，这样下次打开时速度会快很多。有时候mat在解析过程中可能会出现出错的情况，这个时候可以将那些临时文件删除以后重试第一步，如果你的rp够好的话重试也许会解析成功。 
	第二步，查看内存泄漏分析报表。mat解析完成以后会出现如下图的提示： 
![](http://dl.iteye.com/upload/picture/pic/80860/8c91333f-82a6-32af-9481-def0a9db0d1c.png)
	因为我们就是为了查找内存泄漏的问题，所以保持默认选项直接点“Finish”就可以。 
	Mat会非常直观的展现内存泄漏的可疑点，类似下面的报表可以直接看到某个线程占用了大量的内存 
![](http://dl.iteye.com/upload/picture/pic/80858/8477bdc9-a9ba-3ccb-8834-70a0dd7f7d13.jpg)
	问题的详细分析信息： 
![](http://dl.iteye.com/upload/picture/pic/80862/4eb7daf4-f949-3df3-afa3-84d140d03013.jpg)
	第三步，开始寻找导致内存泄漏的代码点。这时往往需要打开对象依赖关系树形视图，点击如图按钮即可。 
![](http://dl.iteye.com/upload/picture/pic/80864/7cee81f9-b624-30c4-882a-612ca621d6ea.jpg)
	这时会看到如下视图 
![](http://dl.iteye.com/upload/picture/pic/80870/c687fef2-be0a-3272-a755-08c71d24c06b.jpg)
	这个视图的左边大区域可以看到对象的依赖关系，选中某个对象以后可以在左边小窗口查看对象的一些属性。如果属性的值是一些内存地址你还可以点击工具栏的搜索按钮来搜索具体的对象信息。在进行具体分析的时候MAT只是起了帮助你进行分析的工具的功能，OOM问题分析没有固定方法和准则。只能发挥你敏锐的洞察力，结合源代码，对内存中的对象进行分析从而找到代码中的BUG. 
	使用贴士： 
	关于shallow size、retained size(摘自http://www.360doc.com/content/11/0830/16/4520139_144514377.shtml) 
	Shallow size就是对象本身占用内存的大小，不包含对其他对象的引用，也就是对象头加成员变量（不是成员变量的值）的总和。在32位系统上，对象头占用8字节，int占用4字节，不管成员变量（对象或数组）是否引用了其他对象（实例）或者赋值为null它始终占用4字节。故此，对于String对象实例来说，它有三个int成员（3*4=12字节）、一个char[]成员（1*4=4字节）以及一个对象头（8字节），总共3*4 +1*4+8=24字节。根据这一原则，对String a=”rosen jiang”来说，实例a的shallow size也是24字节 
	Retained size是该对象自己的shallow size，加上从该对象能直接或间接访问到对象的shallow size之和。换句话说，retained size是该对象被GC之后所能回收到内存的总和。为了更好的理解retained size，不妨看个例子。 
	把内存中的对象看成下图中的节点，并且对象和对象之间互相引用。这里有一个特殊的节点GC Roots，正解！这就是reference chain的起点。 
![](http://seanhe.iteye.com/upload/picture/pic/100569/02faf3c6-f01d-3b05-b1bb-2375aec7e7ac.gif)![](http://seanhe.iteye.com/upload/picture/pic/100575/fc80e088-54dc-3626-a458-eb9618a24705.gif)
	从obj1入手，上图中蓝色节点代表仅仅只有通过obj1才能直接或间接访问的对象。因为可以通过GC Roots访问，所以左图的obj3不是蓝色节点；而在右图却是蓝色，因为它已经被包含在retained集合内。 
	所以对于左图，obj1的retained size是obj1、obj2、obj4的shallow size总和；右图的retained size是obj1、obj2、obj3、obj4的shallow size总和。obj2的retained size可以通过相同的方式计算。 
	如何查看某一个对象占用的内存空间 
	1.按以下方式打开新窗口即可 
![](http://seanhe.iteye.com/upload/picture/pic/100571/8b941d42-7430-3392-9bfa-b16d731a9e18.jpg)
	2.输入类名（输入类的全名） 
![](http://seanhe.iteye.com/upload/picture/pic/100573/1f8c47b5-7242-3d2c-91f1-ad8c85465434.jpg)
