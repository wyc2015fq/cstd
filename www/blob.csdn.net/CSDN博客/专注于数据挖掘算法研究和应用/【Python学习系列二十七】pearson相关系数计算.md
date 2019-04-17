# 【Python学习系列二十七】pearson相关系数计算 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月27日 10:33:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4468
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









场景：计算训练特征和目标之间的相关系数，用于判断是否加入训练。

参考代码：



```python
# -*- coding: utf-8 -*-

import pandas as pd
import time
from sklearn import tree
import numpy as np
from sklearn import metrics  
from sklearn.linear_model import LinearRegression
from scipy.stats import pearsonr
from sklearn.feature_selection import SelectKBest
from sklearn.feature_selection import f_regression

def main():
    #加载标记数据
    label_ds=pd.read_csv(r"link_train_0726.txt",sep='\t',encoding='utf8',\
                         names=['link_id','length','width','link_class','start_date','week','time_interval','time_slot','travel_time',\
                                'avg_travel_time','sd_travel_time','inlinks_num','outlinks_num']) 
    label_ds["link_id"] = label_ds["link_id"].astype("string")
    label_ds["length"] = label_ds["length"].astype("int")
    label_ds["width"] = label_ds["width"].astype("int")
    label_ds["link_class"] = label_ds["link_class"].astype("int")
    label_ds["start_date"] = label_ds["start_date"].astype("string")
    label_ds["week"] = label_ds["week"].astype("int")
    label_ds["time_interval"] = label_ds["time_interval"].astype("string")
    label_ds["time_slot"] = label_ds["time_slot"].astype("int")
    label_ds["travel_time"] = label_ds["travel_time"].astype("float")
    label_ds["avg_travel_time"] = label_ds["avg_travel_time"].astype("float")
    label_ds["sd_travel_time"] = label_ds["sd_travel_time"].astype("float")
    label_ds["inlinks_num"] = label_ds["inlinks_num"].astype("int")
    label_ds["outlinks_num"] = label_ds["outlinks_num"].astype("int")
    #加载预测数据   
    unlabel_ds=pd.read_csv(r"link_test_0726.txt",sep='\t',encoding='utf8',\
                         names=['link_id','length','width','link_class','start_date','week','time_interval','time_slot',\
                                'avg_travel_time','sd_travel_time','inlinks_num','outlinks_num']) 
    unlabel_ds["link_id"] = unlabel_ds["link_id"].astype("string")
    unlabel_ds["length"] = unlabel_ds["length"].astype("int")
    unlabel_ds["width"] = unlabel_ds["width"].astype("int")
    unlabel_ds["link_class"] = unlabel_ds["link_class"].astype("int")
    unlabel_ds["start_date"] = unlabel_ds["start_date"].astype("string")
    unlabel_ds["week"] = unlabel_ds["week"].astype("int")
    unlabel_ds["time_interval"] = unlabel_ds["time_interval"].astype("string")
    unlabel_ds["time_slot"] = unlabel_ds["time_slot"].astype("int")
    unlabel_ds["avg_travel_time"] = unlabel_ds["avg_travel_time"].astype("float")
    unlabel_ds["sd_travel_time"] = unlabel_ds["sd_travel_time"].astype("float")
    unlabel_ds["inlinks_num"] = unlabel_ds["inlinks_num"].astype("int")
    unlabel_ds["outlinks_num"] = unlabel_ds["outlinks_num"].astype("int")
    
    #提取训练集、验证集、测试集
    train_df=label_ds.loc[(pd.to_datetime(label_ds["start_date"])<'2016-06-01')]#训练集
    print "训练集，有", train_df.shape[0], "行", train_df.shape[1], "列"
    valid_df=label_ds.loc[(pd.to_datetime(label_ds["start_date"])>='2016-06-01')]#验证集train_df.sample(frac=0.2)
    print "验证集，有", valid_df.shape[0], "行", valid_df.shape[1], "列"
    test_df=unlabel_ds#测试集
    print "测试集，有", test_df.shape[0], "行", test_df.shape[1], "列"
    #特征选择
    p_X=train_df['outlinks_num']#训练属性
    p_Y=train_df['travel_time']#目标属性
    p_value=pearsonr(p_X,p_Y)
    print p_value
    #选择相关性最强的k个特征参与训练
    #k_feature = f_regression(p_X,p_Y)
    #k_fearture=SelectKBest(lambda X, Y: np.array(map(lambda x:pearsonr(x, Y), X.T)).T, k=9).fit_transform(p_X, p_Y)
    #print k_fearture
    #模型训练
    train_X=train_df[['length','width','link_class','week','time_slot','avg_travel_time']]
    train_y = train_df['travel_time']#标记
    model_lr=LinearRegression()#tree.DecisionTreeRegressor()
    model_lr.fit(train_X, train_y)   
    #模型验证
    valid_X=valid_df[['length','width','link_class','week','time_slot','avg_travel_time']]
    valid_y=valid_df['travel_time']
    pre_valid_y=model_lr.predict(valid_X)
    abs_y=abs(pre_valid_y-valid_y)
    abs_error=abs_y.sum()#求和
    #abs_error=sum(list(abs_y))#求和
    print "mape:",abs_error/valid_df.shape[0]
    print "RMSE:",np.sqrt(metrics.mean_squared_error(valid_y, pre_valid_y))#均方差，模型评估
    #模型预测
    test_X = test_df[['length','width','link_class','week','time_slot','avg_travel_time']]  
    test_info = test_df[['link_id','start_date','time_interval']] 
    test_X=test_X.fillna(0)#空值替换为0
    test_y=model_lr.predict(test_X) 
    pre_test_y=pd.DataFrame(test_y,columns=['travel_time']) 
    outset=test_info.join(pre_test_y,how='left')#输出结果 
    #outset["travel_time"]=outset["travel_time"].apply(lambda x: '{0:.3f}'.format(x))
    outset.to_csv('outit.txt',sep='#',index=False,header=None)#输出预测数据 
    
#执行
if __name__ == '__main__':  
    start = time.clock()  
    main()
    end = time.clock()  
    print('finish all in %s' % str(end - start))
```


pearsonx函数的说明：https://docs.scipy.org/doc/scipy/reference/generated/scipy.stats.pearsonr.html

scikit-learn库中：f_regression和SelectKBest用于选择最佳特征训练，可以批量给出前k个特征。



