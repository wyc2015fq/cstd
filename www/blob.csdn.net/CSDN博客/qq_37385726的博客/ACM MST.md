# ACM MST - qq_37385726的博客 - CSDN博客





2018年05月23日 21:41:45[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：35
个人分类：[ACM](https://blog.csdn.net/qq_37385726/article/category/7683299)









![](https://img-blog.csdn.net/20180523213317612?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


**MST为最小生成树**

**![](https://img-blog.csdn.net/20180523213436487?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

**![](https://img-blog.csdn.net/20180523213450746?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**









**·两种实现方式：Prim算法，Kruskal算法**
**·该题不能使用Prim算法，因为Prim算法需要储存图的邻接矩阵，而根据Hint中n<=1000000,储存1000000*1000000会内存不够。**
**·该题使用Kruskal算法，该算法是通过维护结点的并查集，边长的优先队列来实现的**



**Prim解法（memory limit exceed，MLE）**

```cpp
#include<iostream>
using namespace std;
#include<memory.h>
#include<limits.h>
#define VISITED 1
#define UNVISITED 0

class Graph
{
	private:
		int numVertrix;
		int** matrix;
		int* V;
		bool* mark;
		long long mstEdgenum;
	public:
		Graph(int v);
		void setEdge(int v1,int v2,int w);
		int first(int v);
		int next(int v,int w);
		int minV();
		void Prim();
		int weight(int v1,int v2);
		int total();
};


Graph::Graph(int v)
{
	numVertrix=v;
	V=new int[v];
	mark=new bool[v];
	matrix=new int*[v];
	for(int i=0;i<v;i++)
	{
		matrix[i]=new int[v];
		memset(matrix[i],0,sizeof(matrix[0][0])*v);
		V[i]=INT_MAX;
		mark[i]=UNVISITED;
	}
	V[0]=0;
	mstEdgenum=0;
}



void Graph::setEdge(int v1,int v2,int w)
{
	matrix[v1-1][v2-1]=w;
	matrix[v2-1][v1-1]=w;
}

int Graph::first(int v)
{
	for(int i=0;i<numVertrix;i++)
	{
		if(matrix[v][i]!=0)
		{
			return i;
		}
	}
	return numVertrix;
}

int Graph::next(int v,int w)
{
	for(int i=w+1;v<numVertrix;i++)
	{
		if(matrix[v][i]!=0)
		{
			return i;
		}
	}
	return numVertrix;
}


int Graph::minV()
{
	int i,v;
	for(i=0;i<numVertrix;i++)
	{
		if(mark[i]==UNVISITED)
		{
			v=i;
			break;
		}
	}
	for(int k=i+1;k<numVertrix;k++)
	{
		if(V[k]<V[v]&&mark[k]==UNVISITED)
		{
			v=k;
		}
	}
	return v;
}

void Graph::Prim()
{
	int source[numVertrix];
	memset(source,-1,sizeof(source[0])*numVertrix);
	for(int i=0;i<numVertrix;i++)
	{
		int v=minV();
		mark[v]=VISITED;
		if(v!=0)
		{
			mstEdgenum+=weight(v,source[v]);
		}
		for(int k=first(v);k<numVertrix;k=next(v,k))
		{
			if(V[k]>weight(k,v))
			{
				V[k]=weight(k,v);
				source[k]=v;
			}
		}
	}
}

int Graph::weight(int v1,int v2)
{
	return matrix[v1][v2];
}


int Graph::total()
{
	return mstEdgenum;
}

int main(void)
{
	int n,m;
	while(~scanf("%d %d",&n,&m))
	{
		Graph mst(n);
		int u,v,c;
		for(int i=0;i<m;i++)
		{
			scanf("%d %d %d",&u,&v,&c);
			mst.setEdge(u,v,c);
		}
		mst.Prim();
		printf("%lld\n",mst.total());
	}
	return 0;
}
```

**Kruskal算法**

```cpp
#include<iostream>
using namespace std;
#include<queue>


struct Node
{
	int u,v,c;
	Node(int uu,int vv,int cc);
	bool operator <(Node a) const
	{
		if(c>a.c)
		{
			return 1;
		}
		return 0;
	}
};

Node::Node(int uu,int vv,int cc)
{
	u=uu;
	v=vv;
	c=cc;
}



class UnionFind
{
	private:
		int* array;
		int find(int n);
	public:
		UnionFind(int n);
		void Union(int n1,int n2);
		bool Differ(int n1,int n2);
};


int UnionFind::find(int n)
{
	if(array[n]==n)
	{
		return n;
	}
	array[n]=find(array[n]);
	return array[n];
}



UnionFind::UnionFind(int n)
{
	array=new int[n+1];
	for(int i=1;i<=n;i++)
	{
		array[i]=i;
	}
}


void UnionFind::Union(int n1,int n2)
{
	int num1=find(n1);
	int num2=find(n2);
	if(num1!=num2)
	{
		array[num2]=num1;
	}
}


bool UnionFind::Differ(int n1,int n2)
{
	int num1=find(n1);
	int num2=find(n2);
	return num1!=num2;
}


int main(void)
{
	int n,m;
	while(~scanf("%d %d",&n,&m))
	{
		int total=0;
		UnionFind array(n);
		priority_queue<Node> queue;
		int uu,vv,cc;
		for(int i=0;i<m;i++)
		{
			scanf("%d %d %d",&uu,&vv,&cc);
			queue.push(Node(uu,vv,cc));
		}
		for(int i=0;i<m;i++)
		{
			Node temp=queue.top();
			if(array.Differ(temp.u,temp.v))
			{
				array.Union(temp.u,temp.v);
				total+=temp.c;
			}
			queue.pop();
		}
		printf("%d\n",total);
	}
	return 0;
}
```




















