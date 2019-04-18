# 图像局部显著性—点特征（GLOH） - wishchinYang的专栏 - CSDN博客
2017年03月09日 17:23:58[wishchin](https://me.csdn.net/wishchin)阅读数：2056
        基于古老的Marr视觉理论，视觉识别和场景重建的基础即第一阶段为局部显著性探测。探测到的主要特征为直觉上可刺激底层视觉的局部显著性——特征点、特征线、特征块。
        相关介绍：[局部特征显著性—点特征](http://blog.csdn.net/wishchin/article/details/45146117)（SIFT为例） 
       从特征提取上说，GLOH使用了各向同性平均；从特征选择的角度上说，GLOH使用了PCA方法，体现领域专用同时丧失一定的多领域泛化能力。
**五、GLOH特征（梯度位置方向直方图）**
       2005年MIko等人提出的SIFT的变子，改进为关键点周围的区间划分，由**田字格划分修改为 八象限圆格**划分，如下图：
![](https://img-my.csdn.net/uploads/201206/21/1340243657_6304.jpg)
       在很大的一个训练集上训练得到PCA模型，再将**272维直方图映射到一个128维的描述**子。在整体的测试中，比SIFT性能有显著的提高。
       GLOH以 建立训练模型的方式对特征描述施加影响，在一般情况下可以学习到特定领域图像的特征流形分布，比SIFT要好很多；对于更为广泛的应用，其性能也会受到预训练影响。
**参考资料**：
            GLOH Wiki百科: [https://en.wikipedia.org/wiki/GLOH](https://en.wikipedia.org/wiki/GLOH)
            GLOH原始论文: [Krystian Mikolajczyk and Cordelia Schmid "A performance evaluation of local descriptors", IEEE Transactions on Pattern Analysis and Machine Intelligence, 10, 27, pp 1615--1630, 2005.](http://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=1498756&isnumber=32189)
