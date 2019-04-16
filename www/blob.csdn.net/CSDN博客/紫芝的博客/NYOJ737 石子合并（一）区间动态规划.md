# NYOJ737 石子合并（一）区间动态规划 - 紫芝的博客 - CSDN博客





2018年05月02日 10:58:36[紫芝](https://me.csdn.net/qq_40507857)阅读数：44








## 石子合并（一）

时间限制：1000 ms  |  内存限制：65535 KB

难度：3

描述

    有N堆石子排成一排，每堆石子有一定的数量。现要将N堆石子并成为一堆。合并的过程只能每次将相邻的两堆石子堆成一堆，每次合并花费的代价为这两堆石子的和，经过N-1次合并后成为一堆。求出总的代价最小值。

输入

有多组测试数据，输入到文件结束。

每组测试数据第一行有一个整数n，表示有n堆石子。

接下来的一行有n（0< n <200）个数，分别表示这n堆石子的数目，用空格隔开

输出

输出总代价的最小值，占单独的一行

样例输入

```
3
1 2 3
7
13 7 8 16 21 4 18
```

样例输出

```
9
239
```

来源

[经典问题](http://acm.nyist.edu.cn/JudgeOnline/search_result.php?source=%E7%BB%8F%E5%85%B8%E9%97%AE%E9%A2%98)

## 分析

**第一步：确定状态**

f[i,j]表示合并i到j的所有石子的得分

**第二步：确定状态转移方程**

f[i,j] = max(f[i][j]， f[i][k] + f[k + 1][j] + sum[i][j]) 

sum[i][j] 表示i到j的石子的价值和。边界：f[i][i] = 0

```cpp
#include<bits/stdc++.h>
using namespace std;
#define inf 0x3f3f3f3f
//#define inf 1<<20
const int maxn=210;
int n,a[maxn]; 
int  dp[maxn][maxn];//dp[i][j]表示从第i堆到第j堆合并的代价
int  sum[maxn][maxn];//表示石头的数量 
int main()
{
	ios::sync_with_stdio(0);
	while(cin>>n)
	{
		for(int i=1;i<=n;i++)
		cin>>a[i];
		memset(sum,0,sizeof(sum));
		//fill(dp[0],dp[0]+n*n,inf);//错误 
		fill(dp[0],dp[0]+maxn*maxn,inf);//fill填充量必须是常数 
		
/*
		for(int i=1;i<=n;i++)
		for(int j=1;j<=n;j++)
		//dp[i][j]=inf;
*/	
		for(int i=1;i<=n;i++)
		sum[i][i]=a[i],dp[i][i]=0;
		
		for(int len=1;len<n;len++){//区间长度 
			for(int i=1;i<=n&&i+len<=n;i++){//区间起点 
				int j=i+len;//区间终点
				for(int k=i;k<=j;k++)//用k来表示分割区间 
				{
					sum[i][j]=sum[i][k]+sum[k+1][j];
					dp[i][j]=min(dp[i][j],dp[i][k]+dp[k+1][j]+sum[i][j]);
				 } 
			}
		}
		cout<<dp[1][n]<<endl;
	}
	return 0;
}
```





