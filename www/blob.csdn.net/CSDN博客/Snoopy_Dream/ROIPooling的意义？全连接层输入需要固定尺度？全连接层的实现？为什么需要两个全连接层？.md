# ROIPooling的意义？全连接层输入需要固定尺度？全连接层的实现？为什么需要两个全连接层？ - Snoopy_Dream - CSDN博客





2018年11月01日 11:00:09[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：1199
所属专栏：[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)









![](https://img-blog.csdnimg.cn/20181101191116776.png)

ROIPooling的作用，就是resize到统一尺寸，这样才能利用预训练的全连接层参数，大多是7*7大小，

这是因为全链接层需要固定的输入尺寸.**那么为什么需要固定尺寸呢？**

全连接层的计算其实相当于输入的**特征图数据矩阵和全连接层权值矩**阵进行**内积**

*以vgg16,512*7*7的特征输入为例，紧接着两个全连接层。*

如果是caffe，这个特征图进入全连接层之后就会被**flatten**成一个25088维的向量，同时这个全连接层会初始化一个25088*4096的权值矩阵，这两个进行**矩阵乘积**运算，最后输出一个4096的一维向量。

*[1,512*7*7] * [512*7*7,4096]=[1,4096](你看这里就知道为什么要固定7*7了，**后面****全连接层的参数维度是固定的****)***



换言之：全连接的参数是随前层大小的变化而变的，如果输入图片大小不一样，那么全连接层之前的feature map也不一样，那全连接层的参数数量就不能确定， 所以必须实现固定输入图像的大小。

多个全连接层相接，是因为添加非线性。一个全连接层中一个神经元相当于一个多项式。

如果全连接层宽度不变，增加长度：

**优点**：神经元个数增加，模型复杂度提升；全连接层数加深，模型非线性表达能力提高。理论上都可以提高模型的学习能力。

如果全连接层长度不变，增加宽度：

**优点**：神经元个数增加，模型复杂度提升。理论上可以提高模型的学习能力。

难度长度和宽度都是越多越好？

肯定不是

（1）**缺点**：学习能力太好容易造成过拟合。

（2）**缺点**：运算时间增加，效率变低。

拓展：

作者：魏秀参

链接：https://www.zhihu.com/question/41037974/answer/150522307

来源：知乎


- 全连接层（fully connected layers，FC）在整个卷积神经网络中起到“分类器”的作用。如果说卷积层、池化层和激活函数层等操作是将原始数据映射到隐层特征空间的话，全连接层则起到将学到的“分布式特征表示”映射到样本标记空间的作用。在实际使用中，全连接层可由卷积操作实现：对前层是全连接的全连接层可以转化为卷积核为1x1的卷积；而前层是卷积层的全连接层可以转化为卷积核为hxw的全局卷积，h和w分别为前层卷积结果的高和宽（***注1***）。
- 目前由于全连接层参数冗余（仅全连接层参数就可占整个网络参数80%左右），近期一些性能优异的网络模型如ResNet和GoogLeNet等均用全局平均池化（global average pooling，GAP）取代FC来融合学到的深度特征，最后仍用softmax等损失函数作为网络目标函数来指导学习过程。需要指出的是，用GAP替代FC的网络通常有较好的预测性能。具体案例可参见我们在ECCV'16（视频）表象性格分析竞赛中获得冠军的做法：[「冠军之道」Apparent Personality Analysis竞赛经验分享 - 知乎专栏](https://zhuanlan.zhihu.com/p/23176872) ，project：[Deep Bimodal Regression for Apparent Personality Analysis](https://link.zhihu.com/?target=http%3A//210.28.132.67/weixs/project/APA/APA.html)
- 在FC越来越不被看好的当下，我们近期的研究（[In Defense of Fully Connected Layers in Visual Representation Transfer](https://link.zhihu.com/?target=https%3A//cs.nju.edu.cn/wujx/paper/PCM2017_FC.pdf)）发现，FC可在模型表示能力迁移过程中充当“防火墙”的作用。具体来讲，假设在ImageNet上预训练得到的模型为![\mathcal{M}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BM%7D) ，则ImageNet可视为源域（迁移学习中的source domain）。微调（fine tuning）是深度学习领域最常用的迁移学习技术。针对微调，若目标域（target domain）中的图像与源域中图像差异巨大（如相比ImageNet，目标域图像不是物体为中心的图像，而是风景照，见下图），不含FC的网络微调后的结果要差于含FC的网络。因此FC可视作模型表示能力的“防火墙”，特别是在源域与目标域差异较大的情况下，FC可保持较大的模型capacity从而保证模型表示能力的迁移。（冗余的参数并不一无是处。）
- ![](https://img-blog.csdnimg.cn/20181101190446779.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



