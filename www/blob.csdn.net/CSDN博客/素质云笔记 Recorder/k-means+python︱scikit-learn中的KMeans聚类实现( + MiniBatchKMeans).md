
# k-means+python︱scikit-learn中的KMeans聚类实现( + MiniBatchKMeans) - 素质云笔记-Recorder... - CSDN博客

2017年04月19日 15:07:15[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：88554



> 之前一直用R，现在开始学python之后就来尝试用Python来实现Kmeans。

> 之前用R来实现kmeans的博客：
> [笔记︱多种常见聚类模型以及分群质量评估（聚类注意事项、使用技巧）](http://blog.csdn.net/sinat_26917383/article/details/51611519)

> 聚类分析在客户细分中极为重要。有三类比较常见的聚类模型，K-mean聚类、层次（系统）聚类、最大期望EM算法。在聚类模型建立过程中，一个比较关键的问题是如何评价聚类结果如何，会用一些指标来评价。

> .

> 文章目录

> [@[toc]](#toc_8)

> [一、scikit-learn中的Kmeans介绍](#scikitlearnKmeans_12)

> [1、相关理论](#1_39)

> [2、主函数KMeans](#2KMeans_64)

> [3、简单案例一](#3_103)

> [4、案例二](#4_124)

> [5、案例四——Kmeans的后续分析](#5Kmeans_163)

> [二、大数据量下的Mini-Batch-KMeans算法](#MiniBatchKMeans_231)

> [主函数 :](#__252)

> [三、sklearn中的cluster进行kmeans聚类](#sklearnclusterkmeans_316)

> [四、分类变量聚类方法的K-modes与K-prototype](#KmodesKprototype_332)

> [延伸一：数据如何做标准化](#_360)

> [延伸二：Kmeans可视化案例](#Kmeans_368)

> [延伸三：模型保存](#_412)

> [延伸四：HDBSCAN与Kmeans的聚类的一些纪要](#HDBSCANKmeans_421)


> 一、scikit-learn中的Kmeans介绍

> scikit-learn 是一个基于Python的Machine Learning模块，里面给出了很多Machine

> Learning相关的算法实现，其中就包括K-Means算法。

> 官网scikit-learn案例地址：
> [http://scikit-learn.org/stable/modules/clustering.html\#k-means](http://scikit-learn.org/stable/modules/clustering.html#k-means)

> 部分来自：
> [scikit-learn 源码解读之Kmeans——简单算法复杂的说](http://midday.me/article/f8d29baa83ae41ec8c9826401eb7685e)
![这里写图片描述](https://img-blog.csdn.net/20170419143546349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 各个聚类的性能对比：
![这里写图片描述](https://img-blog.csdn.net/20170419143650433?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 优点：
原理简单
速度快
对大数据集有比较好的伸缩性
缺点：
需要指定聚类 数量K
对异常值敏感
对初始值敏感
> 1、相关理论

> 参考：
> [K-means算法及文本聚类实践](https://mp.weixin.qq.com/s?__biz=MzI1MTIzMzI2MA==&mid=2650560928&idx=1&sn=8d7d52a394f644ca50493250a969932e&chksm=f1feef23c6896635a36512c1d11361fa7ea5b73a58ea8f83dd1bc3924b5f96f8d0648f91f3bf&mpshare=1&scene=1&srcid=0429dA0Yha9hxr7mMxafeuBo#rd)

> （1）中心点的选择

> k-meams算法的能够保证收敛，但不能保证收敛于全局最优点，当初始中心点选取不好时，只能达到局部最优点，整个聚类的效果也会比较差。可以采用以下方法：k-means中心点

> 选择彼此距离尽可能远的那些点作为中心点；

> 先采用层次进行初步聚类输出k个簇，以簇的中心点的作为k-means的中心点的输入。

> 多次随机选择中心点训练k-means，选择效果最好的聚类结果

> （2）k值的选取

> k-means的误差函数有一个很大缺陷，就是随着簇的个数增加，误差函数趋近于0，最极端的情况是每个记录各为一个单独的簇，此时数据记录的误差为0，但是这样聚类结果并不是我们想要的，可以引入结构风险对模型的复杂度进行惩罚：
![这里写图片描述](https://img-blog.csdn.net/20170430110432620?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> λλ是平衡训练误差与簇的个数的参数，但是现在的问题又变成了如何选取λλ了，有研究[参考文献1]指出，在数据集满足高斯分布时，λ=2mλ=2m，其中m是向量的维度。

> 另一种方法是按递增的顺序尝试不同的k值，同时画出其对应的误差值，通过寻求拐点来找到一个较好的k值，详情见下面的文本聚类的例子。

> 2、主函数KMeans

> 参考博客：
> [python之sklearn学习笔记](http://lilian.info/blog/2016/12/sklearn.html)

> 来看看主函数KMeans：

> sklearn.cluster.KMeans(n_clusters=8,
	 init='k-means++', 
	n_init=10, 
	max_iter=300, 
	tol=0.0001, 
	precompute_distances='auto', 
	verbose=0, 
	random_state=None, 
	copy_x=True, 
	n_jobs=1, 
	algorithm='auto'
	)
> 参数的意义：

> n_clusters:簇的个数，即你想聚成几类

> init: 初始簇中心的获取方法

> n_init: 获取初始簇中心的更迭次数，为了弥补初始质心的影响，算法默认会初始10次质心，实现算法，然后返回最好的结果。

> max_iter: 最大迭代次数（因为kmeans算法的实现需要迭代）

> tol: 容忍度，即kmeans运行准则收敛的条件

> precompute_distances：是否需要提前计算距离，这个参数会在空间和时间之间做权衡，如果是True 会把整个距离矩阵都放到内存中，auto 会默认在数据样本大于featurs*samples 的数量大于12e6 的时候False,False 时核心实现的方法是利用Cpython 来实现的

> verbose: 冗长模式（不太懂是啥意思，反正一般不去改默认值）

> random_state: 随机生成簇中心的状态条件。

> copy_x: 对是否修改数据的一个标记，如果True，即复制了就不会修改数据。bool 在scikit-learn 很多接口中都会有这个参数的，就是是否对输入数据继续copy 操作，以便不修改用户的输入数据。这个要理解Python 的内存机制才会比较清楚。

> n_jobs: 并行设置

> algorithm: kmeans的实现算法，有：‘auto’, ‘full’, ‘elkan’, 其中 'full’表示用EM方式实现

> 虽然有很多参数，但是都已经给出了默认值。所以我们一般不需要去传入这些参数,参数的。可以根据实际需要来调用。

> 3、简单案例一

> 参考博客：
> [python之sklearn学习笔记](http://lilian.info/blog/2016/12/sklearn.html)

> 本案例说明了，KMeans分析的一些类如何调取与什么意义。

> import numpy as np
from sklearn.cluster import KMeans
data = np.random.rand(100, 3) \#生成一个随机数据，样本大小为100, 特征数为3
\#假如我要构造一个聚类数为3的聚类器
estimator = KMeans(n_clusters=3)\#构造聚类器
estimator.fit(data)\#聚类
label_pred = estimator.labels_ \#获取聚类标签
centroids = estimator.cluster_centers_ \#获取聚类中心
inertia = estimator.inertia_ \# 获取聚类准则的总和
> estimator初始化Kmeans聚类；estimator.fit聚类内容拟合；

> estimator.label_聚类标签，这是一种方式，还有一种是predict；estimator.cluster_centers_聚类中心均值向量矩阵

> estimator.inertia_代表聚类中心均值向量的总和

> 4、案例二

> 案例来源于：
> [使用scikit-learn进行KMeans文本聚类](https://blog.yueyu.io/p/1614)

> from sklearn.cluster import KMeans
 
num_clusters = 3
km_cluster = KMeans(n_clusters=num_clusters, max_iter=300, n_init=40, \
                    init='k-means++',n_jobs=-1)
\#返回各自文本的所被分配到的类索引
result = km_cluster.fit_predict(tfidf_matrix)
 
print "Predicting result: ", result
> km_cluster是KMeans初始化，其中用init的初始值选择算法用’k-means++’；

> km_cluster.fit_predict相当于两个动作的合并：km_cluster.fit（data）+km_cluster.predict（data），可以一次性得到聚类预测之后的标签，免去了中间过程。

> n_clusters: 指定K的值

> max_iter: 对于单次初始值计算的最大迭代次数

> n_init: 重新选择初始值的次数

> init: 制定初始值选择的算法

> n_jobs: 进程个数，为-1的时候是指默认跑满CPU

> 注意，这个对于单个初始值的计算始终只会使用单进程计算，

> 并行计算只是针对与不同初始值的计算。比如n_init=10，n_jobs=40,

> 服务器上面有20个CPU可以开40个进程，最终只会开10个进程

> 其中：

> km_cluster.labels_
km_cluster.predict(data)
> 这是两种聚类结果标签输出的方式,结果貌似都一样。都需要先km_cluster.fit(data)，然后再调用。

> 5、案例四——Kmeans的后续分析

> Kmeans算法之后的一些分析，参考来源：
> [用Python实现文档聚类](http://python.jobbole.com/85481/)

> from sklearn.cluster import KMeans
 
num_clusters = 5
 
km = KMeans(n_clusters=num_clusters)
 
%time km.fit(tfidf_matrix)
 
 
clusters = km.labels_.tolist()
> 分为五类，同时用%time来测定运行时间，把分类标签labels格式变为list。

> （1）模型保存与载入

> from sklearn.externals import joblib
 
\# 注释语句用来存储你的模型
joblib.dump(km,  'doc_cluster.pkl')
km = joblib.load('doc_cluster.pkl')
clusters = km.labels_.tolist()
> （2）聚类类别统计

> frame = pd.DataFrame(films, index = [clusters] , columns = ['rank', 'title', 'cluster', 'genre'])
frame['cluster'].value_counts()
> （3）质心均值向量计算组内平方和

> 选择更靠近质心的点，其中 km.cluster_centers_代表着一个 （聚类个数*维度数），也就是不同聚类、不同维度的均值。

> 该指标可以知道：

> 一个类别之中的，那些点更靠近质心；

> 整个类别组内平方和。

> 类别内的组内平方和要参考以下公式：
![这里写图片描述](https://img-blog.csdn.net/20170419145648423?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170419145655398?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 通过
> [公式](http://www.tyrtvu.cn/page/wangyekejian/yygltj/yygltj10/26/htm/26_1.html)
> 可以看出：

> 质心均值向量每一行数值-每一行均值（相当于均值的均值）

> 注意是平方。其中，n代表样本量，k是聚类数量（譬如聚类5）

> 其中，整篇的组内平方和可以通过来获得总量：

> km.inertia_
> .

> **公众号“素质云笔记”定期更新博客内容：**
> ![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> 二、大数据量下的Mini-Batch-KMeans算法

> 部分内容参考来源：
> [scikit-learn学习之K-means聚类算法与 Mini Batch K-Means算法](http://blog.csdn.net/gamer_gyt/article/details/51244850)

> 当数据量很大的时候，Kmeans 显然还是很弱的，会比较耗费内存速度也会收到很大影响。scikit-learn 提供了MiniBatchKMeans算法，大致思想就是对数据进行抽样，每次不使用所有的数据来计算，这就会导致准确率的损失。

> MiniBatchKmeans 继承自Kmeans 因为MiniBathcKmeans 本质上还利用了Kmeans 的思想.从构造方法和文档大致能看到这些参数的含义，了解了这些参数会对使用的时候有很大的帮助。batch_size 是每次选取的用于计算的数据的样本量，默认为100.

> Mini Batch K-Means算法是K-Means算法的变种，采用小批量的数据子集减小计算时间，同时仍试图优化目标函数，这里所谓的小批量是指每次训练算法时所随机抽取的数据子集，采用这些随机产生的子集进行训练算法，大大减小了计算时间，与其他算法相比，减少了k-均值的收敛时间，小批量k-均值产生的结果，一般只略差于标准算法。

> 该算法的迭代步骤有两步：

> 1：从数据集中随机抽取一些数据形成小批量，把他们分配给最近的质心

> 2：更新质心

> 与K均值算法相比，数据的更新是在每一个小的样本集上。对于每一个小批量，通过计算平均值得到更新质心，并把小批量里的数据分配给该质心，随着迭代次数的增加，这些质心的变化是逐渐减小的，直到质心稳定或者达到指定的迭代次数，停止计算

> Mini Batch K-Means比K-Means有更快的 收敛速度，但同时也降低了聚类的效果，但是在实际项目中却表现得不明显

> 一张k-means和mini batch k-means的实际效果对比图
![这里写图片描述](https://img-blog.csdn.net/20170419143836717?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 来看一下 MiniBatchKMeans的python实现：

> [官网链接](http://scikit-learn.org/stable/modules/generated/sklearn.cluster.MiniBatchKMeans.html#sklearn.cluster.MiniBatchKMeans)
> 、
> [案例一则链接](http://scikit-learn.org/stable/auto_examples/cluster/plot_mini_batch_kmeans.html#sphx-glr-auto-examples-cluster-plot-mini-batch-kmeans-py)

> 主函数 :

> MiniBatchKMeans(n_clusters=8, init=’k-means++’, max_iter=100, batch_size=100, verbose=0, compute_labels=True, random_state=None, 
tol=0.0, max_no_improvement=10, init_size=None, n_init=3, reassignment_ratio=0.01)
> 相关参数解释（来自博客：
> [用scikit-learn学习K-Means聚类](http://www.cnblogs.com/pinard/p/6169370.html)
> ）：

> random_state: 随机生成簇中心的状态条件,譬如设置random_state = 9

> tol: 容忍度，即kmeans运行准则收敛的条件

> max_no_improvement：即连续多少个Mini Batch没有改善聚类效果的话，就停止算法，

> 和reassignment_ratio， max_iter一样是为了控制算法运行时间的。默认是10.一般用默认值就足够了。

> batch_size：即用来跑Mini Batch

> KMeans算法的采样集的大小，默认是100.如果发现数据集的类别较多或者噪音点较多，需要增加这个值以达到较好的聚类效果。

> reassignment_ratio:

> 某个类别质心被重新赋值的最大次数比例，这个和max_iter一样是为了控制算法运行时间的。这个比例是占样本总数的比例，

> 乘以样本总数就得到了每个类别质心可以重新赋值的次数。如果取值较高的话算法收敛时间可能会增加，尤其是那些暂时拥有样本数较少的质心。

> 默认是0.01。如果数据量不是超大的话，比如1w以下，建议使用默认值。 如果数据量超过1w，类别又比较多，可能需要适当减少这个比例值。

> 具体要根据训练集来决定。

> import time
import numpy as np
import matplotlib.pyplot as plt
from sklearn.cluster import MiniBatchKMeans, KMeans
from sklearn.metrics.pairwise import pairwise_distances_argmin
from sklearn.datasets.samples_generator import make_blobs
\# 获取数据
np.random.seed(0)
batch_size = 45
centers = [[1, 1], [-1, -1], [1, -1]]
n_clusters = len(centers)
X, labels_true = make_blobs(n_samples=3000, centers=centers, cluster_std=0.7)
\# kmeans
\# Compute clustering with Means
k_means = KMeans(init='k-means++', n_clusters=3, n_init=10)
t0 = time.time()
k_means.fit(X)
t_batch = time.time() - t0
\# MiniBatchKMeans
mbk = MiniBatchKMeans(init='k-means++', n_clusters=3, batch_size=batch_size,
                      n_init=10, max_no_improvement=10, verbose=0)
t0 = time.time()
mbk.fit(X)
t_mini_batch = time.time() - t0
> 内容跟kmeans很像，只是一般多加一个参数，batch_size。

> .

> 三、sklearn中的cluster进行kmeans聚类

> 参考博客：
> [python之sklearn学习笔记](http://lilian.info/blog/2016/12/sklearn.html)

> import numpy as np
from sklearn import cluster
data = np.random.rand(100, 3) \#生成一个随机数据，样本大小为100, 特征数为3
k = 3 \# 假如我要聚类为3个clusters
[centroid, label, inertia] = cluster.k_means(data, k)
> 四、分类变量聚类方法的K-modes与K-prototype

> K-prototype与K-modes

> K-modes是K-means用在非数值集合上的一种方法，将原本K-means使用的欧式距离替换成字符间的汉明距离。

> 用去分类变量

> K-prototype是K-means与K-modes的一种集合形式，适用于数值类型与字符类型集合的数据。

> 度量具有混合属性的方法是，数值属性采用K-means方法得到P1，分类属性采用K-modes方法P2，那么D=P1+a*P2，a是权重。如果觉得分类属性重要，则增加a，否则减少a，a=0时即只有数值属性

> 更新一个簇的中心的方法，方法是结合K-means与K-modes的更新。

> code实现可参考：
> [nicodv/kmodes](https://github.com/nicodv/kmodes)

> **公众号“素质云笔记”定期更新博客内容：**
> ![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> .

> 延伸一：数据如何做标准化

> data_zs = 1.0*(data - data.mean())/data.std() \#数据标准化
> .

> 延伸二：Kmeans可视化案例

> 来源于博客：
> [使用python-sklearn-机器学习框架针对140W个点进行kmeans基于密度聚类划分](https://www.urlteam.org/2016/10/%E4%BD%BF%E7%94%A8python-sklearn-%E6%9C%BA%E5%99%A8%E5%AD%A6%E4%B9%A0%E6%A1%86%E6%9E%B6%E9%92%88%E5%AF%B9140w%E4%B8%AA%E7%82%B9%E8%BF%9B%E8%A1%8Ckmeans%E5%9F%BA%E4%BA%8E%E5%AF%86%E5%BA%A6%E8%81%9A/)

> from sklearn.cluster import KMeans
from sklearn.externals import joblib
import numpy
import time
import matplotlib.pyplot as plt
 
if __name__ == '__main__':
    \#\# step 1: 加载数据
    print "step 1: load data..."
    dataSet = []
    fileIn = open('./data.txt')
    for line in fileIn.readlines():
        lineArr = line.strip().split(' ')
        dataSet.append([float(lineArr[0]), float(lineArr[1])])
 
    \#设定不同k值以运算
    for k in range(2,10):
        clf = KMeans(n_clusters=k) \#设定k  ！！！！！！！！！！这里就是调用KMeans算法
        s = clf.fit(dataSet) \#加载数据集合
        numSamples = len(dataSet) 
        centroids = clf.labels_
        print centroids,type(centroids) \#显示中心点
        print clf.inertia_  \#显示聚类效果
        mark = ['or', 'ob', 'og', 'ok', '^r', '+r', 'sr', 'dr', '<r', 'pr']
        \#画出所有样例点 属于同一分类的绘制同样的颜色
        for i in xrange(numSamples):
            \#markIndex = int(clusterAssment[i, 0])
            plt.plot(dataSet[i][0], dataSet[i][1], mark[clf.labels_[i]]) \#mark[markIndex])
        mark = ['Dr', 'Db', 'Dg', 'Dk', '^b', '+b', 'sb', 'db', '<b', 'pb']
        \# 画出质点，用特殊图型
        centroids =  clf.cluster_centers_
        for i in range(k):
            plt.plot(centroids[i][0], centroids[i][1], mark[i], markersize = 12)
            \#print centroids[i, 0], centroids[i, 1]
        plt.show()
![这里写图片描述](https://img-blog.csdn.net/20170419150509067?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 延伸三：模型保存

> from sklearn.externals import joblib
joblib.dump(km_cluster, "/..../train_model.m")
km_cluster = joblib.load(".../train_model.m")
kmeans_SSE.labels_
> 延伸四：HDBSCAN与Kmeans的聚类的一些纪要

> 如果输入数据的变量类型不同，部分是数值型（numerical），部分是分类变量（categorical），需要做特别处理。

> 方法1是将分类变量转化为数值型，但缺点在于如果使用独热编码（one hot encoding）可能会导致数据维度大幅度上升，如果使用标签编码（label encoding）无法很好的处理数据中的顺序（order）。方法2是对于数值型变量和分类变量分开处理，并将结果结合起来，具体可以参考Python的实现[1]，如K-mode和K-prototype。

> 输出结果非固定，多次运行结果可能不同。

> 首先要意识到K-means中是有随机性的，从初始化到收敛结果往往不同。一种看法是强行固定随机性，比如设定sklearn中的random state为固定值。另一种看法是，如果你的K均值结果总在大幅度变化，比如不同簇中的数据量在多次运行中变化很大，那么K均值不适合你的数据，不要试图稳定结果 [2]

> 运行效率与性能之间的取舍。

> 但数据量上升到一定程度时，如>10万条数据，那么很多算法都不能使用。最近读到的一篇对比不同算法性能随数据量的变化很有意思 [Benchmarking Performance and Scaling of Python Clustering Algorithms]。在作者的数据集上，当数据量超过一定程度时仅K均值和HDBSCAN可用。
![在这里插入图片描述](https://img-blog.csdn.net/20180921101040488?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![在这里插入图片描述](https://img-blog.csdn.net/20180921101051821?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> 因此不难看出，K均值算法最大的优点就是运行速度快，能够处理的数据量大，且易于理解。但缺点也很明显，就是算法性能有限，在高维上可能不是最佳选项。

> 一个比较粗浅的结论是，在数据量不大时，可以优先尝试其他算法。当数据量过大时，可以试试HDBSCAN。仅当数据量巨大，且无法降维或者降低数量时，再尝试使用K均值。

> 一个显著的问题信号是，如果多次运行K均值的结果都有很大差异，那么有很高的概率K均值不适合当前数据，要对结果谨慎的分析。

> 此外无监督聚类的评估往往不易，基本都是基于使用者的主观设计，如sklearn中提供的Silhouette Coefficient和 Calinski-Harabaz Index [5]。更多关于无监督学习如何评估可以参考 [微调：一个无监督学习算法，如何判断其好坏呢?]。

> 参考：
> [如何正确使用「K均值聚类」？](https://zhuanlan.zhihu.com/p/34330242)


> [
](https://img-blog.csdn.net/20180921101040488?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzI2OTE3Mzgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
