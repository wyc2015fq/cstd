# 贪心算法——2.5最短路径（Dijkstra） - PeterBishop - CSDN博客





2018年08月21日 10:34:27[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：235














![](https://img-blog.csdn.net/20180819181130171?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20180819181425264?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 图解如下：

![](https://img-blog.csdn.net/20180819182421873?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180819182538167?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20180819184115876?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180819184156180?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20180819184226800?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 代码实现如下：

```cpp
#include<queue>
#include<iostream>
#include<cstring>
using namespace std;
const int N = 100;
const int INF = 1e7;
int map[N][N], dist[N], n, m;
int flag[N];
struct Node
{
	int u, step;
	Node() {};
	Node(int a, int sp)
	{
		u = a;
		step = sp;
	}
	bool operator <(const Node &a)const
	{
		return step > a.step;
	}
};
void Dijkstra(int st)
{
	priority_queue<Node> Q;
	Q.push(Node(st,0));
	memset(flag, 0, sizeof(flag));
	for (int i = 0; i < n; i++)
		dist[i] = INF;
	dist[st] = 0;
	while (!Q.empty())
	{
		Node it = Q.top();
		Q.pop();
		int t = it.u;
		if (flag[t])
			continue;
		flag[t] = 1;
		for (int i = 0; i < n; i++)
		{
			if (!flag[i] && map[t][i] < INF)
			{
				if (dist[i] > dist[t] + map[t][i])
				{
					dist[i] = dist[t] + map[t][i];
					Q.push(Node(i, dist[i]));
				}
			}
		}
	}
}
int main()
{
	int u, v, w, st;
	cout << "请输入城市个数和路线个数"<<endl;
	cin >> n >> m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			map[i][j] = INF;
	cout << "请输入城市之间u，v的路线以及其距离w" << endl;
	while (m--)
	{
		cin >> u >> v >> w;
		map[u][v] = min(map[u][v], w);
	}
	cout << "请输入小明的位置：" << endl;
	cin >> st;
	Dijkstra(st);
	for (int i = 0; i < n; i++)
	{
		cout << "要去的位置：" << i;
		if (dist[i] == INF)
			cout << "无法到达" << endl;
		else
			cout << "最短距离：" << dist[i] << endl;
	}
	return 0;
}
```



