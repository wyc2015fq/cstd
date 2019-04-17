# Find the safest road（HDU-1596） - Alex_McAvoy的博客 - CSDN博客





2018年05月22日 13:55:55[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：45
个人分类：[HDU																[图论——Floyd](https://blog.csdn.net/u011815404/article/category/8312424)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

XX星球有很多城市，每个城市之间有一条或多条飞行通道，但是并不是所有的路都是很安全的，每一条路有一个安全系数s,s是在 0 和 1 间的实数(包括0，1)，一条从u 到 v 的通道P 的安全度为Safe(P) = s(e1)*s(e2)…*s(ek) e1,e2,ek是P 上的边 ，现在8600 想出去旅游，面对这这么多的路，他想找一条最安全的路。但是8600 的数学不好，想请你帮忙 ^_^

# Input

输入包括多个测试实例，每个实例包括：<br>第一行：n。n表示城市的个数n<=1000;<br>接着是一个n*n的矩阵表示两个城市之间的安全系数，(0可以理解为那两个城市之间没有直接的通道)<br>接着是Q个8600要旅游的路线,每行有两个数字，表示8600所在的城市和要去的城市。

# Output

如果8600无法达到他的目的地，输出"What a pity!",其他的输出这两个城市之间的最安全道路的安全系数,保留三位小数。

# Sample Input

**31 0.5 0.50.5 1 0.40.5 0.4 131 22 31 3 **

# Sample Output

**0.500**

**0.4000.500**


思路：求一个图中的最长路，类似模版题，本来想用Dijkstra算法，但算了算时间复杂度，发现使用Floyed算法不会超时，果断使用

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
#define INF 9999999999999999999
#define N 1001
#define MOD 1000000007
#define E 1e-12
using namespace std;
double g[N][N];
int main()
{
    int n,m;
    while(scanf("%d",&n)!=EOF)
    {
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                scanf("%lf",&g[i][j]);

        for(int k=1;k<=n;k++)
            for(int i=1;i<=n;i++)
                for(int j=1;j<=n;j++)
                    if(i!=k&&j!=i&&j!=k)
                        if(g[i][j]<g[i][k]*g[k][j])
                            g[i][j]=g[i][k]*g[k][j];

        scanf("%d",&m);
        for(int i=1;i<=m;i++)
        {
            int x,y;
            scanf("%d%d",&x,&y);
            if(g[x][y]!=0)
                printf("%.3lf\n",g[x][y]);
            else
                printf("What a pity!\n");
        }
   }
   return 0;
}
```




