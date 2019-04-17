# Cholesky decomposition for Matrix Inversion - 家家的专栏 - CSDN博客





2013年11月19日 20:09:07[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1125
个人分类：[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)









# Cholesky decomposition for Matrix Inversion

Matrix inversion is a classical problem, and can be very complicated for large matrices. There are many ways to simplify this for special types of matrices. Among
 them, one is to transform the matrix into a set of upper or lower triangular matrices. Consider our target matrix ![\mathbf{A}](http://s0.wp.com/latex.php?zoom=1.5&latex=%5Cmathbf%7BA%7D&bg=ffffff&fg=444444&s=0) which
 is [Hermitian](http://en.wikipedia.org/wiki/Hermitian_matrix) and [positive-definite](http://en.wikipedia.org/wiki/Positive-definite_matrix).
 Such matrices are quite famous and an example is the covariance matrix in statistics. It’s inverse is seen in the Gaussian probability density function for vectors. Then, [Cholesky
 decomposition](http://en.wikipedia.org/wiki/Cholesky_decomposition) breaks





![\mathbf{A} = \mathbf{L}\mathbf{L}^T = \mathbf{U}^T\mathbf{U}](http://s0.wp.com/latex.php?zoom=1.5&latex=%5Cmathbf%7BA%7D+%3D+%5Cmathbf%7BL%7D%5Cmathbf%7BL%7D%5ET+%3D+%5Cmathbf%7BU%7D%5ET%5Cmathbf%7BU%7D&bg=ffffff&fg=444444&s=0)


where ![\mathbf{L}](http://s0.wp.com/latex.php?zoom=1.5&latex=%5Cmathbf%7BL%7D&bg=ffffff&fg=444444&s=0) is
 a lower triangular matrix, while ![\mathbf{U}](http://s0.wp.com/latex.php?zoom=1.5&latex=%5Cmathbf%7BU%7D&bg=ffffff&fg=444444&s=0) is
 an upper triangular matrix.


It is much easier to compute the [inverse
 of a triangular matrix](http://www.mcs.csueastbay.edu/~malek/TeX/Triangle.pdf) and there exist numerical solutions. Then the original matrix inverse is computed simply by multiplying the two inverses as

![\mathbf{A}^{-1} = (\mathbf{L}^{-1})^T(\mathbf{L}^{-1}) = (\mathbf{U}^{-1})(\mathbf{U}^{-1})^T](http://s0.wp.com/latex.php?zoom=1.5&latex=%5Cmathbf%7BA%7D%5E%7B-1%7D+%3D+%28%5Cmathbf%7BL%7D%5E%7B-1%7D%29%5ET%28%5Cmathbf%7BL%7D%5E%7B-1%7D%29+%3D+%28%5Cmathbf%7BU%7D%5E%7B-1%7D%29%28%5Cmathbf%7BU%7D%5E%7B-1%7D%29%5ET+&bg=ffffff&fg=444444&s=0)


As bonus, the determinant is also much easier to compute.

![det(\mathbf{A}) = det(\mathbf{L})^2](http://s0.wp.com/latex.php?zoom=1.5&latex=det%28%5Cmathbf%7BA%7D%29+%3D+det%28%5Cmathbf%7BL%7D%29%5E2+&bg=ffffff&fg=444444&s=0)


One can also use complex matrices, and just use a conjugate-transpose instead of transpose alone.





转自：[http://mobiusfunction.wordpress.com/2010/08/07/the-inverse-of-a-triangular-matrix/](http://mobiusfunction.wordpress.com/2010/08/07/the-inverse-of-a-triangular-matrix/)





