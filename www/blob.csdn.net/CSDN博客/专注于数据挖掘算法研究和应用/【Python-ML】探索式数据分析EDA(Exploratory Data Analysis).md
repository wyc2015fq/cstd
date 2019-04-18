# 【Python-ML】探索式数据分析EDA(Exploratory Data Analysis) - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月24日 16:00:13[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1073
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月24日
@author: Jason.F
@summary: 有监督回归学习-探索式数据分析(EDA，Exploratory Data Analysis)，发现数据的异常和分布情况以及特征间的相互关系
'''
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

#导入波士顿房屋数据集
df=pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/housing/housing.data',header=None,sep='\s+')
df.columns=['CRIM','ZM','INDUS','CHAS','NOX','RM','AGE','DIS','RAD','TAX','PTRATIO','B','LSTAT','MEDV']
#散点图矩阵，可视化不同特征间的两两关系，发现特征间关系以及异常点
sns.set(style='whitegrid',context='notebook')
#sns.reset_orig()#重设matplotlib风格
cols=['LSTAT','INDUS','NOX','RM','MEDV']
sns.pairplot(df[cols],size=2.5)
plt.show()
#皮尔逊积矩相关系数pearson product-moment correlation coefficient,衡量两两特征间的线性依赖关系
#标准化各特征间的协方差
cm = np.corrcoef(df[cols].values.T)
sns.set(font_scale=1.5)
hm= sns.heatmap(cm,cbar=True,annot=True,square=True,fmt='.2f',annot_kws={'size':15},yticklabels=cols,xticklabels=cols)
plt.show()
```

结果：

![](https://img-blog.csdn.net/20180124155952488?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180124160003660?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




