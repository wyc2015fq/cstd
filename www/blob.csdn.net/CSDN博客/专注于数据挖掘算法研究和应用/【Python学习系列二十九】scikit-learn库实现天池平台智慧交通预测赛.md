# 【Python学习系列二十九】scikit-learn库实现天池平台智慧交通预测赛 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年08月14日 08:40:42[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1814
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)










1、背景：https://tianchi.aliyun.com/competition/introduction.htm?spm=5176.100066.0.0.3f6e7d83RQgWEL&raceId=231598


                分数是0.58比较弱，代码这里参考。


2、通过比赛提取的特征如下：


|特征|类型|说明|
|----|----|----|
|link_ID|string|每条路段(link)唯一标识|
|link_seq|int|132条路段从1-132编号；|
|length|int|link长度(米)|
|width|int|link宽度(米)|
|link_class|int|link道路等级，如1代表主干道|
|date|string|日期，如2015-10-01|
|week|int|星期，根据日期映射到星期，从1到7|
|time_interval|string|时间段，如[2015-09-01 00:00:00,2015-09-01 00:02:00)|
|time_slot|int|时间片，根据时间段映射一天24小时，从1到720，每段2分|
|avg_travel_time|float|该时间片平均旅行时间的均值，反应集中趋势|
|inlinks_atl_1|float|车辆在该路段上（timeslot-上游平均旅行时间）时间段的平均旅行时间，上游最多4个路段汇入，如果小于4，则大于4的为0。测试集中，该值是通过决策树回归预测出来的。|
|inlinks_atl_2|float| |
|inlinks_atl_3|float| |
|inlinks_atl_4|float| |
|inlinks_atl_1|float|车辆在该路段上（timeslot+平均旅行时间）时间段的平均旅行时间，下游最多4个路段汇出，如果小于4，则大于4的为0。测试集中，该值是通过决策树回归预测出来的。|
|inlinks_atl_2|float| |
|inlinks_atl_3|float| |
|inlinks_atl_4|float| |
|travel_time|float|车辆在该路段上的平均旅行时间(秒)|

3、代码参考：



