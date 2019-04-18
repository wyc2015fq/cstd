# 【Python-ML】聚类的性能评价指标 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月29日 17:05:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1867








参考：http://scikit-learn.org/stable/modules/clustering.html#clustering-performance-evaluation


1、兰德指数



```python
from sklearn import metrics
labels_true = [0, 0, 0, 1, 1, 1]
labels_pred = [0, 0, 1, 1, 2, 2]

print (metrics.adjusted_rand_score(labels_true, labels_pred))
```

![](https://img-blog.csdn.net/20180129165134352?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2、互信息



```python
from sklearn import metrics
labels_true = [0, 0, 0, 1, 1, 1]
labels_pred = [0, 0, 1, 1, 2, 2]
print (metrics.adjusted_mutual_info_score(labels_true, labels_pred) )
```

![](https://img-blog.csdn.net/20180129165555238?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3、Homogeneity, completeness and V-measure


同质性homogeneity：每个群集只包含单个类的成员。

完整性completeness：给定类的所有成员都分配给同一个群集。

两者的调和平均V-measure。



```python
from sklearn import metrics
labels_true = [0, 0, 0, 1, 1, 1]
labels_pred = [0, 0, 1, 1, 2, 2]
print (metrics.homogeneity_score(labels_true, labels_pred))
print (metrics.completeness_score(labels_true, labels_pred))
print (metrics.v_measure_score(labels_true, labels_pred))
```


4、Fowlkes-Mallows scores

![](https://img-blog.csdn.net/20180129165941925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



```python
from sklearn import metrics
labels_true = [0, 0, 0, 1, 1, 1]
labels_pred = [0, 0, 1, 1, 2, 2]
print (metrics.fowlkes_mallows_score(labels_true, labels_pred))
```


5、Silhouette Coefficient 轮廓系数

参考：

http://blog.csdn.net/fjssharpsword/article/details/79161570![](https://img-blog.csdn.net/20180129170132961?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

6、Calinski-Harabaz Index

类别内部数据的协方差越小越好，类别之间的协方差越大越好，这样的Calinski-Harabasz分数会高。 
![](https://img-blog.csdn.net/20180129170418371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20180129170511818?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







