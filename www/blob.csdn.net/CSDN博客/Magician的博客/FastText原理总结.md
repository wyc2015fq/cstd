
# FastText原理总结 - Magician的博客 - CSDN博客


2018年06月05日 11:03:40[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：7591


[目录：](#目录)[1、应用场景](#1应用场景)
[2、优缺点](#2优缺点)
[3、FastText的原理](#3fasttext的原理)
[4、FastText词向量与word2vec对比](#4fasttext词向量与word2vec对比)


# 目录：
## 1、应用场景
fastText是一种Facebook AI Research在16年开源的一个**文本分类器**。 其**特点就是fast**。相对于其它文本分类模型，如SVM，Logistic Regression和neural network等模型，fastText在保持分类效果的同时，大大缩短了训练时间。
## 2、优缺点
适合大型数据+高效的训练速度：能够训练模型“在使用标准多核CPU的情况下10分钟内处理超过10亿个词汇”
支持多语言表达：利用其语言形态结构，fastText能够被设计用来支持包括英语、德语、西班牙语、法语以及捷克语等多种语言。FastText的性能要比时下流行的word2vec工具明显好上不少，也比其他目前最先进的词态词汇表征要好。
fastText专注于文本分类，在许多标准问题上实现当下最好的表现（例如文本倾向性分析或标签预测）。
## 3、FastText的原理
**fastText 方法包含三部分：模型架构、层次 Softmax 和 N-gram 特征。**
fastText 模型**输入一个词的序列**（一段文本或者一句话)，**输出这个词序列属于不同类别的概率。**
序列中的词和词组组成特征向量，特征向量通过线性变换映射到中间层，中间层再映射到标签。
fastText 在预测标签时使用了非线性激活函数，但在中间层不使用非线性激活函数。
fastText 模型架构和 Word2Vec 中的 CBOW 模型很类似。不同之处在于，fastText 预测标签，而 CBOW 模型预测中间词。
**第一部分：fastText的模型架构类似于CBOW，两种模型都是基于Hierarchical Softmax，都是三层架构：输入层、 隐藏层、输出层。**
![这里写图片描述](https://img-blog.csdn.net/20180605105219552?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180605105219552?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
CBOW模型又基于N-gram模型和BOW模型，此模型将W(t−N+1)……W(t−1)W(t−N+1)……W(t−1)作为输入，去预测W(t)
fastText的模型则是将整个文本作为特征去预测文本的类别。
**第二部分：层次之间的映射**
将输入层中的词和词组构成特征向量，再将特征向量通过线性变换映射到隐藏层，隐藏层通过求解最大似然函数，然后根据每个类别的权重和模型参数构建Huffman树，将Huffman树作为输出。
![这里写图片描述](https://img-blog.csdn.net/20180605105517721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180605105517721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
具体的数学求解过程可参考博客：
[https://blog.csdn.net/yick_liao/article/details/62222153](https://blog.csdn.net/yick_liao/article/details/62222153)
**第三部分：fastText的N-gram特征**
常用的特征是词袋模型（将输入数据转化为对应的Bow形式）。**但词袋模型不能考虑词之间的顺序**，因此 fastText 还加入了 N-gram 特征。
**“我 爱 她” 这句话中的词袋模型特征是 “我”，“爱”, “她”。这些特征和句子 “她 爱 我” 的特征是一样的。**
**如果加入 2-Ngram，第一句话的特征还有 “我-爱” 和 “爱-她”，这两句话 “我 爱 她” 和 “她 爱 我” 就能区别开来了。当然，为了提高效率，我们需要过滤掉低频的 N-gram。**
在fastText 中一个低维度向量与每个单词都相关。隐藏表征在不同类别所有分类器中进行共享，使得文本信息在不同类别中能够共同使用。这类表征被称为词袋（bag of words）（此处忽视词序）。在 fastText中也使用向量表征单词 n-gram来将局部词序考虑在内，这对很多文本分类问题来说十分重要。
举例来说：fastText能够学会“男孩”、“女孩”、“男人”、“女人”指代的是特定的性别，并且能够将这些数值存在相关文档中。然后，当某个程序在提出一个用户请求（假设是“我女友现在在儿？”），它能够马上在fastText生成的文档中进行查找并且理解用户想要问的是有关女性的问题。
## 4、FastText词向量与word2vec对比
FastText= word2vec中 cbow + h-softmax的灵活使用
灵活体现在两个方面：
模型的输出层：word2vec的输出层，对应的是每一个term，计算某term的概率最大；而fasttext的输出层对应的是
分类的label。不过不管输出层对应的是什么内容，起对应的vector都不会被保留和使用；
模型的输入层：word2vec的输入层，是 context window 内的term；而fasttext 对应的整个sentence的内容，包括term，也包括 n-gram的内容；
两者本质的不同，体现在 h-softmax的使用。
Word2vec的目的是得到词向量，该词向量 最终是在输入层得到，输出层对应的 h-softmax 也会生成一系列的向量，但最终都被抛弃，不会使用。
fasttext则充分利用了h-softmax的分类功能，遍历分类树的所有叶节点，找到概率最大的label（一个或者N个）。
参考：
[https://blog.csdn.net/sinat_26917383/article/details/54850933](https://blog.csdn.net/sinat_26917383/article/details/54850933)
[https://blog.csdn.net/yick_liao/article/details/62222153](https://blog.csdn.net/yick_liao/article/details/62222153)

