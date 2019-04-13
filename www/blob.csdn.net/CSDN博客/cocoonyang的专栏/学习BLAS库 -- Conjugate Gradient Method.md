
# 学习BLAS库 -- Conjugate Gradient Method - cocoonyang的专栏 - CSDN博客


2017年02月13日 11:13:31[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：260


Conjugate gradient method implementation based on OpenBLAS


```python
#include "cblas.h"
#include <stdio.h>
#define ERROR 1.0e-5
void conjugateGradient_sche( float *A, float *x, float *d, float *r, int n )
{
	if( ( NULL == A ) || (NULL == x ) || (NULL == d ) || (NULL == r ) )
	{
		return;
	}
	if( n < 0 )
	{
		return;
	}
	int inc1 = 1;
	int inc2 = 1;
	float *tmp = (float*) calloc(n, sizeof(float));
	if( NULL == tmp )
	{
		return;
	}
	for( int i = 0; i < n; i++ )
	{
		tmp[0] = 0.0;
	}
	for( int i = 0; i < n*n; i++)
	{
		printf("A[%i] = %f \n", i, A[i]);
	}
	for( int i = 0; i < n; i++)
	{
		printf("d[%i] = %f \n", i, d[i]);
	}
	float rr = cblas_sdot( n, r, inc1, r, inc2);
	//
	// tmp = 1.0 A d + 1.0 tmp
	cblas_sgemv(CblasColMajor, CblasNoTrans, n, n, 1.0, A, n, d, 1, 1.0, tmp, 1);
	for( int i = 0; i < n; i++)
	{
		printf("tmp[%i] = %f \n", i, tmp[i]);
	}
	//
	//cblas_scopy(n, d, 1, tmp, 1);
	float dAd = cblas_sdot( n, d, inc1, tmp, inc2);
	float alpha = rr / dAd ;
	//
	// x_{k+1} = x_{k} + alpha d_{k}
	cblas_saxpy(n, alpha, d, 1, x, 1);
	printf("   rr = %f \n",  rr );
	printf("  dAd = %f \n",  dAd );
	printf("alpha = %f \n",  alpha );
	for (int i = 0; i < n; i++) {
		printf("x[%i] = %f \n", i, x[i]);
	}
	// r_{k+1} = r_{k} - aplha p_{k}
	cblas_saxpy(n, -alpha, tmp, 1, r, 1);
	if (NULL != tmp)
	{
		free(tmp);
		tmp = NULL;
	}
	for( int i = 0; i < n; i++)
	{
		printf("r[%i] = %f \n", i, r[i]);
	}
	// k + 1
	float rr_k1 = cblas_sdot( n,  r, inc1,  r, inc2);
	//
	if( rr_k1 < ERROR )
	{
		printf("rTr = %f, new r is sufficently small. \n",  rr_k1 );
		return;
	}
	//
	float beta = rr_k1 / rr ;
	printf(" beta = %f \n",  beta );
	//
	// p_{k+1} = r_{k} + beta p_{k}
	cblas_sscal( n, beta, d, 1 );
	cblas_saxpy(n, 1, r, 1, d, 1);
	for( int i = 0; i < n; i++)
	{
		printf("d[%i] = %f \n", i, d[i]);
	}
	for( int i = 0; i < n; i++)
	{
		printf("d[%i] = %f \n", i, d[i]);
	}
	return;
}

void testConjugate()
{
	int n =2;
	float error = 0.00001;
	float gg, gam, fp, dgg;
	float *x = (float*) calloc(n, sizeof(float));
	float *b = (float*) calloc(n, sizeof(float));
	float *A = (float*) calloc(n * n, sizeof(float));
	float *p = (float*) calloc(n, sizeof(float));
	float *r = (float*) calloc(n, sizeof(float));
        float *g = (float*)calloc(n  , sizeof(float));
	A[0] = 4;
	A[1] = 1;
	A[2] = 1;
	A[3] = 3;
	b[0] = 1;
	b[1] = 2;
	x[0] = 100;
	x[1] = 2000;
	cblas_sgemv(CblasColMajor, CblasNoTrans, n, n, -1.0, A, n, x, 1,  1.0, r, 1);
	// x_{k+1} = x_{k} + alpha d_{k}
	cblas_saxpy(n, 1.0, b, 1, r, 1);
	for( int i = 0; i < n; i++)
	{
		printf("r0[%d] = %f \n", i, r[i]);
	}
	for( int i = 0; i < n; i++)
	{
		g[i] =  r[i];
	}
	 conjugateGradient_sche( A, x,  g,  r, n);
	 printf("\n");
	 conjugateGradient_sche( A, x,  g,  r, n);
	 printf("\n");
	free(x);
	free(b);
	free(A);
	free(p);
	free(r);
	free(g);
	return;
}

int main(int argc, char* argv[]) {
 	 testConjugate();
	return 0;
}
```



编译运行结果

```python
r0[0] = -8.000000
r0[1] = -3.000000
A[0] = 4.000000
A[1] = 1.000000
A[2] = 1.000000
A[3] = 3.000000
d[0] = -8.000000
d[1] = -3.000000
tmp[0] = -35.000000
tmp[1] = -17.000000
   rr = 73.000000
  dAd = 331.000000
alpha = 0.220544
x[0] = 0.235650
x[1] = 0.338369
r[0] = -0.280967
r[1] = 0.749245
 beta = 0.008771
d[0] = -0.351138
d[1] = 0.722931
d[0] = -0.351138
d[1] = 0.722931
A[0] = 4.000000
A[1] = 1.000000
A[2] = 1.000000
A[3] = 3.000000
d[0] = -0.351138
d[1] = 0.722931
tmp[0] = -0.681620
tmp[1] = 1.817654
   rr = 0.640310
  dAd = 1.553380
alpha = 0.412204
x[0] = 0.090909
x[1] = 0.636364
r[0] = -0.000000
r[1] = 0.000000
rTr = 0.000000, new r is sufficently small.
```



