
# 推荐系统之UserCF2：用户对商品的感兴趣程度 - lpty的博客 - CSDN博客

2016年10月06日 19:16:44[lpty](https://me.csdn.net/sinat_33741547)阅读数：3091


基于用户的协同过滤，从定义来说，可以分为以下两步进行：
一、找到和目标用户兴趣相似的用户集合
二、找和这个集合中的用户喜欢的，且目标用户没有听说过的物品推荐给目标用户
在上一篇已经介绍过了如何计算与目标用户最相似的K个用户，接下来就是第二步，推荐商品了。
计算用户对商品的感兴趣程度，我们用下面的公式来求解：
![](https://img-blog.csdn.net/20161006194710503?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



其中u代表用户，i代表商品，S(u,k)包含和用户兴趣最接近的K个用户，N(i)是对物品i有过行为的用户集合，w代表用户u和用户v的相似程度，r代表用户v对物品i的兴趣。
(1)S(u,k)，w计算详见我的上一篇博客，这里不做介绍
(2)N(i)可以通过数据集求得
(3)用户v对物品i的兴趣r，这里可以由用户对电影的评分数据代替
那么，首先是计算用户对某一物品的感兴趣程度

```python
def calcuteInterest(frame,similarSeries,targetItemID):
    '''
    计算目标用户对目标物品的感兴趣程度
    :param frame: 数据
    :param similarSeries: 目标用户最相似的K个用户
    :param targetItemID: 目标物品
    :return:感兴趣程度
    '''
    similarUserID = similarSeries.index                                                 #和用户兴趣最相似的K个用户
    similarUsers = [frame[frame['UserID'] == i] for i in similarUserID]                 #K个用户数据
    similarUserValues = similarSeries.values                                            #用户和其他用户的兴趣相似度
    UserInstItem = []
    for u in similarUsers:                                                              #其他用户对物品的感兴趣程度
        if targetItemID in u['MovieID'].values: UserInstItem.append(u[u['MovieID']==targetItemID]['Rating'].values[0])
        else: UserInstItem.append(0)
    interest = sum([similarUserValues[v]*UserInstItem[v]/5 for v in range(len(similarUserValues))])
    return interest
```
接下来，就是对某一用户进行推荐，默认是对用户1进行top10推荐


```python
def calcuteItem(csvpath,targetUserID=1,TopN=10):
    '''
    计算推荐给targetUserID的用户的TopN物品
    :param csvpath: 数据路径
    :param targetUserID: 目标用户
    :param TopN:
    :return: TopN个物品及感兴趣程度
    '''
    frame = pd.read_csv(csvpath)                                                        #读取数据
    similarSeries = calcuteUser(csvpath=csvpath, targetID=targetUserID)                 #计算最相似K个用户
    userMovieID = set(frame[frame['UserID'] == 1]['MovieID'])                           #目标用户感兴趣的物品
    otherMovieID = set(frame[frame['UserID'] != 1]['MovieID'])                          #其他用户感兴趣的物品
    movieID = list(userMovieID ^ otherMovieID)                                          #差集
    interestList = [calcuteInterest(frame,similarSeries,movie) for movie in movieID]    #推荐
    interestSeries = pd.Series(interestList, index=movieID)
    return interestSeries.sort_values()[-TopN:]
```
结果就是这个样子，用了80多秒，这个需要再优化啦
物品ID  感兴趣程度
480      1.563953
2137    1.570173
593      1.579013
364      1.695860
596      1.836217
2080    1.864335
2085    2.088053
2096    2.163719
2078    2.356070
2081    2.642339
最后我们看看这个推荐结果，第一位推荐的是:
2081::Little Mermaid, The (1989)::Animation|Children's|Comedy|Musical|Romance
根据用户1的数据 1::F::1::10::48067，可以看出系统给未满18岁的女学生推荐了一部相对轻松的电影，结合用户1的观影历史，还是比较靠谱的。


