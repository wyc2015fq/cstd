# 推荐模块︱apple.Turicreate个性化推荐recommender（五） - 素质云笔记/Recorder... - CSDN博客





2018年01月05日 16:39:19[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：1838








这是第五篇apple.Turicreate，主要模块是个性化推荐模块。一如既往的简洁，集成了非常多内容。 

笔者从实践来看，一直觉得关系网络模块、近邻模块以及如今的推荐模块都有很多相似的地方。 

从apple.Turicreate封装的内容以及可实现的功能来看，确实做到了互通有无，有种贯穿各类算法任督二脉的感觉。
> 
跟近邻模块相比，近邻模块每个节点都有自己的特征向量； 

  跟关系网络对比，推荐这边节点内是存在逻辑关系的，由用户ID–>电影ID定义。


**该模块主要是6款推荐算法 + model.recommend()函数。**
|推荐算法|函数名|内容|结果|
|----|----|----|----|
|基于item相似推荐|item_similarity_recommender|有预测功能，item之间喜爱的相似程度。适用在给未知人群推荐的时候，可以寻找到item的相似对|![这里写图片描述](https://img-blog.csdn.net/20180105171438434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)|
|因式分解|ranking_factorization_recommender以及factorization_recommender|**最常用,支持附加信息共同进模型**|![这里写图片描述](https://img-blog.csdn.net/20180105171433995?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)|
|基于内容的相似推荐|item_content_recommender|没有user概念，Item自己内容（多维度）决定，同类推荐，且没有点评数据可以提取的时候可以应用|数据格式不满足|
|项目流行度推荐|item popularity|基于项目流行程度来推荐，user不进入模型，缺点：并不能因人而异，受异常值影响较大|![这里写图片描述](https://img-blog.csdn.net/20180105171111411?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)|

用下面基本recommender中的数据来看看每个算法返回结果

参考网址1： 
[https://apple.github.io/turicreate/docs/api/turicreate.toolkits.recommender.html](https://apple.github.io/turicreate/docs/api/turicreate.toolkits.recommender.html)

参考网址2： 
[https://apple.github.io/turicreate/docs/userguide/recommender/introduction.html](https://apple.github.io/turicreate/docs/userguide/recommender/introduction.html)

.
# 0、基本recommender算法

开篇先上最基本的recommender，主函数：

```
turicreate.recommender.create(observation_data, user_id='user_id', item_id='item_id', 
                              target=None, user_data=None, item_data=None, ranking=True, verbose=True)
```

参数解析：
- target代表等级评分；
- user_data代表用户的辅助信息；
- item_data代表项目的辅助信息；
- 默认使用FactorizationRecommender模型。

案例：

```php
sf2 = turicreate.SFrame({'user_id': ["Ann", "Ann", "Ann", "Brian", "Brian", "Brian"],
                          'item_id': ["Item1", "Item2", "Item4", "Item2", "Item3", "Item5"],
                          'rating': [1, 3, 2, 5, 4, 2]})
m2 = turicreate.recommender.create(sf2, target="rating", ranking = False)
m2.recommend()
```

**数据结构理解就是，user_id看了item_id电影，然后给出了rating的评分。**

target就是标定评分。 

**当然，模型保存与调用还是一如既往的方式：**

```
# 模型保存与调用
model.save("my_model.model")
model = tc.load_model("my_model.model")
```

.

# 1、model.recommend()函数

主函数：
`recommend(users=None, k=10, exclude=None, items=None, new_observation_data=None, new_user_data=None, new_item_data=None, exclude_known=True, diversity=0, random_seed=None, verbose=True)`- 补齐ranking功能。如果A用户一些电影没有评价得分，那么推荐系统出来的时候，会预测A用户评价全电影的得分，并m.recommend()出来的结果，会排除已评价的内容。
- 返回的结果是排序的、筛选前K个指标,其中scores的得分是非标准化预测得分。
- 这些分数的语义含义可能因模型而异。例如，对于线性回归模型，用户的平均得分越高意味着该模型认为该用户通常比其他人更热情。

其余参数：
- k代表返回前K个推荐item内容；
- exclude,返回中不要包含exclude中的内容；
- exclude_known = False,返回所有的item的结果,一般情况下,用户评价过的不会返回
- items = item_subset， 只推荐items列举的案例可以给出一个范围。
- new_observation_data 新个案加入推荐

#### 举例：

```
import turicreate
data = turicreate.SFrame({'user_id': ["Ann", "Ann", "Ann", "Brian", "Brian", "Brian"],
                          'item_id': ["Item1", "Item2", "Item4", "Item2", "Item3", "Item5"],
                          'rating': [1, 3, 2, 5, 4, 2]})
m = turicreate.factorization_recommender.create(data, target='rating')

# 默认返回前10个内容
recommendations = m.recommend()

# 特定用户推荐
recommendations = m.recommend(users=['Brian'])

# 特定用户、特定items内容（只会在给出的两个item中推荐）
m.recommend(users=['Brian'],
            items=['Item1','Item2'])

# 某个用户不要推荐某些item、exclude字段
exclude_pairs = turicreate.SFrame({'user_id' : ['Ann'],
                                    'item_id' : ['Item3']})

recommendations = m.recommend(['Ann'], k = 5, exclude = exclude_pairs)

# 在某些主题、item范围内推荐
item_subset = turicreate.SArray(["Item3", "Item5", "Item2"])
recommendations = m.recommend(['Ann'], items = item_subset)
recommendations

# 推荐给从来没有出现过的人
m.recommend(['Matt','Moker'])  

# 附加信息一同进模型
user_info = turicreate.SFrame({'user_id': ['Ann', 'Brian'],
                                'age_category': ['2', '3']})
m_side_info = turicreate.factorization_recommender.create(data, target='rating',
                                                             user_data=user_info)
     # 带辅助信息的预测                                                        
new_user_info = turicreate.SFrame({'user_id' : ['Charlie'],
                                 'age_category' : ['2']})
recommendations = m_side_info.recommend(['Charlie'],
                                        new_user_data = new_user_info)

# 新个案信息灌入+推荐
m_item_sim = turicreate.item_similarity_recommender.create(data)
new_obs_data = turicreate.SFrame({'user_id' : ['Charlie', 'Charlie'],
                                'item_id' : ['Item1', 'Item5']})
recommendations = m_item_sim.recommend(['Charlie'], new_observation_data = new_obs_data)
```

特别说明：
- m.recommend()，推荐给从来没出现过的人，模型内会默认给每个电影一个得分，然后如果有新人过来，会直接推荐给他这个榜单;
- 附加信息一同进模型，一般选用的是factorization_recommender模型，主要有三种共同分解的方法：LinearRegressionModel, 

MatrixFactorizationModel, 

FactorizationModel。最简单就是线性的LinearRegressionModel，FactorizationModel是最强势的- 新个案信息灌入+推荐，利用item_similarity_recommender，基于源数据，新生成一个模型m_item_sim，该模型也可以利用，new_observation_data，进行添加。

.

# 2 五大推荐算法

> 
多的是使用jaccard距离去进行相似性判断，数值越大，代表相似性程度越高。跟近邻模块相比，近邻模块每个节点都有自己的特征向量，跟关系网络对比，推荐这边节点内是存在逻辑关系的，由用户ID给予电影ID定义。 

  该模块拥有predict功能。


## 2.1 基于item相似推荐（item_similarity_recommender）

Create a content-based recommender model in which the similarity between the items recommended is determined by the content of those items rather than learned from user interaction data([link](https://apple.github.io/turicreate/docs/api/generated/turicreate.recommender.item_content_recommender.create.html#turicreate.recommender.item_content_recommender.create)).

### 2.1.1 主函数create：
`turicreate.recommender.item_similarity_recommender.create(observation_data, user_id='user_id', item_id='item_id', target=None, user_data=None, item_data=None, nearest_items=None, similarity_type='jaccard', threshold=0.001, only_top_k=64, verbose=True, target_memory_usage=8589934592, **kwargs)`
参数列举：
- similarity_type : {‘jaccard’, ‘cosine’, ‘pearson’}
- target_memory_usage:内存使用峰值,8589934592==8G
- only_top_k:相似内容存储个数，默认是存储每个节点相似的前64个

### 2.1.2 得到相似对的主函数get_similar_items：
`FactorizationRecommender.get_similar_items(items=None, k=10, verbose=False)`
item_similarity_recommender将根据用户选择的相似性返回最相似的项目;将根据潜在项目因子之间的余弦相似度返回最近的项目。K代表返回的列表的前K个

### 2.1.3 应用案例

```
data = turicreate.SFrame({'user_id': ["Ann", "Ann", "Ann", "Brian", "Brian", "Brian"],
                          'item_id': ["Item1", "Item2", "Item4", "Item2", "Item3", "Item5"],
                          'rating': [1, 3, 2, 5, 4, 1]})

m = turicreate.item_similarity_recommender.create(data,target="rating")
nn = m.get_similar_items()
    # m.get_similar_users() # 这个接口不可以使用了,可以通过反转create()内的user_id 和 item_id来达到目的
print(nn)
    # 可以后续接内容 并进行预测
m2 = turicreate.item_similarity_recommender.create(data, nearest_items=nn)
```

m.get_similar_items()得到如下内容： 
![这里写图片描述](https://img-blog.csdn.net/20180105155825434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

得到了在推荐系统中，哪些item喜好度比较相似。 

根据items之间的喜好度，通过设置nearest_items，可以再次建模，`item_similarity_recommender`

得到模型之后的一些操作：
```
m2.predict(data)
    >>> dtype: float
    >>> Rows: 6
    >>> [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

m2.recommend()
m2.evaluate(data)
```

predict()预测操作；evaluate()评估内容。 
![这里写图片描述](https://img-blog.csdn.net/20180105160722458?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 2.2 因式分解factorization_recommender

### 2.2.1 因式分解两主函数

这里有两种主函数方式：

```python
#### ranking_factorization_recommender
turicreate.recommender.ranking_factorization_recommender.create(observation_data, user_id='user_id', item_id='item_id', target=None, user_data=None, item_data=None, num_factors=32, regularization=1e-09, linear_regularization=1e-09, side_data_factorization=True, ranking_regularization=0.25, unobserved_rating_value=None, num_sampled_negative_examples=4, max_iterations=25, sgd_step_size=0, random_seed=0, binary_target=False, solver='auto', verbose=True, **kwargs)

#### factorization_recommender
turicreate.recommender.factorization_recommender.create(observation_data, user_id='user_id', item_id='item_id', target=None, user_data=None, item_data=None, num_factors=8, regularization=1e-08, linear_regularization=1e-10, side_data_factorization=True, nmf=False, binary_target=False, max_iterations=50, sgd_step_size=0, random_seed=0, solver='auto', verbose=True, **kwargs)
```

两者的solver 参数，ials、adagrad、sgd三种方式。
- factorization_recommender中，binary_target,假设target由0/1构成，则会使用Logistic 

loss来进行拟合
- ranking_factorization_recommender学习了每个用户、Item的潜在因子，用这些潜因子来计算推荐排名。
- ranking_regularization,惩罚因子
- side_data_factorization :如果为true,以及辅助信息都有的情况下，就会用Factorization 

Machine model训练 

两者的区别,跟factorization_recommender一定需要’rating’评分，如果没有得分项则需要ranking_factorization_recommender 

原文解释： 

A RankingFactorizationRecommender learns latent factors for each user and item and uses them to rank recommended items according to the likelihood of observing those (user, item) pairs.  

This is commonly desired when performing collaborative filtering for implicit feedback datasets or datasets with explicit ratings for which ranking prediction is desired.
### 2.2.2 案例

```
sf = turicreate.SFrame({'user_id': ["0", "0", "0", "1", "1", "2", "2", "2"],
                      'item_id': ["a", "b", "c", "a", "b", "b", "c", "d"],
                      'rating': [1, 3, 2, 5, 4, 1, 4, 3]})

user_info = turicreate.SFrame({'user_id': ["0", "1", "2"],
                             'name': ["Alice", "Bob", "Charlie"],
                             'numeric_feature': [0.1, 12, 22]})
item_info = turicreate.SFrame({'item_id': ["a", "b", "c", "d"],
                             'name': ["item1", "item2", "item3", "item4"],
                             'dict_feature': [{'a' : 23}, {'a' : 13},
                                              {'b' : 1},
                                              {'a' : 23, 'b' : 32}]})
m2 = turicreate.ranking_factorization_recommender.create(sf, target='rating',
                                                         ranking_regularization  = 0.1,
                                                         unobserved_rating_value = 1,
                                              user_data=user_info,
                                              item_data=item_info)
# 可以通过solver修改方法，solver = 'ials'
m2.recommend()
```

![这里写图片描述](https://img-blog.csdn.net/20180105163204966?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
## 2.3 基于内容的相似推荐item_content_recommender

主要是针对Item，item的内容可以是向量或多个， 

主函数：
`turicreate.recommender.item_content_recommender.create(item_data, item_id, observation_data=None, user_id=None, target=None, weights='auto', similarity_metrics='auto', item_data_transform='auto', max_item_neighborhood_size=64, verbose=True)`
案例： 

某my_item_id下，有两类数据源，都对同一ID内容进行评价，得到向量组。

```
item_data = tc.SFrame({"my_item_id" : ['a','b','c','d'],
                           "data_1" : [ [1, 0,1], [1, 0,4], [0, 1,6], [0.5, 0.5,0.5] ],
                           "data_2" : [ [0, 1,2], [1, 0,3], [0, 1,7], [0.5, 0.5,1] ] })

m = tc.recommender.item_content_recommender.create(item_data, "my_item_id")
    # 项目名称
m.recommend_from_interactions([0, 1,2])
```

recommend_from_interactions是在已知的item中根据相互作用关系，推荐前K个内容。此时输入的是，观测向量。 

.

## 2.4 项目流行度推荐 item popularity

项目流行度，可以认为是暂时不考虑user的情况下，看每个电影的平均分，来进行电影排名。
`turicreate.recommender.popularity_recommender.create(observation_data, user_id='user_id', item_id='item_id', target=None, user_data=None, item_data=None, random_seed=0, verbose=True)`
如果没有评分rating的话,默认按照item的被观察的次数来进行计算; 

有评分的话，按照评分的平均数来进行计算 

案例有：
```
sf = turicreate.SFrame({'user_id': ["0", "0", "0", "1", "1", "2", "2", "2"],
                      'item_id': ["a", "b", "c", "a", "b", "b", "c", "d"],
                      'rating': [1, 3, 2, 5, 4, 1, 4, 3]})
m = turicreate.popularity_recommender.create(sf, target='rating')
m.recommend()
```

![这里写图片描述](https://img-blog.csdn.net/20180105171723432?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.

## 2.5模型对比compare_models
`turicreate.recommender.util.compare_models(dataset, models, model_names=None, user_sample=1.0, metric='auto', target=None, exclude_known_for_precision_recall=True, make_plot=False, verbose=True, **kwargs)`
metric 参数有: str, {‘auto’, ‘rmse’, ‘precision_recall’}

```
import turicreate
train_data = turicreate.SFrame({'user_id': ["0", "0", "0", "1", "1", "2", "2", "2"],
                              'item_id': ["a", "c", "e", "b", "f", "b", "c", "d"]})
test_data = turicreate.SFrame({'user_id': ["0", "0", "1", "1", "1", "2", "2"],
                             'item_id': ["b", "d", "a", "c", "e", "a", "e"]})
m1 = turicreate.item_similarity_recommender.create(train_data)
m2 = turicreate.item_similarity_recommender.create(train_data, only_top_k=1)
m3 = turicreate.ranking_factorization_recommender.create(train_data, 'user_id', 'item_id',solver='ials')
m4 = turicreate.ranking_factorization_recommender.create(train_data, 'user_id', 'item_id',solver='sgd')
m5 = turicreate.ranking_factorization_recommender.create(train_data, 'user_id', 'item_id',solver='adagrad')
turicreate.recommender.util.compare_models(test_data, [m1, m2, m3 ,m4,m5],
                                         metric='precision_recall')
```

得到的结论很多，准确率与召回率。 
![这里写图片描述](https://img-blog.csdn.net/20180105163509556?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)













