# 【Python学习系列二十】scikit-learn库模型持久化 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月30日 11:49:43[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1977
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









场景：需要将模型保存到内存，或磁盘。

代码：



```python
# -*- coding: utf-8 -*-

import pandas as pd
import pickle as pkl
from sklearn.externals import joblib
from sklearn import svm
#加载数据
label_ds=pd.read_csv(r"D:\\tmp\\sam_11.csv",sep=',',encoding='utf8',\
                     names=['u_spu_num','u_brand_num','u_cat_num','u_cat_spu','u_brand_spu','u_spu_date','action_type']) 
print "训练集，有", label_ds.shape[0], "行", label_ds.shape[1], "列" 
#模型训练
label_X  = label_ds[['u_spu_num','u_brand_num','u_cat_num','u_cat_spu','u_brand_spu','u_spu_date']]
label_y = label_ds['action_type']#类别
model = svm.SVC()
model.fit(label_X, label_y)  
print model
#模型导出导入磁盘
joblib.dump(model, 'D:\\tmp\\model.pkl')
model2 = joblib.load('D:\\tmp\\model.pkl') 
print model2
#模型保存
s = pkl.dumps(model)
model3 = pkl.loads(s)
print model3
```
数据集：





```
0,0,6,6,0,0,1
0,0,2,2,0,0,1
0,0,3,3,0,0,1
0,0,2,2,0,0,1
0,0,0,0,0,0,1
0,0,1,1,0,0,0
0,0,9,9,0,0,0
0,0,1,1,0,0,0
0,0,3,3,0,0,0
```

执行结果：



```
训练集，有 9 行 7 列
SVC(C=1.0, cache_size=200, class_weight=None, coef0=0.0,
  decision_function_shape=None, degree=3, gamma='auto', kernel='rbf',
  max_iter=-1, probability=False, random_state=None, shrinking=True,
  tol=0.001, verbose=False)
SVC(C=1.0, cache_size=200, class_weight=None, coef0=0.0,
  decision_function_shape=None, degree=3, gamma='auto', kernel='rbf',
  max_iter=-1, probability=False, random_state=None, shrinking=True,
  tol=0.001, verbose=False)
SVC(C=1.0, cache_size=200, class_weight=None, coef0=0.0,
  decision_function_shape=None, degree=3, gamma='auto', kernel='rbf',
  max_iter=-1, probability=False, random_state=None, shrinking=True,
  tol=0.001, verbose=False)
```








1）采用Python内建的持久性模型 pickle 保存scikit的模型，字符串形式；



2）用joblib（joblib.dump&joblib.load）保存到磁盘，文件形式；





