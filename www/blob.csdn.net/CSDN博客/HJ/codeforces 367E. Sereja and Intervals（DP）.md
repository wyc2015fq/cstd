# codeforces 367E. Sereja and Intervals（DP） - HJ - CSDN博客
2018年03月07日 21:00:37[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：111
个人分类：[====ACM====																[--------动态规划---------																[---------OJ---------																[codeforce																[基础DP](https://blog.csdn.net/feizaoSYUACM/article/category/7238027)](https://blog.csdn.net/feizaoSYUACM/article/category/6353637)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/7238093)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
题目链接 [http://codeforces.com/problemset/problem/367/E](http://codeforces.com/problemset/problem/367/E)
解题思路：
 题意大意是给出一个区间，在这个区间内选n个L和R值，使得这n个L，R不能存在包含关系，问一共有多少种方法。
设dp[i][l][r] 表示当前区间长度为i的时候左节点有l个，右节点有r个的方法数
那么状态转移方程可以分为四种：
①第i个数可能为左边界
②第i个数可能为右边界
③第i个数可能既为左边界又为右边界
④第i个数可能既不是左边界又不是右边界
注意特殊情况，当i==x的时候,状态转移只能是①或③
[](http://codeforces.com/problemset/problem/367/E)
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
typedef long long LL;
const int mod=1000000007;
LL dp[2][500][500];
LL add(LL a,LL b)
{
	LL ans=(a+b)%mod;
	return ans;
}
int main()
{
	LL n,m,x;
	while(cin>>n>>m>>x)
	{
		if(n>m)
		{
			printf("0\n");
			continue;
		}
		memset(dp,0,sizeof(dp));
		dp[0][0][0]=1;
		LL now=0;
		for(int i=1;i<=m;i++)
		{
			now^=1;
			for(int l=0;l<=n;l++)
			  for(int r=0;r<=n;r++)
			    dp[now][l][r]=0;
			for(int l=0;l<=n;l++)
			  for(int r=0;r<=l;r++)
		      {
		    	  dp[now][l+1][r]=add(dp[now][l+1][r],dp[now^1][l][r]);
		    	  dp[now][l+1][r+1]=add(dp[now][l+1][r+1],dp[now^1][l][r]);
		    	  if(i!=x)
		    	  {
		    	  	  dp[now][l][r+1]=add(dp[now][l][r+1],dp[now^1][l][r]);
		    	  	  dp[now][l][r]=add(dp[now][l][r],dp[now^1][l][r]);
		    	  }
		      }
		}
		LL ans=dp[now][n][n]%mod;
		for(int i=1;i<=n;i++)
		  ans=(ans*i)%mod;
		printf("%I64d\n",ans);
	}
	return 0;
}
```
