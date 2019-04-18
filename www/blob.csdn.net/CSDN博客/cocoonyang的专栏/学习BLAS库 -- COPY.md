# 学习BLAS库 -- COPY - cocoonyang的专栏 - CSDN博客





2017年03月17日 22:17:42[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：238








函数语法: 


    XCOPY(  N,  X, INCX,  Y,  INCY)  

功能：

        COPY copies a vector X to a vector Y.

        BLAS level 1 function
参数：
      Input, int N, the number of elements in DX and DY.
      Input, double  X[*], the first vector.
      Input, int  incX, the increment between successive entries of  X.
      Output, double Y[*], the second vector.
      Input, int incY, the increment between successive entries of  Y. 




```cpp
void cblas_dcopy(const int N, const double *X, const int incX, double *Y,
		const int incY)
{

	if (N <= 0)
	{
		return;
	}
	//
	//        code for unequal increments or equal increments
	//          not equal to 1
	//
	int i;
	if ((incX != 1) || (incY != 1))
	{
		int ix = 1;
		int iy = 1;
		if (incX < 0)
		{
			ix = (1 - N) * incX + 1;
		}
		if (incY < 0)
		{
			iy = (1 - N) * incY + 1;
		}
		for ( i = 0; i < N; i++)
		{
			Y[iy] = X[ix];
			ix = ix + incX;
			iy = iy + incY;
		}
		return;
	}

	//
	//        code for both increments equal to 1
	//
	//
	//        clean-up loop
	//
	int m = (N % 7);
	if (0 != m)
	{
		for ( i = 0; i < m; i++)
		{
			Y[i] = X[i];
		}
		if (N < 7)
		{
			return;
		}
	}
	int mp1 = m ;
	for ( i = mp1; i < N; i += 7)
	{
		Y[i] = X[i];
		Y[i + 1] = X[i + 1];
		Y[i + 2] = X[i + 2];
		Y[i + 3] = X[i + 3];
		Y[i + 4] = X[i + 4];
		Y[i + 5] = X[i + 5];
		Y[i + 6] = X[i + 6];
	}
	return;

}
```





使用f2c转换出的dcopy函数代码：




```cpp
/*  -- translated by f2c (version 19940927).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int dcopy_(integer *n, doublereal *dx, integer *incx, 
	doublereal *dy, integer *incy)
{


    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i, m, ix, iy, mp1;


/*     copies a vector, x, to a vector, y.   
       uses unrolled loops for increments equal to one.   
       jack dongarra, linpack, 3/11/78.   
       modified 12/3/93, array(1) declarations changed to array(*)   


    
   Parameter adjustments   
       Function Body */
#define DY(I) dy[(I)-1]
#define DX(I) dx[(I)-1]


    if (*n <= 0) {
	return 0;
    }
    if (*incx == 1 && *incy == 1) {
	goto L20;
    }

/*        code for unequal increments or equal increments   
            not equal to 1 */

    ix = 1;
    iy = 1;
    if (*incx < 0) {
	ix = (-(*n) + 1) * *incx + 1;
    }
    if (*incy < 0) {
	iy = (-(*n) + 1) * *incy + 1;
    }
    i__1 = *n;
    for (i = 1; i <= *n; ++i) {
	DY(iy) = DX(ix);
	ix += *incx;
	iy += *incy;
/* L10: */
    }
    return 0;

/*        code for both increments equal to 1   


          clean-up loop */

L20:
    m = *n % 7;
    if (m == 0) {
	goto L40;
    }
    i__1 = m;
    for (i = 1; i <= m; ++i) {
	DY(i) = DX(i);
/* L30: */
    }
    if (*n < 7) {
	return 0;
    }
L40:
    mp1 = m + 1;
    i__1 = *n;
    for (i = mp1; i <= *n; i += 7) {
	DY(i) = DX(i);
	DY(i + 1) = DX(i + 1);
	DY(i + 2) = DX(i + 2);
	DY(i + 3) = DX(i + 3);
	DY(i + 4) = DX(i + 4);
	DY(i + 5) = DX(i + 5);
	DY(i + 6) = DX(i + 6);
/* L50: */
    }
    return 0;
} /* dcopy_ */
```













