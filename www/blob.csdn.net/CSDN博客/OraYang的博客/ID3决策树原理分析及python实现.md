
# ID3决策树原理分析及python实现 - OraYang的博客 - CSDN博客

2017年10月08日 09:08:53[OraYang](https://me.csdn.net/u010665216)阅读数：2488所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



**本文所有代码都是基于python3.6的，数据及源码下载：****[传送门](https://gitee.com/orayang_admin/ID3_decisiontree/tree/master)**
## 引言
今天我们来分享下[决策树](https://en.wikipedia.org/wiki/Decision_tree_learning)的相关算法，众所周知，树在我们现实生活中，有许多算法应用。对机器学习领域也有着极其广泛影响，主要涉及分类和回归。在决策分析中，一个决策树可以用于可视化和显示地表示决策和决策制定的过程。顾名思义，它使用树状的决策模型，这也是本次分享我们所关注的重点。
### 什么是决策树
那么到底什么才是决策树呢？我们采用[wiki](https://en.wikipedia.org/wiki/Decision_tree)上的一段定义：
> A decision tree is a flowchart-like structure in which each internal node represents a “test” on an attribute (e.g. whether a coin flip comes up heads or tails), each branch represents the outcome of the test, and each leaf node represents a class label (decision taken after computing all attributes). The paths from root to leaf represent classification rules.—-from wiki
简单点说就是，决策树是一种类似于流程图的结构，其中每个内部节点代表一个属性上的“测试”(例如，一个硬币是正面还是反面)，每个分支代表测试的结果，每个叶节点代表一个类标签(计算所有属性后做出的决定)。从根到叶的路径表示分类规则。为了进一步帮助大家了解决策树，我们用titanic数据来预测一个乘客是否能生存，我们选用三个属性的数据集,即性别、年龄和sibsp(配偶或子女的数量)。决策树算法结构如下：
![titanic](https://img-blog.csdn.net/20171008092243498?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171008092243498?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
决策过程：假如一个乘客数据如下所示（其中年龄被映射到0-10）：
[
](https://img-blog.csdn.net/20171008092243498?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)|Passengers|Age|Sex|sibsp|
|---|---|---|---|
|Jack|2.4|male|0|
[

](https://img-blog.csdn.net/20171008092243498?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)Jack首先判断性别男；然后判断年龄2.4；然后判断子女数0，最后决策生还！
根据上述例子我们发现，整个决策过程的最终结论也就是每个叶节点对应了我们的判定结果，例如”die”或“survived”；其中每个内部结点即判定问题都是某个属性的”测试“，例如：“is sex male?”,”is age > 9.5?”。
> 决策树学习的目的是为了产生一颗泛化能力强，即处理未见示例能力强的决策树。其基本流程遵循简单而直观的”分而治之“策略。

## 决策树的构造
现在我们已经大致了解了决策树的定义，及应用，接下来我们就来学习下如何构造决策树。周志华的西瓜书，一上来就扔给你一堆伪代码，我感觉这明显让大家读不懂嘛，本文将按照由浅入深地方式，给大家讲讲如何一步一步地来构造决策树。我们看决策树的定义可以发现，每个结点都有一个判断的过程，这个判断的过程就是对数据集的划分。
### 划分数据集1
划分数据集，我们遵循以下原则：**如果某个分支的数据属于同一类型，则无需进一步对数据进行划分；如果数据子集内的数据不属于同一类型，则需要重复划分数据子集的过程。**
创建分支的伪代码函数createBranch()如下所示：
![createBranch](https://img-blog.csdn.net/20171008130141887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171008130141887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上面的伪代码createBranch是一个递归函数，其中第四行是find the best feature to split the data。从上面的伪代码，我们知道了划分数据集的过程，那么我们在划分数据集的过程中，如何找到划分数据的最佳特征呢？
[

](https://img-blog.csdn.net/20171008130141887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 信息增益
[

](https://img-blog.csdn.net/20171008130141887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### 信息熵
[
](https://img-blog.csdn.net/20171008130141887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)划分数据集，一般而言，随着划分过程的不断进行，我们希望决策树的分支结点所包含的样本尽可能的属于一个类，即结点的”纯度“（purity）越来越高。总结下就是：划分数据集的大原则是将无序的数据变得更加有序。
划分数据集前后信息发生的变化称为信息增益，著名的[ID3决策树](https://en.wikipedia.org/wiki/ID3_algorithm)算法就是以信息增益为准则的。香农熵是度量信息增益的的一种常用度量方法（度量信息无序程度有两种方法，除了香农熵之外就是基尼不纯度，[CART决策树](https://en.wikipedia.org/wiki/Predictive_analytics#Classification_and_regression_trees_.28CART.29)就是使用”基尼指数“来选择划分属性，此处不作延伸）。香农熵定义为信息的期望值，首先如果多个待分类的事务可能被划分在多个分类之中，那么符号$x_i$的信息被定义为：

$$
l(x_i) = log_2p(x_i)
$$
其中$p(x_i)$是选择该分类的概率。
那么为了计算香农熵，我们需要计算所有类别所有可能包含的信息期望值，公式如下所示：

$$
H = - \sum_{i=1}^n p(x_i)log_2p(x_i)
$$
其中n是分类数目。
对应的python代码如下所示：
```python
def
```
```python
calcShannonEnt
```
```python
(dataSet)
```
```python
:
```
```python
numEntries = len(dataSet)
    labelCounts = {}
```
```python
for
```
```python
featVec
```
```python
in
```
```python
dataSet:
```
```python
#the the number of unique elements and their occurance
```
```python
currentLabel = featVec[-
```
```python
1
```
```python
]
```
```python
if
```
```python
currentLabel
```
```python
not
```
```python
in
```
```python
labelCounts.keys(): labelCounts[currentLabel] =
```
```python
0
```
```python
labelCounts[currentLabel] +=
```
```python
1
```
```python
shannonEnt =
```
```python
0.0
```
```python
for
```
```python
key
```
```python
in
```
```python
labelCounts:
        prob = float(labelCounts[key])/numEntries
        shannonEnt -= prob * log(prob,
```
```python
2
```
```python
)
```
```python
#log base 2
```
```python
return
```
```python
shannonEnt
```
上述代码，我们统计所有类别标签的发生频率然后计算类别出现的概率–香农熵。有了香农熵就能来度量信息增益了。
#### 信息增益
这里信息增益就是当前子结点的父节点的香农熵减去当前子结点的香农熵。此处要注意的是，当前特征有许多属性取值，因此假如特征a有许多属性取值$\{a^1,a^2,...,a^V\}$，按当前特征a划分样本集，则会产生V个分支，其中第v个分支所在特征a上取值为$a^v$的样本，记为$D^v$。那么特征a对样本集D进行划分的所获得的”信息增益“为：

$$
Gain(D,a) = Ent(D) - \sum_{v=1}^V\dfrac{|D^v|}{|D|}H(D^v)
$$

### 划分数据集2
有了前面的公式，接下来我们就能来寻找最佳特征来划分数据集了，代码如下：
```python
def
```
```python
splitDataSet
```
```python
(dataSet, axis, value)
```
```python
:
```
```python
retDataSet = []
```
```python
for
```
```python
featVec
```
```python
in
```
```python
dataSet:
```
```python
if
```
```python
featVec[axis] == value:
            reducedFeatVec = featVec[:axis]
```
```python
#chop out axis used for splitting
```
```python
reducedFeatVec.extend(featVec[axis+
```
```python
1
```
```python
:])
            retDataSet.append(reducedFeatVec)
```
```python
return
```
```python
retDataSet
```
```python
def
```
```python
chooseBestFeatureToSplit
```
```python
(dataSet)
```
```python
:
```
```python
numFeatures = len(dataSet[
```
```python
0
```
```python
]) -
```
```python
1
```
```python
#the last column is used for the labels
```
```python
baseEntropy = calcShannonEnt(dataSet)
    bestInfoGain =
```
```python
0.0
```
```python
; bestFeature = -
```
```python
1
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
range(numFeatures):
```
```python
#iterate over all the features
```
```python
featList = [example[i]
```
```python
for
```
```python
example
```
```python
in
```
```python
dataSet]
```
```python
#create a list of all the examples of this feature
```
```python
uniqueVals = set(featList)
```
```python
#get a set of unique values
```
```python
newEntropy =
```
```python
0.0
```
```python
for
```
```python
value
```
```python
in
```
```python
uniqueVals:
            subDataSet = splitDataSet(dataSet, i, value)
            prob = len(subDataSet)/float(len(dataSet))
            newEntropy += prob * calcShannonEnt(subDataSet)     
        infoGain = baseEntropy - newEntropy
```
```python
#calculate the info gain; ie reduction in entropy
```
```python
if
```
```python
(infoGain > bestInfoGain):
```
```python
#compare this to the best gain so far
```
```python
bestInfoGain = infoGain
```
```python
#if better than current best, set to best
```
```python
bestFeature = i
```
```python
return
```
```python
bestFeature
```
```python
#returns an integer
```
上述代码使用了两层for循环，第一层循环遍历数据集中的所有特征。第二层for循环遍历当前特征中所有的唯一属性值，对每个特征划分的属性值进行熵求和。最后比较所有特征中的信息增益，返回最好特征划分的索引值。
**Note:**
> 信息增益准则对取值数目较多的属性有所偏好，为了减少这种偏好，著名的
> [C4.5决策树](https://en.wikipedia.org/wiki/C4.5_algorithm)
> 算法加入了增益率来选择最优划分属性，基本思想是一种启发式的思想：先从候选划分属性中找出信息增益高于平均水平的属性，在从中选择增益率最高的。公式如下：

> Gain\_ratio(D,a) = \dfrac{Gain(D,a)}{IV(a)}

> 其中

> IV(a) = -\sum_{v=1}^V\dfrac{|D^v|}{|D|}log_2\dfrac{|D^v|}{|D|}

> 递归构建决策树

> 前面铺垫了这么多：知道了用香农熵度量信息增益，知道了如何有效划分数据集，这一节我们将介绍如何来使用递归算法构建决策树。我们先来回顾下决策树构造过程：得到原始数据集，然后基于最好的特征值划分数据集，由于属性值可能多于一个，因此可能存在大于两个分支的数据集划分。第一次划分后数据将被向下传递到树分支的下一个结点，在这个结点上，我们可以再次划分数据。这很明显是个递归的过程。那么递归算法的临界条件是：
> 程序遍历完所有划分数据集的属性，或者每个分支下的所有实例都具有相同的分类。
> 如果当程序遍历完所有划分数据集的属性，但是当前叶子结点的类标签依然不是唯一的，此时我们就采用以前一篇博客：
> [K近邻算法讲解与python实现（附源码demo下载链接）](http://blog.csdn.net/u010665216/article/details/78088321)
> 中采用的投票表决方法：
![knn](https://img-blog.csdn.net/20171009104154180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171009104154180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 代码如下：

> [

](https://img-blog.csdn.net/20171009104154180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> def
> majorityCnt
> (classList)
> :
> classCount={}
> for
> vote
> in
> classList:
> if
> vote
> not
> in
> classCount.keys(): classCount[vote] =
> 0
> classCount[vote] +=
> 1
> sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(
> 1
> ), reverse=
> True
> )
> return
> sortedClassCount[
> 0
> ][
> 0
> ]
> [
](https://img-blog.csdn.net/20171009104154180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 创建树的代码如下，我们用字典来存储树的信息：

> [

](https://img-blog.csdn.net/20171009104154180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> def
> createTree
> (dataSet,labels)
> :
> \#label is the name of features!
> classList = [example[-
> 1
> ]
> for
> example
> in
> dataSet]
> if
> classList.count(classList[
> 0
> ]) == len(classList):
> return
> classList[
> 0
> ]
> \#stop splitting when all of the classes are equal
> if
> len(dataSet[
> 0
> ]) ==
> 1
> :
> \#stop splitting when there are no more features in dataSet
> return
> majorityCnt(classList)
    bestFeat = chooseBestFeatureToSplit(dataSet)
    bestFeatLabel = labels[bestFeat]
    myTree = {bestFeatLabel:{}}
> del
> (labels[bestFeat])
    featValues = [example[bestFeat]
> for
> example
> in
> dataSet]
    uniqueVals = set(featValues)
> for
> value
> in
> uniqueVals:
        subLabels = labels[:]
> \#copy all of labels, so trees don't mess up existing labels
> myTree[bestFeatLabel][value] = createTree(splitDataSet(dataSet, bestFeat, value),subLabels)
> return
> myTree
> [

](https://img-blog.csdn.net/20171009104154180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 分类与存储

> [
](https://img-blog.csdn.net/20171009104154180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 前面我们介绍了如何构建决策树，这一大节，我们将重点关注如何利用决策树执行数据分类以及如何存储我们构建好的树

> [

](https://img-blog.csdn.net/20171009104154180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 分类

> [
](https://img-blog.csdn.net/20171009104154180?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 我么依靠训练数据构造了决策树之后，可以将其应用于实际数据的分类之中。流程很简单：程序比较测试数据与决策树上的数值，递归执行该过程直到进入叶子结点；最后将测试数据定义为叶子结点所属的类型。

> 分类代码如下：

> def
> classify
> (inputTree,featLabels,testVec)
> :
> firstStr = list(inputTree.keys())[
> 0
> ]
    secondDict = inputTree[firstStr]
    featIndex = featLabels.index(firstStr)
    key = testVec[featIndex]
    valueOfFeat = secondDict[key]
> if
> isinstance(valueOfFeat, dict): 
        classLabel = classify(valueOfFeat, featLabels, testVec)
> else
> : classLabel = valueOfFeat
> return
> classLabel
> 存储

> 通过代码我们发现，构造决策树使用了递归，这种算法在数据集很大的情况下很耗时间。为了节省时间，我们可以将构建好的决策树存储起来，在每次执行分类是调用构造好的决策树。代码如下：

> def
> storeTree
> (inputTree,filename)
> :
> import
> pickle
    fw = open(filename,
> 'w'
> )
    pickle.dump(inputTree,fw)
    fw.close()
> 通过上面的代码，我们就将构造好的决策树存储在磁盘上了，这样每次对数据进行分类时就能重复使用了。

> 实际应用

> 我们从titanic数据集中预处理一部分数据如下图所示：
![titanic](https://img-blog.csdn.net/20171009161544217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20171009161544217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 从左往右每列数据分别代表：年龄，性别，是否有子女，存活状态

> [

](https://img-blog.csdn.net/20171009161544217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 构建树

> [
](https://img-blog.csdn.net/20171009161544217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 构建：

> [

](https://img-blog.csdn.net/20171009161544217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> import
> trees
fr = open(
> 'titanic.txt'
> )
titanic = [inst.strip().split(
> '\t'
> )
> for
> inst
> in
> fr.readlines()]
titanic_fea_name = [
> 'age'
> ,
> 'sex'
> ,
> 'sibsp'
> ,
> 'survival station'
> ]
> \#\#构建树
> titanicTree = trees.createTree(titanic,titanic_fea_name)
titanicTree
> [
](https://img-blog.csdn.net/20171009161544217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 输出：

> [
](https://img-blog.csdn.net/20171009161544217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> {‘sex’: {‘female’: ‘survived’,

> ‘male’: {‘age’: {‘mid’: ‘died’,

> ‘old’: ‘died’,

> ‘young’: {‘sibsp’: {‘no’: ‘survived’, ‘yes’: ‘died’}}}}}}

> 可视化

> \#\#可视化
> import
> treePlotter
> import
> importlib
importlib.reload(treePlotter)
treePlotter.createPlot(titanicTree)
> 输出：
![titanic](https://img-blog.csdn.net/20171009162538487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 测试

> 我们选择一个乘客jack，乘客信息如下：青年，男性，暂无子女

> \#\#测试乘客jack
> import
> importlib
importlib.reload(trees)
labels = [
> 'age'
> ,
> 'sex'
> ,
> 'sibsp'
> ]
jack = [
> 'young'
> ,
> 'male'
> ,
> 'no'
> ]
trees.classify(titanicTree,labels,jack)
> 输出：

> ‘survived’

> 总结

> 到目前为止，决策树的大部分内容我们已经介绍完了，但是这个算法只是个baseline，因为我们数据的特殊性，已经构造的决策树很好的匹配了实验数据，所以在我们看来，这个结果貌似不错。然而，我们需要知道的一点是，决策树由于其算法的本质决定了在匹配过程中，存在匹配项过多的情况，这就会导致过度匹配（overfitting），为了解决这个问题，我们可以裁剪决策树，去掉一些不必要的叶子结点。周志华的西瓜书中对剪枝处理有很详细的介绍。本文将在后来的回归算法讲CART时介绍剪枝的方法。还有本文讲的ID3算法并不能很好的处理数值型数据，从数据集我们也可以发现数据是离散属性的。C4.5中采用了二分法对连续属性进行处理，我们这里暂时不做延伸讲解。


