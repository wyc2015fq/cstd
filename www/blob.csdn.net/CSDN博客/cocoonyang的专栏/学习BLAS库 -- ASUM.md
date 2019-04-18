# 学习BLAS库 -- ASUM - cocoonyang的专栏 - CSDN博客





2017年03月17日 21:57:49[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：732
个人分类：[BLAS																[C](https://blog.csdn.net/cocoonyang/article/category/6705806)](https://blog.csdn.net/cocoonyang/article/category/5715477)








函数语法: 


   XASUM( N,  X, INCX)

功能：


   ASUM 是 BLAS 中 第1层函数。ASUM函数计算一个向量中所有元素绝对值的累加和( Calculating summary of element absolute value of a vector).

参数：
    输入值:
     int N,    the number of entries in the vector.
         X[*], the vector to be examined.
     int incX, the increment between successive entries of X.
                     incX must not be negative.

    返回值:
       the sum of the absolute values of X.



## Fortran语言版DASUM
源代码：

```
DOUBLE PRECISION FUNCTION DASUM(N,DX,INCX)
*     .. Scalar Arguments ..
      INTEGER INCX,N
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION DX(*)
*     ..
*
*  Purpose
*  =======
*
*     takes the sum of the absolute values.
*     jack dongarra, linpack, 3/11/78.
*     modified 3/93 to return if incx .le. 0.
*     modified 12/3/93, array(1) declarations changed to array(*)
*
*
*     .. Local Scalars ..
      DOUBLE PRECISION DTEMP
      INTEGER I,M,MP1,NINCX
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC DABS,MOD
*     ..
      DASUM = 0.0d0
      DTEMP = 0.0d0
      IF (N.LE.0 .OR. INCX.LE.0) RETURN
      IF (INCX.EQ.1) GO TO 20
*
*        code for increment not equal to 1
*
      NINCX = N*INCX
      DO 10 I = 1,NINCX,INCX
          DTEMP = DTEMP + DABS(DX(I))
   10 CONTINUE
      DASUM = DTEMP
      RETURN
*
*        code for increment equal to 1
*
*
*        clean-up loop
*
   20 M = MOD(N,6)
      IF (M.EQ.0) GO TO 40
      DO 30 I = 1,M
          DTEMP = DTEMP + DABS(DX(I))
   30 CONTINUE
      IF (N.LT.6) GO TO 60
   40 MP1 = M + 1
      DO 50 I = MP1,N,6
          DTEMP = DTEMP + DABS(DX(I)) + DABS(DX(I+1)) + DABS(DX(I+2)) +
     +            DABS(DX(I+3)) + DABS(DX(I+4)) + DABS(DX(I+5))
   50 CONTINUE
   60 DASUM = DTEMP
      RETURN
      END
```






## C语言版DASUM
源代码：
```cpp
#include "math.h"
float cblas_dasum(const int N, const double *X, const int incX)
{
	double dtemp = 0.0;
	if ((N <= 0) || (incX <= 0))
	{
		printf("Error: in cblas_dasum(), Array number %6d less than zero.", N);
		return dtemp;
	}
	int i;
	if (incX != 1)
	{
		//--------------------------------------------
		//        code for increment not equal to 1
		//--------------------------------------------
		int nincX = N * incX;
		for ( i = 0; i < nincX; i += incX)
		{
			dtemp = dtemp + fabs(X[i]);
		}
		return dtemp;
	}
	//--------------------------------------------
	//        code for increment equal to 1
	//
	//
	//        clean-up loop
	//--------------------------------------------

	int m = N % 6;
	if (0 != m)
	{
		for ( i = 0; i < m; i++)
		{
			dtemp = dtemp + fabs(X[i]);
		}
		if (N < 6)
		{
			return dtemp;
		}
	}
	//----------------------------------
	//        code for m equal to 0
	//----------------------------------
	for ( i = m; i < N; i += 6)
	{
		dtemp += fabs(X[i]) + fabs(X[i + 1]) + fabs(X[i + 2]) + fabs(
				X[i + 3]) + fabs(X[i + 4]) + fabs(X[i + 5]);
	}
	return dtemp;
}
```




DASUM测试算例：

```cpp
void test_dasum()
{
	char functionName[] = "DASUM";
	printf("================================================ \n");
	printf("Testing BLAS library function -- %s \n", functionName);
	printf("  DASUM adds the absolute values of elements of a vector.\n");
	printf("\n");

	double result;
	int inc = 1;
	int n = 3;

	double X[] =
	{ 12.1, -12.1, 12.1 };
	result = cblas_dasum(n, X, inc);

	printf("Test array is \n");
	showDoubleArray(n, X);
	printf("%s  of the array is %f. \n", functionName, result );
	if (fabs(36.3 - result) < 0.0001)
	{
		printf("%s test SUCCESS.\n", functionName);
	}
	else
	{
		printf("[Fail] %s test failed.\n", functionName);
	}
	printf("\n");


	n = 6;
	double Y[] =
	{ 1.1, 1.1, 1.1, 1.1, 1.1, 1.1 };
	result = cblas_dasum(n, Y, inc);

	printf("Test array is \n");
	showDoubleArray(n, Y);
	printf("%s  of the array is %f. \n", functionName, result );
	if (fabs(6.6 - result) < 0.0001)
	{
		printf("%s test SUCCESS.\n", functionName);
	}
	else
	{
		printf("[Fail] %s test failed.\n", functionName);
	}
	printf("\n");

	// 12 elements
	n = 12;
	double longX[] =
	{ 1.1, 1.1, -1.1, 1.1, 1.1, -1.1, 1.1, 1.1, -1.1, 1.1, 1.1, 1.1 };
	result = cblas_dasum(n, longX, inc);


	printf("Test array is \n");
	showDoubleArray(n, longX);
	printf("%s  of the array is %f. \n", functionName, result );
	if (fabs(13.2 - result) < 0.0001)
	{
		printf("%s test SUCCESS.\n", functionName);
	}
	else
	{
		printf("[Fail] %s test failed.\n", functionName);
	}
	printf("\n");

}
```

测试结果：



```cpp
================================================ 
Testing BLAS library function -- DASUM 
  DASUM adds the absolute values of elements of a vector.

Test array is 
       0       12.100000
       1      -12.100000
       2       12.100000
DASUM  of the array is 36.300000. 
DASUM test SUCCESS.

Test array is 
       0        1.100000
       1        1.100000
       2        1.100000
       3        1.100000
       4        1.100000
       5        1.100000
DASUM  of the array is 6.600000. 
DASUM test SUCCESS.

Test array is 
       0        1.100000
       1        1.100000
       2       -1.100000
       3        1.100000
       4        1.100000
       5       -1.100000
       6        1.100000
       7        1.100000
       8       -1.100000
       9        1.100000
      10        1.100000
      11        1.100000
DASUM  of the array is 13.200000. 
DASUM test SUCCESS.
```







