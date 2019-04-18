# K-L变换 - YZXnuaa的博客 - CSDN博客
2018年04月12日 18:58:42[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1210
#### **K-L变换的理论知识**
K-L变换是除了PCA外的另一种常用的特征提取方法，它有很多种形式，最基本的形式跟PCA类似，它跟PCA的不同在于，PCA是一种无监督的特征变换，而K-L变换能够考虑到不同的分类信息，实现有监督的特征提取。
根据随机过程中的KL展开理论，将随机过程描述为无数个正交函数的线性组合，而在模式识别问题中，通常可以将一个样本看成是随机向量的某一次实现结果，所以假设有一d维随机向量x，可以写成一组正交基![](http://latex.codecogs.com/gif.latex?%5Csigma_%7Bi%7D%20%2Ci%20%3D%201%2C%5Ccdots%20%2C%5Cinfty)的线性组合，且它们的模为1：
![](http://latex.codecogs.com/gif.latex?x%3D%5Csum_%7Bi%3D1%7D%5E%7B%5Cinfty%20%7Dy_%7Bi%7D%5Csigma_%7Bi%7D)
对上式变形得到：
![](http://latex.codecogs.com/gif.latex?y_%7Bi%7D%3D%5Csigma_%7Bi%7D%5E%7BT%7Dx)                           （**初见K-L变换，通常需要先对样本进行零均值化或平移**）
假设有用信息就集中在其中的q维上，那么现在我们来尝试用着q维去近似x：
![](http://latex.codecogs.com/gif.latex?%7Bx%7D%27%3D%5Csum_%7Bi%3D1%7D%5E%7Bq%7Dy_%7Bi%7D%5Csigma_%7Bi%7D)
近似前后样本向量的差向量为：![](http://latex.codecogs.com/gif.latex?x-%7Bx%7D%27)
考查上述差向量的均方误差（MSE）为：
![](http://latex.codecogs.com/gif.latex?e%3DE%5Cleft%20%5C%7B%20%28x-%7Bx%7D%27%29%28x-%7Bx%7D%27%29%5E%7BT%7D%20%5Cright%20%5C%7D%3DE%5Cleft%20%5C%7B%20%28%5Csum_%7Bi%3Dq&plus;1%7D%5E%7B%5Cinfty%20%7Dy_%7Bi%7D%5Csigma_%7Bi%7D%29%28%5Csum_%7Bi%3Dq&plus;1%7D%5E%7B%5Cinfty%20%7Dy_%7Bi%7D%5Csigma_%7Bi%7D%29%5E%7BT%7D%20%5Cright%20%5C%7D%3DE%5Cleft%20%5C%7B%20%5Csum_%7Bi%3Dq&plus;1%7D%5E%7B%5Cinfty%20%7Dy_%7Bi%7Dy_%7Bi%7D%5E%7BT%7D%20%5Cright%20%5C%7D%3D%5Csum_%7Bi%3Dq&plus;1%7D%5E%7B%5Cinfty%20%7D%5Csigma_%7Bi%7D%5E%7BT%7DE%5Cleft%20%5C%7B%20xx%5E%7BT%7D%20%5Cright%20%5C%7D%5Csigma_%7Bi%7D%3D%5Csum_%7Bi%3Dq&plus;1%7D%5E%7B%5Cinfty%20%7D%5Csigma_%7Bi%7D%5E%7BT%7D%5CSigma_%7BX%7D%20%5Csigma_%7Bi%7D)
其中，变换矩阵![](http://latex.codecogs.com/gif.latex?%5CSigma_%7BX%7D)是原样本向量x的二阶矩阵（注意，这里还可以是其他矩阵，如协方差矩阵），可以与PCA中比较一下，形式大致相同，但在PCA中使用的变换矩阵是协方差矩阵；
我们的目的是最小化上述MSE，同PCA中的求解方法，得到下面拉格朗日目标函数：
![](http://latex.codecogs.com/gif.latex?g%28%5Csigma%20%29%3D%5Csum_%7Bi%3Dq&plus;1%7D%5E%7B%5Cinfty%20%7D%5Csigma_%7Bi%7D%5E%7BT%7D%5CSigma_%7BX%7D%20%5Csigma_%7Bi%7D-%5Csum_%7Bi%3Dq&plus;1%7D%5E%7B%5Cinfty%20%7D%5Clambda_%7Bi%7D%20%28%5Csigma_%7Bi%7D%5E%7BT%7D%5Csigma_%7Bi%7D-1%29)
对sigma求导并令其等于零，有：
![](http://latex.codecogs.com/gif.latex?%28%5CSigma_%7BX%7D%20-%5Clambda_%7Bi%7DI%29%20%5Csigma_%7Bi%7D%3D0%2Ci%3Dq&plus;1%2C%5Ccdots%20%2C%5Cinfty)
看到熟悉的面孔了，哈哈，![](http://latex.codecogs.com/gif.latex?%5Clambda_%7Bi%7D)就是![](http://latex.codecogs.com/gif.latex?%5CSigma_%7BX%7D)的特征值，所以上面要求的均方误差就解开了神秘的面纱：
![](http://latex.codecogs.com/gif.latex?e%3D%5Csum_%7Bi%3Dq&plus;1%7D%5E%7B%5Cinfty%20%7D%5Clambda_%7Bi%7D)
分析到这里，应该不难看出，简直跟PCA就是一对双胞胎啊，太像了有木有，其实当K-L变换的变换矩阵![](http://latex.codecogs.com/gif.latex?%5CSigma_%7BX%7D)为协方差矩阵时，K-L变换就变成了PCA。
回到使用q维逼近样本向量x的问题上来，通过上面的分析我们知道了，如果想用q维来表示样本向量并使MSE最小化，合理的做法就是：把变换矩阵![](http://latex.codecogs.com/gif.latex?%5CSigma_%7BX%7D)的特征值从大到小排列，然后选择前q个特征值对应的特征向量就行，此时截断误差能够保证最小，其中![](http://latex.codecogs.com/gif.latex?%5Csigma_%7Bi%7D%20%2Ci%20%3D%201%2C%5Ccdots%20%2C%5Cinfty)中的前q个正交向量就组成了新的特征空间，而原样本向量x在这个新特征空间上的展开系数yi就组成了新的特征向量，**这种变换就叫做K-L变换**，对于它的其他不同的形式，主要是基于变换矩阵![](http://latex.codecogs.com/gif.latex?%5CSigma_%7BX%7D)的具体形式。
可以发现，得到这q个新特征与PCA中的d个主成分是类似的，当对原特征x进行中心化时，K-L变换等价于PCA；
#### **K-L变换的几个重要性质**
1.变换后得到的新特征满足零均值：
证明：
设有如下K-L变换：![](http://latex.codecogs.com/gif.latex?Y%3DA%28X-m_%7Bx%7D%29)，其中矩阵A是变换核矩阵；
对X的变换结果Y球其均值：
![](http://latex.codecogs.com/gif.latex?m_%7By%7D%3DE%5Cleft%20%5C%7B%20Y%20%5Cright%20%5C%7D%3DE%5Cleft%20%5C%7BA%20%28X-m_%7Bx%7D%20%29%5Cright%20%5C%7D%3DAE%5Cleft%20%5C%7B%20X-m_%7Bx%7D%20%5Cright%20%5C%7D%3DAE%5Cleft%20%5C%7B%20X%20%5Cright%20%5C%7D-Am_%7Bx%7D%3D0)
2.K-L变换是一种正交变换；
3.K-L变换的新特征彼此之间不相关；
4.K-L变换的新特征向量的二阶矩阵是对角阵，且对角线元素就是原特征的二阶矩阵的特征值；
证明：
![](http://latex.codecogs.com/png.latex?%5CSigma_%7B%5Cxi%20%7D%20%3DE%5Cleft%20%5B%20%5Cxi%5Cxi%5E%7BT%7D%20%5Cright%20%5D%3DE%5Cleft%20%5B%20A%5E%7BT%7D%28x-u%29%28A%5E%7BT%7D%28x-u%29%20%29%5E%7BT%7D%20%5Cright%20%5D%3DE%5Cleft%20%5B%20A%28x-u%29%20%28x-u%29%5E%7BT%7DA%5E%7BT%7D%5Cright%20%5D%3DAE%5Cleft%20%5B%20%28x-u%29%20%28x-u%29%5E%7BT%7D%5Cright%20%5DA%5E%7BT%7D%3DA%5CSigma%20A%5E%7BT%7D%3D%5Cbegin%7Bbmatrix%7D%20%5Cgamma_%7B1%7D%20%26%20%26%20%26%20%5C%5C%20%26%20%5Cgamma_%7B2%7D%20%26%20%26%20%5C%5C%20%26%20%26%20%5Cddots%20%26%20%5C%5C%20%26%20%26%20%26%20%5Cgamma_%7Bp%7D%20%5Cend%7Bbmatrix%7D)
5.K-L变换是信号的最佳压缩表示，用q维新特征表示原样本特征带来的误差在所有q维正交坐标变换中最小；
6.用K-L坐标系来表示原数据，意味着熵最小，即样本的方差信息最大程度的集中在较少的维数上；
#### **K-L变换与PCA的联系与区别**
联系：
都属于正交变换；
当对原特征x进行中心化时（即变换矩阵为协方差矩阵），K-L变换等价于PCA；
PCA是离散K-L变换；
都可以实现降维变换；
区别：
K-L变换可以实现有监督的特征提取，但是PCA的变换是一种无监督的；
在含义上，K-L变换较广义，PCA较狭义；
K-L变换可以处理连续和离散情况，而PCA只针对离散情况；
K-L变换的变换矩阵可以是很多种，如二阶矩阵、协方差矩阵（总体散布矩阵）等，或者说自相关矩阵，而PCA的变换矩阵就是协方差矩阵；
但是，在一些地方就认为两者是没什么区别的，因为实际应用中不管是协方差矩阵，还是自相关矩阵，其实只是差了个对样本进行均值的平移，但是在PCA中这种平移并不会影响主成分的方向，所以PCA中也通常会先对样本平移，这样看来自相关矩阵就变成了协方差矩阵。
协方差矩阵：
![](http://latex.codecogs.com/gif.latex?C_%7Bx%7D%3DE%5Cleft%20%5C%7B%20%28x-m_%7Bx%7D%29%28x-m_%7Bx%7D%29%20%5E%7BT%7D%20%5Cright%20%5C%7D)
自相关矩阵：
![](https://img-blog.csdn.net/20160623171543710)
其中，![](http://latex.codecogs.com/gif.latex?x%5E%7BH%7D)是共轭转置矩阵，当为实矩阵时，等价于转置矩阵；
协方差矩阵和自相关矩阵的关系：
![](https://img-blog.csdn.net/20160623171813296)
