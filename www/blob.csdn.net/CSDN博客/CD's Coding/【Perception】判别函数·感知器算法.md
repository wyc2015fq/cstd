# 【Perception】判别函数·感知器算法 - CD's Coding - CSDN博客





2017年01月03日 18:50:40[糖果天王](https://me.csdn.net/okcd00)阅读数：2215








## 0x00 前言

这是中科院（国科大）黄庆明老师的《模式识别与机器学习》课程 (2016Autumn) 中的内容，复习期间为了加深记忆尝试着敲了敲，对于MachineLearning而言，这在判别函数中是一种入门级的算法，作为入门ML的新手，我决定敲一下，于我而言感觉可以更有助于理解~

## 0x01 感知器算法
- 感知器算法实质上是一种赏罚过程
- 对正确分类的模式则“赏”，实际上是“不罚”，即权向量不变。
- 对错误分类的模式则“罚”，使w(k)加上一个正比于xk的分量。
- 当用全部模式样本训练过一轮以后，只要有一个模式是判别错误的，则需要进行下一轮迭代，即用全部模式样本再训练一次。
- 如此不断反复直到全部模式样本进行训练都能得到正确的分类结果为止。

![算法照片](https://img-blog.csdn.net/20170103184847149?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 0x02 Source Code

```python
#coding=utf8
# ========================================================
#   Copyright (C) 2016 All rights reserved.
#   
#   filename : hwk2.py
#   author   : okcd00
#   date     : 2017-01-02
#   desc     : Single Perceptron Algorithm
#   homepage : blog.csdn.net/okcd00
# ======================================================== 

import os,sys,time
import numpy as np
import pylab as pl
import matplotlib.pyplot as plt

reload(sys)
sys.setdefaultencoding('utf8')

def readFile(filename):
    with open(filename,'r') as f:
        matP, matN = [], []
        Flag = True
        for line in f.readlines():
            line = line.strip()
            if not line or line.startswith('#'): 
                Flag = False
                continue
            # Augmented vector
            if Flag: matP.append([int(item) for item in line.split(' ')] + [1])
            else   : matN.append([int(item) for item in line.split(' ')] + [1]) 
    return matP, matN

def showResult(l):
    ret = "d(x) = "
    for idx in range(l.__len__()-1):
        if l[idx] != 0.0: ret += "%.2f*x%d + " % (l[idx], idx+1)
    return "%s%.2f" % (ret, l[-1])

def Iterative_Solve(mat, w, row, midProcess = False):
    iter_idx, idx = 0, 0
    while True:
        Flag = True
        iter_idx += 1
        if midProcess: print "\nIteration <%d>:" % iter_idx
        for r in range(row):
            idx += 1
            judge = not np.dot(w, mat[r,:].T) > 0
            if judge: 
                Flag = False
                w = w + mat[r,:]
                if midProcess: 
                    print "w(%d) = w(%d) + x(%d)" % (idx, idx-1, r+1)            
                    print "w(%d) = " % idx, w
            elif midProcess: print "w(%d) = w(%d)" % (idx, idx-1)
        if Flag: break
    return showResult(list(w)[0])

if __name__ == '__main__':
    matP, matN = readFile('hwk2Data.txt')
    mat = np.concatenate((np.array(matP), -np.array(matN)), axis=0)

    size = mat.shape
    matw = np.zeros((1, size[1]))
    print "Matrix_X = \n", mat
    ans = Iterative_Solve(mat, matw, size[0], True)

    print "\nCorresponding discriminant function is:\n", ans
```

## 0x03 CodeTest
- 
实际问题 （实例 · 维度3） 

用感知器算法求下列模式分类的解向量w: 

ω1: {(0 0 0)T, (1 0 0)T, (1 0 1)T, (1 1 0)T} 

ω2: {(0 0 1)T, (0 1 1)T, (0 1 0)T, (1 1 1)T}- 
hwk2Data.txt


```
0 0 0
1 0 0
1 0 1
1 1 0
#####
0 0 1
0 1 1
0 1 0
1 1 1
```
- Result

```
>python hwk2.py
Matrix_X =
[[ 0  0  0  1]
 [ 1  0  0  1]
 [ 1  0  1  1]
 [ 1  1  0  1]
 [ 0  0 -1 -1]
 [ 0 -1 -1 -1]
 [ 0 -1  0 -1]
 [-1 -1 -1 -1]]

Iteration <1>:
w(1) = w(0) + x(1)
w(1) =  [[ 0.  0.  0.  1.]]
w(2) = w(1)
w(3) = w(2)
w(4) = w(3)
w(5) = w(4) + x(5)
w(5) =  [[ 0.  0. -1.  0.]]
w(6) = w(5)
w(7) = w(6) + x(7)
w(7) =  [[ 0. -1. -1. -1.]]
w(8) = w(7)

Iteration <2>:
w(9) = w(8) + x(1)
w(9) =  [[ 0. -1. -1.  0.]]
w(10) = w(9) + x(2)
w(10) =  [[ 1. -1. -1.  1.]]
w(11) = w(10)
w(12) = w(11)
w(13) = w(12) + x(5)
w(13) =  [[ 1. -1. -2.  0.]]
w(14) = w(13)
w(15) = w(14)
w(16) = w(15)

Iteration <3>:
w(17) = w(16) + x(1)
w(17) =  [[ 1. -1. -2.  1.]]
w(18) = w(17)
w(19) = w(18) + x(3)
w(19) =  [[ 2. -1. -1.  2.]]
w(20) = w(19)
w(21) = w(20) + x(5)
w(21) =  [[ 2. -1. -2.  1.]]
w(22) = w(21)
w(23) = w(22) + x(7)
w(23) =  [[ 2. -2. -2.  0.]]
w(24) = w(23)

Iteration <4>:
w(25) = w(24) + x(1)
w(25) =  [[ 2. -2. -2.  1.]]
w(26) = w(25)
w(27) = w(26)
w(28) = w(27)
w(29) = w(28)
w(30) = w(29)
w(31) = w(30)
w(32) = w(31)

Iteration <5>:
w(33) = w(32)
w(34) = w(33)
w(35) = w(34)
w(36) = w(35)
w(37) = w(36)
w(38) = w(37)
w(39) = w(38)
w(40) = w(39)

Corresponding discriminant function is:
d(x) = 2.00*x1 + -2.00*x2 + -2.00*x3 + 1.00
```

## 0xFF 关于感知器(Perception Approach)
- 基本思想 

采用感知器算法能通过对训练模式样本集的“学习”得到判别函数的系数。
- 说明 

这里采用的算法不需要对各类别中模式的统计性质做任何假设，因此称为确定性的方法。
- 出发点 

一旦判别函数的形式确定下来，不管它是线性的还是非线性的，剩下的问题就是如何确定它的系数。 

在模式识别中，系数确定的一个主要方法就是通过对已知样本的训练和学习来得到。 

感知器算法就是通过训练样本模式的迭代和学习，产生线性（或广义线性）可分的模式判别函数。- 收敛性 

只要模式类别是线性可分的，就可以在有限的迭代步数里求出权向量。
- 背景 

“感知器”一词出自于20世纪50年代中期到60年代中期人们对一种分类学习机模型的称呼，它是属于有关动物和机器学习的仿生学领域中的问题。 

当时的一些研究者认为感知器是一种学习机的强有力模型，后来发现估计过高了，但发展感知器的一些相关概念仍然沿用下来。- 更多信息也可参阅 [Wikipedia传送门](https://www.baidu.com/link?url=Tjop1z2uZdTMqqwvUJzwL-v4N0d8SHMlCfOERvsNITQdMDReUTqdPe5MfM8nVSsCzUpkE0aRXxVlNyOXNKKthK&wd=&eqid=999e25ab0000eb5100000003586b7d79)







