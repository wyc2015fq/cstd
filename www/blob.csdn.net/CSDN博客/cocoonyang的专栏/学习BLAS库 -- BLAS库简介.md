
# 学习BLAS库 -- BLAS库简介 - cocoonyang的专栏 - CSDN博客


2017年03月18日 01:51:16[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：8546个人分类：[BLAS																](https://blog.csdn.net/cocoonyang/article/category/5715477)



BLAS(Basic Linear Algebra Subprograms)是一组线性代数计算中通用的基本运算操作函数集合[1] 。BLAS Technical (BLAST) Forum负责规范BLAS函数接口, 并在网站[1]公布一个由Fortran语言编写的BLAS库。这个Fortran版BLAS库通常被称为BLAS参考库(the reference implementation)。 BLAS参考库使用的算法能高效地给出正确的结果，但仍有许多优化潜力。要想获得更高的计算效率，可以使用优化的BLAS库。

## BLAS 函数
BLAS库中函数根据运算对象的不同，分为三个类：
Level 1 函数处理单一向量的线性运算以及两个向量的二元运算。Level 1 函数最初出现在1979年公布的BLAS库中。
Level 2 函数处理 矩阵与向量的运算，同时也包含线性方程求解计算。 Level 2 函数公布于1988年。
Level 3 函数包含矩阵与矩阵运算。Level 3 函数发表于1990年。
```python
Level 1
PROTG
 - Description: generate plane rotation
 - Syntax: PROTG( A, B, C, S)
    - P: S(single float), D(double float)
PROTMG
 - Description: generate modified plane rotation
 - Syntax: PROTMG( D1, D2, A, B, PARAM)
    - P: S(single float), D(double float)
PROT
 - Description: apply plane rotation
 - Syntax: PROT( N, X, INCX, Y, INCY, C, S)
    - P: S(single float), D(double float)
PROTM
 - Description: apply modified plane rotation
 - Syntax: PROTM( N, X, INCX, Y, INCY, PARAM)
    - P: S(single float), D(double float)
PSWAP
 - Description: swap x and y
 - Syntax: PSWAP( N, X, INCX, Y, INCY)
    - P: S(single float), D(double float), C(complex), Z(complex*16)
PSCAL
 - Description: x = a*x
 - Syntax: PSCAL( N, ALPHA, X, INCX)
    - P: S(single float), D(double float), C(complex), Z(complex*16), CS, ZD

PCOPY
 - Description: copy x into y
 - Syntax: PCOPY( N, X, INCX, Y, INCY)
    - P: S(single float), D(double float), C(complex), Z(complex*16)
PAXPY
 - Description: copy x into y
 - Syntax: PAXPY( N, ALPHA, X, INCX, Y, INCY)
    - P: S(single float), D(double float), C(complex), Z(complex*16)
PDOT
 - Description: dot product
 - Syntax: PDOT( N, X, INCX, Y, INCY)
    - P: S(single float), D(double float), DS
PNRM2
 - Description: Euclidean norm
 - Syntax: PNRM2( N, X, INCX)
    - P:  S(single float), D(double float), CS, ZD
PASUM
 - Description: sum of absolute values
 - Syntax: PASUM( N, X, INCX)
    - P:  S(single float), D(double float), CS, ZD
IXAMAX
 - Description: index of max absolute value
 - Syntax: IXAMAX( N, X, INCX)

Level 2
PGEMV
 - Description: matrix vector multiply
 - Syntax: PGEMV( TRANS, M, N, ALPHA, A, LDA, X, INCX, BETA, Y, INCY)
    - P:  S(single float), D(double float), C(complex), Z(complex*16)
PGBMV
 - Description: banded matrix vector multiply
 - Syntax: PGEMV( TRANS, M, N, KL, KU, ALPHA, A, LDA, X, INCX, BETA, Y, INCY)
    - P:  S(single float), D(double float), C(complex), Z(complex*16)
PSYMV
 - Description: symmetric matrix vector multiply
 - Syntax: PGEMV( TRANS, N, ALPHA, A, LDA, X, INCX, BETA, Y, INCY)
    - P:  S(single float), D(double float),
PSBMV
 - Description: symmetric banded matrix vector multiply
 - Syntax: PGEMV( TRANS, N, K, ALPHA, A, LDA, X, INCX, BETA, Y, INCY)
    - P:  S(single float), D(double float),
PSPMV -
 - Description: symmetric packed matrix vector multiply
 - Syntax: PGEMV( TRANS, N, ALPHA, AP, X, INCX, BETA, Y, INCY)
    - P:  S(single float), D(double float),
PTRMV
 - Description: triangular matrix vector multiply
 - Syntax: PTRMV( UPLO, TRANS, DIAG, A, LDA, X, INCX)
    - P:  S(single float), D(double float), C(complex), Z(complex*16)
PTBMV -
 - Description: triangular banded matrix vector multiply
 - Syntax: PTRSV( UPLO, TRANS, DIAG, N, K, A, LDA, X, INCX)
    - P:  S(single float), D(double float), C(complex), Z(complex*16)
PTPMV
 - Description: triangular packed matrix vector multiply
 - Syntax: PTPMV( UPLO, TRANS, DIAG, N, AP, X, INCX)
    - P:  S(single float), D(double float), C(complex), Z(complex*16)
PTRSV
 - Description: solving triangular matrix problems
 - Syntax: PTRSV( UPLO, TRANS, DIAG, N, A, LDA, X, INCX)
    - P:  S(single float), D(double float), C(complex), Z(complex*16)
PTBSV
 - Description: solving triangular banded matrix problems
 - Syntax: PTBSV( UPLO, TRANS, DIAG, N, K, A, LDA, X, INCX)
    - P:  S(single float), D(double float), C(complex), Z(complex*16)
PTPSV
 - Description: solving triangular packed matrix problems
 - Syntax: PGER( UPLO, TRANS, DIAG, N, AP, X, INCX)
    - P:  S(single float), D(double float), C(complex), Z(complex*16)
PGER
 - Description: performs the rank 1 operation A := alpha*x*y' + A
 - Syntax: PGER( M, N, ALPHA, X, INCX, Y, INCY, A, LDA)
    - P:  S(single float), D(double float)
PSYR
 - Description: performs the symmetric rank 1 operation A := alpha*x*x' + A
 - Syntax: PSYR( UPLO,  N, ALPHA, X, INCX, A, LDA)
    - P: S(single float), D(double float)
PSPR -
 - Description: symmetric packed rank 1 operation A := alpha*x*x' + A
 - Syntax: PSPR( UPLO,  N, ALPHA, X, AP)
    - P: S(single float), D(double float)
PSYR2
 - Description: performs the symmetric rank 2 operation, A := alpha*x*y' + alpha*y*x' + A
 - Syntax: PSYR2( UPLO,  N, ALPHA, X, INCX, Y, INCY, A, LDA)
    - P: S(single float), D(double float)
PSPR2
 - Description: performs the symmetric packed rank 2 operation, A := alpha*x*y' + alpha*y*x' + A
 - Syntax: PSPR2( UPLO,  N, ALPHA, X, INCX, Y, INCY, AP)
    - P: S(single float), D(double float)
Level 3
PGEMM
 - Description: matrix matrix multiply
 - Syntax: PGEMM( TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC)
    - P: S(single float), D(double float), C(complex), Z(complex*16)
PSYMM
 - Description: symmetric matrix matrix multiply
 - Syntax: PTRSM( SIDE, UPLD, M, N, ALPHA, A, LDA, B, LDB, BETA, C, LDC)
    - P: S(single float), D(double float), C(complex), Z(complex*16)
PSYRK
 - Description: symmetric rank-k update to a matrix
 - Syntax: PSYR2K( UPLD, TRANSA, N, K, ALPHA, A, LDA, BETA, C, LDC)
    - P: S(single float), D(double float), C(complex), Z(complex*16)
PSYR2K
 - Description: symmetric rank-2k update to a matrix
 - Syntax: PSYR2K( UPLD, TRANSA, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC)
    - P: S(single float), D(double float), C(complex), Z(complex*16)
PTRMM -
 - Description: triangular matrix matrix multiply
 - Syntax: PTRMM( SIDE, UPLD, TRANSA, DIAG, M, N, ALPHA, A, LDA, B, LDB)
    - P: S(single float), D(double float), C(complex), Z(complex*16)
PTRSM
 - Description: solving triangular matrix with multiple right hand sides
 - Syntax: PTRSM( SIDE, UPLD, TRANSA, DIAG, M, N, ALPHA, A, LDA, B, LDB)
    - P: S(single float), D(double float), C(complex), Z(complex*16)
```
BLAS 函数接口命名规范
BLAS Level 1 接口函数名称由前缀+操作简称组成。
例如**S****ROTG**，其中
**S**-- 标明矩阵或向量中元素数据类型的前缀；
**ROTG**-- 向量运算简称.
前缀： 矩阵或向量内元素的数据类型，有以下几种：
S - 单精度浮点数
D - 双精度浮点数
C - 复数
Z - 16位复数

Level 2 和 Level 3函数涉及矩阵运算，接口函数名称由前缀 + 矩阵类型 + 操作简称组成。
例如:**S****GE****MV**
**S**-- 标明矩阵或向量中元素数据类型的前缀；
**GE**-- 矩阵类型
**MV**-- 向量或矩阵运算简称
BLAS库中使用的矩阵类型有以下几种：
GE - GEnearl  稠密矩阵
GB - General Band 带状矩阵
SY - SYmmetric    对称矩阵
SB - Symmetric Band 对称带状矩阵
SP - Symmetric Packed  压缩存储对称矩阵
HE - HEmmitian     Hemmitian矩阵，自共轭矩阵
HB - Hemmitian Band   带状Hemmitian矩阵
HP - Hemmitian Packed  压缩存储Hemmitian矩阵
TR - TRiangular      三角矩阵
TB - Triangular Band  三角带状矩阵
TP - Triangular Packed  压缩存储三角矩阵

## BLAS库的实现
向量和矩阵运算是数值计算的基础，BLAS库通常是一个软件计算效率的决定性因素。除了BLAS参考库以外，还有多种衍生版本和优化版本。这些BLAS库实现中，有些仅实现了其它编程语言的BLAS库接口，有些是基于BLAS参考库的Fortran语言代码翻译成其它编程语言，有些是通过二进制文件代码转化方法将BLAS参考库转换成其它变成语言代码，有些是在BLAS参考库的基础上，针对不同硬件(如CPU，GPU)架构特点做进一步优化[4][5]。
**ATLAS BLAS**[3]
The ATLAS (Automatically Tuned Linear Algebra Software) project is an ongoing research effort focusing on applying empirical techniques in order to provide portable performance. At present, it provides C and Fortran77 interfaces to a portably efficient BLAS implementation, as well as a few routines from LAPACK.
**OpenBLAS**[4]
OpenBLAS is an optimized BLAS library based on GotoBLAS2 1.13 BSD version.
**Intel® Math Kernel Library**[5]
Intel® Math Kernel Library (Intel® MKL) accelerates math processing and neural network routines that increase application performance and reduce development time. Intel MKL includes highly vectorized and threaded Linear Algebra, Fast Fourier Transforms (FFT), Neural Network, Vector Math and Statistics functions. The easiest way to take advantage of all of that processing power is to use a carefully optimized math library. Even the best compiler can’t compete with the level of performance possible from a hand-optimized library. If your application already relies on the BLAS or LAPACK functionality, simply re-link with Intel MKL to get better performance on Intel and compatible architectures.
**cuBLAS**[6]
The NVIDIA CUDA Basic Linear Algebra Subroutines (cuBLAS) library is a GPU-accelerated version of the complete standard BLAS library that delivers 6x to 17x faster performance than the latest MKL BLAS.
**clBLAS**[7]
This repository houses the code for the OpenCL™ BLAS portion of clMath. The complete set of BLAS level 1, 2 & 3 routines is implemented.
**BLIS**[10]
BLIS is a portable software framework for instantiating high-performance BLAS-like dense linear algebra libraries. The framework was designed to isolate essential kernels of computation that, when optimized, enable optimized implementations of most of its commonly used and computationally intensive operations. Select kernels have been optimized for the AMD EPYCTM processor family. The optimizations are done for single and double precision routines.
**其它矩阵计算库**
**SparseLib++**--- Numerical Sparse Matrix Classes in C++
[http://math.nist.gov/sparselib](http://math.nist.gov/sparselib)
SparseLib++ is a C++ class library for efficient sparse matrix computations
across various computational platforms.  The software package consists of
matrix objects representing several sparse storage formats currently in use
(in this release: compressed row, compressed column and coordinate formats),
providing basic functionality for managing sparse matrices, together with
efficient kernel mathematical operations (e.g. sparse matrix-vector multiply).
Routines based on the Sparse BLAS are used to enhance portability and
performance. Included in the package are various preconditioners commonly
used in iterative solvers for linear systems of equations.  The focus is on
computational support for iterative methods, but the sparse matrix objects
presented here can be used on their own.
SparseLib++最新版是 v. 1.7. 最近更新时间是2008年(已经好久没更新了)
SparseLib++ 1.7 使用了complex.h等C99特性。 使用g++ v. 4.0.1以上版本能编译。Visual Studio对不支持所有C99特性，不能直接使用VS编译SparseLib++ 1.7(可以通过mingw编译)
**PETSc**
https://www.mcs.anl.gov/petsc/
PETSc, pronounced PET-see (the S is silent), is a suite of data structures and routines for the scalable (parallel) solution of scientific applications modeled by partial differential equations. It supports MPI, and GPUs through CUDA or OpenCL, as well as hybrid MPI-GPU parallelism. PETSc (sometimes called PETSc/Tao) also contains the Tao optimization software library.
**SuitSparse**
http://faculty.cse.tamu.edu/davis/suitesparse.html
SuiteSparse is a suite of sparse matrix algorithms,
另外， 网页[8]上列举了许多矩阵计算库
参考文献
[1] http://www.netlib.org/blas/
[2 ]https://en.wikipedia.org/wiki/Basic_Linear_Algebra_Subprograms
[3] http://math-atlas.sourceforge.net/
[4] http://www.openblas.net/
[5] https://software.intel.com/en-us/intel-mkl/
[6] https://developer.nvidia.com/cublas
[7] https://github.com/clMathLibraries/clBLAS
[8] https://scicomp.stackexchange.com/questions/351/recommendations-for-a-usable-fast-c-matrix-library
[9] https://martin-thoma.com/solving-linear-equations-with-gaussian-elimination/
[10] https://developer.amd.com/amd-cpu-libraries/blas-library/

