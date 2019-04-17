# 离散傅里叶变换（DFT）与三角插值法 - Machine Learning with Peppa - CSDN博客





2018年04月11日 16:51:08[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：940









### **离散傅里叶变换（DFT）**

**1. 标准正交基**
向量空间![V](https://www.zhihu.com/equation?tex=V)（![\mathbb{R}^N](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5EN)或![\mathbb{C}^N](https://www.zhihu.com/equation?tex=%5Cmathbb%7BC%7D%5EN)）的标准正交基![\left\{b_k\right\}_{k=0}^{N-1}](https://www.zhihu.com/equation?tex=%5Cleft%5C%7Bb_k%5Cright%5C%7D_%7Bk%3D0%7D%5E%7BN-1%7D)满足以下两个条件：


![\left<b_k,b_l\right>=0,k \ne l](https://www.zhihu.com/equation?tex=%5Cleft%3Cb_k%2Cb_l%5Cright%3E%3D0%2Ck+%5Cne+l+)

![\|b_k\|_2=1](https://www.zhihu.com/equation?tex=%5C%7Cb_k%5C%7C_2%3D1)
我们可以得到一个![N \times N](https://www.zhihu.com/equation?tex=N+%5Ctimes+N)的标准正交基矩阵：

![\mathbf{B}=\left[b_0|b_1|\cdots|b_{N-1}\right]](https://www.zhihu.com/equation?tex=%5Cmathbf%7BB%7D%3D%5Cleft%5Bb_0%7Cb_1%7C%5Ccdots%7Cb_%7BN-1%7D%5Cright%5D)
再把每一个标准正交基对应的系数![\alpha_k](https://www.zhihu.com/equation?tex=%5Calpha_k)写成一个列向量：

![a=\left[\begin{array}{c}\alpha_0 \\ \alpha_1 \\\vdots \\\alpha_{N-1}\end{array}\right]](https://www.zhihu.com/equation?tex=a%3D%5Cleft%5B%5Cbegin%7Barray%7D%7Bc%7D%0A%5Calpha_0+%5C%5C+%0A%5Calpha_1+%5C%5C%0A%5Cvdots+%5C%5C%0A%5Calpha_%7BN-1%7D%0A%5Cend%7Barray%7D%5Cright%5D)
则信号![x](https://www.zhihu.com/equation?tex=x%0A)的标准正交基表示：

![x=\alpha_0b_0+\alpha_1b_1+\cdots+\alpha_{N-1}b_{N-1}=\sum_{k=0}^{N-1}{\alpha_kb_k} =\left[b_0|b_1|\cdots|b_{N-1}\right]\left[\begin{array}{c}\alpha_0 \\ \alpha_1 \\\vdots \\\alpha_{N-1}\end{array}\right]=\mathbf{B}a](https://www.zhihu.com/equation?tex=x%3D%5Calpha_0b_0%2B%5Calpha_1b_1%2B%5Ccdots%2B%5Calpha_%7BN-1%7Db_%7BN-1%7D%3D%5Csum_%7Bk%3D0%7D%5E%7BN-1%7D%7B%5Calpha_kb_k%7D+%3D%5Cleft%5Bb_0%7Cb_1%7C%5Ccdots%7Cb_%7BN-1%7D%5Cright%5D%5Cleft%5B%5Cbegin%7Barray%7D%7Bc%7D%0A%5Calpha_0+%5C%5C+%0A%5Calpha_1+%5C%5C%0A%5Cvdots+%5C%5C%0A%5Calpha_%7BN-1%7D%0A%5Cend%7Barray%7D%5Cright%5D%3D%5Cmathbf%7BB%7Da)
那么![a=\mathbf{B}^{-1}x=\mathbf{B}^{H}x](https://www.zhihu.com/equation?tex=a%3D%5Cmathbf%7BB%7D%5E%7B-1%7Dx%3D%5Cmathbf%7BB%7D%5E%7BH%7Dx)（这里，![\mathbf{B}^H](https://www.zhihu.com/equation?tex=%5Cmathbf%7BB%7D%5EH)是指![\mathbf{B}](https://www.zhihu.com/equation?tex=%5Cmathbf%7BB%7D)的共轭转置矩阵，不难证明![\mathbf{B}^{-1}=\mathbf{B}^H](https://www.zhihu.com/equation?tex=%5Cmathbf%7BB%7D%5E%7B-1%7D%3D%5Cmathbf%7BB%7D%5EH)）
关键结论：
对于一组标准正交基![\left\{b_k\right\}_{k=0}^{N-1}](https://www.zhihu.com/equation?tex=%5Cleft%5C%7Bb_k%5Cright%5C%7D_%7Bk%3D0%7D%5E%7BN-1%7D)和标准正交基矩阵![\mathbf{B}](https://www.zhihu.com/equation?tex=%5Cmathbf%7BB%7D)，对于任意的信号![x](https://www.zhihu.com/equation?tex=x)，我们有以下的表达：

综合式：

![x=\mathbf{B}a=\sum_{k=0}^{N-1}{\alpha_kb_k}](https://www.zhihu.com/equation?tex=x%3D%5Cmathbf%7BB%7Da%3D%5Csum_%7Bk%3D0%7D%5E%7BN-1%7D%7B%5Calpha_kb_k%7D+)

分析式：![a=\mathbf{B}^Hx](https://www.zhihu.com/equation?tex=a%3D%5Cmathbf%7BB%7D%5EHx) 或 ![\alpha_k=\left<x, b_k\right>](https://www.zhihu.com/equation?tex=%5Calpha_k%3D%5Cleft%3Cx%2C+b_k%5Cright%3E)

综合式表明信号![x](https://www.zhihu.com/equation?tex=x)可以表示成标准正交基的线性组合。
分析式给出了计算标准正交基对应系数![\alpha_k](https://www.zhihu.com/equation?tex=%5Calpha_k)的方法，![\alpha_k](https://www.zhihu.com/equation?tex=%5Calpha_k)的大小表征了信号![x](https://www.zhihu.com/equation?tex=x%0A)与标准正交基向量![b_k](https://www.zhihu.com/equation?tex=b_k)
之间的相似度。

**2.特征向量与特征值**
结论：LTI系统的特征向量是复正弦谐波（证明略）：


![s_k\left[n\right]=\frac{e^{j{\frac{2\pi}{N}kn}}}{\sqrt[]{N}},0\le n,k\le N-1](https://www.zhihu.com/equation?tex=s_k%5Cleft%5Bn%5Cright%5D%3D%5Cfrac%7Be%5E%7Bj%7B%5Cfrac%7B2%5Cpi%7D%7BN%7Dkn%7D%7D%7D%7B%5Csqrt%5B%5D%7BN%7D%7D%2C0%5Cle+n%2Ck%5Cle+N-1)
可以看出复正弦谐波是一组标准正交基。
**3.标准化的DFT（Normalized DFT）**
对于标准正交基![\left\{s_k\right\}_{k=0}^{N-1}](https://www.zhihu.com/equation?tex=%5Cleft%5C%7Bs_k%5Cright%5C%7D_%7Bk%3D0%7D%5E%7BN-1%7D)和标准正交基矩阵![\mathbf{S}](https://www.zhihu.com/equation?tex=%5Cmathbf%7BS%7D%0A)，我们定义长度为![N](https://www.zhihu.com/equation?tex=N)的有限长信号![x](https://www.zhihu.com/equation?tex=x%0A)的标准化DFT为：
综合式(IDFT)：![x=\mathbf{S}X](https://www.zhihu.com/equation?tex=x%3D%5Cmathbf%7BS%7DX)

![x\left[n\right]=\sum_{k=0}^{N-1}{X\left[k\right]\frac{e^{j{\frac{2\pi}{N}kn}}}{\sqrt[]{N}}}](https://www.zhihu.com/equation?tex=x%5Cleft%5Bn%5Cright%5D%3D%5Csum_%7Bk%3D0%7D%5E%7BN-1%7D%7BX%5Cleft%5Bk%5Cright%5D%5Cfrac%7Be%5E%7Bj%7B%5Cfrac%7B2%5Cpi%7D%7BN%7Dkn%7D%7D%7D%7B%5Csqrt%5B%5D%7BN%7D%7D%7D+)

分析式(DFT)：![X=\mathbf{S}^Hx](https://www.zhihu.com/equation?tex=X%3D%5Cmathbf%7BS%7D%5EHx)



![X\left[k\right]=\left<x,s_k\right>=\sum_{n=0}^{N-1}{x\left[n\right]\frac{e^{-j{\frac{2\pi}{N}kn}}}{\sqrt[]{N}}}](https://www.zhihu.com/equation?tex=X%5Cleft%5Bk%5Cright%5D%3D%5Cleft%3Cx%2Cs_k%5Cright%3E%3D%5Csum_%7Bn%3D0%7D%5E%7BN-1%7D%7Bx%5Cleft%5Bn%5Cright%5D%5Cfrac%7Be%5E%7B-j%7B%5Cfrac%7B2%5Cpi%7D%7BN%7Dkn%7D%7D%7D%7B%5Csqrt%5B%5D%7BN%7D%7D%7D+)
通过标准正交基得到的DFT一种表达，也是比较容易被人理解的一种形式。但这并不是我们通常能够见到的DFT表达。
**4.未标准化的DFT（Unnormalized DFT）**
未标准化的DFT是通过正交基而非标准正交基得到的一种DFT表达，这也是我们常见的一种形式。这种形式可以避免计算上的复杂性，对于计算机来说，这是一种比较优雅的形式。由于传统，在书本、文献中一般统一采用这种DFT表达。
综合式(IDFT)：

![x\left[n\right]=\frac{1}{N}\sum_{k=0}^{N-1}{X_u\left[k\right]e^{j{\frac{2\pi}{N}kn}}}](https://www.zhihu.com/equation?tex=x%5Cleft%5Bn%5Cright%5D%3D%5Cfrac%7B1%7D%7BN%7D%5Csum_%7Bk%3D0%7D%5E%7BN-1%7D%7BX_u%5Cleft%5Bk%5Cright%5De%5E%7Bj%7B%5Cfrac%7B2%5Cpi%7D%7BN%7Dkn%7D%7D%7D+)

分析式(DFT)：

![X_u\left[k\right]=\sum_{n=0}^{N-1}{x\left[n\right]e^{-j{\frac{2\pi}{N}}kn}}](https://www.zhihu.com/equation?tex=X_u%5Cleft%5Bk%5Cright%5D%3D%5Csum_%7Bn%3D0%7D%5E%7BN-1%7D%7Bx%5Cleft%5Bn%5Cright%5De%5E%7B-j%7B%5Cfrac%7B2%5Cpi%7D%7BN%7D%7Dkn%7D%7D+)




### 三角插值

三角插值（trigonometric interpolation），常用的插值方法之一，指取插值函数为三角多项式的插值方法。特别适用于对周期函数的插值.设被插值函数f(x)为以2二为周期的函数，取n阶三角多项式，称上式为高斯三角插值公式。


三角多项式在数学分支，如数学分析和数值分析中都有应用，例如在傅里叶分析中，三角多项式被用于傅里叶级数的表示，在三角插值法中，三角多项式被用于逼近周期性函数。

一个函数T如果能够写成：的形式，其中



![](https://gss1.bdstatic.com/-vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D376/sign=c67f4d5fbc4543a9f11bfccb28168a7b/7af40ad162d9f2d3224ab744a2ec8a136227ccc6.jpg)

![](https://gss1.bdstatic.com/9vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D69/sign=a0f6d9a56c09c93d03f20dfe9e3d6c30/9f2f070828381f3080c51e02a2014c086f06f0b9.jpg)
，对于所有的an和bn都是复数，那么就称其为N阶复三角多项式。运用欧拉公式，这个函数可以写为：


![](https://gss3.bdstatic.com/-Po3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D211/sign=c9bd5484b5096b63851959513d318733/0df431adcbef760988cd0a3425dda3cc7dd99e53.jpg)



### 二者的联系
简单来说，DFT就是有限长信号的一种基变换，以复正弦谐波作为变换域的基是因为复正弦谐波是LTI系统的特征函数。这样，对于有限长信号，DFT就很自然成为分析LTI系统的工具了。离散傅里叶变换是将原来时域上N个数据点的数据变化到频域上，得到的是和原数据点数目相同的N个复数。



三角插值就是将得到的第k个复数的幅值A作为三角函数前的系数（也就是该频率分量的振幅），这个复数的幅角φ作为三角函数的相位，k作为三角函数的频率，得到一个三角函数。这样就将频域上的第k个复数变成了一个时域上该频率分量的连续函数。最后将所有频率的三角函数相加得到一个多项式，就是我们用来拟合这N个数据的函数，这样就可以估计出原数据点之间的插值点的函数值啦。当然实际上三角插值不需要通过离散傅里叶变换也可以直接用数据点的数据和一些公式求出这些振幅和相位，只不过离散傅里叶变换的步骤意义更加清楚。






