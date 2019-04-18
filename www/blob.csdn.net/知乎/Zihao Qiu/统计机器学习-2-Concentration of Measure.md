# 统计机器学习-2-Concentration of Measure - 知乎
# 

（这篇文章没有具体的证明，我主要记述了大致的思想脉络，证明细节见参考资料）

concentration of measure可以翻译成“度量集中”，这里姑且使用英文。这个名词描述的是一种统计学中的重要现象，那就是一些随机变量会聚集出现在这些随机变量的期望附近。大数定律就是这一现象的粗略描述，Hoeffding不等式为这种聚集程度定了界，这个不等式在后面的许多证明中有着重要的地位，其内容为：如果 ![Z_1,\cdots,Z_n](https://www.zhihu.com/equation?tex=Z_1%2C%5Ccdots%2CZ_n) 相互独立，且 ![P(a_i\leq Z_i \leq b_i)=1](https://www.zhihu.com/equation?tex=P%28a_i%5Cleq+Z_i+%5Cleq+b_i%29%3D1) ，那么对任意 ![t>0](https://www.zhihu.com/equation?tex=t%3E0) ，有：

![P(|\bar{Z_n}-\mu|>\epsilon)\leq 2e^{-\frac{2n\epsilon^2}{c}}](https://www.zhihu.com/equation?tex=P%28%7C%5Cbar%7BZ_n%7D-%5Cmu%7C%3E%5Cepsilon%29%5Cleq+2e%5E%7B-%5Cfrac%7B2n%5Cepsilon%5E2%7D%7Bc%7D%7D)

其中 ![\mu](https://www.zhihu.com/equation?tex=%5Cmu) 是 ![Z](https://www.zhihu.com/equation?tex=Z) 的期望， ![c=\frac{1}{n}\sum_{i=1}^n(b_i-a_i)^2](https://www.zhihu.com/equation?tex=c%3D%5Cfrac%7B1%7D%7Bn%7D%5Csum_%7Bi%3D1%7D%5En%28b_i-a_i%29%5E2) ， ![\bar{Z_n}=\frac{1}{n}\sum_{i=1}^n Z_i](https://www.zhihu.com/equation?tex=%5Cbar%7BZ_n%7D%3D%5Cfrac%7B1%7D%7Bn%7D%5Csum_%7Bi%3D1%7D%5En+Z_i)

具体到一个分类问题上，我们有数据 ![(X_1,Y_1),\cdots,(X_n,Y_n)](https://www.zhihu.com/equation?tex=%28X_1%2CY_1%29%2C%5Ccdots%2C%28X_n%2CY_n%29) 并且已有分类器 ![h](https://www.zhihu.com/equation?tex=h) ，那么设训练误差为 ![\hat{R}(h)=\frac{1}{n}\sum \mathbb{I}(Y_i\ne h(X_i))](https://www.zhihu.com/equation?tex=%5Chat%7BR%7D%28h%29%3D%5Cfrac%7B1%7D%7Bn%7D%5Csum+%5Cmathbb%7BI%7D%28Y_i%5Cne+h%28X_i%29%29) ，泛化误差为 ![R(h)=P(Y\ne h(X))](https://www.zhihu.com/equation?tex=R%28h%29%3DP%28Y%5Cne+h%28X%29%29) ，则有Hoeffding不等式，可以知道训练误差和泛化误差应该是以大概率相近的。

然而我们一般遇到的问题是要求算法从函数空间 ![\mathcal{H}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BH%7D) 中选择一个分类器 ![h](https://www.zhihu.com/equation?tex=h) 而非给定一个 ![h](https://www.zhihu.com/equation?tex=h) ，要想说明这个算法足够好，就要说明训练误差 ![\hat{R}(h)](https://www.zhihu.com/equation?tex=%5Chat%7BR%7D%28h%29) 与泛化误差 ![R(h)](https://www.zhihu.com/equation?tex=R%28h%29) 应该处处都很接近（即uniform convergence），即 ![P(\sup_{h\in\mathcal{H}} |\hat{R}(h)-R(h)|>\epsilon)\leq small](https://www.zhihu.com/equation?tex=P%28%5Csup_%7Bh%5Cin%5Cmathcal%7BH%7D%7D+%7C%5Chat%7BR%7D%28h%29-R%28h%29%7C%3E%5Cepsilon%29%5Cleq+small) ，从后面的推理中可以看到， ![small](https://www.zhihu.com/equation?tex=small) 可以表示成 ![\boxed{\cdots}e^{-n\epsilon^2c}](https://www.zhihu.com/equation?tex=%5Cboxed%7B%5Ccdots%7De%5E%7B-n%5Cepsilon%5E2c%7D) ，后面是Hoeffding不等式中的项，而 ![\boxed{\cdots}](https://www.zhihu.com/equation?tex=%5Cboxed%7B%5Ccdots%7D) 是与函数空间复杂度有关的项，一般可以使用VC维或Rademacher复杂度来度量。

小小总结一下，我们现在有一类函数 ![\mathcal{F}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BF%7D) ， ![f\in\mathcal{F}](https://www.zhihu.com/equation?tex=f%5Cin%5Cmathcal%7BF%7D) ，经验误差为 ![\frac{1}{n}\sum f(X_i)](https://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7Bn%7D%5Csum+f%28X_i%29) ，泛化误差为 ![Ef(x)=\int f(x)dP(x)=\mu_{f}](https://www.zhihu.com/equation?tex=Ef%28x%29%3D%5Cint+f%28x%29dP%28x%29%3D%5Cmu_%7Bf%7D) ，现在的目标是bound住 ![P(\sup_{f\in\mathcal{F}} |\frac{1}{n}\sum f(X_i)-\mu_{f}|>\epsilon)](https://www.zhihu.com/equation?tex=P%28%5Csup_%7Bf%5Cin%5Cmathcal%7BF%7D%7D+%7C%5Cfrac%7B1%7D%7Bn%7D%5Csum+f%28X_i%29-%5Cmu_%7Bf%7D%7C%3E%5Cepsilon%29) ，显然这个式子的大小取决于 ![f](https://www.zhihu.com/equation?tex=f) 会有多复杂。

在介绍VC维之前先介绍shattering number。假设 ![\mathcal{F}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BF%7D) 是一组binary functions， ![f(z)\in\{0,1\}](https://www.zhihu.com/equation?tex=f%28z%29%5Cin%5C%7B0%2C1%5C%7D) ，可定义所有可能标签的集合为： ![\mathcal{F}_{z1,\cdots,z_n}=\{(f(z_1),\cdots,f(z_n)):f\in\mathcal{F}\}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BF%7D_%7Bz1%2C%5Ccdots%2Cz_n%7D%3D%5C%7B%28f%28z_1%29%2C%5Ccdots%2Cf%28z_n%29%29%3Af%5Cin%5Cmathcal%7BF%7D%5C%7D) 。于是shattering number ![s(\mathcal{F},n)=\sup_{z_1,\cdots,z_n}|\mathcal{F}_{z_1,\cdots,z_n}|](https://www.zhihu.com/equation?tex=s%28%5Cmathcal%7BF%7D%2Cn%29%3D%5Csup_%7Bz_1%2C%5Ccdots%2Cz_n%7D%7C%5Cmathcal%7BF%7D_%7Bz_1%2C%5Ccdots%2Cz_n%7D%7C) ，即最多可能的贴标签的方式。

若某函数空间 ![\mathcal{H}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BH%7D) 的VC维为n，那么就是说对 ![h\in\mathcal{H}](https://www.zhihu.com/equation?tex=h%5Cin%5Cmathcal%7BH%7D) ， ![h](https://www.zhihu.com/equation?tex=h) 有能力为n个样本贴上 ![2^n](https://www.zhihu.com/equation?tex=2%5En) 种不同的标签（对二分类问题），但不能为n+1个样本贴上 ![2^{n+1}](https://www.zhihu.com/equation?tex=2%5E%7Bn%2B1%7D) 种不同的标签。或者表示为 ![\sup\{n:s(\mathcal{F},n)=2^n\}](https://www.zhihu.com/equation?tex=%5Csup%5C%7Bn%3As%28%5Cmathcal%7BF%7D%2Cn%29%3D2%5En%5C%7D) 。这就反映了该函数空间的复杂度，并且从本质上，它给出了一种如何度量势为无穷的函数空间的一种方法——从是否能鉴别每种情况的角度。

可以看出VC维是与数据无关的，而Rademacher复杂度考虑了数据，因此比VC维更紧，其定义如下：加上有随机变量 ![\sigma_1,\cdots,\sigma_n](https://www.zhihu.com/equation?tex=%5Csigma_1%2C%5Ccdots%2C%5Csigma_n) 服从独立同分布（iid），且 ![P(\sigma_i = 1)=P(\sigma_i=-1)=\frac{1}{2}](https://www.zhihu.com/equation?tex=P%28%5Csigma_i+%3D+1%29%3DP%28%5Csigma_i%3D-1%29%3D%5Cfrac%7B1%7D%7B2%7D) ，那么称其为Rademacher变量；Rademacher复杂度定义为： ![Rad_n(\mathcal{F})=E(\sup_{f\in \mathcal{F}} |\frac{1}{n}\sum_{i=1}^n\sigma_if(z_i)|)](https://www.zhihu.com/equation?tex=Rad_n%28%5Cmathcal%7BF%7D%29%3DE%28%5Csup_%7Bf%5Cin+%5Cmathcal%7BF%7D%7D+%7C%5Cfrac%7B1%7D%7Bn%7D%5Csum_%7Bi%3D1%7D%5En%5Csigma_if%28z_i%29%7C%29) ， ![z_i](https://www.zhihu.com/equation?tex=z_i) 是第i个样本，假设 ![f(z_i)\in \{-1,1\}](https://www.zhihu.com/equation?tex=f%28z_i%29%5Cin+%5C%7B-1%2C1%5C%7D) 。由此可见，加入 ![f](https://www.zhihu.com/equation?tex=f) 很复杂，则可以始终选取 ![\sigma_i](https://www.zhihu.com/equation?tex=%5Csigma_i) 与 ![f(z_i)](https://www.zhihu.com/equation?tex=f%28z_i%29) 同号，于是 ![Rad_n(\mathcal{F})](https://www.zhihu.com/equation?tex=Rad_n%28%5Cmathcal%7BF%7D%29) 便会很大。

Rademacher复杂度有些很好的性质，比如 ![\mathcal{F}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BF%7D) 的凸包的Rademacher复杂度与 ![\mathcal{F}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BF%7D) 相同等，具体可参见参考资料的Lemma 14。

Rademacher复杂度通常难以计算，常利用VC维来bound。有如下关系式 ![Rad_n(\mathcal{F})\leq\sqrt{\frac{2\log s(\mathcal{F},n)}{n}}](https://www.zhihu.com/equation?tex=Rad_n%28%5Cmathcal%7BF%7D%29%5Cleq%5Csqrt%7B%5Cfrac%7B2%5Clog+s%28%5Cmathcal%7BF%7D%2Cn%29%7D%7Bn%7D%7D) ， ![s(\mathcal{F},n)](https://www.zhihu.com/equation?tex=s%28%5Cmathcal%7BF%7D%2Cn%29) 为VC维中的shuttering number，n为样本个数。证明可以参加参考资料的Theorem 17，其中关键在于使用sub-Gaussian随机变量的一个性质。若VC维有限，利用Sauer's theorem，上式可以写成 ![Rad_n(\mathcal{F})\leq\sqrt{\frac{2\log s(\mathcal{F},n)}{n}}\leq \sqrt{\frac{2d\log n}{n}}](https://www.zhihu.com/equation?tex=Rad_n%28%5Cmathcal%7BF%7D%29%5Cleq%5Csqrt%7B%5Cfrac%7B2%5Clog+s%28%5Cmathcal%7BF%7D%2Cn%29%7D%7Bn%7D%7D%5Cleq+%5Csqrt%7B%5Cfrac%7B2d%5Clog+n%7D%7Bn%7D%7D) 。

最后，我们回到开始的问题——如何bound ![P(\sup_{f\in\mathcal{F}} |\frac{1}{n}\sum f(X_i)-\mu_{f}|>\epsilon)](https://www.zhihu.com/equation?tex=P%28%5Csup_%7Bf%5Cin%5Cmathcal%7BF%7D%7D+%7C%5Cfrac%7B1%7D%7Bn%7D%5Csum+f%28X_i%29-%5Cmu_%7Bf%7D%7C%3E%5Cepsilon%29) 。称为uniform bounds问题。

首先是大名鼎鼎的Vapnik and Chervonenkis Theorem：

![P(\sup_{f\in\mathcal{F}} |\frac{1}{n}\sum f(X_i)-\mu_{f}|>\epsilon)\leq 4s(\mathcal{F},2n)e^{-n\epsilon^2/8}](https://www.zhihu.com/equation?tex=P%28%5Csup_%7Bf%5Cin%5Cmathcal%7BF%7D%7D+%7C%5Cfrac%7B1%7D%7Bn%7D%5Csum+f%28X_i%29-%5Cmu_%7Bf%7D%7C%3E%5Cepsilon%29%5Cleq+4s%28%5Cmathcal%7BF%7D%2C2n%29e%5E%7B-n%5Cepsilon%5E2%2F8%7D)

这个定理的证明很巧妙，参见参考资料Theorem 21。这里的一个关键的问题是如何计算泛化误差 ![\mu_f](https://www.zhihu.com/equation?tex=%5Cmu_f) ，一个天才的想法就是使用Systematization，其大致思想是，若 ![\bar{X}](https://www.zhihu.com/equation?tex=%5Cbar%7BX%7D) 与 ![\mu](https://www.zhihu.com/equation?tex=%5Cmu) 很接近，那么 ![\bar{X}](https://www.zhihu.com/equation?tex=%5Cbar%7BX%7D) 与 ![\bar{X^{'}}](https://www.zhihu.com/equation?tex=%5Cbar%7BX%5E%7B%27%7D%7D) 也不会相差太多。

还可以使用Rademacher复杂度的表示来获得uniform bounds（参考资料Theorem 23）：

下式成立的概率大于 ![1-\delta](https://www.zhihu.com/equation?tex=1-%5Cdelta) : ![\sup_{f\in\mathcal{F}}|\frac{1}{n}\sum f(X_i)-\mu_{f}|\leq 2Rad_n(\mathcal{F})+\sqrt{\frac{\log\frac{2}{\delta}}{2n}}](https://www.zhihu.com/equation?tex=%5Csup_%7Bf%5Cin%5Cmathcal%7BF%7D%7D%7C%5Cfrac%7B1%7D%7Bn%7D%5Csum+f%28X_i%29-%5Cmu_%7Bf%7D%7C%5Cleq+2Rad_n%28%5Cmathcal%7BF%7D%29%2B%5Csqrt%7B%5Cfrac%7B%5Clog%5Cfrac%7B2%7D%7B%5Cdelta%7D%7D%7B2n%7D%7D)

这个定理的证明用到了McDiarmid不等式，这相当于Hoeffding不等式的泛化，其大概意思为，若一组随机变量中一个发生改变对总体的影响有限，那么这些随机变量的均值应该很接近其期望。

最后总结一下，VC维和Rademacher复杂度都可将关于sup的概率论述简化成一个可以被bound的项，这对于估计算法的表现很有价值，因为这相当于将无穷的函数空间用一个有穷的项定界。在后面对各种算法的分析中，用到最多的还是Hoeffding不等式和VC维。


参考资料：
[http://www.stat.cmu.edu/~larry/=sml/Concentration-of-Measure.pdf​www.stat.cmu.edu](https://link.zhihu.com/?target=http%3A//www.stat.cmu.edu/~larry/%3Dsml/Concentration-of-Measure.pdf)

