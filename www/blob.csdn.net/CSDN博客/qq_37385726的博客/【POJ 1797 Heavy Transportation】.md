# 【POJ 1797 Heavy Transportation】 - qq_37385726的博客 - CSDN博客





2018年05月28日 18:31:55[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：33标签：[ACM																[poj1797																[dijikstr																[floyd																[dfs](https://so.csdn.net/so/search/s.do?q=dfs&t=blog)
个人分类：[ACM																[ACM 图论](https://blog.csdn.net/qq_37385726/article/category/7693052)](https://blog.csdn.net/qq_37385726/article/category/7683299)





![](https://img-blog.csdn.net/20180528182702444?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018052818271741?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




**方法有**

**1.dijikstra  （原来的变形，更改初始化条件和更新条件，每一项记录当前从第一点到该点的最大承重值）**

**![](https://img-blog.csdn.net/20180528182848601?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

```cpp
#include<iostream>
using namespace std;
#include<algorithm>
#include<memory.h>


int v[1005];
int mark[1005];
int graph[1005][1005];
int n;

int getMax()
{
	int max=-1;
	int temp=0;
	for(int i=0;i<n;i++)
	{
		if(v[i]>max&&mark[i]==0)
		{
			temp=i;
			max=v[i];
		}
	}
	return temp;
}

void dijikstra()
{
	for(int i=0;i<n;i++)
	{
		v[i]=graph[0][i];
	}
	for(int i=0;i<n;i++)
	{
		int max=getMax();
		mark[max]=1;
		for(int k=0;k<n;k++)
		{
			if(!mark[k]&&v[k]<min(graph[max][k],v[max]))
			{
				v[k]=min(graph[max][k],v[max]);
			}
		}
	}
}

int main(void)
{
	int times;
	cin>>times;
	for(int i=1;i<=times;i++)
	{
		int m;
		cin>>n>>m;
		memset(mark,0,sizeof(mark[0])*n);
		
		for(int k=0;k<n;k++)
		{
			memset(graph[k],0,sizeof(graph[k][0])*n);
		}
		for(int k=0;k<m;k++)
		{
			int a1,a2,w1;
			cin>>a1>>a2>>w1;
			graph[a1-1][a2-1]=w1;
			graph[a2-1][a1-1]=w1;
		}
		dijikstra();
		cout<<"Scenario #"<<i<<":"<<endl;
		cout<<v[n-1]<<endl<<endl;
	}
	return 0;
 }
```


**2.Floyd （原来的变形，更改更新条件，但是TLE了）**

**![](https://img-blog.csdn.net/20180528182953278?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**

```cpp
/*TLE*/

#include<iostream>
using namespace std;
#include<algorithm>
#include<memory.h>


int graph[1005][1005];
int n;

void floyd()
{
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<n;k++)
		{
			for(int t=0;t<n;t++)
			{
				graph[k][t]=max(graph[k][t],min(graph[k][i],graph[i][t]));
			}
		}
	}
} 


int main(void)
{
	int times;
	cin>>times;
	for(int i=1;i<=times;i++)
	{
		int m;
		cin>>n>>m;
		for(int k=0;k<n;k++)
		{
			memset(graph[k],0,sizeof(graph[k][0])*n);
		}
		for(int k=0;k<m;k++)
		{
			int a1,a2,w1;
			cin>>a1>>a2>>w1;
			graph[a1-1][a2-1]=w1;
			graph[a2-1][a1-1]=w1;
		}
		floyd();
		cout<<"Scenario #"<<i<<":"<<endl;
		cout<<graph[0][n-1]<<endl<<endl;
	}
	return 0;
}
```


**3.dfs+回溯（但是TLE了）**

```cpp
#include<iostream>
/*TLE*/ 

using namespace std;
#include<queue>
#include<memory.h>

priority_queue<int,vector<int>,less<int> > pq;

vector<int> str[1100];
vector<int> weight[1100];
int n;

void dfs(int t,int min,bool mark[])
{
	if(t==n-1)
	{
		pq.push(min);
		return;
	}
	for(int i=0;i<str[t].size();i++)
	{
		int temp=str[t][i];
		
		if(mark[temp]==0)
		{
			mark[temp]=1;
			int tempmin=min;
			
			if(weight[t][i]<min)
			{
				tempmin=weight[t][i];
			}
			dfs(temp,tempmin,mark);
			mark[temp]=0;
		}
	}
 } 
 
 
 
int main(void)
{
	int time;
	cin>>time;
	for(int i=1;i<=time;i++)
	{
		pq.empty();
		bool mark[1005];
		mark[0]=1;
		int min=10000005;
		int m;
		cin>>n>>m;
		memset(mark,0,sizeof(mark[0])*n);
		for(int p=0;p<n;p++)
		{
			str[p].clear();  //清空当前元素 
			weight[p].clear();
		}
		for(int k=0;k<m;k++)
		{
			int a1,a2,w1;
			cin>>a1>>a2>>w1;
			str[a1-1].push_back(a2-1);
			str[a2-1].push_back(a1-1);
			weight[a1-1].push_back(w1);
			weight[a2-1].push_back(w1);
		}
		dfs(0,min,mark);
		cout<<"Scenario #"<<i<<":"<<endl;
		cout<<pq.top()<<endl;
	}
}
```](https://so.csdn.net/so/search/s.do?q=floyd&t=blog)](https://so.csdn.net/so/search/s.do?q=dijikstr&t=blog)](https://so.csdn.net/so/search/s.do?q=poj1797&t=blog)](https://so.csdn.net/so/search/s.do?q=ACM&t=blog)




