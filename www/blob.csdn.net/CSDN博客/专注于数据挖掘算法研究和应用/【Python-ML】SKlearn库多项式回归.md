# 【Python-ML】SKlearn库多项式回归 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月25日 12:07:52[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：388
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月24日
@author: Jason.F
@summary: 有监督回归学习-多项式回归，通过多项式特征拟合非线性关系
'''
import pandas as pd
import numpy as np
import time
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
from sklearn.metrics.regression import mean_squared_error, r2_score

if __name__ == "__main__":   
    start = time.clock()    
    
    df=pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/housing/housing.data',header=None,sep='\s+')
    df.columns=['CRIM','ZM','INDUS','CHAS','NOX','RM','AGE','DIS','RAD','TAX','PTRATIO','B','LSTAT','MEDV'] 
    X = df[['LSTAT']].values
    y = df['MEDV'].values
    regr = LinearRegression()
    #创建多项式特征
    quadratic = PolynomialFeatures(degree=2)#二项式
    cubic = PolynomialFeatures(degree=3)
    X_quad = quadratic.fit_transform(X)
    X_cubic = cubic.fit_transform(X)
    
    #线性拟合
    X_fit = np.arange(X.min(),X.max(),1)[:,np.newaxis]
    regr = regr.fit(X,y)
    linear_r2 = r2_score(y,regr.predict(X))
    y_lin_fit = regr.predict(X_fit)
    #二项式拟合
    regr = regr.fit(X_quad,y)
    quadratic_r2 = r2_score(y,regr.predict(X_quad))
    y_quad_fit =regr.predict(quadratic.fit_transform(X_fit))
    #三项式
    regr = regr.fit(X_cubic,y)
    cubic_r2=r2_score(y,regr.predict(X_cubic))
    y_cubic_fit = regr.predict(cubic.fit_transform(X_fit))
    #可视化效果
    plt.scatter(X,y,label='training points',color='lightgray')
    plt.plot(X_fit,y_lin_fit,label='linear(d=1),$R^2=%.2f$'%linear_r2,color='blue',lw=2,linestyle=':')
    plt.plot(X_fit,y_quad_fit,label='quadratic(d=2),$R^2=%.2f$'%quadratic_r2,color='red',lw=2,linestyle='-')
    plt.plot(X_fit,y_cubic_fit,label='cubic(d=3),$R^2=%.2f$'%cubic_r2,color='green',lw=2,linestyle='--')
    plt.xlabel('% lower status of the population [LSTAT]')
    plt.ylabel('Price in $1000\'s [LSTAT]')
    plt.legend(loc='upper right')
    plt.show()
    '''
    X = np.array([258.0,270.0,294.0,320.0,342.0,368.0,396.0,446.0,480.0,586.0])[:,np.newaxis]
    y = np.array([236.4,234.4,252.8,298.6,314.2,342.2,360.8,368.0,391.2,390.8])
    lr = LinearRegression()
    pr = LinearRegression()
    quadratic = PolynomialFeatures(degree=2)#二项式
    X_quad = quadratic.fit_transform(X)
    #一项线性回归
    lr.fit(X,y)
    X_fit = np.arange(250,600,10)[:,np.newaxis]
    y_lin_fit =lr.predict(X_fit)
    #多项项目回归
    pr.fit(X_quad,y)
    y_quad_fit = pr.predict(quadratic.fit_transform(X_fit))
    #可视化效果
    plt.scatter(X,y,label='training points')
    plt.plot(X_fit,y_lin_fit,label='linear fit',linestyle='--')
    plt.plot(X_fit,y_quad_fit,label='quadratic fit')
    plt.legend(loc='upper left')
    plt.show()
    #评估
    y_lin_pred = lr.predict(X)
    y_quad_pred = pr.predict(X_quad)
    print ('Training MSE linear:%.3f,quadratic:%.3f'%(mean_squared_error(y,y_lin_pred),mean_squared_error(y,y_quad_pred)))
    print ('Training R^2 linear:%.3f,quadratic:%.3f'%(r2_score(y,y_lin_pred),r2_score(y,y_quad_pred)))
    '''
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```

结果：

![](https://img-blog.csdn.net/20180125120738158?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




