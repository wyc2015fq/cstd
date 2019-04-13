
# Batch Normalization的测试或推理过程及样本参数更新方法 - BrightLamp的博客 - CSDN博客


2018年12月30日 15:44:28[BrightLampCsdn](https://me.csdn.net/oBrightLamp)阅读数：149



## 摘要
本文探讨 Batch Normalization 在测试或推断时使用的算法及其原理.
## 相关
*配套代码, 请参考文章 :*
Python和PyTorch对比实现批标准化 Batch Normalization 函数在测试或推理过程中的算法.
*系列文章索引 :*
[https://blog.csdn.net/oBrightLamp/article/details/85067981](https://blog.csdn.net/oBrightLamp/article/details/85067981)
## 正文
Batch Normalization 在训练时使用每批数据的均值和方差进行数据的规范化, 在测试或推断的时候使用全体数据的特征.
我们不可能在训练前再次遍历全体数据的特征, 耗时太大. 也不可能记录所有批次的中间结果, 内存消耗太大.
目前主流的深度学习框架 TensorFlow 和 PyTorch 等采用的是参数估计加滑动平均法, 并引入的一个超参数来解决这个问题.
接下来我们详细探讨这个方法的理论基础.
## 1. 分步使用样本特征计算总体特征
### 1.1 分步使用样本均值计算总体均值
已知一个$k$维数组$x$的均值为$\mu$, 标准差为$\sigma$, 排列成列数量不相等的矩阵$X$, 共$m$行, 每行$n_i$个元素, 下标$i$表示第$i$行.
其形式类似如下 :
$$
x_{11}
$$
$$
x_{12}
$$
$$
x_{13}
$$
$$
x_{14}
$$
$$
(n_1 = 4)
$$
|
|---|---|---|---|---|---|
$$
x_{21}
$$
$$
x_{22}
$$
$$
x_{23}
$$
$$
(n_2 = 3)
$$
|
$$
x_{31}
$$
$$
x_{32}
$$
$$
x_{33}
$$
$$
x_{34}
$$
$$
x_{35}
$$
$$
(n_3 = 5)
$$
|
$$
x_{41}
$$
$$
x_{42}
$$
$$
(n_4 = 2)
$$
|
$$
x_{51}
$$
$$
x_{52}
$$
$$
x_{53}
$$
$$
x_{54}
$$
$$
(n_5 = 3)
$$
|
设第$i$组的均值为$\bar x_i$, 标准差为$s_i$. 求$\bar x$与$\mu$的关系, 求$s$与$\sigma$的关系.
由题意, 知矩阵的元素的总数量$k$, 均值$\mu$, 方差$\sigma^2$:
$$
k = \sum_{i=1}^{m}n_i
\;\\
\mu =\frac{1}{k}\sum_{i=1}^{m}\sum_{j=1}^{n_i}x_{ij}\\
\;\\
\sigma^2 = \frac{1}{k}\sum_{i=1}^{m}\sum_{j=1}^{n_i}(x_{ij}-\mu)^2
$$
因为 :
$$
\sum_{i=1}^{m}\sum_{j=1}^{n_i}x_{ij}=\sum_{j=1}^{n}n_i\bar x_i\\
$$

$$
\mu=\frac{1}{k}\sum_{j=1}^{n}n_i\bar x_i
$$
令$n = 2$:
$$
\mu=\frac{n_1\bar x_1 +n_2\bar x_2}{(n_1+n_2)}
$$
改成迭代式 :
$$
\mu_{i+1}=\frac{k_i\mu_i +n_{i+1}\bar x_{i+1}}{(k_i+n_{i+1})}
$$
式中$\mu_i$表示前$i$行 (包括$i$) 所有元素的均值,$k_i$表示前$k_i$行 (包括$i$) 所有元素的总数量.
在编程实战中, 使用迭代式, 可以分步计算总体的均值, 节省宝贵的计算机内存.
### 1.2 分步使用样本方差计算总体方差
总方差 :
$$
\sigma^2 = \frac{1}{k}\sum_{i=1}^{m}\sum_{j=1}^{n_i}(x_{ij}-\mu)^2=\frac{1}{k}\sum_{i=1}^{m}\sum_{j=1}^{n_i}(x_{ij}^2-2x_{ij}\mu-\mu^2)\\
\;\\
=\frac{1}{k}(\sum_{i=1}^{m}\sum_{j=1}^{n_i}x_{ij}^2-2\sum_{i=1}^{m}\sum_{j=1}^{n_i}x_{ij}\mu+\sum_{i=1}^{m}\sum_{j=1}^{n_i}\mu^2)\\
\;\\
=\frac{1}{k}(\sum_{i=1}^{m}\sum_{j=1}^{n_i}x_{ij}^2-2k\mu^2+k\mu^2)\\
$$
得 :
$$
\sum_{i=1}^{m}\sum_{j=1}^{n_i}x_{ij}^2=k\sigma^2+k\mu^2=k(\sigma^2+\mu^2)
$$
同样的, 对于第$i$行样本方差, 有 :
$$
\sum_{j=1}^{n_i}x_{ij}^2=n_is_i^2+k_i\bar n_i^2=n_i(s_i^2+\bar x_i^2)
$$
得 :
$$
\sum_{i=1}^{m}n_i(s_i^2+\bar x_i^2)=k(\sigma^2+\mu^2)\\
\;\\
\sigma^2=\frac{1}{k}\sum_{i=1}^{m}n_i(s_i^2+\bar x_i^2)-\mu^2
$$
令$n = 2$:
$$
\sigma^2=\frac{n_1(s_1^2+\bar x_1^2)+n_2(s_2^2+\bar x_2^2)}{n_1+n_2}-\mu^2
$$
改成迭代式 :
$$
\sigma_{i+1}=\frac{k_i(\sigma_i^2+\mu_i^2) +n_{i+1}(s_{i+1}^2+\bar x_{i+1}^2)}{(k_i+n_{i+1})}-\mu_{i+1}^2
$$
式中$\mu_i$表示前$i$行 (包括$i$) 所有元素的均值,$\sigma_i$表示前$i$行 (包括$i$) 所有元素的标准差,$k_i$表示前$k_i$行 (包括$i$) 所有元素的总数量.
使用迭代公式计算总体方差会明显的加大计算量, 并不是理想的算法. 仅适用于需要精确解, 同时对内存消耗非常敏感的环境中.
## 2. 使用样本特征的期望估算总体特征 (概率统计)
在概率论和数理统计学中, 期望 (expectation) 的定义是 :
$$
E(X)=\sum_{i=1}^{m}\sum_{j=1}^{n_i}p_{ij}x_{ij}
$$
$p_{ij}$是$x_{ij}$出现的概率. 在本例中,$p_{ij}=1/k$:
$$
E(X)=\frac{1}{k}\sum_{i=1}^{m}\sum_{j=1}^{n_i}x_{ij}=\mu
$$
对$\bar x,\;s^2$使用期望公式 :
$$
E(\bar x)=\frac{1}{m}\sum_{i=1}^{m}\bar x_i=\frac{1}{m}\sum_{i=1}^{m}\frac{\sum_{j=1}^{n_i}x_{ij}}{n_i}=\mu\\
\;\\
E(s^2)=\frac{1}{m}\sum_{i=1}^{m}s_i^2
$$
因为 :
$$
s_i^2=\sum_{j=1}^{n_i}\frac{(x_{ij}-\bar x_i)^2}{n_i}=\sum_{j=1}^{n_i}\frac{(x_{ij}-\mu+\mu-\bar x_i)^2}{n_i}\\
\;\\
=\sum_{j=1}^{n_i}\frac{(x_{ij}-\mu)^2+2(x_{ij}-\mu)(\mu-\bar x_i)+(\mu-\bar x_i)^2}{n_i}\\
\;\\
=\frac{1}{n_i}\sum_{j=1}^{n_i}(x_{ij}-\mu)^2
-\frac{2}{n_i}\sum_{j=1}^{n_i}(x_{ij}-\mu)(\bar x_i-\mu)
+\frac{1}{n_i}\sum_{j=1}^{n_i}(\mu-\bar x_i)^2\\
\;\\
=\frac{1}{n_i}\sum_{j=1}^{n_i}(x_{ij}-\mu)^2
-2(\bar x_i-\mu)^2+(\mu-\bar x_i)^2\\
\;\\
=\frac{1}{n_i}\sum_{j=1}^{n_i}(x_{ij}-\mu)^2
-(\bar x_i-\mu)^2\\
$$
所以 :
$$
E(s^2)=\frac{1}{m}\sum_{i=1}^{m}s_i^2=\frac{1}{m}\sum_{i=1}^{m}\frac{1}{n_i}\sum_{j=1}^{n_i}(x_{ij}-\mu)^2-\frac{1}{m}\sum_{i=1}^{m}(\bar x_i-\mu)^2\\
$$
当$n_i = n,\;k=mn$时 :
$$
\frac{1}{m}\sum_{i=1}^{m}\frac{1}{n_i}\sum_{j=1}^{n_i}(x_{ij}-\mu)^2=\frac{1}{mn}\sum_{i=1}^{m}\sum_{j=1}^{n_i}(x_{ij}-\mu)^2=\sigma^2
$$

$$
E(s^2)=\sigma^2-\frac{1}{m}\sum_{i=1}^{m}(\bar x_i-\mu)^2
$$
根据统计学中的公式 :
$$
Var(\bar x)=\frac{1}{m}\sum_{i=1}^{m}(\bar x_i-\mu)^2=\frac{\sigma}{n}
$$
这个公式的证明请看后文. 汇总可得 :
$$
\hat\mu=E(\bar x)\\
\;\\
\hat\sigma^2=\frac{n}{n-1}E(s^2)
$$
其中,$n$为每一组样本中元素的个数,$\hat\mu,\;\hat\sigma$上面的尖号表示估计.
由此, 我们可以用样本均值的期望来估计总体均值, 使用样本方差的期望乘以修正系数$n/(n-1)$来估计总体方差.
## 3. 滑动平均法
在上例中, 我们得出了使用样本均值和方差估计整体均值和方差的办法. 但这里还有一个问题, 我们需要记录每一个批次的样本特征及其权重, 才能在需要的时候进行计算期望计算.
随着训练批次的增加, 对内存的消耗会变得无法忍受.
在目前流行的深度学习框架中, 比如 TensorFlow 和 PyTorch, 使用了滑动平均 (moving average) 的方法来替代期望求解.
滑动平均法的定义如下 :
$$
\hat x_{i+1} =(1-\eta) * \hat x_i +\eta * x_{i+1}
$$
$\hat x_i$是历史总体特征的估计值,$x_{i+1}$是当前样本特征 (若是样本方差则需要按的系数修正).
其中,$\eta$为动量 (momentum), 是一个经验值为 0.01~ 0.1 的超参数.
注意,$\hat\mu$的初始值为 0,$\hat\sigma^2$的初始值为 1 .
## 4. 样本均值的方差公式
下面的证明摘抄自《深入浅出统计学》(美) 格里菲斯(Griffiths, D.) 著
$$
Var(\bar x)=Var(\frac{x_1+x_2+x_3+\cdots+x_n}{n})\\
\;\\
=Var(\frac{1}{n}x_1+\frac{1}{n}x_2+\frac{1}{n}x_3+\cdots+\frac{1}{n}x_n)
$$
当$x_i$为相互独立的样本时 :
$$
Var(\frac{1}{n}x_1+\frac{1}{n}x_2+\frac{1}{n}x_3+\cdots+\frac{1}{n}x_n)\\
\;\\
=Var(\frac{1}{n}x_1)+Var(\frac{1}{n}x_2)+\cdots+Var(\frac{1}{n}x_n)\\
\;\\
=\frac{1}{n^2}(\sigma^2+\sigma^2+\cdots+\sigma^2)\;\\
=\frac{\sigma^2}{n}
$$
需要强调的是, 在证明的过程中, 非常重要的条件是样本数量足够大且满足大数定律, 同时所抽取的样本之间相互独立, 缺一不可. 我们再次检查上文推导过程中的关键一步 :
$$
Var(\bar x)=\frac{1}{m}\sum_{i=1}^{m}(\bar x_i-\mu)^2=\frac{\sigma}{n}
$$
实际生产环境中, 我们往往不能得到满足大数定律的足够多的样本, 也不能保证样本独立,  Batch Normalization 的计算方法在理论上会有潜在的误差. 每批数量越少, 误差越大.
使用滑动平均法, 可以在一定程度上缓和样本异常值所带来的误差, 而超参数动量 (momentum) 则给与了某种人为主动修正的可能性.
全文完.

