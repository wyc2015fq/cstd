# 生成模型--GAN的损失函数改进 - wydbyxr的博客 - CSDN博客
2018年10月26日 10:26:02[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：523
# GAN的损失函数改进
  除了像cycleGAN、PTGAN等这类在网络理解上的改进，对于GAN而言，还有一类改进是直接针对于损失函数的。
  WGAN、LSGAN、WGAN-GP都是在 GAN 的损失函数上采取改进，将衡量真实和生成样本的分布的 JS 距离换为更加优越的衡量标准。
