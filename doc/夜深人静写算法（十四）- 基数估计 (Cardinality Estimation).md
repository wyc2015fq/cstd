# 夜深人静写算法（十四）- 基数估计 (Cardinality Estimation)

2019年01月12日 18:56:42

英雄哪里出来

阅读数：76416

 								标签： 																[基数估计																](http://so.csdn.net/so/search/s.do?q=基数估计&t=blog)[HyperLogLog																](http://so.csdn.net/so/search/s.do?q=HyperLogLog&t=blog)[LogLog																](http://so.csdn.net/so/search/s.do?q=LogLog&t=blog)[Linear Counting																](http://so.csdn.net/so/search/s.do?q=Linear Counting&t=blog)[大数据																](http://so.csdn.net/so/search/s.do?q=大数据&t=blog) 							更多

 								个人分类： 																[夜深人静写算法																](https://blog.csdn.net/WhereIsHeroFrom/article/category/7370637) 							

 								所属专栏： 																[夜深人静写算法](https://blog.csdn.net/column/details/21195.html) 																 							

 									

# 目录

### 一、概述        1、最小值估值法        2、哈希法        3、k-前缀法  二、Linear Counting        1、算法思路        2、算法证明  三、LogLog Counting        1、算法思路        2、算法证明        3、误差消减  四、HyperLogLog Counting        1、算法思路        2、并行化  五、参考资料

 

### 一、概述

​        基数估计算法是为了解决这样一个问题：设想你有一个巨大的含有重复项的数据集合，这个数据大到无法完全存储到内存中，但是你想知道这个数据集合中有多少不同的元素，这个不同元素的个数就叫基数（Cardinality）。
​          举个简单的例子：统计一个游戏所有服务器上的注册用户总数。传统的方法是给每个用户分配一个唯一标识，然后用一个数据结构（哈希表、平衡二叉树、红黑树  等等）来维护这个唯一标识的插入，最后统计这个数据结构的元素个数。这种方法是最常用的，但是在大数据下，内存占用会随着玩家的增多呈线性增长。所以这种方法是不可行的。

​        **1、最小值估计法**
​         我们把问题简化一下，假设集合中的元素都是整数，数值上限为 M，x 为目前找到的数字的最小值，那么我们可以估计这个集合的基数为 M / x。
​         例如，一个集合的数值上限 100，找到最小的数是 2，那么估计这个集合的基数为 100 / 2 = 50。这个方法很直观，然而准确度一般。但是，它可以作为基数估计的切入点，从而引入更加复杂的算法。

![img](https://img-blog.csdnimg.cn/20190109075816756.png)

​        **2、哈希法**
​          最小值估计法虽然直观，但是局限性很大。我们可能遇到的情况诸如：集合中不同元素个数很少，但是最小值非常小；集合中不同元素个数很大，但是最小值并不小；集合元素是字符串   等等。这些问题源于集合元素并非均匀分布。解决这个的办法是：我们可以利用一些良好的哈希函数，将任意数据集映射到随机分布的下标中。然后再利用简单估计法进行计算。

![img](https://img-blog.csdnimg.cn/2019010908350251.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1doZXJlSXNIZXJvRnJvbQ==,size_16,color_FFFFFF,t_70)

​        **k-前缀法**
​         随机数集合中，通过计算每一个元素哈希值的二进制表示的 0 前缀，设 k 为当前集合中最长的 0  前缀的长度，则平均来说集合中大约有 2^k  个不同的元素；我们可以用这个方法估计基数。同样，这种估计方法准确性也不高，但是这个估计方法比较节省资源：对于 16 位的哈希值来说，只需要 4  比特去存储 0 前缀的长度。

![img](https://img-blog.csdnimg.cn/20190111091634164.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1doZXJlSXNIZXJvRnJvbQ==,size_16,color_FFFFFF,t_70)

​        为了提高准确性，我们可以采用多个相互独立的哈希函数，计算每个哈希函数产生的最长 0 前缀，然后取平均值来提高算法精度。

### 二、Linear Counting

​        Linear Counting 简称 LC，在 1990 年的一篇论文 “A linear-time  probabilistic counting algorithm for database applications”  中被提出。作为一个早期的基数估计算法，实际空间复杂度并没有太大改善，但是可以作为更复杂的基数估计算法的基础，还是有必要了解一下。
​         **1、算法思路**
​         假设一个哈希函数 H，它的映射值域为 [0, M)，且哈希函数服从均匀分布。使用一个长度为 M 的 bitmap  ，每个比特代表一个桶 （bucket），初始化每个桶的值为 0。一个集合的基数为 n，将集合中所有元素用哈希函数 H 映射到这个 bitmap  中，如果一个元素被哈希到第 i 个桶，则将它对应的比特位的值置为 1。当集合中所有元素哈希完毕，bitmap 中还剩余 x 个 0 （这里的 x  称为 空桶数），则有集合的基数的近似估计值（最大似然估计）如下：

![img](https://img-blog.csdnimg.cn/20190107081006887.png)集合基数近似值

​        **2、算法证明**
​         由于哈希函数 H 服从均匀分布，所以一个元素经过哈希函数映射后映射到任何一个桶的概率都为 1 / M。那么，经过 n 个元素哈希后，第 i 个桶为 0 的概率 p 如下：

![img](https://img-blog.csdnimg.cn/20190107113100359.png)n 次哈希都避开了第 i 个桶

​        M 是个固定值，所以当 n 为常量时，*p* 就是一个常量。从而可知，经过 n 个元素哈希后，每个桶为 0 的概率相等，即它们是相互独立的事件。
​        根据组合原理，当有 x 个桶为 0 的方案数就是 C(M，x)。从而得到 x 个桶为 0 的概率如下：

![img](https://img-blog.csdnimg.cn/20190107113519120.png)

​        很明显，它服从二项分布，所以期望 *μ* 就是独立事件次数 M 和该事件发生概率 p 的乘积，有：

![img](https://img-blog.csdnimg.cn/20190107114005691.png)空桶个数的期望

​        数学中的第二重要极限表示如下（证明可以采用 洛必达法则）：

![img](https://img-blog.csdnimg.cn/20190107084901764.png)自然常数 e

那么，当 M 的值足够大的时候，我们可以把上面的期望计算进行一个变形，得到如下近似式：

![img](https://img-blog.csdnimg.cn/20190107121633355.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1doZXJlSXNIZXJvRnJvbQ==,size_16,color_FFFFFF,t_70)

然后移项，将 n 转化成期望 *μ* 和 M 的表达式如下（其中 ln 为自然对数）：

![img](https://img-blog.csdnimg.cn/20190107121729254.png)集合基数近似表达式

​        二项分布的极限分布为正态分布。故当 M 很大时，二项分布的概率可用正态分布的概率作为近似值。何谓 M 很大呢 ? 一般规定：当 p < 0.5 且 Mp ≥ 5，这时的 M 就被认为很大，可以用正态分布的概率作为近似值了。
​         正态分布的期望 *μ* 的最大似然估计为样本均值（可用利用偏导进行推导），如下图所示，表示的是 M = 100，n = 65 的空桶个数的分布曲线图：

![img](https://img-blog.csdnimg.cn/20190108072420492.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1doZXJlSXNIZXJvRnJvbQ==,size_16,color_FFFFFF,t_70)M=100，n=65 的空桶个数分布曲线

​        空桶个数大概率出现在正态分布密度函数图像的中轴所在位置，所以我们可以将上面的式子再做一个变形，也就是刚开始提到的集合基数近似值：

![img](https://img-blog.csdnimg.cn/20190108072932933.png)其中 x 为统计所得空桶数


         再来看几个例子加深下理解，我们令 M = 1000，然后 n 取不同值，采用 Python 的 matplotlib.pyplot 绘制函数曲线。下图展示了 n 为 50、200、500、1000、2000 时的函数曲线：

![img](https://img-blog.csdnimg.cn/20190108070843525.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1doZXJlSXNIZXJvRnJvbQ==,size_16,color_FFFFFF,t_70)M = 1000 的情况下空桶个数的概率分布呈二项分布

​        我们发现：n 越小， 空桶个数相对越多；反之，则个数相对越少。这也是符合正常思维逻辑的，在运用这个算法的时候，如果 M 比  n 小太多，可能导致所有的桶都被哈希到，这样空桶数为 0，估算公式的值为无穷大。所以在取值的时候需要保证满桶的概率非常小。而实际情况是 M  是固定值，n 会随着加入的元素增加而呈线性增长，所以 LC 算法往往在数据量较小时发挥作用，一般用来配合 LogLog 或者  HyperLogLog 使用 （ Redis 中就是用 Linear Counting 来 配合 HyperLogLog）。

### 三、LogLog Counting

​        LogLog Counting 简称 LLC，出自论文“Loglog Counting of Large  Cardinalities”。LLC的空间复杂度仅为 O( log2( log2(N) ))，故此得名。数亿级别的数据量可以在 KB  级的内存中得到良好的估值。
​         **1、算法思路**         假设一个哈希函数 H，哈希结果服从均匀分布，且哈希后的结果是一个长度为 L 的比特串。令 g 为服从均匀分布的样本空间中随机抽取的一个样本，那么它的每个比特位服从 0-1 分布。即每个比特位为 0 和 1 的概率都为 0.5，且各个比特位之间相互独立。
​         令 h(g) 为 g 的比特串中第一个出现 "1" 的位置。那么 h(g) 的值域为 [1, L]。为了排除 g 等于 0  的情况，可以在计算出的哈希值末尾 位或 上 1。然后，我们遍历集合中所有元素计算哈希值，找到所有比特串中的 h(g) 最大值，记为 hmax。
​          此时，可以用 2 的 hmax 次幂作为该集合基数的粗糙估计，即：

![img](https://img-blog.csdnimg.cn/201901081911513.png)

​          **2、算法证明**
​           哈希函数得到比特串的每一位都是相互独立的，所以我们可以把每一位的结果看作是掷硬币的过程，比特位的 0 代表硬币反面，1 代表硬币正面。假设我们的集合总共有 n 个元素，分两种情况讨论：
​           a) 每个元素的 h(g) 都小于等于 k 的概率，记为 *P*1；

![img](https://img-blog.csdnimg.cn/20190111214726215.png)

​          可以这么考虑，h(g) 大于 k 这个事件可以描述成 “前 k 次掷硬币得到都是反面”，这个概率就是 1/2 的 k 次幂；那么，相反的，小于等于 k 的概率就是用 1 去减。然后 n 次独立事件概率的乘积就是最后的结果了。
​           b) 至少一个元素的 h(g) 大于等于 k 的概率，记为 *P*2；

![img](https://img-blog.csdnimg.cn/201901112151313.png)

​         利用 a) 的思路，可以得到每个元素的 h(g)  都小于k 的概率，然后在用 1 去减，就能得到 b) 的概率了。
​          当 n>>2^k 时 a) 的概率几乎为 0；当n<<2^k 时 b) 的概率几乎为  0。所以一旦集合中出现 h(g) = k，那么从概率上讲 n 的值不可能远大于2^k、也不可能远小于 2^k。所以对于集合中最大的 h(g) =  hmax，有集合基数近似值为 2^hmax。

​          **3、误差消减**

​         上述分析给出了 LLC 的基本思想，不过如果直接使用上面的单一估计量进行基数估计会由于偶然性而存在较大误差。因此，LLC  采用了均值法来消减误差。具体来说，就是将哈希空间平均分成 m 份，每份称之为一个桶（bucket）。对于每一个元素，其哈希值的前 k  比特作为桶编号（即 m = 2^k），后 (L - k)  个比特作为真正用于基数估计的比特串。桶编号相同的元素被分配到同一个桶，在进行基数估计时，首先计算第 i 个桶内元素最大的第一个 “1” 的位置  M[i]，然后对这 m 个值取平均后再进行估计，即：

![img](https://img-blog.csdnimg.cn/20190111222253237.png)

​          这相当于物理试验中经常使用的多次试验取平均的做法，可以有效消减因偶然性带来的误差。这里的 M[i] 相当于求了算数平均数，但是是作用在指数上的，所以其实真正做的是几何平均数。
​           假设 H 的哈希长度为 16 比特，分桶数 m 定为 32，那么 k 就是  5。设一个元素哈希值的比特串为“0011001010001110”，由于 m 为 32，因此前 5  个为桶编号，所以这个元素应该归入“00110” 即 6 号桶（桶编号从0开始，最大编号为 m -  1），而剩下部分是“01010001110”且显然h(01010001110) = 2，所以桶编号为 “00110” 的元素 M[6] 的值为  2。
​         相比 LC 其最大的优势就是内存使用极少。不过LLC也有自己的问题，就是当 n 不是特别大时，其估计误差过大，因此目前实际使用的基数估计算法都是基于 LLC 改进的算法。

### 四、HyperLogLog Counting

​          HyperLogLog Counting 简称 HLLC，基本思想是在 LLC  的基础上做改进，具体细节请参考“HyperLogLog: the analysis of a near-optimal cardinality  estimation algorithm”这篇论文。
​           HLLC 在 LLC 基础上的一个改进是用 “调和平均数” 取代 “几何平均数”。所谓调和平均数，就是各个数倒数的平均数的倒数。表示如下：

![img](https://img-blog.csdnimg.cn/20190111230742534.png)

​        **1、算法思路**
​        沿用了 LLC 的算法思路，只是在多个桶取平均值时，用  “调和平均数” 取代 “几何平均数”，最终的代数表示如下：

![img](https://img-blog.csdnimg.cn/20190112135605114.png)

​        上述估值公式中的 C，是根据 m 得到的一直数值常量，具体值如下：

![img](https://img-blog.csdnimg.cn/20190112135704375.png)

![img](https://img-blog.csdnimg.cn/20190112135725613.png)

​        Redis 的 HyperLogLog 算法中，m 的取值为 2 的 14 次，即 16384。那么常量 C 的值计算如下：

![img](https://img-blog.csdnimg.cn/20190112142031926.png)

​        HLLC 的另一项改进是在基数 n 相对与 m 较小或者较大时做了分支判断，采用不同的策略计算基数值。主要分三种情况，下面给出 Python 伪代码，其中 n 为用调和平均计算出来的基数初始估值：

```python
if n <= m*5/2:
	if V != 0:
		# V表示M[i]=0的个数，个数非0则采用 LC
		return m*log(m * 1.0 / V)
	else:
		# 否则，继续采用HLLC
		return n
elif n <= (1<<32)/30.0:
	return n
else :
	# n 非常大的情况
	return -(1<<32) * log(1 - n * 1.0 / (1<<32))
```

​        **2、并行化**

​        以上几种基数估计算法，存储的实际数据结构都和本身集合的大小无关，所以无论集合多大，数据结构的大小都是固定的。这样的数据结构下，能够很好的实现两个集合之间的合并。这样就可以实现多个线程同时计算，最后合并统计，从而实现并行计算。

 

### 五、参考资料

[基数估计算法概览（推荐）](http://blog.codinglabs.org/articles/cardinality-estimation.html)

[基数估计算法（英文）](http://blog.notdot.net/2012/09/Dam-Cool-Algorithms-Cardinality-Estimation)

[LLC和HLLC的可视化数据调试程序](http://content.research.neustar.biz/blog/hll.html)

[基数估计的概率算法](http://www.docin.com/p-600734346.html)

[大数据处理中基于概率的数据结构](https://www.cnblogs.com/fxjwind/p/3289221.html)

[HyperlogLog 原理简介](https://blockchain.iethpay.com/hyperloglog-theory.html)

[HyperlogLog 详解（英文）](http://dsinpractice.com/2015/09/02/counting-unique-items-fast/)

[HyperlogLog 集合合并 ](http://dsinpractice.com/2015/09/07/counting-unique-items-fast-unions-and-intersections/)