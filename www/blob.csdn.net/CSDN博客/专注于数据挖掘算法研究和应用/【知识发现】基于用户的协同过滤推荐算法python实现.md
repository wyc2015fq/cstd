# 【知识发现】基于用户的协同过滤推荐算法python实现 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年09月18日 18:56:10[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1390
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









1、协同过滤推荐算法分为两类，分别是基于用户的协同过滤算法（user-based collaboratIve filtering），和基于物品的协同过滤算法（item-based collaborative filtering）。




      1）基于用户的协同过滤算法是通过用户的历史行为数据发现用户对商品或内容的喜欢（如商品购买，收藏，内容评论或分享），并对这些喜好进行度量和打分。根据不同用户对相同商品或内容的态度和偏好程度计算用户之间的关系。在有相同喜好的用户间进行商品推荐。




      2）基于物品的协同过滤算法与基于用户的协同过滤算法很像，将商品和用户互换。通过计算不同用户对不同物品的评分获得物品间的关系。基于物品间的关系对用户进行相似物品的推荐。这里的评分代表用户对商品的态度和偏好。




2、这里做了初步的实现基于用户的协同过滤，相关理论可以进一步查阅。



```python
# -*- coding: utf-8 -*-
'''
Created on 2017年9月18日

@author: Jason.F
'''
import time
import pandas as pd
import math
import numpy as np
from operator import itemgetter 

def Recommend(user,df_test,W,TopK=3):
    rvi=1 #用户v对物品i的兴趣，默认为1
    #TopK：与用户user兴趣最接近的k个用户
    rank=dict()
    #df_itemU=df_sample[df_sample['userid'] == user]
    #u_items=list(set(df_itemU['itemid'].values))
    for v,wuv in sorted(W[user].items(),key=itemgetter(1),reverse=True)[0:TopK]:
        df_itemV=df_test[df_test['userid'] == v]
        v_items=list(set(df_itemV['itemid'].values))
        for i in v_items:
            #if i in u_items:
            #    continue
            if i not in rank.keys():
                rank[i]=0
            rank[i] +=wuv*rvi          
    return rank #返回推荐的物品

def UserSimilarity(df_sample):#计算用户相似度
    #建立物品到用户的倒排表
    item_users=dict()
    item_list = list(set(df_sample['itemid'].values))#获取物品列表
    for item in item_list:
        item_users[item]=set()
        df_user=df_sample[df_sample['itemid'] == item]
        user_list= list(set(df_user['userid'].values))#获取物品对应的用户列表
        for user in user_list:    
            item_users[item].add(user)
    #计算用户之间物品相似度
    C = dict()
    N = dict()
    #初始化
    for u in list(set(df_sample['userid'].values)):
        N[u]=0
        for v in list(set(df_sample['userid'].values)):
            if u!=v:
                AddTwoDict(C,u,v,0)
    W=C #初始化相似度字段
    #计算           
    for item,users in item_users.items():
        for u in users:
            N[u] += 1
            for v in users:
                if u==v:
                    continue
                #根据用户行为计算用户相似度，改良余弦相似度公式
                C[u][v] +=( 1/math.log(1+len(users)) )
    #计算用户相似度矩阵
    for u,related_users in C.items():
        for v,cuv in related_users.items():
            W[u][v]=cuv/math.sqrt(N[u]*N[v])
    return W

#二维词典插入
def AddTwoDict(theDict, key_a, key_b, val): 
    if key_a in theDict: 
        theDict[key_a].update({key_b: val}) 
    else: 
        theDict.update({key_a:{key_b: val}})
        
#计算准确率指标    
def Precision(df_test,W,N=5):
    hit=0
    all=0
    #N最靠前的5个物品
    for user in list(set(df_test['userid'].values)):
        rank=Recommend(user,df_test,W)
        df_item=df_test[df_test['userid'] == user]
        for item,pui in sorted(rank.items(),key=itemgetter(1),reverse=True)[0:N]:
            if item in list(set(df_item['itemid'].values)):
                hit+=1
            all += N
    return hit/(all*1.0)

#召回率
def Recall(df_test,W,N=5):
    hit=0
    all=0
    #N最靠前的5个物品
    for user in list(set(df_test['userid'].values)):
        rank=Recommend(user,df_test,W)
        df_item=df_test[df_test['userid'] == user]
        for item,pui in sorted(rank.items(),key=itemgetter(1),reverse=True)[0:N]:
            if item in list(set(df_item['itemid'].values)):
                hit+=1
        all += len(df_item)
    return hit/(all*1.0)
    
if __name__ == '__main__':    
    start = time.clock()  
    
    #导入数据
    df_sample = pd.read_csv("D:\\tmp\\ratings.csv",names=['userid','itemid','ratings','time'],header=0)  
    df_sample_=df_sample[['userid','itemid']]
    #模型训练
    W=UserSimilarity(df_sample)
    #模型预测
    #rank=Recommend(1,df_sample,W)#user=1
    #print (rank)
    #模型评估，准确率指标
    df_test=df_sample.sample(frac=0.2)#抽20%来测试
    print (Precision(df_test,W))#准确率
    print (Recall(df_test,W))#准确率

    end = time.clock()    
    print('finish all in %s' % str(end - start))
```



要结合实际业务定义相关参数，以及采集样本（兴趣度或偏好）。

基于物品的协同过滤实现类似。



