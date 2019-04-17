# 《统计学习方法》学习笔记（6）-- 决策树-附代码（sklearn） - 蓁蓁尔的博客 - CSDN博客





2017年04月25日 17:09:38[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：660








决策树，**特征选择**的三个准则：信息增益（ID3），信息增益比（C4.5），基尼系数（CART）。**决策树的生成，决策树的剪枝**。 
![这里写图片描述](https://img-blog.csdn.net/20170425170831742?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170425170844562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170425170854133?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20170425170904109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

代码： 

数据：decision tree.csv

```
RID,age,income,student,credit_rating,class_buys_computer
1,youth,high,no,fair,no
2,youth,high,no,excellent,no
3,middle_aged,high,no,fair,yes
4,senior,medium,no,fair,yes
5,senior,low,yes,fair,yes
6,senior,low,yes,excellent,no
7,middle_aged,low,yes,excellent,yes
8,youth,medium,no,fair,no
9,youth,low,yes,fair,yes
10,senior,medium,yes,fair,yes
11,youth,medium,yes,excellent,yes
12,middle_aged,medium,no,excellent,yes
13,middle_aged,high,yes,fair,yes
14,senior,medium,no,excellent,no
```

代码：

```python
# coding=utf-8
from sklearn.feature_extraction import DictVectorizer
import csv
from sklearn import tree
from sklearn import preprocessing
from sklearn.externals.six import StringIO

# Read in the csv file and put features into list of dict and list of class label
allElectronicsData = open(r'decision_tree.csv', 'rb')
reader = csv.reader(allElectronicsData) # 按行读取内容
headers = reader.next() # 内容的第一行
#
# print(headers)

featureList = []
labelList = []

for row in reader:
    labelList.append(row[len(row)-1]) # 取每一行的最后一个值
    rowDict = {}
    for i in range(1, len(row)-1):
        rowDict[headers[i]] = row[i]
    featureList.append(rowDict)

# print(featureList) # 如下面每一个字典是数据文件中的一行
# # [{'credit_rating': 'fair', 'age': 'youth', 'student': 'no', 'income': 'high'},
# # {'credit_rating': 'excellent', 'age': 'youth', 'student': 'no', 'income': 'high'},。。。。

# Vetorize features
vec = DictVectorizer() # sklearn 中提供了一个工具，可以将包含字典类型的list直接转化为数值型的数据。
dummyX = vec.fit_transform(featureList) .toarray()

# print("dummyX: " + str(dummyX))
# print(vec.get_feature_names())

# print("labelList: " + str(labelList))
# vectorize class labels
lb = preprocessing.LabelBinarizer()

dummyY = lb.fit_transform(labelList)
print("dummyY: " + str(dummyY))

# Using decision tree for classification
# clf = tree.DecisionTreeClassifier()
clf = tree.DecisionTreeClassifier(criterion='entropy')
clf = clf.fit(dummyX, dummyY)
# print("clf: " + str(clf))

# Visualize model
with open("decision_tree.dot", 'w') as f:
    f = tree.export_graphviz(clf, feature_names=vec.get_feature_names(), out_file=f)
# graphviz 打印出这个tree来看看，打印的时候将转化前的feature_names找回来。
# 上面生成dot文件，可以将其转化为pdf文件可视化出来。
# 转化命令：dot -Tpdf decision_tree.dot -o decision_tree.pdf
oneRowX = dummyX[0, :]
print("oneRowX: " + str(oneRowX))

newRowX = oneRowX
newRowX[0] = 1
newRowX[2] = 0
print("newRowX: " + str(newRowX))

predictedY = clf.predict(newRowX)
print("predictedY: " + str(predictedY))
```




