# KNN最邻近规则分类算法实践实现【Python实现】 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年04月23日 18:36:36[QuJack](https://me.csdn.net/A784586)阅读数：657








**     KNN算法实践【Python实现】  分类算法**

本博客按照理论思想实现KNN，算法理论分析见上一篇博客。

[【点我】KNN算法介绍（最邻近规则分类算法）](http://blog.csdn.net/a784586/article/details/70500114)




1.例子：根据花的四个特征预测某种花属于某种

数据集介绍

数据特征：

萼片长度，萼片宽度，花瓣长度，花瓣宽度

(sepal length, sepal width, petal length and petal width）

类别lable：

Iris setosa, Iris versicolor, Iris virginica.


![](https://img-blog.csdn.net/20170423182354633?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


总共有150个样例，选择部分作为训练集，部分作为测试集。



**代码实践：调用Python库sklearn实现**

1.安装Python和机器学习库，和一些依赖包；

本人是直接安装了包含了众多包的Anaconda3 ，下载后再window7 64bit上双击安装即可；

Anaconda3较大，如果网速不好，可以从百度云下载地址：http://pan.baidu.com/s/1dFIfoYX

2.打开cmd 输入：pip list 可以查看到已经安装的包；

3. 在cmd中运行如下的Python程序：





```python
from sklearn import neighbors
from sklearn import datasets

knn = neighbors.KNeighborsClassifier()

iris = datasets.load_iris()
# save data
# f = open("iris.data.csv", 'wb')
# f.write(str(iris))
# f.close()

print (iris)

knn.fit(iris.data, iris.target)

predictedLabel = knn.predict([[0.1, 0.2, 0.3, 0.4]])
#print ("hello")
print ("predictedLabel is :" + predictedLabel)
#print (predictedLabel)
```

2.自己用Python实现该算法：




```python
import csv
import random
import math
import operator


def loadDataset(filename, split, trainingSet = [], testSet = []):
    with open(filename, 'r') as csvfile:
        lines = csv.reader(csvfile)
        dataset = list(lines)
        for x in range(len(dataset)-1):
            for y in range(4):
                dataset[x][y] = float(dataset[x][y])
            if random.random() < split:
                trainingSet.append(dataset[x])
            else:
                testSet.append(dataset[x])


def euclideanDistance(instance1, instance2, length):
    distance = 0
    for x in range(length):
        distance += pow((instance1[x]-instance2[x]), 2)
    return math.sqrt(distance)


def getNeighbors(trainingSet, testInstance, k):
    distances = []
    length = len(testInstance)-1
    for x in range(len(trainingSet)):
        #testinstance
        dist = euclideanDistance(testInstance, trainingSet[x], length)
        distances.append((trainingSet[x], dist))
        #distances.append(dist)
    distances.sort(key=operator.itemgetter(1))
    neighbors = []
    for x in range(k):
        neighbors.append(distances[x][0])
        return neighbors


def getResponse(neighbors):
    classVotes = {}
    for x in range(len(neighbors)):
        response = neighbors[x][-1]
        if response in classVotes:
            classVotes[response] += 1
        else:
            classVotes[response] = 1
    sortedVotes = sorted(classVotes.items(), key=operator.itemgetter(1), reverse=True)#.iteritems()
    return sortedVotes[0][0]


def getAccuracy(testSet, predictions):
    correct = 0
    #print (len(testSet))
    #print (len(predictions))
    for x in range(len(testSet)):
        print (testSet[x][-1])
        print (predictions[x])
        if testSet[x][-1] == predictions[x]:
            correct += 1
    return (correct/float(len(testSet)))*100.0


def main():
    #prepare data
    trainingSet = []
    testSet = []
    split = 0.67
    loadDataset('irisdata.txt', split, trainingSet, testSet)
    print ('Train set: ' + repr(len(trainingSet)))
    print ('Test set: ' + repr(len(testSet)))
    #generate predictions
    predictions = []
    k = 3
    for x in range(len(testSet)):
        # trainingsettrainingSet[x]
        neighbors = getNeighbors(trainingSet, testSet[x], k)
        result = getResponse(neighbors)
        predictions.append(result)
        print ('>predicted=' + repr(result) + ', actual=' + repr(testSet[x][-1]))
    accuracy = getAccuracy(testSet, predictions)
    print('Accuracy: ' + repr(accuracy) + '%')

if __name__ == '__main__':
    main()
```


运行截图：



![](https://img-blog.csdn.net/20170423183222181?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170423183228056?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**附录：**

代码源码和数据集.留下邮箱，24h内发送到您邮箱。















