# 【Python-ML】SKlearn库性能指标-混淆矩阵和F1 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月19日 10:43:33[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1568
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月19日
@author: Jason.F
@summary: 
混淆矩阵：TP、TN、FP、FP、FN
F1：召回率和争取率
'''
import pandas as pd
from sklearn.preprocessing import LabelEncoder
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.pipeline import Pipeline
from sklearn.svm import SVC
import numpy as np
from sklearn.metrics import confusion_matrix
import matplotlib.pyplot as plt
from sklearn.metrics import precision_score, recall_score, f1_score
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
pipe_svc.fit(X_train,y_train)
y_pred=pipe_svc.predict(X_test)
#混淆矩阵并可视化
confmat= confusion_matrix(y_true=y_test,y_pred=y_pred)#输出混淆矩阵
print (confmat)
fig,ax = plt.subplots(figsize=(2.5,2.5))
ax.matshow(confmat,cmap=plt.cm.Blues,alpha=0.3)
for i in range(confmat.shape[0]):
    for j in range(confmat.shape[1]):
        ax.text(x=j,y=i,s=confmat[i,j],va='center',ha='center')
plt.xlabel('predicted label')
plt.ylabel('true label')
plt.show()
#召回率、准确率、F1
print ('precision:%.3f' %precision_score(y_true=y_test, y_pred=y_pred))
print ('recall:%.3f' %recall_score(y_true=y_test, y_pred=y_pred))
print ('F1:%.3f' %f1_score(y_true=y_test, y_pred=y_pred))

#参考：http://scikit-learn.org/stable/modules/model_evaluation.html
```


结果：



```
[1 0]
[[71  1]
 [ 2 40]]
precision:0.976
recall:0.952
F1:0.964
```
![](https://img-blog.csdn.net/20180119104324198?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




