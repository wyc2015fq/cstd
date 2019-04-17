# 聚类︱python实现 六大 分群质量评估指标（兰德系数、互信息、轮廓系数） - 素质云笔记/Recorder... - CSDN博客





2017年04月24日 11:54:17[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：23607








之前关于聚类题材的博客有以下两篇： 

1、 [笔记︱多种常见聚类模型以及分群质量评估（聚类注意事项、使用技巧）](http://blog.csdn.net/sinat_26917383/article/details/51611519)

2、[k-means+python︱scikit-learn中的KMeans聚类实现](http://blog.csdn.net/sinat_26917383/article/details/70240628)

.
# 1 R语言中的分群质量——轮廓系数

因为先前惯用R语言，那么来看看R语言中的分群质量评估，节选自[笔记︱多种常见聚类模型以及分群质量评估（聚类注意事项、使用技巧）](http://blog.csdn.net/sinat_26917383/article/details/51611519)：

没有固定标准，一般会3-10分群。或者用一些指标评价，然后交叉验证不同群的分群指标。 

一般的指标：轮廓系数silhouette（-1,1之间，值越大，聚类效果越好）（fpc包），兰德指数rand；R语言中有一个包用30种方法来评价不同类的方法（NbClust），但是速度较慢 

商业上的指标：分群结果的覆盖率；分群结果的稳定性；分群结果是否从商业上易于理解和执行 

轮廓系数旨在将某个对象与自己的簇的相似程度和与其他簇的相似程度进行比较。轮廓系数最高的簇的数量表示簇的数量的最佳选择。 
![这里写图片描述](https://img-blog.csdn.net/20170326144153209)

一般来说，平均轮廓系数越高，聚类的质量也相对较好。在这，对于研究区域的网格单元，最优聚类数应该是2，这时平均轮廓系数的值最高。但是，聚类结果（k=2）的 SSE 值太大了。当 k=6 时，SEE 的值会低很多，但此时平均轮廓系数的值非常高，仅仅比 k=2 时的值低一点。因此，k=6 是最佳的选择。 

.
# 2 python中的分群质量

主要参考来自官方文档：[Clustering](http://scikit-learn.org/stable/modules/clustering.html#clustering-performance-evaluation)

部分内容来源于：[机器学习评价指标大汇总](http://www.zhaokv.com/2016/03/ml-metric.html)

个人比较偏好的三个指标有：Calinski-Harabaz Index（未知真实index的模型评估）、Homogeneity, completeness and V-measure（聚类数量情况）、轮廓系数

## 1.1 Adjusted Rand index 调整兰德系数

![这里写图片描述](https://img-blog.csdn.net/20170424115905680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```python
>>> from sklearn import metrics
>>> labels_true = [0, 0, 0, 1, 1, 1]
>>> labels_pred = [0, 0, 1, 1, 2, 2]

>>> metrics.adjusted_rand_score(labels_true, labels_pred)  
0.24
```

.

## 1.2 Mutual Information based scores 互信息

![这里写图片描述](https://img-blog.csdn.net/20170424115959648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Two different normalized versions of this measure are available, Normalized Mutual Information(NMI) and Adjusted Mutual Information(AMI). NMI is often used in the literature while AMI was proposed more recently and is normalized against chance:
```python
>>> from sklearn import metrics
>>> labels_true = [0, 0, 0, 1, 1, 1]
>>> labels_pred = [0, 0, 1, 1, 2, 2]

>>> metrics.adjusted_mutual_info_score(labels_true, labels_pred)  
0.22504
```

.

## 1.3 Homogeneity, completeness and V-measure

同质性homogeneity：每个群集只包含单个类的成员。 

完整性completeness：给定类的所有成员都分配给同一个群集。

```
>>> from sklearn import metrics
>>> labels_true = [0, 0, 0, 1, 1, 1]
>>> labels_pred = [0, 0, 1, 1, 2, 2]

>>> metrics.homogeneity_score(labels_true, labels_pred)  
0.66...

>>> metrics.completeness_score(labels_true, labels_pred) 
0.42...
```

两者的调和平均V-measure：

```
>>> metrics.v_measure_score(labels_true, labels_pred)    
0.51...
```

.

## 1.4 Fowlkes-Mallows scores

The Fowlkes-Mallows score FMI is defined as the geometric mean of the pairwise precision and recall: 
![这里写图片描述](https://img-blog.csdn.net/20170424113439787?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```
>>> from sklearn import metrics
>>> labels_true = [0, 0, 0, 1, 1, 1]
>>> labels_pred = [0, 0, 1, 1, 2, 2]
>>>
>>> metrics.fowlkes_mallows_score(labels_true, labels_pred)  
0.47140...
```

.

## 1.5 Silhouette Coefficient 轮廓系数

![这里写图片描述](https://img-blog.csdn.net/20170424120012179?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
>>> import numpy as np
>>> from sklearn.cluster import KMeans
>>> kmeans_model = KMeans(n_clusters=3, random_state=1).fit(X)
>>> labels = kmeans_model.labels_
>>> metrics.silhouette_score(X, labels, metric='euclidean')
...                                                      
0.55...
```

.

## 1.6 Calinski-Harabaz Index

这个计算简单直接，得到的Calinski-Harabasz分数值ss越大则聚类效果越好。Calinski-Harabasz分数值ss的数学计算公式是（理论介绍来源于：[用scikit-learn学习K-Means聚类](http://www.cnblogs.com/pinard/p/6169370.html)）：

![这里写图片描述](https://img-blog.csdn.net/20170424152526470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

　也就是说，类别内部数据的协方差越小越好，类别之间的协方差越大越好，这样的Calinski-Harabasz分数会高。 

　在scikit-learn中， Calinski-Harabasz Index对应的方法是metrics.calinski_harabaz_score. 

在真实的分群label不知道的情况下，可以作为评估模型的一个指标。 

同时，数值越小可以理解为：组间协方差很小，组与组之间界限不明显。 

与轮廓系数的对比，笔者觉得最大的优势：快！相差几百倍！毫秒级
```
>>> import numpy as np
>>> from sklearn.cluster import KMeans
>>> kmeans_model = KMeans(n_clusters=3, random_state=1).fit(X)
>>> labels = kmeans_model.labels_
>>> metrics.calinski_harabaz_score(X, labels)  
560.39...
```


**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)









