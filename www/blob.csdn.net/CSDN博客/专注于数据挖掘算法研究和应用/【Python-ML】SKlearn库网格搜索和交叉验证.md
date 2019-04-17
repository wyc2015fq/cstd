# 【Python-ML】SKlearn库网格搜索和交叉验证 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月19日 10:25:48[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：621
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月18日
@author: Jason.F
@summary: 
GridSearch网格搜索：同一模型下组合参数选择最优；
嵌套交叉验证：不同模型选择最优；
'''
import pandas as pd
from sklearn.preprocessing import LabelEncoder
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.grid_search import GridSearchCV
from sklearn.svm import SVC
import numpy as np
from sklearn.model_selection import cross_val_score
from sklearn.tree import DecisionTreeClassifier
#导入数据
df = pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/breast-cancer-wisconsin/wdbc.data',header=None)
X=df.loc[:,2:].values
y=df.loc[:,1].values
le=LabelEncoder()
y=le.fit_transform(y)#类标整数化
print (le.transform(['M','B']))
#划分训练集合测试集
X_train,X_test,y_train,y_test = train_test_split (X,y,test_size=0.20,random_state=1)
#建立pipeline
pipe_svc=Pipeline([('scl',StandardScaler()),('clf',SVC(random_state=1))])
param_range=[0.0001,0.001,0.01,0.1,1.0,10.0,100.0,1000.0]
param_grid=[{'clf__C':param_range,'clf__kernel':['linear']},\
            {'clf__C':param_range,'clf__gamma':param_range,'clf__kernel':['rbf']}]
#网格搜索
gs=GridSearchCV(estimator=pipe_svc,param_grid=param_grid,scoring='accuracy',cv=10,n_jobs=1)
gs=gs.fit(X_train,y_train)
print (gs.best_score_)
print (gs.best_params_)
#选择最佳模型
clf=gs.best_estimator_
clf.fit(X_train,y_train)
print ('Test accuracy:%.3f' % clf.score(X_test,y_test))

#嵌套交叉验证
gs_svm=GridSearchCV(estimator=pipe_svc,param_grid=param_grid,scoring='accuracy',cv=10,n_jobs=1)#-1
scores_svm=cross_val_score(gs_svm,X,y,scoring='accuracy',cv=5)
print ('SVM CV accuracy:%.3f +/- %.3f'%(np.mean(scores_svm),np.std(scores_svm)))

#比较决策树交叉验证
gs_dt=GridSearchCV(estimator=DecisionTreeClassifier(random_state=0),param_grid=[{'max_depth':[1,2,3,4,5,6,7,None]}],scoring='accuracy',cv=5)
scores_dt=cross_val_score(gs_dt,X,y,scoring='accuracy',cv=5)
print ('DT CV accuracy:%.3f +/- %.3f'%(np.mean(scores_dt),np.std(scores_dt)))
```


结果：



```
[1 0]
0.978021978022
{'clf__C': 0.1, 'clf__kernel': 'linear'}
Test accuracy:0.965
SVM CV accuracy:0.972 +/- 0.012
DT CV accuracy:0.917 +/- 0.009
```





