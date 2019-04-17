# 高斯混合模型的重叠度计算 （Overlap Rate, OLR） - 小平子的专栏 - CSDN博客





2017年05月18日 15:44:36[阿拉丁吃米粉](https://me.csdn.net/jinping_shi)阅读数：2993








# 简介

本文章实现了Haojun Sun提出的一种计算高斯混合模型（GMM）重叠率的方法（论文：[Measuring the component overlapping in the Gaussian mixture model](https://link.springer.com/article/10.1007/s10618-011-0212-3)）。这篇文论提出的方法可以计算任意两个混合高斯分布之间的重叠度。

关于高斯混合模型（GMM）的相关概念可以参考另一篇博文：[高斯混合模型及其EM算法的理解](http://46aae4d1e2371e4aa769798941cef698.devproxy.yunshipei.com/jinping_shi/article/details/59613054)

使用GMM聚类或分析两个高斯混合分布的数据时，我们有时会希望两个高斯分布离得越远越好，这样表示数据才有可分性。但很多情况下两个高斯分布会有重叠。一维和二维的重叠情况如下所示（图片取自作者论文）。

![overlap example](https://img-blog.csdn.net/20170518153741939?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlucGluZ19zaGk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

我们可以计算一些指标来间接反映两个高斯分布的重叠情况。比如可以计算Mahalanobis距离，Bhattacharyya距离或Kullback-Leibler (KL)距离，可以衡量两个高斯分布的相似性。但是Mahalanobis距离预设两个分布具有相同的协方差，Bhattacharyya距离和KL距离都考虑了协方差，但却没有考虑高斯混合分布的混合系数（mixing coefficient）。而且KL距离对高维的正态分布没有解析解，计算复杂。

这篇论文提出的计算OLR的方法考虑了高斯混合分布中的所有参数，包括均值，协方差和混合系数。

# OLR计算

假设有$n$个$d$维的样本$\boldsymbol{X} = \{X_1,..., X_n\}$. 其中$X_i$是一个$d$维向量。一个混合高斯模型的pdf可以表示为： 


$p(X) = \sum_{i=1}^k \alpha_iG_i(X, \mu_i, \Sigma_i) \tag{1}$

其中$\alpha_i$是混合系数，满足$\alpha_i > 0$且$\sum_{i=1}^k\alpha_i=1$.
$G_i(X)$是一个$d$维高斯分布，可以表示为下面的形式： 


$G_i(X) = \frac{1}{(2\pi)^{d/2} |\Sigma_i|^{1/2}} \exp \left( \frac{1}{2} (X-\mu_i)^T \Sigma_i^{-1}(X-\mu_i)\right) \tag{2}$.

以二维高斯分布为例。当两个高斯分布有重叠时，会形成鞍状。如上图的d和e，二维高斯分布混合时会出现两个峰和一个鞍部；当两个分布几乎完全混合时，鞍部可能消失，但峰还在，此时明显的峰只有一个，如上图中的f。

论文中的两个高斯分布的OLR定义如下： 


$OLR(G_1, G_2) = \begin{cases}	1 &\mbox{if $p(X)$ has one peak} \\	\frac{p(X_{saddle})}{p(X_{submax})} &\mbox{if $p(X)$ has two peaks}\end{cases} \tag{3}$

其中$X_{saddle}$是pdf中的鞍点（saddle point），$X_{submax}$是pdf中的较低的峰（lower peak point）。OLR的示意图如下图所示。OLR计算的是鞍点的pdf与较低峰的pdf的比值。这么做是因为鞍点的pdf与混合系数$\alpha_i$有关。注意到OLR并不是落在重叠区域内数据的比例。定义中的$p(X_{submax})$容易求，只需将两个均值带入(1)试，取较小的值即可。但是$p(X_{saddle})$不容直接求得。

![OLR](https://img-blog.csdn.net/20170518153821267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlucGluZ19zaGk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注意到两个峰点和鞍点在整个曲面上都应该是极值点。因此$X_{saddle}$和$X_{submax}$应该满足下式： 


$\begin{cases}	\frac{\partial p}{\partial x_1} = A_{x_1}\alpha_1G_1 + B_{x_1}\alpha_2 G_2  \\	\frac{\partial p}{\partial x_2} = A_{x_2}\alpha_1G_1 + B_{x_2}\alpha_2 G_2\end{cases} \tag{4}$

其中， 


$\begin{split}\left(\begin{matrix}A_{x_1} \\A_{x_2}\end{matrix}\right) = \nabla ||X-\mu_1||_{\Sigma_1^{-1}}^2 = -\Sigma_1^{-1}(X - \mu_1) \\\left(\begin{matrix}B_{x_1} \\B_{x_2}\end{matrix}\right)= \nabla ||X-\mu_2||_{\Sigma_1^{-1}}^2 = -\Sigma_1^{-1}(X - \mu_2) \end{split} \tag{5}$
如果$X$已知，(5)式可求出。论文接下来证明，峰点和鞍点会在同一条曲线上，曲线方程如下： 


$A_{x_1} B_{x_2} - B_{x_1} A_{x_2} = 0 \tag{6}$

而且，鞍点会在以两个峰点（均值处的pdf）之间的曲线段上。因此只要从第一个均值开始，沿着曲线(4)一直找到另一个均值，这个过程中的极小值点就是鞍点。得到鞍点的坐标，带入(1)式，就可以求得鞍点的pdf值。(6)式中的曲线称为**Ridge Curve (RC)**.

OLR的算法如下： 

1. 输入混合高斯分布的参数$(\mu_1, \mu_2, \Sigma_1, \Sigma_2, \alpha_1, \alpha_2)$

2. 计算RC： $A_{x_1} B_{x_2} - B_{x_1} A_{x_2} = 0$

3. 沿着RC，从$\mu_1$到$\mu_2$按步长$\delta$找到RC中$p(X)$取得最大值和最小值的点 

    3.1 令$X_0 = \mu_1$，$X_0$的下一个点$X_{i+1}$的第一维（x坐标）$X_{i+1}^1 = \{X_i + \delta(\mu_1-\mu_2)\}^1$. 

    3.2 将$X_{i+1}^1$带入RC方程(6)，求得$X_{i+1}$的第二维（y坐标）$X_{i+1}^2$

    3.3 根据(1)式计算$p(X_i)$

    3.4 if $p(X_i) - p(X_{i-1}) > 0$ and $p(X_i) - p(X_{i+1}) > 0$, $X_i$ is maximum point (peak) 

    3.5 if $p(X_i) - p(X_{i-1}) < 0$ and $p(X_i) - p(X_{i+1}) < 0$, $X_i$ is minimum point 

4. 根据(3)式计算OLR
上述算法$\delta$可以取$\delta = ||\mu_1 - \mu_2|| / 1000$. 作者认为当OLR小于0.6时，两个类别可分性良好（visually well separated），当OLR大于0.8时，两个类别严重重叠（strongly overlapping）。

# 算法实现

求$p(X_i)$可以用scipy.stats中的multivariate_normal计算。输入两个高斯分布的参数可以求出pdf值。

完整代码可以参考[GMM Overlap Rate](https://github.com/SJinping/GMM-overlap-rate)。论文中给出的算法有一些问题。

```python
import math
import numpy as np
import matplotlib.pyplot as plt
from numpy.linalg import inv
from scipy.stats import multivariate_normal

class BiGauss(object):
    """docstring for BiGauss"""
    def __init__(self, mu1, mu2, Sigma1, Sigma2, pi1, pi2, steps = 100):
        super(BiGauss, self).__init__()
        self.mu1      = mu1
        self.mu2      = mu2
        self.Sigma1   = Sigma1
        self.Sigma2   = Sigma2
        self.pi1      = pi1
        self.pi2      = pi2
        self.biGauss1 = multivariate_normal(mean = self.mu1, cov = self.Sigma1, allow_singular = True)
        self.biGauss2 = multivariate_normal(mean = self.mu2, cov = self.Sigma2, allow_singular = True)
        self.steps    = steps
        self.inv_Sig1 = -inv(self.Sigma1)
        self.inv_Sig2 = -inv(self.Sigma2)

        # variables to calculate RC
        self.A_1 = self.inv_Sig1[0][0]
        self.B_1 = self.inv_Sig1[0][1]
        self.C_1 = self.inv_Sig1[1][0]
        self.D_1 = self.inv_Sig1[1][1]
        self.A_2 = self.inv_Sig2[0][0]
        self.B_2 = self.inv_Sig2[0][1]
        self.C_2 = self.inv_Sig2[1][0]
        self.D_2 = self.inv_Sig2[1][1]
```

计算pdf

```python
def pdf(self, x):
        return self.pi1 * self.biGauss1.pdf(x) + self.pi2 * self.biGauss2.pdf(x)
```

根据$x$求出$y$，使得$(x,y)$在RC上

```python
def RC(self, x):
        E = self.A_1 * (x - self.mu1[0])
        F = self.C_1 * (x - self.mu1[0])
        G = self.A_2 * (x - self.mu2[0])
        H = self.C_2 * (x - self.mu2[0])

        I = E * self.D_2 - F * self.B_2
        J = H * self.B_1 - G * self.D_1
        K = self.B_1 * self.D_2 - self.B_2 * self.D_1
        M = F * G - E * H

        P = K
        Q = I + J - K * (self.mu2[1] + self.mu1[1])
        S = -(M + I * self.mu2[1] + J * self.mu1[1])

        if Q**2 - 4*P*S < 0:
            return None

        y = max((-Q + math.sqrt(Q**2 - 4*P*S)) / (2*P), (-Q - math.sqrt(Q**2 - 4*P*S)) / (2*P))

        return y
```

求OLR

```python
def OLR(self):
        e      = math.sqrt((self.mu1[0] - self.mu2[0])**2 + (self.mu1[1] - self.mu2[1])**2) / float(self.steps)
        x_step = e*(self.mu1[0]-self.mu2[0]) # each step for x
        y_step = e*(self.mu1[1]-self.mu2[1]) # each step for y
        p_x    = self.mu1[0] - x_step

        while self.RC(p_x) == None:
            p_x = p_x - x_step

        p_y   = self.RC(p_x)
        p     = [p_x, p_y]
        p_pre = self.mu1
        p_min = min(self.pdf(p), self.pdf(p_pre))
        p_max = max(self.pdf(p), self.pdf(p_pre))
        index = 0
        while index < self.steps:
            if self.RC(p[0] - x_step) != None:
                p_next = [p[0] - x_step, self.RC(p[0] - x_step)] # next point on ridge curve
                if self.pdf(p) > self.pdf(p_pre) and self.pdf(p) > self.pdf(p_next):
                    p_max = self.pdf(p)
                if self.pdf(p) < self.pdf(p_pre) and self.pdf(p) < self.pdf(p_next):
                    p_min = self.pdf(p)
            p_pre = p
            p     = p_next
            index += 1

        pdf_mu1 = self.pdf(self.mu1)
        pdf_mu2 = self.pdf(self.mu2)
        return p_min / min(pdf_mu1, pdf_mu2) if p_min < min(pdf_mu1, pdf_mu2) else 1.0
```

上述代码有时会计算出OLR大于1的情况，还没有分析原因。因此代码中做了限制，如果求出的OLR大于1，那么只会返回1.

论文中探讨了混合系数、均值间距离和协方差对OLR的影响。论文中给出了一个例子，如下。当$\alpha_1 = 0.46$时该例子可以取到最小的OLR $r_{min}$. 论文没有给出$r_{min}$的具体数值，但是给出了OLR随$\alpha_1$取值变化的曲线图。上述代码算出来的结果是$r_{min} = 0.660$，也确实在$\alpha_1 = 0.46$处取得。与曲线图中的位置吻合。论文中提到当$\alpha_1 = 0.3$时，ORL等于0.7288，上述代码给出的结果是0.7270.

示例代码中也画出了OLR随$\alpha_1$变化的曲线图和OLR随两个均值之间距离变化的曲线图。曲线走势与论文中的图示一致，但具体数值有些差别。






