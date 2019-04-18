# 深度学习基础--上采样(upsample)的方法 - wydbyxr的博客 - CSDN博客
2018年11月07日 10:31:44[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：549
# 上采样(upsample)的方法
  在神经网络中，扩大特征图的方法，即upsample/上采样的方法
  1）unpooling：恢复max的位置，其余部分补零
  2）deconvolution(反卷积)：先对input补零，再conv
  3）插值方法，双线性插值等；
  4）扩张卷积，dilated conv；
