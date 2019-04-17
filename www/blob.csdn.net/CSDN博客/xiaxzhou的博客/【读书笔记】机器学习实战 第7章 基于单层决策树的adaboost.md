# 【读书笔记】机器学习实战 第7章 基于单层决策树的adaboost - xiaxzhou的博客 - CSDN博客





2017年06月05日 22:43:08[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：1754








# 机器学习实战

> 
第7章 集成学习


#### **bagging** : 基于数据随机重抽样的分类器构建方法

> 
方法：从原始数据集通过随机抽样选择s次后得到s个新数据集，然后基于每个采样集训练出一个基学习器，再将这些基学习器进行结合。


bagging通过**训练数据的扰动**提升性能

#### **随机森林**：在以决策树为基学习器构建bagging集成的基础上，加入随机属性选择

> 
方法：传统决策树在选择划分属性时时在当前节点的属性集合中选择最优属性，而在随机森林中，对基决策树的每个节点，先从该节点的属性集合中**随机选择k个属性**子集，再从子集中选择最优的属性用于划分。


随机森林中的基学习器的多样性不仅来自**样本的扰动**，还来自**属性的扰动**，使得最终集成的泛化性能通过个体学习器之间差异的增加进一步提升。

#### **boosting**：

> 
方法：先从初始训练集训练出基学习器，再根据基学习器的表现对 样本的分布进行调整，是的被分错的样本在后续受到更多的关注，然后基于调整后的样本分布训练下一个基学习器。

- 最著名的代表：**AdaBoost**（adaptive boosting）

训练数据中的每个样本，并赋予其一个权重，权重构成了向量D，一开始初始化为相同值。 

首先在训练数据上训练出一个弱分类器并计算该分类器的错误率，然后在同一数据集上再次训练弱分类器。 

在第二次训练过程中，将会重新调整每个样本的权重，分对的样本权重降低，分错的样本权重提高。 

每个分类器也分配一个基于当前分类器错误率计算的权重（alpha）用于最终分类： 


$e = \frac{错误分类样本数}{样本数}$


$alpha =\frac{1}{2} ln\frac{1-e}{e}$
计算出alpha后，对权重向量进行更新，以使得正确分类的样本权重降低，错误分类的样本权重增高：
- 正确分类： 


$D_i^{t+1} = \frac{D_i^te^{-alpha}}{Sum(D)}$
- 错误分类： 


$D_i^{t+1} = \frac{D_i^te^{alpha}}{Sum(D)}$

D为概率分布向量，Sum(D) = 1，故初始化为1/m。

bagging主要关注降低**方差**

boosting主要关注降低**偏差**

偏差，度量了学习算法的期望预测与真实结果之间的偏离程度，刻画了拟合能力

方差，度量了同样大小的训练集的变动所导致的学习性能的变化，刻画了数据扰动所造成的影响。

为了取得好的泛化性能，则需使偏差较小，即能够充分拟合数据，并且使方差较小，即使得数据扰动产生的影响小

参考：[http://blog.csdn.net/pipisorry/article/details/50638749](http://blog.csdn.net/pipisorry/article/details/50638749)

> 
**偏差刻画的是构建的模型和真实模型之间的差异。**例如数据集所反映的真实模型为二次模型，但是构建的是线性模型，则该模型的结果总是和真实值结果直接存在差异，这种差异是有构建的模型的不准确所导致的，即为偏置bias。

**方差刻画的是构建的模型自身的稳定性**。例如数据集本身是二次模型，但是构建的是三次模型，对于多个不同的训练集，可以得到多个不同的三次模型，那么对于一个固定的测试点，这多个不同的三次模型得到多个估计值，这些估计值之间的差异即为模型的方差。


第7章 7.2-7.5 基于单层决策树的adaboost

```python
#!/usr/bin/python
# -*- coding: utf-8 -*-

from numpy import *

def loadSimpData():
    datMat = matrix([[1.,2.1],
                     [2.,1.1],
                     [1.3,1.],
                     [1.,1.],
                     [2.,1.],
                     ])
    classLabels = [1.0,1.0,-1.0,-1.0,1.0]
    return datMat,classLabels

 # 单层决策树：树桩弱分类器
def stumpClassify(dataMatrix, dimen, threshVal, threshIneq): #输入：样本特征矩阵，指定特征，阈值，是否反转类别
    retArray = ones((shape(dataMatrix)[0], 1))
    if threshIneq == 'lt':
        retArray[dataMatrix[:, dimen] <= threshVal] = -1.0
    else:
        retArray[dataMatrix[:, dimen] > threshVal] = -1.0
    return retArray # 返回类别向量

# 遍历分类函数所有可能的输入 找到基于D的最佳单层决策树
def buildStump(dataArr, classLabels, D): # 输入：样本特征矩阵，样本标签，样本权重
    dataMatrix = mat(dataArr);
    labelMat = mat(classLabels).T
    m, n = shape(dataMatrix)
    numSteps = 10.0;  # 用于遍历特征的阈值
    bestStump = {};   # 记录最佳单层树桩的信息
    bestClasEst = mat(zeros((m, 1)))
    minError = inf  # 记录最小错误率
    for i in range(n):  # 遍历所有特征
        rangeMin = dataMatrix[:, i].min();
        rangeMax = dataMatrix[:, i].max();
        stepSize = (rangeMax - rangeMin) / numSteps
        for j in range(-1, int(numSteps) + 1):  # 遍历所有阈值
            for inequal in ['lt', 'gt']:        # 反转类别
                threshVal = (rangeMin + float(j) * stepSize)
                predictedVals = stumpClassify(dataMatrix, i, threshVal,inequal)  # 指定特征，阈值，类别判定 后 树桩进行分类
                errArr = mat(ones((m, 1)))
                errArr[predictedVals == labelMat] = 0
                weightedError = D.T * errArr     # 计算加权错误率
                #print "split: dim %d, thresh %.2f, thresh ineqal: %s, the weighted error is %.3f" % (i, threshVal, inequal, weightedError)
                if weightedError < minError: # 更新最佳树桩配置信息
                    minError = weightedError
                    bestClasEst = predictedVals.copy()
                    bestStump['dim'] = i
                    bestStump['thresh'] = threshVal
                    bestStump['ineq'] = inequal
    return bestStump, minError, bestClasEst# 返回决策树，最小错误率，及其预测的类别

# 基于单层决策树(decision stump :DS)的adaboost训练
def adaBoostTrainDS(dataArr,classLabels,numIt=40):# 输入：样本特征矩阵、样本标签、迭代次数
    weakClassArr = []           # 弱分类器数组
    m = shape(dataArr)[0]
    D = mat(ones((m,1))/m)      # 初始化权重
    aggClassEst = mat(zeros((m,1)))# 记录类别估计的累积值
    for i in range(numIt):      # 迭代
        bestStump,error,classEst = buildStump(dataArr,classLabels,D)# 建立单层决策树
        print "D:",D.T
        alpha = float(0.5*log((1.0-error)/max(error,1e-16)))    # 计算当前分类器的权重值
        bestStump['alpha'] = alpha
        weakClassArr.append(bestStump)                  # 存储当前分类器
        print "classEst: ",classEst.T
        expon = multiply(-1*alpha*mat(classLabels).T,classEst) # exponent for D calc, getting messy
        D = multiply(D,exp(expon))                             # 更新样本权重值
        D = D/D.sum()
        #calc training error of all classifiers, if this is 0 quit for loop early (use break)
        aggClassEst += alpha*classEst  # 更新当前组合分类器的决策
        print "aggClassEst: ",aggClassEst.T
        aggErrors = multiply(sign(aggClassEst) != mat(classLabels).T,ones((m,1)))
        errorRate = aggErrors.sum()/m
        print "total error: ",errorRate
        if errorRate == 0.0: break # 分类无误则提前结束迭代
    return weakClassArr,aggClassEst

# 利用训练得到的弱分类器组合成强分类器进行分类
def adaClassify(datToClass,classifierArr):# 输入：测试样本集，弱分类器数组
    dataMatrix = mat(datToClass)#do stuff similar to last aggClassEst in adaBoostTrainDS
    m = shape(dataMatrix)[0]
    aggClassEst = mat(zeros((m,1)))
    for i in range(len(classifierArr)):
        classEst = stumpClassify(dataMatrix,classifierArr[i]['dim'],classifierArr[i]['thresh'],classifierArr[i]['ineq']) # 每个弱分类器进行分类
        aggClassEst += classifierArr[i]['alpha']*classEst # 组合弱分类器分类结果
        print aggClassEst
    return sign(aggClassEst)

D = mat(ones((5,1))/5)
datMat,classLabels = loadSimpData()
#buildStump(datMat,classLabels,D)
classifierrr,agg = adaBoostTrainDS(datMat,classLabels)
re = adaClassify([0,0],classifierrr)
```




