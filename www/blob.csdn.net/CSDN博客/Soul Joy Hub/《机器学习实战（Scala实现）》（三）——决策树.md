# 《机器学习实战（Scala实现）》（三）——决策树 - Soul Joy Hub - CSDN博客

2017年03月26日 18:19:45[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：3033
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



# 信息熵
- p(x):分类结果x的概率，即分类结果为x的数据量/总数据量
- 信息：l(x) = -log2(p(x))
- 信息熵：信息的期望值 p(x1)l(x1) + p(x2)l(x2) + …… ，可以评价一组不同类别的划分结果的混沌度。

```python
def calcShannonEnt(dataset):
     numEntries = len(dataset)
     labelCounts = {}
     for featVec in dataset:
         currentLabel = featVec[-1]
         if currentLabel not in labelCounts.keys():
             labelCounts[currentLabel] = 0
         labelCounts[currentLabel] += 1
     shannonEnt = 0.0
     for key in labelCounts:
         prob = float(labelCounts[key])/numEntries
         shannonEnt -= prob * log(prob,2)
     return shannonEnt
```

# 按给定特征划分数据集

```python
# axis 特征 ， value 给定的该特征的值
 def splitDataSet(dataSet , axis , value):
     retDataSet = []
     for featVec in dataSet:
         if featVec[axis] == value:
             reducedFeatVec = featVec[:axis]
             '''
             b = [1,2]
             a = [1,2]
             b.append（a） 函数： 往列表b里面添加元素a：
             结果： b = [1,2,[1,2]]
             b.extend（a） 函数： 用列表a扩张列表b：
             结果： b = [1,2,1,2] 
             '''
             reducedFeatVec.extend(featVec[axis+1:])
             retDataSet.append(reducedFeatVec)
     return retDataSet
```

# 寻找划分数据集的最好特征

```python
def chooseBestFeatureToSplit(dataset):
     numFeatures = len(dataset[0]) - 1
     baseEntropy = calcShannonEnt(dataset)
     bestInfoGain = 0.0
     bestFeature = -1
     numDatas = len(dataset)
     for i in range(numFeatures):
         featList = [example[i] for example in dataset] # 第i列
         # uniqueValsl里面保存着第i个特征的所有可能的取值
         uniqueVals = set(featList) 
         newEntropy = 0.0
         for value in uniqueVals:
             subDataSet = splitDataSet(dataset,i,value)
             prob = float(len(subDataSet))/numDatas
             # 求划分后信息熵的期望
             newEntropy += prob*calcShannonEnt(subDataSet) 
         #信息熵可以表现数据的混沌性，所以划分后信息熵的期望越小越好
         infoGain = baseEntropy - newEntropy
         if (infoGain > bestInfoGain):
             bestInfoGain = infoGain
             bestFeature = i
     return bestFeature
```

# 决策树算法

## 算法说明

创建决策树算法 createTree()：

```bash
if 数据集中的每个子项的类别完全相同：
    return 该类别
else if 遍历完所有的特征：
return 频率最高的类别
else :
    寻找划分数据集的最好特征
    创建分支节点
    划分数据集
    for 每个划分的子集
        调用createTree()并且增加返回结果到分支节点中
    return 分支节点
```

## 频率最高的类别函数

```python
def majorityCnt(classList):
     classCount = {}
     for vote in classList:
         if vote not in classCount.keys():
             classCount[vote] = 0
         classCount[vote] += 1
     sortedclassCount = sorted(classCount.iteritems(),key=operator.itemgetter(1),reverse=True)
     return sortedclassCount[0][0]
```

## 创建决策树

```python
def createTree(dataSet,labels):
    classList = [example[-1] for example in dataSet]
    #if 数据集中的每个子项的类别完全相同：return 该类别
    if(classList.count(classList[0]) == len(classList)):
        return classList[0]
    #if 遍历完所有的特征：return 频率最高的类别
    if(len(dataSet[0]) == 1):
        return majorityCnt(classList)
    #寻找划分数据集的最好特征
    bestFeat = chooseBestFeatureToSplit(dataSet)
    bestFeatLabel = labels[bestFeat]
    #创建分支节点
    myTree = {bestFeatLabel:{}}
    del(labels[bestFeat])
    #划分数据集
    featValues = [example[bestFeat] for example in dataSet]
    uniqueVals = set(featValues)
    #for 每个划分的子集
    for value in uniqueVals:
        #调用createTree()并且增加返回结果到分支节点中
        sublabels = labels[:]
        myTree[bestFeatLabel][value] = createTree(splitDataSet(dataSet,bestFeat,value),sublabels)
    #return 分支节点
    return myTree
```

# Scala实现

```
import scala.collection.mutable.Map
import scala.collection.mutable.ArrayBuffer

class MyTree(a: String, b: Map[Int, MyTree]) {
  var nodes = b
  val value = a
}

object DTree {
  def createDataSet() = {
    val dataSet = Array((Array(1, 1), "yes"), (Array(1, 1), "yes"),
      (Array(1, 0), "no"), (Array(0, 1), "no"), (Array(0, 1), "no"))
    // 这应该称为属性 而不是标签
    val TreeAttributes = Array("no surfacing", "flippers")
    (dataSet, TreeAttributes)
  }

  def calShannonEnt(dataSet: Array[Tuple2[Array[Int], String]]) = {
    val numEntries = dataSet.length
    var labelCounts: Map[String, Int] = Map.empty
    for (featVec <- dataSet) {
      val currentLabel = featVec._2
      labelCounts(currentLabel) = labelCounts.getOrElse(currentLabel, 0) + 1
    }
    var shannoEnt = 0.0
    for (value <- labelCounts.values) {
      val prob = value.toDouble / numEntries
      shannoEnt -= prob * math.log(prob) / math.log(2)
    }
    shannoEnt
  }

  def splitDataSet(dataSet: Array[Tuple2[Array[Int], String]], axis: Int, value: Int) = {
    var retDataSet: ArrayBuffer[Tuple2[Array[Int], String]] = ArrayBuffer.empty
    for (featVec <- dataSet) {
      if (featVec._1(axis) == value) {
        val reducedFeatvec = featVec._1.zipWithIndex.filter(_._2 != axis).map(_._1)
        retDataSet.+=((reducedFeatvec, featVec._2))
      }
    }
    retDataSet.toArray
  }

  def chooseBestFeatureToSplit(dataSet: Array[Tuple2[Array[Int], String]]) = {
    val baseEntropy = calShannonEnt(dataSet)
    var bestInfoGain = 0.0
    var bestFeature = -1
    for (i <- 0 to dataSet(0)._1.length - 1) {
      val uniqueVals = dataSet.map(_._1(i)).toSet
      var newEntropy = 0.0
      for (value <- uniqueVals) {
        val subDataSet = splitDataSet(dataSet, i, value)
        newEntropy += subDataSet.length.toDouble / dataSet.length * calShannonEnt(subDataSet)
      }
      val infoGain = baseEntropy - newEntropy
      if (infoGain > bestInfoGain) {
        bestInfoGain = infoGain
        bestFeature = i
      }
    }
    bestFeature
  }

  def creatTree(dataSet: Array[Tuple2[Array[Int], String]], attribute: Array[String]): MyTree = {
    val classList = dataSet.map(_._2);
    if (classList.count(_ == classList(0)) == classList.length) {
      new MyTree(classList(0), Map.empty)
    } else if (dataSet.length == 1) {
      val str = classList.map((_, 1)).groupBy(_._1).map(x => (x._1, x._2.map(_._2).reduce((x, y) => x + y))).toList.maxBy(_._2)._1
      new MyTree(str, Map.empty)
    } else {
      val bestFeat = chooseBestFeatureToSplit(dataSet)
      val bestFeatAttribute = attribute(bestFeat)
      var myTree = new MyTree(bestFeatAttribute, Map.empty)
      var Vattribute = attribute
      Vattribute = Vattribute.filter(_ != bestFeatAttribute)
      val uniqueVals = dataSet.map(_._1(bestFeat)).distinct
      for (value <- uniqueVals) {
        myTree.nodes.+=((value -> creatTree(splitDataSet(dataSet, bestFeat, value), Vattribute)))
      }
      myTree
    }
  }

  def classify(inputTree: MyTree, attribute: Array[String], testVec: Array[Int]): String = {
    var classLabel = ""
    val firstStr = inputTree.value
    val secondTrees = inputTree.nodes
    val featIndex = attribute.zipWithIndex.filter(_._1 == firstStr)(0)._2
    for (key <- secondTrees.keySet) {
      if (testVec(featIndex) == key) {
        if (secondTrees(key).nodes.isEmpty)
          return secondTrees(key).value
        else classLabel = classify(secondTrees(key), attribute, testVec)
      }
    }
    classLabel
  }

  def main(agrs: Array[String]): Unit = {
    val name = 1
    val DataSet = createDataSet()
    val dataSet = DataSet._1
    val attribute = DataSet._2
    val inputTree = creatTree(dataSet, attribute)
    println(classify(inputTree, attribute, Array(1, 1)))
  }
}
```

![这里写图片描述](https://img-blog.csdn.net/20170609195358100?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

