# 决策树算法实现应用【基于Python语言实现】 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月22日 17:52:04[QuJack](https://me.csdn.net/A784586)阅读数：835








**决策树算法应用【基于Python语言实现】**

**本文包括源码，数据提供，环境等；**




**1.需要Python基础**

**2.  Python机器学习的库：scikit-learn**

**scikit-learn****中关于****决策树的**介绍：http://scikit-learn.org/stable/modules/tree.html

      2.1： 特性：

> 
简单高效的数据挖掘和机器学习分析

对所有用户开放，根据不同需求高度可重用性

基于Numpy, SciPy和matplotlib

开源，商用级别：获得 BSD许可


     2.2 覆盖问题领域：**分类（classification), 回归（regression), 聚类（clustering), **

**降维(dimensionality reduction)，****模型选择(model selection), 预处理(preprocessing)**

**3. 使用用scikit-learn**

     安装scikit-learn方式: pip, easy_install, windows installer

     安装必要package：numpy， SciPy和matplotlib，可使用**Anaconda**(包含numpy, scipy等科学计算常用package）

     安装注意问题：Python解释器版本（2.7 or 3.4？）, 32-bit or 64-bit系统


**4.例子：**利用**决策树算法**，根据训练数据集，预测包含年龄，收入，是否是学生，信用记录的特征数据，判断该记录是否购买电脑？

![](https://img-blog.csdn.net/20170422223551197?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**代码实践：**

1.安装Python和机器学习库，和一些依赖包；

本人是直接安装了包含了众多包的Anaconda3 ，下载后再window7 64bit上双击安装即可；

Anaconda3较大，如果网速不好，可以从百度云下载地址：http://pan.baidu.com/s/1dFIfoYX

2.打开cmd 输入：pip list 可以查看到已经安装的包；

3. 在cmd中运行如下的Python程序：


```python
from sklearn.feature_extraction import DictVectorizer
import csv
from sklearn import tree
from sklearn import preprocessing
from sklearn.externals.six import StringIO

# Read in the csv file and put features into list of dict and list of class label
allElectronicsData = open(r'AllElectronics.csv', 'r')
reader = csv.reader(allElectronicsData)
#http://bugs.python.org/msg82661
#http://www.cnblogs.com/feichexia/archive/2012/10/30/2747225.html
#reader.next()改为reader.__next__() ，并且把open中的rb改为r
#https://docs.python.org/3/library/csv.html#csv.csvreader.__next__
headers = reader.__next__() 
print(headers)

featureList = []
labelList = []

for row in reader:
    labelList.append(row[len(row)-1])
    rowDict = {}
    for i in range(1, len(row)-1):
        rowDict[headers[i]] = row[i]
    featureList.append(rowDict)

print(featureList)

# Vetorize features
vec = DictVectorizer()
dummyX = vec.fit_transform(featureList) .toarray()

print("dummyX: " + str(dummyX))
print(vec.get_feature_names())

print("labelList: " + str(labelList))

# vectorize class labels
lb = preprocessing.LabelBinarizer()
dummyY = lb.fit_transform(labelList)
print("dummyY: " + str(dummyY))

# Using decision tree for classification
# clf = tree.DecisionTreeClassifier()
clf = tree.DecisionTreeClassifier(criterion='entropy')
clf = clf.fit(dummyX, dummyY)
print("clf: " + str(clf))


# Visualize model
with open("allElectronicInformationGainOri.dot", 'w') as f:
    f = tree.export_graphviz(clf, feature_names=vec.get_feature_names(), out_file=f)

oneRowX = dummyX[0, :]
print("oneRowX: " + str(oneRowX))

newRowX = oneRowX
newRowX[0] = 1
newRowX[2] = 0
print("newRowX: " + str(newRowX))

predictedY = clf.predict(newRowX)
print("predictedY: " + str(predictedY))
```
4.运行截图：【将上述代码和数据放在同一文件夹下，进入该文件夹下运行该文件】

![](https://img-blog.csdn.net/20170422233707285?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170422233715671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





运行后该目录下回生成一个文件allElectronicInformationGainOri.dot。

![](https://img-blog.csdn.net/20170422234729117?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


5.安装Graphviz - Graph Visualization Software  官网下载安装[http://www.graphviz.org/](http://www.graphviz.org/)


       安装后将安装路径C:\Graphviz2.38\bin配置到操作系统环境变量path中。

打开cmd,将allElectronicInformationGainOri.dot转化为pdf,可视化决策树；




转化dot文件至pdf可视化决策树：打开cmd输入如下命令：

dot -TpdfallElectronicInformationGainOri.dot
 -o output.pdf

打开pdf如下：

![](https://img-blog.csdn.net/20170423002326985?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



备注：1.配置path环境变量后任然提示dot不是内部命令，关闭cmd,重新打开即可；

   2.生成的pdf打不开，关闭dot文件所在文件夹，重新生成，重新找到pdf打开；




---------------------------------------------------------------------

附录：

其中CSV文件内容为：

![](https://img-blog.csdn.net/20170422234350695?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**附录：**

代码源码和数据集.留下邮箱，24h内发送到您邮箱。


将代码和数据下载后放在同一文件夹下，进入该文件夹下运行该文件！


































