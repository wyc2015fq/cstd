# 【Python-ML】SKlearn库集成学习器Boosting - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月20日 12:34:11[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：302
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月19日

@author: Jason.F
@summary: Boosting，无放回抽样，串行训练基学习器，用整个训练集来训练弱学习机，训练样本在每次迭代中都会重新赋予一个权重，在上一弱学习机错误的基础上进行学习进而构建一个更强大的分类器。
'''
import pandas as pd
import numpy as np
from sklearn.preprocessing import LabelEncoder
from sklearn.cross_validation import train_test_split
from sklearn.ensemble import AdaBoostClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.metrics import accuracy_score
import matplotlib.pyplot as plt

#导入数据和数据处理
df_wine = pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/wine/wine.data',header=None)
df_wine.columns=['Class label','Alcohol','Malic acid','Ash','Alcalinity of ash','Magnesium','Total phenols','Flavanoids','Nonflavanoid phenols','Proanthocyanins','Color intensity','Hue','OD280/OD315 of diluted wines','Proline']
print ('class labels:',np.unique(df_wine['Class label']))
df_wine=df_wine[df_wine['Class label']!=1]#选择2和3类别
y=df_wine['Class label'].values
X=df_wine[['Alcohol','Hue']].values #选择Alcohol和 Hue两个特征
le=LabelEncoder()
y=le.fit_transform(y)
X_train,X_test,y_train,y_test=train_test_split(X,y,test_size=0.40,random_state=1)
#引用BaggingClassifier训练
tree=DecisionTreeClassifier(criterion='entropy',max_depth=None)#构建基学习器
ada=AdaBoostClassifier(base_estimator=tree,n_estimators=500,learning_rate=0.1,random_state=0)
#评分，比较baggin集成器和单颗未剪枝决策树的性能差异
#单颗树评估
tree=tree.fit(X_train,y_train)
y_train_pred=tree.predict(X_train)
y_test_pred=tree.predict(X_test)
tree_train=accuracy_score(y_train, y_train_pred)
tree_test=accuracy_score(y_test, y_test_pred)
print ('Decision tree train/test accuracies %.3f/%.3f'%(tree_train,tree_test))#未剪枝，过拟合
#集成器评估
ada=ada.fit(X_train,y_train)
y_train_pred=ada.predict(X_train)
y_test_pred=ada.predict(X_test)
ada_train=accuracy_score(y_train, y_train_pred)
ada_test=accuracy_score(y_test, y_test_pred)
print ('Adaboost train/test accuracies %.3f/%.3f'%(ada_train,ada_test))
#可视化决策区域
x_min = X_train[:,0].min()-1
x_max = X_train[:,0].max()+1
y_min = X_train[:,1].min()-1
y_max = X_train[:,1].max()+1
xx,yy =np.meshgrid(np.arange(x_min,x_max,0.1),np.arange(y_min,y_max,0.1))
f,axarr= plt.subplots(nrows=1,ncols=2,sharex='col',sharey='row',figsize=(8,3))
for idx,clf,tt in zip([0,1],[tree,ada],['Decision Tree','Adaboost']):
    clf.fit(X_train,y_train)
    Z=clf.predict(np.c_[xx.ravel(),yy.ravel()])
    Z=Z.reshape(xx.shape)
    axarr[idx].contourf(xx,yy,Z,alpha=0.3)
    axarr[idx].scatter(X_train[y_train==0,0],X_train[y_train==0,1],c='blue',marker='^')
    axarr[idx].scatter(X_train[y_train==1,0],X_train[y_train==1,1],c='red',marker='o')
    axarr[idx].set_title(tt)
axarr[0].set_ylabel('Alcohol',fontsize=12)
plt.text(10.2,-1.2,s='Hue',ha='center',va='center',fontsize=12)
plt.show()
```

结果：



```
('class labels:', array([1, 2, 3], dtype=int64))
Decision tree train/test accuracies 1.000/0.833
Adaboost train/test accuracies 1.000/0.833
```
![](https://img-blog.csdn.net/20180120123351635?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




