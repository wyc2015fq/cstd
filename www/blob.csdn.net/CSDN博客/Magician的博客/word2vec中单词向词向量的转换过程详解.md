
# word2vec中单词向词向量的转换过程详解 - Magician的博客 - CSDN博客


2018年05月07日 16:41:00[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：2753


[目录](#目录)[前言：](#前言)
[1、Word2Vec两种模型的大致印象](#1word2vec两种模型的大致印象)
[2、CBOW模型流程举例](#2cbow模型流程举例)
[3、CBOW模型流程举例](#3cbow模型流程举例)
[总结：](#总结)


# 目录
## 前言：
针对word2vec是如何得到词向量的？这篇文章肯定能解决你的疑惑。该篇文章主要参考知乎某大神的回答，个人在此基础上做了一个总结。
word2vec是如何得到词向量的？这个问题比较大。从头开始讲的话，首先有了文本语料库，你需要对语料库进行预处理，这个处理流程与你的语料库种类以及个人目的有关，比如，如果是英文语料库你可能需要大小写转换检查拼写错误等操作，如果是中文日语语料库你需要增加分词处理。这个过程其他的答案已经梳理过了不再赘述。得到你想要的processed corpus之后，将他们的one-hot向量作为word2vec的输入，通过word2vec训练低维词向量（word embedding）就ok了。不得不说word2vec是个很棒的工具，目前有两种训练模型（CBOW和Skip-gram），两种加速算法（Negative Sample与Hierarchical Softmax）。本文旨在阐述word2vec如何将corpus的one-hot向量（模型的输入）转换成低维词向量（模型的中间产物，更具体来说是输入权重矩阵），真真切切感受到向量的变化，不涉及加速算法。
## 1、Word2Vec两种模型的大致印象
刚才也提到了，Word2Vec包含了两种词训练模型：CBOW模型和Skip-gram模型。
**CBOW模型根据中心词W(t)周围的词来预测中心词。**
**Skip-gram模型则根据中心词W(t)来预测周围词。**
## 2、CBOW模型流程举例
其实数学基础及英文好的同学可以参照斯坦福大学Deep Learning for NLP课堂笔记。
当然，懒省事儿的童鞋们就跟随我的脚步慢慢来吧。
先来看着这个结构图，用自然语言描述一下CBOW模型的流程：
![这里写图片描述](https://img-blog.csdn.net/20180507162347386?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**NOTE：花括号内{}为解释内容.**
输入层：上下文单词的onehot. {假设单词向量空间dim为V，上下文单词个数为C}
所有onehot分别乘以共享的输入权重矩阵W. {V*N矩阵，N为自己设定的数，初始化权重矩阵W}
所得的向量 {因为是onehot所以为向量} 相加求平均作为隐层向量, size为1*N.
乘以输出权重矩阵W’ {N*V}
得到向量 {1*V} ，激活函数处理得到V-dim概率分布 {PS:因为是onehot嘛，其中的每一维都代表着一个单词}，概率最大的index所指示的单词为预测出的中间词（target word）
与true label的onehot做比较，误差越小越好
所以，需要定义loss function（一般为交叉熵代价函数），**采用梯度下降算法**更新W和W’。训练完毕后，**输入层的每个单词与矩阵W相乘得到的向量的就是我们想要的词向量（word embedding）**，这个矩阵（所有单词的word embedding）也叫做look up table（其实聪明的你已经看出来了，其实这个look up table就是矩阵W自身），**也就是说，任何一个单词的onehot乘以这个矩阵都将得到自己的词向量。有了look up table就可以免去训练过程直接查表得到单词的词向量了。**
这回就能解释题主的疑问了！如果还是觉得我木有说明白，别着急！跟我来随着栗子走一趟CBOW模型的流程！
## 3、CBOW模型流程举例
假设我们现在的Corpus是这一个简单的只有四个单词的document：
{I drink coffee everyday}
我们选coffee作为中心词，window size设为2
也就是说，我们要根据单词”I”,”drink”和”everyday”来预测一个单词，并且我们希望这个单词是coffee。
**第一步：先确定输入和输出单词的one-hot向量**
![这里写图片描述](https://img-blog.csdn.net/20180507162752738?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180507162752738?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**第二步：初始化一个权重矩阵W（自定义），然后分别利用输入的单词的onehot向量X和W相乘得到向量V（代表一个单词）**
![这里写图片描述](https://img-blog.csdn.net/20180507162918665?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**第三步：将得到的三个单词的V向量相加作为隐藏层的矩阵（隐藏层也就代表周围词的一个集合（即符合由周围的词来预测中心的词））**
![这里写图片描述](https://img-blog.csdn.net/20180507163016520?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**第四步：将隐藏层向量乘以W’**
![这里写图片描述](https://img-blog.csdn.net/20180507163039291?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**第五步：激活函数处理得到V-dim概率分布 {PS: 因为是onehot嘛，其中的每一维都代表着一个单词}，概率最大的index所指示的单词为预测出的中间词（target word）。与true label的onehot做比较，误差越小越好**
![这里写图片描述](https://img-blog.csdn.net/20180507163413499?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 总结：
目前博主也是刚开始接触NLP相关的知识，许多东西还处于小白阶段，以上的总结如有不足或错误希望大家指导批评。

