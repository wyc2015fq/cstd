# 当我们在谈论 Deep Learning：DNN 与 Backpropagation

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

57 人赞同了该文章

**本系列意在长期连载分享，内容上可能也会有所增删改减；**

**因此如果转载，请务必保留源地址，非常感谢！**

**知乎专栏：当我们在谈论数据挖掘**

## **引言**

Deep Learning 是众多机器学习算法中的一种。它起源于60年代的 Perceptron，经过80年代的 Artificial Neural Network，现阶段被称为 Deep Learning。迄今为止，是“有监督学习”领域最强大的算法类型，暂时还没有“之一”。同时，它也正在往”无监督“和”强化学习“领域扩散。

除了算法效果突出，Deep Learning 另一个特点则是又一个”应用倒逼理论“的例子。Deep Learning 依靠强大的泛化能力在越来越多的应用领域开花，但是却极其缺乏理论依据。不论是模型的解释性、结构的设计、参数的选择等等，现阶段大多是依靠经验、试错。很多在以前或许被称为 Trick 的手段，在 Deep Learning 中却可能是重要的调参方法。

这个系列意在从 DNN（Deep Neural Network）开始，对 Deep Learning 领域一些常见的算法进行介绍及说明。

## **DNN 的结构**

DNN（Deep Neural Network）的基本结构如下。其中![x](https://www.zhihu.com/equation?tex=x)表示一个训练样本向量，![y](https://www.zhihu.com/equation?tex=y)表示期望的输出向量。



![img](https://pic1.zhimg.com/80/v2-d2603b46273986b2986293e40269d554_hd.png)

先明确本文后续使用的变量的定义。若将其中相邻的两层 Hidden Layers 提取出来，其表示的含义如下。![z^l_i](https://www.zhihu.com/equation?tex=z%5El_i)表示第![l](https://www.zhihu.com/equation?tex=l)层第![i](https://www.zhihu.com/equation?tex=i)个神经元的输入，![a^l_i](https://www.zhihu.com/equation?tex=a%5El_i)表示第![l](https://www.zhihu.com/equation?tex=l)层第![i](https://www.zhihu.com/equation?tex=i)个神经元的输出，![b^l_i](https://www.zhihu.com/equation?tex=b%5El_i)表示第![l](https://www.zhihu.com/equation?tex=l)层第![i](https://www.zhihu.com/equation?tex=i)个神经元输入对应的偏置。![w^l_{ij}](https://www.zhihu.com/equation?tex=w%5El_%7Bij%7D)表示从![a^{l-1}_j](https://www.zhihu.com/equation?tex=a%5E%7Bl-1%7D_j)连接到![z^l_i](https://www.zhihu.com/equation?tex=z%5El_i)的权重，于是![z^l_i](https://www.zhihu.com/equation?tex=z%5El_i)本质就是所有第![l-1](https://www.zhihu.com/equation?tex=l-1)层输出跟![b^l_i](https://www.zhihu.com/equation?tex=b%5El_i)的线性组合。同时，第![l-1](https://www.zhihu.com/equation?tex=l-1)层到第![l](https://www.zhihu.com/equation?tex=l)层的所有权重能写成向量的形式![W^l](https://www.zhihu.com/equation?tex=W%5El)，其中第![i](https://www.zhihu.com/equation?tex=i)列表示从![a^{l-1}_i](https://www.zhihu.com/equation?tex=a%5E%7Bl-1%7D_i)发出的权重。



![img](https://pic1.zhimg.com/80/v2-c5c03ee74d4f132c96dc6422f6a90e4c_hd.png)

![img](https://pic2.zhimg.com/80/v2-aa06a36455d4c25757caf076cfd1ee5d_hd.png)

接下来我们分别用![z^l,a^l,b^l](https://www.zhihu.com/equation?tex=z%5El%2Ca%5El%2Cb%5El)分别表示第![l](https://www.zhihu.com/equation?tex=l)层所有输入构成的向量、所有输出构成的向量、以及所有偏置构成的向量。于是，![z^l](https://www.zhihu.com/equation?tex=z%5El)与![a^{l-1}](https://www.zhihu.com/equation?tex=a%5E%7Bl-1%7D)的关系可以表示为![z^l=W_l*a^{l-1}+b^l](https://www.zhihu.com/equation?tex=z%5El%3DW_l%2Aa%5E%7Bl-1%7D%2Bb%5El)，过程说明如下



![img](https://pic2.zhimg.com/80/v2-f5a2a0aa7ed05d0190cc906216029e61_hd.png)

对于第![l](https://www.zhihu.com/equation?tex=l)层第![i](https://www.zhihu.com/equation?tex=i)个神经元，其输入![z^l_i](https://www.zhihu.com/equation?tex=z%5El_i)与输出![a^l_i](https://www.zhihu.com/equation?tex=a%5El_i)的关系如下图，即经过一个激活函数![\sigma](https://www.zhihu.com/equation?tex=%5Csigma)进行了变换。我们也可以将它们的关系描述成向量的形式，即![a^l=\sigma(z^l)](https://www.zhihu.com/equation?tex=a%5El%3D%5Csigma%28z%5El%29)。



![img](https://pic3.zhimg.com/80/v2-88ebdfa118882ed94cf248a34a833c9a_hd.png)

因此，一个基础的 DNN 的网络就是下图的形式

![img](https://pic4.zhimg.com/80/v2-9691a82ed11dcee543a402b92cb752c7_hd.png)

## **Back Propagation**

当设计好了 DNN 的结构，且有了训练样本，再给出损失函数的定义，接下来就可以求 DNN 中的参数![W](https://www.zhihu.com/equation?tex=W)和![b](https://www.zhihu.com/equation?tex=b)了。而所用的算法，本质就是梯度下降法，这里称为 BP（Back Propagation） 算法。需要注意的是，Deep Learning 中所说的 BP 跟传统的 BP 不完全一样，已经简化成纯粹的梯度下降，更容易理解，以下介绍的也是这种方法。

下图先简单地回顾下求导的链式法则。

![img](https://pic4.zhimg.com/80/v2-aa2220618be853da6c114d5ed2f7c73f_hd.png)

假设我们的训练样本集合为![\{(x^1,\hat y^1),...,(x^r,\hat y^r),...(x^R,\hat y^R)\}](https://www.zhihu.com/equation?tex=%5C%7B%28x%5E1%2C%5Chat+y%5E1%29%2C...%2C%28x%5Er%2C%5Chat+y%5Er%29%2C...%28x%5ER%2C%5Chat+y%5ER%29%5C%7D)，且定义损失函数为![C(\theta)](https://www.zhihu.com/equation?tex=C%28%5Ctheta%29)，其中![\theta](https://www.zhihu.com/equation?tex=%5Ctheta)为损失函数的参数向量，则有

![\begin{align} & C(\theta)=\frac1R\sum_rC^r(\theta) \\ & \nabla C(\theta)=\frac1R\sum_r \nabla C^r(\theta) \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26+C%28%5Ctheta%29%3D%5Cfrac1R%5Csum_rC%5Er%28%5Ctheta%29+%5C%5C+%26+%5Cnabla+C%28%5Ctheta%29%3D%5Cfrac1R%5Csum_r+%5Cnabla+C%5Er%28%5Ctheta%29+%5Cend%7Balign%7D)

对于 DNN 来说，![\theta](https://www.zhihu.com/equation?tex=%5Ctheta)其实只是包含了![w](https://www.zhihu.com/equation?tex=w)和![b](https://www.zhihu.com/equation?tex=b)，所以我们的目的就是求![\partial C^r/\partial w^l_{ij}](https://www.zhihu.com/equation?tex=%5Cpartial+C%5Er%2F%5Cpartial+w%5El_%7Bij%7D)和![\partial C^r/\partial b^l_i](https://www.zhihu.com/equation?tex=%5Cpartial+C%5Er%2F%5Cpartial+b%5El_i)。

我们抽出第![l-1](https://www.zhihu.com/equation?tex=l-1)与第![l](https://www.zhihu.com/equation?tex=l)层的网络，根据链式法则，![\partial C^r/\partial w^l_{ij}](https://www.zhihu.com/equation?tex=%5Cpartial+C%5Er%2F%5Cpartial+w%5El_%7Bij%7D)其实由两部分相乘所得，如下图

![img](https://pic2.zhimg.com/80/v2-7594fead516e46f1690fb43c750f07f5_hd.png)

对于第一项，![\partial z^l_i/\partial w^l_{ij}](https://www.zhihu.com/equation?tex=%5Cpartial+z%5El_i%2F%5Cpartial+w%5El_%7Bij%7D)可以根据下面的方式计算。

当![l>1](https://www.zhihu.com/equation?tex=l%3E1)，即观察对象是中间的两个 Hidden Layer 时，有

![img](https://pic2.zhimg.com/80/v2-0c8f524e82316cc7ae74bf2af4d7d6ed_hd.png)

当![l=1](https://www.zhihu.com/equation?tex=l%3D1)，即观察对象是从输入层到第一个 Hidden Layer时，有

![img](https://pic1.zhimg.com/80/v2-297caf914f02d65f6b6fe7e9becd30f0_hd.png)

归纳起来，即对于第一项，有

![\frac{\partial z^l_i}{\partial w^l_{ij}}= \begin{cases} x^r_j,& \text{if $ l= $1} \\ a^{l-1}_j,& \text{if $ l> $1} \end{cases}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+z%5El_i%7D%7B%5Cpartial+w%5El_%7Bij%7D%7D%3D+%5Cbegin%7Bcases%7D+x%5Er_j%2C%26+%5Ctext%7Bif+%24+l%3D+%241%7D+%5C%5C+a%5E%7Bl-1%7D_j%2C%26+%5Ctext%7Bif+%24+l%3E+%241%7D+%5Cend%7Bcases%7D)

对于第二项，我们将它定义成![\delta^l_i](https://www.zhihu.com/equation?tex=%5Cdelta%5El_i)，即![\delta^l_i=\partial C^r/\partial z^l_i](https://www.zhihu.com/equation?tex=%5Cdelta%5El_i%3D%5Cpartial+C%5Er%2F%5Cpartial+z%5El_i)。当![l<L](https://www.zhihu.com/equation?tex=l%3CL)，即我们观察的是中间某层 Hidden Layer 时，同样根据链式法则，![\delta^l_i](https://www.zhihu.com/equation?tex=%5Cdelta%5El_i)与![\delta^{l+1}_i](https://www.zhihu.com/equation?tex=%5Cdelta%5E%7Bl%2B1%7D_i)有以下关系

![\begin{align} \delta^l_i & = \frac{\partial C^r}{\partial z^l_i} \\ & = \frac{\partial a^l_i}{\partial z^l_i}\frac{\partial C^r}{\partial a^l_i} \\ & = \frac{\partial a^l_i}{\partial z^l_i}\sum_k \frac{\partial z^{l+1}_k}{\partial a^l_i}\frac{\partial C^r}{\partial z^{l+1}_k} \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cdelta%5El_i+%26+%3D+%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+z%5El_i%7D+%5C%5C+%26+%3D+%5Cfrac%7B%5Cpartial+a%5El_i%7D%7B%5Cpartial+z%5El_i%7D%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+a%5El_i%7D+%5C%5C+%26+%3D+%5Cfrac%7B%5Cpartial+a%5El_i%7D%7B%5Cpartial+z%5El_i%7D%5Csum_k+%5Cfrac%7B%5Cpartial+z%5E%7Bl%2B1%7D_k%7D%7B%5Cpartial+a%5El_i%7D%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+z%5E%7Bl%2B1%7D_k%7D+%5C%5C+%5Cend%7Balign%7D)

其中，每一项分别可以表示为

![\begin{align} & \sigma^{\prime}(z^l_i)=\frac{\partial a^l_i}{\partial z^l_i} \\ & w^{l+1}_{ki}=\frac{\partial z^{l+1}_k}{\partial a^l_i} \\ & \delta^{l+1}_k=\frac{\partial C^r}{\partial z^{l+1}_k} \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26+%5Csigma%5E%7B%5Cprime%7D%28z%5El_i%29%3D%5Cfrac%7B%5Cpartial+a%5El_i%7D%7B%5Cpartial+z%5El_i%7D+%5C%5C+%26+w%5E%7Bl%2B1%7D_%7Bki%7D%3D%5Cfrac%7B%5Cpartial+z%5E%7Bl%2B1%7D_k%7D%7B%5Cpartial+a%5El_i%7D+%5C%5C+%26+%5Cdelta%5E%7Bl%2B1%7D_k%3D%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+z%5E%7Bl%2B1%7D_k%7D+%5C%5C+%5Cend%7Balign%7D)

可得

![\begin{align} \delta^l_i & = \frac{\partial a^l_i}{\partial z^l_i}\sum_k \frac{\partial z^{l+1}_k}{\partial a^l_i}\frac{\partial C^r}{\partial z^{l+1}_k} \\ & = \sigma^{\prime}(z^l_i) \sum_k w^{l+1}_{ki} \delta^{l+1}_k \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cdelta%5El_i+%26+%3D+%5Cfrac%7B%5Cpartial+a%5El_i%7D%7B%5Cpartial+z%5El_i%7D%5Csum_k+%5Cfrac%7B%5Cpartial+z%5E%7Bl%2B1%7D_k%7D%7B%5Cpartial+a%5El_i%7D%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+z%5E%7Bl%2B1%7D_k%7D+%5C%5C+%26+%3D+%5Csigma%5E%7B%5Cprime%7D%28z%5El_i%29+%5Csum_k+w%5E%7Bl%2B1%7D_%7Bki%7D+%5Cdelta%5E%7Bl%2B1%7D_k+%5C%5C+%5Cend%7Balign%7D)

当![l=L](https://www.zhihu.com/equation?tex=l%3DL)，即我们观察的是输出层时

![\begin{align} \delta^L_i & = \frac{\partial C^r}{\partial z^L_i} \\ & = \frac{\partial y^r_i}{\partial z^L_i}\frac{\partial C^r}{\partial y^r_i} \\ & = \sigma^{\prime}(z^L_i) \frac{\partial C^r}{\partial y^r_i} \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cdelta%5EL_i+%26+%3D+%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+z%5EL_i%7D+%5C%5C+%26+%3D+%5Cfrac%7B%5Cpartial+y%5Er_i%7D%7B%5Cpartial+z%5EL_i%7D%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+y%5Er_i%7D+%5C%5C+%26+%3D+%5Csigma%5E%7B%5Cprime%7D%28z%5EL_i%29+%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+y%5Er_i%7D+%5C%5C+%5Cend%7Balign%7D)

其中，![\partial C^r/\partial y^r_i](https://www.zhihu.com/equation?tex=%5Cpartial+C%5Er%2F%5Cpartial+y%5Er_i)根据![C^r](https://www.zhihu.com/equation?tex=C%5Er)的定义计算。如定义为![C^r=||y^r-\hat y^r||](https://www.zhihu.com/equation?tex=C%5Er%3D%7C%7Cy%5Er-%5Chat+y%5Er%7C%7C)。

归纳起来，即对于第二项，我们写成向量形式，如下。其中![\odot ](https://www.zhihu.com/equation?tex=%5Codot+)表示向量 element-wise 的乘法（感谢 @雨辰 提醒）。

![\delta^l= \begin{cases} (W^{l+1})^T\delta^{l+1})\odot \sigma^{\prime}(z^l),& \text{if $ l<L $} \\ \nabla_{y^r}C^r\odot \sigma^{\prime}(z^L),& \text{if $ l=L $} \end{cases}](https://www.zhihu.com/equation?tex=%5Cdelta%5El%3D+%5Cbegin%7Bcases%7D+%28W%5E%7Bl%2B1%7D%29%5ET%5Cdelta%5E%7Bl%2B1%7D%29%5Codot+%5Csigma%5E%7B%5Cprime%7D%28z%5El%29%2C%26+%5Ctext%7Bif+%24+l%3CL+%24%7D+%5C%5C+%5Cnabla_%7By%5Er%7DC%5Er%5Codot+%5Csigma%5E%7B%5Cprime%7D%28z%5EL%29%2C%26+%5Ctext%7Bif+%24+l%3DL+%24%7D+%5Cend%7Bcases%7D)

为了更方便理解，其实对于![\delta^l_i](https://www.zhihu.com/equation?tex=%5Cdelta%5El_i)的计算，可以看做是 DNN 网络的反向传播，如下图

![img](https://pic3.zhimg.com/80/v2-d32f803fba7e2f76a061312eb96fa94a_hd.png)

此外，我们还需要计算![\partial C^r/\partial b^l_i](https://www.zhihu.com/equation?tex=%5Cpartial+C%5Er%2F%5Cpartial+b%5El_i)，其推导如下：

![\begin{align} \frac{\partial C^r}{\partial b^l_i} & = \frac{\partial z^l_i}{\partial b^l_i}\frac{\partial C^r}{\partial z^l_i} \\ & = \delta^l_i \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+b%5El_i%7D+%26+%3D+%5Cfrac%7B%5Cpartial+z%5El_i%7D%7B%5Cpartial+b%5El_i%7D%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+z%5El_i%7D+%5C%5C+%26+%3D+%5Cdelta%5El_i+%5Cend%7Balign%7D)

因此，整个 Back Propagation 的过程本质就是先第一层正向计算![\partial z^l_i/\partial w^l_{ij}](https://www.zhihu.com/equation?tex=%5Cpartial+z%5El_i%2F%5Cpartial+w%5El_%7Bij%7D)，再从最后一层反向计算![\partial C^r/\partial z^l_i](https://www.zhihu.com/equation?tex=%5Cpartial+C%5Er%2F%5Cpartial+z%5El_i)，最后求出![\partial C^r/\partial w^l_{ij}](https://www.zhihu.com/equation?tex=%5Cpartial+C%5Er%2F%5Cpartial+w%5El_%7Bij%7D)和![\partial C^r/\partial b^l_i](https://www.zhihu.com/equation?tex=%5Cpartial+C%5Er%2F%5Cpartial+b%5El_i)。也即其名字所示

![img](https://pic2.zhimg.com/80/v2-cdc8150f982f1208d68f983e9bbfd2c1_hd.png)

综上所示，即 Back Propagation 的流程，传说中 BP 的四个公式也在过程中都推导过了，如下

![\begin{align} & \delta^l=(W^{l+1})^T\delta^{l+1})\odot \sigma^{\prime}(z^l) \\ & \delta^L=\nabla_{y^r}C^r\odot \sigma^{\prime}(z^L) \\ & \frac{\partial C^r}{\partial b^l_i}=\delta^l_i \\ & \frac{\partial C^r}{\partial w^l_{ij}}=a^{l-1}_j \delta^l_j \\ \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26+%5Cdelta%5El%3D%28W%5E%7Bl%2B1%7D%29%5ET%5Cdelta%5E%7Bl%2B1%7D%29%5Codot+%5Csigma%5E%7B%5Cprime%7D%28z%5El%29+%5C%5C+%26+%5Cdelta%5EL%3D%5Cnabla_%7By%5Er%7DC%5Er%5Codot+%5Csigma%5E%7B%5Cprime%7D%28z%5EL%29+%5C%5C+%26+%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+b%5El_i%7D%3D%5Cdelta%5El_i+%5C%5C+%26+%5Cfrac%7B%5Cpartial+C%5Er%7D%7B%5Cpartial+w%5El_%7Bij%7D%7D%3Da%5E%7Bl-1%7D_j+%5Cdelta%5El_j+%5C%5C+%5Cend%7Balign%7D)

## **Mini-batch Gradient Descent**

我们都知道 GD（Gradient Descent） 的本质就是希望逐步逼近最优，其迭代公式为

![\theta^i=\theta^{i-1}-\eta\nabla C(\theta^{i-1})](https://www.zhihu.com/equation?tex=%5Ctheta%5Ei%3D%5Ctheta%5E%7Bi-1%7D-%5Ceta%5Cnabla+C%28%5Ctheta%5E%7Bi-1%7D%29)

我们最常用的 GD，是使用所有的训练样本来求梯度，即

![\nabla C(\theta^{i-1})=\frac 1R\sum_r\nabla C^r(\theta^{i-1})](https://www.zhihu.com/equation?tex=%5Cnabla+C%28%5Ctheta%5E%7Bi-1%7D%29%3D%5Cfrac+1R%5Csum_r%5Cnabla+C%5Er%28%5Ctheta%5E%7Bi-1%7D%29)

利用所有的训练样本来求梯度，好处是梯度下降的方向会稳定地朝着极值方向并收敛，不容易受噪声影响；但是问题也比较明显，一个是考虑了所有的数据所有收敛慢，同时容易陷入局部最优。随着数据量的增大，更大的问题就是每更新一次参数，计算量太大；同时，由于考虑了所有数据，收敛就慢。

因此 SGD（Stochastic Gradient Descent） 就应运而生：每次 Iteration 计算梯度并更新参数时只考虑一个样本，对每一个样本执行完这个过程称为一次 Epoch。即

![\theta^i=\theta^{i-1}-\eta\nabla C^r(\theta^{i-1})](https://www.zhihu.com/equation?tex=%5Ctheta%5Ei%3D%5Ctheta%5E%7Bi-1%7D-%5Ceta%5Cnabla+C%5Er%28%5Ctheta%5E%7Bi-1%7D%29)

SGD 的好处就是加快了收敛的速度。问题就是由于根据一个样本求的梯度，方向并不一定指向极值方向；甚至可能出现每一次 Iteration 求出的梯度方向差异巨大，最终无法收敛。

因此 Mini-batch GD（Stochastic Gradient Descent） 又应运而生：每次 Iteration 计算梯度并更新参数时考虑 Batch_Size 个样本（称为一个 Batch），对所有样本执行完这个过程称为一次 Epoch。其公式如下，![b](https://www.zhihu.com/equation?tex=b)表示本次选择的 Batch，![B](https://www.zhihu.com/equation?tex=B)表示 Batch_Size



![\nabla C(\theta^{i-1})=\frac 1B\sum_{x^r\in b}\nabla C^r(\theta^{i-1})](https://www.zhihu.com/equation?tex=%5Cnabla+C%28%5Ctheta%5E%7Bi-1%7D%29%3D%5Cfrac+1B%5Csum_%7Bx%5Er%5Cin+b%7D%5Cnabla+C%5Er%28%5Ctheta%5E%7Bi-1%7D%29)

## **尾巴**

这一部分主要介绍了 DNN 与 Back Propagation，顺带介绍了 Mini-batch GD。其实关于 DNN 还有很多可以说，比如 Activation Function、Weight Decay、Dropout，Mini-batch GD 也有 Rprop、RMSProp 等方法，不过感觉内容还不少，准备独立出来。同时，关于 Deep Learning 的脉络，也即后续怎么写也还在考虑。有建议也欢迎提出。

## **Reference**

1. 文中 PPT 截图出自[李宏毅](http://link.zhihu.com/?target=http%3A//speech.ee.ntu.edu.tw/%7Etlkagk/index.html)老师的课件

## **本系列其他文章：**

Supervised Learning：



[当我们在谈论 Deep Learning：DNN 与 Backpropagation](https://zhuanlan.zhihu.com/p/25794795)

[当我们在谈论 Deep Learning：DNN 与它的参数们（壹）](https://zhuanlan.zhihu.com/p/26122560)

[当我们在谈论 Deep Learning：DNN 与它的参数们（贰）](https://zhuanlan.zhihu.com/p/26392287)

[当我们在谈论 Deep Learning：DNN 与它的参数们（叁）](https://zhuanlan.zhihu.com/p/26682707)

[当我们在谈论 Deep Learning：CNN 其常见架构（上）](https://zhuanlan.zhihu.com/p/27023778)

Unsupervised Learning：



Reinforcement Learning：