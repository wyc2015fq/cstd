# 目标检测（四）--ICF - 工作笔记 - CSDN博客





2017年08月27日 11:17:05[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5425
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿转自：[http://blog.csdn.net/carson2005/article/details/8455837](http://blog.csdn.net/carson2005/article/details/8455837)

Integral Channel Features; Piotr Dollar、Zhuowen Tu; BMVC2009;




目标检测[算法](http://lib.csdn.net/base/datastructure)的性能主要受到两个因素的制约：学习算法和特征描述算子。积分通道特征（作者自创的词汇，后面会详述）主要从特征描述的角度来探讨解决问题的可能性。积分通道特征(integral channel features)的大概思路是：通过对输入图像做各种线性和非线性的变换，诸如局部求和、直方图、haar-like及它们的变种之类的特征便可以通过积分图来快速计算出来。这算不算类似CNN特征计算的本质，加上积分图加速方法？
![](https://img-my.csdn.net/uploads/201212/31/1356945530_5761.png)

![](https://img-my.csdn.net/uploads/201212/31/1356945540_1428.png)


![](https://img-my.csdn.net/uploads/201212/31/1356945551_9243.png)


![](https://img-my.csdn.net/uploads/201212/31/1356945564_4560.png)


![](https://img-my.csdn.net/uploads/201212/31/1356945574_7686.png)


![](https://img-my.csdn.net/uploads/201212/31/1356945587_3576.png)

补充（部分内容有重复）：

积分通道特征的大致含义是：通过对输入图像进行各种线性和非线性的变换，图像的很多（常用）特征，例如，局部求和，直方图，haar以及它们的变种，可以借助于积分图来快速，高效的计算出来。作者通过一系列的实验数据和相关公式，证明了积分通道特征不仅比HOG特征具有更好的性能（当然，是指代行人检测方面），而且它还具有以下四个特征：

（1）从不同角度集成了各种特征信息。

（2）在检测过程中，空间定位准确性更高；

（3）具有较少的参数设置；

（4）跟级联分类器相互结合之后，具有更快的检测效果；

![](https://img-my.csdn.net/uploads/201301/13/1358065217_2621.png)

![](https://img-my.csdn.net/uploads/201301/13/1358065299_6368.png)


![](https://img-my.csdn.net/uploads/201301/13/1358065370_9178.png)


![](https://img-my.csdn.net/uploads/201301/13/1358065391_1912.png)


![](https://img-my.csdn.net/uploads/201301/13/1358065440_5624.png)


![](https://img-my.csdn.net/uploads/201301/13/1358065529_6708.png)


![](https://img-my.csdn.net/uploads/201301/13/1358065567_4110.png)


![](https://img-my.csdn.net/uploads/201301/13/1358065639_8607.png)


![](https://img-my.csdn.net/uploads/201301/13/1358065654_2814.png)


![](https://img-my.csdn.net/uploads/201301/13/1358065670_6173.png)


![](https://img-my.csdn.net/uploads/201301/13/1358065694_5154.png)


![](https://img-my.csdn.net/uploads/201301/13/1358065747_3476.png)


积分通道特征的matlab代码可以去这里下载：[http://vision.ucsd.edu/~pdollar/toolbox/doc/index.html](http://vision.ucsd.edu/~pdollar/toolbox/doc/index.html)

﻿﻿



