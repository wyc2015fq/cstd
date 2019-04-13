
# 单源最短路径Bellman_Ford算法C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月09日 22:27:46[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1481标签：[算法																](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[struct																](https://so.csdn.net/so/search/s.do?q=struct&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[存储																](https://so.csdn.net/so/search/s.do?q=存储&t=blog)[system																](https://so.csdn.net/so/search/s.do?q=system&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=存储&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=null&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=struct&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
// 单源最短路径Bellman_Ford算法.cpp : Defines the entry point for the console application.
//
\#include "stdafx.h"
\#include<iostream>
\#define MAX 100
\#define Infinity 65535
typedef int WeiType;
using namespace std;
struct edgeNode
{
int no; //边尾端的序号
char info; //边端的名称
WeiType weight; //边权值
struct edgeNode * next; //下一个
};
struct vexNode
{
char info;  //节点名称
struct edgeNode *link; //与之相连的端点
};
//存储节点信息
vexNode adjlist[MAX];
//前驱节点
int parent[MAX];
//源点到节点j最短路径的花费
WeiType lowcost[MAX];
//建立邻接表存储
//adjlist为节点集，n为节点个数，e为边数目
//返回源点序号
int createGraph(vexNode *adjlist,int n,int e)
{
int i;
for(i=1;i<=n;i++)
{
cout<<"请输入节点"<<i<<"的名称：";
cin>>adjlist[i].info;
adjlist[i].link = NULL;
parent[i] = i;
lowcost[i] = Infinity;
}
edgeNode *p1;
int v1,v2;
WeiType weight;
for(i=1;i<=e;i++)
{
cout<<"请输入边"<<i<<"的起始节点与尾节点序号：";
cin>>v1>>v2;
cout<<"请输入此边的权值：";
cin>>weight;
p1 = (edgeNode*)malloc(sizeof(edgeNode));
p1->no = v2;
p1->weight = weight;
p1->info = adjlist[v2].info;
p1->next = adjlist[v1].link;
adjlist[v1].link = p1;
}
cout<<"请输入源点序号：";
int v0;
cin>>v0;
lowcost[v0] = 0;
return v0;
}
/*
//
void relax(WeiType *lowcost,int *parent,const int u,const int v,const WeiType w)
{
if(lowcost[v]>lowcost[u]+w)
{
lowcost[v] = lowcost[u] + w;
parent[v] = u;
}
}
*/
//
bool Bellman_Ford(vexNode *adjlist,WeiType *lowcost,int *parent,const int n)
{
int i,j;
for(j=1;j<n;j++)
{
for(i=1;i<=n;i++)
{
edgeNode *p1 = adjlist[i].link;
while(p1 != NULL)
{
if(lowcost[i]+p1->weight <lowcost[p1->no])
{
lowcost[p1->no] = lowcost[i]+p1->weight;
parent[p1->no] = i;
}
p1 = p1->next;
}
}
}
//检查有无负回路
for(i=1;i<=n;i++)
{
edgeNode *p2 = adjlist[i].link;
while(p2 != NULL)
{
if(lowcost[p2->no]>lowcost[i]+p2->weight)
return false;
p2 = p2->next;
}
}
return true;
}
//打印源点到节点i最短路径
void print_it(vexNode *adjlist,int *parent,const int v0,const int i)
{
if(i == v0)
cout<<"("<<v0<<":"<<adjlist[v0].info<<")  ";
else
{
print_it(adjlist,parent,v0,parent[i]);
cout<<"("<<i<<":"<<adjlist[i].info<<")  ";
}
}
int _tmain(int argc, _TCHAR* argv[])
{
int cases;
cout<<"请输入案例的个数：";
cin>>cases;
while(cases--)
{
int n,e;
cout<<"请输入节点数：";
cin>>n;
cout<<"请输入边数：";
cin>>e;
//创建邻接表
int v0 = createGraph(adjlist,n,e);
//调用Bellman-Ford算法
bool flag = Bellman_Ford(adjlist,lowcost,parent,n);
int i;
//输出源点到其余每一点的最短路径（节点序号：节点名称）
for(i=1;i<=n;i++)
{
cout<<"从源点"<<adjlist[v0].info<<"到点"<<adjlist[i].info<<"的路径为："<<endl;
print_it(adjlist,parent,v0,i);
cout<<endl;
cout<<"此路径花费为："<<lowcost[i]<<endl;
}
}
system("pause");
return 0;
}
--------------------------------------------------------程序测试--------------------------------------------------------
![](http://hi.csdn.net/attachment/201111/9/0_1320848768BKW8.gif)
请输入案例的个数：1
请输入节点数：5
请输入边数：10
请输入节点1的名称：s
请输入节点2的名称：t
请输入节点3的名称：x
请输入节点4的名称：y
请输入节点5的名称：z
请输入边1的起始节点与尾节点序号：1 2
请输入此边的权值：6
请输入边2的起始节点与尾节点序号：1 4
请输入此边的权值：7
请输入边3的起始节点与尾节点序号：2 3
请输入此边的权值：5
请输入边4的起始节点与尾节点序号：2 4
请输入此边的权值：8
请输入边5的起始节点与尾节点序号：2 5
请输入此边的权值：-4
请输入边6的起始节点与尾节点序号：3 2
请输入此边的权值：-2
请输入边7的起始节点与尾节点序号：4 3
请输入此边的权值：-3
请输入边8的起始节点与尾节点序号：4 5
请输入此边的权值：9
请输入边9的起始节点与尾节点序号：5 1
请输入此边的权值：2
请输入边10的起始节点与尾节点序号：5 3
请输入此边的权值：7
请输入源点序号：1
从源点s到点s的路径为：
(1:s)
此路径花费为：0
从源点s到点t的路径为：
(1:s)  (4:y)  (3:x)  (2:t)
此路径花费为：2
从源点s到点x的路径为：
(1:s)  (4:y)  (3:x)
此路径花费为：4
从源点s到点y的路径为：
(1:s)  (4:y)
此路径花费为：7
从源点s到点z的路径为：
(1:s)  (4:y)  (3:x)  (2:t)  (5:z)
此路径花费为：-2
请按任意键继续. . .

[
](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
