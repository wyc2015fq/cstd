
# Deep contextualized word representations - lpty的博客 - CSDN博客

2019年04月04日 09:49:04[lpty](https://me.csdn.net/sinat_33741547)阅读数：58



# 引言
这是2018年NAACL上的Best Paper，即是大名鼎鼎的ELMo，原文地址：[https://arxiv.org/pdf/1802.05365.pdf](https://arxiv.org/pdf/1802.05365.pdf)
对比传统Word2Vec这种形式的词向量，本文提出的模型是一种动态模型。在以往的词向量表示中，词都是一种静态的形式，无论在任何的上下文中都使用同一个向量。这种情况下很难表示一词多义的现象，而ELMo则可以通过上下文动态生成词向量，从理论上会是更好的模型，从实测效果来看在很多任务上也都达到了当时的SOTA成绩。
# 模型
## Bidirectional language models
一般的语言模型都是利用前k-1个token来预测第k个token，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190403184137908.PNG)
而双向语言模型就是添加了另一个方向的预测，利用N~k+1个token来预测第k个token，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190403184259820.PNG)
目标函数便是上述两者的联合损失，如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190403184422477.PNG)
这里需要注意的是$\theta_x$是token的表示，而$\theta_s$则为softmax参数，后续对此会详细说明。
[
](https://img-blog.csdnimg.cn/20190403184422477.PNG)
## ELMo
[
](https://img-blog.csdnimg.cn/20190403184422477.PNG)不同于其他类型的模型采用最顶层的输出作为token的表示，Elmo采用多个层的线性加和作为token的表示，对于每个token，在一个$L$层的biLm中可以输出$2L+1$个表示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090558935.PNG)
在下游任务中，可以将上述$2L+1$个表示进行整合，
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190404090916534.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
其中，$s_j ^{task}=softmax(w)$，$w$是一个独立的可训练的参数shape = (num_layers, )，用来加权各个layer的分布，毕竟layer所表示的东西不同。同样的，$γ$也是一个可以训练的常量，用在具体任务中去缩放ELMo的大小，作者在论文中提到简单使用顶层输出作为token的表示，效果比baseline还要差。
[
](https://img-blog.csdnimg.cn/20190404090916534.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
## Pre-trained bidirectional language model architecture
[
](https://img-blog.csdnimg.cn/20190404090916534.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)关于ELMo模型的整体结构及训练，论文中只是简单带过，具体借鉴了以下两篇文章：
[
](https://img-blog.csdnimg.cn/20190404090916534.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)`Exploring the limits of language modeling
Character-Aware Neural Language Models`[
](https://img-blog.csdnimg.cn/20190404090916534.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)总的来说还是按照训练语言模型的方式，使用了CNN-BIG-LSTM结构，和一个层之间的残差链接：
[
](https://img-blog.csdnimg.cn/20190404090916534.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9scHR5LW5scC5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
> The final model uses L = 2 biLSTM layers with 4096 units and 512

> dimension projections and a residual connection from the first to

> second layer. The context insensitive type representation uses 2048

> character n-gram convolutional filters followed by two highway layers

> (Srivastava et al., 2015) and a linear projection down to a 512

> representation.

> 最后的结果是, 整个模型针对一个token, 可以产生三个向量, 原始 , 第一层以及第二层。作者认为低层的bi-LSTM层能提取语料中的句法信息，高层的bi-LSTM能提取语料中的语义信息。

> Using biLMs for supervised NLP tasks

> ELMo在监督任务中可以先固定权重，然后和预训练的词向量拼接为
$$
[x_k; ELMo^{task}_k]
$$
> 这种形式作为模型的输入；

> 也可以与RNN的隐藏层拼接，如
$$
[h_k; ELMo^{task}_k]
$$
> 这种形式作为输出进行使用，需要注意这里的ELMo权重应该和输入端的权重不相等。

> 同时，也可以在ELMo模型中加入
$$
dropout
$$
> , 以及采用
$$
L2 loss
$$
> 的方法来提升模型。这里的
$$
L2
$$
> 项的系数 ​
$$
\lambda
$$
> 越大, 越有取各层平均值的意思, 越小, 越可以发挥各个层之间的不同带来的效果.。但是并不是说, 越小越好。

> 实验

> 论文从 Question answering, Textual entailment, Semantic role labeling, Coreference resolution, Named entity extraction, Sentiment analysis 等六个任务的验证中都取得了提升。

> 具体的验证结果这里就不贴出来了，详细参考原论文。

> 结论

> 动态的词向量生成方式，目前来说应该是在词向量这块最好的模型，当然在具体任务中效果还需要评测。但顶层使用基于字符的CNN，在英文这种形态丰富的语言中效果可能会比中文好很多，在中文任务中，顶层的token表示可能采用word级别的输入会更好一些。

> 引用

> 1、Deep contextualized word representations


