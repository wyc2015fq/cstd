# t-SNE和主成分分析 - Haidebian0514的博客 - CSDN博客





2017年05月12日 15:13:26[飞翔小鸥](https://me.csdn.net/Haidebian0514)阅读数：3045








?降维，什么是降维，简单说就是将多个特征字段用少数的特征字段来表示，方便对数据的后续分析以及进行2维和3维的可视化。降维的方法有多种，主成分分析、主因子分析、随机森林、决策树、LASSO回归以及t-SNE等，其实降维可以理解成对变量的一种选择。本文不对所有的方法进行介绍，主要介绍一下主成分分析和t-SNE这两种方法。 
![这里写图片描述](https://img-blog.csdn.net/20170512150232096?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGFpZGViaWFuMDUxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

?首先，先介绍主成分分析（PCA），主成分分析的基本思想是研究如何通过原来变量的少数几个线性组合来解释原来变量绝大多数信息的一种多元统计方法。主成分分析的基本理论通过研究原始变量的相关矩阵或协方差矩阵内部结构关系的研究，利用原始变量的线性组合形成几个综合指标（主成分），在保留原始变量主要信息的前提下起到降维与简化问题的作用。一般来说，利用主成分分析得到的主成分与原始变量之间有如下基本关系： 

1．  每一个主成分都是各原始变量的线性组合。 

2．  主成分的数目大大少于原始变量的数目。（严格说主成分和原始变量的个数是一样的，只是后面根据方差累积贡献率选取的主成分个数是远远少于原始变量的） 

3．  主成分保留了原始变量绝大多数信息。 

4．  各主成分之间不相关。 

   ? 主成分是把p个随机变量的总方差分解为P个不相关的随机变量的方差之和，使得第一个主成分的方差达到最大，第一主成分是以变化最大的方向向量各分量为系数的原始变量的线性函数，最大方差/总方差的比值称为第一主成分的贡献率。这个值越大，表明第一个主成分组合原始变量信息的能力越强。对于主成分个数的选取，就看前k个主成分的方差累积 贡献率达到85%，当所取的主成分的累积贡献率大于85%，就取k。这样是使得原始变量的信息损失不太多，又达到减少变量、简化问题的目的。需要注意的是主成分变换对正交向量的尺度敏感。数据在变换前需要进行归一化处理。同样也需要注意的是，新的主成分并不是由实际系统产生的，因此在进行 PCA 变换后会丧失数据的解释性。如果说，数据的解释能力对你的分析来说很重要，那么 PCA 对你来说可能就不适用了。 

?其次，介绍t-SNE又称为t分布随机领域嵌入算法，它是用于探索高维数据的非线性维数降低算法。它将多维数据映射到适合人类观察的两个或多个维度。t-sne是流行学习的一种，属于非线性降维的一种，主要是保证高维空间中相似的数据点在低维空间中尽量挨得近。是从sne演化而来，sne中用高斯分布衡量高维和地位空间数据点之间的相似性，t-sne主要是为了解决sne中的“拥挤问题”，用t分布定义低维空间低维空间中点的相似性。但是t-sne不能算是一种通用的降维方法吧，时间复杂度也挺高的。 

?t-SNE的原理是将距离转化成概率的形式，它表示原始数据点间较近的点在映射后也保持较近的关系，下面给出两点的条件概率： 
![这里写图片描述](https://img-blog.csdn.net/20170512150405811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGFpZGViaWFuMDUxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

|x_i?x_j|计算两个数据点间的欧式距离，|y_i?y_j| 表示映射点的距离 
![这里写图片描述](https://img-blog.csdn.net/20170512150419440?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGFpZGViaWFuMDUxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170512151248492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGFpZGViaWFuMDUxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)和 ![这里写图片描述](https://img-blog.csdn.net/20170512151227933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGFpZGViaWFuMDUxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)足够接近，即达到使数据点和映射点足够接近的目的。 

   ?  最后，用实际数据进行应用，数据也是一个APP用户行为的数据，做一个降维的可视化，代码和运用主成分分析和t-SNE进行降维的图形如下：
```python
import pandas as pd
from sklearn.preprocessing import LabelEncoder,MaxAbsScaler
from sklearn.manifold import TSNE
from matplotlib import pyplot as plt
from sklearn.decomposition import PCA
mbs = MaxAbsScaler()
e_cl = LabelEncoder()
col = ['visit_order',  'stay_time',
       'device_brand', 'device_type', 'Wdevice_resolution',
       'Hdevice_resolution', 'network_type', 'network_operator',
       'location_gps_long', 'location_gps_lat', 'extra_data']
df = pd.read_csv('out1.csv',engine='c')
df = df[col]
df = df.dropna()
df['device_brand'] = e_cl.fit_transform(df['device_brand'].values)
df['device_type'] = e_cl.fit_transform(df['device_type'].values)
df['network_type'] = e_cl.fit_transform(df['network_type'].values)
df = df.apply(pd.to_numeric,errors='coerce')
df = df.dropna()
X = mbs.fit_transform(df.values)
X = X[:6000]
digits_proj = TSNE(random_state=20150101).fit_transform(X)
pca_y = PCA(n_components=2).fit_transform(X)
plt.subplot(211)
plt.scatter(digits_proj[:,0],digits_proj[:,1])
plt.subplot(212)
plt.scatter(pca_y[:,0],pca_y[:,1])
plt.show()
```

结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20170512150532269?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSGFpZGViaWFuMDUxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

?从图形的结果看出，两个降维的差别比较大，主成分分析的方法出现了聚集性，说明用户行为出现了聚集性，但是t-SNE的降维方法，看不出用户行为的规律性，几乎所有高维数据集都可以使用t-SNE，但它广泛应用于图像处理，NLP，基因组数据和语音处理。而且t-SNE的耗时较长，如果数据量太大，又是单机运算的话，计算会特别慢。
参考文献： 
[http://tech.idcquan.com/78484.shtml](http://tech.idcquan.com/78484.shtml)

何晓群，《多元统计分析》 
[http://blog.csdn.net/u012162613/article/details/45920827](http://blog.csdn.net/u012162613/article/details/45920827)
[http://blog.csdn.net/lzl1663515011/article/details/46328337](http://blog.csdn.net/lzl1663515011/article/details/46328337)
[https://www.analyticsvidhya.com/blog/2017/01/t-sne-implementation-r-python/](https://www.analyticsvidhya.com/blog/2017/01/t-sne-implementation-r-python/)





