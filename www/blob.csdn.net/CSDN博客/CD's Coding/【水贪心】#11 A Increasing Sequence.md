# 【水贪心】#11 A. Increasing Sequence - CD's Coding - CSDN博客





2014年04月25日 01:31:24[糖果天王](https://me.csdn.net/okcd00)阅读数：504
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















A sequence *a*0, *a*1, ..., *a**t* - 1 is
 called increasing if *a**i* - 1 < *a**i* for
 each *i*: 0 < *i* < *t*.


You are given a sequence *b*0, *b*1, ..., *b**n* - 1 and
 a positive integer *d*. In each move you may choose one element of the given sequence and add *d* to
 it. What is the least number of moves required to make the given sequence increasing?




Input


The first line of the input contains two integer numbers *n* and *d* (2 ≤ *n* ≤ 2000, 1 ≤ *d* ≤ 106).
 The second line contains space separated sequence *b*0, *b*1, ..., *b**n* - 1 (1 ≤ *b**i* ≤ 106).




Output


Output the minimal number of moves needed to make the sequence increasing.




Sample test(s)




input
4 2
1 3 3 2




output
3















一道模拟题，其实直接模拟就出了，不过用一下除法来提升效率还是养成习惯的好。

贪心：遍历一次，每个当前数比自己前一个数大即可

C++：



```cpp
#include <algorithm>
#include <iostream>
#include <cstdio>
using namespace std;

int main()
{
	int n,d,cnt=0;
	scanf("%d%d",&n,&d);
	int val[n];
	for(int i=0;i<n;i++)
		scanf("%d",&val[i]);
	
	for(int i=1;i<n;i++)
	{
			if(val[i]>val[i-1])continue;
			else 
			{
				int temp=val[i-1]-val[i];
				val[i]+=(temp/d+1)*d;
				cnt+=(temp/d+1);
			}
		
	}
	printf("%d",cnt);
	return 0;
}
```














