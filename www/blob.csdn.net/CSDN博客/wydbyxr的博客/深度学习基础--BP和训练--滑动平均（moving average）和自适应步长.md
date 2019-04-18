# 深度学习基础--BP和训练--滑动平均（moving average）和自适应步长 - wydbyxr的博客 - CSDN博客
2018年11月13日 09:43:29[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：426
# 滑动平均（moving average）
  要前进的方向不再由当前梯度方向完全决定，而是最近几次梯度方向的滑动平均。利用滑动平均思想的优化算法有带动量（momentum）的SGD、Nesterov动量、Adam（ADAptive Momentum estimation）等。
# 自适应步长
  自适应地确定权值每一维的步长。当某一维持续震荡时，我们希望这一维的步长小一些；当某一维一直沿着相同的方向前进时，我们希望这一维的步长大一些。利用自适应步长思想的优化算法有AdaGrad、RMSProp、Adam等。
