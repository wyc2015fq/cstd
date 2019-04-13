
# 多模态(RGB-D)——人脸识别 - 小花生的博客 - CSDN博客


2018年10月04日 21:29:19[Peanut_范](https://me.csdn.net/u013841196)阅读数：943


《RGB-D Face Recognition via Deep Complementary and Common Feature Learning》
FG 2018，Hao Zhang, Hu Han, Jiyun Cui, Shiguang Shan, Xilin Chen.
近年，利用RGB-D数据进行人脸识别的方案已经被广泛采用，然而现有方法多使用相同处理方式处理所有的模态，这没有充分考虑模态的差异，忽略了模态之间的相关性。作者提出一种新的RGBD多模态融合的人脸识别方案，能够学习多个模态之间的共同特征以及互补特征，进一步扩展多模态的学习能力。
RGB-D人脸识别由两个典型应用场景组成：
（1）multi-modality matching, e.g.,RGB-D probe vs. RGB-D gallery
（2）cross-modality matching, e.g.,RGB probe vs. RGB-D gallery
![在这里插入图片描述](https://img-blog.csdn.net/20181004205711768?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
1.针对multi-modality matching
由于RGB和depth分别描述了人脸的纹理和形状信息，这两种模态应该是相关且可以互补的，通过引入联合损失，来增强模态之间的交互。基本架构如下图所示：
![在这里插入图片描述](https://img-blog.csdn.net/20181004205807858?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
为了得到互补性的特征，同时优化各个模态的单独损失和联合损失，损失函数如下式所示：
![在这里插入图片描述](https://img-blog.csdn.net/20181004205834545?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
其中，
$$
W_{j}
$$
为单个模态学习的权重矩阵，
$$
W_{j}
$$
为联合学习的权重矩阵，
$$
y_{j}
$$
为样本的id。为了得到更好的实验结果，作者对深度图的训练样本做了预处理，处理方式如下图所示，去除了较多的噪声：
![在这里插入图片描述](https://img-blog.csdn.net/20181004210618527?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
整个网络的实现细节如下图所示：
![在这里插入图片描述](https://img-blog.csdn.net/20181004210658563?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
引入loss4，增强互补特征学习，实现细节：
![在这里插入图片描述](https://img-blog.csdn.net/20181004210723920?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后，使用RGB的FC1024特征和Depth的FC1024特征分别计算RGB-RGB的匹配得分值，Depth-Depth的匹配得分值，之后通过加权融合得到最后得分：
![在这里插入图片描述](https://img-blog.csdn.net/20181004210808215?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.针对cross-modality matching
当深度图缺失时，提出把cross-modality matching：RGB to RGB-D分成两个子问题，(i)
RGB to RGB matching,and (ii) RGB to depth matching.
(i)RGB to RGB matching
直接使用提出的上面互补特征学习的网络，来获得RGB-to-RGB的匹配得分。
(ii) RGB to depth matching
提出学习共性特征从RGB和Depth，具体如下图所示：
![在这里插入图片描述](https://img-blog.csdn.net/2018100421090494?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3.RGB-D评测数据集：
![在这里插入图片描述](https://img-blog.csdn.net/20181004211003645?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.互补特征的实验结果：
![在这里插入图片描述](https://img-blog.csdn.net/20181004211708981?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20181004211838685?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5.错误数据分析
![在这里插入图片描述](https://img-blog.csdn.net/20181004212155685?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
6.参考：
报告人：韩琥
报告：《多模态人脸识别与属性学习》
主页：[https://sites.google.com/site/huhanhomepage](https://sites.google.com/site/huhanhomepage)
概述：主要介绍了在多模态和跨模态人脸识别方向的若干研究进展，包括RGB-D多模态互补特征学习、夜间远距离人脸识别、模拟画像与照片跨模态识别和多模态融合的复杂场景人脸识别。
---
注：博众家之所长，集群英之荟萃。

