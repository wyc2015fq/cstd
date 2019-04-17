# OpenCV学习笔记（三十八）——显示当前FPS - 迭代的是人，递归的是神 - CSDN博客





2011年12月22日 16:20:29[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：16724
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









最近做一个东西，需要实时显示，于是想在屏幕显示FPS。FPS是Frame Per Second的缩写，中文意思是每秒帧数，即帧速。FPS是测量用于保存、显示动态视频的信息数量。通俗来讲就是指动画或视频的画面数。

这就需要我在系统函数一讲里提到的getTickCount、getTickFrequency这两个函数了。前一个函数返回tick次数，后一个函数返回每秒tick次数，它们的比就是时间咯。

如果想得到一段程序的运行时间，可以套用下面的例子：



```cpp
double t = (double)getTickCount();
// do something ...
t = ((double)getTickCount() - t)/getTickFrequency();
```

以上这段程序在测试算法的时间消耗在哪里是很好的测试程序。我这里只需要稍微改写一下这个例子，得到每帧之间的时间，再用我之前在绘图函数那节讲过的putText把FPS显示到屏幕上咯。


不过有个问题还是让我感觉不舒服，就是用它做的定时不是很准，感觉时间比实际的快一些。测试较少，也不敢乱讲，欢迎大家测试并发表意见

简单的很，源程序下载地址：[http://download.csdn.net/detail/yang_xian521/3957523](http://download.csdn.net/detail/yang_xian521/3957523)



