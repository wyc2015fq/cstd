# 深度学习基础--BP和训练--中继监督优化(intermediate supervision) - wydbyxr的博客 - CSDN博客
2018年11月13日 09:52:31[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：465
# 中继监督优化(intermediate supervision)
  如果直接对整个网络进行梯度下降，输出层的误差经过多层反向传播会大幅减小，即发生vanishing gradients现象。为解决此问题，在每个阶段的输出上都计算损失。这种方法可以保证底层参数正常更新。
  类似于GoogleNet的auxiliary classfier，用来解决网络随着深度加深而梯度消失的问题。
