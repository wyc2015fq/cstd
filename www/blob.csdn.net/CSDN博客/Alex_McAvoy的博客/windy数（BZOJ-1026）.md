# windy数（BZOJ-1026） - Alex_McAvoy的博客 - CSDN博客





2018年09月10日 22:55:33[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：36
个人分类：[动态规划——数位 DP																[其它 OJ](https://blog.csdn.net/u011815404/article/category/8820652)](https://blog.csdn.net/u011815404/article/category/8008418)








# Problem Description

windy定义了一种windy数。不含前导零且相邻两个数字之差至少为2的正整数被称为windy数。 windy想知道，在A和B之间，包括A和B，总共有多少个windy数？

# **Input**

包含两个整数，A B

# Output

一个整数

# Sample Input

**1 1025 50**

# Sample Output

**920**

————————————————————————————————————————————————————

思路：半天没写出来，只考虑数位DP的问题，没有想到与前缀和组合起来解题，具体可以看这一篇：[点击这里](https://blog.csdn.net/zz_ylolita/article/details/50754618)

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
int dp[N][N];
int solve(int x)
{
    int pos=0;
    while(x)
    {
        bit[++pos]=x%10;
        x/=10;
    }

    int res=0;
    for(int i=1;i<=bit[pos]-1;i++)
        res+=dp[pos][i];

    for(int i=pos-1;i>0;i--)
        for(int j=1;j<=9;j++)
            res+=dp[i][j];

    for(int i=pos-1;i>0;i--)
    {
        for(int j=0;j<=bit[i]-1;j++)
            if(abs(bit[i+1]-j)>=2)
                res+=dp[i][j];
        if(abs(bit[i+1]-bit[i])<2)
            break;
    }

    return res;
}
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        memset(dp,0,sizeof(dp));
        for(int i=0;i<=9;i++)
            dp[1][i]=1;
        for(int i=2;i<=10;i++)
            for(int j=0;j<=9;j++)
                for(int k=0;k<=9;k++)
                    if(abs(j-k)>=2)
                        dp[i][j]+=dp[i-1][k];

        printf("%d\n",solve(m+1)-solve(n));
    }
    return 0;
}
```






