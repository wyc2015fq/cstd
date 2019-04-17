# DBN的浅显易懂解释 - lijunweiyhn的博客 - CSDN博客





2016年10月15日 16:16:01[_Junwei](https://me.csdn.net/lijunweiyhn)阅读数：3486









转载自：http://blog.163.com/silence_ellen/blog/static/176104222201431710264087/





本篇非常简要地介绍了深度信念网络的基本概念。文章先简要介绍了深度信念网络（包括其应用实例）。接着分别讲述了：(1) 其基本组成结构——受限玻尔兹曼机的的基本情况，以及，(2) 这个基本结构如何组成深度信念网络。 本文仅仅能使读者了解深度信念网络这一概念，内容非常浅显，甚至有许多不严密的地方。如果有愿意深入了解受限玻尔兹曼机、深度信念网络的，想对深度学习有更多了解的，请访问深度学习官方网站。或者读者可以参考文末的“推荐阅读”一节。 



目录 


概述 .............................................................................................................................. 2 


受限玻尔兹曼机 ............................................................................................................ 3 


神经元之间的条件独立性 .......................................................................................... 3 


使用 RBM 的过程 ..................................................................................................... 4 


训练 RBM ................................................................................................................. 5 


深度信念网络 ................................................................................................................ 5 


训练过程： ................................................................................................................ 6 


调优过程 (Fine-Tuning) ： ...................................................................................... 6 


使用过程 ................................................................................................................... 7 


推荐阅读 ....................................................................................................................... 8 


论文 .......................................................................................................................... 8 


博文和网络教程 ........................................................................................................ 8 





概述


深度信念网络 (Deep Belief Network, DBN) 由 Geoffrey Hinton 在 2006 年提出。它是一种生成模型，通过训练其神经元间的权重，我们可以让整个神经网络按照最大概率来生成训练数据。我们不仅可以使用 DBN 识别特征、分类数据，还可以用它来生成数据。下面的图片展示的是用
 DBN 识别手写数字： 





![](https://img-blog.csdn.net/20161201163437773?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




图 1 用深度信念网络识别手写数字。图中右下角是待识别数字的黑白位图，它的上方有三层隐性神经元。每一个黑色矩形代表一层神经元，白点代表处于开启状态的神经元，黑色代表处于关闭状态的神经元。注意顶层神经元的左下方即使别结果，与画面左上角的对应表比对，得知这个 DBN 正确地识别了该数字。







下面是展示了一个学习了大量英文维基百科文章的 DBN 所生成的自然语言段落： 

In 1974 Northern Denver had been overshadowed by CNL, and several Irish intelligence agencies in the Mediterranean region. However, on the Victoria,
 Kings Hebrew stated that Charles decided to escape during an 


alliance. The mansion house was completed in 1882, the second in its bridge are omitted, while closing is the proton reticulum composed below it aims, such that it is the blurring of appearing
 on any well-paid type of box printer. 




DBN 由多层神经元构成，这些神经元又分为显性神经元和隐性神经元（以下简称显元和隐元）。显元用于接受输入，隐元用于提取特征。因此隐元也有个别名，叫特征检测器 (feature
 detectors)。最顶上的两层间的连接是无向的，组成联合内存 (associative memory)。较低的其他层之间有连接上下的有向连接。最底层代表了数据向量 (data
 vectors)，每一个神经元代表数据向量的一维。 


DBN 的组成元件是受限玻尔兹曼机 (Restricted Boltzmann Machines, RBM)。训练 DBN 的过程是一层一层地进行的。在每一层中，用数据向量来推断隐层，再把这一隐层当作下一层
 (高一层) 的数据向量。 







受限玻尔兹曼机 


如前所述，RBM 是 DBN 的组成元件。事实上，每一个 RBM 都可以单独用作聚类器。 


RBM 只有两层神经元，一层叫做显层 (visible layer)，由显元 (visible
 units) 组成，用于输入训练数据。另一层叫做隐层 (Hidden layer)，相应地，由隐元(hidden
 units) 组成，用作特征检测器 (feature detectors)。 

![【转】深度学习--深度信念网络（Deep Belief Network） - 路过天堂_ellen - 狗狗的地盘_ellen](http://img1.ph.126.net/dYm8YlDUvtIe_YjyRZkOVQ==/6598194567053621299.jpg)


图 2 受限玻尔兹曼机的结构。图中的较上一层神经元组成隐层，较下的神经元组成显元。每一层都可以用一个向量来表示，每一维表示每个神经元。注意这两层间的对称 (双向) 连接。 




神经元之间的条件独立性


应注意到，显层和隐层内部的神经元都没有互连，只有层间的神经元有对称的连接线。这样的好处是，在给定所有显元的值的情况下，每一个隐元取什么值是互不相关的。也就是说， 


![](https://img-blog.csdn.net/20161201163526008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

同样，在给定隐层时，所有显元的取值也互不相关： 


![【转】深度学习--深度信念网络（Deep Belief Network） - 路过天堂_ellen - 狗狗的地盘_ellen](http://img2.ph.126.net/5dWsl-LOtIgjtK5IYKtzdg==/6608207819445875657.png)



有了这个重要性质，我们在计算每个神经元的取值情况时就不必每次计算一个，而是同时并行地计算整层神经元。 



使用 RBM 的过程 


假设我们现在已经得到一个训练好的 RBM，每个隐元与显元间的权重用矩阵


W表示，且： 

![【转】深度学习--深度信念网络（Deep Belief Network） - 路过天堂_ellen - 狗狗的地盘_ellen](http://img2.ph.126.net/Y7VazLvGOHq-c5TPrISzww==/6597542556658407011.png)




其中Wij代表从第i个显元到第j个隐元的权重，M代表显元的个数，N代表隐元的个数。那么，当我们把一条新来的数据![【转】深度学习--深度信念网络（Deep Belief Network） - 路过天堂_ellen - 狗狗的地盘_ellen](http://img2.ph.126.net/XrcQBxXxoIMhyun-Z0VTaw==/6597981261797884619.png)附给
 (clamp to) 显层后，RBM 将会依照权值W来决定开启或关闭隐元。具体的操作如下： 


首先，将每个隐元的激励值 (activation) 计算出来： 


![【转】深度学习--深度信念网络（Deep Belief Network） - 路过天堂_ellen - 狗狗的地盘_ellen](http://img1.ph.126.net/knQ-ZeJHx20jR_SINIiF6g==/1499135726061160549.jpg)
注意，这里用到了前面提到的神经元之间的条件独立性。 


然后，将每个隐元的激励值都用 S 形函数进行标准化，变成它们处于开启状 (用 1 表示) 的概率值： 


![](https://img-blog.csdn.net/20161201163555431?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




此处的 S 形函数我们采用的是 Logistic 函数： 

![](https://img-blog.csdn.net/20161201163559412?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


至此，每个隐元hj开启的概率被计算出来了。其处于关闭状态 (用 0 表示) 的概率自然也就是 


![【转】深度学习--深度信念网络（Deep Belief Network） - 路过天堂_ellen - 狗狗的地盘_ellen](http://img1.ph.126.net/-HHsOjSqjSDHGpXVY_CsTw==/758012112381956563.png)
那么到底这个元开启还是关闭，我们需要将开启的概率与一个从 0, 1 均匀分布中抽取的随机值 


![](https://img-blog.csdn.net/20161201163602244?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

进行如下比较 


![【转】深度学习--深度信念网络（Deep Belief Network） - 路过天堂_ellen - 狗狗的地盘_ellen](http://img1.ph.126.net/TLJPCJZs4INvzQX0F6G1UA==/1411034058351247948.png)



然后开启或关闭相应的隐元。


给定隐层，计算显层的方法是一样的。 




训练 RBM 


RBM 的训练过程，实际上是求出一个最能产生训练样本的概率分布。也就是说，要求一个分布，在这个分布里，训练样本的概率最大。由于这个分布的决定性因素在于权值W ，所以我们训练 RBM 的目标就是寻找最佳的权值。为了保持读者的兴趣，这里我们不给出最大化对数似然函数的推导过程，直接说明如何训练
 RBM。 


G. Hinton 提出了名为对比散度 (Contrastive Divergence) 的学习算法。下面我们来详述它的具体过程。 


我们沿用前文的符号记法。 


算法1. 对比散度 


对于训练集中的每一条记录



![【转】深度学习--深度信念网络（Deep Belief Network） - 路过天堂_ellen - 狗狗的地盘_ellen](http://img2.ph.126.net/0EEmT3V9P2kQZv5r1eRHdw==/6597420510866548832.png)




如此训练之后的 RBM 就能较为准确地提取显层的特征，或者根据隐层所代表


的特征还原显层了。 







深度信念网络 


前文我们已经介绍了 RBM 的基本结构和其训练、使用过程，接下来我们介绍DBN 的相关内容。

DBN 是由多层 RBM 组成的一个神经网络，它既可以被看作一个生成模型，也可以当作判别模型，其训练过程是：使用非监督贪婪逐层方法去预训练获得权值。 

训练过程： 


1. 首先充分训练第一个 RBM； 


2. 固定第一个 RBM 的权重和偏移量，然后使用其隐性神经元的状态，作为第二个 RBM 的输入向量； 


3. 充分训练第二个 RBM 后，将第二个 RBM 堆叠在第一个 RBM 的上方； 


4. 重复以上三个步骤任意多次； 


5. 如果训练集中的数据有标签，那么在顶层的 RBM 训练时，这个 RBM 的显层中除了显性神经元，还需要有代表分类标签的神经元，一起进行训练： 

a) 假设顶层 RBM 的显层有 500 个显性神经元，训练数据的分类一共分成了 10 类； 

b) 那么顶层 RBM 的显层有 510 个显性神经元，对每一训练训练数据，相应的标签神经元被打开设为 1，而其他的则被关闭设为 0。 


6. DBN 被训练好后如下图： (示意) 


![【转】深度学习--深度信念网络（Deep Belief Network） - 路过天堂_ellen - 狗狗的地盘_ellen](http://img1.ph.126.net/xk0GLgXqidDXhXaH_KPSow==/6598174775844322066.png)

图 3 训练好的深度信念网络。图中的绿色部分就是在最顶层 RBM 中参与训练的标签。注意调优 (FINE-TUNING) 过程是一个判别模型 



 另：


![【转】深度学习--深度信念网络（Deep Belief Network） - 路过天堂_ellen - 狗狗的地盘_ellen](http://img2.ph.126.net/cDaEkOavrC5rFl__anaLIQ==/6597632716611883265.png)


调优过程 (Fine-Tuning) ： 


生成模型使用 Contrastive Wake-Sleep 算法进行调优，其算法过程是： 

1. 除了顶层 RBM，其他层 RBM 的权重被分成向上的认知权重和向下的生成权重； 


2. Wake 阶段：认知过程，通过外界的特征和向上的权重 (认知权重) 产生每一层的抽象表示 (结点状态) ，并且使用梯度下降修改层间的下行权重 (生成权重) 。也就是“如果现实跟我想象的不一样，改变我的权重使得我想


象的东西就是这样的”。 


3. Sleep 阶段：生成过程，通过顶层表示 (醒时学得的概念) 和向下权重，生成底层的状态，同时修改层间向上的权重。也就是“如果梦中的景象不是我脑中的相应概念，改变我的认知权重使得这种景象在我看来就是这个概


念”。 




使用过程 ：


1. 使用随机隐性神经元状态值，在顶层 RBM 中进行足够多次的吉布斯抽样； 


2. 向下传播，得到每层的状态。 






推荐阅读 


论文 


1. Representation Learning: A Review and New Perspectives, Yoshua Bengio, Aaron Courville, Pascal Vincent, Arxiv, 2012. 


2. The monograph or review paper Learning Deep Architectures for AI (Foundations & Trends in Machine Learning, 2009). 


3. Deep Machine Learning – A New Frontier in Artificial Intelligence Research – a survey paper by Itamar Arel, Derek C. Rose, and Thomas P. Karnowski. 


4. A Fast Learning Algorithm for Deep Belief Nets by Geoffrey E. Hinton and Simon Osindero. 


博文和网络教程 


1. Introduction to Restricted Boltzmann Machines by Edwin Chen. 


2. An Introduction to Restricted Boltzmann Machines by Yuhuan Jiang. 


3. Restricted Boltzmann Machine - Short Tutorial by iMonad. 


4. 《深度学习学习笔记整理系列》 by Zouxy. 



