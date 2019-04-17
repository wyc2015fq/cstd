# PaperWeekly 第52期 | 更别致的词向量模型：Simpler GloVe - Part 1 - Paper weekly - CSDN博客





2017年11月22日 00:00:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：773









作者丨苏剑林

学校丨中山大学硕士生

研究方向丨NLP，神经网络

个人主页丨kexue.fm




# 前言




如果问我哪个是最方便、最好用的词向量模型，我觉得应该是 word2vec，但如果问我哪个是最漂亮的词向量模型，我不知道，我觉得各个模型总有一些不足的地方。且不说试验效果好不好（这不过是评测指标的问题），就单看理论也没有一个模型称得上漂亮的。




本文讨论了一些大家比较关心的词向量的问题，很多结论基本上都是实验发现的，缺乏合理的解释，包括：



- 
如果去构造一个词向量模型？




- 
为什么用余弦值来做近义词搜索？




- 
向量的内积又是什么含义？




- 
词向量的模长有什么特殊的含义？




- 
为什么词向量具有词类比性质？（国王-男人+女人=女王）




- 
得到词向量后怎么构建句向量？词向量求和作为简单的句向量的依据是什么？





这些讨论既有其针对性，也有它的一般性，有些解释也许可以直接迁移到对 glove 模型和 skip gram 模型的词向量性质的诠释中，读者可以自行尝试。 




围绕着这些问题的讨论，**本文提出了一个新的类似 glove 的词向量模型，这里称之为 simpler glove，**并基于斯坦福的 glove 源码进行修改，给出了本文的实现，具体代码在 Github
[1]上**。**




为什么要改进 glove？可以肯定的是 glove 的思想是很有启发性的，然而尽管它号称媲美甚至超越 word2vec，但它本身却是一个比较糟糕的模型（后面我们也会解释它为什么糟糕），因此就有了改进空间。




# 1. 从条件概率到互信息




目前，词向量模型的原理基本都是词的上下文的分布可以揭示这个词的语义，就好比“看看你跟什么样的人交往，就知道你是什么样的人”，所以词向量模型的核心就是对上下文的关系进行建模。




除了 glove 之外，几乎所有词向量模型都是在对条件概率
*P(w|context)* 进行建模，比如 Word2Vec 的 skip gram 模型就是对条件概率 *P(w2|w1)* 进行建模。




但这个量其实是有些缺点的，首先它是不对称的，即
*P(w2|w1) *不一定等于*P(w1|w2)*，这样我们在建模的时候，就要把上下文向量和目标向量区分开，它们不能在同一向量空间中；其次，它是有界的、归一化的量，这就意味着我们必须使用 softmax 等方法将它压缩归一，这造成了优化上的困难。 




事实上，在NLP的世界里，有一个更加对称的量比单纯的
*P(w2|w1)* 更为重要，那就是：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPVUYxMe0ibtTYZSwtofORvuYSTFrbvYuRFib74NneW259PEYlpR7X0yvQ/?)




这个量的大概意思是“两个词真实碰面的概率是它们随机相遇的概率的多少倍”，如果它远远大于 1，那么表明它们倾向于共同出现而不是随机组合的，当然如果它远远小于 1，那就意味着它们俩是刻意回避对方的。




这个量在 NLP 界是举足轻重的，我们暂且称它为“相关度“，当然，它的对数值更加出名，大名为点互信息（Pointwise Mutual Information，PMI）：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkP3U2XlxuYxib2zqLvGrb7yZjvGGiaicXtpfyTGxkjrpB4EOYVancDCEfUA/?)




有了上面的理论基础，我们认为，如果能直接对相关度进行建模，会比直接对条件概率* P(w2|w1)*建模更加合理，所以本文就围绕这个角度进行展开。在此之前，我们先进一步展示一下互信息本身的美妙性质。




# 1.1 互信息的可加性




