# 训练过程--BN和RELU的trick - wydbyxr的博客 - CSDN博客
2018年12月06日 13:33:47[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：193
  ReLU对于0的输出的梯度为0， 所以一旦陷入了0输出， 就没法恢复了。
  过小的 feature_size/kernel_size的比值会造成BN的统计值不稳定；kernel_dim 过小的时候， 加ReLU 容易使得整个kernel退化成空白。
  BN在最后的时候会fix，然后再训练5~10W次。
