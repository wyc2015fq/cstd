# 用Python做投资-小试牛刀 - lyx的专栏 - CSDN博客





2016年10月11日 21:30:14[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2643








        统计、机器学习这种东西，用来做别的有点不好玩，但是用来玩股票真的可以吗？

       市场的复杂性不是统计可以刻画的，所以，量化投资做的再好，这也仅仅是一种辅助罢了，至少当前的自己是这么理解数学在金融市场中的地位的。这一地位，不是低，而是很高，很科学，不是拍脑袋做决策，而是有数据驱动的依据。

        希望以后这个系列的文章能够更新下去吧，不一定是股票，只要和投资有关，和程序有关，好玩有趣，都可以和大家分享。

        先做一个简单的每日收益率的折线图吧，我们考察的股票是BYD。



```python
# -*- coding: utf-8 -*-
"""
Created on Tue Oct 11 20:47:10 2016

@author: Luyixiao
"""

import numpy as np
from scipy import *
from data import *
import matplotlib.pyplot as plt
import tushare as ts
byd = ts.get_hist_data('002594') #一次性获取全部日k线数据
#byd5 = ts.get_hist_data('002594',ktype='15') #一次性获取全部日k线数据

p_change = (byd['p_change'])#从pandas中获取每日价格变化百分比，换句话说，就是日收益率
p_change = p_change.tolist()#把pandas切片下来的数据转换成List
for i in range(0,len(p_change)/2):#由于Tushare的数据是近期数据在list的开始，所以我们做个逆转
    temp = p_change[i]
    p_change[i] = p_change[len(p_change)-1-i]
    p_change[len(p_change)-1-i] = temp
date = byd.index#获取对应的日期
ind=date.tolist()
for i in range(0,len(ind)/2):
    temp = ind[i]
    ind[i] = ind[len(ind)-1-i]
    ind[len(ind)-1-i] = temp
x_ax=range(0,724)
plt.plot(x_ax, p_change)#绘制曲线
plt.show()
```
![](https://img-blog.csdn.net/20161011212829643?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




        最后就是这样的一个图片，我们看到400天左右的位置，老是涨停跌停，我们可以发现这就是15年股灾的时候的数据。



