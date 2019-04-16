# Metric Learning——度量学习 - 一个跳popping的quant的博客 - CSDN博客





2018年04月10日 15:30:29[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：5482








看到一篇知乎大神Flood Sung发表在CVPR2018上的paper，介绍了一种基于metric的模式识别方法，创新之处在于它不同于常用的matric-based方法，使用人为定义的度量，像简单的欧式距离、马氏距离，而是采用了用神经网络去训练这个度量，模型虽然简单，但是效果却很显著。

### 1、度量(Metric)

先说一下关于度量这个概念：在数学中，一个度量（或距离函数）是一个**定义集合中元素之间距离的函数**。一个具有度量的集合被称为**度量空间**。

### 2、度量学习的作用
度量学习也叫作相似度学习，根据这个叫法作用就很明确了。
之所以要进行度量学习，一方面在一些算法中需要依赖给定的度量：如Kmeans在进行聚类的时候就用到了欧式距离来计算样本点到中心的距离、KNN算法也用到了欧式距离等。这里计算的度量，就是比较样本点与中心点的相似度。

这里的度量学习在模式识别领域，尤其是在图像识别这方面，在比较两张图片是否是相同的物体，就通过比较两张图片的相似度，相似度大可能性就高。

因为在研究时间序列这方面的问题，所以想到了在时间序列中度量学习的体现，如果是判断两个区间的相似性，通常用到的度量方式就是采用常用到的欧式或者其他人为定义的距离函数，这样也就局限于了这样一个二维或者多维的空间中，而如果是用到Flood Sung大神提出的方法的话，我们把思路拓宽，能不能也是用神经网络来训练这个度量，这样的好处就是：
- 长度不同的片段也可以进行比较。
- 可以拓宽维度，从其他维度上寻找关联。

### 以下摘自：https://blog.csdn.net/nehemiah_li/article/details/44230053

### 3、常用到的度量学习方法

从广义上将度量学习分为：通过线性变换的度量学习和度量学习的非线性模型。

#### **3.1线性变换的度量学习**

线性的度量学习问题也称为马氏度量学习问题，可以分为监督的和非监督的学习算法。

##### 3.1.1监督的全局度量学习
- Information-theoretic metric learning(ITML)
- Mahalanobis Metric Learning for Clustering(MMC)
- Maximally Collapsing Metric Learning (MCML)

##### 3.1.2监督的局部度量学习
- Neighbourhood Components Analysis (NCA)
- Large-Margin Nearest Neighbors (LMNN)
- Relevant Component Analysis(RCA)
- Local Linear Discriminative Analysis(Local LDA)

##### 3.1.3非监督的度量学习
- 主成分分析(Pricipal Components Analysis, PCA)
- 多维尺度变换(Multi-dimensional Scaling, MDS)
- 非负矩阵分解(Non-negative Matrix Factorization,NMF)
- 独立成分分析(Independent components analysis, ICA)
- 邻域保持嵌入(Neighborhood Preserving Embedding,NPE)
- 局部保留投影(Locality Preserving Projections. LPP)

#### 3.2度量学习的非线性模型
非线性降维算法可以看作属于非线性度量学习：- 等距映射(Isometric Mapping,ISOMAP)
- 局部线性嵌入(Locally Linear Embedding, LLE) 

- 拉普拉斯特征映射(Laplacian Eigenmap，LE ) 


通过核方法来对线性映射进行扩展：
- Non-Mahalanobis Local Distance Functions
- Mahalanobis Local Distance Functions
- Metric Learning with Neural Networks

### 4、论文推荐
- Distance metric learning with application to clustering with side-information
- Information-theoretic metric learning(关于ITML)
- Distance metric learning for large margin nearest neighbor classification(关于LMNN)
- Learning the parts of objects by non-negative matrix factorization（Nature关于RCA的文章）
- Neighbourhood components analysis(关于NCA)
- Metric Learning by Collapsing Classes(关于MCML)
- Distance metric learning a comprehensive survey(一篇经典的综述)




### 个人总结

想了一下，度量学习定位的话应该是最基础的部分。现在在用的无论是深度学习、强化学习还是神经网络或是监督学习，为了避免结果发散或者收敛结果好些，在使用前一般需要一个特征转换或者聚类的处理，在进行特征转换或者聚类时最基本的和最容易忽略的部分就是关于度量的学习，使用人为定义的方法必然有其局限性，那么通过神经网络或者其他方法是否效果会更好呢，将这个可以进一步研究。




