# Cow Hurdles（POJ-3615 ） - Alex_McAvoy的博客 - CSDN博客





2018年06月06日 17:15:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：64
个人分类：[POJ																[图论——Floyd](https://blog.csdn.net/u011815404/article/category/8312424)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Farmer John wants the cows to prepare for the county jumping competition, so Bessie and the gang are practicing jumping over hurdles. They are getting tired, though, so they want to be able to use as little energy as possible to jump over the hurdles.

Obviously, it is not very difficult for a cow to jump over several very short hurdles, but one tall hurdle can be very stressful. Thus, the cows are only concerned about the height of the tallest hurdle they have to jump over.

The cows' practice room has N (1 ≤ N ≤ 300) stations, conveniently labeled 1..N. A set of M (1 ≤ M ≤ 25,000) one-way paths connects pairs of stations; the paths are also conveniently labeled 1..M. Path i travels from station Si to station Ei and contains exactly one hurdle of height Hi (1 ≤ Hi ≤ 1,000,000). Cows must jump hurdles in any path they traverse.

The cows have T (1 ≤ T ≤ 40,000) tasks to complete. Task i comprises two distinct numbers, Ai and Bi (1 ≤ Ai ≤ N; 1 ≤ Bi ≤ N), which connote that a cow has to travel from station Ai to station Bi (by traversing over one or more paths over some route). The cows want to take a path the minimizes the height of the tallest hurdle they jump over when traveling from Ai to Bi . Your job is to write a program that determines the path whose tallest hurdle is smallest and report that height.

# **Input**

Line 1: Three space-separated integers: N, M, and T

Lines 2..M+1: Line i+1 contains three space-separated integers: Si , Ei , and Hi 

Lines M+2..M+T+1: Line i+M+1 contains two space-separated integers that describe task i: Ai and Bi

# Output

Lines 1..T: Line i contains the result for task i and tells the smallest possible maximum height necessary to travel between the stations. Output -1 if it is impossible to travel between the two stations.

# Sample Input

**5 6 3**

**1 2 123 2 81 3 52 5 33 4 42 4 83 41 25 1**

# Sample Output

**48-1**


题意：已知有n个木桩，以及其中m对木桩的高度差，询问t次，每次求从木桩u跳到木桩v，最少的跳跃高度总和，如果无法到达，输出-1。

思路：题目求的是每条路径所经过的相邻的木桩差值的最大值，然后取所有方案的最小值。用 Floyd 算法跑一遍，关键在于当不同类 g[i][k] 和 g[k][j] 合并时，求从i到j内的最大值；当同类两个 g[i][j] 选最优值时，求从i到j所有值的最小值。

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
#define N 1001
#define MOD 1001
#define E 1e-12
using namespace std;
int g[N][N];
int main()
{
    int n,m,t;
    while(scanf("%d%d%d",&n,&m,&t)!=EOF)
    {
        memset(g,INF,sizeof(g));

        int x,y,w;
        while(m--)//输入m组数据
        {
            scanf("%d%d%d",&x,&y,&w);
            g[x][y]=w;
        }

        
        /*
            当不同类g[i][k]和g[k][j]合并时，求从i到j内的最大值
            当同类两个g[i][j]选最优值时，求从i到j所有值的最小值
        */
        for(int k=1;k<=n;k++)
            for(int i=1;i<=n;i++)
                for(int j=1;j<=n;j++)
                    if(g[i][k]!=INF&&g[k][j]!=INF)
                        g[i][j]=min(g[i][j],max(g[i][k],g[k][j]));

        while(t--)//t组询问
        {
            scanf("%d%d",&x,&y);
            if(g[x][y]==INF)
                printf("-1\n");
            else
                printf("%d\n",g[x][y]);
        }
    }
    return 0;
}
```






