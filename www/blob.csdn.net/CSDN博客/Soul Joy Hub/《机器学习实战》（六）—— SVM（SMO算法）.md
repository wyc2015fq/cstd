# 《机器学习实战》（六）—— SVM（SMO算法） - Soul Joy Hub - CSDN博客

2017年08月14日 21:17:27[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：6444
所属专栏：[机器学习技法与实战](https://blog.csdn.net/column/details/16096.html)



[http://blog.csdn.net/u011239443/article/details/77170119](http://blog.csdn.net/u011239443/article/details/77170119)

关于SVM的讲解可以参阅《机器学习技法》的相关笔记：[http://blog.csdn.net/u011239443/article/details/76572743](http://blog.csdn.net/u011239443/article/details/76572743)

# SMO算法

![](http://upload-images.jianshu.io/upload_images/1621805-47f9e35d885db9b2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-9b66def703b5be62.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-7aa53cef14dc22e2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-8efcf55be9a8f3e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-a0c8d865fc4ef0d8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-1ca80518b5e9d431.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-0b443ce2ee7f5d2f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-c2cbf8fc358ccfad.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-f7ed35b29a459ba0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-9c6c1e417cc71354.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

![](http://upload-images.jianshu.io/upload_images/1621805-a0724ac2af1410d2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 实现

```python
# -*- coding: utf-8 -*-
from numpy import *
# 加载数据
def loadDataSet(fileName):
    dataMat = [];labelMat = []
    fr = open(fileName)
    for line in fr.readlines():
        lineArr = line.strip().split('\t')
        dataMat.append([float(lineArr[0]),float(lineArr[1])])
        labelMat.append([float(lineArr[2])])
    return  dataMat,labelMat

# 从0到m中产生一个不为i的整数
def selectJrand(i,m):
    j = i
    while(j == i):
        j = int(random.uniform(0,m))
    return j

# 使得aj 在边界值[L,H]以内
def clipAlpha(aj,H,L):
    if aj > H:
        aj = H
    if L > aj:
        aj = L
    return aj

#SMO 序列最小优化
def smoSimple(dataMatIn,classLabels,C,toler,maxIter):
    # 这里书上的代码有误，不用mat(classLabels)不用转置
    dataMat = mat(dataMatIn);labelMat = mat(classLabels);
    b = 0;m,n = shape(dataMat)
    alphas = mat(zeros((m,1)))
    iter = 0
    # alphaPairsChanged 用来更新的次数
    # 当遍历  连续无更新 maxIter 轮，则认为收敛，迭代结束 
    while iter < maxIter:
        alphaPairsChanged = 0
        for i in range(m):
            # KKT 条件计算出
            fXi = float(multiply(alphas,labelMat).T * (dataMat*dataMat[i,:].T))+b
            # 误差
            Ei = fXi - float(labelMat[i])
            # toler：容忍错误的程度
            # labelMat[i]*Ei < -toler 则需要alphas[i]增大，但是不能>=C
            # labelMat[i]*Ei > toler 则需要alphas[i]减小，但是不能<=0
            if ((labelMat[i]*Ei < -toler) and (alphas[i] < C)) or ((labelMat[i]*Ei > toler) and (alphas[i] > 0)):
                # 从0到m中产生一个不为i的整数
                j = selectJrand(i,m)
                fXj = float(multiply(alphas,labelMat).T * (dataMat*dataMat[j,:].T)+b)
                Ej = fXj - float(labelMat[j])
                alphaIold = alphas[i].copy()
                alphaJold = alphas[j].copy()
               # 算法讲解“二变量优化问题”部分
                if labelMat[i] != labelMat[j]:
                    L = max(0,alphas[j]-alphas[i])
                    H = min(C,C+alphas[j]-alphas[i])
                else:
                    L = max(0,alphas[j] + alphas[i] - C)
                    H = min(C,alphas[j] + alphas[i])

                if L == H:print "L == H";continue

               # 见式 7.107
                eta = -2.0* dataMat[i,:] * dataMat[j,:].T + dataMat[i,:] *\
                                                            dataMat[i,:].T + dataMat[j,:] * dataMat[j,:].T
                if eta <= 0:print "eta <= 0";continue
                # 见式 7.106
                alphas[j] += labelMat[j]*(Ei-Ej)/eta
                # 见式 7.108 ，讲alphas[j] 约束在 [L,H]
                alphas[j] = clipAlpha(alphas[j],H,L)
                if(abs(alphas[j] - alphaJold) < 0.00001):
                    print "j not moving"
               # 见式 7.109
                alphas[i] += labelMat[j]*labelMat[i]*(alphaJold-alphas[j])
               # 见式 7.114
                b1 = b - Ei -labelMat[i]*(alphas[i]-alphaIold)*dataMat[i,:]*dataMat[i,:].T\
                     - labelMat[j]*(alphas[j]-alphaJold)*dataMat[i,:]*dataMat[j,:].T
                b2 = b - Ej - labelMat[i] * (alphas[i] - alphaIold) * dataMat[i, :] * dataMat[j, :].T \
                     - labelMat[j] * (alphas[j] - alphaJold) * dataMat[j, :] * dataMat[j, :].T

                if  (0 < alphas[i]) and (C > alphas[i]):
                    b = b1
                elif (0 < alphas[j]) and (C > alphas[j]):
                    b = b2
                else:
                    b = (b1+b2)/2.0

                alphaPairsChanged += 算法1
                print "iter: %d i:%d pairs: %d" %(iter,i,alphaPairsChanged)

        if alphaPairsChanged == 0:
            iter += 1
        else:
            iter = 0
        print "iter %d" %iter

    return b,alphas
```

# SOM背后的含义

我们来看下SVM的对偶问题：

![](http://upload-images.jianshu.io/upload_images/1621805-5a7b92743c8edfbd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

没错，我们在不断的调整alphas，其实就是在不断的调整alpha来优化问题。

就如我们在《机器学习技法》中说的，无论是SVM 还是 PLA，w都是$y_nz_n$的组合，可以看成是由原始的数据表示出来的：

![](http://upload-images.jianshu.io/upload_images/1621805-74d9305ae0879b4d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

而有些数据中的记录没用，我们就将它的alpha调整为0，有用记录 alpha > 0。所谓的有用的，也就是我们边界上边的点，即支持向量。

b：
`[[-3.83903814]]`
alphas[alphas>0]，即支持向量所对应的alpha:
`[[ 0.12744319  0.24145985  0.36890304]]`
![这里写图片描述](https://img-blog.csdn.net/20170814211806130?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

