# 深度学习基础--不同网络种类--Highway Network - wydbyxr的博客 - CSDN博客
2018年11月11日 10:08:00[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：84
# Highway Network
  受LSTM启发，增加了一个门函数，让网络的输出由两部分组成，分别是网络的直接输入以及输入变形后的部分。
  假设定义一个非线性变换为y=H(x,W_h)，定义门函数T(x,W_t)，携带函数C(x,W_c)=1-T(x,W_t)。对于门函数取极端的情况0/1会有y=0或y=H(x,W_h)，而对应的门函数使用sigmoid函数T(x)=sig(W_t*x+b_t)，则极端的情况不会出现。
  一个网络的输出最终变为，注意这里的乘法是element-wise multiplication。
  注意，门函数，转换，与的维度应该是相同的。如果不足，可以用0补或者用一个卷积层去变化。
  参考资料：[https://blog.csdn.net/jzrita/article/details/72732037](https://blog.csdn.net/jzrita/article/details/72732037)
## ResNet的结构与Highway很类似
  如果把Highway的网络变一下形会得到，而在ResNet中，直接把门函数T(x)去掉，就得到一个残差函数，而且会得到一个恒等的映射 x ，对的，这叫残差网络，它解决的问题与Highway一样，都是网络加深导致的训练困难且精度下降的问题。
