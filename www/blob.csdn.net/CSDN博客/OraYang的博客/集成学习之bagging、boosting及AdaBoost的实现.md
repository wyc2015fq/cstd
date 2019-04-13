
# 集成学习之bagging、boosting及AdaBoost的实现 - OraYang的博客 - CSDN博客

2017年10月31日 10:22:12[OraYang](https://me.csdn.net/u010665216)阅读数：1115所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



本文所有代码都是基于python3.6的，数据及源码下载：[传送门](https://gitee.com/orayang_admin/AdaBoost/tree/master)
## 引言
前面博客分享，我们已经讲解了不少分类算法，有knn、决策树、朴素贝叶斯、逻辑回归、svm。我们知道，当做重要决定时，大家可能都会考虑吸取多个专家而不是一个人的意见。机器学习处理问题时同样如此。集成学习（ensemble learning)通过构建并结合多个学习器来完成学习任务，有时被称为多分类器学习系统、基于委员会的学习等。
## 个体与集成
下图显示出集成学习的一般结构：先产生一组“个体学习器”，再用某种策略将它们结合起来。
![ensemble](https://img-blog.csdn.net/20171031100837402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171031100837402?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我们前面已经分享了五种不同的分类算法，我们可以将其用不同的分类器组合起来，这种组合结果则被称为集成方法或者元算法。使用集成方法时会有多种形式：1.集成中只包含同种类型的个体学习器，这种个体学习器也被称为基学习器。2.集成中也可包含不同类型的个体学习器，这种异质集成的个体学习器是由不同的学习算法生成。
一般来说，集成学习通过将多个学习器进行结合，常可获得比单一学习器显著优越的泛化性能。但是从实际经验中发现，要获得好的集成，个体学习器应“好而不同”，即个体学习器要有一定的“准确性”，即学习器不能太坏，并且要有“多样性”，即学习器之间具有差异。
根据个体学习器的生成方式，目前的集成学习方法有两大代表：一个是bagging，一个是boosting。
## bagging
自举汇聚法，也称bagging方法，是一种基于数据随机重抽样的分类器构建方法。bagging原理如下：
> 给定包含m个样本的数据集，我们先随机取出一个样本放入采样集中，再把该样本放回初始数据集，使得下次采样时刻该样本仍有肯能被选中，这样经过m次随机采样操作，我们得到含m个样本的采样集，初始训练集中有的样本在采样集中多次出险，有的则从未出现。照这样，我们可采样出T个含m个训练样本的采样集。
bagging的特点
> 训练一个bagging集成与直接使用基学习算法训练一个学习器的复杂度同阶

> 与标准的adboost只适用于二分类任务不同的是，bagging能不经修改地用于多分类、回归等任务

> 由于自助采样过程的性质，包外样本可以用作包外估计，可用来辅助剪枝，减小过拟合风险

> 从偏差-方差角度看，bagging主要关注降低方差，因此它在不剪枝决策树、神经网络等易受样本扰动的学习器上效用更明显。

### 随机森林
随机森林是更先进的bagging方法。RF是在以决策树为基学习器构建bagging集成的基础上，进一步在决策树的训练过程中引入了随机属性选择。此处详情请戳：[RF](https://www.stat.berkeley.edu/~breiman/RandomForests/)
## boosting
boosting是一种与bagging很类似的技术。不论是boosting还是bagging，所使用的分类器的类型都是一致的。但是bagging是个体学习器间不存在强依赖关系、可同时生成的并行化方法；boosting是个体学习器间存在强依赖关系、必须串行生成的序列化方法。
boosting不同的分类器是通过串行训练而获得的，每个新分类器都根据已训练出的分类器性能来进行训练。boosting是通过集中关注被已有分类器错分的那些数据来获得新的分类器。
由于boosting分类的结果是基于所有分类器的加权求和结果的，因此boosting与bagging不太一样。bagging中的分类器权重是相等的，而boosting中的分类器权重并不相等，每个权重代表其对应分类器在上一轮迭代中的成功度。
boosting族算法最具代表性的是AdaBoost。关于AdaBoost我前面有篇博客有分享：[利用AdaBoost元算法提高分类性能](http://blog.csdn.net/u010665216/article/details/76855242)。这里我将结合那篇博客的内容深入分析AdaBoost的原理与实现。
### 最小化指数损失函数
AdaBoost算法有很多的推导方式，比较容易理解的是基于“加性模型”，即基学习器的线性组合：

$$
H(x) = \sum_{t=1}^{T}\alpha_th_t(x)
$$
来最小化指数损失函数，中间推导不作详细介绍。
AdaBoost是adaptive boosting的缩写，其运行过程如下：
> 训练数据中的每个样本，并赋予其一个权重，这些权重构成了向量D。一开始这些权重全部被初始化成相等的值。首先在训练数据上训练出一个弱分类器并计算该分类器的错误率，然后在同一数据集上再次训练弱分类器。在分类器的第二次训练中，将会重新调整每个样本的权重，其中第一次分对的样本权重将会降低，而第二次分错的样本权重将会提高。为了从所有弱分类器中得到最终的分类结果，AdaBoost为每个分类器都分配了一个权重值alpha，这些alpha值是基于每个弱分类器的错误率进行计算的。
我们定义弱分类器的错误率为：

$$
\varepsilon = \frac{未正确分类的样本数}{所有样本数}
$$
通过最小化损失函数，求得AdaBoost给每个分类器分配的权重值alpha，公式如下：

$$
\alpha = \frac{1}{2}ln(\frac{1-\varepsilon_t}{\varepsilon_t})
$$
AdaBoost算法的流程如下图所示：
![adaboost](https://img-blog.csdn.net/20171031124352687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171031124352687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如上图所示：左边是数据集，其中直方图的不同宽度表示每个样例上的不同权重。经过一个分类器之后，加权的预测结果会通过三角形中的alpha值进行加权。每个三角形中输出的加权结果在圆形中求和，从而得到最终的输出结果。
计算出alpha值后可以对权重向量D进行更新，以使得那些正确分类的样本的权重降低，而错分样本的权重升高。D的计算方法如下。
如果某个样本被正确分类，那么该样本的权重更改为：

$$
D_i^{(t+1)} = \frac{D_i^{(t)}e^{-\alpha}}{Sum(D)}
$$
如果某个样本被错分，那么该样本的权重更改为：

$$
D_i^{(t+1)} = \frac{D_i^{(t)}e^{\alpha}}{Sum(D)}
$$
在计算出D之后，AdaBoost又开始进入下一轮迭代。AdaBoost会不断地重复训练和调整权重的过程，直到训练错误率为0或者弱分类器的数目达到用户的指定值为止。
## AdaBoost的实现
### 基于单层决策树构建弱分类器
单层决策树是一种简单的决策树。前面我们已经介绍了决策树的工作原理，接下来构建一个单层决策树，而它仅仅基于单个决策特征来做决策。由于这棵树只有一次分裂过程，因此它实际上就是一个树桩。因此也被称为决策树桩。
![stump](https://img-blog.csdn.net/20171031130347585?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171031130347585?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如上图所示，我们希望从某个坐标轴上选择一个值来将上图中的所有圆形点和方形点分开，这显然是不可能的。这就是单层决策树难以处理的一个著名的问题。通过使用多棵单层决策树，我们就能构建出对该数据集完全正确分类的分类器。
单层决策树的伪代码如下所示：
![stump](https://img-blog.csdn.net/20171031131055202?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171031131055202?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上述伪代码的核心思想就是寻找具有最低错误率的单层决策树。
代码如下：
```python
def
```
```python
stumpClassify
```
```python
(dataMatrix,dimen,threshVal,threshIneq)
```
```python
:
```
```python
#just classify the data
```
```python
retArray = ones((shape(dataMatrix)[
```
```python
0
```
```python
],
```
```python
1
```
```python
))
```
```python
if
```
```python
threshIneq ==
```
```python
'lt'
```
```python
:
        retArray[dataMatrix[:,dimen] <= threshVal] = -
```
```python
1.0
```
```python
else
```
```python
:
        retArray[dataMatrix[:,dimen] > threshVal] = -
```
```python
1.0
```
```python
return
```
```python
retArray
```
```python
def
```
```python
buildStump
```
```python
(dataArr,classLabels,D)
```
```python
:
```
```python
dataMatrix = mat(dataArr); labelMat = mat(classLabels).T
    m,n = shape(dataMatrix)
    numSteps =
```
```python
10.0
```
```python
; bestStump = {}; bestClasEst = mat(zeros((m,
```
```python
1
```
```python
)))
    minError = inf
```
```python
#init error sum, to +infinity
```
```python
for
```
```python
i
```
```python
in
```
```python
range(n):
```
```python
#loop over all dimensions
```
```python
rangeMin = dataMatrix[:,i].min(); rangeMax = dataMatrix[:,i].max();
        stepSize = (rangeMax-rangeMin)/numSteps
```
```python
for
```
```python
j
```
```python
in
```
```python
range(-
```
```python
1
```
```python
,int(numSteps)+
```
```python
1
```
```python
):
```
```python
#loop over all range in current dimension
```
```python
for
```
```python
inequal
```
```python
in
```
```python
[
```
```python
'lt'
```
```python
,
```
```python
'gt'
```
```python
]:
```
```python
#go over less than and greater than
```
```python
threshVal = (rangeMin + float(j) * stepSize)
                predictedVals = stumpClassify(dataMatrix,i,threshVal,inequal)
```
```python
#call stump classify with i, j, lessThan
```
```python
errArr = mat(ones((m,
```
```python
1
```
```python
)))
                errArr[predictedVals == labelMat] =
```
```python
0
```
```python
weightedError = D.T*errArr
```
```python
#calc total error multiplied by D
```
```python
#print "split: dim %d, thresh %.2f, thresh ineqal: %s, the weighted error is %.3f" % (i, threshVal, inequal, weightedError)
```
```python
if
```
```python
weightedError < minError:
                    minError = weightedError
                    bestClasEst = predictedVals.copy()
                    bestStump[
```
```python
'dim'
```
```python
] = i
                    bestStump[
```
```python
'thresh'
```
```python
] = threshVal
                    bestStump[
```
```python
'ineq'
```
```python
] = inequal
```
```python
return
```
```python
bestStump,minError,bestClasEst
```
上述代码权重错误weightedError是AdaBoost和分类器交互的地方。构建决策树桩的算法核心就是在一个加权的数据集中循环，然后找到具有最低错误率的单层决策树。
到目前为止我们已经构建了一个决策树桩，接下来我们就通过使用多个弱分类器来构建AdaBoost代码。
### 基于决策树桩的AdaBoost的构建
#### AdaBoost训练
AdaBoost训练的伪代码构造如下：
![adaboost](https://img-blog.csdn.net/20171031140809738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171031140809738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码如下：
[

](https://img-blog.csdn.net/20171031140809738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
def
```
```python
adaBoostTrainDS
```
```python
(dataArr,classLabels,numIt=
```
```python
40
```
```python
)
```
```python
:
```
```python
weakClassArr = []
    m = shape(dataArr)[
```
```python
0
```
```python
]
    D = mat(ones((m,
```
```python
1
```
```python
))/m)
```
```python
#init D to all equal
```
```python
aggClassEst = mat(zeros((m,
```
```python
1
```
```python
)))
```
```python
for
```
```python
i
```
```python
in
```
```python
range(numIt):
        bestStump,error,classEst = buildStump(dataArr,classLabels,D)
```
```python
#build Stump
```
```python
#print "D:",D.T
```
```python
alpha = float(
```
```python
0.5
```
```python
*log((
```
```python
1.0
```
```python
-error)/max(error,
```
```python
1e-16
```
```python
)))
```
```python
#calc alpha, throw in max(error,eps) to account for error=0
```
```python
bestStump[
```
```python
'alpha'
```
```python
] = alpha  
        weakClassArr.append(bestStump)
```
```python
#store Stump Params in Array
```
```python
#print "classEst: ",classEst.T
```
```python
expon = multiply(-
```
```python
1
```
```python
*alpha*mat(classLabels).T,classEst)
```
```python
#exponent for D calc, getting messy
```
```python
D = multiply(D,exp(expon))
```
```python
#Calc New D for next iteration
```
```python
D = D/D.sum()
```
```python
#calc training error of all classifiers, if this is 0 quit for loop early (use break)
```
```python
aggClassEst += alpha*classEst
```
```python
#print "aggClassEst: ",aggClassEst.T
```
```python
aggErrors = multiply(sign(aggClassEst) != mat(classLabels).T,ones((m,
```
```python
1
```
```python
)))
        errorRate = aggErrors.sum()/m
```
```python
print
```
```python
"total error: "
```
```python
,errorRate
```
```python
if
```
```python
errorRate ==
```
```python
0.0
```
```python
:
```
```python
break
```
```python
return
```
```python
weakClassArr,aggClassEst
```
[

](https://img-blog.csdn.net/20171031140809738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### AdaBoost测试
[
](https://img-blog.csdn.net/20171031140809738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)这里我们只是简单地应用了单层决策树。输出的估计类别值乘上该单层决策树的alpha权重，然后进行累加，就完成了分类过程。代码如下：
[

](https://img-blog.csdn.net/20171031140809738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
def
```
```python
adaClassify
```
```python
(datToClass,classifierArr)
```
```python
:
```
```python
dataMatrix = mat(datToClass)
```
```python
#do stuff similar to last aggClassEst in adaBoostTrainDS
```
```python
m = shape(dataMatrix)[
```
```python
0
```
```python
]
    aggClassEst = mat(zeros((m,
```
```python
1
```
```python
)))
```
```python
for
```
```python
i
```
```python
in
```
```python
range(len(classifierArr)):
        classEst = stumpClassify(dataMatrix,classifierArr[i][
```
```python
'dim'
```
```python
],\
                                 classifierArr[i][
```
```python
'thresh'
```
```python
],\
                                 classifierArr[i][
```
```python
'ineq'
```
```python
])
```
```python
#call stump classify
```
```python
aggClassEst += classifierArr[i][
```
```python
'alpha'
```
```python
]*classEst
```
```python
print
```
```python
aggClassEst
```
```python
return
```
```python
sign(aggClassEst)
```
[

](https://img-blog.csdn.net/20171031140809738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### Adaboost应用
[
](https://img-blog.csdn.net/20171031140809738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)我们应用上图方形与圆形数据点数据，来进行分类。
首先我们看一下训练数据：
![train](https://img-blog.csdn.net/20171031151803911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171031151803911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
然后看一下训练结果：
![train_result](https://img-blog.csdn.net/20171031151858913?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171031151858913?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里训练了三个弱分类器。
最后看一下测试结果，这里我们选择了两个训练数据（0，0）；（5，5）：
![result](https://img-blog.csdn.net/20171031152120000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 总结
本次分享介绍了两种集成方法：bagging，boosting。在bagging中，是通过随机抽样的替换方式得到与原始数据集规模一样的数据集。boosting比bagging思想更进一步，在数据集上顺序应用了多个不同的分类器。本文后半部分重点讲述了AdaBoost的简化版实现方法，AdaBoost函数可以应用于任意分类器，只要该分类器能够处理加权数据即可。

[
  ](https://img-blog.csdn.net/20171031151858913?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)