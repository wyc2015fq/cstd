# FastText词向量表示 - 技术成长笔记 - CSDN博客





2018年11月07日 10:23:43[zhixuhao](https://me.csdn.net/u012931582)阅读数：1184








> 
论文《Enriching Word Vectors with Subword Information》


# 介绍

FastText的作者也就是word2vec的作者，所以两者是一脉相承的。

目前的词向量模型都是把每一个单词作为单独的向量，并没有考虑词语的内部结构，那么FastText相比于word2vec的创新就是考虑了词语的形态构成，也就是加上了sub-word的信息，这样的好处在于对于词汇量很大的语言，通常有很多未登录词的这种，也可以通过sub-word去构成word进行词向量表示。

# 模型

关于模型就不细讲了，就是一个skipgram model with negative，和word2vec一样的，公式如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181107100346192.png)

C_t表示上下文范围内的单词，N_t,c表示负样本，其中l表示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181107100544210.png)

s表示score function，这里用：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181107100724560.png)
## subword model

整篇文章最主要的创新点就在于这个subword model，对于每一个单词w，拆分成字符n-gram进行表示，并且加入了尖括号<>在单词外面，因为这样可以区分前缀和后缀，比如一个单词where如果用3-gram来表示，那么可以表示为：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018110710123135.png)

以及:
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181107101456326.png)

那么在模型训练的时候计算scoring function的时候，当前单词的词向量就用n-gram的向量的和：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181107101336167.png)

其中z_g表示n-gram的向量。
# 结果

用人工标记的单词相似度来衡量几个模型的效果。作为baseline的模型是skipgrim和cbow，由于FastText相对于baseline的一大优势在于可以输出未登录词的词向量，所以增加了一个比对的模型，sisg-，这个模型也是FastText，只不过将所有未登录词输出变为NULL，这样可以比对在失去对未登录词的适应性这个优势的时候，FastText效果如何，从下表可以看出，即使将未登录词输出都变为NULL，结果也依然不弱于两个baseline，如果是正常的FastText，sisg优势就更加明显了。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181107102307331.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTI5MzE1ODI=,size_16,color_FFFFFF,t_70)





