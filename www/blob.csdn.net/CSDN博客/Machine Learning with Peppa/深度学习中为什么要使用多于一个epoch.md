# 深度学习中为什么要使用多于一个epoch? - Machine Learning with Peppa - CSDN博客





2018年11月25日 11:42:55[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：2478








**什么是epoch？**

当一个完整的数据集通过了神经网络一次并且返回了一次，这个过程称为一次epoch。然而，当一个epoch对于计算机而言太庞大的时候，就需要把它分成多个小块。

**为什么要使用多于一个epoch?**

在神经网络中传递完整的数据集一次是不够的，而且我们需要将完整的数据集在同样的神经网络中传递多次。但请记住，我们使用的是有限的数据集，并且我们使用一个迭代过程即梯度下降来优化学习过程。如下图所示。因此仅仅更新一次或者说使用一个epoch是不够的。

![](https://img-blog.csdn.net/20171122112111189)

随着epoch数量增加，神经网络中的权重的更新次数也在增加，曲线从欠拟合变得过拟合。

那么，问题来了，几个epoch才是合适的呢？

不幸的是，这个问题并没有正确的答案。对于不同的数据集，答案是不一样的。但是数据的多样性会影响合适的epoch的数量。比如，只有黑色的猫的数据集，以及有各种颜色的猫的数据集。

**其他与epoch有关的参数**

Batch Size

batch size将决定我们一次训练的样本数目。注意：batch size 和 number of batches是不同的。在不能将数据一次性通过神经网络的时候，就需要将数据集分成几个batch。

Iteration

Iteration是batch需要完成一个epoch的次数。

举个例子：

CIFAR10 数据集有 50000 张训练图片，10000 张测试图片。现在选择 Batch Size = 256 对模型进行训练。
- 每个 Epoch 要训练的图片数量： ![50000](http://www.zhihu.com/equation?tex=50000)
- 训练集具有的 Batch 个数： ![50000 / 256 = 195 + 1 = 196](http://www.zhihu.com/equation?tex=50000+%2F+256+%3D+195+%2B+1+%3D+196)
- 每个 Epoch 需要完成的 Batch 个数： ![196](http://www.zhihu.com/equation?tex=196)
- 每个 Epoch 具有的 Iteration 个数： ![196](http://www.zhihu.com/equation?tex=196)
- 每个 Epoch 中发生模型权重更新的次数： ![196](http://www.zhihu.com/equation?tex=196)
- 训练 ![10](http://www.zhihu.com/equation?tex=10) 代后，模型权重更新的次数： ![196 * 10 = 1960](http://www.zhihu.com/equation?tex=196+%2A+10+%3D+1960)
- 不同代的训练，其实用的是同一个训练集的数据。第 ![1](http://www.zhihu.com/equation?tex=1) 代和第 ![10](http://www.zhihu.com/equation?tex=10) 代虽然用的都是训练集的五万张图片，但是对模型的权重更新值却是完全不同的。因为不同代的模型处于代价函数空间上的不同位置，模型的训练代越靠后，越接近谷底，其代价越小。

![model1](https://github.com/persistforever/cifar10-tensorflow/raw/master/exps/cifar10-v1/cifar10-v1.png)



