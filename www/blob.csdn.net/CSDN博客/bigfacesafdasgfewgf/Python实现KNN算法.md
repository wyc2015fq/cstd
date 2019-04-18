# Python实现KNN算法 - bigfacesafdasgfewgf - CSDN博客





2015年01月16日 16:16:13[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1558








**Python实现KNN算法**



    KNN算法的实际用处很多，主要用于分类阶段，是一个基础的分类算法。KNN主要基于距离的计算，一般可以在原始的欧氏空间中计算样本之间的距离。改进版本有：先特征提取到一个更加鉴别的空间中，然后计算距离；或者先使用metric learning度量学习的技术来获得一个鉴别的度量空间，然后计算样本间的马氏距离。

    不管怎么说，KNN在很多算法的分类阶段都可以用到，我们这里用python实现KNN。



# 1. sklearn自带的KNN





**fromsklearn.neighborsimport** NearestNeighbors

就可以调用最近邻算法了。



```python
'''
python实现KNN算法
'''

#只是返回近邻点，不分类
from sklearn.neighbors import NearestNeighbors  #加载最近邻算法
samples = [[0, 0, 0], [0, 0.5, 0], [1, 1, 0.5]]; 
neigh = NearestNeighbors(n_neighbors=2)  #set the number of neighbors 
neigh.fit(samples) 
print neigh.kneighbors([1, 1, 1]) #return the same number of neighbors 
#return two arrays, the first is the calculated distance; the second is the indexs of neighbors, strarting from 0 



#实现分类
from sklearn.neighbors import KNeighborsClassifier 
knnclf = KNeighborsClassifier(n_neighbors=1)  #we set the k=1, while default with k=5 
samples = [[0, 0, 0], [0, 0.5, 0], [1, 1, 0.5]]  #training samples features
labels = [0, 0, 1]  #the labels 
knnclf.fit(samples, labels) 
print knnclf.predict([1, 1, 1])  #return the classification label, that is, [1]
```



# 2. 源码实现



    我先自己用实现了一遍，然后再看它的源码，对比发现对python的使用还有待提高！




    自己实现的KNN代码：






```python
#编码实现KNN

from numpy import * 
import operator 

def creatDataset(): 
    samples = [[0, 0, 0, 0.5], [0, 0.5, 0, 1], [1, 1, 0.5, 0]]  #training samples features
    samples = mat(samples) 
    labels = [0, 0, 1]  #the labels 
    return samples, labels 

def kNNClassifier(traSamples, lables, k, tstSample): 
    samNum,feaDim = shape(traSamples);  # each line is one sample 
    minDist = 10
    classifiedLabel = labels[0]
    for i in range(samNum): 
        tmpDist = (traSamples[i] - tstSample) * (traSamples[i] - tstSample).T  # notice that tmpDist is a matrix here
        print tmpDist 
        if(tmpDist[0][0] < minDist):  # since tmpDist is a matrix 
            minDist = tmpDist 
            classifiedLabel = labels[i] 
    return classifiedLabel 

tstSample = mat([[1, 1, 1, 0]] )
samples, labels = creatDataset()
print kNNClassifier(samples, labels, 1, tstSample)
```





    源码KNN： 



```python
def classify0(inX, dataSet, labels, k): 
    dataSetSize = dataSet.shape[0] # the number of samples 
    
    # tile function is the same as "replicate" function of MATLAB
    # 这个技巧就避免了循环语句
    diffMat = tile(inX, (dataSetSize, 1)) - dataSet # replicate inX into dataSetSize * 1
    sqDiffMat = diffMat**2  # 对应元素平方
    sqDistances = sqDiffMat.sum(axis = 1)  # 按行求和
    distances = sqDistances**0.5  # 开方求距离
    
    sortedDistIndicies = distances.argsort()  # argsort函数返回的是数组值从小到大的索引值
    classCount = {} 
    # 投票
    for i in range(k): 
        voteIlabel = labels[sortedDistIndicies[i]] #排名第i近的样本的label
        classCount[voteIlabel] = classCount.get(voteIlabel, 0) + 1  #get字典的元素，如果不存在key，则为0
    # operator.itemgetter(1)按照value排序；也可以用 key = lambda asd:asd[1]
    # 排序完，原classCount不变
    sortedClassCount = sorted(classCount.iteritems(),  # 键值对
                              key = operator.itemgetter(1), reverse = True)  #逆序排列 
    
    return sortedClassCount[0][0]  #输出第一个，也就是最近邻
```


详细的解释上面有了，**总结：注意使用tile(), **2, **0.5, sum(axis = 1), 数组的argsort(), 字典的get(), 和sorted用法。**








