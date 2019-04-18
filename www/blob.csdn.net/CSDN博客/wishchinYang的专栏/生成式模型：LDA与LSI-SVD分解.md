# 生成式模型：LDA与LSI-SVD分解 - wishchinYang的专栏 - CSDN博客
2014年08月10日 14:22:38[wishchin](https://me.csdn.net/wishchin)阅读数：835
**        原文链接：[http://blog.sina.com.cn/s/blog_5033f3b40101flbj.html](http://blog.sina.com.cn/s/blog_5033f3b40101flbj.html)**
**        文章图文并茂，我就不转载了！！！**
        科普文：**[判别式模型与生成式模型区别](https://blog.csdn.net/huangfei711/article/details/79834780)**；
        分类器的数学表达式为：给定输入变量 X以及分类变量 Y，求 P(Y|X)。**判别式模型**通过直接数据回归分析，直接估算 P(Y|X)。生成式模型的思想是先估计联合概率密度 P(X,Y)，再通过贝叶斯公式计算边缘分布，求出 P(Y|X)。比如最大熵模型为判别式模型，而HMM、LDA、贝叶斯网络等为生成式模型。
**LSI-Latent Semantic Indexing.浅语义分析**
针对缺点1，LSI（1990）将矩阵X进行奇异值分解，然后只取一部分作为其特征，此过程其实就相当于对X进行pca降维。将原始的向量转化到一个低维的隐含语义空间中，而保留下来的维度（根据奇异值大小决定）所对应的奇异值就对应了每个‘隐含语义’的权重，去掉的那些维度就相当于把那些不重要的‘隐含语义’的权重赋值为0.
LSI的作者Deerwester称由LSI得到的特征能够捕获一些基本的语义概念，例如同义词等。个人理解，这是由pca的性质决定的，。
LSI如其名字Latent Semantic Indexing, 旨在在词频矩阵X的基础上找出latent semantic,潜藏的语义信息。
其缺点是：不能解决多义词问题；
个人理解：这种方法就像词包模型一样，有一定的道理，但没有明确化，不像概率模型一样具体化。原文中说‘Given a generative model of text, however, it isnot clear why one should adopt the LSImethodology’，个人觉得就是说他的理论基础不够明白，所以后续推出PLSI，就是能够从数学上，从理论上具有严格意义的说明是怎么回事，到底是为什么有效，又怎么得出理论解。
**模型的扩展性：如何解决长尾数据问题？**
       相关文章：[搜索引擎算法——浅谈语义主题计算](http://www.semgle.com/search-engine-algorithms-mystery-behind-search-on-the-calculation-of-semantic-topic)
