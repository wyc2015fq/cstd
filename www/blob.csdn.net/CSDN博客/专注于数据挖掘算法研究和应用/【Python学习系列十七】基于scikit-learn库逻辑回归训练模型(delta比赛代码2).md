# 【Python学习系列十七】基于scikit-learn库逻辑回归训练模型(delta比赛代码2) - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月16日 10:00:29[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2167
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









机器学习任务流程：学习任务定义->数学建模->训练样本采样->特征分析和抽取->算法设计和代码->模型训练和优化(性能评估和度量)->泛化能力评估(重采样和重建模）；




算法思路：应用半监督学习思路，先用训练集训练出一个模型，然后用模型给预测集打标签，之后将打上标签的预测集也加入到训练集中用模型再训练，用f1-scror作为性能评估的依据。这个代码和之前比，主要是增加model.predict_proba()函数返回正例概率，自己设置阈值来选择正例样本。代码如下：



```python
# -*- coding: utf-8 -*-

import pandas as pd
import time
from sklearn import metrics
from sklearn.linear_model import LogisticRegression
from sklearn import preprocessing
#from sklearn.tree import DecisionTreeClassifier  

def main():
    #省份和地市映射
    data = {"province":['河北省', '山西省', '内蒙古自治区', '辽宁省', '吉林省', '黑龙江省', '江苏省', '浙江省', '安徽省', '福建省', '江西省', '山东省', '河南省', '湖北省', '湖南省', '广东省', '广西壮族自治区', '海南省', '四川省', '贵州省', '云南省', '西藏自治区', '陕西省', '甘肃省', '青海省', '宁夏回族自治区', '新疆维吾尔自治区', '北京市', '天津市', '上海市', '重庆市'],
            "pro_code":[13,14,15,21,22,23,32,33,34,35,36,37,41,42,43,44,45,46,51,52,53,54,61,62,63,64,65,11,12,31,50]}
    province = pd.DataFrame(data, columns = ["province", "pro_code"])
    citydata=pd.read_csv(r"D:\city.csv")#加载地市映射表
    #加载带标记数据
    label_ds=pd.read_csv(r"D:\label.csv")
    label_ds = pd.merge(label_ds, province, how = "left", on = "province")
    label_ds = pd.merge(label_ds, citydata, how = "left", on = "city")
    label_df = pd.DataFrame(label_ds[['denomination','min_amount','pro_code','age','sex','account_age','txn_count','use_nums',\
                                      'txn_min_amount','txn_amount_mean','avg_discount','voucher_num','avg_txn_amt',\
                                      'use_ratio','voucher_ratio','batch_no','voucher_no','city_id','label']])
    label_df["denomination"] = label_df["denomination"].astype("int")
    label_df["min_amount"] = label_df["min_amount"].astype("int")
    label_df["pro_code"] = label_df["pro_code"].astype("int")   
    label_df["age"] = label_df["age"].astype("int")
    label_df["sex"] = label_df["sex"].astype("int")
    label_df["account_age"] = label_df["account_age"].astype("int")
    label_df["txn_count"] = label_df["txn_count"].astype("int")
    label_df["use_nums"] = label_df["use_nums"].astype("int")
    label_df["txn_min_amount"] = label_df["txn_min_amount"].astype("int")
    label_df["txn_amount_mean"] = label_df["txn_amount_mean"].astype("int")
    label_df["avg_discount"] = label_df["avg_discount"].astype("int")
    label_df["voucher_num"] = label_df["voucher_num"].astype("int")
    label_df["avg_txn_amt"] = label_df["avg_txn_amt"].astype("int")
    label_df["use_ratio"] = label_df["use_ratio"].astype("float")
    label_df["voucher_ratio"] = label_df["voucher_ratio"].astype("float")
    label_df["batch_no"] = label_df["batch_no"].astype("int")
    label_df["voucher_no"] = label_df["voucher_no"].astype("str")
    label_df["city_id"] = label_df["city_id"].astype("int")
    label_df["label"] = label_df["label"].astype("int")
    #加载未标记数据
    unlabel_ds=pd.read_csv(r"D:\unlabel.csv")
    unlabel_ds = pd.merge(unlabel_ds, province, how = "left", on = "province")
    unlabel_ds = pd.merge(unlabel_ds, citydata, how = "left", on = "city")
    unlabel_df = pd.DataFrame(unlabel_ds[['denomination','min_amount','pro_code','age','sex','account_age','txn_count','use_nums',\
                                          'txn_min_amount','txn_amount_mean','avg_discount','voucher_num','avg_txn_amt',\
                                          'use_ratio','voucher_ratio','batch_no','city_id','phone','voucher_no']])  
    unlabel_df["denomination"] = unlabel_df["denomination"].astype("int")
    unlabel_df["min_amount"] = unlabel_df["min_amount"].astype("int")    
    unlabel_df["pro_code"] = unlabel_df["pro_code"].astype("int") 
    unlabel_df["age"] = unlabel_df["age"].astype("int")
    unlabel_df["sex"] = unlabel_df["sex"].astype("int")
    unlabel_df["account_age"] = unlabel_df["account_age"].astype("int")
    unlabel_df["txn_count"] = unlabel_df["txn_count"].astype("int")
    unlabel_df["use_nums"] = unlabel_df["use_nums"].astype("int")
    unlabel_df["txn_min_amount"] = unlabel_df["txn_min_amount"].astype("int")
    unlabel_df["txn_amount_mean"] = unlabel_df["txn_amount_mean"].astype("int")
    unlabel_df["avg_discount"] = unlabel_df["avg_discount"].astype("int")
    unlabel_df["voucher_num"] = unlabel_df["voucher_num"].astype("int")
    unlabel_df["avg_txn_amt"] = unlabel_df["avg_txn_amt"].astype("int")
    unlabel_df["use_ratio"] = unlabel_df["use_ratio"].astype("float")
    unlabel_df["voucher_ratio"] = unlabel_df["voucher_ratio"].astype("float")
    unlabel_df["batch_no"] = unlabel_df["batch_no"].astype("int")
    unlabel_df["city_id"] = unlabel_df["city_id"].astype("int")
    unlabel_df["phone"] = unlabel_df["phone"].astype("str")
    unlabel_df["voucher_no"] = unlabel_df["voucher_no"].astype("str") 

    #模型训练和预测
    f1_score_old=float(0)#f1-score
    f1_score=float(0.3)#高于全部设置1的分数
    outset=[]
    flag=int(1)   
    label_df_cons=label_df#训练样本数不变
    while (f1_score-f1_score_old)>0.0001 :#迭代收敛到f1-score不再提升
        if flag==0 :#第一次训练排除样本数量带来的问题
            f1_score_old=f1_score
        #训练数据采样，80%训练，20%验证           
        print "总样本，有", label_df.shape[0], "行", label_df.shape[1], "列"
        train_label_df=label_df#全量训练，ample(frac=0.8) 
        print "训练集，有", train_label_df.shape[0], "行", train_label_df.shape[1], "列"
        test_label_df=label_df_cons.sample(frac=0.3) #用训练集来测试f1-score
        print "验证集，有", test_label_df.shape[0], "行", test_label_df.shape[1], "列"
    
        #模型训练
        label_X = train_label_df[['pro_code','city_id','age','sex','account_age',\
                                  'txn_count','txn_amount_mean','txn_min_amount']]
        label_X = preprocessing.scale(label_X)#归一化
        label_y = train_label_df['label']
        model = LogisticRegression()
        #if flag==0 :
        #    model = LogisticRegression()#逻辑回归，第一次预训练
        #else :
        #    model = DecisionTreeClassifier()#决策树
        model.fit(label_X, label_y)
        
        if flag==0 :#模型验证，第一次训练不评分
            expected = test_label_df['label']
            predicted_X=test_label_df[['pro_code','city_id','age','sex','account_age',\
                                       'txn_count','txn_amount_mean','txn_min_amount']]
            predicted_X=preprocessing.scale(predicted_X)#归一化
            predicted = model.predict(predicted_X)
            f1_score = metrics.f1_score(expected, predicted) #模型评估
            print f1_score
        flag=int(0)
        if f1_score_old<f1_score :#为未标记样本打上标记，然后加入训练集
            unlabel_X=unlabel_df[['pro_code','city_id','age','sex','account_age',\
                              'txn_count','txn_amount_mean','txn_min_amount']]
            unlabel_X_noScale=unlabel_X
            unlabel_X=preprocessing.scale(unlabel_X)#归一化
            unlabel_y=model.predict(unlabel_X)
            out_y=pd.DataFrame(unlabel_y.reshape(-1,1),columns=['label'])
            unlabel_X_new=unlabel_X_noScale.join(out_y,how='left')
            label_df=pd.DataFrame()#原样本清空
            label_df=label_df_cons.append(unlabel_X_new)#构成新的训练集
        else : #迭代训练结束，输出结果
            unlabel_X=unlabel_df[['pro_code','city_id','age','sex','account_age',\
                              'txn_count','txn_amount_mean','txn_min_amount']]
            unlabel_info = unlabel_df[['phone','voucher_no']]
            unlabel_X=preprocessing.scale(unlabel_X)#归一化
            unlabel_y=model.predict_proba(unlabel_X)[:,1]#预测返回概率值，通过概率值阈值选择正例样本
            out_y=pd.DataFrame(unlabel_y,columns=['prob']) #返回判定正例的比例
            outset=unlabel_info.join(out_y,how='left')#输出结果
            outset["label"] = outset.apply(lambda x:  0 if x["prob"] <0.57 else 1, axis = 1)
            outset= outset[outset['label']==1]  
            outset=outset[['phone','voucher_no','label']]
            outsetds=pd.DataFrame(outset)
            outsetds.to_csv('D:\gd_delta.csv',index=False,header=None)#输出预测数据
            
            #评价f1
            #unlabel_X=pd.DataFrame(unlabel_X,columns=['pro_code','city_id','age','sex','account_age',\
            #                  'txn_count','txn_amount_mean','txn_min_amount'])
            #print unlabel_X.head(5)
            #outset=unlabel_X.join(out_y,how='left')#输出结果
            #outset["label"] = outset.apply(lambda x:  0 if x["prob"] <0.57 else 1, axis = 1)
            #expected = outset['label']
            #predicted_X=outset[['pro_code','city_id','age','sex','account_age',\
            #                           'txn_count','txn_amount_mean','txn_min_amount']]
            #predicted_X=preprocessing.scale(predicted_X)#归一化
            #predicted = model.predict(predicted_X)
            #f1_score = metrics.f1_score(expected, predicted) #模型评估
            #print f1_score#0.855946148093
            #退出循环
            break
      
#执行
if __name__ == '__main__':  
    start = time.clock()  
    main()
    end = time.clock()  
    print('finish all in %s' % str(end - start))
```


继续提升有三点：



1）可以尝试给预测集打标签用一个模型，迭代训练用另一个模型；

2）可以尝试抽取不同的特征来建模，其次对特征值做离散化处理；

3）可以尝试用部分特征来预训练，另一部分特征来做训练模型，可以降低过拟合问题；



