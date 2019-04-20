# 注意力机制(Attention)最新综述论文及相关源码 - 人工智能学家 - CSDN博客
2018年11月17日 18:07:17[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：503
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVj7tZOx7Ugv82njfCGNza8UUXcAGScoSZFz12pFfQAiaRrQML2xpPzv7IG0u7yKkVFrt5r2SLiaCHg/640?wx_fmt=jpeg)
来源：专知
注意力机制(Attention)起源于模仿人类的思维方式，后被广泛应用于机器翻译、情感分类、自动摘要、自动问答等、依存分析等机器学习应用中。专知编辑整理了Arxiv上一篇关于注意力机制在NLP中应用的综述《An Introductory Survey on Attention Mechanisms in NLP Problems》，并提供一些相关的代码链接。
**简介**
在下图中，左边是传统的Seq2Seq模型（将序列编码，然后解码为序列），可以看出，这是一个传统的基于LSTM模型，在解码器Decoder中，某一时间戳的隐藏状态只依赖于当前时间戳的隐藏状态和上一时间戳的输出。右边是基于Attention的Seq2Seq模型，Decoder的输出还需要依赖于一个上下文特征(c)，这个上下文特征是通过Encoder中所有时间戳的隐藏状态的加权平均得到的，加权平均所使用的权值，就是当前时间戳和Encoder中每一时间戳的Attention Score(a)。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw1h2ArehAgSlDOj5ia1Jew7ykUq3PHdDB0RrQzxK30VxS0VXbdoN6Lg1FJ0QB4QiccuRtzjn1rNQOmQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw1h2ArehAgSlDOj5ia1Jew7y9Jlh0Q7KJXicprB1pXnNtqu8nN6ribNvnWIiab2jB7GgKic9SlHcUwDOOg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw1h2ArehAgSlDOj5ia1Jew7ypib8LrTiaMtK2xppgtczNYpVCWlFsNEem9wmuBkvmhA1cmYib0HqKUpdQ/640?wx_fmt=png)
**Attention的一般形式**
下面的公式是Attention的基础形式(Basic Attention)，其中u是基于当前任务的匹配特征向量，用于和上下文进行交互。vi是时序中某个时间戳的特征向量，ei是未归一化的Attention Score，ai是归一化的Attention Score，c是基于Attention Score和特征序列v计算出的当前时间戳的上下文特征。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw1h2ArehAgSlDOj5ia1Jew7yDicCEhricLCsFr4F5sZInt55MYRpFydOgGGw8FzltuHxVSf9lz0eqtXw/640?wx_fmt=png)
在大多数情况下，ei可以通过下面几种方法计算：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw1h2ArehAgSlDOj5ia1Jew7yjorFOzkLhzSmEAZsvqbsLN6ABCFvrxicm7yEmRl4GRWknuzRLbzNrNw/640?wx_fmt=png)
在实际应用中，除了基础的Attention，Attention还有多种变式，下面我们来介绍一些常见的变式：
**变式——Multi-dimensional Attention**
对于每个u，Basic Attention对于每个vi都会生成一个Attention Score ai，即每个u会对应一个1-D的Attention Score向量。而Multi-dimensional Attention会产生更高维度的Attention矩阵，旨在获取不同特征空间中的Attention特征，例如2D Attention的一些形式如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw1h2ArehAgSlDOj5ia1Jew7ylaS1VS2qsl0OsT3Frrb2haphPdic7iaRuk4gJFhdF6yn89icfyzgibSx1w/640?wx_fmt=png)
**变式——Hierarchical Attention**
一些Attention算法同时考虑不同语义层级之间的Attention，例如下面的模型先后利用词级别和句子级别的Attention来获得更好的特征：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw1h2ArehAgSlDOj5ia1Jew7yAhITdrL1EicSibZiblqmLYsezdbEcA4MC0pxwicL9ZSSlr5ILnn2zoTWTA/640?wx_fmt=png)
**变式——Self Attention**
将上面公式中的u替换为上下文序列中的vi，即为Self Attention。在NLP中Self Attention可以获取句子中词之间的一些依存关系。另外在一些任务中，一个词的语义与上下文密切相关，例如在下面两个句子中，bank分别指银行和河岸，要准确判断bank当前的语义，可以通过句子的上下文来判断。
I arrived at the bank after crossing the street. 
I arrived at the bank after crossing the river
**变式——Memory-based Attention**
Memory-based Attention的形式如下，其中{(ki, vi)}被称作Memory，这里Memory其实是输入的同义词。尤其，当ki和vi相等时，Memory-based Attention和Basic Attention是相同的。  
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw1h2ArehAgSlDOj5ia1Jew7yZv03Y61l1twtdWNNTibgP4FvsTsrwCUUW9PUPHjjMrXKCBSyxHGibHoQ/640?wx_fmt=png)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/AefvpgiaIPw1h2ArehAgSlDOj5ia1Jew7yLAUARWSoTaUAj96Z49OnNlZhnpnSWWsSGuN9XuGrLtEibu0ialvPTj1Q/640?wx_fmt=jpeg)
例如在QA问题中，Memory-based Attention可以通过迭代地更新Memory来将注意力转移到答案所在的位置。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw1h2ArehAgSlDOj5ia1Jew7ywZ3gWObVdMXCQomdwzsNpASD4C97lGiacp4PupCdz3zaXWWqkvQWwNg/640?wx_fmt=png)
**Attention的评价**
定量的Attention的评价可以通过intrinsic和extrinsic两种方式。Intrinsic基于标注的Alignment数据进行判断，因此需要大量的人工标注。Extrinsic方式比较简单，即直接比较模型在具体任务上的效果，但extrinsic评价方式的问题在于很难知道效果的提升是否是由Attention机制所带来的。
定量的评价一般通过可视化热力图来实现：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw1h2ArehAgSlDOj5ia1Jew7yaWbmlOYxCNIHvKqtibIqddCGm7aOicgWkoDNiccyO0gW4AgMibtX9PVibJw/640?wx_fmt=png)
**相关Attention代码**
- 
《Neural Machine Translation by Jointly Learning to Align and Translate》：https://github.com/tensorflow/nmt
- 
《Hierarchical Attention Networks for Document Classification》：https://github.com/richliao/textClassifier
- 
《Coupled Multi-Layer Attentions for Co-Extraction of Aspect and Opinion Terms》：https://github.com/happywwy/Coupled-Multi-layer-Attentions
- 
《Attention Is All You Need》：https://github.com/Kyubyong/transformer
- 
《End-To-End Memory Networks》：https://github.com/facebook/MemNN
**参考资料：**
《An Introductory Survey on Attention Mechanisms in NLP Problems》：https://arxiv.org/abs/1811.05544
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
