# Hard-Aware Point-to-Set Deep Metric for Person Re-identification 论文笔记 - wxf19940618的博客 - CSDN博客





2018年11月06日 17:39:37[rxdffz](https://me.csdn.net/wxf19940618)阅读数：148








一、提出问题

度量学习是行人重识别任务中的重要组成部分之一，但是度量损失的性能受采样方法的显着影响，传统的采样方法极大地限制了深度度量学习的性能。

二、论文贡献
- 提出了一种新的度量损失，即硬感知点集（HAP2S）损失，该损失涉及自适应困难挖掘机制；
- 在Market-1501，CUHK03和DukeMTMC-reID 三个数据集上进行测试，HAP2S损失达到了SOTA。

三、模型结构

![](https://img-blog.csdnimg.cn/20181106173911934.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-1 模型结构

3.1 概况

深度度量度学习的目的是学习一个将图像x映射到对应的特征表示![](https://img-blog.csdnimg.cn/20181106173911882.png)的深度神经网络，其适合于预定义的度量，在Θ中包含网络的参数权重和偏差。 对于行人重识别，可以通过训练好的深度提取探测器提取probe和gallery图像的特征，然后计算两者特征之间的距离并获得距离排序列表。在其中度量损失的作用是为监督网络训练提供判别度量。

       模型结构如图3-1所示，采用预训练的CNN模型作为骨干网络，将每个行人图像转换为特征嵌入，本文选用的骨干网络是ResNet-50模型，它由五个下采样块和一个全局平均池化层组成。骨干网络之后是两个全连接层，分别具有1024和128个神经元。在骨干网和两个FC层之后，利用网络提取的输出特征计算小批量中的HAP2S损失，用欧式距离作为点对点（P2P）的度量。

3.2 回顾Triplet Loss

       设一个三元组![](https://img-blog.csdnimg.cn/20181106173911921.png)，![](https://img-blog.csdnimg.cn/20181106173911923.png)是锚点，![](https://img-blog.csdnimg.cn/20181106173911927.png)是正样本，![](https://img-blog.csdnimg.cn/20181106173911932.png)是负样本，三者的特征可表示为![](https://img-blog.csdnimg.cn/20181106173911946.png)![](https://img-blog.csdnimg.cn/201811061739121.png)，则Triplet Loss可表示为：

![](https://img-blog.csdnimg.cn/2018110617391220.png)



在训练具有Triplet Loss的CNN时，多数候选三元组很容易满足约束条件：

![](https://img-blog.csdnimg.cn/2018110617391222.png)

这就使得这些选定的三元组对训练没有贡献，因此，困难样本的挖掘对Triplet Loss至关重要。赫尔曼斯等人提出了一种Triplet Loss的变体，它采用简单而强大的硬挖掘方案，定义为：

![](https://img-blog.csdnimg.cn/2018110617391230.png)

选择小批量中每个锚点的最难正样本和最难的负样来构成三元组。

3.3 硬感知的P2S loss
- Triplet Loss是一种P2P的loss
- 这种简单的硬挖掘策略带来两个问题
- 排除了其他困难样本在梯度下降训练中的贡献;
- 容易受到最难样本异常值的影响，最难的样本若出现异常则会导致错误的反向传播
- 以上两个问题说明简单的硬挖掘策略的健壮性存在问题



本文提出P2S(point-to-set) loss，设锚点的标签为![](https://img-blog.csdnimg.cn/2018110617391231.png)，![](https://img-blog.csdnimg.cn/2018110617391229.png)是mini batch内的正样本集，相应的，![](https://img-blog.csdnimg.cn/2018110617391296.png)![](https://img-blog.csdnimg.cn/20181106173912114.png)是负样本集，P2S loss定义为：

![](https://img-blog.csdnimg.cn/20181106173912125.png)

       其中D代表P2S距离，P2S Triplet Loss是一种更普通的形式，如果P2S距离定义为以下形式，则可以将P2S Triplet Loss转换成方程（3）中的P2P Triplet Loss。

![](https://img-blog.csdnimg.cn/20181106173912118.png)

换句话说，从P2S loss的角度来看，方程（3）中的Triplet Loss是P2S loss的一种特殊形式，只选择最难的样本来表示整个集合。

       本文提出的HAP2S loss与方程（4）具有相同的形式，HAP2S loss的关键是通过计算P2S距离为每组中的点分配不同的权重，HAP2S距离定义为：

![](https://img-blog.csdnimg.cn/20181106173912138.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

       其中![](https://img-blog.csdnimg.cn/20181106173912172.png)和![](https://img-blog.csdnimg.cn/20181106173912206.png)分别表示正样本集合和负样本集合中的元素![](https://img-blog.csdnimg.cn/20181106173912185.png)和![](https://img-blog.csdnimg.cn/20181106173912190.png)的权重。有效的硬挖掘策略应该为一组中的更困难的样本分配更高的权重，考虑到度量损失，样本的“难度级别”应该根据该店到锚点的距离。因此，对于正样本集，锚点的远点是困难点，应赋予更高的权重，而对于负样本集，最靠近锚点的点是最困难的。基于此，为HAP2S损失引入了两种加权方案。
- **指数加权。**每组中元素的权重定义为

![](https://img-blog.csdnimg.cn/20181106173912197.png)

其中σ> 0是用于调整权重分布的系数。每个样本的权重以指数方式适应其“难度级别”。 HAP2S loss![](https://img-blog.csdnimg.cn/20181106173912210.png)的完整公式由方程（4），方程（6）和方程（7）组成。
- **多项式加权。**通过具有实数系数的单变量多项式函数为每个集合中的元素赋予权重，定义HAP2S损失：

![](https://img-blog.csdnimg.cn/20181106173912246.png)

其中α> 0也是用于调整重量分布的系数。![](https://img-blog.csdnimg.cn/20181106173912272.png)包括方程（4），方程（6）和方程（8）。

为了区分两种加权方式，将前者表示为![](https://img-blog.csdnimg.cn/20181106173912277.png)，后者表示为![](https://img-blog.csdnimg.cn/20181106173912274.png)

3.4 多损失训练

由于各种损失建立了不同的优化目标，因此对不同损失的联合监督通常有助于训练更好的深度行人重识别模型。本文用HAP2S loss与softmax loss联合训练以获得更好的性能。



四、实验结果

数据集：Market-1501，CUHK03，DukeMTMC-reID

各种loss结果对比

![](https://img-blog.csdnimg.cn/20181106173912332.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

各种triplet loss对比

![](https://img-blog.csdnimg.cn/20181106173912335.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

与其他算法对比

![](https://img-blog.csdnimg.cn/20181106173912354.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)



