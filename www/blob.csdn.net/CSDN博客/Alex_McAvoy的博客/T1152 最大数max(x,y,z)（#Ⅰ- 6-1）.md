# T1152	最大数max(x,y,z)（#Ⅰ- 6 - 1） - Alex_McAvoy的博客 - CSDN博客





2018年03月05日 22:13:34[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：2241








### 【题目描述】



已知:

m=max(a,b,c)max(a+b,b,c)×max(a,b,b+c)

输入a,b,c，求m。把求三个数的最大数max(x,y,z)分别定义成函数和过程来做。


### 【输入】

输入a,b,c。


### 【输出】

求m，保留到小数点后三位。


### 【输入样例】

1 2 3

### 【输出样例】

0.200

### 【源程序】

```cpp
#include<iostream>
#include<cstdio>
using namespace std;
int max(int a,int b,int c);

int main()
{
	int a,b,c;
	double m;

	cin>>a>>b>>c;//输入a、b、c
	m=1.0*max(a,b,c)/(max(a+b,b,c)*max(a,b,b+c));//计算m
	printf("%.3lf\n",m);
	return 0;
}

int max(int a,int b,int c)//求三个数中的最大值
{
	int temp=a;
	if(a<b)
		temp=b;
	if(temp<c)
		temp=c;
	return temp;

}
```




