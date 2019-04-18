# 《机器学习实战》（十三）—— PCA - Soul Joy Hub - CSDN博客

2017年08月18日 09:55:50[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1718
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



[http://blog.csdn.net/u011239443/article/details/77363466](http://blog.csdn.net/u011239443/article/details/77363466)

# 协方差矩阵

## 统计学的基本概念

![](http://upload-images.jianshu.io/upload_images/1621805-de8127f205b1494d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 协方差

上面几个统计量看似已经描述的差不多了，但我们应该注意到，标准差和方差一般是用来描述一维数据的，但现实生活我们常常遇到含有多维数据的数据集，最简单的大家上学时免不了要统计多个学科的考试成绩。面对这样的数据集，我们当然可以按照每一维独立的计算其方差，但是通常我们还想了解更多，协方差就是这样一种用来度量两个随机变量关系的统计量，我们可以仿照方差的定义：

![](http://upload-images.jianshu.io/upload_images/1621805-b3b288fe9fd2a536.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

来度量各个维度偏离其均值的程度，标准差可以这么来定义：

![](http://upload-images.jianshu.io/upload_images/1621805-8e860eebbdf97ac8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

从协方差的定义上我们也可以看出一些显而易见的性质，如：

![](http://upload-images.jianshu.io/upload_images/1621805-eedefa422e89ee91.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 协方差矩阵

举一个简单的三维的例子，假设数据集有{x,y,z}{x,y,z}三个维度，则协方差矩阵为：

![](http://upload-images.jianshu.io/upload_images/1621805-35aeeb6920484902.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

## 求解协方差矩阵的步骤

举个例子：

![](http://upload-images.jianshu.io/upload_images/1621805-b44a99bfa4ca1f27.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# PCA

## 算法步骤
- 形成样本矩阵，样本中心化
- 计算样本矩阵的协方差矩阵
- 对协方差矩阵进行特征值分解，选取最大的 p 个特征值对应的特征向量组成投影矩阵
- 对原始样本矩阵进行投影，得到降维后的新样本矩阵

## 推导

为什么PCA和协方差扯上关系呢？接下来我们来进行推导：

对于每个点：

![](http://upload-images.jianshu.io/upload_images/1621805-0119b082b2d213ff.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

会有一个对应的编码向量： 
![](http://upload-images.jianshu.io/upload_images/1621805-4c8328c8644ddf00.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

我们希望找到一个编码函数，根据输入返回编码，f(x) = c；我们也希望找到一个解码函数，给定编码重构输入：

![](http://upload-images.jianshu.io/upload_images/1621805-f49bc3367a550ec2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

为了简化解码器，我们使用矩阵乘法将编码映射回$R^n$ ，即 g(c) = Dc

![](http://upload-images.jianshu.io/upload_images/1621805-62142dbb1d70e3a2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-2fe2cd5833e215d0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-c009def72c39dad5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-782505c2a61e2221.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-8b18a43713701b9b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/1621805-2686fe99aed02378.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

推导到这里，我们可以看到我们的最有解和协方差矩阵的联系。其实协方差矩阵度量的是维度与维度之间的关系，而非样本与样本之间。协方差矩阵的主对角线上的元素是各个维度上的方差(即能量)，其他元素是两两维度间的协方差(即相关性)。

# 实现

```python
from numpy import *


# 加载数据
def loadDataSet(filename,delim = '\t'):
    fr = open(filename)
    stringArr = [line.strip().split(delim) for line in fr.readlines()]
    dataArr = [map(float,line) for line in stringArr]
    return mat(dataArr)

def pca(dataMat,topN=999999):
    # 形成样本矩阵，样本中心化
    meanVals= mean(dataMat,axis=0)
    meanRemoved = dataMat - meanVals
    # 计算样本矩阵的协方差矩阵
    covMat = cov(meanRemoved,rowvar=0)
    #  对协方差矩阵进行特征值分解，选取最大的 p 个特征值对应的特征向量组成投影矩阵
    eigVals,eigVects =  linalg.eig(mat(covMat))
    eigValInd = argsort(eigVals)
    eigValInd = eigValInd[:-(topN+1):-1]
    redEigVects = eigVects[:,eigValInd]
    # 对原始样本矩阵进行投影，得到降维后的新样本矩阵
    lowDDataMat = meanRemoved * redEigVects
    reconMat = (lowDDataMat * redEigVects.T)+meanVals
    return lowDDataMat,reconMat
```

```python
import myPca
import matplotlib
import matplotlib.pyplot as plt
dataMat = myPca.loadDataSet('testSet.txt')
lowMat,reconMat = myPca.pca(dataMat,1)
fig = plt.figure()
ax = fig.add_subplot(111)
ax.scatter(dataMat[:,0].flatten().A[0],dataMat[:,1].flatten().A[0],marker='^',s=90)
ax.scatter(reconMat[:,0].flatten().A[0],reconMat[:,1].flatten().A[0],marker='o',s=50,c='red')
plt.show()
```

![](http://upload-images.jianshu.io/upload_images/1621805-f7be0762ec43adf2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![这里写图片描述](https://img-blog.csdn.net/20170818095527394?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

