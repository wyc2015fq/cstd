# 标签二值化LabelBinarizer - 博客堂 - CSDN博客





2017年08月30日 18:36:42[最小森林](https://me.csdn.net/u012052268)阅读数：8734








对于标称型数据来说，preprocessing.LabelBinarizer是一个很好用的工具。比如可以把yes和no转化为0和1，或是把incident和normal转化为0和1。当然，对于两类以上的标签也是适用的。这里举一个简单的例子，说明将标签二值化以及其逆过程。

```python
# -*- coding: UTF-8 -*-
from sklearn import preprocessing
from sklearn import tree

# help(preprocessing.LabelBinarizer)#取消注释可以查看详细用法

# 特征矩阵
featureList=[[1,0],[1,1],[0,0],[0,1]]
# 标签矩阵
labelList=['yes', 'no', 'no', 'yes']
# 将标签矩阵二值化
lb = preprocessing.LabelBinarizer()
dummY=lb.fit_transform(labelList)
# print(dummY)
# 模型建立和训练
clf = tree.DecisionTreeClassifier()
clf = clf.fit(featureList, dummY)
p=clf.predict([[0,1]])
# print(p)#取消注释可以查看p的值

# 逆过程
yesORno=lb.inverse_transform(p)
print(yesORno)
```



