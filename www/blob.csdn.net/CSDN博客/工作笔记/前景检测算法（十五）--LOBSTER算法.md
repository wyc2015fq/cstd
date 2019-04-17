# 前景检测算法（十五）--LOBSTER算法 - 工作笔记 - CSDN博客





2016年07月10日 20:37:42[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5941
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









﻿﻿

**原文:**

**[http://blog.csdn.net/kezunhai/article/details/45972683](http://blog.csdn.net/kezunhai/article/details/45972683)**

**基于局部二值相似性模式（LBSP）的运动目标检测算法**

[kezunhai@gmail.com](mailto:kezunhai@gmail.com)

[http://blog.csdn.net/kezunhai](http://blog.csdn.net/kezunhai)


       本文根据论文：**Improving background subtraction using local binary similarity patternsWACV2014**的内容及自己的理解而成，如果想了解更多细节，请参考原文。该文章思想借鉴了VIBE，其实可以**理解成是VIBE+LBP算子变种（LBSP）运动目标检测算法的组合**。在VIBE中，算法主要是基于像素之间的比较，而没有考虑到领域空间的相关信息。在这篇文章里作者采用具有空间信息LBSP描述子进行背景建模，提高了算法的检测性能和稳定性，通过CDnet的数据集的测试表明该算法优于大多数算法，下面对该文章进行具体介绍。

        本文算法作者又简称为LOBSTER（**LO**cal  **B**inary**S**imlilarity
 segmen**TER**）。首先介绍下LBP算子，LBP算子的计算公式如下，

![](https://img-blog.csdn.net/20150525184224155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2V6dW5oYWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


而LBSP该算子与LBP的唯一不同点就是中间像素与周围像素进行比较时采用的是绝对值，当该绝对值小于某一值时，则为0，否则为1，计算公式如下：


![](https://img-blog.csdn.net/20150525184807490?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2V6dW5oYWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


算法的其他处理与VIBE算法的处理一致。作者也给出了伪代码：

![](https://img-blog.csdn.net/20150525185347764?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2V6dW5oYWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


源代码可以在这里下载：[https://bitbucket.org/pierre_luc_st_charles/lobster](https://bitbucket.org/pierre_luc_st_charles/lobster)

更多内容请参考：

１、**mproving background subtraction using local binary similarity patternsWACV2014**

2、**Change Detection in feature space using local binary similarity patternsBilodeauetalCRV2013**



