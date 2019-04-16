# POJ2195 Going Home  最小费用最大流 - 紫芝的博客 - CSDN博客





2018年10月05日 17:53:43[紫芝](https://me.csdn.net/qq_40507857)阅读数：94








# [POJ 2195 Going Home 二分图的最大权匹配](https://blog.csdn.net/qq_40507857/article/details/82178862)

# [Going Home](https://vjudge.net/problem/POJ-2195)
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 25567||**Accepted:** 12838|

Description

On a grid map there are n little men and n houses. In each unit time, every little man can move one unit step, either horizontally, or vertically, to an adjacent point. For each little man, you need to pay a $1 travel fee for every step he moves, until he enters a house. The task is complicated with the restriction that each house can accommodate only one little man. 


Your task is to compute the minimum amount of money you need to pay in order to send these n little men into those n different houses. The input is a map of the scenario, a '.' means an empty space, an 'H' represents a house on that point, and am 'm' indicates there is a little man on that point. 

![](http://poj.org/images/2195_1.jpg)



You can think of each point on the grid map as a quite large square, so it can hold n little men at the same time; also, it is okay if a little man steps on a grid with a house without entering that house.

Input

There are one or more test cases in the input. Each case starts with a line giving two integers N and M, where N is the number of rows of the map, and M is the number of columns. The rest of the input will be N lines describing the map. You may assume both N and M are between 2 and 100, inclusive. There will be the same number of 'H's and 'm's on the map; and there will be at most 100 houses. Input will terminate with 0 0 for N and M.

Output

For each test case, output one line with the single integer, which is the minimum amount, in dollars, you need to pay.

Sample Input

```
2 2
.m
H.
5 5
HH..m
.....
.....
.....
mm..H
7 8
...H....
...H....
...H....
mmmHmmmm
...H....
...H....
...H....
0 0
```

Sample Output

```
2
10
28
```

Source

[Pacific Northwest 2004](http://poj.org/searchproblem?field=source&key=Pacific+Northwest+2004)

### **题意：**

有k个人，k间房子，每人进入一个房子，求最小的总距离
**分析：**

关键在于建图：

源点为0，汇点为2*n+1,n为房子个数（房子数等于人数）；

**源点到每个人建立一条容量为1，费用为0的边；**

**每个人到每个房子建立一条容量为1，费用为两者距离的边；**

**每个人到汇点建立一条容量为1，费用为0的边；**

然后跑一遍最小费用最大流即可

![](https://img-blog.csdn.net/20181005175251548?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```cpp
#include<stdio.h>
#include<iostream>
#include<string.h>
#include<queue>
#include<cstdio>
#include<string>
#include<math.h>
#include<algorithm>
#include<map>
#include<set>
#include<stack>
#define mod 998244353
#define INF 0x3f3f3f3f
#define eps 1e-6
using namespace std;
typedef long long ll;
#define MAXN 1003
#define MAXM 40004
//最小费用最大流
struct Edge{
    int to,next;
    int flow,cost,cap;
}edge[MAXM];
int tol,head[MAXN];
void init()
{
    tol=0;
    memset(head,-1,sizeof head);
}
void addEdge(int u,int v,int cap,int cost){
    edge[tol].to=v;
    edge[tol].cap=cap;
    edge[tol].cost=cost;
    edge[tol].flow=0;
    edge[tol].next=head[u];
    head[u]=tol++;

    edge[tol].to=u;
    edge[tol].cap=0;
    edge[tol].cost=-cost;
    edge[tol].flow=0;
    edge[tol].next=head[v];
    head[v]=tol++;
}

bool inq[MAXN];//标记是否点是否在队列
int dis[MAXN];//最短距离
int pre[MAXN];//记录路径
int q[MAXN*10];//队列
//单位费用可能是负值，所以用SPFA
bool spfa(int st,int en)
{
    memset(inq,0,sizeof inq);
    memset(dis,INF,sizeof dis);
    memset(pre,-1,sizeof pre);

    int rear=0,front=0;
    dis[st]=0;
    inq[st]=true;
    q[front++]=st;
    while(rear<front){
        int u=q[rear++];
        inq[u]=false;

        for(int e=head[u];e!=-1;e=edge[e].next){
            int v=edge[e].to;
            if(edge[e].cap>edge[e].flow&&dis[v]>dis[u]+edge[e].cost){
                dis[v]=dis[u]+edge[e].cost;
                pre[v]=e;//表示边e-->v,e就是v的前驱
                if(!inq[v])
                    inq[v]=true,q[front++]=v;
            }
        }
    }
    return pre[en]!=-1;
}
int minCostMaxFlow(int st,int en,int &cost,int &flow)
{
    //如果能找到从源点到汇点的最短路，说明还没有达到最小费用最大流
    while(spfa(st,en)){
        int Min=INF;//最小残余流量
        //沿着当前路径返回
        for(int i=pre[en];i!=-1;i=pre[edge[i^1].to]){
            int rem=edge[i].cap-edge[i].flow;
            Min=Min>rem?rem:Min;
        }
        for(int i=pre[en];i!=-1;i=pre[edge[i^1].to]){
            edge[i].flow+=Min;//正向边添加残余流量
            edge[i^1].flow-=Min;//反向边减少残余流量
            cost+=Min*edge[i].cost;
        }
        flow+=Min;
    }
}
//以上为最小费用最大流模板

int n,m;
struct point{
    int x,y;
}house[MAXN],men[MAXN];
char str[110];
int main()
{
    while(scanf("%d%d",&n,&m)&&n&&m){
        init();
        int houseNum=1,menNum=1;
        for(int i=0;i<n;i++){
            scanf("%s",str);
            for(int j=0;j<m;j++){
                if(str[j]=='H')
                house[houseNum].x=i,house[houseNum].y=j,houseNum++;

                if(str[j]=='m')
                men[menNum].x=i,men[menNum].y=j,menNum++;
            }
        }
        int st=0;
        int en=houseNum+menNum-1;

        for(int i=1;i<menNum;i++){
            for(int j=1;j<houseNum;j++){
                int w=abs(men[i].x-house[j].x)+abs(men[i].y-house[j].y);
                addEdge(i,menNum+j-1,1,w);
            }
        }

        for(int i=1;i<menNum;i++){
            addEdge(0,i,1,0);
            addEdge(menNum+i-1,en,1,0);
        }

        int cost=0,flow=0;
        minCostMaxFlow(st,en,cost,flow);
        printf("%d\n",cost);
    }
}
```



