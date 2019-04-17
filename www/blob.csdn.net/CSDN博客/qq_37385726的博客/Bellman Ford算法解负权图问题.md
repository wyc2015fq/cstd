# Bellman Ford算法解负权图问题 - qq_37385726的博客 - CSDN博客





2018年05月30日 21:51:41[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：55










## [B - 耳机楼的通道](https://vjudge.net/problem/POJ-3259)
[POJ - 3259](https://vjudge.net/problem/10533/origin)




**题意解析**

要求判断经过走廊和一些秘密通道，能否回到出发之前的时间。——即有没有权值和为负（之前的时间）的回路（出发之前，要求起点和终点一样才是回到了出发之前的时间）

1.走廊，等价为从S到E权值为W的有向边和从E到S权值为W的有向边

2.秘密通道，等价为从S到E权值为W的有向边



**解法**

**因为权值中有负数，自然不能dijikstra，并且又是判断有没有负权回路，即该负权图有没有最短路径（有最短路径，无负权回路；无最短路径，有负权回路；反之一样）则使用Bellman Fork算法解**

**算法思想**



**松弛计算**


**对象**：边

**涉及**：和边关联的结点，u,v

**计算**: 如果dis[u]+w<dis[v]，则dis[v]=dis[u]+w   (有点像dijikstra，其中dist数组的含义和dijikstra中的d数组含义是一样的，表示当前起点到该点的最短路径)

**例如**

**![](https://img-blog.csdn.net/20180530214815668?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)因为3+2<8，则更新成5![](https://img-blog.csdn.net/20180530214850947?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

**![](https://img-blog.csdn.net/20180530214938271?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)因为3+4=7>6，则不更新**


**第一，初始化所有点。每一个点保存一个值，表示从原点到达这个点的距离，将原点的值设为0，其它的点的值设为无穷大（表示不可达）。第二，进行循环，循环下标为从1到n－1（n等于图中点的个数）。在循环内部，遍历所有的边，进行松弛计算。第三，遍历途中所有的边（edge（u，v）），判断是否存在这样情况：d（v） > d (u) + w(u,v)**

**则返回false，表示途中存在从源点可达的权为负的回路。**

```cpp
void relax(int u,int v,int w)
{
	if(dist[u]+w<dist[v])
	{
		dist[v]=dist[u]+w;
	}
}

bool BF()
{
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<m;k++)
		{
			relax(edge[k].u,edge[k].v,edge[k].w);
		}
	}
	int flag=0;
	for(int i=0;i<m;i++)
	{
		if(dist[edge[i].u]+edge[i].w<dist[edge[i].v])
		{
			flag=1;
			break;
		}
	}
	return flag;
}
```










**对于该题**

将输入转换成关于边的数组

![](https://img-blog.csdn.net/20180530213935909?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)其中u为该边的头结点，v为该边的尾结点，w为该边的权值


为结点保存一个从初始到每一点的当前最小路径值

![](https://img-blog.csdn.net/20180530214059806?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)dist[0]被初始化为0，其余为INF





```cpp
#include<iostream>
using namespace std;
#include<queue>
#include<algorithm>
#include<memory.h>




struct Node
{
	int u,v,w;
};

Node edge[10000];

int n,m;


int dist[500];

void relax(int u,int v,int w)
{
	if(dist[u]+w<dist[v])
	{
		dist[v]=dist[u]+w;
	}
}

bool BF()
{
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<m;k++)
		{
			relax(edge[k].u,edge[k].v,edge[k].w);
		}
	}
	int flag=0;
	for(int i=0;i<m;i++)
	{
		if(dist[edge[i].u]+edge[i].w<dist[edge[i].v])
		{
			flag=1;
			break;
		}
	}
	return flag;
}


int main(void)
{
	int F;
	cin>>F;
	while(F--)
	{
		int a1,a2,a3;
		cin>>a1>>a2>>a3;
		n=a1;
		m=a2*2+a3;
		int num=0;
		for(int i=0;i<n;i++)
		{
			dist[i]=100000000;
		}
		dist[0]=0;
		for(int i=0;i<a2;i++)
		{
			int b1,b2,b3;
			cin>>b1>>b2>>b3;
			edge[num].u=b1-1;
			edge[num].v=b2-1;
			edge[num].w=b3;
			num++;
			edge[num].u=b2-1;
			edge[num].v=b1-1;
			edge[num].w=b3;
			num++;
		}
		for(int i=0;i<a3;i++)
		{
			int c1,c2,c3;
			cin>>c1>>c2>>c3;
			edge[num].u=c1-1;
			edge[num].v=c2-1;
			edge[num].w=0-c3;
			num++;
		}
		if(BF())
		{
			cout<<"YES"<<endl;
		}
		else
		{
			cout<<"NO"<<endl;
		}
	}
	return 0;
}
```







