# 前景检测算法（十二）--基于模糊Choquet积分 - 工作笔记 - CSDN博客





2016年07月10日 15:58:30[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6811
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文:

[http://www.cnblogs.com/pangblog/p/3303956.html](http://www.cnblogs.com/pangblog/p/3303956.html)

本文根据论文：**Fuzzy Integral for Moving Object Detection-FUZZ-IEEE_2008**的内容及自己的理解而成，如果想了解更多细节，请参考原文。在背景建模中，我们对于像素的分类总是采用非此即彼的方式来分，即该像素要么是背景要么是前景。然而，由于噪声、光照变化以及阴影等特殊情况导致像素会存在错误，即像素存在一定的不确定性。为了处理这种不确定性，本文提出了基于模型Choquet积分的目标检测算法。

            首先，我们来看看这个算法的基本流程，如下图所示。

![](https://img-blog.csdn.net/20130904212832500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2V6dW5oYWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


从上图可以看到，该算法分成三部分：① 计算颜色和纹理相似性；② 将计算得到的颜色和纹理相似性利用Choquet积分进行融合；③  根据Choquet积分的结果进行前景、背景的分类。下面对算法的这些核心部分进行详细的介绍。

          1）颜色相似计算

            在本文中，作者对多种颜色空间进行了研究与分析，通过实验分析，觉得在RGB、HSV、YCrCb、Ohta等四个颜色空间处理效果较佳。推荐使用YCrCb、HSV和Ohta，因此相对于RGB，这三个空间对光照变化具有更强的鲁棒性。颜色相似性的计算公式如下所示：

![](https://img-blog.csdn.net/20130904213520921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2V6dW5oYWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


其中Ic表示当前帧，而Ib表示背景帧，k表示通道数。

          2）纹理相似性计算

            纹理相似性本文采用了具有原理简单、计算高效快捷、对光照具有一定鲁棒性的LBP特征，LBP特征的计算可以参考我前面的博文（建议采用LBP的变体算子）：[局部二值模式LBP（Local
 Binary Pattern）实现代码](http://blog.csdn.net/kezunhai/article/details/10238339)。而关于LBP纹理特征的相似性计算如下式所示：

![](https://img-blog.csdn.net/20130904214114281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2V6dW5oYWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


         3）Choquet积分

           要了解关于choquet积分的更多内容，可以参考：**The Choquet integral for the aggregation of interval scales in multicriteria decision making**。而choquet积分的定义如下：

![](https://img-blog.csdn.net/20130904214630328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2V6dW5oYWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


            4）前景背景的分类

             前景背景的分类主要是根据前面choquet积分的结果进行阈值化，但是论文中采用的是全局阈值，这也是论文的一个缺点。 但是从总体上说，本文提出的算法对光照具有很强的鲁棒性、对突然光照的变化不会引起大面积的前景。博主通过实验表明：算法效果还可以，但是由于采用的是全局阈值，所以也会存在所有目标检测中阈值选取中的问题。建议：可以尝试计算各个像素相似性的均值和方差，然后根据像素的特点自适应确定各个像素的阈值。

﻿﻿



