# B-number（HDU-3652） - Alex_McAvoy的博客 - CSDN博客





2018年09月04日 20:46:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：38
个人分类：[HDU																[动态规划——数位 DP](https://blog.csdn.net/u011815404/article/category/8008418)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

A wqb-number, or B-number for short, is a non-negative integer whose decimal form contains the sub- string "13" and can be divided by 13. For example, 130 and 2613 are wqb-numbers, but 143 and 2639 are not. Your task is to calculate how many wqb-numbers from 1 to n for a given integer n.

# **Input**

Process till EOF. In each line, there is one positive integer n(1 <= n <= 1000000000).

# Output

Print each answer in a single line.

# Sample Input

**13 100 200 1000**

# Sample Output

**1 1 2 2**

————————————————————————————————————————————————————

题意：给一个数 n，求 1~n 中有多少数 x 符合 x%13==0 且 x 中出现过 “13” 这个子串

思路：开三维数组，分别表示位数、mod、状态，数位DP即可

每次注意保留 sta 的状态：用 0 表示当前枚举的前一位不是 1，1 表示前一位是 1，2 表示之前枚举的数位里已有 13  

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 31
#define MOD 10007
#define E 1e-6
typedef long long LL;
using namespace std;
int bit[N];
int dp[N][N][N];
int dfs(int pos,int mod,int sta,bool limit)
{
    if(pos<=0)
        return sta==2&&!mod;
    if(!limit && dp[pos][mod][sta]!=-1)
        return dp[pos][mod][sta];

    int temp=0;
    int up=limit?bit[pos]:9;
    for(int i=0;i<=up;i++)
    {
        int sta_temp;
        int mod_temp=(mod*10+i)%13;
        if(sta==2||sta==1&&i==3)
            sta_temp=2;
        else if(i==1)
            sta_temp=1;
        else
            sta_temp=0;

        temp+=dfs(pos-1,mod_temp,sta_temp,limit&&i==up);
    }

    if(!limit)
        dp[pos][mod][sta]=temp;
    return temp;
}
int solve(int x)
{
    int pos=0;
    while(x)
    {
        bit[++pos]=x%10;
        x/=10;
    }
    return dfs(pos,0,0,true);
}
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF)
    {
        memset(dp,-1,sizeof(dp));
        printf("%d\n",solve(n));
    }
    return 0;
}
```






