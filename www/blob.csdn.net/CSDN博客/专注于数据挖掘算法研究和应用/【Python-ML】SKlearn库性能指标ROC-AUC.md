# 【Python-ML】SKlearn库性能指标ROC-AUC - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月19日 11:28:26[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：955
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月19日
@author: Jason.F
@summary: ROC(receiver operator characteristic，基于模型真正率和假正率等性能指标评估分类模型
'''
import pandas as pd
from sklearn.preprocessing import LabelEncoder
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.svm import SVC
import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LogisticRegression
from sklearn.metrics import roc_curve,auc,roc_auc_score,accuracy_score
from scipy import interp
from sklearn.cross_validation import StratifiedKFold
from sklearn.svm import SVC
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
pipe_lr=Pipeline([('scl',StandardScaler()),('clf',LogisticRegression(random_state=1))])
#ROC计算
X_train2=X_train[:,[4,14]]
cv=StratifiedKFold(y_train,n_folds=3,random_state=1)
fig=plt.figure(figsize=(7,5))
mean_tpr=0.0
mean_fpr=np.linspace(0,1,100)
all_tpr=[]

for i,(train,test) in enumerate(cv):
    probas=pipe_lr.fit(X_train2[train],y_train[train]).predict_proba(X_train2[test])
    fpr,tpr,thresholds = roc_curve(y_train[test],probas[:,1],pos_label=1)
    mean_tpr+=interp(mean_fpr,fpr,tpr)
    mean_tpr[0]=0.0
    roc_auc = auc(fpr,tpr)
    plt.plot(fpr,tpr,lw=1,label='ROC fold %d (area = %0.2f)'%(i+1,roc_auc))
    
plt.plot([0,1],[0,1],linestyle='--',color=(0.6,0.6,0.6),label='random guessing')
mean_tpr/=len(cv)
mean_tpr[-1]=1.0
mean_auc=auc(mean_fpr,mean_tpr)
plt.plot(mean_fpr,mean_tpr,'k--',label='mean ROC (area=%0.2f)'%mean_auc,lw=2)
plt.plot([0,0,1],[0,1,1],lw=2,linestyle=':',color='black',label='perfect performance')
plt.xlim([-0.05,1.05])
plt.ylim([-0.05,1.05])
plt.xlabel('false positive rate')
plt.ylabel('true positive rate')
plt.title('Receiver Operator Characteristic')
plt.legend(loc='lower right')
plt.show()
 
#直接使用roc_auc_curve函数  
pipe_svc=Pipeline([('scl',StandardScaler()),('clf',SVC(random_state=1))])
pipe_svc=pipe_svc.fit(X_train2,y_train)
y_pred2=pipe_svc.predict(X_test[:,[4,14]])
print ('ROC AUC:%.3f'%roc_auc_score(y_true=y_test,y_score=y_pred2))
print ('Accuracy:%.3f'%accuracy_score(y_true=y_test,y_pred=y_pred2))
```
结果：

![](https://img-blog.csdn.net/20180119112809128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



```
[1 0]
ROC AUC:0.671
Accuracy:0.728
```





