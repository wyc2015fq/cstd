# 大话机器学习（二）--KNN - lyx的专栏 - CSDN博客





2016年02月01日 16:37:51[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1373








## 一、有监督与无监督学习

总体来说讲呢，机器学习又两种学习方法，一个叫有监督学习（Supervised），一种叫无监督学习（Unsupervised）。顾名思义啊，一个就是有人看着，一个就是没有。在机器学习中呢，就是有监督学习，会先告诉学习算法，我有200本书，这些是我喜欢的，那些是我觉得一般的，那些是我讨厌的。好，现在又给你一本书，请你告诉我，我对这本书的态度。

这样的学习过程就是有监督的。

无监督的就是直接给你200本书，告诉你，给我分成几类吧。分好后，你在给他一本书，问他，这本书和哪些是同一类？

大致就是这样的意思。

## 二、KNN

今天，我们就来看一个很简单的监督学习算法，KNN（k-Nearest Neighbor）。基本步骤就是这样:

1.把书分成n类，告诉算法每一本书分别属于哪一类

2.给算法一本新书

3.算法计算这本书和之前200本书的距离

4.取k个与这本书距离最近的已知类别的书。k本书中，哪种类别的书最多，那么算法就认为这本新书属于哪种类别。换句话说，这些已知类别的书在投票。

可见，这本书与200本书的距离计算是一个很重要的点。投票的方法也是很重要的点。上诉只是讲了最简单的投票方法而已，可以按照不同的方法计算权重。KNN更加纤细的一些概要可以看我转载的这一文章： http://blog.csdn.net/qtlyx/article/details/50618659

## 三、sklearn机器学习方法函数概要

咱们大概了解这一算法之后呢，看一下sklearn的函数。

从刚才的讲述中可以看出，算法的输入是样本（samples），就是我们说的两百本书；以及这些书对应的标签（labels）。把这两个告诉算法之后，就是学习完成了。

sklearn里面的机器学习方法函数有如下基本方法

### 1.设置。

这就是设置一个方法的参数，比如KNN算法，就可以设置距离计算的方法，欧式距离啊、明科夫斯基距离之类的；还有k的取值，我们要最邻近的10个样本还是15个；还有投票权重的计算；等等等等。

一般设置语句是这样的：

设置完成可用的机器学习方法 = 机器学习方法名（参数1，参数2，参数3.。。。。。。）**。**


### **2.训练**

**设置好参数，就是训练，有监督的放样本和标签，无监督的，只有样本。通常设置语句是这样的：**

**设置完成可用的机器学习方法.fit（训练样本，[标签]）**

**3.预测。**

**训练完之后就可以用这个东西来预测新的输入的类别了。通常设置语句是这样的：**

**设置完成可用的机器学习方法.predict(新样本)**

****



## 四、sklearn的KNN算法示例

1.数据

我们使用鸢尾花数据集，这是sklearn自带的数据集，也是数据挖掘领域里面相当有名的数据集。每一条记录都是一类鸢尾花，记录的内容是鸢尾花的属性，比如多高啊，花的直径啊这类。
![](https://img-blog.csdn.net/20160201154731954)



数据是这样的：

![](https://img-blog.csdn.net/20160201162512391)

这是数据集的内容，共150条，这里只截取了部分

![](https://img-blog.csdn.net/20160201162520751)


这是对应的150个类别。

2.提取训练数据集和测试数据集



```python
i = 0
list = []

for i in range(0,150):
    if i%3 != 0 :
        list.append(i)
```
上面的代码产生了一个0到149，不能被三整除的数，就是说，我们从150个数据中选出三分之一。、





```python
samples= iris.data[list]
lab = iris.target[list]
```
如此就提取出来了。



3.设置



```python
n_neighbors = 15
clf = neighbors.KNeighborsClassifier(n_neighbors, weights='uniform')
```
如此，设置了算法的参数，k取15，投票权重为最简单的，大家权重都一样。



4.训练



```python
clf.fit(samples,lab)
```
5测试





```python
clf.predict(iris.data[3])
clf.predict(iris.data[6])
clf.predict(iris.data[147])
```


将这数据和iris.target对应下表比较发现时一样的。

如：



![](https://img-blog.csdn.net/20160201163330060)

五.完整代码



```python
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 01 16:27:13 2016

@author: Luyixiao
"""
# -*- coding: utf-8 -*-


import numpy as np
from sklearn import neighbors, datasets

n_neighbors = 15
iris = datasets.load_iris()

i = 0
list = []
for i in range(0,150):
    if i%3 != 0 :
        list.append(i)
        
samples= iris.data[list]
lab = iris.target[list]

n_neighbors = 15
clf = neighbors.KNeighborsClassifier(n_neighbors, weights='uniform')
```
















