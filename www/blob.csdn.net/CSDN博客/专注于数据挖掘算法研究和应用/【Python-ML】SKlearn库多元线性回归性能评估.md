# 【Python-ML】SKlearn库多元线性回归性能评估 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月25日 10:25:49[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：708
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月24日
@author: Jason.F
@summary: 有监督回归学习-多元线性回归的性能评估
'''
import pandas as pd
import numpy as np
import time
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.cross_validation import train_test_split
from sklearn.metrics import mean_squared_error
from sklearn.metrics import r2_score
if __name__ == "__main__":   
    start = time.clock()     
    #导入波士顿房屋数据集
    df=pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/housing/housing.data',header=None,sep='\s+')
    df.columns=['CRIM','ZM','INDUS','CHAS','NOX','RM','AGE','DIS','RAD','TAX','PTRATIO','B','LSTAT','MEDV']
    X=df.iloc[:,:-1].values
    y=df['MEDV'].values#房间价格
    X_train,X_test,y_train,y_test=train_test_split(X,y,test_size=0.3,random_state=0)
    slr=LinearRegression()
    slr.fit(X_train,y_train)
    y_train_pred=slr.predict(X_train)
    y_test_pred = slr.predict(X_test)
    #可视化残差值和预测值
    plt.scatter(y_train_pred,y_train_pred-y_train,c='blue',marker='o',label='Training data')
    plt.scatter(y_test_pred,y_test_pred-y_test,c='lightgreen',marker='s',label='Test data')
    plt.xlabel('Predicted values')
    plt.ylabel('Residuals')
    plt.legend(loc='upper left')
    plt.hlines(y=0,xmin=-10,xmax=50,lw=2,colors='red')
    plt.xlim([-10,50])
    plt.show()
    #评估均方误差
    print ('MSE train: %.3f,test:%.3f' % (mean_squared_error(y_train,y_train_pred),mean_squared_error(y_test,y_test_pred)))
    #评估决定系数(coefficient of determination)，是MSE的标准化
    print ('R^2 train: %.3f,test:%.3f' % (r2_score(y_train,y_train_pred),r2_score(y_test,y_test_pred)))
    
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```

结果：



```
MSE train: 19.958,test:27.196
R^2 train: 0.765,test:0.673
finish all in 12.8996295796
```
![](https://img-blog.csdn.net/20180125102539995?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




