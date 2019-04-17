# 前景检测算法（八）--SACON算法 - 工作笔记 - CSDN博客





2016年07月09日 14:34:31[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6455
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

原文：

[http://www.cnblogs.com/dwdxdy/p/3530862.html](http://www.cnblogs.com/dwdxdy/p/3530862.html)







# [【背景建模】SACON](http://www.cnblogs.com/dwdxdy/p/3530862.html)





**SACON（SAmple CONsensus）**算法是基于样本一致性的运动目标检测算法。该算法通过对每个像素进行样本一致性判断来判定像素是否为背景。

**算法框架图**

![](http://images.cnitblog.com/blog/414008/201401/231013315072.png)

　　由上图可知，该算法主要分为四个主要部分，分别是邻域差分、SACON算法核心处理、空洞填充后处理、TOM(Time Out Map)，其中TOM(Time Out Map)主要用于背景模型更新，其他部分属于前景目标检测。

**背景模型建立**

　　SACON算法建立背景模型的方法是**直接取视频序列的前N帧作为背景模型**。

　　对于每个像素而言，其背景模型可以表示为：C(c1,c2,...,cN)，对于彩色图像，ci=(r,g,b)，对于灰度图像，则对应于灰度值。

**前景目标检测**

**1.邻域帧间差分**

　　为了减少计算量，加快计算速度，采用邻域帧间差分法提取可能的运动像素。

**2.SACON算法核心**

　　对于新像素，将其与对应的背景模型比较，判断新像素是否满足背景模型样本的一致性。判定方法如下：

![](http://images.cnitblog.com/blog/414008/201401/231025046322.png)

　　其中，两个公式分别计算新像素与背景模型的样本是否距离相近和统计距离相近的样本数目，当距离相近的样本数目大于阈值时，则判定新像素为背景，Bt(m) = 1。

　　公式涉及的阈值主要有Tr和Tn，**当背景模型的样本集合的数目N越大，距离阈值Tr越大，则近似样本数目阈值Tn越大，因而Tn正比于N和Tr**，Tn
 = a * N * Tr，a为比例因子。

**3.后处理**

　　后处理主要包括两个操作：阴影去除和空洞填充。

　　阴影去除：采用进一步的验证规则，去除阴影区域，验证规则如下：

![](http://images.cnitblog.com/blog/414008/201401/231036597413.png)

　　其中，{r,g,I}是归一化的色彩空间，r = R/(R+G+B), g = G/(R+G+B),I = (R+G+B)/3，具体细节请参考论文资料。

　　空洞填充：对于检测到的前景，采用连通性分析，填充前景区域的内部空洞，主要通过形态学滤波实现。

**背景模型更新**

　　考虑到背景的移出或前景的停止，SACON算法采用TOM(Time of Map)进行处理，将背景的移出和运动目标的停止而引起的变化很快的融入到背景中，具体的更新方法分为Pixel-level和Blob-level。

**1.Pixel-level**

![](http://images.cnitblog.com/blog/414008/201401/231044586321.png)

　　该方法主要统计像素m连续分为前景的次数，TOMt(m)。

　　如果TOMt(m)的值超过一个阈值，则更新像素m对应的背景模型。

**2.Blob-level**

![](http://images.cnitblog.com/blog/414008/201401/231049121631.png)

　　如果一个目标(a blob)处于静止状态，则该目标所包含的所有像素的TOM都加1；

　　如果不处于静止状态，则该目标所包含的所有像素的TOM都置为0；

　　如果目标的TOM值大于阈值时，则该目标所包含的所有像素更新为背景像素。

　　Blob-level的更新方法，主要是弥补Pixel-level的不足，当一个目标包含运动的部分和不运动的部分，不运动的部分会持续被判定会前景，从而会被Pixel-level更新为背景；而运动的部分会时而被判定为前景，时而判定为背景，不会被Pixel-level更新为背景；最终的更新后结果是一个目标被分割，部分被检测为前景，部分被检测为背景，不合理。

　　Blob-level通过整体判断目标是否处于运动或静止，从而整体更新目标，保证了目标的完整性。

![](http://images.cnitblog.com/blog/414008/201401/231059289292.png)

　　如上图所示，描述一个人走入房间，坐下并打电话，打电话过程中，人大部分区域处于静止状态，只有部分区域处于轻微运动状态。

　　(a)是原始图片，(b)是利用Pixel-level更新方法后的检测结果，(c)是结合Pixel-level和Blob-level更新方法的检测结果。

　　由显示结果可以看出，Pixel-level将处于静止状态的区域更新为背景，而Blob-level方法判定人整体处于运动状态，因而人未被更新为背景。

**注意：**

**1.背景更新时，背景模型样本集合的替换策略未说明，是先进先出，还是随机替换，每次替换的数目也未说明。**

**2.如何判定一个目标(a blob)是处于运动状态，还是处于静止状态，文中只是提到可以利用目标的中心和像素的数目判定，但具体的策略也未说明。**

**参考资料：**

[http://blog.csdn.net/kezunhai/article/details/9500637](http://blog.csdn.net/kezunhai/article/details/9500637)

[http://www.dotblogs.com.tw/dragon229/archive/2012/01/13/65615.aspx](http://www.dotblogs.com.tw/dragon229/archive/2012/01/13/65615.aspx)

Background Subtraction Based on a Robust Consensus Method

SACON: A Consensus Based Model for Background Subtraction











