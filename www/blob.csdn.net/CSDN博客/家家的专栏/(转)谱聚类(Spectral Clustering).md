# (转)谱聚类(Spectral Clustering) - 家家的专栏 - CSDN博客





2010年11月23日 20:50:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：5032








广义上来说，任何在算法中用到SVD/特征值分解的，都叫Spectral Algorithm。

Spectral Clustering，中文通常称为“谱聚类”。由于使用的矩阵的细微差别，谱聚类实际上可以说是一“类”算法。

Spectral Clustering 和传统的聚类方法（例如 K-means）比起来有不少优点：

1）和 K-medoids 类似，Spectral Clustering 只需要数据之间的相似度矩阵就可以了，而不必像 K-means 那样要求数据必须是 N 维欧氏空间中的向量。

2）由于抓住了主要矛盾，忽略了次要的东西，因此比传统的聚类算法更加健壮一些，对于不规则的误差数据不是那么敏感，而且 performance 也要好一些。许多实验都证明了这一点。事实上，在各种现代聚类算法的比较中，K-means 通常都是作为 baseline 而存在的。

3）计算复杂度比 K-means 要小，特别是在像文本数据或者平凡的图像数据这样维度非常高的数据上运行的时候。

Spectral Clustering 算法的全貌：

1）根据数据构造一个 Graph ，Graph 的每一个节点对应一个数据点，将相似的点连接起来，并且边的权重用于表示数据之间的相似度。把这个 Graph 用邻接矩阵的形式表示出来，记为 W 。

2)把的每一列元素加起来得到N 个数，把它们放在对角线上（其他地方都是零），组成一个N*N的矩阵，记为D 。并令L = D - W 。

3)求出L的前k个特征值（在本文中，除非特殊说明，否则“前k个”指按照特征值的大小从小到大的顺序）以及对应的特征向量。

4)把这k个特征（列）向量排列在一起组成一个N*k的矩阵，将其中每一行看作k维空间中的一个向量，并使用 K-means 算法进行聚类。聚类的结果中每一行所属的类别就是原来 Graph 中的节点亦即最初的N个数据点分别所属的类别。

下面是Spectral Clustering 的一个简单的 Matlab 实现：

```
function idx = spectral_clustering(W, k)
    D = diag(sum(W));
    L = D-W;
 
    opt = struct('issym', true, 'isreal', true);
    [V dummy] = eigs(L, D, k, 'SM', opt);
 
    idx = kmeans(V, k);
end
```

      最后，我们再来看一下本文一开始说的 Spectral Clustering 的几个优点：

1）只需要数据的相似度矩阵就可以了。这个是显然的，因为 Spectral Clustering 所需要的所有信息都包含在W中。不过一般W并不总是等于最初的相似度矩阵——回忆一下，是我们构造出来的 Graph 的邻接矩阵表示，通常我们在构造 Graph 的时候为了方便进行聚类，更加强到“局部”的连通性，亦即主要考虑把相似的点连接在一起，比如，我们设置一个阈值，如果两个点的相似度小于这个阈值，就把他们看作是不连接的。另一种构造 Graph 的方法是将 n 个与节点最相似的点与其连接起来。

2)抓住了主要矛盾，忽略了次要的东西，Performance 比传统的 K-means 要好。实际上 Spectral Clustering 是在用特征向量的元素来表示原来的数据，并在这种“更好的表示形式”上进行 K-means 。

3)计算复杂度比 K-means 要小。这个在高维数据上表现尤为明显。例如文本数据，通常排列起来是维度非常高（比如，几千或者几万）的稀疏矩阵，对稀疏矩阵求特征值和特征向量有很高效的办法，得到的结果是一些 k 维的向量（通常 k 不会很大），在这些低维的数据上做 K-means 运算量非常小。但是对于原始数据直接做 K-means 的话，虽然最初的数据是稀疏矩阵，但是 K-means 中有一个求 Centroid 的运算，就是求一个平均值：许多稀疏的向量的平均值求出来并不一定还是稀疏向量，事实上，在文本数据里，很多情况下求出来的 Centroid 向量是非常稠密，这时再计算向量之间的距离的时候，运算量就变得非常大，直接导致普通的 K-means 巨慢无比，而 Spectral Clustering 等工序更多的算法则迅速得多的结果。


作者：[洞庭散人](http://phinecos.cnblogs.com/)

出处：[http://phinecos.cnblogs.com/](http://phinecos.cnblogs.com/)




