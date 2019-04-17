# 稀疏编码（Sparse Coding） - curryche的博客 - CSDN博客





2018年06月08日 16:23:24[curryche](https://me.csdn.net/whwan11)阅读数：359








稀疏编码（Sparse Coding）的思想是通过码本（Codebook）$\{\boldsymbol{d}_i\}_{i=1}^{K},\boldsymbol{d}_i\in R^{d}$的线性组合来重构输入变量$\boldsymbol{x}\in R^{d}$，而每个码本的权重系数构成一个向量$\boldsymbol{y}\in R^{m}$。由于在重构时一般只会用到码本中少数的码字，因此向量$\boldsymbol{y}$通常是稀疏的。稀疏编码可以通过最小化一下目标函数得到： 


$\min\limits_{\boldsymbol{y}}\Bigg\lVert\boldsymbol{x}_t-\sum\limits_{i=1}^{K}\boldsymbol{d}_i[\boldsymbol{y}]_i \Bigg\rVert^2+\lambda\lVert \boldsymbol{y}\rVert_1$

其中$\lambda$是正则化系数，$[\boldsymbol{y}]_i$表示$\boldsymbol{y}$的第$i$个元素，即在重构中第$i$个码字的系数。



