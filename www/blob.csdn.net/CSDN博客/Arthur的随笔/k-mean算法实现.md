# k-mean算法实现 - Arthur的随笔 - CSDN博客
2012年05月31日 23:30:45[largetalk](https://me.csdn.net/largetalk)阅读数：1509标签：[算法																[dataset																[distance																[float																[lambda																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=lambda&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=distance&t=blog)](https://so.csdn.net/so/search/s.do?q=dataset&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[python																[R&datamine																[algorithm](https://blog.csdn.net/largetalk/article/category/941205)](https://blog.csdn.net/largetalk/article/category/1063234)](https://blog.csdn.net/largetalk/article/category/715661)
k-means 算法的工作过程说明如下：
  初始化：聚类数k，初始聚类中心x,迭代次数或者收敛条件。
  首先,从n个数据对象任意选择 k 个对象作为初始聚类中心；而对于所剩下其它对象，则根据它们与这些聚类中心的相似度（距离），分别将它们分配给与其最相似的（聚类中心所代表的）聚类；
  然后,再计算每个所获新聚类的聚类中心（该聚类中所有对象的均值）；
  再次，不断重复上面的过程直到满足收敛条件或者迭代次数为止. 
目标：各聚类本身尽可能的紧凑，而各聚类之间尽可能的分开.
#################################################
```python
import random
import time
t1 = time.time()
dataset = []
with open('iries.txt', 'r') as f:
    for line in f:
        ds = tuple([ float(x) for x in line.split() ])
        dataset.append(ds)
#for x in  dataset:
#    print x[0], x[1], x[2], x[3]
#
def distance(x, y):
    z = 0
    for i in range(4):
        z = z + pow((x[i] - y[i]), 2)
    return pow(z, 0.5)
#print distance(dataset[1], dataset[2])
def get_means(ds):
    a = b = c = d = 0.0
    l = len(ds)
    for x in ds:
        a += x[0]
        b += x[1]
        c += x[2]
        d += x[3]
    return (a/l, b/l, c/l , d/l)
def gettotaldistance(clusters):
    '''
    [[(1,2,3,4), [(2,3,4,5),(3,4,5,6)...]], [(6,7,8,9),[...]],...]
    '''
    sum = 0
    for cluster in clusters:
        for point in cluster[1]:
            sum += distance(cluster[0], point)
    return sum
def push_into_cluster(clusters, point):
    c = 0
    d = distance(clusters[0][0], point)
    for x in range(1, len(clusters)):
        temp = distance(clusters[x][0], point)
        if temp < d:
            d = temp
            c = x
    clusters[c][1].append(point)
def kmeans(k = 3):
    rn_center = random.sample(dataset, k)
    clusters = []
    for x in rn_center:
        clusters.append([x, []])
    print clusters
 
    oldtotal = 999999999
    while True:
        for point in dataset:
            push_into_cluster(clusters, point)
        for cluster in clusters:
            cluster[0] = get_means(cluster[1])
        newtotal = gettotaldistance(clusters)
        if oldtotal - newtotal > 1:
            oldtotal = newtotal
            for cluster in clusters:
                cluster[1] = []
        else:
            print '============================'
            for x in clusters:
                print '-------------------------'
                print x[0]
            break
kmeans(k = 5)
print time.time() - t1
```
其中 iries.txt 来自 [http://www.codeforge.cn/read/186226/irises.txt__html](http://www.codeforge.cn/read/186226/irises.txt__html)
下面是一个使用了[pprocess](http://www.boddie.org.uk/python/pprocess.html)模块来并行计算得k-means算法实现， 但不是太好，只有一点点用到了并行，对并行计算还不是太熟
```python
import random
import time
import pprocess
t1 = time.time()
limit = 2 #core num
dataset = []
with open('iries.txt', 'r') as f:
    for line in f:
        ds = tuple([ float(x) for x in line.split() ])
        dataset.append(ds)
#for x in  dataset:
#    print x[0], x[1], x[2], x[3]
#
def distance(x, y):
    z = 0
    for i in range(4):
        z = z + pow((x[i] - y[i]), 2)
    return pow(z, 0.5)
#print distance(dataset[1], dataset[2])
def get_means(ds):
    a = b = c = d = 0.0
    l = len(ds)
    for x in ds:
        a += x[0]
        b += x[1]
        c += x[2]
        d += x[3]
    return (a/l, b/l, c/l , d/l)
def gettotaldistance(clusters):
    '''
    [[(1,2,3,4), [(2,3,4,5),(3,4,5,6)...]], [(6,7,8,9),[...]],...]
    '''
    td = 0
    results = pprocess.Map(limit = limit)
    calc = results.manage(pprocess.MakeParallel(distance))
    for cluster in clusters:
        for point in cluster[1]:
            calc(cluster[0], point)
    
    td = sum(results)
    print '------------------', results, td
    return td
def push_into_cluster(clusters, point):
    
    #results = pprocess.pmap(lambda x: distance(x, point), [ y[0] for y in clusters], limit = limit)
    #print '#########', results
    #minvalue = min(results)
    #print minvalue
    c = 0
    d = distance(clusters[0][0], point)
    for x in range(1, len(clusters)):
        temp = distance(clusters[x][0], point)
        if temp < d:
            d = temp
            c = x
    clusters[c][1].append(point)
def kmeans(k = 3, eps = 1):
    rn_center = random.sample(dataset, k)
    clusters = []
    for x in rn_center:
        clusters.append([x, []])
    print clusters
 
    oldtotal = 999999999
    while True:
        for point in dataset:
            push_into_cluster(clusters, point)
        for cluster in clusters:
            cluster[0] = get_means(cluster[1])
        newtotal = gettotaldistance(clusters)
        if oldtotal - newtotal > eps:
            oldtotal = newtotal
            for cluster in clusters:
                cluster[1] = []
        else:
            print '============================'
            for x in clusters:
                print '-------------------------'
                print x[0]
            break
kmeans(k = 5, eps = 0.5)
print time.time() - t1
```

