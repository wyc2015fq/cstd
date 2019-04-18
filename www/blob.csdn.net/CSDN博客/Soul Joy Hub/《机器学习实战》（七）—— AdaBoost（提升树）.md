# 《机器学习实战》（七）—— AdaBoost（提升树） - Soul Joy Hub - CSDN博客

2017年08月17日 09:50:20[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1047
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



[http://blog.csdn.net/u011239443/article/details/77294201](http://blog.csdn.net/u011239443/article/details/77294201)

# AdaBoost

![](http://upload-images.jianshu.io/upload_images/1621805-1a0095c0c74605d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-5955a3d10097930c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-a8274ae5769ac878.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-242512a16ec6bcbb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 提升树

![](http://upload-images.jianshu.io/upload_images/1621805-bd8c82568e64945e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-4f9423f7f3385f45.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 例子

![](http://upload-images.jianshu.io/upload_images/1621805-63065112bb070b56.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

将“身体”设为A，“业务”设为B，“潜力”设为C。对该题做大致的求解：

![](http://upload-images.jianshu.io/upload_images/1621805-2ab7bb9242bf0cff.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

这里我们只计算到了f2，相信读者也知道如何继续往下计算。这里特征的取值较少，所以直接使用是否等于某个取值来作为分支条件。实际中，可以设置是否大于或者小于等于某个阈值来作为分支条件。接下来我们就来看看如何实现提升树。

# 实现

```python
# -*- coding: utf-8 -*-
from numpy import *

# 加载数据
def loadSimpData():
    datMat = matrix([[ 1. ,  2.1],
        [ 2. ,  1.1],
        [ 1.3,  1. ],
        [ 1. ,  1. ],
        [ 2. ,  1. ]])
    classLabels = [1.0, 1.0, -1.0, -1.0, 1.0]
    return datMat,classLabels

# 决策桩分类
# dimen : 选取的特征
# threshVal ： 特征的阈值
# threshInseq ： 判别大于或者小于等于该阈值
def stumpClassify(dataMat,dimen,threshVal,threshIneq):
    retArray = ones((shape(dataMat)[0],1))
    if threshIneq == 'lt':
        retArray[dataMat[:,dimen] <= threshVal] = -1.0
    else:
        retArray[dataMat[:,dimen] > threshVal] = -1.0
    return retArray

#  构建决策树桩
def buildStump(dataArr,classLabels,D):
    dataMat = mat(dataArr);labelMat = mat(classLabels).T
    m,n = shape(dataMat)
    numSteps = 10.0;bestStump = {};bestClassEst = mat(zeros((m,1)))
    minError = inf
    for i in range(n):
        rangeMin = dataMat[:,i].min();rangeMax = dataMat[:,i].max()
        stepSize = (rangeMax - rangeMin)/numSteps
        # 由于是 ‘<= threshVal’，所以要从-1开始，使得出现全都 > threshVal 的情况
        for j in range(-1,int(numSteps)+1):
            for inequal in ['lt','gt']:
                threshVal = rangeMin + j * stepSize
                predictedVals = stumpClassify(dataMat,i,threshVal,inequal)
                errArr = mat(ones((m,1)))
                errArr[predictedVals == labelMat] = 0
                weightedError = D.T * errArr

                if weightedError < minError:
                    minError = weightedError
                    bestClassEst = predictedVals.copy()
                    bestStump['dim'] = i
                    bestStump['thresh'] = threshVal
                    bestStump['ineq'] = inequal

    return bestStump,minError,bestClassEst

def adaBoostTrainDS(dataArr,classLabels,numIt = 40):
    # 保存不同的决策树桩
    weakClassArr = []
    m = shape(dataArr)[0]
    # 权值初始化为 1/m
    D = mat(ones((m,1))/m)
    aggClassEst = mat(zeros((m,1)))
    for i in range(numIt):
        bestStump,error,ClassEst = buildStump(dataArr,classLabels,D)
        # 见式 8.2
        alpha = float(0.5*log((1-error)/max(error,1e-16)))
        bestStump['alpha'] = alpha
        weakClassArr.append(bestStump)
        # 见式 8.4
        expon = multiply(-1*alpha*mat(classLabels).T,ClassEst)
        D = multiply(D,exp(expon))
        D = D/D.sum()
        # 见式 8.6
        aggClassEst += alpha*ClassEst
        aggErrors = multiply(sign(aggClassEst) != mat(classLabels).T,ones((m,1)))
        errorRate = aggErrors.sum()/m
        print ("error rate : ",errorRate)
        if errorRate == 0:
            break
    return weakClassArr

def adaClassify(dataToClass,classifierArr):
    dataMat = mat(dataToClass)
    m = shape(dataMat)[0]
    aggClassEst = mat(zeros((m,1)))
    for i in range(len(classifierArr)):
        classEst = stumpClassify(dataMat,classifierArr[i]['dim'],\
                                 classifierArr[i]['thresh'], \
                                 classifierArr[i]['ineq'])
        # 见式 8.7
        aggClassEst += classifierArr[i]['alpha']*classEst
        print aggClassEst
    return sign(aggClassEst)
```

## 测试

```python
import myAdaboost

dataMat,classLabels = myAdaboost.loadSimpData()

classifierArray = myAdaboost.adaBoostTrainDS(dataMat,classLabels,30)

print myAdaboost.adaClassify([0,0],classifierArray)
```

## 结果

```
('error rate : ', 0.20000000000000001)
('error rate : ', 0.20000000000000001)
('error rate : ', 0.0)
[[-0.69314718]]
[[-1.66610226]]
[[-2.56198199]]
[[-1.]]
```

![这里写图片描述](https://img-blog.csdn.net/20170817094922488?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

