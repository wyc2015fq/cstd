# 【poj(1011)——Sticks】dfs+剪枝 - qq_37385726的博客 - CSDN博客





2018年05月26日 23:07:46[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：30标签：[ACM																[图论																[df																[poj 1011](https://so.csdn.net/so/search/s.do?q=poj 1011&t=blog)
个人分类：[ACM 图论																[ACM](https://blog.csdn.net/qq_37385726/article/category/7683299)](https://blog.csdn.net/qq_37385726/article/category/7693052)





![](https://img-blog.csdn.net/20180526230327574?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


![](https://img-blog.csdn.net/20180526230342347?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


**涉及到dfs和剪枝，并且不剪枝就会TLE，剪枝了，也可能TLE  ......关键看其他地方效率高不高，最好还是用qosrt函数，而不是sort函数**

```cpp
#include<iostream>
using namespace std;
#include<cstdio>
#include<memory.h>
#include<cstdlib>
int a[64];
int mark[64];
int temp;
int n;



int compare(const void* a,const void* b)
{
	if(*(int*)b>*(int*)a)
	{
		return 1;
	}
	if(*(int*)b==*(int*)a)
	{
		return 0;
	}
	return -1;
}


bool dfs(int s,int rest,int sum)
{
	if(rest==sum)
	{
		return true;
	}
	for(int i=s;i<n;i++)
	if(!mark[i]&&a[i]<=rest)
	{
		mark[i]=1;
		if(a[i]==rest)
		{
			if(dfs(0,temp,sum-a[i]))
			{
				return true;
			}
		}
		else if(dfs(i+1,rest-a[i],sum-a[i]))
		{
			return true;
		}
		mark[i]=0;
		if(rest==temp)
		{
			return false;
		}
		if(a[i]==rest)
		{
			return false;
		}
		while(i<n-1&&a[i+1]==a[i])
		{
			i++;
		}
	}
	return false;
}

int main(void)
{
	while(~scanf("%d",&n))
	{
		if(n==0)
		{
			break;
		}
		int sum=0;
		for(int i=0;i<n;i++)
		{
			cin>>a[i];
			mark[i]=0;
			sum+=a[i];
		}
		qsort(a,n,sizeof(a[0]),compare);
		temp=a[0];
		for(int i=temp;i<=sum/2;i++)
		{
			while(sum%i!=0)
			{
				i++;
			}
			temp=i;
			if(dfs(0,temp,sum))
			{
				cout<<i<<endl;
				break;
			}
		}
	}
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=df&t=blog)](https://so.csdn.net/so/search/s.do?q=图论&t=blog)](https://so.csdn.net/so/search/s.do?q=ACM&t=blog)




