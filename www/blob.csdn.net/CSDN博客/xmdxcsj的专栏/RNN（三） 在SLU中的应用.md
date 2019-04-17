# RNN（三） 在SLU中的应用 - xmdxcsj的专栏 - CSDN博客





2015年10月05日 20:40:19[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2220








LU的主要任务是提取语义或者fillingslots，传统的解决SLU的方法包括生成模型（HMM/CFG）或者判别模型（CRF），最近RNN也开始用于SLU。

SLU的语义解析包括三个方面任务：domain detection, intent determination, and slot filling，例子如下：

![](https://img-blog.csdn.net/20151005203805291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

对于slot filling来讲，输入是句子，输出是slot序列。

## Word embeddings

词向量：使用向量的形式表示词典里的词。向量的产生可以使用NN、降维等手段实现。使用词向量来作为语义解析或者句法分析的输入，可以提升NLP的效果。

见参考文献前两个。

## Short-term

没有时间反馈的神经网络称为MLP（multi-layer perception），如果需要包含短时间反馈的信息，可以使用word context window（一帧的输入考虑到前后帧的输入）。类似于：

![](https://img-blog.csdn.net/20151005203830204?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## long-term

神经网络的短时反馈可以使用时间窗加以体现，对于超过时间窗口的长时反馈，需要使用RNN这种网络结构。有两种形式的RNN网络：Elman-type RNN和Jordan-type RNN。

### Elman-type RNN

上一时刻的隐层对当前时候的隐层产生影响，因此实现系统的长时反馈：

![](https://img-blog.csdn.net/20151005203845637?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### Jordan-type RNN

上一时刻的输出对当前时刻的隐层产生影响：

![](https://img-blog.csdn.net/20151005203852434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## Lerning methods

对词向量做fine-tune

梯度下降使用一句话作为一个mini-batch

Dropout



## 参考文献

[https://en.wikipedia.org/wiki/Word_embedding](https://en.wikipedia.org/wiki/Word_embedding)

[http://licstar.net/archives/tag/word-embedding](http://licstar.net/archives/tag/word-embedding)

Investigationof Recurrent-Neural-Network Architectures and Learning Methods for SpokenLanguage Understanding
            


