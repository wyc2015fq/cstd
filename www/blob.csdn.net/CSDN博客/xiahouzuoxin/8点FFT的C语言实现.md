# 8点FFT的C语言实现 - xiahouzuoxin - CSDN博客





2012年08月10日 16:54:09[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：9022








程序原理来源：程佩青的《数字信号处理教程》中按时间抽选的基-2 FFT蝶形图

说明：（1）处理的对象是复数，使用的是纯C语言

           （2）程序针对的是N=8的FFT

           （3）因为是按时间抽选，所以输入为倒位序，输出为正常序，因此输入数据要进行倒位序




**请注意后面的更正！！！**



【.c文件】

```cpp
#include "fft.h"

complex WN0 = {1,0};  
complex WN1 = {0.7109, -0.7109};
complex WN2 = {0,-1};
complex WN3 = {-0.7109,-0.7109};

complex ComplexMul(complex c1, complex c2)
{
	complex r;

	r.re = c1.re*c2.re - c1.im*c2.im;  //Re
	r.im = c1.re*c2.im + c1.im*c2.re;  //Im

	return r;
}

complex ComplexAdd(complex c1, complex c2)
{
	complex r;

	r.re = c1.re + c2.re;
	r.im = c1.im + c2.im;

	return r;
}

complex ReverseComplex(complex c)
{
	c.re = -c.re;
	c.im = -c.im;

	return c;
}

/*
 * 8点基-2时间FFT算法
 */
void fft(complex *x, complex *r)
{
	complex temp1[8];
	complex temp2[8];

	temp1[0] = x[0];
	temp1[1] = ComplexMul(x[1], WN0);
	temp1[2] = temp1[2] = x[2];
	temp1[3] = ComplexMul(x[3], WN0);
	temp1[4] = x[4];
	temp1[5] = ComplexMul(x[5], WN0);
	temp1[6] = x[6];
	temp1[7] = ComplexMul(x[7], WN0);

	temp2[0] = ComplexAdd(temp1[0],temp1[1]);
	temp2[1] = ComplexAdd(temp1[0],ReverseComplex(temp1[1]));
	temp2[2] = ComplexAdd(temp1[2],temp1[3]);
	temp2[3] = ComplexAdd(temp1[2],ReverseComplex(temp1[3]));
	temp2[4] = ComplexAdd(temp1[4],temp1[5]);
	temp2[5] = ComplexAdd(temp1[4],ReverseComplex(temp1[5]));
	temp2[6] = ComplexAdd(temp1[6],temp1[7]);
	temp2[7] = ComplexAdd(temp1[6],ReverseComplex(temp1[7]));

	temp2[2] = ComplexMul(temp2[2], WN0);
	temp2[3] = ComplexMul(temp2[3], WN2);
	temp2[6] = ComplexMul(temp2[6], WN0);
	temp2[7] = ComplexMul(temp2[7], WN2);

	temp1[0] = ComplexAdd(temp2[0],temp2[2]);
	temp1[1] = ComplexAdd(temp2[1],temp2[3]);
	temp1[2] = ComplexAdd(temp2[0],ReverseComplex(temp2[2]));
	temp1[3] = ComplexAdd(temp2[1],ReverseComplex(temp2[3]));
	temp1[4] = ComplexAdd(temp2[4],temp2[6]);
	temp1[5] = ComplexAdd(temp2[5],temp2[7]);
	temp1[6] = ComplexAdd(temp2[4],ReverseComplex(temp2[6]));
	temp1[7] = ComplexAdd(temp2[5],ReverseComplex(temp2[7]));

	temp1[4] = ComplexMul(temp1[4], WN0);
	temp1[5] = ComplexMul(temp1[5], WN1);
	temp1[6] = ComplexMul(temp1[6], WN2);
	temp1[7] = ComplexMul(temp1[7], WN3);

	r[0] = ComplexAdd(temp1[0], temp1[4]);
	r[1] = ComplexAdd(temp1[1], temp1[5]);
	r[2] = ComplexAdd(temp1[2], temp1[6]);
	r[3] = ComplexAdd(temp1[3], temp1[7]);
	r[4] = ComplexAdd(temp1[0], ReverseComplex(temp1[4]));
	r[5] = ComplexAdd(temp1[1], ReverseComplex(temp1[5]));
	r[6] = ComplexAdd(temp1[2], ReverseComplex(temp1[6]));
	r[7] = ComplexAdd(temp1[3], ReverseComplex(temp1[7]));
}

/*
 * 倒位序变换
 */
void BitReverse(complex *x, complex *r, uint8 n, uint8 l)
{
	uint8 i = 0;
	uint8 j = 0;
	uint8 stk = 0;

	for(i=0; i<n; i++)
	{
		stk = 0;
		j = 0;
		do
		{			
			stk |= (i>>(j++)) & 0x01;
			if(j<l)
			{
				stk <<= 1;
			}
		}while(j<l);

		if(stk < n)              /* 满足倒位序输出 */
		{
			r[stk] = x[i];
		}		
	}
}

int main(void)
{
	complex a[8] = {{1,0}, {1,0}, {2,1}, {2,1}, {2,1}, {2,1}, {1,0}, {1,0}};
	complex r[8];
	int i = 0;

	BitReverse(a, a, 8, 3);  //倒位序
	fft(a, r);
	for(i=0; i<8; i++)
	{
		printf("(%.4f) + j(%.4f)\n", r[i].re, r[i].im);
	}

	return 0;
}
```



【头文件“fft.h”】 

```cpp
#ifndef _FFT_H
#define _FFT_H

#include <stdio.h>

typedef unsigned int  uint16u;
typedef unsigned char uint8;
typedef unsigned long uint32;
typedef int           sint16;
typedef char          sint8;
typedef long          sint32;
typedef float         fp32;
typedef double        fp64;

#define FFT_N               (8)  //8点FFT                  

typedef struct Complex
{
	fp32 re;
	fp32 im;
} complex;  //复数  FFT中的一个节点单元

//声明
complex WN0;
complex WN1;
complex WN2;
complex WN3;

complex ComplexMul(complex c1, complex c2);
complex ComplexAdd(complex c1, complex c2);
complex ReverseComplex(complex c);
void fft(complex *x, complex *r);
void BitReverse(complex *x, complex *r, uint8 n, uint8 l);

#endif
```

![](https://img-my.csdn.net/uploads/201208/10/1344589564_8128.JPG)





**致歉及更正（2013.08.06）**

上面算法中的BitReverse调用有一些问题，上面的BitReverse函数中的参数x与r不能传入相同的参数，从而上面的结果是有误的。

更正如下：

我将BitReverse作了修改，使用了一个局部的temp变量，下面的代码能对形参x和r传入相同的参数，其它不用修改，



```cpp
/*
 * Bit Reverse
 * === Input ===
 * x : complex numbers
 * n : nodes of FFT. @N should be power of 2, that is 2^(*)
 * l : count by bit of binary format, @l=CEIL{log2(n)}
 * === Output ===
 * r : results after reversed.
 * Note: I use a local variable @temp that result @r can be set
 * to @x and won't overlap.
 */
static void BitReverse(complex *x, complex *r, int n, int l)
{
	int i = 0;
	int j = 0;
	short stk = 0;
	static complex *temp = 0;

	temp = (complex *)malloc(sizeof(complex) * n);
	if (!temp) {
		return;
	}

	for(i=0; i<n; i++) {
		stk = 0;
		j = 0;
		do {
			stk |= (i>>(j++)) & 0x01;
			if(j<l)
			{
				stk <<= 1;
			}
		}while(j<l);

		if(stk < n) {             /* 满足倒位序输出 */
			temp[stk] = x[i];
		}
	}
	/* copy @temp to @r */
	for (i=0; i<n; i++) {
		r[i] = temp[i];
	}
	free(temp);
}
```
最终正确的结果（使用matlab验证过无误）是

![](https://img-blog.csdn.net/20130806145635843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


更多详细进一步的有关FFT的信息参考[http://blog.csdn.net/xiahouzuoxin/article/details/9790455](http://blog.csdn.net/xiahouzuoxin/article/details/9790455)。

在此对误导的过的朋友表示歉意！





