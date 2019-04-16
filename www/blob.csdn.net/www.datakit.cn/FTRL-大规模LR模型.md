# FTRL-大规模LR模型
# FTRL-大规模LR模型


**[Chrispher](http://chrispher.github.com)

- [关于](http://chrispher.github.com/page/about.html)
- [留言板](http://chrispher.github.com/page/message.html)
- [时间轴](http://chrispher.github.com/page/timing.html)
- [笔记专题](#)- [PRML](http://chrispher.github.com/category/#PRML)
- [实践练习](http://chrispher.github.com/category/#实践练习)
- [Ng-机器学习](https://raw.githubusercontent.com/chrispher/chrispher.github.com/master/_drafts/machine_learning_notes_ng.docx)

- [课程图谱](#)- [pySpark 公开课](/page/graph/big_data_with_spark_python.html)













### ****FTRL-大规模LR模型                
**
                    2016年05月11日
                    ****[机器学习](http://chrispher.github.com/category/#机器学习)****[机器学习](http://chrispher.github.com/tag/#机器学习)**** 字数:4719
                


2013年的paper了，大规模离散LR模型，用于ctr预估。

### 目录
- [概述](#概述)- [基础介绍](#基础介绍)

- [FTRL模型](#ftrl模型)
- [FTRL算法](#ftrl算法)
- [工程tricks](#工程tricks)
- [评估与其他考虑](#评估与其他考虑)- [模型评估](#模型评估)
- [置信区间](#置信区间)
- [预估校准](#预估校准)

- [自动特征管理](#自动特征管理)
- [一些失败的尝试](#一些失败的尝试)

参考论文：
- [Ad Click Prediction-a View from the Trenches_H. Brendan McMahan_2013.pdf](https://www.eecs.tufts.edu/~dsculley/papers/ad-click-prediction.pdf)

### 概述

FTRL 近似在线学习算法，具有非常好的稀疏性和收敛特性，在ctr预估中大量的使用。本篇还将涉及一些比较trick的工程实现。

对于ctr预估，用户输入一个query，记为q，对于候选的每一个广告，记为a，需要估计$P(click \mid q, a)$，即如果该广告展示了，被点击的概率。这里我们使用大量的特征，包括广告的文本特征以及其他的相关信息。这会使得数据变得非常的稀疏，而且数据离散化和combine会使得最后特征上百亿。因此，对于每天超大量的查询而言，如何提供预估和快速的模型更新，都是需要认真考虑的。基本的预估流程是：
流式数据=>提取特征=>并行训练模型=>[1] 在线评估 [2] 提供预估服务

这里需要注意的是：我们非常关注参数的稀疏性，这会使得在线服务时，进行更少的计算，意味着更快的响应结果。另外，我们需要注意一点，输入的数据都是离散化数据，一般都是0-1值。

#### 基础介绍

在大数据下，在线学习算法对于广义线性模型具有很多的优点。尽管输入的特征x具有百亿维度，实际每个instance可能只有几百个非零值。这使得在大数据下，高效的使用流式样本进行模型的流式训练<每个instance只用一次>。这里我们简单的说一下LR模型。对于label是0-1的情况下（也有用label为-1，+1的，但是推导过程不变），LR的LogLoss如下：
$$l_t(w_t) = -y_t \log p_t - (1-y)\log (1-p_t)$$

那么LogLoss对于参数w的导数就是$(p_t - y_t)x_t$，这就是用于参数更新的梯度值。在线梯度下降(online gradient descent, OGD)是解决这类问题的非常高效、高精度且非常省计算资源的一种方法。但是，在实际应用中，**非常重要的一点是，考虑模型的最终大小<参数空间，非0权重w决定了内存的使用量>**。而OGD并不能非常高效的产生稀疏解。一般我们是引入$L_1$正则来得到稀疏解，实际上这本质上不会使得权重w真的变为0。而FTRL(Follow The(Proximally) Regularized Leader)，或者FTRL-Proximal是既能提高OGD准确率也能提供比RDA更好的稀疏性。这是因为该算法能够利用lazy representation来更好的实现L1。后面，我们会给出算法流程。

### FTRL模型

这里先介绍一下FTRL，这里直接关注梯度训练模块了。对于一组梯度$g_t \in R^d$, t表示更新次数，OGD对于参数的更新方式是:
$w_{t+1} = w_t - \eta_t g_t$

这里的$\eta$是递减的学习速度，比如$\eta = \frac{1}{\sqrt{t}}$。FTRL采用下面的更新方式来替换上面的而形式:
$w_{t+1} = argmin_w (g_{1:t} w + \frac{1}{2} \sum_{s=1}^t \sigma_s \mid \mid w-w_s \mid \mid _2^2 + \lambda \mid \mid w \mid \mid _1)$

这里$g_{1:t} = \sum_{s=1}^t g_s, \sigma_{1:t} = \frac{1}{\eta_t}$。我们想想这个式子在表达什么：第一项是梯度更新，第二项是保证泛化精度，第三项是产生稀疏解。求解一下，可以看到没那么复杂,上式子可以写为：
$(g_{1:t} - \sum_{s=1}^t \sigma_s w_s) w + \frac{1}{\eta_t}\mid \mid w \mid \mid _2^2 + \lambda_1 \mid \mid w \mid \mid_1 + const$
如果我们一开始就存储了$z_{t-1} = g_{1:t-1} - \sum_{s=1}^{t-1} \sigma_s w_s$,那么这一次的更新就是$z_t = z_{t-1} + g_t + (\frac{1}{\eta_t} - \frac{1}{\eta_{n-1}})w_{t-1}$,上式可以改写为：
$argmin_w (z_t w + \frac{1}{\eta_t}\mid \mid w \mid \mid_2 ^2 + \lambda_1 \mid \mid w \mid \mid _1)$
那么更新新如下（求解很容易，先假设w大于0，再假设w小于0，综合假设的结果）：
$% <![CDATA[w_{t+1, i} = 0 \ \ \ \ if \mid z_{t,i}\mid <= \lambda_1;\\ w_{t+1, i} = -\eta_t(z_{t,i} - sng(z_{t,i})\lambda_1) \ \ \  otherwise; %]]>$

**学习速率**
因为是大规模异步学习，所以不同的参数更新的次数是不一样的，如果都按照$\eta = \frac{1}{t}$,对于t次更新，其实有些参数是没有更新t次的，因此这种学习速率可能并不是最优的。论文中，改进为:
$\eta_{t,i} = \frac{\alpha}{\beta + \sqrt{\sum_{s=1}^{t}}g_{s,i}^2}$
其中，$\alpha$是学习速率，谷歌的调参经验是$\beta=1$基本足够好，也很少调，主要是调整$\alpha$.

### FTRL算法

学习速率见上面的等式，算法流程如下：
- 输入：超参数$\alpha, \beta, \lambda_1, \lambda_2$
- （注：任意$i \in \{ 1, ..., d\}$），初始化$z_i = 0, n_i =0$
- for t = 1 to T do:
    - 收到特征向量$x_t$， 令$I=\{i \mid x_i \ne 0 \}$
- 对于 $i \in I$, 计算
        - if $\mid z_i \mid \le \lambda_1$, then $w_{t,i} = 0$
- otherwise: $w_{t+1, i} = -(\frac{\beta + \sqrt{n_i}}{\alpha} + \lambda_2)(z_{t,i} - sng(z_{t,i})\lambda_1)$

- 根据上面得到的$w_{t,i}$计算预测值$p_t = \sigma(x_t w)$
- 观测label $y_t \in \{0, 1\}$
- for all $i \in I$ do
        - $y_i = (p_t - y_t)x_i$ #误差对于$w_i$的梯度
- $\sigma_i = \frac{1}{\alpha}(\sqrt{n_i + g_i^2} - \sqrt{n_i})$ # 等价于$\frac{1}{\eta_ {t,i}} - \frac{1}{\eta_ {t-1,i}}$
- $z_i = z_i + g_i - \sigma_iw_{t,i}$
- $n_i = n_i + g_i^2$

- end for

- end for

### 工程tricks

谷歌进行了大量的实验和工程改进,下面是几个比较有效的一些尝试。
- 1.特征选择
很多特征出现次数极少，可以考虑去掉。但是在线学习中，不能提前知道哪些特征是极少使用到的，且在预处理中去掉这些特征非常困难<读写数据耗时耗力>。谷歌尝试了几种思路，有两种比较合适：
- Poisson Inclusion:对于一些模型里还没有ready的特征，我们给他一个概率p，当这个feature加入模型之后，在正常更新。那么期望出现的次数就是$\frac{1}{p}$之后，会被加入模型
- Bloom Filter Inclusion:当一个特征出现次数达到一定次数的时候，就加入到模型，使用bloom filter实现，比较省内存。

- 2.更少的bit压缩
对于权重的存储，用32位的float还是64位的？对于谷歌的模型而言，几乎所有的权重值都在(-2, +2)之间。谷歌使用了q2.13 coding的方式而不是采用float。q2.13编码，是用两个比特表示整数位数字，13比特表示小数位数字，1个比特表示正负号，一共需要16bit。这种方式还需要加入一点随机，否则可能导致误差的累加。<太工程了，理解不够透彻>
- 3.同时训练多个相似模型
对于超参数和特征的评估，一般对于相似的模型都有一些相似的效果。这样可以在训练的时候更加高效，比如参数存储的时候可以共享一些为0的特征参数，或者共享一些全局变量，比如计数等等。
- 4.数据结构
有些时候，我们需要评估一系列模型，而这些模型只是变动了很少的一部分，比如增删了部分特征。这里可以使用更高效的数据结构，可以RAM。对于这些模型（稍微变化的一组模型）包含了共同的特征时，Single Value Structure 仅存储一个参数值，而不是对于每一个模型存储一个参数值，之后使用bit-field 来跟踪哪些模型包含了这个参数。
学习的过程是：对于一次OGD更新，每一个模型，根据下标标定的特征，从single value里获取参数值，计算它的预测值和误差。对于每一个特征i，对于用到了i得每一个模型，计算新的参数值。之后把得到的结果平均一下，存储在single value里，而这些模型在下一步中都共享这个参数。
- 5.根据counts计算学习速率
正确的计算梯度很重要，但是之前的公式可以看到我们需要计算每一个特征的梯度和和梯度的平方和，这里使用了一个比较gross的方式来近似这个值。
**假设:**包含给定特征的事件均有相同的概率发生。（这个近似比较terrible，但是这里却比较work）。更进一步，我们假设所有的模型都有累积概率值。如果有N个负例，P个正例，那么概率$p = \frac{P}{N+P}$,如果使用logistic 回归，那么对于正例的梯度就是p-1，对于负例的梯度是p,那么合起来的梯度就是：
$\sum_ g _ {t,i}^2 = \sum_ {pos}(1-p_t)^2 + \sum_ {neg}p_t^2  \approx  P(1-\frac{P}{N+P})^2 + N(\frac{P}{N+P}^2) = \frac{PN}{N+P}$
这种近似可以在计算的时候显得更加的方便
- 6.训练数据子采样
典型的CTR预估中，正例（点击）是非常少的。我们需要进行一些采样，思路如下：
    - 对于包含至少有一个广告被点击的query
- 对于包含所有的广告都未被点击的query，以概率r选择
如果直接使用这些数据进行训练的话，预测的结果会非常的有偏。这个问题，可以通过给每个样本一个权重来解决，对于那些以概率r采样的样本，权重设置为$\frac{1}{r}$


### 评估与其他考虑

#### 模型评估

一般都是用离线的数据进行评估，但是在线的评估也是非常重要的，但是代价比较高。模型有很多评估方式，谷歌选择了AucLoss(即 1 - AUC， AUC就是标准的ROC下面的面积)， LogLoss和SquaredError，此外谷歌也设计了自己的metric。
对于评估数据集，谷歌并不是使用交叉验证，而是使用了online loss。另外，采用绝对的metric惊颤会产生误导，比如不同的地区点击率不同，使用绝对值比较是不合理的，谷歌一般使用相对于baseline模型的metric的相对变化值。
此外，谷歌会使用可视化来强化理解，他们设计了一个高维可视化工具GridViz。

#### 置信区间

很多时候，不仅要预估点击率，还要考虑我们预估的质量如何。在实际过程中，这种评估实际上是在explore和exploit之间的一种权衡：为了更准确的预估，系统必须偶尔展示一些只有很少数据的广告，这必须与在展示一些较好广告的收益之间做权衡。

#### 预估校准

系统的bias(the difference between the average predicted and observed CTR on some slice of data)可以由很多因素造成，比如不准确的模型假设、不同的学习算法，一些潜在的特征在训练中不可用。为了解决这些，需要在预估的基础之上再校准。校准的思路是：在一个数据切片d上，预估值是p，与真实观测的CTR比较接近，我们可以简单的使用$\tau_p=\gamma p^k$来校准。这种piecewise的方式，在谷歌的测试中是较baseline效果要好的。

### 自动特征管理

大规模机器学习很重要的一点是管理规模的进一步扩大，包括配置、代码、计算资源等等内容组合成的机器学习系统。基本的思路是对各种信号进行index，这种管理可以避免各种信号的使用混乱，也可以在定义新得信号之后，进行一些自动化的测试和规范。

### 一些失败的尝试

以下的一些失败的尝试，原因可能有很多种，也许都是场景和业务相关的问题。
- **降低特征空间**
使用哈希trick，使得特征空间降低到$2^{24}$，节省RAM，效果并没有提升。
- **Dropout**
谷歌测试了dropout，使得模型训练类似于引入了bagging的效果，但是实际测试中并没有很好的泛化效果。可能是因为dropout更适合于特征比较稠密的情况，通过dropout使得一些强相关的特征被分别对待。
- **Feature Bagging**
在特征空间里抽样特征，训练k个模型之后进行简单的均值融合。也可以并行的训练这k个模型，但是实际效果略有下降。
- **Feature Vector  Normalization**
在模型中，不同的样本中非零特征数差距可能非常的大，这种差异可能导致收敛较慢或者印象预测准确率。谷歌尝试了很多归一化的方式$\frac{x}{\mid \mid x \mid \mid}$来减少不同样本vector的范围差异。对于谷歌的评价系统，并不能很好的正向，在一些实验中甚至是有害的，这可能是因为他影响了学习速率和正则化。





