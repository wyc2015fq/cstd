# 拉普拉斯特征图降维及其python实现 - 李鑫o_O - CSDN博客

置顶2016年03月10日 21:35:29[hustlx](https://me.csdn.net/HUSTLX)阅读数：4086


这种方法假设样本点在光滑的流形上，这一方法的计算数据的低维表达，局部近邻信息被最优的保存。以这种方式，可以得到一个能反映流形的几何结构的解。

步骤一：构建一个图G=(V,E),其中V={vi，i=1,2,3…n}是顶点的集合，E={eij}是连接顶点的vi和vj边，图的每一个节点vi与样本集X中的一个点xi相关。如果xi，xj相距较近，我们就连接vi，vj。也就是说在各自节点插入一个边eij，如果Xj在xi的k领域中，k是定义参数。

步骤二：每个边都与一个权值Wij相对应，没有连接点之间的权值为0，连接点之间的权值：

![clip_image002[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101249507-576079326.jpg)

步骤三：令*![clip_image004[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101250319-1042598952.gif)，*实现广义本征分解：

![clip_image006[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101250991-443880828.jpg)

使![clip_image008[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101251538-927699087.gif)是最小的m+1个本征值。忽略与![clip_image010[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101252179-992056390.gif)=0相关的本征向量，选取另外m个本征向量即为降维后的向量。

## 1、python实现拉普拉斯降维

```python
def laplaEigen(dataMat,k,t):
    m,n=shape(dataMat)
    W=mat(zeros([m,m]))
    D=mat(zeros([m,m]))
    for i in range(m):
        k_index=knn(dataMat[i,:],dataMat,k)
        for j in range(k):
            sqDiffVector = dataMat[i,:]-dataMat[k_index[j],:]
            sqDiffVector=array(sqDiffVector)**2
            sqDistances = sqDiffVector.sum()
            W[i,k_index[j]]=math.exp(-sqDistances/t)
            D[i,i]+=W[i,k_index[j]]
    L=D-W
    Dinv=np.linalg.inv(D)
    X=np.dot(D.I,L)
    lamda,f=np.linalg.eig(X)
return lamda,f
def knn(inX, dataSet, k):
    dataSetSize = dataSet.shape[0]
    diffMat = tile(inX, (dataSetSize,1)) - dataSet
    sqDiffMat = array(diffMat)**2
    sqDistances = sqDiffMat.sum(axis=1)
    distances = sqDistances**0.5
    sortedDistIndicies = distances.argsort()    
return sortedDistIndicies[0:k]
dataMat, color = make_swiss_roll(n_samples=2000)
lamda,f=laplaEigen(dataMat,11,5.0)
fm,fn =shape(f)
print 'fm,fn:',fm,fn
lamdaIndicies = argsort(lamda)
first=0
second=0
print lamdaIndicies[0], lamdaIndicies[1]
for i in range(fm):
    if lamda[lamdaIndicies[i]].real>1e-5:
        print lamda[lamdaIndicies[i]]
        first=lamdaIndicies[i]
        second=lamdaIndicies[i+1]
        break
print first, second
redEigVects = f[:,lamdaIndicies]
fig=plt.figure('origin')
ax1 = fig.add_subplot(111, projection='3d')
ax1.scatter(dataMat[:, 0], dataMat[:, 1], dataMat[:, 2], c=color,cmap=plt.cm.Spectral)
fig=plt.figure('lowdata')
ax2 = fig.add_subplot(111)
ax2.scatter(f[:,first], f[:,second], c=color, cmap=plt.cm.Spectral)
plt.show()
```

## 2、拉普拉斯降维实验

用如下参数生成实验数据存在swissdata.dat里面：

```python
def make_swiss_roll(n_samples=100, noise=0.0, random_state=None):
    #Generate a swiss roll dataset.
    t = 1.5 * np.pi * (1 + 2 * random.rand(1, n_samples))
    x = t * np.cos(t)
    y = 83 * random.rand(1, n_samples)
    z = t * np.sin(t)
    X = np.concatenate((x, y, z))
    X += noise * random.randn(3, n_samples)
    X = X.T
    t = np.squeeze(t)
return X, t
```

实验结果如下：

N=5，t=15：             N=7，t=15：            N=9，t=15：

![clip_image012[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101252960-7545757.jpg)![clip_image014[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101254163-761294652.jpg)![clip_image016[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101254835-1968418997.jpg)

N=11，t=15：             N=13，t=15：            N=15，t=15：

![clip_image018[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101255569-2053843419.jpg)![clip_image020[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101256413-340873584.jpg)![clip_image022[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101257038-1348774102.jpg)

N=17，t=15：             N=19，t=15：            N=21，t=15：

![clip_image024[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101257975-1906176074.jpg)![clip_image026[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101258694-1942239250.jpg)![clip_image028[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101259366-1429592047.jpg)

N=23，t=15：             N=25，t=15：            N=27，t=15：

![clip_image030[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101300038-1006665569.jpg)![clip_image032[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101301116-284220040.jpg)![clip_image034[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101301757-1280177757.jpg)

N=29，t=15：             N=31，t=15：            N=33，t=15：

![clip_image036[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101302460-1816677347.jpg)![clip_image038[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101303007-170717664.jpg)![clip_image040[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101303647-122572297.jpg)

N=25，t=5：              N=25，t=8：           N=25，t=10：

![clip_image042[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101304288-1272391629.jpg)![clip_image044[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101304913-176023769.jpg)![clip_image046[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101306038-612821624.jpg)

N=25，t=12：            N=25，t=14：               N=25，t=50：

![clip_image048[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101308913-962811292.jpg)![clip_image050[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101310241-382596344.jpg)![clip_image052[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101310913-833737809.jpg)

N=25，t=Inf：

![clip_image054[4]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311101313225-1230650393.jpg)

