# 【Python学习系列十九】基于scikit-learn库进行特征选择 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月29日 17:20:52[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2247
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









场景：特征选择在模型训练前是非常有意义的，实际上就是先期对特征相关性进行分析。

参考：http://blog.csdn.net/fjssharpsword/article/details/73550337

代码：这里基于scikit-learn库联系了几个特征选择方法，实际学习任务当然需要掌握理论来应用，FeatureSelecton.py如下



```python
# -*- coding: utf-8 -*-

import pandas as pd
from sklearn.feature_selection import SelectKBest ,chi2
from sklearn import preprocessing
from sklearn.feature_selection import VarianceThreshold
from sklearn.svm import LinearSVC
from sklearn.feature_selection import SelectFromModel
from sklearn.ensemble import ExtraTreesClassifier
#加载数据
label_ds=pd.read_csv(r"lx_train_features_all.txt",sep='\t',encoding='utf8',names=['u_id','spu_id','brand_id','cat_id',\
                                      'u_spu_num','u_brand_num','u_cat_num','u_cat_spu','u_brand_spu',\
                                      'u_spu_freq','u_spu_date','u_spu_click_freq','u_spu_click_date',\
                                      'u_first_date','u_last_date','u_spu_ratio','u_ratio',\
                                      'action_type']) 
label_X  = label_ds[['u_id','spu_id','brand_id','cat_id',\
                                      'u_spu_num','u_brand_num','u_cat_num','u_cat_spu','u_brand_spu',\
                                      'u_spu_freq','u_spu_date','u_spu_click_freq','u_spu_click_date',\
                                      'u_first_date','u_last_date','u_spu_ratio','u_ratio',\
                                      'action_type']]
label_y = label_ds['action_type']#类别
min_max_scaler = preprocessing.MinMaxScaler()#范围0-1缩放标准化
label_X_scaler=min_max_scaler.fit_transform(label_X)

#单变量特征选择-卡方检验，选择相关性最高的前5个特征
X_chi2 = SelectKBest(chi2, k=5).fit_transform(label_X_scaler, label_y)
print "训练集，有", X_chi2.shape[0], "行", X_chi2.shape[1], "列"
df_X_chi2=pd.DataFrame(X_chi2)
feature_names = df_X_chi2.columns.tolist()#显示列名
print feature_names

#通过方差选择特征。方差为0的特征会被自动移除。剩下的特征按设定的方差的阈值进行选择。
sel = VarianceThreshold(threshold=.06)#设置方差的阈值为0.6
X_sel=sel.fit_transform(label_X_scaler)#选择方差大于0.6的特征
df_X_sel=pd.DataFrame(X_chi2)
feature_names = df_X_sel.columns.tolist()#显示列名
print feature_names

#基于L1的特征选择
lsvc = LinearSVC(C=0.01, penalty="l1", dual=False).fit(label_X_scaler, label_y)
model = SelectFromModel(lsvc, prefit=True)
X_lsvc = model.transform(label_X_scaler)
df_X_lsvc=pd.DataFrame(X_chi2)
feature_names = df_X_lsvc.columns.tolist()#显示列名
print feature_names

#基于树的特征选择，并按重要性阈值选择特征
clf = ExtraTreesClassifier()#基于树模型进行模型选择
clf = clf.fit(label_X_scaler, label_y)
model = SelectFromModel(clf, threshold='1.00*mean',prefit=True)#选择特征重要性为1倍均值的特征，数值越高特征越重要
X_trees = model.transform(label_X_scaler)#返回所选的特征
df_X_trees=pd.DataFrame(X_chi2)
feature_names = df_X_trees.columns.tolist()#显示列名
print feature_names
```





