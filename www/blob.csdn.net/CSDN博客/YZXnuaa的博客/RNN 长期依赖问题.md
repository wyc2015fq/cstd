# RNN 长期依赖问题 - YZXnuaa的博客 - CSDN博客
2018年04月19日 19:39:52[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1024
# 1.长期依赖问题
    RNN面临的最大挑战就是无法解决长期依赖问题。例如对下面两句话：
![](https://img-blog.csdn.net/20180322170928455)
最后的was与were如何选择是和前面的单复数有关系的，但对于简单的RNN来说，两个词相隔比较远，如何判断是单数还是复数就很关键。长期依赖的根本问题是，经过许多阶段传播后的梯度倾向于消失（大部分情况）或爆炸（很少，但对优化过程影响很大）。对于梯度爆炸是很好解决的，可以使用梯度修剪（**Gradient Clipping**），即当梯度向量大于某个阈值，缩放梯度向量。但对于梯度消失是很难解决的。
# 2.简单的数学推导
    如何从数学的角度来理解梯度消失或者爆炸问题呢，我们将以一个简单的、缺少非线性激活函数和输入x的循环神经网络为例。我们可以认为，循环联系为：
![](http://latex.codecogs.com/png.latex?a%5E%7B%3Ct%3E%7D%3DW%5E%7BT%7Da%5E%7B%3Ct-1%3E%7D)
    可以简化为：
![](http://latex.codecogs.com/png.latex?a%5E%7B%3Ct%3E%7D%3D%28W%5E%7Bt%7D%29%5E%7BT%7Da%5E%7B%3C0%3E%7D)
    当W符合下列形式的特征分解时：
![](http://latex.codecogs.com/png.latex?W%3DQ%5CLambda%20Q%5E%7BT%7D)
    其中Q正交，循环联系可以进一步简化为：
![](http://latex.codecogs.com/png.latex?a%5E%7B%3Ct%3E%7D%3DQ%5E%7BT%7D%5CLambda%20%5E%7Bt%7DQa%5E%7B%3C0%3E%7D)
    特征值提升到t次后，幅值小于1的衰减为0，幅值大于1的急剧增大，任何不与最大特征向量对齐的![](http://latex.codecogs.com/png.latex?a%5E%7B%3C0%3E%7D)的部分将最终被丢弃。
# 3.解决方法
   （1）渗透单元及其它多时间尺度的策略
    （2）长短期记忆和其它门控RNN
    （3）优化长期依赖（截断梯度、引导信息流的正则化）
    之后将会对长短期记忆网络（LSTM）以及门控循环单元（GRU）做讲解，其余的部分可查阅花书10.9及10.11节。
