# PCA降维及其实现 - 李鑫o_O - CSDN博客

置顶2016年03月10日 21:24:07[hustlx](https://me.csdn.net/HUSTLX)阅读数：878
个人分类：[机器学习](https://blog.csdn.net/HUSTLX/article/category/6122182)



## 1.1算法流程

假设有m个samples，每个数据有n维。

1. 计算各个feature的平均值，计μj
 ;（Xj(i)表示第i个样本的第j维特征的value）

μj = Σm Xj(i)/m

meanVals = mean(dataMat, axis=0)

2. 将每一个feature scaling：将在不同scale上的feature进行归一化；

3. 将特征进行mean normalization

Xj(i)= (Xj(i)-μj)/sj

meanRemoved = dataMat - meanVals #remove mean

4. 求n×n的协方差矩阵Σ：

![image](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311093000210-2054484909.png)

covMat = cov(meanRemoved, rowvar=0)

5.求取特征值和特征向量：

[U,S,V] = SVD（Σ）

eigVals,eigVects = linalg.eig(mat(covMat))

6. 按特征值从大到小排列，重新组织U

如果使用否则的话应进行排序，并按照该次序找到对应的特征向量重新排列。

eigValInd = argsort(eigVals) 

7. 选择k个分量

按照第五、六步中讲的后，我们得到了一个n×n的矩阵Σ和U，这时，我们就需要从U中选出k个最重要的分量；即选择前k个特征向量，即为Ureduce, 该矩阵大小为n×k

eigValInd = eigValInd[:-(topNfeat+1):-1]  #cut off unwanted dimensions

![image](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311093001413-1141237738.png)

这样对于一个n维向量x，就可以降维到k维向量z了：

![image](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311093002038-349651223.png)

## 1.2、PCA降维实验

老师给的数据swissroll.dat

![image](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311093003585-1994741362.png)![image](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311093004366-1164149766.png)

自己生成数据：

```python
<span style="font-size:14px;"><span style="font-size:14px;">def make_swiss_roll(n_samples=100, noise=0.0, random_state=None):
    #Generate a swiss roll dataset.
    t = 1.5 * np.pi * (1 + 2 * random.rand(1, n_samples))
    x = t * np.cos(t)
    y = 83 * random.rand(1, n_samples)
    z = t * np.sin(t)
    X = np.concatenate((x, y, z))
    X += noise * random.randn(3, n_samples)
    X = X.T
    t = np.squeeze(t)
    return X, t</span></span>
```


1、Y=100*random.rand(1,2000)

![image](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311093005585-406911344.png)![image](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311093006460-1057435786.png)

2、y=21*random.rand(1,2000)

![image](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311093007507-365162676.png)![image](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311093008413-1056849989.png)

2、y=1*random.rand(1,2000)

![image](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311093009725-2061008276.png)![image](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311093010554-1269264113.png)

## 1.3、PCA降维实验小结

可以看到，当y的变化幅度较小时，最后降维之后的数据更类似于x，z轴数据，当y变化较大时，更类似于变化较大的y和x。

