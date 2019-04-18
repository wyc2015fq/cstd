# 【Python-ML】SKlearn库层次聚类凝聚AgglomerativeClustering模型 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月25日 15:09:17[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5212
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月25日
@author: Jason.F
@summary: 无监督聚类学习-层次聚类(hierarchical clustering)，自下向上的凝聚和自顶向下的分裂两种方法。
'''
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial.distance import pdist,squareform
from scipy.cluster.hierarchy import linkage
from scipy.cluster.hierarchy import dendrogram
from sklearn.cluster import AgglomerativeClustering
np.random.seed(123)
variables = ['X','Y','Z']
labels=['ID_0','ID_1','ID_2','ID_3','ID_4']
X=np.random.random_sample([5,3])*10
#层次聚类树
df = pd.DataFrame(X,columns=variables,index=labels)
print (df)
#计算距离关联矩阵，两两样本间的欧式距离
#row_dist = pd.DataFrame(squareform(pdist(df,metric='euclidean')),columns=labels,index=labels)
#print (row_dist)
#print (help(linkage))
row_clusters = linkage(pdist(df,metric='euclidean'),method='complete')#使用抽秘籍距离矩阵
#row_clusters = linkage(df.values,method='complete',metric='euclidean')
print (pd.DataFrame(row_clusters,columns=['row label1','row label2','distance','no. of items in clust.'],index=['cluster %d'%(i+1) for i in range(row_clusters.shape[0])]))
#层次聚类树
row_dendr = dendrogram(row_clusters,labels=labels)
plt.tight_layout()
plt.ylabel('Euclidean distance')
plt.show()
#层次聚类热度图
fig =plt.figure(figsize=(8,8))
axd =fig.add_axes([0.09,0.1,0.2,0.6])
row_dendr = dendrogram(row_clusters,orientation='right')
df_rowclust = df.ix[row_dendr['leaves'][::-1]]
axm = fig.add_axes([0.23,0.1,0.6,0.6])
cax = axm.matshow(df_rowclust,interpolation='nearest',cmap='hot_r')
axd.set_xticks([])
axd.set_yticks([])
for i in axd.spines.values():
    i.set_visible(False)
fig.colorbar(cax)
axm.set_xticklabels(['']+list(df_rowclust.columns))
axm.set_yticklabels(['']+list(df_rowclust.index))
plt.show()

#凝聚层次聚类，应用对层次聚类树剪枝
ac=AgglomerativeClustering(n_clusters=2,affinity='euclidean',linkage='complete')
labels = ac.fit_predict(X)
print ('cluster labels:%s'%labels)
```

结果：



```
X         Y         Z
ID_0  6.964692  2.861393  2.268515
ID_1  5.513148  7.194690  4.231065
ID_2  9.807642  6.848297  4.809319
ID_3  3.921175  3.431780  7.290497
ID_4  4.385722  0.596779  3.980443
           row label1  row label2  distance  no. of items in clust.
cluster 1         0.0         4.0  3.835396                     2.0
cluster 2         1.0         2.0  4.347073                     2.0
cluster 3         3.0         5.0  5.899885                     3.0
cluster 4         6.0         7.0  8.316594                     5.0
cluster labels:[0 1 1 0 0]
```
![](https://img-blog.csdn.net/20180125150857827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180125150907940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





