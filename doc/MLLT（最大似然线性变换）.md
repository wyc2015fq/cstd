# MLLT（最大似然线性变换）

2017年08月10日 14:26:29 [Shmily_Young](https://me.csdn.net/shmilyforyq) 阅读数：1950

 版权声明：本文为博主原创文章，未经博主允许不得转载。

https://blog.csdn.net/shmilyforyq/article/details/77050633

主要目的是：在最大似然（ML）准则下使用一个线性变换矩阵对参数特征矢量进行解相关。

在ML准则下，评价一个模型‘好坏’的标准是**训练数据与模型匹配的似然度**，如果似然度越高的话，我们说这个模型越好。MLLT的作者给出了在最大似然准则下（ML）使用**对角协方差矩阵**的缺点，及其对训练数据集描述似然度的损失。

在原特征空间，建立模型，匹配训练数据，得到似然度$P$。考虑在特征空间做一个线性变换，$y_i = Ax_i$，然后在新的特征空间进行建模、匹配，同样得到一个新的似然度$P_y$。由于似然度分别在两个不同空间计算，所以不能直接相比，解决的办法有两个，一个是限制$|A|=1$，另一个办法就是将似然度变换回原空间的尺度：$P(X_1^N,\{\mu_i\}_x,\{\Sigma_i\}_x)=P_y(y_1^N,\{\mu_i\}_y\{\Sigma_i\}_y)*\prod_{i=1}^	M|A|^{N_i}$。这里，采用第一个限制来叙述，即采取限制$|A|=1$。

为简单起见，采取单高斯模型来分析，在原特征空间，单高斯模型对训练数据的似然度为 
$$
P=a(N,d)exp(\frac{-1}{2}N[(\overline\mu-\mu)^T\Sigma^{-1}(\overline\mu-\mu)+Tr(\Sigma^{-1}\overline\Sigma)+log|\Sigma|]) \tag{1}
$$
这里，$a(N,d)=(2\pi)^{\frac{-Nd}{2}}$。在ML准则下，估计出来的模型参数$μ$和$Σ$的估计值$\widehat{\mu}$和$\widehat{\Sigma}$分别等于训练数据的样本均值$\overline{\mu}$和样本协方差$\overline{\Sigma}​$,代入等式(1)中得到 
$$
P^*(x_1^N)=g(n,d)|\overline\Sigma|^{-\frac{N}{2}} \tag{2}
$$
其中$g(N,d)=(2\pi e)^\frac{-Nd}{2}$是个常数。从公式（1）我们可以看到，在ML准则下，模型与训练集的匹配似然度大小仅仅取决于样本协方差$\overline\Sigma​$。



当对特征矢量做线性变换$y_i=Ax_i$，可以求出$\overline{\mu}_y=A\overline{\mu}$和$\overline{\Sigma}_y=A\overline{\Sigma}A^T$。可以计算出其似然值 
$$
P^*(x_1^N)=g(n,d)|A\overline\Sigma A^T|^{-\frac{N}{2}}=|A|^{-N}P^*(x_1^N) \tag{3}
$$
由于采用了限制$|A|=1$，所以，做了线性变换之后，似然度并没有变化，从ML的角度来说，模型并没有被优化。 
但是在实际应用中的高斯模型是受限的，即样本协方差矩阵被对角化了。也就是说ML的模型参数μμ和ΣΣ的估计值为$\widehat{\mu}=\overline{\mu}$和$\widehat{\Sigma}=diag(\overline{\Sigma})$。那么，式（3）的ML值就变成 
$$
P^*_{diag}(x_1^N)=g(n,d)|diag(\overline{\Sigma})|^{-\frac{N}{2}}\tag{4}
$$
由于有Hadamard不等式，对于对称的非负定的矩阵有$|diag(\overline{\Sigma})|\geq|\overline{\Sigma}|$

,所以有
$$
P^*_{diag}(x_1^N)=g(n,d)|diag(\overline{\Sigma})|^{-\frac{N}{2}}\tag{4}
$$
也就是似然度变小了，模型的精度下降了。



而作了线性变换之后，似然度为$P^*_{diag}(y_1^N)=g(n,d)|diag(\overline{\Sigma})|^{-\frac{N}{2}}$，可见，与式子（4）不同了，如果变换阵A能够使得样本协方差矩阵Σ¯¯¯¯Σ¯尽可能对角化，减少取对角的损失，就可以使得$P^*(x_1^N) \geq P^*_{diag}(x_1^N)$。比如，A为样本协方差矩阵$\overline{\Sigma}$的PCA变换阵，那么由于$A\overline{\Sigma}A^T=\Lambda$，$Λ$是由$\overline{\Sigma}$的特征值组成的对角阵，而且$|\Lambda|=|\overline{\Sigma}|$，所以此时， 
$$
P^*_{diag}(y_1^N)=P^*(x_1^N) \geq P^*_{diag}(x_1^N)\tag{5}
$$
从而使得新空间中，模型与训练集的似然度增加。