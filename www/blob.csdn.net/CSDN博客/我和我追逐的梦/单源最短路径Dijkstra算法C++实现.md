
# 单源最短路径Dijkstra算法C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月09日 20:26:34[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：3601


**// 单源最短路径Dijkstra算法实现.cpp : Defines the entry point for the console application.**
**//**
**\#include "stdafx.h"**
**\#include<iostream>**
**\#define MAX 200**
**\#define Infinity 65535**
**using namespace std;**
**//边尾节点信息结构体**
**struct edgeNode**
**{**
**int no;   //尾接点序号**
**int cost;  //边权值**
**edgeNode *next; //其下一条邻接边尾节点指针**
**};**
**//节点信息结构体**
**struct vexNode**
**{**
**char info;  //节点名称**
**edgeNode *link; //与其相连的边的尾节点链表指针**
**};**
**struct Queue**
**{**
**int no; //队列中节点序号**
**int cost; //以此为尾节点的边的权值**
**};**
**//优先队列**
**Queue priQue[MAX];**
**//节点数组**
**vexNode adjlist[MAX];**
**//指定源点到节点i的最短路径花费**
**int lowcost[MAX];**
**//指定源点到节点i路径中，节点i的前驱节点序号**
**int parent[MAX];**
**//建立图邻接表**
**void createGraph(vexNode *adjlist,int *parent,int * lowcost,const int n,const int e)**
**{**
**int i;**
**for(i=1;i<=n;i++)**
**{**
**cout<<"请输入节点"<<i<<"的名称：";**
**cin>>adjlist[i].info;**
**adjlist[i].link = NULL;**
**lowcost[i] = Infinity;**
**parent[i] = i;**
**}**
**edgeNode *p1;**
**int v1,v2;**
**for(i=1;i<=e;i++)**
**{**
**cout<<"请输入边"<<i<<"的起始节点与尾节点序号：";**
**cin>>v1>>v2;**
**p1 = (edgeNode*)malloc(sizeof(edgeNode));**
**p1->no = v2;**
**cout<<"此边的权值：";**
**cin>>p1->cost;**
**p1->next = adjlist[v1].link;**
**adjlist[v1].link = p1;**
**}**
**}**
**//当插入节点到优先队列时，保持队列优先性**
**void keep_min_heap(Queue *priQue,int &num,const int k)**
**{**
**int l = 2*k;**
**int r = 2*k + 1;**
**int smallest = k;**
**if(l<=num&&priQue[l].cost<priQue[k].cost)**
**smallest = l;**
**if(r<=num&&priQue[r].cost<priQue[smallest].cost)**
**smallest = r;**
**if(smallest != k)**
**{**
**Queue temp = priQue[smallest];**
**priQue[smallest] = priQue[k];**
**priQue[k] = temp;**
**keep_min_heap(priQue,num,smallest);**
**}**
**}**
**//插入节点到优先队列时并且保持队列优先性**
**void heap_insert(Queue *priQue,int &num,int no,int cost)**
**{**
**num +=1;**
**priQue[num].no = no;**
**priQue[num].cost = cost;**
**int i = num;**
**while(i>1&&priQue[i/2].cost>priQue[i].cost)**
**{**
**Queue temp = priQue[i];**
**priQue[i] = priQue[i/2];**
**priQue[i/2] = temp;**
**i = i/2;**
**}**
**}**
**//取出优先队列的队头元素**
**Queue heap_extract_min(Queue *priQue,int &num)**
**{**
**if(num<1)**
**return priQue[0];**
**Queue min = priQue[1];**
**priQue[1] = priQue[num];**
**num -=1;**
**keep_min_heap(priQue,num,1);**
**return min;**
**}**
**//打印指定源点带序号为i的点的最短路径**
**void print_it(int *parent,vexNode *adjlist,int v)**
**{**
**if(parent[v] == v)**
**cout<<"("<<v<<":"<<adjlist[v].info<<") ";**
**else**
**{**
**print_it(parent,adjlist,parent[v]);**
**cout<<"("<<v<<":"<<adjlist[v].info<<") ";**
**}**
**}**
**int _tmain(int argc, _TCHAR* argv[])**
**{**
**int cases;**
**cout<<"请输入案例的个数：";**
**cin>>cases;**
**while(cases--)**
**{**
**int n,e;**
**cout<<"请输入节点数：";**
**cin>>n;**
**cout<<"请输入边数：";**
**cin>>e;**
**//队列中的元素，初始为0**
**int num = 0;**
**int i;**
**//创建邻接表**
**createGraph(adjlist,parent,lowcost,n,e);**
**cout<<endl;**
**cout<<"从哪个节点开始：";**
**int v0;**
**cin>>v0;**
**int v =v0;**
**lowcost[v0] = 0;**
**cout<<endl;**
**Queue queue;**
**for(i=1;i<n;i++)**
**{**
**edgeNode *p = adjlist[v0].link;**
**while(p != NULL)**
**{**
**if(lowcost[v0] + p->cost<lowcost[p->no])**
**{**
**lowcost[p->no] = lowcost[v0] + p->cost;**
**parent[p->no] = v0;**
**heap_insert(priQue,num,p->no,lowcost[p->no]);**
**}**
**p = p->next;**
**}**
**queue = heap_extract_min(priQue,num);**
**v0 = queue.no;**
**}**
**for(i=1;i<=n;i++)**
**{**
**mincost = 0;**
**cout<<"从点"<<adjlist[v].info<<"开始到"<<adjlist[i].info<<"的最短路径为："<<endl;**
**print_it(parent,adjlist,i);**
**cout<<endl;**
**cout<<"距离为："<<lowcost[i]<<endl;**
**}**
**}**
**system("pause");**
**return 0;**
**}**
**--------------------------------------------------程序测试-----------------------------------------------------**![](http://hi.csdn.net/attachment/201111/9/0_1320841120hMLM.gif)
**请输入案例的个数：1**
**请输入节点数：5**
**请输入边数：10**
**请输入节点1的名称：a**
**请输入节点2的名称：b**
**请输入节点3的名称：c**
**请输入节点4的名称：d**
**请输入节点5的名称：e**
**请输入边1的起始节点与尾节点序号：1 2**
**此边的权值：10**
**请输入边2的起始节点与尾节点序号：1 4**
**此边的权值：5**
**请输入边3的起始节点与尾节点序号：2 3**
**此边的权值：1**
**请输入边4的起始节点与尾节点序号：2 4**
**此边的权值：2**
**请输入边5的起始节点与尾节点序号：3 5**
**此边的权值：4**
**请输入边6的起始节点与尾节点序号：4 2**
**此边的权值：3**
**请输入边7的起始节点与尾节点序号：4 3**
**此边的权值：9**
**请输入边8的起始节点与尾节点序号：4 5**
**此边的权值：2**
**请输入边9的起始节点与尾节点序号：5 1**
**此边的权值：7**
**请输入边10的起始节点与尾节点序号：5 3**
**此边的权值：6**
**从哪个节点开始：1**
**从点a开始到a的最短路径为：**
**(1:a)**
**距离为：0**
**从点a开始到b的最短路径为：**
**(1:a) (4:d) (2:b)**
**距离为：8**
**从点a开始到c的最短路径为：**
**(1:a) (4:d) (2:b) (3:c)**
**距离为：9**
**从点a开始到d的最短路径为：**
**(1:a) (4:d)**
**距离为：5**
**从点a开始到e的最短路径为：**
**(1:a) (4:d) (5:e)**
**距离为：7**
**请按任意键继续. . .**