```python
# -*- coding: utf-8 -*-

import pandas as pd
import time
import numpy as np
from sklearn import metrics
from sklearn import tree
from sklearn.linear_model import LinearRegression

def main():
    #加载标记数据
    label_ds=pd.read_csv(r"link_train_0801.txt",sep='\t',encoding='utf8',\
                         names=['link_id','link_seq','length','width','link_class','start_date','week','time_interval','time_slot','travel_time',\
                                'avg_travel_time','sd_travel_time','inlinks_num','outlinks_num','inlinks_avg_travel_time','outlinks_avg_travel_time',\
                                'inlinks_atl_1','inlinks_atl_2','inlinks_atl_3','inlinks_atl_4','outlinks_atl_1','outlinks_atl_2','outlinks_atl_3','outlinks_atl_4']) 
    label_ds["link_id"] = label_ds["link_id"].astype("string")
    label_ds["link_seq"] = label_ds["link_seq"].astype("int")
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
    label_ds["inlinks_avg_travel_time"] = label_ds["inlinks_avg_travel_time"].astype("float")
    label_ds["outlinks_avg_travel_time"] = label_ds["outlinks_avg_travel_time"].astype("float")
    label_ds["inlinks_atl_1"] = label_ds["inlinks_atl_1"].astype("float")
    label_ds["inlinks_atl_2"] = label_ds["inlinks_atl_2"].astype("float")
    label_ds["inlinks_atl_3"] = label_ds["inlinks_atl_3"].astype("float")
    label_ds["inlinks_atl_4"] = label_ds["inlinks_atl_4"].astype("float")
    label_ds["outlinks_atl_1"] = label_ds["outlinks_atl_1"].astype("float")
    label_ds["outlinks_atl_2"] = label_ds["outlinks_atl_2"].astype("float")
    label_ds["outlinks_atl_3"] = label_ds["outlinks_atl_3"].astype("float")
    label_ds["outlinks_atl_4"] = label_ds["outlinks_atl_4"].astype("float")
    #加载预测数据   
    unlabel_ds=pd.read_csv(r"link_test_0801.txt",sep='\t',encoding='utf8',\
                         names=['link_id','link_seq','length','width','link_class','start_date','week','time_interval','time_slot',\
                                'avg_travel_time','sd_travel_time','inlinks_num','outlinks_num','inlinks_avg_travel_time','outlinks_avg_travel_time',\
                                'inlinks_atl_1','inlinks_atl_2','inlinks_atl_3','inlinks_atl_4','outlinks_atl_1','outlinks_atl_2','outlinks_atl_3','outlinks_atl_4']) 
    unlabel_ds["link_id"] = unlabel_ds["link_id"].astype("string")
    unlabel_ds["link_seq"] = unlabel_ds["link_seq"].astype("int")
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
    unlabel_ds["inlinks_avg_travel_time"] = unlabel_ds["inlinks_avg_travel_time"].astype("float")
    unlabel_ds["outlinks_avg_travel_time"] = unlabel_ds["outlinks_avg_travel_time"].astype("float")
    unlabel_ds["inlinks_atl_1"] = unlabel_ds["inlinks_atl_1"].astype("float")
    unlabel_ds["inlinks_atl_2"] = unlabel_ds["inlinks_atl_2"].astype("float")
    unlabel_ds["inlinks_atl_3"] = unlabel_ds["inlinks_atl_3"].astype("float")
    unlabel_ds["inlinks_atl_4"] = unlabel_ds["inlinks_atl_4"].astype("float")
    unlabel_ds["outlinks_atl_1"] = unlabel_ds["outlinks_atl_1"].astype("float")
    unlabel_ds["outlinks_atl_2"] = unlabel_ds["outlinks_atl_2"].astype("float")
    unlabel_ds["outlinks_atl_3"] = unlabel_ds["outlinks_atl_3"].astype("float")
    unlabel_ds["outlinks_atl_4"] = unlabel_ds["outlinks_atl_4"].astype("float")
    
    outit=pd.DataFrame()#输出结果
    mr_df=pd.DataFrame()#输出link的mape和rmse
    mape=0;
    rmse=0;
    train_df=label_ds.loc[(pd.to_datetime(label_ds["start_date"])<'2016-06-01')]#训练集
    valid_df=label_ds.loc[(pd.to_datetime(label_ds["start_date"])>='2016-06-01')]#验证集train_df.sample(frac=0.2) 
    for linkid in range(1,133):
        #提取训练集、验证集、测试集       
        train_df_id=train_df.loc[(train_df["link_seq"]==linkid)]
        print "训练集，有", train_df_id.shape[0], "行", train_df_id.shape[1], "列"        
        valid_df_id=valid_df.loc[(valid_df["link_seq"]==linkid)]   
        print "验证集，有", valid_df_id.shape[0], "行", valid_df_id.shape[1], "列"
        test_df=unlabel_ds.loc[(unlabel_ds["link_seq"]==linkid)]#测试集
        print "测试集，有", test_df.shape[0], "行", test_df.shape[1], "列"
        #特征选择
        #模型训练
        train_X=train_df_id[['link_seq','time_slot','length','avg_travel_time',\
                             'inlinks_atl_1','inlinks_atl_2','inlinks_atl_3','inlinks_atl_4','outlinks_atl_1','outlinks_atl_2','outlinks_atl_3','outlinks_atl_4']]
        train_X=train_X.fillna(0)#空值替换为0
        train_y = train_df_id['travel_time']#标记
        model_it=LinearRegression()#tree.DecisionTreeRegressor()
        model_it.fit(train_X, train_y)   
        #模型验证
        valid_X=valid_df_id[['link_seq','time_slot','length','avg_travel_time',\
                             'inlinks_atl_1','inlinks_atl_2','inlinks_atl_3','inlinks_atl_4','outlinks_atl_1','outlinks_atl_2','outlinks_atl_3','outlinks_atl_4']]
        valid_X=valid_X.fillna(0)#空值替换为0
        valid_y=valid_df_id['travel_time']
        pre_valid_y=model_it.predict(valid_X)
        abs_y=abs(pre_valid_y-valid_y)
        abs_error=abs_y.sum()#求和
        mape_id=abs_error/valid_df_id.shape[0]
        rmse_id=np.sqrt(metrics.mean_squared_error(valid_y, pre_valid_y))#均方差，模型评估
        print "linkseq="+str(linkid)+"的mape=",mape_id
        print "linkseq="+str(linkid)+"的RMSE=",rmse_id
        mr_list=[[linkid,mape_id,rmse_id]]
        mr_df=mr_df.append(mr_list)
        mape=mape+mape_id
        rmse=rmse+rmse_id
        #模型预测
        test_X = test_df[['link_seq','time_slot','length','avg_travel_time',\
                          'inlinks_atl_1','inlinks_atl_2','inlinks_atl_3','inlinks_atl_4','outlinks_atl_1','outlinks_atl_2','outlinks_atl_3','outlinks_atl_4']]  
        test_X=test_X.fillna(0)#空值替换为0
        test_info = test_df[['link_id','start_date','time_interval']]
        test_y=model_it.predict(test_X) 
        test_info["travel_time"]=test_y
        outit=outit.append(test_info)#追加到输出结果
    print "all mape:",mape/132
    print "all RMSE:",rmse/132
    mr_df.to_csv('linkmape.txt',sep='#',index=False,header=None)
    outit.to_csv('outit.txt',sep='#',index=False,header=None)#输出预测数据 
    
#执行
if __name__ == '__main__':  
    start = time.clock()  
    main()
    end = time.clock()  
    print('finish all in %s' % str(end - start))
```





