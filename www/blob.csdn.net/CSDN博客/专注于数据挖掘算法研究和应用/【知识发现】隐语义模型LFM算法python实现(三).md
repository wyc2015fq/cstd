# 【知识发现】隐语义模型LFM算法python实现(三) - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年10月17日 17:04:26[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1419
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









http://blog.csdn.net/fjssharpsword/article/details/78257126

基于上篇再优化。

1、回顾LFM原理，可以更好地理解代码



对于一个给定的用户行为数据集（数据集包含的是所有的user, 所有的item，以及每个user有过行为的item列表），使用LFM对其建模后，可得到如下图所示的模型：（假设数据集中有3个user, 4个item, LFM建模的分类数为4）
![](https://img-blog.csdn.net/20171017170830711?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



R矩阵是user-item矩阵，矩阵值Rij表示的是user i 对item j的兴趣度。对于一个user来说，当计算出其对所有item的兴趣度后，就可以进行排序并作出推荐。

LFM算法从数据集中抽取出若干主题，作为user和item之间连接的桥梁，将R矩阵表示为P矩阵和Q矩阵相乘。其中P矩阵是user-class矩阵，矩阵值Pij表示的是user i对class j的兴趣度；Q矩阵式class-item矩阵，矩阵值Qij表示的是item j在class i中的权重，权重越高越能作为该类的代表。所以LFM根据如下公式来计算用户U对物品I的兴趣度：
![](https://img-blog.csdn.net/20171017170851651?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


使用LFM后， 不需要关心分类的角度，结果都是基于用户行为统计自动聚类的，全凭数据自己说了算：

l 不需要关心分类粒度的问题，通过设置LFM的最终分类数就可控制粒度，分类数越大，粒度约细

l 对于一个item，并不是明确的划分到某一类，而是计算其属于每一类的概率，是一种标准的软分类

l 对于一个user，我们可以得到他对于每一类的兴趣度，而不是只关心可见列表中的那几个类。

l 对于每一个class，我们可以得到类中每个item的权重，越能代表这个类的item，权重越高

现在讨论如何计算矩阵P和矩阵Q中参数值。一般做法就是最优化损失函数来求参数。在定义损失函数之前，先对数据集和兴趣度的取值做一说明。

数据集应该包含所有的user和他们有过行为的（也就是喜欢）的item。所有的这些item构成了一个item全集。对于每个user来说，我们把他有过行为的item称为正样本，规定兴趣度Rui=1，此外我们还需要从item全集中随机抽样，选取与正样本数量相当的样本作为负样本，规定兴趣度为Rui=0。因此，兴趣的取值范围为[0,1]。

采样之后原有的数据集得到扩充，得到一个新的user-item集K={(U,I)}，其中如果(U,I)是正样本，则Rui=1，否则Rui=0。损失函数如下所示：
![](https://img-blog.csdn.net/20171017170913221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


上式中的![](https://img-blog.csdn.net/20171017170931935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

是用来防止过拟合的正则化项，λ需要根据具体应用场景反复实验得到。损失函数的优化使用随机梯度下降算法：

通过求参数Puk和Qki的偏导确定最快的下降方向；
![](https://img-blog.csdn.net/20171017170954068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



迭代计算不断优化参数（迭代次数事先人为设置），直到参数收敛。
![](https://img-blog.csdn.net/20171017171011790?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


其中，α是学习速率，α越大，迭代下降的越快。α和λ一样，也需要根据实际的应用场景反复实验得到。在ratings数据集上进行实验，取分类数F=100，α=0.02，λ=0.01。

综上所述，执行LFM需要：根据数据集初始化P和Q矩阵，确定4个参数：隐类数F、迭代次数N、学习速率α、正则化参数λ。

2、基于原理，代码构建如下：






```python
# -*- coding: utf-8 -*-
'''
Created on 2017年10月16日

@author: Administrator
'''
'''
实现隐语义模型，对隐式数据进行推荐
1.对正样本生成负样本
  -负样本数量相当于正样本
  -物品越热门，越有可能成为负样本
2.使用随机梯度下降法，更新参数
'''
import numpy as np
import pandas as pd
from math import exp
import time
import math
from sklearn import cross_validation
import random
import operator

class LFM:
    
    '''
            初始化隐语义模型
            参数：*data 训练数据,要求为pandas的dataframe
       *F  隐特征的个数      *N  迭代次数        *alpha 随机梯度下降的学习速率  
       *lamda 正则化参数  *ratio 负样本/正样本比例  *topk 推荐的前k个物品
    '''
    def __init__(self,data,ratio,F=5,N=2,alpha=0.02,lamda=0.01,topk=10):
        self.data=data #样本集
        self.ratio =ratio #正负样例比率，对性能最大影响
        self.F = F#隐类数量，对性能有影响
        self.N = N#迭代次数，收敛的最佳迭代次数未知
        self.alpha =alpha#梯度下降步长
        self.lamda = lamda#正则化参数
        self.topk =topk #推荐top k项
    
    '''
                初始化物品池，物品池中物品出现的次数与其流行度成正比
    {item1:次数,item2:次数,...}
    '''    
    def InitItemPool(self):
        itemPool=dict()
        groups = self.data.groupby([1])
        for item,group in groups:
            itemPool.setdefault(item,0)
            itemPool[item] =group.shape[0]
        itemPool=dict(sorted(itemPool.items(), key = lambda x:x[1], reverse = True))
        return itemPool
    '''
                获取每个用户对应的商品（用户购买过的商品）列表，如
    {用户1:[商品A，商品B，商品C],
                 用户2:[商品D，商品E，商品F]...}
    ''' 
    def user_item(self):
        ui = dict()
        groups = self.data.groupby([0])
        for item,group in groups:
            ui[item]=set(group.ix[:,1])
        return ui
    
    '''
                    初始化隐特征对应的参数
      numpy的array存储参数，使用dict存储每个用户（物品）对应的列
    '''
    def initParam(self):
        users=set(self.data.ix[:,0])
        items=set(self.data.ix[:,1])
        
        arrayp = np.random.rand(len(users), self.F) #构造p矩阵，[0,1]内随机值
        arrayq = np.random.rand(self.F, len(items)) #构造q矩阵，[0,1]内随机值
        P = pd.DataFrame(arrayp, columns=range(0, self.F), index=users)
        Q = pd.DataFrame(arrayq, columns=items, index=range(0,self.F))
        return P,Q
        
        '''
        self.Pdict=dict()
        self.Qdict=dict()
        for user in users:
            self.Pdict[user]=len(self.Pdict)
        
        for item in items:
            self.Qdict[item]=len(self.Qdict)
        
        self.P=np.random.rand(self.F,len(users))
        self.Q=np.random.rand(self.F,len(items))
        '''
    '''
        生成负样本
    '''
    def RandSelectNegativeSamples(self,items):
        ret=dict()
        for item in items:
            #所有正样本评分为1
            ret[item]=1
        #负样本个数，四舍五入
        negtiveNum = int(round(len(items)*self.ratio))
        
        N = 0
        #while N<negtiveNum:
            #item = self.itemPool[random.randint(0, len(self.itemPool) - 1)]
        for item,count in self.itemPool.items():
            if N>negtiveNum: 
                break
            if item in items:
                #如果在用户已经喜欢的物品列表中，继续选
                continue
            N+=1
            #负样本评分为0
            ret[item]=0
        return ret
    
    def sigmod(self,x):
        # 单位阶跃函数,将兴趣度限定在[0,1]范围内
        y = 1.0/(1+exp(-x))
        return y
    
    def lfmPredict(self,p, q, userID, itemID):
        #利用参数p,q预测目标用户对目标物品的兴趣度
        p = np.mat(p.ix[userID].values)
        q = np.mat(q[itemID].values).T
        r = (p * q).sum()
        r = self.sigmod(r)
        return r
    '''
            使用随机梯度下降法，更新参数
    '''
    def stochasticGradientDecent(self,p,q):
        alpha=self.alpha
        for i in range(self.N):
            for user,items in self.ui.items():
                ret=self.RandSelectNegativeSamples(items)
                for item,rui in ret.items():
                    eui = rui - self.lfmPredict(p,q, user, item)                  
                    for f in range(0, self.F):
                        #df[列][行]定位
                        tmp= alpha * (eui * q[item][f] - self.lamda * p[f][user])
                        q[item][f] += alpha * (eui * p[f][user] - self.lamda * q[item][f])
                        p[f][user] +=tmp
                    '''                  
                    p=self.P[:,self.Pdict[user]]
                    q=self.Q[:,self.Qdict[item]]
                    eui=rui-sum(p*q)
                    tmp=p+alpha*(eui*q-self.lamda*p)
                    self.Q[:,self.Qdict[item]]+=alpha*(eui*p-self.lamda*q)
                    self.P[:,self.Pdict[user]]=tmp 
                    '''                 
            alpha*=0.9
        return p,q
            
    def Train(self):
        self.itemPool=self.InitItemPool()#生成物品的热门度排行
        self.ui = self.user_item()#生成用户-物品
        p,q=self.initParam()#生成p,q矩阵 
        self.P,self.Q=self.stochasticGradientDecent(p,q)  #随机梯度下降训练
         
    def Recommend(self,user):
        items=self.ui[user]
        predictList = [self.lfmPredict(self.P, self.Q, user, item) for item in items]
        series = pd.Series(predictList, index=items)
        series = series.sort_values(ascending=False)[:self.topk]
        return series
        '''
        #items=self.ui[user]
        p=self.P[:,self.Pdict[user]]
        
        rank = dict()
        for item,id in self.Qdict.items():
            #if item in items:
            #    continue
            q=self.Q[:,id];
            rank[item]=sum(p*q)
        #return sorted(rank.items(),lambda x,y:operator.gt(x[0],y[0]),reverse=True)[0:self.topk-1];
        return sorted(rank.items(),key=operator.itemgetter(1),reverse=True)[0:self.topk-1];
        '''
        
    def recallAndPrecision(self,test):#召回率和准确率
        userID=set(test.ix[:,0])
        hit = 0
        recall = 0
        precision = 0
        for userid in userID:
            #trueItem = test[test.ix[:,0] == userid]
            #trueItem= trueItem.ix[:,1]
            trueItem=self.ui[userid]
            preitem=self.Recommend(userid)
            for item in list(preitem.index):
                if item in trueItem:
                    hit += 1
            recall += len(trueItem)
            precision += len(preitem)
        return (hit / (recall * 1.0),hit / (precision * 1.0))
    
    def coverage(self,test):#覆盖率
        userID=set(test.ix[:,0])
        recommend_items = set()
        all_items = set()
        for userid in userID:
            #trueItem = test[test.ix[:,0] == userid]
            #trueItem= trueItem.ix[:,1]
            trueItem=self.ui[userid]
            for item in trueItem:
                all_items.add(item)
            preitem=self.Recommend(userid)
            for item in list(preitem.index):
                recommend_items.add(item)
        return len(recommend_items) / (len(all_items) * 1.0)
    
    def popularity(self,test):#流行度
        userID=set(test.ix[:,0])
        ret = 0
        n = 0
        for userid in userID:
            preitem=self.Recommend(userid)
            for item in list(preitem.index):
                ret += math.log(1+self.itemPool[item])
                n += 1
        return ret / (n * 1.0)
 
if __name__ == "__main__":   
    start = time.clock()  
    
    #导入数据 
    data=pd.read_csv('D:\\dev\\workspace\\PyRecSys\\demo\\ratings.csv',nrows=10000,header=None)
    data=data.drop(0)
    data=data.ix[:,0:1]
    
    train,test=cross_validation.train_test_split(data,test_size=0.2)
    train = pd.DataFrame(train)
    test = pd.DataFrame(test)
    print ("%3s%20s%20s%20s%20s" % ('ratio',"recall",'precision','coverage','popularity'))
    for ratio in [1,2,3,5]:
        lfm = LFM(data,ratio)
        lfm.Train()
        #rank=lfm.Recommend('1')
        #print (rank)
        recall,precision = lfm.recallAndPrecision(test)
        coverage =lfm.coverage(test)
        popularity =lfm.popularity(test)
        print ("%3d%19.3f%%%19.3f%%%19.3f%%%20.3f" % (ratio,recall * 100,precision * 100,coverage * 100,popularity))

    end = time.clock()    
    print('finish all in %s' % str(end - start))
```


执行后的指标：主要观察ratio比例对结果的影响



```
ratio              recall           precision            coverage          popularity
  1              7.001%            100.000%             12.976%               2.003
  2              7.001%            100.000%              9.663%               2.412
  3              7.001%            100.000%              7.869%               2.627
  5              7.001%            100.000%              5.356%               2.809
finish all in 702.2413190975064
```

















