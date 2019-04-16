# POJ 3259 Wormholes 最短路SPFA 判断负环 - 紫芝的博客 - CSDN博客





2018年10月03日 11:41:15[紫芝](https://me.csdn.net/qq_40507857)阅读数：33
个人分类：[最短路算法](https://blog.csdn.net/qq_40507857/article/category/7617885)









# Wormholes
|**Time Limit:** 2000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 65435||**Accepted:** 24390|

## Description

While exploring his many farms, Farmer John has discovered a number of amazing wormholes. A wormhole is very peculiar because it is a one-way path that delivers you to its destination at a time that is BEFORE you entered the wormhole! Each of FJ's farms comprises *N* (1 ≤ *N* ≤ 500) fields conveniently numbered 1..*N*, *M* (1 ≤ *M* ≤ 2500) paths, and *W* (1 ≤ *W* ≤ 200) wormholes.

As FJ is an avid time-traveling fan, he wants to do the following: start at some field, travel through some paths and wormholes, and return to the starting field a time before his initial departure. Perhaps he will be able to meet himself :) .

To help FJ find out whether this is possible or not, he will supply you with complete maps to *F* (1 ≤ *F* ≤ 5) of his farms. No paths will take longer than 10,000 seconds to travel and no wormhole can bring FJ back in time by more than 10,000 seconds.

### Input

Line 1: A single integer, *F*. *F* farm descriptions follow. 

Line 1 of each farm: Three space-separated integers respectively: *N*, *M*, and *W*

Lines 2..*M*+1 of each farm: Three space-separated numbers (*S*, *E*, *T*) that describe, respectively: a bidirectional path between *S* and *E* that requires *T* seconds to traverse. Two fields might be connected by more than one path. 

Lines *M*+2..*M*+*W*+1 of each farm: Three space-separated numbers (*S*, *E*, *T*) that describe, respectively: A one way path from *S* to *E* that also moves the traveler back *T* seconds.

### Output

Lines 1..*F*: For each farm, output "YES" if FJ can achieve his goal, otherwise output "NO" (do not include the quotes).

### Sample Input

```
2
3 3 1
1 2 2
1 3 4
2 3 1
3 1 3
3 2 1
1 2 3
2 3 4
3 1 8
```

### Sample Output

```
NO
YES
```

Hint

For farm 1, FJ cannot travel back in time. 

For farm 2, FJ could travel back in time by the cycle 1->2->3->1, arriving back at his starting location 1 second before he leaves. He could start from anywhere on the cycle to accomplish this.

Source

[USACO 2006 December Gold](http://poj.org/searchproblem?field=source&key=USACO+2006+December+Gold)

### 题意：

有n个点，m条双向正权边，w条单向负权边，虫洞是一条单向路，不但会把你传送到目的地，还会使时间倒退Ts，把虫洞看成一条负权路，问题转化为求图中是否存在负权回路（负环）？

### 分析：

**1.Bellman-Ford算法分为三个阶段：**

（1）初始化：将除源点之外的所有顶点，最短距离估计值设为无穷大（ dis[start]=inf ），源点到源点的距离为 dis[start]=0

（2）迭代求解：反复对边集E中每条边进行松弛操作，使得顶点集V中的每个顶点的最短距离估计值逐步逼近最短距离，（运行|V|-1次）

（3）检验负权回路：判断边集E中的每一条边的两个端点是否收敛。如果存在未收敛的顶点，返回false，否则返回true。并且从源点可达的顶点V的最短距离保存在 d[v]中

**2.SPFA算法**

SPFA算法是对Bellman-Ford算法的优化，那么如何用SPFA算法来判断负权回路呢？

考虑一个节点入队的条件是前一遍松弛中改变了距离估计值的点，才可能引起他们的邻接点的距离估计值改变。因此，用一个先进先出的队列来存放被成功松弛的顶点。

**定理：如果两点之间有最短路，那么每个节点最多经过一次，即这条路不超过n-1条边。**

如果一个点经过两次，那么走了一个圈。如果这个圈的权为正，显然不划算；如果是负圈，那么最短路不存在；如果是零圈，去掉不影响最优值。

结论：每个点最多入队n-1次，在SPFA算法的基础上，使用一个数组记录每个节点入队的次数，判断当前入队点的入队次数，如果大于n-1，说明存在负权回路

```cpp
#include<stdio.h>
#include<string.h>
#include<queue>
#include<cstdio>
#include<string>
#include<math.h>
#include<algorithm>
#define LL long long
#define PI atan(1.0)*4
#define DD double
#define mod 100
#define dian 1.000000011
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=6e3;
struct Edge{
    int to;
    int val;
    int next;
}edge[maxn];
//链式前向星建图
int cnt=0;
int head[maxn];
void addEdge(int from,int to,int val)
{
    edge[cnt].to=to;
    edge[cnt].val=val;
    edge[cnt].next=head[from];
    head[from]=cnt++;
}
int dis[maxn];//存储起点到各点的最短距离
bool inq[maxn];//标记点是否在队列中
int num[maxn];//节点的入队次数
int n,m,w;
bool spfa(int start)
{
    memset(dis,INF,sizeof dis);
    memset(inq,0,sizeof inq);
    memset(num,0,sizeof num);

    queue<int> q;
    q.push(start);
    dis[start]=0;
    inq[start]=true;
    num[start]++;

    while(!q.empty()){
        int cur=q.front();
        q.pop();
        inq[cur]=false;
        for(int i=head[cur];i!=-1;i=edge[i].next){
            int top=edge[i].to;
            if(dis[top]>dis[cur]+edge[i].val)
            {
                dis[top]=dis[cur]+edge[i].val;
                if(!inq[top]){
                    inq[top]=true;
                    q.push(top);
                    num[top]++;
                //如果入队次数大于等于n次，则存在负权环
                    if(num[top]>=n)  return true;
                }
            }
        }
    }
    return false;
}

int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        scanf("%d%d%d",&n,&m,&w);
        cnt=0;
        memset(head,-1,sizeof(head));
        for(int i=0;i<m;i++)
        {
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            addEdge(u,v,w);
            addEdge(v,u,w);
        }
        for(int i=0;i<w;i++){
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            addEdge(u,v,-w);
        }
        if(spfa(1))
            printf("YES\n");
        else
            printf("NO\n");
    }
    return 0;
}
```





