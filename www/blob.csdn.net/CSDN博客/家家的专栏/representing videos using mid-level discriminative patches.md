# representing videos using mid-level discriminative patches - 家家的专栏 - CSDN博客





2013年06月28日 16:06:06[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1346
个人分类：[乐在科研中](https://blog.csdn.net/yihaizhiyan/article/category/742190)









CVPR13上的一篇，拿来细读~

局部性patch的思想，来学习挖掘中层discriminative patches。

A **representation** for videos    based on    **mid-level discriminative spatio-temporal patches.**

动作分类视频中，有很多背景和冗余信息，分析视频，自动挖掘，把有辨识力的时空patch找出来，做video analysis，定时极好的~

这些discriminative patches组成字典，进行action classification。



**Mining discriminative patches：**满足两方面的要求：一是在一个类中重复出现；二是不同于其他类的patches （有区分度的patch）。

         learning一种有辨识力的度量，然后进行聚类。        an exemplar-based clustering approach.









