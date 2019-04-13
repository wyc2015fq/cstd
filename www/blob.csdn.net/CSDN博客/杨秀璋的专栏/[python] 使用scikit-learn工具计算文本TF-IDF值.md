
# [python] 使用scikit-learn工具计算文本TF-IDF值 - 杨秀璋的专栏 - CSDN博客

2016年08月08日 16:46:08[Eastmount](https://me.csdn.net/Eastmount)阅读数：46094所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)



在文本聚类、文本分类或者比较两个文档相似程度过程中，可能会涉及到TF-IDF值的计算。这里主要讲述基于Python的机器学习模块和开源工具：scikit-learn。
希望文章对你有所帮助，相关文章如下：
[[python爬虫]
 Selenium获取百度百科旅游景点的InfoBox消息盒](http://blog.csdn.net/eastmount/article/details/48234733)
[Python简单实现基于VSM的余弦相似度计算](http://blog.csdn.net/eastmount/article/details/49898133)
[基于VSM的命名实体识别、歧义消解和指代消解](http://blog.csdn.net/eastmount/article/details/48566671)
[[python] 使用Jieba工具中文分词及文本聚类概念](http://blog.csdn.net/eastmount/article/details/50256163)
目录：
一.Scikit-learn概念1.概念知识
2.安装软件
二.TF-IDF基础知识1.TF-IDF
2.举例介绍
三.TF-IDF调用两个方法
1.CountVectorizer
2.TfidfTransformer
3.别人示例


## 一. Scikit-learn概念

### 1.概念知识
官方网址：[http://scikit-learn.org/stable/](http://scikit-learn.org/stable/)
Scikit-learn是一个用于数据挖掘和数据分析的简单且有效的工具，它是基于Python的机器学习模块，基于BSD开源许可证。
![](https://img-blog.csdn.net/20151216043017374)
Scikit-learn的基本功能主要被分为六个部分：分类(Classification)、回归(Regression)、聚类(Clustering)、数据降维(Dimensionality
 reduction)、模型选择(Model selection)、数据预处理(Preprocessing)。
Scikit-Learn中的机器学习模型非常丰富，包括SVM，决策树，GBDT，KNN等等，可以根据问题的类型选择合适的模型，具体可以参考官网文档，推荐大家从官网中下载资源、模块、文档进行学习。
![](https://img-blog.csdn.net/20151216043539514)
Scikit-Learn的安装需要numpy、scipy、matplotlib等模块，windows用户可以到：[http://www.lfd.uci.edu/~gohlke/pythonlibs](http://www.lfd.uci.edu/~gohlke/pythonlibs)直接下载编译好的安装包以及依赖，也可以到这个网站下载：[http://sourceforge.jp/projects/sfnet_scikit-learn/](http://sourceforge.jp/projects/sfnet_scikit-learn/)。
参考文章：[开源机器学习工具scikit-learn入门 - 轩辕森](http://blog.csdn.net/xuanyuansen/article/details/42710741)

### 2.安装软件
Python 2.0我推荐使用"**pip install****scikit-learn**"或"**easy_install****scikit-learn**"全自动安装，再通过"**from****sklearn****import****feature_extraction**"导入。
安装时如果出现错误"unknown encoding: cp65001"，输入"chcp 936"将编码方式由utf-8变为简体中文gbk。
![](https://img-blog.csdn.net/20151216050114475?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


## 二. TF-IDF基础知识
参考官方文档：
gensim中TF-IDF：[http://radimrehurek.com/gensim/models/tfidfmodel.html](http://radimrehurek.com/gensim/models/tfidfmodel.html)
### 1.
### TF-IDF
TF-IDF（Term Frequency-InversDocument Frequency）是一种常用于信息处理和数据挖掘的加权技术。该技术采用一种统计方法，根据字词的在文本中出现的次数和在整个语料中出现的文档频率来计算一个字词在整个语料中的重要程度。它的优点是能过滤掉一些常见的却无关紧要本的词语，同时保留影响整个文本的重要字词。计算方法如下面公式所示。
![](https://img-blog.csdn.net/20160808154825745)
其中，式中*tfidf**i，j*表示词频*tf**i,j*和倒文本词频*idf**i*的乘积。TF-IDF值越大表示该特征词对这个文本的重要性越大。
TF（Term Frequency）表示某个关键词在整篇文章中出现的频率。
IDF（InversDocument Frequency）表示计算倒文本频率。文本频率是指某个关键词在整个语料所有文章中出现的次数。倒文档频率又称为逆文档频率，它是文档频率的倒数，主要用于降低所有文档中一些常见却对文档影响不大的词语的作用。
下面公式是TF词频的计算公式。
![](https://img-blog.csdn.net/20160808154857854)
其中，*n**i,j*为特征词*t**i*在文本*d**j*中出现的次数，是文本*d**j*中所有特征词的个数。计算的结果即为某个特征词的词频。
下面公式是IDF的计算公式。
![](https://img-blog.csdn.net/20160808154922714)
其中，|*D*|表示语料中文本的总数，表示文本中包含特征词*t**i*的数量。为防止该词语在语料库中不存在，即分母为0，则使用作为分母。
### 2.示例
示例参考仿造阮一峰大神的例子进行简单讲解，推荐大家去阅读：
[TF-IDF与余弦相似性的应用（一）：自动提取关键词](http://www.ruanyifeng.com/blog/2013/03/tf-idf.html)
下面通过一个示例进行讲解TF-IDF权重计算的方法。
假设现在有一篇文章《贵州的大数据分析》，这篇文章包含了10000个词组，其中“贵州”、“大数据”、“分析”各出现100次，“的”出现500次（假设没有去除停用词），则通过前面TF词频计算公式，可以计算得到三个单词的词频，即：
![](https://img-blog.csdn.net/20160808160728646)
现在预料库中共存在1000篇文章，其中包含“贵州”的共99篇，包含“大数据”的共19篇，包含“分析”的共“59”篇，包含“的”共“899”篇。则它们的IDF计算如下：
![](https://img-blog.csdn.net/20160808160752037)
由IDF可以发现，当某个词在语料库中各个文档出现的次数越多，它的IDF值越低，当它在所有文档中都出现时，其IDF计算结果为0，而通常这些出现次数非常多的词或字为“的”、“我”、“吗”等，它对文章的权重计算起不到一定的作用。
同时计算TF-IDF值如下：
![](https://img-blog.csdn.net/20160808160817878)
通过TF-IDF计算，“大数据”在某篇文章中出现频率很高，这就能反应这篇文章的主题就是关于“大数据”方向的。如果只选择一个词，“大数据”就是这篇文章的关键词。所以，可以通过TF-IDF方法统计文章的关键词。同时，如果同时计算“贵州”、“大数据”、“分析”的TF-IDF，将这些词的TF-IDF相加，可以得到整篇文档的值，用于信息检索。
TF-IDF算法的优点是简单快速，结果比较符合实际情况。缺点是单纯以词频衡量一个词的重要性，不够全面，有时重要的词可能出现次数并不多。而且，这种算法无法体现词的位置信息。

## 三. TF-IDF计算

Scikit-Learn中TF-IDF权重计算方法主要用到两个类：CountVectorizer和TfidfTransformer。
### 1.
### CountVectorizer
CountVectorizer类会将文本中的词语转换为词频矩阵，例如矩阵中包含一个元素a[i][j]，它表示j词在i类文本下的词频。它通过fit_transform函数计算各个词语出现的次数，通过get_feature_names()可获取词袋中所有文本的关键字，通过toarray()可看到词频矩阵的结果。
代码如下：
```python
# coding:utf-8
from sklearn.feature_extraction.text import CountVectorizer
#语料
corpus = [
    'This is the first document.',
    'This is the second second document.',
    'And the third one.',
    'Is this the first document?',
]
#将文本中的词语转换为词频矩阵
vectorizer = CountVectorizer()
#计算个词语出现的次数
X = vectorizer.fit_transform(corpus)
#获取词袋中所有文本关键词
word = vectorizer.get_feature_names()
print word
#查看词频结果
print X.toarray()
```
输出如下所示：
```python
>>> 
[u'and', u'document', u'first', u'is', u'one', u'second', u'the', u'third', u'this']
[[0 1 1 1 0 0 1 0 1]
 [0 1 0 1 0 2 1 0 1]
 [1 0 0 0 1 0 1 1 0]
 [0 1 1 1 0 0 1 0 1]]
>>>
```
从结果中可以看到，总共包括9个特征词，即：
[u'and', u'document', u'first', u'is', u'one', u'second', u'the', u'third', u'this']
同时在输出每个句子中包含特征词的个数。例如，第一句“This is the first document.”，它对应的词频为[0, 1, 1, 1, 0, 0, 1, 0, 1]，假设初始序号从1开始计数，则该词频表示存在第2个位置的单词“document”共1次、第3个位置的单词“first”共1次、第4个位置的单词“is”共1次、第9个位置的单词“this”共1词。所以，每个句子都会得到一个词频向量。
### 2.TfidfTransformer
TfidfTransformer用于统计vectorizer中每个词语的TF-IDF值。具体用法如下：
```python
# coding:utf-8
from sklearn.feature_extraction.text import CountVectorizer
#语料
corpus = [
    'This is the first document.',
    'This is the second second document.',
    'And the third one.',
    'Is this the first document?',
]
#将文本中的词语转换为词频矩阵
vectorizer = CountVectorizer()
#计算个词语出现的次数
X = vectorizer.fit_transform(corpus)
#获取词袋中所有文本关键词
word = vectorizer.get_feature_names()
print word
#查看词频结果
print X.toarray()
from sklearn.feature_extraction.text import TfidfTransformer
#类调用
transformer = TfidfTransformer()
print transformer
#将词频矩阵X统计成TF-IDF值
tfidf = transformer.fit_transform(X)
#查看数据结构 tfidf[i][j]表示i类文本中的tf-idf权重
print tfidf.toarray()
```
输出结果入下所示：
![](https://img-blog.csdn.net/20160808161738899)


### 3.别人示例
如果需要同时进行词频统计并计算TF-IDF值，则使用核心代码：
vectorizer=CountVectorizer()
transformer=TfidfTransformer()
tfidf=transformer.fit_transform(vectorizer.fit_transform(corpus))
下面给出一个liuxuejiang158大神的例子，供大家学习，推荐大家阅读原文：
[python scikit-learn计算tf-idf词语权重 - liuxuejiang](http://blog.csdn.net/liuxuejiang158blog/article/details/31360765)

```python
# coding:utf-8
__author__ = "liuxuejiang"
import jieba
import jieba.posseg as pseg
import os
import sys
from sklearn import feature_extraction
from sklearn.feature_extraction.text import TfidfTransformer
from sklearn.feature_extraction.text import CountVectorizer
if __name__ == "__main__":
    corpus=["我 来到 北京 清华大学",#第一类文本切词后的结果，词之间以空格隔开
		"他 来到 了 网易 杭研 大厦",#第二类文本的切词结果
		"小明 硕士 毕业 与 中国 科学院",#第三类文本的切词结果
		"我 爱 北京 天安门"]#第四类文本的切词结果
    vectorizer=CountVectorizer()#该类会将文本中的词语转换为词频矩阵，矩阵元素a[i][j] 表示j词在i类文本下的词频
    transformer=TfidfTransformer()#该类会统计每个词语的tf-idf权值
    tfidf=transformer.fit_transform(vectorizer.fit_transform(corpus))#第一个fit_transform是计算tf-idf，第二个fit_transform是将文本转为词频矩阵
    word=vectorizer.get_feature_names()#获取词袋模型中的所有词语
    weight=tfidf.toarray()#将tf-idf矩阵抽取出来，元素a[i][j]表示j词在i类文本中的tf-idf权重
    for i in range(len(weight)):#打印每类文本的tf-idf词语权重，第一个for遍历所有文本，第二个for便利某一类文本下的词语权重
        print u"-------这里输出第",i,u"类文本的词语tf-idf权重------"
        for j in range(len(word)):
            print word[j],weight[i][j]
```
输出如下所示：
```python
-------这里输出第 0 类文本的词语tf-idf权重------           #该类对应的原文本是："我来到北京清华大学"
中国 0.0
北京 0.52640543361
大厦 0.0
天安门 0.0
小明 0.0
来到 0.52640543361
杭研 0.0
毕业 0.0
清华大学 0.66767854461
硕士 0.0
科学院 0.0
网易 0.0
-------这里输出第 1 类文本的词语tf-idf权重------           #该类对应的原文本是： "他来到了网易杭研大厦"
中国 0.0
北京 0.0
大厦 0.525472749264
天安门 0.0
小明 0.0
来到 0.414288751166
杭研 0.525472749264
毕业 0.0
清华大学 0.0
硕士 0.0
科学院 0.0
网易 0.525472749264
-------这里输出第 2 类文本的词语tf-idf权重------           #该类对应的原文本是： "小明硕士毕业于中国科学院“
中国 0.4472135955
北京 0.0
大厦 0.0
天安门 0.0
小明 0.4472135955
来到 0.0
杭研 0.0
毕业 0.4472135955
清华大学 0.0
硕士 0.4472135955
科学院 0.4472135955
网易 0.0
-------这里输出第 3 类文本的词语tf-idf权重------            #该类对应的原文本是： "我爱北京天安门"
中国 0.0
北京 0.61913029649
大厦 0.0
天安门 0.78528827571
小明 0.0
来到 0.0
杭研 0.0
毕业 0.0
清华大学 0.0
硕士 0.0
科学院 0.0
网易 0.0
```

推荐几篇机器学习和NLP领域的大神博客：
[应用scikit-learn做文本分类 - Rachel-Zhang](http://blog.csdn.net/abcjennifer/article/details/23615947)
[python scikit-learn计算tf-idf词语权重 - liuxuejiang](http://blog.csdn.net/liuxuejiang158blog/article/details/31360765)
[用Python开始机器学习（5：文本特征抽取与向量化）（强推）- lsldd](http://blog.csdn.net/lsldd/article/details/41520953)
[再谈word2vec - Felven (强推)](http://blog.csdn.net/jj12345jj198999/article/details/27352659)
[利用word2vec对关键词进行聚类 - Felven (强推)](http://blog.csdn.net/jj12345jj198999/article/details/11069485)
[Python 对文档内容TFIDF处理](http://blog.csdn.net/guaguastd/article/details/39134241)
[Python TF-IDF计算100份文档关键词权重 - chenbjin](http://www.cnblogs.com/chenbjin/p/3851165.html)

最后希望文章对你有所帮助，如果文章中存在不足或错误的地方，还请海涵~还是那句话，挺享受现在的老师生活，不论科研、项目，还是教学，很充实，加油！
但行好事，莫问前程。
待随满天李桃，再追学友趣事。
(By:Eastmount 2016-08-08 下午5点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))



