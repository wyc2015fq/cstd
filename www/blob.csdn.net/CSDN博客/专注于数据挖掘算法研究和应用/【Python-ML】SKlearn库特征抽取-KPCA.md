# 【Python-ML】SKlearn库特征抽取-KPCA - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月18日 16:04:51[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：702
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月18日
@author: Jason.F
@summary: 特征抽取-KPCA方法
'''
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.decomposition import KernelPCA
from sklearn.datasets import make_moons

X,y=make_moons(n_samples=100,random_state=123)#生成半月形数据
scikit_kpca=KernelPCA(n_components=2,kernel='rbf',gamma=15)
X_skernpca=scikit_kpca.fit_transform(X)#映射
#可视化
plt.scatter(X_skernpca[y==0,0],X_skernpca[y==0,1],color='red',marker='^',alpha=0.5)
plt.scatter(X_skernpca[y==1,0],X_skernpca[y==1,1],color='blue',marker='o',alpha=0.5)
plt.xlabel('PC1')
plt.ylabel('PC2')
plt.show()

'''
scikit-learn实现的高级非线性降维技术，参考：
http://scikit-learn.org/stable/modules/manifold.html
'''
```

结果：

![](https://img-blog.csdn.net/20180118160441018?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




