# Game of Connections（POJ-2084） - Alex_McAvoy的博客 - CSDN博客





2018年08月14日 19:10:10[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：80
个人分类：[POJ																[—————组合数学—————](https://blog.csdn.net/u011815404/article/category/7900800)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

This is a small but ancient game. You are supposed to write down the numbers 1, 2, 3, . . . , 2n - 1, 2n consecutively in clockwise order on the ground to form a circle, and then, to draw some straight line segments to connect them into number pairs. Every number must be connected to exactly one another. 

And, no two segments are allowed to intersect. 

It's still a simple game, isn't it? But after you've written down the 2n numbers, can you tell me in how many different ways can you connect the numbers into pairs? Life is harder, right?

# **Input**

Each line of the input file will be a single positive number n, except the last line, which is a number -1.

You may assume that 1 <= n <= 100.

# Output

For each n, print in a single line the number of ways to connect the 2n numbers into pairs.

# Sample Input

**23-1 **

# Sample Output

**25**


题意：给出一个整数 n，现在 2n 个人围成一圈，两两相连，要求连接的线不能相交，求共有多少种连接方式

思路：

能看出来是卡特兰数的应用，但 n 最大到了 100，想到了要用高精度写，于是又跑去重新看之前学的高精模板，写了半天屁都没写出来。。。

搜了搜题解，学到了万进制这个神奇的东西，用来处理高精度是真的神奇。。。

下面是大佬的代码。。。

# Source Program

```cpp
#include <iostream>
#include <cstring>
#include <cstdio>
using namespace std;
#define MAX 100
#define BASE 10000
void multiply(int a[],int Max,int b)
{//大数乘法 
	int i,array=0;
	for(i=Max-1;i>=0;i--)
	{
		array += b*a[i];
		a[i] = array%BASE;
		array /= BASE;		
	}
}
 
void divide(int a[],int Max,int b)
{//大数除法 
	int i,div=0;
	for(i=0;i<Max;i++)
	{
		div =div * BASE + a[i];
		a[i] = div/b;
		div%=b;	
	}
}
 
 
int main()
{
	int i,j,n;
	int a[105][MAX];
	memset(a[1],0,sizeof(a[1]));
	for(i=2,a[1][MAX-1]=1;i<=100;i++)
	{
		memcpy(a[i],a[i-1],sizeof(a[i-1]));
		multiply(a[i],MAX,4*i-2);
		divide(a[i],MAX,i+1);
	}
	while(cin>>n,n!=-1)
	{
		for(i=0;i<MAX && a[n][i]==0;i++);
		cout<<a[n][i++];
		for(;i<MAX;i++)
			printf("%04d",a[n][i]);
		cout<<endl;
	}
	
	return 0;
}
```






