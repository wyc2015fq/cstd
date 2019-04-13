
# 最小生成树Kruskal算法实现C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月08日 18:06:56[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：5032



**// Kruskal算法实现.cpp : Defines the entry point for the console application.**
**//**
**\#include "stdafx.h"**
**\#include<iostream>**
**\#define MAX 100**
**typedef  int WeiType;**
**using namespace std;**
**//**
**struct Edge**
**{**
**int no;   //边的序号**
**int x;    //端点1序号**
**int y;    // 端点2序号**
**WeiType weight;   //权值**
**bool selected; //是否被选择**
**};**
**//边集和**
**Edge edge[MAX];**
**//已找到的最小生成树其中一部分的秩**
**int rank[MAX];**
**//已找到的最小生成树其中一部分的头结点**
**//用来判断一条边的2个端点是否在一个集合中，即加上这条边是否会形成回路**
**int parent[MAX];**
**//找出每一集合的头结点**
**int find_set(int x)**
**{**
**if(x != parent[x] )**
**parent[x] = find_set(parent[x]);**
**return parent[x];**
**}**
**//合并集合**
**void union_set(int x,int y,WeiType w,WeiType &sum)**
**{**
**if(x==y)**
**return;**
**if(rank[x]>rank[y])**
**parent[y]=x;**
**else**
**{**
**if(rank[x]==rank[y])**
**rank[y]++;**
**parent[x]=y;**
**}**
**sum +=w;**
**}**
**//依据边的权值升序快速排序**
**void fast_sort(Edge *edge,int begin,int end)**
**{**
**if(begin<end)**
**{**
**int i = begin-1,j=begin;**
**edge[0] = edge[end];**
**while(j<end)**
**{**
**if(edge[j].weight<edge[0].weight)**
**{**
**i++;**
**Edge temp1 = edge[i];**
**edge[i] = edge[j];**
**edge[j] = temp1;**
**}**
**j++;**
**}**
**Edge temp2 = edge[end];**
**edge[end] = edge[i+1];**
**edge[i+1] = temp2;**
**fast_sort(edge,begin,i);**
**fast_sort(edge,i+2,end);**
**}**
**}**
**int _tmain(int argc, _TCHAR* argv[])**
**{**
**int cases;**
**cout<<"请输入案例的个数：";**
**cin>>cases;**
**while(cases--)**
**{**
**int n;**
**//最小生成树的权值总和**
**WeiType sum = 0;**
**cout<<"请输入边的个数：";**
**cin>>n;**
**int i;**
**//初始化**
**char chx,chy;**
**WeiType weight;**
**for(i=1;i<=n;i++)**
**{**
**edge[i].no = i;**
**cout<<"请输入第"<<i<<"条边的二个端点的名称(小写字符)：";**
**cin>>chx>>chy;**
**edge[i].x = chx-'a'+1;**
**edge[i].y = chy-'a'+1;**
**cout<<"这条边的权值为：";**
**cin>>weight;**
**edge[i].weight = weight;**
**edge[i].selected = false;**
**parent[edge[i].x] = edge[i].x;**
**parent[edge[i].y] = edge[i].y;**
**rank[edge[i].x] = 0;**
**rank[edge[i].y] = 0;**
**}**
**//快排**
**fast_sort(edge,1,n);**
**for(i=1;i<=n;i++)**
**{**
**int x,y;**
**x = find_set(edge[i].x);**
**y = find_set(edge[i].y);**
**//判断即加上这条边是否会形成回路**
**if(x != y )**
**{**
**//选择这条边**
**edge[i].selected = true;**
**//合并不会形成回路的二个集合**
**union_set(x,y,edge[i].weight,sum);**
**}**
**}**
**cout<<"最小生成树的边集为："<<endl;**
**for(i=1;i<=n;i++)**
**{**
**if(edge[i].selected)**
**{**
**cout<<"序号："<<edge[i].no<<"  " <<"端点1："<<(char)(edge[i].x+'a'-1)<<",端点2："<<(char)(edge[i].y+'a'-1)<<endl;**
**}**
**}**
**cout<<"最小生成树的权值为："<<sum<<endl;**
**}**
**system("pause");**
**return 0;**
**}**
**-------------------------------------------程序测试-------------------------------------------**
![](http://hi.csdn.net/attachment/201111/8/0_1320746882OilI.gif)
**请输入案例的个数：1**
**请输入边的个数：14**
**请输入第1条边的二个端点的名称(小写字符)：a b**
**这条边的权值为：4**
**请输入第2条边的二个端点的名称(小写字符)：a h**
**这条边的权值为：8**
**请输入第3条边的二个端点的名称(小写字符)：b c**
**这条边的权值为：8**
**请输入第4条边的二个端点的名称(小写字符)：b h**
**这条边的权值为：11**
**请输入第5条边的二个端点的名称(小写字符)：c d**
**这条边的权值为：7**
**请输入第6条边的二个端点的名称(小写字符)：c f**
**这条边的权值为：4**
**请输入第7条边的二个端点的名称(小写字符)：c i**
**这条边的权值为：2**
**请输入第8条边的二个端点的名称(小写字符)：d e**
**这条边的权值为：9**
**请输入第9条边的二个端点的名称(小写字符)：d f**
**这条边的权值为：14**
**请输入第10条边的二个端点的名称(小写字符)：e f**
**这条边的权值为：10**
**请输入第11条边的二个端点的名称(小写字符)：f g**
**这条边的权值为：2**
**请输入第12条边的二个端点的名称(小写字符)：g h**
**这条边的权值为：1**
**请输入第13条边的二个端点的名称(小写字符)：g i**
**这条边的权值为：6**
**请输入第14条边的二个端点的名称(小写字符)：h i**
**这条边的权值为：7**
**最小生成树的边集为：**
**序号：12  端点1：g,端点2：h**
**序号：11  端点1：f,端点2：g**
**序号：7  端点1：c,端点2：i**
**序号：1  端点1：a,端点2：b**
**序号：6  端点1：c,端点2：f**
**序号：5  端点1：c,端点2：d**
**序号：3  端点1：b,端点2：c**
**序号：8  端点1：d,端点2：e**
**最小生成树的权值为：37**
**请按任意键继续. . .**

