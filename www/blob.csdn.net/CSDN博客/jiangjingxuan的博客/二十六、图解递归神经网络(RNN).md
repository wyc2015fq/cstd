# 二十六、图解递归神经网络(RNN) - jiangjingxuan的博客 - CSDN博客





2017年01月25日 13:55:21[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2901












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_311_default_big.jpeg)



聊天机器人是需要智能的，而如果他记不住任何信息，就谈不上智能，递归神经网络是一种可以存储记忆的神经网络，LSTM是递归神经网络的一种，在NLP领域应用效果不错，本节我们来介绍RNN和LSTM

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 递归神经网络

**递归神经网络**（**RNN**）是两种人工神经网络的总称。一种是**时间递归神经网络**（recurrent neural network），另一种是**结构递归神经网络**（recursive neural network）。时间递归神经网络的神经元间连接构成有向图，而结构递归神经网络利用相似的神经网络结构递归构造更为复杂的深度网络。两者训练的算法不同，但属于同一算法变体（百度百科）。本节我们重点介绍时间递归神经网络，下面提到RNN特指时间递归神经网络。



## 时间递归神经网络

传统的神经网络叫做FNN(Feed-Forward Neural Networks)，也就是前向反馈神经网络，有关传统神经网络的介绍请见《[机器学习教程 十二-神经网络模型的原理](http://www.shareditor.com/blogshow/?blogId=91)》，RNN是在此基础上引入了定向循环，也就是已神经元为节点组成的图中存在有向的环，这种神经网络可以表达某些前后关联关系，事实上，真正的生物神经元之间也是存在这种环形信息传播的，RNN也是神经网络向真实生物神经网络靠近的一个进步。一个典型的RNN是这样的：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/5a3a6de28e660a08c09dc75bc16c38d88f850cd8.png)

图中隐藏层中的节点之间构成了全连接，也就是一个隐藏层节点的输出可以作为另一个隐藏层节点甚至它自己的输入

这种结构可以抽象成：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/489d09ae7d9c122a706ecded04e6a7bde406cf39.png)

其中U、V、W都是变换概率矩阵，x是输入，o是输出

比较容易看出RNN的关键是隐藏层，因为隐藏层能够捕捉到序列的信息，也就是一种记忆的能力

在RNN中U、V、W的参数都是共享的，也就是只需要关注每一步都在做相同的事情，只是输入不同，这样来降低参数个数和计算量

RNN在NLP中的应用比较多，因为语言模型就是在已知已经出现的词的情况下预测下一个词的概率的，这正是一个有时序的模型，下一个词的出现取决于前几个词，刚好对应着RNN中隐藏层之间的内部连接



## RNN的训练方法

RNN的训练方法和传统神经网络一样，都是使用BP误差反向传播算法来更新和训练参数。

因为从输入到最终的输出中间经过了几步是不确定的，因此为了计算方便，我们利用时序的方式来做前向计算，我们假设x表示输入值，s表示输入x经过U矩阵变换后的值，h表示隐藏层的激活值，o表示输出层的值, f表示隐藏层的激活函数，g表示输出层的激活函数：

当t=0时，输入为x0, 隐藏层为h0

当t=1时，输入为x1, s1 = Ux1+Wh0, h1 = f(s1), o1 = g(Vh1)

当t=2时，s2 = Ux2+Wh1, h2 = f(s2), o2 = g(Vh2)

以此类推，st = Uxt + Wh(t-1), ht = f(st), ot = g(Vht)

这里面h=f(现有的输入+过去记忆总结)是对RNN的记忆能力的全然体现

通过这样的前向推导，我们是不是可以对RNN的结构做一个展开，成如下的样子：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/5514cf3cef3c5b2171356ff350dae4e41bd72186.png)

这样从时序上来看更直观明了

下面就是反向修正参数的过程了，每一步输出o和实际的o值总会有误差，和传统神经网络反向更新的方法一样，用误差来反向推导，利用链式求导求出每层的梯度，从而更新参数，反向推导过程中我们还是把神经网络结构看成展开后的样子：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/c015c822cdd69b42766c1aaeeb4d5ab497831f8f.png)

