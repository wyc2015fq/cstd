# 异常检测算法：孤立森林(Isolation Forest) - Machine Learning with Peppa - CSDN博客





2018年08月01日 13:24:36[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：3519








**摘要：iForest用于挖掘异常数据，如网络安全中的攻击检测和流量异常分析，金融机构则用于挖掘出欺诈行为。算法对内存要求很低，且处理速度很快，其时间复杂度也是线性的。可以很好的处理高维数据和大数据，并且也可以作为在线异常检测。**

![](https://upload-images.jianshu.io/upload_images/1297521-bc5add2ee92436d2.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)

0x14.jpg



## 01 孤立森林

isolation，意为孤立/隔离，是名词，其动词为isolate，forest是森林，合起来就是“孤立森林”了，也有叫“独异森林”，好像并没有统一的中文叫法。可能大家更习惯用其英文的名字isolation forest，简称iForest。

iForest算法用于挖掘异常(Anomaly)数据，或者说离群点挖掘，总之是在一大堆数据中，找出与其它数据的规律不太符合的数据。通常用于网络安全中的攻击检测和流量异常等分析，金融机构则用于挖掘出欺诈行为。对于找出的异常数据，然后要么直接清除异常数据，如数据清理中的去除噪声数据，要么深入分析异常数据，比如分析攻击、欺诈的行为特征。

算法起源于08年的一篇论文《Isolation Forest》，这论文由澳大利亚莫纳什大学的两位教授Fei Tony Liu, Kai Ming Ting(这两个名字看起来都像是华人)和南京大学的周志华教授共同完成，而这三人在2011年又发表了《Isolation-based Anomaly Detection》，这两篇论文算是确定了这个算法的基础。

> 
论文地址：
[http://cs.nju.edu.cn/zhouzh/zhouzh.files/publication/icdm08b.pdf](https://link.jianshu.com?t=http://cs.nju.edu.cn/zhouzh/zhouzh.files/publication/icdm08b.pdf)
[http://cs.nju.edu.cn/zhouzh/zhouzh.files/publication/tkdd11.pdf](https://link.jianshu.com?t=http://cs.nju.edu.cn/zhouzh/zhouzh.files/publication/tkdd11.pdf)


关于算法 ，网上的中文资料也并不多，下面是搜索到周志华教授在2015-07-12发的信息：

> 

ICML上遇到国际机器学习学会首任主席Dietterich教授，对我们的iForest算法大赞，说尝试了很多方法，还是这个又快又好。前段时间澳洲某startup公司也说他们发现iForest在信息安全领域的异常检测应用中表现最佳并准备做进产品。isolation Forest，推荐给有异常检测任务的同学。

从上面的评价中来看，iForest算法在实际的应用中应该具有不错的效果，得益于随机森林的思想，能快速处理大规模的数据，在当前的大数据环境下，应该很受欢迎。

## 02 算法原理

与随机森林由大量决策树组成一样，iForest森林也由大量的树组成。iForest中的树叫isolation tree，简称iTree。iTree树和决策树不太一样，其构建过程也比决策树简单，因为其中就是一个完全随机的过程。

假设数据集有N条数据，构建一颗iTree时，从N条数据中均匀抽样(一般是无放回抽样)出ψ个样本出来，作为这颗树的训练样本。

在样本中，随机选一个特征，并在这个特征的所有值范围内(最小值与最大值之间)随机选一个值，对样本进行二叉划分，将样本中小于该值的划分到节点的左边，大于等于该值的划分到节点的右边。

这样得到了一个分裂条件和左、右两边的数据集，然后分别在左右两边的数据集上重复上面的过程，直接达到终止条件。终止条件有两个，一个是数据本身不可再分(只包括一个样本，或者全部样本相同)，另外一个是树的高度达到log2(ψ)。不同于决策树，iTree在算法里面已经限制了树的高度。当然不限制也可以，只是算法为了效率考虑，只需要达到log2(ψ)深度即可。

把所有的iTree树构建好了，就可以对测数据进行预测了。预测的过程就是把测试数据在iTree树上沿对应的条件分支往下走，直到达到叶子节点，并记录这过程中经过的路径长度h(x)，即从根节点，穿过中间的节点，最后到达叶子节点，所走过的边的数量(path length)。

最后，将h(x)带入，计算每条待测数据的异常分数(Anomaly Score)，其计算公式为：

$ s(x,n) = 2^{(-\frac{E( { h(x) })} { c(n) } )} $

其中 $ c(n) = 2H(n − 1) − (2(n − 1)/n) $ 是二叉搜索树的平均路径长度，用来对结果进行归一化处理, 其中的H(k)可以通过公式 $ H(k) = ln(k) + \xi $来估计，$\xi$是欧拉常数，其值为0.5772156649。

$ h(x)$ 为路径长度，$ E(h(x)) $ 为森林中所有iTree树的平均路径长度。

使用异常分数，具有以下性质：

> 
- 如果分数越接近1，其是异常点的可能性越高；
- 如果分数都比0.5要小，那么基本可以确定为正常数据；
- 如果所有分数都在0.5附近，那么数据不包含明显的异常样本。

上面的步骤，可以总结为两步：

> 
- 训练：从训练集中进行采样，并构建iTree树；
- 测试：对iForest森林中的每颗iTree树进行测试，记录path length，然后根据异常分数计算公式，计算每条测试数据的anomaly score。

## 03 算法特点

在论文中，也比较了其它的常用异常挖掘的算法。比如常用的统计方法，基于分类的方法，和基于聚类的方法，这些传统算法通常是对正常的数据构建一个模型，然后把不符合这个模型的数据，认为是异常数据。而且，这些模型通常为正常数据作优化，而不是为异常数据作优化。而iForest可以显示地找出异常数据，而不用对正常的数据构建模型。

由于异常数据的两个特征（少且不同： few and different）

> 
- 异常数据只占很少量;
- 异常数据特征值和正常数据差别很大。

异常数据的这两个特征，确定了算法的理论基础。因此，构建二叉树型结构的时候，异常数据离根更近，而正常数据离根更远，更深。算法为了效率考虑，也限定了树的深度：ceil(log2(n))，这个值近似于树的平均深度，因为只需要关心那些低于平均高度的数据点，而不需要树进行完全生成。

算法只需要两个参数：树的多少与采样的多少。实验发现，在100颗树的时候，路径的长度就已经覆盖得比较好了，因此选100颗也就够了。采样，是为了更好的将正常数据和异常数据分离开来。有别于其它模型，采样数据越多，反面会降低iForest识别异常数据的能力。因为，通常使用256个样本，这也是scikit-learn实现时默认使用的采样数。

由于算法只需要采样数据256条样本，并且对树的深度也有限制，因此，算法对内存要求很低，且处理速度很快，其时间复杂度也是线性的。

不像其它算法，需要计算距离或者密度来寻找异常数据，iForest算法可以很好的处理高维数据和大数据，并且也可以作为在线预测。假设采样为256条，结点最大为511个，假设一个节点占b字节，共使用t颗树，那么需要的内存只有511tb字节，基本上只需要几M到几十M的内存就够了。数据还显示，预测287,748条数据只花了7.6秒。

另外，iForest既能发现群异常数据，也能发现散点异常数据。同时也能处理训练数据中不包含异常数据的情况。

这么好的算法，你还在等什么？

## 04 sklearn示例

IsolationForest在scikit-learn的0.18中才有实现，scikit-learn目前的stable版本为0.17，而0.18是dev版本。需要直接去github中clone。

源码和文档地址：

> 

[https://github.com/scikit-learn/scikit-learn/blob/master/sklearn/ensemble/iforest.py](https://link.jianshu.com?t=https://github.com/scikit-learn/scikit-learn/blob/master/sklearn/ensemble/iforest.py)
[http://scikit-learn.org/dev/modules/generated/sklearn.ensemble.IsolationForest.html](https://link.jianshu.com?t=http://scikit-learn.org/dev/modules/generated/sklearn.ensemble.IsolationForest.html)

下载源码与安装：

```
git clone https://github.com/scikit-learn/scikit-learn.git
cd scikit-learn
python setup.py install
```

算法基本上不需要配置参数就可以直接使用，通常就以下几个(参数明显比随机森林简单)：

> 
n_estimators: 默认为100，配置iTree树的多少

max_samples: 默认为265，配置采样大小

max_features: 默认为全部特征，对高维数据，可以只选取部分特征


示例：

```
import pandas as pd
from sklearn.ensemble import IsolationForest
ilf = IsolationForest(n_estimators=100,
                      n_jobs=-1,          # 使用全部cpu
                      verbose=2,
    )
data = pd.read_csv('data.csv', index_col="id")
data = data.fillna(0)
# 选取特征，不使用标签(类型)
X_cols = ["age", "salary", "sex"]
print data.shape

# 训练
ilf.fit(data[X_cols])
shape = data.shape[0]
batch = 10**6

all_pred = []
for i in range(shape/batch+1):
    start = i * batch
    end = (i+1) * batch
    test = data[X_cols][start:end]
    # 预测
    pred = ilf.predict(test)
    all_pred.extend(pred)

data['pred'] = all_pred
data.to_csv('outliers.csv', columns=["pred",], header=False)
```

算法的训练过程需要的内存很少，但如果数据量太大，在预测的时候，可能会把内存挤爆，上面代码中的for循环便是分块对数据进行预测，最后再组合起来。
- 参考文章

> 

[http://www.cnblogs.com/fengfenggirl/p/iForest.html](https://link.jianshu.com?t=http://www.cnblogs.com/fengfenggirl/p/iForest.html)
[http://qf6101.github.io/machine%20learning/2015/08/01/Isolation-Forest/](https://link.jianshu.com?t=http://qf6101.github.io/machine%20learning/2015/08/01/Isolation-Forest/)



