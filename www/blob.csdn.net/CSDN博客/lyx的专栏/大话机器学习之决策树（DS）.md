# 大话机器学习之决策树（DS） - lyx的专栏 - CSDN博客





2016年06月12日 15:20:06[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1058








      什么是决策树呢？其实很直观，这样的就是

![](https://img-blog.csdn.net/20160612144615853?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      不说了，先看数据：

![](https://img-blog.csdn.net/20160612144902700?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      这是一个医疗检测的数据，前面六个是指标，具体是什么其实没有意义，说的好像化验单上的那些医学术语你都知道似得。最后一个就是结果。我们就是要构建一颗决策树，根据前面六个指标，预测最后结果是消极还是积极。当然，码代码之前我们先要对数据做一点处理。

      对于后面这一行，我们当然是把消极变为0，积极变为1，结果大概呢就是这样的。so easy！

![](https://img-blog.csdn.net/20160612145415848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      对了，数据、还有接下来的代码，大家可以去我的github上面下载整个完整的工程。里面都有。地址在最后给出哦！

      然后就是写代码了。这次我们还是使用sklearn这个库，谁叫他这么好用呢。

#### 1.导入库



```python
import csv
from sklearn import tree 
import numpy as np
```

#### 2.读取数据



```python
def readData(path,ratio):
    reader = csv.reader(file(path, 'rb'))
    data = []
    flag = []
    dataTrain = []
    flagTrain = []   
    dataTest = []
    flagTest = [] 
    for line in reader:
        data.append([float(line[0]),float(line[1]),float(line[2]),float(line[3]),float(line[4]),float(line[5])])
        flag.append(float(line[6]))
    for i in range(0,int(len(data)*ratio)):
        dataTrain.append(data[i])
        flagTrain.append(flag[i])
    for i in range(int(len(data)*ratio),len(data)):
        dataTest.append(data[i])
        flagTest.append(flag[i])
    return dataTrain,flagTrain,dataTest,flagTest
```
      和之前的也没什么区别，不过受到前面ByPython数据的启发，在里面加了个内容，就是能够按照比例，自动产生训练数据和测试数据。

#### 3.测试



```python
path = 'I:/MLtrain/hData.csv'#设置文件的路径
X_train,Y_train,X_test,Y_test = readData(path,0.8)#读取数据，按照4:1的比例分割训练集和测试集
clf = tree.DecisionTreeClassifier()#生成一个DT 分类器
clf = clf.fit(X_train, Y_train)#训练这个DT
pre=clf.predict(X_test)
print np.sum(np.abs(pre - np.array(Y_test)))/len(pre)#输出错误率
```
      代码比较简单。最后的结果是0.08的错误率，也就是92%的准确率，相当不错哦！

#### 4.关于原理

      决策树的原理是什么呢？这里简单说一下。我们简化一下，考虑一个一个离散的情况，

![](https://img-blog.csdn.net/20160612150604564?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      是网上流传的很经典的大龄女性相亲决策树模型。为什么第一个节点是年龄而不是别的呢？我们按照怎么样的顺序去选择这些节点呢。

      一个节点选择的好坏使用信息熵来衡量的，如果一个节点一分类之后，发现把情况彻底分开了，那么这个就是最好的，因为每一个部分都很“纯”，不是0就是1.反过来，如果还是很杂，比如年龄三十以下和年龄三十以上两个部分分开之后，每一个部分还是混了很多不见和见的情况，那么就没什么意义。这种数据的“纯度”就是用信息熵来衡量的。这里就不给大家写什么公式了，有兴趣的可以找数据挖掘的书看看。原理就是这个样子的。

      大家可以去我的Github上面下载代码和数据：https://github.com/Luyixiao/DecisionTree




