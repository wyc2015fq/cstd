# 二、回归——logistic regression实现多类别分类的实现 - Nicole的博客 - CSDN博客
2018年06月08日 13:03:52[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：2478标签：[多类别分类																[logistic regression](https://so.csdn.net/so/search/s.do?q=logistic regression&t=blog)](https://so.csdn.net/so/search/s.do?q=多类别分类&t=blog)
个人分类：[机器学习算法——回归](https://blog.csdn.net/weixin_39541558/article/category/7614265)
# 一、one-Versus-All，OVA方法
     一对所有（one-Versus-All，OVA），给定m个类，训练m个二元分类器(将选取任意一类，再将其它所有类看成是一类，构建一个两类分类器)。分类器j使类j的元组为正类，其余为负类，进行训练。为了对未知元组X进行分类，分类器作为一个组合分类器投票。例如，如果分类器j预测X为正类，则类j得到一票。如果他测得X为正类，则类j得到一票。如果测X为负类，则除j以外的每一个类都得到一票（相当于此类的票数减一）。得票最多的指派给X。这种方法简单有效，而且使用类似logistic这种有概率值大小可以比较的情况下，类边界其实是个有范围的值，可以增加正确率。而且当K(类别数量)很大时，通过投票的方式解决了一部分不平衡性问题。
# 二、官网代码实现
```python
###############################官网示例#######################################
#导入数据
from sklearn import linear_model, datasets  
# import some data to play with
iris = datasets.load_iris()
X = iris.data[:, :2]  # we only take the first two features.
Y = iris.target
print(type(X))
print(type(Y))
X.shape
Y.shape
logreg = linear_model.LogisticRegression(C=1e5)
a = logreg.fit(X, Y)
a.coef_     #返回参数的系数
a.predict(X)  #预测类别
a.predict_log_proba(X)   #预测logit概率，即sigmoid函数值，发生率取对数
a.predict_proba(X)       #预测概率P,时间发生的概率
a.score(X,Y)             #预测精度
#------------------------------------------------------------------------------
```
# 三、使用自编的函数
## 1.获取数据
```python
################获取数据########################################################
os.chdir("E:\python learning\Machine Learning in Action\machinelearninginaction\Ch05")  
#马有三种情况：“仍存活”，“已经死亡”，“已经安乐死”
def colicTest():
    frTrain = open('train_ova.txt'); frTest = open('test_ova.txt')
    trainingSet = []; trainingLabels = []
    
    #获取训练集的数据，并将其存放在list中
    for line in frTrain.readlines():
        currLine = line.strip().split('\t')
        lineArr =[]            #用于存放每一行的数据
        for i in range(21):    #这里的range(21)是为了循环每一列的值，总共有22列
            lineArr.append(float(currLine[i]))
        trainingSet.append(lineArr)
        trainingLabels.append(float(currLine[21]))
    
    testingSet = [];testingLabels=[]
    
    ##获取测试数据
    for line in frTest.readlines():
        currLine1 = line.strip().split('\t')
        lineArr1 =[]            #用于存放每一行的数据
        for i in range(21):    #这里的range(21)是为了循环每一列的值，总共有22列
            lineArr1.append(float(currLine1[i]))
        testingSet.append(lineArr1)
        testingLabels.append(float(currLine1[21]))
    return np.array(trainingSet),trainingLabels,np.array(testingSet),testingLabels
############################例子----多分类模型##################################
dataArr,labelMat,testdata,testlabel= colicTest()
```
## 2.训练模型
```python
##################################训练模型######################################
categorylabels = [0.0,1.0,2.0]   #类别标签  
def myweight(dataArr,labelMat,categorylabels):
    weight1 = list()
    for i in range(len(categorylabels )):#分成三类，生成三个labelMat，判断是是否和给定的类别标签相等，例如将所有的数据和类别一0.0比较，如果相等令其为1，否则为0
        labelMat1 = []  
        for j in range(len(labelMat)):   #把名称变成0或1的数字  
            if labelMat[j] == categorylabels[i]:  
                labelMat1.append(1)  
            else:  
                labelMat1.append(0)
        labelMat1 = np.asarray(labelMat1) #labelMat1为一个列表，每个元素存放着实际标签和categorylabels的对比 
        logreg = linear_model.LogisticRegression(C=1e5)
        a = logreg.fit(dataArr, labelMat1)
        weight1.append(list(a.coef_))
    return weight1
weight1 = myweight(dataArr,labelMat,categorylabels )
###求每个分类器的回归系数
weight1[0]   #标签为0的分类器
weight1[1]   #标签为1的分类器
weight1[2]   #标签为2的分类器
```
## 3.测试模型
```python
####定义sigmoid函数
import math
def sigmoid(inX):
    return 1.0/(1+math.exp(-inX))
def testlabel(dataArr,labelArr,weight):   #输入的数据均为数组
    initial_value = 0  
    list_length = len(labelArr)  
    h = [initial_value]*list_length  
    for j in range(len(labelArr)):
        voteResult = [0,0,0]    #初始化
        for i in range(3):
            h[j] = float(sigmoid(np.dot(dataArr[j],weight[i][0].T)))    #数组的点乘，得到训练结果
            if (h[j] > 0.5) and (h[j] <= 1):
                voteResult[i] = voteResult[i]+1+h[j]   #由于类别少，为了防止同票，投票数要加上概率值  
            elif (h[j] >= 0) and (h[j] <= 0.5):
                voteResult[i] = voteResult[i]-1+h[j]  
            else:  
                print ('Properbility wrong!')  
        h[j] = voteResult.index(max(voteResult))  
    return asarray(h)
mydata,mylabel,testdata,testlabel= colicTest()
h = testlabel(mydata,mylabel,weight1)
h
```
## 4.计算正确率
```python
########################################计算正确率#############################
def error(reallabel,predlabel):   #reallabel,predlabel分别为真实值和预测值
    error = 0.0
    for j in range(len(reallabel)):
        if predlabel[j] != reallabel[j]:
            error = error +1
    pro = 1 - error / len(reallabel)#正确率  
    return pro
error(mylabel,h)
```
源数据：[点击打开链接](https://download.csdn.net/my)   https://download.csdn.net/my
本文为作者原创，如要转载，请先取得联系！！
