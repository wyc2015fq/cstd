# Corn Fields（POJ-3254） - Alex_McAvoy的博客 - CSDN博客





2018年08月22日 10:34:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：61
个人分类：[POJ																[动态规划——状压 DP](https://blog.csdn.net/u011815404/article/category/7955244)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Farmer John has purchased a lush new rectangular pasture composed of M by N (1 ≤ M ≤ 12; 1 ≤ N ≤ 12) square parcels. He wants to grow some yummy corn for the cows on a number of squares. Regrettably, some of the squares are infertile and can't be planted. Canny FJ knows that the cows dislike eating close to each other, so when choosing which squares to plant, he avoids choosing squares that are adjacent; no two chosen squares share an edge. He has not yet made the final choice as to which squares to plant.

Being a very open-minded man, Farmer John wants to consider all possible options for how to choose the squares for planting. He is so open-minded that he considers choosing no squares as a valid option! Please help Farmer John determine the number of ways he can choose the squares to plant.

# **Input**

Line 1: Two space-separated integers: M and N 

Lines 2..M+1: Line i+1 describes row i of the pasture with N space-separated integers indicating whether a square is fertile (1 for fertile, 0 for infertile)

# Output

Line 1: One integer: the number of ways that FJ can choose the squares modulo 100,000,000.

# Sample Input

**2 31 1 10 1 0**

# Sample Output

**9**

————————————————————————————————————————————————————

题意：给出一个大小为 n*m 的地，1 可以种植，0 不可以种植，两株植物不能相邻种植，求所有种植方法数，并取模 100000000

思路：状压 DP 入门题

每一行的情况就可通过一个二进制数 state 来存储，其范围是：0 ~ 1<< state，用 dp[i][state] 来表示第 i 行状态为 state 的情况下满足条件的数目，则状态转移方程为：dp[i][state] += dp[i-1][pre_state]，且 state 和 pre_state 必须满足所给条件，即左右上下均不相邻，答案为最后一行所有状态的情况相加。

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
#define N 1001
#define MOD 100000000
#define E 1e-6
#define LL long long
using namespace std;
int n,m;
int a[15][15];
int dp[15][1<<15];

bool check(int x,int state)
{
    if(state&(state<<1))
        return false;
    for(int i=1;i<=m;i++)
        if(!a[x][i])
            if( (1<<(m-i)&state)!=0 )
                return false;
    return true;
}

int main()
{
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        memset(dp,0,sizeof(dp));
        for(int i=1;i<=n;i++)
            for(int j=1;j<=m;j++)
                cin>>a[i][j];

        dp[0][0]=1;
        LL res=0;
        for(int i=1;i<=n;i++)
        {
            for(int j=0;j<(1<<m);j++)
            {
                if(check(i,j))//判断当前行满足条件的state
                {
                    for(int k=0;k<(1<<m);k++)//枚举上一行的pre_state进行更新
                    {
                        if(!(k&j))
                            dp[i][j]+=dp[i-1][k];
                    }
                    if(i==n)
                        res=(res+dp[i][j]) % MOD;
                }
            }
        }
        printf("%lld\n",res);
    }
    return 0;
}
```