根据链式求导法则，得出隐藏层的残差计算公式为：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/3b7b844fd722b794e06c4c9bd1baf68636695bd9.png)

因此W和U的梯度就是：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/b68efabed77a3b11bb585373350df3ecf155c537.png)

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址



## LSTM(Long Short Tem Momery networks)

特别讲解一下LSTM是因为LSTM是一种特别的RNN，它是RNN能得到成功应用的关键，当下非常流行。RNN存在一个长序列依赖(Long-Term Dependencies)的问题：下一个词的出现概率和非常久远的之前的词有关，但考虑到计算量的问题，我们会对依赖的长度做限制，LSTM很好的解决了这个问题，因为它专门为此而设计。

借用[http://colah.github.io/posts/2015-08-Understanding-LSTMs/](http://colah.github.io/posts/2015-08-Understanding-LSTMs/)中经典的几张图来说明下，第一张图是传统RNN的另一种形式的示意图，它只包含一个隐藏层，以tanh为激发函数，这里面的“记忆”体现在t的滑动窗口上，也就是有多少个t就有多少记忆，如下图

![](http://www.shareditor.com/uploads/media/my-context/0001/01/acb256c2654c0882a3270ae8b9377fddc5146731.png)



那么我们看LSTM的设计，如下，这里面有一些符号，其中黄色方框是神经网络层(意味着有权重系数和激活函数，σ表示sigmoid激活函数，tanh表示tanh激活函数)，粉红圆圈表示矩阵运算(矩阵乘或矩阵加)

![](http://www.shareditor.com/uploads/media/my-context/0001/01/e3004368db50293f66b8bf03fe3cb342e7e365cc.png)

这里需要分部分来说，下面这部分是一个历史信息的传递和记忆，其中粉红×是就像一个能调大小的阀门(乘以一个0到1之间的系数)，下面的第一个sigmoid层计算输出0到1之间的系数，作用到粉红×门上，这个操作表达上一阶段传递过来的记忆保留多少，忘掉多少

其中的sigmoid公式如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/02570e707b30a72965b4a9554ad3ba12f902c71f.png)

可以看出忘掉记忆多少取决于上一隐藏层的输出h{t-1}和本层的输入x{t}

下面这部分是由上一层的输出h{t-1}和本层的输入x{t}得出的新信息，存到记忆中：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/9e39788384b600703ead2f77aad880d16ba6025e.png)

其中包括计算输出值Ct部分的tanh神经元和计算比例系数的sigmoid神经元（这里面既存在sigmoid又存在tanh原因在于sigmoid取值范围是[0,1]天然作为比例系数，而tanh取值范围是[-1,1]可以作为一个输出值）。其中i{t}和Ct计算公式如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/f2cb937dd6c388f9e1b08df79897e160aa665ef5.png)

那么Ct输出就是：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/c897a02a61a3eabd65b3f2364e4ee2df4fdb1866.png)

下面部分是隐藏层输出h的计算部分，它考虑了当前拥有的全部信息（上一时序隐藏层的输出、本层的输入x和当前整体的记忆信息），其中本单元状态部分C通过tanh激活并做一个过滤(上一时序输出值和当前输入值通过sigmoid激活后的系数)

![](http://www.shareditor.com/uploads/media/my-context/0001/01/fdd1613d3074c2a7d5458ae66b3f7fa471a53d3b.png)

计算公式如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/d7a08a9896dab0599a52fbe08ed15c2d3cc4cbf4.png)



LSTM非常适合在NLP领域应用，比如一句话出现的词可以认为是不同时序的输入x，而在某一时间t出现词A的概率可以通过LSTM计算，因为词A出现的概率是取决于前面出现过的词的，但取决于前面多少个词是不确定的，这正是LSTM所做的存储着记忆信息C，使得能够得出较接近的概率。



## 总结

RNN就是这样一种神经网络，它让隐藏层自身之间存在有向环，从而更接近生物神经网络，也具有了存储记忆的能力，而LSTM作为RNN中更有实用价值的一种，通过它特殊的结构设计实现了永久记忆留存，更适合于NLP，这也为将深度学习应用到自然语言处理开了先河，有记忆是给聊天机器人赋予智能的前提，这也为我们的聊天机器人奠定了实践基础。




