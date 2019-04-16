# 用Python做投资-关于term spread - lyx的专栏 - CSDN博客





2016年10月13日 20:44:28[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2077








利率这个东西，说简单也简单，说复杂也复杂。所以玩债券的才是高手，门槛很高，玩股票的都是散户。

我国市场有Shibor这个上海银行间同业拆借利率。有一种说法，就是可以用term spread来判断经济的情况。什么事term spread呢？首先说一下term structure。term structure就是现在开始，不同时间长度借钱你要求的利率，换句话说，就是你的流动性偏好。可想而知，一笔钱，借出去一个月的年化利率和借出去十年的年化利率固然是不同的，而且在正常情况下，后者会显著高于前者，因为时间越长，不确定性越高。如果后者利率低，那么作为一个理性人，你完全可以去市场上借长期的钱，付较低的长期的利息，然后借出短期的钱，获得较高的利息，完成一次完美的无风险套利。

那么究竟什么时候会有长端利率会低于近端利率呢？

答案就是经济危机的时候。这个时候往往企业生产艰难，谁都不看好后面的情况，都想暂时借钱度过难关，于是在term structure上就显示出来，近端利率甚至会高于远端利率。好了，我们上程序。



```python
# -*- coding: utf-8 -*-
"""
Created on Thu Oct 13 20:02:31 2016

@author: Luyixiao
"""

import numpy as np
import pandas as pd
from scipy import *
from data import *
import matplotlib.pyplot as plt
import tushare as ts
df = pd.DataFrame()
for i in range(6,17):#获取每一年的shibor数据
    year = str(2000+i)
    df = df.append(ts.shibor_data(year) )#取当前年份的数据
use = df[['date','3M','1Y']]#挑选出时间、三个月的近端shibor，一年的远端shibor三个列的数据
use['spread'] = use['1Y']-use['3M']#计算term spread
term_spread = use['spread'].tolist()#将term spread转换为list，方便绘图
x_year = use['date'].tolist()#同样的，把年份转成list结构，方便绘图
#x_ax = range(0,len(term_spread))
fig = plt.figure(figsize=(50,5))#显然，我们的时间跨度很长，所以我们把图片的长度设置的长一些
plt.plot(x_year, term_spread)#绘制曲线
plt.show()
```

最后，跑出来的图是这样的：
![](https://img-blog.csdn.net/20161013204238098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


大家可以去翻阅一下经济大事记，看看中间这几段term spread下降的很夸张的时候发生了什么。目前的term spread还是很正常的。



