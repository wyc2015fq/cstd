# kaldi-external matrix - xmdxcsj的专栏 - CSDN博客





2016年03月05日 11:09:53[xmucas](https://me.csdn.net/xmdxcsj)阅读数：756标签：[kaldi](https://so.csdn.net/so/search/s.do?q=kaldi&t=blog)
个人分类：[kaldi](https://blog.csdn.net/xmdxcsj/article/category/6116092)









一、计算库的介绍：

1.BLAS(basic linear algebra subroutines)

参考[https://zh.wikipedia.org/wiki/BLAS](https://zh.wikipedia.org/wiki/BLAS)

基本的矩阵向量基本运算的API标准，具体实现有：“reference BLAS”、“ATLAS”

2.LAPACK(linear algebra package)

参考[https://zh.wikipedia.org/wiki/LAPACK](https://zh.wikipedia.org/wiki/LAPACK)

提供矩阵和向量高级运算的函式集，比如SVD、矩阵求你等。LAPACK使用Fortran实现，依赖于BLAS，可以混合使用LAPACK和BLAS。

3.CLAPACK

[https://people.sc.fsu.edu/~jburkardt/c_src/clapack/clapack.html](https://people.sc.fsu.edu/~jburkardt/c_src/clapack/clapack.html)

使用f2c工具将LAPACK的Fortran转化为C，实际使用LAPACK的时候，就是指的CLAPACK，需要f2c库（编译选项增加-lf2c -lg2c）。

4.ATLAS(automatically tuned linear algebra software)

参考[http://math-atlas.sourceforge.net/](http://math-atlas.sourceforge.net/)

ATLAS包含BLAS的实现以及一部分LAPACK（比如矩阵求逆、 
Cholesky factorization，但是不包括SVD）。ATLAS需要对处理器有特殊的设置，所以编译比较困难。

5.MKL(math kernel library)

参考[](http://blog.sina.com.cn/s/blog_5941f5c40101h4um.html)[https://software.intel.com/en-us/intel-mkl](https://software.intel.com/en-us/intel-mkl)

提供扩展优化的数学函数库，包括BLAS、LAPACK、FFT、vector math、statistics function等。

6.OpenBLAS

参考[https://github.com/xianyi/OpenBLAS/wiki](https://github.com/xianyi/OpenBLAS/wiki)

实现了BLAS和部分LAPACK，继承于GoToBLAS

二、kaldi相关的封装

kaldi的matrix主要源自于对BLAS和LAPACK的源码封装，调用方式有四种：

1.HAVE_ATLAS

使用BLAS和部分LAPACK

2.HAVE_CLAPACK

BLAS和CLAPACK

3.HAVE_MKL

英特尔的MKL，提供BLAS和LAPACK

4.HAVE_OPENBLAS

OpenBlas，提供BLAS和LAPACK



