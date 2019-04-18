# 理解EM算法 - 知乎
# 

获取全文PDF请查看：[理解EM算法](https://link.zhihu.com/?target=http%3A//www.tensorinfinity.com/paper_64.html)

 EM（ expectation-maximization，期望最大化）算法是机器学习中与SVM（支持向量机）、概率图模型并列的难以理解的算法，主要原因在于其原理较为抽象，初学者无法抓住核心的点并理解算法求解的思路。本文对EM算法的基本原理进行系统的阐述，并以求解高斯混合模型为例说明其具体的用法。文章是对已经在清华大学出版社出版的《机器学习与应用》一书中EM算法的讲解，对部分内容作了扩充。

算法的历史

EM算法即期望最大化算法，由Dempster等人在1976年提出[1]。这是一种迭代法，用于求解含有隐变量的最大似然估计、最大后验概率估计问题。至于什么是隐变量，在后面会详细解释。EM算法在机器学习中有大量成功的应用，典型是求解高斯混合模型，隐马尔可夫模型。如果你要求解的机器学习模型中有隐变量存在，并且要估计模型的参数，EM算法很多时候是首选算法。

Jensen不等式

EM算法的推导、收敛性证明依赖于Jensen不等式，我们先对它做一简单介绍。Jensen不等式的表述是，如果f(x)是凸函数，x是随机变量，则下面不等式成立

![E(f(x))>=f(E(x))](https://www.zhihu.com/equation?tex=E%28f%28x%29%29%3E%3Df%28E%28x%29%29)

在这里E是数学期望，对于离散型随机变量，数学期望是求和，对连续型随机变量则为求定积分。如果f(x)是一个严格凸函数，当且仅当x是常数时不等式取等号：

![E(f(x))=f(E(x)) ](https://www.zhihu.com/equation?tex=E%28f%28x%29%29%3Df%28E%28x%29%29+)

如果对这一不等式的证明感兴趣，可以阅读相关的数学教材。

高斯混合模型

EM算法的目标是求解似然函数或后验概率的极值，而样本中具有无法观测的隐含变量。下面以聚类问题和高斯混合模型为例进行说明。有一批样本，分属于3个类，假设每个类都服从正态分布，均值和协方差未知，各样本属于哪个类也是未知的，算法需要在此条件下估计每个正态分布的均值和协方差。下图是一个例子，3类样本都服从正态分布，但每个样本属于哪个类是未知的：
![](https://pic3.zhimg.com/v2-d30b736e314aede45d6cc4c5c78dacda_b.png)
样本所属的类别就是隐变量，我们无法直接观察到它的值，这种隐变量的存在导致了用最大似然估计求解时的困难，后面会解释。上面这个例子可以用高斯混合模型进行描述，它的概率密度函数是多个高斯分布（正态分布）的加权和。

高斯混合模型（Gaussian Mixture Model，简称GMM）通过多个正态分布的加权和来描述一个随机变量的概率分布，概率密度函数定义为：

![\(p(x)=\sum_{i=1}^{k}w_{i}N_{i}(x;\mu _{i},\Sigma _{i})\) ](https://www.zhihu.com/equation?tex=%5C%28p%28x%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bk%7Dw_%7Bi%7DN_%7Bi%7D%28x%3B%5Cmu+_%7Bi%7D%2C%5CSigma+_%7Bi%7D%29%5C%29+)

其中x为随机向量，k为高斯分布的数量， ![w_{i}](https://www.zhihu.com/equation?tex=w_%7Bi%7D) 为高斯分布的权重，是一个正数，μ为高斯分布的均值向量，∑为协方差矩阵。所有高斯分布的权重之和为1，即：

![ \(\sum_{i=1}^{k}w_{i}=1\)  ](https://www.zhihu.com/equation?tex=+%5C%28%5Csum_%7Bi%3D1%7D%5E%7Bk%7Dw_%7Bi%7D%3D1%5C%29++)

高斯混合模型的样本可以看作是这样产生的：

先从k个高斯分布中选择出一个，选择第i个高斯分布的概率为![w_{i}](https://www.zhihu.com/equation?tex=w_%7Bi%7D)，再由第i个高斯分布N(x, ![\mu_{i}](https://www.zhihu.com/equation?tex=%5Cmu_%7Bi%7D) , ![\Sigma_{i}](https://www.zhihu.com/equation?tex=%5CSigma_%7Bi%7D) )产生出样本数据x。

高斯混合模型可以逼近任何一个连续的概率分布，因此它可以看做是连续型概率分布的万能逼近器。之所有要保证权重的和为1，是因为概率密度函数必须满足（+∞，-∞）在内的积分值为1。

回忆一下用最大似然估计来确定单个高斯分布的参数的过程，给定一组训练样本，构造它们的对数似然函数，对参数求导并令导数为0，即可通过最大化对数似然函数而确定高斯分布的参数。

对于高斯混合模型，也可以使用最大似然估计确定模型的参数，但每个样本属于哪个高斯分布是未知的，而计算高斯分布的参数时需要用到这个信息；反过来，样本属于哪个高斯分布又是由高斯分布的参数确定的。因此存在循环依赖，解决此问题的办法是打破此循环依赖，从高斯分布的一个不准确的初始猜测值开始，计算样本属于每个高斯分布的概率，然后又根据这个概率更新每个高斯分布的参数。这就是EM算法求解时的做法。

从另外一个角度看，高斯混合模型的对数似然函数为：

![\(\sum_{i=1}^{l}\ln \left ( \sum_{j=1}^{k}w_{j}N_{j}(x;\mu _{j},\Sigma _{j}) \right )\)](https://www.zhihu.com/equation?tex=%5C%28%5Csum_%7Bi%3D1%7D%5E%7Bl%7D%5Cln+%5Cleft+%28+%5Csum_%7Bj%3D1%7D%5E%7Bk%7Dw_%7Bj%7DN_%7Bj%7D%28x%3B%5Cmu+_%7Bj%7D%2C%5CSigma+_%7Bj%7D%29+%5Cright+%29%5C%29)

由于对数函数中有k个求和项，以及参数 ![w_{j}](https://www.zhihu.com/equation?tex=w_%7Bj%7D) 的存在，无法像单个高斯模型那样通过最大似然估计求得公式解。如果用梯度下降法近似求解，则要保证![w_{j}](https://www.zhihu.com/equation?tex=w_%7Bj%7D)非负并且和为1，同样存在困难。

算法的推导

下面考虑一般的情况。有一个概率分布p(x,θ)，从它生成了个样本。每个样本包含观测数据 ![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D) ，以及无法观测到的隐变量![z_{i}](https://www.zhihu.com/equation?tex=z_%7Bi%7D)，这个概率分布的参数θ是未知的，现在需要根据这些样本估计出参数θ的值。因为不知道隐变量的值，所以要消掉它，这通过对其求边缘概率而实现。采用最大似然估计，可以构造出对数似然函数：

![\(L(\theta )=\sum_{i=1}^{l}\ln p(x_{i};\theta )\\=\sum_{i=1}^{l}\ln \sum _{z}p(x_{i},z_{i};\theta )\) ](https://www.zhihu.com/equation?tex=%5C%28L%28%5Ctheta+%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bl%7D%5Cln+p%28x_%7Bi%7D%3B%5Ctheta+%29%5C%5C%3D%5Csum_%7Bi%3D1%7D%5E%7Bl%7D%5Cln+%5Csum+_%7Bz%7Dp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%5C%29+)

这里的![z_{i}](https://www.zhihu.com/equation?tex=z_%7Bi%7D)是一个无法观测到（即不知道它的值）的隐含变量，可以看作离散型随机变量，上式对隐含变量z的所有情况下的联合概率p(x,z,θ)求和得到x的边缘概率。因为隐含变量的存在，无法直接通过最大化似然函数得到参数的公式解。如果使用梯度下降法或牛顿法求解，则要保证隐变量所满足的等式和不等式约束

![ \(\sum _{z}p(z)=1\\p(z)\geq 0\) ](https://www.zhihu.com/equation?tex=+%5C%28%5Csum+_%7Bz%7Dp%28z%29%3D1%5C%5Cp%28z%29%5Cgeq+0%5C%29+)

这同样存在困难。

 EM算法所采用的思路是构造出对数似然函数的一个下界函数，这个下界函数更容易优化，然后优化这个下界。不断的改变优化变量的值使得下界函数的值升高，从而使得对数似然函数的值也上升。

对每个样本i，假设*Qi*为隐变量![z_{i}](https://www.zhihu.com/equation?tex=z_%7Bi%7D)的一个概率分布，根据对概率分布的要求它必须满足：

![\(\sum _{z}Q_{i}(z)=1\\Q_{i}(z)\geq 0\)](https://www.zhihu.com/equation?tex=%5C%28%5Csum+_%7Bz%7DQ_%7Bi%7D%28z%29%3D1%5C%5CQ_%7Bi%7D%28z%29%5Cgeq+0%5C%29)

利用这个概率分布，将对数似然函数变形，可以得到：

![ \(\sum_{i=1}^{l}\ln p(x_{i};\theta )=\sum_{i=1}^{l}\ln \sum _{z_{i}}p(x_{i},z_{i};\theta )\\=\sum_{i=1}^{l}\ln \sum _{z_{i}}Q_{i}(z_{i})\frac{p(x_{i},z_{i};\theta )}{Q_{i}(z_{i})}\\\geq \sum_{i=1}^{l}\sum _{z_{i}}Q_{i}(z_{i})\ln \frac{p(x_{i},z_{i};\theta )}{Q_{i}(z_{i})}\) ](https://www.zhihu.com/equation?tex=+%5C%28%5Csum_%7Bi%3D1%7D%5E%7Bl%7D%5Cln+p%28x_%7Bi%7D%3B%5Ctheta+%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bl%7D%5Cln+%5Csum+_%7Bz_%7Bi%7D%7Dp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%5C%5C%3D%5Csum_%7Bi%3D1%7D%5E%7Bl%7D%5Cln+%5Csum+_%7Bz_%7Bi%7D%7DQ_%7Bi%7D%28z_%7Bi%7D%29%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D%5C%5C%5Cgeq+%5Csum_%7Bi%3D1%7D%5E%7Bl%7D%5Csum+_%7Bz_%7Bi%7D%7DQ_%7Bi%7D%28z_%7Bi%7D%29%5Cln+%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D%5C%29+)

上式的第二步凑出了数学期望，最后一步利用了Jensen不等式。如果令函数

![ f(x)=Inx ](https://www.zhihu.com/equation?tex=+f%28x%29%3DInx+)

按照数学期望的定义（注意，在这里![z_{i}](https://www.zhihu.com/equation?tex=z_%7Bi%7D)是随机变量，是对它求数学期望），有： ![ \(\ln \sum _{z_{i}}Q_{i}(z_{i})\frac{p(x_{i},z_{i};\theta )}{Q_{i}(z_{i})}\\=f\left ( E_{Q_{i}(z_{i})}\left ( \frac{p(x_{i},z_{i};\theta )}{Q_{i}(z_{i})} \right )\right )=ln\left ( E_{Q_{i}(z_{i})}\left ( \frac{p(x_{i},z_{i};\theta )}{Q_{i}(z_{i})} \right ) \right )\\ \geq E_{Q_{i}(z_{i})}f\left ( \frac{p(x_{i},z_{i};\theta )}{Q_{i}(z_{i})} \right )=E_{Q_{i}(z_{i})}ln\left ( \frac{p(x_{i},z_{i};\theta )}{Q_{i}(z_{i})} \right )\\=\sum _{z_{i}}Q_{i}(z_{i})ln\frac{p(x_{i},z_{i};\theta )}{Q_{i}(z_{i})}\) ](https://www.zhihu.com/equation?tex=+%5C%28%5Cln+%5Csum+_%7Bz_%7Bi%7D%7DQ_%7Bi%7D%28z_%7Bi%7D%29%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D%5C%5C%3Df%5Cleft+%28+E_%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D%5Cleft+%28+%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D+%5Cright+%29%5Cright+%29%3Dln%5Cleft+%28+E_%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D%5Cleft+%28+%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D+%5Cright+%29+%5Cright+%29%5C%5C+%5Cgeq+E_%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7Df%5Cleft+%28+%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D+%5Cright+%29%3DE_%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7Dln%5Cleft+%28+%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D+%5Cright+%29%5C%5C%3D%5Csum+_%7Bz_%7Bi%7D%7DQ_%7Bi%7D%28z_%7Bi%7D%29ln%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D%5C%29+)

对数函数是凹函数，Jensen不等式反号。上式给出了对数似然函数的一个下界，*Qi*可以是任意一个概率分布，因此可以利用参数θ的当前估计值来构造*Qi*。显然，这个下界函数更容易求极值，因为对数函数里面已经没有求和项，对参数求导并令导数为0时一般可以得到公式解。

算法的流程

算法在实现时首先初始化参数θ的值，接下来循环迭代直至收敛，每次迭代时分为两步：

E步，基于当前的参数估计值θ*i*，计算在给定x时对隐变量z的条件概率：

![\(Q_{i}(z_{i})=p({z_{i}|x_{i};\theta })\) ](https://www.zhihu.com/equation?tex=%5C%28Q_%7Bi%7D%28z_%7Bi%7D%29%3Dp%28%7Bz_%7Bi%7D%7Cx_%7Bi%7D%3B%5Ctheta+%7D%29%5C%29+)

接下来根据该概率论构造目标函数（下界函数），这个目标函数是对z的数学期望，这就是EM算法中“E”的含义。

M步，求解如下极值问题，更新θ的值：

![\(\theta =argmax_{\theta }\sum _{i}\sum _{z_{i}}Q_{i}(z_{i})ln\frac{p(x_{i},z_{i};\theta )}{Q_{i}(z_{i})}\)](https://www.zhihu.com/equation?tex=%5C%28%5Ctheta+%3Dargmax_%7B%5Ctheta+%7D%5Csum+_%7Bi%7D%5Csum+_%7Bz_%7Bi%7D%7DQ_%7Bi%7D%28z_%7Bi%7D%29ln%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D%5C%29)

上面的目标函数中对数内部没有求和项，更容易求得θ的公式解。这就是EM算法中“M”的含义。由于*Qi*可以是任意个概率分布，实现时*Qi*可以按照下面的公式计算：

![\(Q_{i}(z_{i})=\frac{p(x_{i},z_{i};\theta )}{_{z} ^{\sum}p(x_{i},z;\theta )}\)](https://www.zhihu.com/equation?tex=%5C%28Q_%7Bi%7D%28z_%7Bi%7D%29%3D%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7B_%7Bz%7D+%5E%7B%5Csum%7Dp%28x_%7Bi%7D%2Cz%3B%5Ctheta+%29%7D%5C%29)

迭代终止的判定规则是相邻两次函数值之差小于指定阈值。

收敛性的证明

假设第t次迭代时的参数值为*θt*，第t+1次迭代时的参数值为*θt+1*。如果能证明每次迭代时对数似然函数的值单调增，即：

![\(L(\theta _{t})\leq L(\theta _{t+1})\)](https://www.zhihu.com/equation?tex=%5C%28L%28%5Ctheta+_%7Bt%7D%29%5Cleq+L%28%5Ctheta+_%7Bt%2B1%7D%29%5C%29)

则算法能收敛到局部极值点。由于在迭代时选择了：

![ \(Q_{it}(z_{i})=p({z_{i}|x_{i};\theta _{t}})\) ](https://www.zhihu.com/equation?tex=+%5C%28Q_%7Bit%7D%28z_%7Bi%7D%29%3Dp%28%7Bz_%7Bi%7D%7Cx_%7Bi%7D%3B%5Ctheta+_%7Bt%7D%7D%29%5C%29+)

因此有：

![\(\frac{p(x_{i},z_{i};\theta )}{Q_{i}(z_{i})}=\frac{p(x_{i},z_{i};\theta )}{p({z_{i}|x_{i};\theta _{t}})}=\frac{p(x_{i},z_{i};\theta )}{p(x_{i},z_{i};\theta )/p(x_{i};\theta )}=p(x_{i};\theta )\)](https://www.zhihu.com/equation?tex=%5C%28%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7BQ_%7Bi%7D%28z_%7Bi%7D%29%7D%3D%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7Bp%28%7Bz_%7Bi%7D%7Cx_%7Bi%7D%3B%5Ctheta+_%7Bt%7D%7D%29%7D%3D%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%7D%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+%29%2Fp%28x_%7Bi%7D%3B%5Ctheta+%29%7D%3Dp%28x_%7Bi%7D%3B%5Ctheta+%29%5C%29)

这和![z_{i}](https://www.zhihu.com/equation?tex=z_%7Bi%7D)无关，因此是一个常数，从而保证Jensen不等式可以取等号。因此有下面的等式成立：

![\(L(\theta _{t})=\sum _{i}ln\sum _{z_{i}}Q_{it}(z_{i})\frac{p(x_{i},z_{i};\theta_t )}{Q_{it}(z_{i})}=\sum _{i}\sum _{z_{i}}Q_{it}(z_{i})ln\frac{p(x_{i},z_{i};\theta_t )}{Q_{it}(z_{i})}\)](https://www.zhihu.com/equation?tex=%5C%28L%28%5Ctheta+_%7Bt%7D%29%3D%5Csum+_%7Bi%7Dln%5Csum+_%7Bz_%7Bi%7D%7DQ_%7Bit%7D%28z_%7Bi%7D%29%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta_t+%29%7D%7BQ_%7Bit%7D%28z_%7Bi%7D%29%7D%3D%5Csum+_%7Bi%7D%5Csum+_%7Bz_%7Bi%7D%7DQ_%7Bit%7D%28z_%7Bi%7D%29ln%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta_t+%29%7D%7BQ_%7Bit%7D%28z_%7Bi%7D%29%7D%5C%29)

从而有： ![\(L(\theta _{t+1})\geq \sum _{i}\sum _{z_{i}}Q_{it}(z_{i})ln\frac{p(x_{i},z_{i};\theta _{t+1})}{Q_{it}(z_{i})}\\\geq \sum _{i}\sum _{z_{i}}Q_{it}(z_{i})ln\frac{p(x_{i},z_{i};\theta _{t})}{Q_{it}(z_{i})}\\=L(\theta _{t})\) ](https://www.zhihu.com/equation?tex=%5C%28L%28%5Ctheta+_%7Bt%2B1%7D%29%5Cgeq+%5Csum+_%7Bi%7D%5Csum+_%7Bz_%7Bi%7D%7DQ_%7Bit%7D%28z_%7Bi%7D%29ln%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+_%7Bt%2B1%7D%29%7D%7BQ_%7Bit%7D%28z_%7Bi%7D%29%7D%5C%5C%5Cgeq+%5Csum+_%7Bi%7D%5Csum+_%7Bz_%7Bi%7D%7DQ_%7Bit%7D%28z_%7Bi%7D%29ln%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3B%5Ctheta+_%7Bt%7D%29%7D%7BQ_%7Bit%7D%28z_%7Bi%7D%29%7D%5C%5C%3DL%28%5Ctheta+_%7Bt%7D%29%5C%29+)

上式第一步利用了Jensen不等式，第二步成立是因为*θt+1*是函数的极值，因此会大于等于任意点处的函数值，第三步在上面已经做了说明，是Jensen不等式取等式。上面的结论保证了每次迭代时函数值会上升，直到到达局部极大值点处，但只能保证收敛到局部极值。

下图直观的解释了EM算法的原理
![](https://pic1.zhimg.com/v2-cbea7e1504e68ece30f17f2460c29800_b.png)EM算法示意图
图中的蓝色曲线为要求解的对数似然函数，黄色曲线为构造出的下界函数。首先用参数的当前估计值*θt*计算出每个训练样本的隐变量的概率分布估计值 ![Q_{t}](https://www.zhihu.com/equation?tex=Q_%7Bt%7D) ，然后用该值构造下界函数，在参数的当前估计值![\theta_{t}](https://www.zhihu.com/equation?tex=%5Ctheta_%7Bt%7D)处，下界函数与对数似然函数的值相等（对应图中左侧第一条虚线）。然后求下界函数的极大值，得到参数新的估计值![\theta_{t+1}](https://www.zhihu.com/equation?tex=%5Ctheta_%7Bt%2B1%7D)，再以当前的参数值 ![\theta_{t+1}](https://www.zhihu.com/equation?tex=%5Ctheta_%7Bt%2B1%7D) 计算隐变量的概率分 ![Q_{t+1}](https://www.zhihu.com/equation?tex=Q_%7Bt%2B1%7D) 布，构造出新的下界函数，然后求下界函数的极大值得到 ![\theta_{t+2}](https://www.zhihu.com/equation?tex=%5Ctheta_%7Bt%2B2%7D) 。如此反复，直到收敛。

算法的精髓在于：

构造下界函数（Jensen不等式成立），通过巧妙的取Q的值而保证在参数的当前迭代点处下界函数与要求解的目标函数值相等（Jensen不等式取等号），从而保证优化下界函数后在新的迭代点处目标函数值是上升的。 

求解高斯混合模型

下面介绍EM算法如何求解高斯混合模型。假设有一批样本{x1,...,![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D)}。为每个样本![x_{i}](https://www.zhihu.com/equation?tex=x_%7Bi%7D)增加一个隐变量 ![z_{i}](https://www.zhihu.com/equation?tex=z_%7Bi%7D) ，表示样本来自于哪个高斯分布。这是一个离散型的随机变量，取值范围为{**1**,..,**k**}，取每个值的概率为![w_{i}](https://www.zhihu.com/equation?tex=w_%7Bi%7D)。x和z的联合概率可以写成

![\(\begin{align*}p(x,z=j) &=p(z=j)p(x\mid z=j)\\&= w_{j}N(x;\mu _j,\Sigma_{j})\end{align*}\)](https://www.zhihu.com/equation?tex=%5C%28%5Cbegin%7Balign%2A%7Dp%28x%2Cz%3Dj%29+%26%3Dp%28z%3Dj%29p%28x%5Cmid+z%3Dj%29%5C%5C%26%3D+w_%7Bj%7DN%28x%3B%5Cmu+_j%2C%5CSigma_%7Bj%7D%29%5Cend%7Balign%2A%7D%5C%29)

这是样本的隐变量取值为j，并且样本向量值为x的概率。在E步构造Q函数

![\(\begin{align*}Q_{i}(z_{i}=j)=q_{ij} &=\frac{p(x_{i},z_{i}=j;\theta )}{_{z}^{\sum}p(x_{i},z;\theta )}\\&= \frac{w_{j}N(x;\mu_{j},\sum_{j})}{\Sigma_{t=1}^{k}w_{t}N(x;\mu _{t},\sum_{t})}\end{align*}\)](https://www.zhihu.com/equation?tex=%5C%28%5Cbegin%7Balign%2A%7DQ_%7Bi%7D%28z_%7Bi%7D%3Dj%29%3Dq_%7Bij%7D+%26%3D%5Cfrac%7Bp%28x_%7Bi%7D%2Cz_%7Bi%7D%3Dj%3B%5Ctheta+%29%7D%7B_%7Bz%7D%5E%7B%5Csum%7Dp%28x_%7Bi%7D%2Cz%3B%5Ctheta+%29%7D%5C%5C%26%3D+%5Cfrac%7Bw_%7Bj%7DN%28x%3B%5Cmu_%7Bj%7D%2C%5Csum_%7Bj%7D%29%7D%7B%5CSigma_%7Bt%3D1%7D%5E%7Bk%7Dw_%7Bt%7DN%28x%3B%5Cmu+_%7Bt%7D%2C%5Csum_%7Bt%7D%29%7D%5Cend%7Balign%2A%7D%5C%29)

这个值根据*μ,∑,w*的当前迭代值计算，是一个常数。得到z的分布即Q值之后，要求解的目标函数为
![](https://pic3.zhimg.com/v2-dcb334a66a02a81ffffd705a9e654f5a_b.png)

 在这里 ![q_{ij}](https://www.zhihu.com/equation?tex=q_%7Bij%7D) 已经是一个常数而不是*μ*和*∑*的函数。对 ![\mu_{j}](https://www.zhihu.com/equation?tex=%5Cmu_%7Bj%7D) 求梯度并令梯度为0，可以得到

![\(\bigtriangledown _{\mu _{j}}L(w,\mu ,\sum )\begin{align*}&= \bigtriangledown _{\mu _{j}}\sum_{i=1}^{l}\sum_{j=1}^{k}q_{ij}\left ( ln\frac{1}{(2\pi )^{\frac{n}{2}}\left | \sum_{j}^{} \right |^{\frac{1}{2}}q_{ij}}+lnw_{j}-\frac{1}{2}(x_{i}-\mu _{j})^{T}\sum_{j}^{-1}(x_{i}-\mu _{j}) \right )\\ &=-\sum_{i=1}^{l}q_{ij}\sum_{j}^{-1}(x_{i}-\mu _{j})=0\end{align*}\)](https://www.zhihu.com/equation?tex=%5C%28%5Cbigtriangledown+_%7B%5Cmu+_%7Bj%7D%7DL%28w%2C%5Cmu+%2C%5Csum+%29%5Cbegin%7Balign%2A%7D%26%3D+%5Cbigtriangledown+_%7B%5Cmu+_%7Bj%7D%7D%5Csum_%7Bi%3D1%7D%5E%7Bl%7D%5Csum_%7Bj%3D1%7D%5E%7Bk%7Dq_%7Bij%7D%5Cleft+%28+ln%5Cfrac%7B1%7D%7B%282%5Cpi+%29%5E%7B%5Cfrac%7Bn%7D%7B2%7D%7D%5Cleft+%7C+%5Csum_%7Bj%7D%5E%7B%7D+%5Cright+%7C%5E%7B%5Cfrac%7B1%7D%7B2%7D%7Dq_%7Bij%7D%7D%2Blnw_%7Bj%7D-%5Cfrac%7B1%7D%7B2%7D%28x_%7Bi%7D-%5Cmu+_%7Bj%7D%29%5E%7BT%7D%5Csum_%7Bj%7D%5E%7B-1%7D%28x_%7Bi%7D-%5Cmu+_%7Bj%7D%29+%5Cright+%29%5C%5C+%26%3D-%5Csum_%7Bi%3D1%7D%5E%7Bl%7Dq_%7Bij%7D%5Csum_%7Bj%7D%5E%7B-1%7D%28x_%7Bi%7D-%5Cmu+_%7Bj%7D%29%3D0%5Cend%7Balign%2A%7D%5C%29)

可以解得

![\(\mu _j=\frac{\sum_{i=1}^{l}q_{ij}x_{i}}{\sum_{i=1}^{l}q_{ij}}\)](https://www.zhihu.com/equation?tex=%5C%28%5Cmu+_j%3D%5Cfrac%7B%5Csum_%7Bi%3D1%7D%5E%7Bl%7Dq_%7Bij%7Dx_%7Bi%7D%7D%7B%5Csum_%7Bi%3D1%7D%5E%7Bl%7Dq_%7Bij%7D%7D%5C%29)

对![\Sigma_{j}](https://www.zhihu.com/equation?tex=%5CSigma_%7Bj%7D)求梯度并令梯度为0，根据正态分布最大似然估计的结论，可以解到

![\(\Sigma_j=\frac{\sum_{i=1}^{l}q_{ij}(x_i-\mu _j)^{T}(x_i-\mu _j)^{T}}{\sum_{i=1}^{l}q_{ij}}\) ](https://www.zhihu.com/equation?tex=%5C%28%5CSigma_j%3D%5Cfrac%7B%5Csum_%7Bi%3D1%7D%5E%7Bl%7Dq_%7Bij%7D%28x_i-%5Cmu+_j%29%5E%7BT%7D%28x_i-%5Cmu+_j%29%5E%7BT%7D%7D%7B%5Csum_%7Bi%3D1%7D%5E%7Bl%7Dq_%7Bij%7D%7D%5C%29+)

最后处理w。上面的目标函数中，只有In ![w_{j}](https://www.zhihu.com/equation?tex=w_%7Bj%7D) 和w有关，因此可以简化。由于 ![w_{i}](https://www.zhihu.com/equation?tex=w_%7Bi%7D) 有等式约束![\sum_{i=1}^{k}{w_{i}}=1](https://www.zhihu.com/equation?tex=%5Csum_%7Bi%3D1%7D%5E%7Bk%7D%7Bw_%7Bi%7D%7D%3D1) ，因此构造拉格朗日乘子函数

![\(L(w,\lambda )=\sum_{i=1}^{l}\sum_{j=1}^{k}q_{ij}lnw_{j}+\lambda \left ( \sum_{j=1}^{k}w_j-1 \right )\)](https://www.zhihu.com/equation?tex=%5C%28L%28w%2C%5Clambda+%29%3D%5Csum_%7Bi%3D1%7D%5E%7Bl%7D%5Csum_%7Bj%3D1%7D%5E%7Bk%7Dq_%7Bij%7Dlnw_%7Bj%7D%2B%5Clambda+%5Cleft+%28+%5Csum_%7Bj%3D1%7D%5E%7Bk%7Dw_j-1+%5Cright+%29%5C%29)

对w求梯度并令梯度为0，可以得到下面的方程组

![\(\sum_{i=1}^{l}\sum_{j=1}^{k}\frac{q_{ij}}{w_{j}}+\lambda =0\)](https://www.zhihu.com/equation?tex=%5C%28%5Csum_%7Bi%3D1%7D%5E%7Bl%7D%5Csum_%7Bj%3D1%7D%5E%7Bk%7D%5Cfrac%7Bq_%7Bij%7D%7D%7Bw_%7Bj%7D%7D%2B%5Clambda+%3D0%5C%29)

![\(\sum_{i=1}^{k}w_i=1\)](https://www.zhihu.com/equation?tex=%5C%28%5Csum_%7Bi%3D1%7D%5E%7Bk%7Dw_i%3D1%5C%29)

最后解得

![\(w_j=\frac{1}{l}\sum_{i=1}^{l}q_{ij}\)](https://www.zhihu.com/equation?tex=%5C%28w_j%3D%5Cfrac%7B1%7D%7Bl%7D%5Csum_%7Bi%3D1%7D%5E%7Bl%7Dq_%7Bij%7D%5C%29)

由此得到求解高斯混合模型的EM算法流程。首先初始化*μ,∑,w*，接下来循环进行迭代，直至收敛，每次迭代时的操作为：

E步，根据模型参数的当前估计值，计算第i个样本来自第j个高斯分布的概率：

![\(q_{ij}=p(z_i=j\mid x_i,w,\mu ,\sum )\)](https://www.zhihu.com/equation?tex=%5C%28q_%7Bij%7D%3Dp%28z_i%3Dj%5Cmid+x_i%2Cw%2C%5Cmu+%2C%5Csum+%29%5C%29)

M步，计算模型的参数。权重的计算公式为：

![\(w_j=\frac{1}{l}\sum_{i=1}^{l}q_{ij}\)](https://www.zhihu.com/equation?tex=%5C%28w_j%3D%5Cfrac%7B1%7D%7Bl%7D%5Csum_%7Bi%3D1%7D%5E%7Bl%7Dq_%7Bij%7D%5C%29)

均值的计算公式为：

![\(\mu _j=\frac{\sum_{i=1}^{l}q_{ij}x_{i}}{\sum_{i=1}^{l}q_{ij}}\)](https://www.zhihu.com/equation?tex=%5C%28%5Cmu+_j%3D%5Cfrac%7B%5Csum_%7Bi%3D1%7D%5E%7Bl%7Dq_%7Bij%7Dx_%7Bi%7D%7D%7B%5Csum_%7Bi%3D1%7D%5E%7Bl%7Dq_%7Bij%7D%7D%5C%29)

协方差的计算公式为：

![ \(\Sigma_{j}=\frac{\sum_{i=1}^{l}q_{ij}(x_{i}-\mu _{j})(x_{i}-\mu _{j})^{T}}{\sum_{i=1}^{l}q_{ij}}\)](https://www.zhihu.com/equation?tex=+%5C%28%5CSigma_%7Bj%7D%3D%5Cfrac%7B%5Csum_%7Bi%3D1%7D%5E%7Bl%7Dq_%7Bij%7D%28x_%7Bi%7D-%5Cmu+_%7Bj%7D%29%28x_%7Bi%7D-%5Cmu+_%7Bj%7D%29%5E%7BT%7D%7D%7B%5Csum_%7Bi%3D1%7D%5E%7Bl%7Dq_%7Bij%7D%7D%5C%29)

参考文献

[1] Arthur P Dempster, Nan M Laird, Donald B Rubin. Maximum Likelihood from Incomplete Data via the EM Algorithm. Journal of the royal statistical society series b-methodological, 1976。

