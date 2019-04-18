# 受限玻尔兹曼机（RBM）理解 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年02月23日 11:30:31[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：17675








受限玻尔兹曼机（RBM）多见深度学习，不过笔者发现推荐系统也有相关专家开始应用RBM。实际上，作为一种概率图模型，用在那，只要场景和数据合适都可以。有必要就RBM做一个初步了解。

1、 RBM定义

RBM记住三个要诀：1）两层结构图，可视层和隐藏层；2）同层无边，上下层全连接；3）二值状态值，前向反馈和逆向传播求权参。定义如下：



RBM包含两个层，可见层（visible layer）和隐藏层（hidden layer）。神经元之间的连接具有如下特点：**层内无连接，层间全连接，显然RBM对应的图是一个二分图**。一般来说，可见层单元用来描述观察数据的一个方面或一个特征，而隐藏层单元的意义一般来说并不明确，可以看作特征提取层。RBM和BM的不同之处在于，BM允许层内神经元之间有连接，而RBM则要求层内神经元之间没有连接，因此RBM的性质：**当给定可见层神经元的状态时，各隐藏层神经元的激活条件独立；反之当给定隐藏层神经元的状态是，可见层神经元的激活也条件独立**。



![](https://pic1.zhimg.com/80/v2-e0fa15857850847afb3a7c0b15e6706b_hd.jpg)
如图给出了一个RBM网络结构示意图。其中：![n_{v},n_{h}](https://www.zhihu.com/equation?tex=n_%7Bv%7D%2Cn_%7Bh%7D)分别表示可见层和隐藏层中包含神经元的数目，下标v，h代表visible和hidden；![v=(v_{1},v_{2},...,v_{n_{v}},)^{T}](https://www.zhihu.com/equation?tex=v%3D%28v_%7B1%7D%2Cv_%7B2%7D%2C...%2Cv_%7Bn_%7Bv%7D%7D%2C%29%5E%7BT%7D)表示可见层的状态向量；![h=(h_{1},h_{2},...,h_{n_{h}},)^{T}](https://www.zhihu.com/equation?tex=h%3D%28h_%7B1%7D%2Ch_%7B2%7D%2C...%2Ch_%7Bn_%7Bh%7D%7D%2C%29%5E%7BT%7D)表示隐藏层的状态向量；![a=(a_{1},a_{2},...,a_{n_{v}},)^{T}](https://www.zhihu.com/equation?tex=a%3D%28a_%7B1%7D%2Ca_%7B2%7D%2C...%2Ca_%7Bn_%7Bv%7D%7D%2C%29%5E%7BT%7D)表示可见层的偏置向量；![b=(b_{1},b_{2},...,b_{n_{h}},)^{T}](https://www.zhihu.com/equation?tex=b%3D%28b_%7B1%7D%2Cb_%7B2%7D%2C...%2Cb_%7Bn_%7Bh%7D%7D%2C%29%5E%7BT%7D)表示隐藏层的偏置向量；![W=(w_{i,j})\in \Re ^{n_{h}\times n_{v}}](https://www.zhihu.com/equation?tex=W%3D%28w_%7Bi%2Cj%7D%29%5Cin+%5CRe+%5E%7Bn_%7Bh%7D%5Ctimes+n_%7Bv%7D%7D)表示隐藏层和可见层之间的权值矩阵，![w_{i,j}](https://www.zhihu.com/equation?tex=w_%7Bi%2Cj%7D)表示隐藏层中第i个神经元与可见层中第j个神经元之间的连接权重。记![\theta =(W,a,b)](https://www.zhihu.com/equation?tex=%5Ctheta+%3D%28W%2Ca%2Cb%29)表示RBM中的参数，可将其视为把W,a,b中的所有分量拼接起来得到的长向量。

RBM的求解也是基于梯度求对数自然函数。



给定训练样本，RBM的训练意味着调整参数![\theta](https://www.zhihu.com/equation?tex=%5Ctheta)，从而拟合给定的训练样本，使得参数条件下对应RBM表示的概率分布尽可能符合训练数据。

假定训练样本集合为![S=\{v_1,v_2,...,v^{n_s}\}](https://www.zhihu.com/equation?tex=S%3D%5C%7Bv_1%2Cv_2%2C...%2Cv%5E%7Bn_s%7D%5C%7D)，其中![n_s](https://www.zhihu.com/equation?tex=n_s)为训练样本的数目，![v^i=(v_1^i,v_2^i,...,v_{n_v}^i)^T,i=1,2,...,n_s](https://www.zhihu.com/equation?tex=v%5Ei%3D%28v_1%5Ei%2Cv_2%5Ei%2C...%2Cv_%7Bn_v%7D%5Ei%29%5ET%2Ci%3D1%2C2%2C...%2Cn_s)，它们是独立同分布的，则训练RBM的目标就是最大化如下似然![L_{\theta,S}=\prod_{i=1}^{n_s}P(v^i)](https://www.zhihu.com/equation?tex=L_%7B%5Ctheta%2CS%7D%3D%5Cprod_%7Bi%3D1%7D%5E%7Bn_s%7DP%28v%5Ei%29+)，一般通过对数转化为连加的形式，其等价形式：![lnL_{\theta,S}=ln\prod_{i=1}^{n_s}P(v^i) =\sum_{i=1}^{n_s}lnP(v^i)](https://www.zhihu.com/equation?tex=lnL_%7B%5Ctheta%2CS%7D%3Dln%5Cprod_%7Bi%3D1%7D%5E%7Bn_s%7DP%28v%5Ei%29+%3D%5Csum_%7Bi%3D1%7D%5E%7Bn_s%7DlnP%28v%5Ei%29+)。简洁起见，将![L_{\theta,S}](https://www.zhihu.com/equation?tex=L_%7B%5Ctheta%2CS%7D)简记为![L_S](https://www.zhihu.com/equation?tex=L_S)。
最大化![L_S](https://www.zhihu.com/equation?tex=L_S)常用的数值方法是梯度上升法（Gradient Ascent），通过迭代的方法进行逼近，迭代形式：![\theta:=\theta+\eta\frac{\partial lnL_S}{\partial\theta}](https://www.zhihu.com/equation?tex=%5Ctheta%3A%3D%5Ctheta%2B%5Ceta%5Cfrac%7B%5Cpartial+lnL_S%7D%7B%5Cpartial%5Ctheta%7D)，其中![\eta>0](https://www.zhihu.com/equation?tex=%5Ceta%3E0)表示学习速率。其关键就是计算梯度![\frac{\partial lnL_S}{\partial\theta}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+lnL_S%7D%7B%5Cpartial%5Ctheta%7D)（![lnL_S](https://www.zhihu.com/equation?tex=lnL_S)关于各个参数的偏导数![\frac{\partial lnL_S}{\partial w_{i,j}}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+lnL_S%7D%7B%5Cpartial+w_%7Bi%2Cj%7D%7D)，![\frac{\partial lnL_S}{\partial a_i}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+lnL_S%7D%7B%5Cpartial+a_i%7D),![\frac{\partial lnL_S}{\partial b_i}](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5Cpartial+lnL_S%7D%7B%5Cpartial+b_i%7D)）。一般采用MCMC采样来估计，但由于常规的MCMC需要经过许多步的状态转移才能保证采集到的样本符合目标分布。**若我们以训练样本作为起点，就可以仅需要很少次的状态转移抵达RBM的分布**。Hinton教授2002年基于这个上想法发明了**对比散度（Contrastive Divergence，CD）**算法，目前已经成为训练RBM的标准算法。
2、RBM DEMO

引自很好的一个介绍RBM文章，还有python demo，参考：

http://blog.echen.me/2011/07/18/introduction-to-restricted-boltzmann-machines/

为怕链接失效，还是复制过来比较靠谱。



# [Introduction to Restricted Boltzmann Machines](http://blog.echen.me/2011/07/18/introduction-to-restricted-boltzmann-machines/)




Suppose you ask a bunch of users to rate a set of movies on a 0-100 scale. In classical factor analysis, you could then try to explain each movie and user in terms of a set of latent **factors**. For example, movies like Star Wars and Lord of the Rings might have strong associations with a latent science fiction and fantasy factor, and users who like Wall-E and Toy Story might have strong associations with a latent Pixar factor.

Restricted Boltzmann Machines essentially perform a *binary* version of factor analysis. (This is one way of thinking about RBMs; there are, of course, others, and lots of different ways to use RBMs, but I’ll adopt this approach for this post.) Instead of users rating a set of movies on a continuous scale, they simply tell you whether they like a movie or not, and the RBM will try to discover latent factors that can explain the activation of these movie choices.

More technically, a Restricted Boltzmann Machine is a **stochastic neural network** (neural network meaning we have neuron-like units whose binary activations depend on the neighbors they’re connected to; stochastic meaning these activations have a probabilistic element) consisting of:
- One layer of **visible units** (users’ movie preferences whose states we know and set);
- One layer of **hidden units** (the latent factors we try to learn); and
- A bias unit (whose state is always on, and is a way of adjusting for the different inherent popularities of each movie).

Furthermore, each visible unit is connected to all the hidden units (this connection is undirected, so each hidden unit is also connected to all the visible units), and the bias unit is connected to all the visible units and all the hidden units. To make learning easier, we restrict the network so that no visible unit is connected to any other visible unit and no hidden unit is connected to any other hidden unit.

For example, suppose we have a set of six movies (Harry Potter, Avatar, LOTR 3, Gladiator, Titanic, and Glitter) and we ask users to tell us which ones they want to watch. If we want to learn two latent units underlying movie preferences – for example, two natural groups in our set of six movies appear to be SF/fantasy (containing Harry Potter, Avatar, and LOTR 3) and Oscar winners (containing LOTR 3, Gladiator, and Titanic), so we might hope that our latent units will correspond to these categories – then our RBM would look like the following:

![RBM Example](http://i.imgur.com/sadnLks.png)

(Note the resemblance to a factor analysis graphical model.)

# State Activation

Restricted Boltzmann Machines, and neural networks in general, work by updating the states of some neurons given the states of others, so let’s talk about how the states of individual units change. Assuming we know the connection weights in our RBM (we’ll explain how to learn these below), to update the state of unit 


:
- Compute the **activation energy**
 of unit , where the sum runs over all units  that unit  is connected to,  is the weight of the connection between  and , and  is the 0 or 1 state of unit . In other words, all of unit ’s neighbors send it a message, and we compute the sum of all these messages.Let , where 11 is the logistic function. Note that  is close to 1 for large positive activation energies, and  is close to 0 for negative activation energies.We then turn unit  on with probability , and turn it off with probability 1- .
- (In layman’s terms, units that are positively connected to each other try to get each other to share the same state (i.e., be both on or off), while units that are negatively connected to each other are enemies that prefer to be in different states.)

For example, let’s suppose our two hidden units really do correspond to SF/fantasy and Oscar winners.
- If Alice has told us her six binary preferences on our set of movies, we could then ask our RBM which of the hidden units her preferences activate (i.e., ask the RBM to explain her preferences in terms of latent factors). So the six movies send messages to the hidden units, telling them to update themselves. (Note that even if Alice has declared she wants to watch Harry Potter, Avatar, and LOTR 3, this doesn’t guarantee that the SF/fantasy hidden unit will turn on, but only that it will turn on with high probability. This makes a bit of sense: in the real world, Alice wanting to watch all three of those movies makes us highly suspect she likes SF/fantasy in general, but there’s a small chance she wants to watch them for other reasons. Thus, the RBM allows us to generate models of people in the messy, real world.)
- Conversely, if we know that one person likes SF/fantasy (so that the SF/fantasy unit is on), we can then ask the RBM which of the movie units that hidden unit turns on (i.e., ask the RBM to generate a set of movie recommendations). So the hidden units send messages to the movie units, telling them to update their states. (Again, note that the SF/fantasy unit being on doesn’t guarantee that we’ll always recommend all three of Harry Potter, Avatar, and LOTR 3 because, hey, not everyone who likes science fiction liked Avatar.)

# Learning Weights

So how do we learn the connection weights in our network? Suppose we have a bunch of training examples, where each training example is a binary vector with six elements corresponding to a user’s movie preferences. Then for each epoch, do the following:
- Take a training example (a set of six movie preferences). Set the states of the visible units to these preferences.
- Next, update the states of the hidden units using the logistic activation rule described above: for the 
th hidden unit, compute its activation energy , and set  to 1 with probability  and to 0 with probability 1. Then for each edge , compute  (i.e., for each pair of units, measure whether they’re both on).Now **reconstruct** the visible units in a similar manner: for each visible unit, compute its activation energy , and update its state. (Note that this reconstruction may not match the original preferences.) Then update the hidden units again, and compute  for each edge.Update the weight of each edge  by setting , where -  is a learning rate.
- Repeat over all training examples.

Continue until the network converges (i.e., the error between the training examples and their reconstructions falls below some threshold) or we reach some maximum number of epochs.

Why does this update rule make sense? Note that
- In the first phase, 
 measures the association between the th and th unit that we want the network to learn from our training examples;In the “reconstruction” phase, where the RBM generates the states of visible units based on its hypotheses about the hidden units alone, -  measures the association that the network itself generates (or “daydreams” about) when no units are fixed to training data.

So by adding 

 to each edge weight, we’re helping the network’s daydreams better match the reality of our training examples.

(You may hear this update rule called **contrastive divergence**, which is basically a fancy term for “approximate gradient descent”.)

# Examples

I wrote a [simple RBM implementation](https://github.com/echen/restricted-boltzmann-machines) in Python (the code is heavily commented, so take a look if you’re still a little fuzzy on how everything works), so let’s use it to walk through some examples.

First, I trained the RBM using some fake data.
- Alice: (Harry Potter = 1, Avatar = 1, LOTR 3 = 1, Gladiator = 0, Titanic = 0, Glitter = 0). Big SF/fantasy fan.
- Bob: (Harry Potter = 1, Avatar = 0, LOTR 3 = 1, Gladiator = 0, Titanic = 0, Glitter = 0). SF/fantasy fan, but doesn’t like Avatar.
- Carol: (Harry Potter = 1, Avatar = 1, LOTR 3 = 1, Gladiator = 0, Titanic = 0, Glitter = 0). Big SF/fantasy fan.
- David: (Harry Potter = 0, Avatar = 0, LOTR 3 = 1, Gladiator = 1, Titanic = 1, Glitter = 0). Big Oscar winners fan.
- Eric: (Harry Potter = 0, Avatar = 0, LOTR 3 = 1, Gladiator = 1, Titanic = 1, Glitter = 0). Oscar winners fan, except for Titanic.
- Fred: (Harry Potter = 0, Avatar = 0, LOTR 3 = 1, Gladiator = 1, Titanic = 1, Glitter = 0). Big Oscar winners fan.

The network learned the following weights:

![Weights](http://i.imgur.com/mbsjJmX.png)

Note that the first hidden unit seems to correspond to the Oscar winners, and the second hidden unit seems to correspond to the SF/fantasy movies, just as we were hoping.

What happens if we give the RBM a new user, George, who has (Harry Potter = 0, Avatar = 0, LOTR 3 = 0, Gladiator = 1, Titanic = 1, Glitter = 0) as his preferences? It turns the Oscar winners unit on (but not the SF/fantasy unit), correctly guessing that George probably likes movies that are Oscar winners.

What happens if we activate only the SF/fantasy unit, and run the RBM a bunch of different times? In my trials, it turned on Harry Potter, Avatar, and LOTR 3 three times; it turned on Avatar and LOTR 3, but not Harry Potter, once; and it turned on Harry Potter and LOTR 3, but not Avatar, twice. Note that, based on our training examples, these generated preferences do indeed match what we might expect real SF/fantasy fans want to watch.

# Modifications

I tried to keep the connection-learning algorithm I described above pretty simple, so here are some modifications that often appear in practice:
- Above, 

 was determined by taking the product of the th and th units after reconstructing the visible units once and then updating the hidden units again. We could also take the product after some larger number of reconstructions (i.e., repeat updating the visible units, then the hidden units, then the visible units again, and so on); this is slower, but describes the network’s daydreams more accurately.Instead of using , where  and  are binary 0 or 1 states, we could also let  and/or  be activation probabilities. Similarly for .We could penalize larger edge weights, in order to get a sparser or more regularized model.When updating edge weights, we could use a momentum factor: we would add to each edge a weighted sum of the current step as described above (i.e., ) and the step previously taken.Instead of using only one training example in each epoch, we could use batches of examples in each epoch, and only update the network’s weights after passing through all the examples in the batch. This can speed up the learning by taking advantage of fast matrix-multiplication algorithms.

文中demo地址：https://github.com/echen/restricted-boltzmann-machines




3、RBM预备知识


理解RBM，需要一些预备知识：


- sigmiod函数，神经网络中常用的激活函数之一，![sigmoid(x)=\frac{1}{1+e^{-x}}](https://www.zhihu.com/equation?tex=sigmoid%28x%29%3D%5Cfrac%7B1%7D%7B1%2Be%5E%7B-x%7D%7D)。
- Bayes定理，![P(A|B)=\frac{P(A,B)}{P(B)}](https://www.zhihu.com/equation?tex=P%28A%7CB%29%3D%5Cfrac%7BP%28A%2CB%29%7D%7BP%28B%29%7D)，![P(B|A)=\frac{P(A,B)}{P(A)}](https://www.zhihu.com/equation?tex=P%28B%7CA%29%3D%5Cfrac%7BP%28A%2CB%29%7D%7BP%28A%29%7D)推出![P(A|B)=P(A)\frac{P(B|A)}{P(B)}](https://www.zhihu.com/equation?tex=P%28A%7CB%29%3DP%28A%29%5Cfrac%7BP%28B%7CA%29%7D%7BP%28B%29%7D)，![P(A)](https://www.zhihu.com/equation?tex=P%28A%29)先验概率（Prior probability）、![P(A|B)](https://www.zhihu.com/equation?tex=P%28A%7CB%29)后验概率（Posterior probability），![\frac{P(B|A)}{P(B)}](https://www.zhihu.com/equation?tex=%5Cfrac%7BP%28B%7CA%29%7D%7BP%28B%29%7D)可能性函数（Likelyhood）。
- 二分图（bipartite graph），图论中一种特殊模型，如果无向图中顶点可分割为两个互不相交的子集，而且图中的每条边关联的两个顶点分别属于这两个不同的顶点集。
- MCMC，蒙特卡罗方法最初用于通过随机化的方法计算积分，假设计算![\int_{a}^{b} h(x)dx](https://www.zhihu.com/equation?tex=%5Cint_%7Ba%7D%5E%7Bb%7D+h%28x%29dx)，如果无法通过数学推导直接求出解析解，一般不可能对区间(a,b)上所有的x值进行枚举，我们可以将h(x)分解为某个函数f(x)和一个定义在(a,b)上的概率密度函数p(x)的乘积，则整个积分可以写成![\int_{a}^{b}h(x)dx=\int_{a}^{b}f(x)p(x)dx=E_{p(x)}[f(x)]](https://www.zhihu.com/equation?tex=%5Cint_%7Ba%7D%5E%7Bb%7Dh%28x%29dx%3D%5Cint_%7Ba%7D%5E%7Bb%7Df%28x%29p%28x%29dx%3DE_%7Bp%28x%29%7D%5Bf%28x%29%5D)，这样原积分等同于f(x)在p(x)这个分布上的均值。这时，如果我们从分布p(x)上采集大量的样本点，这些样本符合分布p(x)，即有![\frac{x_{i}}{\sum_{i=1}^{n}x_{i}}\approx p(x_{i})](https://www.zhihu.com/equation?tex=%5Cfrac%7Bx_%7Bi%7D%7D%7B%5Csum_%7Bi%3D1%7D%5E%7Bn%7Dx_%7Bi%7D%7D%5Capprox+p%28x_%7Bi%7D%29)。那么我们就可以通过这些样本来逼近这个均值![\int_{a}^{b}h(x)dx=\int_{a}^{b}f(x)p(x)dx=E_{p(x)}[f(x)]\approx \frac{1}{n}\sum_{i=1}^{n}f(x_{i})](https://www.zhihu.com/equation?tex=%5Cint_%7Ba%7D%5E%7Bb%7Dh%28x%29dx%3D%5Cint_%7Ba%7D%5E%7Bb%7Df%28x%29p%28x%29dx%3DE_%7Bp%28x%29%7D%5Bf%28x%29%5D%5Capprox+%5Cfrac%7B1%7D%7Bn%7D%5Csum_%7Bi%3D1%7D%5E%7Bn%7Df%28x_%7Bi%7D%29)，这就是蒙特卡罗方法的基本思想。蒙特卡罗方法的核心问题是**如何从分布上随机采集样本**，一般采用马尔可夫链蒙特卡罗方法（Markov Chain Monte Carlo，MCMC）产生指定分布下的样本。
- 马尔可夫链，离散时间上随机变量随时间变化的转移概率仅仅依赖于当前值的序列，MCMC建立的理论基础：**如果我们想在某个分布下采样，只需要模拟以其为平稳分布的马尔科夫过程，经过足够多次转移之后，我们的样本分布就会充分接近于该平稳分布**，也就意味着我们近似地采集目标分布下的样本。
- 正则分布，统计力学的一个基本结论：当系统与外界达到热平衡时，系统处于状态![i](https://www.zhihu.com/equation?tex=i)的概率![p_{i}](https://www.zhihu.com/equation?tex=p_%7Bi%7D)具有以下形式![p_{i}=\frac{1}{Z_{T}} e^{-\frac{E_{i}}{T}}](https://www.zhihu.com/equation?tex=p_%7Bi%7D%3D%5Cfrac%7B1%7D%7BZ_%7BT%7D%7D+e%5E%7B-%5Cfrac%7BE_%7Bi%7D%7D%7BT%7D%7D)，其中![Z_{T}=\sum_{i}{e^{-\frac{E_{i}}{T}}}](https://www.zhihu.com/equation?tex=Z_%7BT%7D%3D%5Csum_%7Bi%7D%7Be%5E%7B-%5Cfrac%7BE_%7Bi%7D%7D%7BT%7D%7D%7D)被称作归一化常数，T为正数，表示系统所处的温度，这种概率分布的形式叫做正则分布。

- Metropolis-Hasting采样，MCMC算法中，为了在一个指定的分布上采样，先从系统的任意一个状态出发，然后模拟马尔可夫过程，不断进行状态转移，根据马尔可夫的性质，经过足够的转移次数之后，我们所处的状态即符合目标分布，此时的状态就可以作为一个采集到的样本。算法的关键就是**设计出合理的状态转移过程**。Metropolist-Hastings是一个非常重要的MCMC采样算法，并且对于设计状态转移过程建立了统一的框架。
对于采样的特定数据场景还是需要进一步理解的。






