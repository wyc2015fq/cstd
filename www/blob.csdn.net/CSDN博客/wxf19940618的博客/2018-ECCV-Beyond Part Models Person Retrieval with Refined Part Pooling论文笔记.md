# 2018-ECCV-Beyond Part Models:Person Retrieval with Refined Part Pooling论文笔记 - wxf19940618的博客 - CSDN博客





2019年01月04日 15:14:32[rxdffz](https://me.csdn.net/wxf19940618)阅读数：24








**2018-ECCV-Beyond Part Models:Person Retrieval with Refined Part Pooling论文笔记**

一、提出问题

本文主要研究基于分块特征的re-id方法，目前的基于分块特征的方法性能较好，但是一方面这些方法利用了额外的信息(pose estimation等)，另一方面因为目标域与源域的差异性，这些方法产生局部块的模型精度相对不高而且容易引入噪声影响最终re-id的性能，所以如何在不使用额外的线索的情况下利用分块特征提高re-id精度是本文研究的问题。

二、论文贡献
- 提出了Part-based Convolution Baseline(PCB)通过对特征图进行均等划分来学习分块特征（不需要额外线索）；
- 为了提高划分区域的内容一致性，本文提出了adaptive pooling method来精修均等划分的区域，通过该方法在Market1501上超过了大多数方法。

三、模型结构

3.1 PCB结构

![](https://img-blog.csdnimg.cn/20190104151220580.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-1 PCB结构图


- 去掉Resnet50 的全局平均池化层及以后的部分；
- 将最后一层feature map 分成 p个水平条。分别对p个水平条做全局平均池化就得到了p个局部特征![](https://img-blog.csdnimg.cn/20190104151220556.png)；
- 因为 Resnet50 最后一层feature map的通道数为2048，作者又用1x1 conv将其降到256维；
- 接着用p个n（训练集ID数目）分类softmax作为分类器进行训练。损失函数使用交叉熵损失；
- 测试时分别串联向量g和h作为行人图像的特征表示![](https://img-blog.csdnimg.cn/20190104151340855.png)。

重要参数：
- 输入大小[H,W]：384 x 128，高宽比为3:1；
- T的空间大小：等同下采样率，较低的下采样率能带来精度上的提升，本文移除了backbone最后一个空间下采样操作，最终 T的大小为24 x 8；
- T被均等划分为6个水平条。

3.2 Refined Part Pooling （RPP）精修零件池

![](https://img-blog.csdnimg.cn/20190104151220617.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-2 真实边界和均分边界不一致的可视化图

如图3-2统一分块存在的内部不一致现象，所以作者提出RPP作为补救措施，以加强内部的一致性。
- 作者在最后一层 feature map 后面训练了一个部分分类器。部分分类器使用的是线性层 + softmax，参数记为 W。
- 接着将平均池化改为向量的加权和，权值即分类器的后验概率。

![](https://img-blog.csdnimg.cn/20190104151220585.png)

RPP的训练：RPP 只有一项参数 W，训练分三步：
- 训练 PCB 至收敛；
- 将平均池化替换为部分分类；
- 固定其它参数训练 W 至收敛；
- 放开全部参数，fine tune。



四、实验结果
- 数据集：Market-1501，DuckMTMC-reID，CUHK03

![](https://img-blog.csdnimg.cn/20190104151220622.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图 4-1 在Market-1501，DuckMTMC-reID，CUHK03数据集上的实验结果







