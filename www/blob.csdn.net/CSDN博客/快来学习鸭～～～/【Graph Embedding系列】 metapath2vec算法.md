# 【Graph Embedding系列】: metapath2vec算法 - 快来学习鸭～～～ - CSDN博客





2019年03月17日 21:54:23[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：96








[metapath2vec: Scalable Representation Learning for Heterogeneous Networks](https://dl.acm.org/citation.cfm?id=3098036) 发表在kdd2017，用于解决异构网络的embedding表示。个人看完文章的感觉就是将deepwalk的算法思路引入到异构网络当中，并针对异构网络的特点，针对deepwalk算法中的各个步骤，针对性的进行优化。所以看在了解metapath2vec之前建议先了解一下deepwalk。

# 什么是异构网络？

异构网络不同于同构网络在于，同构网络中的nodes都是同一类型的节点，link都是针对同一类型节点的链接。而异构网络节点可以是不同类型的实体，链接也可以是不同实体之间的链接。拿电商行为来说，用户A购买了店铺S1的商品L1，用户A购买了店铺S2的商品L2。如果我们想用同构网络来表示用户的购买session，那么网络的节点就是可以用商品来表示，并把同一用户的购买商品按顺序链接起来。但是这样就丢失了用户和店铺这两个实体。异构网络就可以解决这个问题，我们可以直接把用户和店铺进行链接，店铺和商品进行链接，用户和商品进行链接起来，形成一个异构网络。

# 定义问题

**异构网络（Heterogeneous Networks）: **异构网络由图G(V,E,T)表示，其中节点v和链接e分别对应映射函数：

![\phi (v):V->T_v](https://private.codecogs.com/gif.latex?%5Cphi%20%28v%29%3AV-%3ET_v) 和 ![\phi (e):E->T_E](https://private.codecogs.com/gif.latex?%5Cphi%20%28e%29%3AE-%3ET_E), 这里的![T_E](https://private.codecogs.com/gif.latex?T_E)和 ![T_v](https://private.codecogs.com/gif.latex?T_v)指对象和关系类型的集合。并且![|T_V|+|T_E|>2](https://private.codecogs.com/gif.latex?%7CT_V%7C&plus;%7CT_E%7C%3E2),如果等于2的话就是同构网络。这里想要说明的意思其实就是说 异构网络是由一个或者多个类型的节点，以及一个或者多个类型的链接组成的图。

**异构网络表示学习(Heterogeneous Network Reperesentation Learning):**给定一个异构网络G，这里的工作就是将图中的节点v表示成一个低纬向量![X \in R^{|V|\times d} , d\ll |V|](https://private.codecogs.com/gif.latex?X%20%5Cin%20R%5E%7B%7CV%7C%5Ctimes%20d%7D%20%2C%20d%5Cll%20%7CV%7C),用于学习出图中节点的结构和语义相关性。而该问题的输出则为低纬矩阵X，表示的是一个所有节点的低纬向量，所以它的大小是节点个数*d。而该问题因为同时优化不同类型的节点，所以最后生成的节点低纬向量都在同一高纬空间。所以我们能很方便的将学习出的向量应用于各种各样的任务。

# 异构网络embedding:metapath2vec

在metapath2vec中，采用和deepwalk类似的方式，利用skip-gram来学习图的embedding。主要步骤由两步，1. 利用随机游走来从图中获取序列，2.利用skip-gram优化提取的序列。但是针对异构图，这部分中都存在一定的差异。这里分别说一下。

**Heterogeneous Skip-Gram. 给定**异构网络![G=(V,E,T), |T_V| > 1](https://private.codecogs.com/gif.latex?G%3D%28V%2CE%2CT%29%2C%20%7CT_V%7C%20%3E%201), 目标就是在给定节点v后，使其上下文内容存在的概率最大化，如下：

![](https://img-blog.csdnimg.cn/20190317194606543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

这里的![N_t(v)](https://private.codecogs.com/gif.latex?N_t%28v%29)指的是在节点v的邻近节点中，为第t个类型的节点。而概率函数![p(c_t|v;\theta )](https://private.codecogs.com/gif.latex?p%28c_t%7Cv%3B%5Ctheta%20%29)则为softmax。可表示为：

![p(c_t|v;\theta )=\frac{e^{X_{ct}\cdot X_v}}{\sum_{u\in V} e^{{X_u}\cdot X_v}}](https://private.codecogs.com/gif.latex?p%28c_t%7Cv%3B%5Ctheta%20%29%3D%5Cfrac%7Be%5E%7BX_%7Bct%7D%5Ccdot%20X_v%7D%7D%7B%5Csum_%7Bu%5Cin%20V%7D%20e%5E%7B%7BX_u%7D%5Ccdot%20X_v%7D%7D)

这里![X_v](https://private.codecogs.com/gif.latex?X_v)就是从矩阵X从取出来的第v行向量，它表示节点v的embedding向量。为了减少计算量，进一步优化为负采样后的优化目标：

![log \sigma (X_{c_t}\cdot X_v)+\sum ^M_{m=1}E_{u^m}~P(u) [log \sigma (-X_{u^m}\cdot X_v)]](https://private.codecogs.com/gif.latex?log%20%5Csigma%20%28X_%7Bc_t%7D%5Ccdot%20X_v%29&plus;%5Csum%20%5EM_%7Bm%3D1%7DE_%7Bu%5Em%7D~P%28u%29%20%5Blog%20%5Csigma%20%28-X_%7Bu%5Em%7D%5Ccdot%20X_v%29%5D)

其中![\sigma](https://private.codecogs.com/gif.latex?%5Csigma)是sigmoid函数，p(u)是预定义的函数，用于采样节点M次。这里它并没有区分不同的节点来进行采样，对不同节点进行均匀采样。

**元路径随机游走（Meta-Path-Based Random Walks.）**在deepwalk和node2vec中都采用的是随机游走的方式，如果不考虑节点之间的类型，我们也可以直接使用随机游走来定义转化概率![p(v^{i+1}|v^i)](https://private.codecogs.com/gif.latex?p%28v%5E%7Bi&plus;1%7D%7Cv%5Ei%29),然后生成的序列直接利用skipgram来进行优化。但是sun等人证明来随机游走是偏向于高可见类型的节点，即那些具有主导优势的节点，所以本文提出来一个基于元路径的随机游走方法。

定义matapath P的形式为

![](https://img-blog.csdnimg.cn/20190317205952756.png)

这里的![V_1,V_2,....](https://private.codecogs.com/gif.latex?V_1%2CV_2%2C....)指的都是节点类型，metapath的意思就是事先定义好节点类型的变化规律。而整个变化规律就是从![V_1](https://private.codecogs.com/gif.latex?V_1)到![V_l](https://private.codecogs.com/gif.latex?V_l)。这样就可以将变化规律限制在定义好的metapath之中，用于解决随机游走偏向于选择高可见类型节点的问题。具体的在每步的转移概率为：

![](https://img-blog.csdnimg.cn/20190317211950279.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

其中![v_t^i\in V_t](https://private.codecogs.com/gif.latex?v_t%5Ei%5Cin%20V_t),而![N_{t+1}(v_t^i)](https://private.codecogs.com/gif.latex?N_%7Bt&plus;1%7D%28v_t%5Ei%29)指的是节点![v_t^i](https://private.codecogs.com/gif.latex?v_t%5Ei)的![V_{t+1}](https://private.codecogs.com/gif.latex?V_%7Bt&plus;1%7D)类型的邻近节点，而转移概率就是该类型节点个数的倒数。表达的是只有在下一步为指定mtapath位置上的节点类型的时候才发生转移，并且转移概率为领域内该类型节点数的倒数。基于metapath的随机游走保证来语义变化的正确性。比如如下图中的a4所示，如果其上一步为CMU，那么它可以转移到a2,a3,a5,p2,p3,CMU中任意和其由链接的节点。但是如果定义来metapath “OAPVPAO”，随机游走就会偏向于选择P类型的节点。

# metapath2vec++

在前面的metapath中，存在一个小问题，就是随机游走这一步是考虑来节点的类型了，但是skipgram在训练的时候并有没对其区分对待。所以进一步针对这点提出了**Heterogeneous negative sampling.** 优化函数如下：

![](https://img-blog.csdnimg.cn/20190317214536177.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

![V_t](https://private.codecogs.com/gif.latex?V_t)是整个网络中类型为t的集合，即根据同类型的节点进行softmax归一化。这样的话，相当于每个类型的节点都会有了一个概率分布。按文章给的例子来说，给定一个目标节点a4，那么利用这个方法就能得到对应的V,A,O,P四种节点类型的分布。

优化后的负采样目标如下：

![](https://img-blog.csdnimg.cn/20190317215102749.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)



以上就是metapath2vec的核心思想，具体的实验结果，可以详细看论文。具体的实现代码和实验数据可以参考：

[https://ericdongyx.github.io/metapath2vec/m2v.html](https://ericdongyx.github.io/metapath2vec/m2v.html)

论文下载地址：

[https://www3.nd.edu/~dial/publications/dong2017metapath2vec.pdf](https://www3.nd.edu/~dial/publications/dong2017metapath2vec.pdf)

完



