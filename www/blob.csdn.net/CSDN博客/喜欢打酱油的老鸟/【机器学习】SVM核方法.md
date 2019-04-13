
# 【机器学习】SVM核方法 - 喜欢打酱油的老鸟 - CSDN博客


2018年08月07日 12:25:52[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：170


[https://blog.csdn.net/qq_32742009/article/details/81430534](https://blog.csdn.net/qq_32742009/article/details/81430534)

# Kernel Trick
在 SVM 中引入核方法便可使得 SVM 变为非线性分类器，给定非线性可分数据集![\left \{ (x_i,y_i)\right\}_{i=1}^N](https://private.codecogs.com/gif.latex?%5Cleft%20%5C%7B%20%28x_i%2Cy_i%29%5Cright%5C%7D_%7Bi%3D1%7D%5EN)，如下图所示，此时找不到一个分类平面来将数据分开，核方法可以将数据投影到新空间，使得投影后的数据线性可分，下图给出一个![\mathbb{R}^2\rightarrow \mathbb{R}^2](https://private.codecogs.com/gif.latex?%5Cmathbb%7BR%7D%5E2%5Crightarrow%20%5Cmathbb%7BR%7D%5E2)的映射，原空间为![x=(x^{(1)},x^{(2)})](https://private.codecogs.com/gif.latex?x%3D%28x%5E%7B%281%29%7D%2Cx%5E%7B%282%29%7D%29),新空间为![z = \phi(x) = \left \{ (x^{(1)})^2,(x^{(2)})^2\right \}](https://private.codecogs.com/gif.latex?z%20%3D%20%5Cphi%28x%29%20%3D%20%5Cleft%20%5C%7B%20%28x%5E%7B%281%29%7D%29%5E2%2C%28x%5E%7B%282%29%7D%29%5E2%5Cright%20%5C%7D)，根据图可以看出映射后样本点的变化，此时样本便为线性可分的了，直接用![w_1 \cdot z^{(1)} +w_2 \cdot z^{(2)} +b= 0](https://private.codecogs.com/gif.latex?w_1%20%5Ccdot%20z%5E%7B%281%29%7D%20&plus;w_2%20%5Ccdot%20z%5E%7B%282%29%7D%20&plus;b%3D%200)分类即可。
![1](https://images2015.cnblogs.com/blog/743682/201608/743682-20160815173552906-243377824.png)
上图是一个![\mathbb{R}^2\rightarrow \mathbb{R}^2](https://private.codecogs.com/gif.latex?%5Cmathbb%7BR%7D%5E2%5Crightarrow%20%5Cmathbb%7BR%7D%5E2)的映射，但一般情况下，特征空间的选取往往是很高维度的![\mathbb{R}^2\rightarrow \mathbb{R}^n](https://private.codecogs.com/gif.latex?%5Cmathbb%7BR%7D%5E2%5Crightarrow%20%5Cmathbb%7BR%7D%5En),如下为一个![\mathbb{R}^2\rightarrow \mathbb{R}^3](https://private.codecogs.com/gif.latex?%5Cmathbb%7BR%7D%5E2%5Crightarrow%20%5Cmathbb%7BR%7D%5E3)的映射：
![1](https://images2015.cnblogs.com/blog/743682/201608/743682-20160815173607359-1015225033.png)
# 核函数
下面给**核函数**一个正式定义，设![\chi](https://private.codecogs.com/gif.latex?%5Cchi)为输入空间，![\omega](https://private.codecogs.com/gif.latex?%5Comega)为特征空间，如果存在一个![\chi](https://private.codecogs.com/gif.latex?%5Cchi)到![\omega](https://private.codecogs.com/gif.latex?%5Comega)的映射![\phi(x):\chi \rightarrow \omega](https://private.codecogs.com/gif.latex?%5Cphi%28x%29%3A%5Cchi%20%5Crightarrow%20%5Comega)，对所有的![x,z \in \chi](https://private.codecogs.com/gif.latex?x%2Cz%20%5Cin%20%5Cchi)，函数![K(x,z)](https://private.codecogs.com/gif.latex?K%28x%2Cz%29)满足![K(x,z) = \phi(x)\cdot\phi(z)](https://private.codecogs.com/gif.latex?K%28x%2Cz%29%20%3D%20%5Cphi%28x%29%5Ccdot%5Cphi%28z%29),则称![\phi(x)](https://private.codecogs.com/gif.latex?%5Cphi%28x%29)为输入空间到特征空间的映射函数，![K(x,z)](https://private.codecogs.com/gif.latex?K%28x%2Cz%29)为核函数。
核函数常用的技巧是不计算映射函数![\phi(x)](https://private.codecogs.com/gif.latex?%5Cphi%28x%29)，因为特征空间![\omega](https://private.codecogs.com/gif.latex?%5Comega)通常是高维的，甚至无穷维，所以![\phi(x)](https://private.codecogs.com/gif.latex?%5Cphi%28x%29)计算并不容易，而计算核函数![K(x,z)](https://private.codecogs.com/gif.latex?K%28x%2Cz%29)却相对简单。映射![\phi(x)](https://private.codecogs.com/gif.latex?%5Cphi%28x%29)取法多种多样，可以取不同的特征空间，即使在同一特征空间也可以取不同的映射。映射后的样本一般是线性可分带有异常值的，这时考虑 SVM 的优化目标：
![](http://latex.codecogs.com/png.latex?%5Cbegin%7Baligned%7D%20%26%5Cmin_a%20%5C%20%5C%20%5Cfrac%7B1%7D%7B2%7D%5Csum_%7Bi%3D1%7D%5EN%5Csum_%7Bj%3D1%7D%5EN%20a_ia_jy_iy_j%28x_i%20%5Ccdot%20x_j%29%20-%20%5Csum_%7Bi%3D1%7D%5ENa_i%20%5C%5C%20%26s.t.%20%5C%20%5C%20%5C%20%5C%20%5C%200%20%5Cle%20a_i%20%5Cle%20C%20%2C%20%5C%20i%20%3D%201%2C2%2C%5Ccdots%20%2CN%5C%5C%20%26%20%5C%20%5C%20%5C%20%5C%20%5C%20%5C%20%5C%20%5C%20%5C%20%5Csum_%7Bi%3D1%7D%5ENa_iy_i%20%3D%200%2C%5C%20i%20%3D%201%2C2%2C%5Ccdots%20%2CN%20%5Cend%7Baligned%7D)
在实际优化中还需加上**KKT条件**。
由于在输入空间计算的是![x_i ,x_j](https://private.codecogs.com/gif.latex?x_i%20%2Cx_j)的内积，所以经过映射后分别为![\phi(x_i)](https://private.codecogs.com/gif.latex?%5Cphi%28x_i%29)与![\phi(x_j)](https://private.codecogs.com/gif.latex?%5Cphi%28x_j%29)，现在只需修改目标函数为![\phi(x_i)](https://private.codecogs.com/gif.latex?%5Cphi%28x_i%29)与![\phi(x_j)](https://private.codecogs.com/gif.latex?%5Cphi%28x_j%29)的内积即可，又由于![\phi(x_i) \cdot \phi(x_j) = K(x_i,x_j)](https://private.codecogs.com/gif.latex?%5Cphi%28x_i%29%20%5Ccdot%20%5Cphi%28x_j%29%20%3D%20K%28x_i%2Cx_j%29)，所以不需要定义映射函数![\phi(x)](https://private.codecogs.com/gif.latex?%5Cphi%28x%29)，只需要定义核函数便可得到高维空间中内积的结果，而这便是**Kernel Trick**。所以线性不可分的数据集的优化目标变为：
![](http://latex.codecogs.com/png.latex?%5Cbegin%7Baligned%7D%20%26%5Cmin_a%20%5C%20%5C%20%5Cfrac%7B1%7D%7B2%7D%5Csum_%7Bi%3D1%7D%5EN%5Csum_%7Bj%3D1%7D%5EN%20a_ia_jy_iy_jK%28x_i%20%2C%20x_j%29%20-%20%5Csum_%7Bi%3D1%7D%5ENa_i%20%5C%5C%20%26s.t.%20%5C%20%5C%20%5C%20%5C%20%5C%200%20%5Cle%20a_i%20%5Cle%20C%20%2C%20%5C%20i%20%3D%201%2C2%2C%5Ccdots%20%2CN%5C%5C%20%26%20%5C%20%5C%20%5C%20%5C%20%5C%20%5C%20%5C%20%5C%20%5C%20%5Csum_%7Bi%3D1%7D%5ENa_iy_i%20%3D%200%2C%5C%20i%20%3D%201%2C2%2C%5Ccdots%20%2CN%20%5Cend%7Baligned%7D)
也就是说给定核函数![K(x,z)](https://private.codecogs.com/gif.latex?K%28x%2Cz%29)，即可用求解线性 SVM 的方法来求解非线性问题，核技巧的好处在**于不需要显式的定义特征空间与映射函数，只需要选择一个合适的核函数即可**。综上核函数是用来免去**显式**计算高维变换的，直接用**低维度的参数带入**核函数来**等价计算高维度的向量的内积**。
核函数的基本作用就是接受两个低维空间里的向量，能够计算出经过某个变换后在高维空间里的向量内积值。
不用去寻找高维空间的映射函数![\phi(x)](https://private.codecogs.com/gif.latex?%5Cphi%28x%29)，直接选定一个合适的核函数就好了（如高斯核就像是一个距离度量。）
插几句大白话，按照正常的思路来做，就是先计算出![x_1](https://private.codecogs.com/gif.latex?x_1)，![x_2](https://private.codecogs.com/gif.latex?x_2)映射到高维之后的![\phi (x_1)](https://private.codecogs.com/gif.latex?%5Cphi%20%28x_1%29),![\phi (x_2)](https://private.codecogs.com/gif.latex?%5Cphi%20%28x_2%29)，然后计算![\phi (x_1)](https://private.codecogs.com/gif.latex?%5Cphi%20%28x_1%29)与![\phi (x_2)](https://private.codecogs.com/gif.latex?%5Cphi%20%28x_2%29)的内积，假设![\phi (x)=x^2](https://private.codecogs.com/gif.latex?%5Cphi%20%28x%29%3Dx%5E2)这个是多项适核。注意一下，这里x不是一个变量而是一个向量![x=(x_1,x_2,\cdots ,x_N)](https://private.codecogs.com/gif.latex?x%3D%28x_1%2Cx_2%2C%5Ccdots%20%2Cx_N%29)，那么计算![\phi (x_1)](https://private.codecogs.com/gif.latex?%5Cphi%20%28x_1%29)需要![N^2](https://private.codecogs.com/gif.latex?N%5E2)次乘法，同理计算![\phi (x_2)](https://private.codecogs.com/gif.latex?%5Cphi%20%28x_2%29)也需要![N^2](https://private.codecogs.com/gif.latex?N%5E2)次乘法，然后计算![\phi (x_1)](https://private.codecogs.com/gif.latex?%5Cphi%20%28x_1%29)与![\phi (x_2)](https://private.codecogs.com/gif.latex?%5Cphi%20%28x_2%29)的乘机又需要![N^2](https://private.codecogs.com/gif.latex?N%5E2)的乘法。
![](https://img-blog.csdn.net/2018080516071073?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMyNzQyMDA5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
像这个例子一样。**一方面**乘法计算次数很多，计算代价很高。**另一方面**根本无法计算，例如高斯核，高斯核函数将样本映射成无穷维，无穷维的空间是无法单独计算![\phi (x)](https://private.codecogs.com/gif.latex?%5Cphi%20%28x%29)的。
那我们如果使用核函数，以多项式核为例。
![](https://img-blog.csdn.net/20180805161004267?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMyNzQyMDA5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180805161203793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMyNzQyMDA5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
直接计算x点乘y，只需要3次乘法，然后一个平方的乘法，共4次乘法。
计算代价就降低了很多，可以传入低维的特征更高效的去计算出与不用高斯核一样的结果，不用高斯核就需要先映射到高维空间，然后两个高维特征再相乘。
核函数那么好用，当然也有自身局限。一方面在于它难以构造，另一方面在于选择一个合适的核函数也是较为困难的一件事情，需要调参不少时间。
# 核函数的选择
什么样的函数![K(x,z)](https://private.codecogs.com/gif.latex?K%28x%2Cz%29)可以作为一个有效核函数呢？答案是只要满足**Mercer 定理**即可，即如果函数![K(x,z)](https://private.codecogs.com/gif.latex?K%28x%2Cz%29)是![\mathbb{R}^n \times \mathbb{R}^n \rightarrow \mathbb{R}](https://private.codecogs.com/gif.latex?%5Cmathbb%7BR%7D%5En%20%5Ctimes%20%5Cmathbb%7BR%7D%5En%20%5Crightarrow%20%5Cmathbb%7BR%7D)上的映射（ 也就是两个![n](https://private.codecogs.com/gif.latex?n)维向量映射到实数域 ）。那么如果![K(x,z)](https://private.codecogs.com/gif.latex?K%28x%2Cz%29)是一个有效核函数（也称为Mercer核函数），那么当且仅当其训练样本![\left \{x_1,x_2\cdots ,x_N \right \}](https://private.codecogs.com/gif.latex?%5Cleft%20%5C%7Bx_1%2Cx_2%5Ccdots%20%2Cx_N%20%5Cright%20%5C%7D)相应的核函数矩阵是对称半正定的，这里先解释一下**正定矩阵**：
![](https://img-blog.csdn.net/20180805153031985?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMyNzQyMDA5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
对于 N 个训练样本，每一个样本![x_i](https://private.codecogs.com/gif.latex?x_i)对应一个训练样例。那么，我们可以将任意两个![x_i](https://private.codecogs.com/gif.latex?x_i)和![x_j](https://private.codecogs.com/gif.latex?x_j)带入核函数中，计算![K_{ij} = K(x_i,x_j)](https://private.codecogs.com/gif.latex?K_%7Bij%7D%20%3D%20K%28x_i%2Cx_j%29)。这样可以把![K_{ij}](https://private.codecogs.com/gif.latex?K_%7Bij%7D)表示为一个![m \times m](https://private.codecogs.com/gif.latex?m%20%5Ctimes%20m)的 Gram 矩阵，只要 Gram 矩阵为对称半正定的(半正定就是大于等于0)，则![K(x,z)](https://private.codecogs.com/gif.latex?K%28x%2Cz%29)即为一个有效的核函数，Gram 矩阵如下：
![](http://latex.codecogs.com/png.latex?%5Cbegin%7Bbmatrix%7D%20K_%7B11%7D%26%20K_%7B12%7D%26%20%5Ccdots%26%20K_%7B1m%7D%26%20%5C%5C%20K_%7B21%7D%26%20K_%7B22%7D%26%20%5Ccdots%26%20K_%7B2m%7D%26%20%5C%5C%20%5Cvdots%20%26%20%5Cvdots%26%20%5Cddots%26%20%5Cvdots%26%20%5C%5C%20K_%7Bm1%7D%26%20K_%7Bm2%7D%26%20%5Ccdots%26%20K_%7Bmm%7D%20%5Cend%7Bbmatrix%7D)
显然对于自己定义的核函数判定是否为正定核不太容易，所以在工业生产中一般使用一些常用的核函数，下面给出几个：
1）**线性核**：线性核其实就是不采用非线性分类器，认为样本是线性可分的；
![](http://latex.codecogs.com/png.latex?K%28x%2Cz%29%20%3D%20x%20%5Ccdot%20z%20&plus;c)
2）**多项式核**：该核函数对应的是一个 p 次多项式的分类器，这时需要额外调节的参数为 c ，p ；
![](http://latex.codecogs.com/png.latex?K%28x%2Cz%29%20%3D%20%28x%20%5Ccdot%20z%20&plus;c%29%5Ep)
多项式核函数可以实现将低维的输入空间映射到高纬的特征空间，但是多项式核函数的参数多，当多项式的阶数比较高的时候，核矩阵的元素值将趋于无穷大或者无穷小，计算复杂度会大到无法计算。
3）**高斯核(RBF核函数)：**或者叫做径向基核，该核函数甚至可以将特征空间映射为无穷维，这时需要额外调节的参数为![\delta](https://private.codecogs.com/gif.latex?%5Cdelta)；
![](http://latex.codecogs.com/png.latex?K%28x%2Cz%29%20%3D%20exp%20%5Cleft%20%28%20%5Cfrac%7B-%7C%7Cx-z%7C%7C%5E2%20%7D%7B2%20%5Cdelta%5E2%7D%5Cright%20%29)
高斯径向基函数是一种局部性强的核函数，其可以将一个样本映射到一个更高维的空间内，该核函数是应用最广的一个，无论大样本还是小样本都有比较好的性能，而且其相对于多项式核函数参数要少，因此大多数情况下在不知道用什么核函数的时候，优先使用高斯核函数。
如果![\delta](https://private.codecogs.com/gif.latex?%5Cdelta)选得很大的话，高次特征上的权重实际上衰减得非常快，所以实际上（数值上近似一下）相当于一个低维的子空间；反过来，如果![\delta](https://private.codecogs.com/gif.latex?%5Cdelta)选得很小，则可以将任意的数据映射为线性可分，当然，这并不一定是好事，因为随之而来的可能是非常严重的过拟合问题。总的来说，通过调控参数![\delta](https://private.codecogs.com/gif.latex?%5Cdelta)，高斯核实际上具有相当高的灵活性，也是使用最广泛的核函数之一。
![](https://img-blog.csdn.net/20170626152424353?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGluemhpOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![\delta](https://private.codecogs.com/gif.latex?%5Cdelta)越大，山顶就越尖，就越容易被线性可分，但容易过拟合。
4）**sigmoid核函数：**Sigmoid核函数来源于神经网络，被广泛用于深度学习和机器学习中。采用sigmoid核函数，支持向量机实现的就是一种多层神经网络。
![](http://latex.codecogs.com/png.latex?K%28x%2C%20z%29%20%3D%20tanh%28%5Ceta%3Cx%2C%20z%3E%20&plus;%20%5Ctheta%29)
支持向量机的理论基础决定了它最终求得的是全局最优值而不是局部最小值，也保证了它对于未知样本的良好泛化能力而不会出现过学习现象。
# Algorithm 1.4
综上，给出**非线性可分支持向量机的学习算法1.4**：
![](https://img-blog.csdn.net/20180805163125556?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzMyNzQyMDA5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
参考文章：
SVM核函数
核函数实例

