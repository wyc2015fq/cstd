# 机器学习之支持向量机（support vector machine）（二）：线性SVM - sinat_31425585的博客 - CSDN博客
2018年09月14日 17:32:03[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：63
个人分类：[machine learning](https://blog.csdn.net/sinat_31425585/article/category/7309303)
**   一、带松弛因子的SVM **
   通常情况下，存在以下两种情况：
   1、分类完全正确的超平面不一定是最好的；
   2、样本数据不是线性可分的，类别之间数据存在重合；（Pattern Recongnition and Machine Learning）
   如图1所示，如果按照完全分对这个准则来划分时，L1为最优分割超平面，但是实际情况如果按照L2来进行划分，效果可能会更好，分类结果会更加鲁棒。
![](https://img-blog.csdn.net/20180914135715860?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                      图1  样本分布示意图
    通过引入松弛因子![](https://private.codecogs.com/gif.latex?)![\xi _{i} >= 0](https://private.codecogs.com/gif.latex?%5Cxi%20_%7Bi%7D%20%3E%3D%200)，放宽对于离群点的约束![(\vec{W}^{T}\phi (\vec{X}_{i})+b)y_{i}>=1-\xi _{i}](https://private.codecogs.com/gif.latex?%28%5Cvec%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29&plus;b%29y_%7Bi%7D%3E%3D1-%5Cxi%20_%7Bi%7D)，注意，如果![\xi _{i}](https://private.codecogs.com/gif.latex?%5Cxi%20_%7Bi%7D)任意大，任意超平面都会符合条件，因此我们需要对![\xi _{i}](https://private.codecogs.com/gif.latex?%5Cxi%20_%7Bi%7D)做一定限制，目标函数就变成了：
![arg\ min_{\vec{W},b} \ \frac{1}{2} ||\vec{W}||+C\sum_{i=0}^{n}\xi _{i}](https://private.codecogs.com/gif.latex?arg%5C%20min_%7B%5Cvec%7BW%7D%2Cb%7D%20%5C%20%5Cfrac%7B1%7D%7B2%7D%20%7C%7C%5Cvec%7BW%7D%7C%7C&plus;C%5Csum_%7Bi%3D0%7D%5E%7Bn%7D%5Cxi%20_%7Bi%7D)
![s.t. (\vec{W}^{T}\phi (\vec{X}_{i})+b)y_{i}\geqslant 1-\xi _{i},i=1,2,...,n](https://private.codecogs.com/gif.latex?s.t.%20%28%5Cvec%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29&plus;b%29y_%7Bi%7D%5Cgeqslant%201-%5Cxi%20_%7Bi%7D%2Ci%3D1%2C2%2C...%2Cn)
![\xi _{i}\geqslant 0,i=1,2,...,n](https://private.codecogs.com/gif.latex?%5Cxi%20_%7Bi%7D%5Cgeqslant%200%2Ci%3D1%2C2%2C...%2Cn)
    引入拉格朗日乘子有：
![L = \frac{1}{2} ||\vec{W}||^{2}+C\sum_{i=0}^{n}\xi _{i}-\sum_{i=1}^{n}\alpha _{i}((\vec{W}^{T}\phi (\vec{X}_{i})+b)y_{i}-1+\xi _{i} )-\sum_{i=1}^{n}\mu _{i}\xi _{i}](https://private.codecogs.com/gif.latex?L%20%3D%20%5Cfrac%7B1%7D%7B2%7D%20%7C%7C%5Cvec%7BW%7D%7C%7C%5E%7B2%7D&plus;C%5Csum_%7Bi%3D0%7D%5E%7Bn%7D%5Cxi%20_%7Bi%7D-%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Calpha%20_%7Bi%7D%28%28%5Cvec%7BW%7D%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29&plus;b%29y_%7Bi%7D-1&plus;%5Cxi%20_%7Bi%7D%20%29-%5Csum_%7Bi%3D1%7D%5E%7Bn%7D%5Cmu%20_%7Bi%7D%5Cxi%20_%7Bi%7D)
    将L分别对![\vec{W},\xi,b](https://private.codecogs.com/gif.latex?%5Cvec%7BW%7D%2C%5Cxi%2Cb)求偏导有：
![\vec{W}=\sum_{i=1}^{N}\alpha _{i}y_{i}\phi (\vec{x_{i}})](https://private.codecogs.com/gif.latex?%5Cvec%7BW%7D%3D%5Csum_%7Bi%3D1%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%5Cphi%20%28%5Cvec%7Bx_%7Bi%7D%7D%29)
![C-\alpha _{i}=\mu _{i},i=1,2,...,n](https://private.codecogs.com/gif.latex?C-%5Calpha%20_%7Bi%7D%3D%5Cmu%20_%7Bi%7D%2Ci%3D1%2C2%2C...%2Cn)
![\sum_{i=1}^{N}\alpha _{i}y_{i}=0,i=1,2,...,n](https://private.codecogs.com/gif.latex?%5Csum_%7Bi%3D1%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%3D0%2Ci%3D1%2C2%2C...%2Cn)
     将三个条件回代到L有：
![L = \sum_{i=1}^{N}\alpha _{i}-\frac{1}{2}\sum_{i=1}^{N}\sum_{j=1}^{N}\alpha _{i}\alpha _{j}\phi _( \vec {x_{i}})^{T}\phi _( \vec {x_{j}})](https://private.codecogs.com/gif.latex?L%20%3D%20%5Csum_%7Bi%3D1%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D-%5Cfrac%7B1%7D%7B2%7D%5Csum_%7Bi%3D1%7D%5E%7BN%7D%5Csum_%7Bj%3D1%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%5Calpha%20_%7Bj%7D%5Cphi%20_%28%20%5Cvec%20%7Bx_%7Bi%7D%7D%29%5E%7BT%7D%5Cphi%20_%28%20%5Cvec%20%7Bx_%7Bj%7D%7D%29)
    目标函数和约束条件为：
![\alpha ^{*}=max_{\alpha _{i}} (\sum_{i=0}^{N}\alpha _{i} -\frac{1}{2}\sum_{i=0}^{N}\sum_{j=0}^{N}\alpha _{i}\alpha _{j}y_{i}y_{j}\phi (\vec{X}_{i})^{T}\phi (\vec{X}_{j}))](https://private.codecogs.com/gif.latex?%5Calpha%20%5E%7B*%7D%3Dmax_%7B%5Calpha%20_%7Bi%7D%7D%20%28%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%20-%5Cfrac%7B1%7D%7B2%7D%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Csum_%7Bj%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%5Calpha%20_%7Bj%7Dy_%7Bi%7Dy_%7Bj%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bj%7D%29%29)
![s.t. \ \sum_{i=1}^{N}\alpha _{i}y_{i}=0,i=1,2,...,n](https://private.codecogs.com/gif.latex?s.t.%20%5C%20%5Csum_%7Bi%3D1%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%3D0%2Ci%3D1%2C2%2C...%2Cn)
![0\leqslant \alpha _{i} \leqslant C,i=1,2,...,n](https://private.codecogs.com/gif.latex?0%5Cleqslant%20%5Calpha%20_%7Bi%7D%20%5Cleqslant%20C%2Ci%3D1%2C2%2C...%2Cn)
    第二个约束条件是因为![\mu _{i} \geqslant 0](https://private.codecogs.com/gif.latex?%5Cmu%20_%7Bi%7D%20%5Cgeqslant%200)，即  ![C-\alpha _{i}\geqslant 0](https://private.codecogs.com/gif.latex?C-%5Calpha%20_%7Bi%7D%5Cgeqslant%200)，得 ![\alpha _{i} \leqslant C](https://private.codecogs.com/gif.latex?%5Calpha%20_%7Bi%7D%20%5Cleqslant%20C)
    这里C需要预先设定参数，C越大，容许的![\xi _{i}](https://private.codecogs.com/gif.latex?%5Cxi%20_%7Bi%7D)就越小，对于离群点偏离群体的位置容忍度就越小，SVM对应排空区域（过渡带）就越小，分类器容易发生过拟合；C越小，容许的![\xi _{i}](https://private.codecogs.com/gif.latex?%5Cxi%20_%7Bi%7D)就越大，对于离群点偏离群体的位置容忍度就越大，SVM对应排空区域（过渡带）就越大，C如果过小，分类器的性能就会较差。
    直观来看，如图1所示。
![](https://img-blog.csdn.net/20180914174341493?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                           图1 随着C增大，排空区域大小示意图
**    二、![\alpha _{i}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7Bi%7D)  的意义**
   由上可知， ![0\leqslant \alpha _{i} \leqslant C,i=1,2,...,n](https://private.codecogs.com/gif.latex?0%5Cleqslant%20%5Calpha%20_%7Bi%7D%20%5Cleqslant%20C%2Ci%3D1%2C2%2C...%2Cn)，那么![\alpha _{i}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7Bi%7D) 不同取值对应具体什么意义？
   1) 如果 ![\alpha _{i}=0](https://private.codecogs.com/gif.latex?%5Calpha%20_%7Bi%7D%3D0)，表明对应数据点对于模型构建没有任何帮助，对应于远离magin的数据点；
   2) 如果 ![0 < \alpha _{i} < C](https://private.codecogs.com/gif.latex?0%20%3C%20%5Calpha%20_%7Bi%7D%20%3C%20C)，需要 ![\xi _{i}=0](https://private.codecogs.com/gif.latex?%5Cxi%20_%7Bi%7D%3D0)，因此，这些数据点位于margin上；
   3) 如果![\alpha _{i}=C](https://private.codecogs.com/gif.latex?%5Calpha%20_%7Bi%7D%3DC)，则 ![\mu _{i}=0](https://private.codecogs.com/gif.latex?%5Cmu%20_%7Bi%7D%3D0)，这个时候，![\xi _{i}](https://private.codecogs.com/gif.latex?%5Cxi%20_%7Bi%7D) 可以取任意值，因此，这些数据点位于margin内部。
**参考资料：**
2016小象学院-机器学习升级版II（邹博）
支持向量机通俗导论（理解SVM的三层境界）
周志华. 机器学习 : = Machine learning[M]. 清华大学出版社, 2016.
Abramson N, Braverman D, Sebestyen G. Pattern Recognition and Machine Learning[M]. Springer, 2006. 
