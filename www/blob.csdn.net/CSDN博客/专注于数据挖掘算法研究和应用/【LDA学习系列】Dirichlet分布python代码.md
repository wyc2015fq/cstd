# 【LDA学习系列】Dirichlet分布python代码 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年05月17日 11:02:18[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：567








![](https://img-blog.csdn.net/20180517110146678)

代码：

```java
# -*- coding: utf-8 -*-
'''
Created on 2018年5月15日
@author: user
@attention:  dirichret distribution
'''
from scipy.stats import dirichlet
import matplotlib.pyplot as plt
import numpy as np

TOL = 1.48e-8
  
def test_dirichret_distribution():
    
    print (dirichlet.pdf([0.6,0.3,0.1],[3,2,1]))
    print (dirichlet.logpdf([0.6,0.3,0.1],[1,2,3]))
    
if __name__ == '__main__': 
    #scipy.stats库
    test_dirichret_distribution()#beta分布
    
    #pip install dirichlet 
    #https://github.com/ericsuh/dirichlet
```

结果：

```
6.479999999999995
-1.7147984280919264
```