相关度（等价地，互信息）在朴素假设下，有着非常漂亮的分解性质。所谓朴素假设，就是指特征之间是相互独立的，这样我们就有
*P(a,b)=P(a)P(b)*，也就是将联合概率进行分解，从而简化模型。




比如，考虑两个量 *Q,A* 之间的互信息，*Q,A* 不是单个特征，而是多个特征的组合：*Q=(q1,…,qk),A=(a1,…,al)*，现在考虑它们的相关度，即：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPjOmKELoebsTeOa170OgJhIsBcq2NGbHic01CSaUY9aQ14DeK9r3o6Pg/?)




用朴素假设就得到：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPUlicEg8PmdKCfjHYMKwxG757BMYc7eqMqCRPiaKTzWMvfqpS8qDso2MQ/?)




用贝叶斯公式，得到：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPctaZzZwZe4nOUUNDjO5PjJwgWUeUjUUBwHzhL6tC2MwickRHVIaBQxQ/?)




再用一次朴素假设，得到：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPQvtS2DfWp4IlSy9EnRGT47gXGmmvAibw8f31W8184G9ZhwpuUFLGmnw/?)




这表明，在朴素假设下，两个多元变量的相关度，等于它们两两单变量的相关度的乘积。如果两边取对数，那么结果就更加好看了，即：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPrPgUAOic7S4yD4jxG7UoVK6j3fnViaeA8am6ECQWTDSib1jNVcibbfBurQ/?)




也就是说，**两个多元变量之间的互信息，等于两两单变量之间的互信息之和，换句话说，互信息是可加的。**




# 1.2 插播：番外篇




为了让大家更直观地理解词向量建模的原理，现在让我们想象自己是语言界的“月老”，我们的目的是测定任意两个词之间的“缘分”，为每个词寻找最佳的另一半铺路。




所谓“有缘千里来相会，无缘见面不相识”，对于每个词来说，最佳的另一半肯定都是它的“有缘词”。怎样的两个词才算是“有缘”呢？那自然是“你的眼里有我，我的眼里也有你”了。




前面已经说了，skip gram 模型关心的是条件概率 *P(w2|w1)*，导致的结果是“*w1 *的眼里有 *w2*，*w2 *的眼里却未必有 *w1*”，也就是说，*w2* 更多的是词语界的“花花公子”，如“的”、“了”这些停用词，它们跟谁都能混在一起，但未必对谁都真心。




因此，为了“你中有我，我中有你”，就必须同时考虑* P(w2|w1)*和 *P(w1|w2)*，或者考虑一个更加对称的量——也就是前面说的“相关度”了。所以“月老”决定用相关度来定量描述两个词之间的“缘分”值。 




接下来，“月老”就开始工作了，开始逐一算词与词之间的“缘分”了。算着算着，他就发现严重的问题了。 




首先，数目太多了，算不完。要知道词语界可是有数万甚至数十万、将来还可能是数百万的词语，如果两两的缘分都算一次并记录下来，那将要一个数十亿乃至数万亿的表格，而且这工作量也不少，也许月老下岗了也还不能把它们都算完，但从负责任的角度，我们不能忽略任意两个词在一起的可能性。 




其次，词与词之间的 N 次邂逅，相对于漫漫历史长河，也不过是沧海一粟。两个词没有碰过面，真的就表明它们毫无缘分了吗？现在没有，可不代表将来没有。作为谨慎的月老，显然是不能这么武断下结论的。




**词与词之间的关系错综复杂，因此哪怕两个词没有碰过面，也不能一刀切，也得估算一下它们的缘分值。**




# 2. 几何词向量




上述“月老”之云虽说只是幻想，但所面临的问题却是真实的。按照传统 NLP 的手段，我们可以统计任意两个词的共现频率以及每个词自身的频率，然后去算它们的相关度，从而得到一个“相关度矩阵”。




