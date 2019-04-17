# 2251 Dungeon Master - PeterBishop - CSDN博客





2018年07月25日 14:18:39[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：18
个人分类：[POJ](https://blog.csdn.net/qq_40061421/article/category/7589386)









```cpp
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <queue>
#include <algorithm>
using namespace std;

char map[35][35][35];//直接用三维数组
int vis[35][35][35];//记录是否被访问过
int k, n, m, sx, sy, sz, ex, ey, ez;
int to[6][3] = { { 0,0,1 },{ 0,0,-1 },{ 0,1,0 },{ 0,-1,0 },{ 1,0,0 },{ -1,0,0 } };

struct node
{
	int x, y, z, step;
};

int check(int x, int y, int z)
{
	if (x<0 || y<0 || z<0 || x >= k || y >= n || z >= m)
		return 1;
	else if (map[x][y][z] == '#')
		return 1;
	else if (vis[x][y][z])
		return 1;
	return 0;
}

int bfs()
{
	int i;
	node a, next;
	queue<node> Q;
	a.x = sx, a.y = sy, a.z = sz;
	a.step = 0;
	vis[sx][sy][sz] = 1;
	Q.push(a);
	while (!Q.empty())
	{
		a = Q.front();
		Q.pop();
		if (a.x == ex && a.y == ey && a.z == ez)
			return a.step;
		for (i = 0; i<6; i++)//六个方向
		{
			next = a;
			next.x = a.x + to[i][0];
			next.y = a.y + to[i][1];
			next.z = a.z + to[i][2];
			if (check(next.x, next.y, next.z))
				continue;
			vis[next.x][next.y][next.z] = 1;
			next.step = a.step + 1;
			Q.push(next);
		}
	}
	return 0;
}
//整个题目就是一个bfs的最短路
int main()
{
	int i, j, r;
	while (scanf("%d%d%d", &k, &n, &m), n + m + k)
	{
		for (i = 0; i<k; i++)
		{
			for (j = 0; j<n; j++)
			{
				scanf("%s", map[i][j]);
				for (r = 0; r<m; r++)
				{
					if (map[i][j][r] == 'S')
					{
						sx = i, sy = j, sz = r;
					}
					else if (map[i][j][r] == 'E')
					{
						ex = i, ey = j, ez = r;
					}
				}
			}
		}
		memset(vis, 0, sizeof(vis));
		int ans;
		ans = bfs();
		if (ans)
			printf("Escaped in %d minute(s).\n", ans);
		else
			printf("Trapped!\n");
	}

	return 0;
}
```





