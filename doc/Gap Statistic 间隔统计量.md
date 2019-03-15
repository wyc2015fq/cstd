# Gap Statistic 间隔统计量

2017年04月26日 01:44:03 [Tongust](https://me.csdn.net/baidu_17640849) 阅读数：3908



- Gap Statistic
  - [聚类的紧支测度 measure of the compactness](https://blog.csdn.net/baidu_17640849/article/details/70769555#%E8%81%9A%E7%B1%BB%E7%9A%84%E7%B4%A7%E6%94%AF%E6%B5%8B%E5%BA%A6-measure-of-the-compactness)
  - [间隔统计量GS](https://blog.csdn.net/baidu_17640849/article/details/70769555#%E9%97%B4%E9%9A%94%E7%BB%9F%E8%AE%A1%E9%87%8Fgs)
  - Python 实现
    - [测试](https://blog.csdn.net/baidu_17640849/article/details/70769555#%E6%B5%8B%E8%AF%95)



# Gap Statistic

[Gap statistic](https://web.stanford.edu/~hastie/Papers/gap.pdf)由Tibshirani等人提出，用以解决聚类问题确定所判定类的数目。

## 聚类的紧支测度 （measure of the compactness）

最简单的方法是使用类内样本点之间的欧式距离来表示，记为DkDk，DKDK越小聚类的紧支性越好。[Ref](https://datasciencelab.wordpress.com/2013/12/27/finding-the-k-in-k-means-clustering/) 



Dk=∑xi∈Ck∑xj∈Ck||xi−xj||2=2nk∑xi∈Ck||xi−μk||2Dk=∑xi∈Ck∑xj∈Ck||xi−xj||2=2nk∑xi∈Ck||xi−μk||2

标准化后：

 



Wk=∑k=1K12nkDkWk=∑k=1K12nkDk

WkWk

是elbow method的基础，见下图。

 





## 间隔统计量（GS）

[GS](https://web.stanford.edu/~hastie/Papers/gap.pdf)的基本思路是：引入参考的测度直，这个参考直可以有Monte Carlo采样的方法获得。 



Gapn(k)=E∗nlogWk−logWkE∗nlogWk=(1/B)∑b=1Blog(W∗kb)Gapn(k)=En∗logWk−logWkEn∗logWk=(1/B)∑b=1Blog(Wkb∗)

B是sampling的次数。为了修正MC带来的误差，我们计算

sksk

也即标准差来矫正GS。

 



w′=(1/B)∑b=1Blog(W∗kb)sd(k)=(1/B)∑b(logW∗kb−w′)2−−−−−−−−−−−−−−−−−−−√sk=1+BB−−−−−−√sd(k)w′=(1/B)∑b=1Blog(Wkb∗)sd(k)=(1/B)∑b(logWkb∗−w′)2sk=1+BBsd(k)

选择满足

Gapk>=Gapk+1−sk+1Gapk>=Gapk+1−sk+1

的最小的

kk

作为最有的聚类个数。下图阐释了GS的过程。

 





## Python 实现

```
import scipy
import scipy.cluster.vq
import scipy.spatial.distance
import numpy as np
EuclDist = scipy.spatial.distance.euclidean
def gapStat(data, resf=None, nrefs=10, ks=range(1,10)):
    '''
    Gap statistics
    '''
    # MC
    shape = data.shape
    if resf == None:
        x_max = data.max(axis=0)
        x_min = data.min(axis=0)
        dists = np.matrix(np.diag(x_max-x_min))
        rands = np.random.random_sample(size=(shape[0], shape[1], nrefs))
        for i in xrange(nrefs):
            rands[:,:,i] = rands[:,:,i]*dists+x_min
    else:
        rands = refs
    gaps = np.zeros((len(ks),))
    gapDiff = np.zeros(len(ks)-1,)
    sdk = np.zeros(len(ks),)
    for (i,k) in enumerate(ks):
        (cluster_mean, cluster_res) = scipy.cluster.vq.kmeans2(data, k)
        Wk = sum([EuclDist(data[m,:], cluster_mean[cluster_res[m],:]) for m in xrange(shape[0])])
        WkRef = np.zeros((rands.shape[2],))
        for j in xrange(rands.shape[2]):
            (kmc,kml) = scipy.cluster.vq.kmeans2(rands[:,:,j], k)
            WkRef[j] = sum([EuclDist(rands[m,:,j],kmc[kml[m],:]) for m in range(shape[0])])
        gaps[i] = scipy.log(scipy.mean(WkRef))-scipy.log(Wk)
        sdk[i] = np.sqrt((1.0+nrefs)/nrefs)*np.std(scipy.log(WkRef))

        if i > 0:
            gapDiff[i-1] = gaps[i-1] - gaps[i] + sdk[i]
    return gaps, gapDiff123456789101112131415161718192021222324252627282930313233343536
```

### 测试

```
mean = (1, 2)
cov = [[1, 0], [0, 1]]
#np.random.multivariate_normal(1.1, [[0,1],[1,0]])
Nf = 1000;
dat1 = np.zeros((3000,2))
dat1[0:1000,:] = numpy.random.multivariate_normal(mean, cov, 1000)
mean = [5, 6]
dat1[1000:2000,:] = numpy.random.multivariate_normal(mean, cov, 1000)
mean = [3, -7]
dat1[2000:3000,:] = numpy.random.multivariate_normal(mean, cov, 1000)
plt.plot(dat1[::,0], dat1[::,1], 'b.', linewidth=1)
plt.legend()123456789101112
```

![fig1](https://img-blog.csdn.net/20170426014040249?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMTc2NDA4NDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

```
gaps,gapsDiff = gapStat(dat1)
%matplotlib inline
f, (a1,a2) = plt.subplots(2,1)
a1.plot(gaps, 'g-o')
a2.bar(np.arange(len(gapsDiff)),gapsDiff)
f.show()123456
```

![img](https://img-blog.csdn.net/20170426014216015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMTc2NDA4NDk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
从图中可以看到k应该取3.