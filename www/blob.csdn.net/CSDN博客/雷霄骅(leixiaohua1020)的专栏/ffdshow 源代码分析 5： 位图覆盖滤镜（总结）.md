# ffdshow 源代码分析 5： 位图覆盖滤镜（总结） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月30日 19:38:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：35











注：写了一系列的文章，都是关于ffdshow中的位图覆盖滤镜的源代码分析的，在此列一个列表：

[ffdshow 源代码分析 2： 位图覆盖滤镜（对话框部分Dialog）](http://blog.csdn.net/leixiaohua1020/article/details/12981725)
[ffdshow 源代码分析 3： 位图覆盖滤镜（设置部分Settings）](http://blog.csdn.net/leixiaohua1020/article/details/13004051)
[ffdshow 源代码分析 4： 位图覆盖滤镜（滤镜部分Filter）](http://blog.csdn.net/leixiaohua1020/article/details/13006213)
[ffdshow 源代码分析 5： 位图覆盖滤镜（总结）](http://blog.csdn.net/leixiaohua1020/article/details/13660583)

![](https://img-blog.csdn.net/20140616103721062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

前面写了三篇文章，介绍了位图覆盖滤镜的3个部分：



第一篇文章介绍了ffdshow的位图覆盖滤镜的对话框（Dialog）部分：[ffdshow 源代码分析2 ： 位图覆盖滤镜（对话框部分Dialog）](http://blog.csdn.net/leixiaohua1020/article/details/12981725)

第二篇文章介绍了ffdshow的位图覆盖滤镜的设置（Settings）部分：[ffdshow 源代码分析 3： 位图覆盖滤镜（设置部分Settings）](http://blog.csdn.net/leixiaohua1020/article/details/13004051)

第三篇文章介绍了ffdshow的位图覆盖滤镜的滤镜（Filter）部分：[ffdshow 源代码分析 4： 位图覆盖滤镜（滤镜部分Filter）](http://blog.csdn.net/leixiaohua1020/article/details/13006213)

在此，用一张图总结他们之间的关系：



![](https://img-blog.csdn.net/20131030191924781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如图中所示，设置（Settings）部分是直接和系统上层关联的，它包含两个接口函数：createPages()和createFilters()。分别用于创建对话框（Dialog）和滤镜（Filter）。其中在TbitmapPage中对话框直接和注册表变量关联。而在TbitmapSettings中注册表变量和系统中的变量关联。TimgFilterBitmap最终读取TbitmapSettings中的变量完成相应的操作。

目前来开TimgFilterBitmap是不会直接读取TbitmapPage类中的值的。




























