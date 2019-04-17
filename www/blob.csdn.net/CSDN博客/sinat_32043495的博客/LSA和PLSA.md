# LSA和PLSA - sinat_32043495的博客 - CSDN博客





2017年12月11日 11:17:14[LZXandTM](https://me.csdn.net/sinat_32043495)阅读数：731








##  1.词袋模型 

  词袋模型(BOW, Bag-of-Words) 模型是NLP领域中的一个基本假设，一个文档(document)被表示为一组单词(word/term)的无序组合，而忽略了语法或者词序的部分,存在缺陷:

  稀疏性: 对于大词典，尤其是包括了生僻字的词典，文档稀疏性不可避免；

  多义词: BOW模型只统计单词出现的次数，而忽略了一个词可能存在多种含义，一词多义；

  同义词(Synonym): 多个单词可以表示同一个意思，一义多词；

  从同义词和多义词问题可以看到：单词也许不是文档的最基本组成元素，在单词与文档之间还有一层隐含的关系，我们称之为主题(Topic)**，我们更关注隐藏在词之后的意义和概念**。在写文章时首先确定的是文章的主题，再根据主题选择合适的单词来表达观点。在BOW模型中引入Topic的因素，即潜在语义分析(LSA, Latent SemanticAnalysis)和概率潜在语义分析(pLSA,probabilistic Latent Semantic Analysis)

![](https://img-blog.csdn.net/20171210205506195?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 2.LSA模型

    LSA的基本思想就是，将document从稀疏的高维Vocabulary空间映射到一个低维的向量空间，我们称之为隐含语义空间(Latent Semantic Space).

     具体说来就是对一个大型的文档集合使用一个合理的维度建模，并将词和文档都表示到该空间，比如有2000个文档，包含7000个索引词，LSA使用一个维度为100的向量空间将文档和词表示到该空间，进而在该空间进行信息检索。

![](https://img-blog.csdn.net/20171210205459980?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   在图上，**每一个圆圈表示一个主题**，每一个红色的点，**都表示一个词**，每一个蓝色的点，**都表示一篇文档**，这样我们可以对这些词和文档进行聚类，比如说 stock 和 market 可以放在一类，因为他们老是出现在一起，real 和 estate 可以放在一类，dads，guide
 这种词就看起来有点孤立了，我们就不对他们进行合并了。按这样聚类出现的效果，在某个主题下，会有多个文档和多个单词。

![](https://img-blog.csdn.net/20171210205510990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

   而将文档表示到此空间的过程就是SVD奇异值分解和降维的过程。降维是LSA分析中最重要的一步，通过降维，去除了文档中的“噪音”，也就是无关信息（比如词的误用或不相关的词偶尔出现在一起），语义结构逐渐呈现。

![](https://img-blog.csdn.net/20171210205454823?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


   每个奇异值对应的是每个“语义”维度的权重，将不太重要的权重置为0，只保留最重要的维度信息，去掉一些信息“nosie”,因而可以得到文档的一种更优表示形式。理解:矩阵 U 中的**每一列表示一个关键词，每个key word与各个词的相关性**，数值越大越相关；矩阵 V
 中的**每一行表示一类主题，其中的每个非零元素表示一个主题与一个文档的相关性**。中间的矩阵 D 则表示文章主题和keyword之间的相关性。因此，我们只要对关联矩阵 X 进行一次奇异值分解，我们就可以同时完成了近义词分类和文章的分类。（同时得到每个主题的相关性）。

   LSA的优点

1）低维空间表示可以刻画同义词，同义词会对应着相同或相似的主题。

2）降维可去除部分噪声，是特征更鲁棒。

3）充分利用冗余数据。

4）无监督/完全自动化。

5）与语言无关。

    LSA的缺点

1）LSA可以处理向量空间模型无法解决的一义多词(synonymy)问题，但不能解决一词多义(polysemy)问题。因为**LSA将每一个词映射为潜在语义空间中的一个点，也就是说一个词的多个意思在空间中对于的是同一个点，并没有被区分**。

2）SVD的优化目标基于L-2 norm 或者 FrobeniusNorm 的，这相当于隐含了对数据的高斯分布假设。而 term 出现的次数是非负的，这明显不符合 Gaussian 假设，而更接近 Multi-nomial 分布。

3）特征向量的方向没有对应的物理解释。

4）SVD的计算复杂度很高，而且当有新的文档来到时，若要更新模型需重新训练。

5）没有刻画term出现次数的概率模型。

6）对于count vectors 而言，欧式距离表达是不合适的（重建时会产生负数）。

7）维数的选择是ad-hoc的。

##  3.PLSA模型

   假设你要写M篇文档，由于一篇文档由各个不同的词组成，所以你需要确定每篇文档里每个位置上的词。再假定你一共有K个可选的主题，有V个可选的词，咱们来玩一个扔骰子的游戏。

***  1***. 假设你每写一篇文档会制作一颗K面的“文档-主题”骰子（扔此骰子能得到K个主题中的任意一个），和K个V面的“主题-词项” 骰子（每个骰子对应一个主题，K个骰子对应之前的K个主题，且骰子的每一面对应要选择的词项，V个面对应着V个可选的词）。

   比如可令K=3，即制作1个含有3个主题的“文档-主题”骰子，这3个主题可以是：教育、经济、交通。然后令V = 3，制作3个有着3面的“主题-词项”骰子，其中，教育主题骰子的3个面上的词可以是：大学、老师、课程，经济主题骰子的3个面上的词可以是：市场、企业、金融，交通主题骰子的3个面上的词可以是：高铁、汽车、飞机。

***  2***. 每写一个词，先扔该“文档-主题”骰子选择主题，得到主题的结果后，使用和主题结果对应的那颗“主题-词项”骰子，扔该骰子选择要写的词。     {**每篇文档有不同的文档-主题 骰子**}

   先扔“文档-主题”的骰子，假设（以一定的概率）得到的主题是教育，所以下一步便是扔教育主题筛子，（以一定的概率）得到教育主题筛子对应的某个词：大学。

  上面这个投骰子产生词的过程简化下便是：“先以一定的概率选取主题，再以一定的概率选取词”。事实上，一开始可供选择的主题有3个：教育、经济、交通，那为何偏偏选取教育这个主题呢？其实是随机选取的，只是这个随机遵循一定的概率分布。比如3个主题的概率分布是{教育：0.5，经济：0.3，交通：0.2}，      我们把**各个主题z在文档d中出现的概率分布称之为主题分布，**且是一个**多项分布**。

   同样的，从主题分布中随机抽取出教育主题后，依然面对着3个词：大学、老师、课程，这3个词都可能被选中，但它们被选中的概率也是不一样的。比如3个词的概率分布是{大学：0.5，老师：0.3，课程：0.2}，我们把**各个词语w在主题z下出现的概率分布称之为词分布**，这个词分布也是一个**多项分布**。

    所以，**选主题和选词都是两个随机的过程，先从主题分布{教育：0.5，经济：0.3，交通：0.2}中抽取出主题：教育，然后从该主题对应的词分布{大学：0.5，老师：0.3，课程：0.2}中抽取出词：大学**。

***3***. 最后，你不停的重复扔“文档-主题”骰子和”主题-词项“骰子，重复N次（产生N个词），完成一篇文档，重复这产生一篇文档的方法M次，则完成M篇文档。

![](https://img-blog.csdn.net/20171211104429998?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171211104435549?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171211105529357?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


如果将样本看作观察值，潜在类别看作是隐藏变量，那么聚类问题也就是参数估计问题，只不过聚类问题中参数分为隐含类别变量和其他参数


![](https://img-blog.csdn.net/20171211105546558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171211105605746?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171211105627675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**假定参数已知，计算此时隐变量的后验概率。**


![](https://img-blog.csdn.net/20171211105647197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**Step 2: Maximization**

**带入隐变量的后验概率，最大化样本分布的对数似然函数，求解相应的参数。**

    求关于参数P(zk|dm)和P(wj|zk)的**Complete data对数似然函数期望**的极大值，得到最优解。带入E步迭代循环。

由式(*)可得：

![](https://img-blog.csdn.net/20171211105931869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




   此式后部分为常量。故令：

![](https://img-blog.csdn.net/20171211110109782?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20171211110028711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


建立以下目标函数与约束条件：

![](https://img-blog.csdn.net/20171211110213362?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

只有等式约束，使用Lagrange乘子法解决：

![](https://img-blog.csdn.net/20171211110257629?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




对P(zk|dm)和P(wj|zk)求驻点，得：

![](https://img-blog.csdn.net/20171211110335622?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171211110343733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


令![](https://img-blog.csdn.net/20171211110640882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)为0：

![](https://img-blog.csdn.net/20171211110418634?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


故有：

![](https://img-blog.csdn.net/20171211110429740?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


同理：

![](https://img-blog.csdn.net/20171211110442750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


将P(zk|dm)和P(wj|zk)代回E步

![](https://img-blog.csdn.net/20171211110504049?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzIwNDM0OTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


PLUS：

**EM算法理论：**



**em****算法指的是最大期望算法（****ExpectationMaximization Algorithm****，又译期望最大化算法），是一种迭代算法，用于含有隐变量（****latentvariable****）的概率参数模型的最大似然估计或极大后验概率估计。**

简版：猜（E-step）,反思（M-step）,重复；

啰嗦版：

你知道一些东西（观察的到的数据）， 你不知道一些东西（观察不到的），你很好奇，想知道点那些不了解的东西。怎么办呢，你就根据一些假设（parameter）先猜（E-step），把那些不知道的东西都猜出来，假装你全都知道了; 然后有了这些猜出来的数据，你反思一下，更新一下你的假设（parameter）, 让你观察到的数据更加可能(Maximizelikelihood; M-stemp); 然后再猜，在反思，最后，你就得到了一个可以解释整个数据的假设了。



pLSA的建模思想较为简单，对于observed variables建立likelihood function，将latent variable暴露出来，并使用E-M算法求解。其中M步的标准做法是引入Lagrange乘子求解后回代到E步。

**总结**一下使用EM算法求解pLSA的基本实现方法：

(1)E步骤：求隐含变量Given当前估计的参数条件下的后验概率。

(2)M步骤：最大化**Complete data对数似然函数**的期望，此时我们使用E步骤里计算的隐含变量的后验概率，得到新的参数值。

两步迭代进行直到收敛。



参考：

1.http://blog.csdn.net/pipisorry/article/details/42560331

2.http://blog.csdn.net/pipisorry/article/details/42560693

3.https://www.cnblogs.com/bentuwuying/p/6219970.html

4.https://www.zhihu.com/question/27976634/answer/39132183



