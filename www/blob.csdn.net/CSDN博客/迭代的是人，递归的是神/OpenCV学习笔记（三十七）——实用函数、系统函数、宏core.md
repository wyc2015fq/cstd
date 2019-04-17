# OpenCV学习笔记（三十七）——实用函数、系统函数、宏core - 迭代的是人，递归的是神 - CSDN博客





2011年12月20日 16:22:31[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：15005
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









如果你想在OpenCV的基础上自己开发一些算法，我觉得core这部分内容不得不精啊，能熟练使用OpenCV的数据结构是开发的基础，又是重中之重。最近就又拌在这上头了，所以再重温一下。这次分析一下Utility and System Functions and Macros这部分，就是实用函数、系统函数和宏。

OpenCV在这部分里包含一些类似标准c++、c里面的一些东西。

系统函数有很多。有些是内存方面的操作，类似new delete之类的操作，很多都是为了防止内存溢出的函数有alignPtr、alignSize、allocate、deallocate、fastMalloc、fastFree。格式输出的函数啦format（是不是太熟悉了）。还有一些和系统相关的东西checkHardwareSupport、getNumThreads、getThreadNum、getTickCount、getTickFrequency、getCPUTickCount、setNumThreads。还有一些比较有意思的东西，比如功能类似#ifdef
 #endf这样的开关，也有函数setUseOptimized实现 ，相关的还有useOptimized函数。还有一个不懂的函数saturate_cast（求指点）等待各位补充。

实用函数很接近<math.h>里的一些函数，很有意思，可以方便大家开发，有计算向量角度的函数fastAtan2、计算立方根的函数cubeRoot、向上取整函数cvCeil、向下取整函数cvFloor、四舍五入函数cvRound。注意cvCeil、cvFloor、cvRound和大家常用的ceil、floor、round函数略有不同，标准库函数返回值并不是int型的变量，必要时需强制转换，而OpenCV里面的取整函数返回值为int型。还有一些类似matlab里面的函数，比如cvIsInf判断自变量是否无穷大，cvIsNaN判断自变量是否Not
 a Number。

宏也很多，这些宏在标准c++、c里也有出现，CV_Assert是个断言，不知道和assert（）有什么区别。error也是一种异常，还有Exception类这个异常类。

有了这些东西，开发起来有没有更顺手一些呢~~随着使用的深入，我会再补充一些上诉东西的使用心得



