# 压缩映射：简单最邻近搜索-（SLH）Simple Linear Hash - wishchinYang的专栏 - CSDN博客
2016年05月18日 19:24:24[wishchin](https://me.csdn.net/wishchin)阅读数：1051
Compact Projection: Simple and Efficient Near Neighbor Search with Practical memory Requirement
  Autor：Kerui Min1,2, Linjun Yang2, John Wright2, Lei Wu2,3, Xian-Sheng Hua2, Yi Ma2 
 Institute： 1 School of Computer Science, Fudan University  krmin@fudan.edu.cn
                  2 Microsoft Research Asia    {linjury,jowrig,xshua,mayi}@microsoft.com
                  3 MOE-MS Keynote Lab of MCC, University of Science and Technology of China  wulei@live.com
《压缩映射：根据实际内存需要的 一个简单而有效的 线性最近邻搜索方法》
    Image similarity search is a fundamental problem in   computer vision. Efficient similarity search across large image databases depends critically on the availability of compact image representations and good data structures for indexing them.  Numerous
 approaches to the problem of generating and indexing image codes have been presented in the literature, but existing schemes generally lack explicit estimates of the number of bits needed to effectively index a given large image database. We present a very
 simple algorithm for generating compact binary representations of imagery data, based on random projections. Our analysis gives the first explicit bound on the number of bits needed to effectively solve the indexing problem. When applied to real image search
 tasks, these theoretical improvements translate into practical performance gains: experimental resultsshow that the new method, while using significantly less memory, is several times faster than existing alternatives.
    图像相似度搜索是图像处理中的基本问题。对于大数据结构的 有效的相似性检索 严重依赖于图像表达的压缩可行性和 一个好的数据结构索引。生成和索引图像编码的很多方法被提出，但是 现存的算法无法给出 索引一个大数据库 精确的内存需要值。我们提出了 基于随机映射的 一个简单的 图像数据的二进制压缩表达。 我们的分析给出了 开创性的 清晰地陈述 解决索引问题的 内存需要。 当应用于真实图像数据库上时，这些原理有了下面显著的提高：实验结果显示出新方法 比其他现有方法 利用更少内存，并且快几倍。
**1. Introduction**
The growing popularity of online image and video sharing sites has made efficiently indexing and searching large image databases an extremely active area of research in recent  years [5]. Much of this research has focused on the fundamental problem of retrieving
 images that are as similar as possible to a given query image. This similarity search is a key component of many commercial image search engines, helping to organize results and remove near duplicates. Current state-of-the-art systems for similarity search
 generally built around approximately invariant image descriptors such as SIFT [10] or GIST [12]. These features are quantized
and their statistics are used to efficiently index the image database, using an inverted file system (see Figure 1). This methodology finds application throughout the computer vision literature, including areas that are not traditionally considered part of
 image search, such as 3D reconstruction [20] and face recognition [21].
   近几年大量增长的在线视频和图片分享网站产生了大量的有效的图像索引数据库。很多这些研究聚焦于 给出图像的 图像相似性检索，这种相似性检索是很多商业引擎的关键元素，用于帮助组织检索结果和移动相似性拷贝。现存的先进的相似性检索系统 主要围绕 旋转不变性描绘子 比如 SIFT 和全局SIFT。这些特征被量化 并用于来有效地 统计索引图像数据库，基于一个倒排文件系统。这种方法的贯通了 视觉的领域，包括 传统的部分图像检索，并且用于3D重建 和人脸识别。
    The practicality of such image retrieval systems depends critically on two factors: the quality of the image representation employed, and the efficiency of the indexing system. Unfortunately, these two factors are often in tension.Research in image descriptors
 generally suggests that better discrimination can be achieved with higher dimensional  feature descriptors. However, finding nearest neighbors in the high-dimensional feature space is impractical for webscale image databases. The naive nearest neighbor algorithm
 suffers from the “curse of dimensionality”, as do classical alternatives such as KD-trees [13] and its variants [6, 11], R-trees [7] etc.
         实际的图像检索系统严重依赖于两个因素：图像表达的质量 和索引系统的质量，不幸的是，这两个因素经常很受压力。在图像特征的研究上普遍显示更好的描述会造成更高维的特征向量，然而，寻找高维特征空间的最近邻方法 对于网站级别的图像数据是 不实际的。实际的近邻算法承受着“维数灾难”诅咒，同样 其他的经典算法如 K决策树和R-K决策树也有此问题。
     The difficulty of exact nearest neighbor search has led to the development of many alternative strategies, with varying levels of practicality and theoretical optimality. Weber et al. [18] proposed a vector approximation scheme to accelerate the sequential
 scan. Wang et al. [17] proposed a binary hash coding (BHC), which projected images from a high dimensional feature space into a low dimensional hash code space and then filtered similar images by nearest neighbor search. Weiss et al. [19] used a spectral graph
 based method to find a best hashing code space in which Hamming distance is correlated with the semantic similarity. Torralba et al. [15] proposed to thumbnail the images into small code to speed up the similarity search in a large database. While many of
 the above techniques demonstrate excellent performance on specific image retrieval tasks and databases, they generally lack theoretical guarantees or guidelines as to the number of bits needed to achieve a given level of performance. This lack of foundation
 makes their application to new databases mostly a matter of trial-and-error.
   随着各种水平的使用化和原理最优化发展，提取最近邻的研究 导致了其他策略的发展。Weber et al. [18] 提出了一个向量近似方案 来加快线性检索。Wang et al. [17]提出了一个二进制哈希编码，其把图像从一个高维特征空间投影到低维 哈希编码空间，然后利用最近邻搜索 过滤相似性图像。Weiss et al. [19]利用基于图谱的方法 在 与语义相似性有关的汉明距离空间 寻找最优哈希编码。Torralba et al. [15]提出一个极小化图像编码加快 大数据库的 相似性检索。
 以上很多技巧在特定的数据检索任务和数据集上有良好的表现，但普遍 在理论上不能给出一个 能达到在给出的结果要求上  可计算的内存空间。这种缺陷导致应用于新的数据库 几乎是一个 尝试--试错过程（没有普遍性）。
   On the other hand, researchers in the theoretical computer science community have developed approximate nearest neighbor algorithms with excellent performance guarantees, in some cases nearly optimal. The current state-of the-art in this area is arguably
 the locality sensitive hashing (LSH) approach of [2, 4], which exploits the distance preserving properties of random projections. However, in spite of their appealing properties, there are practical drawbacks to this and related algorithms, most importantly
 intensive memory requirements that hinder their application to real image search tasks. This difficulty manifests itself in theory as well as practice: state of-the-art schemes lack an explicit bound on the number of bits needed to effectively index a given
 set of data.
    另一方面，理论计算机科学团体已经提出一个接近最优的近邻算法，在一些情况下接近最优。现有在此领域最好的 公认的是局部敏感哈希（LSH）[2, 4]，它利用随机映射同时保留了距离；然而，尽管有吸引人的性能，但仍然有实际的缺陷，最终要的是内存敏感性 阻碍了算法的实际应用。这些困难在理论和实际上也显示出来：对于一个给定的数据集现有的方法无法给出一个内存占有的边界上界。
   In this paper, we study a very simple algorithm for generating compact binary representations of a given image database. Like LSH, our algorithm is based on random
projections. However, it achieves much better space complexity. We prove an explicit bound on the number of bits needed to encode the data with a certain guaranteed retrieval performance. Experimental results show that our algorithm, while using significantly
 less memory, is several times faster than existing alternatives.
    在此文中，我们研究出一个简单的算法 用于一个给定的数据集给出压缩映射。相似于LSH，我们的算法基于随机映射，但在空间复杂度上表现较好。我们给出了一个数据集的映射编码的内存需要边界，实验结果显示内存少，速度快。
**2. Problem Statement and Algorithm**
   Our algorithm takes as its input a large set of highd imensional vectors X = x1 . . . xn ∈ Rd. In the context of image retrieval, these could be feature descriptors representing n images. Given a query vector q ∈ Rd, the core retrieval task is to efficiently
 return a vector x¤ ∈ X that is as similar as possible q: the distance d(q, x¤) is as small as possible. The correct distance d(·, ·) may vary depending on application and dataset. However, if the image features
are well-chosen, the ℓp normshave been observed to perform well for retrieval tasks [10,16]. In this paper, we focus on the Euclidean case p = 2.
  我们的算法 如下：
输入为高维特征向量集合：vectors X = x1 . . . xn ∈ Rd.（在基于内容的图像检索里面，为图像特征描述子）
给出一个 查询向量：vector q ∈ Rd .（核心任务是 查询出q的最近邻 x在Rd中，使d(x,q)最小，在此文中 取欧氏距离）
 The most straightforward approach to this problem is to simply scan through the database and return the point x¤ ∈ X that minimizes d(q, xi), yielding an excellent
neighbor: d(q, x¤) = minx2X d(q, x). However, this approach requires O(dn) operations. In practical image retrieval scenarios, both n and d could be very large: web image
databases such as Flickr contain millions of images or more, while for excellent retrieval performance is generally believed to require high-dimensional image representations (e.g., d = 512 dimensions for the GIST descriptor [12]). This unfavorable complexity
 rules out the trivial algorithm for all but the smallest image databases. Fortunately, in applications such as image search it is not necessary to return the point that is nearest to the query point; a set of well chosen near neighbors may suffice. This gives
 the algorithm designer considerable freedom to trade off approximation error for search complexity. This problem can be formalized as follows:
Problem 1 ( c-approximate near neighbor (c-NN)). Given a setX of points in a d-dimensional space Rd, devise a data structure, which for any query point q ∈ Rd finds a point x¤ ∈ X such that d(q, x¤) ≤ c · minx2X d(q, x).
 最直接的方法是线性扫描数据库返回距离值，寻找最小距离，产生最优近邻。这种方法要求的时间为（O(dn)）。在实际的检索操作中n和d可能很大，并且图像特征必须维度很高才能区分大量图像(e.g., d = 512 dimensions for the GIST descriptor [12]).这种最优策略只能适用于很小的数据集。
幸运的是，诸如此类的图像搜索并不必要返回最近邻查询点，一个被选择的候选集便足够。这给了算法设计者足够的自由在检索错误率和搜索完备性之间一个权衡。可以描述如下：
    （c相似性近邻）：在D维空间的一系列点集Xi（向量），设计一个数据结构，对任意的查询点q，可以找到一个点使d(q,Xk)为min(q，X) ( X in Xi )
 This problem has received a great deal of attention in the theoretical computer science community (see [14] and the references therein). Some of the simplest and best performing algorithms for solving it rely on the fact that in high dimensional spaces,
 random projections preserve distances with very high probability. More precisely, if
we sample a random vector r ∈ Rd with components iid N(0, 1), then var(r · (x − y)) ∼ kx − yk22. Locality Sen-sitive Hashing (LSH) builds on this property to give a stateof-the-art approximate nearest neighbor scheme [2]. However, that algorithm relies on an
 amplification technique that essentially trades off storage space for query time. The result is a scheme that is extremely memory intensive, requiring superlinear space, which limits its practical applicability to large image databases.
   此问题受到理论计算机科学的极大注意(see [14] and the references therein)。许多简单有效的算法依赖于事实：随机映射很大概率上 保持了 样本距离。     .........LSH基于此属性给出了合适的最近邻方法[2]。但是方法本质上依赖于 储存空间和查询时间的折中。结果是此种方法是极度内存敏感的，依赖于超线性空间，影响了其实际应用。
重点：算法 本文的算法：
        我们提出一个可以缓解LSH内存瓶颈的方法，利用一个更简单的hash方法。
Algorithm 1 ：Compact Projection Generation
**Input:** Data samples x1 . . . xn ∈ Rd.
**Output:** Compact projections y1 . . . yn ∈ {0, 1}^k.
   Generate R ∈ R^k×d  with independent normal entries
    Rij ∼ N(0, 1).
     for each i = 1 . . . n do
         Compute Rxi
          Set yi = σ(Rxi).
     end for
