# 各种优化方法总结比较（sgd/momentum/Nesterov/adagrad/adadelta） - 数据之美的博客 - CSDN博客
2017年08月20日 05:40:09[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：484

版权声明：本文为博主原创文章，未经博主允许不得转载。
目录[(?)](http://blog.csdn.net/luo123n/article/details/48239963#)[[-]](http://blog.csdn.net/luo123n/article/details/48239963#)
- [前言](http://blog.csdn.net/luo123n/article/details/48239963#t0)
- [SGD](http://blog.csdn.net/luo123n/article/details/48239963#t1)
- [Momentum](http://blog.csdn.net/luo123n/article/details/48239963#t2)
- [Nesterov Momentum](http://blog.csdn.net/luo123n/article/details/48239963#t3)
- [Adagrad](http://blog.csdn.net/luo123n/article/details/48239963#t4)
- [Adadelta](http://blog.csdn.net/luo123n/article/details/48239963#t5)
- [各个方法的比较](http://blog.csdn.net/luo123n/article/details/48239963#t6)
- [其他总结文章](http://blog.csdn.net/luo123n/article/details/48239963#t7)
## 前言
这里讨论的优化问题指的是，给定目标函数f(x)，我们需要找到一组参数x，使得f(x)的值最小。
本文以下内容假设读者已经了解[机器学习](http://lib.csdn.net/base/machinelearning)基本知识，和梯度下降的原理。
## SGD
SGD指stochastic gradient descent，即随机梯度下降。是梯度下降的batch版本。
对于训练数据集，我们首先将其分成n个batch，每个batch包含m个样本。我们每次更新都利用一个batch的数据，而非整个训练集。即： 
其中，为学习率，为x在t时刻的梯度。
这么做的好处在于：
- 当训练数据太多时，利用整个数据集更新往往时间上不显示。batch的方法可以减少机器的压力，并且可以更快地收敛。
- 当训练集有很多冗余时（类似的样本出现多次），batch方法收敛更快。以一个极端情况为例，若训练集前一半和后一半梯度相同。那么如果前一半作为一个batch，后一半作为另一个batch，那么在一次遍历训练集时，batch的方法向最优解前进两个step，而整体的方法只前进一个step。
## Momentum
SGD方法的一个缺点是，其更新方向完全依赖于当前的batch，因而其更新十分不稳定。解决这一问题的一个简单的做法便是引入momentum。
momentum即动量，它模拟的是物体运动时的惯性，即更新的时候在一定程度上保留之前更新的方向，同时利用当前batch的梯度微调最终的更新方向。这样一来，可以在一定程度上增加稳定性，从而学习地更快，并且还有一定摆脱局部最优的能力： 
其中， 即momentum，表示要在多大程度上保留原来的更新方向，这个值在0-1之间，在训练开始时，由于梯度可能会很大，所以初始值一般选为0.5；当梯度不那么大时，改为0.9。 是学习率，即当前batch的梯度多大程度上影响最终更新方向，跟普通的SGD含义相同。 与  之和不一定为1。
## Nesterov Momentum
这是对传统momentum方法的一项改进，由Ilya Sutskever(2012 unpublished)在Nesterov工作的启发下提出的。
其基本思路如下图（转自Hinton的coursera公开课lecture 6a）：
![Nesterov Momentum](https://img-blog.csdn.net/20150906103038485)
首先，按照原来的更新方向更新一步（棕色线），然后在该位置计算梯度值（红色线），然后用这个梯度值修正最终的更新方向（绿色线）。上图中描述了两步的更新示意图，其中蓝色线是标准momentum更新路径。
公式描述为： 
## Adagrad
上面提到的方法对于所有参数都使用了同一个更新速率。但是同一个更新速率不一定适合所有参数。比如有的参数可能已经到了仅需要微调的阶段，但又有些参数由于对应样本少等原因，还需要较大幅度的调动。
Adagrad就是针对这一问题提出的，自适应地为各个参数分配不同学习率的[算法](http://lib.csdn.net/base/datastructure)。其公式如下： 
其中 同样是当前的梯度，连加和开根号都是元素级别的运算。 是初始学习率，由于之后会自动调整学习率，所以初始值就不像之前的算法那样重要了。而是一个比较小的数，用来保证分母非0。
其含义是，对于每个参数，随着其更新的总距离增多，其学习速率也随之变慢。
## Adadelta
Adagrad算法存在三个问题
- 其学习率是单调递减的，训练后期学习率非常小
- 其需要手工设置一个全局的初始学习率
- 更新时，左右两边的单位不同一
Adadelta针对上述三个问题提出了比较漂亮的解决方案。
首先，针对第一个问题，我们可以只使用adagrad的分母中的累计项离当前时间点比较近的项，如下式： 
这里是衰减系数，通过这个衰减系数，我们令每一个时刻的随之时间按照指数衰减，这样就相当于我们仅使用离当前时刻比较近的信息，从而使得还很长时间之后，参数仍然可以得到更新。
针对第三个问题，其实sgd跟momentum系列的方法也有单位不统一的问题。sgd、momentum系列方法中： 
类似的，adagrad中，用于更新的单位也不是x的单位，而是1。
而对于牛顿迭代法： 
其中H为Hessian矩阵，由于其计算量巨大，因而实际中不常使用。其单位为： 
注意，这里f无单位。因而，牛顿迭代法的单位是正确的。
所以，我们可以模拟牛顿迭代法来得到正确的单位。注意到： 
这里，在解决学习率单调递减的问题的方案中，分母已经是的一个近似了。这里我们可以构造的近似，来模拟得到的近似，从而得到近似的牛顿迭代法。具体做法如下： 
可以看到，如此一来adagrad中分子部分需要人工设置的初始学习率也消失了，从而顺带解决了上述的第二个问题。
## 各个方法的比较
Karpathy做了一个这几个方法在MNIST上性能的[比较](http://cs.stanford.edu/people/karpathy/convnetjs/demo/trainers.html)，其结论是： 
adagrad相比于sgd和momentum更加稳定，即不需要怎么调参。而精调的sgd和momentum系列方法无论是收敛速度还是precision都比adagrad要好一些。在精调参数下，一般Nesterov优于momentum优于sgd。而adagrad一方面不用怎么调参，另一方面其性能稳定优于其他方法。
实验结果图如下：
Loss vs. Number of examples seen
![Loss vs. Number of examples seen](https://img-blog.csdn.net/20150906184848267)
Testing Accuracy vs. Number of examples seen
![Testing Accuracy vs. Number of examples seen](https://img-blog.csdn.net/20150906185430051)
Training Accuracy vs. Number of examples seen![这里写图片描述](https://img-blog.csdn.net/20150906185500235)
# 其他总结文章
最近看到了一个很棒的[总结文章](http://sebastianruder.com/optimizing-gradient-descent/)，除了本文的几个算法，还总结了RMSProp跟ADAM（其中ADAM是目前最好的优化算法，不知道用什么的话用它就对了）
