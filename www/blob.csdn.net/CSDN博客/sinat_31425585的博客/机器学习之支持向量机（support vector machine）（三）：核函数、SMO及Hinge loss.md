# 机器学习之支持向量机（support vector machine）（三）：核函数、SMO及Hinge loss - sinat_31425585的博客 - CSDN博客
2018年09月16日 23:03:12[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：101
    对于线性可分SVM和线性SVM而言，其实默认数据是线性可分，或近似线性可分的。但是实际数据大多数为线性不可分的，因此，为了能够对数据进行准确划分，需要引入一些 tricks（kernal tricks），这就是核函数要做的事情，通过将低维数据映射到维核空间，使得数据在高维核空间可分，进而实现对类别的准确划分，其工作原理如图1所示。
![](https://img-blog.csdn.net/20180919080811978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                         图1 核函数的工作原理示意图（摘自资料[1]）
    一、常见核函数
    1) 多项式核函数：
![K(\vec{X_{1}},\vec{X_{2}})=(||\vec{X_{1}}-\vec{X_{2}}||^{a}+r)^{b}](https://private.codecogs.com/gif.latex?K%28%5Cvec%7BX_%7B1%7D%7D%2C%5Cvec%7BX_%7B2%7D%7D%29%3D%28%7C%7C%5Cvec%7BX_%7B1%7D%7D-%5Cvec%7BX_%7B2%7D%7D%7C%7C%5E%7Ba%7D&plus;r%29%5E%7Bb%7D)
    其中，a，b，r都为常数。
    2) 高斯核函数：
![K(\vec{X_{1}},\vec{X_{2}})=exp(\frac{-||\vec{X_{1}}-\vec{X_{2}}||^{2}}{2\sigma ^{2}})](https://private.codecogs.com/gif.latex?K%28%5Cvec%7BX_%7B1%7D%7D%2C%5Cvec%7BX_%7B2%7D%7D%29%3Dexp%28%5Cfrac%7B-%7C%7C%5Cvec%7BX_%7B1%7D%7D-%5Cvec%7BX_%7B2%7D%7D%7C%7C%5E%7B2%7D%7D%7B2%5Csigma%20%5E%7B2%7D%7D%29)
    3) sigmoid核函数：
![K(\vec{X_{1}},\vec{X_{2}})=tanh(\gamma ||\vec{X_{1}}-\vec{X_{2}}||^{a}+r)](https://private.codecogs.com/gif.latex?K%28%5Cvec%7BX_%7B1%7D%7D%2C%5Cvec%7BX_%7B2%7D%7D%29%3Dtanh%28%5Cgamma%20%7C%7C%5Cvec%7BX_%7B1%7D%7D-%5Cvec%7BX_%7B2%7D%7D%7C%7C%5E%7Ba%7D&plus;r%29)
    其中，![\gamma ,r, \alpha](https://private.codecogs.com/gif.latex?%5Cgamma%20%2Cr%2C%20%5Calpha)都为常数。
    核函数具有什么性质？下面这个例子来自于资料[1]。
    假设存在两个数据点![\vec{X_{1}}={1,2,3},\vec{X_{2}}={4,5,6}](https://private.codecogs.com/gif.latex?%5Cvec%7BX_%7B1%7D%7D%3D%7B1%2C2%2C3%7D%2C%5Cvec%7BX_%7B2%7D%7D%3D%7B4%2C5%2C6%7D)，核函数![\phi (\vec{X})=(x_{1}^{2},x_{1}x_{2},x_{1}x_{3},x_{2}^{2},x_{2}x_{3},x_{3}x_{1},x_{3}x_{2},x_{3}^{2})](https://private.codecogs.com/gif.latex?%5Cphi%20%28%5Cvec%7BX%7D%29%3D%28x_%7B1%7D%5E%7B2%7D%2Cx_%7B1%7Dx_%7B2%7D%2Cx_%7B1%7Dx_%7B3%7D%2Cx_%7B2%7D%5E%7B2%7D%2Cx_%7B2%7Dx_%7B3%7D%2Cx_%7B3%7Dx_%7B1%7D%2Cx_%7B3%7Dx_%7B2%7D%2Cx_%7B3%7D%5E%7B2%7D%29)，因此有：
![\phi (\vec{X_{1}})=(1,2,3,2,4,6,3,6,9)](https://private.codecogs.com/gif.latex?%5Cphi%20%28%5Cvec%7BX_%7B1%7D%7D%29%3D%281%2C2%2C3%2C2%2C4%2C6%2C3%2C6%2C9%29)
![\phi (\vec{X_{2}})=(16,20,24,20,25,36,24,30,36)](https://private.codecogs.com/gif.latex?%5Cphi%20%28%5Cvec%7BX_%7B2%7D%7D%29%3D%2816%2C20%2C24%2C20%2C25%2C36%2C24%2C30%2C36%29)
    直接计算有：
![K(\vec{X_{1}},\vec{X_{2}})=<\phi (\vec{X_{1}}),\phi (\vec{X_{2}})>](https://private.codecogs.com/gif.latex?K%28%5Cvec%7BX_%7B1%7D%7D%2C%5Cvec%7BX_%7B2%7D%7D%29%3D%3C%5Cphi%20%28%5Cvec%7BX_%7B1%7D%7D%29%2C%5Cphi%20%28%5Cvec%7BX_%7B2%7D%7D%29%3E)![=16+40+72+40+100+180+72+180+324=1024](https://private.codecogs.com/gif.latex?%3D16&plus;40&plus;72&plus;40&plus;100&plus;180&plus;72&plus;180&plus;324%3D1024)
    先求![<\vec{X_{1}},\vec{X_{2}}>](https://private.codecogs.com/gif.latex?%3C%5Cvec%7BX_%7B1%7D%7D%2C%5Cvec%7BX_%7B2%7D%7D%3E)，再做核映射，有：
![K(\vec{X_{1}},\vec{X_{2}})=(4+10+18)^{2}=1024](https://private.codecogs.com/gif.latex?K%28%5Cvec%7BX_%7B1%7D%7D%2C%5Cvec%7BX_%7B2%7D%7D%29%3D%284&plus;10&plus;18%29%5E%7B2%7D%3D1024)
    不难发现，我们先将点X1，X2做高维核变换之后再求内积，等价于先对点X1，X2求内积，再做高维核变换。这样做的好处在于，如果先做核变换，再对高维数据求内积，当维度过大时，计算量爆炸，因此，一般采用先在低维上做内积，再将内积值映射到高维。                                      
    二、SMO算法，求解![\alpha](https://private.codecogs.com/gif.latex?%5Calpha)及决策超平面
    其实不难发现，线性可分SVM为线性SVM的特例，因此，只需要求解线性SVM即可。原问题最大化问题：
![\alpha ^{*}=max_{\alpha _{i}} (\sum_{i=0}^{N}\alpha _{i} -\frac{1}{2}\sum_{i=0}^{N}\sum_{j=0}^{N}\alpha _{i}\alpha _{j}y_{i}y_{j}\phi (\vec{X}_{i})^{T}\phi (\vec{X}_{j}))](https://private.codecogs.com/gif.latex?%5Calpha%20%5E%7B*%7D%3Dmax_%7B%5Calpha%20_%7Bi%7D%7D%20%28%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%20-%5Cfrac%7B1%7D%7B2%7D%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Csum_%7Bj%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%5Calpha%20_%7Bj%7Dy_%7Bi%7Dy_%7Bj%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bj%7D%29%29)
    可以转换成对应最小化：
![\alpha ^{*}=min_{ \alpha _{i}} (\frac{1}{2}\sum_{i=0}^{N}\sum_{j=0}^{N}\alpha _{i}\alpha _{j}y_{i}y_{j}\phi (\vec{X}_{i})^{T}\phi (\vec{X}_{j}) - \sum_{i=0}^{N}\alpha _{i})](https://private.codecogs.com/gif.latex?%5Calpha%20%5E%7B*%7D%3Dmin_%7B%20%5Calpha%20_%7Bi%7D%7D%20%28%5Cfrac%7B1%7D%7B2%7D%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Csum_%7Bj%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%5Calpha%20_%7Bj%7Dy_%7Bi%7Dy_%7Bj%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bj%7D%29%20-%20%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%29)
![s.t. \ \sum_{i=1}^{N}\alpha _{i}y_{i}=0,i=1,2,...,n](https://private.codecogs.com/gif.latex?s.t.%20%5C%20%5Csum_%7Bi%3D1%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%3D0%2Ci%3D1%2C2%2C...%2Cn)
![0\leqslant \alpha _{i} \leqslant C,i=1,2,...,n](https://private.codecogs.com/gif.latex?0%5Cleqslant%20%5Calpha%20_%7Bi%7D%20%5Cleqslant%20C%2Ci%3D1%2C2%2C...%2Cn)
    按照前面描述，![\phi (\vec{X}_{i})^{T}\phi (\vec{X}_{j})](https://private.codecogs.com/gif.latex?%5Cphi%20%28%5Cvec%7BX%7D_%7Bi%7D%29%5E%7BT%7D%5Cphi%20%28%5Cvec%7BX%7D_%7Bj%7D%29)等价于![\phi (<\vec{X}_{i},\vec{X}_{j}>)](https://private.codecogs.com/gif.latex?%5Cphi%20%28%3C%5Cvec%7BX%7D_%7Bi%7D%2C%5Cvec%7BX%7D_%7Bj%7D%3E%29)，保证和前面核函数描述一致，记为![K(\vec{X_{1}},\vec{X_{2}})](https://private.codecogs.com/gif.latex?K%28%5Cvec%7BX_%7B1%7D%7D%2C%5Cvec%7BX_%7B2%7D%7D%29)
![\alpha ^{*}=min_{ \alpha _{i}} (\frac{1}{2}\sum_{i=0}^{N}\sum_{j=0}^{N}\alpha _{i}\alpha _{j}y_{i}y_{j}K(\vec{X_{1}},\vec{X_{2}}) - \sum_{i=0}^{N}\alpha _{i})](https://private.codecogs.com/gif.latex?%5Calpha%20%5E%7B*%7D%3Dmin_%7B%20%5Calpha%20_%7Bi%7D%7D%20%28%5Cfrac%7B1%7D%7B2%7D%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Csum_%7Bj%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%5Calpha%20_%7Bj%7Dy_%7Bi%7Dy_%7Bj%7DK%28%5Cvec%7BX_%7B1%7D%7D%2C%5Cvec%7BX_%7B2%7D%7D%29%20-%20%5Csum_%7Bi%3D0%7D%5E%7BN%7D%5Calpha%20_%7Bi%7D%29)
![s.t. \ \sum_{i=1}^{N}\alpha _{i}y_{i}=0,i=1,2,...,n](https://private.codecogs.com/gif.latex?s.t.%20%5C%20%5Csum_%7Bi%3D1%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%3D0%2Ci%3D1%2C2%2C...%2Cn)
![0\leqslant \alpha _{i} \leqslant C,i=1,2,...,n](https://private.codecogs.com/gif.latex?0%5Cleqslant%20%5Calpha%20_%7Bi%7D%20%5Cleqslant%20C%2Ci%3D1%2C2%2C...%2Cn)
    SMO(Sequential Minimal Optimization)即序列最小优化，通过固定其中N+2个参数，将原始问题就转换成了求解二元函数问题。
    假设，当前选取的变量为![\alpha _{1},\alpha_{2}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D%2C%5Calpha_%7B2%7D)，有：
![\alpha _{1}y_{1}+\alpha_{2}y_{2}=\sum_{i=3}^{N}\alpha _{i}y_{i}=\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7Dy_%7B1%7D&plus;%5Calpha_%7B2%7Dy_%7B2%7D%3D%5Csum_%7Bi%3D3%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%3D%5Czeta)
** 2.1 边界条件**
    这里可以分为两种情况：![y_{1},y_{2}](https://private.codecogs.com/gif.latex?y_%7B1%7D%2Cy_%7B2%7D)同号和![y_{1},y_{2}](https://private.codecogs.com/gif.latex?y_%7B1%7D%2Cy_%7B2%7D)异号。
**2.1.1 ![y_{1},y_{2}](https://private.codecogs.com/gif.latex?y_%7B1%7D%2Cy_%7B2%7D)异号**
![y_{1},y_{2}](https://private.codecogs.com/gif.latex?y_%7B1%7D%2Cy_%7B2%7D)异号时有：
    1、![y_{1}=1,y_{2}=-1](https://private.codecogs.com/gif.latex?y_%7B1%7D%3D1%2Cy_%7B2%7D%3D-1)则：  ![\alpha _{1}-\alpha_{2}=\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D-%5Calpha_%7B2%7D%3D%5Czeta)
    2、![y_{1}=-1,y_{2}=1](https://private.codecogs.com/gif.latex?y_%7B1%7D%3D-1%2Cy_%7B2%7D%3D1)则：  ![\alpha _{1}-\alpha_{2}=-\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D-%5Calpha_%7B2%7D%3D-%5Czeta)
    而![0\leqslant \alpha _{1} ,\alpha _{2}\leqslant C](https://private.codecogs.com/gif.latex?0%5Cleqslant%20%5Calpha%20_%7B1%7D%20%2C%5Calpha%20_%7B2%7D%5Cleqslant%20C)，所以![y_{1},y_{2}](https://private.codecogs.com/gif.latex?y_%7B1%7D%2Cy_%7B2%7D)异号时，图像如图2所示。
![](https://img-blog.csdn.net/20180919114408591?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                     图2 ![y_{1},y_{2}](https://private.codecogs.com/gif.latex?y_%7B1%7D%2Cy_%7B2%7D)异号
![\alpha _{1},\alpha_{2}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D%2C%5Calpha_%7B2%7D)即要在正方形区域内，又要在直线上，![\alpha _{1}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D)可以由![\alpha_{2}](https://private.codecogs.com/gif.latex?%5Calpha_%7B2%7D)表示，![\alpha_{2}](https://private.codecogs.com/gif.latex?%5Calpha_%7B2%7D)的取值范围为：
![L=max\begin{Bmatrix} 0,- \zeta \end{Bmatrix},H=min\begin{Bmatrix} C,C+ \zeta \end{Bmatrix}](https://private.codecogs.com/gif.latex?L%3Dmax%5Cbegin%7BBmatrix%7D%200%2C-%20%5Czeta%20%5Cend%7BBmatrix%7D%2CH%3Dmin%5Cbegin%7BBmatrix%7D%20C%2CC&plus;%20%5Czeta%20%5Cend%7BBmatrix%7D)
   而L中![\alpha _{1}-\alpha_{2}=\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D-%5Calpha_%7B2%7D%3D%5Czeta)，H中![\alpha _{1}-\alpha_{2}=-\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D-%5Calpha_%7B2%7D%3D-%5Czeta)，有：
![L=max\begin{Bmatrix} 0,\alpha_{2}-\alpha_{1} \end{Bmatrix},H=min\begin{Bmatrix} C, \alpha_{2}-\alpha_{1}+C \end{Bmatrix}](https://private.codecogs.com/gif.latex?L%3Dmax%5Cbegin%7BBmatrix%7D%200%2C%5Calpha_%7B2%7D-%5Calpha_%7B1%7D%20%5Cend%7BBmatrix%7D%2CH%3Dmin%5Cbegin%7BBmatrix%7D%20C%2C%20%5Calpha_%7B2%7D-%5Calpha_%7B1%7D&plus;C%20%5Cend%7BBmatrix%7D)
** 2.1.2 ![y_{1},y_{2}](https://private.codecogs.com/gif.latex?y_%7B1%7D%2Cy_%7B2%7D)同号**
![y_{1},y_{2}](https://private.codecogs.com/gif.latex?y_%7B1%7D%2Cy_%7B2%7D)同号时，为保证与异号时![\zeta](https://private.codecogs.com/gif.latex?%5Czeta)符号的一致性，即![\zeta\leq 0](https://private.codecogs.com/gif.latex?%5Czeta%5Cleq%200)，有：    
    1、![y_{1}=1,y_{2}=1](https://private.codecogs.com/gif.latex?y_%7B1%7D%3D1%2Cy_%7B2%7D%3D1)则：  ![\alpha _{1}+\alpha_{2}=-\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D&plus;%5Calpha_%7B2%7D%3D-%5Czeta)
    2、![y_{1}=-1,y_{2}=-1](https://private.codecogs.com/gif.latex?y_%7B1%7D%3D-1%2Cy_%7B2%7D%3D-1)则：  ![\alpha _{1}+\alpha_{2}=\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D&plus;%5Calpha_%7B2%7D%3D%5Czeta)
    而![0\leqslant \alpha _{1} ,\alpha _{2}\leqslant C](https://private.codecogs.com/gif.latex?0%5Cleqslant%20%5Calpha%20_%7B1%7D%20%2C%5Calpha%20_%7B2%7D%5Cleqslant%20C)，所以![y_{1},y_{2}](https://private.codecogs.com/gif.latex?y_%7B1%7D%2Cy_%7B2%7D)同号时，图像如图3所示。
![](https://img-blog.csdn.net/20180919115711543?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
                                                                                             图3 ![y_{1},y_{2}](https://private.codecogs.com/gif.latex?y_%7B1%7D%2Cy_%7B2%7D)同号
![\alpha _{1},\alpha_{2}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D%2C%5Calpha_%7B2%7D)即要在正方形区域内，又要在直线上，![\alpha_{2}](https://private.codecogs.com/gif.latex?%5Calpha_%7B2%7D)可以![\alpha _{1}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D)由表示，![\alpha _{1}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D)的取值范围为：
![L=max\begin{Bmatrix} 0,\zeta-C \end{Bmatrix},H=min\begin{Bmatrix} C,-\zeta \end{Bmatrix}](https://private.codecogs.com/gif.latex?L%3Dmax%5Cbegin%7BBmatrix%7D%200%2C%5Czeta-C%20%5Cend%7BBmatrix%7D%2CH%3Dmin%5Cbegin%7BBmatrix%7D%20C%2C-%5Czeta%20%5Cend%7BBmatrix%7D)
   而L中![\alpha _{1}+\alpha_{2}=\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D&plus;%5Calpha_%7B2%7D%3D%5Czeta)，H中![\alpha _{1}+\alpha_{2}=-\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D&plus;%5Calpha_%7B2%7D%3D-%5Czeta)，有：
![L=max\begin{Bmatrix} 0,\alpha_{2}+\alpha_{1} -C\end{Bmatrix},H=min\begin{Bmatrix} C, \alpha_{2}+\alpha_{1} \end{Bmatrix}](https://private.codecogs.com/gif.latex?L%3Dmax%5Cbegin%7BBmatrix%7D%200%2C%5Calpha_%7B2%7D&plus;%5Calpha_%7B1%7D%20-C%5Cend%7BBmatrix%7D%2CH%3Dmin%5Cbegin%7BBmatrix%7D%20C%2C%20%5Calpha_%7B2%7D&plus;%5Calpha_%7B1%7D%20%5Cend%7BBmatrix%7D)
    最后，求解![\alpha](https://private.codecogs.com/gif.latex?%5Calpha)有，![\alpha](https://private.codecogs.com/gif.latex?%5Calpha)取值应该位于区间![[L,H]](https://private.codecogs.com/gif.latex?%5BL%2CH%5D)，如果求取得到的![\alpha](https://private.codecogs.com/gif.latex?%5Calpha)超出这个范围，取边界条件，然后进行验证。
**2.2 求解**
    由于假定![\alpha _{1},\alpha_{2}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D%2C%5Calpha_%7B2%7D)为变量，其余![\alpha _{3}, \alpha _{4},...,\alpha_{N}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B3%7D%2C%20%5Calpha%20_%7B4%7D%2C...%2C%5Calpha_%7BN%7D)为常量，求目标函数最小值时，我们只需要考虑包含![\alpha _{1},\alpha_{2}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D%2C%5Calpha_%7B2%7D)的项，则目标函数变为：
![\alpha ^{*}=min_{ \alpha _{1},\alpha_{2}} f](https://private.codecogs.com/gif.latex?%5Calpha%20%5E%7B*%7D%3Dmin_%7B%20%5Calpha%20_%7B1%7D%2C%5Calpha_%7B2%7D%7D%20f)
![f=\frac{1}{2}K_{11}\alpha _{1}^{2}+\frac{1}{2}K_{22}\alpha _{2}^{2}+K_{12}\alpha _{1}\alpha _{2}y_{1}y_{2}-\alpha _{1}-\alpha _{2}](https://private.codecogs.com/gif.latex?f%3D%5Cfrac%7B1%7D%7B2%7DK_%7B11%7D%5Calpha%20_%7B1%7D%5E%7B2%7D&plus;%5Cfrac%7B1%7D%7B2%7DK_%7B22%7D%5Calpha%20_%7B2%7D%5E%7B2%7D&plus;K_%7B12%7D%5Calpha%20_%7B1%7D%5Calpha%20_%7B2%7Dy_%7B1%7Dy_%7B2%7D-%5Calpha%20_%7B1%7D-%5Calpha%20_%7B2%7D)
![+\alpha _{1}y_{1}\sum_{i=3}^{N}\alpha _{i}y_{i}K_{i1}+\alpha _{2}y_{2}\sum_{i=3}^{N}\alpha _{i}y_{i}K_{i2}](https://private.codecogs.com/gif.latex?&plus;%5Calpha%20_%7B1%7Dy_%7B1%7D%5Csum_%7Bi%3D3%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7DK_%7Bi1%7D&plus;%5Calpha%20_%7B2%7Dy_%7B2%7D%5Csum_%7Bi%3D3%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7DK_%7Bi2%7D)
    由假设有：
![\alpha _{1}y_{1}+\alpha_{2}y_{2}=\sum_{i=3}^{N}\alpha _{i}y_{i}=\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7Dy_%7B1%7D&plus;%5Calpha_%7B2%7Dy_%7B2%7D%3D%5Csum_%7Bi%3D3%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%3D%5Czeta)
   由于![y _{1}*y _{1}=1](https://private.codecogs.com/gif.latex?y%20_%7B1%7D*y%20_%7B1%7D%3D1)，两边同时乘以![y _{1}](https://private.codecogs.com/gif.latex?y%20_%7B1%7D)有：
![\alpha _{1}+\alpha_{2}y_{1}y_{2}=y_{1}\sum_{i=3}^{N}\alpha _{i}y_{i}=y_{1}\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D&plus;%5Calpha_%7B2%7Dy_%7B1%7Dy_%7B2%7D%3Dy_%7B1%7D%5Csum_%7Bi%3D3%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7D%3Dy_%7B1%7D%5Czeta)
   则：
![\alpha _{1}=y_{1}(\zeta-\alpha_{2}y_{2})](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D%3Dy_%7B1%7D%28%5Czeta-%5Calpha_%7B2%7Dy_%7B2%7D%29)
   令:
![v_{j}=\sum_{i=3}^{N}\alpha _{i}y_{i}k_{ij}](https://private.codecogs.com/gif.latex?v_%7Bj%7D%3D%5Csum_%7Bi%3D3%7D%5E%7BN%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7Dk_%7Bij%7D)
    将![\alpha _{1}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D)回代到![f](https://private.codecogs.com/gif.latex?f)中有：
![f=\frac{1}{2}k_{11}\alpha _{1}^{2}+\frac{1}{2}k_{22}\alpha _{2}^{2}+y_{1}y_{2}\alpha _{1}\alpha _{2}k_{12}-(\alpha _{1}+\alpha _{2})+y_{1}\alpha _{1}v_{1}+y_{2}\alpha _{2}v_{2}](https://private.codecogs.com/gif.latex?f%3D%5Cfrac%7B1%7D%7B2%7Dk_%7B11%7D%5Calpha%20_%7B1%7D%5E%7B2%7D&plus;%5Cfrac%7B1%7D%7B2%7Dk_%7B22%7D%5Calpha%20_%7B2%7D%5E%7B2%7D&plus;y_%7B1%7Dy_%7B2%7D%5Calpha%20_%7B1%7D%5Calpha%20_%7B2%7Dk_%7B12%7D-%28%5Calpha%20_%7B1%7D&plus;%5Calpha%20_%7B2%7D%29&plus;y_%7B1%7D%5Calpha%20_%7B1%7Dv_%7B1%7D&plus;y_%7B2%7D%5Calpha%20_%7B2%7Dv_%7B2%7D)
![=\frac{1}{2}k_{11}(\zeta -\alpha _{2}y_{2})^{2}+\frac{1}{2}k_{22}\alpha _{2}^{2}+y_{2}(\zeta -\alpha _{2}y_{2})\alpha _{2}k_{12}](https://private.codecogs.com/gif.latex?%3D%5Cfrac%7B1%7D%7B2%7Dk_%7B11%7D%28%5Czeta%20-%5Calpha%20_%7B2%7Dy_%7B2%7D%29%5E%7B2%7D&plus;%5Cfrac%7B1%7D%7B2%7Dk_%7B22%7D%5Calpha%20_%7B2%7D%5E%7B2%7D&plus;y_%7B2%7D%28%5Czeta%20-%5Calpha%20_%7B2%7Dy_%7B2%7D%29%5Calpha%20_%7B2%7Dk_%7B12%7D)
![-(\alpha _{1}+\alpha _{2})+(\zeta -\alpha _{2}y_{2})v_{1}+y_{2}\alpha _{2}v_{2}](https://private.codecogs.com/gif.latex?-%28%5Calpha%20_%7B1%7D&plus;%5Calpha%20_%7B2%7D%29&plus;%28%5Czeta%20-%5Calpha%20_%7B2%7Dy_%7B2%7D%29v_%7B1%7D&plus;y_%7B2%7D%5Calpha%20_%7B2%7Dv_%7B2%7D)
    将![f](https://private.codecogs.com/gif.latex?f)对![\alpha _{2}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B2%7D)求导数并令其为0，有：
![(k_{11}-2k_{12}+k_{22})\alpha _{2}=y_{2}(y_{1}-y_{2}+v_{1}-v_{2}-\zeta (k_{11}-k_{12}))](https://private.codecogs.com/gif.latex?%28k_%7B11%7D-2k_%7B12%7D&plus;k_%7B22%7D%29%5Calpha%20_%7B2%7D%3Dy_%7B2%7D%28y_%7B1%7D-y_%7B2%7D&plus;v_%7B1%7D-v_%7B2%7D-%5Czeta%20%28k_%7B11%7D-k_%7B12%7D%29%29)
    我们然后用![\alpha _{2},\alpha _{2}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B2%7D%2C%5Calpha%20_%7B2%7D)表示![v_{1},v_{2}](https://private.codecogs.com/gif.latex?v_%7B1%7D%2Cv_%7B2%7D)有：
![v_{j}=y(x_{j})-\sum_{i=1}^{2}\alpha _{i}y_{i}k_{ij}-b](https://private.codecogs.com/gif.latex?v_%7Bj%7D%3Dy%28x_%7Bj%7D%29-%5Csum_%7Bi%3D1%7D%5E%7B2%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7Dk_%7Bij%7D-b)
    回代到上面式子有：
![(k_{11}-2k_{12}+k_{22})\alpha _{2}=y_{2}(y_{1}-y_{2}+ (y(x_{1})-\sum_{i=1}^{2}\alpha _{i}y_{i}k_{i1}-b)-](https://private.codecogs.com/gif.latex?%28k_%7B11%7D-2k_%7B12%7D&plus;k_%7B22%7D%29%5Calpha%20_%7B2%7D%3Dy_%7B2%7D%28y_%7B1%7D-y_%7B2%7D&plus;%20%28y%28x_%7B1%7D%29-%5Csum_%7Bi%3D1%7D%5E%7B2%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7Dk_%7Bi1%7D-b%29-)
![(y(x_{2})-\sum_{i=1}^{2}\alpha _{i}y_{i}k_{i 2}-b) -\zeta (k_{12}-k_{11}))](https://private.codecogs.com/gif.latex?%28y%28x_%7B2%7D%29-%5Csum_%7Bi%3D1%7D%5E%7B2%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7Dk_%7Bi%202%7D-b%29%20-%5Czeta%20%28k_%7B12%7D-k_%7B11%7D%29%29)
    定义残差：
![E_{i}=y(x_{i}) - y_{i}](https://private.codecogs.com/gif.latex?E_%7Bi%7D%3Dy%28x_%7Bi%7D%29%20-%20y_%7Bi%7D)
    则：
![(k_{11}-2k_{12}+k_{22})\alpha _{2}=y_{2}(E_{2}-E_{1}-\sum_{i=1}^{2}\alpha _{i}y_{i}k_{i2}+\sum_{i=1}^{2}\alpha _{i}y_{i}k_{i1} -\zeta (k_{12}-k_{11}))](https://private.codecogs.com/gif.latex?%28k_%7B11%7D-2k_%7B12%7D&plus;k_%7B22%7D%29%5Calpha%20_%7B2%7D%3Dy_%7B2%7D%28E_%7B2%7D-E_%7B1%7D-%5Csum_%7Bi%3D1%7D%5E%7B2%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7Dk_%7Bi2%7D&plus;%5Csum_%7Bi%3D1%7D%5E%7B2%7D%5Calpha%20_%7Bi%7Dy_%7Bi%7Dk_%7Bi1%7D%20-%5Czeta%20%28k_%7B12%7D-k_%7B11%7D%29%29)
    将![\alpha _{1}y_{1}+\alpha_{2}y_{2}=\zeta](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7Dy_%7B1%7D&plus;%5Calpha_%7B2%7Dy_%7B2%7D%3D%5Czeta)代入有：
![(k_{11}-2k_{12}+k_{22})\alpha _{2}^{new}=(k_{11}-2k_{12}+k_{22})\alpha _{2}^{old}+y_{2}(E_{2}-E_{1})](https://private.codecogs.com/gif.latex?%28k_%7B11%7D-2k_%7B12%7D&plus;k_%7B22%7D%29%5Calpha%20_%7B2%7D%5E%7Bnew%7D%3D%28k_%7B11%7D-2k_%7B12%7D&plus;k_%7B22%7D%29%5Calpha%20_%7B2%7D%5E%7Bold%7D&plus;y_%7B2%7D%28E_%7B2%7D-E_%7B1%7D%29)
    两边同时除以两边常量有：
![\alpha _{2}^{new}=\alpha _{2}^{old}+\frac{y_{2}(E_{2}-E_{1})}{(k_{11}-2k_{12}+k_{22})}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B2%7D%5E%7Bnew%7D%3D%5Calpha%20_%7B2%7D%5E%7Bold%7D&plus;%5Cfrac%7By_%7B2%7D%28E_%7B2%7D-E_%7B1%7D%29%7D%7B%28k_%7B11%7D-2k_%7B12%7D&plus;k_%7B22%7D%29%7D)
    利用上式，就可以迭代的得到![\alpha _{2}^{new}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B2%7D%5E%7Bnew%7D)，利用![\alpha _{2}^{new}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B2%7D%5E%7Bnew%7D)与![\alpha _{1}^{new}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D%5E%7Bnew%7D)之间线性关系，就可以得到![\alpha _{1}^{new}](https://private.codecogs.com/gif.latex?%5Calpha%20_%7B1%7D%5E%7Bnew%7D)，终于得到这个递推式子，非常感谢[刘建平Pinard](https://home.cnblogs.com/u/pinard/)的博客[3]，终于对照着推出来了。
    最后贴一下SMO的步骤，这两页ppt摘自参考资料[4].。
![](https://img-blog.csdn.net/20180920225323875?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180920225407594?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
     三、Hinge Loss
    求解SVM除了采用SMO来做，还可以采用优化的思路，定义一个误差，使用梯度下降法来进行优化，通过最小化误差项来求解SVM。
    参考资料
    [1] 唐宇迪的机器学习课程
    [2] [http://www.cnblogs.com/jerrylead/archive/2011/03/18/1988419.html](http://www.cnblogs.com/jerrylead/archive/2011/03/18/1988419.html)
    [3] [https://www.cnblogs.com/pinard/p/6111471.html](https://www.cnblogs.com/pinard/p/6111471.html)
    [4] 2016小象学院-机器学习升级版II（邹博）
