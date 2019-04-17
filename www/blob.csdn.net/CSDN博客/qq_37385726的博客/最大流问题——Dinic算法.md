# 最大流问题——Dinic算法 - qq_37385726的博客 - CSDN博客





2018年05月30日 10:00:12[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：107








```cpp

```

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

[**参考**](https://www.cnblogs.com/SYCstudio/p/7260613.html)





