# 【Graph Embedding系列】: SDNE算法 - 快来学习鸭～～～ - CSDN博客





2019年04月09日 23:17:01[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：28
个人分类：[深度学习																[graph embedding																[推荐系统](https://blog.csdn.net/sxf1061926959/article/category/8408661)




论文“Structural Deep Network Embedding”发表在kdd2016

论文下载地址：[https://www.kdd.org/kdd2016/papers/files/rfp0191-wangAemb.pdf](https://www.kdd.org/kdd2016/papers/files/rfp0191-wangAemb.pdf)

论文利用深度自编码器来学习图中节点的embedding向量，结合一阶和二阶相似度进行联合训练，将二阶相似度作为无监督信息，捕获全局网络结构信息，一阶相似度作为有监督信息，用于捕获局部网络结构。

### 提出问题

文章提出 graph embedding存在一下几个问题：

**1.高非线性(high non-linearity)：**图的潜在结构是高非线性的，因此浅层模型很难捕获全面的非线性结构。

**2.结构保持(structure-preserving): **学习出来的节点embedding需要能保留住图的全局和局部结构信息。

**3.稀疏性(sparsity): **真实环境下，网络通常都是稀疏的，会有大量合理的链接丢失。

### 如何解决问题

**解决高非线性问题：**浅层模型很难学习好高非线性结构，所以本文采用深度模型来拟合非线性结构。

**解决结构保持和稀疏性问题**：联合训练一阶和二阶相似性，一阶指的是直接有边相连的邻近节点，用于学习局部网络结构。但是真实的网络往往都是稀疏的，缺失了大量合理链接，所以一阶相似性节点的数据少，需要利用二阶相似度来做扩展，二阶相似度表示不同定点之间的邻域结构的相似性，以捕获全局网络结构。

### 问题定义

### 图

定义图为G=(V,E),其中![V= \{v_1,...,v_n\}](https://private.codecogs.com/gif.latex?V%3D%20%5C%7Bv_1%2C...%2Cv_n%5C%7D)表示n个节点，![E=\{e_{i,j}\}^n_{i,j=1}](https://private.codecogs.com/gif.latex?E%3D%5C%7Be_%7Bi%2Cj%7D%5C%7D%5En_%7Bi%2Cj%3D1%7D)表示节点i和j之间的边。每条边![e_{i,j}](https://private.codecogs.com/gif.latex?e_%7Bi%2Cj%7D)分配有一个权重![s_{i,j}\geqslant 0](https://private.codecogs.com/gif.latex?s_%7Bi%2Cj%7D%5Cgeqslant%200),如果节点i和j没有边相连的话，那么![s_{i,j}=0](https://private.codecogs.com/gif.latex?s_%7Bi%2Cj%7D%3D0)，否则的话，无权图![s_{i,j}=1](https://private.codecogs.com/gif.latex?s_%7Bi%2Cj%7D%3D1)，加权图![s_{i,j}>0](https://private.codecogs.com/gif.latex?s_%7Bi%2Cj%7D%3E0)

### 一阶相似度(First-Order Proximity)

一阶相似其实就是指的节点的邻域节点，对于任意一对节点，如果![s_{i,j}>0](https://private.codecogs.com/gif.latex?s_%7Bi%2Cj%7D%3E0),那么节点i和j之间就存在正的一阶相似度，否则为0。

### 二阶相似度(Second-Order Proximity)

二级相似描述的是两个节点，它们的邻域节点相似的情况。令![N_u=\{s_{u,1},...,s_{u,|V|}\}](https://private.codecogs.com/gif.latex?N_u%3D%5C%7Bs_%7Bu%2C1%7D%2C...%2Cs_%7Bu%2C%7CV%7C%7D%5C%7D)表示节点u和其他所有节点的所有一阶相似度。

那么两个节点的二阶相似度就由![N_u](https://private.codecogs.com/gif.latex?N_u)和![N_v](https://private.codecogs.com/gif.latex?N_v)表示。

### 图embedding(Network Embedding)

给定一个图G=(V,E),图embedding 的目标是学习每个节点的映射函数，使得将节点映射到一个d维的低纬空间。即学习一个映射函数![f:v_i\mapsto y_i\subset R^d](https://private.codecogs.com/gif.latex?f%3Av_i%5Cmapsto%20y_i%5Csubset%20R%5Ed)，而优化的目标就是利用一阶和二阶相似度优化![y_i](https://private.codecogs.com/gif.latex?y_i)和![y_j](https://private.codecogs.com/gif.latex?y_j)的相似度。

### 模型

![](https://img-blog.csdnimg.cn/20190409224907651.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

整个模型的框架如图所示，是一个双通道的自编码器，两个通道共享全值。具体的一阶相似和二阶相似在损失函数中体现。

### 损失函数

![](https://img-blog.csdnimg.cn/20190409225639710.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

上图为文章定义的表示符号，其中带^的帽子的符号是网络重建的输出结构。

1.自编码器重建二阶相似度，即输入当前节点和其他所有节点的链接权重，没有链接则权重为0。然后用自编码器重建所有数据。那么正常的自编码器损失函数为：

![](https://img-blog.csdnimg.cn/20190409225547779.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

但是邻域链接的节点很稀疏，这样的话输入数据![Xi](https://private.codecogs.com/gif.latex?Xi),即![N_i](https://private.codecogs.com/gif.latex?N_i)的大部分维度上都是0，那样网络只要输出全0，也能达到很好的效果，所以这里损失函数对非零项做了加权。

![](https://img-blog.csdnimg.cn/20190409225958359.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

这里的⊙表示哈达马乘积（即对应元素相乘），![b_i=\{b_{i,j}\}^n_j=1](https://private.codecogs.com/gif.latex?b_i%3D%5C%7Bb_%7Bi%2Cj%7D%5C%7D%5En_j%3D1),如果![s_{i,j}=0](https://private.codecogs.com/gif.latex?s_%7Bi%2Cj%7D%3D0),即节点i和j没有边连接，那么![b_{i,j}=1](https://private.codecogs.com/gif.latex?b_%7Bi%2Cj%7D%3D1)，否则![b_{i,j}=\beta >1](https://private.codecogs.com/gif.latex?b_%7Bi%2Cj%7D%3D%5Cbeta%20%3E1)这样就起到来加权的效果。

2.损失函数引入一阶相似度限制，如果两个节点有边直接相连，那么让两个节点的embedding向量尽量的靠近，所以可以直接最小化两个embedding向量的最小二乘：

![](https://img-blog.csdnimg.cn/20190409230818653.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

这里的![y^{(K)}_i](https://private.codecogs.com/gif.latex?y%5E%7B%28K%29%7D_i)和![y^{(K)}_j](https://private.codecogs.com/gif.latex?y%5E%7B%28K%29%7D_j)就是自编码器中间编码出来的向量，也就是最后要提取出来的节点embedding。因为网络结构是双通道的，所以可以直接在损失函数中加入这项损失。

3.加入L2正则，这个就不多说了。

![](https://img-blog.csdnimg.cn/20190409231143185.png)

那么将上面的三项合并起来就可以获得最终的损失函数：

![](https://img-blog.csdnimg.cn/20190409231111722.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

然后就可以训练自编码器了，把训练好的中间向量提取出来就是最终的节点embedding啦。

完](https://blog.csdn.net/sxf1061926959/article/category/8767239)](https://blog.csdn.net/sxf1061926959/article/category/6564385)





