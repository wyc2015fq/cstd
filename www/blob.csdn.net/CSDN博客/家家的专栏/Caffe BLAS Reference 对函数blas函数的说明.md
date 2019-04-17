# Caffe BLAS Reference 对函数blas函数的说明 - 家家的专栏 - CSDN博客





2014年09月03日 19:44:48[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4730








**详细的对每个函数的说明文档：https://developer.apple.com/library/mac/documentation/Accelerate/Reference/BLAS_Ref/Reference/reference.html**







从某个网页复制过来的其中一个函数的说明：

计算矩阵乘法的函数之一是 cblas_sgemm，使用单精度实数，另外还有对应双精度实数，单精度复数和双精度复数的函数。在此以
 cblas_sgemm为例。

函数定义为：


void cblas_sgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,


const enum CBLAS_TRANSPOSE TransB, const int M, const int N,


const int K, const float alpha, const float  *A,


const int lda, const float  *B, const int ldb,


const float beta, float  *C, const int ldc)


得到的结果是:

C = alpha*op( A )*op(
 B ) + beta*C

const enum CBLAS_ORDER Order，这是指的数据的存储形式，在CBLAS的函数中无论一维还是二维数据都是用一维数组存储，这就要涉及是行主序还是列主序，在C语言中数组是用 行主序，fortran中是列主序。我还是习惯于是用行主序，所以这个参数是用CblasRowMajor，如果是列主序的话就是
 CblasColMajor。

const int M，矩阵A的行，矩阵C的行


const int N，矩阵B的列，矩阵C的列


const int K，矩阵A的列，矩阵B的行


const float alpha， const float beta，计算公式中的两个参数值，如果只是计算C=A*B，则alpha=1,beta=0


const float  *A， const float  *B， const float  *C，矩阵ABC的数据


const int lda， const int ldb， const int ldc，在BLAS的文档里，这三个参数分别为ABC的行数，但是实际使用发现，在CBLAS里应该是列数。


The following program computes the product of two matrices using the Level-3 BLAS function SGEMM,

[ 0.11 0.12 0.13 ]  [ 1011 1012 ]     [ 367.76 368.12 ]
[ 0.21 0.22 0.23 ]  [ 1021 1022 ]  =  [ 674.06 674.72 ]
                    [ 1031 1032 ]


The matrices are stored in row major order but could be stored in column major order if the first argument of the call to `cblas_sgemm` was changed to `CblasColMajor`.

#include <stdio.h>
#include <gsl/gsl_cblas.h>

int
main (void)
{
  int lda = 3;

  float A[] = { 0.11, 0.12, 0.13,
                0.21, 0.22, 0.23 };

  int ldb = 2;
  
  float B[] = { 1011, 1012,
                1021, 1022,
                1031, 1032 };

  int ldc = 2;

  float C[] = { 0.00, 0.00,
                0.00, 0.00 };

  /* Compute C = A B */

  cblas_sgemm (CblasRowMajor, 
               CblasNoTrans, CblasNoTrans, 2, 2, 3,
               1.0, A, lda, B, ldb, 0.0, C, ldc);

  printf ("[ %g, %g\n", C[0], C[1]);
  printf ("  %g, %g ]\n", C[2], C[3]);

  return 0;  
}


To compile the program use the following command line,

$ gcc -Wall demo.c -lgslcblas


There is no need to link with the main library `-lgsl` in this case as the CBLAS library is an independent unit. Here is the output from the program,


$ ./a.out
[ 367.76, 368.12
  674.06, 674.72 ].$ ./a.out
[ 367.76, 368.12
  674.06, 674.72 ].
转自：http://blog.sina.com.cn/s/blog_4a03c0100101ethf.html



https://www.gnu.org/software/gsl/manual/html_node/GSL-CBLAS-Examples.html



