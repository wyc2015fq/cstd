# 【LDA学习系列】M-H采样python代码 - 专注于数据挖掘算法研究和应用 - CSDN博客
2018年05月19日 09:18:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：481
LDA说的比较利索参考：https://segmentfault.com/a/1190000012215533
![](https://img-blog.csdn.net/2018051909145981)
```python
# -*- coding: utf-8 -*-
'''
Created on 2018年5月16日
@author: user
p:输入的概率分布，离散情况采用元素为概率值的数组表示
N:认为迭代N次马尔可夫链收敛
Nlmax:马尔可夫链收敛后又取的服从p分布的样本数
isMH:是否采用MH算法，默认为True
'''
from __future__ import division
import matplotlib.pyplot as plt
import numpy as np
from array import array
def mcmc(p,N=10000,Nlmax=10000,isMH=True):
    
    A = np.array([p for y in range(len(p))], dtype=np.float64) #第一步：构造转移概率矩阵
    X0 = np.random.randint(len(p))
    count = 0
    samplecount = 0
    L = array("d",[X0])
    l = array("d")
    
    while True:
        X = int(L[samplecount])#第二步：初始化x0
        cur = np.argmax(np.random.multinomial(1,A[X]))#第三步：采样候选样本
        count += 1
        if isMH:
            a = (p[cur]*A[cur][X])/(p[X]*A[X][cur])#第四步：计算是否满足马氏平稳条件
            alpha = min(a,1)
        else:
            alpha = p[cur]*A[cur][X]
        u = np.random.uniform(0,1) #第五步：生成阈值
        if u<alpha:#第六步：是否接受样本
            samplecount += 1
            L.append(cur)
            if count>N:
                l.append(cur)
        if len(l)>=Nlmax:
            break
        else:
            continue
    La = np.frombuffer(L)
    la = np.frombuffer(l)
    return La,la
def count(q,n):
    L = array("d")
    l1 = array("d")
    l2 = array("d")
    for e in q:
        L.append(e)
    for e in range(n):
        l1.append(L.count(e))
    for e in l1:
        l2.append(e/sum(l1))
    return l1,l2
if __name__ == '__main__':    
    p = np.array([0.6,0.3,0.1]) 
    a = mcmc(p)[1]
    l1 = ['state%d'% x for x in range(len(p))]
    plt.pie(count(a,len(p))[0],labels=l1,labeldistance=0.3,autopct='%1.2f%%')
    plt.title("sampling")
    plt.show()
```
