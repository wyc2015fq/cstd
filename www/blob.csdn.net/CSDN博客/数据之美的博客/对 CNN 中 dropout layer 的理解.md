# 对 CNN 中 dropout layer 的理解 - 数据之美的博客 - CSDN博客
2017年06月17日 10:11:43[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：345
dropout layer的目的是为了防止CNN 过拟合。那么为什么可以有效的防止过拟合呢？
首先，想象我们现在只训练一个特定的网络，当迭代次数增多的时候，可能出现网络对训练集拟合的很好（在训练集上loss很小），但是对验证集的拟合程度很差的情况。所以，我们有了这样的想法：可不可以让每次跌代随机的去更新网络参数（weights），引入这样的随机性就可以增加网络generalize 的能力。所以就有了dropout 。
在训练的时候，我们只需要按一定的概率（retaining probability）p 来对weight layer 的参数进行随机采样，将这个子网络作为此次更新的目标网络。可以想象，如果整个网络有n个参数，那么我们可用的子网络个数为 2^n 。 并且，当n很大时，每次迭代更新 使用的子网络基本上不会重复，从而避免了某一个网络被过分的拟合到训练集上。
那么[测试](http://lib.csdn.net/base/softwaretest)的时候怎么办呢？ 一种最naive的方法是，我们把
 2^n 个子网络都用来做测试，然后以某种 voting 机制将所有结果结合一下（比如说平均一下下），然后得到最终的结果。但是，由于n实在是太大了，这种方法实际中完全不可行！ 
所以有人提出，那我做一个大致的估计不就得了，我从2^n个网络中随机选取 m 个网络做测试，最后在用某种voting 机制得到最终的预测结果。这种想法当然可行，当m很大时但又远小于2^n时，能够很好的逼近原2^n个网络结合起来的预测结果。但是，有没有更好的办法呢？ of course！那就是dropout 自带的功能，能够通过一次测试得到逼近于原2^n个网络组合起来的预测能力！ 
虽然训练的时候我们使用了dropout， 但是在测试时，我们不使用dropout （不对网络的参数做任何丢弃，这时dropout layer相当于进来什么就输出什么）。然后，把测试时dropout layer的输出乘以训练时使用的retaining probability  p （这时dropout layer相当于把进来的东东乘以p）。仔细想想这里面的意义在哪里呢？？？ 事实上，由于我们在测试时不做任何的参数丢弃，如上面所说，dropout
 layer 把进来的东西原样输出，导致在统计意义下，测试时 每层 dropout layer的输出比训练时的输出多加了【（1 - p）*100】%  units 的输出。 即 【p*100】% 个units 的和  是同训练时随机采样得到的子网络的输出一致，另【（1 - p）*100】%  的units的和  是本来应该扔掉但是又在测试阶段被保留下来的。所以，为了使得dropout layer 下一层的输入和训练时具有相同的“意义”和“数量级”，我们要对测试时的伪dropout layer的输出（即下层的输入）做
 rescale： 乘以一个p，表示最后的sum中只有这么大的概率，或者这么多的部分被保留。这样以来，只要一次测试，将原2^n个子网络的参数全部考虑进来了，并且最后的 rescale 保证了后面一层的输入仍然符合相应的物理意义和数量级。
假设x是dropout layer的输入，y是dropout layer的输出，W是上一层的所有weight parameters，![](http://latex.codecogs.com/gif.latex?W%7C_%7Bp%7D) 是以retaining probability
 为p 采样得到的weight parameter子集。把上面的东西用公式表示（忽略bias）：
train：![](http://latex.codecogs.com/gif.latex?y%3DW%7C_%7Bp%7D*x)
test:![](http://latex.codecogs.com/gif.latex?y%3DW*px)
但是一般写程序的时候，我们想直接在test时用  ![](http://latex.codecogs.com/gif.latex?y%3DW%27*x) ， 这种表达式。（where  ![](http://latex.codecogs.com/gif.latex?W%27%3DW%5Ccdot%20p)） 
因此我们就在训练的时候就直接训练 ![](http://latex.codecogs.com/gif.latex?W%27) 。 所以训练时，第一个公式修正为  ![](http://latex.codecogs.com/gif.latex?y%3D%5Cfrac%7BW%27%7C_%7Bp%7D%7D%7Bp%7D*x%3DW%27%7C_%7Bp%7D*%5Cfrac%7Bx%7D%7Bp%7D)  。
 即把dropout的输入乘以p 再进行训练，这样得到的训练得到的weight 参数就是 ![](http://latex.codecogs.com/gif.latex?W%27) ，测试的时候除了不使用dropout外，不需要再做任何rescale。Caffe 和Lasagne 里面的代码就是这样写的。
参考文献： Improving Neural Networks with Dropout， Hinton学生的一片thesis
       Dropout: A Simple Way to Prevent Neural Networks from Overfitting
