# ACM 图论 最大流问题 - qq_37385726的博客 - CSDN博客





2018年05月30日 09:56:40[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：116








**一：最大流问题简介**

**![](https://img-blog.csdn.net/20180530093859606?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

**1.源点，点**

****源点：只出不进，即图中Vs****

****汇点：只进不出，即图中Vt****

**2.容量**

**c(u,v)为从点u到点v能通过的最大流量。**

**上图为容量图，则c(2,1)=4**

**3.流量**

**f(u,v)为从点u到点v通过的实际流量**

**4.可行流**

**满足：**

**![](https://img-blog.csdn.net/2018053009451751?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

**（初始化的可行流为每边流量全为0）**

**5.残量**

**res(u,v)=c(u,v)-f(u,v)**

**为点u到点v间还能通过的流量**

**6.增广路径**

**为从源点到汇点，所经过的每一条边，满足残量大于0，则该路径为增广路径**

**![](https://img-blog.csdn.net/20180530094958253?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

**x/y=f(u,v)/c(u,v)**

**一条增广路径为s-A-C-T**





**二：Dinic算法简介**

**该算法就是通过不断找增广路径来扩充流。**

**核心要点：bfs构建层次图，dfs找增广路径的增广值（该路径上最小的残量值）并更新当前流量，Dinic循环上述过程，至无增广路径，此时为最大流**



**·bfs()**

```cpp
bool Graph::bfs()
{
	memset(level,0,sizeof(level[0])*n);
	memset(mark,0,sizeof(mark[0])*n);
	queue<int> q;
	while(!q.empty())
	{
		q.pop();
	}
	q.push(0);
	mark[0]=1;
	while(q.size())
	{
		int v=q.front();
		q.pop();
		for(int i=first(v);i<n;i=next(v,i))
		{
			if(mark[i]==0&&flow[v][i]<res[v][i])
			{
				level[i]=level[v]+1;
				q.push(i);
				mark[i]=1;
			}
			if(i==n-1&&level[i]!=0)
			{
				return 1;
			}
		}
	}
	return 0;
}
```



**·dfs()**


```cpp
int Graph::dfs(int s,int dis)
{
	if(s==n-1)
	{
		return dis;
	}
	for(int i=first(s);i<n;i=next(s,i))
	{
		if(level[i]==level[s]+1)
		{
			int di=dfs(i,min(dis,res[s][i]-flow[s][i]));
			if(di>0)
			{
				flow[s][i]+=di;
				flow[i][s]-=di;
				return di;
			}
		}
	}
	return 0;
}
```




**·Dinic()**


```cpp
int Graph::Dinic()
{
	maxflow=0;
	while(bfs())
	{
		int di;
		while(di=dfs(0,1000000))
		{
			maxflow+=di;
		}
	}
	return maxflow;
}
```




**·完整代码**


```cpp
#include<iostream>
using namespace std;
#include<queue>
#include<memory.h>
#include<stdlib.h>

class Graph
{
	private:
		int res[101][101];
		int n;
		int flow[101][101];
		int level[101];
		int mark[101];
		int maxflow;
	public:
		Graph(int nn);
		int first(int v);
		int next(int v,int w);
		bool bfs();
		int dfs(int s,int dis);
		int Dinic();
};


Graph::Graph(int nn)
{
	n=nn;
	for(int i=0;i<n;i++)
	{
		memset(res[i],0,sizeof(res[i][0])*n);
		memset(flow[i],0,sizeof(flow[i][0])*n);
		level[i]=0;
		mark[i]=0;
	}
	maxflow=0;
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<n;k++)
		{
			cin>>res[i][k];
		}
	}
}


int Graph::first(int v)
{
	for(int i=0;i<n;i++)
	{
		if(res[v][i])
		{
			return i;
		}
	}
	return n;
}


int Graph::next(int v,int w)
{
	for(int i=w+1;i<n;i++)
	{
		if(res[v][i])
		{
			return i;
		}
	}
	return n;
}


bool Graph::bfs()
{
	memset(level,0,sizeof(level[0])*n);
	memset(mark,0,sizeof(mark[0])*n);
	queue<int> q;
	while(!q.empty())
	{
		q.pop();
	}
	q.push(0);
	mark[0]=1;
	while(q.size())
	{
		int v=q.front();
		q.pop();
		for(int i=first(v);i<n;i=next(v,i))
		{
			if(mark[i]==0&&flow[v][i]<res[v][i])
			{
				level[i]=level[v]+1;
				q.push(i);
				mark[i]=1;
			}
			if(i==n-1&&level[i]!=0)
			{
				return 1;
			}
		}
	}
	return 0;
}


int Graph::dfs(int s,int dis)
{
	if(s==n-1)
	{
		return dis;
	}
	for(int i=first(s);i<n;i=next(s,i))
	{
		if(level[i]==level[s]+1)
		{
			int di=dfs(i,min(dis,res[s][i]-flow[s][i]));
			if(di>0)
			{
				flow[s][i]+=di;
				flow[i][s]-=di;
				return di;
			}
		}
	}
	return 0;
}


int Graph::Dinic()
{
	maxflow=0;
	while(bfs())
	{
		int di;
		while(di=dfs(0,1000000))
		{
			maxflow+=di;
		}
	}
	return maxflow;
}


int main(void)
{
	int n;
	while(~scanf("%d",&n))
	{
		Graph g(n);
		cout<<g.Dinic()<<endl;
	}
}
```




