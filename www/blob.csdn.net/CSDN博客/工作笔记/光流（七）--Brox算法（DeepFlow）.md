# 光流（七）--Brox算法（DeepFlow） - 工作笔记 - CSDN博客





2016年07月15日 12:06:27[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8734
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：

http://www.cnblogs.com/dzyBK/p/5096860.html



参考论文：

1. High Accuracy Optical Flow Estimation Based on a Theory for Warping, Thomas Box, ECCV2004

2. Beyond Pixels Exploring New Representations and pplications for Motion Analysis, Ce Liu, MIT2009

3. Lucas/Kanade meets Horn/Schunck: Combining local and global optic flow methods, Andres Bruhn, IJCV2005

阅读基础：

1.了解变分法，这项可选。

2.熟悉HS光流算法，这项必选。

基于能量最小化的光流算法都比较复杂，这里分成以下几部分来阐述

(1)欧拉-拉格朗日方程的推导

(2)数值求解

(3)整理数据项

(4)整理平滑项

 懒得再写一遍啦，还是上传图像吧。

![](http://images2015.cnblogs.com/blog/705780/201601/705780-20160106224350762-1640419583.jpg)![](http://images2015.cnblogs.com/blog/705780/201601/705780-20160106224356559-1785976846.jpg)![](http://images2015.cnblogs.com/blog/705780/201601/705780-20160106224401918-1132217899.jpg)![](http://images2015.cnblogs.com/blog/705780/201601/705780-20160106224408543-146140917.jpg)![](http://images2015.cnblogs.com/blog/705780/201601/705780-20160106224414481-118053604.jpg)![](http://images2015.cnblogs.com/blog/705780/201601/705780-20160106224419387-766556160.jpg)



