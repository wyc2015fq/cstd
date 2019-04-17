# 学习BLAS库 -- AXPY - cocoonyang的专栏 - CSDN博客





2017年02月12日 16:29:06[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1051








**函数语法: **

    saxpy(int n,float sa, float x[],int incx, float y[], int incy)

**功能**：

    AXPY是 BLAS 中 第1级函数。AXPY计算向量的数乘运算和向量的加法运算。



```
| Y |      | X |   | Y |
| Y |      | X |   | Y |
| Y | = sa | X | + | Y |
| Y |      | X |   | Y |
| Y |      | X |   | Y |
```







## Fortran语言版SAXPY

**源代码**：

```
SUBROUTINE SAXPY(N,SA,SX,INCX,SY,INCY)
*     .. Scalar Arguments ..
      REAL SA
      INTEGER INCX,INCY,N
*     ..
*     .. Array Arguments ..
      REAL SX(*),SY(*)
*     ..
*
*  Purpose
*  =======
*
*     SAXPY constant times a vector plus a vector.
*     uses unrolled loop for increments equal to one.
*     jack dongarra, linpack, 3/11/78.
*     modified 12/3/93, array(1) declarations changed to array(*)
*
*
*     .. Local Scalars ..
      INTEGER I,IX,IY,M,MP1
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC MOD
*     ..
      IF (N.LE.0) RETURN
      IF (SA.EQ.0.0) RETURN
      IF (INCX.EQ.1 .AND. INCY.EQ.1) GO TO 20
*
*        code for unequal increments or equal increments
*          not equal to 1
*
      IX = 1
      IY = 1
      IF (INCX.LT.0) IX = (-N+1)*INCX + 1
      IF (INCY.LT.0) IY = (-N+1)*INCY + 1
      DO 10 I = 1,N
          SY(IY) = SY(IY) + SA*SX(IX)
          IX = IX + INCX
          IY = IY + INCY
   10 CONTINUE
      RETURN
*
*        code for both increments equal to 1
*
*
*        clean-up loop
*
   20 M = MOD(N,4)
      IF (M.EQ.0) GO TO 40
      DO 30 I = 1,M
          SY(I) = SY(I) + SA*SX(I)
   30 CONTINUE
      IF (N.LT.4) RETURN
   40 MP1 = M + 1
      DO 50 I = MP1,N,4
          SY(I) = SY(I) + SA*SX(I)
          SY(I+1) = SY(I+1) + SA*SX(I+1)
          SY(I+2) = SY(I+2) + SA*SX(I+2)
          SY(I+3) = SY(I+3) + SA*SX(I+3)
   50 CONTINUE
      RETURN
      END
```









saxpy测试算例：

```cpp
/*
 * y <- alpha x + y
 void cblas_saxpy(
 OPENBLAS_CONST blasint n,
 OPENBLAS_CONST float alpha,
 OPENBLAS_CONST float *x,
 OPENBLAS_CONST blasint incx,
 float *y,
 OPENBLAS_CONST blasint incy);
 */
void axpy_test() {

	int n = 2;

	float *x = (float*) calloc(n, sizeof(float));
	float *b = (float*) calloc(n, sizeof(float));
	float *A = (float*) calloc(n * n, sizeof(float));
	float *p = (float*) calloc(n, sizeof(float));
	float *r = (float*) calloc(n, sizeof(float));

	A[0] = 4;
	A[1] = 1;
	A[2] = 1;
	A[3] = 3;

	b[0] = 0;
	b[1] = 0;

	x[0] = 2;
	x[1] = 1;

	for (int i = 0; i < n; i++) {
		printf("x[i] = %f \n", x[i]);
	}

	for (int i = 0; i < n; i++) {
		printf("b[i] = %f \n", b[i]);
	}

	cblas_saxpy(n, 2.0, x, 1, b, 1);

	for (int i = 0; i < n; i++) {
		printf("b[i] = %f \n", b[i]);
	}

	free(x);
	free(b);
	free(A);
	free(p);
	free(r);
}
```



测试结果：



```
x[i] = 2.000000
x[i] = 1.000000
b[i] = 0.000000
b[i] = 0.000000
b[i] = 4.000000
b[i] = 2.000000
```













