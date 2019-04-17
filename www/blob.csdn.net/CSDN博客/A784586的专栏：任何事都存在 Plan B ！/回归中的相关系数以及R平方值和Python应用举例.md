# 回归中的相关系数以及R平方值和Python应用举例 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年05月09日 17:23:19[QuJack](https://me.csdn.net/A784586)阅读数：6639








**回归中的相关系数以及R平方值和Python应用举例**



1. 皮尔逊相关系数 (Pearson Correlation Coefficient):

1.1 衡量两个值线性相关强度的量

1.2 取值范围 [-1, 1]: 

正向相关: >0, 负向相关：<0, 无相关性：=0

![](https://img-blog.csdn.net/20170509171931646?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20170509171935937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2. R平方值:


2.1定义：决定系数，反应因变量的全部变异能通过回归关系被自变量解释的比例。


2.2 描述：如R平方为0.8，则表示回归关系可以解释因变量80%的变异。换句话说，如果我们能控制自变量不变，则因变量的变异程度会减少80%


2.3： 简单线性回归：R^2 = r * r

多元线性回归：
![](https://img-blog.csdn.net/20170509172139581?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170509172144490?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





Python实现;



```python
import numpy as np
from astropy.units import Ybarn
import math

def computeCorrelation(X, Y):
    xBar = np.mean(X)
    yBar = np.mean(Y)
    SSR = 0
    varX = 0
    varY = 0
    for i in range(0 , len(X)):
        diffXXBar = X[i] - xBar
        diffYYBar = Y[i] - yBar
        SSR += (diffXXBar * diffYYBar)
        varX +=  diffXXBar**2
        varY += diffYYBar**2
    
    SST = math.sqrt(varX * varY)
    return SSR / SST

testX = [1, 3, 8, 7, 9]
testY = [10, 12, 24, 21, 34]

print (computeCorrelation(testX, testY))
```





