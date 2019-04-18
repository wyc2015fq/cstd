# Pseudo-siamese网络 - wydbyxr的博客 - CSDN博客
2018年11月27日 10:42:02[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：82
# Pseudo-siamese网络
  这个网络与siamese network网络最大的区别在于两个分支是权值不共享的，是真正的双分支网络模型。
  Pseudo-siamese在网络的两个分支上，每个分支是不同的映射函数，也就是说它们提取特征的结构是不一样的，左右两个分支，有不同的权值、或者不同的网络层数等，两个函数互不相关，只是在最后的全连接层，将他们连接在一起了。
  这个网络相当于训练参数比Siamese网络的训练参数多了将近一倍，当然它比Siamese网络更加灵活。
