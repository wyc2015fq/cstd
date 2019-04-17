# Cow Contest（POJ-3660 ） - Alex_McAvoy的博客 - CSDN博客





2018年05月29日 20:07:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：73
个人分类：[POJ																[图论——Floyd](https://blog.csdn.net/u011815404/article/category/8312424)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

N (1 ≤ N ≤ 100) cows, conveniently numbered 1..N, are participating in a programming contest. As we all know, some cows code better than others. Each cow has a certain constant skill rating that is unique among the competitors.

The contest is conducted in several head-to-head rounds, each between two cows. If cow A has a greater skill level than cow B (1 ≤ A ≤ N; 1 ≤ B ≤ N; A ≠ B), then cow A will always beat cow B.

Farmer John is trying to rank the cows by skill level. Given a list the results of M (1 ≤ M ≤ 4,500) two-cow rounds, determine the number of cows whose ranks can be precisely determined from the results. It is guaranteed that the results of the rounds will not be contradictory.

# **Input**

Line 1: Two space-separated integers: N and M

Lines 2..M+1: Each line contains two space-separated integers that describe the competitors and results (the first integer, A, is the winner) of a single round of competition: A and B

# Output

Line 1: A single integer representing the number of cows whose ranks can be determined

# Sample Input

**5 5**

**4 34 23 21 22 5**

# Sample Output

**2**


题意：给出两头牛之间的竞争的结果(a,b)，a赢b，确定有多少头牛能够确定其排名。

思路：对于一头牛，如果想确定其排名，那么就是在明确的知道比它高的有几只、低的有几只，且前后加起来正好是 n-1 只时，可以确定他的排名。

所给的关系可以看做是一条路，只需判断任意两点是否连通，再确定一个点的入度和出度相加是否等于 n-1，因此可以判断这个点是否为所求的点。

因此本题实质是判断一个有向无权图的任意两点是否导通，即求图的传递闭包，使用 Floyd 即可。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 101
#define MOD 2520
#define E 1e-12
using namespace std;
int g[N][N];
int degree[N][N];
int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF)
    {
        memset(g,0,sizeof(g));
        for(int i=0;i<m;i++)
        {
            int x,y;
            scanf("%d%d",&x,&y);
            g[x][y]=1;
        }
        for(int k=1;k<=n;k++)
            for(int i=1;i<=n;i++)
                for(int j=1;j<=n;j++)
                    if(g[i][k]&&g[k][j])
                        g[i][j]=1;

        
        memset(degree,0,sizeof(degree));
        for(int i=1;i<=n;i++)//计算点的度数
        {
            for(int j=1;j<=n;j++)if(i!=j)
            {
                if(mp[i][j])
                {
                    degree[i]++;
                    degree[j]++;
                }
            }
        }
            
        int cnt=0;
        for(int i=1;i<=n;i++)
            if(degree[i]==n-1)
                cnt++;
        printf("%d\n",cnt);

    }
    return 0;
}
```






