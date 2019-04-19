# 机器学习之支持向量机（support vector machine）（一）：线性可分SVM - sinat_31425585的博客 - CSDN博客
2018年09月14日 13:42:27[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：72
个人分类：[machine learning](https://blog.csdn.net/sinat_31425585/article/category/7309303)
    总结一下，不然过段时间就全忘了，加油~
**    1、问题描述**
    假设，存在两类数据A，B，如图1所示，A中数据对应于图中的实心点，B中数据对应图中的空心点，现在我们需要得到一条直线，能够将二者进行区分，这样的线存在无数条，如图1中的黑色直线所示，这些线都能够分对。
![](https://img-blog.csdn.net/20180914091932630?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                图1 A，B两类数据分布示意图
    因此，在分对的前提下，我们希望分得最好，那么什么样的直线能够分得最好？直观上来说，就是在分对前提下，分割线尽量离两个类别样本数据远，如图2所示。
![](https://img-blog.csdn.net/20180914093835725?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                            图2 最优分割示意图
    而决定最优分割线的样本为距离分割线最近的样本点（位于两条虚线上的样本点），这些样本点就是支持向量（每一个样本点都为一个向量），由这些支持向量决策，获取得到的最优分割线即为支持向量机。这是一个二维数据，对应最优分割线，对于多维数据，对应的就是分割超平面。
**    2、求解思路**
    由问题可知，最优分割超平面由位于分割超平面两侧的支持向量(support vector)决定，而支持向量对应于距离分割超平面最近的样本点，那么，如果假定分割超平面已知，那么支持向量即到分割超平面最近的样本点。
     我们还是以二维数据为例，首先，回顾一下高中知识，已知一个点![(x_{0},y_{0})](https://private.codecogs.com/gif.latex?%28x_%7B0%7D%2Cy_%7B0%7D%29)和一条直线![Ax+By+C=0](https://private.codecogs.com/gif.latex?Ax&plus;By&plus;C%3D0)，求点到直线的距离有：
![d=\frac{Ax_{0}+By_{0}+C}{\sqrt{A^{2}+B^{2}}}](https://private.codecogs.com/gif.latex?d%3D%5Cfrac%7BAx_%7B0%7D&plus;By_%7B0%7D&plus;C%7D%7B%5Csqrt%7BA%5E%7B2%7D&plus;B%5E%7B2%7D%7D%7D)
    如果取绝对值，就是对应距离，这里d的符号其实表征了当前点与分割线之间的位置关系，如果点位于分割线的正向，d的符号为正，如果点位于分割线的负向，d的符号就为负。
    我们将d进行展开有：
![d=\frac{Ax_{0}}{\sqrt{A^{2}+B^{2}}} + \frac{By_{0}}{\sqrt{A^{2}+B^{2}}} +\frac{C}{\sqrt{A^{2}+B^{2}}}](https://private.codecogs.com/gif.latex?d%3D%5Cfrac%7BAx_%7B0%7D%7D%7B%5Csqrt%7BA%5E%7B2%7D&plus;B%5E%7B2%7D%7D%7D%20&plus;%20%5Cfrac%7BBy_%7B0%7D%7D%7B%5Csqrt%7BA%5E%7B2%7D&plus;B%5E%7B2%7D%7D%7D%20&plus;%5Cfrac%7BC%7D%7B%5Csqrt%7BA%5E%7B2%7D&plus;B%5E%7B2%7D%7D%7D)
    令：
![\vec{W}=\binom{\frac{A}{\sqrt{A^{2}+B^{2}}}}{\frac{B}{\sqrt{A^{2}+B^{2}}}} , \vec{X}=\binom{x}{y},b=\frac{C}{\sqrt{A^{2}+B^{2}}}](https://private.codecogs.com/gif.latex?%5Cvec%7BW%7D%3D%5Cbinom%7B%5Cfrac%7BA%7D%7B%5Csqrt%7BA%5E%7B2%7D&plus;B%5E%7B2%7D%7D%7D%7D%7B%5Cfrac%7BB%7D%7B%5Csqrt%7BA%5E%7B2%7D&plus;B%5E%7B2%7D%7D%7D%7D%20%2C%20%5Cvec%7BX%7D%3D%5Cbinom%7Bx%7D%7By%7D%2Cb%3D%5Cfrac%7BC%7D%7B%5Csqrt%7BA%5E%7B2%7D&plus;B%5E%7B2%7D%7D%7D)
     那么，如果样本点进行了归一化处理，任意样本点到分割超平面的距离就是：
![y=\vec{W}^{T}\vec{X}+b](https://private.codecogs.com/gif.latex?y%3D%5Cvec%7BW%7D%5E%7BT%7D%5Cvec%7BX%7D&plus;b)
    在求得支持向量之后，调整![\vec{W}](https://private.codecogs.com/gif.latex?%5Cvec%7BW%7D)和![b](https://private.codecogs.com/gif.latex?b)，使得margin最大，因此，表示成数学公式有：
![\vec{W}^{*} ,b^{*}=arg\ max_{j}\ min_{i}\ \vec{W}^{T}_{j}\vec{X}_{i}+b_{j}](https://private.codecogs.com/gif.latex?%5Cvec%7BW%7D%5E%7B*%7D%20%2Cb%5E%7B*%7D%3Darg%5C%20max_%7Bj%7D%5C%20min_%7Bi%7D%5C%20%5Cvec%7BW%7D%5E%7BT%7D_%7Bj%7D%5Cvec%7BX%7D_%7Bi%7D&plus;b_%7Bj%7D)
    由于直线![\vec{W},b](https://private.codecogs.com/gif.latex?%5Cvec%7BW%7D%2Cb)取值有无穷多个，用这个思路来进行求解是行不通的。
**    3、目标函数**
    我们假定分割超平面为![y(\vec{x})=\vec{W}^{T}\phi (\vec{X})+b](https://private.codecogs.com/gif.latex?y%28%5Cvec%7Bx%7D%29%3D%5Cvec%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D%29&plus;b)，有：![y(\vec{x}_{i})>0,y_{i}=+1;y(\vec{x}_{i})>0,y_{i}=-1](https://private.codecogs.com/gif.latex?y%28%5Cvec%7Bx%7D_%7Bi%7D%29%3E0%2Cy_%7Bi%7D%3D&plus;1%3By%28%5Cvec%7Bx%7D_%7Bi%7D%29%3E0%2Cy_%7Bi%7D%3D-1)，因此有：![y(\vec{x})y_{i}>0](https://private.codecogs.com/gif.latex?y%28%5Cvec%7Bx%7D%29y_%7Bi%7D%3E0)。那么，任意一个样本点到分割超平面的距离为：
![\frac{y(\vec{x}_{i})y_{i}}{||\vec{W}||}=\frac{(\vec{W}^{T}\phi (\vec{X}_{i})+b)y_{i}}{||\vec{W}||}](https://private.codecogs.com/gif.latex?%5Cfrac%7By%28%5Cvec%7Bx%7D_%7Bi%7D%29y_%7Bi%7D%7D%7B%7C%7C%5Cvec%7BW%7D%7C%7C%7D%3D%5Cfrac%7B%28%5Cvec%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29&plus;b%29y_%7Bi%7D%7D%7B%7C%7C%5Cvec%7BW%7D%7C%7C%7D)
    那么，我们优化的目标函数就变成了：
![arg\ max_{\vec{W},b} \ \frac{1}{||\vec{W}||} \ min_{i} \ (\vec{W}^{T}\phi (\vec{X}_{i})+b)y_{i}](https://private.codecogs.com/gif.latex?arg%5C%20max_%7B%5Cvec%7BW%7D%2Cb%7D%20%5C%20%5Cfrac%7B1%7D%7B%7C%7C%5Cvec%7BW%7D%7C%7C%7D%20%5C%20min_%7Bi%7D%20%5C%20%28%5Cvec%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29&plus;b%29y_%7Bi%7D)
    由于直线方程乘以某一个不为零的缩放因子f后，不会改变方程，所以，可以通过对直线方程乘以某个缩放因子，使得满足：  
![(\vec{W}^{T}\phi (\vec{X}_{i})+b)y_{i}>=1](https://private.codecogs.com/gif.latex?%28%5Cvec%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29&plus;b%29y_%7Bi%7D%3E%3D1)
    那么，目标函数就可以写成：
![arg\ min_{\vec{W},b} \ ||\vec{W}||](https://private.codecogs.com/gif.latex?arg%5C%20min_%7B%5Cvec%7BW%7D%2Cb%7D%20%5C%20%7C%7C%5Cvec%7BW%7D%7C%7C)
![s.t. \ (\vec{W}^{T}\phi (\vec{X}_{i})+b)y_{i}>=1](https://private.codecogs.com/gif.latex?s.t.%20%5C%20%28%5Cvec%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29&plus;b%29y_%7Bi%7D%3E%3D1)
**   4、目标函数求解**
    求解这种带约束的极值问题，引入拉格朗日乘子，有：
![L(\vec{w},b,\alpha )=\frac{1}{2}||\vec{w}||^{2}-\sum_{i=0}^{N}\alpha {_{i}}((\vec{W}^{T}\phi (\vec{X}_{i})+b)y_{i}-1)](https://private.codecogs.com/gif.latex?L%28%5Cvec%7Bw%7D%2Cb%2C%5Calpha%20%29%3D%5Cfrac%7B1%7D%7B2%7D%7C%7C%5Cvec%7Bw%7D%7C%7C%5E%7B2%7D-%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20%7B_%7Bi%7D%7D%28%28%5Cvec%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29&plus;b%29y_%7Bi%7D-1%29)
    其中，![\alpha _{i}>0,i=1,2,...N](https://private.codecogs.com/gif.latex?%5Calpha%20_%7Bi%7D%3E0%2Ci%3D1%2C2%2C...N)，由于![-(\vec{W}^{T}\phi (\vec{X}_{i})+b)y_{i}-1<=0](https://private.codecogs.com/gif.latex?-%28%5Cvec%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29&plus;b%29y_%7Bi%7D-1%3C%3D0)，因此，![\alpha {_{i}}](https://private.codecogs.com/gif.latex?%5Calpha%20%7B_%7Bi%7D%7D)越大，L越小。因此，目标函数就转换为：
![min_{\vec{w},b} \ max_{\alpha _{i}} \ L(\vec{w},b,\alpha )](https://private.codecogs.com/gif.latex?min_%7B%5Cvec%7Bw%7D%2Cb%7D%20%5C%20max_%7B%5Calpha%20_%7Bi%7D%7D%20%5C%20L%28%5Cvec%7Bw%7D%2Cb%2C%5Calpha%20%29)
    等价于其对偶问题：
![\ max_{\alpha _{i}}min_{\vec{w},b} \ L(\vec{w},b,\alpha )](https://private.codecogs.com/gif.latex?%5C%20max_%7B%5Calpha%20_%7Bi%7D%7Dmin_%7B%5Cvec%7Bw%7D%2Cb%7D%20%5C%20L%28%5Cvec%7Bw%7D%2Cb%2C%5Calpha%20%29)
    因此，分别用L对![\vec{w},b](https://private.codecogs.com/gif.latex?%5Cvec%7Bw%7D%2Cb)求偏导，并令其等于零有：
![\vec{w}=\sum_{i=0}^{N}\alpha _{i}y_{i}\phi (\vec{X}_{i})](https://private.codecogs.com/gif.latex?%5Cvec%7Bw%7D%3D%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29)
![\sum_{i=0}^{N}\alpha _{i}y_{i}=0](https://private.codecogs.com/gif.latex?%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%3D0)
    回代到L中有：
![L(\vec{w},b,\alpha )=\sum_{i=0}^{N}\alpha _{i} -\frac{1}{2}\sum_{i=0}^{N}\sum_{j=0}^{N}\alpha _{i}\alpha _{j}y_{i}y_{j}\phi (\vec{X}_{i})^{T}\phi (\vec{X}_{j})](https://private.codecogs.com/gif.latex?L%28%5Cvec%7Bw%7D%2Cb%2C%5Calpha%20%29%3D%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%20-%5Cfrac%7B1%7D%7B2%7D%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Csum_%7Bj%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%5Calpha%20_%7Bj%7Dy_%7Bi%7Dy_%7Bj%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bj%7D%29)
   那么最优的![\alpha ^{*}](https://private.codecogs.com/gif.latex?%5Calpha%20%5E%7B*%7D)为：
![\alpha ^{*}=max_{\alpha _{i}} (\sum_{i=0}^{N}\alpha _{i} -\frac{1}{2}\sum_{i=0}^{N}\sum_{j=0}^{N}\alpha _{i}\alpha _{j}y_{i}y_{j}\phi (\vec{X}_{i})^{T}\phi (\vec{X}_{j}))](https://private.codecogs.com/gif.latex?%5Calpha%20%5E%7B*%7D%3Dmax_%7B%5Calpha%20_%7Bi%7D%7D%20%28%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%20-%5Cfrac%7B1%7D%7B2%7D%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Csum_%7Bj%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%5Calpha%20_%7Bj%7Dy_%7Bi%7Dy_%7Bj%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bj%7D%29%29)
![s.t. \sum_{i=0}^{N}\alpha _{i}y_{i}=0](https://private.codecogs.com/gif.latex?s.t.%20%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%3D0)
![\alpha _{i}>=0,i=1,2,...,N](https://private.codecogs.com/gif.latex?%5Calpha%20_%7Bi%7D%3E%3D0%2Ci%3D1%2C2%2C...%2CN)
   求解![\alpha ^{*}](https://private.codecogs.com/gif.latex?%5Calpha%20%5E%7B*%7D)过程，使用的SMO算法，这个后面补充，这里，假设我们已经求得最优的![\alpha ^{*}](https://private.codecogs.com/gif.latex?%5Calpha%20%5E%7B*%7D)，那么最优超平面参数怎么求？在取得最优![\alpha ^{*}](https://private.codecogs.com/gif.latex?%5Calpha%20%5E%7B*%7D)时，支持向量![\vec {x}_{i}](https://private.codecogs.com/gif.latex?%5Cvec%20%7Bx%7D_%7Bi%7D)满足：
![(\vec{W}^{T}\phi (\vec{X}_{i})+b)y_{i}-1=0](https://private.codecogs.com/gif.latex?%28%5Cvec%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29&plus;b%29y_%7Bi%7D-1%3D0)
   因为，![y_{i}y_{i}=1](https://private.codecogs.com/gif.latex?y_%7Bi%7Dy_%7Bi%7D%3D1)，则：
![b = y_{i}-\vec {W}^{T}\phi (\vec{X}_{i})](https://private.codecogs.com/gif.latex?b%20%3D%20y_%7Bi%7D-%5Cvec%20%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29)
   将![\vec{w}](https://private.codecogs.com/gif.latex?%5Cvec%7Bw%7D)代入有：
![\vec{w}=\sum_{i=0}^{N}\alpha _{i}y_{i}\phi (\vec{X}_{i})](https://private.codecogs.com/gif.latex?%5Cvec%7Bw%7D%3D%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29)
![b = y_{i}-\sum_{i=0}^{N}\alpha _{i}y_{i}\phi (\vec{X}_{i})\phi (\vec{X}_{j})](https://private.codecogs.com/gif.latex?b%20%3D%20y_%7Bi%7D-%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29%5Cphi%20%28%5Cvec%7BX%7D_%7Bj%7D%29)
 未完待续~~~~
参考资料：
 2016小象学院-机器学习升级版II（邹博）
Abramson N, Braverman D, Sebestyen G. Pattern Recognition and Machine Learning[M]. Springer, 2006. 
