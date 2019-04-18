# 【Python-ML】SKlearn库线性回归器LinearRegression - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月24日 16:50:02[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：348
个人分类：[Big data](https://blog.csdn.net/fjssharpsword/article/category/6309934)

所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月24日
@author: Jason.F
@summary: 有监督回归学习-基于最小二乘法构建线性回归模型
最小二乘法(Ordinary Least Square,LOS)，估计回归曲线的参数，使得回归曲线到样本点垂直距离(残差或误差)的平方和最小
'''
import pandas as pd
import numpy as np
import time
import matplotlib.pyplot as plt
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LinearRegression

def lin_regplot(X,y,model):
    plt.scatter(X,y,c='blue')
    plt.plot(X,model.predict(X),color='red')
    return None
'''
class LinearRegressionGD(object):
    def __init__(self,eta=0.01,n_iter=20):
        self.eta=eta
        self.n_iter=n_iter
    
    def fit(self,X,y):
        self.w_=np.zeros(1+X.shape[1])
        self.costs_=[]
        
        for _ in range(self.n_iter):
            output=self.net_input(X)
            errors=(y-output)
            self.w_[1:] += self.eta * X.T.dot(errors)
            self.w_[0]  += self.eta * errors.sum()
            cost=(errors ** 2).sum() /2.0
            self.costs_.append(cost)
        return self
    
    def net_input(self,X):
        #calculate net input
        return np.dot(X,self.w_[1:])+self.w_[0]
    
    def predict(self,X):
        #return class label after unit step
        return self.net_input(X)
'''    
if __name__ == "__main__":   
    start = time.clock()     
    #导入波士顿房屋数据集
    df=pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/housing/housing.data',header=None,sep='\s+')
    df.columns=['CRIM','ZM','INDUS','CHAS','NOX','RM','AGE','DIS','RAD','TAX','PTRATIO','B','LSTAT','MEDV']
    X=df[['RM']].values#房间数
    y=df['MEDV'].values#房间价格
    '''
    #标准化
    sc_x=StandardScaler()
    sc_y=StandardScaler()
    X_std=sc_x.fit_transform(X)
    y_std=sc_y.fit_transform(y)
    #训练
    lr=LinearRegressionGD()
    lr.fit(X_std,y_std)
    #梯度下降收敛验证
    plt.plot(range(1,len(lr.costs_)+1),lr.costs_)
    plt.ylabel('SSE')
    plt.xlabel('Epoch')
    plt.show()
    #可视化房间数和房屋价格之间的关系
    lin_regplot(X_std, y_std, lr)
    plt.xlabel('Average number of rooms[RM](standardized)')
    plt.ylabel('Price in $1000\'s[MEDV](standardized)')
    plt.show()
    #预测
    num_rooms_std=sc_x.transform([5.0])
    price_std = lr.predict(num_rooms_std)
    print ('Price in $1000\'s:%.3f'%sc_y.inverse_transform(price_std))
    print ('Slope:%.3f'%lr.w_[1])
    print ('Intercept:%.3f'%lr.w_[0])
    '''
    #利用sklearn库中的线性回归模型
    slr=LinearRegression()
    slr.fit(X,y)
    print ('Slope:%.3f'%slr.coef_[0])
    print ('Intercept:%.3f'%slr.intercept_)
    lin_regplot(X, y, slr)
    plt.xlabel('Average number of rooms[RM](standardized)')
    plt.ylabel('Price in $1000\'s[MEDV](standardized)')
    plt.show()
    
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```

结果：



```
Slope:9.102
Intercept:-34.671
finish all in 44.0196931201
```
![](https://img-blog.csdn.net/20180124164952135?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




