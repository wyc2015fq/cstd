# 【POJ 2253 Frogger】 - qq_37385726的博客 - CSDN博客





2018年05月28日 20:23:58[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：22标签：[ACM																[poj																[2253																[dijikstra](https://so.csdn.net/so/search/s.do?q=dijikstra&t=blog)
个人分类：[ACM																[ACM 图论](https://blog.csdn.net/qq_37385726/article/category/7693052)](https://blog.csdn.net/qq_37385726/article/category/7683299)





![](https://img-blog.csdn.net/20180528201955148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180528202014583?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



**变种的dijikstra**

**![](https://img-blog.csdn.net/20180528202047238?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**



**注意输出格式：**

**![](https://img-blog.csdn.net/20180528202151354?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**



```cpp
#include<iostream>
using namespace std;
#include<algorithm>
#include<math.h>
#include<memory.h>

int n;
double v[205];
bool mark[205];
double temp[205][205];
double graph[205][205];

int getMin()
{
	int temp,min=100000;
	for(int i=0;i<n;i++)
	{
		if(mark[i]==0&&min>v[i])
		{
			temp=i;
			min=v[i];
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
		int min=getMin();
		mark[min]=1;
		for(int k=0;k<n;k++)
		{
			if(v[k]>max(v[min],graph[k][min]))
			{
				v[k]=max(v[min],graph[k][min]);
			}
		}
	}
}

int main(void)
{
	int times=0;
	cin>>n;
	while(n)
	{
		times++;
		for(int i=0;i<n;i++)
		{
			memset(graph[i],0,sizeof(graph[i][0])*n);
		}
		for(int i=0;i<n;i++)
		{
			cin>>temp[i][0]>>temp[i][1];
			for(int k=i-1;k>=0;k--)
			{
				double distance=sqrt(pow(temp[k][0]-temp[i][0],2)+pow(temp[k][1]-temp[i][1],2));
				graph[k][i]=distance;
				graph[i][k]=distance;
			}
		}
		memset(mark,0,sizeof(mark[0])*n);
		dijikstra();
		cout<<"Scenario #"<<times<<endl;
		cout<<"Frog Distance = ";
		printf("%.3f\n\n",v[1]);
		cin>>n;
	}
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=2253&t=blog)](https://so.csdn.net/so/search/s.do?q=poj&t=blog)](https://so.csdn.net/so/search/s.do?q=ACM&t=blog)




