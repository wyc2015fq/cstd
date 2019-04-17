# word2vec - 蓁蓁尔的博客 - CSDN博客





2017年07月02日 16:37:19[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：3676








没太有什么公式，从实践角度来看的，主要是skip gram 模型。 

一。 

1. 首先搞明白为什么要用低维实值向量表示word，而不使用one-hot来表示。 

“one-hot”：将 word 转化为含有一个1，其他都是0的稀疏向量，向量的维度等于词表大小，只有对应位置上的数字为1，其他都为0。 

（1）one-hot太稀疏，维度太高之后不易处理； 

（2）最主要的是 one-hot 捕捉不到任何的上下文（context）信息。特别是在自然语言处理任务中，这是一个大缺陷。
2.word2vec方法 

基于上述 one-hot 方法的两个缺点，Word2vec需要做到两个目标： 

（1）word embedding：将每一个单词映射为低维向量，可以自己设置。例如在Python的gensim包中封装的Word2Vec接口默认的词向量大小为100，一般取100-300之间。 

（2）让（1）中生成的词向量包含上下文信息，两种方法：
```
法一：输入中间单词，输出其周围词（context）出现的可能性，称为Skip Gram 模型
      法二：与上相反，输入周围词，输出为中间次出现的可能性，称为Continuous Bag Of Words(CBOW)。
      下面主要讨论第一种方法，Skip gram。
```

【补充：】gram的理解：A gram is a group of n （continuous ）words, where n is the gram window size. 例如：这个句子“The cat sat on the mat”的 3-gram representation 是 “The cat sat”, “cat sat on”, “sat on the”, “on the mat”.

3.下图是skip gram的模型架构—一个简单三层神经网络。 
![这里写图片描述](https://img-blog.csdn.net/20170724165449049?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

设词汇表中共有10000个不同的单词，那么模型的输入为一个10000维的 one hot 的向量，隐层没有使用任何激活函数，只是简单的进行了相加的操作，输出层使用了sotfmax，也就是输出也是一个10000维度（词汇表的大小）的向量，它包含了10000个概率，每一个概率代表着当前词是输入样本中output word的概率大小。以 The cat sat on the mat 句子为例，
> 
**设输入单词**：cat  
**设窗口大小**：3（在Python的gensim包中封装的Word2Vec接口默认的window_size为5） 
**那么形成的训练对有**（用于解释原理，没去停止词，一般来说预处理时会去停止词和去掉在语料库中出现频率小于5的单词（太低频，噪声））：  

  （cat的onehot编码，the 的onehot编码）  

  （cat 的onehot编码，on 的onehot编码）  

  （cat 的onehot编码，the 的onehot编码） 
**在一个训练语料中，针对不同的输入单词可以形成很多很多训练对。**
4.上面3步骤中，训练样本和模型都确定了以后，接下来的目标是学习隐层的输出，也就是输入的单词对应的词向量（word embedding）。设输入为X（1*10000列向量），输出层和隐层之间的权重矩阵为W（10000*300），隐层输出为H（1*300），则： 
![这里写图片描述](https://img-blog.csdn.net/20170724182120812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

为了有效地进行计算，这种稀疏状态下不会进行矩阵乘法计算，可以看到矩阵的计算的结果实际上是矩阵对应的向量中值为1的索引。这样，模型中的隐层权重矩阵便成了一个”查找表“（lookup table），进行矩阵计算时，直接去查输入向量中取值为1的维度下对应的那些权重值。这些权重值组成的向量也就是隐层的输出也就是每个输入单词的“嵌入词向量”。
二。 

1. 思考：虽然上面的方法可以得到 word embedding，但是假设词表中有10000个词，每个词向量是300维，那么输入和隐层之间以及隐层和输出之间的权重值都是有3000000个，参数太多了，训练起来相当慢，并且需要大量的训练样本来避免过拟合。

2.所以，在Word2Vec的作者在它的第二篇论文中强调了这些问题，下面是作者在第二篇论文中的三个创新：

> 
（1）将常见的单词组合（word pairs）或者词组作为单个“words”来处理。  

  （2）对高频次单词进行抽样来减少训练样本的个数。 

  （3）对优化目标采用“negative sampling”方法，这样每个训练样本的训练只会更新一小部分的模型权重，从而降低计算负担。事实证明，对常用词抽样并且对优化目标采用“negative sampling”不仅降低了训练过程中的计算负担，还提高了训练的词向量的质量。 （有点类似与通信中加噪声提高鲁棒性）
（2）对高频次单词进行抽样来减少训练样本的个数：
- 为什么要去掉高频词？ 

一个是因为像是 a，the 等高频词在很多很多word的上下文中都有出现，提供的语义信息不多；在一个是因为这些高频词在训练数据中出现的频率太高，对应的训练样本对太多太多了，远大于所需要的。
- 如何抽样？ 
![这里写图片描述](https://img-blog.csdn.net/20170724183653959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
某单词wi被保留下来的概率为： 
![这里写图片描述](https://img-blog.csdn.net/20170724183733386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在gensim中设置的是0.001. 

从上图我们可以看出：- 当Z(wi)<=0.0026时，P(wi)=1.0。当单词在语料中出现的频率小于0.0026时，100%被保留的，也就是在语料中出现频率超过0.26%的单词才会被采样。
- 当Z(wi)=0.00746时，P(wi)=0.5，意味着这一部分的单词有50%的概率被保留。
- 当Z(wi)=1.0时，P(wi)=0.033，意味着这部分单词以3.3%的概率被保留。

（3）负采样（negative sampling）–每次让一个训练样本只更新一小部分权重，上面一。中针对每一个训练样本，所有的权重都要进行调整。
- 例如：用训练样本 ( input word: “fox”，output word: “quick”) 来训练时，“ fox”和“quick”都是经过one-hot编码的。如果词表为10000维时，在输出层，我们期望对应“quick”单词的那个神经元结点输出1，其余9999个都应该输出0。在这里，这9999个我们期望输出为0的神经元结点所对应的单词我们称为“negative” word。
- 当使用负采样时，我们将随机选择一小部分的negative words（比如选5个negative words）加上“positive” word来更新对应的权重（在论文中，作者指出指出对于小规模数据集，选择5-20个negative words会比较好，对于大规模数据集可以仅选择2-5个negative words）。
- 一个单词被选作negative sample的概率跟它出现的频次有关，出现频次越高的单词越容易被选作negative words。在word2vec的C语言实现中，每个单词被选为“negative words”的概率计算公式为：

![这里写图片描述](https://img-blog.csdn.net/20170724185322850?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzUyNzQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

三。word2vec 的两个模型分别都对应两种加速计算和提升训练的词向量质量的策略，总共4种。整个上面的一。+二。部分可以看做是skip gram模型+negative sampling策略。下面介绍skip gram模型+hierarchical softmax策略。 

  该策略将上面的模型中第三层换成霍夫曼树结构。霍夫曼编码，通信中非等长编码的一种，频率越高编码长度越短，提高效率。可以参考：[http://blog.csdn.net/itplus/article/details/37969635](http://blog.csdn.net/itplus/article/details/37969635) 中霍夫曼树的构造部分内容。

参考： 

（1）[Word2Vec word embedding tutorial in Python and TensorFlow](http://adventuresinmachinelearning.com/word2vec-tutorial-tensorflow/)

（2）[理解 Word2Vec 之 Skip-Gram 模型](https://zhuanlan.zhihu.com/p/27234078)

（3）博客系列：[http://blog.csdn.net/weixin_36541072/article/details/53065022](http://blog.csdn.net/weixin_36541072/article/details/53065022)

（4） [word2vec 中的数学原理详解](http://blog.csdn.net/itplus/article/details/37969519)













