# 学习BLAS库 -- TRMV - cocoonyang的专栏 - CSDN博客





2017年02月12日 14:18:57[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：443








**函数语法:**

    XTRMV( UPLO, TRANS, DIAG, A, LDA, X, INCX)

**功能**：

    Triangular matrix vector multiply



Open BLAS   trmv methods testing benchmark

```cpp
//
// TRMV   x <- Ax
//
/*
 * void cblas_strmv(
 OPENBLAS_CONST enum CBLAS_ORDER order,
 OPENBLAS_CONST enum CBLAS_UPLO Uplo,
 OPENBLAS_CONST enum CBLAS_TRANSPOSE TransA,
 OPENBLAS_CONST enum CBLAS_DIAG Diag,
 OPENBLAS_CONST blasint N,
 OPENBLAS_CONST float *A,
 OPENBLAS_CONST blasint lda,
 float *X,
 OPENBLAS_CONST blasint incX);
 */
void strmv_test() {
	int n = 2;

	float *x = (float*) calloc(n, sizeof(float));
	float *b = (float*) calloc(n, sizeof(float));
	float *A = (float*) calloc(n * n, sizeof(float));
	float *p = (float*) calloc(n, sizeof(float));
	float *r = (float*) calloc(n, sizeof(float));

	A[0] = 4;
	// A[1] = 0;
	A[2] = 1;
	A[3] = 3;

	b[0] = 0;
	b[1] = 0;

	x[0] = 2;
	x[1] = 1;

	for (int i = 0; i < n; i++) {
		printf("x[i] = %f \n", x[i]);
	}

	for (int i = 0; i < n * n; i++) {
		printf("A[i] = %f \n", A[i]);
	}

	cblas_strmv(CblasColMajor, CblasUpper, CblasNoTrans, CblasNonUnit, 2, A, 2,
			x, 1);

	for (int i = 0; i < n; i++) {
		printf("x[i] = %f \n", x[i]);
	}

	free(x);
	free(b);
	free(A);
	free(p);
	free(r);
}
```



测试结果 

```
x[i] = 2.000000
x[i] = 1.000000
A[i] = 4.000000
A[i] = 0.000000
A[i] = 1.000000
A[i] = 3.000000
x[i] = 9.000000
x[i] = 3.000000
```



