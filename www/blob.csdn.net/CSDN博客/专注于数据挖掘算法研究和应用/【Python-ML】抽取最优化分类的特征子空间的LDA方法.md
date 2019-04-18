# 【Python-ML】抽取最优化分类的特征子空间的LDA方法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月18日 12:12:17[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：301
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月18日
@author: Jason.F
@summary: 特征抽取-LDA方法，监督，发现最优化分类的特征子空间，基于特征呈正态分布和特征间相互独立
'''
import pandas as pd
import numpy as np
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import MinMaxScaler
from sklearn.preprocessing import StandardScaler
from sklearn.linear_model import LogisticRegression
import matplotlib.pyplot as plt
#第一步:导入数据，对原始d维数据集做标准化处理
df_wine = pd.read_csv('https://archive.ics.uci.edu/ml/machine-learning-databases/wine/wine.data',header=None)
df_wine.columns=['Class label','Alcohol','Malic acid','Ash','Alcalinity of ash','Magnesium','Total phenols','Flavanoids','Nonflavanoid phenols','Proanthocyanins','Color intensity','Hue','OD280/OD315 of diluted wines','Proline']
print ('class labels:',np.unique(df_wine['Class label']))
#分割训练集合测试集
X,y=df_wine.iloc[:,1:].values,df_wine.iloc[:,0].values
X_train,X_test,y_train,y_test=train_test_split(X,y,test_size=0.3,random_state=0)
#特征值缩放-标准化
stdsc=StandardScaler()
X_train_std=stdsc.fit_transform(X_train)
X_test_std=stdsc.fit_transform(X_test)
#第二步：对于每一类别计算d维的均值向量
np.set_printoptions(precision=4)
mean_vecs=[]
for label in range(1,4):
    mean_vecs.append(np.mean(X_train_std[y_train==label],axis=0))
    print ('MV %s: %s \n' %(label,mean_vecs[label-1]))
#第三步：构造类间的散布矩阵和类内的散布矩阵
d=13 #特征数量
#计算类内散布矩阵
#观察训练集的类别样本是否均匀，计算散布矩阵的前提是训练集的类标是均匀分布的
print ('class label distribution:%s' %np.bincount(y_train)[1:])
S_W=np.zeros((d,d))#初始化类内散布矩阵
for label,mv in zip(range(1,4),mean_vecs):
    #class_scatter=np.zeros((d,d))
    #for row in X[y==label]:
    #    row,mv =row.reshape(d,1),mv.reshape(d,1)
    #    class_scatter+= (row-mv).dot((row-mv).T)
    #类标分布不均匀，对特征值做标准化，用标准化后的特征值计算散布矩阵
    class_scatter=np.cov(X_train_std[y_train==label].T)#协方差矩阵是归一化的散布矩阵
    S_W += class_scatter
print ('Within-class scatter matrix: %sx%s' %(S_W.shape[0],S_W.shape[1]))
#计算类间散布矩阵
mean_overall = np.mean(X_train_std,axis=0)
S_B=np.zeros((d,d))#初始化类间散布矩阵
for i ,mean_vec in enumerate(mean_vecs):
    n=X_train_std[y_train==i+1,:].shape[0]
    mean_vec=mean_vec.reshape(d,1)
    mean_overall=mean_overall.reshape(d,1)
    S_B+=n*(mean_vec-mean_overall).dot((mean_vec-mean_overall).T)
print ('Between-class scatter matrix: %sx%s' %(S_B.shape[0],S_B.shape[1]))
#第四部：计算类间类内乘积的矩阵的特征值和特征向量
eigen_vals,eigen_vecs=np.linalg.eig(np.linalg.inv(S_W).dot(S_B))
eigen_pairs=[(np.abs(eigen_vals[i]), eigen_vecs[:, i]) for i in range(len(eigen_vals))]
eigen_pairs=sorted(eigen_pairs,key=lambda k:k[0],reverse=True)
print ('Eigenvalues in decreasing order:\n')
for eigen_val in eigen_pairs:
    print (eigen_val[0])
#可视化特征判定类别区分能力的图，按照特征值排序绘制出特征对线性判别信息保持程度
tot=sum(eigen_vals.real)
discr=[(i/tot) for i in sorted(eigen_vals.real,reverse=True)]
cum_discr=np.cumsum(discr)
plt.bar(range(1,14),discr,alpha=0.5,align='center',label='individual discriminability')
plt.step(range(1,14),cum_discr,where='mid',label='cumulative discriminability')
plt.ylabel('discriminability ratio')
plt.xlabel('Linear Discriminants')
plt.ylim([-0.1,1.1])
plt.legend(loc='best')
plt.show()
#第五步：选取前k个特征值所对应的特征向量，构造一个dXk维的转换矩阵W，其中特征向量以列的形式排列
w=np.hstack((eigen_pairs[0][1][:,np.newaxis].real,eigen_pairs[1][1][:,np.newaxis].real))#选取前2个特征，构建13X2维的映射矩阵W
print ('Matrix W:\n',w)
#第六步：使用转换矩阵W将样本映射到新的特征子空间
X_train_lda=X_train_std.dot(w)
X_test_lda=X_test_std.dot(w)
colors=['r','b','g']
markers=['s','x','o']
for l,c,m in zip(np.unique(y_train),colors,markers):
     plt.scatter(X_train_lda[y_train == l, 0],X_train_lda[y_train == l, 1],c=c, label=l, marker=m)
plt.xlabel('LD 1')
plt.ylabel('LD 2')
plt.legend(loc='upper right')
plt.show()
#第五步：转换后的数据集进行线性训练
lr=LogisticRegression()
lr.fit(X_train_lda,y_train)
print ('Training accuracy:',lr.score(X_train_lda, y_train))
print ('Test accuracy:',lr.score(X_test_lda, y_test))
```

结果：



```
('class labels:', array([1, 2, 3], dtype=int64))
MV 1: [ 0.9259 -0.3091  0.2592 -0.7989  0.3039  0.9608  1.0515 -0.6306  0.5354
  0.2209  0.4855  0.798   1.2017] 

MV 2: [-0.8727 -0.3854 -0.4437  0.2481 -0.2409 -0.1059  0.0187 -0.0164  0.1095
 -0.8796  0.4392  0.2776 -0.7016] 

MV 3: [ 0.1637  0.8929  0.3249  0.5658 -0.01   -0.9499 -1.228   0.7436 -0.7652
  0.979  -1.1698 -1.3007 -0.3912] 

class label distribution:[40 49 35]
Within-class scatter matrix: 13x13
Between-class scatter matrix: 13x13
Eigenvalues in decreasing order:

452.721581245
156.43636122
1.07585370555e-13
4.43873563999e-14
2.87266009341e-14
2.84217094304e-14
2.40168676571e-14
1.59453089024e-14
1.59453089024e-14
9.93723443031e-15
9.93723443031e-15
2.82769841287e-15
2.82769841287e-15
('Matrix W:\n', array([[-0.0662, -0.3797],
       [ 0.0386, -0.2206],
       [-0.0217, -0.3816],
       [ 0.184 ,  0.3018],
       [-0.0034,  0.0141],
       [ 0.2326,  0.0234],
       [-0.7747,  0.1869],
       [-0.0811,  0.0696],
       [ 0.0875,  0.1796],
       [ 0.185 , -0.284 ],
       [-0.066 ,  0.2349],
       [-0.3805,  0.073 ],
       [-0.3285, -0.5971]]))
('Training accuracy:', 0.99193548387096775)
('Test accuracy:', 1.0)
```
![](https://img-blog.csdn.net/20180118121141623?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20180118121154395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







