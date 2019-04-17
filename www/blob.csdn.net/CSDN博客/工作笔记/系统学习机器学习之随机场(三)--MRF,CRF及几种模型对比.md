# 系统学习机器学习之随机场(三)--MRF,CRF及几种模型对比 - 工作笔记 - CSDN博客





2016年12月15日 09:26:17[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：15356








参考：[http://blog.csdn.net/discxuwei/article/details/6331617](http://blog.csdn.net/discxuwei/article/details/6331617)

条件随机场(Conditional random fields)，是一种判别式图模型，因为其强大的表达能力和出色的性能，得到了广泛的应用。从最通用角度来看，CRF本质上是给定了观察值集合 (observations)的马尔可夫随机场（MRF）。在这里，我们直接从最通用的角度来认识和理解CRF，最后可以看到，线性CRF和所谓的高阶CRF，都是某种特定结构的CRF。
**1. 随机场**

简单地讲，随机场可以看成是一组随机变量的集合（这组随机变量对应同一个样本空间）。当然，这些随机变量之间可能有依赖关系，一般来说，也只有当这些变量之间有依赖关系的时候，我们将其单独拿出来看成一个随机场才有实际意义。
*2. Markov随机场(MRF)*

这是加了Markov性质限制的随机场。首先，一个Markov随机场对应一个无向图。这个无向图上的每一个节点对应一个随机变量，节点之间的边表示节点对应的随机变量之间有概率依赖关系。因此，Markov随机场的结构本质上反应了我们的先验知识——哪些变量之间有依赖关系需要考虑，而哪些可以忽略。Markov性质是指，对Markov随机场中的任何一个随机变量，给定场中其他所有变量下该变量的分布，等同于给定场中该变量的邻居节点下该变量的分布。这让人立刻联想到马式链的定义：它们都体现了一个思想：离当前因素比较遥远(这个遥远要根据具体情况自己定义）的因素对当前因素的性质影响不大。

Markov性质可以看作是Markov随机场的微观属性，那么其宏观属性就是其联合概率的形式。

假设MRF的变量集合为

S=｛y1,…yn},

P(y1,…yn)= 1/Z * exp{-1/T * U(y1,..yn)},

其中Z是归一化因子，即对分子的所有y1,..yn求和得到。U(y1,..yn)一般称为能量函数（energy function）, 定义为在MRF上所有团的势函数（clique-potential）之和。T称为温度，一般取1。什么是团势函数clique-potential呢? 就是在MRF对应的图中，每一个团（clique）对应一个函数，称为团势函数（clique-potential）。（补充：团是马尔科夫无向图中，节点的集合使得该集合中任意两个节点之间都存在一条边，不采用条件概率，在无向图中用势函数来体现局部约束，不必有概率解释，且有更大的自由度，偏爱某种局部配置，而不是其他局部配置）。这个联合概率形式又叫做吉布斯分布（Gibbs distribution）。Hammersley and Clifford定理表达了这两种属性的等价性。

如果团势函数的定义和团在图中所处的位置无关，则称该MRF是齐次的；如果团势函数的定义和 团在图中的朝向(orientation)无关，则称该MRF是各向的。一般来说，为了简化计算，都是假定MRF即是 齐次的，也是各向的。

补充：

尽管在给定每个节点的条件下，分配给该节点一个条件概率是可能的，但条件随机场的无向性很难保证每个节点在给定它的邻接点条件下得到的条件概率和以图中其它节点为条件得到的条件概率一致。因此导致我们不能用条件概率参数化表示联合概率，而要从一组条件独立的原则中找出一系列局部函数的乘积来表示联合概率。选择局部函数时，必须保证能够通过分解联合概率使没有边的两个节点不出现在同一局部函数中。简单的局部函数是定义在图结构中的大团(clique)上的势函数(Potential function)，并且是严格正实值的函数形式。但是一组正实数函数的乘积并不能满足概率公理，则必须引入一个归一化因子Z ，这样可以确保势函数的乘积满足概率公理，且是G 中节点所表示的随机变量的联合概率分布。

![](https://img-blog.csdn.net/20161225135829026?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其中C 为大团集合，利用 Hammersley-Clifford 定理，可以得到联合概率公式如下： 

![](https://img-blog.csdn.net/20161225135853058?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

基于条件独立的概念，条件随机场的无向图结构可以用来把关于 Υv ∈Υ 的联合分布因式化正的和实值的势函数的乘积，每个势函数操作在一个由G中顶点组成的随机变量子集上。根据无向图模型条件独立的定义，如果两个顶点间没有边，则意味着这些顶点对应的随机变量在给定图中其它顶点条件下是条件独立的。所以在因式化条件独立的随机变量联合概率时，必须确保这些随机变量不在同一个势函数中。满足这个要求的容易的方法是 要求每个势函数操作在一个图G 的大团上，这些大团由随机变量相应顶点组成。这确保了没有边的顶点在不同的势函数中，在同一个大团中的顶点都是有边相连的。在无向图 中，任何一个全连通（任意两个顶点间都有边相连）的子图称为一个团(clique)，而称不能被其它团所包含的才为大团(maximal clique)。

3.从Markov随机场到CRF

现在，如果给定的MRF中每个随机变量下面还有观察值，我们要确定的是给定观察集合下，这个MRF的分布，也就是条件分布，那么这个MRF就称为 CRF(Conditional Random Field)。它的条件分布形式完全类似于MRF的分布形式,只不过多了一个观察集合x，即P(y1,..yn|x) = 1/Z(x) * exp{ -1/T * U(y1,…yn,x)。U(y1,..yn,X)仍旧是click-potential之和。

来点正式的：

关联数据有两个特点：第一，我们准备建立模型的实体之间存在统计依赖性，第二，每个实体自身具有丰富的有利于分类的特征例如，当Web文档进行分类时候，网页上的文本类标签提供了很多信息，但超链接定义的页面之间的关系，可以进一步提高分类的准确度，图模型很自然的建立了实体之间的结构化关系。通常来讲，图模型已被用于表示联合概率分布P(Y,X)，其中的变量y代表我们希望预测的属性，输入变量X代表获得的实体信息。利用关系数据中的局部特征对联合分布建模非常困难，因为这需要对包含复杂依赖关系的P(X)建模，引入数据之间的关系是把双刃剑，如果模型中引入它，导致模型非常复杂，如果忽略它，那么模型的准确性就会受到影响.对于分类问题来说，直接模拟的条件分布P(Y|X)条件随机场就是这么做的。条件随机场是关联这一个图结构的条件分布P(Y| X)，因为直接对条件概率建模，输入变量X之间的关联不需要显示的表达出来

通常我看到一个模型，我总是会试着和我所知道的一些模型去进行比较。CRF算是机器学习模型中比较复杂的一个模型。同输入输出角度， CRF从数据空间来看是一个序列化的输入和序列化的输出， 从函数空间角度，就是一个指数函数，和逻辑回归函数形式一致，只是他的输出是一个序列（一条句子）或者一个矩阵（一副图像），损失函数是cross entropy，优化算法一般采用梯度下降类算法（LBFGS)，但是在优化的过程中伴随着复杂的inference过程。

**判别式模型和产生式模型**

朴素贝叶斯和逻辑回归模型之间的一个重要区别是，朴素贝叶斯是产生式模型，它基于联合分布P(X,Y)建模，而逻辑回归是判别式模型,它直接对条件分布P(Y|X)建模，这一段中我们讨论产生式模型和判别式模型的区别和判别式模型在很多应用中的优点，具体来说，我们会集中以朴素贝叶斯和逻辑回归为例，但是这些区别可以推广到一般的产生式模型和判别式模型。

对条件分布p（Y| X）建模，不包括对P(X) 建模（P(X)对分类来说无关紧要），对P(X)建模非常困难，因为P(X)包含很多相互依赖的特征,比如在 NER应用中，HMM只依赖一个特征，这个词本身，但是很多词，特别是一些特定的名字可能没有出现在训练集合中，因此词本身这个特征是未知的，为了标注未登陆词，我们需要利用词的其他的特征，如词性，相邻词，前缀和后缀等。

在产生式模型中包含相互依赖的特征有两种方式，加强模型来表达输入数据之前的关联，或者简单的做独立性假设，比如朴素贝叶斯假设，第一种方法通常很难在提高模型复杂度的同时仍然保持模型优化的简便，比如我们很难建立词性和词的后缀之间的关系，而实际上我们也不是特别愿意去这么做，第二种方法，对输入数据做独立性假设会使得准确性降低，尽管朴素贝叶斯在文本分类应用中表现出其的好，但是在很多其他的应用中都比逻辑回归要逊色虽然朴素贝叶斯的分类准确率较高，但是在概率估计应用方便表现较差。

对于CRF，我们给出准确的数学语言描述：

　　设X与Y是随机变量，P(Y|X)是给定X时Y的条件概率分布，若随机变量Y构成的是一个马尔科夫随机场，则称条件概率分布P(Y|X)是条件随机场。

注意在CRF的定义中，我们并没有要求X和Y有相同的结构。而实现中，我们一般都假设X和Y有相同的结构，即:

![](https://img-blog.csdn.net/20161215103616662?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQXBwXzEyMDYyMDEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20181012162457817?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们一般考虑如下图所示的结构：X和Y有相同的结构的CRF就构成了线性链条件随机场(Linear chain Conditional Random Fields,以下简称 linear-CRF)。

![](https://img-blog.csdn.net/20181012162527419?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们再来看看 linear-CRF的数学定义：

设 X=(X1,X2,...Xn),Y=(Y1,Y2,...Yn)均为线性链表示的随机变量序列，在给定随机变量序列X的情况下，随机变量Y的条件概率分布P(Y|X)构成条件随机场，即满足马尔科夫性：

![](https://img-blog.csdn.net/20181012162705932?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 则称P(Y|X)为线性链条件随机场。　

我们如何将其转化为可以学习的机器学习模型呢？这是通过特征函数和其权重系数来定义的。什么是特征函数呢？

在linear-CRF中，特征函数分为两类，第一类是定义在Y节点上的节点特征函数，这类特征函数只和当前节点有关，记为：

![](https://img-blog.csdn.net/20181012162838798?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中L是定义在该节点的节点特征函数的总个数，i是当前节点在序列的位置。

第二类是定义在Y上下文的局部特征函数，这类特征函数只和当前节点和上一个节点有关，记为：

![](https://img-blog.csdn.net/20181012162947604?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中K是定义在该节点的局部特征函数的总个数，i是当前节点在序列的位置。之所以只有上下文相关的局部特征函数，没有不相邻节点之间的特征函数，是因为我们的linear-CRF满足马尔科夫性。

　　无论是节点特征函数还是局部特征函数，它们的取值只能是0或者1。即满足特征条件或者不满足特征条件。同时，我们可以为每个特征函数赋予一个权值，用以表达我们对这个特征函数的信任度。假设tk的权重系数是λk,sl的权重系数是μl,则linear-CRF由我们所有的tk,λk,sl,μl共同决定。

　此时我们得到了linear-CRF的参数化形式如下(其实这里看出，还是类似MaxEnt,SoftMax，多元逻辑回归模型)：

![](https://img-blog.csdn.net/2018101216313529?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　其中，Z(x)为规范化因子：

![](https://img-blog.csdn.net/2018101216320518?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

回到特征函数本身，每个特征函数定义了一个linear-CRF的规则，则其系数定义了这个规则的可信度。所有的规则和其可信度一起构成了我们的linear-CRF的最终的条件概率分布。

再次整理特征函数，统一表示特征。

假设我们在某一节点我们有K1个局部特征函数和K2个节点特征函数，总共有K=K1+K2个特征函数。我们用一个特征函数fk(yi−1,yi,x,i)来统一表示如下:

![](https://img-blog.csdn.net/20181012163445609?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

对fk(yi−1,yi,x,i)在各个序列位置求和得到：

![](https://img-blog.csdn.net/20181012163524629?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

同时我们也统一fk(yi−1,yi,x,i)对应的权重系数wk如下：

![](https://img-blog.csdn.net/20181012163618128?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这样，我们的linear-CRF的参数化形式简化为：

![](https://img-blog.csdn.net/20181012163653249?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中，Z(x)为规范化因子：

![](https://img-blog.csdn.net/20181012163827686?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果将上两式中的wk与fk的用向量表示，即:

![](https://img-blog.csdn.net/20181012163853772?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

则linear-CRF的参数化形式简化为内积形式如下：

![](https://img-blog.csdn.net/20181012163914581?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们还可以将linear-CRF的参数化形式写成矩阵形式。为此我们定义一个m×m的矩阵M ，m为y所有可能的状态的取值个数。M定义如下：

![](https://img-blog.csdn.net/20181012164015397?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们引入起点和终点标记y0=start,yn+1=stop, 这样，标记序列y的非规范化概率可以通过n+1个矩阵元素的乘积得到，即： 

![](https://img-blog.csdn.net/20181012164053852?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中Zw(x)为规范化因子。

linear-CRF的三个基本问题

　　　　在HMM中，我们讲到了HMM的三个基本问题，而linear-CRF也有三个类似的的基本问题。不过和HMM不同，在linear-CRF中，我们对于给出的观测序列x是一直作为一个整体看待的，也就是不会拆开看(x1,x2,...)，因此linear-CRF的问题模型要比HMM简单一些，如果你很熟悉HMM，那么CRF的这三个问题的求解就不难了。

 linear-CRF第一个问题是评估，即给定 linear-CRF的条件概率分布P(y|x), 在给定输入序列x和输出序列y时，计算条件概率P(yi|x)和P(yi−1，yi|x)以及对应的期望. 本文接下来会详细讨论问题一。

 linear-CRF第二个问题是学习，即给定训练数据集X和Y，学习linear-CRF的模型参数wk和条件概率Pw(y|x)，这个问题的求解比HMM的学习算法简单的多，普通的梯度下降法，拟牛顿法都可以解决。

 linear-CRF第三个问题是解码，即给定 linear-CRF的条件概率分布P(y|x),和输入序列x, 计算使条件概率最大的输出序列y。类似于HMM，使用维特比算法可以很方便的解决这个问题。

**评估：**

要计算条件概率P(yi|x)和P(yi−1，yi|x)，我们也可以使用和HMM类似的方法，使用前向后向算法来完成。首先我们来看前向概率的计算。

　　　　我们定义αi(yi|x)表示序列位置i的标记是yi时，在位置i之前的部分标记序列的非规范化概率。之所以是非规范化概率是因为我们不想加入一个不影响结果计算的规范化因子Z(x)在分母里面。我们还是引用之前的矩阵形式：

![](https://img-blog.csdn.net/2018101310163053?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这个式子定义了在给定yi−1时，从yi−1转移到yi的非规范化概率。也就是转移概率（非归一化）。

　　　　这样，我们很容易得到序列位置i+1的标记是yi+1时，在位置i+1之前的部分标记序列的非规范化概率αi+1(yi+1|x)的递推公式：

![](https://img-blog.csdn.net/20181012165532214?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在起点处，我们定义：

![](https://img-blog.csdn.net/20181012165620675?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

假设我们可能的标记总数是m, 则yi的取值就有m个，我们用αi(x)表示这m个值组成的前向向量如下：

![](https://img-blog.csdn.net/20181012165642721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

同时用矩阵Mi(x)表示由Mi(yi−1,yi|x)形成的m×m阶矩阵：

![](https://img-blog.csdn.net/20181013101939982?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　这样递推公式可以用矩阵乘积表示：

![](https://img-blog.csdn.net/20181012165749810?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

同样的。我们定义βi(yi|x)表示序列位置i的标记是yi时，在位置i之后的从i+1到n的部分标记序列的非规范化概率。

　　　　这样，我们很容易得到序列位置i+1的标记是yi+1时，在位置i之后的部分标记序列的非规范化概率βi(yi|x)的递推公式：

![](https://img-blog.csdn.net/20181012165842152?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在终点处，我们定义：

![](https://img-blog.csdn.net/20181012165904818?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

　如果用向量表示，则有：

![](https://img-blog.csdn.net/20181012165920729?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

由于规范化因子Z(x)的表达式是：

![](https://img-blog.csdn.net/20181012165940586?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

也可以用向量来表示Z(x):

![](https://img-blog.csdn.net/20181012170000391?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

其中，1是m维全1向量。

　有了前向后向概率的定义和计算方法，我们就很容易计算序列位置i的标记是yi时的条件概率P(yi|x):

![](https://img-blog.csdn.net/20181012170050646?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

也容易计算序列位置i的标记是yi，位置i−1的标记是yi−1时的条件概率P(yi−1,yi|x):

![](https://img-blog.csdn.net/20181012170123587?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们也可以很方便的计算联合分布P(x,y)P(x,y)与条件分布P(y|x)P(y|x)的期望。

特征函数fk(x,y)关于条件分布P(y|x)的期望表达式是：

![](https://img-blog.csdn.net/20181012170200786?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

同样可以计算联合分布P(x,y)的期望：

![](https://img-blog.csdn.net/20181012170223234?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

假设一共有K个特征函数，则k=1,2,...K

**训练：**

在linear-CRF模型参数学习问题中，我们给定训练数据集X和对应的标记序列Y，K个特征函数fk(x,y)，需要学习linear-CRF的模型参数wk和条件概率Pw(y|x)，其中条件概率Pw(y|x)和模型参数wk满足一下关系：

![](https://img-blog.csdn.net/20181013102121276?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

所以我们的目标就是求出所有的模型参数wk，这样条件概率Pw(y|x)Pw(y|x)可以从上式计算出来。

　　　　求解这个问题有很多思路，比如梯度下降法，牛顿法，拟牛顿法。同时，这个模型中Pw(y|x)的表达式和[最大熵模型原理小结](http://www.cnblogs.com/pinard/p/6093948.html)中的模型一样，也可以使用最大熵模型中使用的改进的迭代尺度法(improved iterative scaling, IIS)来求解。

　　　　说白了，就是极大似然估计问题，不再赘述。

**解码：**

在这个问题中，给定条件随机场的条件概率P(y|x)和一个观测序列x,要求出满足P(y|x)最大的序列y。

　　　　这个解码算法最常用的还是和HMM解码类似的维特比算法。

　　　　维特比算法本身是一个动态规划算法，利用了两个局部状态和对应的递推公式，从局部递推到整体，进而得解。对于具体不同的问题，仅仅是这两个局部状态的定义和对应的递推公式不同而已。由于在之前已详述维特比算法，这里就是做一个简略的流程描述。

　　　　对于我们linear-CRF中的维特比算法，我们的第一个局部状态定义为δi(l),表示在位置i标记l各个可能取值(1,2...m)对应的非规范化概率的最大值。之所以用非规范化概率是，规范化因子Z(x)不影响最大值的比较。根据δi(l)的定义，我们递推在位置i+1标记l的表达式为：

![](https://img-blog.csdn.net/20181013102528565?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

和HMM的维特比算法类似，我们需要用另一个局部状态Ψi+1(l)来记录使δi+1(l)达到最大的位置i的标记取值,这个值用来最终回溯最优解，Ψi+1(l)的递推表达式为：

![](https://img-blog.csdn.net/20181013102613351?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

现在我们总结下 linear-CRF模型维特比算法流程：

　　　　输入：模型的K个特征函数，和对应的K个权重。观测序列x=(x1,x2,...xn)，可能的标记个数m

　　　　输出：最优标记序列y∗=(y∗1,y∗2,...y∗n)

　　　　1) 初始化：

![](https://img-blog.csdn.net/20181013102738642?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2) 对于i=1,2...n−1,进行递推：

![](https://img-blog.csdn.net/20181013102806954?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3) 终止：

![](https://img-blog.csdn.net/20181013102829545?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4)回溯：

![](https://img-blog.csdn.net/20181013102845326?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0FwcF8xMjA2MjAxMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最终得到最优标记序列y∗=(y∗1,y∗2,...y∗n)

linear-CRF模型和HMM模型有很多相似之处，尤其是其三个典型问题非常类似，除了模型参数学习的问题求解方法不同以外，概率估计问题和解码问题使用的算法思想基本也是相同的。同时，两者都可以用于序列模型，因此都广泛用于自然语言处理的各个方面。

　　　　现在来看看两者的不同点。最大的不同点是linear-CRF模型是判别模型，而HMM是生成模型，即linear-CRF模型要优化求解的是条件概率P(y|x),则HMM要求解的是联合分布P(x,y。第二，linear-CRF是利用最大熵模型的思路去建立条件概率模型，对于观测序列并没有做马尔科夫假设。而HMM是在对观测序列做了马尔科夫假设的前提下建立联合分布的模型。

最后想说的是，只有linear-CRF模型和HMM模型才是可以比较讨论的。但是linear-CRF是CRF的一个特例，CRF本身是一个可以适用于很复杂条件概率的模型，因此理论上CRF的使用范围要比HMM广泛的多。

**模型比较**

优点：

（1）和HMM相比，CRF直接对后验概率建模，因而可以容纳任意的上下文信息。特征设计灵活，

（2）和MEMM相比，由于CRF计算全局最优输出节点的条件概率，MEMM只是由前一个状态转移的状态进行的归一，而是对序列的所有可能组合进行归一，克服了最大熵马尔可夫模型标记偏置（Label-bias）的缺点

（3）和ME相比，CRF是在给定需要标记的观察序列的条件下，计算整个标记序列的联合概率分布，考虑标号序列之间的关系，而不是把假设标注之间独立，意思就是说，单独的ME(MaxEnt)不能用做序列分类，而HMM,MEMM,CRF可以看成是序列分类，不是单独词分类。

缺点：训练代价大、复杂度高

**4.训练补充**

通过一组样本，我们希望能够得到CRF对应的分布形式，并且用这种分布形式对[测试](http://lib.csdn.net/base/softwaretest)样本进行分类。也就是测试样本中每个随机变量的取值。

在实际应用中，clique-potential主要由用户自己定义的特征函数组成，即用户自己定义一组函数，这些函数被认为是可以用来帮助描述随机变量分布的。而这些特征函数的强弱以及正向、负向是通过训练得到的一组权重来表达的，这样，实际应用中我们需要给出特征函数以及权重的共享关系(不同的特征函数可能共享同一个权重),而clicque-potential本质上成了对应特征函数的线性组合。这些权重就成了CRF的参数。因此，本质上，图的结构是用户通过给出特征函数的定义确定的（例如，只有一维特征函数，对应的图上是没有边的)还有，CRF的分布成了对数线性形式。

看到这个分布形式，我们自然会想到用最大似然准则来进行训练。对其取log之后，会发现，表达式是convex的，也就是具有全局最优解——这是很让人振奋的事情。而且，其梯度具有解析解，这样可以用LBFGS来求解极值。
此外，也可以使用最大熵准则进行训练，这样可以用比较成熟的GIS和IIS[算法](http://lib.csdn.net/base/datastructure)进行训练。由于对数线性的分布形式下，最大熵准则和最大似然准则本质上是一样的，所以两者区别不是很大（好吧，三年后，我才真正理解这句话）。

此外，由于前面两种训练方法在每一轮迭代时，都需要inference,这样会极大地降低训练速度。因此普遍采用另一种近似的目标函数，称为伪似然。它用每个随机变量的条件分布(就是给定其他所有随件变量的分布)之积来替代原来的似然函数，根据markov性质，这个条件分布只和其邻居有关(Markov Blanket),这样在迭代过程中不需要进行全局的inference,速度会得到极大的提升。我自己的经验表明，当特征函数很多取实数值时，伪似然的效果跟最大似然的差不多，甚至略好于后者。但对于大量二元特征(binary-valued)，伪似然的效果就很差了。

5.推断

如前所述，训练的过程中我们需要概率推断，分类的时候我们需要找出概率最大的一组解，这都涉及到推断。这个问题本质上属于图模型上的概率推断问题。对于最简单的线性框架的结构，我们可以使用Viterbi算法。如果图结果是树形的，可以采用信念传播(belief propogation),用sum-product得到概率，用max-product得到最优的configuration.但是对于任意图，这些方法就无效了。一种近似的算法，称为loopy-belief propogation,就是在非树形结构上采用信念传播来进行推断，通过循环传播来得到近似解。这么做据说在某些场合下效果不错。但是，在训练时如果采用近似推断的话，可能会导致长时间无法收敛。

基于任意图上的概率推断算法称为 junction  tree。这个算法能够保证对任意图进行精确推理。它首先把原来的图进行三角化，在三角化的图上把clique按照某种方式枚举出来作为节点（实际上就是合并特征函数),clicque之间如果有交集，对应的节点之间就有边，这样就得到一个新的图，通过对这个图求最大生成树，就得到了Junction tree. 最后在junction tree上进行信念传播可以保证得到精确解。

本质上这3中算法都属于动态规划的思想。Viterbi的想法最直观，信念传播首先将特征函数都转换为factor，并将其与随机变量组合在一起形成 factor-graph, 这样在factor-graph上用动态规划的思想进行推断（即做了一些预处理)。junction tree的做法是通过合并原有的特征函数, 形成一种新的图，在这个图上可以保证动态规划的无后效性，于是可以进行精确推理。(做了更为复杂的预处理)

值得注意的是，junction tree虽然极大地避开了组合爆炸，但由于它要合并特征函数并寻找clique, 用户的特征函数如果定义的维数过大，它得到新的clique也会很大，这样在计算的时候还是会很低效，因为在推断的过程中它需要遍历所有clique中的配置，这和clique的大小是呈指数级的。所以，用户要避免使用维数过高的特征。

与其他算法的比较

优点：

（1）CRF没有HMM那样严格的独立性假设条件，因而可以容纳任意的上下文信息。特征设计灵活（与ME一样）————与HMM比较

（2）同时，由于CRF计算全局最优输出节点的条件概率，它还克服了最大熵马尔可夫模型标记偏置（Label-bias）的缺点。­­————与MEMM比较

（3）CRF是在给定需要标记的观察序列的条件下，计算整个标记序列的联合概率分布，而不是在给定当前状态条件下，定义下一个状态的状态分布。

­­ ————与ME比较

缺点：训练代价大、复杂度高

HMM,MEMM,CRF模型的比较

这三个模型都可以用来做序列标注模型。但是其各自有自身的特点，HMM模型是对转移概率和表现概率直接建模，统计共现概率。而MEMM模型是对转移概率和表现概率建立联合概率，统计是统计的是条件概率。MEMM容易陷入局部最优，是因为MEMM只在局部做归一化，而CRF模型中，统计了全局概率，在做归一化时，考虑了数据在全局的分布，而不是仅仅在局部归一化，这样就解决了MEMM中的标记偏置的问题。

举个例子，对于一个标注任务，“我爱北京天安门“，

                                  标注为" s s  b  e b c e"

对于HMM的话，其判断这个标注成立的概率为 P= P(s转移到s)*P('我'表现为s)* P(s转移到b)*P('爱'表现为s)* ...*P().训练时，要统计状态转移概率矩阵和表现矩 阵。

对于MEMM的话，其判断这个标注成立的概率为 P= P(s转移到s|'我'表现为s)*P('我'表现为s)* P(s转移到b|'爱'表现为s)*P('爱'表现为s)*..训练时，要统计条件状态转移概率矩阵和表现矩阵。

对于CRF的话，其判断这个标注成立的概率为 P= F(s转移到s,'我'表现为s)....F为一个函数，是在全局范围统计归一化的概率而不是像MEMM在局部统计归一化的概率。

[机器学习](http://lib.csdn.net/base/machinelearning)方法的两种分类：产生式模型和判别式模型

假定输入x, 类别标签y

—产生式模型（生成模型）估计联合概率 P(x, y), 因可以根据联合概率来生成样本 —: HMMs

—判别式模型（判别模型）估计条件概率 P(y|x), 因为没有x的知识，无法生成样本，只能判断分类: SVMs,CRF,MEM

一个举例：

(1,0), (1,0), (2,0), (2, 1)

产生式模型：

p(x, y)：

P(1, 0) = 1/2, P(1, 1) = 0, P(2, 0) = 1/4, P(2, 1) = 1/4.

判别式模型：

P(y|x)：

P(0|1) = 1, P(1|1) = 0, P(0|2) = 1/2, P(1|2) = 1/2

—o和s分别代表观察序列和标记序列

—产生式模型

—     构建o和s的联合分布p(s,o)

—判别式模型

—     构建o和s的条件分布p(s|o)

—产生式模型中，观察序列作为模型的一部分；

—判别式模型中，观察序列只作为条件，因此可以针对观察序列设计灵活的特征。

产生式模型：无穷样本==》概率密度模型 = 产生模型==》预测

判别式模型：有限样本==》判别函数 = 预测模型==》预测

    一般认为判别型模型要好于生成型模型，因为它是直接根据数据对概率建模，而生成型模型还要先求两个难度相当的概率﻿﻿﻿﻿

补充下标记偏置问题：

原文：http://blog.csdn.net/zhoubl668/article/details/7787690

![](https://img-my.csdn.net/uploads/201207/26/1343269058_5049.png)

路径1-1-1-1的概率：0.4*0.45*0.5=0.09

路径2-2-2-2的概率:0.018

路径1-2-1-2:0.06

路径1-1-2-2:0.066

由此可得最优路径为1-1-1-1

![](https://img-my.csdn.net/uploads/201207/26/1343269069_8385.png)

而实际上，在上图中，状态1偏向于转移到状态2，而状态2总倾向于停留在状态2，这就是所谓的标注偏置问题，由于分支数不同，概率的分布不均衡，导致状态的转移存在不公平的情况。

PS：标注偏置问题存在于最大熵马尔可夫模型（MEMM）中，虽然MEMM解决了HMM输出独立性假设的问题，但是只解决了观察值独立的问题，状态之间的假设则是标注偏置问题产生的根源，CRF则解决了标注偏置问题，是HMM模型的进一步优化。

![](https://img-my.csdn.net/uploads/201207/26/1343269282_9526.png)

HMM模型中存在两个假设：一是输出观察值之间严格独立，二是状态的转移过程中当前状态只与前一状态有关(一阶马尔可夫模型)。

![](https://img-my.csdn.net/uploads/201207/26/1343269289_5220.png)

MEMM模型克服了观察值之间严格独立产生的问题，但是由于状态之间的假设理论，使得该模型存在标注偏置问题。

![](https://img-my.csdn.net/uploads/201207/26/1343269296_1861.png)

CRF模型解决了标注偏置问题，去除了HMM中两个不合理的假设，当然，模型相应得也变复杂了。

几种统计模型对比：

HMM模型将标注看作马尔可夫链，一阶马尔可夫链式针对相邻标注的关系进行建模，其中每个标记对应一个概率函数。HMM是一种产生式模型，定义了联合概率分布 ，其中x和y分别表示观察序列和相对应的标注序列的随机变量。为了能够定义这种联合概率分布，产生式模型需要枚举出所有可能的观察序列，这在实际运算过程中很困难，因为我们需要将观察序列的元素看做是彼此孤立的个体即假设每个元素彼此独立，任何时刻的观察结果只依赖于该时刻的状态。

HMM模型的这个假设前提在比较小的数据集上是合适的，但实际上在大量真实语料中观察序列更多的是以一种多重的交互特征形式表现，观察元素之间广泛存在长程相关性。在命名实体识别的任务中，由于实体本身结构所具有的复杂性，利用简单的特征函数往往无法涵盖所有的特性，这时HMM的假设前提使得它无法使用复杂特征(它无法使用多于一个标记的特征。

最大熵模型可以使用任意的复杂相关特征，在性能上最大熵分类器超过了Byaes分类器。但是，作为一种分类器模型，这两种方法有一个共同的缺点：每个词都是单独进行分类的，标记之间的关系无法得到充分利用，具有马尔可夫链的HMM模型可以建立标记之间的马尔可夫关联性，这是最大熵模型所没有的。

最大熵模型的优点：首先，最大熵统计模型获得的是所有满足约束条件的模型中信息熵极大的模型;其次，最大熵统计模型可以灵活地设置约束条件，通过约束条件的多少可以调节模型对未知数据的适应度和对已知数据的拟合程度;再次，它还能自然地解决了统计模型中参数平滑的问题。

最大熵模型的不足：首先，最大熵统计模型中二值化特征只是记录特征的出现是否，而文本分类需要知道特征的强度，因此，它在分类方法中不是最优的;其次，由于算法收敛的速度较慢，所以导致最大熵统计模型它的计算代价较大，时空开销大;再次，数据稀疏问题比较严重。

最大熵马尔科夫模型把HMM模型和maximum-entropy模型的优点集合成一个产生式模型，这个模型允许状态转移概率依赖于序列中彼此之间非独立的特征上，从而将上下文信息引入到模型的学习和识别过程中，提高了识别的精确度，召回率也大大的提高，有实验证明，这个新的模型在序列标注任务上表现的比HMM和无状态的最大熵模型要好得多。

CRF模型的特点：首先，CRF在给定了观察序列的情况下，对整个的序列的联合概率有一个统一的指数模型。一个比较吸引人的特性是其 损失函数 的凸面性。其次，条件随机域模型相比较改进的隐马尔可夫模型可以更好更多的利用待识别文本中所提供的上下文信息以得更好的实验结果。条件随机域在中文组块识别方面有效，并避免了严格的独立性假设和数据归纳偏置问题。条件随机域(CRF)模型应用到了中文名实体识别中，并且根据中文的特点，定义了多种特征模板。并且有测试结果表明：在采用相同特征集合的条件下，条件随机域模型较其他概率模型有更好的性能表现。再次，词性标注主要面临兼类词消歧以及未知词标注的难题，传统隐马尔科夫方法不易融合新特征，而最大熵马尔科夫模型存在标注偏置等问题。论文引入条件随机域建立词性标注模型，易于融合新的特征，并能解决标注偏置的问题。

CRFs具有很强的推理能力，并且能够使用复杂、有重叠性和非独立的特征进行训练和推理，能够充分地利用上下文信息作为特征，还可以任意地添加其他外部特征，使得模型能

够获取的信息非常丰富。同时，CRFs解决了最大熵模型中的“label bias”问题。CRFs与最大熵模型的本质区别是：最大熵模型在每个状态都有一个概率模型，在每个状态转移时都要进行归一化。如果某个状态只有一个后续状态，那么该状态到后续状态的跳转概率即为1。这样，不管输入为任何内容，它都向该后续状态跳转。而CRFs是在所有的状态上建立一个统一的概率模型，这样在进行归一化时，即使某个状态只有一个后续状态，它到该后续状态的跳转概率也不会为1，从而解决了“labelbias”问题。因此，从理论上讲，CRFs非常适用于中文的词性标注。

      CRF模型的优点：首先，CRF模型由于其自身在结合多种特征方面的优势和避免了标记偏置问题。其次，CRF的性能更好，CRF对特征的融合能力比较强，对于实例较小的时间类ME来说，CRF的识别效果明显高于ME的识别结果。

      CRF模型的不足：首先，通过对基于CRF的结合多种特征的方法识别英语命名实体的分析，发现在使用CRF方法的过程中，特征的选择和优化是影响结果的关键因素，特征选择问题的好与坏，直接决定了系统性能的高低。其次，训练模型的时间比ME更长，且获得的模型很大，在一般的PC机上无法运行。

再补充一点：

实际上，网上大多数资料中CRF主要用在词性标注问题上，也就是解码过程。



