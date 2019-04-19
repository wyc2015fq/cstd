# OpenMP变量的私有与共享 - Koma Hub - CSDN博客
2017年11月30日 22:18:23[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：540
![](https://img-blog.csdn.net/20171130221709246?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```cpp
#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif
int gval = 8;//gval是共享的
void funcb(int *x, int *y, int z)//z是线程私有，z-i
{
	static int sv;//静态变量，共享的，在内存中只有一份，所以会引起冲突
	int u;//自变量，线程私有的
	u = (*y)*gval;//y本身是私有变量，其指向私有内存temp，
	*x = u + z;//x本身是私有的指针变量，但其指向的内存空间是共享的，私有指针指向共享内存
}
void funca(int *a, int n)//a和n是共享的
{
	int i;//循环控制变量，所以线程私有
	int cc = 9;//在并行化语句外声明，是共享的
	#pragma omp parallel for
	for(i = 0; i < n; i ++){
		
		int temp = cc;//并行化语句内部声明，是线程私有的
		funcb(&a[i], &temp, i);
	}
}
void main(int argc, char *argv[])
{
	int n = 10;
	int *a = malloc(sizeof(int)*n);
	funca(a,n);
}
```
函数funca调用了funcb，并且在函数funca中使用了OpenMP进行并行化
全局变gval是共享的
在funca函数的内部，变量i由于是循环控制变量，因此是线程私有的
cc在并行化语句外声明，是共享的
temp在循环并行化语句内部的自动变量，是线程私有的
输入的指针变量a以及n是共享的，都在循环并行化语句之外声明
在函数 funcb内部，静态变量sv是共享的，在程序内存空间中只有一份，因此，在这种使用方式下会引起数据冲突
变量u是自动变量，由于被并行线程调用，是线程私有的
参数x的本身是私有的指针变量，但是*x指向的内存空间是共享的，其实际参数即函数funca中的a数组
参数y的本身是私有的指针变量，指向的*y也是私有的，其实际内存空间即私有的temp占用的空间
数值参数z是线程私有的。
