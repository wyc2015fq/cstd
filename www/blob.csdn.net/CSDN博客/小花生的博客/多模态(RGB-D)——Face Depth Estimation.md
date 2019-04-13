
# 多模态(RGB-D)——Face Depth Estimation - 小花生的博客 - CSDN博客


2018年10月04日 22:23:26[Peanut_范](https://me.csdn.net/u013841196)阅读数：338


《Improving 2D Face Recognition via Discriminative Face Depth Estimation》
ICB 2018，Jiyun Cui, Hao Zhang, Hu Han, Shiguang Shan, and Xilin Chen.
1.引言：
1）.RGB-D人脸识别无法避免的问题：模态缺失或不可用
![在这里插入图片描述](https://img-blog.csdn.net/20181004213250322?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2）.可能的解决方法：深度图像生成
![在这里插入图片描述](https://img-blog.csdn.net/20181004213337364?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3）.可能采用的方法：
![在这里插入图片描述](https://img-blog.csdn.net/20181004213428160?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考：
E. Shelhamer, J. Long, T. Darrell. Fully Convolutional Networks for Semantic Segmentation. TPAMI, 2017.
Du et al, Articulated Multi-Instrument 2D Pose Estimation Using Fully Convolutional Networks, TMI, 2017.
2.正文：
本文作者提出了一种具有可区分性的人脸深度估计方法来提升2维人脸识别的准确率，
网络结构较为简单，通过FCN从RGB中重建出depth，之后用估计出的depth送入CNN中进行判别，迫使估计出的depth具有可区分性。最后识别效果RGB+estimated depth要好于单纯的RGB。其网络结构如下：
![在这里插入图片描述](https://img-blog.csdn.net/20181004213556243?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
上面是FCN网络：
[
](https://img-blog.csdn.net/20181004213556243?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)包含8 conv. 和3 deconv. Layers
输入图像尺寸：2242243
输出图像尺寸：2242241
损失函数：L2 loss
[
](https://img-blog.csdn.net/20181004213556243?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)下面是CNN网络：
[
](https://img-blog.csdn.net/20181004213556243?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)VGG-11 network，8 conv. 和 3 FC Layers
损失函数： softmax
[
](https://img-blog.csdn.net/20181004213556243?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)整体损失函数：
![在这里插入图片描述](https://img-blog.csdn.net/20181004214434811?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所生成的深度Z，既要和真实深度相比具有较小的误差，又要保持不同人的区分度。
注意：由于L2loss远远大于softmaxloss，在训练时会将L2loss乘以0.001。
3.数据预处理：
![在这里插入图片描述](https://img-blog.csdn.net/20181004214624847?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4.实验结果：
评估标准：（1）估计出的深度图用MAE作为评估指标，（2）人脸识别准确率
（1）深度生成结果的可视化与MAE误差
![在这里插入图片描述](https://img-blog.csdn.net/20181004220657383?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20181004220723198?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
（2）将所生成的深度用于RGB-D人脸识别
![在这里插入图片描述](https://img-blog.csdn.net/20181004220755400?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5.参考：
报告人：韩琥
报告：《多模态人脸识别与属性学习》
主页：[https://sites.google.com/site/huhanhomepage](https://sites.google.com/site/huhanhomepage)
概述：主要介绍了在多模态和跨模态人脸识别方向的若干研究进展，包括RGB-D多模态互补特征学习、夜间远距离人脸识别、模拟画像与照片跨模态识别和多模态融合的复杂场景人脸识别。
---
注：博众家之所长，集群英之荟萃。

