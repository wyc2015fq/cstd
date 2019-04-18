# RNN--zoneout - wydbyxr的博客 - CSDN博客
2018年11月01日 10:45:59[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：257
# zoneout
  来自《Zoneout: Regularizing RNNs by Randomly Preserving Hidden Activations》。
  zoneout是rnn 时间维度上的“dropout”，要么维持前一个时刻的hidden vector，要么按照一般的样子更新。不是指单独的cell，而是指训练时的一种trick。
