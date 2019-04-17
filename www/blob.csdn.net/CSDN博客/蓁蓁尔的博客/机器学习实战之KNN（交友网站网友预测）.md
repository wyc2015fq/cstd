# 机器学习实战之KNN（交友网站网友预测） - 蓁蓁尔的博客 - CSDN博客





2016年06月30日 21:46:51[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：542








一、数据集：datingTestData.txt，格式如下： 
![这里写图片描述](https://img-blog.csdn.net/20160630210107606)

前三个为特征，分别是： 

（1）每年获得的飞行常客里程数 

（2）玩视频游戏所耗的时间百分比 

（3）每周消费的冰淇淋公升数 

标签：不喜欢、魅力一般、极具魅力。 

二、使用python处理文本文件，转为矩阵形式。
```python
def file2matrix(filename):
    fr = open(filename)
    numberOfLines = len(fr.readlines())         #get the number of lines in the file
    returnMat = zeros((numberOfLines,3))        #prepare matrix to return
    classLabelVector = []                       #prepare labels return   
    fr = open(filename)
    index = 0
    for line in fr.readlines():
        line = line.strip()
        listFromLine = line.split('\t')
        returnMat[index,:] = listFromLine[0:3]
        classLabelVector.append(listFromLine[-1])
        index += 1
    return returnMat,classLabelVector
```

三.因为是算每两个样本间的欧氏距离，易发现，上面数字差值最大的属性对最后结果影响太大，所以要先做归一化处理，将任意特征值的取值范围控制在相同的0-1之间。

```python
def autoNorm(dataSet):
    minVals = dataSet.min(0)
    maxVals = dataSet.max(0)
    ranges = maxVals - minVals
    normDataSet = zeros(shape(dataSet))
    m = dataSet.shape[0]
    normDataSet = dataSet - tile(minVals, (m,1))
    normDataSet = normDataSet/tile(ranges, (m,1))   #element wise divide
    return normDataSet, ranges, minVals
```

四. 程序主要部分，KNN算法实现部分。

```python
def classify0(inX, dataSet, labels, k):
    dataSetSize = dataSet.shape[0]
    diffMat = tile(inX, (dataSetSize,1)) - dataSet
    sqDiffMat = diffMat**2
    sqDistances = sqDiffMat.sum(axis=1)
    distances = sqDistances**0.5
    sortedDistIndicies = distances.argsort()     
    classCount={}          
    for i in range(k):
        voteIlabel = labels[sortedDistIndicies[i]]
        classCount[voteIlabel] = classCount.get(voteIlabel,0) + 1
    sortedClassCount = sorted(classCount.iteritems(), key=operator.itemgetter(1), reverse=True)
    return sortedClassCount[0][0]
```

五。整个过程实现部分

```python
def classifyPerson():
    resultList = ['not at all','in small doses','in large doses']
    percentTats = float(raw_input("percentage of time spent playing video games?"))
    ffMiles = float(raw_input("frequent flier miles earned per year?"))
    iceCream = float(raw_input("liters of ice cream consumed per year?"))
    datingDataMat,datingLabels = file2matrix('datingTestSet2.txt')
    normMat, ranges, minVals = autoNorm(datingDataMat)
    inArr = array([ffMiles,percentTats,iceCream])
    classifierResult = classify0((inArr - minVals)/ranges,normMat,datingLabels,3)
    print classifierResult
    print "you will like this person:",resultList[int(classifierResult) - 1]
```

六. 结果：

```
>>>classifyPerson()
percentage of time spent playing video games?10
frequent flier miles earned per year?10000
liters of ice cream consumed per year?0.5
2
you will like this person: in small doses
```




