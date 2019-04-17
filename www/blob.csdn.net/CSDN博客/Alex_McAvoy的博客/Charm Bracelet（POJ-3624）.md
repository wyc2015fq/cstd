# Charm Bracelet（POJ-3624） - Alex_McAvoy的博客 - CSDN博客





2018年06月29日 00:04:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：42
个人分类：[POJ																[动态规划——背包问题](https://blog.csdn.net/u011815404/article/category/7813163)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

Bessie has gone to the mall's jewelry store and spies a charm bracelet. Of course, she'd like to fill it with the best charms possible from theN (1 ≤ N ≤ 3,402) available charms. Each charm i in the supplied list has a weightWi (1 ≤ Wi ≤ 400), a 'desirability' factorDi (1 ≤ Di ≤ 100), and can be used at most once. Bessie can only support a charm bracelet whose weight is no more thanM (1 ≤ M ≤ 12,880).

Given that weight limit as a constraint and a list of the charms with their weights and desirability rating, deduce the maximum possible sum of ratings.



# **Input**

Line 1: Two space-separated integers: N and M

Lines 2..N+1: Line i+1 describes charm i with two space-separated integers:Wi and Di

# Output

Line 1: A single integer that is the greatest sum of charm desirabilities that can be achieved given the weight constraints

# Sample Input

**4 6**

**1 42 63 122 7**

# Sample Output

**23**

——————————————————————————————————————————————————————————————————————————————————————————

题意：给出 n 个物品，背包容量为 m，第 i 个物品重 w[i]，价值 d[i]，求背包能装入物品的最大价值。

思路：日常水题。。。01背包。。。套模版直接做。。。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 100001
#define MOD 123
#define E 1e-6
using namespace std;
int c[N];
int w[N],d[N];
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        memset(c,0,sizeof(c));
        for(int i=1;i<=n;i++)
            scanf("%d%d",&w[i],&d[i]);

        for(int i=1;i<=n;i++)
            for(int j=m;j>=w[i];j--)
                c[j]=max(c[j],c[j-w[i]]+d[i]);

        printf("%d\n",c[m]);
    }
    return 0;
}
```






