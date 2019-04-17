# 动手写机器学习算法：PCA主成分分析（降维） - 知乎
# 



 有时我们的数据中包括很多属性，有些是没意义的，有些是重复的，有些组合后意义更明显。此时，我们需要简化属性节约算力，去噪，去冗余，求取更典型的属性，同时又希望不损失数据本身的意义。
  主成分分析（Principal Component Analysis，PCA），是一种统计方法。通过正交变换将一组可能存在相关性的变量转换为一组线性不相关的变量，转换后的这组变量叫主成分。数据分析中常使用PCA给数据降维，它能在指定的损失范围内最大的简化属性。本篇将用python实现主成分分析降维，以便大家更好理解PCA。

**全部代码**

> [https://github.com/lawlite19/MachineLearning_Python/blob/master/PCA/PCA.py](https://link.zhihu.com/?target=https%3A//github.com/lawlite19/MachineLearning_Python/blob/master/PCA/PCA.py)




**用处**

数据压缩（Data Compression）,使程序运行更快

可视化数据，例如3D-->2D等

......

**2D-->1D，nD-->kD**

如下图所示，所有数据点可以投影到一条直线，是投影距离的平方和（投影误差）最小 
![](https://pic1.zhimg.com/v2-abb66b6f755f3729fe41f497caf8930c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='565' height='440'></svg>)
注意数据需要归一化处理

思路是找1个向量u,所有数据投影到上面使投影距离最小

那么nD-->kD就是找k个向量
![](https://pic2.zhimg.com/v2-a7687e7784fa813463148408695a498d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='116' height='22'></svg>)
，

所有数据投影到上面使投影误差最小

eg:3D-->2D,2个向量

就代表一个平面了，所有点投影到这个平面的投影误差最小即可




**主成分分析PCA与线性回归的区别**

线性回归是找x与y的关系，然后用于预测y

PCA是找一个投影面，最小化data到这个投影面的投影误差




**PCA降维过程**

**数据预处理（均值归一化）**

公式：
![](https://pic4.zhimg.com/v2-d97278d9ad753126aa2011afc71474db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='115' height='53'></svg>)
就是减去对应feature的均值，然后除以对应特征的标准差（也可以是最大值-最小值）




实现代码：

> # 归一化数据
  def featureNormalize(X):
      '''（每一个数据-当前列的均值）/当前列的标准差'''
      n = X.shape[1]
      mu = np.zeros((1,n));
      sigma = np.zeros((1,n))

      mu = np.mean(X,axis=0)
      sigma = np.std(X,axis=0)
      for i in range(n):
          X[:,i] = (X[:,i]-mu[i])/sigma[i]
      return X,mu,sigma




计算协方差矩阵Σ（Covariance Matrix）：
![](https://pic3.zhimg.com/v2-66c61b2d84b5236307458dad3042b0c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='164' height='51'></svg>)
注意这里的Σ和求和符号不同 




**协方差矩阵对称正定（不理解正定的看看线代）**

大小为nxn,n为feature的维度

实现代码：

> Sigma = np.dot(np.transpose(X_norm),X_norm)/m  # 求Sigma 

计算Σ的特征值和特征向量

可以是用svd奇异值分解函数：U,S,V = svd(Σ)

返回的是与Σ同样大小的对角阵S（由Σ的特征值组成）[注意：matlab中函数返回的是对角阵，在python中返回的是一个向量，节省空间]

还有两个酉矩阵U和V，且
![](https://pic1.zhimg.com/v2-6818b4be57c32baef62b210c205d3b74_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='385' height='171'></svg>)
注意：svd函数求出的S是按特征值降序排列的，若不是使用svd,需要按特征值大小重新排列U

**降维**

选取U中的前K列（假设要降为K维）
![](https://pic1.zhimg.com/v2-8589a39a8436a9fe43c6d77de97380ac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='221'></svg>)
Z就是对应降维之后的数据


实现代码：

> # 映射数据
  def projectData(X_norm,U,K):
      Z = np.zeros((X_norm.shape[0],K))

      U_reduce = U[:,0:K]          # 取前K个
      Z = np.dot(X_norm,U_reduce) 
      return Z




**过程总结：**

Sigma = X'*X/m

U,S,V = svd(Sigma)

Ureduce = U[:,0:k]

Z = Ureduce'*x

**数据恢复**

因为：
![](https://pic2.zhimg.com/v2-17e33a0263ceaed2e6887a54bcd461e5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='146' height='22'></svg>)
，

所以：
![](https://pic1.zhimg.com/v2-ff3d30a36e63782e17c781ddbfdb30b4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='167' height='22'></svg>)
（注意这里是X的近似值）

又因为Ureduce为正定矩阵，【正定矩阵满足：
![](https://pic1.zhimg.com/v2-13696a4e71bb1ebb52584a07060278c4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='131' height='16'></svg>)
，所以：
![](https://pic4.zhimg.com/v2-7ad8fad0b7fe232f217ff0330e64893f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='76' height='16'></svg>)
】，

所以这里：
![](https://pic3.zhimg.com/v2-32abfe2db68fc7615463d938c83fbb52_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='253' height='22'></svg>)



实现代码：

> # 恢复数据 
   def recoverData(Z,U,K):
       X_rec = np.zeros((Z.shape[0],U.shape[0]))
       U_recude = U[:,0:K]
       X_rec = np.dot(Z,np.transpose(U_recude))  # 还原数据（近似）
       return X_rec




**主成分个数的选择（即要降的维度）**

**如何选择**

投影误差（project error）：
![](https://pic2.zhimg.com/v2-57d8c4c5c2c7e5e396107302444ddf95_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='168' height='51'></svg>)
总变差（total variation）:
![](https://pic2.zhimg.com/v2-d436fc0d72a0a38e36bd0de3b1430515_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='100' height='51'></svg>)
若误差率（error ratio）：
![](https://pic1.zhimg.com/v2-c7bbbf82b03265464f91dbb6ee042ebc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='219' height='87'></svg>)
，则称99%保留差异性

误差率一般取1%，5%，10%等




**如何实现**

若是一个个试的话代价太大

之前U,S,V = svd(Sigma),我们得到了S，这里误差率error ratio:
![](https://pic4.zhimg.com/v2-fd08b363b987b468c77fca393cc7d2cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='272' height='90'></svg>)
可以一点点增加K尝试。




**使用建议**

不要使用PCA去解决过拟合问题Overfitting，还是使用正则化的方法（如果保留了很高的差异性还是可以的）

只有在原数据上有好的结果，但是运行很慢，才考虑使用PCA




**运行结果**

**2维数据降为1维**

要投影的方向
![](https://pic2.zhimg.com/v2-af91f4aa05d71ad78dfa3ff34bd967c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='515' height='518'></svg>)



2D降为1D及对应关系
![](https://pic2.zhimg.com/v2-d2a4ef2e78df10e01398cbc7d6c19899_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='533' height='520'></svg>)
**人脸数据降维**

原始数据
![](https://pic2.zhimg.com/v2-73f196b23d8cb94b170e1da33b853ee9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='436' height='432'></svg>)
可视化部分U矩阵信息
![](https://pic2.zhimg.com/v2-6536ae22f2b814b5ca594104bb27306d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='418' height='414'></svg>)
恢复数据
![](https://pic3.zhimg.com/v2-63949904c60a30ec098a460fb486bdd6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='579' height='582'></svg>)



**使用scikit-learn库中的PCA实现降维**




> [https://github.com/lawlite19/MachineLearning_Python/blob/master/PCA/PCA.py_scikit-learn.py](https://link.zhihu.com/?target=https%3A//github.com/lawlite19/MachineLearning_Python/blob/master/PCA/PCA.py_scikit-learn.py)




导入需要的包：

> #-*- coding: utf-8 -*-
# Author:bob
# Date:2016.12.22
import numpy as np
from matplotlib import pyplot as plt
from scipy import io as spio
from sklearn.decomposition import pca
from sklearn.preprocessing import StandardScaler




归一化数据

> '''归一化数据并作图'''     
scaler = StandardScaler()     
scaler.fit(X)    
x_train = scaler.transform(X) 




使用PCA模型拟合数据，并降维

> n_components对应要将的维度

'''拟合数据'''
K=1 # 要降的维度
model = pca.PCA(n_components=K).fit(x_train)   # 拟合数据，n_components定义要降的维度
Z = model.transform(x_train)    # transform就会执行降维操作




数据恢复

model.components_会得到降维使用的U矩阵

> '''数据恢复并作图'''     
Ureduce = model.components_     # 得到降维用的Ureduce     
x_rec = np.dot(Z,Ureduce)       # 数据恢复 




作者：lawlite19 

[https://github.com/lawlite19/MachineLearning_Python#](https://link.zhihu.com/?target=https%3A//github.com/lawlite19/MachineLearning_Python%23)


