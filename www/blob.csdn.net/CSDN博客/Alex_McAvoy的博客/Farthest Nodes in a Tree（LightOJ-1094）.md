# Farthest Nodes in a Tree（LightOJ-1094） - Alex_McAvoy的博客 - CSDN博客





2019年01月16日 16:25:01[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：181
个人分类：[LightOJ																[树的相关算法——树的直径](https://blog.csdn.net/u011815404/article/category/8616282)](https://blog.csdn.net/u011815404/article/category/8037630)








# Problem Description

Given a tree (a connected graph with no cycles), you have to find the farthest nodes in the tree. The edges of the tree are weighted and undirected. That means you have to find two nodes in the tree whose distance is maximum amongst all nodes.

# **Input**

Input starts with an integer T (≤ 10), denoting the number of test cases.

Each case starts with an integer n (2 ≤ n ≤ 30000) denoting the total number of nodes in the tree. The nodes are numbered from 0 to n-1. Each of the next n-1 lines will contain three integers u v w (0 ≤ u, v < n, u ≠ v, 1 ≤ w ≤ 10000) denoting that node u and v are connected by an edge whose weight is w. You can assume that the input will form a valid tree.

# Output

For each case, print the case number and the maximum distance.Sample Input

Sample Output

# Sample Input

**2**

**4**

**0 1 20**

**1 2 30**

**2 3 50**

**5**

**0 2 20**

**2 1 10**

**0 3 29**

**0 4 50**

# Sample Output

**Case 1: 100**

**Case 2: 80**

————————————————————————————————————————————————————

题意：给出 t 组数据，每组给出 n 个点，这 n 个点组成一棵树，给出树的 n-1 条边与他们的权值，求树的直径

思路：树的直径模版题

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 1000001
#define LL long long
using namespace std;
struct Edge{
    int w;
    int from,to;
    int next;
}edge[N];
int head[N],edgeNum=0;
int n,m;
int dis[N];
bool vis[N];
int start,endd;
void addEdge(int from,int to,int w) {
    edge[edgeNum].to=to;
    edge[edgeNum].w=w;
    edge[edgeNum].next=head[from];
    edge[edgeNum].from=from;
    head[from]=edgeNum++;
}
void dfs(int x) {
    vis[x]=true;
    for(int i=head[x];i!=-1;i=edge[i].next){
        int to=edge[i].to;
        if(!vis[to]) {
            dis[to]=dis[x]+edge[i].w;
            dfs(to);
        }
    }
}
int main()
{
    int t;
    scanf("%d\n",&t);
    int Case=1;
    while(t--){
        scanf("%d",&n);
        m=n-1;

        memset(head,-1,sizeof(head));
        edgeNum=0;

        for(int i=0;i<m;i++){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            addEdge(x,y,w);
            addEdge(y,x,w);
        }

        memset(vis,false,sizeof(vis));
        memset(dis,INF,sizeof(dis));
        dis[0]=0;
        dfs(0);//从1号点开始找最远的点

        int maxx=-INF;
        for(int i=0;i<n;i++){
            if(dis[i]>maxx&&dis[i]!=INF) {
                maxx=dis[i];
                start=i;//树的直径的起点
            }
        }

        memset(vis,false,sizeof(vis));
        memset(dis,INF,sizeof(dis));
        dis[start]=0;
        dfs(start);

        maxx=-INF;
        for(int i=0;i<n;i++)
            if(dis[i]>maxx&&dis[i]!=INF) 
                maxx=dis[i];

        printf("Case %d: %d\n",Case++,maxx);

    }
    return 0;
}
```






