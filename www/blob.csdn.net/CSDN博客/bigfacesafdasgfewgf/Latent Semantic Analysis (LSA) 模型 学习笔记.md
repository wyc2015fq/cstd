# Latent Semantic Analysis (LSA) 模型 学习笔记 - bigfacesafdasgfewgf - CSDN博客





2014年12月03日 13:57:34[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：4093








**Latent Semantic Analysis (LSA) 模型 学习笔记**



    Latent Semantic Analysis 模型，隐性语义分析，也就是我们常说的LSA模型。后面还有他的兄弟PLSA和LDA模型，这个我们后面再说。这几个都是NLP中比较经典的模型！学习这个模型，主要总结到了三个方面：LSA模型可以应用在哪儿？LSA的理论部分，以及LSA的优缺点分析。




# 1. LSA的应用




    LSA可以在VSM中降低样本的维度，并且可以从文本中发现隐含的语义维度。

   在VSM中，文档被表示成由特征词出现的概率组成的多位向量，这个方法的好处就是可以把一个文本转化成数值向量，然后做一些相似度计算，聚类分类等等。

    但是，在VSM中，无法处理一词多义和一义多词问题。例如，在VSM模型中，“被子”和“被褥”是两个完全不同的维度，而“笔记本”（notebook or laptop?）又被表示成相同的维度，因此不能够体现文本中隐含的语义。

    所以说，LSA模型可以用来挖掘文本中的语义信息，减轻一词多义和一义多词问题。




# **2. LSA的理论分析**




    LSA模型的理论部分是基于奇异值分解SVD的，这个SVD在数据挖掘领域是很常见的，基于SVD的算法还有著名的降维算法：主成份分析法，PCA（Primary Component Analysis）。而且，我觉得PCA和LSA在一定程度上非常相似，只不过LSA明确了应用背景是NLP。

    Step 1：在VSM模型中，一个文本被表示成一个向量，许多个文本被表示成为了一个矩阵C。C中的每一列都是一个文本，每一行即使一个term。

Step 2：我们对矩阵C做SVD分解，如下：

![](https://img-blog.csdn.net/20141203141948603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


学过矩阵论就会直到，中间的sigma就是有C的特征值组成的一个对角矩阵。假设C矩阵有r个特征值，那么我们把r个特征值从大到小排列，前k个保留，后面r-k个置零，得到了sigman_k。

Step 3：我们计算一个近似的分解矩阵，如下：

![](https://img-blog.csdn.net/20141203142522100?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


由于sigma_k中只有k个非零值，所以C_k的rank（秩）不会超过k。




    新得到的C_k就是我们经过LSA模型提取到的新矩阵，而且C_k就是新的低维隐含语义空间。该空间中，每个奇异值对应的是每个“语义”维度的权重，我们刚才将不太重要的权重置为零，只保留最重要的维度信息，因而可以得到文档的一种更优表达形式。




# **3. LSA模型的优缺点**




**优点：**可以把原文本特征空间降维到一个低维语义空间；减轻一词多义和一义多词问题。




** 缺点：**在SVD分解的时候，特别耗时，而且一般而言一个文本特征矩阵维数都会特别庞大，SVD此时就更加耗时；

                而且，LSA缺乏严谨的数理统计基础。















