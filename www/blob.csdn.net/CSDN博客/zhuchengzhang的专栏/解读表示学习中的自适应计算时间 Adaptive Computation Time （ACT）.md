# 解读表示学习中的自适应计算时间 Adaptive Computation Time （ACT） - zhuchengzhang的专栏 - CSDN博客





2019年04月07日 10:17:42[无声云泪](https://me.csdn.net/zhuchengzhang)阅读数：80标签：[深度学习																[模型自适应](https://so.csdn.net/so/search/s.do?q=模型自适应&t=blog)](https://so.csdn.net/so/search/s.do?q=深度学习&t=blog)
个人分类：[表示学习](https://blog.csdn.net/zhuchengzhang/article/category/8828590)








最近在学习网红模型[Universal Transformers](https://arxiv.org/pdf/1807.03819.pdf)时接触到了自适应计算时间（Adaptive Computation Time, ACT）这个新奇的算法。查了一下，这个算法其实并不算新，2016年就被Google DeepMind组的Alex Graves在论文[《Adaptive Computation Time for Recurrent Neural Networks》](https://arxiv.org/pdf/1603.08983.pdf)中提出。但是如果从引用量来说，这篇论文确实还算很新的，截止到发稿时止，该论文的引用量仅为113次。这个引用量对于Transormers模型的红火程度来说确实太少了。那为何ACT算法引用数如此少呢？是太难理解没有流传？还是适用范围太小？抑或是并没有那么实用的效果呢？本文就来解读一下这个ACT算法。

# 自适应计算时间ACT的用途

上来先划重点，说说这个方法有什么用吧。这个方法最直接的用途是**控制RNN模型中每一个时刻重复运算的次数**。如果不太理解的话，可以看做是**控制RNN在每一个时刻状态计算网络的深度**。扩展来说，ACT算法的思想还可以用到**自动控制深度网络的深度**，甚至**控制模型的复杂度**。

# 自适应计算时间ACT的原理

## 问题定义

考虑一个传统的递归神经网络（recurrent neural network）$\mathcal{R}$，这个网络由一个输入权重矩阵（maxtrix of input weights）$W_x$，一个参数化的状态转移模型（parametric state transition model）$\mathcal{S}$，一组输出权重（a set of output weights）$W_y$，以及一个输出偏置量（output bias）$b_y$组成。当输入一个序列$\mathbf{x} = (x_1, \cdots, x_T)$时，$\mathcal{R}$在时间$t=1$到$T$通过迭代如下公式来计算状态序列$\mathbf{s} = (s_1, \cdots, s_T)$和输出序列$\mathbf{y} = (y_1, \cdots, y_T)$：

$$(1)s_t = \mathcal{S}(s_{t-1}, W_xx_t)  \tag{1}$$

$$(2)y_t = W_ys_t +b_y  \tag{2}$$

在这儿，公式[(1)](#eq1)中得到的状态$s_t$是一个向量，包含着序列的动态信息。我们可以理解公式[(1)](#eq1)中$\mathcal{S}$模型将当前的输入$x_t$和上一时刻的状态$s_{t-1}$按照某种关系进行结合，从而产生了当前状态$s_t$。当前状态$s_t$又经过某一种线性变换，即公式[(2)](#eq2)，（也可以不变换，如GRU中，此时$W_y$固定为全1矩阵，$b_y$固定为0）得到了最终的输出结果$y_t$。这也是RNN网络的一个基本思路。根据不同的模型$\mathcal{S}$和不同的线性变化方式，就可以得到不同的RNN网络实例化，如LSTM, GRU，NTM等等。

![传统递归神经网络RNN结构图](https://img-blog.csdnimg.cn/20190407092859178.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podWNoZW5nemhhbmc=,size_12,color_FFFFFF,t_70)**图1**: 传统递归神经网络RNN结构图

自适应时间计算（ACT）修改了传统的递归神经网络，使得$\mathcal{R}$对于每一个时刻的输入$x_t$可以计算多个状态$(s_t^{1}, \cdots, s_t^{N(t)})$，以及多个输出$(y_t^1, \cdots, y_t^{N(t)})$，此处$N(t)$表示$\mathcal{R}$在$t$时刻计算的状态和输出个数。需要注意的是，这儿的多个状态之间存在着序列依赖关系，即$s_t^{n}$的值依赖于$s_t^{n-1}$。形式上，多个状态和输出可由如下公式计算得出：
$$(3)s_t^n =  \begin{cases} 		\mathcal{S}(s_{t-1}, x_t^1) & if\quad n = 1\\		\mathcal{S}(s_t^{n-1}, x_t^n) & otherwise				\end{cases} \tag{3}$$

$$(4)y_t^n = W_ys_t^n + b_y \tag{4}$$

此处$x_t^n = x_t +\delta_{n,1}$是在$t$时刻的输入$x_t$的修正值。根据[原文](https://arxiv.org/pdf/1603.08983.pdf)中的说法，$\delta_{n,1}$是一个二值标志位用以指示当前状态的输入量在状态计算时是否进行了多步，使得神经网络可以区分是否是更新的输入量或者是对于一个输入量重复的计算。此处我还没有理解透: （1）$x_t^n$ 是在第一步之后都保持为$x_t$+1还是在第一步之后进行累加得到$x_t+n-1$？（2）为什么这儿可以使得神经网络区分出是否进行了多步计算和对一个输入量的重复计算？是否会对输入的值产生影响？对输入值的范围是否有要求？放上原文供参考：

> 
where $x_t^n = x_t +\delta_{n,1}$ is the input at time $t$ augmented with a binary flag that indicates whether the input step has just been incremented, allowing the network to distinguish between repeated inputs and repeated computations for the same input.


这儿值得注意的是，对于某一时刻的多步运算，和不同时刻的运算，采用的模型$\mathcal{S}$都是一样的。同样，对于某一时刻的多个输出，和不同时刻的输出，采用的线性变换也是一样的。虽然可以采用不同的模型和线性变换来处理同一时刻的多步运算以及不同时刻的运算，但是会显著增加模型的参数、复杂度、运算时间开销。值得思考的是，如果运用的模型是一样的话，会对于结果产生什么影响呢？性能损失会有多大？

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190407093251618.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podWNoZW5nemhhbmc=,size_16,color_FFFFFF,t_70)
**图2**: 采用ACT算法的递归神经网络RNN结构图


## 为何要对某一时刻的输入重复多次状态运算

在我看来，对于同一个输入的重复运算，降低了模型的复杂性和学习难度。从直观上说这相当于把复杂的问题简单化，复杂的状态通过多步运算来模拟。从理论上说，相当于通过多个非线性运算来逼近一个复杂的函数，与深度学习加深网络深度得到更好表示能力的原理是一致的。那么，接下来的问题是，**需要重复多少次运算（需要多深的网络）才能让网络的学习能力适应数据的复杂性**？深度太浅，网络不能很好的学到数据中所有的复杂关系；网络太深，又可能存在过拟合或者不好训练的问题。通常情况下，对于一个深度网络，大家都会将网络的深度作为一个超参数，通过不断调整这个超参数来找到一个适用于数据复杂性的网络。但是，这毕竟费时费力，特别是对于大数据的学习来说，通常很难在有限时间内找到一个较优的模型深度。因此，让模型自适应的确定网络深度在深度学习中显得尤为重要。而本文所介绍的自适应计算时间ACT就是用来解决这个问题的。

## 如何控制某一时刻重复运算的次数（网络深度）

自适应计算时间ACT通过在某一时刻的每一次重复运算的输出中引入一个额外的**停止单元（halting unit）**$h$来自动控制重复运算的次数。某一次重复运算中的停止单元$h$由这次运算得到的状态$s$和一个单层的以sigmoid为激活函数的神经网络决定，具体公式如下：
$$(5)h_t^n = \sigma(W_hs_t^n + b_h) \tag{5}$$

而后，这个停止单元$h$被用来计算每一次重复运算的**停止概率（halting probability）**$p_t^n$：
$$(6)p_t^n =  \begin{cases} 		R(t) & if\quad n = N(t)\\		h_t^n & otherwise				\end{cases} \tag{6}$$

其中
$$(7)N(t) = \min\{n': \sum\limits_{n=1}^{n'}h_t^n \geq 1- \epsilon\} \tag{7}$$

$R(t)$被称为残余量（remainder），定义如下：

$$(8)R(t) = 1 - \sum\limits_{n=1}^{N(t)-1}h_t^n \tag{8}$$

$\epsilon$是一个很小的常量（[原文](https://arxiv.org/pdf/1603.08983.pdf)中设定为0.01）用于使重复计算可以在第一次计算后就终止（如果$h_t^1 \geq 1 - \epsilon$）。据此，根据公式[(6)](#eq6)的定义，$p_t^n$是一个概率分布函数，满足$\sum\limits_{n=1}^{N(t)}p_t^n = 1$并且$0\leq p_t^n \leq 1 \quad \forall n$。这儿可以多说两句，公式[(7)](#7)可以理解为当在第$n'$次重复运算之后如果前面各次重复运算累计的停止概率非常大则没有必要继续再重复运算了（有极大的概率在第$n'$词重复运算前就终止）。此时，第$n'$步的停止概率，不仅仅需要考虑计算出来$h_t^{n'}$，还要考虑之后所有可能重复运算的概率之和，这两部分之和就是公式[(8)](#eq8)所计算的残余量。

由此我们可以看到，根据公式[(6)](#eq6)中的概率公式，我们就可以采样出所需的神经网络深度啦。看到这儿，我们发现所谓自适应计算时间ACT算法，本质上就是引入了一个额外的神经网络来计算每一次重复运算（类比于深度网络中的每一层）的停止概率，而这儿神经网络的输入就是上一次运算的输出（类别与深度网络中上一层的输出）。是不是觉得这儿有点像**把注意力机制（Attention mechanism）应用在了深度网络的层数控制上了**（由此联想可以扩展很多工作哦）。

## 如何得到自适应计算次数后的输出结果

虽然上文中构建了重复计算次数的控制模型，但是还没有提到如何采样，以及如何将采样融入到深度网络的训练之中。实际上，如果按照上述公式进行随机采样运算的话会在网络训练时导致非常大的噪音梯度值（noisy gradients），因此ACT方法使用了**平均场**的思想来得到自适应计算次数后的输出结果。换而言之，ACT算法并没有对重复运算次数进行采样，而是根据重复运算的停止概率对多个重复运算结果进行加权平均，即计算了按照重复运算停止概率采样运算次数得到结果的**期望值**。具体公式如下：
$$(9)s_t = \sum\limits_{n=1}^{N(t)}p_t^n s_t^n, \quad y_t=\sum\limits_{n=1}^{N(t)}p_t^ny_t^n \tag{9}$$

当然，这里面是包含着一个假设：*状态和输出向量是近似线性的*。具体的假设合理性这儿我们就不多阐释了，[原文](https://arxiv.org/pdf/1603.08983.pdf)说得很清楚，有兴趣的朋友可以翻看。

## 对于学到重复运算次数（网络层数）的约束

上文中讨论了自适应计算时间ACT算法的原理，本质上就是额外的引入了控制每一次运算（每一层网络）停止概率运算的一个神经网络。那么，这个额外引入的神经网络如何学习呢？当然，它可以用最终的任务标签来训练，随着整个网络一起优化学习。但是，可能会出现一个问题，就是学到的重复运算的次数非常非常大（网络非常非常深）。这个很好理解，因为神经网络也想偷懒，想利用尽可能多的运算来减少每一次运算的难度。这样的话，和ACT算法的设计初衷就背道而驰了，可能导致模型的过拟合并增加计算开销。因此，ACT算法中引入了对重复运算次数（网络层数）的约束。这个约束体现在两处，第一处就是在目标函数中增加一个约束性使得学到的重复运算次数（网络层数），第二处是限定一个重复运算次数（网络层数）的最大值。

就第一处约束来说，ACT算法统计了一个**思考序列（ponder sequence）**$(\rho_1, \cdots, \rho_T)$，其中$t$时刻的值为：
$$(10)\rho_t = N(t) + R(t) \tag{10}$$

由这个思考序列，ACT算法计算得到一个**思考损失值(ponder cost)**$\mathcal{P}(x)$：
$$(11)\mathcal{P}(\mathbf{x})=\sum\limits_{t=1}^{T}\rho_t \tag{11}$$

最后，ACT算法将这个思考损失值和原神经网络的损失函数$\mathcal{L}(\mathbf{x}, \mathbf{y})$相加，得到新的损失函数：
$$(12)\hat{\mathcal{L}}(\mathbf{x}, \mathbf{y}) = \mathcal{L}(\mathbf{x}, \mathbf{y})  + \tau\mathcal{P}(\mathbf{x}) \tag{12}$$

此处，$\tau$被称为**时间惩罚参数**（time penalty parameter）用以控制对于重复运算次数约束的重要性。[原文](https://arxiv.org/pdf/1603.08983.pdf)的实验显示，ACT算法的结果对于这个参数$\tau$非常敏感。这应该也是模型可以改进的方向之一吧。

第二处约束在于ACT算法用一个超参数$M$作为重复运算次数的上限，从而减低刚开始训练网络时学到非常多的重复次数造成的额外计算开销。使用超参数$M$后，公式[(7)](#eq7)被修改为:
$$(13)N(t) = \min\{M, \min\{n': \sum\limits_{n=1}^{n'} h_t^n \geq 1 - \epsilon \}\} \tag{13}$$

## 算法的实现

[原文](https://arxiv.org/pdf/1603.08983.pdf)中给出了ACT算法的目标函数公式[(12)](#eq12)如何进行优化，有兴趣的同学可以翻看学习。但是在实际应用中，我们一般采用现有的机器学习框架来完成模型自动的优化求解。ACT算法已经在多个现有流行的深度学习框架平台实现，包括[keras实现](https://github.com/kpot/keras-transformer/blob/master/keras_transformer/transformer.py)，[TensorFlow实现](https://github.com/DeNeutoy/act-tensorflow)，[PyTorch实现](https://github.com/zphang/adaptive-computation-time-pytorch)等等，大家可以直接调用或者参考编码。





