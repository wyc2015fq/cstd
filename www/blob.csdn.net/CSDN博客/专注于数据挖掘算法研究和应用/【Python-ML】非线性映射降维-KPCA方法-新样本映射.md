# 【Python-ML】非线性映射降维-KPCA方法-新样本映射 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月18日 15:56:56[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：742
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月18日
@author: Jason.F
@summary: 特征抽取-KPCA方法，核主成分分析方法，RBF核实现，增加新样本映射功能
          KPCA基于内存，每次映射新样本，需要计算训练数据集中每一个样本和新样本之间的相似度(点积)
'''
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.spatial.distance import pdist,squareform
from scipy import exp
from scipy.linalg import eigh
from sklearn.datasets import make_moons
from sklearn.datasets import make_circles
from sklearn.decomposition import PCA
from matplotlib.ticker import FormatStrFormatter
from sklearn.gaussian_process.gpc import LAMBDAS
def rbf_kernel_pca(X,gama,n_components):
    '''
    RBF kernel PCA implementation.
    
    Parameters:
    X:{Numpy ndarray},shape=[n_samples,n_features]
    gama:float,Tuning parameter of the RBF kernel
    n_components:int,Number of principal components to return
    
    Returns:
    X_pc:{Numpy ndarray},shape=[n_samples,n_features],Projected dataset
    lambdas:list,Eigenvalues
    '''
    #1：计算样本对欧几里得距离，并生成核矩阵
    #k(x,y)=exp(-gama *||x-y||^2)，x和y表示样本，构建一个NXN的核矩阵，矩阵值是样本间的欧氏距离值。
    #1.1:calculate pairwise squared Euclidean distances in the MXN dimensional dataset.
    sq_dists = pdist (X, 'sqeuclidean') #计算两两样本间欧几里得距离
    
    #1.2:convert pairwise distances into a square matrix.
    mat_sq_dists=squareform(sq_dists) #距离平方
    
    #1.3:compute the symmetric kernel matrix.
    K=exp(-gama * mat_sq_dists) 
    
    #2:聚集核矩阵K'=K-L*K-K*L + L*K*L，其中L是一个nXn的矩阵(和核矩阵K的维数相同，所有的值都是1/n。
    #聚集核矩阵的必要性是：样本经过标准化处理后，当在生成协方差矩阵并以非线性特征的组合替代点积时，所有特征的均值为0；但用低维点积计算时并没有精确计算新的高维特征空间，也无法确定新特征空间的中心在零点。
    #center the kernel matrix.
    N=K.shape[0]
    one_n = np.ones((N,N))/N #NXN单位矩阵
    K=K - one_n.dot(K) - K.dot(one_n) + one_n.dot(K).dot(one_n)
    
    #3：对聚集后的核矩阵求取特征值和特征向量
    #obtaining eigenpairs from the centered kernel matrix
    #numpy.eigh returns them in sorted order.
    eigvals,eigvecs = eigh(K)
    
    #4：选择前k个特征值所对应的特征向量，和PCA不同，KPCA得到的K个特征，不是主成分轴，而是高维映射到低维后的低维特征数量
    #核化过程是低维映射到高维，pca是降维，经过核化后的维度已经不是原来的特征空间。
    #核化是低维映射到高维，但并不是在高维空间计算(非线性特征组合)而是在低维空间计算(点积)，做到这点关键是核函数，核函数通过两个向量点积来度量向量间相似度，能在低维空间内近似计算出高维空间的非线性特征空间。
    #collect the top k eigenvectors (projected samples).
    X_pc = np.column_stack((eigvecs[:,-i] for i in range(1,n_components+1)))
    #collect the corresponding eigenvalues
    #保存特征值用于新样本映射
    lambdas = [eigvals[-i] for i in range(1,n_components+1)]

    return X_pc,lambdas

#映射新样本，新样本和训练集中所有样本计算相似度，并生成新核矩阵
def project_x(x_new,X,gamma,X_kpca,lambdas):
    pair_dist = np.array([np.sum((x_new-row)**2) for row in X])
    k=np.exp(-gamma * pair_dist)
    return k.dot(X_kpca/lambdas) #新核矩阵映射的低维空间返回值

#生成半月形数据，并映射到低维空间
X,y=make_moons(n_samples=100,random_state=123)
X_kpca,lambdas= rbf_kernel_pca (X,gama=15,n_components=1)#使用rbf+pca将样本映射到一维的子空间上
#新样本映射
x_new=X[25]#取数据集中的低26个样本作为新样本
print (x_new)
x_proj= X_kpca[25] #在训练集中映射的值
print (x_proj)
x_reproj=project_x(x_new,X,gamma=15,X_kpca=X_kpca,lambdas=lambdas)#新样本加入后映射的值
print (x_reproj)
#可视化效果
plt.scatter(X_kpca[y==0,0],np.zeros((50)),color='red',marker='^',alpha=0.5)
plt.scatter(X_kpca[y==1,0],np.zeros((50)),color='blue',marker='o',alpha=0.5)
plt.scatter(x_proj,0,color='black',label='original projection of point X[25]',marker='^',s=100)
plt.scatter(x_reproj,0,color='green',label='remapped point X[25]',marker='x',s=500)
plt.legend(scatterpoints=1)
plt.show()
```


结果：

![](https://img-blog.csdn.net/20180118155642142?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




