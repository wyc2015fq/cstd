# 十一、用scikit-learn做聚类分析 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 10:23:18[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：1205









![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_136_default_big.jpeg)



线性回归和逻辑回归都是监督学习方法，聚类分析是非监督学习的一种，可以从一批数据集中探索信息，比如在社交网络数据中可以识别社区，在一堆菜谱中识别出菜系。本节介绍K-means聚类算法

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## K-means

k是一个超参数，表示要聚类成多少类。K-means计算方法是重复移动类的重心，以实现成本函数最小化，成本函数为：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/2224c88869e0627a6f76ee18d107c2d740029c10.png)



其中μk是第k类的重心位置



## 试验

构造一些样本用户试验，如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

import matplotlib.pyplot as plt
import numpy as np

# 生成2*10的矩阵，且值均匀分布的随机数
cluster1 = np.random.uniform(0.5, 1.5, (2, 10))
cluster2 = np.random.uniform(3.5, 4.5, (2, 10))

# 顺序连接两个矩阵，形成一个新矩阵,所以生成了一个2*20的矩阵，T做转置后变成20*2的矩阵,刚好是一堆(x,y)的坐标点
X = np.hstack((cluster1, cluster2)).T

plt.figure()
plt.axis([0, 5, 0, 5])
plt.grid(True)
plt.plot(X[:,0],X[:,1],'k.')
plt.show()
```

图像如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/c82215e79118bb31db5763bfb7712e530950bcee.png)

通过k-means做聚类，输出重心点，增加如下代码：

```python
from sklearn.cluster import KMeans
kmeans = KMeans(n_clusters=2)
kmeans.fit(X)
plt.plot(kmeans.cluster_centers_[:,0], kmeans.cluster_centers_[:,1], 'ro')
```



输出图像：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/a5dfa8fb5bbeef962c0ea86886caa43a8498bfde.png)

可以看到找到了两个重心点

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 肘部法则

现实情况是多个点并不像上面这么聚类清晰，很可能是这样的：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/b01ef63a4b30ac45f7be44c04913ace4b30b7a76.png)



你说不清它应该聚类成2、3、4个点，因此我们需要通过分别计算k=(2,3,4)的聚类结果，并比较他们的成本函数值，随着k的增大，成本函数值会不断降低，只有快速降低的那个k值才是最合适的k值，如下：



```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

import matplotlib.pyplot as plt
import numpy as np
from sklearn.cluster import KMeans
from scipy.spatial.distance import cdist

# 生成2*10的矩阵，且值均匀分布的随机数
cluster1 = np.random.uniform(0.5, 1.5, (2, 10))
cluster2 = np.random.uniform(1.5, 2.5, (2, 10))
cluster3 = np.random.uniform(1.5, 3.5, (2, 10))
cluster4 = np.random.uniform(3.5, 4.5, (2, 10))

# 顺序连接两个矩阵，形成一个新矩阵,所以生成了一个2*20的矩阵，T做转置后变成20*2的矩阵,刚好是一堆(x,y)的坐标点
X1 = np.hstack((cluster1, cluster2))
X2 = np.hstack((cluster3, cluster4))
X = np.hstack((X1, X2)).T

K = range(1, 10)
meandistortions = []
for k in K:
    kmeans = KMeans(n_clusters=k)
    kmeans.fit(X)
    # 求kmeans的成本函数值
    meandistortions.append(sum(np.min(cdist(X, kmeans.cluster_centers_, 'euclidean'), axis=1)) / X.shape[0])

plt.figure()
plt.grid(True)
plt1 = plt.subplot(2,1,1)
# 画样本点
plt1.plot(X[:,0],X[:,1],'k.');
plt2 = plt.subplot(2,1,2)
# 画成本函数值曲线
plt2.plot(K, meandistortions, 'bx-')
plt.show()
```

图像如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/a55469f0d28a1f89bf8ca1d826e722260237d2d7.png)

从曲线上可以看到，随着k的增加，成本函数值在降低，但降低的变化幅度不断在减小，因此急速降低才是最合适的，这里面也许3是比较合适的，你也许会有不同看法

通过这种方法来判断最佳K值的方法叫做肘部法则，你看图像像不像一个人的胳膊肘？



