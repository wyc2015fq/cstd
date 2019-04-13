
# K近邻算法讲解与python实现（附源码demo下载链接） - OraYang的博客 - CSDN博客

2017年09月25日 18:56:02[OraYang](https://me.csdn.net/u010665216)阅读数：717所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



**k近邻算法概述**
对应demo源码及数据：[传送门](http://download.csdn.net/download/u010665216/9994775)
K近邻（k-Nearest Neighbor,简称kNN）算法，是一种应用很广泛的监督学习算法。它非常有效且易于掌握，其工作机制也很简单：给定测试样本，基于某种距离度量找出训练集中与其最靠近的k个训练样本，然后基于这k个"邻居"的信息来进行预测。K近邻既可以用于分类又可以用于回归，在分类问题中：我们使用"投票法"，即选择这k个样本中出现最多的类别标记作为预测结果；在回归问题中：我们使用"平均法"，即将这k个样本的输出的平均值作为预测结果。但是在工业界，knn被广泛应用，因此在本次介绍中，我们将着重介绍k近邻在分类方面的应用。
**什么时候使用k近邻呢？**
通常在评估一个算法时，我们通常看三个方面：
易于解释
时间复杂度
预测效果
我们将k近邻与其他几种算法量化比较，表格如下：
![](https://img-blog.csdn.net/20170925185554791)
从上面的表格我们可以看出，knn因为在解释性，时间复杂度上比较优秀所以被广泛使用。
**k近邻算法是如何工作的呢？**
我们来举个例子说明下如何用k近邻来分类爱情片和动作片。有人曾统计过很多电影的打斗镜头和接吻镜头，下图显示了6部电影的打斗镜头和接吻镜头数。假如有一部未看过的电影，如何确定它是爱情片还是动作片呢？
![](https://img-blog.csdn.net/20170925185556870)
首先我们需要知道这个未知镜头存在多少个打斗镜头和接吻镜头，上图的问号位置是该未知电影出现的镜头数图形化展示，具体数字参见下表：
![](https://img-blog.csdn.net/20170925185557657)
现在我们就可以利用knn来利用一种度量方式，计算未知电影与样本中其他电影的距离，如下表所示：
![](https://img-blog.csdn.net/20170925185558026)
现在我们得到了样本中所有电影与未知电影的距离，将距离升序排序，可以找到离未知样本距离最近的k个顶啊应。比如说本题中取k=3,那么最靠近未知电影的三个电影分别是He's Not Really into Dudes、Beautiful Woman 和Califinia Man。K近邻算法然后按照这三部电影的类型决定未知电影的类型，而这电影全是爱情片，因此我们就判定未知电影是爱情片。
**到底有哪些度量方法以及k是如何确定的呢？**
度量方法又叫"metric learning"，这个概念在计算机视觉领域比较常见到。这种度量方法一般是距离度量，空间上有欧氏距离，路径上有曼哈顿距离，国际象棋上的一致范数：切比雪夫距离等等。一般来说需要根据不同的应用场景选择不同的度量方法。
说完了，度量方法，那么knn算法中的k到底该如何取值呢？首先我们得尝试去理解k在算法中是如何起作用的。所以我们直接在实际环境中应用k紧邻算法来说明k值得选择。
**用python语言实现k近邻算法**
首先我们来看下k近邻的伪代码：
![](https://img-blog.csdn.net/20170925185558440)
其python实现如下：
fromnumpyimport*
importoperator
fromosimportlistdir

defclassify0(inX, dataSet, labels, k):
\#inx:用于分类的输入向量
\#dataSet:输入的训练样本集
\#labels:标签向量
\#k:选择最近邻的数目
dataSetSize = dataSet.shape[0]
diffMat = tile(inX, (dataSetSize,1)) - dataSet  \#行方向重复dataSetSize次，列方向1次
sqDiffMat = diffMat**2
sqDistances = sqDiffMat.sum(axis=1)
distances = sqDistances**0.5  \#求距离
sortedDistIndicies = distances.argsort()   \#排序
classCount={}
foriinrange(k):
voteIlabel = labels[sortedDistIndicies[i]]
classCount[voteIlabel] = classCount.get(voteIlabel,0) + 1  \#字典get方法
sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)  \#reverse降序、iteritem返回迭代器、itemgetter(1)取value
returnsortedClassCount[0][0]
我们从网上download一个在线约会网站的例子，一部分数据如下图所示：
![](https://img-blog.csdn.net/20170925185559033)
其中每一行是一条会员的信息，依次为每年获得的飞行常客里程数，玩视频游戏所耗时间百分比，每周消费的冰淇淋公升数。
，我们取前两列的数据来画一个二维的散点图，如下图所示：
![](https://img-blog.csdn.net/20170925185559613)
这里因为数据差值比较大，在应用knn算法前我们先将原始数据做一个归一化处理，将任意取值范围的特征值转化为0到1区间的值。公式如下：
![](https://img-blog.csdn.net/20170925185559986)
归一化代码如下：
defautoNorm(dataSet):
minVals = dataSet.min(0)
maxVals = dataSet.max(0)
ranges = maxVals - minVals
normDataSet = zeros(shape(dataSet))
m = dataSet.shape[0]
normDataSet = dataSet - tile(minVals, (m,1))
normDataSet = normDataSet/tile(ranges, (m,1))\#element wise divide
returnnormDataSet, ranges, minVals
**测试算法**
上面我们已经将knn算法实现了，那么我们怎么才能对这个算法做个评估呢？这里我们采用最原始的做法即：错误率，对于分类器来说，错误率就是分类器给出错误结果的次数除以测试数据的总数。那么我们测试数据的代码如下所示：
defdatingClassTest():
hoRatio = 0.10\#hold out 10%
datingDataMat,datingLabels = file2matrix('datingTestSet2.txt')\#load data setfrom file
normMat, ranges, minVals = autoNorm(datingDataMat)
m = normMat.shape[0]
numTestVecs = int(m*hoRatio)
errorCount = 0.0
foriinrange(numTestVecs):
classifierResult = classify0(normMat[i,:],normMat[numTestVecs:m,:],datingLabels[numTestVecs:m],3)
print("the classifier came back with: %d, the real answer is: %d"% (classifierResult, datingLabels[i]))
if(classifierResult != datingLabels[i]): errorCount += 1.0
print("the total error rate is: %f"% (errorCount/float(numTestVecs)))
print(errorCount)
这里我们默认取k=3，每测试一条数据就打印输出，最后汇总错误率。log打印输出如下：
![](https://img-blog.csdn.net/20170925185600534)
可以看到，分类错误总计五个，错误率为5%。
我们随机找个人的信息输入进去，测试结果如下：
![](https://img-blog.csdn.net/20170925185600962)
**K值的确定**
说了这么多，我们还是没说如何确定k值。我们是这样思考的：选定训练集和对应的测试机，然后选取不同的k值，把其中错误率最低的k作为分类的k值，当有新的训练集更新时，我们再运行模型，不断迭代更新。我们用上面的例子，将k的取值范围定位3-20（一般来说k是不超过20的整数）,运行结果如下：
![](https://img-blog.csdn.net/20170925185601959)
最终我们确定可最佳k为7。
**总结**
k近邻算法是分类数据最简单有效的算法，k-近邻必须保存全部数据集，而且必须对数据集中每个数据计算距离值所以空间复杂度与时间复杂度都可能会比较大。K-近邻还有个缺陷就是无法给出任何数据的基础结构信息但是这个问题可以在概率分类中得到解决。

