# 【Python学习系列十一】Python实现决策树实现C4.5(信息增益率) - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月12日 16:31:53[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3341
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









C4.5是基于ID3改进的分类决策树算法，特点是C4.用信息增益率来选择属性，而ID3使用的是熵（entropy， 熵是一种不纯度度量准则），且对非离散数据也能处理，能够对不完整数据进行处理。


- 
1、信息熵：
- ![用Python实现C4.5算法](http://s13.sinaimg.cn/mw690/002NTcFtzy73eY5DLhq8c&690)
- 2、条件熵：
- 
![用Python实现C4.5算法](http://s2.sinaimg.cn/mw690/002NTcFtzy73eY8ddmx41&690)


- 
3、信息增益：

g(D,A)​=H(D)-H(D/A)


4、信息增益率：

gr(D,A)=g(D,A)/H(A)​


参考代码如下：






1）C45DTree.py



```python
# -*- coding: utf-8 -*-

from numpy import *
import math
import copy
import cPickle as pickle


class C45DTree(object):
    def __init__(self):  # 构造方法
        self.tree = {}  # 生成树
        self.dataSet = []  # 数据集
        self.labels = []  # 标签集


    # 数据导入函数
    def loadDataSet(self, path, labels):
        recordList = []
        fp = open(path, "rb")  # 读取文件内容
        content = fp.read()
        fp.close()
        rowList = content.splitlines()  # 按行转换为一维表
        recordList = [row.split(",") for row in rowList if row.strip()]  # strip()函数删除空格、Tab等
        self.dataSet = recordList
        self.labels = labels


    # 执行决策树函数
    def train(self):
        labels = copy.deepcopy(self.labels)
        self.tree = self.buildTree(self.dataSet, labels)


    # 构件决策树：穿件决策树主程序
    def buildTree(self, dataSet, lables):
        cateList = [data[-1] for data in dataSet]  # 抽取源数据集中的决策标签列
        # 程序终止条件1：如果classList只有一种决策标签，停止划分，返回这个决策标签
        if cateList.count(cateList[0]) == len(cateList):
            return cateList[0]
        # 程序终止条件2：如果数据集的第一个决策标签只有一个，返回这个标签
        if len(dataSet[0]) == 1:
            return self.maxCate(cateList)
        # 核心部分
        bestFeat, featValueList= self.getBestFeat(dataSet)  # 返回数据集的最优特征轴
        bestFeatLabel = lables[bestFeat]
        tree = {bestFeatLabel: {}}
        del (lables[bestFeat])
        for value in featValueList:  # 决策树递归生长
            subLables = lables[:]  # 将删除后的特征类别集建立子类别集
            # 按最优特征列和值分隔数据集
            splitDataset = self.splitDataSet(dataSet, bestFeat, value)
            subTree = self.buildTree(splitDataset, subLables)  # 构建子树
            tree[bestFeatLabel][value] = subTree
        return tree


    # 计算出现次数最多的类别标签
    def maxCate(self, cateList):
        items = dict([(cateList.count(i), i) for i in cateList])
        return items[max(items.keys())]


    # 计算最优特征
    def getBestFeat(self, dataSet):
        Num_Feats = len(dataSet[0][:-1])
        totality = len(dataSet)
        BaseEntropy = self.computeEntropy(dataSet)
        ConditionEntropy = []     # 初始化条件熵
        slpitInfo = []    # for C4.5,caculate gain ratio
        allFeatVList = []
        for f in xrange(Num_Feats):
            featList = [example[f] for example in dataSet]
            [splitI, featureValueList] = self.computeSplitInfo(featList)
            allFeatVList.append(featureValueList)
            slpitInfo.append(splitI)
            resultGain = 0.0
            for value in featureValueList:
                subSet = self.splitDataSet(dataSet, f, value)
                appearNum = float(len(subSet))
                subEntropy = self.computeEntropy(subSet)
                resultGain += (appearNum/totality)*subEntropy
            ConditionEntropy.append(resultGain)    # 总条件熵
        infoGainArray = BaseEntropy*ones(Num_Feats)-array(ConditionEntropy)
        infoGainRatio = infoGainArray/array(slpitInfo)  # C4.5信息增益的计算
        bestFeatureIndex = argsort(-infoGainRatio)[0]
        return bestFeatureIndex, allFeatVList[bestFeatureIndex]

    # 计算划分信息
    def computeSplitInfo(self, featureVList):
        numEntries = len(featureVList)
        featureVauleSetList = list(set(featureVList))
        valueCounts = [featureVList.count(featVec) for featVec in featureVauleSetList]
        pList = [float(item)/numEntries for item in valueCounts]
        lList = [item*math.log(item, 2) for item in pList]
        splitInfo = -sum(lList)
        return splitInfo, featureVauleSetList


 # 计算信息熵
    # @staticmethod
    def computeEntropy(self, dataSet):
        dataLen = float(len(dataSet))
        cateList = [data[-1] for data in dataSet]  # 从数据集中得到类别标签
        # 得到类别为key、 出现次数value的字典
        items = dict([(i, cateList.count(i)) for i in cateList])
        infoEntropy = 0.0
        for key in items:  # 香农熵： = -p*log2(p) --infoEntropy = -prob * log(prob, 2)
            prob = float(items[key]) / dataLen
            infoEntropy -= prob * math.log(prob, 2)
        return infoEntropy


    # 划分数据集： 分割数据集； 删除特征轴所在的数据列，返回剩余的数据集
    # dataSet : 数据集； axis: 特征轴； value: 特征轴的取值
    def splitDataSet(self, dataSet, axis, value):
        rtnList = []
        for featVec in dataSet:
            if featVec[axis] == value:
                rFeatVec = featVec[:axis]  # list操作：提取0~（axis-1）的元素
                rFeatVec.extend(featVec[axis + 1:])   # 将特征轴之后的元素加回
                rtnList.append(rFeatVec)
        return rtnList

    # 存取树到文件
    def storetree(self, inputTree, filename):
        fw = open(filename,'w')
        pickle.dump(inputTree, fw)
        fw.close()

    # 从文件抓取树
    def grabTree(self, filename):
        fr = open(filename)
        return pickle.load(fr)
```


2）C45DTreeDemo.py



```python
# -*- coding: utf-8 -*-

from numpy import *
from C45DTree import *

dtree = C45DTree()
dtree.loadDataSet("D:\dataset.dat",["outlook", "temperature", "humidity", "windy"])
dtree.train()

dtree.storetree(dtree.tree, "data.tree")
mytree = dtree.grabTree("data.tree")

print mytree
```


3）测试数据和执行结果：



```
0, 0, 0, 0, N 
0, 0, 0, 1, N 
1, 0, 0, 0, Y 
2, 1, 0, 0, Y 
2, 2, 1, 0, Y 
2, 2, 1, 1, N 
1, 2, 1, 1, Y
```

`{'windy': {' 0': {'outlook': {'1': ' Y ', '0': ' N ', '2': ' Y '}}, ' 1': {'outlook': {'1': ' Y', '0': ' N ', '2': ' N '}}}}`




