# 密度聚类DBSCAN - 浅梦的博客 - CSDN博客





2017年09月25日 16:46:28[浅梦s](https://me.csdn.net/u012151283)阅读数：532








# 密度聚类

基于密度的聚类，假设聚类结构能够通过**样本分布的紧密程度**确定。通常情况下，密度聚类算法从样本密度的角度来考察样本之间的可连接性。

## 基于密度聚类的特性
- 发现任意形状的聚类
- 处理噪声
- 一遍扫描（只检查局部区域来判断密度）
- 需要密度参数作为终止条件

## 一些研究
- DBSCAN (KDD’96)
- OPTICS (sigmod’99)
- DENCLUE (kdd’98)
- CLIQUE (SIGMOD’98)也是基于网格的

# DBSCAN

全称Density-Based Spatial Clustering Appliacations with Noise 

DBSCAN，它基于一组”领域”参数$(\epsilon,MinPts)$来刻画样本分布的紧密程度。

## 几个重要概念
- e-邻域 

对$x_j\in D$,其$\epsilon$-邻域包含样本集D种与$x_j$的距离不大于$\epsilon$的样本，即$N_\epsilon(x_j)={[x_j\in D|dist(x_i,x_j)\le\epsilon]}$
- 核心对象(core object) 

若$x_j$的$\epsilon$-邻域至少包含$Minpts$个样本，即$|N_\epsilon(x_j)\ge MinPts$，则$x_j$是一个核心对象
- 密度直达(directly density-raechable) 

若p位于q的$\epsilon$邻域，且q是核心对象，则p由q密度直达。
- 密度可达(density-reachable) 

对于p和q，若存在样本序列$p_1,p_2,...,p_n$其中$p_1=q,p_n=p$且$p_{i+1}$由$p_i$密度直达。则称p由q密度可达。
- 密度相连(density-connected) 

若p和q均由o密度可达到，则p和q密度相连接

密度直达通常不满足对称性。 

密度可达关系满足直递性，不满足对称性。 

密度相连关系满足对称性。
## DBSCAN簇定义

DBSCAN将簇定义为： 

由密度可达关系导出的最大的密度相连的样本集合。 

形式化定义，给定邻域参数$(\epsilon,MinPts)$，簇$C\subseteq D$是满足以下性质的非空样本子集：- 连接性(connectivity):$x_i\in C,x_j\in C \rightarrow x_i与x_j密度相连$
- 最大性(maximality):$x_i\in C,x_j由x_i密度可达\rightarrow x_j\in C$

D种不属于任何簇的样本被认为是噪声或异常样本

## 如何找簇

若x是核心对象，由x密度可达的所有样本组成的集合为满足连接性和最大性的簇

## 算法描述

![这里写图片描述](https://img-blog.csdn.net/20170925164401743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 
参考资料 

  《机器学习》周志华 p.211 

  Cluster Analysis in Data Mining-Coursera






