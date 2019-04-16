# 《机器学习实战》朴素贝叶斯（Naive Bayes）分类 - littlely_ll的博客 - CSDN博客





2017年01月20日 11:47:16[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：715








[1. 《机器学习实战》K近邻(KNN)分类](http://blog.csdn.net/littlely_ll/article/details/54290007)
[2. 《机器学习实战》决策树](http://blog.csdn.net/littlely_ll/article/details/54290610)
$\quad$对于朴素贝叶斯理论分析可看[朴素贝叶斯法及其R实现](http://blog.csdn.net/littlely_ll/article/details/53738124)，对于这篇的R实现，只是在这种特殊情况，对于一般的情况并没有实现，所以，本篇文章使用python实现朴素贝叶斯分类的一般方法，并对垃圾邮件进行分类。
```python
##word list vector function
def loadDataSet():
    postingList = [['my','dog','has','flea','problems','help','please'],['maybe','not','take','him','to','dog','park','stupid'],['my','dalmation','is','so','cute','I','love','him'],['stop','posting','stupid','worthless','garbage'],['mr','licks','ate','my','steak','how','to','stop','him'],['quit','buying','worthless','dog','food','stupid']]
    classVec = [0,1,0,1,0,1]
    return postingList, classVec

def createVocabList(dataSet):
    vocabSet = set([])
    for document in dataSet:
        vocabSet = vocabSet | set(document)#创建集合的并集
    return list(vocabSet)

def setOfWords2Vec(vocabList,inputSet):
    returnVec = [0]*len(vocabList)
    for word in inputSet:
        if word in vocabList:
            returnVec[vocabList.index(word)] = 1
        else: print "the word: %s is not in my Vocabulary!" % word
    return returnVec

##change setOfWords2Vec() to bagOfWords2VecMN() (bag of words model)
def bagOfWords2VecMN(vocabList,inputSet):
    returnVec = [0]*len(vocabList)
    for word in inputSet:
        if word in vocabList:
            returnVec[vocabList.index(word)] += 1
    return returnVec

##naive bayes classifier training function
from numpy import *
def trainNB0(trainMatrix,trainCategory):
    numTrainDocs = len(trainMatrix)
    numWords = len(trainMatrix[0])
    pAbusive = sum(trainCategory)/float(numTrainDocs)
    p0Num = ones(numWords); p1Num = ones(numWords)#每个单词至少有一个，即使用拉普拉斯方法
    p0Denom = 2.0; p1Denom = 2.0 #对每一个单词有2个类，所以加总为2
    for i in range(numTrainDocs):
        if trainCategory[i] == 1:
            p1Num += trainMatrix[i]
            p1Denom += sum(trainMatrix[i])
        else:
            p0Num += trainMatrix[i]
            p0Denom += sum(trainMatrix[i])
    p1Vect = log(p1Num/p1Denom)  #change to log()，防止数值过小
    p0Vect = log(p0Num/p0Denom)  #change to log()，同上
    return p0Vect, p1Vect, pAbusive

##Naive Bayes classify function    
def classifyNB(vec2Classify,p0Vec,p1Vec,pClass1):
    p1 = sum(vec2Classify * p1Vec) + log(pClass1)
    p0 = sum(vec2Classify *p0Vec) + log(1.0 - pClass1)
    if p1> p0:
        return 1
    else:
        return 0

def testingNB():
    listOPosts, listClasses = loadDataSet()
    myVocabList = createVocabList(listOPosts)
    trainMat = []
    for postinDoc in listOPosts:
        trainMat.append(setOfWords2Vec(myVocabList,postinDoc))
    p0V,p1V,pb = trainNB0(array(trainMat),array(listClasses))
    testEntry = ['love','my','dalmation']
    thisDoc = array(setOfWords2Vec(myVocabList,testEntry))
    print testEntry,'classified as: ',classifyNB(thisDoc,p0V,p1V,pAb)
    testEntry = ['stupid','garbage']
    thisDoc = array(setOfWords2Vec(myVocabList,testEntry))
    print testEntry,'classified as: ',classifyNB(thisDoc,p0V,p1V,pAb)

##file parsing and full spam test functions
def textParse(bigString):
    import re
    listOfTokens = re.split(r'\W*',bigString)
    return [tok.lower() for tok in listOfTokens if len(tok)>2]

def spamTest():
    docList = []; classList = []; fullText = []
    for i in range(1,26):
        wordList = textParse(open(r'c:/Users/ll/Documents/spam/%d.txt' % i).read())
        docList.append(wordList)
        fullText.extend(wordList)
        classList.append(1)
        wordList = textParse(open(r'c:/Users/ll/Documents/ham/%d.txt' % i).read())
        docList.append(wordList)
        fullText.extend(wordList)
        classList.append(0)
    vocabList = createVocabList(docList)
    trainingSet = range(50); testSet = []
    for i in range(10):
        randIndex = int(random.uniform(0,len(trainingSet)))
        testSet.append(trainingSet[randIndex])
        del(trainingSet[randIndex])
    trainMat = []; trainClasses = []
    for docIndex in trainingSet:
        trainMat.append(setOfWords2Vec(vocabList,docList[docIndex]))
        trainClasses.append(classList[docIndex])
    p0V,p1V,pSpam = trainNB0(array(trainMat),array(trainClasses))
    errorCount = 0
    for docIndex in testSet:
        wordVector = setOfWords2Vec(vocabList,docList[docIndex])
        if classifyNB(array(wordVector),p0V,p1V,pSpam) != classList[docIndex]:
            errorCount += 1
            print docList[docIndex]
    print 'the error rate is: ', float(errorCount)/len(testSet)
```




