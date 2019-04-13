
# 无向图的广度优先搜索（采用邻接表存储）C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月05日 17:07:36[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：2523


**// 无向图的广度优先搜索（采用邻接表存储）.cpp : Defines the entry point for the console application.**
**//**
**\#include "stdafx.h"**
**\#include<iostream>**
**\#define MAX 100**
**\#define MAXQ 50**
**using namespace std;**
**struct edgeNode**
**{**
**int no; //边端的序号**
**char info; //边端的名称**
**struct edgeNode * next; //下一个**
**};**
**struct vexNode**
**{**
**char info;  //节点名称**
**struct edgeNode *link; //与之相连的端点**
**};**
**//存储节点信息**
**vexNode adjlist[MAX];**
**//循环队列**
**int queue[MAXQ];**
**//访问标志**
**bool visited[MAX];**
**//建立邻接表存储**
**void createGraph(vexNode *adjlist)**
**{**
**int n,e;**
**cout<<"请输入节点数：";**
**cin>>n;**
**cout<<"请输入边数：";**
**cin>>e;**
**int i;**
**for(i=1;i<=n;i++)**
**{**
**cout<<"请输入节点"<<i<<"的名称：";**
**cin>>adjlist[i].info;**
**adjlist[i].link = NULL;**
**}**
**edgeNode *p1,*p2;**
**int v1,v2;**
**for(i=1;i<=e;i++)**
**{**
**cout<<"请输入边"<<i<<"的二端的节点序号：";**
**cin>>v1>>v2;**
**p1 = (edgeNode*)malloc(sizeof(edgeNode));**
**p2 = (edgeNode*)malloc(sizeof(edgeNode));**
**p1->no = v1;**
**p1->info = adjlist[v1].info;**
**p1->next = adjlist[v2].link;**
**adjlist[v2].link = p1;**
**p2->no = v2;**
**p2->info = adjlist[v2].info;**
**p2->next = adjlist[v1].link;**
**adjlist[v1].link = p2;**
**}**
**}**
**//广度优先搜索无向无权图**
**void BFS(vexNode *adjlist,int *queue,bool *visited)**
**{**
**int front,rear,v1;**
**cout<<"请输入从哪个序号的点开始搜索：";**
**cin>>v1;**
**front = 0;**
**rear = 1;**
**queue[rear] = v1;**
**int i;**
**//访问标志清空**
**for(i=1;i<MAX;i++)**
**visited[i] = false;**
**visited[v1] = true;**
**cout<<"广度优先搜索次序为："<<endl;**
**cout<<"节点"<<v1<<",名称"<<adjlist[v1].info<<endl;**
**int vx;**
**edgeNode *p;**
**while(front != rear)**
**{**
**front = (front + 1)%MAXQ;**
**vx = queue[front];**
**p = adjlist[vx].link;**
**while(p!=NULL)**
**{**
**if(!visited[p->no])**
**{**
**visited[p->no] = true;**
**cout<<"节点"<<p->no<<",名称"<<adjlist[p->no].info<<endl;**
**rear = (rear + 1)%MAXQ;**
**queue[rear] = p->no;**
**}**
**p=p->next;**
**}**
**}**
**}**
**int _tmain(int argc, _TCHAR* argv[])**
**{**
**int cases;**
**cout<<"请输入案例的个数：";**
**cin>>cases;**
**while(cases--)**
**{**
**//创建邻接表**
**createGraph(adjlist);**
**//广度优先搜索**
**BFS(adjlist,queue,visited);**
**}**
**system("pause");**
**return 0;**
**}**
**---------------------------------------------程序测试--------------------------------------------------------**
![](http://hi.csdn.net/attachment/201111/5/0_13204903657yzy.gif)
**请输入案例的个数：1**
**请输入节点数：8**
**请输入边数：10**
**请输入节点1的名称：r**
**请输入节点2的名称：s**
**请输入节点3的名称：t**
**请输入节点4的名称：u**
**请输入节点5的名称：v**
**请输入节点6的名称：w**
**请输入节点7的名称：x**
**请输入节点8的名称：y**
**请输入边1的二端的节点序号：1 2**
**请输入边2的二端的节点序号：1 3**
**请输入边3的二端的节点序号：2 4**
**请输入边4的二端的节点序号：3 5**
**请输入边5的二端的节点序号：3 6**
**请输入边6的二端的节点序号：5 6**
**请输入边7的二端的节点序号：5 8**
**请输入边8的二端的节点序号：6 7**
**请输入边9的二端的节点序号：6 8**
**请输入边10的二端的节点序号：7 8**
**请输入从哪个序号的点开始搜索：2**
**广度优先搜索次序为：**
**节点2,名称s**
**节点4,名称u**
**节点1,名称r**
**节点3,名称t**
**节点6,名称w**
**节点5,名称v**
**节点8,名称y**
**节点7,名称x**
**请按任意键继续. . .**

