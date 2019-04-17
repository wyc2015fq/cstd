# 【VS2013】windows多线程编程_并行评测 - CD's Coding - CSDN博客





2014年11月19日 22:12:54[糖果天王](https://me.csdn.net/okcd00)阅读数：3386










以数值积分的方法计算Pi的值，采用Win32 API来实现程序的并行化。

步骤： 

1、用Microsoft Visual Studio 2013创建控制台项目WinPi 

2、创建WinPi.cpp

将代码加入到程序中合适的位置，输出计算时间。






3、 编译执行， Pi的值为:*  3.141592654  *

The time ofcalculationwas *1.062000(calc 12345678times)*seconds

4 、将WinPi.cpp程序修改为WindowsThreads并行程序。

*（1）    *分析代码中的并行域是： *x = (i + 0.5)*step;*

*       sum += 4.0 / (1.0 + x*x);*

（2）定义线程执行的函数。

函数原型为DWORDWINAPI 函数名(LPVOIDp);  *见源代码*

（3）提取并行域代码，作为（2）中定义的函数的函数体

（4）生成多个线程调用（3）中的函数

（5）解决线程间的同步和互斥。



5 、采用临界区的方法进行必要的互斥。

要多线程实现的话，我们主要是把for循环的计算过程分到几个线程中去，由于每次计算我们都要更新sum的值，就有可能发生一个线程已经更新了sum的值，而另一个线程读到的还是旧的sum值，所以在这里我们使用临界区，把sum放到临界区中，这样一次只能有一个线程访问和修改sum的值。



【临界区主要代码】

CRITICAL_SECTIONg_cs;

InitializeCriticalSection(&g_cs);

EnterCriticalSection(&g_cs);

临界区计算

LeaveCriticalSection(&g_cs);

DeleteCriticalSection(&g_cs);



6、编译执行，Pi的值为：*3.141592654*

The time ofcalculationwas  1.375   seconds



7、不同线程数，相同计算量，计算时间对比分析。

|线程数|时间|线程数|时间|
|----|----|----|----|
|1|1.343|8|1.296|
|2|1.328|9|1.312|
|3|1.343|10|1.328|
|4|1.375|12|1.344|
|5|1.328|15|1.359|
|6|1.313|20|1.312|
|7|1.328|25|1.343|
|30|1.328| | |


8、线程数相同，计算量不同，计算时间对比分析。（调整num_step可调整计算量）

|计算量|时间|
|----|----|
|1500000|0|
|15000000|0.125|
|1.50E+09|1.312|
|15000000000|13.196|









```cpp
// WinPi.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <time.h>

int i = 0;
double step, pi;
clock_t start, stop, allstart, allstop;
double x, sum = 0.0;
const int numThreads = 5;//5-threads calc
static long num_steps = 1500000000;
CRITICAL_SECTION g_cs;

DWORD WINAPI calc(LPVOID p)
{
	int cnt = 0;
	printf("Now a new Thread is calc-ing\n");
	start = clock();
	EnterCriticalSection(&g_cs);
	for (; cnt<num_steps / numThreads; cnt++, i++)
	{
		x = (i + 0.5)*step;
		sum += 4.0 / (1.0 + x*x);
	}
	LeaveCriticalSection(&g_cs);
	stop = clock();
	printf("the time of calc was %f s\n", ((double)(stop - start) / 1000.0));
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hThread[numThreads];
	int tNum[50];
	step = 1.0 / (double)num_steps;
	allstart = clock();
	InitializeCriticalSection(&g_cs);
	for (i = 0; i < numThreads; i++)
	{
		tNum[i] = i;
		hThread[i] = CreateThread(NULL, 0, calc, (LPVOID)&tNum[i], 0, NULL);
	}
	WaitForMultipleObjects(numThreads, hThread, TRUE, INFINITE);

	allstop = clock();
	printf("Now step=%f ,sum=%f \n", step, sum);
	pi = step*sum;
	printf("Pi=%12.9f\n", pi);
	printf("the time of total was %f s\n", ((double)(allstop - allstart) / 1000.0));
	DeleteCriticalSection(&g_cs);
	system("pause");
	return 0;
}
```











