# Telephone Linse（POJ-3662） - Alex_McAvoy的博客 - CSDN博客





2018年05月29日 20:17:30[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：86








> 
# Problem Description

Farmer John wants to set up a telephone line at his farm. Unfortunately, the phone company is uncooperative, so he needs to pay for some of the cables required to connect his farm to the phone system.

There are N (1 ≤ N ≤ 1,000) forlorn telephone poles conveniently numbered 1..N that are scattered around Farmer John's property; no cables connect any them. A total of P (1 ≤ P ≤ 10,000) pairs of poles can be connected by a cable; the rest are too far apart.

The i-th cable can connect the two distinct poles Ai and Bi, with length Li (1 ≤ Li ≤ 1,000,000) units if used. The input data set never names any {Ai, Bi} pair more than once. Pole 1 is already connected to the phone system, and pole N is at the farm. Poles 1 and N need to be connected by a path of cables; the rest of the poles might be used or might not be used.

As it turns out, the phone company is willing to provide Farmer John with K (0 ≤ K < N) lengths of cable for free. Beyond that he will have to pay a price equal to the length of the longest remaining cable he requires (each pair of poles is connected with a separate cable), or 0 if he does not need any additional cables.

Determine the minimum amount that Farmer John must pay.

# **Input**

Line 1：Three space-separated integers: N, P, and K

Lines 2：P+1: Line i+1 contains the three space-separated integers: Ai, Bi, and Li

# Output

Line 1: A single integer, the minimum amount Farmer John can pay. If it is impossible to connect the farm to the phone company, print -1.

# Sample Input

**5 7 1**

**1 2 53 1 42 4 83 2 35 2 93 4 74 5 6**

# Sample Output

**4**


题意：给出n个点，p条边，可以任选k条边免费。现在要使1到n连通，从1到n的路径上，在不免费的边中最大的长度的费用即支付费用，求最小费用。

思路

由于要求最小费用，所以让公司提供k根比较长的，剩下的由个人提供支付费用，然后从中挑出最长的L，也是说只要是长度比L长的且在这条路上用到的电缆，那么就应该由电信公司提供。 

对于每个方案，假设个人提供的最长电缆长度是L，从1到N找出一条路来，使这条路上的长度比L大的电缆的个数不超过k即可，然后求最短路。

对于所有方案，不断二分查找，使L尽可能的小。 

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1001
#define MOD 2520
#define E 1e-12
using namespace std;
int n,p,k;
int x[N*10],y[N*10],w[N*10];
int head[N],tail;//添加方案时的指针
queue<int>Queue;//队列
int dis[N];//存储最短路路径
bool vis[N];//用于最短路路径判断
struct Node{
    int w;//节点的长度
    int pre;//结点的前驱
    int next;//结点的后继
}a[20005];
void addedge(int mid)
{
    tail=0;
    memset(head,-1,sizeof(head));
    for(int i=0;i<p;i++)
    {
        /*添加从x到y*/
        if(w[i]>mid)
            a[tail].w=1;
        else
            a[tail].w=0;
        a[tail].pre=y[i];
        a[tail].next=head[x[i]];
        head[x[i]]=tail;
        tail++;

        /*添加从y到x*/
        if(w[i]>mid)
            a[tail].w=1;
        else
            a[tail].w=0;
        a[tail].pre=x[i];
        a[tail].next=head[y[i]];
        head[y[i]]=tail;
        tail++;
    }
}
int SPFA()
{
    memset(vis,0,sizeof(vis));
    memset(dis,INF,sizeof(dis));

    dis[1]=0;
    vis[1]=1;
    Queue.push(1);
    while(!Queue.empty())
    {
        int v=Queue.front();
        vis[v]=0;
        Queue.pop();

        for(int i=head[v];i!=-1;i=a[i].next)
        {
            Node temp=a[i];
            if(dis[temp.pre]>dis[v]+temp.w)
            {
                dis[temp.pre]=dis[v]+temp.w;
                if(vis[temp.pre]==0)
                {
                    Queue.push(temp.pre);
                    vis[temp.pre]=1;
                }
            }
        }
    }
    return dis[n];
}

int main()
{
    while(scanf("%d%d%d",&n,&p,&k)!=EOF)
    {
        int maxx=-INF;
        for(int i=0;i<p;i++)
        {
            scanf("%d%d%d",&x[i],&y[i],&w[i]);
            maxx=max(maxx,w[i]);//寻找最大长度
        }

        if(n==1)
            printf("0\n");
        else//二分查找最佳答案
        {
            int left=-1,right=maxx,mid;
            while(right-left>1)
            {
                mid=(right+left)/2;
                addedge(mid);//将当前mid的值视为一种方案添加到队列中
                int minn=SPFA();
                if(minn==INF)
                {
                    right=-1;
                    break;
                }
                else if(minn<=k)
                    right=mid;
                else
                    left=mid;
            }
            printf("%d\n",right);
        }
    }
    return 0;
}
```





