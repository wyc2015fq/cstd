
# 基于概率论的分类方法：朴素贝叶斯及CSDN_RSS源分析 - OraYang的博客 - CSDN博客

2017年10月14日 09:12:21[OraYang](https://me.csdn.net/u010665216)阅读数：881所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



本文所有代码都是基于python3.6的，数据及源码下载：[传送门](https://gitee.com/orayang_admin/Naive_Bayes/tree/master)
## 引言
最简单的解决方法通常是最强大的，朴素贝叶斯呢就是一个很好的证明。尽管在过去的几年里机器学习取得了巨大的进步，各种优秀算法层出不穷，但朴素贝叶斯依旧证明了它不仅简单，而且快速、准确、可靠。朴素贝叶斯已经成功地用于许多机器学习领域，但它在自然语言处理(NLP)问题上尤其有效。
朴素贝叶斯是一种利用概率论和贝叶斯定理来预测样本类别的概率算法。概率性，这意味着朴素贝叶斯计算给定样本的每个类别的概率，然后输出最高的类别。它们得到这些概率的方法是使用贝叶斯定理，它描述了一个特征的概率，基于可能与这个特征相关的先验知识。
众所周知，概率论是许多机器学习算法的基础，所以理解概率论相关知识对我们深刻理解这一主题就显得尤为重要。前面决策树分享：[ID3决策树原理分析及python实现](http://blog.csdn.net/u010665216/article/details/78173064)我们在计算香农熵时，有涉及到一些概率知识。这次分享将在此基础上做进一部分的拓展。
## 一个简单的例子
我们接下来看看在一个实际的例子，假设我们正在构造一个分类器，它表示文本是否与运动有关。我们的训练集一共有五个句子：
|Text|Category|
|---|---|
|“A great game”|Sports|
|“The election was over”|Not Sports|
|“Very clean match”|Sports|
|“A clean but forgettable game”|Sports|
|“It was a close election”|Not Sports|
那么问题来了，文本”A very close game”属于哪一类呢？（结合前面已经分享的kNN、decision tree）。既然朴素贝叶斯是一个概率分类器，那么我们只要计算出该文本属于Sports的概率P1以及不属于Sports的概率P2，选择概率最大的类别即可。从数学公式上来说就是：$P(Sports|A\ very\ close\ game)$——给定句子A very close game，该句子类别是Sports的概率。
啊哈，如何？简单吧，那么我们该如何求这些概率呢？
## 特征工程
在创建机器模型时，我们所做的第一件事是把什么用作特征。在这个例子中，我们没有数值特征，只有文本。我们需要把文本转换成我们可以用来计算概率的数字。那么我们该怎么做呢？很简单！我们使用词频。也就是说，我们忽略单词顺序和句子结构，把每个文本都看成是它包含的单词的集合。我们的特征就是是每一个单词的计数。尽管这种方法看起来过于简单，但效果却出奇的好。
## 贝叶斯定理
现在我们需要把我们想要计算的概率转换成可以用词频来计算的东西。为此，我们将使用概率的一些基本性质，以及贝叶斯定理。贝叶斯定理在处理条件概率时很好用。
### 贝叶斯决策论
贝叶斯决策论是概率框架下实施决策的基本方法。对于分类任务来说，在所有相关概率都已知的理想情形下，贝叶斯决策论考虑如何基于**这些概率和误判损失**来选择最优的类别标记。我们用上面的例子举例。假设有两种可能的类别标记$N=2$，则类别标签$y=\{c_1,c_2\}$，其中$\lambda_{ij}$是将一个真实标记为$c_j$的样本误分类为$c_i$所产生的损失。基于后验概率$P(c_i|x)$可获得将样本$x$分类为$c_i$所产生的期望损失，即样本x上的”条件风险“

$$
R(c_i|x) = \sum_{j=1}^N \lambda_{ij}P(c_j|x)
$$
接下来我们的任务是寻找一个判定准则来最小化总体风险：

$$
R(h) = E_x[R(h(x)|x)]
$$
为了最小化总体风险，我们就有了贝叶斯判定准则：为了最小化总体风险，只需在每个样本上选择那个能使条件风险$R(c|x)$最小的类别标记。
对于上述例子中的二分类问题，若目标是最小化分类错误率，则误判损失可写为：

$$
\lambda_{ij} = \{_{1,otherwise,}^{0 , if i=j;}
$$
此时条件风险

$$
R(c|x) = 1-P(c|x)
$$
于是我们最小化
$$
R(c|x)
$$
就是最大化后验概率
$$
P(c|x)
$$
其实，机器学习所要实现的就是基于有限的训练样本尽可能准确地估计出后验概率
$$
P(c|x)
$$
大体来说，主要有两种策略：
> 给定x，直接建模
> P(c|x)
> 来预测c，这样的得到的是”判别式模型“

> 给定x，先对联合概率分布
> P(x,c)
> 建模，然后再由
> P(c|x) = \frac{P(x,c)}{P(x)}
> 获得
> P(c|x)
> 。

> 本文讲的朴素贝叶斯就是生成式模型，基于贝叶斯定理，
> P(c|x)
> 可写为

> P(c|x) = \frac{P(c)P(x|c)}{P(x)}

> 其中，P(c)是类”先验“概率；
> P(x|c)
> 是样本x相对于类标记c的类条件概率，或称为”似然“；
> P(x)
> 是用于归一化的”证据“因子。

> 那么在前面的举例中，通过贝叶斯定理我们可以由下面的公式转换求得：

> P(Sports|A\ very\ close\ game)  = \frac{P(Sports)  P(A\ very\ close\ game|Sports)}{P(A\ very\ close\ game)}

> P(Not\ Sports|A\ very\ close\ game)  = \frac{P(Not\ Sports)  P(A\ very\ close\ game|Not\ Sports)}{P(A\ very\ close\ game)}

> 通过这个转换我们就能计算概率了，我们只需要计算句子”A very close game“在类别Sports中出现的次数就能通过运算获得
> P(Sports|A\ very\ close\ game)
> 。

> 但是问题来了，句子”A very close game“并没有在训练集的类别中出现，那么概率就是0么？很明显”未被观测到“与”出现概率为0“通常是不同的。

> 朴素起来

> 极大似然估计

> 在讲朴素贝叶斯之前我们得提一下极大似然估计。我们前面说了，类条件概率
> P(x|c)
> 直接计数是不可取的，那么不能直接计数得到，那么我们就来”估算“得到。意思就是我们先假定类条件概率具有某种确定的概率分布形式，然后我们再基于训练样本对概率分布的参数进行估计。

> 事实上，概率模型的训练过程就是参数估计过程。对于参数估计，统计学界有两种不同的观点从而分成了两种学派：

> 频率主义学派：参数虽然未知，但却是客观存在的固定值，因此可以通过一些准则来确定参数值。

> 贝叶斯学派：参数是未观察到的随机变量，其本身也可有分布，因此可以先假定参数服从一个先验分布，然后基于观测到的数据来计算参数的后验分布。

> 我们这篇博文讲的就是频率主义学派的极大似然估计MLE，根据数据采样来估计参数的经典方法。

> 朴素贝叶斯

> 前面我们遇到的句子不在训练样本中出现的本质原因是类条件概率
> P(x|c)
> 是所有属性上的联合概率，难以从有限的训练样本中直接估计得到。在朴素贝叶斯中，有种假设：

> 我们假设特征之间相互独立，就好比上面的句子”A very close game“单词very出现的可能性与very和其他单词相邻没有关系。也就是句子”A very close game“、”very A close game“、”very close A game“是同一个句子！这个假设就是我们标题
> 朴素(naive)
> 的含义了。

> 因此我们有

> P(A very close game|Sports) = P(A|Sports)P(very|Sports)P(close|Sports)P(game|Sports)

> Note:朴素贝叶斯通常有两种实现方式：一种基于贝努利模型实现：该实现方式不考虑词在文档中出现的次数，只考虑出不出现（这种就是假设词是等权重的）；一种基于多项式模型实现：它考虑词在文档中出现的次数。这两种方法在准备数据时，分别有两种实现方式，一种是词条向量模型，一种是词袋模型

> 词集模型的python实现

> 从文本中构建词向量

> 首先我们先要知道下什么叫词条向量？一个词条是字符的任意组合，把每一个文本片段表示为一个词条向量，其中值为1表示词条出现在文档中，0表示词条未出现（这里假设每个词条都是同等重要的）。

> 从词向量中计算概率

> 伪代码如下：
![trainNB0](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 训练代码如下：

> [

](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> def
> trainNB0
> (trainMatrix,trainCategory)
> :
> numTrainDocs = len(trainMatrix)
    numWords = len(trainMatrix[
> 0
> ])
    pAbusive = sum(trainCategory)/float(numTrainDocs)
    p0Num = ones(numWords); p1Num = ones(numWords)
> \#change to ones()
> p0Denom =
> 2.0
> ; p1Denom =
> 2.0
> \#change to 2.0
> for
> i
> in
> range(numTrainDocs):
> if
> trainCategory[i] ==
> 1
> :
            p1Num += trainMatrix[i]
            p1Denom += sum(trainMatrix[i])
> else
> :
            p0Num += trainMatrix[i]
            p0Denom += sum(trainMatrix[i])
    p1Vect = log(p1Num/p1Denom)
> \#change to log()
> p0Vect = log(p0Num/p0Denom)
> \#change to log()
> return
> p0Vect,p1Vect,pAbusive
> [
](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 分类代码如下：

> [

](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> def
> classifyNB
> (vec2Classify, p0Vec, p1Vec, pClass1)
> :
> p1 = sum(vec2Classify * p1Vec) + log(pClass1)
> \#element-wise mult
> p0 = sum(vec2Classify * p0Vec) + log(
> 1.0
> - pClass1)
> if
> p1 > p0:
> return
> 1
> else
> :
> return
> 0
> [

](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 词袋模型的python实现

> [
](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 前面我们将每个词出现与否作为一个特征，也就是词集模型。如果一个词在文档中出现不止一次，这可能意味着包含该词是否出现在文档中所不能表达的某种信息，这种方法被称为词袋模型。在词袋中每个单词可以出现多次，而在词集中，每个单词只能出现一次。两者在代码实现上的唯一区别就是词袋模型每当遇到一个单词时，它会增加词向量中的对应值，而不是只将对应数值设为1。

> [

](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 实际应用–使用朴素贝叶斯来从CSDN博客中获取博客主文章特征分析

> [
](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 我们接下来将使用两位博主的文章来训练一个分类器，然后观察分类器的效果。我们的目的并不是使用该分类器进行分类，而是通过观察单词和条件概率值来发现与特定博主相关的内容。

> [

](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 收集数据：导入RSS源

> [
](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> CSDN (Chinese Software Developer Network) 创立于1999年，是中国最大的IT社区和服务平台，为中国的软件开发者和IT从业者提供知识传播、职业发展、软件开发等全生命周期服务，满足他们在职业发展中学习及共享知识和信息、建立职业发展社交圈、通过软件开发实现技术商业化等刚性需求。
> [博主OraYang的博客](http://blog.csdn.net/u010665216)
> 及
> [博主j_shine的博客](http://blog.csdn.net/J_Shine)
> 均支持RSS订阅。

> [

](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> import
> feedparser
ora = feedparser.parse(
> 'http://blog.csdn.net/u010665216/rss/list'
> )
lujie = feedparser.parse(
> 'http://blog.csdn.net/j_shine/rss/list'
> )
> [

](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 构造RSS源分类器

> [

](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> def
> localWords
> (feed1,feed0)
> :
> import
> feedparser
    docList=[]; classList = []; fullText =[]
    minLen = min(len(feed1[
> 'entries'
> ]),len(feed0[
> 'entries'
> ]))
> for
> i
> in
> range(minLen):
        wordList = textParse(feed1[
> 'entries'
> ][i][
> 'summary'
> ])
        docList.append(wordList)
        fullText.extend(wordList)
        classList.append(
> 1
> )
> \#NY is class 1
> wordList = textParse(feed0[
> 'entries'
> ][i][
> 'summary'
> ])
        docList.append(wordList)
        fullText.extend(wordList)
        classList.append(
> 0
> )
    vocabList = createVocabList(docList)
> \#create vocabulary
> top30Words = calcMostFreq(vocabList,fullText)
> \#remove top 30 words
> for
> pairW
> in
> top30Words:
> if
> pairW[
> 0
> ]
> in
> vocabList: vocabList.remove(pairW[
> 0
> ])
    trainingSet = list(range(
> 2
> *minLen)); testSet=[]
> \#create test set
> for
> i
> in
> range(
> 20
> ):
        randIndex = int(random.uniform(
> 0
> ,len(trainingSet)))
        testSet.append(trainingSet[randIndex])
> del
> (trainingSet[randIndex])  
    trainMat=[]; trainClasses = []
> for
> docIndex
> in
> trainingSet:
> \#train the classifier (get probs) trainNB0
> trainMat.append(bagOfWords2VecMN(vocabList, docList[docIndex]))
        trainClasses.append(classList[docIndex])
    p0V,p1V,pSpam = trainNB0(array(trainMat),array(trainClasses))
    errorCount =
> 0
> for
> docIndex
> in
> testSet:
> \#classify the remaining items
> wordVector = bagOfWords2VecMN(vocabList, docList[docIndex])
> if
> classifyNB(array(wordVector),p0V,p1V,pSpam) != classList[docIndex]:
            errorCount +=
> 1
> print(
> 'the error rate is: '
> ,float(errorCount)/len(testSet))
> return
> vocabList,p0V,p1V
> [

](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 显示最具表征性的单词

> [

](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> def
> getTopWords
> (ny,sf)
> :
> import
> operator
    vocabList,p0V,p1V=localWords(ny,sf)
    topNY=[]; topSF=[]
> for
> i
> in
> range(len(p0V)):
> if
> p0V[i] > -
> 6.5
> : topSF.append((vocabList[i],p0V[i]))
> if
> p1V[i] > -
> 6.5
> : topNY.append((vocabList[i],p1V[i]))
    sortedSF = sorted(topSF, key=
> lambda
> pair: pair[
> 1
> ], reverse=
> True
> )
    print(
> "Lujie***********************************************************************************************************Lujie"
> )
> for
> item
> in
> sortedSF:
        print(item[
> 0
> ])
    sortedNY = sorted(topNY, key=
> lambda
> pair: pair[
> 1
> ], reverse=
> True
> )
    print(
> "Ora*****************************************************************************************************************Ora"
> )
> for
> item
> in
> sortedNY:
        print(item[
> 0
> ])
> [

](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 实验结果显示

> [
](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![RSS](https://img-blog.csdn.net/20171016213825677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171016213825677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 从显示结果的单词中很容易看到两位博主的侧重点不同，J_shine博文偏向于java后台相关的内容，OraYang则偏向于python、机器学习、基础算法。

> [

](https://img-blog.csdn.net/20171016213825677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 总结

> [
](https://img-blog.csdn.net/20171016213825677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 本篇博文分享的贝叶斯概率及贝叶斯准则提供了一种利用已知值来估计未知概率的有效方法。文中提到的，条件独立性假设，可以降低对数据量的要求。在计算条件概率时还会遇到下溢出的问题，此时可以用取对数的方法解决。词袋模型比词集模型在解决文档分类的问题上效果更优。最后我们使用RSS源分析中，还可以通过移除停用词来进一步优化咱们的结果（代码中没实现），但让我感觉最重要的还是需要对读取RSS源并对文本进行切分进行大量优化，这是提高分类器效果的重要点。本文还有一个点没有提到，就是当算出的概率值为0时，一般采用“拉普拉斯修正”来对数据进行平滑处理（分子加1，分母加类别可能数）。

> [            ](https://img-blog.csdn.net/20171016213825677?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> [
](https://img-blog.csdn.net/20171016172835372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
