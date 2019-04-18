# 《deep learning》学习笔记（7）——深度学习中的正则化 - Soul Joy Hub - CSDN博客

2017年10月02日 18:49:10[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：3307
所属专栏：[神经网络与深度学习](https://blog.csdn.net/column/details/16408.html)



[http://blog.csdn.net/u011239443/article/details/78152022](http://blog.csdn.net/u011239443/article/details/78152022)

机器学习中的一个核心问题是设计不仅在训练集上误差小，而且在新样本上泛化能力好的算法。许多机器学习算法都需要采取相应的策略来减少测试误差，这些策略被统称为正则化。而神经网络由于其强大的表示能力经常遭遇过拟合，所以需要使用许多不同形式的正则化策略。

正则化通过对学习算法的修改，旨在减少泛化误差而不是训练误差。目前有很多正则化策略，有些是向机器学习模型中添加限制参数值的额外约束，有些是向目标函数添加额外项来对参数值进行软约束。在本章中我们将更详细地介绍正则化，重点介绍深度模型的正则化策略，包括参数范数惩罚、提前终止、Dropout等等。

# 7.1 参数范数惩罚

![](http://upload-images.jianshu.io/upload_images/1621805-56ad8e25848b951b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 7.1.1 L 2 参数正则化

L2 参数正则化（也称为岭回归、Tikhonov正则）通常被称为权重衰减（weight decay)，是通过向目标函数添加一个正则项  
![](http://upload-images.jianshu.io/upload_images/1621805-2a2af56cda724162.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

使权重更加接近原点。 

目标函数

![](http://upload-images.jianshu.io/upload_images/1621805-6f3a6af2219a3cf9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

计算梯度

![](http://upload-images.jianshu.io/upload_images/1621805-cd2008ae30191c0f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

更新权重

![](http://upload-images.jianshu.io/upload_images/1621805-30774f334188c4be.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-b9c92b7dc6a039df.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-6343ccaef9888c47.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-e92823ffee43ee6b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-6a2bc79acc6d4c4c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 7.1.2 L1 参数正则化

将 L2 由权重衰减项修改为各个参数的绝对值之和，即得到 L1 正则化： 
![](http://www.zhihu.com/equation?tex=%5COmega%28%5Ctheta%29%3D%5C%7C%5Comega%5C%7C_1%3D%5Csum_i+%7C%5Comega_i%7C)

目标函数：

![](http://upload-images.jianshu.io/upload_images/1621805-b983f084b1e89e07.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

梯度：

![](http://upload-images.jianshu.io/upload_images/1621805-e654851866897ca5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

其中 sgn(w)为符号函数，取各个元素的正负号。与之前的分析一样，将目标函数作二次泰勒展开近似： 
![](http://www.zhihu.com/equation?tex=%5Chat%7BJ%7D%28%5Comega%3BX%2Cy%29%3DJ%28%5Comega%5E%2A+%3BX%2Cy%29%2B%5Cfrac%7B1%7D%7B2%7D%28%5Comega-%5Comega%5E%2A%29%5ET+H%28%5Comega+-+%5Comega%5E%2A%29)

我们进一步假设Hessian是对角矩阵，即 ![](http://www.zhihu.com/equation?tex=H%3Ddiag%28%5BH_%7B1%2C1%7D%2C%E2%80%A6%2CH_%7Bn%2Cn%7D%5D%29) ，  
![](http://upload-images.jianshu.io/upload_images/1621805-381bbca4b561e520.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

，于是L1 正则化目标函数二次近似为：

![](http://upload-images.jianshu.io/upload_images/1621805-5201785d60f7c1eb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-7fbf9dbab73b7938.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

最小化这个近似函数，有如下形式的解析解： 
![](http://www.zhihu.com/equation?tex=%5Comega_i%3Dsgn%28%5Comega_i%5E%2A%29max%5C%7B%7C%5Comega_i%5E%2A+%7C-%5Cfrac%7B%5Calpha%7D%7BH_%7Bi%2Ci%7D%7D%2C0%5C%7D)

![](http://upload-images.jianshu.io/upload_images/1621805-f6a39a372ac4d9a7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 7.2 作为约束的范数惩罚

考虑参数范数正则化的代价函数：

![](http://upload-images.jianshu.io/upload_images/1621805-0a1467bc5bac3234.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在上一节中考虑的是无约束问题，如果想约束 ![\Omega(\theta)<k](http://www.zhihu.com/equation?tex=%5COmega%28%5Ctheta%29%3Ck) ，k 是某个常数，可以构造广义Lagrange函数

![](http://upload-images.jianshu.io/upload_images/1621805-b77afc4998b7149a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

该约束问题的解是

![](http://upload-images.jianshu.io/upload_images/1621805-80a364705b7a0b37.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-1be97b100dc56741.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 7.3 正则化和欠约束问题

机器学习中许多线性模型，如线性回归和PCA，都依赖与矩阵 X^TX 求逆，如果 X^TX 不可逆，这些方法就会失效。这种情况下，正则化的许多形式对应求逆 
![](http://upload-images.jianshu.io/upload_images/1621805-6373d2a4498d5941.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

 ，这个正则化矩阵是可逆的。大多数正则化方法能够保证应用于欠定问题的迭代方法收敛。

# 7.4 数据集增强

让机器学习模型泛化得更好的最好办法是使用更多的数据进行训练，因此需要在有限的数据中创建假数据并添加到训练集中。数据集增强在对象识别领域是特别有效的方法。
- 数据集的各种变换，如对图像的平移、旋转和缩放。
- 在输入层注入噪声，也可以看作数据集增强的一种方法（如去噪自编码器）。通过将随机噪声添加到输入再进行训练能够大大改善神经网络的健壮性。

# 7.5 噪声鲁棒性
- 将噪声加入到输入。在一般情况下,注入噪声远比简单地收缩参数强大,特别是噪声被添加到隐藏单元时会更加强大（如Dropout）。对于某些模型而言，向输入添加方差极小的噪声等价于对权重施加范数惩罚。
- 将噪声加入到权重。这项技术主要用于循环神经网络。这可以被解释为关于权重的贝叶斯推断的随机实现。贝叶斯学习过程将权重视为不确定的,并且可以通过概率分布表示这种不确定性，向权重添加噪声是反映这种不确定性的一种实用的随机方法。

![](http://upload-images.jianshu.io/upload_images/1621805-82e5dbb4ccc3cc2c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 7.6 半监督学习

![](http://upload-images.jianshu.io/upload_images/1621805-93d024e56c18ad90.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 7. 多任务学习
- 多任务学习是通过合并几个任务中的样例(可以视为对参数施加的软约束)来提高泛化的一种方式。当模型的一部分被多个额外的任务共享时，这部分将被约束为良好的值，通常会带来更好的泛化能力。

![](http://upload-images.jianshu.io/upload_images/1621805-7ed00577b7cf9630.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

- 从深度学习的观点看，底层的先验知识为：能解释数据变化的因素中，某些因素是跨多个任务共享的。

# 7.8 提前终止

![](http://upload-images.jianshu.io/upload_images/1621805-e02e8d67324d5e16.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-b5ef07f795748fb6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-8e4f89abe8522b64.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-f7a12002114145fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-c4123f5ca8df8b31.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 7.9 参数绑定和参数共享

参数范数惩罚或约束是相对于固定区域或点，如 L2 正则化是对参数偏离0 固定值进行惩罚。但有时我们需要对模型参数之间的相关型进行惩罚，使模型参数尽量接近或者相等。

## 7.9.1 卷积神经网络
- 参数共享：强迫模型某些参数相等
- 主要应用：卷积神经网络（CNN）
- 优点：显著降低了CNN模型的参数数量（CNN模型参数数量经常是千万量级以上），减少模型所占用的内存，并且显著提高了网络大小而不需要相应的增加训练数据。

# 7.10 稀疏表示

稀疏表示也是卷积神经网络经常用到的正则化方法。 L1 正则化会诱导稀疏的参数，使得许多参数为0；而稀疏表示是惩罚神经网络的激活单元，稀疏化激活单元。换言之，稀疏表示是使得每个神经元的输入单元变得稀疏，很多输入是0。 

例如下图， h_3 只依赖于上一层的3个神经元输入 x_2 、 x_3 、 x_4 ，而其他神经元到 h_3的输入都是0。

![](http://upload-images.jianshu.io/upload_images/1621805-d7f8f35ebb748435.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 7.11 Bagging 和其他集成方法

Bagging(bootstrap aggregating)是通过结合几个模型降低泛化误差的技术。主要想法是分别训练几个不同的模型，然后让所有模型表决测试样例的输出。这是机器学习中常规策略的一个例子,被称为模型平均(model averaging)。采用这种策略的技术被称为集成方法。 

Bagging是一种允许重复多次使用同一种模型、训练算法和目标函数的方法。具体来说,Bagging涉及构造 k 个不同的数据集。每个数据集从原始数据集中重复采样构成，和原始数据集具有相同数量的样例。 

模型平均是一个减少泛化误差的非常强大可靠的方法。例如我们假设有k个回归模型，每个模型误差是 epsilon_i，误差服从零均值、方差为 v、协方差为 c 的多维正态分布。则模型平均预测的误差为 ![](http://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7Bk%7D%5Csum_i%5Cepsilon_i) ，均方误差的期望为

![](http://upload-images.jianshu.io/upload_images/1621805-a860c7ff3d9d48b5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

在误差完全相关即 c=v 的情况下，均方误差为 v ，模型平均没有帮助。在误差完全不相关即 c=0 时，模型平均的均方误差的期望仅为 (1/k)*v 。这说明集成平方误差的期望随集成规模的增大而线性减少。

![](http://upload-images.jianshu.io/upload_images/1621805-d1a8d0309991bda1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

其他集成方法，如Boosting，通过向集成逐步添加神经网络，可以构建比单个模型容量更高的集成模型。

# 7.12 Dropout

参阅： 
[http://blog.csdn.net/u011239443/article/details/76360294#t42](http://blog.csdn.net/u011239443/article/details/76360294#t42)

## Dropout与Bagging区别
- Bagging：所有模型都是独立的。
- 
Dropout：所有模型共享参数，其中每个模型继承父神经网络参数的不同子集。参数共享使得在有限内存下表示指数级数量的模型变得可能。

- 
Bagging：每个模型在其相应的训练集上训练到收敛。

- Dropout：大部分模型没有被显式地被训练，因为父神经网络通常很大，几乎不可能采样完指数级数量的子网络；取而代之的是，在单个步骤中训练一小部分子网络，通过参数共享使得剩余的子网络也有好的参数设定。

## Dropout优缺点
- 
Dropout优点
- 计算方便。训练过程中使用Dropout产生 n 个随机二进制数与状态相乘即可。每个样本每次更新的时间复杂度： O(n)，空间复杂度： O(n)。
- 适用广。Dropout不怎么限制适用的模型或训练过程，几乎在所有使用分布式表示且可以用随机梯度下降训练的模型上都表现很好。包括：前馈神经网络、概率模型、受限波尔兹曼机、循环神经网络等。
- 相比其他正则化方法（如权重衰减、过滤器约束和稀疏激活）更有效。也可与其他形式的正则化合并，得到进一步提升。

- 
Dropout缺点
- 不适合宽度太窄的网络。否则大部分网络没有输入到输出的路径。
- 不适合训练数据太小（如小于5000）的网络。训练数据太小时，Dropout没有其他方法表现好。
- 不适合非常大的数据集。数据集大的时候正则化效果有限（大数据集本身的泛化误差就很小），使用Dropout的代价可能超过正则化的好处。

# 7.13 对抗训练

对抗样本主要用于计算机安全领域。在正则化背景下，通过对抗训练（adversarial training）可以减少原有独立同分布的测试集的错误率——在对抗扰动的训练集样本上训练网络。 

主要原因之一是高度线性，神经网络主要是基于线性模块构建的。输入改变 epsilon ，则权重为 w 的线性函数将改变 ![](http://www.zhihu.com/equation?tex=%5Cepsilon%7C%7C%5Comega%7C%7C_1) ，对于高维的 w 这是一个非常大的数。 

对抗训练通过鼓励网络在训练数据附件的局部区域恒定来限制这一个高度敏感的局部线性行为。

![](http://upload-images.jianshu.io/upload_images/1621805-3998ad0aee62f379.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-bbd4d8ae36190647?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

