# Approximate Nearest Neighbors.接近最近邻搜索 - wishchinYang的专栏 - CSDN博客
2014年02月13日 16:31:23[wishchin](https://me.csdn.net/wishchin)阅读数：2786
****（一）：次优最近邻**：[http://en.wikipedia.org/wiki/Nearest_neighbor_search](http://en.wikipedia.org/wiki/Nearest_neighbor_search)有少量修改；如有疑问，请看链接原文.....**1.Survey:****Nearest neighbor search (NNS), also known as proximity search,similarity search or[closest point search](http://en.wikipedia.org/wiki/Closest_pair_of_points_problem),
 is an[optimization problem](http://en.wikipedia.org/wiki/Optimization_problem) for finding closest (or most similar) points. Closeness is typically expressed in terms of a dissimilarity
 function: The less similar are the objects the larger are the function values. Formally, the nearest-neighbor (NN) search problem is defined as follows: given a set*S* of points in a space*M* and a query point*q* ∈ *M*, find the
 closest point in*S* to*q*. [Donald Knuth](http://en.wikipedia.org/wiki/Donald_Knuth) in vol. 3 of *[The Art of Computer Programming](http://en.wikipedia.org/wiki/The_Art_of_Computer_Programming)* (1973) called it thepost-office problem,
 referring to an application of assigning to a residence the nearest post office. A direct generalization of this problem is a*k*-NN search, where we need to find the*k* closest points.寻找高维近邻点的最优化问题；
Most commonly *M* is a [metric space](http://en.wikipedia.org/wiki/Metric_space) and dissimilarity is expressed as a [distance metric](http://en.wikipedia.org/wiki/Distance_metric), which is symmetric and satisfies the [triangle inequality](http://en.wikipedia.org/wiki/Triangle_inequality). Even more common, *M* is taken to be the *d*-dimensional[vector space](http://en.wikipedia.org/wiki/Vector_space) where dissimilarity is measured using the[Euclidean
 distance](http://en.wikipedia.org/wiki/Euclidean_distance), [Manhattan distance](http://en.wikipedia.org/wiki/Taxicab_geometry) or other [distance metric](http://en.wikipedia.org/wiki/Statistical_distance). However, the dissimilarity function can be arbitrary. One example are asymmetric[Bregman divergences](http://en.wikipedia.org/wiki/Bregman_divergence), for which the
 triangle inequality does not hold.[[1]](http://en.wikipedia.org/wiki/Nearest_neighbor_search#cite_note-Cayton2008-1)
距离矩阵的度量问题；****2.方法：(遇到的问题：维数灾难；**)**
### Linear search(适合小范围的距离计算)
The simplest solution to the NNS problem is to compute the distance from the query point to every other point in the database, keeping track of the "best so far". This algorithm, sometimes referred
 to as the naive approach, has a [running time](http://en.wikipedia.org/wiki/Running_time) of *O*(*Nd*) where *N* is the [cardinality](http://en.wikipedia.org/wiki/Cardinality) of *S* and *d* is the dimensionality of *M*. There are no search data structures to maintain, so linear search has no space complexity beyond the storage of the database. Naive search can, on average, outperform space partitioning
 approaches on higher dimensional spaces.[[2]](http://en.wikipedia.org/wiki/Nearest_neighbor_search#cite_note-2)
### Space partitioning（决策树？）
Since the 1970s, [branch and bound](http://en.wikipedia.org/wiki/Branch_and_bound) methodology has been applied to the problem. In the case of Euclidean space this approach is known as[spatial
 index](http://en.wikipedia.org/wiki/Spatial_index) or spatial access methods. Several [space-partitioning](http://en.wikipedia.org/wiki/Space_partitioning) methods have been developed for solving the NNS problem. Perhaps the simplest is the[k-d tree](http://en.wikipedia.org/wiki/K-d_tree), which iteratively bisects the search space
 into two regions containing half of the points of the parent region. Queries are performed via traversal of the tree from the root to a leaf by evaluating the query point at each split. Depending on the distance specified in the query, neighboring branches
 that might contain hits may also need to be evaluated. For constant dimension query time, average complexity is*O*(log *N*)[[3]](http://en.wikipedia.org/wiki/Nearest_neighbor_search#cite_note-3)
 in the case of randomly distributed points, worst case complexity analyses have been performed.[[4]](http://en.wikipedia.org/wiki/Nearest_neighbor_search#cite_note-Lee1977-4)
 Alternatively the[R-tree](http://en.wikipedia.org/wiki/R-tree) data structure was designed to support nearest neighbor search in dynamic context, as it has efficient algorithms for insertions and deletions.
In case of general metric space branch and bound approach is known under the name of[metric
 trees](http://en.wikipedia.org/wiki/Metric_trees). Particular examples include [vp-tree](http://en.wikipedia.org/wiki/Vp-tree) and [BK-tree](http://en.wikipedia.org/wiki/BK-tree).
Using a set of points taken from a 3-dimensional space and put into a[BSP
 tree](http://en.wikipedia.org/wiki/Binary_space_partitioning), and given a query point taken from the same space, a possible solution to the problem of finding the nearest point-cloud point to the query point is given in the following description of an algorithm. (Strictly speaking, no such point may exist,
 because it may not be unique. But in practice, usually we only care about finding any one of the subset of all point-cloud points that exist at the shortest distance to a given query point.) The idea is, for each branching of the tree, guess that the closest
 point in the cloud resides in the half-space containing the query point. This may not be the case, but it is a good heuristic. After having recursively gone through all the trouble of solving the problem for the guessed half-space, now compare the distance
 returned by this result with the shortest distance from the query point to the partitioning plane. This latter distance is that between the query point and the closest possible point that could exist in the half-space not searched. If this distance is greater
 than that returned in the earlier result, then clearly there is no need to search the other half-space. If there is such a need, then you must go through the trouble of solving the problem for the other half space, and then compare its result to the former
 result, and then return the proper result. The performance of this algorithm is nearer to logarithmic time than linear time when the query point is near the cloud, because as the distance between the query point and the closest point-cloud point nears zero,
 the algorithm needs only perform a look-up using the query point as a key to get the correct result.
空间划分是一个构建空间树的过程，其构建过程比较复杂，涉及到大量的计算；
### Locality sensitive hashing（hash过程，可以近似O(1)的时间查询表）
[Locality sensitive hashing](http://en.wikipedia.org/wiki/Locality_sensitive_hashing) (LSH) is a technique
 for grouping points in space into 'buckets' based on some distance metric operating on the points. Points that are close to each other under the chosen metric are mapped to the same bucket with high probability.[[5]](http://en.wikipedia.org/wiki/Nearest_neighbor_search#cite_note-5)
### Nearest neighbor search in spaces with small intrinsicdimension
The [cover tree](http://en.wikipedia.org/wiki/Cover_tree) has a theoretical bound that is based on the dataset's [doubling constant](http://en.wikipedia.org/w/index.php?title=Doubling_constant&action=edit&redlink=1). The bound on search time is *O*(*c*12 log *n*) where*c* is the[expansion
 constant](http://en.wikipedia.org/wiki/Expansivity_constant) of the dataset.
### Vector approximation files
In high dimensional spaces, tree indexing structures become useless because an increasing percentage of the nodes need to be examined anyway. To speed up linear search, a compressed version of the
 feature vectors stored in RAM is used to prefilter the datasets in a first run. The final candidates are determined in a second stage using the uncompressed data from the disk for distance calculation.[[6]](http://en.wikipedia.org/wiki/Nearest_neighbor_search#cite_note-6)
### Compression/clustering based search
The VA-file approach is a special case of a compression based search, where each feature component is compressed uniformly and independently. The optimal compression technique in multidimensional
 spaces is Vector Quantization (VQ), implemented through clustering. The database is clustered and the most "promising" clusters are retrieved. Huge gains over VA-File, tree-based indexes and sequential scan have been observed.[[7]](http://en.wikipedia.org/wiki/Nearest_neighbor_search#cite_note-7)[[8]](http://en.wikipedia.org/wiki/Nearest_neighbor_search#cite_note-8)
 Also note the parallels between clustering and LSH.
**3.次优最近邻**
Algorithms that support the approximate nearest neighbor search include[locality-sensitive
 hashing](http://en.wikipedia.org/wiki/Locality-sensitive_hashing#LSH_algorithm_for_nearest_neighbor_search),[best bin first](http://en.wikipedia.org/wiki/Best_bin_first) and[balanced
 box-decomposition tree](http://en.wikipedia.org/w/index.php?title=Balanced_box-decomposition_tree&action=edit&redlink=1) based search.[[9]](http://en.wikipedia.org/wiki/Nearest_neighbor_search#cite_note-9)
(1):**ε-approximate nearest neighbor search** is a special case of the[nearest neighbor search](http://en.wikipedia.org/wiki/Nearest_neighbor_search) problem. The solution
 to the ε-approximate nearest neighbor search is a point or multiple points within distance (1+ε) R from a query point, where R is the distance between the query point and its true nearest neighbor.
Reasons to approximate nearest neighbor search include the space and time costs of exact solutions in high dimensional spaces (see[curse
 of dimensionality](http://en.wikipedia.org/wiki/Curse_of_dimensionality)) and that in some domains, finding an approximate nearest neighbor is an acceptable solution.
Approaches for solving
**ε-approximate nearest neighbor search** include[kd-trees](http://en.wikipedia.org/wiki/Kd-tree),[Locality
 Sensitive Hashing](http://en.wikipedia.org/wiki/Locality_Sensitive_Hashing) and[brute force search](http://en.wikipedia.org/wiki/Brute_force_search).
(2):
**Best bin first** is a
[search algorithm](http://en.wikipedia.org/wiki/Search_algorithm) that is designed to efficiently find an approximate solution to the
[nearest neighbor search](http://en.wikipedia.org/wiki/Nearest_neighbor_search) problem** in very-high-dimensional spaces**. The algorithm is based on a variant of the[kd-tree](http://en.wikipedia.org/wiki/Kd-tree)
 search algorithm which makes indexing higher dimensional spaces possible. Best bin first is an approximate algorithm which returns the nearest neighbor** for a large fraction of queries and a very close neighbor otherwise**.[[1]](http://en.wikipedia.org/wiki/Best_bin_first#cite_note-1)
## Differences from kd tree:
- Backtracking is according to a priority queue based on closeness.
- Search a fixed number of nearest candidates and stop.
- A speedup of two orders of magnitude is typical.                                        主要是对于超大型数据库进行相似性查询；
## References:Beis, J.; Lowe, D. G. (1997). "Shape indexing using approximate nearest-neighbour search in high-dimensional spaces". Conference on Computer Vision and Pattern Recognition. Puerto Rico. pp. 1000–1006.[CiteSeerX](http://en.wikipedia.org/wiki/CiteSeer#CiteSeerX):[10.1.1.23.9493](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.23.9493).
(3):LSH:[http://en.wikipedia.org/wiki/Locality_sensitive_hashing](http://en.wikipedia.org/wiki/Locality_sensitive_hashing)
**Locality-sensitive hashing** (**LSH**) is a method of performing probabilistic[dimension reduction](http://en.wikipedia.org/wiki/Dimension_reduction)
 of high-dimensional data. The basic idea is to[hash](http://en.wikipedia.org/wiki/Hash_Function) the input items so that similar items are mapped to the same buckets with
 high probability (the number of buckets being much smaller than the universe of possible input items). This is different from the conventional hash functions, such as those used in[cryptography](http://en.wikipedia.org/wiki/Cryptography),
 as in the LSH case the goal is to maximize probability of "collision" of similar items rather than avoid collisions.[[1]](http://en.wikipedia.org/wiki/Locality_sensitive_hashing#cite_note-MOMD-1)
 Note how locality-sensitive hashing, in many ways, mirrors [data clustering](http://en.wikipedia.org/wiki/Cluster_analysis) and[Nearest neighbor search](http://en.wikipedia.org/wiki/Nearest_neighbor_search).
**（二）：局部敏感hash：（三）：minihash：**原文链接：**[http://my.oschina.net/pathenon/blog/65210](http://my.oschina.net/pathenon/blog/65210)[转自wiki：http://en.wikipedia.org/wiki/Locality_sensitive_hashing](http://en.wikipedia.org/wiki/Locality_sensitive_hashing)**
    传统的hash算法只负责将原始内容尽量均匀随机地映射为一个签名值，原理上相当于伪随机数产生算法。传统hash算法产生的两个签名，如果相等，说明原始内容在一定概率下是相等的；如果不相等，除了说明原始内容不相等外，不再提供任何信息，因为即使原始内容只相差一个字节，所产生的签名也很可能差别极大。从这个意义上来说，**要设计一个hash算法，对相似的内容产生的签名也相近，是更为艰难的任务**，因为它的签名值除了提供原始内容是否相等的信息外，还能额外提供不相等的原始内容的差异程度的信息。
    MinHash[1最小哈希方法可看作是[局部性敏感哈希](http://en.wikipedia.org/wiki/locality_sensitive_hashing)的一个实例。局部性敏感哈希是使用哈希将大集合的数据对象映射到更小的哈希值的技术集合，通过这样的方法当两个对象距离相近时，它们的哈希值也可以相同。在最小哈希方法实例中，一个集合的签名可看作是它的哈希值。其它局部性敏感哈希技术还有针对集合间的[海明距离](http://en.wikipedia.org/wiki/Hamming_distance)，以及[向量](http://zh.wikipedia.org/wiki/%E5%90%91%E9%87%8F)间的[余弦距离](http://en.wikipedia.org/wiki/cosine_distance)等。另外，局部性敏感哈希还在[最近邻搜索](http://en.wikipedia.org/wiki/nearest_neighbor_search)算法有着重要的应用。[[9]](http://zh.wikipedia.org/wiki/%E6%9C%80%E5%B0%8F%E5%93%88%E5%B8%8C#cite_note-9)
1.**概述introduction**：
    跟SimHash一样，MinHash也是[LSH](http://en.wikipedia.org/wiki/Locality-sensitive_hashing)的一种，可以用来快速估算两个集合的相似度。MinHash由Andrei
 Broder提出，最初用于在搜索引擎中检测重复网页。它也可以应用于大规模聚类问题。
**2.Jaccard index：雅可比相似度与最小哈希值**
      在介绍MinHash之前，我们先介绍下Jaccard index。
     Jaccard index是用来计算相似性，也就是距离的一种度量标准。假如有集合A、B，那么，
![](http://static.oschina.net/uploads/space/2012/0706/132840_VcO0_576409.jpg)
      也就是说，集合A,B的Jaccard系数等于A,B中共同拥有的元素数与A,B总共拥有的元素数的比例。很显然，Jaccard系数值区间为[0,1]。
       假定*h*是一个将*A*和*B*中的元素映射到一些不相交整数的[哈希函数](http://zh.wikipedia.org/wiki/%E5%93%88%E5%B8%8C%E5%87%BD%E6%95%B0)，而且针对给定的*S*，定义*h*min(*S*)为S集合中具有最小h(x)函数值的元素x。这样，只有当最小哈希值的并集*A*
 ∪ *B*依赖于交集*A* ∩*B*时，有*h*min(*A*) =*h*min(*B*)。 因此，
Pr[*h*min(*A*) =*h*min(*B*)] =*J*(*A*,*B*).
        另一方面来说，如果*r*是一个当*h*min(*A*) =*h*min(*B*)时值为1，其它情况下值为0的随机变量，那么*r*可认为是*J*(*A*,*B*)的[无偏估计](http://en.wikipedia.org/wiki/unbiased_estimator)。尽管此时[方差](http://zh.wikipedia.org/wiki/%E6%96%B9%E5%B7%AE)过高，单独使用时没什么用处。最小哈希方法的思想是通过**平均用同一方式构造的许多随机变量，从而减少方差**。
**3.MinHash：多哈希函数，单一哈希函数：**
    先定义几个符号术语：
    h(x):  把x映射成一个整数的哈希函数。   
*h*min(*S*)：集合S中的元素经过h(x)哈希后，具有最小哈希值的元素。
    那么对集合A、B，*h*min(*A*) = *h*min(*B*)成立的条件是*A* ∪ *B* 中具有最小哈希值的元素也在 ∩ B中。这里有一个假设，h(x)是一个良好的哈希函数，它具有很好的均匀性，能够把不同元素映射成不同的整数。
    所以有，Pr[*h*min(*A*)
 = *h*min(*B*)] = *J*(*A*,*B*)，即集合A和B的相似度为集合A、B经过hash后最小哈希值相等的概率。
   有了上面的结论，我们便可以根据MinHash来计算两个集合的相似度了。一般有两种方法：
**第一种：使用多个hash函数**
        为了计算集合A、B具有最小哈希值的概率，我们可以选择一定数量的hash函数，比如K个。然后用这K个hash函数分别对集合A、B求哈希值，对
每个集合都得到K个最小值。比如Min(A)k={a1,a2,...,ak}，Min(B)k={b1,b2,...,bk}。
        那么，集合A、B的相似度为|Min(A)k ∩ Min(B)k|
 / |Min(A)k  ∪  Min(B)k|，及Min(A)k和Min(B)k中相同元素个数与总的元素个数的比例。
**第二种：使用单个hash函数**
    第一种方法有一个很明显的缺陷，那就是计算复杂度高。使用单个hash函数是怎么解决这个问题的呢？请看：
前面我们定义过 *h*min(*S*)为集合S中具有最小哈希值的一个元素，那么我们也可以定义*h*mink(*S*)为集合S中具有最小哈希值的K个元素。这样一来，
我们就只需要对每个集合求一次哈希，然后取最小的K个元素。计算两个集合A、B的相似度，就是集合A中最小的K个元素与集合B中最小的K个元素
的交集个数与并集个数的比例。
     看完上面的，你应该大概清楚MinHash是怎么回事了。但是，**MinHash的好处到底在哪里呢？**计算两篇文档的相似度，就直接统计相同的词数和总的
次数，然后就Jaccard index不就可以了吗？对，如果仅仅对两篇文档计算相似度而言，MinHash没有什么优势，反而把问题复杂化了。但是如果有海量的文档需要求相似度，比如在推荐系统
中计算物品的相似度，如果两两计算相似度，计算量过于庞大。下面我们看看MinHash是怎么解决问题的。
     比如元素集合{a,b,c,d,e},其中s1={a,d},s2={c},s3={b,d,e},s4={a,c,d}那么这四个集合的矩阵表示为：
[](http://my.oschina.net/pathenon/blog/undefined)![](http://static.oschina.net/uploads/space/2012/0706/133057_OKX6_576409.jpeg)
       如果要对某一个集合做MinHash，则可以从上面矩阵的任意一个行排列中选取一个，然后MinHash值是排列中第一个1的行号。
    例如，对上述矩阵，我们选取排列
beadc，那么对应的矩阵为
[](http://my.oschina.net/pathenon/blog/undefined)![](http://static.oschina.net/uploads/space/2012/0706/133117_cyk9_576409.jpeg)
        那么， 
h(S1) = a，同样可以得到h(S2) = c, h(S3) = b, h(S4) = a。
        如果只对其中一个行排列做MinHash，不用说，计算相似度当然是不可靠的。因此，我们要选择多个行排列来计算MinHash，最后根据Jaccard index公式来计算相似度。但是求排列本身的复杂度比较高，特别是针对很大的矩阵来说。因此，我们可以设计一个随机哈希函数去模拟排列，能够把行号0~n随机映射到0~n上。比如H(0)=100,H(1)=3...。当然，冲突是不可避免的，冲突后可以二次散列。并且如果选取的随机哈希函数够均匀，并且当n较大时，冲突发生的概率还是比较低的。
        说到这里，只是讨论了用MinHash对海量文档求相似度的具体过程，但是它到底是怎么减少复杂度的呢？
        比如有n个文档，每个文档的维度为m，我们可以选取其中k个排列求MinHash，由于每个对每个排列而言，MinHash把一篇文档映射成一个整数，所以对k个排列计算MinHash就得到k个整数。那么所求的MinHash矩阵为n*k维，而原矩阵为n*m维。n>>m时，计算量就降了下来。
**4.参考文献**
     (1):[http://en.wikipedia.org/wiki/MinHash](http://en.wikipedia.org/wiki/MinHash)
              (2)  :[http://fuliang.iteye.com/blog/1025638](http://fuliang.iteye.com/blog/1025638)
              (3)：**[^](http://zh.wikipedia.org/wiki/%E6%9C%80%E5%B0%8F%E5%93%88%E5%B8%8C#cite_ref-6)**Chum,
 Ond?ej; Philbin, James; Isard, Michael; Zisserman, Andrew, Scalable near identical image and shot detection, Proceedings of the 6th ACM International Conference on Image and Cideo Retrieval (CIVR'07). 2007,[doi](http://zh.wikipedia.org/wiki/DOI):[10.1145/1282280.1282359](http://dx.doi.org/10.1145%2F1282280.1282359);Chum,
 Ond?ej; Philbin, James; Zisserman, Andrew,[Near duplicate image detection: min-hash and tf-idf weighting](http://www.bmva.org/bmvc/2008/papers/119.pdf), Proceedings of the British
 Machine Vision Conference,
