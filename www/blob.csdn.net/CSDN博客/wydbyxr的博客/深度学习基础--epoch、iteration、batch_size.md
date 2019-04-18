# 深度学习基础--epoch、iteration、batch_size - wydbyxr的博客 - CSDN博客
2018年11月07日 10:53:49[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：30
# epoch、iteration、batch_size
  深度学习中经常看到epoch、 iteration和batchsize，下面说说这三个的区别：
  1）batchsize：批大小。在深度学习中，一般采用SGD训练，即每次训练在训练集中取batchsize个
样本训练；
  2）iteration：1个iteration等于使用batchsize个样本训练一次；batch 需要完成一个 epoch 的次数。记住：在一个 epoch 中，batch 数和迭代数是相等的。
  3）epoch：1个epoch等于使用训练集中的全部样本训练一次；
## 例子
  对于一个有 2000 个训练样本的数据集。将 2000 个样本分成大小为 500 的 batch，那么完成一个 epoch 需要 4 个 iteration。
