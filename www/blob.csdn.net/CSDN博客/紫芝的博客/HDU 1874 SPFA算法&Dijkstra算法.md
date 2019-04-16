# HDU 1874 SPFA算法&Dijkstra算法 - 紫芝的博客 - CSDN博客





2018年04月23日 23:57:39[紫芝](https://me.csdn.net/qq_40507857)阅读数：50








## SPFA 快速最短路算法 队列优化


```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=205;
vector<pair<int,int> >E[maxn];//表示边权 
int n,m;
int d[maxn];//起点到终点的距离 
int inq[maxn];//表示点是否存在于队列 
void init()
{
	for(int i=0;i<maxn;i++)
	E[i].clear();
	
	for(int i=0;i<maxn;i++)
	inq[i]=0;
	
	for(int i=0;i<maxn;i++)
	d[i]=1e9;
}

int main()
{
	while(cin>>n>>m)
	{
		init();
		for(int i=0;i<m;i++)
		{
			int a,b,x;
			cin>>a>>b>>x;
	//城镇A和城镇B之间有一条长度为X的双向道路。
	E[a].push_back(make_pair(b,x));
	E[b].push_back(make_pair(a,x));
		 }
		 int s,t;
		 cin>>s>>t;
		 
		 queue<int> Q;
		 Q.push(s);
		 d[s]=0;//起点到起点的距离为 0 
		 inq[s]=1; 
		 while(!Q.empty()) 
		 {
		 	int now=Q.front();
		 	Q.pop();
		 	inq[now]=0;
		 	for(int i=0;i<E[now].size();i++)
		 	{
		 		int v=E[now][i].first;
		 		if(d[v]>d[now]+E[now][i].second)
		 		{
		 			d[v]=d[now]+E[now][i].second; 
		 			if(inq[v]==1)	continue;
		 			inq[v]=0;
		 			Q.push(v);
				 }
			 }
		 }
		 if(d[t]==1e9)	printf("-1\n");
		 else printf("%d\n",d[t]);
	}
	return 0;
}
```

## Dijkstra算法：通过优先队列，对SPFA优化

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=205;
vector<pair<int,int> >E[maxn];//表示边权 
int n,m;
int d[maxn];//起点到终点的距离 

void init()
{
	for(int i=0;i<maxn;i++)
	E[i].clear();
	
	for(int i=0;i<maxn;i++)
	d[i]=1e9;
}

int main()
{
	while(cin>>n>>m)
	{
		init();
		for(int i=0;i<m;i++)
		{
			int a,b,x;
			cin>>a>>b>>x;
	//城镇A和城镇B之间有一条长度为X的双向道路。
	E[a].push_back(make_pair(b,x));
	E[b].push_back(make_pair(a,x));
		 }
		 int s,t;
		 cin>>s>>t;
		 
		 priority_queue<pair<int,int> > Q;
		 //优先队列默认返回最大值
		  
		 d[s]=0;//起点到起点的距离为 0 
		 Q.push(make_pair(-d[s],s));
		 
		 while(!Q.empty()) 
		 {
		 	int now=Q.top().second;
		 	Q.pop();

		 	for(int i=0;i<E[now].size();i++)
		 	{
		 		int v=E[now][i].first;
		 		if(d[v]>d[now]+E[now][i].second)
		 		{
		 			d[v]=d[now]+E[now][i].second; 
		 		
		 			Q.push(make_pair(-d[v],v));
				 }
			 }
		 }
		 if(d[t]==1e9)	printf("-1\n");
		 else printf("%d\n",d[t]);
	}
	return 0;
}
```








