# 【POJ 1125 Stockbroker Grapevine】floyd/dijikstra - qq_37385726的博客 - CSDN博客





2018年05月28日 10:13:15[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：32标签：[ACM																[poj																[1125																[floyd																[dijikstra](https://so.csdn.net/so/search/s.do?q=dijikstra&t=blog)
个人分类：[ACM																[ACM 图论](https://blog.csdn.net/qq_37385726/article/category/7693052)](https://blog.csdn.net/qq_37385726/article/category/7683299)





![](https://img-blog.csdn.net/20180528100456935?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180528100516267?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


题意：

众所周知，证券经纪业依靠的就是过度的传言。您需要想出股票经纪人中传播假情报的方法，让您的雇主在股票市场的占据优势。为了获得最大的效果，你必须蔓延最快的方式谣言。 
不幸的是你，股票经纪人信息只信任他们的“可靠来源”，这意味着你在你传播谣言之前必须考虑到他们的接触结构。它需要特定股票经纪人和一定的时间把谣言传递给他的每一位同事。你的任务将是写一个程序，告诉您选择哪一个股票经纪人作为谣言的出发点和所花费多少时间将谣言扩散到整个社会的股票经纪人。这一期限是衡量过去的人收到信息所需的时间。 
输入 
你的程序包含多组股票经纪人的输入数据。每组以股票经纪人的人数开始。接下来的几行是每个经纪人与其他人接触的一些信息，包括这些人都是谁，以及将讯息传达到他们所需的时间。每个经纪人与其他人接触信息的格式如下：开头的第一个数表示共有n个联系人，接下来就有n对整数。每对整数列出的第一个数字指的是一个联系人（例如，一个'1'是指编号1的人），其次是在传递一个信息给那个人时所采取分钟的时间。没有特殊的标点符号或空格规则。 
每个人的编号为1至经纪人数目。所花费的传递时间是从1到10分钟（含10分种）。股票经纪的人数范围是从1到100。当输入股票经纪人的人数为0时，程序终止。 
输出 
在对于每一组数据，你的程序必须输出一行，包括的信息有传输速度最快的人，以及在最后一个人收到消息后，所总共使用的时间（整数分钟计算）。 


你的程序可能会收到的一些关系会排除一些人，也就是有些人可能无法访问。如果你的程序检测到这样一个破碎的网络，只需输出消息“disjoint”。请注意，所花费的时间是从A传递消息到B，B传递信息到A不一定是花费同样的传递时间，但此类传播也是可能的。




**Note：**

第一组sample input分析

从第三个人到第一个和第二个人的时间都是2，且这时是最短的。

所以是3 2

也就是说，不是要求3->2->1或者3->1->2这种，而是由一个人可以到所有他关联的人




**#algorithm#**

1.dijikstra算法

则需要循环 人数， 使每一个人都有机会成为第一个被告诉的人

2.floyd算法

维护G矩阵，G[i][j]存储从i到j目前的最短路径

G被初始化为：对角线全为0，若第i元素与第j元素关联，则G[i][j]=length，反之G[i][j]=1000000（一个足够大的数）

G的更新

![](https://img-blog.csdn.net/20180528101421798?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





#使用Floyd算法实现#


```cpp
#include<iostream>
using namespace std;
#include<algorithm>
#include<limits.h>


class Graph
{
	public:
		int G[101][101];
		int n;
		Graph(int size);
		void floyd();
};

Graph::Graph(int size)
{
	n=size;
	int m,a,b;
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<n;k++)
		{
			G[i][k]=100000;
			if(i==k)
			{
				G[i][k]=0;
			}
		}
	}
	for(int i=0;i<n;i++)
	{
		cin>>m;
		for(int k=0;k<m;k++)
		{
			cin>>a>>b;
			G[i][a-1]=b;	
		}	
	}
}

void Graph::floyd()
{
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<n;k++)
		{
			for(int t=0;t<n;t++)
			{
				G[k][t]=min(G[k][t],G[k][i]+G[i][t]);
			}
		}
	}
}



int main(void)
{
	int n;
	while(~scanf("%d",&n))
	{
		if(n==0)
		{
			break;
		}
		Graph graph(n);
		graph.floyd();
		int min=INT_MAX;
		int index;
		for(int i=0;i<n;i++)
		{
			int temp=INT_MIN;
			for(int k=0;k<n;k++)
			{
				if(graph.G[i][k]>temp)
				{
					temp=graph.G[i][k];
				}
			}
			if(temp<min)
			{
				min=temp;
				index=i;
			}
		}
		if(min!=100000)
		{
			cout<<index+1<<" "<<min<<endl;
		}
		else
		{
			cout<<"disjoint"<<endl;
		}
	}
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=floyd&t=blog)](https://so.csdn.net/so/search/s.do?q=1125&t=blog)](https://so.csdn.net/so/search/s.do?q=poj&t=blog)](https://so.csdn.net/so/search/s.do?q=ACM&t=blog)




