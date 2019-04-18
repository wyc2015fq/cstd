# 深度学习基础--不同网络种类--反馈深度网络(feed-back deep networks, FBDN) - wydbyxr的博客 - CSDN博客
2018年11月08日 12:03:39[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：250
# 深度神经网络可以分为3类：
  1）前馈深度网络(feed-forwarddeep networks, FFDN)
  2）反馈深度网络(feed-back deep networks, FBDN)
  3）双向深度网络(bi-directionaldeep networks, BDDN)
# 反馈深度网络(feed-back deep networks, FBDN)
  由多个解码器层叠加而成。
## 主流的反馈深度网络
### 1）反卷积网络(deconvolutionalnetworks, DN)
  （a）单层反卷积网络：反卷积网络是通过先验学习，对信号进行稀疏分解和重构的正则化方法。
  （b）反卷积网络：单层反卷积网络叠加（多个反卷积层叠加），可得到反卷积网络。
  常见于GAN和编码解码器。主要应用于网络的可视化、GAN。
  反卷积网络训练时，使用一组不同的信号y，求解C(y)，进行滤波器组f和特征图z的迭代交替优化。训练从第1层开始，采用贪心算法，逐层向上进行优化，各层间的优化是独立的。
### 2）层次稀疏编码网络(hierarchical sparse coding, HSC)
  一种无监督的神经网络学习架构。不是自编码器！而是自编码器的解码部分。
  反卷积网络与层次稀疏编码网络的不同：在反卷积网络中对图像的分解采用矩阵卷积的形式，而在稀疏编码中采用矩阵乘积的方式。
