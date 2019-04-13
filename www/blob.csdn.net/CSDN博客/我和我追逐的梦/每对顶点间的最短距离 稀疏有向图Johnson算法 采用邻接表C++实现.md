
# 每对顶点间的最短距离 稀疏有向图Johnson算法 采用邻接表C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月16日 16:42:57[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1356标签：[算法																](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[c++																](https://so.csdn.net/so/search/s.do?q=c++&t=blog)[struct																](https://so.csdn.net/so/search/s.do?q=struct&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[c																](https://so.csdn.net/so/search/s.do?q=c&t=blog)[insert																](https://so.csdn.net/so/search/s.do?q=insert&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=c&t=blog)[
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
**每对顶点间的最短距离 稀疏有向图Johnson算法 C++实现**
**// 稀疏有向图Johnson算法.cpp : Defines the entry point for the console application.**
**//**
**\#include "stdafx.h"**
**\#include<iostream>**
**\#define Infinity 65535**
**\#define MAX 100**
**using namespace std;**
**//边尾节点结构体**
**struct edgeNode**
**{**
**int no;//节点序号**
**int weight; //此边权值**
**edgeNode *next; //下一条邻接边**
**};**
**//节点信息**
**struct vexNode**
**{**
**char info; //节点序号**
**edgeNode *link; //与此节点为首节点的边的尾节点链表**
**};**
**//优先队列元素结构体**
**struct PriQue**
**{**
**int no;  //节点元素序号**
**int weight;  //源点到此节点的权值**
**};**
**//节点数组**
**vexNode adjlist[MAX];**
**//添加一个序号为0节点到其他各节点的最小权值**
**int d[MAX];**
**//源点到各节点的最小权值**
**int lowcost[MAX];**
**//各节点对间的最小权值**
**int mincost[MAX][MAX];**
**//优先队列**
**PriQue queue[2*MAX];**
**//建立图的邻接表**
**void createGraph(vexNode *adjlist,int n,int e)**
**{**
**int i;**
**cout<<"请输入这些节点的信息："<<endl;**
**for(i=1;i<=n;i++)**
**{**
**cout<<"节点"<<i<<"的名称:";**
**cin>>adjlist[i].info;**
**adjlist[i].link = NULL;**
**}**
**cout<<"请输入这些边的信息："<<endl;**
**int v1,v2;**
**edgeNode *p1;**
**int weight1;**
**for(i=1;i<=e;i++)**
**{**
**cout<<"边"<<i<<"的首尾节点：";**
**cin>>v1>>v2;**
**cout<<"请输入此边的权值：";**
**cin>>weight1;**
**p1 = (edgeNode*)malloc(sizeof(edgeNode));**
**p1->no = v2;**
**p1->weight = weight1;**
**p1->next = adjlist[v1].link;**
**adjlist[v1].link = p1;**
**}**
**//添加节点0，到每一个节点的距离都是0**
**adjlist[0].info ='0';**
**adjlist[0].link = NULL;**
**for(i=n;i>=1;i--)**
**{**
**d[i] = 0;**
**p1 = (edgeNode*)malloc(sizeof(edgeNode));**
**p1->no = i;**
**p1->weight = 0;**
**p1->next = adjlist[0].link;**
**adjlist[0].link = p1;**
**}**
**}**
**//bellman_ford算法求节点0到其他各节点的最短距离**
**bool bellman_ford(vexNode *adjlist,int *d,int n)**
**{**
**int i,j;**
**d[0] = 0;**
**edgeNode *p1;**
**for(j=1;j<=n;j++)**
**{**
**for(i=0;i<=n;i++)**
**{**
**p1= adjlist[i].link;**
**while(p1 != NULL)**
**{**
**if(d[p1->no]>d[i]+p1->weight)**
**d[p1->no] = d[i] + p1->weight;**
**p1 = p1->next;**
**}**
**}**
**}**
**for(i=0;i<=n;i++)**
**{**
**p1= adjlist[i].link;**
**while(p1 != NULL)**
**{**
**if(d[p1->no]>d[i]+p1->weight)**
**return false;**
**p1 = p1->next;**
**}**
**}**
**return true;**
**}**
**//johnson算法中，需要对每一条边重新赋权值产生非负的权**
**void G_w_to_G1_w1(int *d,const int n)**
**{**
**int i;**
**edgeNode *p1;**
**for(i=0;i<=n;i++)**
**{**
**p1= adjlist[i].link;**
**while(p1 != NULL)**
**{**
**p1->weight = p1->weight + d[i] - d[p1->no];**
**p1 = p1->next;**
**}**
**}**
**}**
**//保持优先队列的优先性，以指定源点到每一点的最少距离为关键字**
**void keep_heap(PriQue *queue,int &num,int i)**
**{**
**int smallest = i;**
**int left = 2*i,right = 2*i+1;**
**if(left<=num&&queue[left].weight<queue[i].weight)**
**smallest = left;**
**if(right<=num&&queue[right].weight<queue[smallest].weight)**
**smallest = right;**
**if(smallest != i)**
**{**
**PriQue q = queue[smallest];**
**queue[smallest] = queue[i];**
**queue[i] = q;**
**keep_heap(queue,num,smallest);**
**}**
**}**
**//插入一个元素到优先队列中，并保持队列优先性**
**void insert_heap(PriQue *queue,int &num,int no,int wei)**
**{**
**num += 1;**
**queue[num].no = no;**
**queue[num].weight = wei;**
**int i = num;**
**while(i>1&&queue[i].weight<queue[i/2].weight)**
**{**
**PriQue q1;**
**q1 = queue[i/2];**
**queue[i/2] = queue[i];**
**queue[i] = q1;**
**i = i/2;**
**}**
**}**
**//取出队列首元素**
**PriQue heap_extract_min(PriQue *queue,int &num)**
**{**
**if(num<1)**
**return queue[0];**
**PriQue que = queue[1];**
**queue[1] = queue[num];**
**num = num -1;**
**keep_heap(queue,num,1);**
**return que;**
**}**
**//dijkstra算法求节点i到其他每一个节点的最短距离**
**void dijkstra(vexNode *adjlist,PriQue * queue,int i,const int n,int &num)**
**{**
**int v = i;**
**//lowcost[v] = 0;**
**int j;**
**for(j=1;j<n;j++)**
**{**
**edgeNode *p1 = adjlist[v].link;**
**while(p1 != NULL)**
**{**
**if(lowcost[p1->no] > lowcost[v] + p1->weight)**
**{**
**lowcost[p1->no] = lowcost[v] + p1->weight;**
**insert_heap(queue,num,p1->no,lowcost[p1->no]);**
**}**
**p1 = p1->next;**
**}**
**v = heap_extract_min(queue,num).no;**
**if(v==0)**
**{**
**cout<<"队列中没有节点！"<<endl;**
**return;**
**}**
**}**
**}**
**int _tmain(int argc, _TCHAR* argv[])**
**{**
**int cases;**
**cout<<"请输入案例的个数：";**
**cin>>cases;**
**//用队列0元素作为哨兵，如果队列中没有元素，则返回队列0元素**
**queue[0].no = 0;**
**queue[0].weight = 0;**
**while(cases--)**
**{**
**int n,e;**
**cout<<"请输入节点数：";**
**cin>>n;**
**cout<<"请输入边数：";**
**cin>>e;**
**//队列中的元素，初始为0**
**int num = 0;**
**int i,j;**
**//创建邻接表**
**createGraph(adjlist,n,e);**
**cout<<endl;**
**memset(d,Infinity,sizeof(d));**
**//bellman_ford算法求节点0到其他各节点的最短距离**
**bool flag = bellman_ford(adjlist,d,n);**
**if(!flag)**
**{**
**cout<<"此图存在负回路，不正确！"<<endl;**
**continue;**
**}**
**//johnson算法中，需要对每一条边重新赋权值产生非负的权**
**G_w_to_G1_w1(d,n);**
**//运用dijkstra算法求得每一对节点间的最短距离**
**for(i=1;i<=n;i++)**
**{**
**for(j=1;j<=n;j++)**
**lowcost[j] = Infinity;**
**lowcost[i] =0;**
**dijkstra(adjlist,queue,i,n,num);**
**//重新把原值赋值回来，因为在函数G_w_to_G1_w1()中改变过**
**for(j=1;j<=n;j++)**
**mincost[i][j] = lowcost[j] + d[j] - d[i];**
**}**
**cout<<"下面输出每一对顶点之间的最短距离："<<endl;**
**for(i=1;i<=n;i++)**
**for(j=1;j<=n;j++)**
**{**
**cout<<"顶点("<<i<<":"<<adjlist[i].info<<")到顶点("<<j<<":"<<adjlist[j].info<<")的最短距离为："<<mincost[i][j]<<endl;**
**}**
**}**
**system("pause");**
**return 0;**
**}**
**-------------------------------------------程序测试----------------------------------------------------------**
**请输入案例的个数：1**
**请输入节点数：5**
**请输入边数：9**
**请输入这些节点的信息：**
**节点1的名称:a**
**节点2的名称:b**
**节点3的名称:c**
**节点4的名称:d**
**节点5的名称:e**
**请输入这些边的信息：**
**边1的首尾节点：1 2**
**请输入此边的权值：3**
**边2的首尾节点：1 3**
**请输入此边的权值：8**
**边3的首尾节点：1 5**
**请输入此边的权值：-4**
**边4的首尾节点：2 4**
**请输入此边的权值：1**
**边5的首尾节点：2 5**
**请输入此边的权值：7**
**边6的首尾节点：3 2**
**请输入此边的权值：4**
**边7的首尾节点：4 1**
**请输入此边的权值：2**
**边8的首尾节点：4 3**
**请输入此边的权值：-5**
**边9的首尾节点：5 4**
**请输入此边的权值：6**
**下面输出每一对顶点之间的最短距离：**
**顶点(1:a)到顶点(1:a)的最短距离为：0**
**顶点(1:a)到顶点(2:b)的最短距离为：1**
**顶点(1:a)到顶点(3:c)的最短距离为：-3**
**顶点(1:a)到顶点(4:d)的最短距离为：2**
**顶点(1:a)到顶点(5:e)的最短距离为：-4**
**顶点(2:b)到顶点(1:a)的最短距离为：3**
**顶点(2:b)到顶点(2:b)的最短距离为：0**
**顶点(2:b)到顶点(3:c)的最短距离为：-4**
**顶点(2:b)到顶点(4:d)的最短距离为：1**
**顶点(2:b)到顶点(5:e)的最短距离为：-1**
**顶点(3:c)到顶点(1:a)的最短距离为：7**
**顶点(3:c)到顶点(2:b)的最短距离为：4**
**顶点(3:c)到顶点(3:c)的最短距离为：0**
**顶点(3:c)到顶点(4:d)的最短距离为：5**
**顶点(3:c)到顶点(5:e)的最短距离为：3**
**顶点(4:d)到顶点(1:a)的最短距离为：2**
**顶点(4:d)到顶点(2:b)的最短距离为：-1**
**顶点(4:d)到顶点(3:c)的最短距离为：-5**
**顶点(4:d)到顶点(4:d)的最短距离为：0**
**顶点(4:d)到顶点(5:e)的最短距离为：-2**
**顶点(5:e)到顶点(1:a)的最短距离为：8**
**顶点(5:e)到顶点(2:b)的最短距离为：5**
**顶点(5:e)到顶点(3:c)的最短距离为：1**
**顶点(5:e)到顶点(4:d)的最短距离为：6**
**顶点(5:e)到顶点(5:e)的最短距离为：0**
**请按任意键继续. . .**

[
](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
