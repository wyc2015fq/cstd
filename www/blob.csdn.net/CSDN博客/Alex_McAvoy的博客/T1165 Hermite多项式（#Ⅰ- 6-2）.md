# T1165	Hermite多项式（#Ⅰ- 6 - 2） - Alex_McAvoy的博客 - CSDN博客





2018年03月11日 23:03:34[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：1596








### 【题目描述】

用递归的方法求Hermite多项式的值

![](http://ybt.ssoier.cn:8088/pic/1165.gif)

对给定的x和正整数n，求多项式的值，并保留两位小数。

### 【输入】

给定的n和正整数x。


### 【输出】

多项式的值。


### 【输入样例】

1 2

### 【输出样例】

4.00

### 【源程序】

```cpp
#include<iostream>
#include<cstdio>
using namespace std;
double calculate(int n,int x);

int main()
{
	int n,x;
	double result;
	cin>>n>>x;
	result=calculate(n,x);
	printf("%.2lf\n",result);
	return 0;
}

double calculate(int n,int x)
{
	if(n==0)//判断是否到达递归边界n=0
		return 1;
	else if(n==1)//判断是否到达递归边界n=1
		return 2*x;
	else//否则继续递归
		return ( 2*x*calculate(n-1,x)-2*(n-1)*calculate(n-2,x) );
}
```




