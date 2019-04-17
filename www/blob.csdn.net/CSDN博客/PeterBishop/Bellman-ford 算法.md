# Bellman-ford 算法 - PeterBishop - CSDN博客





2018年08月25日 19:56:35[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：234








贝尔曼-福特算法，它的原理是对图进行V-1次松弛操作，得到所有可能的最短路径。其优于Dijkstra算法的方面是边的权值可以为负数、实现简单，缺点是时间复杂度过高，高达O(VE)。

**1. 算法流程**

给定图G(V, E)（其中V、E分别为图G的顶点集与边集），源点s，数组Distant[i]记录从源点s到顶点i的路径长度，初始化数组Distant[n]为, Distant[s]为0；

以下操作循环执行至多n-1次，n为顶点数：

对于每一条边e(u, v)，如果Distant[u] + w(u, v) < Distant[v]，则另Distant[v] = Distant[u]+w(u, v)。w(u, v)为边e(u,v)的权值；

若上述操作没有对Distant进行更新，说明最短路径已经查找完毕，或者部分点不可达，跳出循环。否则执行下次循环；

为了检测图中是否存在负环路，即权值之和小于0的环路。对于每一条边e(u, v)，如果存在Distant[u] + w(u, v) < Distant[v]的边，则图中存在负环路，即是说改图无法求出单源最短路径。否则数组Distant[n]中记录的就是源点s到各顶点的最短路径长度。

我个人倒是觉得有点像DFS啊，对刚刚更新的结点继续下一层的搜索、计算权值，取更小的那个作为新的权值。当每个结点在这一轮都不再更新的时候，算法结束。

**2. 算法的三部分**

第一，初始化所有点。每一个点保存一个值，表示从原点到达这个点的距离，将原点的值设为0，其它的点的值设为无穷大（表示不可达）。

第二，进行循环，循环下标为从1到n－1（n等于图中点的个数）。在循环内部，遍历所有的边，进行松弛计算。

第三，遍历途中所有的边（edge(u,v)），判断是否存在这样情况：

d(v)>d(u) + w(u,v)

则返回false，表示途中存在从源点可达的权为负的回路。 

之所以需要第三部分的原因，是因为，如果存在从源点可达的权为负的回路。则应为无法收敛而导致不能求出最短路径。

可知，Bellman-Ford算法寻找单源最短路径的时间复杂度为O(VE).

**3. 算法示例**

![](https://images0.cnblogs.com/blog/632767/201411/051928507675106.png)



```cpp
#include<iostream>  
#include<cstdio>  
using namespace std;

#define INF 0x3f3f3f3f  
#define N 1010  
int nodenum, edgenum, original; //点，边，起点  

typedef struct Edge //边  
{
	int u, v;
	int cost;
}Edge;

Edge edge[N];
int dis[N], pre[N];

bool Bellman_Ford()
{
	for (int i = 1; i <= nodenum; ++i) //初始化  
		dis[i] = (i == original ? 0 : INF);
	for (int i = 1; i <= nodenum - 1; ++i)
		for (int j = 1; j <= edgenum; ++j)
			if (dis[edge[j].v] > dis[edge[j].u] + edge[j].cost) //松弛（顺序一定不能反~）  
			{
				dis[edge[j].v] = dis[edge[j].u] + edge[j].cost;
				pre[edge[j].v] = edge[j].u;
			}
	bool flag = 1; //判断是否含有负权回路  
	for (int i = 1; i <= edgenum; ++i)
		if (dis[edge[i].v] > dis[edge[i].u] + edge[i].cost)
		{
			flag = 0;
			break;
		}
	return flag;
}

void print_path(int root) //打印最短路的路径（反向）  
{
	while (root != pre[root]) //前驱  
	{
		printf("%d-->", root);
		root = pre[root];
	}
	if (root == pre[root])
		printf("%d\n", root);
}

int main()
{
	scanf("%d%d%d", &nodenum, &edgenum, &original);
	pre[original] = original;
	for (int i = 1; i <= edgenum; ++i)
	{
		scanf("%d%d%d", &edge[i].u, &edge[i].v, &edge[i].cost);
	}
	if (Bellman_Ford())
		for (int i = 1; i <= nodenum; ++i) //每个点最短路  
		{
			printf("%d\n", dis[i]);
			printf("Path:");
			print_path(i);
		}
	else
		printf("have negative circle\n");
	return 0;
}
```





