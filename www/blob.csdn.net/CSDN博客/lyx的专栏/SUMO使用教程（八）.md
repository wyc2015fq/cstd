# SUMO使用教程（八） - lyx的专栏 - CSDN博客





2015年11月08日 15:09:32[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1587
所属专栏：[SUMO使用教程](https://blog.csdn.net/column/details/15585.html)









好久没更新SUMO的博客了。
今天来看一下之前生成的一个Trips文件。

下面这个就是随机生成的Trips问价的部分截图，trip，中文意思就是旅程，其实就是车辆走过的轨迹。之前我们在router文件里面定义了车辆行驶的路径，很显然，相当费力气，需要一条一条的去规划，但是在trip文件中，我们只需要说明起始点就可以了，SUMO的duarouter.exe工具会自动计算最优化路径，并且生成router文件。这也就是为什么在教程一中我们randomTrips生成的是trip文件而不直接是router文件的原因。不可否认，SUMO的模块化工作是做的很细致的。

![](https://img-blog.csdn.net/20160708230317481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这是我用于测试的一个trip文件，从徐汇区一个小区域左下端开始，自动规划后行驶到上端。
![](https://img-blog.csdn.net/20160708230325608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


但是，似乎用trip文件有一个问题，就是一个trip标签只能有一辆车。
![](https://img-blog.csdn.net/20160708230328997?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其实还有一个flows文件，这个文件可以设置走同一条路径的车辆数，可以等间隔的从指定起点发车。






