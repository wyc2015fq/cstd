# 【水hash】#22 A. Second Order Statistics - CD's Coding - CSDN博客





2014年05月30日 14:21:09[糖果天王](https://me.csdn.net/okcd00)阅读数：627
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















Once Bob needed to find the second order statistics of a sequence of integer numbers. Lets choose each number from the sequence exactly once and sort them. The value on the second position is the second order statistics of the given sequence. In other words
 it is the smallest element strictly greater than the minimum. Help Bob solve this problem.




Input


The first input line contains integer *n* (1 ≤ *n* ≤ 100)
 — amount of numbers in the sequence. The second line contains *n*space-separated integer numbers — elements of the sequence. These numbers don't exceed 100
 in absolute value.




Output


If the given sequence has the second order statistics, output this order statistics, otherwise output NO.




Sample test(s)




input
4
1 2 2 -4




output
1




input
5
1 2 3 1 1




output
2















5min内秒掉的题呢^_^~~~

感觉良好~但是学不到东西……

用pair似乎比较省内存，但是才100，我就直接上hash表了



```cpp
#include<cstdio>
#include<string>
#include<cstring>
#include<iostream>
#include<algorithm>
using namespace std;

int mark[201]; 

int main()
{
	int n=0;
	scanf("%d",&n);
	memset(mark,0,sizeof(mark));
	for(int i=1;i<=n;i++)
	{
		int now=0;
		scanf("%d",&now);
		mark[now+100]=1;
	}
	int flag=0;
	for(int j=0;j<=200;j++)
	{
		if(flag==1&&mark[j]==1)
		{
			printf("%d",j-100); 
			return 0;
		}
		if(flag==0&&mark[j]==1) flag=1;
	}
	printf("NO");
	return 0;
}
```











