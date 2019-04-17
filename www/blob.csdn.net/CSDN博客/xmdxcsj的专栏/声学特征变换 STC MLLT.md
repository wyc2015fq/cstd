# 声学特征变换 STC/MLLT - xmdxcsj的专栏 - CSDN博客





2017年11月12日 16:30:15[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1435








## 背景

Global Semi-tied Covariance (STC)/Maximum Likelihood Linear Transform (MLLT) estimation 

gmm建模方差使用对角矩阵的前提是假设特征之间相互独立，使用full或者block-diagonal矩阵可以对相关性的特征建模，但是参数增多。为了解决使用这个问题，有两种方法： 

- feature-space 使用DCT或者LDA去相关 

- model-space 不同的模型可以使用不同的转换，更灵活
semi-tied covariance matrices是model-space里面的一种形式，也是为了解决使用full covariance的参数量大的问题。相比于full covariance，这种方法的每个高斯分量有两个方差矩阵： 

1. diagonal covariance$\sum_{diag}^{(m)}$

2. semi-tied class-dependent nondiagonal matrix $H^{(r)}$，可以在多个高斯分量之间共享（比如所有monophone对用状态的高斯分量）。
最后的方差矩阵:$\sum^{(m)}=H^{(r)}\sum_{diag}^{(m)}H^{(r)T}$,使用最大似然估计结合EM算法求解对应的参数。 

如果做变换$A^{(r)}=H^{(r)-1}$，把在分母上的semi-tied covariance转换到分子上面，也就是相当于特征$o(\tau)$和均值$\mu^{(m)}$同时乘以$A^{(r)}$，也就是对特征和gmm的均值同时做MLLT。

## kaldi实现

steps/train_lda_mllt.sh 

假设特征40维，得到的mllt转换矩阵是40*40，转换矩阵同时作用于gmm的均值和特征

## 参考

1.Semi-tied covariance matrices for hidden Markov models





