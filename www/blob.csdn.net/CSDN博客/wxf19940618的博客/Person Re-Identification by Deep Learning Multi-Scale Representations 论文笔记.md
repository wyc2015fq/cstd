# Person Re-Identification by Deep Learning Multi-Scale Representations 论文笔记 - wxf19940618的博客 - CSDN博客





2018年12月13日 14:50:22[rxdffz](https://me.csdn.net/wxf19940618)阅读数：23








**Person Re-Identification by Deep Learning Multi-Scale Representations ****论文笔记**

一、提出问题

现有的re-id方法主要依赖于单一尺度的外观信息，但是这不仅会忽略了其他不同尺度的潜在有用的显性信息，而且还失去了跨尺度挖掘隐式相关互补的优势。

![](https://img-blog.csdnimg.cn/20181213144858691.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)
- 在不同尺度上的特征学习可能不同甚至相互不一致，因此多尺度的直接特征连接不可能导致最优特征融合;
- 不同特征金字塔等级之间的任何互补关系是未知的，并且对于不同的图像可能不是恒定的，因此必须在数据之间协同地学习和优化;
- 行人在开放式监视场景中可以以任意比例(未知)出现。

二、论文贡献
- 研究了re-id的多尺度特征学习问题；
- 提出一种新颖的深层金字塔特征学习（DPFL）设计，不仅通过同时优化同一人标签信息上的多个分类损失函数来学习规模特定的判别特征，而且还通过闭环形式的多尺度一致正则化来最大化联合多尺度互补融合特征选择。该设计通过金字塔中的层间特征交互克服了跨尺度特征学习差异，同时在小批量训练迭代中实现累积的多尺度互补特征选择。

三、多尺度行人重识别

3. 1 问题描述

目标：在没有任何度量变换的情况下学习基于通用距离（例如，L1，L2）的行人重识别的深度特征表示模型。

训练集![](https://img-blog.csdnimg.cn/20181213144858689.png)，标签![](https://img-blog.csdnimg.cn/20181213144858689.png)，同一个人的不同图像![](https://img-blog.csdnimg.cn/20181213144858688.png)，![](https://img-blog.csdnimg.cn/20181213144858692.png)

3. 2 DPFL模型

![](https://img-blog.csdnimg.cn/20181213144858749.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-1 模型结构

DPFL模型的整体网络设计如图3-1所示，DPFL模型具有（m + 1）个前馈子网络分支：

（1）m个分支：每个分支是具有相同结构的特定规模子网络，用于学习多张边框图像构成的单人金字塔尺度的最具辨别力的视觉特征;

（2）一个融合分支：学习有辨别力的特征选择和同一张图像的m个尺度特定表示的最优整合特征。

目标：优化每个尺度的判别特征表示，同时确定金字塔中不同尺度特征选择之间的相关互补组合

DPFL模型=单尺度特征学习+多尺度共识学习+共识传播的特征正则化

3. 2. 1 单尺度特征学习

使用42层Inception-V3 CNN架构设计构建尺度特定的分支，对于单尺度模型训练，利用**Softmax****分类损失函数**来优化行人身份识别，给出从成对标记的re-ID数据集中提取的多行人的训练标签。预测训练图像![](https://img-blog.csdnimg.cn/20181213144858739.png)在给定身份标签![](https://img-blog.csdnimg.cn/20181213144858731.png)上的后验概率![](https://img-blog.csdnimg.cn/20181213144858734.png)：

![](https://img-blog.csdnimg.cn/20181213144858759.png)

一批次![](https://img-blog.csdnimg.cn/20181213144858757.png)图像的每个尺度模型训练损失计算如下：

![](https://img-blog.csdnimg.cn/20181213144858773.png)

损失函数：选择分类损失而不是更常见的成对或三重损失函数是因为：（i）显着简化的训练数据批量构造（ii）视觉心理物理学研究结果表明，针对分类任务优化的表示能很好地泛化新的类别，而re-id任务是关于模型识别到未训练过的测试行人身份。 DPFL模型学习利用了一般分类学习原理，结果超出了大多数现有re-id模型中严格的成对相对验证损失。

3. 2. 2 多尺度特征学习

首先进行跨尺度的特征融合，在Inception-V3的DPFL模型中，经过平均池化->向量联接 ->dropout后，在最顶层的卷积层实现特征融合，特征大小为c*c*2048，c与输入图像分辨率成比例。同样利用Softmax分类损失（公式（2））进行共识分类学习。

3. 2. 3 共识传播的特征正则化

本文提出对特定尺寸进行正则化，由此整个特征学习形成了一个闭环。具体地，模型将共识作为额外的反馈信息进行传播，同时对所有特定尺寸分支的批量学习进行正则化。通过逐点概率预测![](https://img-blog.csdnimg.cn/20181213144858760.png)，其元素定义为：

![](https://img-blog.csdnimg.cn/20181213144858780.png)

利用共识概率![](https://img-blog.csdnimg.cn/20181213144858768.png)作为教师信号（称为“软目标”而不是真实的“硬目标”）来同时引导所有尺寸特定的分支（学生）的学习过程。 在公式(2)中强制执行正则化：

![](https://img-blog.csdnimg.cn/20181213144858778.png)

超参数λ权衡两项之间的重要性。![](https://img-blog.csdnimg.cn/20181213144858778.png)定义相应的特定尺度分支的所有nid身份类的概率预测(公式(1))。![](https://img-blog.csdnimg.cn/20181213144858777.png)是共识正则化项，表示两个分布![](https://img-blog.csdnimg.cn/20181213144858783.png)和![](https://img-blog.csdnimg.cn/20181213144858788.png)之间的交叉熵，即:

![](https://img-blog.csdnimg.cn/20181213144858800.png)

 “软目标”和“硬目标”同样对每个特定规模分支的学习过程都会有贡献。

3. 3 模型优化

使用SGD算法反向传播计算每个分支loss的梯度。由于DPFL中的所有分支都以闭环形式进行交互和关联，因此需要按顺序进行优化。Alg1中展示了整个DPFL优化过程。

![](https://img-blog.csdnimg.cn/20181213144858834.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

3. 4 行人重识别的多尺度DPFL特征

在训练DPFL模型之后，需要对行人重识别进行多尺度融合的特征表示。具体地，给定来自一个摄像机视图的测试探测图像![](https://img-blog.csdnimg.cn/20181213144858833.png)和来自其他非重叠摄像机视图的一组测试图像图像![](https://img-blog.csdnimg.cn/20181213144858830.png)：（1）通过将多尺度图像送入训练好的模型来计算它们对应的融合特征向量，表示为![](https://img-blog.csdnimg.cn/20181213144858832.png)和![](https://img-blog.csdnimg.cn/20181213144858831.png)。（2）通过通用匹配度量来计算![](https://img-blog.csdnimg.cn/20181213144858847.png)和![](https://img-blog.csdnimg.cn/20181213144858846.png)之间的跨摄像头的匹配距离，例如L2距离。（3）通过它们与探测图像的匹配距离按升序排列所有图库图像。

四、实验结果

数据集：Market-1501，DukeMTMC-reID，CUHK03

![](https://img-blog.csdnimg.cn/20181213144858898.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181213144858902.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181213144858907.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181213144858896.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)



