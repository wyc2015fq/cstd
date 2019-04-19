# 将Fortran程序转化成大写[cwp] - Koma Hub - CSDN博客
2018年12月17日 22:26:40[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：119
个人分类：[C/C++																[SeismicUnix																[Fortran](https://blog.csdn.net/Rong_Toa/article/category/8540068)](https://blog.csdn.net/Rong_Toa/article/category/7156242)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
我是代码的搬运工
```cpp
/* Copyright (c) Colorado School of Mines, 2011.*/
/* All rights reserved.                       */
/*********************** self documentation **********************/
/*
 * UPFORT - change Fortran programs to upper case, preserving strings
 *
 * Usage:   upfort < infile.f > outfile.f
 *
 * Reverse of: downfort
 *
 */
/**************** end self doc ********************************/
/*
 * Author: Brian Sumner
 */
#include "cwp.h"
#include <ctype.h>
int
main()
{
	register int c;
	while ((c = getchar()) != EOF) {
		if (c=='\n') putchar(c);
		else if (c=='*' || c=='C' || c=='c' || c=='D') {
			putchar(c);
			while((c = getchar()) != '\n') putchar(c);
			putchar(c);
		} else {
			putchar(c);
			while ((c = getchar()) != '\n') {
				if (c != '\'') putchar(islower(c) ? toupper(c)
								  : c);
				else {
					putchar(c);
					if ((c = getchar()) != '(') {
						putchar(c);
						while((c = getchar()) != '\'')
							putchar(c);
						putchar(c);
					} else {
						putchar(c);
						while((c = getchar()) != '\'')
				 			putchar(islower(c)
								 ? toupper(c)
								 : c);
						putchar(c);
					}
				}
			}
			putchar(c);
		}
	}
	fclose(stdout);
	return EXIT_SUCCESS;
}
```
