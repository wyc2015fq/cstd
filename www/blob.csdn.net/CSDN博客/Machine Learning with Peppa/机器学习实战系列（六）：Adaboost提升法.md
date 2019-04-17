# 机器学习实战系列（六）：Adaboost提升法 - Machine Learning with Peppa - CSDN博客





2018年07月13日 13:56:26[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：120标签：[adaboost																[机器学习](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)](https://so.csdn.net/so/search/s.do?q=adaboost&t=blog)
个人分类：[Data Science：机器学习																[Data Science：统计学习																[编程之美：Python](https://blog.csdn.net/qq_39521554/article/category/7392111)




课程的所有数据和代码在我的Github：[Machine learning in Action](https://github.com/Y1ran/Machine-Learning-in-Action-Python3)，目前刚开始做，有不对的欢迎指正，也欢迎大家star。除了 版本差异，代码里的部分函数以及代码范式也和原书不一样（因为作者的代码实在让人看的别扭，我改过后看起来舒服多了）。在这个系列之后，我还会写一个scikit-learn机器学习系列，因为在实现了源码之后，带大家看看SKT框架如何使用也是非常重要的。    

Adaboost提升算法是机器学习中很好用的两个算法之一，另一个是SVM支持向量机；机器学习面试中也会经常提问到Adaboost的一些原理；另外本文还介绍了一下非平衡分类问题的解决方案，这个问题在面试中也经常被提到，比如信用卡数据集中，失信的是少数，5：10000的情况下怎么准确分类？

## 元算法（集成算法）：

多个弱分类器的组合；弱分类器的准确率很低 50%接近随机了

这种组合可以是 不同算法 或 同一算法不同配置 或是 数据集的不同部分分配给不同分类器;

## bagging：

把原始数据集随机抽样成S个与原始数据集一样大新数据集(允许有重复值)，然后训练S个分类器，最后投票结果集成；

代表：随机森林



## boosting：



关注以后分类器错分的数据，而得到新的分类器；

代表：adaboost


bagging和boosting类似，都是抽样的方式构造多个数据集(特别适用于数据集有限的时候)，并且多个组合分类器的类型都相同，但bagging是串行的，下一个分类器在上一个分类器的基础上继续训练得到的，权重均等；而boosting关注的是错分的数据，错分的数据权重大；



## adaboost(adaptive boost)自适应提升算法



AdaBoost的一般流程如下所示：

（1）收集数据

（2）准备数据：依赖于所用的基分类器的类型，这里的是单层决策树，即树桩，该类型决策树可以处理任何类型的数据。

（3）分析数据

（4）训练算法：利用提供的数据集训练分类器

（5）测试算法：利用提供的测试数据集计算分类的错误率

（6）使用算法：算法的相关推广，满足实际的需要

原理：为每一个样本赋均等的权重(D = 1/n)，先用这个数据集训练第一个弱分类器，计算错误率，错误率是为了计算这个分类器最后投票的权重alpha，错分的样本权重提升，对分的样本权重降低。然后用这个数据集训练第二个若分类器，迭代到弱分类器错误率为0或迭代指定个数的弱分类器停止





## 构建决策树



```python
import copy
from numpy import *
from math import inf
import numpy as np
"""
#构建单层分类器
#单层分类器是基于最小加权分类错误率的树桩
#伪代码
#将最小错误率minError设为+∞
#对数据集中的每个特征(第一层特征)：
    #对每个步长(第二层特征)：
        #对每个不等号(第三层特征)：
            #建立一颗单层决策树并利用加权数据集对它进行测试
            #如果错误率低于minError，则将当前单层决策树设为最佳单层决策树
#返回最佳单层决策树
"""
def stumpClassify(dataMatrix,dimen,threshVal,threshIneq):#just classify the data
    retArray = ones((shape(dataMatrix)[0],1))
    if threshIneq == 'lt':
        retArray[dataMatrix[:,dimen] <= threshVal] = -1.0
    else:
        retArray[dataMatrix[:,dimen] > threshVal] = -1.0
    return retArray
        
def buildStump(dataArr,classLabels,D):
    dataMatrix = mat(dataArr); labelMat = mat(classLabels).T
    m,n = shape(dataMatrix)
    numSteps = 10.0; bestStump = {}; bestClasEst = mat(zeros((m,1)))
    minError = inf #init error sum, to +infinity
    for i in range(n):#loop over all dimensions
        rangeMin = dataMatrix[:,i].min(); rangeMax = dataMatrix[:,i].max();
        stepSize = (rangeMax-rangeMin)/numSteps
        for j in range(-1,int(numSteps)+1):#loop over all range in current dimension
            for inequal in ['lt', 'gt']: #go over less than and greater than
                threshVal = (rangeMin + float(j) * stepSize)
                predictedVals = stumpClassify(dataMatrix,i,threshVal,inequal)#call stump classify with i, j, lessThan
                errArr = mat(ones((m,1)))
                errArr[predictedVals == labelMat] = 0
                weightedError = D.T*errArr  #calc total error multiplied by D
                #print "split: dim %d, thresh %.2f, thresh ineqal: %s, the weighted error is %.3f" % (i, threshVal, inequal, weightedError)
                if weightedError < minError:
                    minError = weightedError
                    bestClasEst = predictedVals.copy()
                    bestStump['dim'] = i
                    bestStump['thresh'] = threshVal
                    bestStump['ineq'] = inequal
    return bestStump,minError,bestClasEst
```

## 训练ADABOOST

```python
from numpy import *
from Stump_classify import *
#adaBoost算法
#@dataArr：数据矩阵
#@classLabels:标签向量
#@numIt:迭代次数    
def adaBoostTrainDS(dataArr,classLabels,numIt=40):
    '''
    @adaBoost算法
    @dataArr：数据矩阵
    @classLabels:标签向量
    @numIt:迭代次数  
    '''
    #弱分类器相关信息列表
    weakClassArr=[]
    #获取数据集行数
    m=shape(dataArr)[0]
    #初始化权重向量的每一项值相等
    D=mat(ones((m,1))/m)
    #累计估计值向量
    aggClassEst=mat((m,1))
    #循环迭代次数
    for i in range(numIt):
        #根据当前数据集，标签及权重建立最佳单层决策树
        bestStump,error,classEst=buildStump(dataArr,classLabels,D)
        #打印权重向量
        print("D:",D.T)
        #求单层决策树的系数alpha
        alpha=float(0.5*log((1.0-error)/(max(error,1e-16))))
        #存储决策树的系数alpha到字典
        bestStump['alpha']=alpha
        #将该决策树存入列表
        weakClassArr.append(bestStump)
        #打印决策树的预测结果
        print("classEst:",classEst.T)
        #预测正确为exp(-alpha),预测错误为exp(alpha)
        #即增大分类错误样本的权重，减少分类正确的数据点权重
        expon=multiply(-1*alpha*mat(classLabels).T,classEst)
        #更新权值向量
        D=multiply(D,exp(expon))
        D=D/D.sum()
        #累加当前单层决策树的加权预测值
        aggClassEst = aggClassEst + alpha * classEst
        #aggClassEst = array(aggClassEst)
        print("aggClassEst",aggClassEst.T)
        #求出分类错的样本个数
        aggErrors=multiply(sign(aggClassEst)!=\
                    mat(classLabels).T,ones((m,1)))
        #计算错误率
        errorRate=aggErrors.sum()/m
        print("total error:",errorRate,"\n")
        #错误率为0.0退出循环
        if errorRate==0.0:break
    #返回弱分类器的组合列表
    return weakClassArr
```



## 利用ADA分类

```python
#测试adaBoost，adaBoost分类函数
#@datToClass:测试数据点
#@classifierArr：构建好的最终分类器
def adaClassify(datToClass,classifierArr):
    #构建数据向量或矩阵
    dataMatrix=mat(datToClass)
    #获取矩阵行数
    m=shape(dataMatrix)[0]
    #初始化最终分类器
    aggClassEst=mat(zeros((m,1)))
    #遍历分类器列表中的每一个弱分类器
    for i in range(len(classifierArr)):
        #每一个弱分类器对测试数据进行预测分类
        classEst=stumpClassify(dataMatrix,classifierArr[i]['dim'],\
                                classifierArr[i]['thresh'],
                                classifierArr[i]['ineq'])
        #对各个分类器的预测结果进行加权累加
        aggClassEst+=classifierArr[i]['alpha']*classEst
        print('aggClassEst',aggClassEst)
    #通过sign函数根据结果大于或小于0预测出+1或-1
    return sign(aggClassEst)

def loadDataSet(filename):
    #创建数据集矩阵，标签向量
    dataMat=[];labelMat=[]
    #获取特征数目(包括最后一类标签)
    #readline():读取文件的一行
    #readlines:读取整个文件所有行
    numFeat=len(open(filename).readline().split('\t'))
    #打开文件
    fr=open(filename)
    #遍历文本每一行
    for line in fr.readlines():
        lineArr=[]
        curLine=line.strip().split('\t')
        for i in range(numFeat-1):
            lineArr.append(float(curLine[i]))
        #数据矩阵
        dataMat.append(lineArr)
        #标签向量
        labelMat.append(float(curLine[-1]))
    return dataMat,labelMat

#训练和测试分类器
def classify():
    #利用训练集训练分类器
    datArr,labelArr=loadDataSet('horseColicTraining.txt')
    #得到训练好的分类器
    classifierArray=adaBoostTrainDS(datArr,labelArr,10)
    #利用测试集测试分类器的分类效果
    testArr,testLabelArr=loadDataSet('horseColicTest.txt')
    prediction=adaClassify(testArr,classifierArray)
    #输出错误率
    num=shape(mat(labelArr))[1]
    errArr=mat(ones((num,1)))
    error=errArr[prediction!=mat(testLabelArr).T].sum()
    print("the errorRate is: %.2f",errorRate=float(error)/float((num)))
```

## ROC曲线绘制

```python
def plotROC(predStrengths, classLabels):
    import matplotlib.pyplot as plt
    cur = (1.0,1.0) #cursor
    ySum = 0.0 #variable to calculate AUC
    numPosClas = sum(array(classLabels)==1.0)
    yStep = 1/float(numPosClas); xStep = 1/float(len(classLabels)-numPosClas)
    sortedIndicies = predStrengths.argsort()#get sorted index, it's reverse
    fig = plt.figure()
    fig.clf()
    ax = plt.subplot(111)
    #loop through all the values, drawing a line segment at each point
    for index in sortedIndicies.tolist()[0]:
        if classLabels[index] == 1.0:
            delX = 0; delY = yStep;
        else:
            delX = xStep; delY = 0;
            ySum += cur[1]
        #draw line from cur to (cur[0]-delX,cur[1]-delY)
        ax.plot([cur[0],cur[0]-delX],[cur[1],cur[1]-delY], c='b')
        cur = (cur[0]-delX,cur[1]-delY)
    ax.plot([0,1],[0,1],'b--')
    plt.xlabel('False positive rate'); plt.ylabel('True positive rate')
    plt.title('ROC curve for AdaBoost horse colic detection system')
    ax.axis([0,1,0,1])
    plt.show()
    print ("the Area Under the Curve is: ",ySum*xStep)
```

## 测试函数

```python
def loadSimpData():
    dataMat=matrix([[1. ,2.1],
        [2. ,1.1],
        [1.3,1. ],
        [1. ,1. ],
        [2. ,1. ]])
    classLabels=[1.0,1.0,-1.0,-1.0,1.0]
    return dataMat,classLabels

if __name__ == '__main__':
    data, label = loadSimpData()
    datArr,labelArr = loadDataSet('horseColicTraining.txt')
    classifierArr = adaBoostTrainDS(datArr,labelArr,9)
    testArr,testLabelArr = loadDataSet('horseColicTest.txt')
    prediciton = adaClassify(testArr,classifierArr)

    error = mat(ones((67,1)))
    error[prediciton != mat(testLabelArr ).T] .sum()
```
|分类器数目|训练错误率(%)|测试错误率(%)|
|----|----|----|
|1|0.28|0.27|
|10|0.23|0.24|
|50|0.19|0.21|
|100|0.19|0.22|
|500|0.16|0.25|
|1000|0.14|0.31|
|10000|0.11|0.33|](https://blog.csdn.net/qq_39521554/article/category/7361350)](https://blog.csdn.net/qq_39521554/article/category/7361355)





