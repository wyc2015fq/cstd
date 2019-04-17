# Avoid The Lakes（POJ-3620） - Alex_McAvoy的博客 - CSDN博客





2018年06月02日 00:49:15[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60
个人分类：[POJ																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Farmer John's farm was flooded in the most recent storm, a fact only aggravated by the information that his cows are deathly afraid of water. His insurance agency will only repay him, however, an amount depending on the size of the largest "lake" on his farm.

The farm is represented as a rectangular grid with N (1 ≤ N ≤ 100) rows and M (1 ≤ M ≤ 100) columns. Each cell in the grid is either dry or submerged, and exactly K (1 ≤ K≤ N × M) of the cells are submerged. As one would expect, a lake has a central cell to which other cells connect by sharing a long edge (not a corner). Any cell that shares a long edge with the central cell or shares a long edge with any connected cell becomes a connected cell and is part of the lake.

# **Input**

Line 1: Three space-separated integers: N, M, and K

Lines 2..K+1: Line i+1 describes one submerged location with two space separated integers that are its row and column: R and C

# Output

Line 1: The number of cells that the largest lake contains.

# Sample Input

**3 4 5**

**3 22 23 12 31 1**

# Sample Output

**4**


题意：第一行给出图大小N*M，有K个单元被淹没了，其后的K行代表被淹没的单元的坐标，连通的标准是上下左右有接触，求整张图最大的潮湿的单元格的大小。

思路：dfs水题无误

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
#define MOD 2520
#define E 1e-12
using namespace std;
int n,m,k;
int g[N][N];
int dir[4][2]={{0,1},{0,-1},{1,0},{-1,0}};
int sum;
void dfs(int x,int y)
{
    if(g[x][y]==0)
        return;

    sum++;
    g[x][y]=0;

    for(int i=0;i<4;i++)
    {
        int nx=x+dir[i][0];
        int ny=y+dir[i][1];
        if(nx>=1&&nx<=n&&ny>=1&&ny<=m&&g[nx][ny]==1)
            dfs(nx,ny);
    }
}
int main()
{
    while(scanf("%d%d%d",&n,&m,&k)!=EOF)
    {
        int maxx=-INF;
        memset(g,0,sizeof(g));

        while(k--)
        {
            int x,y;
            scanf("%d%d",&x,&y);
            g[x][y]=1;
        }

        for(int i=1;i<=n;i++)
        {
            for(int j=1;j<=m;j++)
            {
                if(g[i][j]==1)
                {
                    sum=0;
                    dfs(i,j);
                    maxx=max(sum,maxx);
                }
            }
        }

        printf("%d\n",maxx);
    }
    return 0;
}
```






