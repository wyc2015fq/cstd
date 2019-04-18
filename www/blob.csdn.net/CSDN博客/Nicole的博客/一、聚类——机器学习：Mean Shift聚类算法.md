# 一、聚类——机器学习：Mean Shift聚类算法 - Nicole的博客 - CSDN博客
2018年07月24日 14:59:09[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：264
本文由ChardLau原创，转载请添加原文链接[https://www.chardlau.com/mean-shift/](https://www.chardlau.com/mean-shift/)
今天的文章介绍如何利用`Mean Shift`算法的基本形式对数据进行聚类操作。而有关`Mean Shift`算法加入核函数计算漂移向量部分的内容将不在本文讲述范围内。实际上除了聚类，`Mean Shift`算法还能用于计算机视觉等场合，有关该算法的理论知识请参考[这篇文章](http://www.cse.psu.edu/~rtc12/CSE598G/introMeanShift_6pp.pdf)。
### `Mean Shift`算法原理
下图展示了`Mean Shift`算法计算飘逸向量的过程：![Mean Shift](https://www.chardlau.com/content/images/2018/07/------_a041dda2-c3bd-4c1a-a4c7-7e3c033552ee.png)
`Mean Shift`算法的关键操作是通过感兴趣区域内的数据密度变化计算中心点的漂移向量，从而移动中心点进行下一次迭代，直到到达密度最大处（中心点不变）。从每个数据点出发都可以进行该操作，在这个过程，统计出现在感兴趣区域内的数据的次数。该参数将在最后作为分类的依据。
与`K-Means`算法不一样的是，`Mean Shift`算法可以自动决定类别的数目。与`K-Means`算法一样的是，两者都用集合内数据点的均值进行中心点的移动。
### 算法步骤
下面是有关`Mean Shift`聚类算法的步骤：
- 在未被标记的数据点中随机选择一个点作为起始中心点center；
- 找出以center为中心半径为radius的区域中出现的所有数据点，认为这些点同属于一个聚类C。同时在该聚类中记录数据点出现的次数加1。
- 以center为中心点，计算从center开始到集合M中每个元素的向量，将这些向量相加，得到向量shift。
- center = center + shift。即center沿着shift的方向移动，移动距离是||shift||。
- 重复步骤2、3、4，直到shift的很小（就是迭代到收敛），记住此时的center。注意，这个迭代过程中遇到的点都应该归类到簇C。
- 如果收敛时当前簇C的center与其它已经存在的簇C2中心的距离小于阈值，那么把C2和C合并，数据点出现次数也对应合并。否则，把C作为新的聚类。
- 重复1、2、3、4、5直到所有的点都被标记为已访问。
- 分类：根据每个类，对每个点的访问频率，取访问频率最大的那个类，作为当前点集的所属类。