然而正如前面所说，这个共现矩阵太庞大了，必须压缩降维，同时还要做数据平滑，给未出现的词对的相关度赋予一个合理的估值。




在已有的机器学习方案中，我们已经有一些对庞大的矩阵降维的经验了，比如 SVD 和 pLSA，SVD 是对任意矩阵的降维，而 pLSA 是对转移概率矩阵
*P(j|i) *的降维，两者的思想是类似的，都是将一个大矩阵 *A* 分解为两个小矩阵的乘积 *A≈BC*，其中 *B* 的行数等于 *A* 的行数，*C* 的列数等于* A* 的列数，而它们本身的大小则远小于 *A *
的大小。




如果对 *B,C* 不做约束，那么就是 SVD；如果对* B,C* 做正定归一化约束，那就是 pLSA。




但是如果是相关度矩阵，那么情况不大一样，它是正定的但不是归一的，我们需要为它设计一个新的压缩方案。借鉴矩阵分解的经验，我们可以设想把所有的词都放在
*n* 维空间中，也就是用 *n* 维空间中的一个向量来表示，并假设它们的相关度就是内积的某个函数（为什么是内积？因为矩阵乘法本身就是不断地做内积）。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPaibic0QgneQcxYr32w0tibZSHasMp559z5SzyLa0LUB5xrDhZVzOTBGrQ/?)




其中加粗的
*vi,vj *表示词* wi,wj *对应的词向量。从几何的角度看，我们就是把词语放置到了 *n* 维空间中，用空间中的点来表示一个词。




因为几何给我们的感觉是直观的，而语义给我们的感觉是复杂的，因此，理想情况下我们希望能够通过几何关系来反映语义关系。下面我们就根据我们所希望的几何特性，来确定待定的函数
*f*。




**事实上，glove 词向量的那篇论文中做过类似的事情，很有启发性**，但 glove 的推导实在是不怎么好看。请留意，**这里的观点是新颖的——从我们希望的性质，来确定我们的模型，而不是反过来有了模型再推导性质。**




# 2.1 机场-飞机+火车=火车站




词向量最为人津津乐道的特性之一就是它的**“词类比（word analogy）”**，比如那个经典的“国王-男人+女人=女王”（这项性质是不是词向量所必需的，是存在争议的，但这至少算是个加分项）。




然而中英文语境不同，在中文语料中这个例子是很难复现的，当然，这样的例子不少，没必要死抠“洋例子”，比如在中文语料中，就很容易发现有“机场-飞机+火车=火车站”，准确来说，是：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPh2QDVhLnjI8OClR9T6OTmrTMxDVJrMSVSVoepUUfdcqcEAey26UNEQ/?)




为什么词向量会具有这种特性呢？最近一篇文章《Skip-Gram – Zipf + Uniform = Vector Additivity》对这个现象做了理论分析，文章中基于一些比较强的假设，最后推导出了这个结果。现在我们要做的一件可能比较惊人的事情是：**把这个特性直接作为词向量模型的定义之一**。




具体来说，就是**词义的可加性直接体现为词向量的可加性，这个性质是词向量模型的定义**。我们是要从这个性质出发，**反过来把前一部分还没有确定下来的函数* f* 找出来**。这样一来，我们不仅为确定这个* f* 找到了合理的依据，还解释了词向量的线性运算特性——因为这根本是词向量模型的定义，而不是模型的推论。 




既然是线性运算，我们就可以移项得到“机场+火车=火车站+飞机”。现在我们来思考一下，单从语义角度来理解，这个等式究竟表达了什么？




文章开头已经提到，词向量模型的假设基本都是用上下文的分布来推导词义，既然“机场+火车=火车站+飞机”，那么很显然就是说，“机场”与“火车”它们的共同的上下文，跟“火车站”与“飞机”的共同的上下文，两者基本是一样的。




说白了，语义等价就相当于说**“如果两个人的择偶标准是很接近的，那么他们肯定也有很多共同点”**。到这里，*f*的形式就呼之欲出了。




