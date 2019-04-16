# LightOJ - 1074 Extended Traffic (负环) - 紫芝的博客 - CSDN博客





2018年10月04日 21:32:20[紫芝](https://me.csdn.net/qq_40507857)阅读数：29
个人分类：[最短路算法](https://blog.csdn.net/qq_40507857/article/category/7617885)









## Extended Traffic

[LightOJ - 1074](https://cn.vjudge.net/problem/26791/origin)

Dhaka city is getting crowded and noisy day by day. Certain roads always remain blocked in congestion. In order to convince people avoid shortest routes, and hence the crowded roads, to reach destination, the city authority has made a new plan. Each junction of the city is marked with a positive integer **(≤ 20)** denoting the busyness of the junction. Whenever someone goes from one junction (the source junction) to another (the destination junction), the city authority gets the amount **(busyness of destination - busyness of source)3** (that means the cube of the difference) from the traveler. The authority has appointed you to find out the minimum total amount that can be earned when someone intelligent goes from a certain junction (the zero point) to several others.

Input

Input starts with an integer **T (****≤ 50)**, denoting the number of test cases.

Each case contains a blank line and an integer **n (1 < n ≤ 200)** denoting the number of junctions. The next line contains **n** integers denoting the busyness of the junctions from **1** to **n** respectively. The next line contains an integer **m**, the number of roads in the city. Each of the next **m** lines (one for each road) contains two junction-numbers (source, destination) that the corresponding road connects (all roads are unidirectional). The next line contains the integer **q**, the number of queries. The next **q** lines each contain a destination junction-number. There can be at most one direct road from a junction to another junction.

Output

For each case, print the case number in a single line. Then print **q** lines, one for each query, each containing the minimum total earning when one travels from junction **1** (the zero point) to the given junction. However, for the queries that gives total earning less than **3**, or if the destination is not reachable from the zero point, then print a** '?'**.

Sample Input

2



5

6 7 8 9 10

6

1 2

2 3

3 4

1 5

5 4

4 5

2

4

5



2

10 10

1

1 2

1

2

Sample Output

Case 1:

3

4

Case 2:

?

### 题目大意：

给出每个点的权值和边，假设边为(u, v)，那么该边的权值就是(val[v] - val[u]) ^ 3 

问从点1出发，到达所给点的最短路，如果距离小于3或者无法到达就输出 “？”

### 解题思路：

有可能会出现负环，出现负环的话，那么和这一点相连的点的距离就会无限小，所以使用dfs搜索，将负环内的点所能到达的所有点都标记一下；

**在SPFA算法过程中，如果一个点的入队次数超过n次，说明出现负环；**

详情请见：***[POJ 3259 Wormholes 最短路SPFA 判断负环](https://blog.csdn.net/qq_40507857/article/details/82930760)***

**邻接表存图，实现简单，浪费时间**

```cpp
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

const int MAXN = 220;
const int INF = 0x3f3f3f3f;
struct Edge
{
    int v,cost;
    Edge(int _v = 0, int _cost = 0)
    {
        v = _v;
        cost = _cost;
    }
};
vector<Edge>E[MAXN];
void addedge(int u,int v,int w)
{
    E[u].push_back(Edge(v,w));
}

bool vis[MAXN];//标记节点是否在队列
int cnt[MAXN];//节点入队次数
int dist[MAXN];//最短距离

bool cir[MAXN];//节点是否在负环中

//将负环内的点所能到达的所有点都标记一下；
void dfs(int u)
{
    cir[u] = true;
    for(int i = 0;i < E[u].size();i++)
        if(!cir[E[u][i].v])
            dfs(E[u][i].v);
}

void SPFA(int start,int n)
{
    memset(vis,false,sizeof(vis));
    for(int i = 1;i <= n;i++)
        dist[i] = INF;
    vis[start] = true;
    dist[start] = 0;
    queue<int>que;
    while(!que.empty())que.pop();
    que.push(start);
    memset(cnt,0,sizeof(cnt));
    cnt[start] = 1;
    memset(cir,false,sizeof(cir));
    while(!que.empty())
    {
        int u = que.front();
        que.pop();
        vis[u] = false;
        for(int i = 0;i < E[u].size();i++)
        {
            int v = E[u][i].v;
            if(cir[v])continue;
            if(dist[v] > dist[u] + E[u][i].cost)
            {
                dist[v] = dist[u] + E[u][i].cost;
                if(!vis[v])
                {
                    vis[v] = true;
                    que.push(v);
                    cnt[v]++;
                    if(cnt[v] > n)
                        dfs(v);
                }
            }
        }
    }
}
int a[MAXN];
int main()
{
    //freopen("in.txt","r",stdin);
    //freopen("out.txt","w",stdout);
    int T;
    int iCase = 0;
    scanf("%d",&T);
    while(T--)
    {
        iCase++;
        int n;
        scanf("%d",&n);
        for(int i = 1;i <= n;i++)
            scanf("%d",&a[i]);
        int m;
        int u,v;
        for(int i = 1;i <= n;i++)
            E[i].clear();
        scanf("%d",&m);
        while(m--)
        {
            scanf("%d%d",&u,&v);
            addedge(u,v,(a[v]-a[u])*(a[v]-a[u])*(a[v]-a[u]));
        }
        SPFA(1,n);
        printf("Case %d:\n",iCase);
        scanf("%d",&m);
        while(m--)
        {
            scanf("%d",&u);
            if(cir[u] || dist[u] < 3 || dist[u] == INF)
                printf("?\n");
            else printf("%d\n",dist[u]);
        }
    }
    return 0;
}
```

**链式前向星存图，似乎能快一点**

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
//#define mod 998244353
#define INF 0x3f3f3f3f
#define eps 1e-6
using namespace std;
typedef long long ll;
const int mod=1e9+7;
const int maxn=1000;
int n;
int cnt;
int head[maxn];
struct Edge
{
    int to;
    int next;
    int val;
}edge[maxn];

void addEdge(int from,int to,int val)
{
    edge[cnt].to=to;
    edge[cnt].val=val;
    edge[cnt].next=head[from];
    head[from]=cnt++;
}
bool inq[maxn];
int dis[maxn];

int qnum[maxn];//节点入队次数
bool cir[maxn];//标记负环中的点

void dfs(int x)
{
    cir[x]=true;
    for(int i=head[x];i!=-1;i=edge[i].next){
        int to=edge[i].to;
        if(!cir[to])    dfs(to);
    }
}
void spfa(int start)
{
    memset(cir,0,sizeof cir);
    memset(inq,0,sizeof inq);
    memset(dis,INF,sizeof dis);
    memset(qnum,0,sizeof qnum);
    int top=0;

    queue<int>q;
    q.push(start);

    inq[start]=true;
    dis[start]=0;
    qnum[start]++;
    while(!q.empty()){
        int cur=q.front();
        q.pop();
        inq[cur]=false;

        for(int i=head[cur];i!=-1;i=edge[i].next){
            int to=edge[i].to;
            if(cir[to]) continue;
            if(dis[to]>dis[cur]+edge[i].val){
                dis[to]=dis[cur]+edge[i].val;
                if(!inq[to]){
                 inq[to]=1,q.push(to),qnum[to]++;
                    if(qnum[to]>n)
                        dfs(to);
                }
            }
        }
    }
}
int a[maxn];
int main()
{
    int T,ca=1;
    scanf("%d",&T);
    while(T--){
        cnt=0;
        memset(head,-1,sizeof head);
       scanf("%d",&n);
       for(int i=1;i<=n;i++)
        scanf("%d",&a[i]);
       int m;
       scanf("%d",&m);
       while(m--){
        int u,v;
        scanf("%d%d",&u,&v);
        int w=(a[v]-a[u])*(a[v]-a[u])*(a[v]-a[u]);
        addEdge(u,v,w);
       }
       spfa(1);
       printf("Case %d:\n",ca++);
       scanf("%d",&m);
       while(m--){
        int x;
        scanf("%d",&x);
        if(dis[x]<3||cir[x]||dis[x]==INF)
            printf("?\n");
        else
            printf("%d\n",dis[x]);
       }
}
    return 0;
}
```





