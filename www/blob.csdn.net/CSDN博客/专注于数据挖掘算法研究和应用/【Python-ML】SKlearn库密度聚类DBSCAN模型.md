# 【Python-ML】SKlearn库密度聚类DBSCAN模型 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月25日 16:00:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：506
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月25日
@author: Jason.F
@summary: 无监督聚类学习-基于密度 空间的聚类算法(Density-based Spatial Clustering of Application with Noise,DBSCAN)
密度：半径范围内样本点的数量
核心点：如果在一个点周边的指定半径内，其他样本点的数量不小于指定数量(MinPts)，则该样本点为核心点
边界点：在指定半径内，如果一个点的邻居点少于MinPts个，但却包含一个核心点，则为边界点
噪声点：除核心点和边界点外的样本点都是噪声点
DBSCAN的簇空间不一定是球状的，可识别和移除噪声点，因此不一定会将所有的样本点都划分到某一簇中
如果数据集中的密度差异相对较大，则找到半径和MinPts较为困难。
'''
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from sklearn.datasets import make_moons
from sklearn.cluster import KMeans,AgglomerativeClustering,DBSCAN
X,y = make_moons(n_samples=200,noise=0.05,random_state=0)#创建半月形数据
plt.scatter(X[:,0],X[:,1])
plt.show()
f,(ax1,ax2) = plt.subplots(1,2,figsize=(8,3))
#原型聚类KMeans模型
km=KMeans(n_clusters=2,random_state=0)
y_km = km.fit_predict(X)
ax1.scatter(X[y_km==0,0],X[y_km==0,1],c='lightblue',marker='o',s=40,label='cluster 1')
ax1.scatter(X[y_km==1,0],X[y_km==1,1],c='red',marker='s',s=40,label='cluster 2')
ax1.set_title('K-means clustering')
#层次聚类凝聚模型
ac=AgglomerativeClustering(n_clusters=2,affinity='euclidean',linkage='complete')#全连接，欧式距离计算联合矩阵
y_ac= ac.fit_predict(X)
ax2.scatter(X[y_ac==0,0],X[y_ac==0,1],c='lightblue',marker='o',s=40,label='cluster 1')
ax2.scatter(X[y_ac==1,0],X[y_ac==1,1],c='red',marker='s',s=40,label='cluster 2')
ax2.set_title('Agglomerative clustering')
plt.legend()
plt.show()
#密度聚类DBSCAN，成功对半月形数据进行分类
db =DBSCAN(eps=0.2,min_samples=5,metric='euclidean')#欧式距离，样本点数量5，半径0.2
y_db = db.fit_predict(X)
plt.scatter(X[y_db==0,0],X[y_db==0,1],c='lightblue',marker='o',s=40,label='cluster 1')
plt.scatter(X[y_db==1,0],X[y_db==1,1],c='red',marker='s',s=40,label='cluster 2')
plt.legend()
plt.show()

'''
1、三种最基本的聚类方法：KMeans基于原型的聚类、凝聚层次聚类、使用DBSCAN基于密度的聚类。
2、其他方法：图聚类(最突出方法使谱聚类算法)，使用基于相似矩阵的特征向量来获得簇间关系，
      参考：http://arxiv.org/pdf/0711.0189v1.pdf   
     <A Tutorial on Spectral Clustering.Statistics and computing>
3、聚类算法取决于三个要素，算法、参数以及距离度量，而距离度量很容易面临维度灾难(curse of dimensionality)
'''
```

结果：

![](https://img-blog.csdn.net/20180125155958933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180125160006990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180125160014464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




