# Sklearn学习之路（1）——从20newsgroups开始讲起 - 刘炫320的博客 - CSDN博客
2017年04月17日 15:19:37[刘炫320](https://me.csdn.net/qq_35082030)阅读数：8017
# 1. Sklearn 简介
Sklearn是一个机器学习的python库，里面包含了几乎所有常见的机器学习与数据挖掘的各种算法。
具体的，它常见的包括数据预处理（preprocessing）（正则化，归一化等），特征提取（feature_extraction）（TFIDF等），降维（decomposition）（PCA等），以及常见的机器学习算法（分类、聚类、回归），更特别的，它也包括了评估（混淆矩阵与PRF及Acc值）和参数优化等（GridSearchCV），甚至是交叉验证（cross_validation）等都包含在内，可谓是机器学习整个流程都有了。
# 2. 20newsgroups案例
下面我们从20新闻语料这个案例开始，熟悉这些我们应该会的部分。首先给出全部代码：
```python
from sklearn.datasets import fetch_20newsgroups
categories = ['alt.atheism', 'soc.religion.christian', 'comp.graphics', 'sci.med']
twenty_train = fetch_20newsgroups(subset='train', categories=categories, shuffle=True, random_state=42)
'''
这是开始提取特征，这里的特征是词频统计。
'''
from sklearn.feature_extraction.text import CountVectorizer
count_vect = CountVectorizer()
X_train_counts = count_vect.fit_transform(twenty_train.data)
'''
这是开始提取特征，这里的特征是TFIDF特征。
'''
from sklearn.feature_extraction.text import TfidfTransformer
tfidf_transformer = TfidfTransformer()
X_train_tfidf = tfidf_transformer.fit_transform(X_train_counts)
'''
使用朴素贝叶斯分类,并做出简单的预测
'''
from sklearn.naive_bayes import MultinomialNB
clf = MultinomialNB().fit(X_train_tfidf, twenty_train.target)
docs_new = ['God is love', 'OpenGL on the GPU is fast']
X_new_counts = count_vect.transform(docs_new)
X_new_tfidf = tfidf_transformer.transform(X_new_counts)
predicted = clf.predict(X_new_tfidf)
for doc, category in zip(docs_new, predicted):
    print('%r => %s' % (doc, twenty_train.target_names[category]))
'''
使用测试集来评估模型好坏。
'''
from sklearn import metrics
import numpy as np;
twenty_test = fetch_20newsgroups(subset='test',categories=categories, shuffle=True, random_state=42)
docs_test = twenty_test.data
X_test_counts = count_vect.transform(docs_test)
X_test_tfidf = tfidf_transformer.transform(X_test_counts)
predicted = clf.predict(X_test_tfidf)
print(metrics.classification_report(twenty_test.target, predicted,target_names=twenty_test.target_names))
print("accurary\t"+str(np.mean(predicted == twenty_test.target)))
```
这个案例是一个新闻标题分类的案例，NLPCC 2017 Shared Task也有一个类似的案例。因此我们先拿这个下手了。整个过程概括起来分为以下几步：
- 数据采集
- 特征提取
- 模型训练
- 模型评估
接下来我们对这4个部分的代码进行详细的讲解。
## 2.1 数据采集
从上面的代码中，我们可以看到获取数据很简单：
```python
from sklearn.datasets import fetch_20newsgroups
categories = ['alt.atheism', 'soc.religion.christian', 'comp.graphics', 'sci.med']
twenty_train = fetch_20newsgroups(subset='train', categories=categories, shuffle=True, random_state=42)
```
那么这里最重要的就是这个`fetch_20newsgroups`方法了，下面我们来详细讲解：
```python
##函数原型是这样的。
'''
fetch_20newsgroups(data_home=None,subset='train',categories=None,shuffle=True,random_state=42,remove=(),download_if_missing=True)
'''
'''
data_home指的是数据集的地址，如果默认的话，所有的数据都会在'~/scikit_learn_data'文件夹下.
subset就是train,test,all三种可选，分别对应训练集、测试集和所有样本。
categories:是指类别，如果指定类别，就会只提取出目标类，如果是默认，则是提取所有类别出来。
shuffle:是否打乱样本顺序，如果是相互独立的话。
random_state:打乱顺序的随机种子
remove:是一个元组，用来去除一些停用词的，例如标题引用之类的。
download_if_missing: 如果数据缺失，是否去下载。
'''
```
经过测试可知 
twenty_train.data是一个list类型，每一个元素是str类型，也就是一篇文章。 
twenty_train.target则是它的标签。 
这样数据集就基本采集好了。
## 2.2 特征提取
数据采集完成以后，就要开始提取特征了，我们这里使用的是TFIDF特征。
```python
'''
这是开始提取特征，这里的特征是词频统计。
'''
from sklearn.feature_extraction.text import CountVectorizer
count_vect = CountVectorizer()
X_train_counts = count_vect.fit_transform(twenty_train.data)
'''
这是开始提取特征，这里的特征是TFIDF特征。
'''
from sklearn.feature_extraction.text import TfidfTransformer
tfidf_transformer = TfidfTransformer()
X_train_tfidf = tfidf_transformer.fit_transform(X_train_counts)
'''
这是用来转换成矩阵的，里面还有几个能调节的参数tf最大值最小值，停用词等。
转换后的矩阵为系数矩阵，存储方式为（x,y） 目标值。
'''
```
## 2.3 模型训练
这里我们使用朴素贝叶斯来进行训练，而且使用的是默认值，官网上面有很多参数可以调节这个算法。
```python
'''
使用朴素贝叶斯分类,并做出简单的预测
'''
from sklearn.naive_bayes import MultinomialNB
#这是进行训练的过程。
clf = MultinomialNB().fit(X_train_tfidf, twenty_train.target)
docs_new = ['God is love', 'OpenGL on the GPU is fast']
X_new_counts = count_vect.transform(docs_new)
X_new_tfidf = tfidf_transformer.transform(X_new_counts)
#重要的一行在这里，这是进行预测的过程。
predicted = clf.predict(X_new_tfidf)
for doc, category in zip(docs_new, predicted):
    print('%r => %s' % (doc, twenty_train.target_names[category]))
```
其最终的测试结果我们也可以看到：
> 
‘God is love’ => soc.religion.christian 
  ‘OpenGL on the GPU is fast’ => comp.graphics
说明这个分类效果还是不错的，但是我们评估一个模型的好坏不能靠一个2个样例来决定。下面才是评估的正确方法。
## 2.4模型的评估
模型的评估一般使用PRF（精确率，召回率，F1值）和Acc值（准确值）来评估，因此我们使用以下代码：
```python
'''
使用测试集来评估模型好坏。
'''
from sklearn import metrics
import numpy as np;
twenty_test = fetch_20newsgroups(subset='test',categories=categories, shuffle=True, random_state=42)
docs_test = twenty_test.data
X_test_counts = count_vect.transform(docs_test)
X_test_tfidf = tfidf_transformer.transform(X_test_counts)
predicted = clf.predict(X_test_tfidf)
#使用metrics.classification_report方法可以轻松获取这些信息。
print(metrics.classification_report(twenty_test.target, predicted,target_names=twenty_test.target_names))
#使用这个方法可以比较两个target的差异。
print("accurary\t"+str(np.mean(predicted == twenty_test.target)))
```
# 3 小结
从这几个简单的代码里，我们大概获知了这么几个知识点：
- Sklearn的优势
- 一个机器学习的大致流程，当然这个案例会比较特殊，我们在以后的讲解中再详细介绍。
- 知道了如何获取sklearn自带的数据集
- 知道了如何进行TFIDF特征抽取
- 知道了如何进行朴素贝叶斯分类的学习和预测过程
- 知道了如何评估一个机器学习算法的优劣
相比较而言，收获还是挺多的，接下来我们会进一步认识Sklearn里的一些很重要的部分。
