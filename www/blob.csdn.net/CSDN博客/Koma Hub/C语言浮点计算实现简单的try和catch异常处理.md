# C语言浮点计算实现简单的try和catch异常处理 - Koma Hub - CSDN博客
2018年12月08日 16:29:17[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：79
个人分类：[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)
```cpp
/*	file name: 		fexcepts.h
 *	author:			Rong Tao
 *	create time:	2018.11.18
 * 	
 */
#ifndef _T_MATH_FLOAT_EXCEPT_H__
#define _T_MATH_FLOAT_EXCEPT_H__
#include <math.h>
#include "types.h"
/*some Examining the FPU status word*/
#include <fenv.h>
#define T_FE_ALL_EXCEPT FE_ALL_EXCEPT
#define T_FE_INEXACT FE_INEXACT
#define T_FE_DIVBYZERO FE_DIVBYZERO
#define T_FE_UNDERFLOW FE_UNDERFLOW
#define T_FE_OVERFLOW FE_OVERFLOW
#define T_FE_INVALID FE_INVALID
/*int feclearexcept (int excepts)*/
#define t_feclearexcept(excepts) feclearexcept(excepts)
/*int feraiseexcept (int excepts)*/
#define t_feraiseexcept(excepts) feraiseexcept(excepts)
/*int fetestexcept (int excepts)*/
#define t_fetestexcept(excepts) fetestexcept(excepts)
/*int fegetexceptflag (fexcept t *flagp, int excepts)*/
#define t_fegetexceptflag(flagp, excepts) fegetexceptflag(flagp, excepts)
/*int fesetexceptflag (const fexcept t *flagp, int excepts)*/
#define t_fesetexceptflag(flagp, excepts) fesetexceptflag(flagp, excepts)
/*int fegetround (void)*/
#define t_fegetround() fegetround()
/*int fesetround (int round)*/
#define t_fesetround(round) fesetround(round)
/*int fegetenv (fenv t *envp)*/
#define t_fegetenv(envp) fegetenv(envp)
/*int feholdexcept (fenv t *envp)*/
#define t_feholdexcept(envp) feholdexcept(envp)
/*int fesetenv (const fenv t *envp)*/
#define t_fesetenv(envp) fesetenv(envp)
/*int feupdateenv (const fenv t *envp)*/
#define t_feupdateenv(envp) feupdateenv(envp)
/*int feenableexcept (int excepts)*/
#define t_feenableexcept(excepts) feenableexcept(excepts)
/*int fedisableexcept (int excepts)*/
#define t_fedisableexcept(excepts) fedisableexcept(excepts)
/*int fegetexcept (int excepts)*/
#define t_fegetexcept(excepts) fegetexcept(excepts)
/**
 *	just like try catch throw of java or c# etc,
 *	but is not like that.
 *	Author:	Rongtao
 *	Time:	2018.11.18
 */
#define t_float_try \
	t_feclearexcept(T_FE_ALL_EXCEPT); 
#define t_float_catch(excepts) \
	do{\
		if(T_FE_INEXACT&t_fetestexcept (excepts)){\
			fprintf(stdout, "T_FE_INEXACT\n"); \
			exit(0); \
		}\
		if(T_FE_DIVBYZERO&t_fetestexcept (excepts)){\
			fprintf(stdout, "T_FE_DIVBYZERO\n"); \
			exit(0); \
		}\
		if(T_FE_UNDERFLOW&t_fetestexcept (excepts)){\
			fprintf(stdout, "T_FE_UNDERFLOW\n"); \
			exit(0); \
		}\
		if(T_FE_OVERFLOW&t_fetestexcept (excepts)){\
			fprintf(stdout, "T_FE_OVERFLOW\n"); \
			exit(0); \
		}\
		if(T_FE_INVALID&t_fetestexcept (excepts)){\
			fprintf(stdout, "T_FE_INVALID\n"); \
			exit(0); \
		}\
	}while(0)
	
#define t_float_catchall(...) t_float_catch(T_FE_ALL_EXCEPT)
#define t_float_throw(excepts) t_feraiseexcept(excepts)
/* use the exception macro like this:
	t_float_try{
	t_float_throw(T_FE_OVERFLOW);
	}t_float_catchall(T_FE_ALL_EXCEPT);
*/
#endif /*<_T_MATH_FLOAT_EXCEPT_H__>*/
```
