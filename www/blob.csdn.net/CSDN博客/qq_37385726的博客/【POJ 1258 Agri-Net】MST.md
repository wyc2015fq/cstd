# 【POJ 1258 Agri-Net】MST - qq_37385726的博客 - CSDN博客





2018年05月29日 10:50:16[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：36标签：[ACM																[poj 1258																[MST																[Prim																[图论](https://so.csdn.net/so/search/s.do?q=图论&t=blog)
个人分类：[ACM 图论																[ACM](https://blog.csdn.net/qq_37385726/article/category/7683299)](https://blog.csdn.net/qq_37385726/article/category/7693052)





Agri-Net

|**Time Limit:** 1000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 66487||**Accepted:** 27548|
Description

Farmer John has been elected mayor of his town! One of his campaign promises was to bring internet connectivity to all farms in the area. He needs your help, of course. 
Farmer John ordered a high speed connection for his farm and is going to share his connectivity with the other farmers. To minimize cost, he wants to lay the minimum amount of optical fiber to connect his farm to all the other farms. 
Given a list of how much fiber it takes to connect each pair of farms, you must find the minimum amount of fiber needed to connect them all together. Each farm must connect to some other farm such that a packet can flow from any one farm to any other farm. 
The distance between any two farms will not exceed 100,000. 


Input

The input includes several cases. For each case, the first line contains the number of farms, N (3 <= N <= 100). The following lines contain the N x N conectivity matrix, where each element shows the distance from on farm to another. Logically, they are N lines of N space-separated integers. Physically, they are limited in length to 80 characters, so some lines continue onto others. Of course, the diagonal will be 0, since the distance from farm i to itself is not interesting for this problem.

Output

For each case, output a single integer length that is the sum of the minimum length of fiber required to connect the entire set of farms.

Sample Input
4
0 4 9 21
4 0 8 17
9 8 0 16
21 17 16 0

Sample Output

28



**MST题，使用Prim算法解**

```cpp
#include<iostream>
using namespace std;
#include<algorithm>
#include<memory.h>

class Graph
{
	private:
		int n;
		int graph[105][105];
		bool mark[105];
		int v[105];
		int d[105];
		int num;
	public:
		Graph(int nn);
		int getMin();
		int first(int v);
		int next(int min,int v);
		void Prim();
		int getNum();
};


Graph::Graph(int nn)
{
	n=nn;
	for(int i=0;i<n;i++)
	{
		memset(graph[i],0,sizeof(graph[i][0])*n);
		mark[i]=0;
		v[i]=-1;
		d[i]=1000000;
	}
	d[0]=0;
	num=0;
	char ch;
	int count;
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<n;k++)
		{
			cin>>graph[i][k];
		}
	}
}


int Graph::getMin()
{
	int temp=-1,min=1001000;
	for(int i=0;i<n;i++)
	{
		if(mark[i]==0&&d[i]<min)
		{
			temp=i;
			min=d[i];
		}
		
	}
	return temp;
}

int Graph::first(int v)
{
	for(int i=0;i<n;i++)
	{
		if(mark[i]==0&&graph[v][i]!=0)
		{
			return i;
		}
	}
	return n;
}

int Graph::next(int min,int v)
{
	for(int i=v+1;i<n;i++)
	{
		if(mark[i]==0&&graph[min][i]!=0)
		{
			return i;
		}
	}
	return n;
}

void Graph::Prim()
{
	for(int i=0;i<n;i++)
	{
		int min=getMin();
		mark[min]=1;
		if(min!=0)
		{
			num+=graph[min][v[min]];
		}
		for(int k=first(min);k<n;k=next(min,k))
		{
			if(d[k]>graph[min][k])
			{
				d[k]=graph[min][k];
				v[k]=min;
			}
		}
	}
}


int Graph::getNum()
{
	return num;
}



int main(void)
{
	int n;
	while(~scanf("%d",&n))
	{
		Graph g(n);
		g.Prim();
		cout<<g.getNum()<<endl;
	}
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=Prim&t=blog)](https://so.csdn.net/so/search/s.do?q=MST&t=blog)](https://so.csdn.net/so/search/s.do?q=poj 1258&t=blog)](https://so.csdn.net/so/search/s.do?q=ACM&t=blog)




