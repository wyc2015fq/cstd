# 机器翻译模型之Fairseq：《Convolutional Sequence to Sequence Learning》 - 技术成长笔记 - CSDN博客





2018年11月04日 21:13:30[zhixuhao](https://me.csdn.net/u012931582)阅读数：1813








近年来，NLP领域发展迅速，而机器翻译是其中比较成功的一个应用，自从2016年谷歌宣布新一代谷歌翻译系统上线，神经机器翻译（NMT，neural machine translation）就取代了统计机器翻译（SMT，statistical machine translation），在翻译质量上面获得了大幅的提高。目前神经机器翻译模型主要分为三种：

1. 一种是以rnn为基础的模型， 一般是LSTM+attention，顺序处理输入信息。

2. 一种是以cnn为基础的模型，今天要讲的Fairseq就属于这种

3. 一种是完全依靠attention的模型，如谷歌的transformer
# 1. 背景介绍

Fairseq 这个翻译模型由Facebook AI实验室在2017年提出，和以往以RNN为基础的翻译模型相比，采用了以cnn为主的模型结构。

RNN的链式结构，能够很好地应用于处理序列信息。但是，RNN也存在着劣势：一个是由于RNN运行时是将序列的信息逐个处理，不能实现并行操作，导致运行速度慢；另一个是传统的RNN并不能很好地处理句子中的结构化信息，或者说更复杂的关系信息。

相比之下，CNN的优势就凸显出来。文章提到用CNN做seq-seq这种翻译任务有3个好处：

1. 通过卷积的叠加可以精确地控制上下文的长度，因为卷积之间的叠加可以通过公式直接计算出感受野是多少，从而知道上下文的长度，RNN虽然理论上有长时记忆的功能，但是在实际的训练过程中，间隔较远的时候，很难学到这种词与词之间的联系。

2. 卷积可以进行并行计算，而RNN模型是时序的，只有前一帧得出结果才能进行后续的计算。

3. 对于输入的一个单词而言，输入CNN网络，所经过的卷积核和非线性计算数量都是固定的，不过对于输入RNN的单词而言，第一个单词要经过n次unit的计算和非线性，但是最后一个单词只经过1次，文章说固定队输入所施加的非线性计算会有助于训练。
# 2. 模型

好，下面主要讲一下Fairseq的模型，模型结构如下图所示：
![fairseq结构](https://img-blog.csdnimg.cn/20181104200118863.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI5MzE1ODI=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://github.com/pytorch/fairseq/raw/master/fairseq.gif)

下面进行分步讲解：
## Position Embedding

输入除了词向量之外，还加入了位置信息，最后的输入向量为词向量加上位置向量。
![位置向量](https://img-blog.csdnimg.cn/20181104200450309.png)

其中w为词向量，p为位置向量。
## Convolution

假设每一个卷积核的宽度是k=5，也就是每次对5个输入的单词进行卷积，那么卷积核的参数是
![卷积参数](https://img-blog.csdnimg.cn/20181104201342536.png)

输入是：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104201641777.png)

其中d是词向量的长度，经过卷积之后，输出的向量维度是2d，经过卷积之后，还要经过一个非线性单元，一般在图像当中，这个非线性单元是relu，sigmoid这样的激活函数，在这里，Fairseq采用的是GLU（Gated  Linear Units）。
## GLU

GLU是线性门控单元，假设一个卷积核的输出是：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104201954419.png)

GLU的公式是：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104202204937.png)

表示向量A点乘sigmoid处理B的结果，这里可以将B理解为权重，控制着A中的哪些维度是相关的，哪些是无关的。
## Residual connection

这里参考了图像领域鼎鼎大名的Resnet的结构，加入了residual connection，第l层卷积的输出就变成了：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104202630840.png)
## Multi-step Attention

这是一个离散的attention机制，和rnn的不同，这个机制对于每一个卷积层有一个attention，计算公式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104204603366.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI5MzE1ODI=,size_16,color_FFFFFF,t_70)
和传统的attention相似，权重由decoder当前的输出h_i和encoder的所有输出z决定，利用该权重对encoder的输出进行加权，得到c，之后将c和h连接在一起组成新的h，作为下一卷积层的输入。

和传统attention不同的地方是在计算c的时候，将原始的输入向量e加入进来了。

这就是多跳注意机制multi-hop attention。这样做的好处是使得模型在得到下一个注意时，能够考虑到之前的已经注意过的词。
# 结果

同以往的RNN模型做比较，发现BLEU和时间上都明显占优。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104210925766.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI5MzE1ODI=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181104210944900.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI5MzE1ODI=,size_16,color_FFFFFF,t_70)

# 代码地址

可以参考官方代码：[https://github.com/pytorch/fairseq](https://github.com/pytorch/fairseq)

# 总结

Fairseq成功地将卷积应用到了机器翻译上，主要的亮点有position embedding，GLU作为非线性单元，残差连接，multi-attention机制，从效果和效率上都要优于传统的RNN为基础的模型。














