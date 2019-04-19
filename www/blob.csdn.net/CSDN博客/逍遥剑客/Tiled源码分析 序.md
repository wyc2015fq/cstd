# Tiled源码分析: 序 - 逍遥剑客 - CSDN博客
2014年01月14日 22:53:55[xoyojank](https://me.csdn.net/xoyojank)阅读数：2638
最近业余玩玩了cocos2d-x, 在做地图部分时发现了[Tiled](http://www.mapeditor.org/)这个开源的地图编辑器
深入了解之后, 发现这个编辑十分强大, 好用到[其他2D引擎](https://github.com/bjorn/tiled/wiki/Support-for-TMX-maps)都支持其地图格式, 几乎成为了一种标准
![](https://img-blog.csdn.net/20140114224845875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG95b2phbms=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在使用过程中, 有时会有对其编辑功能进行扩充的需要, 所以顺便看了一下它的代码
读过之后, 越看越喜欢, 做为开源的编辑器, 代码能写这么漂亮很不简单
想想自己用Qt写的几个工具, 总是感觉有点耦合, 所以就有了学习一下Tiled代码的想法, 以此为序
