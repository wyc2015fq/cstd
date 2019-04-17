# F(x)（HDU-4734） - Alex_McAvoy的博客 - CSDN博客





2018年09月05日 19:29:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：53
个人分类：[HDU																[动态规划——数位 DP](https://blog.csdn.net/u011815404/article/category/8008418)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

For a decimal number x with n digits (An An-1 An-2 ... A2 A1), we define its weight as F(x) = An * 2n-1 + An-1 * 2n-2 + ... + A2 * 2 + A1 * 1. Now you are given two numbers A and B, please calculate how many numbers are there between 0 and B, inclusive, whose weight is no more than F(A).

# **Input**

The first line has a number T (T <= 10000) , indicating the number of test cases.

For each test case, there are two numbers A and B (0 <= A,B < 10^9)

# Output

For every case,you should output "Case #t: " at first, without quotes. The t is the case number starting from 1. Then output the answer.

# Sample Input

**30 1001 105 100**

# Sample Output

**Case #1: 1Case #2: 2Case #3: 13**

————————————————————————————————————————————————————

题意：定义函数 F(x) = An * 2n-1 + An-1 * 2n-2 + … + A2 * 2 + A1 * 1，A 是十进制数，给出一个区间 [a,b]，求区间 [0,b] 内满足f(i)<=f(a) 的 i 的个数。

思路：用 dp[i][j] 表示 i 位比 j 小的数的个数，先计算出 F(a)，然后找出比 F(a) 小的 B 以内的数即可。

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
#define N 10001
#define MOD 10007
#define E 1e-6
typedef long long LL;
using namespace std;
int bit[N];
int dp[N][N];
int num;
int f(int x)
{
    if(x==0)
        return 0;
    int sum=f(x/10);
    return sum*2+(x%10);
}
int dfs(int pos,int sta,bool limit)
{
    if(pos==-1)
        return sta<=num;
    if(sta>num)
        return 0;

    if(!limit&&dp[pos][num-sta]!=-1)
        return dp[pos][num-sta];

    int res=0;
    int up=limit?bit[pos]:9;
    for(int i=0;i<=up;i++)
    {
        int next=sta+i*(1<<pos);
        res+=dfs(pos-1,next,limit&&(i==bit[pos]));
    }
    if(!limit)
        dp[pos][num-sta]=res;

    return res;
}
int solve(int x)
{
    int len=0;
    while(x)
    {
        bit[len++]=x%10;
        x/=10;
    }

    return dfs(len-1,0,true);
}
int main()
{
    int t;
    scanf("%d",&t);
    memset(dp,-1,sizeof(dp));

    int Case=1;
    while(t--)
    {
        int a,b;
        scanf("%d%d",&a,&b);

        num=f(a);
        printf("Case #%d: %d\n",Case++,solve(b));
    }
    return 0;
}
```






