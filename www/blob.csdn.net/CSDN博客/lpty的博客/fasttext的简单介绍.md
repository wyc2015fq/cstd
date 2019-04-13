
# fasttext的简单介绍 - lpty的博客 - CSDN博客

2017年12月12日 20:06:02[lpty](https://me.csdn.net/sinat_33741547)阅读数：4155



## 一、前言
fasttext是Facebook AI Reserch在16年开源的一个词向量及文本分类工具。
在模型架构上跟word2vec非常相似，毕竟作者都是Tomas Mikolov。其实从另一种角度理解，fasttext算是word2vec的一种衍生模型。fasttext的两篇基础论文，更像是提出了一些技巧，来更好的训练词向量和进行分类。
关于word2vec的可以参考：[对word2vec的理解](http://blog.csdn.net/sinat_33741547/article/details/78759289)
## 二、词向量
### 1、
### [Enriching Word Vectors with Subword Information](https://arxiv.org/pdf/1607.04606v1.pdf)
上述论文主要介绍一种基于语言形态学进行词向量训练的方法，基本模型与word2vec一致，但是在输入层位置进行了改变，原论文表述：
![](https://img-blog.csdn.net/20171212192728132)
大意是对于输入的上下文中的每一个词，都进行基于词的n-gram，之后将所有的n-gram和原词相加，来代表上下文信息。这种做法的好处是英文单词中，可以由前缀或者后缀等语言形态上的相似性，在词与词之间建立联系。
### 2、
### [Joint Learning of Character and Word Embeddings](http://www.thunlp.org/~lzy/publications/ijcai2015_character.pdf)
上述的论文只是列举了英文的语料，但是在中文里面的表现如何呢？这篇论文做了类似的工作。
论文在进行词向量训练的时候，将词语中组成词语的汉字单独抽取出来，和词语进行训练。这样就能够使得共享汉字的词语之间产生联系。
![](https://img-blog.csdn.net/20171212193953757)
可以看到，模型还是采用的CBOW，但是对输入的上下文词向量进行单独汉字的分割。文章中提到，这种模型对于具有“”semantically compositional“的词语具有一定的作用，即是组成词语的单个的词与整个词语在语义上有一定联系，比如：智能=智+能，但是比如”巧克力“等就不太适合了。
## 三、分类
### 1、
### [Bag of Tricks for Efficient Text Classification](https://arxiv.org/pdf/1607.01759v2.pdf)
上述的论文正式提出了fasttext模型，同样基于word2vec的模型，只是在CBOW中，将原本用上下文来预测中间词，改成了用整个序列文本预测分类标签。
文章提到了一点，传统的word2vec模型，比如CBOW，只是将上下文词向量相加后进行模型训练，这样上下文信息中的序列信息其实被忽略，如：
（1）喜欢 他 不
（2）不 喜欢 他
上述两个句子，如果使用传统的word2vec模型，基本视为相同的一句话进行训练。基于这个问题，fasttext提出另一种方法，利用了词之间的n-gram进行词向量表征，原文：
![](https://img-blog.csdn.net/20171212195305317)
基本方法与词向量很相似，但这里的n-gram是词级别的，如：a b c = ab abc bc，这样就在一定程度上考虑了词序列的信息，理论上能达到更好的结果。
### 2、其他
fasttext宣扬了一种思想，如果只是简单的任务，用简单的模型如LR就能够达到目的，而没有必要一定去用复杂的模型。
作者讲到，当类别的数量巨大时，计算线性分类器的计算量相当高，更准确的说，计算复杂度是 O(kh) ，k 是类别的数量，h 是文本特征的维度数。
基于 Huffman 树的 hierarchical softmax，可以将计算复杂度降到 O(hlog2(k)) 。
但是fastText在预测时，计算复杂度仍然是O(kh)，因为fasttext需要遍历整个huffman树，找到概率最大的前k个类型。

## 四、参考
1、[Enriching Word Vectors with Subword Information](https://arxiv.org/pdf/1607.04606v1.pdf)
2、[Joint Learning of Character and Word Embeddings](http://www.thunlp.org/~lzy/publications/ijcai2015_character.pdf)
3、[Bag of Tricks for Efficient Text Classification](https://arxiv.org/pdf/1607.01759v2.pdf)

