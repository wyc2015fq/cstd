# 【日本雅虎新闻推荐】：Embedding-based News Recommendation for Millions of Users(附开源代码) - 快来学习鸭～～～ - CSDN博客





2019年01月02日 20:35:10[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：332








本篇论文由日本雅虎团队发表于KDD 2017，利用深度学习方法将用户和文章进行embedding化，再进行推荐。

下载地址：[http://dacemirror.sci-hub.tw/proceedings-article/b79bf692bc190d28d255671a64aedf02/okura2017.pdf#view=FitH](http://dacemirror.sci-hub.tw/proceedings-article/b79bf692bc190d28d255671a64aedf02/okura2017.pdf#view=FitH)

代码：[https://github.com/Leavingseason/rnn_recsys](https://github.com/Leavingseason/rnn_recsys)

有个标准日本英语的介绍视频：[https://www.kdd.org/kdd2017/papers/view/embedding-based-news-recommendation-for-millions-of-users](https://www.kdd.org/kdd2017/papers/view/embedding-based-news-recommendation-for-millions-of-users)

## 摘要

    对于新闻推荐来说最重要的就是要把文章的内容和用户的喜好表达清楚，以及它们之间的关系。协同过滤和低秩分解方法等基于ID的算法并不是很适用于新闻推荐，因为这类算法很难做到实时更新。常用于信息检索中的word-based方法虽然在性能上较好，但是它们在处理同义和字的变体以及根据用户自身的历史行为定义每个用户的个性化query都存在一定的局限。所以本文提出了一个三层的基于embedding的分布式表示方法。1利用一个特殊的自编码器去为文章编码。2将历史信息输入RNN为用户进行编码 3.直接将编码的文章和用户按向量进行内积，为用户找到最合适的topN文章。

该方法已经利用日本雅虎首页推荐的数据进行离线评估，获得较好的效果。并在线上进行测试，相比于传统的方法，点击率提升了23%，用户停留时间延长了10%

## 介绍

    新闻类文章具有很强的时效性，所以利用传统的基于ID的方法，比如协同过滤等非实时的更新就不太合理。而新闻类的推荐需要解决的问题有三个：

1.理解文章的内容

2.理解用户的爱好

3.基于文章的内容和用户的喜好为每个用户选出他最喜欢的topN文章列表。

对于以上三个问题，作者给出了一个最基本的baseline方法。

就是把一篇文章看作是一系列词的集合；一个用户看作是该用户浏览过的所有文章中的词的集合；

而最简单的度量文章和用户的相关度的方法就是看文章中的词和组成用户的词的同现数。这样的话把所有的词建立其倒排索引，根据词的共现就可以很快的为用户找到匹配的文章。

### 这个方法存在的问题有几点：

1.不能区分同义词。比如“川普”和“特朗普”其实表达的就是同一个人，但是基于词的方法就无法对其进行区分，这就会导致在词频统计的时候，把明明是同一个意思的词分到不同的桶里，照成统计的误差。

2.用户的浏览历史其实是个时序相关的多个sessions，而不是简单的词的集合。新闻类的推荐更多的是在用户的及时兴趣上，如果过了这个兴趣的窗口期，就过了推荐的最佳时间。而剔除了时序的词集合完全不能考虑到这个特征；并且用户不同的浏览量都会照成不同长度的浏览历史，如何有效的对不同长度的浏览历史进行统一也是一个问题。

所以考虑到这些，作者提出了一个基于embedding的推荐策略，该策略分三步：

1.基于一种弱监督的降噪自编码器来对文章进行embedding向量化；

2.利用用户的历史浏览信息作为rnn的训练数据生成用户的embedding向量；

3.直接利用文章embedding向量和用户的embedding向量做内积，匹配出用户最可能感兴趣的topN文章列表，并对该列表做去重。

该方法的关键在于只需要使用内积来计算文章和用户的相关性，这样的话就可以把该算法工业化落地，因为内积的速度是可以满足真实场景下的实时推荐的。而推荐系统只需要提前计算好文章和用户的embedding向量。该方法可以说同时在效率和效果上都能满足工业化需求。

### 整体推荐流程

日本雅虎团队将该推荐策略应用在来日本雅虎的智能手机主页推荐业务上。整个流程分为五部分：

### **1.识别**

 获得预先根据用户历史数据计算好的用户特征

### **2.匹配**

使用用户特征从所有可用的文章中提取出候选文章集

### 3.排序

按照一定的排序规则算法对文章进行排序

### 4.去重

移除包含相同信息的文章

### 5.广告

做业务还是要挣钱的。

    而以上这一系列动作都需要在用户的请求和显示推荐的文章这之间的几百毫秒内全部完成，但是每天能推荐的文章是一直在变的，有成千上万的文章被删除，也有成千上万的文章加进来，所以必须得提前将所有的文章emebdding和用户embedding计算好。然后再针对每个用户去选取候选的文章，选取的方式就是计算用户embedding和文章embedding的内积，根据内积的大小设置预测来选取一定数量的候选文章集。

    有了候选文章集就可以继续利用其他的因素，来对文章进一步的精排序，比如文章的访问量，文章的新鲜度等等，这就需要进一步对文章，用户以及文章用户之间的上下文信息进行特征工程。

    这里需要处理的另外一个问题就是提取出的文章肯定会存在相似文章的情况，比如现在有两片文章分别问“特朗普大选”和“川普大选”其实是一样的文章，但是如果用前面提到的基于词的算法就无法对这两篇文章进行区分，但是基于embedding的话就可以对这两篇文章进行区分了。本文作者采用的是利用贪心策略对推荐出的文章集合进行去重。分别计算推荐出的文章embedding的余弦相似度，当排序后更加靠前的文章和后面的文章余弦相似度超过一定阈值后，就把后面的文章认为和前面的文章重复，把后面出现的文章从推荐列表中去掉。这一步对于大数据量的新闻推荐来说是很重要的，因为推荐的文章数据量一大，就会存在很大重复的文章，如果不对重复文章进行去重，那么一旦推荐列表里存在推荐概率较高的文章有很多是重复文章的情况，就会给用户推荐大量的重复文章，降低用户的满意度和体验度，因为用户看到了大量的一样的文章。还降低了文章的多样性，不能进一步探索用户的兴趣，无法很好的粘住用户。

    最后是广告这块，这块就不说了，工作就是为了挣钱，毕竟广告是流量变现最快最好的方式。

## 文章embedding的生成方法

作者利用降噪自编码器来生成embedding向量，但是自编码器只能将原始的输入进行编码，无法学习输入的文章是否是相同或者不同的文章，所以需要对降噪自编码器的损失函数做来一点修改，加入一点监督信息，让模型能学习到文章之间的差异和相似。

原始的降噪自编码器是这样的：

![](https://img-blog.csdnimg.cn/20190106190635796.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

原始输入x，经过损坏函数q(x),破坏原始的输入x，生成了被损坏的![\widetilde{x}](https://private.codecogs.com/gif.latex?%5Cwidetilde%7Bx%7D),然后进过全连接f(x)就可以隐层的表示h了，再通过同样的方式把h重建回原来的x；就是上图中的y。最终的损失函数就是要让重建的y和真实的输出x尽量一致。做q(x)的目的就是为了让学习出来的f(x)能更加的鲁棒。

作者对损失函数做了优化：

![](https://img-blog.csdnimg.cn/20190106191951560.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

和原始的降噪自编码器对一一下就能发现，改动主要在损失函数，但是这里整体的网络结构是个三元组,![(x_0,x_1,x_2)](https://private.codecogs.com/gif.latex?%28x_0%2Cx_1%2Cx_2%29)。其中![x_0](https://private.codecogs.com/gif.latex?x_0)和![x_1](https://private.codecogs.com/gif.latex?x_1)来自同一类目，![x_0](https://private.codecogs.com/gif.latex?x_0)和![x_2](https://private.codecogs.com/gif.latex?x_2)来自不同的类目。可以看出来主要的区别就是在损失函数中加入了对三个输出的隐层的度量，使得![h_0](https://private.codecogs.com/gif.latex?h_0)和![h_1](https://private.codecogs.com/gif.latex?h_1)的相似度能大于![h_0](https://private.codecogs.com/gif.latex?h_0)和![h_2](https://private.codecogs.com/gif.latex?h_2)的相似度。从![L_T](https://private.codecogs.com/gif.latex?L_T)这部分损失就可以看出来，log(1+exp(x))要想最小化就需要exp(x)尽量等于0，那么x就要趋近于负无穷。这里的x就是![h_0^Th_2-h_0^Th_1](https://private.codecogs.com/gif.latex?h_0%5ETh_2-h_0%5ETh_1),要让![h_0^Th_2-h_0^Th_1](https://private.codecogs.com/gif.latex?h_0%5ETh_2-h_0%5ETh_1)趋近于负无穷就是要让![h_0^Th_1](https://private.codecogs.com/gif.latex?h_0%5ETh_1)远大于![h_0^Th_2](https://private.codecogs.com/gif.latex?h_0%5ETh_2)。

最后模型生成的h就是输入x的emebdding向量。

## 用户embedding向量的生成方法

 先从前面提到过的基于词的模型讲其，再循环渐进。
**第一个是基于词的模型：**

    就是把一篇文章看作词的集合；

    一个用户看作他浏览的文章的词的集合；

    用户和文章的相关性就利用词的共现的线性函数表示；

文章a就是类似词袋模型一样是个稀疏向量，假设全部词汇的数量是v，文章表示向量a的维度就是就是v，但是和词袋模型不一样的是，这里有出现的是1，没有出现时0。没有数量的统计。

用户的一样，维度也是v，浏览的文章所有出现词，出现的词位置为1，没有出现的为0，向量的值只有0或者1。

相似度度量如下：

![](https://img-blog.csdnimg.cn/20190107231127982.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

这里![u_t](https://private.codecogs.com/gif.latex?u_t)就是当前t时刻的用户向量，a就是文章的向量，两者的维度都是词汇的总数v，那么上面的相似度度量R()，其实就是直接内积，只有两者都为1才能有值，否则就是0。就用该方法来计算相似度。

很明显这里的问题是：

1. 不管用户向量或者文章向量都很稀疏，必须两者有相同的词才会认为相关，但是如果一篇文章包含了相似词但是不是相同词就会被认为不相关，这很明显是不合理的，会影响文章召回的精度。

2. 还有一点是把文章和用户浏览历史都看成来集合，没有考虑到用户浏览的时间顺序，显然用户对最近浏览的文章最感兴趣，而且词汇出现的次数也能反应用户对不同词汇的喜好程度。所以作者们后提出来一个**衰减模型。**

### **衰减模型**

主要是对上面的基于词的模型做了两点优化。

1.就是把文章的表示不在使用词来表示，而是用基于自编码器的embedding来表示，这个方法就能解决相似词不能识别的问题；

2.就是为了考虑时序信息，对用户的浏览历史做了加权，加大来近期的权重，减少来较长时间前浏览的权重。

用户的表示向量可以被表示为

![](https://img-blog.csdnimg.cn/20190107232347430.png)

看着花里胡哨的，这里的![\alpha](https://private.codecogs.com/gif.latex?%5Calpha)是需要学习出来的和文章a同样维度的向量，中间那个同心圆就是基于元素位的点乘，后面的![\beta](https://private.codecogs.com/gif.latex?%5Cbeta)是衰减系数，如果![\beta](https://private.codecogs.com/gif.latex?%5Cbeta)为0其实就是正常的对向量的平均。

### 循环模型

虽然使用了衰减模型，但是模型衰减的程度是依靠超参数设置的指数衰减决定的，而且时序的相关性也不能很好的体现。所以最后作者们用上了RNN，利用RNN来按时序计算用户的embedding，而RNN的输入就是用户浏览文章的序列，文章以其自编码器生产的embedding向量作为输入。作者分别尝试来原始的RNN，LSTM，GRU三种RNN结构。原始RNN和LSTM都在一定程度上存在梯度爆炸和消失问题，只有基于GRU的RNN没有类似情况发生。

最后是实现结果，不用说，肯定提升比较明显，不然也不发论文了。   

![](https://img-blog.csdnimg.cn/2019010820015619.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

这里有一点：该模型上线其实相对比较困难，不能频繁更新模型，原因如下

 1.模型训练时间长，当前部署的模型在 GPU 上得训练一周时间；

 2. 一旦更新了计算文章embedding的DAE，那么就需要对这些文章的索引全部重新更新；

 3. 一旦文章的embedding变了，那么用户的embedding也得跟着变；

 4. 保存用户embedding的数据存储和保存文章embedding的搜索索引必须同步更新。

所以线上使用了两个分开的模型，方便模型的切换，并且经过数据分析，三个月后系统的性能下降到和基线一致，所以每三个月更新一次最佳。

### 总结

这算是把深度学习技术应用到实际线上的一次尝试把，感觉它使用自编码器这步操作还是挺6的，利用浏览历史来生产用户embedding是一个 很直观的方式，但是文章也说了，上线是比较麻烦的，感觉是不是会有更好的计算用户embedding的方式，比如用上tranformer来做，可不可能实际应用起来会方便点，这个也不敢确定。

还有一点就是给我的感觉这里的推荐其实就是基于序列的点击率预估，可以结合这篇paper《Session-based Recommendations with Recurrent Neural Networks 》看一下，或者直接看我的博客[https://blog.csdn.net/sxf1061926959/article/details/84332081](https://blog.csdn.net/sxf1061926959/article/details/84332081)

两片文章有异曲同工之妙，利用用户点击的session来预估下次点击的item，而雅虎这篇将输出的不是item而是直接输出预测的embedding，然后直接将该embedding和所有的文章进行比较相似度，这可以说是工业落地的一个优化吧。还有Session-based Recommendations with Recurrent Neural Networks 》文章的输入是直接输入的id，或者将id做embedding一起训练，但是这样就没有了类别信息，雅虎这篇单独利用自编码器硬生生把类别信息给加到embedding里，可以说是比较6的。我说小米你说6～～～～

完结。



