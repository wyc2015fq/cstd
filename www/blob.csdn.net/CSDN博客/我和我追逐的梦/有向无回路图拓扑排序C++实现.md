
# 有向无回路图拓扑排序C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月05日 23:24:19[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2431


// 有向无回路图拓扑排序.cpp : Defines the entry point for the console application.
//
\#include "stdafx.h"
\#include<iostream>
\#define MAX 100
using namespace std;
enum Color{white,gray,black};
struct edgeNode
{
int no; //边尾端的序号
char info; //边端的名称
struct edgeNode * next; //下一个
};
struct vexNode
{
char info;  //节点名称
struct edgeNode *link; //与之相连的端点
};
//存储节点信息
vexNode adjlist[MAX];
//访问层次
//还没访问为white，访问了但是还没完成它的所有后裔的搜索为gray
//完成它的所有后裔的搜索为black
Color color[MAX];
//访问的开始时间
int d[MAX];
//访问的完成时间
int f[MAX];
//前驱节点
int parent[MAX];
//拓扑排序后的数组，按照f[]的大小排序，即先完成搜索的节点排在前面
int topu[MAX];
//建立邻接表存储
//adjlist为节点集，n为节点个数，e为边数目
void createGraph(vexNode *adjlist,int n,int e)
{
int i;
for(i=1;i<=n;i++)
{
cout<<"请输入节点"<<i<<"的名称：";
cin>>adjlist[i].info;
adjlist[i].link = NULL;
}
edgeNode *p1;
int v1,v2;
for(i=1;i<=e;i++)
{
cout<<"请输入边"<<i<<"的起始节点序号：";
cin>>v1;
cout<<"请输入边"<<i<<"的尾节点序号：";
cin>>v2;
p1 = (edgeNode*)malloc(sizeof(edgeNode));
p1->no = v2;
p1->info = adjlist[v2].info;
p1->next = adjlist[v1].link;
adjlist[v1].link = p1;
}
}
//深度优先搜索有向无权图
//parent[i]为节点i前驱节点，time为一个全局时间戳，v是第几个节点，index是topu数组的全局偏移
void DFS(vexNode *adjlist,int *parent,int &time,int v,int &index)
{
color[v] = gray;
time += 1;
d[v] = time;
int i;
edgeNode *p;
p = adjlist[v].link;
while(p != NULL)
{
if(color[p->no]==white)
{
parent[p->no] = v;
DFS(adjlist,parent,time,p->no,index);
}
p = p->next;
}
color[v] = black;
time += 1;
f[v] = time;
topu[index++] = v;
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
//访问节点的时间戳
int time = 0;
//index是topu数组的全局偏移
int index = 1;
//访问标志清空与前驱节点都初始化为0
int i;
for(i=1;i<=n;i++)
{
color[i] = white;
parent[i] = 0;
}
//创建邻接表
createGraph(adjlist,n,e);
for(i=1;i<=n;i++)
{
if(color[i]==white)
DFS(adjlist,parent,time,i,index);
}
cout<<"输出拓扑排序结果："<<endl;
for(i=1;i<=n;i++)
cout<<adjlist[topu[i]].info<<"  ";
cout<<endl;
}
system("pause");
return 0;
}
----------------------------------------------------程序测试---------------------------------------------------
![](http://hi.csdn.net/attachment/201111/5/0_1320506539j1Z1.gif)
请输入案例的个数：1
请输入节点数：9
请输入边数：9
请输入节点1的名称：a
请输入节点2的名称：b
请输入节点3的名称：c
请输入节点4的名称：d
请输入节点5的名称：e
请输入节点6的名称：f
请输入节点7的名称：g
请输入节点8的名称：h
请输入节点9的名称：i
请输入边1的起始节点序号：1
请输入边1的尾节点序号：2
请输入边2的起始节点序号：1
请输入边2的尾节点序号：4
请输入边3的起始节点序号：2
请输入边3的尾节点序号：3
请输入边4的起始节点序号：4
请输入边4的尾节点序号：3
请输入边5的起始节点序号：5
请输入边5的尾节点序号：6
请输入边6的起始节点序号：5
请输入边6的尾节点序号：8
请输入边7的起始节点序号：6
请输入边7的尾节点序号：4
请输入边8的起始节点序号：6
请输入边8的尾节点序号：8
请输入边9的起始节点序号：7
请输入边9的尾节点序号：8
输出拓扑排序结果：
c  d  b  a  h  f  e  g  i
请按任意键继续. . .

