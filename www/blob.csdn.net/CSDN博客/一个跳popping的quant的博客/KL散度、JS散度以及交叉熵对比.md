# KL散度、JS散度以及交叉熵对比 - 一个跳popping的quant的博客 - CSDN博客





2018年06月07日 21:18:10[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：8745








在看论文《Detecting Regions of Maximal Divergence for Spatio-Temporal Anomaly Detection》时，文中提到了这三种方法来比较时间序列中不同区域概率分布的差异。



## KL散度、JS散度和交叉熵

三者都是用来衡量两个概率分布之间的差异性的指标。不同之处在于它们的数学表达。

对于概率分布P(x)和Q(x)

### 1）KL散度（Kullback–Leibler divergence）

又称KL距离，相对熵。

![](https://img-blog.csdn.net/20180607205421902)

当P(x)和Q(x)的相似度越高，KL散度越小。

KL散度主要有两个性质：

（1）不对称性

尽管KL散度从直观上是个度量或距离函数，但它并不是一个真正的度量或者距离，因为它不具有对称性，即D(P||Q)!=D(Q||P)。

（2）非负性

相对熵的值是非负值，即D(P||Q)>0。



### 2）JS散度（Jensen-Shannon divergence）

JS散度也称JS距离，是KL散度的一种变形。

![](https://img-blog.csdn.net/20180607205109219)

但是不同于KL主要又两方面：

（1）值域范围

JS散度的值域范围是[0,1]，相同则是0，相反为1。相较于KL，对相似度的判别更确切了。

（2）对称性

即 JS(P||Q)=JS(Q||P)，从数学表达式中就可以看出。



### 3）交叉熵（Cross Entropy）

在神经网络中，交叉熵可以作为损失函数，因为它可以衡量P和Q的相似性。

![](https://img-blog.csdn.net/20180607210747981)

交叉熵和相对熵的关系：

![](https://img-blog.csdn.net/20180607211309446)



以上都是基于离散分布的概率，如果是连续的数据，则需要对数据进行Probability Density Estimate来确定数据的概率分布，就不是求和而是通过求积分的形式进行计算了。



