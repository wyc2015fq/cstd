# 读懂Word2Vec之Skip-Gram - Spark高级玩法 - CSDN博客
2017年12月08日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：9805
本教程将介绍Word2Vec的skip gram神经网络体系结构。我这篇文章的目的是跳过对Word2Vec的一般的介绍和抽象见解，并深入了解其细节。具体来说，我正在深入skipgram神经网络模型。
**模型介绍**
skip-gram神经网络模型其最基本的形式实际上是惊人的简单; Word2Vec使用了一个你可能在机器学习中看到过的技巧。我们将训练一个带有单个隐藏层的简单的神经网络来完成某个任务，但是实际上我们并没有将这个神经网络用于我们训练的任务。相反，目标实际上只是为了学习隐藏层的权重 - 我们会看到这些权重实际上是我们试图学习的“单词向量”。
另一个你可能已经看到这个技巧的地方是无监督的特征学习，在这里你训练一个自动编码器来压缩隐藏层中的输入向量，并在输出层将其解压缩回原始数据。训练完成后，您将剥离输出图层（解压缩步骤），然后使用隐藏层 -这是学习良好图像特征而不标记训练数据的一种技巧。
**假任务**
我们要训练神经网络做以下事情。给定一个句子中的特定单词（输入单词），查看附近的单词并随机选择一个单词。网络将告诉我们，我们词汇表中每个单词是我们选择的“邻近单词”的概率。
当说“附近”时，算法实际上有一个“window size”参数。一个典型的窗口大小可能是5，即5个字在前，5个字在后（共10个）。
输出概率将和我们查找到字典单词在我们输入词附近的可能性有关。例如，如果你给训练有素的网络输入单词“苏联”，输出是比如“联盟”和“俄罗斯”的概率将比“西瓜”和“袋鼠”这样无关的单词高得多。
我们将通过在训练文档中找到单词对来训练神经网络。下面的例子显示了一些训练样本（单词对），这些单词对将从“The quick brown fox jumps over the lazy dog.”获取。这里采用了一个小的窗口大小2,。标为高亮蓝色的是我们的输入单词。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWiahZWHibYABGr4QKSibMlgPbLLEkrUzrXLicwzIlIBJQvR5ZfZ4s8QfFJppKribsygg0DsraPh6a1vHA/?wx_fmt=png)
该神经网络会学习每一对单词出现频率的统计信息。例如，该神经网络获得的(““Soviet”, “Union”)训练样本数，远多于（“Soviet”, “Sasquatch”）。训练结束后，如果输入“苏联”一词作为输入，则输出““Union””或“Russia””的概率比“Sasquatch”的概率高得多。
**模型细节**
首先，要知道我们不可能直接将字符串输出给神经网络，所以需要对单词进行编码，以满足神经网络的输入要求。为了做这件事，我们可以使用训练文档集，去构建一个单词字典。比如，我们有10000个单词的字典。
我们将一个输入的单词表示成一个one-hot向量。这个向量有10000各元素(字典中的每个单词都是站一位，而形成一个向量)，我们将单词出现的位置设置为1，其它位置设置为0。
神经网络的输出是一个单独的向量，依然有10000元素，包含了对于词典中的每个单词，随机选中的单词是那个词典单词的概率。
下图是神经网络的架构
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWiahZWHibYABGr4QKSibMlgPbqLjUxMRGib0ibRVh0opo4HOaRfcr9fgMq3sLYL0XDvxMqUDyYsqNiabdg/?wx_fmt=png)
隐藏层没有激活函数，但是输出层使用的是softmax。一会儿再说这个问题。
当在单词对上训练这个网络时，输入是表示输入词的one-hot向量，训练输出也是表示输出词的one-hot向量。但当你使用输入词去评估训练完的网络的时候，输出向量实际上就是变成了一个概率分布（即一堆浮点值，而不是一个one-hot向量）。
**隐藏层**
下面讲到的例子，学习的是具有300个特征的单词向量。所以隐藏层将由一个10,000行（每一行都针对的是词汇表中的每个单词）和300列（每个隐藏的神经元一个列）的权重矩阵来表示。
300个特征是Google在Google新闻数据集上训练的发布模型中使用的特征。特征的数量是一个超参数，对于你自己的应用你需要有自己的调整(即尝试不同的值，看看什么产生最好的结果)。
如果你看这个权重矩阵的行，这些实际上是我们的词向量！
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWiahZWHibYABGr4QKSibMlgPbAV9zVibJNoyrsO3erYqjcsnCBZticGZ3xhMqTgPnibkEQ3OQ12bOUeP3w/?wx_fmt=png)
因此，所有这些的最终目标实际上只是为了学习这个隐藏的层权重矩阵 - 当我们完成时，我们就会抛弃输出层！
但是，让我们回过头来看看我们要训练的这个模型的定义。
这时候你可能会问自己-“one-hot向量基本都是0，将会有什么影响？”如果你使用1X10000的one-hot向量和一个10000X300的向量相乘。下面，用一个小的例子给大家呈现一下：
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWiahZWHibYABGr4QKSibMlgPbb2DCPIW1uTVyNfMxicJCC79HGPwrB4qwsIGVYrDiaL3ic9lCxLPCSJtbg/?wx_fmt=png)
这意味着这个模型的隐藏层实际上只是作为一个查找表来操作，隐藏层的输出只是输入单词的“word vector”。
**输出层**
然后1 x 300，“ants”这个单词向量被送到输出层。输出层是一个softmax分类器。要点是，每个输出的神经元(一个在字典中的单词向量)将会产生一个输出值，该值在0-1之间，并且所有的输出值求和之后是1.
具体而言，每个输出神经元都有一个权向量，它与隐层的单词向量相乘，然后将该函数exp(x)应用于结果。最后，为了得到输出总和为1，我们将这个结果除以所有 10,000个输出节点的结果之和。
下面是计算单词“car”的输出神经元输出的图示。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWiahZWHibYABGr4QKSibMlgPbSjF44MGd2TCvBWFq2c6s4iaYbEYQsyQ6gS6I4JU9EAtAwgciayrtA7BA/?wx_fmt=png)
请注意，神经网络不知道输出词相对于输入词的偏移量。对于输入词之前，之后的单词，他不会学习不同的概率集合。为了理解这个含义，假设在我们的训练语料库中，每个单词“York”的前面都有“New”这个词。也就意味着，至少在测试集中，New在York附近的概率将会是100%。然而，如果你取出York附近的十个单词，然后随机的从他们中间取出一个，是New的可能性并不是100%；你可能会在其附近获得了一个其他单词。
**直觉**
好的，你准备好了解这个网络的一个令人兴奋的一点？
**如果两个不同的单词具有非常相似的“上下文”（也就是说，他们周围可能出现哪些单词），那么我们的模型需要为这两个单词输出非常相似的结果。如果两个单词向量相似，那么网路将会为这个两个单词输出相似的上下文。假设两个单词有相同的上下文，那么我们的网络将会学习相似的单词向量。**
那么两个单词有相似的上下文是什么意思呢？我认为你可能人为同义词，如“intelligent” 和“smart”，将会有相似的上下文。或者，像“engine”和“transmission”这样相关的词语也可能具有相似的上下文。
**引入的问题**
你可能已经注意到了，skip-gram神经网络包含一个巨大的权重矩阵。例如，300个特征，10000个单词字典，那么输出层和隐藏层都将有3M个权重。在一个大的数据集上进行训练是很难的，所以word2vec作者引入了一些调整来使训练变得可行。
**优化**
在我给出的例子中，我们有300个组件的单词向量和10,000个单词的词汇表。回想一下，神经网络有两个权重矩阵 - 隐藏层和输出层。这两个层都有一个权重矩阵，每个权重矩阵为300 x 10,000 = 300万！
在大型的神经网络上运行梯度下降会很慢。更糟糕的是，你需要大量的训练数据来调整许多权重，避免过度拟合。数以百万计的训练样本数百亿次，意味着训练这个模型将成为一个野兽。
Word2Vec的作者在他们的第二个解决这些问题的文章。
第二篇论文有三个创新：
1，在他们的模型中将常见单词对或短语视为单个“单词”。
2，对频繁的词进行抽样以减少训练样例的数量。
3，使用他们所谓的“负抽样”技术来修改优化目标，使得每个训练样本只更新模型权重的一小部分。
值得注意的是，对频繁词进行二次抽样和应用负抽样不仅减少了训练过程的计算负担，而且也提高了它们产生的词向量的质量。
**词对和短语**
作者指出，像“波士顿环球报”（一家报纸）这样的单词对，与“波士顿”和“环球”这两个单词的含义有很大不同。因此，无论在文本中出现在什么地方都将“波士顿环球报”都视为具有自己的单词向量表示的单个单词，是有道理的。
您可以在他们发布的模型中看到结果，该模型已从Google新闻数据集中获得1000亿字的训练。模型中短语使词汇量增加到300万！
我认为他们的短语检测方法不是他们论文的一个重要贡献，但是我会分享一些，因为它非常简单。
他们的工具每一个通过只看两个单词的组合，但你可以运行多次，以获得更长的短语。他们的工具每一个通过只看两个单词的组合，但你可以运行多次，以获得更长的短语。因此，第一遍会选择“New_York”这个短语，然后再次运行会将“New_York_City”视为“New_York”和“City”的组合。
该工具会统计两个单词组合在文本中出现的次数，然后将这些计数用于方程中以确定将哪些单词组合成短语。这个方程设计的目的是将单词组合成短语，原因是单词组合出现相对于单个出现的的频率更高。他也关心由少见的单词组成的短语，避免一些常见的词组成短语，比如”and the”,”this is ”。
**抽样频繁词语**
在一个比较大的样本集合中，最频繁的词将会以数以百万计的频率出现，比如：in，the，a。相对于罕见的词来讲，这些单词往往带有很少的信息量。例如，skip-gram从观察”france”和”paris”组合出现收益程度，远高于从观察”france”和“the”组合出现收益的程度，因为在一个句子中机会每个单词都有和the组合出现。该观点也可以死 被应用与相反的方向。频繁词的向量表征在训练了数百万个例子后不会发生显著变化。
****频繁词语**抽样率的确定**
word2vec C代码实现了一个计算词汇中给定单词的概率的公式。
wi代表单词，z(wi)代表wi单词在预料库中所有单词出现次数的占比。例如，如果”peanut”出现了1000次，语料库中有10亿，那么z(“peanut”)= 1E-6。
代码中还有一个参数，叫做”sample”，它控制着抽样的比例，默认值是0.001。采样率假如过小，那么很多低频的词可能无法保留下来。
P(wi)是保留该词的概率。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWiahZWHibYABGr4QKSibMlgPbrTlXl1T2hddY5Xhn7BTwROzGLbm7Liay6ZyPIHemicsoC1l7kKWf41AA/?wx_fmt=png)
可以将它画出来，方便大家查看其形状
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWiahZWHibYABGr4QKSibMlgPb9H2cIo0gvS7279WibsjmNIDyszZ8uKNYdfwFOx8wiajiacJsIJuWKxwDg/?wx_fmt=png)
没有一个单词应该是语料库的一个很大的比例，所以我们想要在X轴上看看非常小的值。
在这个函数里有一些有趣的点，使用默认的样本值0.001。
P(wi)=1.0 (将100%可能被保留)当z(wi)<= 0.0026。超过总单词0.26%的单词会被采样到。
P(wi)=0.5 有1/2的概率被采样到，z(wi) = 0.00746
P(wi)=0.033 3.3%机会被采样到，z(wi) = 1.0
也就是说，如果这个语料库完全是由wi这个词组成的，那当然是荒谬的。
您可能会注意到，这篇论文定义的这个函数与C代码中实现的稍有不同，但是我认为C实现是更权威的版本。
**负采样**
训练一个神经网络意味着取一个训练本并稍微调整所有的神经元权重，以便更准确地预测训练样本。换句话说，每个训练样本将调整神经网络的所有的权重。
正如我们上面所讨论的，单词词汇的大小意味着我们的skip-gram神经网络拥有大量的权重，所有这些都将被我们数十亿训练样本中的每一个进行微更新！
负采样通过让每个训练样本只修改一小部分权重而不是全部权重来解决这个问题。这是如何工作的。
当在单词对(“fox”, “quick”)上训练网络时，记住网络的“标签”或“正确输出”是one-hot向量。也就是说，对于对应于“quick”的输出神经元输出1，并且对于所有其他数千个输出神经元输出0。
在负抽样的情况下，我们将随机选择少量的“negative”单词（比如5）来更新权重。（在这种情况下，一个“negative”字是我们希望网络输出0的字）。我们还将更新我们的“正面”单词（在我们当前的例子中是“quick”）的权重。
**该论文指出，选择5-20个单词适用于较小的数据集，并且对于大型数据集只能使用2-5个单词。**
回想一下，我们模型的输出层有一个300 x 10,000的权重矩阵。所以我们只是更新我们的positive词（“quick”）的权重，加上我们要输出0的其他5个词的权重。这总共有6个输出神经元，总共有1,800个权重值。这只是输出层3M重量的**0.06％**！
在隐藏层中，只有输入词的权重才会更新（无论您是否使用负采样，这都是正确的）。
**选择负样本**
“负样本”（也就是我们要输出为0的5个字）是用“unigram distribution”来选择的。
本质上，选择一个词作为负样本的概率与其频率有关，越频繁的词更有可能被选作负样本。
在word2vec的c代码中，可以找到该概率的方程。每个单词被赋予一个等于它的频率（单词数）提升到3/4次方的权重。选择一个单词的概率只是它的权重除以所有单词的权重之和。
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWiahZWHibYABGr4QKSibMlgPbwOXNrumXI8lBjyTXWI5wr8LTrz5Cm1xGlV8icrEfiad2Yc7DdGfPHdlA/?wx_fmt=png)
把频率提高到3/4的决定似乎是经验的;你可以看看函数的形状-只需要在google中输入:” plot y = x^(3/4) and y = x” 然后放大范围x = [0,1]。
这个选择在C代码中实现的方式很有趣。他们有一个100M的元素（它们称为单字表）大阵列。他们用词汇表中的每个单词的索引多次填充这个表格，单词索引出现在表格中的次数由P(wi) * table_size给出。
然后，为了选出一个负样本，你仅仅只需要产生一个随机整数，整数范围是0-100M，然后使用当前索引在表中指定的word。由于高概率的单词在表中出现的次数多，你有更高的概率选择到这些。
**层级Softmax**
针对softmax的大幅度提高计算效率的版本就是层级softmax。在神经网络语言模型的背景下，Morin和Bengio首次提出。主要优点是不需要评估神经网络中的W输出节点来获得概率分布，只需要估计log2（W）节点。
层级softmax使用二叉树代表输出层，使用W(单词矩阵)单词代表叶子。针对每个节点，明确的指出了其子节点的概率。这些定义了将概率赋予单词的随机游走。
**更确切地说，每个单词w可以通过从树根开始的适当路径到达。使用n(w, j)代表从根节点到单词w路径中的第j个节点，并且使用L(w)代表路径的长度。所以，n(w, 1) = root同时n(w,L(w)) = w。另外，对于任何内部节点n，使用ch(n)作为任意的固定子节点。[[x]]为1，如果x是true，假如x是假，当然为-1。然后，层级softmax定义输入单词w1和输出单词w2关系为：**
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWiahZWHibYABGr4QKSibMlgPbHYBMiaSSt1B6vriabv9kbquaymCPBWqzM8KkaP1C4HQiaR3LTjZ46Gl6Q/?wx_fmt=png)
其中：![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWiahZWHibYABGr4QKSibMlgPbofb2Aicic1Z60S9REibEp8lq2Yee6LQ63t17dH0KreK1mEhTFYIajYWTg/?wx_fmt=png)
![?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWiahZWHibYABGr4QKSibMlgPbQ3pZCico5DUiaSHWWMSnvzpyjyLYFtjRZLHHW0VhIWjfMy6TBEGMQbGw/?wx_fmt=png)这个等式可以证明是正确的。这意味着计算成本和正比于L(), 它平均不大于logW。不同于标准的softmax公式(标准的softmax公式会为每个单词分配两个表征和)，层级softmax公式针对每个单词有一个表征，针对每个二叉树内部的节点n有一个表征.
层级softmax使用的树的结构对性能有相当大的影响。Mnih和Hinton探索了构建树结构的一些方法，以及对训练时间和模型精度的影响。在我们的工作中，**我们使用二元霍夫曼树，因为它将短代码分配给频繁词汇可使得训练速度加快。**之前已经观察到，通过它们的频率将词组合在一起作为基于神经网络的语言模型的非常简单的加速技术是很好的
**推荐阅读：**
1，[CountVectorizer](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484313&idx=1&sn=939b59d6ae32963d4418390db988dc85&chksm=9f38e0b1a84f69a773fe140c21eaae240fd3f398d780d8afe79212c3f89701c66e85b3fa34d5&scene=21#wechat_redirect)
2，[基于java的中文分词工具ANSJ](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484304&idx=1&sn=2c8280e8ec0c008074d2cf958c7d7489&chksm=9f38e0b8a84f69aed49600507e469f42c6697b77ab2edfdf3ef8904f46990d10c77932845d0a&scene=21#wechat_redirect)
3，[案例：Spark基于用户的协同过滤算法](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484291&idx=1&sn=4599b4e31c2190e363aa379a92794ace&chksm=9f38e0aba84f69bd5b78b48e31b3f5b3792ec40e2d25fdbe6bc735f9c98ceb4584462b08e439&scene=21#wechat_redirect)
4，[Hive : SORT BY vs ORDER BY vs DISTRIBUTE BY vs CLUSTER BY](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484236&idx=1&sn=ee9622d02aa4d6888650014d139e0793&chksm=9f38e064a84f697293ecc7bcd2773c990e26f014dda4d095ff17ec48b35849f76fe7107ba9b2&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

