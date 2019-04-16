# 《机器学习实战》K近邻(KNN)分类 - littlely_ll的博客 - CSDN博客





2017年01月09日 16:49:23[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：297








## R与Python在机器学习中的比较

$\quad$最近开始用python实现机器学习算法，通过对比R与python实现机器学习算法，本人发现，用python实现算法比R实现算法更好理解。使用python能让你理解算法的每一步是怎样做的，而在R中，只有一个实现算法的函数和其中要设置的参数，虽然只要理解机器学习算法的内容就能知道参数所代表的含义进而设置其中的参数，但是R中的计算就像一个黑盒子，不知道过程是怎样实现的。而使用python能一步一步地实现算法，进而更能理解算法的含义。我认为，在刚开始学习机器学习时应该用python来实现，这样能更快的理解算法，当对算法熟练到一定程度时，可以用R来实现，因为这时你就不用再一步一步地去用python编写大量的程序了，而直接可以用R的一个函数来实现。当然，对于大型的机器学习而言，我认为python的优势比较明显，因为它更容易实现，而对于用R来说，它有一些力不从心，当然这是对于我自己来说的。所以我认为学习R与python都是必须的，两门都掌握将会帮助你在机器学习领域大展身手。这些都是我自己的一些浅解，python我也正在学习中。 
$\quad$最近在看《机器学习实战》，里面讲的还是挺不错的，虽然有一些错误，但也瑕不掩瑜，是入手python和机器学习的最佳资料。本博文也是记录我在学习《机器学习实战》中的一些理解，并对其中的一些代码做一些解释，以备忘记。

## KNN实现

```python
## prepare: importing data with python
from numpy import *
import operator

def createDataSet():
    group = array([[1.0,1.1],[1.0,1.0],[0,0],[0,0.1]])
    labels = ['A','A','B','B']
    return group, labels

group,labels = createDataSet()

## k-Nearest Neighbors algorithm
def classify0(inX, dataSet, labels, k):
    dataSetSize = dataSet.shape[0] #shape见《python小函数（一）》
    diffMat = tile(inX, (dataSetSize,1)) - dataSet #tile见《python小函数（一）》
    sqDiffMat = diffMat**2 # 特征坐标的差值
    sqDistances = sqDiffMat.sum(axis=1)# 对差值按行求和，sum见《python小函数（一）》
    distances = sqDistances**0.5 #计算欧氏距离
    sortedDistIndicies = distances.argsort() #对距离按从小到大排序，argsort见《python小函数（一）》，其类似于R中的order函数
    classCount={} #空列表，存储类的个数
    for i in range(k):
        voteIlabel = labels[sortedDistIndicies[i]]
        classCount[voteIlabel] = classCount.get(voteIlabel,0) + 1
    sortedClassCount = sorted(classCount.iteritems(),
     key=operator.itemgetter(1), reverse=True) #sorted函数在不改变原序列条件下排序，itermgetter返回的是一个用于提取数值的函数，
    return sortedClassCount[0][0]

## text record to numpy parsing code
def file2matrix(filename):
    fr = open(filename)
    numberOfLines = len(fr.readlines())
    returnMat = zeros((numberOfLines,3)) #返回零矩阵
    classLabelVector = []
    fr = open(filename)
    index = 0
    for line in fr.readlines():
        line = line.strip() #strip见《python小函数（一）》
        listFromLine = line.split('\t')#split见《python小函数（一）》
        returnMat[index,:] = listFromLine[0:3]# 从0到3但不包括3
        classLabelVector.append(int(listFromLine[-1])) #append见《python小函数（一）》
        index += 1
    return returnMat, classLabelVector

##create scatter plot with matplotlib    
import matplotlib
import matplotlib.pyplot as plt
fig = plt.figure()
ax = fig.add_subplot(111)#111分别表示表示1行1列1层
ax.scatter(datingDataMat[:,1],datingDataMat[:,2])
plt.show()

## data-normalizing code
def autoNorm(dataSet):
    minVals = dataSet.min(0)#对列求最小值
    maxVals = dataSet.max(0)#对列求最大值
    ranges = maxVals - minVals
    m = dataSet.shape[0]#dataset有多少行
    normDataSet = dataSet - tile(minVals,(m,1))#求差值必须一一对应，而不像R中可以循环替代
    normDataSet = normDataSet/tile(ranges,(m,1)) #上同
    return normDataSet, ranges, minVals

##classifier testing code for dating site    
def datingClassTest():
    hoRatio = 0.10
    datingDataMat, datingLabels = file2matrix(r'c:/Users/ll/Documents/datingTestSet2.txt')
    normMat, ranges, minVals = autoNorm(datingDataMat)
    m = normMat.shape[0]
    numTestVecs = int(m*hoRatio)#抽总数据中一定比例检测
    errorCount = 0.0
    for i in range(numTestVecs):
        classifierResult = classify0(normMat[i,:],normMat[numTestVecs:m,:],\
        datingLabels[numTestVecs:m],3)
        print i, " the classifier came back with: %d, the real answer is: %d" % (classifierResult,datingLabels[i])
        if (classifierResult != datingLabels[i]):
            errorCount += 1.0
    print "the total error rate is: %f" % (errorCount/float(numTestVecs))

##dating site predictor function
def classifyPerson():
    resultList = ['not at all','in small doses','in large doses']
    personTats = float(raw_input("percentage of time spent playing video games?"))
    ffMiles = float(raw_input("frequent flier miles earned per year?"))
    iceCream = float(raw_input("liters of ice cream consued per year?"))
    datingDataMat,datingLabels = file2matrix(r'c:/Users/ll/Documents/datingTestSet2.txt')
    normMat, ranges, minVals = autoNorm(datingDataMat)
    inArr = array([ffMiles,personTats, iceCream])
    classifierResult = classify0((inArr-minVals)/ranges,normMat,datingLabels,3)
    print "You will probably like this person: ", resultList[classifierResult - 1]

def img2vector(filename):
    returnVect = zeros((1,1024))
    fr = open(filename)
    for i in range(32):#每个图像32行32列
        lineStr = fr.readline()
        for j in range(32):
            returnVect[0,32*i+j] = int(lineStr[j])
    return returnVect

##handwritten digits testing code
import os    
def handwritingClassTest():
    hwLabels = []
    trainingFileList = os.listdir(r'c:/Users/ll/Documents/trainingDigits')
    m = len(trainingFileList)
    trainingMat = zeros((m,1024))
    for i in range(m):
        fileNameStr = trainingFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classNumStr = int(fileStr.split('_')[0])
        hwLabels.append(classNumStr)
        trainingMat[i,:] = img2vector(r'c:/Users/ll/Documents/trainingDigits/%s' % fileNameStr)
testFileList = os.listdir(r'c:/Users/ll/Documents/testDigits')
errorCount = 0.0
mTest = len(testFileList)
for i in range(mTest):
    fileNameStr = testFileList[i]
    fileStr = fileNameStr.split('.')[0]
    classNumStr = int(fileStr.split('_')[0])
    vectorUnderTest = img2vector(r'c:/Users/ll/Documents/testDigits/%s' % fileNameStr)
    classifierResult = classify0(vectorUnderTest,trainingMat,hwLabels, 3)
    print "the classifier came back with: %d, the real answer is: %d" % (classifierResult,classNumStr)
    if (classifierResult != classNumStr): errorCount += 1.0
print "\nthe total number of error is: %d" % errorCount
print "\nthe total error rate is: %f" % (errorCount/float(mTest))
```



