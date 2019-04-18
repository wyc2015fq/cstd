# 【Python-ML】SKlearn库谱聚类SpectralClustering模型 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月29日 17:54:05[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2963








      在scikit-learn的类库中，sklearn.cluster.SpectralClustering实现了基于Ncut的谱聚类，没有实现基于RatioCut的切图聚类。同时，对于相似矩阵的建立，也只是实现了基于K邻近法和全连接法的方式，没有基于-邻近法的相似矩阵。最后一步的聚类方法则提供了两种，K-Means算法和
 discretize算法。

       SpectralClustering的参数解释如下：

![](https://img-blog.csdn.net/20180129175055545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20180129175102846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

代码示例：



```
import time
from sklearn.cluster import SpectralClustering
from sklearn import metrics

if __name__ == "__main__":    
    start = time.clock() 
    #X是训练集
    
    for index, gamma in enumerate((0.01,0.1,1)):
        for index, k in enumerate((3,4,5,6)):
            y_pred = SpectralClustering(n_clusters=k, gamma=gamma).fit_predict(X)
            print "Calinski-Harabasz Score with gamma=", gamma, "n_clusters=", k,"score:", metrics.calinski_harabaz_score(X, y_pred) 
                          
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```





