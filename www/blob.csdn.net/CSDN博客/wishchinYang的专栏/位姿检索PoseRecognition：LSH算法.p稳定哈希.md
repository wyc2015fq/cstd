# 位姿检索PoseRecognition：LSH算法.p稳定哈希 - wishchinYang的专栏 - CSDN博客
2014年02月13日 16:24:42[wishchin](https://me.csdn.net/wishchin)阅读数：3411
        位姿检索使用了LSH方法，而不使用PNP方法，是有一定的来由的。主要的工作会转移到特征提取和检索的算法上面来，有得必有失![可怜](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cute.gif)。因此，放弃了解析的方法之后，又放弃了优化的方法，最后陷入了检索的汪洋大海。
**0：**转自wiki：[http://en.wikipedia.org/wiki/Locality_sensitive_hashing](http://en.wikipedia.org/wiki/Locality_sensitive_hashing)
以下参考资料仅供参考：LSH理解及相关资料：[http://s99f.blog.163.com/blog/static/35118365201262691335382/](http://s99f.blog.163.com/blog/static/35118365201262691335382/)
有一篇代码的实现：[http://blog.sina.com.cn/s/blog_ad9597a30101o0ix.html](http://blog.sina.com.cn/s/blog_ad9597a30101o0ix.html)
**一：局部敏感哈希—**原始**分解**
         原文链接：[http://www.jiahenglu.net/NSFC/LSH.html](http://www.jiahenglu.net/NSFC/LSH.html)
LSH(Location Sensitive Hash),即位置敏感哈希函数。为保序哈希，也就是散列前的相似点经过哈希之后，也能够在一定程度上相似，并且具有一定的概率保证。
形式化定义：
        对于任意q,p属于S，若从集合S到U的函数族H={h1,h2...hn}对距离函数D(,)，如欧式距离、曼哈顿距离等等，满足条件：
![image](http://hi.csdn.net/attachment/201005/28/0_1275042131VCyi.gif) 
 则称D(,)是位置敏感的。
如下图，空间上的点经位置敏感哈希函数散列之后，对于q，其rNN有可能散列到同一个桶（如第一个桶）,即散列到第一个桶的概率较大，会大于某一个概率阈值p1;而其(1+emxilong)rNN之外的对象则不太可能散列到第一个桶，即散列到第一个桶的概率很小，会小于某个阈值p2.
![image](http://hi.csdn.net/attachment/201005/28/0_1275042131m8SZ.gif)
**LSH的作用**
◆高维下近似查询
        相似性检索在各种领域特别是在视频、音频、图像、文本等含有丰富特征信息领域中的应用变得越来越重要。丰富的特征信息一般用高维向量表示，由此相似性检索一般通过K近邻或近似近邻查询来实现。一个理想的相似性检索一般需要满足以下四个条件：
1. 高准确性。即返回的结果和线性查找的结果接近。
2. 空间复杂度低。即占用内存空间少。理想状态下，空间复杂度随数据集呈线性增长，但不会远大于数据集的大小。
3. 时间复杂度低。检索的时间复杂度最好为O（1）或O（logN)。
4. 支持高维度。能够较灵活地支持高维数据的检索。
       传统主要方法是基于空间划分的算法——tree类似算法，如R-tree，Kd-tree，SR-tree。这种算法返回的结果是精确的，但是这种算法在高维数据集上的时间效率并不高。实验[1]指出维度高于10之后，基于空间划分的算法时间复杂度反而不如线性查找。LSH方法能够在保证一定程度上的准确性的前提下，时间和空间复杂度得到降低，并且能够很好地支持高维数据的检索。
**◆分类和聚类**
       根据LSH的特性，即可将相近（相似）的对象散列到同一个桶之中，则可以对图像、音视频、文本等丰富的高维数据进行分类或聚类。
◆数据压缩。如广泛地应用于信号处理及数据压缩等领域的Vector Quantization量子化技术。
总而言之，哪儿需要近似kNN查询，哪儿都能用上LSH.
[1] Weber R, Schek H, Blott S. A quantitative analysis and performance study for similarity search methods in high dimensional spaces Proc.of the 24th Intl.Conf.on Very Large Data Bases (VLDB).1998:194-205
LSH的经典论文作者：[http://www.informatik.uni-trier.de/~ley/pers/hd/m/Ma:Yi](http://www.informatik.uni-trier.de/~ley/pers/hd/m/Ma:Yi)
[http://research.microsoft.com/en-us/people/mayi/publications.aspx](http://research.microsoft.com/en-us/people/mayi/publications.aspx)
32.[Segmentation of Natural Images by Texture and Boundary Compression,](http://yima.csl.illinois.edu/psfile/ijcv09-texture.pdf)
Hossein Mobahi, Shankar Rao, Allen Yang, Shankar Sastry, and**Yi Ma**, submitted to the International Journal of Computer Vision (IJCV), March 2010.
X: Compact Projection: Simple and Efficient Near Neighbor Search with Practical Memory Requirements
**二：方法：Methods**
### 汉明Bit重采样：Bit sampling for Hamming distance（original hash ）
One of the easiest ways to construct an LSH family is by bit sampling.[[3]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-IndykMotwani98-3)
 This approach works for the [Hamming distance](http://en.wikipedia.org/wiki/Hamming_distance) over d-dimensional vectors ![\{0,1\}^d](http://upload.wikimedia.org/math/4/5/e/45e7ee8a83d88274ff77c99dde5fe1bc.png). Here, the family![\mathcal F](http://upload.wikimedia.org/math/e/b/a/eba702bd306ff22b00791b8802453c73.png)
 of hash functions is simply the family of all the projections of points on one of the![d](http://upload.wikimedia.org/math/8/2/7/8277e0910d750195b448797616e091ad.png) coordinates, i.e.,  必须把特征转化到汉明空间，利用汉明距离；![{\mathcal F}=\{h:\{0,1\}^d\to \{0,1\}\mid h(x)=x_i,i =1 ... d\}](http://upload.wikimedia.org/math/0/f/c/0fc872171b6570b9cf01bf9d9c1a71d0.png),
 where ![x_i](http://upload.wikimedia.org/math/0/5/e/05e42209d67fe1eb15a055e9d3b3770e.png) is the![i](http://upload.wikimedia.org/math/8/6/5/865c0c0b4ab0e063e5caa3387c1a8741.png)th coordinate of![x](http://upload.wikimedia.org/math/9/d/d/9dd4e461268c8034f5c8564e155c67a6.png).
 A random function![h](http://upload.wikimedia.org/math/2/5/1/2510c39011c5be704182423e3a695e91.png) from![{\mathcal F}](http://upload.wikimedia.org/math/e/b/a/eba702bd306ff22b00791b8802453c73.png) simply selects
 a random bit from the input point. This family has the following parameters:![P_1=1-R/d](http://upload.wikimedia.org/math/a/5/b/a5b12a4a58a1fcb91f76ecbb81d557d7.png),![P_2=1-cR/d](http://upload.wikimedia.org/math/9/e/0/9e034d5cb8ac3b7a6d00a79dbdddf800.png).
### Min-wise independent permutations
Main article:[MinHash   ：最小hash！](http://en.wikipedia.org/wiki/MinHash)
Suppose ![U](http://upload.wikimedia.org/math/4/c/6/4c614360da93c0a041b22e537de151eb.png) is composed of subsets of some ground set of enumerable items![S](http://upload.wikimedia.org/math/5/d/b/5dbc98dcc983a70728bd082d1a47546e.png)
 and the similarity function of interest is the[Jaccard index](http://en.wikipedia.org/wiki/Jaccard_index)![J](http://upload.wikimedia.org/math/f/f/4/ff44570aca8241914870afbc310cdb85.png).
 If![\pi](http://upload.wikimedia.org/math/5/2/2/522359592d78569a9eac16498aa7a087.png) is a permutation on the indices of![S](http://upload.wikimedia.org/math/5/d/b/5dbc98dcc983a70728bd082d1a47546e.png), for![A \subseteq S](http://upload.wikimedia.org/math/4/7/f/47f567560e6dcd6ca31fb80b84413541.png)
 let![h(A) = \min_{a \in A} \{ \pi(a) \}](http://upload.wikimedia.org/math/3/3/1/331a3d56385f9f83491396e77a79f6c2.png). Each possible choice of![\pi](http://upload.wikimedia.org/math/5/2/2/522359592d78569a9eac16498aa7a087.png)
 defines a single hash function![h](http://upload.wikimedia.org/math/2/5/1/2510c39011c5be704182423e3a695e91.png) mapping input sets to integers.
Define the function family![H](http://upload.wikimedia.org/math/c/1/d/c1d9f50f86825a1a2302ec2449c17196.png) to be the set of all such functions and let![D](http://upload.wikimedia.org/math/f/6/2/f623e75af30e62bbd73d6df5b50bb7b5.png)
 be the uniform distribution. Given two sets![A,B \subseteq S](http://upload.wikimedia.org/math/1/f/b/1fbfb28b89e54b0fc955e7e3226b2530.png) the event that![h(A) = h(B)](http://upload.wikimedia.org/math/3/9/8/39860c7897b353cddbcd00c07086494c.png)
 corresponds exactly to the event that the minimizer of![\pi](http://upload.wikimedia.org/math/5/2/2/522359592d78569a9eac16498aa7a087.png) lies inside![A \bigcap B](http://upload.wikimedia.org/math/a/a/8/aa84614df1607a4651d1b4f4c9f541f9.png).
 As![h](http://upload.wikimedia.org/math/2/5/1/2510c39011c5be704182423e3a695e91.png) was chosen uniformly at random,![Pr[h(A) = h(B)] = J(A,B)\,](http://upload.wikimedia.org/math/a/9/a/a9a217f38e4d755925fc794bcd5af162.png)
 and![(H,D)\,](http://upload.wikimedia.org/math/8/c/3/8c38e569fd26e2bb86be342d6818cd82.png) define an LSH scheme for the Jaccard index.   集合的 jaccard距离：一般用来判定文本相似度；
Because the symmetric group on n elements has size n!, choosing a truly random permutation from the full symmetric group is infeasible for even moderately sized n. Because of this fact, there has
 been significant work on finding a family of permutations that is "min-wise independent" - a permutation family for which each element of the domain has equal probability of being the minimum under a randomly chosen![\pi](http://upload.wikimedia.org/math/5/2/2/522359592d78569a9eac16498aa7a087.png).
 It has been established that a min-wise independent family of permutations is at least of size![lcm(1, 2, ..., n) \ge e^{n-o(n)}](http://upload.wikimedia.org/math/7/5/9/759bd99f82d42314bb1a4a09e471cd8f.png).[[9]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-Broder1998-9)
 and that this boundary is tight[[10]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-10)
Because min-wise independent families are too big for practical applications, two variant notions of min-wise independence are introduced: restricted min-wise independent permutations families,
 and approximate min-wise independent families. Restricted min-wise independence is the min-wise independence property restricted to certain sets of cardinality at most k.[[11]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-Matousek2002-11)
 Approximate min-wise independence differs from the property by at most a fixed ![\epsilon](http://upload.wikimedia.org/math/c/5/0/c50b9e82e318d4c163e4b1b060f7daf5.png).[[12]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-Saks2000-12)
### Nilsimsa Hash
Main article:[Nilsimsa Hash](http://en.wikipedia.org/wiki/Nilsimsa_Hash)
**Nilsimsa** is an[anti-spam](http://en.wikipedia.org/wiki/Anti-spam_techniques) focused locality-sensitive hashing algorithm.[[13]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-13)
 The goal of Nilsimsa is to generate a hash digest of an email message such that the digests of two similar messages are similar to each other. Nilsimsa satisfies three requirements outlined by the paper's authors:
- The digest identifying each message should not vary signicantly (sic) for changes that can be produced automatically.
- The encoding must be robust against intentional attacks.
- The encoding should support an extremely low risk of false positives.
### Random projection：
The random projection method of LSH[[4]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-Charikar2002-4)
 (termed arccos by Andoni and Indyk [[14]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-Andoni2008-14)) is designed to approximate the[cosine
 distance](http://en.wikipedia.org/wiki/Cosine_distance) between vectors. The basic idea of this technique is to choose a random[hyperplane](http://en.wikipedia.org/wiki/Hyperplane) (defined by a normal unit vector![r](http://upload.wikimedia.org/math/4/b/4/4b43b0aee35624cd95b910189b3dc231.png))
 at the outset and use the hyperplane to hash input vectors.
Given an input vector![v](http://upload.wikimedia.org/math/9/e/3/9e3669d19b675bd57058fd4664205d2a.png) and a hyperplane defined by![r](http://upload.wikimedia.org/math/4/b/4/4b43b0aee35624cd95b910189b3dc231.png),
 we let![h(v) = sgn(v \cdot r)](http://upload.wikimedia.org/math/0/3/b/03b6e9b49c787184fb673407012d50c9.png). That is,![h(v) = \pm 1](http://upload.wikimedia.org/math/d/9/3/d93fe16814c87a50f6b04c648acf9383.png)
 depending on which side of the hyperplane![v](http://upload.wikimedia.org/math/9/e/3/9e3669d19b675bd57058fd4664205d2a.png) lies.
Each possible choice of![r](http://upload.wikimedia.org/math/4/b/4/4b43b0aee35624cd95b910189b3dc231.png) defines a single function. Let![H](http://upload.wikimedia.org/math/c/1/d/c1d9f50f86825a1a2302ec2449c17196.png)
 be the set of all such functions and let![D](http://upload.wikimedia.org/math/f/6/2/f623e75af30e62bbd73d6df5b50bb7b5.png) be the uniform distribution once again. It is not difficult to prove that, for two vectors![u,v](http://upload.wikimedia.org/math/7/0/3/703f259d2f1a609d112d61c642e0f17d.png),![Pr[h(u) = h(v)] = 1 - \frac{\theta(u,v)}{\pi}](http://upload.wikimedia.org/math/b/f/f/bffc2c783d9b8913a15a9d01397b93e3.png),
 where![\theta(u,v)](http://upload.wikimedia.org/math/b/8/d/b8dc9de54cbb8c6adc1e9fd6927bd640.png) is the angle between![u](http://upload.wikimedia.org/math/7/b/7/7b774effe4a349c6dd82ad4f4f21d34c.png) and![v](http://upload.wikimedia.org/math/9/e/3/9e3669d19b675bd57058fd4664205d2a.png).![1 - \frac{\theta(u,v)}{\pi}](http://upload.wikimedia.org/math/8/2/6/826224bf0d9a56b01a7c22d72868b033.png)
 is closely related to![\cos(\theta(u,v))](http://upload.wikimedia.org/math/f/9/5/f95410121a7e1fe85a5d405ed9fc5126.png).
In this instance hashing produces only a single bit. Two vectors' bits match with probability proportional to the cosine of the angle between them.
### Stable distributions：基于p稳定分布的hash.
      The hash function[[15]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-DIIM04-15)![h_{\mathbf{a},b} (\boldsymbol{\upsilon}) : \mathcal{R}^d\to \mathcal{N}](http://upload.wikimedia.org/math/0/3/8/038d8ca82f1aa2370f2072178c4907f4.png)
 maps a*d* dimensional vector ![\boldsymbol{\upsilon}](http://upload.wikimedia.org/math/7/e/3/7e3f66f8a74f16f898c0515becf53a4d.png) onto a set of integers映射到一个数轴线段区间的整数上. Each hash function in the family is indexed by a choice
 of random![\mathbf{a}](http://upload.wikimedia.org/math/3/c/4/3c47f830945ee6b24984ab0ba188e10e.png) and![b](http://upload.wikimedia.org/math/9/2/e/92eb5ffee6ae2fec3ad71c777531578f.png) where![\mathbf{a}](http://upload.wikimedia.org/math/3/c/4/3c47f830945ee6b24984ab0ba188e10e.png)
 is a*d* dimensional vector with entries chosen independently from a [stable distribution](http://en.wikipedia.org/wiki/Stable_distribution) and ![b](http://upload.wikimedia.org/math/9/2/e/92eb5ffee6ae2fec3ad71c777531578f.png) is a real number chosen uniformly from the range [0,r]. For a fixed![\mathbf{a},b](http://upload.wikimedia.org/math/d/3/b/d3b089c72c319c84c22bf857e3591ff7.png)
 the hash function![h_{\mathbf{a},b}](http://upload.wikimedia.org/math/a/a/5/aa5f32d77e5fab17e5073c2d2ceb5c50.png) is given by![h_{\mathbf{a},b} (\boldsymbol{\upsilon}) = \left \lfloor\frac{\mathbf{a}\cdot \boldsymbol{\upsilon}+b}{r} \right \rfloor](http://upload.wikimedia.org/math/b/7/f/b7fbac3f09285ab91025ced9ac5d4558.png).
Other construction methods for hash functions have been proposed to better fit the data.[[16]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-PJA10-16)
 In particular k-means hash functions are better in practice than projection-based hash functions, but without any theoretical guarantee.
**三：基于p稳定分布的LSH**
原文链接：[http://blog.sina.com.cn/s/blog_67914f2901019p3v.html](http://blog.sina.com.cn/s/blog_67914f2901019p3v.html)
        LSH是用局部敏感的方法解决近似最近邻搜索的问题。在原始的LSH方法中，通过将原始空间嵌入到Hamming空间中，将d维空间转换成d＇=Cd维的Hamming空间（C是指原始空间中点的坐标的最大值，具体情况参见上一部分中的第4节-算法步骤），使用(r,(1+e)r,1-r/d＇,1-(1+e)r/d＇)-敏感哈希函数来解决(r,e)-Neighbor问题。而后来提出的p-stableLSH算法中，不需要将原始空间嵌入到Hamming空间中，可以直接在欧几里得空间下进行局部敏感哈希运算。
1、背景介绍
 p-stableLSH应用在d维lp-norm下的欧几里得空间中，0<p<=2。p-stableLSH是LSH的进化版本，要解决的问题相同，而使用的方法和应用环境不同。因此，下面重点介绍p-stableLSH的应用环境，对于LSH的细节参见第一部分。
 p-stableLSH使用的(R,cR,p1,p2)-敏感哈希中，c=1+e，并且不失一般性，设R=1。下面的工作主要是确定在1（即R）和c（即cR）下的p1与p2。
2、v概念解释
 p-stableLSH之所以会叫这个名字，是因为该算法应用到p-stabledistribution（p-稳定分布）的概念。下面给出的就是p-稳定分布的概念：
 Def 1 :一个分布D称为p-稳定分布，如果对于任意n个实数v1,v2,…,vn和符合D分布的n个独立同分布随机变量X1,X2,…,Xn，都存在一个p>=0，使得变量Vi和 
 其映射 具有相同的分布，此处X是一个符合D分布的随机变量。
    p-稳定分布不是具体的分布，而是满足某条件的分布族。当p=1时，代表是标准柯西分布，密度函数为 p(x) =(1/3.14)*(1/(1+x*x))    ；当p=2时，代表是标准正态分布（高斯分布）。
     重点：**近邻**特性
**     p-stable分布有一个重要的应用，就是可以估计给定向量v在欧几里得空间p-norm下长度，记为||v||p。方法是对于取定的d维向量v，从p-稳定分布中抽取d个随机变量组成d维向量a，计算a与v的点积a.v（点积的概念是将向量对应位置的元素相乘后所有乘积之和），根据p-stable的定义，由于a.v=Add（Vi*Xi）,因此a.v与||v||pX是同分布的（X是p-stable分布的一个随机变量）。选取若干个向量a，计算多个a.v的值，称为向量v的“概略（sketch）”，利用v的“sketch”可以用来估算||v||p的值。**
3、局部敏感哈希函数
在p-stableLSH中，**a**与**v**的点积**a.v**不用来估计||**v**||p的值，而**是用来生成哈希函数族，且该哈希函数族是局部敏感的**（即空间中距离较近的点映射后发生冲突的概率高，空间中距离较远的点映射后发生冲突的概率低）。大体方法是将一条直线分成等长且长度为r的若干段，给映射到同一段的点赋予相同的hash值，映射到不同段的点赋予不同的hash值。(**a.v1**-**a.v2**)是映射后的距离，而其值与||**v1**-**v2**||pX同分布，因此，原始距离（||**v1**-**v2**||p）较小时，映射后的距离也小，因此使用点积来生成哈希函数族可以保持局部敏感性。
 哈希函数族的形式为：![h_{\mathbf{a},b} (\boldsymbol{\upsilon}) = \left \lfloor\frac{\mathbf{a}\cdot \boldsymbol{\upsilon}+b}{r} \right \rfloor](http://upload.wikimedia.org/math/b/7/f/b7fbac3f09285ab91025ced9ac5d4558.png)，其中b是(0,r)里的随机数，r为直线上分段的段长。哈希族中的函数根据**a**和b的不同建立函数索引。
从哈希函数族中随机选取一个哈希函数，现在估计两个向量**v1**和**v2**在该哈希函数下映射后发生冲突的概率。定义符合p-stable分布的随机变量绝对值的概率密度函数为fp(t)，设c=||**v1**-**v2**||p，则**a.v1**-**a.v2**与cX同分布，X为p-stable分布下的随机变量。给出概率的计算公式如下，之后会有详细分析。
Ｐ（Ｃ）＝Ｐ（ａ，ｂ）［ Ha,b（V1）= Ha,b（V2）］=积分（1/c *fp(t/c)(1/t/r)）t
因为|**a.v1**-**a.v2**|=||**v1**-**v2**||p|X|=c|X|，X为p-stable分布下的随机变量，|X|的概率密度函数为fp(t)。若要向量**v1**和**v2**映射后发生冲突，需要满足如下条件：**v1**和**v2**通过与**a**进行点积运算分别映射到一段长度为r线段后，再通过加b运算，能使映射后的点在同一条线段上。
**以下是对该概率公式正确性的证明：**
设点**a.v1**在点M处，点**a.v2**在点N处，此处设N点在靠近Q的位置。
**（一）b对映射后点的影响**
 在加b后，因为b>0，因此加b后点会后移。不失一般性，设r=1，则有以下两种情况：
（1）若映射到同一条线段上，不妨设为线段PQ（P为前端点，Q为后端点），设|MN|=t，|NQ|=m，则若要保证加b后点M和点N仍在同一条线段中，则要满足0<b<=m（此时加b后M,N仍在线段PQ中），或者t+m<=b<r（此时加b后点M,N落入下一条线段中）。
  （2）若映射到不同线段上，但|MN|<r（此时必在相邻线段中），不妨设相邻两条线段为PQ和QR，设|MQ|=m，则|QN|=t-m，则若要保证加b后点M和点N仍在同一条线段中，则要满足m<b<r-(t-m)。
可以看到，不管是那种情况，b的取值范围都是r-t，而b是(0,r)内的随机数，因此取得满足条件b的概率是(r-t)/r=1-t/r。现在只需讨论向量v1和v2经过a的点积映射后的距离为t的概率（因为讨论b是设|MN|=t，即b是在向量映射后距离为t的情况下讨论的），即求|**a.v1**-**a.v2**|=||**v1**-**v2**||p|X|=c|X|=t的概率。
**（二）点积对映射后点的影响：**
      因为随机变量|X|的概率密度函数为fp(x)，而这里要求的是c|X|=t的概率。在这里有一个误区，要注意的是，c|X|=t的概率并不是Pr(|X|=t/c)=fp(t/c)，这是因为|X|是连续随机变量，
 不能通过某点的概率来生成其密度函数，虽然密度函数的意义是fp(x)=Pr(|X|=x)，但反过来是不成立的。因此，要求c|X|=t的概率，只能通过密度函数的定义来解决。
  密度函数的大致定义是：对于随机变量X的分布函数F(x)，如果存在函数f(x)，使得F(x)是f(x)在全部定义域内（一般就可取负无穷到正无穷，随机变量取不到的地方概率为0）的积分，那么f(x)就称为X的概率密度函数。F(x)=Pr(X<x)，f(x)=Pr(X=x)。这里再强调一遍，对于连续型随机变量，第二个式子的反过来没有意义，因为连续型随机变量在某点的概率恒为0。而分布函数代表的是某段区域内概率之和，因此，第二个式子反过来推导是有意义的。
因此，要求c|X|=t的概率，可用如下方法：设随机变量Y=c|X|，则原始问题转化成求Y=t的概率。设|X|的分布函数为Fp(t)，Y的分布函数为Gp(t)，则Gp(t)=Pr(Y<t)=Pr(c|X|<t)=Pr(|X|<t/c)=Fp(t/c)，因此，c|X|=t的概率为Gp'(t)=Fp'(t/c)=1/c*fp(t/c)，这样，经过点积映射后，两向量在线上点的距离等于t的概率便求出来了。
至此，我们得到了原始空间中的两个向量经过点积运算后映射到线段上的距离为t的概率以及在距离为t的前提下加b后能落在同一线段上的概率。因为如果两个向量经过点积后映射到线段上的距离大于r，且b是(0,r)上的随机数，因此这种情况下不论b取多少，两点都不可能落入同一条线段上。因此，t的取值范围是(0,r)。综上所述，该概率公式得证。
在上概率公式中，对于给定的r，概率p(c)是关于c的单调递减函数。即，c=||**v1**-**v2**||越大，映射后发生冲突的概率就越小，这符合局部敏感哈希函数的要求。因此，所选取的哈希函数族是局部敏感哈希函数族，并且是(r1,r2,p1,p2)-敏感的，其中p1=p(1),p2=p(c),r2/r1=c。c>1时，满足p1>p2，r1<r2。
以上就是对p-stableLSH的讨论，它通过涉入稳定分布和点积的概念，实现了LSH算法在欧几里得空间下的直接应用，而不需要嵌入Hamming空间。p-stableLSH中，度量是欧几里得空间下的lp准则，即向量**v1**与**v2**的距离定义为||**v1**-**v2**||p，然后通过设定的哈希函数将原始点映射到直线的等长线段上，每条线段便相当于一个哈希桶，与LSH方法类似，距离较近的点映射到同一哈希桶（线段）中的概率大，距离较远的点映射到同一哈希桶中的概率小，正好符合局部敏感的定义。
**四：hash方法的使用过程：**
### Amplification：详细描述
Given a ![(d_1, d_2, p_1, p_2)](http://upload.wikimedia.org/math/f/e/5/fe5558ec5b92747bd11fbb8c7c1a2999.png)-sensitive family![\mathcal F](http://upload.wikimedia.org/math/e/b/a/eba702bd306ff22b00791b8802453c73.png),
 we can construct new families![\mathcal G](http://upload.wikimedia.org/math/b/4/0/b40a7a19cd8d1aab464d18c05d7c21d5.png) by either the AND-construction or OR-construction of![\mathcal F](http://upload.wikimedia.org/math/e/b/a/eba702bd306ff22b00791b8802453c73.png).[[1]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-MOMD-1)
To create an AND-construction, we define a new family![\mathcal G](http://upload.wikimedia.org/math/b/4/0/b40a7a19cd8d1aab464d18c05d7c21d5.png) of hash functions![g](http://upload.wikimedia.org/math/b/2/f/b2f5ff47436671b6e533d8dc3614845d.png),
 where each function![g](http://upload.wikimedia.org/math/b/2/f/b2f5ff47436671b6e533d8dc3614845d.png) is constructed from![k](http://upload.wikimedia.org/math/8/c/e/8ce4b16b22b58894aa86c421e8759df3.png) random
 functions![h_1, ..., h_k](http://upload.wikimedia.org/math/2/7/4/2740c01b88b2e3cb96850bdb1d947f6f.png) from![\mathcal F](http://upload.wikimedia.org/math/e/b/a/eba702bd306ff22b00791b8802453c73.png). We then
 say that for a hash function![g \in \mathcal G](http://upload.wikimedia.org/math/8/2/f/82f4e6fbe963a45d7de5c5f041a3c5c7.png),![g(x) = g(y)](http://upload.wikimedia.org/math/b/2/c/b2c580d6158397f995e0ac044b6fdb2d.png)
 if and only if all![h_i(x) = h_i(y)](http://upload.wikimedia.org/math/d/a/8/da84a45a31a4247116d449521d8ad1dd.png) for![i = 1, 2, ..., k](http://upload.wikimedia.org/math/b/a/0/ba069691331d7a0885ad489d2f755c00.png).
 Since the members of![\mathcal F](http://upload.wikimedia.org/math/e/b/a/eba702bd306ff22b00791b8802453c73.png) are independently chosen for any![g \in \mathcal G](http://upload.wikimedia.org/math/8/2/f/82f4e6fbe963a45d7de5c5f041a3c5c7.png),![\mathcal G](http://upload.wikimedia.org/math/b/4/0/b40a7a19cd8d1aab464d18c05d7c21d5.png)
 is a![(d_1, d_2, p_{1}^r, p_{2}^r)](http://upload.wikimedia.org/math/c/b/b/cbb623639cfb3d05a105c9b3a56f8b0d.png)-sensitive family.
To create an OR-construction, we define a new family![\mathcal G](http://upload.wikimedia.org/math/b/4/0/b40a7a19cd8d1aab464d18c05d7c21d5.png) of hash functions![g](http://upload.wikimedia.org/math/b/2/f/b2f5ff47436671b6e533d8dc3614845d.png),
 where each function![g](http://upload.wikimedia.org/math/b/2/f/b2f5ff47436671b6e533d8dc3614845d.png) is constructed from![k](http://upload.wikimedia.org/math/8/c/e/8ce4b16b22b58894aa86c421e8759df3.png) random
 functions![h_1, ..., h_k](http://upload.wikimedia.org/math/2/7/4/2740c01b88b2e3cb96850bdb1d947f6f.png) from![\mathcal F](http://upload.wikimedia.org/math/e/b/a/eba702bd306ff22b00791b8802453c73.png). We then
 say that for a hash function![g \in \mathcal G](http://upload.wikimedia.org/math/8/2/f/82f4e6fbe963a45d7de5c5f041a3c5c7.png),![g(x) = g(y)](http://upload.wikimedia.org/math/b/2/c/b2c580d6158397f995e0ac044b6fdb2d.png)
 if and only if![h_i(x) = h_i(y)](http://upload.wikimedia.org/math/d/a/8/da84a45a31a4247116d449521d8ad1dd.png) for one or more values of![i](http://upload.wikimedia.org/math/8/6/5/865c0c0b4ab0e063e5caa3387c1a8741.png).
 Since the members of![\mathcal F](http://upload.wikimedia.org/math/e/b/a/eba702bd306ff22b00791b8802453c73.png) are independently chosen for any![g \in \mathcal G](http://upload.wikimedia.org/math/8/2/f/82f4e6fbe963a45d7de5c5f041a3c5c7.png),![\mathcal G](http://upload.wikimedia.org/math/b/4/0/b40a7a19cd8d1aab464d18c05d7c21d5.png)
 is a![(d_1, d_2, 1- (1 - p_1)^r, 1 - (1 - p_2)^r)](http://upload.wikimedia.org/math/b/0/d/b0d5bb818d9e0b01adff26b541201f50.png)-sensitive family.  重点是：如何构建hash函数族...
**LSH algorithm for nearest neighbor search:算法步骤**
One of the main applications of LSH is to provide a method for efficient approximate[nearest
 neighbor search](http://en.wikipedia.org/wiki/Nearest_neighbor_search) algorithms. Consider an LSH family ![\mathcal F](http://upload.wikimedia.org/math/e/b/a/eba702bd306ff22b00791b8802453c73.png). The algorithm has two main parameters: the width parameter![k](http://upload.wikimedia.org/math/8/c/e/8ce4b16b22b58894aa86c421e8759df3.png)
 and the number of hash tables![L](http://upload.wikimedia.org/math/d/2/0/d20caec3b48a1eef164cb4ca81ba2587.png).
In the first step, we define a new family![\mathcal G](http://upload.wikimedia.org/math/b/4/0/b40a7a19cd8d1aab464d18c05d7c21d5.png) of hash functions![g](http://upload.wikimedia.org/math/b/2/f/b2f5ff47436671b6e533d8dc3614845d.png),
 where each function![g](http://upload.wikimedia.org/math/b/2/f/b2f5ff47436671b6e533d8dc3614845d.png) is obtained by concatenating![k](http://upload.wikimedia.org/math/8/c/e/8ce4b16b22b58894aa86c421e8759df3.png)
 functions![h_1, ..., h_k](http://upload.wikimedia.org/math/2/7/4/2740c01b88b2e3cb96850bdb1d947f6f.png) from![\mathcal F](http://upload.wikimedia.org/math/e/b/a/eba702bd306ff22b00791b8802453c73.png), i.e.,![g(p) = [h_1(p), ..., h_k(p)]](http://upload.wikimedia.org/math/8/8/6/886deee6e89bebf9f1b4cc97bbdeaed5.png).
 In other words, a random hash function![g](http://upload.wikimedia.org/math/b/2/f/b2f5ff47436671b6e533d8dc3614845d.png) is obtained by concatenating![k](http://upload.wikimedia.org/math/8/c/e/8ce4b16b22b58894aa86c421e8759df3.png)
 randomly chosen hash functions from![\mathcal F](http://upload.wikimedia.org/math/e/b/a/eba702bd306ff22b00791b8802453c73.png). The algorithm then constructs![L](http://upload.wikimedia.org/math/d/2/0/d20caec3b48a1eef164cb4ca81ba2587.png)
 hash tables, each corresponding to a different randomly chosen hash function![g](http://upload.wikimedia.org/math/b/2/f/b2f5ff47436671b6e533d8dc3614845d.png).
In the preprocessing step we hash all![n](http://upload.wikimedia.org/math/7/b/8/7b8b965ad4bca0e41ab51de7b31363a1.png) points from the data set![S](http://upload.wikimedia.org/math/5/d/b/5dbc98dcc983a70728bd082d1a47546e.png)
 into each of the![L](http://upload.wikimedia.org/math/d/2/0/d20caec3b48a1eef164cb4ca81ba2587.png) hash tables. Given that the resulting hash tables have only![n](http://upload.wikimedia.org/math/7/b/8/7b8b965ad4bca0e41ab51de7b31363a1.png)
 non-zero entries, one can reduce the amount of memory used per each hash table to![O(n)](http://upload.wikimedia.org/math/7/b/a/7ba55e7c64a9405a0b39a1107e90ca94.png) using standard[hash
 functions](http://en.wikipedia.org/wiki/Hash_functions).
Given a query point
![q](http://upload.wikimedia.org/math/7/6/9/7694f4a66316e53c8cdd9d9954bd611d.png), the algorithm iterates over the![L](http://upload.wikimedia.org/math/d/2/0/d20caec3b48a1eef164cb4ca81ba2587.png) hash functions![g](http://upload.wikimedia.org/math/b/2/f/b2f5ff47436671b6e533d8dc3614845d.png).
 For each![g](http://upload.wikimedia.org/math/b/2/f/b2f5ff47436671b6e533d8dc3614845d.png) considered, it retrieves the data points that are hashed into the same bucket as![q](http://upload.wikimedia.org/math/7/6/9/7694f4a66316e53c8cdd9d9954bd611d.png).
 The process is stopped as soon as a point within distance![cR](http://upload.wikimedia.org/math/f/e/0/fe0e71283be6ca769a1313a6b4060dd5.png) from![q](http://upload.wikimedia.org/math/7/6/9/7694f4a66316e53c8cdd9d9954bd611d.png)
 is found.
Given the parameters![k](http://upload.wikimedia.org/math/8/c/e/8ce4b16b22b58894aa86c421e8759df3.png) and![L](http://upload.wikimedia.org/math/d/2/0/d20caec3b48a1eef164cb4ca81ba2587.png),
 the algorithm has the following performance guarantees:
- preprocessing time:![O(nLkt)](http://upload.wikimedia.org/math/2/d/1/2d1421a5e6f73d736bbef3594ab49503.png), where![t](http://upload.wikimedia.org/math/e/3/5/e358efa489f58062f10dd7316b65649e.png)
 is the time to evaluate a function![h \in \mathcal F](http://upload.wikimedia.org/math/6/f/c/6fc4cd437e02d800251293bf2483d6a6.png) on an input point![p](http://upload.wikimedia.org/math/8/3/8/83878c91171338902e0fe0fb97a8c47a.png);
- space: ![O(nL)](http://upload.wikimedia.org/math/2/6/7/267c4a3a7956f2c7c49cc9e1d36a624a.png), plus the space for storing data points;
- query time: ![O(L(kt+dnP_2^k))](http://upload.wikimedia.org/math/e/7/c/e7c1458f27d270c98c85d707a51404c7.png);
- the algorithm succeeds in finding a point within distance![cR](http://upload.wikimedia.org/math/f/e/0/fe0e71283be6ca769a1313a6b4060dd5.png) from![q](http://upload.wikimedia.org/math/7/6/9/7694f4a66316e53c8cdd9d9954bd611d.png)
 (if there exists a point within distance![R](http://upload.wikimedia.org/math/e/1/e/e1e1d3d40573127e9ee0480caf1283d6.png)) with probability at least![1 - ( 1 - P_1^k ) ^ L](http://upload.wikimedia.org/math/4/7/3/47316af2c6732248a088e777bc071bf9.png);
For a fixed approximation ratio![c=1+\epsilon](http://upload.wikimedia.org/math/9/3/9/939cc719611da07c309ef3c9ed4f5c74.png) and probabilities![P_1](http://upload.wikimedia.org/math/4/7/d/47ddd0a8d1607438330cf19c0c1ac45e.png)
 and![P_2](http://upload.wikimedia.org/math/2/d/1/2d145e3684093dda8dbfe869afa543f9.png), one can set![k={\log n \over \log 1/P_2}](http://upload.wikimedia.org/math/5/c/d/5cda7631b01501affd1528ec1b209c08.png)
 and![L = n^{\rho}](http://upload.wikimedia.org/math/a/d/6/ad62610f94e8e3892633ffc3225d6507.png), where![\rho={\log P_1\over \log P_2}](http://upload.wikimedia.org/math/a/c/c/accb83b758a2a9db4f40cc7514e078c6.png).
 Then one obtains the following performance guarantees:
- preprocessing time:![O(n^{1+\rho}kt)](http://upload.wikimedia.org/math/4/7/9/4790a073ba268a180b7499053618c9fd.png);
- space: ![O(n^{1+\rho})](http://upload.wikimedia.org/math/1/e/c/1ec4fc11a7636695481f6008f31bf8ad.png), plus the space for storing data points;
- query time: ![O(n^{\rho}(kt+d))](http://upload.wikimedia.org/math/c/2/5/c2523591297b8db63bf111f777ae3034.png);
五：[Locality
 Sensitive Hashing(LSH)之随机投影法](http://www.strongczq.com/2012/04/locality-sensitive-hashinglsh%E4%B9%8B%E9%9A%8F%E6%9C%BA%E6%8A%95%E5%BD%B1%E6%B3%95.html)
原文链接：http://www.strongczq.com/2012/04/locality-sensitive-hashinglsh%E4%B9%8B%E9%9A%8F%E6%9C%BA%E6%8A%95%E5%BD%B1%E6%B3%95.html
 为什么随即投影法是可行的？应该怎么去函数族的参数？以及正确率表现？
# 1. 概述
[LSH](http://en.wikipedia.org/wiki/Locality-sensitive_hashing)是由文献[1]提出的一种用于高效求解最近邻搜索问题的Hash算法。LSH算法的基本思想是利用一个hash函数把集合中的元素映射成hash值，使得相似度越高的元素hash值相等的概率也越高。LSH算法使用的关键是针对某一种相似度计算方法，找到一个具有以上描述特性的hash函数。LSH所要求的hash函数的准确数学定义比较复杂，以下给出一种通俗的定义方式：
> 
对于集合S，集合内元素间相似度的计算公式为sim(*,*)。如果存在一个hash函数h(*)满足以下条件：存在一个相似度s到概率p的单调递增映射关系，使得S中的任意两个满足sim(a,b)>=s的元素a和b，h(a)=h(b)的概率大于等于p。那么h(*)就是该集合的一个LSH算法hash函数。
一般来说在最近邻搜索中，元素间的关系可以用相似度或者距离来衡量。如果用距离来衡量，那么距离一般与相似度之间存在单调递减的关系。以上描述如果使用距离来替代相似度需要在单调关系上做适当修改。
根据元素相似度计算方式的不同，LSH有许多不同的hash算法。两种比较常见的hash算法是随机投影法和min-hash算法。本文即将介绍的随机投影法适用于集合元素可以表示成向量的形式，并且相似度计算是基于向量之间夹角的应用场景，如余弦相似度。min-hash法在参考文献[2]中有相关介绍。
# 2 随机投影法(Random projection)
   假设集合S中的每个元素都是一个n维的向量：
，集合中两个元素和之间的相似度定义为
。
对于以上元素集合S的随机投影法hash函数h(*)可以定义为如下：
在n维空间中随机选取一个非零向量。考虑以该向量为法向量且经过坐标系原点的超平面，该超平面把整个n维空间分成了两部分，将法向量所在的空间称为正空间，另一空间为负空间。那么集合S中位于正空间的向量元素hash值为1，位于负空间的向量元素hash值为0。判断向量属于哪部分空间的一种简单办法是判断向量与法向量之间的夹角为锐角还是钝角，因此具体的定义公式可以写为
。
根据以上定义，假设向量和之间的夹角为，由于法向量是随机选取的，那么这两个向量未被该超平面分割到两侧（即hash值相等）的概率应该为：。假设两个向量的相似度值为s，那么根据,有
。因此，存在相似度s到概率p的单调递增映射关系，使得对于任意相似度大于等于s的两个元素，它们hash值相等的概率大于等于。所以，以上定义的hash值计算方法符合LSH算法的要求。
以上所描述的h(*)函数虽然符合LSH算法的要求，但是实用性不高。因为该hash函数只产生了两个hash值，没有达到hash函数将元素分散到多个分组的目的。为了增加不同hash值的个数，可以多次生成独立的函数h(*)，只有当两个元素的多个h(*)值都相等时才算拥有相同的hash值。根据该思路可以定义如下的hash函数H(*)：
。其中每个表示一个独立的h(*)函数，H(*)函数值的二进制表现形式中每一位都是一个h(*)函数的结果。
以H(*)为hash函数的话，两个相似度为s的元素具有相同hash值的概率公式为
。hash值的个数为。很容易看出H(*)函数同样也是符合LSH算法要求的。一般随机按投影算法选用的hash函数就是H(*)。其中参数b的取值会在后面小节中讨论。
# 3 随机投影法在最近邻搜索中的应用
###### 3.1 最近邻搜索
最近邻搜索可以简单的定义为：对于m个元素的集合T，为一个待查询元素q找到集合中相似度最高的k个元素。
最近邻搜索最简单的实现方法为：计算q与集合T中每一个元素的相似度，使用一个具有k个元素的大顶堆（优先队列）保存相似度计算结果（相似度值为key）。这种实现方法每一次查询都要遍历整个集合T来计算相似度，当m很大并且查询的频率很高的时候这种暴力搜索的方法无法满足性能要求。
当最近邻搜索的近邻要求并不是那么严格的时候，即允许top k近邻的召回率不一定为1（但是越高越好），那么可以考虑借助于LSH算法。
###### 3.2 随机投影法提高执行速度
这里我们介绍当集合T的元素和查询元素q为同维度向量(维度为n)，并且元素相似度计算方法为余弦相似度时，使用随机投影法来提高最近邻搜索的执行速度。具体的实现方法为：
> 
**预处理阶段：**使用hash函数H(*)计算集合T中所有元素的hash值，将集合T分成一个个分组，每个分组内的元素hash值均相等。用合适的数据结构保存这些hash值到分组的映射关系（如HashMap）。
> 
**查询阶段：**计算查询元素q的hash值H(q)，取集合T中所有hash值为H(q)的分组，以该分组内的所有元素作为候选集合，在候选该集合内使用简单的最近邻搜索方法寻找最相似的k个元素。
该方法的执行效率取决于H(*)的hash值个数，也就是分组的个数。理想情况下，如果集合T中的向量元素在空间中分布的足够均匀，那么每一个hash值对应的元素集合大小大致为。当m远大于向量元素的维度时，每次查询的速度可以提高到倍。
根据以上分析H(*)中b的取值越大算法的执行速度的提升越多，并且是指数级别的提升。但是，在这种情况下H(*)函数下的概率公式p(s)，实际上表示与查询元素q的相似度为s的元素的召回率。当b的取值越大时，top k元素的召回率必然会下降。因此算法执行速度的提升需要召回率的下降作为代价。例如：当b等于10时，如果要保证某个元素的召回率不小于0.9，那么该元素与查询元素q的相似度必须不小于0.9999998。
###### 3.3 提高召回率改进
为了在保证召回率的前提下尽可能提高算法的执行效率，一般可以进行如下改进：
> 
**预处理阶段：**生成t个独立的hash函数，根据这t个不同的hash函数，对集合T进行t种不同的分组，每一种分组方式下，同一个分组的元素在对应hash函数下具有相同的hash值。用合适的数据结构保存这些映射关系（如使用t个HashMap来保存）。
> 
**查询阶段：**对于每一个hash函数，计算查询元素q的hash值，将集合T中所对应的分组方式下hash值为的分组添加到该次查询的候选集合中。然后，在该候选集合内使用简单的最近邻搜索方法寻找最相似的k个元素。
以上改进使得集合中元素与查询元素q的t个hash值中，只要任意一个相等，那么该集合元素就会被加入到候选集中。那么，相似度为s的元素的召回率为
在执行效率上，预处理阶段由于需要计算t个hash函数的值，所以执行时间上升为t倍。查询阶段，如果单纯考虑候选集合大小对执行效率的影响，在最坏的情况下，t个hash值获得的列表均不相同，候选集集合大小的期望值为，查询速度下降至，与简单近邻搜索相比查询速度提升为倍。
下图是召回率公式在不同的b和t取值下的s-p曲线。我们通过这些曲线来分析这里引入参数t的意义。4条蓝色的线以及最右边红色的线表示当t取值为1（相当于没有引入t），而b的取值从1变化到5的过程，从图中可以看出随着b的增大，不同相似度下的召回率都下降的非常厉害，特别的，当相似度接近1时曲线的斜率很大，也就说在高相似度的区域，召回率对相似度的变化非常敏感。10条红色的线从右到左表示b的取值为5不变，t的取值从1到10的过程，从图中可以看出，随着t的增大，曲线的形状发生了变化，高相似度区域的召回率变得下降的非常平缓，而最陡峭的地方渐渐的被移动到相对较低的相似度区域。因此，从以上曲线的变化特点可以看出，引入适当的参数t使得高相似度区域在一段较大的范围内仍然能够保持很高的召回率从而满足实际应用的需求。
###### 3.4 参数选取
根据以上分析，H(*)函数的参数b越大查询效率越高，但是召回率越低；参数t越大查询效率越低但是召回率越高。因此选择适当参数b和t来折中查询效率与召回率之间的矛盾是应用好随机投影法的关键。下面提供一种在实际应用中选取b和t的参考方法。
根据实际应用的需要确定一对(s,p)，表示相似度大于等于s的元素，召回率的最低要求为p。然后将召回率公式表示成b-t之间的函数关系。根据(s,p)的取值，画出b-t的关系曲线。如s=0.8,p=0.95时的b-t曲线如下图所示。考虑具体应用中的实际情况，在该曲线上选取一组使得执行效率可以达到最优的(b,t)组合。
![image](http://www.strongczq.com/wp-content/uploads/2012/04/image_thumb6.png)
###### 3.5 关于最近邻文本搜索
在最近邻文本搜索中，一般待检索的文本或查询文本，都已被解析成一系列带有权重的关键词，然后通过余弦相似度公式计算两个文本之间的相似度。这种应用场景下的最近邻搜索与以上所提到的最近邻搜索问题相比存在以下两个特点：
- 如果把每个文本的带权重关键词表都看作是一个向量元素的话，每个关键词都是向量的一个维度，关键词权重为该维度的值。理论上可能关键词的个数并不确定（所有单词的组合都可能是一个关键词），因此该向量元素的维数实际上是不确定的。
- 由于关键词权重肯定是大于零的，所以向量元素的每一个维度的值都是非负的。
对于第一个特点，我们需要选取一个包含n个关键词的关键词集合，在进行文本相似度计算时只考虑属于该集合的关键词。也就是说，每一个文本都视为是一个n维度的向量，关键词权重体现为对应维度的值。该关键词集合可以有很多种生成办法，比如可以是网站上具有一定搜索频率的关键词集合，总的来说该关键词集合应当能够涵盖所有有意义并且具有一定使用频率的关键词。通常n的取值会比较大，如几十万到几百万，由于在使用随机投影算法时，每一个生成的随机向量维度都为n，这种情况下需要特别考虑利用这些高维随机向量对执行效率造成的影响，在确定b、t参数时需要考虑到这方面的影响。
对于第二个特点，由于向量元素各维度值都非负，那么这些元素在高维空间中只会出现在特定的区域中。比如当n为3时，只会出现在第一象限中。一个直观的感觉是在生成随机向量的时候，会不会生成大量的无用切割平面（与第一个象限空间不相交，使得所有元素都位于切割平面的同侧）。这些切割平面对应的H(*)函数hash值中的二进制位恒定为1或者0，对于提高算法执行速度没有帮助。以下说明这种担心是没有必要的：
> 
切割平面与第一象限空间不相交等价于其法向量的每一个维度值都有相同的符号（都为正或者负），否则总能在第一象限空间中找到两个向量与法向量的乘积符号不同，也就是在切割平面的两侧。那么，随机生成的n维向量所有维度值都同号的概率为，当n的取值很大时，该概率可以忽略不计。
# 参考文献
[1] P. Indyk and R. Motwani. Approximate Nearest Neighbor:Towards Removing the Curse of Dimensionality. In Proc. of the 30th Annual ACM Symposium on Theory of Computing, 1998, pp. 604–613.
[2] Google News Personalization: Scalable Online Collaborative Filtering
**后记：**
        然而最后，我使用了还是随机投影的方法，这个是正确率和速度的权衡![可怜](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cute.gif)
