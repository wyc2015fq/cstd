# Pose-Normalized Image Generation for Person Re-identification 论文笔记 - wxf19940618的博客 - CSDN博客





2018年11月13日 09:06:16[rxdffz](https://me.csdn.net/wxf19940618)阅读数：203








一、提出问题

现实情况中在跨视角摄像机下，同一行人的姿态是多变的，而现有方法缺乏在大的姿势变化时跨视角的训练数据对及在这种情况下差异性特征和不变性特征的学习。

![](https://img-blog.csdnimg.cn/20181113090553706.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

二、论文贡献
- 作者认为姿势的多变性是使深度re-id模型学习不到有效身份特征和视角不敏感特征的最重要原因，本文提出一种基于生成姿态归一化图像的解决方案，解决了现有模型可扩展性和泛化性弱的问题；
- 提出了一种行人图像生成模型PN-GAN，用于生成逼真的、身份一致的、姿态可控的姿态归一化图像。利用规范姿势的合成图像，学习互补的特征并与用原始图像学习的特征相结合，在几种标准数据集上本文的方法达到了SOTA；
- 本文考虑了一种更为现实的无监督迁移学习。目标数据集中没有数据用于模型更新，即从标记源域训练的模型无需任何修改就可以应用于目标域。

三、模型结构

3.1 问题定义及概述

**问题定义**：设训练集为包含N个行人的数据集![](https://img-blog.csdnimg.cn/20181113090553527.png)![](https://img-blog.csdnimg.cn/20181113090553736.png)，![](https://img-blog.csdnimg.cn/20181113090553737.png)和![](https://img-blog.csdnimg.cn/20181113090553738.png)为第k个行人的图像与id，在训练阶段，图像I经过特征提取函数![](https://img-blog.csdnimg.cn/20181113090553741.png)提取到的特征为![](https://img-blog.csdnimg.cn/20181113090553740.png)，而在测试阶段，需要判断测试集![](https://img-blog.csdnimg.cn/20181113090554165.png)中的行人的图像对![](https://img-blog.csdnimg.cn/20181113090554163.png)是否为同一身份，即![](https://img-blog.csdnimg.cn/20181113090554162.png)或者![](https://img-blog.csdnimg.cn/20181113090554163.png)，这一过程是通过计算![](https://img-blog.csdnimg.cn/20181113090554165.png)和![](https://img-blog.csdnimg.cn/20181113090554167.png)之间的欧氏距离相似性度量来判断的。

**框架概述**：如图3-1所示，模型框架有两个关键组件，即基于GAN的行人图像生成模型和行人重识别特征学习模型。

![](https://img-blog.csdnimg.cn/20181113090554873.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-1 模型框架

3.2 深度图像生成器

目的：产生同一行人不同姿态的图像

输入：行人图像![](https://img-blog.csdnimg.cn/20181113090554282.png)和期望的姿态图像![](https://img-blog.csdnimg.cn/20181113090554282.png)

输出：合成图像![](https://img-blog.csdnimg.cn/20181113090554279.png)

模型：图像生成器有两个组件，即生成器![](https://img-blog.csdnimg.cn/20181113090554456.png)和判别器![](https://img-blog.csdnimg.cn/20181113090554452.png)。生成器生成给定姿势的行人图像; 判别器从生成的样本中区分出真实的数据样本。

![](https://img-blog.csdnimg.cn/20181113090555138.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-2 PN-GAN模型

**姿态估计器****:**

输入：行人图像![](https://img-blog.csdnimg.cn/20181113090554622.png)

输出：姿势图像![](https://img-blog.csdnimg.cn/20181113090554622.png)（姿态骨架图）

模型：预训练的现成模型，本文使用了姿势检测工具包 – OpenPose，该工具包在不需要重新进行训练，仅给定输入行人图像![](https://img-blog.csdnimg.cn/201811130905557.png)，姿势估计器可以产生姿势图像![](https://img-blog.csdnimg.cn/201811130905559.png)，包含其定位和检测18个关节点及它们的连接。本文主要关注姿势归一化，所以选用八个规范姿势，如图3-3（a）所示。

![](https://img-blog.csdnimg.cn/20181113090555692.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-3 8种规范姿势及其聚类结果

**生成器：**

输入：行人图像![](https://img-blog.csdnimg.cn/2018111309055511.png)和由![](https://img-blog.csdnimg.cn/20181113090555268.png)产生的姿态图像![](https://img-blog.csdnimg.cn/20181113090555484.png)，其中与![](https://img-blog.csdnimg.cn/20181113090555520.png)姿势不同但身份相同

输出：合成图像![](https://img-blog.csdnimg.cn/20181113090555568.png)

模型：生成器是基于ResNet的encoder-decoder网络，encoder-decoder网络首先将![](https://img-blog.csdnimg.cn/20181113090555689.png)下采样到bottleneck层，然后反转该过程以生成![](https://img-blog.csdnimg.cn/20181113090555711.png)。 encoder包含9个ResNet基本模块。

模型设计目的：通过学习残差信息来生成新图像。 “ResNet”的一般公式是y = f（x）+ x，它可用于将不变信息从编码器的底层传递到解码器，并改变姿势变化信息。通过这种架构，编码器-解码器网络可以帮助学习提取存储在瓶颈层中的语义信息，而ResNet块可以传递丰富的行人身份信息来帮助合成更逼真的图像，改变姿势变化信息并实现姿态归一化。

由GP（·）为由编码器子网GEnc（·）和解码器子网GDec（·）组成的生成器网络的目标函数为：

![](https://img-blog.csdnimg.cn/20181113090555783.png)

![](https://img-blog.csdnimg.cn/20181113090555818.png)

![](https://img-blog.csdnimg.cn/20181113090555818.png)和![](https://img-blog.csdnimg.cn/20181113090555906.png)是来自具有身体姿势![](https://img-blog.csdnimg.cn/20181113090555908.png)的输入图像![](https://img-blog.csdnimg.cn/20181113090555906.png)与![](https://img-blog.csdnimg.cn/20181113090555910.png)的重建图像。 这里使用L1范数来产生更清晰，更清晰的图像。 λ1是平衡每个术语重要性的加权系数

**判别器：**

目的：区分输入图像是真实图像还是合成图像

给定输入图像![](https://img-blog.csdnimg.cn/20181113090556106.png)和目标输出图像![](https://img-blog.csdnimg.cn/2018111309055699.png)，鉴别器网络的目标可以表示为：

![](https://img-blog.csdnimg.cn/20181113090556106.png)

3.3 姿势规范化的行人重识别

如图3-1所示，在整个框架中训练了两个重识别模型，一个模型使用训练集中的原始图像进行训练，用来在姿势变化的情况下提取不变身份特征；另一个模型使用PN-GAN生成的具有标准化姿势的合成图像进行训练，用来计算摒除姿势变化的重识别特征（即对姿势变化不敏感的特征），最后将两个模型输出的特征融合为最终的特征表示。

**姿势规范化**：由于本文关注姿势归一化，所以需要获得一套规范姿势，这些姿势代表了监控摄像机捕获的公众所展示的典型视点和身体结构。为此，作者预测数据集中所有训练图像的姿势，然后将姿势分组为八个群集![](https://img-blog.csdnimg.cn/2018111309055699.png)。 具体地，使用在ImageNet ILSVRC-2012数据集上预训练的VGG-19模型提取每个姿势图像的特征，并且使用K-means算法将训练姿势图像聚类成规范姿势，然后将这些聚类的平均姿势图像用作规范姿势。在Market-1501上获得的八个姿势如图3-3（a）所示。通过这些姿势，给定每个图像![](https://img-blog.csdnimg.cn/20181113090556100.png)，我们的生成器将通过用这些姿势替换原始姿势来合成八个图像![](https://img-blog.csdnimg.cn/20181113090556139.png)。

**姿势变化下的重识别特征**：本文使用原始训练图像训练一个re-id模型，提取具有姿势变化的重识别特征。ResNet-50模型用作基础网络，将它在ILSVRC-2012数据集上进行预训练，并对用给定的行人重识别数据集进行了微调，用来以对训练数据进行分类。将该网络命名为ResNet-50-A（基础网络A），如图3-1所示。 给定输入图像![](https://img-blog.csdnimg.cn/20181113090556534.png)，ResNet-50-A产生特征集![](https://img-blog.csdnimg.cn/20181113090556587.png)，其中layer指示从哪个网络层提取重识别特征，将ResNet-50结构的5a，5b和5c卷积层合并到FC层之后的1024-d特征向量中。

**没有姿势变化的重识别特征**：第二个模型ResNet-50-B与ResNet-50-A具有相同的架构，区别是使用姿势标准化的合成图像进行特征学习，因此，八个姿势可获得八组特征![](https://img-blog.csdnimg.cn/20181113090556705.png)。

**测试阶段：**在测试期间，对于每个gallery图像，输入到ResNet-50-A获得一个特征向量；对于合成的具有规范姿势的八个图像，将它们输入ResNet-50-B以获得8个无姿势特征和一个额外的FC层，用于融合原始特征和每个姿势特征，这部分工作可以离线完成。给出查询图像![](https://img-blog.csdnimg.cn/20181113090556707.png)，重复以上工作获得九个特征向量![](https://img-blog.csdnimg.cn/20181113090556666.png)，利用元素最大化计算融合九个特征向量来获得一个最终特征向量。最后，计算查询的最终特征向量与gallery图像之间的欧氏距离，并使用距离对gallery图像进行排名

四、实验结果

数据集：Market-1501、CUHK03、DukeMTMC-reID、CUHK01

结果：

![](https://img-blog.csdnimg.cn/20181113090556909.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181113090556843.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2018111309055723.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181113090557285.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)



