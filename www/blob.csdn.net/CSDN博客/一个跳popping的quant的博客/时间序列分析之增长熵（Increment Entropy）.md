# 时间序列分析之增长熵（Increment Entropy） - 一个跳popping的quant的博客 - CSDN博客





2019年03月10日 12:16:33[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：60








#### 增长熵（Increment Entropy，IncrEn）

#### 概念

ApEn主要是通过计算时间序列中子序列出现的频率（frequency）来作为衡量相似性的指标，频率越高越相似，ApEn也就越低。它的缺陷主要是由于不同参数的组合会出现不一样的结果，缺少相对一致性（relative consistency）；由于它的自匹配，导致ApEn对规律性具有一定的偏向；另外，它对数据集的长度比较敏感。

SmapEn克服了ApEn的缺陷，它避免了自匹配，并且具有较高的相对一致性，而且对立与数据集的长度。

尽管ApEn和SampEn都具有一定的应用，但是它们都忽略了序列中元素的时间关系。PeEn则考虑了时间序列中元素的时间关系，但是却忽略了时间序列中元素之间的大小尺度的变化，只是将处于一定条件的值看做了相同的符号。当然为了解决这方面的问题，也有一些方法对PeEn进行了改进，如 fine-grained PeEn 和 weighted PeEn。

增长熵是16年由 X. Liu 等人提出的一种新型衡量时间序列复杂度的指标。它的提出主要还是为了克服ApEn、SampEn以及PeEn的一些局限性。

#### 增长熵求法

增长熵的求法类似于排列熵，只不过它不仅仅考虑了时间序列中的次序等级（rank order），而考虑了变化的尺度。

1、设有长度为$N$的时间序列$u(1),u(2),u(3),...,u(N)$，规定一个**嵌入维度**$m$ (embedding dimension)和一个**表示波动幅度的精度**$q$（ precision of the fluctuation amplitudes） 。

2、通过将原序列进行增长重构，其实就是将原序列进行一阶差分，得到$v(j)$，其中$1 \leq j \leq N-1$。

3、然后对$v(i)$序列进行重构，划分为$N - m$个子序列，每个子序列中含有$m$个元素，每个子序列表示为$V(i) = v(i),v(i+1),...,v(i+m-1), 1\leq i\leq N-m$

4、接下来对上面的每个子序列$V(i)$求它对应的模式向量$w(i)$，每个$w(i)$都表示为对$V(i)$中每个元素的sign和magnitude的组合，即$w(i) = [(s(1),q(1)),(s(2),q(2)),...,(s(m),q(m))]$，其中$s(k) = sgn(v(k))$，$q(k) = min(q, |\frac{v(j)×q}{std(v(j))}|)$，且$q(k)= 0, if \space  std(v(j)) = 0$。这里的$s(k)$和$q(k)$其实就可以理解为增长的方向和增长的尺度(magnitude)。

5、计算每个子序列中的模式向量的种类出现的个数$Q(w(i)$，然后计算它的频率$P(w(i))=\frac{Q(w(i))}{N-m}$

6、最后计算时间序列的增长熵：$$H(m) = \frac{1}{m-1}\sum_{i=1}^{k}P(w(i))log\frac{1}{P(w(i))}$$

#### ref

Appropriate use of the increment entropy for electrophysiological time series



