
# XX^{T} 和 X^{T}X 的关系 - 颹蕭蕭 - CSDN博客


2018年11月01日 21:51:27[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：2689标签：[特征相量																](https://so.csdn.net/so/search/s.do?q=特征相量&t=blog)[PCA																](https://so.csdn.net/so/search/s.do?q=PCA&t=blog)[协方差矩阵																](https://so.csdn.net/so/search/s.do?q=协方差矩阵&t=blog)[格拉姆矩阵																](https://so.csdn.net/so/search/s.do?q=格拉姆矩阵&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=协方差矩阵&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=PCA&t=blog)个人分类：[矩阵论																](https://blog.csdn.net/itnerd/article/category/8104256)[机器学习																](https://blog.csdn.net/itnerd/article/category/8068411)[
							](https://blog.csdn.net/itnerd/article/category/8104256)
[
				](https://so.csdn.net/so/search/s.do?q=PCA&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=PCA&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=特征相量&t=blog)
记$A = X^TX， B = XX^T$。假设$X \in \R^{m \times n}$，则$A \in \R^{n \times n}$，$B \in \R^{m \times m}$。易得
$$
dim(A) = dim(B) = dim(X)
$$
显然，A 和 B 都是对称半正定矩阵，且 A 有 n 个特征向量， B 有 m 个特征向量。
**特征向量之间的关系：**
若 u 是 B 的特征向量，即$Bu = \lambda u$，则$X^T u$是 A 的特征向量。
**证明：**
$$
A(X^T u) = X^TXX^Tu = X^TBu = X^T(\lambda u) = \lambda(X^T u)
$$
同理， 若 v 是 A 的特征向量，即$Av = \lambda v$，则$X v$是 B 的特征向量。

