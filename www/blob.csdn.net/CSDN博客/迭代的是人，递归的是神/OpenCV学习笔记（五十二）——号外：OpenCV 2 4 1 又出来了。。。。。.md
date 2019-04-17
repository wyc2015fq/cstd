# OpenCV学习笔记（五十二）——号外：OpenCV 2.4.1 又出来了。。。。。 - 迭代的是人，递归的是神 - CSDN博客





2012年06月06日 08:41:24[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：9580
所属专栏：[OpenCV学习笔记](https://blog.csdn.net/column/details/opencv-manual.html)









距离2.4.0放出来才一个月。儿童节当天2.4.1就出炉了。。。。真心跟不上节奏啊，路漫漫其修远兮。。。。

这次主要的改变还是bug的修订，之前有朋友告诉我，说他的2.3.1版本说什么都不能成功配置CUDA，后来他用2.2版本就成功了，看来新出的版本bug还是很多的，也不能盲目求新。这次的2.4.1版本更新如下：

GPU模块支持了CUDA4.1、CUDA4.2版本，添加了一个文件字符串存储回读的API接口

完善了光流法的clacOpticalFlowPyrLK，支持了金字塔图像作为输入

完善了文档，使文档和版本更加对应

修正了SURF、MSER的python包装

修正了其他45处bug

最近更新比较慢，感觉没有学习OpenCV过程有新的太大收获，还请见谅



