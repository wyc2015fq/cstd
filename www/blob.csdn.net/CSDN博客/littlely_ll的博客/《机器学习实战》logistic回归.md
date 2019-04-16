# 《机器学习实战》logistic回归 - littlely_ll的博客 - CSDN博客





2017年01月23日 21:44:31[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：275







- [《机器学习实战》K近邻(KNN)分类](http://blog.csdn.net/littlely_ll/article/details/54290007)
- [《机器学习实战》决策树](http://blog.csdn.net/littlely_ll/article/details/54290610)
- [《机器学习实战》朴素贝叶斯（Naive Bayes）分类](http://blog.csdn.net/littlely_ll/article/details/54630182)
$\quad$上一章学习了朴素贝叶斯分类器，贝叶斯分类器在文本分类方面表现很好，贝叶斯分类器是基于概率的分类器，而本章的logistic回归也可以说是基于概率的分类，虽然其名为带“回归”两字，其实是一种分类器。Logistic回归采用的是sigmoid函数，其取值在0~1之间。在分类的时候，sigmoid函数值小于0.5时标志为0，大于0.5是标志为1。 
![sigmoid函数](https://img-blog.csdn.net/20170123213030794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl0dGxlbHlfbGw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
***$sigmoid函数$***

下面是实现的代码：
```python
##Logistic regression gradient ascent optimization functions
def loadDataSet():
    dataMat = []; labelMat = []
    fr = open(r'c:/Users/ll/Documents/testSet.txt')
    for line in fr.readlines():
        lineArr = line.strip().split()
        dataMat.append([1.0, float(lineArr[0]),float(lineArr[1])])
        labelMat.append(int(lineArr[2]))
    return dataMat, labelMat

def sigmoid(inX):
    return 1.0/(1+np.exp(-inX))

import numpy as np     
def gradAscent(dataMatIn, classLabels):
    dataMatrix = np.mat(dataMatIn)
    labelMat = np.mat(classLabels).transpose()
    m,n = np.shape(dataMatrix)
    alpha = 0.001
    maxCycles = 500
    weights = np.ones((n,1))
    for k in range(maxCycles):
        h = sigmod(dataMatrix * weights)
        error = (labelMat - h)
        weights = weights + alpha * dataMatrix.transpose() * error
    return weights

##plotting the logistic regression best-fit line and dataset    
def plotBestFit(wei):
    import matplotlib.pyplot as plt
    weights = wei.getA() # <-- 此处可以去掉,把函数参数wei改为weights
    dataMat, labelMat = loadDataSet()
    dataArr = np.array(dataMat)
    n = np.shape(dataArr)[0]
    xcord1 = []; ycord1 = []
    xcord2 = []; ycord2 = []
    for i in range(n):
        if int(labelMat[i]) == 1:
            xcord1.append(dataArr[i,1]); ycord1.append(dataArr[i,2])
        else:
            xcord2.append(dataArr[i,1]); ycord2.append(dataArr[i,2])
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(xcord1,ycord1,s=30,c='red',marker='s')
    ax.scatter(xcord2,ycord2,s=30,c='green')
    x = np.arange(-3.,3.,0.1)
    y = (-weights[0] - weights[1] * x)/weights[2]
    ax.plot(x,y)
    plt.xlabel('X1');plt.ylabel('X2')
    plt.show()

##Stochastic gradient ascent
def stocGradAscent(dataMatrix, classLabels):
    m,n = np.shape(dataMatrix)
    alpha = 0.01
    weights = np.ones(n)
    for i in range(m):
        h = sigmoid(sum(dataMatrix[i]*weights))
        error = classLabels[i] - h
        weights = weights + alpha * error * dataMatrix[i]
    return weights

##Modified stochastic gradient ascent
import random
def stocGradAscent1(dataMatrix,classLabels,numIter=150):
    m,n = np.shape(dataMatrix)
    weights = np.ones(n)
    for j in range(numIter): 
        dataIndex = range(m)
        for i in range(m):
            alpha = 4/(1.+j+i)+0.01
            randIndex = int(random.uniform(0,len(dataIndex)))
            h = sigmoid(sum(dataMatrix[randIndex]*weights))
            error = classLabels[randIndex] - h
            weights = weights + alpha *error *dataMatrix[randIndex]
            del(dataIndex[randIndex])
    return weights

##Logistic regression classification function
def classifyVector(inX, weights):
    prob = sigmoid(sum(inX*weights))
    if prob > 0.5: return 1.
    else: return 0.

def colicTest():
    frTrain = open(r'c:/Users/ll/Documents/horseColicTraining.txt')
    frTest = open(r'c:/Users/ll/Documents/horseColicTest.txt')
    trainingSet = []; trainingLabels = []
    for line in frTrain.readlines():
        currLine = line.strip().split('\t')
        lineArr = []
        for i in range(21):
            lineArr.append(float(currLine[i]))
        trainingSet.append(lineArr)
        trainingLabels.append(float(currLine[21]))
    trainWeights = stocGradAscent1(np.array(trainingSet),trainingLabels,500)
    errorCount = 0; numTestVec = 0.0
    for line in frTest.readlines():
        numTestVec += 1.0
        currLine = line.strip().split('\t')
        lineArr = []
        for i in range(21):
            lineArr.append(float(currLine[i]))
        if int(classifyVector(np.array(lineArr), trainWeights)) != int(currLine[21]):
            errorCount += 1
    errorRate = (float(errorCount)/numTestVec)
    print "the error rate of this test is: %f" % errorRate
    return errorRate

def multiTest():
    numTests = 10; errorSum = 0.0
    for k in range(numTests):
        errorSum += colicTest()
    print "after %d iterations, the average error rate is: %f" % (numTests, errorSum/float(numTests))
```

上述代码还是比较简单的，就不多介绍了。




