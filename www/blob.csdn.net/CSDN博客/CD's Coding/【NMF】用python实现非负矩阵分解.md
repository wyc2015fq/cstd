# 【NMF】用python实现非负矩阵分解 - CD's Coding - CSDN博客





2016年10月29日 20:31:27[糖果天王](https://me.csdn.net/okcd00)阅读数：4947








## 0x00 前言

论文阅读理解之—— 

《algorithms-for-non-negative-matrix-factorization》 

这是一篇网络数据挖掘专业课中，导师推荐阅读的论文，NMF是非负矩阵分解的意思，这种算法旨在针对现实中的问题（图像像素信息等数据往往不会出现负值），将一个N x M的矩阵分解为N x P和P x M两个矩阵，并尽可能的令乘积与源矩阵相近，顺手走了个PPT。
> 
通常,一个好的变换方法应具备两个基本的特性: 

  (1)可使数据的某种潜在结构变得清晰 

  (2)能使数据的维数得到一定程度的约减 

  ——《非负矩阵分解算法综述 李乐,章毓晋》
![NMF-PPT-01](https://img-blog.csdn.net/20161029160952674)

## 0x01 NMF应用场景

NMF的广泛应用，源于其对事物的局部特性有很好的解释。在众多应用中，NMF能被用于发现数据库中的图像特征，便于快速自动识别应用；能够发现文档的语义相关度，用于信息自动索引和提取；能够在DNA阵列分析中识别基因等等，[比如](http://blog.csdn.net/abcjennifer/article/details/9186523%20%E6%AF%94%E5%A6%82%E8%AF%B4)：
- 图像分析
- 文本聚类/数据挖掘
- 语音处理
- 机器人控制
- 生物医学工程和化学工程

![NMF-PPT-02](https://img-blog.csdn.net/20161029170116925)

## 0x02 NMF-I/O

![NMF-PPT-03](https://img-blog.csdn.net/20161029173513065)
![NMF-PPT-04](https://img-blog.csdn.net/20161029173532959)
![NMF-PPT-05](https://img-blog.csdn.net/20161029173551612)
## 0x03 Cost

![NMF-PPT-06](https://img-blog.csdn.net/20161029184748369)
![NMF-PPT-07](https://img-blog.csdn.net/20161029202429872)
## 0x04 Update Algorithm

![NMF-PPT-08](https://img-blog.csdn.net/20161029202753439)
![NMF-PPT-09](https://img-blog.csdn.net/20161029202908425)
![NMF-PPT-10](https://img-blog.csdn.net/20161029202922157)
![NMF-PPT-11](https://img-blog.csdn.net/20161029203025520)
## 0x05 Code

```python
#!/usr/bin/env python

from numpy import *
from nmf import *

# Define constants
Matrix_ORDER = 5
Matrix_RANK = 2

# Generate a target Matrix
origin_W = random.randint(0,9,size=(Matrix_RANK, Matrix_ORDER))
origin_H = random.randint(0,9,size=(Matrix_ORDER, Matrix_RANK))
v = dot(origin_W, origin_H) 

# Generate initial Matrix W and H (Random generation)
w = random.randint(0,9,size=(Matrix_RANK, Matrix_ORDER))
h = random.randint(0,9,size=(Matrix_ORDER, Matrix_RANK))

# Calculate W&H by multiple iteration
(output_W, output_H) = nmf(v, w, h, 0.001, 50, 100)

# Show Results
print "\n==Original_Matrix=="
print "Matrix_W :\n", origin_W 
print "Matrix_H :\n", origin_H
print "Target_V :\n", v

print "\n===Output_Matrix==="
print "Matrix_W :\n", output_W
print "Matrix_H :\n", output_H
print "Output_V :\n", dot(output_W, output_H)
```

```python
# nmf.py
from numpy import *
from numpy.linalg import norm
from time import time
from sys import stdout

def nmf(V, Winit, Hinit, tol, timelimit, maxiter):
    """
        (W,H) = nmf(V, Winit, Hinit, tol, timelimit, maxiter)
        W,H: output solution
        Winit,Hinit: initial solution
        tol: tolerance for a relative stopping condition
        timelimit, maxiter: limit of time and iterations
    """

    W = Winit; H = Hinit; initt = time();

    gradW = dot(W, dot(H, H.T)) - dot(V, H.T)
    gradH = dot(dot(W.T, W), H) - dot(W.T, V)
    initgrad = norm(r_[gradW, gradH.T])
    # print 'Init gradient norm %f' % initgrad 
    tolW = max(0.001, tol) * initgrad
    tolH = tolW

    for iter in xrange(1,maxiter):
        # stopping condition
        projnorm = norm(r_[gradW[logical_or(gradW<0, W>0)], 
                           gradH[logical_or(gradH<0, H>0)]])
        if projnorm < tol*initgrad or time() - initt > timelimit: break

        (W, gradW, iterW) = nlssubprob(V.T, H.T, W.T, tolW, 1000)
        W, gradW = W.T, gradW.T

        if iterW==1: tolW = 0.1 * tolW

        (H,gradH,iterH) = nlssubprob(V, W, H, tolH, 1000)
        if iterH==1: tolH = 0.1 * tolH

        if iter % 10 == 0: stdout.write('.')

    print '\nIter = %d Final proj-grad norm %f' % (iter, projnorm)
    return (W, H)

def nlssubprob(V, W, Hinit, tol, maxiter):
    """
        H, grad: output solution and gradient
        iter: #iterations used
        V, W: constant matrices
        Hinit: initial solution
        tol: stopping tolerance
        maxiter: limit of iterations
    """

    H = Hinit
    WtV = dot(W.T, V)
    WtW = dot(W.T, W) 

    alpha = 1; beta = 0.1;
    for iter in xrange(1, maxiter):  
        grad = dot(WtW, H) - WtV
        projgrad = norm(grad[logical_or(grad < 0, H >0)])
        if projgrad < tol: break

        # search step size 
        for inner_iter in xrange(1,20):
            Hn = H - alpha*grad
            Hn = where(Hn > 0, Hn, 0)
            d = Hn-H
            gradd = sum(grad * d)
            dQd = sum(dot(WtW,d) * d)
            suff_decr = 0.99*gradd + 0.5*dQd < 0;
            if inner_iter == 1:
                decr_alpha = not suff_decr; Hp = H;
            if decr_alpha: 
                if suff_decr:
                    H = Hn; break;
                else:
                    alpha = alpha * beta;
            else:
                if not suff_decr or (Hp == Hn).all():
                    H = Hp; break;
                else:
                    alpha = alpha/beta; Hp = Hn;

        if iter == maxiter:
            print 'Max iter in nlssubprob'
    return (H, grad, iter)
```









