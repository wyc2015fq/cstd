# 学习BLAS库 -- DOT - cocoonyang的专栏 - CSDN博客





2015年08月06日 21:59:33[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：3589








函数语法: 

    XDOT( N, X, INCX, Y, INCY)

**功能**：

 Computes the dot product of two vectors. 

 dot <- X^{T}Y

**参数**：

     X: S(single float), D(double float), DS
     N: vector dimension
     X: vector x
INCX: spacing between elements of x
     Y: vector y
INCY: spacing between elements of y



## Fortran语言版DDOT
源代码：
```
DOUBLE PRECISION FUNCTION DDOT(N,DX,INCX,DY,INCY)
*     .. Scalar Arguments ..
      INTEGER INCX,INCY,N
*     ..
*     .. Array Arguments ..
      DOUBLE PRECISION DX(*),DY(*)
*     ..
*
*  Purpose
*  =======
*
*     forms the dot product of two vectors.
*     uses unrolled loops for increments equal to one.
*     jack dongarra, linpack, 3/11/78.
*     modified 12/3/93, array(1) declarations changed to array(*)
*
*
*     .. Local Scalars ..
      DOUBLE PRECISION DTEMP
      INTEGER I,IX,IY,M,MP1
*     ..
*     .. Intrinsic Functions ..
      INTRINSIC MOD
*     ..
      DDOT = 0.0d0
      DTEMP = 0.0d0
      IF (N.LE.0) RETURN
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
          DTEMP = DTEMP + DX(IX)*DY(IY)
          IX = IX + INCX
          IY = IY + INCY
   10 CONTINUE
      DDOT = DTEMP
      RETURN
*
*        code for both increments equal to 1
*
*
*        clean-up loop
*
   20 M = MOD(N,5)
      IF (M.EQ.0) GO TO 40
      DO 30 I = 1,M
          DTEMP = DTEMP + DX(I)*DY(I)
   30 CONTINUE
      IF (N.LT.5) GO TO 60
   40 MP1 = M + 1
      DO 50 I = MP1,N,5
          DTEMP = DTEMP + DX(I)*DY(I) + DX(I+1)*DY(I+1) +
     +            DX(I+2)*DY(I+2) + DX(I+3)*DY(I+3) + DX(I+4)*DY(I+4)
   50 CONTINUE
   60 DDOT = DTEMP
      RETURN
      END
```




## C语言版(f2c)DDOT
源代码：
```cpp
/*  -- translated by f2c (version 19940927).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

doublereal ddot_(integer *n, doublereal *dx, integer *incx, doublereal *dy, 
	integer *incy)
{


    /* System generated locals */
    integer i__1;
    doublereal ret_val;

    /* Local variables */
    static integer i, m;
    static doublereal dtemp;
    static integer ix, iy, mp1;


/*     forms the dot product of two vectors.   
       uses unrolled loops for increments equal to one.   
       jack dongarra, linpack, 3/11/78.   
       modified 12/3/93, array(1) declarations changed to array(*)   


    
   Parameter adjustments   
       Function Body */
#define DY(I) dy[(I)-1]
#define DX(I) dx[(I)-1]


    ret_val = 0.;
    dtemp = 0.;
    if (*n <= 0) {
	return ret_val;
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
	dtemp += DX(ix) * DY(iy);
	ix += *incx;
	iy += *incy;
/* L10: */
    }
    ret_val = dtemp;
    return ret_val;

/*        code for both increments equal to 1   


          clean-up loop */

L20:
    m = *n % 5;
    if (m == 0) {
	goto L40;
    }
    i__1 = m;
    for (i = 1; i <= m; ++i) {
	dtemp += DX(i) * DY(i);
/* L30: */
    }
    if (*n < 5) {
	goto L60;
    }
L40:
    mp1 = m + 1;
    i__1 = *n;
    for (i = mp1; i <= *n; i += 5) {
	dtemp = dtemp + DX(i) * DY(i) + DX(i + 1) * DY(i + 1) + DX(i + 2) * 
		DY(i + 2) + DX(i + 3) * DY(i + 3) + DX(i + 4) * DY(i + 4);
/* L50: */
    }
L60:
    ret_val = dtemp;
    return ret_val;
} /* ddot_ */
```




## C语言版DDOT
源代码：
```cpp
/*
 * cblas_ddot.c
 *
 * The program is a C version of ddot.
 * All right reserved.
 */
double cblas_ddot(const int N, const double *X, const int incX,
		const double *Y, const int incY)
{
	int i;
	int m;
	int mp1;
	double result = 0.0;
	if (N < 0)
	{
		return result;
	}


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
		for (i = 0; i < N; i++)
		{
			result += Y[iy] * X[ix];
			ix = ix + incX;
			iy = iy + incY;
		}
		return result;
	}

	m = (N % 5); 
	if (0 != m)
	{
		for (i = 0; i < m; i++)
		{
			result += Y[i] * X[i];
		}
		if (N < 5)
		{
			return result;
		}
	}

	mp1 = m ;
	for (i = mp1; i < N; i += 5)
	{
		result += (Y[i] * X[i] + Y[i + 1] * X[i + 1] + Y[i + 2] * X[i + 2]
				+ Y[i + 3] * X[i + 3] + Y[i + 4] * X[i + 4]);

	}
	return result;

}
```






## 汇编语言版(VC )DDOT
源代码：

```
_TEXT	SEGMENT
_iy$4493 = -32						; size = 4
_ix$4492 = -28						; size = 4
_m$ = -24						; size = 4
_mp1$ = -20						; size = 4
_result$ = -16						; size = 8
_i$ = -4						; size = 4
_N$ = 8							; size = 4
_X$ = 12						; size = 4
_incX$ = 16						; size = 4
_Y$ = 20						; size = 4
_incY$ = 24						; size = 4
_cblas_ddot PROC
; Line 15
	push	ebp
	mov	ebp, esp
	sub	esp, 32					; 00000020H
	push	esi
; Line 18
	mov	DWORD PTR _mp1$[ebp], 0
; Line 19
	fldz
	fstp	QWORD PTR _result$[ebp]
; Line 20
	cmp	DWORD PTR _N$[ebp], 0
	jge	SHORT $LN16@cblas_ddot
; Line 22
	fld	QWORD PTR _result$[ebp]
	jmp	$LN17@cblas_ddot
$LN16@cblas_ddot:
; Line 26
	cmp	DWORD PTR _incX$[ebp], 1
	jne	SHORT $LN14@cblas_ddot
	cmp	DWORD PTR _incY$[ebp], 1
	je	$LN15@cblas_ddot
$LN14@cblas_ddot:
; Line 28
	mov	DWORD PTR _ix$4492[ebp], 1
; Line 29
	mov	DWORD PTR _iy$4493[ebp], 1
; Line 30
	cmp	DWORD PTR _incX$[ebp], 0
	jge	SHORT $LN13@cblas_ddot
; Line 32
	mov	eax, 1
	sub	eax, DWORD PTR _N$[ebp]
	imul	eax, DWORD PTR _incX$[ebp]
	add	eax, 1
	mov	DWORD PTR _ix$4492[ebp], eax
$LN13@cblas_ddot:
; Line 34
	cmp	DWORD PTR _incY$[ebp], 0
	jge	SHORT $LN12@cblas_ddot
; Line 36
	mov	ecx, 1
	sub	ecx, DWORD PTR _N$[ebp]
	imul	ecx, DWORD PTR _incY$[ebp]
	add	ecx, 1
	mov	DWORD PTR _iy$4493[ebp], ecx
$LN12@cblas_ddot:
; Line 38
	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $LN11@cblas_ddot
$LN10@cblas_ddot:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$LN11@cblas_ddot:
	mov	eax, DWORD PTR _i$[ebp]
	cmp	eax, DWORD PTR _N$[ebp]
	jge	SHORT $LN9@cblas_ddot
; Line 40
	mov	ecx, DWORD PTR _iy$4493[ebp]
	mov	edx, DWORD PTR _Y$[ebp]
	mov	eax, DWORD PTR _ix$4492[ebp]
	mov	esi, DWORD PTR _X$[ebp]
	fld	QWORD PTR [edx+ecx*8]
	fmul	QWORD PTR [esi+eax*8]
	fadd	QWORD PTR _result$[ebp]
	fstp	QWORD PTR _result$[ebp]
; Line 41
	mov	ecx, DWORD PTR _ix$4492[ebp]
	add	ecx, DWORD PTR _incX$[ebp]
	mov	DWORD PTR _ix$4492[ebp], ecx
; Line 42
	mov	edx, DWORD PTR _iy$4493[ebp]
	add	edx, DWORD PTR _incY$[ebp]
	mov	DWORD PTR _iy$4493[ebp], edx
; Line 43
	jmp	SHORT $LN10@cblas_ddot
$LN9@cblas_ddot:
; Line 44
	fld	QWORD PTR _result$[ebp]
	jmp	$LN17@cblas_ddot
$LN15@cblas_ddot:
; Line 47
	mov	eax, DWORD PTR _N$[ebp]
	cdq
	mov	ecx, 5
	idiv	ecx
	mov	DWORD PTR _m$[ebp], edx
; Line 49
	cmp	DWORD PTR _m$[ebp], 0
	je	SHORT $LN4@cblas_ddot
; Line 51
	mov	DWORD PTR _i$[ebp], 0
	jmp	SHORT $LN7@cblas_ddot
$LN6@cblas_ddot:
	mov	edx, DWORD PTR _i$[ebp]
	add	edx, 1
	mov	DWORD PTR _i$[ebp], edx
$LN7@cblas_ddot:
	mov	eax, DWORD PTR _i$[ebp]
	cmp	eax, DWORD PTR _m$[ebp]
	jge	SHORT $LN5@cblas_ddot
; Line 53
	mov	ecx, DWORD PTR _i$[ebp]
	mov	edx, DWORD PTR _Y$[ebp]
	mov	eax, DWORD PTR _i$[ebp]
	mov	esi, DWORD PTR _X$[ebp]
	fld	QWORD PTR [edx+ecx*8]
	fmul	QWORD PTR [esi+eax*8]
	fadd	QWORD PTR _result$[ebp]
	fstp	QWORD PTR _result$[ebp]
; Line 54
	jmp	SHORT $LN6@cblas_ddot
$LN5@cblas_ddot:
; Line 55
	cmp	DWORD PTR _N$[ebp], 5
	jge	SHORT $LN4@cblas_ddot
; Line 57
	fld	QWORD PTR _result$[ebp]
	jmp	$LN17@cblas_ddot
$LN4@cblas_ddot:
; Line 61
	mov	ecx, DWORD PTR _m$[ebp]
	mov	DWORD PTR _mp1$[ebp], ecx
; Line 63
	mov	edx, DWORD PTR _mp1$[ebp]
	mov	DWORD PTR _i$[ebp], edx
	jmp	SHORT $LN3@cblas_ddot
$LN2@cblas_ddot:
	mov	eax, DWORD PTR _i$[ebp]
	add	eax, 5
	mov	DWORD PTR _i$[ebp], eax
$LN3@cblas_ddot:
	mov	ecx, DWORD PTR _i$[ebp]
	cmp	ecx, DWORD PTR _N$[ebp]
	jge	SHORT $LN1@cblas_ddot
; Line 66
	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR _Y$[ebp]
	mov	ecx, DWORD PTR _i$[ebp]
	mov	esi, DWORD PTR _X$[ebp]
	fld	QWORD PTR [eax+edx*8]
	fmul	QWORD PTR [esi+ecx*8]
	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR _Y$[ebp]
	mov	ecx, DWORD PTR _i$[ebp]
	mov	esi, DWORD PTR _X$[ebp]
	fld	QWORD PTR [eax+edx*8+8]
	fmul	QWORD PTR [esi+ecx*8+8]
	faddp	ST(1), ST(0)
	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR _Y$[ebp]
	mov	ecx, DWORD PTR _i$[ebp]
	mov	esi, DWORD PTR _X$[ebp]
	fld	QWORD PTR [eax+edx*8+16]
	fmul	QWORD PTR [esi+ecx*8+16]
	faddp	ST(1), ST(0)
	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR _Y$[ebp]
	mov	ecx, DWORD PTR _i$[ebp]
	mov	esi, DWORD PTR _X$[ebp]
	fld	QWORD PTR [eax+edx*8+24]
	fmul	QWORD PTR [esi+ecx*8+24]
	faddp	ST(1), ST(0)
	mov	edx, DWORD PTR _i$[ebp]
	mov	eax, DWORD PTR _Y$[ebp]
	mov	ecx, DWORD PTR _i$[ebp]
	mov	esi, DWORD PTR _X$[ebp]
	fld	QWORD PTR [eax+edx*8+32]
	fmul	QWORD PTR [esi+ecx*8+32]
	faddp	ST(1), ST(0)
	fadd	QWORD PTR _result$[ebp]
	fstp	QWORD PTR _result$[ebp]
; Line 71
	jmp	$LN2@cblas_ddot
$LN1@cblas_ddot:
; Line 73
	fld	QWORD PTR _result$[ebp]
$LN17@cblas_ddot:
; Line 75
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	0
_cblas_ddot ENDP
_TEXT	ENDS
```

编译参数：

```cpp
cl main.c
```






## 汇编语言版(VC, 计算速度优化 )DDOT
源代码：
```
;	COMDAT _cblas_ddot
_TEXT	SEGMENT
_i$ = -12						; size = 4
_result$ = -8						; size = 8
tv1489 = 8						; size = 4
tv683 = 8						; size = 4
_N$ = 8							; size = 4
_X$ = 12						; size = 4
_incX$ = 16						; size = 4
_Y$ = 20						; size = 4
_incY$ = 24						; size = 4
_cblas_ddot PROC					; COMDAT
; Line 15
	sub	esp, 12					; 0000000cH
; Line 19
	xorps	xmm1, xmm1
	push	edi
; Line 20
	mov	edi, DWORD PTR _N$[esp+12]
	movsd	QWORD PTR _result$[esp+16], xmm1
	test	edi, edi
	jns	SHORT $LN16@cblas_ddot
; Line 22
	fldz
	pop	edi
; Line 75
	add	esp, 12					; 0000000cH
	ret	0
$LN16@cblas_ddot:
; Line 26
	mov	edx, DWORD PTR _incX$[esp+12]
	push	ebx
	push	ebp
	push	esi
	mov	esi, DWORD PTR _incY$[esp+24]
	cmp	edx, 1
	jne	$LN14@cblas_ddot
	cmp	esi, 1
	jne	$LN14@cblas_ddot
; Line 49
	mov	ebp, DWORD PTR _Y$[esp+24]
	mov	eax, 1717986919				; 66666667H
	imul	edi
	sar	edx, 1
	mov	eax, edx
	shr	eax, 31					; 0000001fH
	add	eax, edx
	lea	eax, DWORD PTR [eax+eax*4]
	mov	ebx, edi
	sub	ebx, eax
	je	$LN32@cblas_ddot
; Line 51
	xor	esi, esi
	cmp	ebx, 4
	jl	SHORT $LC26@cblas_ddot
	mov	edx, DWORD PTR _X$[esp+24]
	lea	ecx, DWORD PTR [edx+24]
; Line 53
	sub	edx, ebp
	mov	DWORD PTR tv683[esp+24], edx
	lea	edx, DWORD PTR [ebx-4]
	shr	edx, 2
	inc	edx
	lea	esi, DWORD PTR [edx*4]
	mov	DWORD PTR _i$[esp+28], esi
	mov	esi, DWORD PTR tv683[esp+24]
	lea	eax, DWORD PTR [ebp+8]
	npad	9
$LL27@cblas_ddot:
	movsd	xmm0, QWORD PTR [eax-8]
	mulsd	xmm0, QWORD PTR [ecx-24]
	addsd	xmm0, xmm1
	movsd	xmm1, QWORD PTR [esi+eax]
	mulsd	xmm1, QWORD PTR [eax]
	addsd	xmm0, xmm1
	movsd	xmm1, QWORD PTR [eax+8]
	mulsd	xmm1, QWORD PTR [ecx-8]
	addsd	xmm0, xmm1
	movsd	xmm1, QWORD PTR [eax+16]
	mulsd	xmm1, QWORD PTR [ecx]
	add	eax, 32					; 00000020H
	add	ecx, 32					; 00000020H
	dec	edx
	addsd	xmm1, xmm0
	jne	SHORT $LL27@cblas_ddot
	mov	esi, DWORD PTR _i$[esp+28]
	movsd	QWORD PTR _result$[esp+28], xmm1
$LC26@cblas_ddot:
; Line 51
	cmp	esi, ebx
	jge	SHORT $LN31@cblas_ddot
	mov	edx, DWORD PTR _X$[esp+24]
	mov	ecx, ebx
	sub	edx, ebp
	lea	eax, DWORD PTR [ebp+esi*8]
	sub	ecx, esi
	npad	5
$LC7@cblas_ddot:
; Line 53
	movsd	xmm0, QWORD PTR [eax+edx]
	mulsd	xmm0, QWORD PTR [eax]
	add	eax, 8
	dec	ecx
	addsd	xmm1, xmm0
	jne	SHORT $LC7@cblas_ddot
	movsd	QWORD PTR _result$[esp+28], xmm1
$LN31@cblas_ddot:
; Line 55
	cmp	edi, 5
; Line 57
	jl	$LN28@cblas_ddot
$LN32@cblas_ddot:
; Line 63
	cmp	ebx, edi
	jge	$LN28@cblas_ddot
	mov	eax, DWORD PTR _X$[esp+24]
	lea	esi, DWORD PTR [eax+ebx*8+24]
	sub	eax, ebp
	sub	edi, ebx
	mov	DWORD PTR tv683[esp+24], eax
	dec	edi
	mov	eax, -858993459				; cccccccdH
	mul	edi
	shr	edx, 2
	lea	ecx, DWORD PTR [ebp+ebx*8+8]
	inc	edx
	npad	6
$LL3@cblas_ddot:
; Line 66
	mov	eax, DWORD PTR tv683[esp+24]
	movsd	xmm0, QWORD PTR [ecx-8]
	mulsd	xmm0, QWORD PTR [esi-24]
	movsd	xmm2, QWORD PTR [eax+ecx]
	mulsd	xmm2, QWORD PTR [ecx]
	addsd	xmm0, xmm2
	movsd	xmm2, QWORD PTR [ecx+8]
	mulsd	xmm2, QWORD PTR [esi-8]
	addsd	xmm0, xmm2
	movsd	xmm2, QWORD PTR [ecx+16]
	mulsd	xmm2, QWORD PTR [esi]
	addsd	xmm0, xmm2
	movsd	xmm2, QWORD PTR [ecx+24]
	mulsd	xmm2, QWORD PTR [esi+8]
	add	ecx, 40					; 00000028H
	add	esi, 40					; 00000028H
	dec	edx
	addsd	xmm0, xmm2
	addsd	xmm1, xmm0
	jne	SHORT $LL3@cblas_ddot
; Line 40
	pop	esi
	pop	ebp
	movsd	QWORD PTR _result$[esp+20], xmm1
; Line 44
	fld	QWORD PTR _result$[esp+20]
	pop	ebx
	pop	edi
; Line 75
	add	esp, 12					; 0000000cH
	ret	0
$LN14@cblas_ddot:
; Line 28
	mov	eax, 1
; Line 29
	mov	ecx, eax
; Line 30
	test	edx, edx
	jns	SHORT $LN13@cblas_ddot
; Line 32
	sub	eax, edi
	imul	eax, edx
	inc	eax
$LN13@cblas_ddot:
; Line 34
	test	esi, esi
	jns	SHORT $LN12@cblas_ddot
; Line 36
	sub	ecx, edi
	imul	ecx, esi
	inc	ecx
$LN12@cblas_ddot:
; Line 38
	mov	ebx, DWORD PTR _Y$[esp+24]
	xor	ebp, ebp
	cmp	edi, 4
	jl	SHORT $LC29@cblas_ddot
	lea	ebp, DWORD PTR [edi-4]
	shr	ebp, 2
	inc	ebp
	mov	DWORD PTR tv1489[esp+24], ebp
	add	ebp, ebp
	add	ebp, ebp
	mov	DWORD PTR _i$[esp+28], ebp
	npad	4
$LL30@cblas_ddot:
; Line 40
	mov	ebp, DWORD PTR _X$[esp+24]
	movsd	xmm0, QWORD PTR [ebp+eax*8]
	mulsd	xmm0, QWORD PTR [ebx+ecx*8]
	addsd	xmm0, xmm1
; Line 41
	add	eax, edx
	movsd	xmm1, QWORD PTR [ebp+eax*8]
; Line 42
	add	ecx, esi
	mulsd	xmm1, QWORD PTR [ebx+ecx*8]
	addsd	xmm1, xmm0
	add	eax, edx
	movsd	xmm0, QWORD PTR [ebp+eax*8]
	add	ecx, esi
	mulsd	xmm0, QWORD PTR [ebx+ecx*8]
	add	eax, edx
	addsd	xmm0, xmm1
	movsd	xmm1, QWORD PTR [ebp+eax*8]
	add	ecx, esi
	mulsd	xmm1, QWORD PTR [ebx+ecx*8]
	add	eax, edx
	add	ecx, esi
	dec	DWORD PTR tv1489[esp+24]
	addsd	xmm1, xmm0
	jne	SHORT $LL30@cblas_ddot
; Line 40
	mov	ebp, DWORD PTR _i$[esp+28]
	movsd	QWORD PTR _result$[esp+28], xmm1
$LC29@cblas_ddot:
; Line 38
	cmp	ebp, edi
	jge	SHORT $LN28@cblas_ddot
	add	esi, esi
	add	edx, edx
	add	esi, esi
	add	edx, edx
	lea	ecx, DWORD PTR [ebx+ecx*8]
	mov	ebx, DWORD PTR _X$[esp+24]
	add	esi, esi
	add	edx, edx
	lea	eax, DWORD PTR [ebx+eax*8]
	sub	edi, ebp
	npad	4
$LC11@cblas_ddot:
; Line 40
	movsd	xmm0, QWORD PTR [ecx]
	mulsd	xmm0, QWORD PTR [eax]
; Line 41
	add	eax, edx
; Line 42
	add	ecx, esi
	dec	edi
	addsd	xmm1, xmm0
	jne	SHORT $LC11@cblas_ddot
; Line 40
	movsd	QWORD PTR _result$[esp+28], xmm1
$LN28@cblas_ddot:
; Line 44
	fld	QWORD PTR _result$[esp+28]
	pop	esi
	pop	ebp
	pop	ebx
	pop	edi
; Line 75
	add	esp, 12					; 0000000cH
	ret	0
_cblas_ddot ENDP
_TEXT	ENDS
```

启动编译器优化选项：
`cl main.c /FA /O2 /arch:SSE2`





## 汇编语言版(GotoBLAS2库)DDOT
源代码：
```
/*********************************************************************/
/*                                                                   */
/*             Optimized BLAS libraries                              */
/*                     By Kazushige Goto <kgoto@tacc.utexas.edu>     */
/*                                                                   */
/* Copyright (c) The University of Texas, 2009. All rights reserved. */
/* UNIVERSITY EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES CONCERNING  */
/* THIS SOFTWARE AND DOCUMENTATION, INCLUDING ANY WARRANTIES OF      */
/* MERCHANTABILITY, FITNESS FOR ANY PARTICULAR PURPOSE,              */
/* NON-INFRINGEMENT AND WARRANTIES OF PERFORMANCE, AND ANY WARRANTY  */
/* THAT MIGHT OTHERWISE ARISE FROM COURSE OF DEALING OR USAGE OF     */
/* TRADE. NO WARRANTY IS EITHER EXPRESS OR IMPLIED WITH RESPECT TO   */
/* THE USE OF THE SOFTWARE OR DOCUMENTATION.                         */
/* Under no circumstances shall University be liable for incidental, */
/* special, indirect, direct or consequential damages or loss of     */
/* profits, interruption of business, or related expenses which may  */
/* arise from use of Software or Documentation, including but not    */
/* limited to those resulting from defects in Software and/or        */
/* Documentation, or loss or inaccuracy of data of any kind.         */
/*********************************************************************/

#define ASSEMBLER
#include "common.h"

#define STACK	12
#define ARGS     0
	
#define STACK_N		 4 + STACK + ARGS(%esp)
#define STACK_X		 8 + STACK + ARGS(%esp)
#define STACK_INCX	12 + STACK + ARGS(%esp)
#define STACK_Y		16 + STACK + ARGS(%esp)
#define STACK_INCY	20 + STACK + ARGS(%esp)

#define N	%ebx
#define X	%esi
#define INCX	%ecx
#define Y	%edi
#define INCY	%edx


	PROLOGUE

	pushl	%edi
	pushl	%esi
	pushl	%ebx

	PROFCODE

#if defined(F_INTERFACE_GFORT) || defined(F_INTERFACE_G95)
	EMMS
#endif

	movl	STACK_N,     N
	movl	STACK_X,     X
	movl	STACK_INCX,  INCX
	movl	STACK_Y,     Y
	movl	STACK_INCY,  INCY

#ifdef F_INTERFACE
	movl	(N),N
	movl	(INCX),INCX
	movl	(INCY),INCY
#endif

	leal	(, INCX, SIZE), INCX	
	leal	(, INCY, SIZE), INCY	

	fldz
	fldz
	fldz
	fldz

	cmpl	$SIZE, INCX
	jne	.L14
	cmpl	$SIZE, INCY
	jne	.L14

	movl	N, %eax
	sarl	$2,   %eax
	jle	.L15
	ALIGN_3

.L16:
	FLD	0 * SIZE(X)
	FMUL	0 * SIZE(Y)
	faddp	%st,%st(1)
	FLD	1 * SIZE(X)
	FMUL	1 * SIZE(Y)
	faddp	%st,%st(2)
	FLD	2 * SIZE(X)
	FMUL	2 * SIZE(Y)
	faddp	%st,%st(3)
	FLD	3 * SIZE(X)
	FMUL	3 * SIZE(Y)
	faddp	%st,%st(4)
	addl	$4 * SIZE, X
	addl	$4 * SIZE, Y
	decl	%eax
	jg	.L16
	ALIGN_3

.L15:
	movl	N, %eax
	andl	$3,   %eax
	jle	.L27
	ALIGN_3

.L22:
	FLD	(X)
	addl	$SIZE, X
	FMUL	(Y)
	addl	$SIZE, Y
	faddp	%st,%st(1)
	decl	%eax
	jg	.L22

	jmp	.L27
	ALIGN_3

.L14:
#ifdef F_INTERFACE
	testl	INCX, INCX
	jge	.L28

	movl	N, %eax
	decl	%eax
	imull	INCX, %eax
	subl	%eax, X
	ALIGN_3

.L28:
	testl	INCY, INCY
	jge	.L29

	movl	N, %eax
	decl	%eax
	imull	INCY, %eax
	subl	%eax, Y
	ALIGN_3
.L29:
#endif
	movl	N, %eax
	sarl	$2,   %eax
	jle	.L30
	ALIGN_3

.L31:
	FLD	(X)
	addl	INCX, X
	FMUL	(Y)
	addl	INCY, Y
	faddp	%st,%st(1)

	FLD	(X)
	addl	INCX, X
	FMUL	(Y)
	addl	INCY, Y
	faddp	%st,%st(2)

	FLD	(X)
	addl	INCX, X
	FMUL	(Y)
	addl	INCY, Y
	faddp	%st,%st(3)

	FLD	(X)
	addl	INCX, X
	FMUL	(Y)
	addl	INCY, Y
	faddp	%st,%st(4)

	decl	%eax
	jg	.L31
	ALIGN_3

.L30:
	movl	N, %eax
	andl	$3,   %eax
	jle	.L27
	ALIGN_3

.L37:
	FLD	(X)
	addl	INCX, X
	FMUL	(Y)
	addl	INCY, Y
	faddp	%st, %st(1)
	decl	%eax
	jg	.L37
	ALIGN_3

.L27:
	faddp	%st,%st(2)
	faddp	%st,%st(2)
	faddp	%st,%st(1)

	popl	%ebx
	popl	%esi
	popl	%edi
	ret

	EPILOGUE
```





**使用方法**：

测试DDOT函数

```cpp
unsigned int longGetCycleCount()   
{   
    __asm RDTSC   
} 

void test_ddot()  
{  
    int i;
    int n = 19; 
    int inc1 = 1;   
    int inc2 = 1; 
    double result;  
    double X[] =  
    {  1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1 };  
  
    double Y[] =  
    { 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1, 12.1 }; 
 
    char functionName[] = "ddot";  
    printf("================================================ \n");  
    printf("Testing BLAS library function -- %s \n", functionName);  
 
    for( i = 0; i < 100000; i++ )
    {
        result = cblas_ddot(n, X, inc1, Y, inc2);  
    }
  
    printf("%s  of the arraies is %f \n", functionName, result);  

    if (fabs(12.1 - result) < 0.0001)  
    {  
        printf("%s test SUCCESS.\n", functionName);  
    }  
    else  
    {  
        printf("[Fail] %s test failed.\n", functionName);  
    } 

  
    printf("\n");  
    return;  
}  


int main( void )
{
   
    unsigned int start,end;
    double cost;

    start = longGetCycleCount();
    test_ddot() ; 
    end = longGetCycleCount();

    cost =  (double)  (end-start) / 2900000000 ;   
    printf("%f \n",cost); 

    return 0;
}
```


未开启速度优化选项编译得到的运算时间 为0.005 秒左右。 

开启速度优化选项编译得到的运算时间 为0.0005 秒左右。





开启编译器优化选项可使计算效率提高10倍左右。




**测试环境**：

Visual Studio Express 2010
Operating System：Windows 7, 64-bit 
CPU：Intel Core(TM) CPU 2.90GHz
Memory： 4.00GB
Hard disk: 500G





**参考文献**

[1] http://www.applied-mathematics.net/miniSSEL1BLAS/miniSSEL1BLAS.pdf
[2]  https://software.intel.com/en-us/articles/use-intriniscs/
[3]  Hadi Brais.  Compilers - What Every Programmer Should Know About Compiler Optimizations.  https://msdn.microsoft.com/en-us/magazine/dn904673.aspx.    February 2015.
[4]  Koushik Ghosh. Writing Efficient C and C Code Optimization. http://www.codeproject.com/Articles/6154/Writing-Efficient-C-and-C-Code-Optimization.  26 Feb 2004. 
[5] http://sci.tuomastonteri.fi/programming/sse 
[6] https://www.kernel.org/pub/linux/kernel/people/geoff/cell/ps3-linux-docs/CellProgrammingTutorial/BasicsOfSIMDProgramming.html







