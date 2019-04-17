# 【Transformer模型】：Attention is all your need(附attention的代码) - 快来学习鸭～～～ - CSDN博客





2018年12月12日 21:41:29[DivinerShi](https://me.csdn.net/sxf1061926959)阅读数：364








attention已经火了好长一段时间了，一直只是只闻其名不知其意，特地看了attention is all your need。

    这篇论文摒弃了传统encoder-decoder模型必须结合cnn或者rnn的固有模式，只用了attention模型，可谓大道至简。现在主流的序列转化模型都是基于端到端的encoder-decoder的RNN或者CNN网络结构模式。现在都是在该框架中引入了attention机制来进一步提高模型的性能。而本文提出了一个新的简单网络，Transformer。它只使用了attention机制，完全摒弃了RNN和CNN。实验结果表明在明显降低训练时间消耗的同时模型也获得了较为显著的效果。

## 介绍

    RNN，LSTM，GRU现在已经是语言模型和语言翻译等序列建模中最好的模型基础了。并且已经有大量学者对循环网络和encoder-decoder网络结构做了改进和优化。循环网络的计算考虑了输入输出序列的位置信息。在每个计算的time step中考虑序列的一个位置，将每个位置上的元素按顺序输入网络就可以获得当前time step下序列的隐状态h，而该隐状态是根据当前time step输入的位置信息和上个time step获得的隐状态共同获取得到。但是序列本身的内在序列连续性，限制了模型训练的并行性，虽然有很多很多相关工作对其进行改进（比如多序列并行等），但是其本质问题依然存在。

Attention已经成为序列模型的一个重要部分，它主要是为了解决模型中序列长距离依赖的问题。但是现在主流的做法基本上还是将attention模型和循环网络配合来使用。

本文提出了transformer模型，该模型摒弃了循环的模式，而只完全依赖attention机制来刻画输入输出序列中的全局依赖信息。

## 背景



    现在已有一些序列处理方法ENGPU，BN，CS2S。通过使用CNN对全部输入和输出作为基础模块来达到减少序列数据计算量，可以达到并行计算隐层表示。但是这些方法对输入序列长度和位置关系都是呈线性和对数增长。而对距离的依赖依然是一个没有克服的难点。在Transformer中，我们可以把这些操作都控制在常数范围内，虽然我们用了平均attention位置方法降低了性能，但是我们利用 multi-head attention对其进行了补充。

###  self-attention



    Self-Attention 是attention机制中的一种，通过关联单个序列中的不同位置来计算序列的表示。Self-Attention已经被成功应用在不同的任务中，比如阅读理解，摘要抽取、文本续写。端到端记忆网络是基于循环attention机制，而不是在循环中完成序列对齐。这种框架已经在问题回答，语言模型等中十分常见。

本文提出的Transformer模型，是第一个只依靠Self-Attention计算输入输出序列的转换模型，并没有结合RNN或者CNN。

## 模型结构



    常见的神经序列模型，都是基于Encoder-Decoder框架,本文的框架也不例外。Encoder阶段是将符号表示的输入序列（x1,...,xn）转化为连续表示的序列z=(z1,....zn).给定z，Decoder部分就可以按次输出新的符号序列(y1,...,ym)。模型每一步都是auto-regressive,通过结合前一次生成的符号来生成下一次的输出。

Transformer沿用了Encoder-Decoder框架，但是利用Stacked Self-Attention 和基于点的全连接层替代了CNN和RNN的编码和解码过程。

Transformer 沿袭了encoder-decoder的结构，并在该结构下使用的不再是cnn或者rnn，而是self-attention和基于点的全连接层。将其应用在了encoder和decoder层中。

![](https://img-blog.csdnimg.cn/20181228200137423.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)

如上图，就是一个单个的transfomer结构。具体的结构：

**Encoder：**encoder是由6层独立的layers构成。每层由两个子层，就是上图左边的两个。第一个是一个多头 self-attention结构。第二个是一个简单的基于点的全连接前向网络。并且每个子层都借鉴了残差网络的思想使用了一个残差连接来连接每个子层的输出。并且对输出的结构层上做了LayerNorm。针对一个子层的输出就是LayerNorm(x+Sublayer(x))，其中的Sublayer(x)就是子层结构。为了能使用上残差，所有的输出维度都统一成了一样的维度：512.

**Decoder:** decoder也是由6层的独立层组成。但是和encoder由一点差异，就是decoder不同于encoder两个子层，它用了三个子层。用于对encoder的输出做multi-head self attention。具体结构都一样，也是在子层上应用了残差连接，再在子层输出加上一个LayerNorm。但是论文在decoder中的multi-head self attention 做了一个mask修改。目的是为了防止序列之后的位置被用于预测前面的位置信息，造成对模型的信息泄漏。

### Attention

接下来解释一下到底什么是Attention。attention可以被描述为将一个query，和一对key-value的键值对映射到输出。什么意思呢？可以这样理解，现在由一个query，比如说是“苹果好吃”，

然后key-value对分别是：

key1:苹果好吃吗？value1:好吃

key2:栗子好吃吗？value2:好吃

key3:苹果是什么？ value3:苹果是吃的

当然一般情况下这些query，key，value都是向量，我这里只是打个比喻。然后我们有了一个query：”苹果好吃“，我们就可以根据这个query一一比对key，如果不管query内容，那么query和每个key的对应关系应该都是等价的。但是很明显，这里的query和key1是最相似的，所以我们更关注的是key1的结果，那么我们其实就更在意value1，所以，我们通过query对应上了value1。这个就是attention的主要结构。具体文章中介绍了两种：

### Scaled Dot-Product Attention 基于点乘的attention

![](https://img-blog.csdnimg.cn/20181228202243614.png)

论文针对基于点乘的attenion做了一个优化，叫做scaled dot-product attention。

输入是由dk维的queries和keys，dv维的values组成。然后计算按上面的解释的思路，得去计算query“苹果好吃”和全部keys的相似度，这里的计算方式就是计算他们之间的点乘，然后再除以sqrt(dk),来计算query和key之间的相似度，最后用一个softmax来归一化相似度，作为values。然后query一般不只一个，所以把全部的query向量合并起来就是一个query矩阵，把它定义为Q，同理，所有的keys和values就是K和V，就可以利用矩阵计算来得到结果：

Attention（Q,K,V）=softmax(QK^T/sqrt(dk))V

其实这里就是对常见的点乘attention除以了sqrt(dk)，还有一个常用的attention是Additive attention(加法attention)，加法attention的话就是在queries和keys上加上了一个单层的神经网络，其实就是一个权值，然后将经过映射的query和key加起来来比较相似度。相比的话，肯定是点乘的attention更快了，直接计算矩阵就可以了。当然如果dk的值相对比较小的话，两个attention的效果差不多，但是如果dk比较大的话，加法attention可能还会比点乘的好点。这个可能是点乘要把所有的值乘起来相加，如果维度很大的话，值肯定小不了，那么值一大，在softmax的导数函数中，值越大梯度越小，导致softmax难以优化。所以才有了上面的scaled dot-product attention。

### Multi-Head Attention

多头的意思就是将单个的attention并行的做多个，不做权重共享，然后就会产生多个输出，然后将这些结果串起来就是一个新的向量，然后再做一次映射，合并成一个值。具体的公式如下：

MultiHead(Q,K,V) = Concat(head1,head2,..,Headh)Wo 其中 headi = Attention(QWi,KWi,VWi)

### Attention在模型中的应用

Transormer中的三种multi-head attention使用方式：

1.在encoder-decoder attention 层中，queries是来自之前的decoder层的输出，keys和values是本层encoder的输出。这使得decoder所有位置都能参与输入序列全部位置输入的计算。

2.encoder中的self-attention层。self-attention层中的所有keys,values,和queries都是同一个东西。

3.decoder中的self-attention层。和encoder的一样。

### 基于位置的前向网络

这个的话就是直接在self-attention之后加入一个单隐层的网络，将子层的输出做一个映射。

### 位置编码

如果没有位置编码，那么前面说了这么多，整个模型是没有基于位置信息的，那么这整个模型根本没有考虑到序列的关系，只能算是一个高阶版本的类似的词袋模型。所以如果不用cnn或者rnn就需要引入一个位置信息，这里引入了位置编码。论文将位置直接编码后和输入的embedding相加来作为模型的输入。而位置的编码也不是算法学习出来的，而是直接利用数据公式计算获得，

![](https://img-blog.csdnimg.cn/20181228232056148.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4ZjEwNjE5MjY5NTk=,size_16,color_FFFFFF,t_70)



这里的pos就是序列中元素的位置，i是用于表示该位置的dmodel维向量的第i个位置。直接将位置编码为正弦波，这样就可以相对用线性的表示不同位置之间的关系。

学习了下大佬的代码：[https://kexue.fm/archives/4765](https://kexue.fm/archives/4765)

代码可以上github看下

[https://github.com/bojone/attention](https://github.com/bojone/attention)

[https://github.com/Shicoder/DeepLearning_Demo/tree/master/attention](https://github.com/Shicoder/DeepLearning_Demo/tree/master/attention)



