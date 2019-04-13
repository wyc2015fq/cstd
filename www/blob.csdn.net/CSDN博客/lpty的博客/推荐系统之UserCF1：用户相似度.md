
# 推荐系统之UserCF1：用户相似度 - lpty的博客 - CSDN博客

2016年10月05日 18:50:47[lpty](https://me.csdn.net/sinat_33741547)阅读数：4946


协同过滤是什么，从字面理解就是通过许多用户持续与网站互动，来达到筛选的目的。也就是说，这是一个基于用户行为数据分析的算法。
首先介绍一下用户行为数据，在一个网站上，用户的行为数据一般指的就是用户在浏览网站后留在服务器上的访问日记，大概可以分为这么几类：浏览、点击、购买、评分、评论、分享等等。
协同过滤算法可以有多个方法来实现，像基于邻域的方法，隐语义模型，基于图的随机游走等，接下来的内容选用的是基于邻域的方法，这个也是公认效果相对好的方法。
基于邻域的协同过滤主要分为两类，基于用户的协同过滤和基于物品的协同过滤。前者给用户推荐和他兴趣相似的其他用户喜欢的物品，后者则是推荐和他之前喜欢过的物品相似的物品。
这里介绍基于用户的协同过滤，从定义来说，可以分为以下两步进行：
一、找到和目标用户兴趣相似的用户集合
二、找和这个集合中的用户喜欢的，且目标用户没有听说过的物品推荐给目标用户
切入正题，用户相似度。
1、计算用户相似度的基本算法：
（1）Jaccard 系数：等于样本集交集与样本集合集的比值，即J=|A∩B|/|A∪B|
（2）余弦相似度：又称为余弦相似性。通过计算两个向量的夹角余弦值来评估他们的相似度，即w=|A∩B|/√|A|*|B|
2、数据集
这里选用MovieLens (http://grouplens.org/datasets/movielens/1m/)的1M数据集
3、首先将ratings.dat的数据进行处理，方便使用
```python
def readRatingDate(path=''):
    '''
    读取评分数据并存储为csv文件
    :param path:文件路径
    :return: DataFrame
    '''
    f = pd.read_table(path,sep='::',names=['UserID','MovieID','Rating','Timestamp'])
    f.to_csv('ratings.csv',index=False)
    return f
```
4、接下来还需要一个函数，来计算用户数据的相似度，这里我们用余弦相似度计算
```python
def calcuteSimilar(series1,series2):
    '''
    计算余弦相似度
    :param data1: 数据集1 Series
    :param data2: 数据集2 Series
    :return: 相似度
    '''
    unionLen = len(set(series1) & set(series2))
    if unionLen == 0: return 0.0
    product = len(series1) * len(series2)
    similarity = unionLen / math.sqrt(product)
    return similarity
```
5、最后就是计算啦，默认返回Top10个与用户1最相似的用户
```python
def calcuteUser(csvpath,targetID=1,TopN=10):
    '''
    计算targetID的用户与其他用户的相似度
    :return:相似度TopN Series
    '''
    frame = pd.read_csv(csvpath)                                                        #读取数据
    targetUser = frame[frame['UserID'] == targetID]['MovieID']                          #目标用户数据
    otherUsersID = [i for i in set(frame['UserID']) if i != targetID]                   #其他用户ID
    otherUsers = [frame[frame['UserID'] == i]['MovieID'] for i in otherUsersID]         #其他用户数据
    similarlist = [calcuteSimilar(targetUser,user) for user in otherUsers]              #计算
    similarSeries = pd.Series(similarlist,index=otherUsersID)                           #Series
    return similarSeries.sort_values()[-TopN:]
```
6、结果
ID相似度
681     0.311960
5762    0.315668
4718    0.315709
1858    0.319137
6006    0.320719
5705    0.334187
1283    0.364549
5190    0.377051
1481    0.384610
5343    0.389188





