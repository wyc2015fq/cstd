# 【读书笔记】机器学习实战 第7章 7.6节 在难数据集上应用AdaBoost - xiaxzhou的博客 - CSDN博客





2017年06月06日 15:48:27[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：278








> 
AdaBoost没有**过拟合**：有文献称，对于表现好的数据集，AdaBoost的测试错误率就会达到一个稳定值，**并不会随着分类器的增多而上升**。


```python
# 自适应数据加载函数
def loadDataSet(fileName): # general function to parse tab -delimited floats
    numFeat = len(open(fileName).readline().split('\t')) # 获取列数 ，最后一列为类别标签 get number of fields
    dataMat = []; labelMat = []
    fr = open(fileName)
    for line in fr.readlines():
        lineArr =[]
        curLine = line.strip().split('\t')
        for i in range(numFeat-1): # 不需指定特征维度
            lineArr.append(float(curLine[i]))
        dataMat.append(lineArr) # 样本特征
        labelMat.append(float(curLine[-1])) # 最后一列为类别标签
    return dataMat,labelMat # 返回样本特征和类别

#D = mat(ones((5,1))/5)
#datMat,classLabels = loadSimpData()
#buildStump(datMat,classLabels,D)
#classifierrr,agg = adaBoostTrainDS(datMat,classLabels)
#re = adaClassify([0,0],classifierrr)
datArr,labelArr = loadDataSet('horseColicTraining2.txt')
classifierArray,agg  = adaBoostTrainDS(datArr,labelArr,10)

testArr,testLabelArr = loadDataSet('horseColicTest2.txt')
prediction10 = adaClassify(testArr,classifierArray)
errArr = mat(ones((67,1)))
su = errArr[prediction10 != mat(testLabelArr).T].sum()
```



