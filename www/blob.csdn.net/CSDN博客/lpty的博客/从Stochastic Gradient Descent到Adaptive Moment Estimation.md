
# 从Stochastic Gradient Descent到Adaptive Moment Estimation - lpty的博客 - CSDN博客

2019年02月18日 14:47:25[lpty](https://me.csdn.net/sinat_33741547)阅读数：83



# 引言
深度学习优化算法，基本的发展历程就是：
SGD -> SGDM -> NAG ->AdaGrad -> AdaDelta -> Adam -> Nadam
当说到优化算法时，我们说的是什么？
### 优化算法
对于优化算法，给定一个具有参数θ的目标函数，一般会定义为损失函数，如$L = 1/N ∑ Li$（每个样本损失函数的叠加求均值。
那么优化的目标就是目标函数中的参数θ（是一个集合，θ1、θ2、θ3 …），目标就是找到一个θ使得目标函数取得最大值或最小值。优化算法就是帮助我们找到这个θ的算法。
### 反向传播
反向传播是相对于前向传播而言的，前向传播中将输入信号经过某些变换，转换为输出信号，引入了非线性激活函数对复杂非线性函数进行建模，以拟合任意形状的函数。
反向传播中将损失函数的值，即误差回传。计算计算损失函数$E$相对于权重参数$W$的梯度，在负梯度方向上更新权重。
### 鞍点
在一个光滑函数上，鞍点所在邻域的曲线、曲面或超曲面，都位于这点的切线的不同边。如下，类似马鞍：在x轴方向往上曲，在y轴方向往下曲，鞍点为（0，0）：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190216202930615.png)
鞍点和局部极小值很相似，该点处的梯度都等于零，但在鞍点附近Hessian矩阵是不定的(行列式小于0)，而在局部极值附近的Hessian矩阵是正定的。
[
](https://img-blog.csdnimg.cn/20190216202930615.png)在鞍点附近，基于梯度的优化算法存在严重的问题：
鞍点处的梯度为零，同时鞍点附近存在一片梯度接近于零的平缓区域，会降低神经网络学习速度；
在高维的情况下，这个平坦区域范围可能非常大，可能使梯度下降算法长时间卡在该点附近，很难脱离。而假如鞍点数目非常多，这会导致算法很难收敛。
而高维参数空间就存在大量的鞍点，这导致高维非凸化问题难以优化。
### 优化算法的框架
针对高维参数空间的优化问题，先后提出了许多优化算法，或者说许多基于GD的变体，下面用一个统一的框架描述。
假设有目标函数$f(w)$，待优化参数为$w$，步长为$\alpha$，步骤如下：
1、计算当前参数点处的梯度：$g_t=\nabla f(w_t)$
2、根据历史梯度计算一阶动量及二阶动量：
$m_t=\phi(g_1,g_2,...,g_t)$，$V_t=\psi(g_1,g_2,...,g_t)$
3、计算当前参数的更新量：
$\Delta w=\alpha \frac {m_t} {\sqrt {V_t}}$
4、更新参数：$w_{t+1}=w_t-\Delta w$
# 优化算法简述
### Batch Gradient Descent
BGD采用整个数据集对参数进行更新：
$\theta _{t+1}=\theta _t -\alpha · \nabla_{\theta} J(\theta)$
这种方法在一次更新中对整个数据集计算误差梯度，除以数据集数量得到平均误差梯度，再以此对参数进行更新。由于每次都需要对所有数据进行计算，其速度比较慢，同时很难处理大数据集的问题。
参数更新的快慢由学习率$\alpha$决定，对于凸优化问题可以收敛到全局极小值，在非凸误差曲面上可以收敛到局部极小值。但是在很大的数据集中，很可能存在重复的数据，一次计算整个数据集，就可能存在冗余的参数更新。
### Stochastic Gradient Descent
与BGD一次更新过程中采用整个数据集相比，SGD在更新时只采用一个样本，速度更快同时可以进行在线学习：
$\theta _{t+1}=\theta _t -\alpha · \nabla_{\theta} J(\theta;x_i;y_i)$
在SGD中，可能只使用训练样本中的一部分就把参数$\theta$迭代到最优了，而BGD则需要一次性计算所有样本，同时因为一次更新无法更新参数到最优，还需要多次迭代，速度相比较慢。
但是，SGD在更新过程中，并不是所有的样本都会朝着整体最优的方向，相对来说存在较多噪音，样本中的高方差参数更新可能使得损失函数大幅波动，导致无法收敛到最优点。
相对的，因为SGD更新比较频繁，使得损失函数以不同的强度波动，这种震荡可能使得模型能够发现新的更优的局部极小值；而更新中存在的噪音使得下降过程的曲线更加曲折，这可能使模型具有更强的鲁棒性。
### Mini-Batch Gradient Descent
MBGD更新中一次使用一小批数据进行更新，降低了参数更新过程中的方差，收敛更加稳定，同时利用优化过的矩阵运算加速整体梯度计算，这也是最常用的方法：
$\theta _{t+1}=\theta _t -\alpha · \nabla_{\theta} J(\theta;x_{(i;i+n)};y_{(i;i+n)})$
一般情况下，上述的三种方法都称之为SGD，区别只是在于参数batch size的大小不同，由上面提到的优化算法整体框架而言：
$m_t=g_t$,$V_t=I^2$
因此，$w_{t+1}=w_t-\alpha ·g_t$
上述方法都存在同样的缺点，对学习率$\alpha$较为敏感，如果设置的太小，学习的速度会很慢，但假如设置的太大，将会导致函数在局部极小点的附近来回震荡，无法收敛。应对这种情况，希望在更新前期使用较大的学习率加速下降，而在后期采用较小的学习率以期稳定在局部极小值的附近。
同时，由于参数在更新的过程中，对所有的参数都是保持用一个学习率，不符合预期。一般来说，希望更新过程中，对更新频率较低的参数采用更大的学习率，对频繁更新的参数采用相对小的学习率。
### SGD with Momentum
SGD 在曲面的一个方向比另一个方向更陡，会发生震荡，这种高方差的震荡使得网络很难温稳定收敛。为了抑制SGD的震荡，SGDM在优化过程中加入惯性，通过优化相关方向的训练和弱化无关方向的振荡，来加速SGD训练：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190218005909916.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
在这里引入的一阶动量，如下:
[
](https://img-blog.csdnimg.cn/20190218005909916.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)$m_{t}=\beta _1m_{t-1}+(1-\beta _1)g_t$，$V_t=I^2$
因此，$w_{t+1}=w_t-\alpha ·m_t$
上述一阶动量是各个时刻梯度方向的指数移动平均值，约等于最近$1/(1-\beta _1)$个时刻的梯度向量和的平均值，$\beta _1$的经验值一般为0.9。
指数移动平均的做法可以降低周期性的干扰及应对震荡较为严重的场景，可认为是一种一阶滞后滤波。
但是在初始化时，假设$m_0=0$，那么$m_t$会接近于0，与实际情况存在偏差，需要进行偏差校正：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190218091453372.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
如上图，一般希望拟合曲线是绿色曲线，但实际上可能是紫色曲线，区别在于紫色曲线的起点较低，所以采用下述方法进行偏差校正：
[
](https://img-blog.csdnimg.cn/20190218091453372.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)$m_t=m_t/(1-\beta^t_1)$
[
](https://img-blog.csdnimg.cn/20190218091453372.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)上述修正只对前部分数据作用较大，因为随着$t$的增加，$1-\beta ^{t}$的值也会趋近于1。但是在实际中，有时并不在意前部分曲线拟合的准确度，经常会选择忽视前部分时间的误差，在后部分时间紫色曲线与绿色曲线其实基本是一致的，只是选择偏差校正能够在全程得到更为准确的结果。
[
](https://img-blog.csdnimg.cn/20190218091453372.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
### SGD with Nesterov Acceleration
[
](https://img-blog.csdnimg.cn/20190218091453372.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)同样为了应对SGD在局部最优点的沟壑附近来回震荡，提出了另一种做法NAG。前面说到SGDM利用惯性进行加速，但是假如惯性太大，太快的速度会导致直接冲出了最优点的低谷，所以考虑对梯度进行衡量修正防止冲的太快。NAG考虑按动量方向先走一步，观察该位置的情况再决定如何前进，引入了未来信息。
[
](https://img-blog.csdnimg.cn/20190218091453372.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)NAG在计算梯度时，不在当前位置，而是考虑按累计动量的方向走一步，在该位置上计算梯度，再结合历史累计动量，求得下降梯度：
[
](https://img-blog.csdnimg.cn/20190218091453372.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)$g_t=\nabla f(w_t-\alpha m_{t-1}/\sqrt {V_{t-1}})$
$m_{t}=\beta _1m_{t-1}+(1-\beta _1)g_t$，$V_t=I^2$
因此，$w_{t+1}=w_t-\alpha ·m_t$
如下图，SGDM首先计算一个梯度(短蓝色向量)，再根据累计梯度在更新梯度的方向进行一个大的跳跃(长蓝色向量)；
NAG则首先在累计梯度的方向进行一个大的跳跃(棕色向量)，计算当前位置梯度再进行校正(绿色向量)：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190218094725744.png)
### Adaptive Gradient Algorithm
前面提到的都是为了防止SGD震荡及加速，但还有另一个问题：对于经常更新的参数，已经积累了大量关于它的知识，不希望被单个样本影响太大，希望学习速率慢一些；对于偶尔更新的参数，则了解的信息太少，希望能从每个偶然出现的样本身上多学一些，即学习速率大一些。
所以需要一种自适应学习率的优化算法，AdaGrad其实就是对学习率$\alpha$做了一个约束：
$V_t=\sum _{1} ^{t} g_t ^2$，二阶动量$V_t$为所有历史梯度值的平方和;
$\Delta w=\alpha \frac {m_t} {\sqrt {V_t}}$，则此时的$\alpha$可以认为是$\alpha /\sqrt {V_t}$。
学习率$\alpha$一般默认为0.01。
一般为了避免分母为0，会在分母上加上一个小的平滑项$ϵ＝10e−8$，因此，参数更新越频繁，二阶动量就越大，相应的学习率就会变小。这样就能实现自动的调节学习率，但是由于分母的会不断累计，会导致学习率收缩变得太小，使得训练过程提前结束，即便后续还有数据也无法学到必要的知识。
### Adadelta
Adadelta是AdaGrad的一种变体，前面说到AdaGrad学习率单调递减，累计过去所有的梯度，这里考虑进行改进，只累计窗口大小的梯度，因此用到了指数移动平均值来计算：
$V_{t}=\beta _2V_{t-1}+(1-\beta _2)g_t ^2$，$\beta _2$一般设置为0.9
这样就能够避免梯度持续累计导致学习提前结束的问题。
与AdaGrad还有一点不同，为了提升效率，Adadelta存储的是过往平方梯度的均值，所以实际公式如下：
$E[g^2]_{t}=\beta _2E[g^2]_{t-1}+(1-\beta _2)g_t ^2$
到这里，Adadelta依然依赖于全局的学习率大小，作者将学习率表示为：
$\alpha =\sqrt{\sum _{r=1} ^{r=t-1}\Delta w_r}$
到这里连全局学习率都不需要设置了，所以优化算法的发展历程其实是一个解放炼丹师的过程。
### RMSprop
RMSprop 是 Geoff Hinton 提出的一种自适应学习率方法。与Adadelta很相似，区别只是在与学习率，这里采用手动设置的方式，Hinton 建议设定学习率$\alpha$为0.001.
### Adaptive Moment Estimation
Adam可以认为是前述方法的集大成者，SGDM在SGD的基础上增加了一阶动量，AdaGrad/Adadelta/RMSprop在SGD的基础上增加了二阶动量，而Adam则是在将一阶及二阶动量都利用了起来：
$m_{t}=\beta _1m_{t-1}+(1-\beta _1)g_t$
$V_t=\beta _2V_{t-1}+(1-\beta _2)g_t ^2$，一般$\beta _1$为0.9，$\beta _2$设为0.999，$ϵ$设为10-8。
第一个公式通过系数$\beta _1$计算了梯度的平均值；第二个公式利用系数$\beta _2$计算梯度平方的均值。
前面一阶动量计算时说到了指数移动平均的偏差校正，这里的一二阶动量同理需要做校正。
### Nadam
Adam利用了一阶及二阶动量，前面还提到了另一个方法Nesterov，因此Nadam就是将Nesterov与Adam结合起来，形成的另一个变体：
$g_t=\nabla f(w_t-\alpha m_{t-1}/\sqrt {V_{t-1}})$
到这里，前面提到的所有的方法都被融合进来了，所以实际上一般在模型开始调试的时候，都会选择Adam/Nadam进行快速尝试，毕竟收敛速度快，效果也很不错。
# 算法效果
下面看一下几种算法在鞍点和等高线上的表现：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190218114720323.gif)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190218133632907.gif)
由上图看出，Adagrad, Adadelta, RMSprop 很快就找到正确的方向，收敛速度也相当快；相比之下，其他方法要么速度慢，要么路径更加曲折，无疑具有很大优势。
[
](https://img-blog.csdnimg.cn/20190218133632907.gif)
> 对于稀疏数据，尽量使用学习率可自适应的优化方法，不用手动调节，而且最好采用默认值

> SGD通常训练时间更长，容易陷入鞍点，但是在好的初始化和学习率调度方案的情况下，结果更可靠

> 如果在意更快的收敛，并且需要训练较深较复杂的网络时，推荐使用学习率自适应的优化方法。

> Adadelta，RMSprop，Adam是比较相近的算法，在相似的情况下表现差不多

> 在想使用带动量的RMSprop，或者Adam的地方，大多可以使用Nadam取得更好的效果

> Adam的缺点

> 在前面的篇幅，Adam的来源及计算方式等已经清楚的展现出来了，融合多种方法的Adam理论上收敛速度快同时效果也好，但是不是无脑用Adam就可以了呢？

> 并不是。实际上，上述那么多种优化算法，都是在SGD的基础上做各种学习率的主动控制，自适应的调整能节省不少精力，但也有很多实验表明，精细的调参的SGD效果往往更可靠。

> 不收敛

> ICLR 2018 中有一篇论文
> [ON THE CONVERGENCE OF ADAM AND BEYOND](https://openreview.net/pdf?id=ryQu7f-RZ)
> 中很详细的谈论了Adam算法，同时给出反例证明Adam在某些情况下可能不收敛。

> 回顾一下前文提到的参数更新量计算公式：

$$
\Delta w=\alpha \frac {m_t} {\sqrt {V_t}}
$$
> ，则此时的
$$
\alpha
$$
> 可以认为是
$$
\alpha /\sqrt {V_t}
$$
> 。

> SGD在不使用二阶动量，因此在学习过程中学习率恒定或者使用某些学习率衰减策略；而AdaGrad则使用整个历史梯度进行计算，学习率单调递减；但是Adadelta/Adam等则使用某个窗口大小的梯度进行计算，学习率可能由于数据的不同而发生振荡。

> 可以想到，单调递减的学习率最终模型能够收敛，而学习率振荡的情况下，模型很可能无法收敛。针对这种情况，作者提出了一个修正的方法：

$$
V_t=max(\beta _2V_{t-1}+(1-\beta _2)g_t ^2，V_{t-1})
$$

> 从而使得学习率单调递减，防止模型在后期因为学习率震荡而无法收敛。

> 非最优

> 高维非凸函数的空间中，往往存在大量的局部最优，而优化算法很可能在这些洼地或者高原中迷失，无法找到全局最优点。

> [The Marginal Value of Adaptive Gradient Methods in Machine Learning](https://arxiv.org/pdf/1705.08292.pdf)
> 这篇论文中提到，

> 同样的一个优化问题，不同的优化算法可能会找到不同的答案，但自适应学习率的算法往往找到非常差的答案。他们通过一个特定的数据例子说明，自适应学习率算法可能会对前期出现的特征过拟合，后期才出现的特征很难纠正前期的拟合效果。

> 另外一篇
> [Improving Generalization Performance by Switching from Adam to SGD](https://arxiv.org/pdf/1712.07628.pdf)
> 在CIFAR-10数据集上进行测试，最终发现，

> Adam的收敛速度比SGD要快，但最终收敛的结果并没有SGD好。他们进一步实验发现，主要是后期Adam的学习率太低，影响了有效的收敛。他们试着对Adam的学习率的下界进行控制，发现效果好了很多。

> 所以比较好的方式就是前期使用Adam，尽可能快的下降，后期使用SGD，尽可能寻找最优解。

> 这里存在两个问题，什么时候切换算法，切换算法后采用什么学习率，在这篇论文中都有相应的解释，这里不做详细解释。

> 但是不管是什么算法，最重要的还是从数据出发，找到一种适应当前数据的算法，至于理论上的最优与否还是留待学术研究发掘吧。

> 经验分析

> [An empirical analysis of the optimization of deep network loss surfaces](https://arxiv.org/pdf/1612.04010.pdf)
> 这篇文章对各个优化算法做了对比实验，将目标函数映射到三维的超平面进行直观的观察，可以发现，从同一位置出发的不同算法，最后都到达了不同的局部最优点。更形象化的说法是，在遇到不同的分叉口，算法选择了不一样的道路。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190218143654990.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3,size_16,color_FFFFFF,t_70)
> 参考

> 1、
> [https://zhuanlan.zhihu.com/p/32230623](https://zhuanlan.zhihu.com/p/32230623)

> 2、
> [https://www.cnblogs.com/guoyaohua/p/8542554.html](https://www.cnblogs.com/guoyaohua/p/8542554.html)


