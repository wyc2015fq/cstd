# 【知识发现】天池平台新浪微博互动预测-ItemCF推荐方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年11月01日 12:46:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2179








1、比赛内容：

https://tianchi.aliyun.com/getStart/introduction.htm?spm=5176.100066.0.0.62a0c916DZRdDr&raceId=231574




2、解题思路：将转发、评论、赞次数作为物品推荐给博文（预先用结巴分词抽取关键词作为博文标签）




3、参考代码：

1）主程序



```python
# -*- coding: utf-8 -*-
'''
Created on 2017年10月31日

@author: Jason.F
'''
import time
import pandas as pd
from sklearn import cross_validation
import jieba
import jieba.analyse
from ItemCF import ItemCF 

class cWeibo:
    
    def __init__(self,path):
        self.path=path

    def importData(self):
        path=self.path
        #导入样本集
        data=pd.read_csv(path+'\\weibo_train_data.txt',encoding='utf8',sep='\t',names=['luid','mid','time','fcs','ccs','lcs','cont'])#nrows=1000
        data['fcs']=data['fcs'].astype('int')#博文发表一周后的转发数，权重0.5
        data['ccs']=data['ccs'].astype('int')#博文发表一周后的评论数，权重0.25
        data['lcs']=data['lcs'].astype('int')#博文发表一周后的点赞数，权重0.25
        train,test=cross_validation.train_test_split(data,test_size=0.2)
        self.traindata = pd.DataFrame(data)#全量训练
        self.testdata = pd.DataFrame(test)#测试集
        print '训练集，有：', self.traindata.shape[0], '行', self.traindata.shape[1], '列'
        print '测试集，有：', self.testdata.shape[0], '行', self.testdata.shape[1], '列'
        #导入预测集
        data=pd.read_csv(path+'\\weibo_predict_data.txt',encoding='utf8',sep='\t',names=['luid','mid','time','cont'])#nrows=100
        self.predata=data #预测集
        print '预测集，有：', self.predata.shape[0], '行', self.predata.shape[1], '列'
    
    def ETL(self):
        '''
        #uid映射为数字编号
        ut_train=set(self.traindata.ix[:,0])
        ut_pred=set(self.predata.ix[:,0])
        ut=list(ut_train.symmetric_difference(ut_pred))#取并集并去重
        df_ut=pd.DataFrame(ut,columns=['luid'])
        df_ut['uid']=df_ut.index
        self.traindata=pd.merge(self.traindata,df_ut, on=['luid'], how='left')
        self.traindata=self.traindata[['uid','time','fcs','ccs','lcs','cont']]
        self.testdata=pd.merge(self.testdata,df_ut, on=['luid'], how='left')
        self.testdata=self.testdata[['uid','time','fcs','ccs','lcs','cont']]
        self.predata=pd.merge(self.predata,df_ut, on=['luid'], how='left')
        self.predata=self.predata[['luid','mid','uid','time','cont']]
        '''
        #time转换成0-23数字，必要下要考虑特殊节日
        self.traindata['time']=self.traindata.apply(lambda x:(time.strptime(x['time'],"%Y-%m-%d %H:%M:%S")).tm_hour,axis=1)
        self.traindata.rename(columns=lambda x:x.replace('time','tid'), inplace=True)#修改列名为tid
        self.testdata['time']=self.testdata.apply(lambda x:(time.strptime(x['time'],"%Y-%m-%d %H:%M:%S")).tm_hour,axis=1)
        self.testdata.rename(columns=lambda x:x.replace('time','tid'), inplace=True)
        self.predata['time']=self.predata.apply(lambda x:(time.strptime(x['time'],"%Y-%m-%d %H:%M:%S")).tm_hour,axis=1)
        self.predata.rename(columns=lambda x:x.replace('time','tid'), inplace=True)    
        #cont分词，文本内容要考虑带@和红包的特殊意义词
        #jieba.suggest_freq('@', True)
        self.traindata['cont']=self.traindata.apply(lambda x:",".join(jieba.analyse.extract_tags(x['cont'],topK=50,\
                                    allowPOS=('n','nr','ns','nt','nz','a','ad','an','f','s','i','t','v','vd','vn'))),axis=1)
        self.traindata=self.traindata.drop('cont', axis=1).join(self.traindata['cont'].str.split(',', expand=True).stack().reset_index(level=1, drop=True).rename('tag'))
        self.testdata['cont']=self.testdata.apply(lambda x:",".join(jieba.analyse.extract_tags(x['cont'],topK=50,\
                                    allowPOS=('n','nr','ns','nt','nz','a','ad','an','f','s','i','t','v','vd','vn'))),axis=1)
        self.testdata=self.testdata.drop('cont', axis=1).join(self.testdata['cont'].str.split(',', expand=True).stack().reset_index(level=1, drop=True).rename('tag'))
        self.predata['cont']=self.predata.apply(lambda x:",".join(jieba.analyse.extract_tags(x['cont'],topK=50,\
                                    allowPOS=('n','nr','ns','nt','nz','a','ad','an','f','s','i','t','v','vd','vn'))),axis=1)
        self.predata=self.predata.drop('cont', axis=1).join(self.predata['cont'].str.split(',', expand=True).stack().reset_index(level=1, drop=True).rename('tag'))
        #生成标签表
        ft_train=set(self.traindata.ix[:,6])
        ft_pred=set(self.predata.ix[:,3])
        ft=list(ft_train.symmetric_difference(ft_pred))#取并集并去重
        df_ft=pd.DataFrame(ft,columns=['tag'])
        df_ft['fid']=df_ft.index
        self.traindata=pd.merge(self.traindata,df_ft, on=['tag'], how='left')
        self.traindata=self.traindata[['luid','mid','tid','fid','fcs','ccs','lcs']]
        self.traindata=self.traindata.dropna(axis=0,how='any')  
        self.traindata['fid']=self.traindata['fid'].astype('int')      
        self.testdata=pd.merge(self.testdata,df_ft, on=['tag'], how='left')
        self.testdata=self.testdata[['luid','mid','tid','fid','fcs','ccs','lcs']]
        self.testdata=self.testdata.dropna(axis=0,how='any') 
        self.testdata['fid']=self.testdata['fid'].astype('int')          
        self.predata=pd.merge(self.predata,df_ft, on=['tag'], how='left')
        self.predata=self.predata[['luid','mid','tid','fid']]
        self.predata=self.predata.dropna(axis=0,how='any')
        self.predata['fid']=self.predata['fid'].astype('int')
    
    def callItemCF(self):
        data=self.traindata 
        #data['u']=data.apply(lambda x:str(x['uid'])+':'+x['tid']+':'+str(x['fid']),axis=1)
        data_f=data[['fid','fcs','tid']]
        data_c=data[['fid','ccs','tid']]
        data_l=data[['fid','lcs','tid']]
        #训练转发次数推荐模型
        ic_f = ItemCF(data_f)
        ic_f.user_item()#转化成dict，并生成训练集和测试集
        ic_f.ItemSimilarity()#生成物品相似度矩阵
        self.ic_f=ic_f
        #训练评论次数推荐模型
        ic_c = ItemCF(data_c)
        ic_c.user_item()#转化成dict，并生成训练集和测试集
        ic_c.ItemSimilarity()#生成物品相似度矩阵
        self.ic_c=ic_c
        #训练点赞次数推荐模型
        ic_l = ItemCF(data_l)
        ic_l.user_item()#转化成dict，并生成训练集和测试集
        ic_l.ItemSimilarity()#生成物品相似度矩阵
        self.ic_l=ic_l
        
        test=self.testdata
        test_f=test[['mid','fid','tid','fcs']]
        test_c=test[['mid','fid','tid','ccs']]
        test_l=test[['mid','fid','tid','lcs']]
        test_f['pfcs']=test_f.apply(lambda x: ((ic_f.recommend(x['fid'])).keys())[0] if len((ic_f.recommend(x['fid'])).keys()) else 0,axis=1)
        test_c['pccs']=test_c.apply(lambda x: ((ic_c.recommend(x['fid'])).keys())[0] if len((ic_c.recommend(x['fid'])).keys()) else 0,axis=1)
        test_l['plcs']=test_l.apply(lambda x: ((ic_l.recommend(x['fid'])).keys())[0] if len((ic_l.recommend(x['fid'])).keys()) else 0,axis=1)
        #计算准确率
        precision=self.precision (test_f[['mid','fcs','pfcs']],test_c[['mid','ccs','pccs']],test_l[['mid','lcs','plcs']])
        print (precision)
    
    def precision(self,test_f,test_c,test_l):#转发、评论、点赞次数预测偏差
        test_f=test_f.groupby('mid').mean()
        test_c=test_c.groupby('mid').mean()
        test_l=test_l.groupby('mid').mean()
        test_f['dev_f']=test_f.apply(lambda x:abs(x['fcs']-int(x['pfcs']))/(x['fcs']+5),axis=1)
        test_c['dev_c']=test_c.apply(lambda x:abs(x['ccs']-int(x['pccs']))/(x['ccs']+3),axis=1)
        test_l['dev_l']=test_l.apply(lambda x:abs(x['lcs']-int(x['plcs']))/(x['lcs']+3),axis=1)
        #print(test_f.columns.tolist)
        test=test_f
        test=pd.merge(test,test_c,left_index=True,right_index=True)#以索引连接
        test=pd.merge(test,test_l,left_index=True,right_index=True)
        test['prec']=test.apply(lambda x:(1-0.5*x['dev_f']-0.25*x['dev_c']-0.25*x['dev_l']),axis=1)
        test['count']=test.apply(lambda x: 100 if (x['pfcs']+x['pccs']+x['plcs']>100) else (x['pfcs']+x['pccs']+x['plcs']),axis=1)
        test['sgn']=test.apply(lambda x:1 if (x['prec']-0.8) >0 else 0,axis=1)
        test['on']=test.apply(lambda x:(x['count']+1)*x['sgn'],axis=1)
        test['down']=test.apply(lambda x:(x['count']+1),axis=1)
        prec_df=test[['on','down']]
        #prec_df.loc['Row_sum'] = prec_df.apply(lambda x: x.sum())
        #on=prec_df.get_value(index='Row_num',col='on',takeable=False)
        on=prec_df['on'].sum()
        down=prec_df['down'].sum()
        return (on/down)
           
    def predict(self):
        '''
        文件命名：weibo_result_data.txt
        uid、mid、forward_count字段以tab键分隔
        forward_count、comment_count、like_count字段间以逗号分隔
        '''
        predata=self.predata[['luid','mid','tid','fid']]
        predata['pfcs']=predata.apply(lambda x:((self.ic_f.recommend(x['fid'])).keys())[0] if len((self.ic_f.recommend(x['fid'])).keys()) else 0,axis=1)
        predata['pccs']=predata.apply(lambda x:((self.ic_c.recommend(x['fid'])).keys())[0] if len((self.ic_c.recommend(x['fid'])).keys()) else 0,axis=1)
        predata['plcs']=predata.apply(lambda x:((self.ic_l.recommend(x['fid'])).keys())[0] if len((self.ic_l.recommend(x['fid'])).keys()) else 0,axis=1)
        print (predata)


if __name__ == "__main__":   
    start = time.clock()  
    
    wb=cWeibo('D:\\CVTE\\weibo')
    wb.importData()#导入数据
    wb.ETL()#特征抽取
    wb.callItemCF()#推荐算法
    wb.predict()
    
    end = time.clock()    
    print ('finish all in %s' % str(end - start))
```


