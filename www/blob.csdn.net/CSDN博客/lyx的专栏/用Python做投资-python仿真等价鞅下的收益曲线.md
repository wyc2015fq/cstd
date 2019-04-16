# 用Python做投资-python仿真等价鞅下的收益曲线 - lyx的专栏 - CSDN博客





2016年10月24日 18:21:08[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1681








有一个赌局，抛一枚硬币，正面朝上赢，反面朝上输。然后每次自由下注。

如果我们按照这样的次序下注：1,2,4,8,16，......,2^n.只要有一次获胜，那么我们就从头再来。这里我们可以看出，每次获胜都可以赢得1元钱。因为2^n次方的数列前n-1次项和为2^n-1。这里我们就能看出，只要你有足够多的钱，那么你总能赚钱。这一游戏，就叫做等价鞅。



```python
# -*- coding: utf-8 -*-
"""
Created on Mon Oct 24 16:05:03 2016

@author: Luyixiao
"""

import numpy as np  
import matplotlib.pyplot as plt  
import pandas as pd
# true = win;false = loss
def winOrLossGenerator(p,size):
    randomNumber = np.random.uniform(0,1,size)
    WL = randomNumber < p
    return WL

def player(WL_list):
    amount = 100#1$ as initial money 
    moneyList=[]#use this list to record the lost of the money amount
    gain = 0
    margin = 100   
    lossNumber = 0
    for i in range(0,len(WL_list)):
        #print WL_list[i]
        if WL_list[i] == True:
            lossNumber = 0
            gain = gain + 1
            margin = amount + gain
            
        if WL_list[i] == False:
            margin = margin - 2**lossNumber
            lossNumber = lossNumber + 1
        moneyList.append(margin)
    print moneyList
    fig = plt.figure(figsize=(10,20))
    plt.title("win probability is 0.5")
    plt.plot(range(0,len(moneyList)),moneyList)
            
                   
WL_list = winOrLossGenerator(0.5,100)
#print WL_list
player(WL_list)
```
上面的代码用蒙特拉罗的思想模拟了这一游戏，
```python
winOrLossGenerator
```
函数用于产生size个输或者赢的序列。





```python
player(WL_list)
```
函数则用于模拟赌博的人。每次运行的结果都是不一样的，我们取一次观察一下资金的变化情况。


![](https://img-blog.csdn.net/20161024181015438?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们可以看到，这次仿真中，最大的资金回测大概在72元左右。我们修改一下获胜的概率，假设我们的硬币是不均匀的，而赌场中往往是这呀。

如果我们的获胜概率只有2，那么资金曲线是这样的：

![](https://img-blog.csdn.net/20161024181552112?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


获胜率为0.4，情况还马马虎虎

![](https://img-blog.csdn.net/20161024181558237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


获胜的概率为0.6：

![](https://img-blog.csdn.net/20161024181603815?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


获胜的概率为0.9的时候，资金曲线就比较平稳的向上了：

![](https://img-blog.csdn.net/20161024181614710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)













