
# 学习BLAS库 -- DTRSM - cocoonyang的专栏 - CSDN博客


2015年08月07日 00:09:06[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1112


函数语法:
XTRSM(SIDE, UPLD, TRANSA, DIAG, M, N, ALPHA, A, LDA, B, LDB)
**功能**：
Solves one of the matrix equations
op( A )*X = alpha*B,   or   X*op( A ) = alpha*B,
where alpha is a scalar, X and B are m by n matrices, A is a unit, or
non-unit,  upper or lower triangular matrix  and  op( A )  is one  of
op( A ) = A   or   op( A ) = A**T.
The matrix X is overwritten on B.
**参数释**：
SIDE
SIDE is CHARACTER*1
On entry, SIDE specifies whether op( A ) appears on the left
or right of X as follows:
SIDE = 'L' or 'l'   op( A )*X = alpha*B.
SIDE = 'R' or 'r'   X*op( A ) = alpha*B.
SIDE = 'R' or 'r'   X*op( A ) = alpha*B.
UPLO
UPLO is CHARACTER*1
On entry, UPLO specifies whether the matrix A is an upper or
lower triangular matrix as follows:
UPLO = 'U' or 'u'   A is an upper triangular matrix.
UPLO = 'L' or 'l'   A is a lower triangular matrix.
TRANSA
TRANSA is CHARACTER*1
On entry, TRANSA specifies the form of op( A ) to be used in
the matrix multiplication as follows:
TRANSA = 'N' or 'n'   op( A ) = A.
TRANSA = 'T' or 't'   op( A ) = A**T.
TRANSA = 'C' or 'c'   op( A ) = A**T.
DIAG
DIAG is CHARACTER*1
On entry, DIAG specifies whether or not A is unit triangular
as follows:
DIAG = 'U' or 'u'   A is assumed to be unit triangular.
DIAG = 'N' or 'n'   A is not assumed to be unit
triangular.
M
M is INTEGER
On entry, M specifies the number of rows of B. M must be at
least zero.
N
N is INTEGER
On entry, N specifies the number of columns of B.  N must be
at least zero.
ALPHA
ALPHA is REAL
On entry,  ALPHA specifies the scalar  alpha. When  alpha is
zero then  A is not referenced and  B need not be set before
entry.
A
A is REAL array of DIMENSION ( LDA, k ),
where k is m when SIDE = 'L' or 'l'
and k is n when SIDE = 'R' or 'r'.
Before entry  with  UPLO = 'U' or 'u',  the  leading  k by k
upper triangular part of the array  A must contain the upper
triangular matrix  and the strictly lower triangular part of
A is not referenced.
Before entry  with  UPLO = 'L' or 'l',  the  leading  k by k
lower triangular part of the array  A must contain the lower
triangular matrix  and the strictly upper triangular part of
A is not referenced.
Note that when  DIAG = 'U' or 'u',  the diagonal elements of
A  are not referenced either,  but are assumed to be  unity.
LDA
LDA is INTEGER
On entry, LDA specifies the first dimension of A as declared
in the calling (sub) program.  When  SIDE = 'L' or 'l'  then
LDA  must be at least  max( 1, m ),  when  SIDE = 'R' or 'r'
then LDA must be at least max( 1, n ).
B
B is REAL array of DIMENSION ( LDB, n ).
Before entry,  the leading  m by n part of the array  B must
contain  the  right-hand  side  matrix  B,  and  on exit  is
overwritten by the solution matrix  X.
LDB
LDB is INTEGER
On entry, LDB specifies the first dimension of B as declared
in  the  calling  (sub)  program.   LDB  must  be  at  least
max( 1, m ).
**使用方法**：
测试DTRSM函数
```python
/******************************************************************************/
/*
 Purpose:
 TEST03 tests DTRSM.
 Licensing:
 This code is distributed under the GNU LGPL license.
 Modified:
 07 April 2014
 Author:
 John Burkardt
 */
void test_dtrsm()
{
	double *a;
	double alpha;
	double *b;
	char diag;
	int i;
	int j;
	int lda;
	int ldb;
	int m;
	int n;
	char side;
	char transa;
	char transb;
	char uplo;
	char functionName[] = "DTRSM";
	printf("================================================ \n");
	printf("Testing BLAS library function -- %s \n", functionName);
	printf("\n");
	//printf ( "TEST DTRSM\n" );
	printf("  DTRSM solves a linear system involving a triangular\n");
	printf("  matrix A and a rectangular matrix B.\n");
	printf("\n");
	printf("  1: Solve A  * X  = alpha * B;\n");
	printf("  2: Solve A' * X  = alpha * B;\n");
	printf("  3: Solve X  * A  = alpha * B;\n");
	printf("  4: Solve X  * A' = alpha * B;\n");
	/*
	 Solve A * X = alpha * B.
	 */
	side = 'L';
	uplo = 'U';
	transa = 'N';
	diag = 'N';
	m = 4;
	n = 5;
	alpha = 2.0;
	lda = m;
	ldb = m;
	a = (double *) malloc(lda * m * sizeof(double));
	for (j = 0; j < m; j++)
	{
		for (i = 0; i <= j; i++)
		{
			a[i + j * lda] = (double) (i + j + 2);
			//a[i+j*lda] = 0 ;
		}
		for (i = j + 1; i < m; i++)
		{
			a[i + j * lda] = 0.0;
		}
		//a[j+j*lda] = 1 ;
	}
	transb = 'N';
	for ( j = 0; j < m; j++ )
	{
	     for ( i = 0; i <m; i++ )
	     {
	    	 b[i+j*ldb] = 0 ;
	     }
	     b[j+j*ldb] = 1 ;
	}
	printf("Test matrix \n");
	r8mat_print(m, m, a, "  A = ");
	r8mat_print(m, n, b, "   B:");
	cblas_dtrsm(side, uplo, transa, diag, m, n, alpha, a, lda, b, ldb);
	r8mat_print(m, n, b, "  X = inv ( A ) * alpha * B:");
	free(a);
	free(b);
	/*
	 Solve A' * X = alpha * B.
	 */
	side = 'L';
	uplo = 'U';
	transa = 'T';
	diag = 'N';
	m = 4;
	n = 5;
	alpha = 2.0;
	lda = m;
	ldb = m;
	a = (double *) malloc(lda * m * sizeof(double));
	for (j = 0; j < m; j++)
	{
		for (i = 0; i <= j; i++)
		{
			a[i + j * lda] = (double) (i + j + 2);
			//a[i+j*lda] = 0 ;
		}
		for (i = j + 1; i < m; i++)
		{
			a[i + j * lda] = 0.0;
		}
		//a[j+j*lda] = 1 ;
	}
	transb = 'N';
	b = r8mat_test(transb, ldb, m, n);
	cblas_dtrsm(side, uplo, transa, diag, m, n, alpha, a, lda, b, ldb);
	r8mat_print(m, n, b, "  X = inv ( A' ) * alpha * B:");
	free(a);
	free(b);
	/*
	 Solve X * A = alpha * B.
	 */
	side = 'R';
	uplo = 'U';
	transa = 'N';
	diag = 'N';
	m = 4;
	n = 5;
	alpha = 2.0;
	lda = n;
	ldb = m;
	a = (double *) malloc(lda * n * sizeof(double));
	for (j = 0; j < n; j++)
	{
		for (i = 0; i <= j; i++)
		{
			a[i + j * lda] = (double) (i + j + 2);
		}
		for (i = j + 1; i < n; i++)
		{
			a[i + j * lda] = 0.0;
		}
	}
	transb = 'N';
	b = r8mat_test(transb, ldb, m, n);
	printf("Test matrix \n");
	r8mat_print(n, n, a, "  A = ");
	r8mat_print(m, n, b, "  B = ");
	cblas_dtrsm(side, uplo, transa, diag, m, n, alpha, a, lda, b, ldb);
	r8mat_print(m, n, b, "  X = alpha * B * inv ( A ):");
	free(a);
	free(b);
	/*
	 Solve X * A'' = alpha * B.
	 */
	side = 'R';
	uplo = 'U';
	transa = 'T';
	diag = 'N';
	m = 4;
	n = 5;
	alpha = 2.0;
	lda = n;
	ldb = m;
	a = (double *) malloc(lda * n * sizeof(double));
	for (j = 0; j < n; j++)
	{
		for (i = 0; i <= j; i++)
		{
			a[i + j * lda] = (double) (i + j + 2);
		}
		for (i = j + 1; i < n; i++)
		{
			a[i + j * lda] = 0.0;
		}
	}
	transb = 'N';
	b = r8mat_test(transb, ldb, m, n);
	cblas_dtrsm(side, uplo, transa, diag, m, n, alpha, a, lda, b, ldb);
	r8mat_print(m, n, b, "  X = alpha * B * inv ( A' ):");
	free(a);
	free(b);
	return;
}
```

运行结果:
```python
Testing BLAS library function -- DTRSM 
  DTRSM solves a linear system involving a triangular
  matrix A and a rectangular matrix B.
  1: Solve A  * X  = alpha * B;
  2: Solve A' * X  = alpha * B;
  3: Solve X  * A  = alpha * B;
  4: Solve X  * A' = alpha * B;
Test matrix 
  A = 
  Col:        0           1           2           3     
  Row
    0:           2           3           4           5
    1:           0           4           5           6
    2:           0           0           6           7
    3:           0           0           0           8
   B:
  Col:        0           1           2           3           4     
  Row
    0:          11          12          13          14          15
    1:          21          22          23          24          25
    2:          31          32          33          34          35
    3:          41          42          43          44          45
  X = inv ( A ) * alpha * B:
  Col:        0           1           2           3           4     
  Row
    0:    -7.10938    -6.92708    -6.74479     -6.5625    -6.38021
    1:    -2.84375    -2.77083    -2.69792      -2.625    -2.55208
    2:      -1.625    -1.58333    -1.54167        -1.5    -1.45833
    3:       10.25        10.5       10.75          11       11.25
  X = inv ( A' ) * alpha * B:
  Col:        0           1           2           3           4     
  Row
    0:          11          12          13          14          15
    1:        2.25           2        1.75         1.5        1.25
    2:       1.125           1       0.875        0.75       0.625
    3:    0.703125       0.625    0.546875     0.46875    0.390625
Test matrix 
  A = 
  Col:        0           1           2           3           4     
  Row
    0:           2           3           4           5           6
    1:           0           4           5           6           7
    2:           0           0           6           7           8
    3:           0           0           0           8           9
    4:           0           0           0           0          10
  B = 
  Col:        0           1           2           3           4     
  Row
    0:          11          12          13          14          15
    1:          21          22          23          24          25
    2:          31          32          33          34          35
    3:          41          42          43          44          45
  X = alpha * B * inv ( A ):
  Col:        0           1           2           3           4     
  Row
    0:          11       -2.25      -1.125   -0.703125   -0.492188
    1:          21       -4.75      -2.375    -1.48438    -1.03906
    2:          31       -7.25      -3.625    -2.26563    -1.58594
    3:          41       -9.75      -4.875    -3.04688    -2.13281
  X = alpha * B * inv ( A' ):
  Col:        0           1           2           3           4     
  Row
    0:    0.820313    0.328125      0.1875       0.125           3
    1:     2.46094    0.984375      0.5625       0.375           5
    2:     4.10156     1.64063      0.9375       0.625           7
    3:     5.74219     2.29688      1.3125       0.875           9
```

参考文献
http://people.sc.fsu.edu/~jburkardt/c_src/super_blas/super_blas.html
http://www.netlib.org/lapack/explore-html/


