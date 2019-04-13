
# Relu及其延伸 - 郭云飞的专栏 - CSDN博客


2017年09月30日 10:21:10[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1836


近年来，在深度学习中，ReLU 变的越来越受欢迎。它的数学表达式很简单：*f(x)=max(0,x)。*显然，输入信号<0时，输出都是0；输入信号>0 的情况下，输出等于输入。二维情况下，使用ReLU之后的效果如下：
![](http://7pn4yt.com1.z0.glb.clouddn.com/blog-relu-perf.png)
**ReLU 的优点：**

收敛速度会比 sigmoid/tanh 快很多
相比于 sigmoid/tanh，ReLU 只需要一个阈值就可以得到激活值，而不用去算一大堆复杂的运算。
**ReLU 的缺点：**

训练的时候很”脆弱”，很容易就”die”了。例如当一个非常大的梯度流过一个Relu神经元时，更新参数后，这个神经元再也不会对任何数据有激活现象了。实际操作中，如果你的learning rate很大，那么很有可能你网络中的40%的神经元都”dead”了。 当然，如果你设置了一个合适的较小的learning rate，这个问题发生的情况其实也不会太频繁。
为了解决Relu容易**”die”**这个问题，一些Relu的变形就出来了：
**Leaky-ReLU：**
*f(x)=αx，(x < 0)，*α 是一个很小的常数。
*f(x)=  x，(x>=0)*
这样Leaky-ReLU即修正了数据分布，又保留了一些负轴的值，使得负轴信息不会全部丢失。
![](http://7pn4yt.com1.z0.glb.clouddn.com/blog-leaky.png)
关于Leaky ReLU 的效果，众说纷纭，没有清晰的定论。有些人做了实验发现 Leaky ReLU 表现的很好；有些实验则证明并不是这样。
**Parametric ReLU：**
对于 Leaky ReLU 中的α，通常都是通过先验知识人工赋值的。 然而可以观察到，损失函数对α的导数是可以求得的，可不可以将它作为一个参数进行训练呢？ Kaiming He的论文《*Delving Deep into Rectifiers: Surpassing Human-Level Performance on ImageNet Classification*》指出，不仅可以训练，而且效果更好。公式非常简单，反向传播至未激活前的神经元的公式就不写了，很容易就能得到。对α的导数如下：

**Randomized ReLU：**
是 leaky ReLU 的random 版本 （α 是random的）。核心思想就是，在训练过程中，α 是从一个高斯分布 U(l,u) 中 随机出来的，然后再测试过程中进行修正（有点像dropout的用法）。
![](https://img-blog.csdn.net/20170930104748414?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





