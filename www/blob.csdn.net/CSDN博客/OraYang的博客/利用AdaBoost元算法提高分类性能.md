
# 利用AdaBoost元算法提高分类性能 - OraYang的博客 - CSDN博客

2017年08月07日 18:25:05[OraYang](https://me.csdn.net/u010665216)阅读数：714标签：[机器学习																](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)[分类																](https://so.csdn.net/so/search/s.do?q=分类&t=blog)[python																](https://so.csdn.net/so/search/s.do?q=python&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=分类&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)个人分类：[Machine_learning																](https://blog.csdn.net/u010665216/article/category/7066495)
[
																					](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)[
							](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)



本节内容
组合相似的分类器来提高分类性能
应用AdaBoost算法
处理非均衡分类问题
元算法（meta-algorithm）是对其他算法进行组合的一种方式。其中AdaBoost是最流行的元算法之一。本节首先讨论不同分类器的集成方法，主要关注boosting方法及其代表分类器Adaboost。接着建立一个单层决策树分类器，并将AdaBoost算法应用在上述单层决策树分类器上。本文将在一个难数据集上应用AdaBoost分类器，以了解该算法是如何迅速超越其他分类器的。最后，本文会讨论所有分类器都会遇到的通用问题：非均衡分类问题。
1 AdaBoost初探
优点：泛化错误率低，易编码，可以应用在大部分分类器上，无参数调整
缺点：对离群点敏感
适用数据类型：数值型和标称型数据
本文介绍两种基于同一分类器的多个不同实例的方法：1.自举汇聚法（bootstrap aggregating）,也称为bagging方法（随机森林是其中之一）2.boosting,boosting是一种与bagging很类似的技术，boosting是集中关注被已有分类器错分的那些数据来获得新的分类器，boosting方法拥有很多版本，本文只关注最流行的版本AdaBoost。
**AdaBoost 的一般流程**
![](https://img-blog.csdn.net/20170807182503028)
2 训练算法：基于错误率提升分类器性能
AdaBoost是adaptive boosting(自适应boosting)的缩写，其运行过程如下：训练数据中的每个样本，并赋予其一个权重，这些权重构成了向量D。一开始这些权重都初始化成相等的值。首先在训练器上训练一个弱分类器并计算该分类器的错误率，然后在同一数据集上再次训练弱分类器。在分类器的第二次训练当中，将会重新调整每个样本的权重，其中第一次分对的样本权重会降低，而第一次分错的样本的权重将会提高。为了从所有弱分类器中得到最终的分类结果，AdaBoost为每个分类器都分配了一个权重值alpha,这些alpha值是基于每个弱分类器的错误率进行计算的。其中错误率e定义为：
![](https://img-blog.csdn.net/20170807182503278)
而alpha的计算公式为
![](https://img-blog.csdn.net/20170807182503475)
AdaBoost算法流程如下图所示：
![](https://img-blog.csdn.net/20170807182503806)
计算出a值之后，可以对权重向量D进行更新D的计算方法如下。
某个样本被正确分类：
![](https://img-blog.csdn.net/20170807182504087)
某个样本被错误分类：
![](https://img-blog.csdn.net/20170807182504297)
在计算出D之后，AdaBoost又开始进入下一轮迭代，直到训练错误率为0或者弱分类器的数目达到用户的指定值为止。
3 基于单层决策树构建弱分类器
单层决策树（decision stump,也称决策树桩）是一种简单的决策树。在构建AdaBoost的代码时，我们将首先通过一个简单的数据集来确保在算法实现上一切就绪。然后，建立一个adaboost.py的新文件并添加如下代码：

```python
from numpy import *
def loadSimpData():
    datMat = matrix([[ 1. ,  2.1],
        [ 2. ,  1.1],
        [ 1.3,  1. ],
        [ 1. ,  1. ],
        [ 2. ,  1. ]])
    classLabels = [1.0, 1.0, -1.0, -1.0, 1.0]
    return datMat,classLabels
```

![](https://img-blog.csdn.net/20170807182504589)
有了数据就可以通过构建多个函数来建立单层决策树了
第一个函数将用于测试是否有某个值小于或者大于我们正在测试的阈值。第二个函数则更加复杂一些，它会在一个加权数据集中循环，并找到具有最低错误率的单层决策树。
该程序的伪代码如下：
![](https://img-blog.csdn.net/20170807182504791)
接下来开始构造这个函数代码如下：

```python
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

4 完整AdaBoost算法的实现
利用前面构建的单层决策树来实现完整的AdaBoost算法，整个算法的伪代码如下：
对每次迭代：
利用buildStump()函数找到最佳的单层决策树
将最佳单层决策树加入到单层决策数组
计算alpha
计算新的权重向量D
更新累计类别估计值
如果错误率等于0.0，则退出循环
具体代码如下：

```python
def adaBoostTrainDS(dataArr,classLabels,numIt=40):
    weakClassArr = []
    m = shape(dataArr)[0]
    D = mat(ones((m,1))/m)   #init D to all equal
    aggClassEst = mat(zeros((m,1)))
    for i in range(numIt):
        bestStump,error,classEst = buildStump(dataArr,classLabels,D)#build Stump
        #print "D:",D.T
        alpha = float(0.5*log((1.0-error)/max(error,1e-16)))#calc alpha, throw in max(error,eps) to account for error=0
        bestStump['alpha'] = alpha  
        weakClassArr.append(bestStump)                  #store Stump Params in Array
        #print "classEst: ",classEst.T
        expon = multiply(-1*alpha*mat(classLabels).T,classEst) #exponent for D calc, getting messy
        D = multiply(D,exp(expon))                              #Calc New D for next iteration
        D = D/D.sum()
        #calc training error of all classifiers, if this is 0 quit for loop early (use break)
        aggClassEst += alpha*classEst
        #print "aggClassEst: ",aggClassEst.T
        aggErrors = multiply(sign(aggClassEst) != mat(classLabels).T,ones((m,1)))
        errorRate = aggErrors.sum()/m
        print "total error: ",errorRate
        if errorRate == 0.0: break
    return weakClassArr,aggClassEst
```

