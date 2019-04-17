# T1163	阿克曼(Ackmann)函数（#Ⅰ- 6 - 2） - Alex_McAvoy的博客 - CSDN博客





2018年03月11日 22:56:23[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：1448








### 【题目描述】

阿克曼(Ackmann)函数A(m，n)中，m，n定义域是非负整数(m≤3,n≤10)，函数值定义为：
    akm(m,n) = n+1;         (m=0时)
    akm(m,n) = akm(m-1,1);  (m>0,n=0时)
    akm(m,n) = akm(m-1,akm(m, n-1)); （m,n>0时)


### 【输入】

输入m和n。


### 【输出】

函数值

### 【输入样例】

2 3

### 【输出样例】

9

### 【源程序】

```cpp
#include<iostream>
using namespace std;
int ack(int m,int n);

int main()
{
	int m,n;
	cin>>m>>n;
	cout<<ack(m,n)<<endl;
	return 0;
}

int ack(int m,int n)
{
	if(m==0)//判断是否到达递归边界m=0
		return (n+1);
	else if(n==0)//判断是否到达递归边界n=0
		return (ack(m-1,1));
	else//否则继续递归
		return (ack(m-1,ack(m,n-1)));
}
```




