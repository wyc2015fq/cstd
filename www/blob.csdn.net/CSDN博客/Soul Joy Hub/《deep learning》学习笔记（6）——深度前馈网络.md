# 《deep learning》学习笔记（6）——深度前馈网络 - Soul Joy Hub - CSDN博客

2017年10月01日 17:29:41[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：3226
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/78148087](http://blog.csdn.net/u011239443/article/details/78148087)
![](http://upload-images.jianshu.io/upload_images/1621805-53bb27bb7a85ef12.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 6.1 实例：学习 XOR

![](http://upload-images.jianshu.io/upload_images/1621805-7c6079213a460e17.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

通过学习一个表示来解决 XOR 问题。图上的粗体数字标明了学得的函数必须在每个点输出的值。(左) 直接应用于原始输入的线性模型不能实现 XOR 函数。当 x 1 = 0 时，模型的输出必须随着 x 2 的增大而增大。当 x 1 = 1 时，模型的输出必须随着 x 2 的增大而减小。线性模型必须对x 2 使用固定的系数 w 2 。因此，线性模型不能使用 x 1 的值来改变 x 2 的系数，从而不能解决这个问题。(右) 在由神经网络提取的特征表示的变换空间中，线性模型现在可以解决这个问题了。在我们的示例解决方案中，输出必须为 1 的两个点折叠到了特征空间中的单个点。换句话说，非线性特征将 x = [1,0] ⊤ 和 x = [0,1] ⊤ 都映射到了特征空间中的单个点 h = [1,0] ⊤ 。线性模型现在可以将函数描述为 h 1 增大和 h 2 减小。在该示例中，学习特征空间的动机仅仅是使得模型的能力更大，使得它可以拟合训练集。在更现实的应用中，学习的表示也可以帮助模型泛化。 
![](http://upload-images.jianshu.io/upload_images/1621805-8c14c99589c53ec6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-7493f0dc728386a1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-ac31ee84fbf36203.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 代码实现：

```python
import tensorflow as tf
import numpy as np

X = np.array([[0, 0], [0, 1], [1, 0], [1, 1]])
Y = np.array([[0], [1], [1], [0]])

x = tf.placeholder(tf.float32, [None, 2])
y = tf.placeholder(tf.float32, [None, 1])

w1_1 = tf.Variable(tf.random_normal([2, 1]))
w1_2 = tf.Variable(tf.random_normal([2, 1]))
w2 = tf.Variable(tf.random_normal([2, 1]))

b1_1 = tf.constant(0.1, shape=[1])
b1_2 = tf.constant(0.1, shape=[1])
b2 = tf.constant(0.1, shape=[1])

h1 = tf.nn.relu(tf.matmul(x, w1_1) + b1_1)
h2 = tf.nn.relu(tf.matmul(x, w1_2) + b1_2)

# 将两数组按列合并
hidden = tf.concat([h1, h2], 1)
out = tf.matmul(hidden, w2) + b2

loss = tf.reduce_mean(tf.square(out - y))

train = tf.train.AdamOptimizer(0.01).minimize(loss)

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    for i in range(1000):
        for j in range(4):
            sess.run(train, feed_dict={x: np.expand_dims(X[j], 0), y: np.expand_dims(Y[j], 0)})
        loss_ = sess.run(loss, feed_dict={x: X, y: Y})
        print("step: %d, loss: %.3f"%(i, loss_))
    print("X: %r"%X)
    print("pred: %r"%sess.run(out, feed_dict={x: X}))
```

![](http://upload-images.jianshu.io/upload_images/1621805-9c134421f99b6f75.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 6.2 基于梯度的学习

神经网络中的非线性导致它的大部分代价函数变得非凸，对于非凸的损失函数，梯度下降算法不能保证收敛到全局最优，因此神经网络模型中的参数初始化是非常重要的，通常会将所有的权重初始化为一个较小的随机数，并且将偏置初始化为0或者较小的正值。同其他机器学习算法一样，基于梯度的学习方法，需要设计代价函数，选择模型输出的表示方法。这里介绍神经网络中关于它们的一些设计方法。

## 6.2.1 代价函数

同其他机器学习模型一样，大多数情况下，神经网络的参数模型定义一个分布 ![](http://www.zhihu.com/equation?tex=p%28%5Cboldsymbol%7By%7D%7C%5Cboldsymbol%7Bx%7D%3B%5Cboldsymbol%7B%5Ctheta%7D%29) ，并且使用最大似然原理，用训练数据和模型预测间的交叉熵作为损失函数。有时候我们不需要预测 y 的完整概率分布，而是仅仅预测在给定 x 的条件下 y 的统计量。这个时候我们会使用一些专门的损失函数来进行模型的评估。

### 6.2.1.1 使用最大似然学习条件分布

大多数现代神经网络使用极大似然原理，也就是说模型的损失函数和训练数据和模型分布间的交叉熵等价。它表示为： 
![](http://upload-images.jianshu.io/upload_images/1621805-8dd6a738d2f890af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

由于神经网络的特殊结构，导致神经网络必须注意的是损失函数的梯度必须有足够大的预测性，这样才能很好的指导算法的学习。很多输出单元都会包含一个指数函数，当变量取绝对值非常大的负值时函数会变得饱和（函数变得很“平”），函数梯度变得很小，而负的对数似然能够抵消输出单元中的指数效果。

### 6.2.1.2 学习条件统计量

我们有时只需要学习给定 x 的某个条件统计量，不需要学习一个完整的概率分布 ![](http://www.zhihu.com/equation?tex=p%28%5Cboldsymbol%7By%7D%7C%5Cboldsymbol%7Bx%7D%3B%5Cboldsymbol%7B%5Ctheta%7D%29) 。这是需要使用变分法（calculus of variations），通过学习到一个函数，这个函数能够计算给定 x 时 y 的均值和中位数。目前我们给出学习给定 x 时的 y 均值和中位数的优化问题： 

预测**y的均值** 通过优化问题 
![](http://www.zhihu.com/equation?tex=f%5E%2A%3D%5Cmathop%7B%5Carg%5Cmin%7D_%7Bf%7D+%5Cmathbb%7BE%7D_%7Bx%2Cy%5Csim%7Bp_%7Bdata%7D%7D%7D%7C%7C%5Cboldsymbol%7By%7D-f%28%5Cboldsymbol%7Bx%7D%29%7C%7C%5E2)

从而得到 
![](http://www.zhihu.com/equation?tex=f%5E%2A%28%5Cboldsymbol%7Bx%7D%29%3D%5Cmathbb%7BE%7D_%7By%5Csim%7Bp_%7Bdata%28%5Cboldsymbol%7By%7D%7C%5Cboldsymbol%7Bx%7D%29%7D%7D%7D%5Cboldsymbol%7B%5By%5D%7D)

只要这个函数属于我们所要优化的函数族，那么我们可以得到一个对于每个 x 预测 y 均值的函数。 

不同的损失函数可以给出不同的统计量，为了**预测y的中位数**，我们通过优化问题 
![](http://www.zhihu.com/equation?tex=f%5E%2A%3D%5Cmathop%7B%5Carg%5Cmin%7D_%7Bf%7D+%5Cmathbb%7BE%7D_%7Bx%2Cy%5Csim%7Bp_%7Bdata%7D%7D%7D%7C%7C%5Cboldsymbol%7By%7D-f%28%5Cboldsymbol%7Bx%7D%29%7C%7C_1)

得到一个函数，如果这个函数属于我们所需优化的函数族，那么这个函数可以对每个 x 的中位数。这个损失函数其实就是平均绝对误差（mean absolute error）。 

需要提出的是，均方误差和平均绝对误差在使用基于梯度的优化方法时，会因为饱和的问题导致效果不好。因此即使我们不需要预测整个 ![](http://www.zhihu.com/equation?tex=p%28%5Cboldsymbol%7By%7D%7C%5Cboldsymbol%7Bx%7D%29) ，也倾向于使用交叉熵损失函数。

## 6.2.2 输出单元

从前文可知，通过极大似然的交叉熵损失函数，与我们选择的输出模型相关。本节讨论输出单元的选择，我们假定本节输出单元对前馈网络提供的一组定义为 ![](http://www.zhihu.com/equation?tex=%5Cboldsymbol%7Bh%7D%3Df%28%5Cboldsymbol%7Bx%7D%2C%5Cboldsymbol%7B%5Ctheta%7D%29) 的隐藏特征进行变换来完成整个网络的任务。

### 6.2.2.1 用于高斯输出分布的线性单元

线性输出层基于仿射变换，仿射变换的单元被称为线性单元。给定特征 h ，线性输出层产生输出向量  
![](http://upload-images.jianshu.io/upload_images/1621805-c8f2d95f10819082.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

 ，线性输出层常被用来产生条件高斯分布均值： 
![](http://www.zhihu.com/equation?tex=p%28%5Cboldsymbol%7By%7D%7C%5Cboldsymbol%7Bx%7D%29%3D%5Cmathcal%7BN%7D%28%5Cboldsymbol%7By%7D%3B%5Cboldsymbol%7B%5Chat%7By%7D%7D%2C%5Cboldsymbol%7BI%7D%29%29)

最大化其最大似然等价于最小化均方误差。线性模型不会饱和，因此适合采用基于梯度的优化算法，也可以使用其他多种优化算法。

### 6.2.2.2 用于 Bernoulli 输出分布的 sigmoid 单元

对于二分类问题，极大似然方法是定义 y 下的Bernoulli分布。Bernoulli分布仅需要预测单个参数 ![](http://www.zhihu.com/equation?tex=P%28y%3D1%7C%5Cboldsymbol%7Bx%7D%29) ，并且这个参数处于区间 [0, 1] 中。 

如果我们使用线性单元，则需要限定它是一个有效的概率值：

![](http://upload-images.jianshu.io/upload_images/1621805-298a5cc076955dfd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

这满足概率要求，但是使用梯度下降算法无法高效训练这个模型，因为当  
![](http://upload-images.jianshu.io/upload_images/1621805-a791e6d300884766.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

 处于单位区间外，模型梯度都将为0，梯度为0则使得算法不能再继续学习参数。我们需要使用一种方法使得无论模型合适给出错误答案，都能有一个比较大的梯度。这就是使用sigmoid单元的原因，sigmoid单元定义如下：

![](http://upload-images.jianshu.io/upload_images/1621805-a4b395b788d066a0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们分析使用sigmoid单元的极大似然交叉熵损失函数，首先忽略对于 x 的依赖性，只讨论如何用z 的概率分布。sigmoid可以构造一个非归一化（和不为1）的概率分布 ![](http://www.zhihu.com/equation?tex=%5Ctilde%7BP%7D%28y%29) 。通过除以一个合适的常数，可以得到一个有效的概率分布。假定非归一化的对数概率对 y 和 z 是线性的，可以对它取指数来得到非归一化的概率。然后进行归一化，可以发现它服从Bernoulli分布，该分布收到 z 的sigmoid变换控制。步骤如下： 
![](http://www.zhihu.com/equation?tex=log%5Ctilde%7BP%7D%28y%29%3Dyz%2C)
![](http://www.zhihu.com/equation?tex=%5Ctilde%7BP%7D%28y%29%3Dexp%28yz%29%2C)

归一化： 
![](http://www.zhihu.com/equation?tex=P%28y%29%3D%5Cfrac%7Bexp%28yz%29%7D%7B%5Csum_%7By%5E%7B%27%7D%3D0%7D%5E%7B1%7Dexp%28y%5E%7B%27%7Dz%29%7D)
![](http://www.zhihu.com/equation?tex=P%28y%29%3D%5Csigma%28%282y-1%29z%29)

定义这种二值分布的变量$z$被称为分对数（logit）。 

由上文可知，基于最大似然交叉熵的损失函数为 ![](http://www.zhihu.com/equation?tex=-logP%28y%7C%5Cboldsymbol%7Bx%7D%29) ，上文中说明了损失函数中的对数抵消了sigmoid中的指数，使得梯度不会饱和。基于极大似然交叉熵的损失函数如下所示： 
![](http://www.zhihu.com/equation?tex=J%28%5Cboldsymbol%7B%5Ctheta%7D%29%3D-logP%28y%7C%5Cboldsymbol%7Bx%7D%29%3D-log%5Csigma%28%282y-1%29z%29%3D%5Czeta%28%281-2y%29z%29)

### 6.2.2.3 用于 Multinoulli 输出分布的 softmax 单元

当我们进行n分类时，我们可以使用softmax函数，softmax函数用于表示 n 个不同分类上的概率分布，用向量 ![](http://www.zhihu.com/equation?tex=%5Chat%7B%5Cboldsymbol%7By%7D%7D) 表示，其中，每个元素 ![](http://www.zhihu.com/equation?tex=%5Chat%7By%7D_i%3DP%28y%3Di%7C%5Cboldsymbol%7Bx%7D%29)*，要求每个*![](http://www.zhihu.com/equation?tex=%5Chat%7By%7D_i) 在0到1之间，并且所有 ![](http://www.zhihu.com/equation?tex=%5Chat%7By%7D_i) 之和为1。类似于上文讲的二分类，多分类首先用线性层预测了未归一化的概率：

![](http://upload-images.jianshu.io/upload_images/1621805-c5947711b999d0d0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

其中 ![](http://www.zhihu.com/equation?tex=z_i%3D%5Chat%7BP%7D%28y%3Di%7C%5Cboldsymbol%7Bx%7D%29) 。softmax函数对其指数化并且归一化来获得满足条件的 ![](http://www.zhihu.com/equation?tex=%5Chat%7B%5Cboldsymbol%7By%7D%7D) 。softmax函数形式为： 
![](http://www.zhihu.com/equation?tex=softmax%28%5Cboldsymbol%7Bz%7D%29_i%3D%5Cfrac%7Bexp%28z_i%29%7D%7B%5Csum_%7Bj%7Dexp%28z_j%29%7D)

使用最大化对数似然来训练softmax输出目标 y，得到如下损失函数： 
![](http://www.zhihu.com/equation?tex=logsoftmax%28%5Cboldsymbol%7Bz%7D%29_i%3Dz_i-log%5Csum_%7Bj%7Dexp%28z_j%29)

这个损失函数不会出现饱和的情况，对于错误的参数总能得到比较大的梯度，需要注意的是，如果不适用对数似然的损失函数，那么由于指数项的存在将导致饱和问题而不能达到好的训练效果。

# 6.3 隐藏单元

## 6.3.1 整流线性单元及其扩展

整流线性单元的激活函数如下： 
![](http://www.zhihu.com/equation?tex=g%28z%29%3Dmax%280%2Cz%29)

整流线性单元的特点是当它处于激活状态时，它们的梯度大于0并且是一致的，一阶导数处处为1，这对于学习算法来说是很有用的。整流线性单元通常作用于仿射变换之上：

![](http://upload-images.jianshu.io/upload_images/1621805-d5c266145be45213.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

初始化的仿射变换参数的建议是将 b 的所有元素设置成一个较小的正值，这样整流线性单元在初始时就对训练集中的大多数输入处于激活状态，并且允许导数通过。 

整流线性单元的一个缺陷是未激活状态不能带来基于梯度学习的效果。因此许多整流线性单元的扩展设计了不同的方法避免这种情况。当 z_i=0*时使用一个非零的斜率* alpha_i ，表示为  
![](http://upload-images.jianshu.io/upload_images/1621805-59ab0bc58bb33f4a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

下面是三种基于这种设计方法的激活函数： 

绝对值整流（absolute value rectification）：通过固定 ![](http://www.zhihu.com/equation?tex=%5Calpha_i%3D-1) 来得到 ![](http://www.zhihu.com/equation?tex=g%28z%29%3D%7Cz%7C)

渗漏整流线性单元（Leaky ReLU）：固定 alpha_i 为一个类似0.01的小值 

参数化整流线性单元（parametric ReLU）或者PReLU：将 alpha_i 也作为一个要学习的参数

maxout单元（maxout unit）进一步扩展了整流线性单元，maxout单元将z划分为每组具有 k 个值的组，表示如下: 
![](http://www.zhihu.com/equation?tex=g%28%5Cboldsymbol%7Bz%7D%29_i%3D%5Cmax%5Climits_%7Bj%5Cin%5Cmathbb%7BG%7D%5E%7B%28i%29%7D%7D%28z_j%29)

其中 ![](http://www.zhihu.com/equation?tex=%5Cmathbb%7BG%7D%5E%7B%28i%29%7D) 是组 i 的输入索引集合 
![](http://upload-images.jianshu.io/upload_images/1621805-8f74d6bd4fea8774.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

 ，maxout可以学习 k 段的分段线性凸函数。当 k 足够大时，maxout可以以任意精度来近似任何凸函数。由于maxout相比普通整流线性单元增加了 k 组，因此参数增加了 k 倍，所以训练maxout需要更多的正则化或者更多训练样本来提高模型的泛化能力。

## 6.3.2 logistic sigmoid与双曲正切函数

对于隐藏单元，logistic sigmoid函数只有在输入接近0的时候它们的梯度才比较大，因此不鼓励将它们作为前馈网络中的隐藏层，对于上文提到的输出层，对数似然损失函数抵消了sigmoid的饱和性，因此可以用在基于梯度学习的输出单元中。 

双曲正切激活函数 ![](http://www.zhihu.com/equation?tex=g%28z%29%3Dtanh%28z%29) 通常比sigmoid函数表现要好，它和sigmoid激活函数关系密切： ![](http://www.zhihu.com/equation?tex=tanh%28z%29%3D2%5Csigma%282z%29-1) 。因此需要用sigmoid函数作为隐藏层激活函数时，可以考虑使用双曲正切函数代替。双曲正切函数在0附近与单位函数类似，因此使用双曲正切函数作为激活函数类似于学习一个线性模型，因此在输入处于0附近时，使用tanh网络更加容易进行学习。 

相比前馈网络的隐藏层，sigmoid激活函数在循环网络等一些额外要求不能使用分段激活函数的场景中更实用。

# 6.4 架构设计

神经网络的架构（architecture）指网络的整体架构：神经网络需要多少单元以及单元之间的连接方式。大多数神经网络被组织成层的单元组，然后将这些层布置成链式结构，其中每一层是前一层的函数。在这种结构中，第一层表示如下：

![](http://upload-images.jianshu.io/upload_images/1621805-4eb2d67c23fd5a92.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

那么第二层为：

![](http://upload-images.jianshu.io/upload_images/1621805-47a0caf3263c0f2b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在这个链式结构中，主要考虑的是网络的深度和每一层的宽度。通常来说更深的网络对每一层能够使用更少的单元数以及参数，并且泛化效果更好，但是它也更能难以训练

## 6.4.1 万能近似性质和深度

万能近似定理（universal approximation theorem）表明一个前馈神经网络如果具有线性输出层和至少一层具有任何一种“挤压”性质的激活函数（如logistic sigmoid激活函数）的隐藏层，只要给与网络足够数量的隐藏单元，它可以以任意精度来近似任何从一个有限维空间到另一有限维空间的Borel可测函数，前馈网络的导数也可以任意精度来近似函数的导数。简单的说，定义在 R^n 的有界闭集上的任意连续函数是Borel可测的，因此可以用神经网络来近似。神经网络也可以近似从任何有限维离散空间映射到另一个的函数。万能近似性质被证明对于比较广泛类别的激活函数都是适用的，其中包括整流线性单元。 

万能近似定理说明了存在达到任意精度的这么一个神经网络，但是没有指出这个网络有多大。Barron提供了单层网络近似一大类函数所需大小的一些界，在最坏的情况下，隐藏单元的数量是指数数量。具有单层的前馈网络足以表达任何函数，但是单元数会多到无法实现，因此无法正确学习和泛化，在很多情况下，使用更深的模型能够减少表示期望函数所需的单元数量，并且可以减少泛化误差。增加网络的深度往往能够得到比增加宽度更加好的泛化能力。增加深度能够更好的泛化的原因可以理解为神经网络将学习分解成了多个步骤（例如先认识点，再认识线，再认识形状等），每一层在上一层的基础上进行了学习。

# 6.5 反向传播和其他的微分算法

可参阅：[http://blog.csdn.net/u011239443/article/details/76680704#t2](http://blog.csdn.net/u011239443/article/details/76680704#t2)

编码例子：[http://blog.csdn.net/u011239443/article/details/75008380](http://blog.csdn.net/u011239443/article/details/75008380)
![](http://upload-images.jianshu.io/upload_images/1621805-e7dd694099f756b7?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

