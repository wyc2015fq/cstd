# 【Python-ML】SKlearn库特征选择-随机森林方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月17日 16:30:39[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4726
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月17日
@author: Jason.F
@summary: Scikit-Learn库随机森林-特征选择
'''
import pandas as pd
import numpy as np
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import StandardScaler
import matplotlib.pyplot as plt
from sklearn.ensemble import RandomForestClassifier
#导入数据
df_wine = pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/wine/wine.data',header=None)
df_wine.columns=['Class label','Alcohol','Malic acid','Ash','Alcalinity of ash','Magnesium','Total phenols','Flavanoids','Nonflavanoid phenols','Proanthocyanins','Color intensity','Hue','OD280/OD315 of diluted wines','Proline']
print ('class labels:',np.unique(df_wine['Class label']))
#分割训练集合测试集
X,y=df_wine.iloc[:,1:].values,df_wine.iloc[:,0].values
X_train,X_test,y_train,y_test=train_test_split(X,y,test_size=0.3,random_state=0)
#特征值缩放-标准化，决策树模型不依赖特征缩放
#stdsc=StandardScaler()
#X_train_std=stdsc.fit_transform(X_train)
#X_test_std=stdsc.fit_transform(X_test)
#随机森林评估特征重要性
feat_labels=df_wine.columns[1:]
forest=RandomForestClassifier(n_estimators=10000,n_jobs=-1,random_state=0)
forest.fit(X_train,y_train)
importances=forest.feature_importances_
indices=np.argsort(importances)[::-1]
for f in range(X_train.shape[1]):
    #给予10000颗决策树平均不纯度衰减的计算来评估特征重要性
    print ("%2d) %-*s %f" % (f+1,30,feat_labels[f],importances[indices[f]]) )
#可视化特征重要性-依据平均不纯度衰减
plt.title('Feature Importance-RandomForest')
plt.bar(range(X_train.shape[1]),importances[indices],color='lightblue',align='center')
plt.xticks(range(X_train.shape[1]),feat_labels,rotation=90)
plt.xlim([-1,X_train.shape[1]])
plt.tight_layout()
plt.show()

#在这个基础上，随机森林海可以通过阈值压缩数据集
X_selected=forest.transform(X_train,threshold=0.15)#大于0.15只有三个特征
print (X_selected.shape)
'''
1) Alcohol                        0.182483
 2) Malic acid                     0.158610
 3) Ash                            0.150948
 4) Alcalinity of ash              0.131987
 5) Magnesium                      0.106589
 6) Total phenols                  0.078243
 7) Flavanoids                     0.060718
 8) Nonflavanoid phenols           0.032033
 9) Proanthocyanins                0.025400
10) Color intensity                0.022351
11) Hue                            0.022078
12) OD280/OD315 of diluted wines   0.014645
13) Proline                        0.013916
'''
```

结果：

![](https://img-blog.csdn.net/20180117163026087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




