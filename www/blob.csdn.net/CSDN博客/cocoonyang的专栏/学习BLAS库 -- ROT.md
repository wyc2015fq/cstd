# 学习BLAS库 -- ROT - cocoonyang的专栏 - CSDN博客





2017年03月17日 22:10:18[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：432
个人分类：[C																[BLAS](https://blog.csdn.net/cocoonyang/article/category/5715477)](https://blog.csdn.net/cocoonyang/article/category/6705806)








函数语法:


     XROT( N, X, INCX, Y, INCY, C, S)

功能：


     Apply a plane rotation

     BLAS level 1 function

参数：


     X: S(single float), D(double float), C(complex), Z(complex*16) 
     N: vector dimension
     X: vector x
INCX: spacing between elements of x
     Y: vector y
INCY: spacing between elements of y
     C: multiplier
     S: multiplier 







## Fortran语言版DROT
源代码：

```
*> \brief \b DROT
*
*  =========== DOCUMENTATION ===========
*
* Online html documentation available at
*            http://www.netlib.org/lapack/explore-html/
*
*  Definition:
*  ===========
*
*       SUBROUTINE DROT(N,DX,INCX,DY,INCY,C,S)
*
*       .. Scalar Arguments ..
*       DOUBLE PRECISION C,S
*       INTEGER INCX,INCY,N
*       ..
*       .. Array Arguments ..
*       DOUBLE PRECISION DX(*),DY(*)
*       ..
*
*
*> \par Purpose:
*  =============
*>
*> \verbatim
*>
*>    DROT applies a plane rotation.
*> \endverbatim
*
*  Arguments:
*  ==========
*
*> \param[in] N
*> \verbatim
*>          N is INTEGER
*>         number of elements in input vector(s)
*> \endverbatim
*>
*> \param[in,out] DX
*> \verbatim
*>          DX is DOUBLE PRECISION array, dimension ( 1 + ( N - 1 )*abs( INCX ) )
*> \endverbatim
*>
*> \param[in] INCX
*> \verbatim
*>          INCX is INTEGER
*>         storage spacing between elements of DX
*> \endverbatim
*>
*> \param[in,out] DY
*> \verbatim
*>          DY is DOUBLE PRECISION array, dimension ( 1 + ( N - 1 )*abs( INCY ) )
*> \endverbatim
*>
*> \param[in] INCY
*> \verbatim
*>          INCY is INTEGER
*>         storage spacing between elements of DY
*> \endverbatim
*>
*> \param[in] C
*> \verbatim
*>          C is DOUBLE PRECISION
*> \endverbatim
*>
*> \param[in] S
*> \verbatim
*>          S is DOUBLE PRECISION
*> \endverbatim
*
*  Authors:
*  ========
*
*> \author Univ. of Tennessee
*> \author Univ. of California Berkeley
*> \author Univ. of Colorado Denver
*> \author NAG Ltd.
*
*> \date November 2017
*
*> \ingroup double_blas_level1
*
*> \par Further Details:
*  =====================
*>
*> \verbatim
*>
*>     jack dongarra, linpack, 3/11/78.
*>     modified 12/3/93, array(1) declarations changed to array(*)
*> \endverbatim
*>
*  =====================================================================
      SUBROUTINE DROT(N,DX,INCX,DY,INCY,C,S)
*
*  -- Reference BLAS level1 routine (version 3.8.0) --
*  -- Reference BLAS is a software package provided by Univ. of Tennessee,    --
*  -- Univ. of California Berkeley, Univ. of Colorado Denver and NAG Ltd..--
*     November 2017
*
*     .. Scalar Arguments ..
      DOUBLE PRECISION C,S
      INTEGER INCX,INCY,N
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION DX(*),DY(*)
*     ..
*
*  =====================================================================
*
*     .. Local Scalars ..
      DOUBLE PRECISION DTEMP
      INTEGER I,IX,IY
*     ..
      IF (N.LE.0) RETURN
      IF (INCX.EQ.1 .AND. INCY.EQ.1) THEN
*
*       code for both increments equal to 1
*
         DO I = 1,N
            DTEMP = C*DX(I) + S*DY(I)
            DY(I) = C*DY(I) - S*DX(I)
            DX(I) = DTEMP
         END DO
      ELSE
*
*       code for unequal increments or equal increments not equal
*         to 1
*
         IX = 1
         IY = 1
         IF (INCX.LT.0) IX = (-N+1)*INCX + 1
         IF (INCY.LT.0) IY = (-N+1)*INCY + 1
         DO I = 1,N
            DTEMP = C*DX(IX) + S*DY(IY)
            DY(IY) = C*DY(IY) - S*DX(IX)
            DX(IX) = DTEMP
            IX = IX + INCX
            IY = IY + INCY
         END DO
      END IF
      RETURN
      END
```






## C语言版DROT
源代码：
```cpp
/*
 *
 * cblas_drot.c
 *
 * The program is a C version of drot.
 *
 *  Purpose
 *  =======
 *
 *     applies a plane rotation.
 *
 *
 *     Written by Chunfeng Yang
 *     Date:  05/11/2014
 *
 *
 *     Test:  Chunfeng Yang
 *     Date: 05 November, 2014
 *
 *
 */
void cblas_drot(const int N, double *DX, const int INCX, double *DY,
		const int INCY, const double C, const double S)
{
	if (N <= 0)
	{
		return;
	}

	double DTEMP;
	int IX;
	int IY;
	int I;
	if ((INCX != 1) || (INCY != 1))
	{
		/*
		 *       code for unequal increments or equal increments not equal
		 *         to 1
		 */
		IX = 1;
		IY = 1;
		if (INCX < 0)
		{
			IX = (-N + 1) * INCX + 1;
		}
		if (INCY < 0)
		{
			IY = (-N + 1) * INCY + 1;
		}
		for (I = 0; I < N; I++)
		{
			DTEMP = C * DX[IX] + S * DY[IY];
			DY[IY] = C * DY[IY] - S * DX[I];
			DX[IX] = DTEMP;
			IX = IX + INCX;
			IY = IY + INCY;
		}
		return;
	}
	/*
	 *       code for both increments equal to 1
	 */
	for (I = 0; I < N; I++)
	{
		DTEMP = C * DX[I] + S * DY[I];
		DY[I] = C * DY[I] - S * DX[I];
		DX[I] = DTEMP;
	}
	return;
}
```