2）基于时间上下文的ItemCF推荐算法：time设置为1，可以自己处理后传入



```python
# -*- coding: utf-8 -*-
'''
Created on 2017年10月31日
基于时间上下文的物品协同过滤推荐算法
@author: Administrator
'''

import math
import numpy as np
import time

class ItemCF:
    def __init__(self,data):
        self.data = data
      
    def user_item(self):#训练集转化成dict
        data=self.data
        self.traindata = {}       
        for user,item,time in  np.array(data).tolist():
            self.traindata.setdefault(user,{})
            self.traindata[user][item] =1#time        
                                                                                 
    def ItemSimilarity(self,alpha=1):
        train=self.traindata
        #calculate co-rated users between items
        C=dict()
        N=dict()
        for u,items in train.items():
            for i,tui in items.items():
                N.setdefault(i,0)
                N[i]+=1
                for j,tuj in items.items():
                    C.setdefault(i,{})
                    if i==j:
                        continue
                    C[i].setdefault(j,0)
                    C[i][j]+=1/(1+alpha*abs(tui-tuj))
        #calculate finial similarity matrix W
        W=dict()
        for i,related_items in C.items():
            W.setdefault(i,{})
            for j, cij in related_items.items():
                W[i].setdefault(j,0)
                W[i][j]=cij/math.sqrt(N[i]*N[j]*1.0)
        self.itemSim=W 
                              
    def recommend(self,user,tid=1,k=1,beta=1):
        W=self.itemSim
        train=self.traindata
        rank=dict()
        ru=train.get(user,{})
        for i,tui in ru.items():
            for j,wj in W[i].items():     
                if j in ru.keys():
                    continue
                rank.setdefault(j,0)
                rank[j]+=wj/(1+beta*abs(tid-tui))
        return dict(sorted(rank.items(), key = lambda x:x[1], reverse = True)[0:k])
                                                           
if __name__ == "__main__":   
    start = time.clock()  
        
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```


预测准确率比较低，主要是：

1）博文标签通过结巴分词标签化后，博文之间的关联性降低了，考虑用textrank或lsa方法；

2）对时间元素如何在推荐算法中体现需要进一步考虑；

3）用户在这里没有参与模型，这个缺失比较严重。

重点是研发基于时间上下文的ItemCF算法，其他思路待考虑。



