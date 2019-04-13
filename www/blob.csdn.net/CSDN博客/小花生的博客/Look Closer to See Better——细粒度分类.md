
# Look Closer to See Better——细粒度分类 - 小花生的博客 - CSDN博客


2018年08月26日 21:38:01[Peanut_范](https://me.csdn.net/u013841196)阅读数：538


《Look Closer to See Better: Recurrent Attention Convolutional Neural Network for Fine-grained Image Recognition》
CVPR2017，Jianlong Fu, Heliang Zheng, Tao Mei，细粒度分类
**Abstract：**
识别纹理细密的物体类别（比如鸟类）是很困难的，这是因为判别区域定位（discriminative region localization）和细粒度特征学习（fine-grained feature learning）是很具有挑战性的。现有方法主要都是单独地来解决这些挑战性问题，然而却忽略了区域检测（region detection）和细粒度特征学习（fine-grained feature learning）之间的相互关联性，而且它们可以互相强化。
本文中，提出了一个全新的循环注意力卷积神经网络（recurrent attention convolutional neural network——**RA-CNN**），用互相强化的方式对判别区域注意力（discriminative region attention）和基于区域的特征表征（region-based feature representation）进行递归学习。在每一尺度规模（scale）上进行的学习都包含一个分类子网络（classification sub-network）和一个注意力建议子网络（attention proposal sub-network——APN）。APN 从完整图像开始，通过把先期预测作为参考，由粗到细迭代地生成区域注意力（region attention），同时精调器尺度网络（finer scale network）以循环的方式从先前的尺度规格输入一个放大的注意区域（amplified attended region）。
RA-CNN 通过尺度内分类损失（intra-scale classification loss）和尺度间排序损失（inter-scale ranking loss）进行优化，以相互学习精准的区域注意力（region attention）和细粒度表征（fine-grained representation）。RA-CNN 并不需要边界框（bounding box）或边界部分的标注（part annotations），而且可以进行端到端的训练。
**Introduction：**
识别细粒度物体这一类任务往往是极具挑战性的，这是因为一些纹理细密的物体种类只能被该领域的专家所识别出来。与一般的识别不同，细粒度图像识别（fine-grained image recognition）是应该能够进行局部定位（localizing），并且能在其从属（subordinate）类别中表征很小的视觉差异的，从而使各种应用受益，比如专家级的图像识别、图像标注等等。
![](https://img-blog.csdn.net/20180826212808805?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图 1. 两种啄木鸟。我们可以从高度局部区域（highly local regions），比如黄色框里的头部，观察到非常不易察觉的视觉差异，这是难以在原始图像规格中进行学习的。然而，如果我们可以学着去把注意区域放大到一个精细的尺度，差异可能就会更加生动和显著。
如图1所示，精确的头部定位可以促进学习辨别头部特征，这进一步帮助精确定位后脑中存在的不同颜色。
提出的RA-CNN是一个叠加网络（stacked network），其输入为从全图像到多尺度的细粒度局部区域（fine-grained local regions）。在网络结构设计上主要包含3个scale子网络，每个scale子网络的网络结构都是一样的，只是网络参数不一样，在每个scale子网络中包含两种类型的网络：分类网络和APN网络。因此数据流是这样的：输入图像通过分类网络提取特征并进行分类，然后APN网络基于提取到的特征进行训练得到attention region信息，再将attention region剪裁（crop）出来并放大（zoom in），再作为第二个scale网络的输入，这样重复进行3次就能得到3个scale网络的输出结果，通过融合不同scale网络的结果能达到更好的效果。
**Related Work：**
关于细粒度图像识别的研究沿着两个维度进行:
1. Discriminative Feature Learning
依赖于强大的卷积深层特征
2. Sophisticated Part Localization
使用无监督的方法来挖掘注意力区域
放大判别性的局部区域,以提高细粒度识别性能.
**Approach：**
![这里写图片描述](https://img-blog.csdn.net/2018082621314584?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/2018082621314584?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
图 2. RA-CNN框架。
输入图像从上到下按粗糙的完整大小的图像到精炼后的区域注意力图像排列。不同的网络分类模块（蓝色部分）通过同一尺度的标注预测 Y(s) 和真实 Y∗之间的分类损失 Lcl 进行优化，注意力建议（红色部分）通过相邻尺度的 p (s) t 和 p (s+1) t 之间的成对排序损失 Lrank（pairwise ranking loss Lrank）进行优化。其中 p (s) t 和 p (s+1) t 表示预测在正确类别的概率，s 代表尺度。APN 是注意力建议网络，fc 代表全连接层，softmax 层通过 fc 层与类别条目（category entry）匹配，然后进行 softmax 操作。+代表「剪裁（crop）」和「放大（zoom in）」。
**每个scale网络是有两个输出的multi-task结构:**
**1. 分类**
p(X) = f(Wc* X)
Wc: (b1)或(b2)或(b3)网络的参数,也就是一些卷积层、池化层和激活层的集合,用来从输入图像中提取特征.
Wc* X: 就是最后提取到的特征.
f()函数: 就是fully-connected层和softmax层,用来将学习到的特征映射成类别概率,也就是p(X).
**2. 区域检测**
[tx, ty, tl] = g(Wc* X)
这里假设检测出来的区域都是正方形,即tx和ty表示区域的中心点坐标,tl表示正方形区域边长的一半.
g()函数: 也就是APN网络,可以用两个fully-connected层实现,其中最后一个fully-connected层的输出channel是3,分别对应tx、ty、tl。
**Loss:**
a. intra-scale classification loss
b. inter-scale pairwise ranking loss​
![](https://img-blog.csdn.net/20180826213332432?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180826213332432?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
一部分是Lcls,也就是classification loss.
Y(s)表示预测的类别概率,Y*表示真实类别.
pairwise ranking loss:​
![](https://img-blog.csdn.net/20180826213419809?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180826213419809?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
pt(s): prediction probability on the correct category labels t.
从Lrank损失函数可以看出,当更后面的scale网络的pt大于相邻的前面的scale网络的pt时,损失较小.
通俗讲模型的训练目标是希望更后面的scale网络的预测更准.
于是这样的网络就可以得到输入图像X的不同scale特征,用{F1, F2, … FN}表示.
N: scale的数量
Fi: 第i个scale的分类子网络全连接层输出,文中称Fi为descriptor.
融合不同scale网络的输出结果:
把每个分类子网络的最后的全连接层堆叠起来,然后将它们连接到一个全连接层,随后通过softmax层,进行分类.
**Training strategy:**
a. 初始化分类子网络: 用预训练的VGG-Net初始化分类子网络中卷积层和全连接层的参数;
b. 初始化APN: 查找分类子网络的最后一层卷积层(conv5_4 in VGG-19)具有最高响应值(highest response)的区域,用该区域的中心点坐标和原图边长的一半来初始化(tx,ty,tl);
c. 固定APN的参数,训练分类子网络直至Lcls收敛; 随后固定分类子网络的参数,训练APN网络直至Lrank收敛.这个训练过程是迭代交替进行的,直到两个网络的损失收敛.
---
**Experiments**
**Datasets:**
Caltech-UCSD Birds (CUB-200-2011)
Stanford Dogs
Stanford Cars
![这里写图片描述](https://img-blog.csdn.net/20180826213623212?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180826213623212?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Baselines:**
采用bounding box/part annotation标注的监督式训练和不采用bounding box标注的无监督式训练这两种不同的attention localization算法。
输入图像尺寸方面：
Input images (at scale 1) and attended regions (at scale 2,3) are resized to 448×448 and 224×224 pixels respectively in training, due to the smaller object size in the coarse scale.
**Conclusion：**
Propose a recurrent attention convolutional neural network(RA-CNN) for fine-grained recognition, which recursively learns discriminative region attention and region-based feature representation at multiple scales.
RA-CNN does not need bounding box/part annotations for training and can be trained end-to-end.
In the future:
1. How to simultaneously preserve global image structure and model local visual cues, to keep improving the performance at finer scales;
2. How to integrate multiple region attention to model more complex fine-grained categories.
**参考：**[https://zhuanlan.zhihu.com/p/42067661](https://zhuanlan.zhihu.com/p/42067661)
---

###### 注：博众家之所长，集群英之荟萃。

