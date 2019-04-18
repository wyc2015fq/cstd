# 【Python-ML】SKlearn库非线性决策树回归 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月25日 12:56:44[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：845
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月24日
@author: Jason.F
@summary: 有监督回归学习-决策树回归模型，无需对数据进行特征转换，就能处理非线性关系的数据
'''
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from sklearn.tree import DecisionTreeRegressor
from sklearn.cross_validation import train_test_split
from sklearn.ensemble import RandomForestRegressor
from sklearn.metrics.regression import mean_squared_error, r2_score
#导入波士顿房屋数据集
df=pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/housing/housing.data',header=None,sep='\s+')
df.columns=['CRIM','ZM','INDUS','CHAS','NOX','RM','AGE','DIS','RAD','TAX','PTRATIO','B','LSTAT','MEDV']

#单颗决策树回归，MSE替代熵作为节点t的不纯度度量标准
X=df[['LSTAT']].values
y=df['MEDV'].values
tree = DecisionTreeRegressor (max_depth=3)
tree.fit(X,y)
sort_idx = X.flatten().argsort()
def lin_regplot(X,y,model):
    plt.scatter(X,y,c='blue')
    plt.plot(X,model.predict(X),color='red')
    return None
lin_regplot(X[sort_idx],y[sort_idx],tree)
plt.xlabel('%lower status of the population[LSTAT]')
plt.ylabel('Price in $1000\'s [MEDV]')
plt.show()

#随机森林，对数据集中的异常值不敏感，且无更多参数调优
#随机森林回归使用MSE作为单颗决策树生成的标准，同时所有决策树预测值的平均数作为预测目标变量的值
X=df.iloc[:,:-1].values
y=df['MEDV'].values
X_train,X_test,y_train,y_test = train_test_split(X,y,test_size=0.4,random_state=1)
forest = RandomForestRegressor(n_estimators=1000,criterion='mse',random_state=1,n_jobs=1)
forest.fit(X_train,y_train)
y_train_pred = forest.predict(X_train)
y_test_pred = forest.predict(X_test)
print ('MSE train:%.3f,test:%.3f'%(mean_squared_error(y_train,y_train_pred),mean_squared_error(y_test,y_test_pred)))
print ('R^2 train:%.3f,test:%.3f'%(r2_score(y_train,y_train_pred),r2_score(y_test,y_test_pred)))
#可视化效果
plt.scatter(y_train_pred,y_train_pred-y_train,c='black',marker='o',s=35,alpha=0.5,label='Training data')
plt.scatter(y_test_pred,y_test_pred-y_test,c='lightgreen',marker='s',s=35,alpha=0.7,label='Test data')
plt.xlabel('Predicted values')
plt.ylabel('Residuals')
plt.legend(loc='upper left')
plt.show()
```

结果：

![](https://img-blog.csdn.net/20180125125623572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



```
MSE train:1.642,test:11.052
R^2 train:0.979,test:0.878
```

![](https://img-blog.csdn.net/20180125125631903?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