# 2.2 模型的形式



因为词与词的相关程度用相关度来描述，所以如果“机场+火车=火车站+飞机”，那么我们会有： 




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPmpaSI3MNBlBW1ibkkib3ibFCZNac1O0OeFxfib9ic5ibtt3fRPB5VtdeNcag/?)




这里的
*w* 是上下文的任意一个词，由于我们不特别关心词序，只关心上下文本身的平均分布，因此，我们可以使用朴素假设来化简上式，那么根据式（6）得到：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPK5QrL60UgiaGVN2BZ7UN0BCg6m0bdGOTeyopETznibUb1pHDRw6bxiaog/?)




代入前面假设的式（8），得到：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPp0AX9CEzZKIWLnicSt57YhN82tIx9iajFHgiaPZ2iaZAIbjeIib6kvXkvXw/?)




这里 *Vw* 是任意的，因此上式等价于成立：





![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPIuUlKFq3013sIpOwUl2PuvOOtlLOBr7IC9qP7vJqTUJibsPmNia2BFyw/?)




加上连续性条件的话，那么上述方程的通解就是（求解过程在一般的数学分析书籍应该都可以找到）：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPub7rnTXjibqXUTVDwlRBU1fRwltPHdbTbcw7KBlNVl6cHHLxCO234pw/?)




也就是指数形式。现在我们就得到如下结果：为了让最后得到的词向量具有可加性，那么就需要对相关度用指数模型建模：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkPWrTcKEmaOTMAZUSjz4Fb9wWiaQaRkzcD1kKqnxibwQkXNBW3XfibOBicQA/?)




等价地，对互信息进行建模：




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgl9vsBlEN1VVRhvlxeMApkP3GLbIJ2bPcOmT8N0v815bvH88vqggOzAlpPTH20MciaErqrBACenVdw/?)




至此，我们完成了模型的形式推导，从形式上看类似对互信息矩阵的 SVD 分解。




# 2.3 忘记归一化




我们没有像通常的概率模型那样，除以一个归一化因子来完成概率的归一化。这样造成的后果是：对于本文的模型，当然也包括 glove 模型，我们不能讨论任意有关归一化的事情，不然会导致自相矛盾的结果。 




事实上，这是一种以空间换取时间的做法，因为我们**没有除以归一化因子来归一化，但又必须让结果接近归一化**，所以我们只能事先统计好所有的共现项的互信息并存好，这往往需要比较大的内存。




而这步骤换来的好处是，所有的共现项其实很有限（“词对”的数目总比句子的数目要少），因此当你有大规模的语料且内存足够多时，用 glove 模型往往比用 word2vec 的 skip gram 模型要快得多。 




此外，既然本文的模型跟 word2vec 的 skip gram 模型基本上就是相差了一个归一化因子，那么很显然，本文的一些推导过程能否直接迁移到 word2vec 的 skip gram 模型中，基本上取决于 skip gram 模型训练后它的归一化因子是否接近于 1。



# 3. 相关链接

[1] 代码：https://github.com/bojone/simpler_glove






我是彩蛋

** 解锁新功能：热门职位推荐！**




PaperWeekly小程序升级啦




**今日arXiv√猜你喜欢√**热门职位****√****




找全职找实习都不是问题



** 解锁方式 **

1. 识别下方二维码打开小程序

2. 用PaperWeekly社区账号进行登陆

3. 登陆后即可解锁所有功能




** 职位发布 **

请添加小助手微信（**pwbot01**）进行咨询




**长按识别二维码，使用小程序**

*点击阅读原文即可注册




![?](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgnwLopkg177jgoQCbq2j2UJqSZOScYnsaSZf7ibXORdFOUEicycYycARG6V9pvHMyY7jYpdZFKpxcSQ/?)










**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/?)

▽ 点击 | 阅读原文|
 加入社区




