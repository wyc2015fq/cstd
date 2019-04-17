# Bomb（HDU-3555） - Alex_McAvoy的博客 - CSDN博客





2018年09月04日 20:32:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42
个人分类：[HDU																[动态规划——数位 DP](https://blog.csdn.net/u011815404/article/category/8008418)](https://blog.csdn.net/u011815404/article/category/7923257)








# Problem Description

The counter-terrorists found a time bomb in the dust. But this time the terrorists improve on the time bomb. The number sequence of the time bomb counts from 1 to N. If the current number sequence includes the sub-sequence "49", the power of the blast would add one point.

Now the counter-terrorist knows the number N. They want to know the final points of the power. Can you help them?

# **Input**

The first line of input consists of an integer T (1 <= T <= 10000), indicating the number of test cases. For each test case, there will be an integer N (1 <= N <= 2^63-1) as the description.

The input terminates by end of file marker.

# Output

For each test case, output an integer indicating the final points of the power.

# Sample Input

**3150500**

# Sample Output

**0115**

————————————————————————————————————————————————————

题意：给一个数字 n，其范围在 1~2^63-1 之间，求 1~n 之间含有 49 的数字个数

思路：跟 不要62（HDU-2089）很像的一道模板题，注意数据精度即可

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
int a[N];
LL dp[N][N][2];
LL dfs(int pos,int pre,int sta,bool limit)
{
    if(pos==0)
    {
        if(sta)
            return 1;
        return 0;
    }

    if(!limit && dp[pos][pre][sta]!=-1)
        return dp[pos][pre][sta];

    LL temp=0;
    int up=limit?a[pos]:9;
    for(int i=up;i>=0;i--)
        temp+=dfs(pos-1,i==4,sta||(i==9&&pre),limit&&i==up);

    if(!limit)
        dp[pos][pre][sta]=temp;
    return temp;
}
LL solve(LL x)
{
    int pos=0;
    while(x)
    {
        a[++pos]=x%10;
        x/=10;
    }
    return dfs(pos,0,0,true);
}
int main()
{
    int t;
    scanf("%d",&t);
    memset(dp,-1,sizeof(dp));
    while(t--)
    {
        LL n;
        scanf("%lld",&n);
        printf("%lld\n",solve(n));
    }
    return 0;
}
```






