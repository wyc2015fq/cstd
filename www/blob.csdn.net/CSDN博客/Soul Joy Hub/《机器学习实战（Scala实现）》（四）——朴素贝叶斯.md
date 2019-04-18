# 《机器学习实战（Scala实现）》（四）——朴素贝叶斯 - Soul Joy Hub - CSDN博客

2017年03月29日 12:55:47[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1598
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



# 原理

关于算法原理可以参阅：[http://blog.csdn.net/u011239443/article/details/53735609#t35](http://blog.csdn.net/u011239443/article/details/53735609#t35)

# 构建词向量

## python

```python
def loadDataSet():
    postingList=[['my', 'dog', 'has', 'flea', 'problems', 'help', 'please'],
                 ['maybe', 'not', 'take', 'him', 'to', 'dog', 'park', 'stupid'],
                 ['my', 'dalmation', 'is', 'so', 'cute', 'I', 'love', 'him'],
                 ['stop', 'posting', 'stupid', 'worthless', 'garbage'],
                 ['mr', 'licks', 'ate', 'my', 'steak', 'how', 'to', 'stop', 'him'],
                 ['quit', 'buying', 'worthless', 'dog', 'food', 'stupid']]
    # 1 代表侮辱性的词 0则不是
    classVec = [0,1,0,1,0,1]    
    return postingList,classVec

def createVocabList(dataSet):
    # 创建空集合
    vocabSet = set([])  
    for document in dataSet:
        # 合并两个集合
        vocabSet = vocabSet | set(document) 
    return list(vocabSet)

def setOfWords2Vec(vocabList, inputSet):
    returnVec = [0]*len(vocabList)
    for word in inputSet:
        if word in vocabList:
            returnVec[vocabList.index(word)] = 1
        else: print "the word: %s is not in my Vocabulary!" % word
    return returnVec
```

# 训练与测试算法

## python

### 训练算法

```python
def trainNB0(trainMatrix,trainCategory):
    numTrainDocs = len(trainMatrix)
    numWords = len(trainMatrix[0])
    pAbusive = sum(trainCategory)/float(numTrainDocs)
    p0Num = ones(numWords); p1Num = ones(numWords)      
    p0Denom = 2.0; p1Denom = 2.0                        
    for i in range(numTrainDocs):
        if trainCategory[i] == 1:
            p1Num += trainMatrix[i]
            p1Denom += sum(trainMatrix[i])
        else:
            p0Num += trainMatrix[i]
            p0Denom += sum(trainMatrix[i])
    p1Vect = log(p1Num/p1Denom)          
    p0Vect = log(p0Num/p0Denom)          
    return p0Vect,p1Vect,pAbusive
```
- 这里的`pAbusive`其实应该计算的是各个类别的概率。但是我们这里是类别只有 0 和 1 的二分类，所以只要返回一个类别为 1 的概率给后续程序就行了。
- `p0Num = ones(numWords); p1Num = ones(numWords)`和`p0Denom = 2.0; p1Denom = 2.0`是为了避免后续计算log中的指和分母值取到`0`。
- `p1Num/p1Denom`得到向量第i个特征即$\large p(w_i/c_1)$，而$\large log(p(w/c_1)) = log(p(w_1/c_1)) + log(p(w_2/c_1)) + ... +  log(p(w_n/c_1))$。所以我们只需要将该向量中的每个特征取对数，再累加就能得到$\large log(p(w/c_1))$

## 测试算法

```python
def classifyNB(vec2Classify, p0Vec, p1Vec, pClass1):
    p1 = sum(vec2Classify * p1Vec) + log(pClass1)    
    p0 = sum(vec2Classify * p0Vec) + log(1.0 - pClass1)
    if p1 > p0:
        return 1
    else: 
        return 0

def testingNB():
    listOPosts,listClasses = loadDataSet()
    myVocabList = createVocabList(listOPosts)
    trainMat=[]
    for postinDoc in listOPosts:
        trainMat.append(setOfWords2Vec(myVocabList, postinDoc))
    p0V,p1V,pAb = trainNB0(array(trainMat),array(listClasses))
    testEntry = ['love', 'my', 'dalmation']
    thisDoc = array(setOfWords2Vec(myVocabList, testEntry))
    print testEntry,'classified as: ',classifyNB(thisDoc,p0V,p1V,pAb)
    testEntry = ['stupid', 'garbage']
    thisDoc = array(setOfWords2Vec(myVocabList, testEntry))
    print testEntry,'classified as: ',classifyNB(thisDoc,p0V,p1V,pAb)
```

由于$\large p(w)$是相同的，所以我们只需要比较$\large p(w/c_0)p(c_0)$和$\large p(w/c_1)p(c_1)$的大小,即$\large log(p(w/c_0)p(c_0))$和$\large log(p(w/c_1)p(c_1))$的大小。如：$\large log(p(w/c_0)p(c_0)) =  log(p(w/c_0)) + log(p(c_0)) = $`sum(vec2Classify * p0Vec) + log(1.0 - pClass1)`

# scala

```
package NativeBayes

import scala.collection.mutable.ArrayBuffer

object NativeBayes {

  def loadDataSet() = {
    val postingList = Array(Array("my", "dog", "has", "flea", "problems", "help", "please"),
      Array("maybe", "not", "take", "him", "to", "dog", "park", "stupid"),
      Array("my", "dalmation", "is", "so", "cute", "I", "love", "him"),
      Array("stop", "posting", "stupid", "worthless", "garbage"),
      Array("mr", "licks", "ate", "my", "steak", "how", "to", "stop", "him"),
      Array("quit", "buying", "worthless", "dog", "food", "stupid"))
    //1 代表不良信息, 反之为 0 
    val classVec = Array(0, 1, 0, 1, 0, 1)
    (postingList, classVec)
  }

  def setOfWords2Vec(vocabList: Array[String], inputSet: Array[String]) = {
    val returnVec = new Array[Int](vocabList.length)
    val vocabListWithIndex = vocabList.zipWithIndex
    for (word <- inputSet) {
      if (vocabList.contains(word))
        returnVec(vocabListWithIndex.filter(_._1 == word)(0)._2) = 1
      else printf("the word: %s is not in my Vocabulary!\n", word)
    }
    returnVec
  }

  def trainNB0(trainMatrix: Array[Array[Int]], trainCategory: Array[Int]) = {
    val numTrainDocs = trainMatrix.length
    val numWords = trainMatrix(0).length
    val pAbusive = trainCategory.sum / numTrainDocs.toDouble
    var p0Num = Array.fill(numWords)(1)
    var p1Num = Array.fill(numWords)(1)
    var p0Denom = 2.0
    var p1Denom = 2.0
    for (i <- 0 to numTrainDocs - 1) {
      if (trainCategory(i) == 1) {
        var cnt = 0
        p1Num = p1Num.map { x =>
          val v = x + trainMatrix(i)(cnt)
          cnt += 1
          v
        }
        p1Denom += trainMatrix(i).sum
      } else {
        var cnt = 0
        p0Num = p0Num.map { x =>
          val v = x + trainMatrix(i)(cnt)
          cnt += 1
          v
        }
        p0Denom += trainMatrix(i).sum
      }
    }
    (p1Num.map(x => math.log(x / p1Denom)), p0Num.map(x => Math.log(x / p0Denom)), pAbusive)
  }

  def classifyNB(vec2Classify: Array[Int], p0Vec: Array[Double], p1Vec: Array[Double], pClass1: Double) = {
    var cnt = 0
    val p1 = vec2Classify.map { x =>
      val v = x * p1Vec(cnt)
      cnt += 1
      v
    }.sum + math.log(pClass1)
    cnt = 0
    val p0 = vec2Classify.map { x =>
      val v = x * p0Vec(cnt)
      cnt += 1
      v
    }.sum + math.log(1.0 - pClass1)

    if (p1 > p0) 1 else 0
  }

  def main(args: Array[String]): Unit = {
    val DataSet = loadDataSet()
    val listOPosts = DataSet._1
    val listClasses = DataSet._2
    val myVocabList = listOPosts.reduce((a1, a2) => a1.++:(a2)).distinct
    var trainMat = new ArrayBuffer[Array[Int]](listOPosts.length)
    listOPosts.foreach(postinDoc => trainMat.append(setOfWords2Vec(myVocabList, postinDoc)))

    val p = trainNB0(trainMat.toArray, listClasses)
    val p0V = p._2
    val p1V = p._1
    val pAb = p._3
    val testEntry = Array("love", "my", "dalmation")
    val thisDoc = setOfWords2Vec(myVocabList, testEntry)
    println(testEntry.mkString(",") + " classified as: " + classifyNB(thisDoc, p0V, p1V, pAb))
    val testEntry2 = Array("stupid", "garbage")
    val thisDoc2 = setOfWords2Vec(myVocabList, testEntry2)
    println(testEntry2.mkString(",") + " classified as: " + classifyNB(thisDoc2, p0V, p1V, pAb))
  }
}
```

![这里写图片描述](https://img-blog.csdn.net/20170609195828566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

