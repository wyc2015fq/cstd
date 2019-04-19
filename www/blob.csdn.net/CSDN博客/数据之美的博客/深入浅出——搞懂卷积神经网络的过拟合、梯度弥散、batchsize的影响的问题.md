# 深入浅出——搞懂卷积神经网络的过拟合、梯度弥散、batchsize的影响的问题 - 数据之美的博客 - CSDN博客
2017年04月25日 20:51:28[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1814
　　上一篇主要是对卷积神经网络的整个训练过程中公式以及误差的推导给出详细的分析。 
　　博客地址：[http://blog.csdn.net/u010402786/article/details/51226825](http://blog.csdn.net/u010402786/article/details/51226825)
　　这一篇主要对卷积神经网络中出现的一些问题进行一个详细的阐述。
## 第一个问题：模型过拟合
１.１　什么是过拟合
　　所谓过拟合（Overfit），是这样一种现象：一个假设在训练数据上能够获得比其他假设更好的拟合，但是在训练数据外的数据集 上却不能很好的拟合数据。此时我们就叫这个假设出现了overfit的现象。 
![这里写图片描述](https://img-blog.csdn.net/20160423205908457)
　　如上图所示：过拟合，就是拟合函数需要顾忌每一个点，最终形成的拟合函数波动很大。在某些很小的区间里，函数值的变化很剧烈。这就意味着函数在某些小区间里的导数值（绝对值）非常大，由于自变量值可大可小，所以只有系数足够大，才能保证导数值很大。
１.２　造成过拟合的原因
　　过拟合其中一个可能的成因就是模型的vc维过高，使用了过强的模型复杂度(model complexity)的能力。（参数多并且过训练） 
　　还有一个原因是数据中的噪声，造成了如果完全拟合的话，也许与真实情景的偏差更大。 
　　最后还有一个原因是数据量有限，这使得模型无法真正了解整个数据的真实分布。 
　　权值学习迭代次数足够多(Overtraining),拟合了训练数据中的噪声和训练样例中没有代表性的特征。
１.3　过拟合解决方法
1.权值衰减 
　　在每次迭代过程中以某个小因子降低每个权值,这等效于修改E的定义,加入一个与网络权值的总量相应的惩罚项,此方法的动机是保持权值较小,避免weight decay,从而使学习过程向着复杂决策面的反方向偏。（L2正则化） 
2.适当的stopping criterion（验证集） 
３.Cross-validation with some patterns 
交叉验证方法在可获得额外的数据提供验证集合时工作得很好,但是小训练集合的过度拟合问题更为严重 
k-fold交叉方法: 
　　把训练样例分成k份,然后进行k次交叉验证过程,每次使用不同的一份作为验证集合,其余k-1份合并作为训练集合.每个样例会在一次实验中被用作验证样例,在k-1次实验中被用作训练样例;每次实验中,使用上面讨论的交叉验证过程来决定在验证集合上取得最佳性能的迭代次数n*,然后计算这些迭代次数的均值,作为最终需要的迭代次数。 
４.正则化 
　　这里有点疑问，正则化是为了防止过拟合还是为了解决过拟合。对部分无用的feature，定义其parameter(p3,p4)非常大，这样会导致训练结果w3,w4非常小，几乎为0，降低模型复杂度。 
　　关于正则化具体的解释请移步到博主另一篇博文中： 
([http://blog.csdn.net/u010402786/article/details/49592239](http://blog.csdn.net/u010402786/article/details/49592239))
## 第二个问题：数据集不平衡
　　不平衡训练集会对结果造成很大的负面影响，而训练集在平衡的情况下，能够达到最好的performance。 
1. 训练数据分布情况对CNN结果产生很大影响； 
2. 平衡的训练集是最优的，数据越不平衡，准确率越差； 
3. 如何提升准确率（使用Oversampling）
　　相关论文是： 
《The Impact of Imbalanced Training Data for Convolutional Neural Networks》
## 第三个问题：Batchsize的影响
3.1Batch_Size三种情况
　　Batch_Size（批尺寸）是[机器学习](http://lib.csdn.net/base/machinelearning)中一个重要参数。 
　　①如果数据集比较小，完全可以采用全数据集 （ Full Batch Learning ）的形式，这样做至少有 2 个好处：其一，由全数据集确定的方向能够更好地代表样本总体，从而更准确地朝向极值所在的方向。其二，由于不同权重的梯度值差别巨大，因此选取一个全局的学习率很困难。 
　　 ②Batch_Size = 1。这就是在线学习（Online Learning）。使用在线学习，每次修正方向以各自样本的梯度方向修正，横冲直撞各自为政，难以达到收敛。 
　　③如果网络中采用minibatch SGD[算法](http://lib.csdn.net/base/datastructure)来优化，所以是一个batch一个batch地将数据输入CNN模型中，然后计算这个batch的所有样本的平均损失，即代价函数是所有样本的平均。而batch_size就是一个batch的所包含的样本数，显然batch_size将影响到模型的优化程度和速度。mini
 batch只是为了充分利用GPU memory而做出的妥协 
３.２改变Batch_Size的影响
　　在合理范围内，增大 Batch_Size 的好处：内存利用率提高了，大矩阵乘法的并行化效率提高。跑完一次 epoch（全数据集）所需的迭代次数减少，对于相同数据量的处理速度进一步加快。在一定范围内，一般来说 Batch_Size 越大，其确定的下降方向越准，引起训练震荡越小。 
　　下面是参考文献中博主给出的实验结果： 
　　Batch_Size 太小，算法在 200 epoches 内不收敛。 
　　随着 Batch_Size 增大，处理相同数据量的速度越快。 
　　随着 Batch_Size 增大，达到相同精度所需要的 epoch 数量越来越多。 
　　由于上述两种因素的矛盾， Batch_Size 增大到某个时候，达到时间上的最优。 
　　由于最终收敛精度会陷入不同的局部极值，因此 Batch_Size 增大到某些时候，达到最终收敛精度上的最优。３.3 Minibatch简谈
　　首先说一下采用mini-batch时的权重更新规则。比如mini-batch size设为100，则权重更新的规则为： 
![这里写图片描述](https://img-blog.csdn.net/20160424101904528)
　　也就是将100个样本的梯度求均值，替代online learning方法中单个样本的梯度值： 
![这里写图片描述](https://img-blog.csdn.net/20160424101915111)
　　当采用mini-batch时，我们可以将一个batch里的所有样本放在一个矩阵里，利用线性代数库来加速梯度的计算，这是工程实现中的一个优化方法。 
　　一个大的batch，可以充分利用矩阵、线性代数库来进行计算的加速，batch越小，则加速效果可能越不明显。当然batch也不是越大越好，太大了，权重的更新就会不那么频繁，导致优化过程太漫长。
## 第四个问题：梯度弥散
４.１　梯度弥散的解释
　　梯度弥散的问题很大程度上是来源于激活函数的“饱和”。因为在后向传播的过程中仍然需要计算激活函数的导数，所以一旦卷积核的输出落入函数的饱和区，它的梯度将变得非常小。 
梯度弥散。使用反向传播算法传播梯度的时候，随着传播深度的增加，梯度的幅度会急剧减小，会导致浅层神经元的权重更新非常缓慢，不能有效学习。这样一来，深层模型也就变成了前几层相对固定，只能改变最后几层的浅层模型。 
４.２　如何解决梯度弥散
　　一种新的方法batch normalization，相关论文： 
《Batch Normalization: Accelerating Deep Network Training by Reducing　Internal Covariate Shift》
　　顾名思义，batch normalization嘛，就是“批规范化”咯。Google在ICML文中描述的非常清晰，即在每次SGD时，通过mini-batch来对相应的activation做规范化操作，使得结果（输出信号各个维度）的均值为0，方差为1. 而最后的“scale and shift”操作则是为了让因训练所需而“刻意”加入的BN能够有可能还原最初的输入，从而保证整个network的capacity。 
![这里写图片描述](https://img-blog.csdn.net/20160423220313299)
　　BN可以应用于网络中任意的activation set。文中还特别指出在CNN中，BN应作用在非线性映射前，即对x=Wu+b做规范化。另外对CNN的“权值共享”策略，BN还有其对应的做法（详见文中3.2节）。 
　　那BN到底是什么原理呢？说到底还是为了防止“梯度弥散”。关于梯度弥散，大家都知道一个简单的栗子：。在BN中，是通过将activation规范为均值和方差一致的手段使得原本会减小的activation的scale变大。可以说是一种更有效的local response normalization方法。
参考文献
１.谈谈[深度学习](http://lib.csdn.net/base/deeplearning)中的 Batch_Size 
[http://blog.csdn.net/ycheng_sjtu/article/details/49804041](http://blog.csdn.net/ycheng_sjtu/article/details/49804041)
２.正则化方法：L1和L2 regularization、数据集扩增、dropout 
[http://blog.csdn.net/u010402786/article/details/49592239](http://blog.csdn.net/u010402786/article/details/49592239)
3.深度学习中 Batch Normalization为什么效果好？ 
[http://www.zhihu.com/question/38102762](http://www.zhihu.com/question/38102762)
