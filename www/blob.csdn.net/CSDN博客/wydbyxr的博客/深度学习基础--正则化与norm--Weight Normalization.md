# 深度学习基础--正则化与norm--Weight Normalization - wydbyxr的博客 - CSDN博客
2018年11月20日 10:38:37[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：128
# Weight Normalization
  即权重归一化，也就是对权重值进行归一化。
## 优点
  1）WN是通过重写深度网络的权重来进行加速的，没有引入对minibatch的依赖，更适合于RNN网络
  2）引入更少的噪声
  3）不需要额外的空间进行存储minibatch的均值和方差，对时间的开销也小，所以速度会更快
