# 光流（六）--L2范数Horn–Schunck 光流法及应用demo - 工作笔记 - CSDN博客





2016年07月14日 22:40:37[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7013
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

原文：

[http://www.cnblogs.com/rocbomb/p/3795719.html?utm_source=tuicool&utm_medium=referral](http://www.cnblogs.com/rocbomb/p/3795719.html?utm_source=tuicool&utm_medium=referral)

1. 基于Horn-Schunck模型的光流算法 

1.1     光流的约束条件 

光流 ![](http://www.openhw.org/include/picture/500/400/data/13-06/1372504902_d3434954.png)的假设条件认为图像序列![](http://www.openhw.org/include/picture/500/400/data/13-06/1372504978_6b9a86e0.png),在时间t
 的某一像素点![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505079_5af9a78f.png)与在时间t+1的这一像素点的偏移量保持不变，即![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505126_3ad4b50a.png)。这就是灰度值守恒假设，通过Taylor展开，就能得到光流的约束条件(OFC):![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505102_17a5ab02.png)，其中下标表示图像的梯度。

1.2     Horn-Schunck 模型 

1981年，Horn和Schunck根据同一个运动物体的光流场具有连续、平滑的特点, 提出一个附加约束条件，将光流场的整体平滑约束转换为一个变分的问题。它的能量方程如下：

![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505156_f5b4aac3.png)

[](http://www.eefocus.com/blog/link2url.php?link=http%3A%2F%2Fwww.openhw.org%2Fblog%2Flink2url.php%3Flink%3D%252Fdata%252F13-06%252F1372505156_f5b4aac3.png)其中数据项表示灰度值守恒约束，平滑项表示光流平滑约束。

1.3     Euler-Lagrange方程 

根据Horn-Schunck能量方程，可以推导出离散的欧拉-拉格朗日方程如下： 

![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505212_ee019d5a.png)

![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505225_945ce981.png)

其中， 表示图像像素点的坐标，  表示一个像素点的上下左右四个方向的相邻的像素点，当然，在图像的边界会少于四个元素。 

1.4     超松弛迭代 （SOR） 

根据上面的欧拉-拉格朗日方程，不难推到出迭代方程。这里选用收敛速度最快的超松弛算法（SOR），光流的初始值是![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505277_50492afe.png)，迭代方程如下：

![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505358_6252510f.png)

![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505369_47f89fe3.png)

其中，  w是迭代的权重因子, k 是迭代的次数， ![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505383_d623a6e9.png)是光流的计算的权值，![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505398_422eb990.png)表示第
 个像素点的上和左的相邻像素点，![](http://www.openhw.org/include/picture/500/400/data/13-06/1372505411_dfded12c.png)表示第
 个像素点的下和右的相邻像素点。 

以上来自[http://www.eefocus.com/wangzhibin/blog/13-08/296536_641cd.html](http://www.eefocus.com/wangzhibin/blog/13-08/296536_641cd.html) 这个是人家的一个HS光流实际项目，先mark下。



2算法时间对比 

使用图片 Grove2 （来自 [http://vision.middlebury.edu/flow/data/](http://vision.middlebury.edu/flow/data/)） 

640 x 480 

金字塔层数 3 

迭代次数 100 

cpu i5 3210m  GPU GT630m 

时间  4.479s     1.496s 

加速比 2.99 



金字塔层数 3 

迭代次数 500 

cpu i5 3210m  GPU GT630m 

时间  20.586s     3.905s 

加速比 5.27



3结论

加速比来自算法的并行计算

如果算法可并行程度高，则计算加快。



