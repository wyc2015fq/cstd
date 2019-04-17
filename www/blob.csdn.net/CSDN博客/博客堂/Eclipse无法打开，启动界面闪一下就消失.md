# Eclipse无法打开，启动界面闪一下就消失 - 博客堂 - CSDN博客





2018年03月14日 21:32:35[最小森林](https://me.csdn.net/u012052268)阅读数：434








Eclipse无法打开，启动界面闪一下就消失.但是后台有eclipse和java的进程在运行，

今天启动eclipse 闪了一下就消失了，及其郁闷，还有：

自己工作的电脑上的eclipse经常打开之后没有界面显示出来，但是后台有eclipse和java的进程在运行（占用比较大的内存与cpu），这个时候的解决办法是： 

要进入到当前eclipse选择的工作目录（也就是打不开eclipse界面的那个工作目录workspace）中 ——> .metadata.plugins 文件夹中，找到一个名叫org.eclipse.e4.workbench的文件夹，删除它就可以正常启动eclipse了。



