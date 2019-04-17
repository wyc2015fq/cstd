# 【Python-ML】SKlearn库L1正则化特征选择 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月17日 14:45:20[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：913
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月17日
@author: Jason.F
@summary: Scikit-Learn库逻辑斯蒂L1正则化-特征选择
'''
import pandas as pd
import numpy as np
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
import matplotlib.pyplot as plt
#导入数据
df_wine = pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/wine/wine.data',header=None)
df_wine.columns=['Class label','Alcohol','Malic acid','Ash','Alcalinity of ash','Magnesium','Total phenols','Flavanoids','Nonflavanoid phenols','Proanthocyanins','Color intensity','Hue','OD280/OD315 of diluted wines','Proline']
print ('class labels:',np.unique(df_wine['Class label']))
#print (df_wine.head(5))
#分割训练集合测试集
X,y=df_wine.iloc[:,1:].values,df_wine.iloc[:,0].values
X_train,X_test,y_train,y_test=train_test_split(X,y,test_size=0.3,random_state=0)
#特征值缩放
#归一化
mms=MinMaxScaler()
X_train_norm=mms.fit_transform(X_train)
X_test_norm=mms.fit_transform(X_test)
#标准化
stdsc=StandardScaler()
X_train_std=stdsc.fit_transform(X_train)
X_test_std=stdsc.fit_transform(X_test)

#L1正则化的逻辑斯蒂模型
lr=LogisticRegression(penalty='l1',C=0.1)#penalty='l2'
lr.fit(X_train_std,y_train)
print ('Training accuracy:',lr.score(X_train_std, y_train))
print ('Test accuracy:',lr.score(X_test_std, y_test))#比较训练集和测试集，观察是否出现过拟合
print (lr.intercept_)#查看截距，三个类别
print (lr.coef_)#查看权重系数，L1有稀疏化效果做特征选择

#正则化效果，减少约束参数值C，增加惩罚力度，各特征权重系数趋近于0
fig=plt.figure()
ax=plt.subplot(111)
colors=['blue','green','red','cyan','magenta','yellow','black','pink','lightgreen','lightblue','gray','indigo','orange']
weights,params=[],[]
for c in np.arange(-4,6,dtype=float):
    lr=LogisticRegression(penalty='l1',C=10**c,random_state=0)
    lr.fit(X_train_std,y_train)
    weights.append(lr.coef_[0])#三个类别，选择第一个类别来观察
    params.append(10**c)
weights=np.array(weights)
for column,color in zip(range(weights.shape[1]),colors):
    plt.plot(params,weights[:,column],label=df_wine.columns[column+1],color=color)
plt.axhline(0,color='black',linestyle='--',linewidth=3)
plt.xlim([10**(-5),10**5])
plt.ylabel('weight coefficient')
plt.xlabel('C')
plt.xscale('log')
plt.legend(loc='upper left')
ax.legend(loc='upper center',bbox_to_anchor=(1.38,1.03),ncol=1,fancybox=True)
plt.show()
```

结果：



```
('class labels:', array([1, 2, 3], dtype=int64))
('Training accuracy:', 0.9838709677419355)
('Test accuracy:', 0.98148148148148151)
[-0.38378625 -0.15815556 -0.70033857]
[[ 0.28028457  0.          0.         -0.02806147  0.          0.
   0.71013567  0.          0.          0.          0.          0.
   1.23592372]
 [-0.64368703 -0.06896342 -0.05715611  0.          0.          0.          0.
   0.          0.         -0.92722893  0.05967934  0.         -0.37098083]
 [ 0.          0.06129709  0.          0.          0.          0.
  -0.63710764  0.          0.          0.49858959 -0.35822494 -0.57004251
   0.        ]]
```
![](https://img-blog.csdn.net/20180117144507592?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




