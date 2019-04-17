# NLP+2vec︱认识多种多样的2vec向量化模型 - 素质云笔记/Recorder... - CSDN博客





2017年04月08日 12:40:58[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：7118








> 
来自于github一位博主的整理，好多都没有看到过+还有我一直期待去研究的。 

  github：[https://github.com/MaxwellRebo/awesome-2vec](https://github.com/MaxwellRebo/awesome-2vec)


## 1、word2vec

耳熟能详的NLP向量化模型。 

Paper: [https://papers.nips.cc/paper/5021-distributed-representations-of-words-and-phrases-and-their-compositionality.pdf](https://papers.nips.cc/paper/5021-distributed-representations-of-words-and-phrases-and-their-compositionality.pdf)

Java: [http://deeplearning4j.org/word2vec](http://deeplearning4j.org/word2vec)

C++: [https://github.com/jdeng/word2vec](https://github.com/jdeng/word2vec)

Python:

[https://radimrehurek.com/gensim/models/word2vec.html](https://radimrehurek.com/gensim/models/word2vec.html)
[https://github.com/danielfrg/word2vec](https://github.com/danielfrg/word2vec)

## 2、doc2vec

Paper: [https://cs.stanford.edu/~quocle/paragraph_vector.pdf](https://cs.stanford.edu/~quocle/paragraph_vector.pdf)

Python: [https://radimrehurek.com/gensim/models/doc2vec.html](https://radimrehurek.com/gensim/models/doc2vec.html)

word2vec模型对词向量进行平均处理，我们仍然忽略了单词之间的排列顺序对情感分析的影响。即上述的word2vec只是基于词的维度进行”语义分析”的，而并不具有上下文的”语义分析”能力。

　　作为一个处理可变长度文本的总结性方法，Quoc Le 和 Tomas Mikolov 提出了 Doc2Vec方法。除了增加一个段落向量以外，这个方法几乎等同于 Word2Vec。和 Word2Vec 一样，该模型也存在两种方法：Distributed Memory(DM) 和 Distributed Bag of Words(DBOW)。DM 试图在给定上下文和段落向量的情况下预测单词的概率。在一个句子或者文档的训练过程中，段落 ID 保持不变，共享着同一个段落向量。DBOW 则在仅给定段落向量的情况下预测段落中一组随机单词的概率。 

（参考：[Algorithm & NLP] 文本深度表示模型——word2vec&doc2vec词向量模型](http://www.cnblogs.com/maybe2030/p/5427148.html#_label4) ）

## 3、tweet2vec

Paper: [https://arxiv.org/abs/1605.03481](https://arxiv.org/abs/1605.03481)

Python: [https://github.com/bdhingra/tweet2vec](https://github.com/bdhingra/tweet2vec)

一些社交文本中的语言结构跟书面语大不相同，所以作者别出心裁的特意做了一个基于字符组合的模型，其可以基于整个微博环境下复杂、非正常语言的字符串中学习到一种向量化的表达方式。

## 4、batter-pitcher-2vec

Python: [https://github.com/airalcorn2/batter-pitcher-2vec](https://github.com/airalcorn2/batter-pitcher-2vec)

## 5、illustration-2vec

Python: [https://github.com/rezoo/illustration2vec](https://github.com/rezoo/illustration2vec)

## 6、lda2vec

Paper: [https://arxiv.org/pdf/1605.02019v1.pdf](https://arxiv.org/pdf/1605.02019v1.pdf)

Slideshare: [http://www.slideshare.net/ChristopherMoody3/word2vec-lda-and-introducing-a-new-hybrid-algorithm-lda2vec-57135994](http://www.slideshare.net/ChristopherMoody3/word2vec-lda-and-introducing-a-new-hybrid-algorithm-lda2vec-57135994)

Python: [https://github.com/cemoody/lda2vec](https://github.com/cemoody/lda2vec)

将LDA与word2vec相结合，LDA是我比较在意的研究方向，这个模型很早提出，不过应用地倒是不多。

![这里写图片描述](https://img-blog.csdn.net/20170408123928526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

lda2vec the topics can be ‘supervised’ and forced to predict another target.
## 7、sentence2vec

Python: [https://github.com/klb3713/sentence2vec](https://github.com/klb3713/sentence2vec)

## 8、wiki2vec

Java/Scala: [https://github.com/idio/wiki2vec](https://github.com/idio/wiki2vec)

## 9、topicvec

Paper: [http://bigml.cs.tsinghua.edu.cn/~jun/topic-embedding.pdf](http://bigml.cs.tsinghua.edu.cn/~jun/topic-embedding.pdf)

Python: [https://github.com/askerlee/topicvec](https://github.com/askerlee/topicvec)

## 10、entity2vec

Python: [https://github.com/ot/entity2vec](https://github.com/ot/entity2vec)

Paper: [http://www.di.unipi.it/~ottavian/files/wsdm15_fel.pdf](http://www.di.unipi.it/~ottavian/files/wsdm15_fel.pdf)

## 11、str2vec

Python: [https://github.com/pengli09/str2vec](https://github.com/pengli09/str2vec)

## 12、node2vec

Paper: [https://arxiv.org/abs/1607.00653](https://arxiv.org/abs/1607.00653)

Page: [https://snap.stanford.edu/node2vec/](https://snap.stanford.edu/node2vec/)

Python: [https://github.com/aditya-grover/node2vec](https://github.com/aditya-grover/node2vec)

可见：[笔记︱基于网络节点的node2vec、论文、算法python实现](http://blog.csdn.net/sinat_26917383/article/details/54406344)

## 13、item2vec

Paper: [https://arxiv.org/abs/1603.04259](https://arxiv.org/abs/1603.04259)

## 14、author2vec

Paper: [https://www.microsoft.com/en-us/research/publication/author2vec-learning-author-representations-by-combining-content-and-link-information/](https://www.microsoft.com/en-us/research/publication/author2vec-learning-author-representations-by-combining-content-and-link-information/)

## 15、playlist-to-vec

Python: [https://github.com/mattdennewitz/playlist-to-vec](https://github.com/mattdennewitz/playlist-to-vec)

## 16、sense2vec

Paper: [https://arxiv.org/abs/1511.06388](https://arxiv.org/abs/1511.06388)

## 17、med2vec

Paper: [https://arxiv.org/abs/1602.05568](https://arxiv.org/abs/1602.05568)

Python: [https://github.com/mp2893/med2vec](https://github.com/mp2893/med2vec)

SCODE Word Embeddings using Substitute Words

Python: [https://github.com/ai-ku/wvec](https://github.com/ai-ku/wvec)

## 18、game2vec

Python: [https://github.com/warchildmd/game2vec](https://github.com/warchildmd/game2vec)

## 19、paper2vec

Paper: [https://arxiv.org/pdf/1703.06587.pdf](https://arxiv.org/pdf/1703.06587.pdf)




