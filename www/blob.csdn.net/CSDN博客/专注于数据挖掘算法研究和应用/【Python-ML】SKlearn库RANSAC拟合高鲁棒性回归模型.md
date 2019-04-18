# 【Python-ML】SKlearn库RANSAC拟合高鲁棒性回归模型 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月24日 20:10:40[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：659
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月24日
@author: Jason.F
@summary: 有监督回归学习-RANSAC拟合高鲁棒性回归模型
高鲁棒性线性回归器是一种清楚异常值的学习模型，采用随机抽样一致性(RANdom SAmple Consensus,RANSAC)，使用数据的内点(inlier,数据集的子集)进行回归模型的拟合
算法流程：
1）从数据集中随机抽取样本构建内点集合来拟合模型
2）使用剩余数据对上一步得到的模型进行测试，并将落差在预定公差范围内的样本点增至内点集合中
3）使用全部的内点集合再次进行模型的拟合
4）使用内点集合来估计模型的误差
5）如果模型性能达到设定的阈值或迭代达到预定次数，则算法终止，否则跳转到第一步
使用RANSAC降低数据集中异常点的潜在影响，但不确定剔除掉异常数据对预测性能存在的影响。
'''
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from sklearn.linear_model import RANSACRegressor
from sklearn.linear_model.base import LinearRegression

#导入波士顿房屋数据集
df=pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/housing/housing.data',header=None,sep='\s+')
df.columns=['CRIM','ZM','INDUS','CHAS','NOX','RM','AGE','DIS','RAD','TAX','PTRATIO','B','LSTAT','MEDV']
X=df[['RM']].values#房间数
y=df['MEDV'].values#房间价格
#模型训练
ransac=RANSACRegressor(LinearRegression(),
                       max_trials=100,#最大迭代次数
                       min_samples=50,#最小抽取的内点样本数量
                       residual_metric=lambda x:np.sum(np.abs(X),axis=1),#计算拟合曲线与样本点垂直距离的绝对值
                       residual_threshold=5.0,#与拟合曲线距离小于该阈值的是内点，加入到下一轮训练集中，与具体问题有关，目前有能够自动选出适宜的内点阈值方法
                       random_state=0)
ransac.fit(X,y)
#获取内点和异常点集合
inlier_mask = ransac.inlier_mask_
outlier_mask = np.logical_not(inlier_mask)
line_X = np.arange(3,10,1)
line_y_ransac = ransac.predict(line_X[:,np.newaxis])
plt.scatter(X[inlier_mask],y[inlier_mask],c='blue',marker='o',label='Inliers')
plt.scatter(X[outlier_mask],y[outlier_mask],c='lightgreen',marker='s',label='Outliers')
plt.plot(line_X,line_y_ransac,color='red')
plt.xlabel('Average number of roosm[RM]')
plt.ylabel('Price in $1000\'s [MEDV]')
plt.legend(loc='upper left')
plt.show()
#显示模型的斜率和截距
print ('Slope:%.3f'%ransac.estimator_.coef_[0])
print ('Intercept:%.3f'%ransac.estimator_.intercept_)
```




