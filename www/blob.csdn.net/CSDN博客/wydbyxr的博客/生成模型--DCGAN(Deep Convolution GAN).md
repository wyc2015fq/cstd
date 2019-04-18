# 生成模型--DCGAN(Deep Convolution GAN) - wydbyxr的博客 - CSDN博客
2018年10月26日 10:14:24[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：83
# DC-GAN(Deep Convolution GAN)
  用深度卷积网络进行对抗生成网络的建模。
  最早的时候，GAN采用的是全连接层。而后也有一些基于卷积神经网络的GAN实现，但是相对来说，DC-GAN的最终表现与同期的模型相比更优秀。
## 模型的一些改进细节
  1）将Pooling层替换成带有stride的卷积层
  2）使用Batch Normalization
  3）放弃使用全连接层
  4）将卷积层的非线性部分换成ReLU或者Leaky ReLU
