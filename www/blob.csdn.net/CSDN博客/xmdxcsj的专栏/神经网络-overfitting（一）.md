# 神经网络-overfitting（一） - xmdxcsj的专栏 - CSDN博客





2015年12月13日 21:30:36[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2467








## 一、过拟合

简单说一下过拟合[1](#fn:footnote)问题。

### （一）、现象

在监督学习中overfit的表现为，随着训练的进行，训练集上面的表现（比如loss function或者accuracy）一直变好，但是在测试集上面的表现却是先变好然后会出现拐点，表现变差。 
![overfit](https://upload.wikimedia.org/wikipedia/commons/1/1f/Overfitting_svg.svg)

上图中的红线表示测试集的误差，蓝线表示训练集的误差。
### （二）、原因

一般是由于模型比较复杂、参数太多，而训练数据相对有限，随着训练的进行，导致训练出的模型过度拟合了有限的训练数据集，从而失去了generalization的能力。

### （三）、解决

为了避免过拟合，在训练模型的时候，可以使用一下方法 

1. cross validation 

2. dropout 

3. 加大数据量 

4. [regularization](http://blog.csdn.net/xmdxcsj/article/details/50286705)
……

## 二、cross-validation

### （一）、定义

通常在监督学习中，训练数据会分为两部分：70%的training set和30%的testing set，称之为“conventional validation”。 

为了解决过拟合的问题，可以选择使用cross-validation的方法，将训练数据分为三部分：training set、testing set和validation set，他们的用途如下：
- training set：用于模型训练
- testing set：用于判断何时模型训练达到预期（比如何时overfit出现）
- validation set：评估训练出的模型的性能

每一轮训练（epoch）结束以后，使用训练出的模型计算validation set数据集的准确度，以此作为依据来判断。当validation set数据集上面的准确度达到饱和的时候，我们就停止训练。这种方法称为**early stopping**[2](#fn:footnote2)。

### （二）、解释

简单解释一下为什么validation可以减弱overfit的影响[3](#fn:footnote3)： 

假设我们只使用training set和testing set，使用testing set的结果去判断什么时候停止训练。那么，此时就有可能导致训练出来的模型对于testing set过拟合，因为模型的训练过程引入了testing set。也就是说最后训练出的模型是跟training set和testing set紧密相关的。所以后续使用testing set对模型做出的评估结果将会相对不客观。 

如果我们引入validation set，当使用testing set模型的评估的时候，就会相对generalization，因为testing set数据集没有参与到模型训练的任何过程。
## 三、dropout

### （一）、定义

对于一个mini-batch来讲，保持输入和输出层不变，在隐层随机删除一半的节点，然后使用修改后的网络进行前向和后向的计算，更新网络的权重，如下图： 
![dropout](http://neuralnetworksanddeeplearning.com/images/tikz31.png)

下一个mini-batch继续随机删除一半的隐层节点，重复以上过程。 

由于训练的时候使用了一半的隐层节点，所以在使用full net（使用全部隐层节点）的时候，每个隐层节点的权重将会除以2。
### （二）、解释

为了提升网络的性能，我们可以选择训练大量的神经网络，然后进行average，但是训练多个神经网络将会非常耗时，所以引出了dropout这种方法[4](#fn:footnote4)。 

使用了dropout以后的神经网络，可以类比成多个神经网络的merge版本。因为每次dropout的节点不同，所以每次的网络的结构将会不同，不同网络可能对应于不同的过拟合，多个网络系统之间的结合将会减弱过拟合的影响，所谓兼听则明。

## 四、增加数据量

当资源有限的时候，可以通过对已有数据进行扩展获得更多的训练数据。比如语音数据加噪音，图片数据旋转等。

- [https://en.wikipedia.org/wiki/Overfitting](https://en.wikipedia.org/wiki/Overfitting)[↩](#fnref:footnote)
- [https://en.wikipedia.org/wiki/Early_stopping](https://en.wikipedia.org/wiki/Early_stopping)[↩](#fnref:footnote2)
- [http://neuralnetworksanddeeplearning.com/chap3.html#overfitting_and_regularization](http://neuralnetworksanddeeplearning.com/chap3.html#overfitting_and_regularization)[↩](#fnref:footnote3)
- [http://arxiv.org/pdf/1207.0580.pdf](http://arxiv.org/pdf/1207.0580.pdf)[↩](#fnref:footnote4)








