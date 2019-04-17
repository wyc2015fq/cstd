# 图像局部特征（十九）--GLOH - 工作笔记 - CSDN博客





2018年02月22日 09:45:47[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6400
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









**转自：http://blog.csdn.net/wishchin/article/details/45146117**

**GLOH特征（梯度位置方向直方图）**


       2005年MIko等人提出的SIFT的变子，改进为关键点周围的区间划分，由**田字格划分修改为 八象限圆格**划分，如下图：


![](https://img-my.csdn.net/uploads/201206/21/1340243657_6304.jpg)



       在很大的一个训练集上训练得到PCA模型，再将**272维直方图映射到一个128维的描述**子。在整体的测试中，比SIFT性能有显著的提高。

       GLOH以 建立训练模型的方式对特征描述施加影响，在一般情况下可以学习到特定领域图像的特征流形分布，比SIFT要好很多；对于更为广泛的应用，其性能也会受到预训练影响。

GLOH原始论文: [Krystian Mikolajczyk and Cordelia Schmid "A performance evaluation of local descriptors", IEEE Transactions on Pattern Analysis and Machine Intelligence, 10, 27, pp 1615--1630, 2005.](http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=1498756&isnumber=32189)





