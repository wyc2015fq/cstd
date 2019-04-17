# 【读书笔记】机器学习实战 5.2节 logistics回归 - xiaxzhou的博客 - CSDN博客





2017年05月09日 21:43:48[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：392








# 机器学习实战

> 
5.2节 logistics回归 梯度上升


logistic回归的目的是寻找一个非线性函数Sigmoid的最佳拟合参数， 


$f(z)=\frac{1}{1+e^{-z}}$


$z=w_0*x_0+w_1*x_1+...=\mathrm{w}^T\mathrm{x}$
求解过程可以由最优化算法来完成

最常用的就是**梯度上升算法**，而梯度上升算法可以优化为**随机梯度上升算法**，后者是一个在线算法，在新数据到来时就完成参数更新，不需要重新计算所有数据。

```python
#!/usr/bin/python
# -*- coding: utf-8 -*-
from numpy import *

def loadDataSet():
    dataMat = [];labelMat = []
    fr = open('testSet.txt')
    for line in fr.readlines():
        lineArr = line.strip().split()  # 分割字符串
        dataMat.append([1.0,float(lineArr[0]),float(lineArr[1])])  # 字符转数字 特征值列表
        labelMat.append(int(lineArr[2]))  # 标签列表
    return dataMat,labelMat

def sigmoid(inX):
    return 1.0/(1+exp(-inX))

def gradAscent(dataMatIn,classLabels):
    dataMatrix = mat(dataMatIn)    # 转化numpy矩阵
    labelMat = mat(classLabels).T  # T：矩阵转置
    m,n = shape(dataMatrix)        # shape：矩阵尺寸
    alpha = 0.001                  # 步长
    maxCycles = 500                # 迭代次数
    weights = ones((n, 1))         # 权重
    for k in range(maxCycles):
        h = sigmoid(dataMatrix*weights)
        error = (labelMat - h)     # 梯度
        weights = weights + alpha*dataMatrix.T*error  # 梯度调整权重
    return weights

def plotBestFit(weights):
    import matplotlib.pyplot as plt
    dataMat,labelMat=loadDataSet()
    dataArr = array(dataMat)
    n = shape(dataArr)[0]
    xcord1 = []; ycord1 = []
    xcord2 = []; ycord2 = []
    for i in range(n):
        if int(labelMat[i])== 1:
            xcord1.append(dataArr[i,1]); ycord1.append(dataArr[i,2])
        else:
            xcord2.append(dataArr[i,1]); ycord2.append(dataArr[i,2])
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(xcord1, ycord1, s=30, c='red', marker='s')
    ax.scatter(xcord2, ycord2, s=30, c='green')
    x = arange(-3.0, 3.0, 0.1)
    y = (-weights[0]-weights[1]*x)/weights[2]
    ax.plot(x, y)
    plt.xlabel('X1'); plt.ylabel('X2');
    plt.show()


def stocGradAscent0(dataMatrix, classLabels):
    m,n = shape(dataMatrix)
    alpha = 0.01        # 步长
    weights = ones(n)   #initialize to all ones
    for i in range(m):
        h = sigmoid(sum(dataMatrix[i]*weights))
        error = classLabels[i] - h
        weights = weights + alpha * error * dataMatrix[i]
    return weights

def stocGradAscent1(dataMatrix, classLabels, numIter=150):
    m,n = shape(dataMatrix)
    weights = ones(n)   #initialize to all ones
    for j in range(numIter):  # 多次迭代
        dataIndex = range(m)
        for i in range(m):
            alpha = 4/(1.0+j+i)+0.0001    # 可变步长
            randIndex = int(random.uniform(0,len(dataIndex)))# 随机选取
            h = sigmoid(sum(dataMatrix[randIndex]*weights))
            error = classLabels[randIndex] - h
            weights = weights + alpha * error * dataMatrix[randIndex]
            del(dataIndex[randIndex])
    return weights

dataArr,labelMat = loadDataSet()
#weights = gradAscent(dataArr,labelMat)
#plotBestFit(weights.getA())

#weights = stocGradAscent0(array(dataArr),labelMat)
#plotBestFit(weights)

weights = stocGradAscent1(array(dataArr),labelMat)
plotBestFit(weights)
pass
```




