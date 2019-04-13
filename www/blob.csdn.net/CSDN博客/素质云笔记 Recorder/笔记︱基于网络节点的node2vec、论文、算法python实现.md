
# 笔记︱基于网络节点的node2vec、论文、算法python实现 - 素质云笔记-Recorder... - CSDN博客

2017年01月13日 10:34:45[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：21935标签：[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[node2vec																](https://so.csdn.net/so/search/s.do?q=node2vec&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=python&t=blog)个人分类：[知识图谱（KG/Neo4j）																](https://blog.csdn.net/sinat_26917383/article/category/6233060)



> 看到一个很有意思的算法，而且腾讯朋友圈lookalike一文中也有提及到，于是蹭一波热点，学习一下。论文是也发KDD2016

> .

> .

> 一、主要论文：node2vec: Scalable Feature Learning for Networks

> 本节引用自

> a、微博洪亮劼  ：
> [【论文每日读】node2vec: Scalable Feature Learning for Networks](http://weibo.com/ttarticle/p/show?id=2309404006947190904515&sudaref=blog.csdn.net&retcode=6102)

> b、简书：
> [node2vec: Scalable Feature Learning for Networks](http://www.jianshu.com/p/a9a2ed8b98be)

> 本文的特征抽取方式类似于聚类分析的非监督方法，本质上都是利用相邻节点之间的联系。文中提到了网络中的节点一般有两种相似度量：1.内容相似性，2.结构相似性。其中内容相似性主要是相邻节点之间的相似性，而结构上相似的的点并不一定是相邻的，可能隔得很远，这也是文中为何要把BFS和DFS相结合来选择邻居节点的原因。

> 文章的主要想法就是，利用SkipGram的方法，来为Networks抽取Representation。那么，自然，根据SkipGram的思路，最重要的就是定义这个Context，或者说是Neighborhood。​从文本的角度来说，这个Neighborhood当然就是当前Word周围的字，这个定义非常自然。但是对于Graph或者Network来说就来得没那么容易了。

> 文章阐述了一般所采用Depth-First Search或者是Breadth-First Search来Sample一个Node的周边Node的问题。简单来说，BFS比较容易有一个Microscopic的View而DFS容易有一个Macro-view，两者都有Representative的问题。

> 文章的核心思想是采用Random Walk来代替DFS或者BFS。文章定义了一种二阶的Random Walk，拥有两个参数，来控制多大的概率反复经过一些Node和控制所谓的Inward和Outward。总之，整个Random Walk的目的就是在DFS和BFS之间采取某种平衡。

> 文章虽然提出的是关于Node Feature提取的算法，但是Edge Feature也可以很容易从Node Feature导出。

> 总体感觉是，硬要用SkipGram或者WordVec的想法在Networks上做，还显得比较牵强。因为有这个Neighborhood的概念，在Graph上，反而不是那么直观得定义，因此所有类似的工作都显得比较别扭。当然，这篇文章也不失为一种不错的Heuristic。​
![这里写图片描述](https://img-blog.csdn.net/20170113100330518?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170113100341956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> .

> .

> 二、python实现

> [github网址](https://github.com/aditya-grover/node2vec)

> 案例：

> To run node2vec on Zachary’s karate club network, execute the following command from the project home directory:

> python src/main
> .py
> --input graph/karate
> .edgelist
> --output emb/karate
> .emd
> Options

> You can check out the other options available to use with node2vec using:

> python
> src/main.py --
> help
> Input

> The supported input format is an edgelist:

> node1_id_int node2_id_int
> <
> weight_float,
> optional
> >
> The graph is assumed to be undirected and unweighted by default. These options can be changed by setting the appropriate flags.

> Output

> The output file has n+1 lines for a graph with n vertices. The first line has the following format:

> num_of_nodes dim_of_representation
> The next n lines are as follows:

> node_id dim1 dim2
> ...
> dimd
> where dim1, … , dimd is the d-dimensional representation learned by node2vec.

> .

> .

> .

> 三、腾讯对node2vec的应用

> 微信公众号infoQ:
> [当机器学习遇上复杂网络：解析微信朋友圈 Lookalike 算法](http://mp.weixin.qq.com/s?__biz=MjM5MDE0Mjc4MA==&mid=2650995211&idx=1&sn=8e32b5590b8e8bff8a5bd8bfb2ceaa7a&chksm=bdbf02588ac88b4e32ea5320e10c7a2e5ac762ea580e7fce8320b6d5c74a273c13410f5475cf&mpshare=1&scene=1&srcid=0113PKe7MsUK1uHM3FkOpV46#rd)

> 这个横轴是与广告进行互动的好友个数，纵轴是用户对广告的关注率（包括查看，点赞或者评论），我们发现这个关注率会随着好友数的增加而上升。这个数据拐点差不多是3到5个好友。
![这里写图片描述](https://img-blog.csdn.net/20170113100854130?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 重点会挖掘这两个价值，就是社交同质性和社交影响力。

> 实际上在一个社交网络的节点也是这样的，我们经常会存在一些大的节点，他会有非常多的好友，有的人好友就达不到那么多。所以说其实在社交网络里面的一个节点的分布也是幂律分布。如何把Wodrd2Vec迁移到node2vec，这个时候就要产生一个节点的序列，它对应到了自然语言处理的一条句子，图结构里面的节点相当于NLP的一个单词。

> 所以在图网络上按照一个搜索的方法生成节点序列，这个节点的序列可以对应到自然语言的一个句子，后面我们通过Wodrd2Vec的框架，将节点embedding为一个向量。所以对于做network embedding的时候，这个生成节点序列的搜索策略非常重要。最简单的一个方法，就是随机游走，随机游走一方面生成节点序列，另一方面也是对图的一种采样，降低了计算量。
![这里写图片描述](https://img-blog.csdn.net/20170113101013736?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 比如说我们的一个社交网络，我的同学会形成一个社团，设计这个P往回走，就更容易走到我这个群体。当P越大，它会越能体现同质性。Q越大的时候，它其实能够体现这种结构的相似性，不同的节点有不同的作用。比如说F节点和E节点它是连接这两个社团的桥接点。当Q大的时候，它体现的是网络结构的相似性。这时候我们怎么选P和Q？这个可以根据实际任务进行半监督的学习。
![这里写图片描述](https://img-blog.csdn.net/20170113101049662?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 给大家看一下node2vec的结果，先给大家看这个算法的输出。这里有一个简单的图，做embedding之后的结果，1和2的节点向量是一样的，它会是重叠的一个向量，3、4、5、6也是一个重合的节点，它表达的是什么呢？为什么1和2完全重叠？其实1和2的网络环境是一模一样的，这个embedding的结果表达是是节点的社交网络环境，也就是我们说的拓扑特征。

> 对社交相似性的学习框架，大家可以看下面的图。 我们建立一个回归的model。现在做的是SVR模型。输入好友网络，沟通网络、文章的转发阅读网络等等，进行embedding得到特征向量表达，通过SVR模型，学习到这些特征和广告相似度的函数关系。这个函数关系计算出好友相似度，可以对好友进行排序。
![这里写图片描述](https://img-blog.csdn.net/20170113101408788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 我们看一下算法的效果。我们评估算法的效果，最直接的就是说我有多个算法，广告主需要100万的用户，我这几个算法都给出100万用户，然后看一下这100万的用户点击量是怎么样的，我们叫Lift值。其他的算法跟它进行对比，看一下它的效果有没有提升。那我们的算法相比直接的二分类模型有2倍-3倍的lift。
![这里写图片描述](https://img-blog.csdn.net/20170113101436835?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> .

> 延伸一：网络与词向量

> 来源于：
> [网络与几何的纠缠 | 张江](https://mp.weixin.qq.com/s?__biz=MzIzMjQyNzQ5MA==&mid=2247485530&idx=1&sn=8b06e3cc598693ca67ff8ba696184958&chksm=e89452d7dfe3dbc1074348d8a1a47f863cf21a37d25aef3663c967557b8b6fa2c34297c0cc7d&mpshare=1&scene=1&srcid=0429itBo7u37rry4Aj0ZVF4V#rd)

> 最近，深度学习成为了科学界的新宠。人们将大量的数据扔进了神经网络中，以期待它能够自己学习到数据中蕴藏的模式。然而，目前的深度学习处理的数据大多仅仅局限在图像和文本，但却不包括网络。这是因为，网络的本质在于节点之间的连接信息，而这种信息很难被结构化为标准的数据。怎么办呢？

> 答案就在于空间。只要我们将网络嵌入到了一个几何空间中，我们就可以将每个节点的坐标视作该节点的特征，从而放到神经网络中进行学习和训练。那么，针对一个一般性的网络，我们又如何计算每一个节点的空间坐标呢？

> 答案就在于DeepWalk算法。它首先在网络上释放大量的随机游走粒子，这些粒子在给定的时间内就会走出一个节点构成的序列。我们不妨将节点视作单词，于是它们生成的序列就构成了句子，我们便可以得到一种节点由序列构成的“语言”。接下来，我们就可以应用强大的Word2Vec算法，计算出每个节点“单词”的向量表示，也就是空间坐标了。
![这里写图片描述](https://img-blog.csdn.net/20170430105040602?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170430105040602?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 这种节点嵌入的算法可以很好地反映节点之间的连接信息，或者我们可以将DeepWalk算法得到的节点坐标视作对每个节点连接信息的编码。那些连接结构上相似的节点会在空间中彼此靠近。
![这里写图片描述](https://img-blog.csdn.net/20170430105051149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170430105051149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 有了从结构到空间的这种转换，我们便可以利用普通的聚类和分类算法来对复杂网络进行处理。

> .

> 延伸二：NE(Network Embedding)论文小览

> 来源：
> [http://blog.csdn.net/dark_scope/article/details/74279582](http://blog.csdn.net/dark_scope/article/details/74279582)

> 自从word2vec横空出世，似乎一切东西都在被embedding，今天我们要关注的这个领域是Network Embedding，也就是基于一个Graph，将节点或者边投影到低维向量空间中，再用于后续的机器学习或者数据挖掘任务，对于复杂网络来说这是比较新的尝试，而且取得了一些效果。

> 本文大概梳理了最近几年流行的一些方法和论文，paper主要是来自thunlp/NRLPapers 这个List，并掺杂了一些其他论文。大概看了一遍，简单总结一下，希望对大家有所帮助，如有不严谨的地方，还望指正。

> 抛开一些传统的流形学习方法不谈，下面大概以这个outline组织（区分并不严格）：

> 此处输入图片的描述
![这里写图片描述](http://ommwjlura.bkt.clouddn.com/QQ20170703-3@2x.jpg)
> DeepWalk(Online Learning of Social Representations.)

> DeepWalk是KDD 2014的一篇文章，彼时word2vec在文本上的成功应用掀起来一波向量化的浪潮，word2vec是根据词的共现关系，将词映射到低维向量，并保留了语料中丰富的信息。DeepWalk算法思路其实很简单，对图从一个节点开始使用random walk来生成类似文本的序列数据，然后将节点id作为一个个「词」使用skip gram训练得到「词向量」。

> 思路虽然简单，背后是有一定道理的，后面一些工作有证明这样做其实等价于特殊矩阵分解(Matrix Factorization)。而DeepWalk本身也启发了后续的一系列工作。

> node2vec(Scalable Feature Learning for Networks)

> node2vec在DW的基础上，定义了一个bias random walk的策略生成序列，仍然用skip gram去训练。

> 论文分析了BFS和DFS两种游走方式，保留的网络结构信息是不一样的。

> DeepWalk中根据边的权重进行随机游走，而node2vec加了一个权重调整参数α：t是上一个节点，v是最新节点，x是候选下一个节点。d(t,x)是t到候选节点的最小跳数。

> 通过不同的p和q参数设置，来达到保留不同信息的目的。当p和q都是1.0的时候，它等价于DeepWalk。
![这里写图片描述](http://ommwjlura.bkt.clouddn.com/QQ20170701-2@2x.jpg)
> MMDW(Max-Margin DeepWalk Discriminative Learning of Network Representation)

> DW本身是无监督的，如果能够引入label数据，生成的向量对于分类任务会有更好的作用。

> 之前提到过有证明DW实际上是对于一个特殊矩阵M的分解，

> 这篇文章将DeepWalk和Max-Margin（SVM）结合起来，从损失函数看是这两部分组成：

> 1.训练的时候是分开优化，固定X,Y优化W和ξ，其实就是multi class 的 SVM。

> 2.固定W和ξ优化X,Y的时候稍微特殊一点，算了一个biased Gradient，因为损失函数里有x和w的组合。

> 这样在训练中同时优化discrimination和representation两部分,达到一个好的效果。
![这里写图片描述](http://ommwjlura.bkt.clouddn.com/QQ20170701-6@2x.jpg)
> TADW(Network Representation Learning with Rich Text Information.)

> 文章里有DeepWark等同于M的矩阵分解的简单证明，而在实际中，一些节点上旺旺会有文本信息，所以在矩阵分解这个框架中，将文本直接以一个子矩阵的方式加入，会使学到的向量包含更丰富的信息。

> 文本矩阵是对TFIDF矩阵的SVD降维结果。
![此处输入图片的描述](http://ommwjlura.bkt.clouddn.com/QQ20170701-7@2x.jpg)
> [ ](http://ommwjlura.bkt.clouddn.com/QQ20170701-7@2x.jpg)

> .

> [
](http://ommwjlura.bkt.clouddn.com/QQ20170701-7@2x.jpg)
> 延伸三：基于word2vec和doc2vec用户表示方法

> （1）用户表示方法

> 本文采用了五种用户表示方法，分别是One-Hot表示、基于用户文本的分布式表示、基于用户关系网络的分布式表示、半监督的网络分布式表示和联合表示。

> 本节使用word2vec和doc2vec两个工具通过用户的文本数据分别学习用户的分布式表示，并采用逻辑回归分类器对用户的不同属性进行分类。

> 利用基于word2vec（生成的向量长度为100，窗口大小为5，模型为CBOW模型，算法为Hierarchical Softmax模型）生成的用户分布式表示实验结果见表 2。
![这里写图片描述](https://img-blog.csdn.net/20170909214050399?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170909214050399?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 利用基于doc2vec（生成的向量长度为100，窗口大小为5，模型为CBOW模型，算法为Hierarchical Softmax模型错误。）生成的用户分布式表示实验结果见表3。
![这里写图片描述](https://img-blog.csdn.net/20170909214102378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170909214102378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 从实验结果的对比可以看出，单纯词向量累加的形式所获取到的用户分布式表示并不能有效地提高实验的效果，相反，各个参数都有所下降。与之相比，采用doc2vec工具直接得到的用户分布式的表现表示虽然较之词袋模型仍然有所下降，但是却要高于word2vec累加的表现。

> [

](https://img-blog.csdn.net/20170909214102378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> （2）用户的关系网络

> [
](https://img-blog.csdn.net/20170909214102378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 我们使用Deepwalk和LINE两个工具通过用户的关系网络数据分别学习用户的分布式表示，并采用逻辑回归分类器对用户的不同属性进行分类。

> [
](https://img-blog.csdn.net/20170909214102378?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 来源：
> [赛尔原创 | 用户表示方法对新浪微博中用户属性分类性能影响的研究](https://mp.weixin.qq.com/s?__biz=MzIxMjAzNDY5Mg==&mid=2650791714&idx=1&sn=fc164c5abb440cd47dc3e43d67de6422&chksm=8f474ac9b830c3df8cffbb9b12976272c90f75ed3ce1c3a84eb8a30c98bd57ea71035890b389&mpshare=1&scene=1&srcid=0819zl8UsnCH01ZqHKUFIbBi#rd)

> 作者： 哈工大SCIR 孙晓飞，丁效，刘挺

> .

> 延伸四：Flownetwork：流网络的开源Python包

> 该包的主要功能如下：
> [https://github.com/chengjun/flownetwork](https://github.com/chengjun/flownetwork)

> 来源： 集智小仙女，
> [《Flownetwork：流网络的开源Python包》](https://mp.weixin.qq.com/s?__biz=MzIzMjQyNzQ5MA==&mid=2247486108&idx=2&sn=02e75be77ddcdc731f7d0e334d8437ab&chksm=e8945011dfe3d90775ad848061cfc3069f922f30c2bef7253c880f6b3cb4516ac8ce8afc86cb&mpshare=1&scene=1&srcid=0819fxLj6wzWcJWanzbfmnf7#rd)

> 南京大学的王成军老师和芝加哥大学的吴令飞博士开发了一个开源工具包Flownetwork，将常用的计算都集成到了一起。初学者可以直接调用该包完成各种有关开放流网络的计算。
![这里写图片描述](https://img-blog.csdn.net/20170909222602006?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 然后进行注意力网络的分析：

> 首先我们可以用help语句来查看一下这个流网络的结构：

> help(fn.constructFlowNetwork)
Help on
> function
> constructFlowNetwork
> in
> module
> flownetwork.flownetwork
> :
> constructFlowNetwork
> (C)
> C is an array of two dimentions, e.g.,
   C = np.array(
> [[user1, item1],
      [user1, item2],
      [user2, item1],
      [user2, item3]]
> )
   Return a balanced flow networ
> 在了解了这个网络的架构之后，我们就可以自己创建一个流网络：

> demo =
> fn
> .
> attention_data
> gd
> =
> fn
> .
> constructFlowNetwork
> (
> demo)
> 为了更好的了解这个流网络的结构，我们可以利用matplotlib画出这个demo的流网络：

> \# drawing a demo network
> fig = plt.figure(figsize=(
> 12
> ,
> 8
> ),facecolor=
> 'white'
> )
pos={
> 0
> : np.
> array
> ([
> 0.2
> ,
> 0.8
> ]),
> 2
> : np.
> array
> ([
> 0.2
> ,
> 0.2
> ]),
> 1
> : np.
> array
> ([
> 0.4
> ,
> 0.6
> ]),
> 6
> : np.
> array
> ([
> 0.4
> ,
> 0.4
> ]),
> 4
> : np.
> array
> ([
> 0.7
> ,
> 0.8
> ]),
> 5
> : np.
> array
> ([
> 0.7
> ,
> 0.5
> ]),
> 3
> : np.
> array
> ([
> 0.7
> ,
> 0.2
> ]),
> 'sink'
> : np.
> array
> ([
> 1
> ,
> 0.5
> ]),
> 'source'
> : np.
> array
> ([
> 0
> ,
> 0.5
> ])}
width=[float(d[
> 'weight'
> ]*
> 1.2
> )
> for
> (u,v,d) in gd.edges(data=
> True
> )]
edge_labels=dict([((u,v,),d[
> 'weight'
> ])
> for
> u,v,d in gd.edges(data=
> True
> )])nx.draw_networkx_edge_labels(gd,pos,edge_labels=edge_labels,
font_size =
> 15
> , alpha =
> .5
> )
nx.draw(gd, pos, node_size =
> 3000
> , node_color =
> 'orange'
> ,
 alpha =
> 0.2
> , width = width, edge_color=
> 'orange'
> ,style=
> 'solid'
> )
nx.draw_networkx_labels(gd,pos,font_size=
> 18
> )
plt.show()
> 然后我们就画出了这个demo的流网络图：
![这里写图片描述](https://img-blog.csdn.net/20170909222718393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

