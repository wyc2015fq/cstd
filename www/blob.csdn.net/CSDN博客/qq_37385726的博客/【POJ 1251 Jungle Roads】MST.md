# 【POJ 1251 Jungle Roads】MST - qq_37385726的博客 - CSDN博客





2018年05月29日 10:32:09[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：29标签：[poj 1251																[ACM																[MST																[Prim](https://so.csdn.net/so/search/s.do?q=Prim&t=blog)
个人分类：[ACM																[ACM 图论](https://blog.csdn.net/qq_37385726/article/category/7693052)](https://blog.csdn.net/qq_37385726/article/category/7683299)





Jungle Roads

|**Time Limit:** 1000MS||**Memory Limit:** 10000K|
|----|----|----|
|**Total Submissions:** 30513||**Accepted:** 14264|
Description

![](http://poj.org/images/1251_1.jpg)
The Head Elder of the tropical island of Lagrishan has a problem. A burst of foreign aid money was spent on extra roads between villages some years ago. But the jungle overtakes roads relentlessly, so the large road network is too expensive to maintain. The Council of Elders must choose to stop maintaining some roads. The map above on the left shows all the roads in use now and the cost in aacms per month to maintain them. Of course there needs to be some way to get between all the villages on maintained roads, even if the route is not as short as before. The Chief Elder would like to tell the Council of Elders what would be the smallest amount they could spend in aacms per month to maintain roads that would connect all the villages. The villages are labeled A through I in the maps above. The map on the right shows the roads that could be maintained most cheaply, for 216 aacms per month. Your task is to write a program that will solve such problems. 



Input

The input consists of one to 100 data sets, followed by a final line containing only 0. Each data set starts with a line containing only a number n, which is the number of villages, 1 < n < 27, and the villages are labeled with the first n letters of the alphabet, capitalized. Each data set is completed with n-1 lines that start with village labels in alphabetical order. There is no line for the last village. Each line for a village starts with the village label followed by a number, k, of roads from this village to villages with labels later in the alphabet. If k is greater than 0, the line continues with data for each of the k roads. The data for each road is the village label for the other end of the road followed by the monthly maintenance cost in aacms for the road. Maintenance costs will be positive integers less than 100. All data fields in the row are separated by single blanks. The road network will always allow travel between all the villages. The network will never have more than 75 roads. No village will have more than 15 roads going to other villages (before or after in the alphabet). In the sample input below, the first data set goes with the map above. 


Output

The output is one integer per line for each data set: the minimum cost in aacms per month to maintain a road system that connect all the villages. Caution: A brute force solution that examines every possible set of roads will not finish within the one minute time limit. 


Sample Input
9
A 2 B 12 I 25
B 3 C 10 H 40 I 8
C 2 D 18 G 55
D 1 E 44
E 2 F 60 G 38
F 0
G 1 H 35
H 1 I 35
3
A 2 B 10 C 40
B 1 C 20
0
Sample Output
216
30


**最小生成树问题，使用Prim算法解**

```cpp
#include<iostream>
using namespace std;
#include<algorithm>
#include<memory.h>

class Graph
{
	private:
		int n;
		int graph[27][27];
		bool mark[27];
		int v[27];
		int d[27];
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
		d[i]=100000;
	}
	d[0]=0;
	num=0;
	char ch;
	int count;
	for(int i=0;i<n-1;i++)
	{
		cin>>ch>>count;
		int temp;
		int tempnum;
		for(int k=0;k<count;k++)
		{
			cin>>ch>>temp;
			tempnum=ch-'A';
			graph[tempnum][i]=temp;
			graph[i][tempnum]=temp;
		}
	}
}


int Graph::getMin()
{
	int temp=-1,min=1000000;
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
	cin>>n;
	while(n)
	{
		Graph g(n);
		g.Prim();
		cout<<g.getNum()<<endl;
		cin>>n;
	}
	return 0;
}
```](https://so.csdn.net/so/search/s.do?q=MST&t=blog)](https://so.csdn.net/so/search/s.do?q=ACM&t=blog)](https://so.csdn.net/so/search/s.do?q=poj 1251&t=blog)




