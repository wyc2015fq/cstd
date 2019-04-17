# 人脸对齐（二十一）--A Recurrent Encoder-Decoder Network for Sequential Face Alignment - 工作笔记 - CSDN博客





2018年08月22日 15:26:12[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5420
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









转自：[https://blog.csdn.net/shuzfan/article/details/52438910](https://blog.csdn.net/shuzfan/article/details/52438910)

本次介绍一篇关于人脸关键点检测(人脸对齐)的文章：

[《ECCV16 A Recurrent Encoder-Decoder Network for Sequential Face Alignment》](http://arxiv.org/abs/1608.05477).

作者主页：[https://sites.google.com/site/xipengcshomepage/project/face-alignment](https://sites.google.com/site/xipengcshomepage/project/face-alignment)（链接好像失效了）, 代码和预训练模型后面会放出来。

## **Introduction**

目前，采用神经网络直接进行回归的的人脸关键点检测方法越来越多。虽然性能越来越好，但还是很容易受到大的人脸姿态变化以及严重遮挡的影响。

So,作者考虑，基于**视频连续帧**构建一个 recurrent encoder-decoder DNN模型。（PS. 个人觉得基于视频会使得应用场景受限，其次是即时速度）

**encoder-decoder：**即编码阶段先做特征学习，然后解码阶段做关键点回归。

**recurrent：** 即利用视频连续帧的信息，完成由粗到细的回归。

## **Recurrent Encoder-Decoder Network**

首先给出方法的整体流程图，我们简单分析下包含哪几个模块。

![RED流程图](https://img-blog.csdn.net/20160904190039904)

## **1. fENC -编码 & fDENC -解码**

首先从整体上来看一下编解码阶段，ENC：encode，DENC：decode.

下图是文章中使用到的编解码网络，是一个全卷积网络：前半部分卷积表示编码，后半部分反卷积表示解码。作者提到使用全卷积，是因为可以得到同样尺寸的输出，这样便于进行递归操作。

![编解码全卷积网络](https://img-blog.csdn.net/20160904190945635)

由图可以看出，input是3通道彩色图，label是图样大小的单通道图(假设L个关键点，则取值{0,…L}，0表示背景)。output与input大小相同但通道数变为L+1L+1 ，即第一个通道表示背景，后续的 LL 个通道依次表示 LL 个关键点的pixel级别的置信度。之所以采用 L+1L+1 个通道是为了保留关键点的整体性。

文中作者使用的是VGG网络，不过去掉了所有的全连接并利用现有的卷积层参数初始化网络，所以模型大小只有14.8M，编码阶段最后的输出为 4×4×2564×4×256 。

此外，编码阶段使用了5个stride=2的 2×22×2 max-pooling, 为了在解码阶段可以有效地还原出原来的空间信息，作者这里为每一个激活值花费2bit来记录max-pooling的选择位置。还有一点，作者在网络中加入了BN来加速收敛（PS. 在我的实验中，对于小网络和回归问题，BN经常会带来反效果。）

## **2. fREG -回归**

REG：Regression。上面的编解码阶段最终的输出是一个 L+1L+1 通道的feature map，而我们最终需要的输出则是一组L个坐标（[x,y],共2L个数值），所以我们还需要一步非线性映射，

这里的Loss就是常见的欧氏距离：

![回归LOSS](https://img-blog.csdn.net/20160904203211731)

其中，y∗y∗ 为ground truth，所有的坐标都是减去mean shape的值。

具体的映射网络如下：

![非线性映射](https://img-blog.csdn.net/20160904205030007)

由图可知，input是 128×128×(L+1)128×128×(L+1) ，output是 2L×12L×1 。网络结构类似于编解码阶段，但规模稍小。

此外，由于输入是高度抽象的缺少细节信息，所以会影响性能。So，作者从编码阶段拿到conv2_2 和 conv4_3的特征和当前网络对应阶段的特征拼接起来，从而利用起来全局和局部的信息。

（PS. 这已经很复杂了模型已经很大了啊T_T，实际还怎么实时啊）

## **3. fsRNN - Spatial Recurrent Learning**

空间递归学习是为了实现由粗到细的关键点定位，而以前的由粗到细方法往往是级联多个网络，这里则只使用一个网络。

示意图如下：

![sRNN](https://img-blog.csdn.net/20160904222354163)

这个示意图其实描述的就是下面这样一个递归公式：

![sRNN公式](https://img-blog.csdn.net/20160904223114687)

输入是 (L+1)(L+1) 通道的response map，输出则是一个单通道的label map。具体的处理过程主要有两步：

**（1）** 将((L+1)\) 通道的response map融合为一个单通道图，该图上的数值为0,…L0,…L 。 融合的方法为选取置信度最大的label，即按照下式：

![Merge](https://img-blog.csdn.net/20160905090642171)

**（2）** 这里主要实现将(1)的结果怎样处理成一个label map，方法是：在(1)的每个关键点分布中心处划定一个小的正方形，在3次递归时举行尺寸依次是7pixels、5pixels和3pixels。

递归采用了LSTM结构，示意图如下(具体结构可参照原文)：

![tRNN-LSTM](https://img-blog.csdn.net/20160905091215388)

## **4. ftRNN - Temporal Recurrent Learning**

本文的方法中，递归学习不仅存在于之前的空间上，还有时间上的递归学习。

编码阶段，通过输入连续多帧视频，我们可以得到多个编码的特征 {Ct，t=1,…T}{Ct，t=1,…T} 。每一个特征都可以被分为两块：一块是身份码字 CidCid ，该项时不变，因为连续输入的都是同一个人；一块是姿态表情码字 CpeCpe ，该项时变，因为随着时间变化人脸的姿态和表情都是有变化的。

示意图如下：

![f_tRNN](https://img-blog.csdn.net/20160905103647588)

## **5. fCLS - Supervised Identity Disentangling**

这里讲最后一个模块，身份识别辅助模块。

前面讲到 fENCfENC 的输出为 4×4×5124×4×512 , 该输出被分割为两个4×4×2564×4×256 的特征，分别用来描述身份和姿态表情变化。

之所以添加身份识别部分，作用类似于汤晓鸥的MultiTask，据说可以收敛到更好鲁棒性也更好。(PS. 据说啊，反正我的实验里面MultiTask, 没有明显的积极作用)

识别部分的网络结构如下，使用的是分类Loss:

![这里写图片描述](https://img-blog.csdn.net/20160905105501066)

## **Result**

最后添加一个实验结果图：

(PS. 我觉得对比试验是不合理的，因为对比方法都是基于静态图像做的检测，而本文方法使用的是多帧视频。)

![Result](https://img-blog.csdn.net/20160905113826922)



