# Lilypad Pondg（POJ-3171） - Alex_McAvoy的博客 - CSDN博客





2018年06月27日 20:53:44[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：75
个人分类：[POJ																[图论——图的搜索](https://blog.csdn.net/u011815404/article/category/8237087)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

FJ has installed a beautiful pond for his cows' aesthetic enjoyment and exercise.

The rectangular pond has been partitioned into square cells of M rows and N columns (1 ≤ M ≤ 30; 1 ≤ N ≤ 30). Some of the cells have astonishingly sturdy lilypads; others have rocks; the remainder are just beautiful, cool, blue water.

Bessie is practicing her ballet moves by jumping from one lilypad to another and is currently located at one of the lilypads. She wants to travel to another lilypad in the pond by jumping from one lilypad to another.

Surprising only to the uninitiated, Bessie's jumps between lilypads always appear as a chess-knight's move: one move in one direction and then two more in the orthogonal direction (or perhaps two in one direction and then one in the orthogonal direction).

Farmer John is observing Bessie's ballet drill and realizes that sometimes she might not be able to jump to her destination lilypad because intermediary lilypads are missing.

Ever thrifty, he wants to place additional lilypads so she can complete her quest (perhaps quickly, perhaps by using a large number of intermediate lilypads). Of course, lilypads cannot be placed where rocks already intrude on a cell.

Help Farmer John determine the minimum number of additional lilypads he has to place, and in how many ways he can place that minimum number.

# **Input**

Line 1: Two space-separated integers: M and N 

Lines 2..M+1: Line i+1 describes row i of the pond using N space-separated integers with these values: 0 indicates empty water; 1 indicates a lilypad in place; 2 indicates rock in place; 3 indicates the lilypad Bessie starts on; 4 indicates the lilypad Bessie wants to travel to.

# Output

Line 1: One integer: the minimum number of additional lilypads required. If it is not possible to help Bessie jump to her destination, print -1. 

Line 2: One integer: the total number of possible ways the additional lilypads can be positioned. This number is guaranteed to fit into a single 64-bit signed integer. Do not output this line if line 1 contains -1.

# Sample Input

**4 5**

**1 0 0 0 03 0 0 0 00 0 2 0 00 0 0 4 0**

# Sample Output

**2**

**3**


题意：给出一 M * N 的矩阵，0为空，1为荷叶，2有石头，3是起点，4是终点，牛要从起点跳到终点，跳时只能走象棋中的马步，要帮助牛从起点到终点，最少放多少片荷叶，以及放最少荷叶的最小方案数。

思路：先把能到达的每个需要建荷花的点进行预处理，将中间跳跃的过程省略，这样从起点到终点的最短路就是最少要放的荷花数，最后的方案数为上一点的方案数相加。

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
#define N 51
#define MOD 123
#define E 1e-6
using namespace std;
int m,n;
int dir[8][2]={{1,2},{1,-2},{2,1},{2,-1},{-1,2},{-1,-2},{-2,1},{-2,-1}};
int g[N][N];
long long dis[N][N],ans[N][N];
int vis[N][N];
int d[N][N][N][N];
long long len[N][N];
int judge(int x,int y)
{
	if(x>=1&&x<=m&&y>=1&&y<=n)
        return 1;
	return 0;
}
void bfs1(int x0,int y0)
{
	queue <int> queue_x;
	queue <int> queue_y;
	queue_x.push(x0);
	queue_y.push(y0);

    memset(vis,0,sizeof(vis));
	vis[x0][y0]=1;

	while(!queue_x.empty())
	{
		int dirx=queue_x.front();
		int diry=queue_y.front();
		queue_x.pop();
        queue_y.pop();
		for(int i=0;i<8;i++)
		{
			int xx=dirx+dir[i][0];
			int yy=diry+dir[i][1];
			if(judge(xx,yy))
			{
				if(vis[xx][yy]==0)
				{
					vis[xx][yy]=1;
					if(g[xx][yy]==1)
					{
						queue_x.push(xx);
						queue_y.push(yy);
					}
					else
						d[x0][y0][xx][yy]=1;
				}
			}
		}
	}
}
void bfs2(int x0,int y0)
{
	queue <int> queue_x;
	queue <int> queue_y;
	queue_x.push(x0);
	queue_y.push(y0);

	memset(ans,0,sizeof(ans));
	memset(dis,INF,sizeof(dis));
	ans[x0][y0]=1;
	dis[x0][y0]=0;
	len[x0][y0]=0;

	while(!queue_x.empty())
	{
		int dirx=queue_x.front();
		int diry=queue_y.front();

		queue_x.pop();
		queue_y.pop();

		if(g[dirx][diry]==2)
            continue;
		for(int i=1;i<=m;i++)
		{
			for(int j=1;j<=n;j++)
			{
				if(d[dirx][diry][i][j])
				{
					if(dis[i][j]==dis[dirx][diry]+1)
						ans[i][j]+=ans[dirx][diry];
                    else if(dis[i][j]>dis[dirx][diry]+1)
					{
						dis[i][j]=dis[dirx][diry]+1;
						ans[i][j]=ans[dirx][diry];
						queue_x.push(i);
						queue_y.push(j);
					}
				}
			}
		}
	}
}
int main()
{
    int start_x,start_y,end_x,end_y;
	while(scanf("%d%d",&m,&n)!=EOF)
	{
		for(int i=1;i<=m;i++)
		{
			for(int j=1;j<=n;j++)
			{
				scanf("%d",&g[i][j]);
				if(g[i][j]==3)//记录始点
				{
					start_x=i;
					start_y=j;
				}
				if(g[i][j]==4)//记录终点
				{
					end_x=i;
					end_y=j;
				}
			}
		}

		memset(d,0,sizeof(d));
		for(int i=1;i<=m;i++)//预处理
			for(int j=1;j<=n;j++)
				bfs1(i,j);

		bfs2(start_x,start_y);
		if(dis[end_x][end_y]>INF)
			printf("-1\n");
        else
			printf("%lld\n%lld\n",dis[end_x][end_y]-1,ans[end_x][end_y]);
	}
	return 0;
}
```






