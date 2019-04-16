# K-Prototypes聚类的Python实现 - littlely_ll的博客 - CSDN博客





2018年04月22日 21:04:15[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：3078








[k-prototypes聚类](https://blog.csdn.net/littlely_ll)

前一篇讲述了K-Prototypes聚类的原理以及它的伪代码，本篇根据上一篇内容编写了实现K-Prototypes的Python代码。

```python
# -*- coding: utf-8 -*-

import numpy as np
import random
from collections import Counter

def dist(x, y):
    return np.sqrt(sum((x-y)**2))

def sigma(x, y):
    return len(x) - sum(x == y)

def findprotos(data, k):
    m, n = data.shape
    num = random.sample(range(m), k)
    O = []
    C = []
    for i in range(n):
        try:
            if isinstance(data[0, i], int) or isinstance(data[0, i], float):
                O.append(i)
            elif isinstance(data[0, i], str):
                C.append(i)
            else:
                raise ValueError("the %d column of data is not a number or a string column" % i)
        except TypeError as e:
            print(e)

    O_data = data[:, O]
    C_data = data[:, C]
    O_protos = O_data[num, :]
    C_protos = C_data[num, :]

    return O, C, O_data, C_data, O_protos, C_protos

def KPrototypes(data, k, max_iters=10, gamma=0):

    m, n = data.shape
    O, C, O_data, C_data, O_protos, C_protos = findprotos(data, k)

    cluster = None
    clusterShip = []
    clusterCount = {}
    sumInCluster = {}
    freqInCluster = {}
    for i in range(m):
        mindistance = float('inf')
        for j in range(k):
            distance = dist(O_data[i,:], O_protos[j,:]) + \
                gamma * sigma(C_data[i,:], C_protos[j,:])
            if distance < mindistance:
                mindistance = distance
                cluster = j
        clusterShip.append(cluster)
        if  clusterCount.get(cluster) == None:
            clusterCount[cluster] = 1
        else:
            clusterCount[cluster] += 1
        for j in range(len(O)):
            if sumInCluster.get(cluster) == None:
                sumInCluster[cluster] = [O_data[i,j]] + [0] * (len(O) - 1)
            else:
                sumInCluster[cluster][j] += O_data[i,j]
            O_protos[cluster,j] = sumInCluster[cluster][j] / clusterCount[cluster]
        for j in range(len(C)):
            if freqInCluster.get(cluster) == None:
                freqInCluster[cluster] = [Counter(C_data[i,j])] + [Counter()] * (len(C) - 1)
            else:
                freqInCluster[cluster][j] += Counter(C_data[i,j])
            C_protos[cluster,j] = freqInCluster[cluster][j].most_common()[0][0]

    for t in range(max_iters):
        for i in range(m):
            mindistance = float('inf')
            for j in range(k):
                distance = dist(O_data[i, :], O_protos[j, :]) + \
                           gamma * sigma(C_data[i, :], C_protos[j, :])
                if distance < mindistance:
                    mindistance = distance
                    cluster = j
            if clusterShip[i] != cluster:
                oldCluster = clusterShip[i]
                clusterShip[i] = cluster
                clusterCount[cluster] += 1
                clusterCount[oldCluster] -= 1

                for j in range(len(O)):
                    sumInCluster[cluster][j] += O_data[i,j]
                    sumInCluster[oldCluster][j] -= O_data[i,j]
                    O_protos[cluster,j] = sumInCluster[cluster][j] / clusterCount[cluster]
                    O_protos[oldCluster, j] = sumInCluster[oldCluster][j] / clusterCount[oldCluster]

                for j in range(len(C)):
                    freqInCluster[cluster][j] += Counter(C_data[i,j])
                    freqInCluster[oldCluster][j] -= Counter(C_data[i,j])
                    C_protos[cluster,j] = freqInCluster[cluster][j].most_common()[0][0]
                    C_protos[oldCluster,j] = freqInCluster[oldCluster][j].most_common()[0][0]

    return clusterShip


if __name__ == "__main__":
    from sklearn.datasets import load_iris
    import pandas as pd
    iris = load_iris()
    O, C, O_data, C_data, O_protos, C_protos = findprotos(iris.data, 3)
    print(O)
    print("==============")
    print(C)
    print("==============")
    print(O_data)
    print("==============")
    print(C_data)
    print("==============")
    print(O_protos)
    print(O_protos[1,1])
    print("==============")
    print(C_protos)
    cluster = KPrototypes(data=iris.data,k=3, max_iters=30)
    print(cluster)
    s2 = pd.DataFrame(np.concatenate([iris.data, np.array([cluster]).T], axis=1))
    s2.to_csv("c:/users/ll/desktop/s2.csv")
```

上述代码对iris数据集进行了聚类，展现图形如下：

```
library(grid)
library(ggplot2)

s1 <- read.csv('s1.csv', header=T)
s2<- read.csv('s2.csv', header=T)
p1 <- ggplot(s1, aes(X0,X1, color=as.factor(X4))) + geom_point()
p2 <- ggplot(s2, aes(X0,X1, color=as.factor(X4))) + geom_point()
grid.newpage()
vp1 <- viewport(x=0, y=0.5, width=1,height=0.5, just=c('left','bottom'))
vp2 <- viewport(x=0, y=0, width=1,height=0.5, just=c('left','bottom'))
print(p1, vp=vp1)
print(p2, vp=vp2)
```

![这里写图片描述](https://img-blog.csdn.net/20180422204357975?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xpdHRsZWx5X2xs/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

以目标变量为着色点，可以看出聚类后的分类和原始的分类非常接近，说明聚类效果比较好。

github地址: [KPrototypes聚类](https://github.com/Alxe1/KPrototypes)



