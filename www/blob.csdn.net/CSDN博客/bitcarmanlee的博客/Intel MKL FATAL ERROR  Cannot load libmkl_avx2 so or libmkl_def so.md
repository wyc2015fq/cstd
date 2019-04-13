
# Intel MKL FATAL ERROR: Cannot load libmkl_avx2.so or libmkl_def.so. - bitcarmanlee的博客 - CSDN博客


2018年10月16日 11:24:44[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：654


在pycharm中运行代码的时候，爆出来如下错误：
`Intel MKL FATAL ERROR: Cannot load libmkl_avx2.so or libmkl_def.so.`最开始看到intel的字样，心里突然一惊，尼玛难道是跟CPU有什么冲突。
赶紧查查这个 intel MKL是神马玩意。
根据[https://software.intel.com/zh-cn/mkl](https://software.intel.com/zh-cn/mkl)的描述：
The Fastest and Most-Used Math Library for Intel®-Based Systems1
Accelerate math processing routines, increase application performance, and reduce development time. This ready-to-use math library includes:
Linear Algebra | Fast Fourier Transforms (FFT) | Vector Statistics & Data Fitting | Vector Math & Miscellaneous Solvers
英特尔数学核心函数库是一套经过高度优化和广泛线程化的数学例程，专为需要极致性能的科学、工程及金融等领域的应用而设计。核心数学函数包括 BLAS、LAPACK、ScaLAPACK1、稀疏矩阵解算器、快速傅立叶转换、矢量数学及其它函数。
原来是一组用于科学计算的工具。
继续google,发现出现该问题似乎一般都是在运行scikit-learn或者其他需要运行cuda的类库比如Theano,keras之类的.
而且大部分人都提到anaconda没有自带intel MKL。正好本人用的就是anaconda环境。
OK，那么更新使用类库的的依赖包,尤其是mkl相关的依赖。
`conda upgrade scikit-learn`更新完毕即可。

