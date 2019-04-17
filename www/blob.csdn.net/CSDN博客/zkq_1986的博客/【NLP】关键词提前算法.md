# 【NLP】关键词提前算法 - zkq_1986的博客 - CSDN博客





2018年02月05日 14:43:35[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：141








# [TextRank算法提取关键词的Java实现](http://www.hankcs.com/nlp/textrank-algorithm-to-extract-the-keywords-java-implementation.html)

[码农场](http://www.hankcs.com/)>[自然语言处理](http://www.hankcs.com/nlp/)2014-03-26阅读(16935)[评论(27)](http://www.hankcs.com/nlp/textrank-algorithm-to-extract-the-keywords-java-implementation.html#respond)




目录




- [PageRank的计算公式：](http://www.hankcs.com/nlp/textrank-algorithm-to-extract-the-keywords-java-implementation.html#h2-0)
- [正规的TextRank公式](http://www.hankcs.com/nlp/textrank-algorithm-to-extract-the-keywords-java-implementation.html#h2-1)
- [TextRank的Java实现](http://www.hankcs.com/nlp/textrank-algorithm-to-extract-the-keywords-java-implementation.html#h2-2)
- [排序后的投票结果：](http://www.hankcs.com/nlp/textrank-algorithm-to-extract-the-keywords-java-implementation.html#h2-3)
- [开源项目地址](http://www.hankcs.com/nlp/textrank-algorithm-to-extract-the-keywords-java-implementation.html#h2-4)



谈起自动摘要算法，常见的并且最易实现的当属TF-IDF，但是感觉TF-IDF效果一般，不如[TextRank](http://acl.ldc.upenn.edu/acl2004/emnlp/pdf/Mihalcea.pdf)好。

TextRank是在Google的PageRank算法启发下，针对文本里的句子设计的权重算法，目标是自动摘要。它利用投票的原理，让每一个单词给它的邻居（术语称窗口）投赞成票，票的权重取决于自己的票数。这是一个“先有鸡还是先有蛋”的悖论，PageRank采用矩阵迭代收敛的方式解决了这个悖论。TextRank也不例外：

## PageRank的计算公式：



![](http://ww4.sinaimg.cn/large/6cbb8645gw1eeti85tt02j20jj03xmx5.jpg)

## 正规的TextRank公式


正规的TextRank公式在PageRank的公式的基础上，引入了边的权值的概念，代表两个句子的相似度。

![](http://ww1.sinaimg.cn/large/6cbb8645gw1eetibn86vij20jy03gmx8.jpg)


但是很明显我只想计算关键字，如果把一个单词视为一个句子的话，那么所有句子（单词）构成的边的权重都是0（没有交集，没有相似性），所以分子分母的权值w约掉了，算法退化为PageRank。所以说，这里称关键字提取算法为PageRank也不为过。


另外，如果你想提取关键句（自动摘要）的话，请参考姊妹篇《[](http://www.hankcs.com/nlp/textrank-algorithm-java-implementation-of-automatic-abstract.html)[TextRank算法自动摘要的Java实现](http://www.hankcs.com/nlp/textrank-algorithm-java-implementation-of-automatic-abstract.html)》。





转载自：http://www.hankcs.com/nlp/textrank-algorithm-to-extract-the-keywords-java-implementation.html



