
# [python] LDA处理文档主题分布代码入门笔记 - 杨秀璋的专栏 - CSDN博客

2016年03月09日 03:23:25[Eastmount](https://me.csdn.net/Eastmount)阅读数：27524所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)



以前只知道LDA是个好东西，但自己并没有真正去使用过。同时，关于它的文章也非常之多，推荐大家阅读书籍《LDA漫游指南》，最近自己在学习文档主题分布和实体对齐中也尝试使用LDA进行简单的实验。这篇文章主要是讲述Python下LDA的基础用法，希望对大家有所帮助。如果文章中有错误或不足之处，还请海涵~

## 一. 下载安装
LDA推荐下载地址包括：其中前三个比较常用。
gensim下载地址：[https://radimrehurek.com/gensim/models/ldamodel.html](https://radimrehurek.com/gensim/models/ldamodel.html)
pip install lda安装地址：[https://github.com/ariddell/lda](https://github.com/ariddell/lda)
scikit-learn官网文档：[LatentDirichletAllocation](http://scikit-learn.org/dev/modules/generated/sklearn.decomposition.LatentDirichletAllocation.html)
其中sklearn的代码例子可参考下面这篇：
[Topic
 extraction with NMF and Latent Dirichlet Allocation](http://scikit-learn.org/dev/auto_examples/applications/topics_extraction_with_nmf_lda.html#example-applications-topics-extraction-with-nmf-lda-py)
其部分输出如下所示，包括各个主体Topic包含的主题词：

```python
Loading dataset...
Fitting LDA models with tf features, n_samples=2000 and n_features=1000...
done in 0.733s.
Topics in LDA model:
Topic #0:
000 war list people sure civil lot wonder say religion america accepted punishment bobby add liberty person kill concept wrong
Topic #1:
just reliable gods consider required didn war makes little seen faith default various civil motto sense currency knowledge belief god
Topic #2:
god omnipotence power mean rules omnipotent deletion policy non nature suppose definition given able goal nation add place powerful leaders
....
```
下面这三个也不错，大家有时间的可以见到看看：
[https://github.com/arongdari/python-topic-model](https://github.com/arongdari/python-topic-model)
[https://github.com/shuyo/iir/tree/master/lda](https://github.com/shuyo/iir/tree/master/lda)
[https://github.com/a55509432/python-LDA](https://github.com/a55509432/python-LDA)
其中第三个作者a55509432的我也尝试用过，模型输出文件为：
`model_parameter.dat 保存模型训练时选择的参数 
wordidmap.dat 保存词与id的对应关系，主要用作topN时查询 
model_twords.dat 输出每个类高频词topN个 
model_tassgin.dat 输出文章中每个词分派的结果，文本格式为词id:类id 
model_theta.dat 输出文章与类的分布概率，文本一行表示一篇文章，概率1 概率2..表示文章属于类的概率 
model_phi.dat 输出词与类的分布概率，是一个K*M的矩阵，K为设置分类的个数，M为所有文章的词的总数`但是短文本信息还行，但使用大量文本内容时，输出文章与类分布概率几乎每行数据存在大量相同的，可能代码还存在BUG。
下面是介绍使用pip install lda安装过程及代码应用：
```python
pip install lda
```
![](https://img-blog.csdn.net/20160308082203752)
参考：[[python] 安装numpy+scipy+matlotlib+scikit-learn及问题解决](http://blog.csdn.net/eastmount/article/details/50347383)


## 二. 官方文档
这部分内容主要参考下面几个链接，强推大家去阅读与学习：
官网文档：[https://github.com/ariddell/lda](https://github.com/ariddell/lda)
[lda: Topic modeling with latent Dirichlet Allocation](http://pythonhosted.org/lda/)
[Getting started with Latent Dirichlet Allocation in Python - sandbox](http://chrisstrelioff.ws/sandbox/2014/11/13/getting_started_with_latent_dirichlet_allocation_in_python.html)
[[翻译] 在Python中使用LDA处理文本 - letiantian](http://my.oschina.net/letiantian/blog/616413?fromerr=ThbaouNJ)
[文本分析之TFIDF/LDA/Word2vec实践 - vs412237401](http://blog.csdn.net/vs412237401/article/details/50628239)


### 1.载入数据

```python
import numpy as np
import lda
import lda.datasets
# document-term matrix
X = lda.datasets.load_reuters()
print("type(X): {}".format(type(X)))
print("shape: {}\n".format(X.shape))
print(X[:5, :5])
# the vocab
vocab = lda.datasets.load_reuters_vocab()
print("type(vocab): {}".format(type(vocab)))
print("len(vocab): {}\n".format(len(vocab)))
print(vocab[:5])
# titles for each story
titles = lda.datasets.load_reuters_titles()
print("type(titles): {}".format(type(titles)))
print("len(titles): {}\n".format(len(titles)))
print(titles[:5])
```
载入LDA包数据集后，输出如下所示：
X矩阵为395*4258，共395个文档，4258个单词，主要用于计算每行文档单词出现的次数（词频），然后输出X[5,5]矩阵；
vocab为具体的单词，共4258个，它对应X的一行数据，其中输出的前5个单词，X中第0列对应church，其值为词频；
titles为载入的文章标题，共395篇文章，同时输出0~4篇文章标题如下。
```python
type(X): <type 'numpy.ndarray'>
shape: (395L, 4258L)
[[ 1  0  1  0  0]
 [ 7  0  2  0  0]
 [ 0  0  0  1 10]
 [ 6  0  1  0  0]
 [ 0  0  0  2 14]]
type(vocab): <type 'tuple'>
len(vocab): 4258
('church', 'pope', 'years', 'people', 'mother')
type(titles): <type 'tuple'>
len(titles): 395
('0 UK: Prince Charles spearheads British royal revolution. LONDON 1996-08-20',
 '1 GERMANY: Historic Dresden church rising from WW2 ashes. DRESDEN, Germany 1996-08-21',
 "2 INDIA: Mother Teresa's condition said still unstable. CALCUTTA 1996-08-23",
 '3 UK: Palace warns British weekly over Charles pictures. LONDON 1996-08-25',
 '4 INDIA: Mother Teresa, slightly stronger, blesses nuns. CALCUTTA 1996-08-25')
```
From the above we can see that there are 395 news items (documents) and a vocabulary of size 4258. The document-term matrix, X, has a count
 of the number of occurences of each of the 4258 vocabulary words for each of the 395 documents.
下面是测试文档编号为0，单词编号为3117的数据，X[0,3117]：
```python
# X[0,3117] is the number of times that word 3117 occurs in document 0
doc_id = 0
word_id = 3117
print("doc id: {} word id: {}".format(doc_id, word_id))
print("-- count: {}".format(X[doc_id, word_id]))
print("-- word : {}".format(vocab[word_id]))
print("-- doc  : {}".format(titles[doc_id]))

'''输出
doc id: 0 word id: 3117
-- count: 2
-- word : heir-to-the-throne
-- doc  : 0 UK: Prince Charles spearheads British royal revolution. LONDON 1996-08-20
'''
```

### 2.训练模型
其中设置20个主题，500次迭代
```python
model = lda.LDA(n_topics=20, n_iter=500, random_state=1)
model.fit(X)          # model.fit_transform(X) is also available
```

### 3.主题-单词（topic-word）分布
代码如下所示，计算'church', 'pope', 'years'这三个单词在各个主题(n_topocs=20，共20个主题)中的比重，同时输出前5个主题的比重和，其值均为1。
```python
topic_word = model.topic_word_
print("type(topic_word): {}".format(type(topic_word)))
print("shape: {}".format(topic_word.shape))
print(vocab[:3])
print(topic_word[:, :3])
for n in range(5):
    sum_pr = sum(topic_word[n,:])
    print("topic: {} sum: {}".format(n, sum_pr))
```
输出结果如下：
```python
type(topic_word): <type 'numpy.ndarray'>
shape: (20L, 4258L)
('church', 'pope', 'years')
[[  2.72436509e-06   2.72436509e-06   2.72708945e-03]
 [  2.29518860e-02   1.08771556e-06   7.83263973e-03]
 [  3.97404221e-03   4.96135108e-06   2.98177200e-03]
 [  3.27374625e-03   2.72585033e-06   2.72585033e-06]
 [  8.26262882e-03   8.56893407e-02   1.61980569e-06]
 [  1.30107788e-02   2.95632328e-06   2.95632328e-06]
 [  2.80145003e-06   2.80145003e-06   2.80145003e-06]
 [  2.42858077e-02   4.66944966e-06   4.66944966e-06]
 [  6.84655429e-03   1.90129250e-06   6.84655429e-03]
 [  3.48361655e-06   3.48361655e-06   3.48361655e-06]
 [  2.98781661e-03   3.31611166e-06   3.31611166e-06]
 [  4.27062069e-06   4.27062069e-06   4.27062069e-06]
 [  1.50994982e-02   1.64107142e-06   1.64107142e-06]
 [  7.73480150e-07   7.73480150e-07   1.70946848e-02]
 [  2.82280146e-06   2.82280146e-06   2.82280146e-06]
 [  5.15309856e-06   5.15309856e-06   4.64294180e-03]
 [  3.41695768e-06   3.41695768e-06   3.41695768e-06]
 [  3.90980357e-02   1.70316633e-03   4.42279319e-03]
 [  2.39373034e-06   2.39373034e-06   2.39373034e-06]
 [  3.32493234e-06   3.32493234e-06   3.32493234e-06]]
topic: 0 sum: 1.0
topic: 1 sum: 1.0
topic: 2 sum: 1.0
topic: 3 sum: 1.0
topic: 4 sum: 1.0
```

### 4.计算各主题Top-N个单词
下面这部分代码是计算每个主题中的前5个单词
```python
n = 5
for i, topic_dist in enumerate(topic_word):
    topic_words = np.array(vocab)[np.argsort(topic_dist)][:-(n+1):-1]
    print('*Topic {}\n- {}'.format(i, ' '.join(topic_words)))
```
输出如下所示：
```python
*Topic 0
- government british minister west group
*Topic 1
- church first during people political
*Topic 2
- elvis king wright fans presley
*Topic 3
- yeltsin russian russia president kremlin
*Topic 4
- pope vatican paul surgery pontiff
*Topic 5
- family police miami versace cunanan
*Topic 6
- south simpson born york white
*Topic 7
- order church mother successor since
*Topic 8
- charles prince diana royal queen
*Topic 9
- film france french against actor
*Topic 10
- germany german war nazi christian
*Topic 11
- east prize peace timor quebec
*Topic 12
- n't told life people church
*Topic 13
- years world time year last
*Topic 14
- mother teresa heart charity calcutta
*Topic 15
- city salonika exhibition buddhist byzantine
*Topic 16
- music first people tour including
*Topic 17
- church catholic bernardin cardinal bishop
*Topic 18
- harriman clinton u.s churchill paris
*Topic 19
- century art million museum city
```

### 5.文档-主题（Document-Topic）分布
计算输入前10篇文章最可能的Topic
```python
doc_topic = model.doc_topic_
print("type(doc_topic): {}".format(type(doc_topic)))
print("shape: {}".format(doc_topic.shape))
for n in range(10):
    topic_most_pr = doc_topic[n].argmax()
    print("doc: {} topic: {}".format(n, topic_most_pr))
```
输出如下所示：
```python
type(doc_topic): <type 'numpy.ndarray'>
shape: (395L, 20L)
doc: 0 topic: 8
doc: 1 topic: 1
doc: 2 topic: 14
doc: 3 topic: 8
doc: 4 topic: 14
doc: 5 topic: 14
doc: 6 topic: 14
doc: 7 topic: 14
doc: 8 topic: 14
doc: 9 topic: 8
```

### 6.两种作图分析
详见英文原文，包括计算各个主题中单词权重分布的情况：
```python
import matplotlib.pyplot as plt
f, ax= plt.subplots(5, 1, figsize=(8, 6), sharex=True)
for i, k in enumerate([0, 5, 9, 14, 19]):
    ax[i].stem(topic_word[k,:], linefmt='b-',
               markerfmt='bo', basefmt='w-')
    ax[i].set_xlim(-50,4350)
    ax[i].set_ylim(0, 0.08)
    ax[i].set_ylabel("Prob")
    ax[i].set_title("topic {}".format(k))
ax[4].set_xlabel("word")
plt.tight_layout()
plt.show()
```
输出如下图所示：
![](https://img-blog.csdn.net/20160309030237091)
第二种作图是计算文档具体分布在那个主题，代码如下所示：
```python
import matplotlib.pyplot as plt
f, ax= plt.subplots(5, 1, figsize=(8, 6), sharex=True)
for i, k in enumerate([1, 3, 4, 8, 9]):
    ax[i].stem(doc_topic[k,:], linefmt='r-',
               markerfmt='ro', basefmt='w-')
    ax[i].set_xlim(-1, 21)
    ax[i].set_ylim(0, 1)
    ax[i].set_ylabel("Prob")
    ax[i].set_title("Document {}".format(k))
ax[4].set_xlabel("Topic")
plt.tight_layout()
plt.show()
```
输出结果如下图：
![](https://img-blog.csdn.net/20160309030702937)


## 三. 总结
这篇文章主要是对Python下LDA用法的入门介绍，下一篇文章将结合具体的txt文本内容进行分词处理、文档主题分布计算等。其中也会涉及Python计算词频tf和tfidf的方法。
由于使用fit()总报错“TypeError: Cannot cast array data from dtype('float64') to dtype('int64') according to the rule 'safe'”，后使用sklearn中计算词频TF方法：
[http://scikit-learn.org/stable/modules/feature_extraction.html\#text-feature-extraction](http://scikit-learn.org/stable/modules/feature_extraction.html#text-feature-extraction)
总之，希望文章对你有所帮助吧！尤其是刚刚接触机器学习、Sklearn、LDA的同学，毕竟我自己其实也只是一个门外汉，没有系统的学习过机器学习相关的内容，所以也非常理解那种不知道如何使用一种算法的过程，毕竟自己就是嘛，而当你熟练使用后才会觉得它非常简单，所以入门也是这篇文章的宗旨吧！
最后非常感谢上面提到的文章链接作者，感谢他们的分享。如果有不足之处，还请海涵~
(By:Eastmount 2016-03-17 深夜3点半[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))

