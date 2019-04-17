# 网络表示学习（DeepWalk，LINE，node2vec，SDNE） - 蓁蓁尔的博客 - CSDN博客





2017年07月24日 12:49:01[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：34144








### **详细的资料可以参考：[网络表示学习相关资料](http://blog.csdn.net/u013527419/article/details/74853633)**

### **1.传统：基于图的表示（又称为基于符号的表示）**

**![](https://img-blog.csdn.net/20170724123001202?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**




如左图G =（V，E），用不同的符号命名不同的节点，用二维数组（邻接矩阵）的存储结构表示两节点间是否存在连边，存在为1，否则为0。



缺点：长尾分布下大部分节点间没有关系，所以邻接矩阵非常稀疏，不利于存储计算。




### 2. 网络表示学习（Network Representation Learning，NRL），也称为图嵌入法（Graph Embedding Method，GEM）：用低维、稠密、实值的向量表示网络中的节点（含有语义关系，利于计算存储，不用再手动提特征（自适应性），且可以将异质信息投影到同一个低维空间中方便进行下游计算）。





**DeepWalk【1】：**

实现1：[https://github.com/phanein/deepwalk](https://github.com/phanein/deepwalk)



用SkipGram的方法进行网络中节点的表示学习。那么，根据SkipGram的思路，最重要的就是定义Context，也就是Neighborhood。​NLP中，Neighborhood是当前Word周围的字，本文用随机游走得到Graph或者Network中节点的Neighborhood。


- （1）随机游走随机均匀地选取网络节点，并生成固定长度的随机游走序列，将此序列类比为自然语言中的句子（节点序列=句子，序列中的节点=句子中的单词），应用skip-gram模型学习节点的分布式表示，skip-gram模型详见：[http://blog.csdn.net/u013527419/article/details/74129996](http://blog.csdn.net/u013527419/article/details/74129996)
- （2）前提：如果一个网络的节点服从幂律分布，那么节点在随机游走序列中的出现次数也服从幂律分布，并且实证发现NLP中单词的出现频率也服从幂律分布。


![](https://img-blog.csdn.net/20170724123438004?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




- （3）大体步骤：



          Network/graph ---------random walk ---------得到节点序列（representation mapping）-------- 放到skip-gram模型中（中间节点预测上下
        文节点）--------- output：representation



![计算机生成了可选文字:网 络 随 机 游 走 t-ti 1 1 与 + 1 + 2 〕 Skip-Gram](https://img-blog.csdn.net/20170724123714819?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**LINE****【****2****】**：

![计算机生成了可选文字:10](https://img-blog.csdn.net/20170724123949044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

（1）先区分两个概念：

**一阶相似度：**直接相连节点间，例如6与7。

定义节点vi和vj间的联合概率为

![计算机生成了可选文字:此@，， ， 与 1 + exp(—üT · ）](https://img-blog.csdn.net/20170724124004291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

v代表节点，u代表节点的embedding。上面式子的意思是两节点越相似，內积越大，sigmoid映射后的值越大，也就是这两节点相连的权重越大，也就是这两个节点间出现的概率越大？？？。



**二阶相似度：**通过其他中介节点相连的节点间例如5与6。

用的是一个条件概率



![计算机生成了可选文字:exp(uy 乥 〕 k ： 1 exp （ 卩 ' ． 豇 ， ）](https://img-blog.csdn.net/20170724124021957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

（2）目标是让NRL前后节点间相似度不变，也节点表示学习前如果两个节点比较相似，那么embedding后的这两个节点表示向量也要很相似。--此文中用的是KL散度，度量两个概率分布之间的距离。KL散度的相关知识详见：[http://blog.csdn.net/u013527419/article/details/51776786](http://blog.csdn.net/u013527419/article/details/51776786)

以保证其一阶相似度为例子：

embedding前：节点vi和vj间的经验联合概率为

![计算机生成了可选文字:= ． 证](https://img-blog.csdn.net/20170724124032815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

所以，最小化：

![计算机生成了可选文字:01 乥 〕 丿 log 夕1@， ， ） 力 e](https://img-blog.csdn.net/20170724124043610?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**Node2vec****【****3****】**：

论文+实现及其他：[http://snap.stanford.edu/node2vec/](http://snap.stanford.edu/node2vec/)

类似于deepwalk，主要的创新点在于改进了随机游走的策略，定义了两个参数p和q，在BFS和DFS中达到一个平衡，同时考虑到局部和宏观的信息，并且具有很高的适应性。

（1）

![计算机生成了可选文字:](https://img-blog.csdn.net/20170724124300672?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

（2）参数控制跳转概率的随机游走，之前完全随机时，p=q=1.

      --返回概率参数（Return parameter）p，对应BFS，p控制回到原来节点的概率，如图中从t跳到v以后，有1/p的概率在节点v处再跳回到t。

     --离开概率参数（In outparameter）q，对应DFS，q控制跳到其他节点的概率。

![计算机生成了可选文字:0 = 1 / q 0 = 1 / q -I/p 1 if if dtT if](https://img-blog.csdn.net/20170724124337423?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上图中，刚从edge （t，v）过来，现在在节点v上，要决定下一步（v，x）怎么走。其中dtx表示节点t到节点x之间的最短路径，dtx=0表示会回到节点t本身，dtx=1表示节点t和节点x直接相连，但是在上一步却选择了节点v，dtx=2表示节点t不与x直接相连，但节点v与x直接相连。

（3）在计算广告、推荐领域中，围绕着node2nec有俩很有意思的应用：

     Facebook：[http://geek.csdn.net/news/detail/200138](http://geek.csdn.net/news/detail/200138)

     Tencent：[http://www.sohu.com/a/124091440_355140](http://www.sohu.com/a/124091440_355140)



**SDNE[4]****:**:

本文的一大贡献在于提出了一种新的半监督学习模型,结合一阶估计与二阶估计的优点,用于表示网络的全局结构属性和局部结构属性。

![计算机生成了可选文字:10 pr 严 { LO structure 0 ； stwing № b structure 2 ： The frame 、 york the semi 、 upervi 、 ed deep model Of 、 D 、 丆](https://img-blog.csdn.net/20170724124522031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

对节点的描述特征向量（比如点的「邻接向量」）使用autoencoder编码，取autoencoder中间层作为向量表示，以此来让获得2ndproximity（相似邻居的点相似度较高，因为两个节点的「邻接向量」相似，说明它们共享了很多邻居，最后映射成的向量y也会更接近）。总觉得上面图中local和global写反了。

目标函数：

![](https://img-blog.csdn.net/20170724124532747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



【1】Perozzi B, Al-Rfou R, Skiena S.Deepwalk: Online learning of social representations[C]，KDD2014:
 701-710.

【2】LINE：Large-scaleInformation Network Embedding。WWW2015，JianTang,
 Meng Qu , Mingzhe Wang, Ming Zhang, Jun Yan, Qiaozhu Mei，MicrosoftResearch Asia;Peking University,China;University of Michigan。

【3】node2vec: Scalable Feature Learning forNetworks，A
 Grover, J Leskovec [StanfordUniversity] (KDD2016)

【4】Structural Deep Network Embedding，KDD 2016



上面都是我比较感兴趣一点的，详细的可以参考：https://github.com/thunlp/NRLpapers








