# AlexNet - wydbyxr的博客 - CSDN博客
2018年11月11日 09:53:29[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：73
个人分类：[深度学习论文阅读](https://blog.csdn.net/wydbyxr/article/category/7151097)
# AlexNet
  使用ReLu（整流线性单位）作为非线性部分，而不是 传统神经网络早期标准Tanh 或Sigmoid函数。
  ReLu相对于S形的优势在于它的训练速度比后者快得多，因为S形的导数在饱和区域变得非常小，因此对权重的更新几乎消失，这被称为  消失梯度问题。
