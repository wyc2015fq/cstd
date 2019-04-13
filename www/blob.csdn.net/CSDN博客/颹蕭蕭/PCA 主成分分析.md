
# PCA 主成分分析 - 颹蕭蕭 - CSDN博客


2018年11月01日 22:27:28[颹蕭蕭](https://me.csdn.net/itnerd)阅读数：2256标签：[PCA																](https://so.csdn.net/so/search/s.do?q=PCA&t=blog)[主成分分析																](https://so.csdn.net/so/search/s.do?q=主成分分析&t=blog)[特征分解																](https://so.csdn.net/so/search/s.do?q=特征分解&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=主成分分析&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=PCA&t=blog)个人分类：[机器学习																](https://blog.csdn.net/itnerd/article/category/8068411)[矩阵论																](https://blog.csdn.net/itnerd/article/category/8104256)[
							](https://blog.csdn.net/itnerd/article/category/8068411)
[
				](https://so.csdn.net/so/search/s.do?q=PCA&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=PCA&t=blog)

#### 问题定义：
若原始数据是 d 维的，我们希望找到一个压缩映射$W \in \R^{n \times d}$和 一个重建映射$U \in \R^{d \times n}$，使得压缩数据在解压后与原数据的误差最小：
$$
(1)
$$
$$
\arg \min_{U,W} \sum_{i=1}^m||x_i - UWx_i||\tag{1}
$$
其中，m 为数据量。
#### 引理
若 (U,W) 为上述问题的最优解，则 U 的列是正交归一的（$U^TU = I_n$），且$W = U^T$。
根据上述引理，原问题变成
$$
(2)
$$
$$
\arg \min_{U \in \R^{d \times n}; U^TU = I_n} \sum_{i=1}^m||x_i - UU^Tx_i||\tag{2}
$$

#### 变形
$$
\sum_{i=1}^m||x_i - UU^Tx_i|| 
\\=\sum_{i=1}^m(x_i - UU^Tx_i)^T(x_i - UU^Tx_i)
\\= \sum_{i=1}^m(x_i^Tx_i -2x_i^TUU^Tx_i +x_i^TUU^TUU^Tx_i)
\\= \sum_{i=1}^m(x_i^Tx_i -x_i^TUU^Tx_i )
\\= \sum_{i=1}^mx_i^Tx_i - \sum_{i=1}^mtrace\{x_i^TUU^Tx_i \}
\\= \sum_{i=1}^mx_i^Tx_i - \sum_{i=1}^mtrace\{UU^Tx_ix_i^T \}
\\= \sum_{i=1}^mx_i^Tx_i - trace\{UU^T\sum_{i=1}^mx_ix_i^T \}
\\（记 X = \sum_{i=1}^mx_ix_i^T）
\\=\sum_{i=1}^mx_i^Tx_i - trace\{U^TXU\}
$$
所以，问题 (2) 等价于
$$
\arg \max_{U \in \R^{d \times n}; U^TU = I_n} trace\{U^TXU\}
$$
等价于
$$
\arg \max_{u_i \perp u_j; ||u_i||=1}\sum_{i=1}^n u_i^TXu_i
$$
显然，该问题的最优解为 X 的前 n 大的特征值对应的特征向量。此时，
$$
\max_{u_i \perp u_j; ||u_i||=1}\sum_{i=1}^n u_i^TXu_i \\ =\sum_{i=1}^n \lambda_{[i]}
$$
其中，$\lambda_{[i]}$表示*第 i 大的特征值*。
根据$XX^T 和 X^TX$[ 的特征向量之间的关系](https://blog.csdn.net/itnerd/article/details/83627685)，可以做一些优化：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181101222846971.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l0bmVyZA==,size_16,color_FFFFFF,t_70)

