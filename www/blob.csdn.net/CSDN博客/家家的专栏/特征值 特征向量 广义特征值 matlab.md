# 特征值 特征向量 广义特征值 matlab - 家家的专栏 - CSDN博客





2011年03月25日 16:02:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：16538








**d=eig(A)**www.iLoveMatlab.cn

返回矩阵A特征值的一个向量d。 
**d=eig(A,B)**

www.iLoveMatlab.cn



如果A和B是方阵的，返回包含广义特征的向量。 
注意：如果S是稀疏对称的，用d = eig(S) 可以返回S的特征值。如果S是稀疏但不对称，若要想得到S的特征向量，用函数eigs代替eig。 
**[V,D]=eig(A)**book.iLoveMatlab.cn

计算矩阵A的特征值D和特征矩阵V，满足A*V = V*D。矩阵D是矩阵A的规范形式：主对角线上的元素是矩阵A的特征值的对角矩阵。 
**[V,D] = eig(A,'nobalance')**
当矩阵A中有与截断误差数量级相差不远的值时，改命令可能更加精确，'nobalance'起误差调节作用。 


**[V,D] = eig(A,B)**
计算广义特征向量矩阵V和广义特征值矩阵D，满足A*V = B*V*D 。 
**[V,D] = eig(A,B,flag)**
由flag指定算法来计算特征值D和特征向量V，flag的可能值为： 
'chol'：表示对B进行cholesky分解算法，这里A是对称的Hermitian矩阵，B为正定矩阵。 Matlab中文论坛
'qz'：表示使用qz分解算法，这里A、B是非对称或非Hermitian矩阵。



广义特征值是求解Ax=λBx的非0解,满足的λ值。当A,B是方阵就可以用eig(A,B)求解

特征值是求解Ax=λx的非0解,满足的λ值。当A是方阵 则可用eig(A)求解 

1. eig(pinv(A)*B) 是求解 pinv(A)*Bx=λx 就是 A-1Bx=λx
2. eig(A,B)  是求解 Ax=λBx  就是 B-1Ax=λx  





在MATLAB中，计算矩阵A的特征值和特征向量的函数是eig(A)，常用的调用格式有

5种：
(1) E=eig(A)：求矩阵A的全部特征值，构成向量E。
(2) [V,D]=eig(A)：求矩阵A的全部特征值，构成对角阵D，并求A的特征向量构成

V的列向量。
(3) [V,D]=eig(A,'nobalance')：与第2种格式类似，但第2种格式中先对A作相似

变换后求矩阵A的特征值和特征向量，而格式3直接求矩阵A的特征值和特征向量。

(4) E=eig(A,B)：由eig(A,B)返回N×N阶方阵A和B的N个广义特征值，构成向量E

。
(5) [V,D]=eig(A,B)：由eig(A,B)返回方阵A和B的N个广义特征值，构成N×N阶对

角阵D，其对角线上的N个元素即为相应的广义特征值，同时将返回相应的特征向

量构成N×N阶满秩矩阵，且满足AV=BVD。

转自：[http://hi.baidu.com/lingquan324/blog/item/98d33f7ab5b2c8f92e73b382.html](http://hi.baidu.com/lingquan324/blog/item/98d33f7ab5b2c8f92e73b382.html)



