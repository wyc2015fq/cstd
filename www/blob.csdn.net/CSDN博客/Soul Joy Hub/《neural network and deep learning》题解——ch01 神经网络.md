# 《neural network  and deep learning》题解——ch01 神经网络 - Soul Joy Hub - CSDN博客

2017年06月28日 22:11:05[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：4820
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/73864223](http://blog.csdn.net/u011239443/article/details/73864223)

在线阅读：[http://neuralnetworksanddeeplearning.com/](http://neuralnetworksanddeeplearning.com/)

# 1.2 S 型神经元

## 问题 1

> 
假设我们把一个感知器网络中的所有权重和偏置乘以一个正的常数,c > 0。证明网络的行为并没有改变。

证：

$\large \color{blue}{\sigma (cw,cb) = \frac{1}{1+e^{-\sum_jcw_jx_j-cb}}  = \frac{1}{1+e^{-cz}}}$

当$c>0$时，假设`z`是一个很大的正数，$e^{-cz}  \approx 0$， 而 $\sigma(z) \approx 1$；假设`z`是一个的负数，$e^{-cz}  -> + \infty$ ，而 $\sigma(z) \approx 0$。所以，网络行为并没有改变。

## 问题 2

> 
假设我们有上题中相同的设置 —— 一个感知器网络。同样假设所有输入被选中。我们不需要实际的输入值,仅仅需要固定这些输入。假设对于网络中任何特定感知器的输入 x,权重和偏置遵循 w · x + b ̸ = 0。现在用 S 型神经元替换所有网络中的感知器,并且把权重和偏置乘以一个正的常量 c > 0。证明在 c → ∞ 的极限情况下,S 型神经元网络的行为和感知器网络的完全一致。当一个感知器的 w · x + b = 0 时又为什么会不同?

当$w*x + b = 0$，则$z = 0$,则$\sigma(z) = \sigma(0) = \frac{1}{1 + 1} = 0.5$。 
$\sigma(z)$恒等于$0.5$，因此网络行为不同。 

当$w*x + b \neq 0 , c -> + \infty$，所以 $c > 0$，与题1同理，网络行为并没有改变。

# 1.4 一个简单的分类手写数字的网络

## 问题 1

> 
通过在上述的三层神经网络加一个额外的一层就可以实现按位表示数字。额外的一层把原来的输出层转化为一个二进制表示,如下图所示。为新的输出层寻找一些合适的权重和偏置。假定原先的 3 层神经网络在第三层得到正确输出(即原来的输出层)的激活值至少是0.99,得到错误的输出的激活值至多是 0.01。

`上述的三层神经网络`如下：

![这里写图片描述](https://img-blog.csdn.net/20170701091542081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

`按位表示数字的神经网络`如下：

![这里写图片描述](https://img-blog.csdn.net/20170701091707055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

首先，我们设想0～9用四位二进制如何表示：
|十进制|二进制|
|----|----|
|0|0000|
|1|0001|
|2|0010|
|3|0011|
|4|0100|
|5|0101|
|6|0110|
|7|0111|
|8|1000|
|9|1001|

一个比较简化而合理的权重是，就选用二进制位置上的数值作为权重。假设正确输出为`0`，这输出层是输入为： 

（可以竖着看二进制的每一位） 

第一个神经元输入 = $0.99 * 0 + 0.01 * 0 + ... + 0.01 * 1+ 0.01 * 1 = 0.02$

同理运算得到: 

第二个神经元输入 = $0.04$

第三个神经元输入 = $0.04$

第三个神经元输入 = $0.05$

所以我们可以把偏置设置为 `-0.06`

# 1.5 使用梯度下降算法进行学习

## 问题 1

> 
证明上一段落的推断。提示:可以利用柯西-施瓦茨不等式。

上一段落为：

> 
事实上,甚至有一种观点认为梯度下降法是求最小值的最优策略。假设我们正在努力去改变∆v 来让 C 尽可能地减小。这相当于最小化 ∆C ≈ ∇C · ∆v。我们首先限制步⻓为小的固定值,即 ∥∆v∥ = ε,ε > 0。当步⻓固定时,我们要找到使得 C 减小最大的下降方向。可以证明,使得∇C · ∆v 取得最小值的 ∆v 为 ∆v = −η∇C,这里 η = ε/∥∇C∥ 是由步⻓限制 ∥∆v∥ = ε 所决定的。因此,梯度下降法可以被视为一种在 C 下降最快的方向上做微小变化的方法。

柯西-施瓦茨不等式：

$\large \color{blue}{|a||b|  >= |ab|}$

因为$ ||∆v|| = ε$,由 柯西-施瓦茨不等式得：

$\large \color{blue}{∇C · ∆v <= ||∇C ||·|| ∆v|| = ε||∇C|| }$

所以想让$∇C · ∆v $得到最大值,即等于$ε||∇C||$ ，则：

$\large \color{blue}{∆v = \frac{ε∇C}{||∇C||}}$

同理，想得到$∇C · ∆v $得到最小值，则：

$\large \color{blue}{∆v = -\frac{ε∇C}{||∇C||} = −η∇C}$

## 问题 2

> 
我已经解释了当 C 是二元及其多元函数的情况。那如果 C 是一个一元函数呢?你能给出梯度下降法在一元函数的几何解释么?

如果 C 是一个一元函数，我们可以几何想象成是一个C为y轴，v为x轴，上的曲线，我们在曲线上寻找C的最低点。

## 问题 3

> 
梯度下降算法一个极端的版本是把小批量数据的大小设为 1。即,假设一个训练输入 x,我们按照规则$ w k → w k ′ = w k − η∂C x /∂w k 和 b l → b ′ l = b l − η∂C x /∂b l$ 更新我们的权重和偏置。然后我们选取另一个训练输入,再一次更新权重和偏置。如此重复。这个过程被称为在线、online、on-line、或者递增学习。在 online 学习中,神经网络在一个时刻只学习一个训练输入(正如人类做的)。对比具有一个小批量输入大小为 20 的随机梯度下降,说出递增学习的一个优点和一个缺点。

优点：一个时刻只学习一个训练输入，能让模型迅速的学习到当前时刻的数据。如根据用户浏览的商品，实时的推荐相关的商品;根据行用卡使用行为数据，实时的预测出欺诈行为。

缺点：对比具有一个小批量输入大小为 20 的随机梯度下降，`online` 学习的实际上的学习率太大，偶然突发性的噪音数据会极大的影响原本的模型。

# 1.6 实现我们的网络来分类数字

## 问题 1

> 
以分量形式写出方程 (22),并验证它和计算 S 型神经元输出的规则 (4) 结果相同。

方程 (22):

$\large \color{blue}{a ′ = σ(wa + b)}$

方程 (4):

$\large \color{blue}{\frac{1}{1+exp(-\sum_jw_jx_j-b)}}$

$\large \color{blue}{a ′ = σ(wa + b) = \frac{1}{1+e^{-wa-b}}=\frac{1}{1+exp(-\sum_jw_ja_j-b)}= 方程（4）}$

## 问题 2

> 
试着创建一个仅有两层的网络 —— 一个输入层和一个输出层,分别有 784 和 10 个神经元,没有隐藏层。用随机梯度下降算法训练网络。你能达到多少识别率?

```python
import mnist_loader
import network

training_data,validation_data,test_data = mnist_loader.load_data_wrapper()
net = network.Network([784,10])
net.SGD(training_data,30,10,3.0,test_data=test_data)

Epoch 22: 7553 / 10000
Epoch 23: 7581 / 10000
Epoch 24: 7581 / 10000
Epoch 25: 7569 / 10000
Epoch 26: 7579 / 10000
Epoch 27: 7600 / 10000
Epoch 28: 7587 / 10000
Epoch 29: 7586 / 10000
```

![](http://upload-images.jianshu.io/upload_images/1621805-e34221289ab19bd4?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

