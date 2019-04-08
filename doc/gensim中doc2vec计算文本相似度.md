# gensim中doc2vec计算文本相似度
2018年09月06日 17:14:20 [飞翔的小绵羊_fly](https://me.csdn.net/yangfengling1023) 阅读数：1174
 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/yangfengling1023/article/details/82464550
最近在做判断两个文本是否是描述的同一件事情，之前是采用gensim中的TF-IDF进行计算的，TF-IDF这种方法没有考虑到文字背后的语义关联，可能在两个文档共同出现的单词很少甚至没有相同的单词，但两个文档是相似的情况下，就需要考虑到文档的语义。我们都知道word2vec是可以分析语义的，那么doc2vec是不是也可以分析出语义呢？于是试了一下gensim中的doc2vec。
**Doc2Vec** 也可叫做 paragraph2vec, sentence embeddings，是一种非监督式算法，可以获得 sentences/paragraphs/documents 的向量表达，是 word2vec 的拓展。学出来的向量可以通过计算距离来找 sentences/paragraphs/documents 之间的相似性，可以用于文本聚类，对于有标签的数据，还可以用监督学习的方法进行文本分类，例如经典的情感分析问题。
Doc2Vec的目的是获得文档的一个固定长度的向量表达
数据：多个文档，以及它们的标签，可以用标题作为标签(这个标签的意思没有明白是什么意思)
影响模型准确率的因素：语料的大小，文档的数量，越多越好；文档的相似性，越相似越好
参考网址：<https://blog.csdn.net/juanjuan1314/article/details/75124046/#commentsedit>
doc2vec还可以细分为：DM模型、DBOW模型，在训练模型中的dm参数进行控制
具体的代码如下所示：
## 1、相关包的导入
```python
# coding:utf-8
import sys
import gensim
import sklearn
import numpy as np
import jieba
from gensim.models.doc2vec import Doc2Vec, LabeledSentence
from jieba import analyse
TaggededDocument = gensim.models.doc2vec.TaggedDocument
```
## 2、读取训练数据

```python
def get_datasest(path_qisu):
    with open(path_qisu, 'r') as cf:
        docs = cf.readlines()
    x_train = []
    for i, text in enumerate(docs):
        word_list = ' '.join(jieba.cut(text)).split(' ') ##保证读入的文件是进行分过词的，我选用的是jieba进行分的词
        l = len(word_list)
        word_list[l - 1] = word_list[l - 1].strip()
        document = TaggededDocument(word_list, tags=[i])
        x_train.append(document)
    return x_train
```
## 3、进行训练模型

```python
def train(x_train, size=200, epoch_num=1):
    model_dm = Doc2Vec(x_train, min_count=1, window=3, size=size, sample=1e-3, negative=5, workers=4)
    model_dm.train(x_train, total_examples=model_dm.corpus_count, epochs=70)
    model_dm.save('./model_wenjian/model_sim')
    return model_dm
```
## 4、给定文档进行测试，并计算相似度

```python
def ceshi(test_text):
    model_dm = Doc2Vec.load('./model_wenjian/model_sim')
    # test_text = ' '.join(jieba.cut(str1)).split(' ')
    inferred_vector_dm = model_dm.infer_vector(test_text.split(' '))
    # print inferred_vector_dm
    # Gensim 中有内置的 most_similar，得到向量后，可以计算相似性
    sims = model_dm.docvecs.most_similar([inferred_vector_dm], topn=10)
    return sims
```
## 5、对待计算相似度的文档进行关键信息的抽取

```python
def extract(line):
    ##引用TF-IDF关键词抽取接口
    tfidf = analyse.extract_tags
    ##结巴分词
    str1_fenci = ' '.join(jieba.cut(line))
    ##停用词的去除
    stop_word = []
    with open('stop_word.txt') as fp:
        for line in fp.readlines():
            line = line.strip()
            if line == '':
                continue
            stop_word.append(line.decode('utf-8'))
    str1_rv_stop_word = ''
    str1_rv_stop_word_fenci = ''
    for each in str1_fenci.split(' '):
        if each not in stop_word:
            if str1_rv_stop_word == '':
                str1_rv_stop_word = each
                str1_rv_stop_word_fenci = each
            else:
                str1_rv_stop_word = str1_rv_stop_word + each
                str1_rv_stop_word_fenci = str1_rv_stop_word_fenci + ' ' + each
    ##关键信息的抽取
    guanjian = tfidf(str1_rv_stop_word)
    guanjian_result = ''
    linshi = []
    for each in str1_rv_stop_word.split(' '):
        if each in guanjian:
            if guanjian_result == '':
                guanjian_result = each
            else:
                guanjian_result = guanjian_result + ' ' + each
            linshi.append(each)
    return guanjian_result
```
## 6、主程序代码

```python
if __name__ == '__main__':
    path_qisu = './qisu_yijianshu/qisu_0.txt'
    path_xunwen = './xunwen_bilu/xunwen_0.txt'
    path_write = './similarity_result/doc/' + 'sim_' + path_qisu.split('_')[-1].split('.')[0] + '.txt'
    ##此处放入起诉意见书的内容
    x_train = get_datasest(path_qisu)
    model_dm = train(x_train)
    ceshi_list = []
    with open(path_xunwen) as fp:
        for line in fp.readlines():
            line = line.strip()
            if line == '':
                continue
            ceshi_list.append(line)
    f1 = open(path_write,'w')
    for line in ceshi_list:
        f1.write(line+'\n')
        line = extract(line) ##关键信息的抽取
        sims = ceshi(line)
        for count, sim in sims:
            sentence = x_train[count]
            words = ''
            for word in sentence[0]:
                words = words + word + u' '
            print words, sim, len(sentence[0])
            f1.write(str(sim)+'\n')
            f1.write(words.encode('utf-8'))
            f1.write('\n')
        f1.write('\n')
    f1.close()
```
doc2vec算法是基于word2vec算法
代码中模型的使用：

```python
model_dm = Doc2Vec(x_train, min_count=1, window=3, size=size, sample=1e-3, negative=5, workers=4)
x_train：是训练文档，训练文档必须是一行一个文本，并且进行过分词
size：表示生成的向量的维度，一般为100维
windows：表示训练窗口的大小，也就是训练数据周围读取了几个数据
min_count：是参与训练的词语的最小词频
```

用到的Doc2vec的功能主要有三个：
1.TaggedDocument语料预处理 
2.train训练模型，save 和 load 加载训练好的模型 
3.docvecs.most_similar 计算相似度