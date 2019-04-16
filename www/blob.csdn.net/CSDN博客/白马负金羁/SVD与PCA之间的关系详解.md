# SVD与PCA之间的关系详解 - 白马负金羁 - CSDN博客





2018年03月01日 11:27:56[白马负金羁](https://me.csdn.net/baimafujinji)阅读数：4653
所属专栏：[图像处理中的数学原理详解](https://blog.csdn.net/column/details/math-imageprocess.html)









假设现在有一个数据矩阵$\bf{X}$，其大小是$n\times p$，其中 $n$ is the number of samples and $p$ is the number of variables （或features）。这里，$\bf{X}^T$可以写成$\{x_1,x_2,\cdots,x_n\}$，${x}_1$表示一个长度为$p$的列向量，也就是说，$\bf{X}^T$ 包含 $n$ independent observations $x_1,x_2,\cdots,x_n$，其中每个都是一个 p-dimensional 的列向量，这与【7】中的写法相一致。

现在，不失普遍性地，让我们假设$\bf{X}$ is centered, 即 column means have been subtracted and are now equal to zero。如果$\bf{X}$ is not centered，也不要紧，我们可以通过计算其与centering matrix $\bf{H}$ 之间的乘法来对其中心化。$\bf{H} = \bf{I}-ee^T/p$, 其中 $e$ is a 每个元素都是1的 column vector。

基于上述条件，可知$p\times p$大小的协方差矩阵 covariance matrix $\bf{C}$ 可由 $\bf{C}=\bf{X^TX}/(n-1)$ 给出。此处，我们稍微补充一下协方差矩阵与相关性矩阵（correlation matrix ）的一些内容。如果你对此已经非常了解，可以直接跳过这一部分。

如果 $X$ 和 $Y$ 是两个随机变量，with means (expected values) $μ_X$ and $μ_Y$ and standard deviations $σ_X$ and $σ_Y$, respectively, then their covariance is 


${\displaystyle \sigma _{XY}=E[(X-\mu _{X})\,(Y-\mu _{Y})]} $

and correlation is: 


$ {\displaystyle \rho _{XY}=E[(X-\mu _{X})\,(Y-\mu _{Y})]/(\sigma _{X}\sigma _{Y})} $

因此 $ {\displaystyle {\text{cov}}_{XY}=\sigma _{XY}=\rho _{XY}\sigma _{X}\sigma _{Y}} $。
如果 $\bf{X}$ are centred data matrices of dimension $n\times p$，an unbiased estimator of the covariance matrix (sample covariance matrix) 


$\bf{C}=\frac{1}{n-1}\bf{X}^T\bf{X}$

另一方面，如果 the column means were known a-priori, 则有 


$\bf{C}=\frac{1}{n}\bf{X}^T\bf{X}$

最后，在MATLAB中计算covariance matrix 和 correlation matrix的方法可以参见【5】。
现在我们知道，$\bf{C}=\bf{X^TX}/(n-1)$是一个对称矩阵，因此它可以对角化，即 


$\bf{C}=\bf{VΛV^T}$

其中，$\bf{V}$ is a matrix of eigenvectors (each column is an eigenvector) and $\bf{Λ}$ is a diagonal matrix with eigenvalues $λ_i$ in the decreasing order on the diagonal。
Any matrix has a singular value decomposition, so we have 


$\bf{X}=U\Sigma V^T$

应该注意到 


$\bf{X^TX}=(U\Sigma V^T)^T (U\Sigma V^T)=V\Sigma ^TU^TU\Sigma V^T=V(\Sigma ^T\Sigma)V^T$

这其实是特征值分解的结果，我们更进一步，把$\bf{C}$引入，则有 


$\bf{C}=\frac{1}{n-1}\bf{X}^T\bf{X}=\frac{1}{n-1}V(\Sigma ^T\Sigma)V^T=V\frac{\Sigma ^2}{n-1}V^T$

也就是说，Covariance matrix $\bf{C}$的特征值 $\lambda_i$ 与 矩阵$\bf{X}$的奇异值$\sigma_i$之间的关系是 $\sigma_i^2=(n-1)\lambda_i$。$\bf{X}$的右singular matrix $\bf{V}$ 中的列是与上述主成分相对应的主方向（principal directions）。最后， 


$\bf{XV}=\bf{U\Sigma V^T V}=\bf{U\Sigma}$

则表明，$\bf{U\Sigma}$就是主成分（PC）。
**参考文献**

【1】[https://stats.stackexchange.com/questions/134282/relationship-between-svd-and-pca-how-to-use-svd-to-perform-pca](https://stats.stackexchange.com/questions/134282/relationship-between-svd-and-pca-how-to-use-svd-to-perform-pca)

【2】[https://intoli.com/blog/pca-and-svd/](https://intoli.com/blog/pca-and-svd/)

【3】[https://en.wikipedia.org/wiki/Estimation_of_covariance_matrices](https://en.wikipedia.org/wiki/Estimation_of_covariance_matrices)

【4】[https://en.wikipedia.org/wiki/Covariance_and_correlation](https://en.wikipedia.org/wiki/Covariance_and_correlation)

【5】[http://www.mathworks.com/help/stats/corrcov.html?s_tid=gn_loc_drop](http://www.mathworks.com/help/stats/corrcov.html?s_tid=gn_loc_drop)

【6】[http://blog.csdn.net/baimafujinji/article/details/6474273](http://blog.csdn.net/baimafujinji/article/details/6474273)

【7】[http://blog.csdn.net/baimafujinji/article/details/79376378](http://blog.csdn.net/baimafujinji/article/details/79376378)







