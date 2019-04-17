# 【Graph Embedding系列】: DeepWalk算法 - 快来学习鸭～～～ - CSDN博客





2019年03月21日 23:35:23[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：37
个人分类：[深度学习																[推荐系统																[graph embedding](https://blog.csdn.net/sxf1061926959/article/category/8767239)




论文“DeepWalk: Online Learning of Social Representations” 发表在kdd2014，

下载地址：[https://arxiv.org/pdf/1403.6652.pdf](https://arxiv.org/pdf/1403.6652.pdf)

作者开源的代码：[https://github.com/phanein/deepwalk](https://github.com/phanein/deepwalk)

文章提出的deepwalk用于学习图网络中节点的低维表示，学习出的低纬embedding可以被用于各种任务。其实deepwalk的思路我已经在[https://blog.csdn.net/sxf1061926959/article/details/88078008](https://blog.csdn.net/sxf1061926959/article/details/88078008)中做了介绍，方便自己以后学习，单独记录一下。

文章提供的思路很简单，我觉得这个算法很不错的一点是可以做实时训练，比如来一个新的节点，因为图节点的训练都是基于随机游走选择的session来进行的，所以新加入的节点，只需要对其邻近的节点选择合适的session进行训练就能使用，非常方便。能很好的应用到工业界中。

# 思路

整体的代码逻辑如下：

![](https://img-blog.csdnimg.cn/20190321232837124.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

## 1.构造graph

根据自身需求构造好一个同构网络G(V,E),指定参数w，为skip-gram的窗口大小，d为学习的embedding向量长度，r为重复训练节点的次数，t为随机游走截取session的长度。

## 2.随机游走

算法以构造好的同构网络作为输入，对图中节点做shuffle后遍历节点，做随机游走。其中每次的随机游走node按DFS从邻近node中均匀采样下一个node，下一个node继续重复以上过程，直到事先指定的长度为止。以上动作重复r次。

## 3.skip-gram训练

第二步可以生成大量的node 的session，将这些session看成句子，session中的node节点看作单词，利用skip-gram算法对其进行训练。从而生成每个node节点的embedding向量。

完](https://blog.csdn.net/sxf1061926959/article/category/8408661)](https://blog.csdn.net/sxf1061926959/article/category/6564385)





