# 【读书笔记】机器学习实战 6.3节 SVM-简化版SMO算法 - xiaxzhou的博客 - CSDN博客





2017年05月12日 15:33:10[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：1881








# 机器学习实战

> 
6.3节 支持向量机SVM -简化版SMO算法


```python
#!/usr/bin/python
# -*- coding: utf-8 -*-
import random
from numpy import *

def loadDataSet(fileName):  # 打开文件，返回特征列表，标签列表
    dataMat = [];labelMat = []
    fr = open(fileName)
    for line in fr.readlines():
        lineArr = line.strip().split('\t')
        dataMat.append([float(lineArr[0]),float(lineArr[1])])  # 特征
        labelMat.append(float(lineArr[2]))  # 标签
    return dataMat,labelMat

def selectJrand(i,m):  # 随机选择函数：返回选择索引
    j = i
    while (j==i):
        j = int(random.uniform(0,m))
    return j

def clipAlpha(aj,H,L):  # 限制aj范围
    if aj>H:
        aj = H
    if L>aj:
        aj =L
    return aj

def smoSimple(dataMatIn, classLabels, C, toler, maxIter):  # 简化版smo算法 ，输入：样本列表，标签列表，C，容错率，最大迭代次数
    dataMatrix = mat(dataMatIn); labelMat = mat(classLabels).transpose() # 样本列表，标签列表转化为numpy矩阵
    b = 0; m,n = shape(dataMatrix)
    alphas = mat(zeros((m,1)))  # alpha向量初始化为0
    iter = 0                    # alpha保持没有改变状态的迭代次数统计
    while (iter < maxIter):
        alphaPairsChanged = 0   # flag：记录alpha是否被优化
        for i in range(m):      # 对每个样本数据
            tmp_1 =  multiply(alphas,labelMat)
            tmp_2 = multiply(tmp_1,dataMatrix)
            tmp_3 = tmp_2*dataMatrix[i,:].T
            fXi = tmp_3.T*mat(ones((m,1))) + b                                              # 模型计算f(Xi)
            fXi = float(multiply(alphas,labelMat).T*(dataMatrix*dataMatrix[i,:].T)) + b     # 预测样本类别f(Xi)，判定是否可以优化
            Ei = fXi - float(labelMat[i])                                                   # 判断样本i误差Ei if checks if an example violates KKT conditions
            if ((labelMat[i]*Ei < -toler) and (alphas[i] < C)) or ((labelMat[i]*Ei > toler) and (alphas[i] > 0)):  # 若误差较大，且alpha没有到边界
                j = selectJrand(i,m)                                                        # 随机选择另一个要优化的alpha
                fXj = float(multiply(alphas,labelMat).T*(dataMatrix*dataMatrix[j,:].T)) + b # 预测f(Xj)
                Ej = fXj - float(labelMat[j])                                               # 判断样本j误差Ej
                alphaIold = alphas[i].copy(); alphaJold = alphas[j].copy();                 # 深拷贝oldalpha值
                # SMO算法参考：http://blog.csdn.net/luoshixian099/article/details/51227754
                if (labelMat[i] != labelMat[j]):   # 参考 http://staff.ustc.edu.cn/~ketang/PPT/PRLec5.pdf
                    L = max(0, alphas[j] - alphas[i])
                    H = min(C, C + alphas[j] - alphas[i])
                else:
                    L = max(0, alphas[j] + alphas[i] - C)
                    H = min(C, alphas[j] + alphas[i])
                if L==H:
                    print "L==H"
                    continue
                eta = 2.0 * dataMatrix[i,:]*dataMatrix[j,:].T - dataMatrix[i,:]*dataMatrix[i,:].T - dataMatrix[j,:]*dataMatrix[j,:].T  # 二阶导数 计算alphas[j]的最优修改量
                if eta >= 0:
                    print "eta>=0"
                    continue
                alphas[j] -= labelMat[j]*(Ei - Ej)/eta      # 修改alphas[j]的值
                alphas[j] = clipAlpha(alphas[j],H,L)        # 限制alphas[j]的值
                if (abs(alphas[j] - alphaJold) < 0.00001):  # 判定alphas[j]是否轻微改变
                    print "j not moving enough"
                    continue
                alphas[i] += labelMat[j]*labelMat[i]*(alphaJold - alphas[j]) #  相反方向修改alphas[i]，改变的大小一样  update i by the same amount as j
                                                                        #the update is in the oppostie direction
                b1 = b - Ei- labelMat[i]*(alphas[i]-alphaIold)*dataMatrix[i,:]*dataMatrix[i,:].T - labelMat[j]*(alphas[j]-alphaJold)*dataMatrix[i,:]*dataMatrix[j,:].T  # 确定偏移项b
                b2 = b - Ej- labelMat[i]*(alphas[i]-alphaIold)*dataMatrix[i,:]*dataMatrix[j,:].T - labelMat[j]*(alphas[j]-alphaJold)*dataMatrix[j,:]*dataMatrix[j,:].T
                if (0 < alphas[i]) and (C > alphas[i]):
                    b = b1
                elif (0 < alphas[j]) and (C > alphas[j]):
                    b = b2
                else:
                    b = (b1 + b2)/2.0
                alphaPairsChanged += 1  # 更新修改flag
                print "iter: %d i:%d, pairs changed %d" % (iter,i,alphaPairsChanged)
        if (alphaPairsChanged == 0):  # 迭代统计
            iter += 1
        else:
            iter = 0
        print "iteration number: %d" % iter
    return b,alphas

dataArr,labelArr = loadDataSet('testSet.txt')
b,alphas = smoSimple(dataArr,labelArr,0.6,0.001,40)

tmp = shape(alphas[alphas>0])
for i in range(100):
    if alphas[i]>0.0:
        print dataArr[i],labelArr[i]
pass
```



