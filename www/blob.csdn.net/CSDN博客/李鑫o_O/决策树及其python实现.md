# 决策树及其python实现 - 李鑫o_O - CSDN博客

置顶2016年03月10日 21:39:34[hustlx](https://me.csdn.net/HUSTLX)阅读数：1979


## 1、剪枝

由于悲观错误剪枝 PEP (Pessimistic Error Pruning)、代价-复杂度剪枝 CCP
 (Cost-Complexity Pruning)、基于错误剪枝 EBP (Error-Based Pruning)、最小错误剪枝 MEP (Minimum Error Pruning)都是用于分类模型，故我们用降低错误剪枝 REP
 ( Reduced Error Pruning)方法进行剪枝。它的基本思路是:对于决策树 T 的每棵非叶子树s, 用叶子替代这棵子树.如果s 被叶子替代后形成的新树关于D 的误差等于或小于s关于D 所产生的误差, 则用叶子替代子树s。降低错误剪枝 REP优点是计算复杂性低、对未知示例预测偏差较小、自底向上处理。

剪枝的具体代码如下：

```python
myDat2=loadDataSet('ex2.txt')
myMat2=mat(myDat2)
myTree=createTree(myMat2, ops=(0,1))
myDatTest=loadDataSet('ex2test.txt')
myMat2Test=mat(myDatTest)
pruneTree=prune(myTree, myMat2Test)
#print "prune tree",pruneTree
yModelHat = createForeCast(pruneTree, myMat2[:,0])
print "model tree",corrcoef(yModelHat, myMat2[:,1],rowvar=0)[0,1]
其中prune函数如下：
def prune(tree, testData):
    if shape(testData)[0] == 0: return getMean(tree) #if we have no test data collapse the tree
    if (isTree(tree['right']) or isTree(tree['left'])):#if the branches are not trees try to prune them
        lSet, rSet = binSplitDataSet(testData, tree['spInd'], tree['spVal'])
    if isTree(tree['left']): tree['left'] = prune(tree['left'], lSet)
    if isTree(tree['right']): tree['right'] =  prune(tree['right'], rSet)
    #if they are now both leafs, see if we can merge them
    if not isTree(tree['left']) and not isTree(tree['right']):
        lSet, rSet = binSplitDataSet(testData, tree['spInd'], tree['spVal'])
        errorNoMerge = sum(power(lSet[:,-1] - tree['left'],2)) +\
            sum(power(rSet[:,-1] - tree['right'],2))
        treeMean = (tree['left']+tree['right'])/2.0
        errorMerge = sum(power(testData[:,-1] - treeMean,2))
        if errorMerge < errorNoMerge:
            print "merging"
            return treeMean
        else: return tree
else: return tree
```

剪枝后可以看到相关系数为99.17%。

![clip_image002](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092216835-1948376929.jpg)

## 2、回归树、模型树和普通回归树比较

以下分别是回归树模型树和普通回归树调用实现：

```python
trainMat=mat(loadDataSet('bikeSpeedVsIq_train.txt'))
testMat=mat(loadDataSet('bikeSpeedVsIq_test.txt'))
myRegTree=createTree(trainMat, ops=(1,20))
yRegHat = createForeCast(myRegTree, testMat[:,0])
print "regression tree",corrcoef(yRegHat, testMat[:,1],rowvar=0)[0,1]
myModelTree=createTree(trainMat, modelLeaf,modelErr,(1,20))
yModelHat = createForeCast(myModelTree, testMat[:,0],modelTreeEval)
print "model tree",corrcoef(yModelHat, testMat[:,1],rowvar=0)[0,1]
ws,X,Y=linearSolve(trainMat)
for i in range(shape(testMat)[0]):
    yModelHat[i]=testMat[i,0]*ws[1,0]+ws[0,0]
print "stardand",corrcoef(yModelHat, testMat[:,1],rowvar=0)[0,1]
```

三者比较结果如下：

![clip_image004](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092218194-63576670.jpg)

可以看到模型树的精确度最高，简单的线性回归效果最差。

## 3、Incremental Learning of Linear Model Trees

Duncan Potts 04年提出了基于线性模型树的增量学习算法，用于处理连续的流式输入数据模型而不必像以往批处理算法一样每次都需要运算整个训练集。作者说这种算法较以往的增量算法运用更少的样本可以得到对未知函数更好的近似，并且这种算法需要的参数更少。

### 3.1 模型树

由最小二乘法RLS可以得到：

![clip_image006](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092219304-1485490850.jpg)

![clip_image008](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092219772-143952795.jpg)

![clip_image010](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092220741-1732972304.jpg)

RSS就是最小的J值（有且仅当![clip_image012](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092221429-1626793012.png)）。

Python实现如下：

```python
def linearSolve(dataSet):   #helper function used in two places
    m,n = shape(dataSet)
    X = mat(ones((m,n))); Y = mat(ones((m,1)))#create a copy of data with 1 in 0th postion
    X[:,1:n] = dataSet[:,0:n-1]; Y = dataSet[:,-1]#and strip out Y
    xTx = X.T*X
    if linalg.det(xTx) == 0.0:
        raise NameError('This matrix is singular, cannot do inverse,\n\
        try increasing the second value of ops')
    ws = xTx.I * (X.T * Y)
    return ws,X,Y
def modelLeaf(dataSet):#create linear model and return coeficients
    ws,X,Y = linearSolve(dataSet)
    return ws
def modelErr(dataSet):
    ws,X,Y = linearSolve(dataSet)
    yHat = X * ws
return sum(power(Y - yHat,2))
```

### 3.2、增量模型树算法

作者在考虑一个有可能的分裂节点分成两个线性模型是否比用一个线性模型对函数![clip_image014](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092222600-41338584.png)的估计更好可以当成一个检验假设。假设H0
 : ![clip_image016](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092223210-535438079.png),如果假设成立则使用单个线性模型，其误差可以通过RLS计算得到RSS0，假设不成立分裂即使用两个线性模型，即![clip_image018](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092224007-2092710627.png)和![clip_image020](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092224835-16733317.png)。它们对应的样本量分别为N1和N2。其误差分别为RSS1和RSS2，一般情况下，一个数据集使用两个子模型来近似总会比使用单个模型来近似要好很多，所以RSS1+RSS2<RSS0.在这个假设之下，可以用一个F分布函数来表示一个数据集来自单个线性模型的可能性，表示如下：

![clip_image022](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092225632-1524410754.jpg)

其值越大，表明此数据集来自单个线性模型的可能性越大。同时由于数据集是流动的，所以在每一个节点的不同特征下需要考虑的分裂值也是动态变化的。所以在每个节点需要维护k(d-1)个潜在的分裂值，其中d表示特征数， k表示每个特征下需要考虑的分裂值。对于模型树的规模必须要加以限制:

![clip_image024](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092227350-89537450.jpg)

随着模型树的规模越大，δ值会越小，所以需要给值提供一个下限。对于F分布的p-value
 python里面scipy库有stats：

pvalue=stats.f.sf(F,N-2*d,d)，其中d,N-2d是自由度。其余代码如下。

### 3.3、Python实现具体如下：

```python
def train(dataSet,tolN,delta0,split):#assume dataSet is NumPy Mat so we can array filtering
    if len(set(dataSet[:,-1].T.tolist()[0])) == 1: #exit cond 1
        return None, modelLeaf(dataSet)
    #feat, val = chooseBestSplit(dataSet, leafType, errType, ops)#choose the best split
    N,d = shape(dataSet)
    #the choice of the best feature is driven by Reduction in RSS error from mean
    RSS0 = modelErr(dataSet)
    bestS = inf; bestIndex = 0; bestValue = 0;minp=inf;minRSS1=0;minRSS2=0;
    print '1ok'
    for featIndex in range(d-1):
        for splitVal in set(dataSet[:,featIndex]):
            mat0, mat1 = binSplitDataSet(dataSet, featIndex, splitVal)
            if (shape(mat0)[0] < tolN) or (shape(mat1)[0] < tolN): continue
            RSS1 = modelErr(mat0)
            RSS2 = modelErr(mat1)
            print '2ok'
            F=(RSS0-RSS1-RSS2)*(N-2*d)/((RSS1+RSS2)*d)
            pvalue=stats.f.sf(F,N-2*d,d)
            if pvalue<minp:
                N1,d1=shape(mat0)
                N2,d2=shape(mat1)
                minp=pvalue
                minRSS1=RSS1
                minRSS2=RSS2
                bestIndex = featIndex
                bestValue = splitVal
    if minRSS2==0:
        return None,modelLeaf(dataSet)
    delta=RSS0/(N-d)-(minRSS1+minRSS2)/(N1+N2-2*d)
    print minp,delta
    if minp > split or delta < delta0:
        return None,modelLeaf(dataSet)
    lSet, rSet = binSplitDataSet(dataSet, bestIndex, bestValue)
    if (shape(lSet)[0] < tolN) or (shape(rSet)[0] < tolN):  #exit cond 3
        return None,modelLeaf(dataSet)
    return bestIndex,bestValue
 
def createTrain(dataSet,tolN,delta0,split):#assume dataSet is NumPy Mat so we can array filtering
    feat, val = train(dataSet,tolN,delta0,split)#choose the best split
    if feat == None: return val #if the splitting hit a stop condition return val
    retTree = {}
    retTree['spInd'] = feat
    retTree['spVal'] = val
    lSet, rSet = binSplitDataSet(dataSet, feat, val)
    retTree['left'] = createTrain(lSet,tolN,delta0,split)
    retTree['right'] = createTrain(rSet,tolN,delta0,split)
return retTree
```

### 3.4、实验过程

我用类sin训练集做测试，为了方便做了以下界面：

![clip_image026](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092228085-1114201931.jpg)

可以看到当asplit过小或者δ0过大时都容易发生欠拟合：

![clip_image028](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092228991-747994136.jpg)![clip_image030](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092230007-757542398.jpg)

相反，当asplit过大或者δ0过小时都容易发生过拟合：

![clip_image032](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092230804-196265314.jpg)![clip_image034](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311092231554-1111783774.jpg)

