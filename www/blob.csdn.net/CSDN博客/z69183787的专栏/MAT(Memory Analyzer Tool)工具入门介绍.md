# MAT(Memory Analyzer Tool)工具入门介绍 - z69183787的专栏 - CSDN博客
2015年09月22日 13:43:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：492
1、MAT是什么？
    MAT(Memory Analyzer Tool)，一个基于Eclipse的内存分析工具，是一个快速、功能丰富的JAVA heap分析工具，它可以帮助我们查找内存泄漏和减少内存消耗。使用内存分析工具从众多的对象中进行分析，快速的计算出在内存中对象的占用大小，看看是谁阻止了垃圾收集器的回收工作，并可以通过报表直观的查看到可能造成这种结果的对象。
![](http://hi.csdn.net/attachment/201104/2/0_13017260876isZ.gif)
2.为什么使用MAT？
    当服务器应用占用了过多内存的时候，会遇到OutOfMemoryError。如何快速定位问题呢？Eclipse MAT的出现使这个问题变得非常简单。它能够离线分析dump的文件数据。
    Eclipse MAT是SAP公司贡献的一个工具，可以在Eclipse网站下载到它，完全免费的。它可比Sun提供的内存镜像分析工具jhat要强太多了。
3.
首页：[http://www.eclipse.org/mat/![](http://blog.csdn.net/fenglibing/article/details/images/icons/linkext7.gif)](http://www.eclipse.org/mat/)
插件更新地址：
[http://download.eclipse.org/mat/1.0/update-site/](http://download.eclipse.org/mat/1.0/update-site/)![](http://blog.csdn.net/fenglibing/article/details/images/icons/linkext7.gif)
先调用jdk的工具得到heap使用情况
我安装的是jdk1.6
C:/>java -version
java version "1.6.0_11"
Java(TM) SE Runtime Environment (build 1.6.0_11-b03)
Java HotSpot(TM) Client VM (build 11.0-b16, mixed mode, sharing)
调用jdk工具jps查看当前的java进程
C:/>jps
3504 Jps
3676 Bootstrap
3496 org.eclipse.equinox.launcher_1.0.201.R35x_v20090715.jar
调用jmap工具得到信息
C:/>jmap -dump:format=b,file=heap.bin 3676
Dumping heap to C:/heap.bin ...
Heap dump file created
这时，我们的C盘根目录，就生成了heap.bin文件，用eclipse的file---->open打开这个文件，首先是一个启动图：
![](http://hi.csdn.net/attachment/201104/2/0_13017267038hni.gif)
这里可以选择查看
1、内存泄露报表，自动检查可能存在内存泄露的对象，通过报表展示存活的对象以及为什么他们没有被垃圾收集；
2、对象报表，对可颖对象的分析，如字符串是否定义重了，空的collection、finalizer以及弱引用等。
我这里选择的是查看内存报表，以下是截的简略图：
![](http://hi.csdn.net/attachment/201104/2/0_1301726708zY2a.gif)
![](http://hi.csdn.net/attachment/201104/2/0_1301726708zY2a.gif)
通过报表展示，蛮清楚的，下面还有详细的说明，这里就没有帖图了，有兴趣的可以继续探究。
