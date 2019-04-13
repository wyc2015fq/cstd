
# 学习BLAS库 -- LAPACK - cocoonyang的专栏 - CSDN博客


2017年10月04日 17:36:59[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：991


BLAS库中Level 3函数是用于矩阵与矩阵之间运算。LAPACK库设计目标是作为BLAS中Level 3函数的扩展库。
|Category|
|---|---|
|应用范围|求解线性方程组，最小二乘问题 特征值问题, 奇异值分解问题|
|编程语言|Fortran90|
|矩阵类型|稠密阵和带状阵(不支持稀疏阵)|
|数据类型|单精度和双精度的实矩阵及复矩阵|
# LAPACK函数
## 命名
LAPCAK中函数名由 数据类型+ 矩阵类型 + 矩阵运算  组成。例如:
SGETRF
其中
-  S  – 数据类型，S代表单精度浮点数
-  GE – 矩阵类型，GE代表稠密阵
- TRF – 矩阵运算, TRF代表 三角分解法
### 数据类型
矩阵内元素的数据类型，有以下几种：
S - 单精度浮点数
D - 双精度浮点数
C - 复数
Z - 16位复数
### 矩阵类型
矩阵类型有：
|缩写代号|矩阵类型|
|---|---|
|BD|bidiagonal 二对角阵|
|DI|diagonal 对角阵|
|GB|general band 普通带状阵|
|GE|general (i.e., unsymmetric, in some cases rectangular) 普通矩阵(例如非对称阵，矩形|
|GG|general matrices, generalized problem (i.e., a pair of general matrices)  普通矩阵组，广义问题(例如，一对普通矩阵)|
|GT|general tridiagonal 普通三角阵|
|HB|(complex) Hermitian band (复) 埃尔米特带状阵|
|HE|(complex) Hermitian (复)埃尔米特矩阵|
|HG|upper Hessenberg matrix, generalized problem (i.e a Hessenberg and a    triangular matrix)  上 海森阵，广义问题(例如一个海森阵和一个三角阵)|
|HP|(complex) Hermitian, packed storage (复)海森阵(压缩存储)|
|HS|upper Hessenberg 上 海森阵|
|OP|(real) orthogonal, packed storage (实)正交阵(压缩存储)|
|OR|(real) orthogonal (实)正交阵|
|PB|symmetric or Hermitian positive definite band 正定对称(埃尔米特矩阵)带状阵|
|PO|symmetric or Hermitian positive definite 正定对称(埃尔米特矩阵)阵|
|PP|symmetric or Hermitian positive definite, packed storage 正定对称(埃尔米特矩阵)阵(压缩存储)|
|PT|symmetric or Hermitian positive definite tridiagonal 正定对称(埃尔米特矩阵)三角阵|
|SB|(real) symmetric band  (实)对称带状阵|
|SP|symmetric, packed storage 对称阵(压缩存储)|
|ST|(real) symmetric tridiagonal (实)对称三对角阵|
|SY|symmetric 对称阵|
|TB|triangular band 三角带状阵|
|TG|triangular matrices, generalized problem (i.e., a pair of triangular matrices) 三角阵组，广义问题(例|
|TP|triangular, packed storage 三角阵(压缩存储)|
|TR|triangular (or in some cases quasi-triangular) 三角阵|
|TZ|trapezoidal 梯形阵|
|UN|(complex) unitary (复)酉矩阵|
|UP|(complex) unitary, packed storage (复)酉矩阵(压缩存储)|
## 函数类别
LAPCAK中矩阵运算函数分为三类：driver函数, computational函数 auxiliary函数
### Driver函数
Driver Routines for Linear Equations
|Name|Element Precision|Matrix Type and Storage Scheme|Description|
|---|---|---|---|
|XGESV|S, C, D, Z|General matrix|Solving linear equations system AX = B|
Driver Routines for Linear Least Squares Problem
|Name|Element Precision|Matrix Type and Storage Scheme|Description|
|---|---|---|---|
|SGELS|S, C, D, Z|General matrix|Solving the linear least squares problem|min || B - A X ||_{2}|using QR or LQ factorization|
|SGELSY|S, C, D, Z|General matrix|Solving the linear least squares problem|min || B - A X ||_{2}|using complete orthogonal factorization|
|SGELSS|S, C, D, Z|General matrix|Solving the linear least squares problem|min || B - A X ||_{2}|using SVD|
|SGELSD|S, C, D, Z|General matrix|Solving the linear least squares problem|min || B - A X ||_{2}|using divide-and-conquer SVD|
### Computational Routines
Routines for Linear Equations
|Name|Element Precision|Matrix Type and Storage Scheme|Description|
|---|---|---|---|
|XGETRF|S, C, D, Z|General matrix|factorize|
|XGETRS|S, C, D, Z|General matrix|solve using factorization|
|XGECON|S, C, D, Z|General matrix|estimate condition number|
|XGERFS|S, C, D, Z|General matrix|error bounds for solution|
|XGETRI|S, C, D, Z|General matrix|invert using factorization|
|XGEEQU|S, C, D, Z|General matrix|equilibrate|
|XPOTRF|S, C, D, Z|Symmetric positive|factorize|
|XPOTRS|S, C, D, Z|Symmetric positive|solve using factorization|
|XPOCON|S, C, D, Z|Symmetric positive|estimate condition number|
|XPORFS|S, C, D, Z|Symmetric positive|error bounds for solution|
|XPOTRI|S, C, D, Z|Symmetric positive|invert using factorization|
|XPOEQU|S, C, D, Z|Symmetric positive|equilibrate|
|XPPTRF|S, C, D, Z|Symmetric positive definite band|factorize|
|XPPTRS|S, C, D, Z|Symmetric positive definite band|solve using factorization|
|XPPCON|S, C, D, Z|Symmetric positive definite band|estimate condition number|
|XPPRFS|S, C, D, Z|Symmetric positive definite band|error bounds for solution|
|XPPTRI|S, C, D, Z|Symmetric positive definite band|invert using factorization|
|XPPEQU|S, C, D, Z|Symmetric positive definite band|equilibrate|
|XPTTRF|S, C, D, Z|Symmetric tridiagonal definite band|factorize|
|XPTTRS|S, C, D, Z|Symmetric tridiagonal definite band|solve using factorization|
|XPTCON|S, C, D, Z|Symmetric tridiagonal definite band|estimate condition number|
|XPTRFS|S, C, D, Z|Symmetric tridiagonal definite band|error bounds for solution|
( … 待续)
# 在Windows的Visual Studio环境中安装与使用CLAPACK
预备工作: 已经安装好Visual Studio环境，本人使用的是Visual Studio 2015.
从[1] 下载LAPACKE_examples.zip, 此文件中已经准备好Windows环境中所需的动态库以及头文件. 解压缩此文件到自定的文件夹中.
进入\example_DGESV_rowmajor 文件夹, 运行example_DGESV_rowmajor.vcxproj, 即可在Release文件夹中生成测试算例.
## 使用nmake编译
从[1] 下载LAPACKE_examples.zip, 此文件中已经准备好Windows环境中所需的动态库以及头文件. 解压缩此文件到自定的文件夹中.
在此文件夹中创建名为src文件夹，在src文件夹main.cpp.
```python
#include <stdlib.h>
```
```python
#include <stdio.h>
```
```python
#include "lapacke.h"
```
```python
/* Auxiliary routine: printing a matrix */
```
```python
void
```
```python
print_matrix(
```
```python
char
```
```python
* desc, lapack_int m, lapack_int n,
```
```python
double
```
```python
* a, lapack_int lda ) {
        lapack_int i, j;
```
```python
printf
```
```python
(
```
```python
"\n %s\n"
```
```python
, desc );
```
```python
for
```
```python
( i =
```
```python
0
```
```python
; i < m; i++ ) {
```
```python
for
```
```python
( j =
```
```python
0
```
```python
; j < n; j++ )
```
```python
printf
```
```python
(
```
```python
" %6.2f"
```
```python
, a[i*lda+j] );
```
```python
printf
```
```python
(
```
```python
"\n"
```
```python
);
        }
}
```
```python
void
```
```python
gesvTest() {
```
```python
printf
```
```python
(
```
```python
"GESV Testing \n"
```
```python
);
```
```python
int
```
```python
n =
```
```python
5
```
```python
;
```
```python
int
```
```python
lda = n;
```
```python
int
```
```python
ldb =
```
```python
1
```
```python
;
```
```python
int
```
```python
nrhs =
```
```python
1
```
```python
;
```
```python
/* Pivot indices */
```
```python
int
```
```python
ipiv[N];
```
```python
double
```
```python
a[LDA*N] = {
```
```python
1.00
```
```python
,  -
```
```python
0.0
```
```python
, -
```
```python
0.0
```
```python
,
```
```python
0.0
```
```python
, -
```
```python
0.0
```
```python
,
```
```python
1.0
```
```python
, -
```
```python
3.30
```
```python
,
```
```python
2.58
```
```python
,
```
```python
2.71
```
```python
, -
```
```python
5.14
```
```python
,
```
```python
0.0
```
```python
,
```
```python
5.36
```
```python
, -
```
```python
2.70
```
```python
,
```
```python
4.35
```
```python
, -
```
```python
7.26
```
```python
,
```
```python
0.0
```
```python
, -
```
```python
4.44
```
```python
,
```
```python
0.27
```
```python
, -
```
```python
7.17
```
```python
,
```
```python
6.08
```
```python
,
```
```python
0.0
```
```python
,
```
```python
1.08
```
```python
,
```
```python
9.04
```
```python
,
```
```python
2.14
```
```python
, -
```
```python
6.87
```
```python
};
```
```python
double
```
```python
b[N] = {
```
```python
2.0
```
```python
,
```
```python
1.0
```
```python
,
```
```python
0.0
```
```python
,
```
```python
0.0
```
```python
,
```
```python
0.0
```
```python
};
```
```python
/* Print Entry Matrix */
```
```python
print_matrix(
```
```python
"Entry Matrix A"
```
```python
, n, n, a, lda );
    print_matrix(
```
```python
"RHS vector"
```
```python
, n,
```
```python
1
```
```python
, b,
```
```python
1
```
```python
);
```
```python
int
```
```python
info = LAPACKE_dgesv( LAPACK_ROW_MAJOR, n, nrhs, a, lda, ipiv,
                            b, ldb );
    print_matrix(
```
```python
"Result vector"
```
```python
, n,
```
```python
1
```
```python
, b,
```
```python
1
```
```python
);
}
```
```python
/* Main program */
```
```python
int
```
```python
main() { 
        gesvTest();
```
```python
exit
```
```python
(
```
```python
0
```
```python
);
}
```
在此文件夹中创建名为tmp文件夹，在tmp文件夹中，创建nmake编译时所需的Makefile文件。Makefile文件如下:
```python
# Target
```
```python
PROGRAM = test
```
```python
.exe
```
```python
LIBDIRS =
```
```python
"../lib/"
```
```python
inc
```
```python
=
```
```python
"../include/"
```
```python
src =
```
```python
"../src/"
```
```python
INCLUDEDIRS =  /I $(
```
```python
inc
```
```python
)  /I $(LIBDIRS)
```
```python
# Flags
```
```python
CPPOPT = $(INCLUDEDIRS) /w /EHsc /D_CRT_SECURE_NO_DEPRECATE /D ADD_ /D HAVE_LAPACK_CONFIG_H /D LAPACK_COMPLEX_STRUCTURE /D WIN32 /D NDEBUG /D _CONSOLE    
    LIBS =  ../lib/liblapacke
```
```python
.lib
```
```python
../lib/liblapack
```
```python
.lib
```
```python
../lib/libblas
```
```python
.lib
```
```python
# Compiler
```
```python
cc = cl     
    CFLAGS =     
    LIBFLAGS =  /LIBPATH $(LIBDIRS)
```
```python
# list of source files
```
```python
CPPSOURCES =  main
```
```python
.cpp
```
```python
# expands to list of object files
```
```python
CPPOBJECTS = $(CPPSOURCES:
```
```python
.cpp
```
```python
=
```
```python
.obj
```
```python
)     
    all: $(PROGRAM)    
    $(PROGRAM): $(CPPOBJECTS)   
        link
```
```python
.exe
```
```python
/
```
```python
out
```
```python
:$(PROGRAM)  $(CPPOBJECTS)   $(LIBS)      
    main
```
```python
.obj
```
```python
:     
        $(cc) $(CPPOPT) /c ../src/main
```
```python
.cpp
```
```python
clean:      
        del $(CPPOBJECTS) $(PROGRAM)
```
4.启动Developer Command Prompt for VS2015，进入tmp文件夹，运行nmake。会生成test.exe 文件。
5. 将压缩包里Release文件夹中的dll库文件拷贝到tmp文件夹中，运行test.exe.
运行结果：
![这里写图片描述](https://img-blog.csdn.net/20180529103027954?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
> [1]
> [http://icl.cs.utk.edu/lapack-for-windows/clapack/](http://icl.cs.utk.edu/lapack-for-windows/clapack/)

> [2]
> [http://www.netlib.org/lapack/index.html](http://www.netlib.org/lapack/index.html)

> [3]
> [http://math.nist.gov/lapack++/](http://math.nist.gov/lapack++/)

> [4]
> [http://www.netlib.org/lapack/lug/](http://www.netlib.org/lapack/lug/)


