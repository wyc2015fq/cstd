
# 一份关于kaggle特征构建技巧和心得 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月12日 19:32:08[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：38标签：[Kaggle																](https://so.csdn.net/so/search/s.do?q=Kaggle&t=blog)[特征构建																](https://so.csdn.net/so/search/s.do?q=特征构建&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Kaggle&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/i6642477603657613831/](https://www.toutiao.com/i6642477603657613831/)
2019-01-04 13:25:00
*摘要：*本文是一份关于如何在Kaggle排行榜上取得出色成绩的提示，包含经纬度数据的处理。
在很长的一段时间里，我们表现出缺乏创造力，所做出的工作被认为是山寨、借鉴，这一点是不可否认，但随着自身的积累，厚积薄发，完成了从借鉴到创造的突破。创造力是我们工作的基本要素之一，这点在各行各业都显得很重要，在机器学习领域也无所例外。
创建特征也需要创造力，因此本文在这里列出了我日常生活中的一些想法，希望对其它人有些启发，以至于能够在此基础上利用创造力在Kaggle排行榜上取得很好的成绩。
这篇文章的灵感来自于 Beluga在Kaggle上分享的文章，本文部分内容是直接摘自该文章中，因此，读者也可以看看这篇文章。以下是分享的正文：
# 1.当不需要时，不要尝试预测未来：
如果训练/测试都来自同一时间线，那么就可以非常巧妙地使用特性。虽然这只是一个kaggle的案例，但可以利用这个优势。例如：在出租车出行持续时间挑战赛中，从训练数据中随机抽取测试数据。在这种情况下，可以使用不同类别变量的平均目标变量作为特征。在这种情况下， Beluga 实际上使用了不同工作日的平均目标变量。然后，将相同的平均值映射为一个变量，并将其映射到测试数据中。
# 2. logloss裁剪技术：
这部分内容是在Jeremy Howard的神经网络课程中学到的内容，它基于一个非常简单的想法。如果我们非常自信和不公正的，Logloss会受到很多惩罚。因此，在必须预测概率的分类问题情况下，将概率剪切在0.05-0.95之间会好得多，这样就对自己的预测变得不是十分确定。
# 3.以gzip格式提交到kaggle：
下面一小段代码可以帮助我们节省无数的上传时间：
```python
df.to_csv('submission.csv.gz', index=False, compression='gzip')
```
# 4.如何最好地使用纬度和经度特征——第1部分：
在Beluga写的文章中，我最喜欢的一部分内容之一就是他如何使用经纬度（Lat / Lon）数据，这里创建了以下特征：
A.两个经纬度之间的Haversine距离：
```python
def haversine_array(lat1, lng1, lat2, lng2):
 lat1, lng1, lat2, lng2 = map(np.radians, (lat1, lng1, lat2, lng2))
 AVG_EARTH_RADIUS = 6371 # in km
 lat = lat2 - lat1
 lng = lng2 - lng1
 d = np.sin(lat * 0.5) ** 2 + np.cos(lat1) * np.cos(lat2) * np.sin(lng * 0.5) ** 2
 h = 2 * AVG_EARTH_RADIUS * np.arcsin(np.sqrt(d))
 return h
```
B.两个经纬度之间的曼哈顿距离：
```python
def dummy_manhattan_distance(lat1, lng1, lat2, lng2):
 a = haversine_array(lat1, lng1, lat1, lng2)
 b = haversine_array(lat1, lng1, lat2, lng1)
 return a + b
```
C.两个经纬度之间的方位：
```python
def bearing_array(lat1, lng1, lat2, lng2):
 AVG_EARTH_RADIUS = 6371 # in km
 lng_delta_rad = np.radians(lng2 - lng1)
 lat1, lng1, lat2, lng2 = map(np.radians, (lat1, lng1, lat2, lng2))
 y = np.sin(lng_delta_rad) * np.cos(lat2)
 x = np.cos(lat1) * np.sin(lat2) - np.sin(lat1) * np.cos(lat2) * np.cos(lng_delta_rad)
 return np.degrees(np.arctan2(y, x))
```
D.取放点之间的中心纬度和经度：
```python
train.loc[:, 'center_latitude'] = (train['pickup_latitude'].values + train['dropoff_latitude'].values) / 2
train.loc[:, 'center_longitude'] = (train['pickup_longitude'].values + train['dropoff_longitude'].values) / 2
```
# 5.如何最好地使用经纬度特征——第2部分：
在Beluga写的文章中，他使用经纬度数据的第二种方式是为取放点的经纬度创建集群，它的工作方式是通过设计在数据中创建了一些行政区。
```python
from sklearn.cluster import MiniBatchKMeans
coords = np.vstack((train[['pickup_latitude', 'pickup_longitude']].values,
 train[['dropoff_latitude', 'dropoff_longitude']].values,
 test[['pickup_latitude', 'pickup_longitude']].values,
 test[['dropoff_latitude', 'dropoff_longitude']].values))
sample_ind = np.random.permutation(len(coords))[:500000]
kmeans = MiniBatchKMeans(n_clusters=100, batch_size=10000).fit(coords[sample_ind])
train.loc[:, 'pickup_cluster'] = kmeans.predict(train[['pickup_latitude', 'pickup_longitude']])
train.loc[:, 'dropoff_cluster'] = kmeans.predict(train[['dropoff_latitude', 'dropoff_longitude']])
test.loc[:, 'pickup_cluster'] = kmeans.predict(test[['pickup_latitude', 'pickup_longitude']])
test.loc[:, 'dropoff_cluster'] = kmeans.predict(test[['dropoff_latitude', 'dropoff_longitude']])
```
然后，他使用这些集群创建了一些特征，例如比如计算某一天外出和入境的次数。
# 6.如何最好地使用纬度和经度特征——第3部分
在Beluga写的文章中，还使用了PCA方法来转换经度和纬度坐标。在这种情况下，它不是进行降维，而是进行了坐标的变换，2D—>2D变换，它实际上做了如下操作。
```python
pca = PCA().fit(coords)
train['pickup_pca0'] = pca.transform(train[['pickup_latitude', 'pickup_longitude']])[:, 0]
train['pickup_pca1'] = pca.transform(train[['pickup_latitude', 'pickup_longitude']])[:, 1]
train['dropoff_pca0'] = pca.transform(train[['dropoff_latitude', 'dropoff_longitude']])[:, 0]
train['dropoff_pca1'] = pca.transform(train[['dropoff_latitude', 'dropoff_longitude']])[:, 1]
test['pickup_pca0'] = pca.transform(test[['pickup_latitude', 'pickup_longitude']])[:, 0]
test['pickup_pca1'] = pca.transform(test[['pickup_latitude', 'pickup_longitude']])[:, 1]
test['dropoff_pca0'] = pca.transform(test[['dropoff_latitude', 'dropoff_longitude']])[:, 0]
test['dropoff_pca1'] = pca.transform(test[['dropoff_latitude', 'dropoff_longitude']])[:, 1]
```
# 7.不要忘记可以用特征做的正常事情：
按Max-Min缩放；
使用标准偏差进行标准化；
基于特征/目标的日志：使用基于特征或基于目标特征的日志;
热编码；
# 8.创建直观的附加特征：
A）日期时间特征：基于时间的特征，如“晚上”、“中午”、“夜晚”、“上月购买行为”，“上周购买行为”等；
B）思想特征：假设有购物车数据，并且想要对行程进行分类（参阅Walmart Recruiting：Kaggle的行程类型分类）；
此外，还可以考虑创建一个像“时尚”这样的特征，可以通过添加属于男装时尚、女装时尚、青少年时尚类别的项目来创建这个变量。
另外，也可以创建一个像“稀有”这样的特征，它是根据我们拥有的数据标记一些稀有物品然后计算购物车中稀有物品的数量而创建的，这些特征可能是有效的或无效的。根据我的观察，它们通常能够提供很多价值。
# 9.做的不那么正常的事情：
这些特征非常不直观，不应在机器学习模型需要解释的地方创建。
A）交互特征：如果有特征A和B，并创建特征A * B、A + B、A / B、AB，这会使得特征空间爆炸。如果你有10个特征，并且要创建两个可变交互特征，这将为模型添加 180个特征。并且，绝大多数时候，都会有超过10个的特征。
B）使用散列的存储桶特征：假设你有数千的特征，并按顺序排好，但考虑到算法的训练时间，并不想使用所有的数千千个特征。一般是使用一些哈希算法来实现这一点，最后完成文本分类任务。
例如：
假设有6个特征A、B、C、D、E、F：
并且数据行是：
```python
A：1、B：1、C：1、D：0、E：1、F：0
```
可能决定使用散列函数，以便这6个特征对应于3个桶并创建使用此特征的数据哈希矢量。
处理完后，数据可能如下所示：
```python
Bucket1：2、Bucket2：2、Bucket3：0
```
A：1、B：1、C：1、D：0、E：1、F：0 之所以发生这种情况是因为A和B掉落在桶1中、C和E落在桶2中、D和F落在桶3中。这里只是总结了上述的观察结果，你也可以用你想要的任何数学函数替换掉上述的加法操作。
之后，将使用Bucket1、Bucket2、Bucket3作为机器学习的变量。
A：1、B：1、C：1、D：0、E：1、F：0 以上是本文的全部内容，后续将持续更新，如果读者有比较好的处理方法，请在下面留言给出。
# 作者信息
Rahul Agarwal，统计分析
本文由阿里云云栖社区组织翻译。
文章原标题《Good Feature Building Techniques and Tricks for Kaggle》，译者：海棠，审校：Uncle_LLD。

