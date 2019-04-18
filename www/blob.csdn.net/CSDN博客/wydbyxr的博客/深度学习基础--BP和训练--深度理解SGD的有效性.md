# 深度学习基础--BP和训练--深度理解SGD的有效性 - wydbyxr的博客 - CSDN博客
2018年11月13日 11:05:59[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：164
# 为什么 SGD 在深度学习模型的训练中如此有效？
  通过大量实验佐证了神经网络在用 SGD训练过程中实现的两个 phases：
  1） empirical error minimization（ERM）；
  2）information compression with the training error constraints。
  其实这个观察和分类是在大家对 SGD 训练 NN 熟知的理解，前段中随机梯度的的均值会 dominate 随机梯度的 variance， 所以主要在做 ERM，而后段随着优化的进行，gradient 的均值会逐渐趋近于零，variance 会 dominate(占据优势)， 相当于做 diffusion(扩散)。
