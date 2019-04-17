# 【SPFA】最短路 _HDU2544 - CD's Coding - CSDN博客





2015年04月22日 00:43:39[糖果天王](https://me.csdn.net/okcd00)阅读数：503








还是不自己写一写自己心里不踏实呀……




首先是理论: (此处引用 DM张鹏飞 @cnBlog 的总结)

SPFA算法是1994年西南交通大学段凡丁提出，只要最短路径存在，SPFA算法必定能求出最小值，SPFA对Bellman-Ford算法优化的关键之处在于意识到：只有那些在前一遍松弛中改变了距离估计值的点，才可能引起他们的邻接点的距离估计值的改变。为什么队列为空就不改变了呢？就是因为要到下一点必须经过它的前一个邻接点。。SPFA可以处理负权边。很多时候，给定的图存在负权边，这时类似Dijkstra等算法便没有了用武之地，而Bellman-Ford算法的复杂度又过高，SPFA算法便派上用场了。简洁起见，我们约定有向加权图G不存在负权回路，即最短路径一定存在。当然，我们可以在执行该算法前做一次拓扑排序，以判断是否存在负权回路。


        初始化： dis数组全部赋值为Inf(无穷大，不能是map[s][i]),path数组全部赋值为s（即源点），或者赋值为-1，表示还没有知道前驱,然后dis[s]=0;  表示源点不用求最短路径，或者说最短路就是0。将源点入队；另外记住在整个算法中有顶点入队了要记得标记vis数组，有顶点出队了记得消除那个标记(可能多次入队)。


        核心：读取队头顶点u，并将队头顶点u出队（记得消除标记）；将与点u相连的所有点v进行松弛操作，如果能更新估计值（即令d[v]变小），那么就更新，另外，如果点v没有在队列中，那么要将点v入队（记得标记），如果已经在队列中了，那么就不用入队以此循环，直到队空为止就完成了单源最短路的求解。


        判断有无负环：如果某个点进入队列的次数超过N次则存在负环(SPFA无法处理带负环的图)，假设这个节点的入度是k(无向权则就是这个节点的连接的边)如果进入这个队列超过k,说明必然有某个边重复了，即成环；换一种思路：用DFS，假设存在负环a1->a2->…->an->a1。那么当从a1深搜下去时又遇到了a1，那么直接可以判断负环了所有用。当某个节点n次进入队列，则存在负环，此时时间复杂度为O(n*m),n为节点，m为边。


        SPFA算法有两个优化算法 SLF 和 LLL： SLF：Small Label First 策略，设要加入的节点是j，队首元素为i，若dist(j)<dist(i)，则将j插入队首，否则插入队尾。 LLL：Large Label Last 策略，设队首元素为i，队列中所有dist值的平均值为x，若dist(i)>x则将i插入到队尾，查找下一元素，直到找到某一i使得dist(i)<=x，则将i出对进行松弛操作。 SLF 可使速度提高 15 ~ 20%；SLF + LLL 可提高约 50%。 在实际的应用中SPFA的算法时间效率不是很稳定，为了避免最坏情况的出现，通常使用效率更加稳定的Dijkstra算法。个人觉得LLL优化每次要求平均值，不太好，为了简单，我们可以之间用c++STL里面的优先队列来进行SLF优化。





然后还是看看代码的好：

首先是SPFA的无附加空白模板：

应用于，Ｎ点Ｍ边，依次输入从ａ到ｂ的权值为ｗ的有向边，最终输出从１到Ｎ点的最短路权值。

Code：



```cpp
#include <cmath> 
#include <queue> 
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;

#define Maxn 100
#define Maxm 10000
#define Max 10000

int used[Maxn],outqueue[Maxn],head[Maxn],low[Maxn],n,m;
struct Edge
{
	int to,w,next;
}edge[Maxm];

bool SPFA (int start)
{
     queue<int> a;
     used[start] = 1;
     low[start] = 0;
     a.push(start);
     while (!a.empty())
     {
           int top = a.front();
           a.pop();
           outqueue[top]++;
           if (outqueue[top] > n) return false;
           for (int k = head[top]; k!= -1; k = edge[k].next)
           {
               if (low[edge[k].to] > low[top] + edge[k].w)
                  low[edge[k].to] = low[top] + edge[k].w;
               if (!used[edge[k].to])
               {
                   used[edge[k].to] = 1;
                   a.push(edge[k].to);
               }
           }
     }
     return true;
}
    
int main()
{
    while (scanf ("%d%d", &n ,&m) != EOF)
    {
          memset (used, 0 ,sizeof(used));
          memset (head, -1 ,sizeof(head));
          memset (outqueue, 0 ,sizeof(outqueue));
          memset (low, Max, sizeof(low));
          int k = 0;
          while (m--)
          {
                int a,b,w;
                scanf ("%d%d%d", &a, &b, &w);
                edge[k].to = b;
                edge[k].w = w;
                edge[k].next = head[a];
                head[a] = k++;
          }
          if (SPFA(1))
             printf ("%d\n", low[n]);
          else
             printf ("不存在最短\n");
    }
}
```


当然，白板肯定是不行的，

至少有了有向边，也得考虑考虑无向边的想法嘛：要不然试着来道题？（HDU2544 最短路） 题目就赤裸裸的告诉你是最短路，而且还真的是个裸的最短路问题。






# 最短路
**Time Limit: 5000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 37907    Accepted Submission(s): 16533**



Problem Description


在每年的校赛里，所有进入决赛的同学都会获得一件很漂亮的t-shirt。但是每当我们的工作人员把上百件的衣服从商店运回到赛场的时候，却是非常累的！所以现在他们想要寻找最短的从商店到赛场的路线，你可以帮助他们吗？







Input


输入包括多组数据。每组数据第一行是两个整数N、M（N<=100，M<=10000），N表示成都的大街上有几个路口，标号为1的路口是商店所在地，标号为N的路口是赛场所在地，M则表示在成都有几条路。N=M=0表示输入结束。接下来M行，每行包括3个整数A，B，C（1<=A,B<=N,1<=C<=1000）,表示在路口A与路口B之间有一条路，我们的工作人员需要C分钟的时间走过这条路。

输入保证至少存在1条商店到赛场的路线。






Output


对于每组输入，输出一行，表示工作人员从商店走到赛场的最短时间





Sample Input


2 1
1 2 3
3 3
1 2 5
2 3 5
3 1 2
0 0






Sample Output


3
2






Source

[UESTC 6th Programming Contest Online](http://acm.hdu.edu.cn/search.php?field=problem&key=UESTC+6th+Programming+Contest+Online&source=1&searchmode=source)





Recommend


lcy







```cpp
#include <cmath> 
#include <queue>
#include <cctype>
#include <cstdio>
#include <string>
#include <cstdlib>
#include <cstring>
#include <memory.h>
#include <iostream>
#include <algorithm>
using namespace std;

const int N = 105;
const int INF = 99999999;

int map[N][N], dist[N];
bool visit[N];
int n, m;

void init()     //初始化
{
    int i, j;
    for(i = 1; i < N; i++)
    {
        for(j = 1; j < N; j++)
        {
            if(i == j) map[i][j] = 0;
            else map[i][j] = map[j][i] = INF;
        }
    }
}

void input()    //输入函数
{
    int vi, vj, cost;
    while(m--)
    {
        scanf("%d %d %d", &vi, &vj, &cost);
        if(cost < map[vi][vj])
            map[vi][vj] = map[vj][vi] = cost;
    }
}

void floyd()    //Floyd算法
{
    int i, j, k;
    for(k = 1; k <= n; k++)     //k为中间点
        for(i = 1; i <= n; i++)
            for(j = 1; j <= n; j++)
                if(map[i][k] + map[k][j] <  map[i][j])
                     map[i][j] = map[i][k] + map[k][j];
}

void dijk()     //Dijkstra算法
{
    int i, j, next, MIN;
    memset(visit, false, sizeof(visit));
    for(i = 1; i <= n; i++) dist[i] = INF;
    dist[1] = 0;
    for(i = 1; i <= n; i++)
    {
        MIN = INF;
        for(j = 1; j <= n; j++)
            if(!visit[j] && dist[j] <= MIN)
                MIN = dist[next=j];
        if(MIN == INF) break;
        visit[next] = true;
        for(j = 1; j <= n; j++)
            if(!visit[j] && dist[j] > dist[next] + map[next][j])
                dist[j] = dist[next] + map[next][j];
    }
}

void spfa()     //SPFA算法
{
    int i, now;
    memset(visit, false, sizeof(visit));
    for(i = 1; i <= n; i++) dist[i] = INF;
    dist[1] = 0;
    queue<int> Q;
    Q.push(1);
    visit[1] = true;
    while(!Q.empty())
    {
        now = Q.front();
        Q.pop();
        visit[now] = false;
        for(i = 1; i <= n; i++)
        {
            if(dist[i] > dist[now] + map[now][i])
            {
                dist[i] = dist[now] + map[now][i];
                if(visit[i] == 0)
                {
                    Q.push(i);
                    visit[i] = true;
                }
            }
        }
    }
}

int main()
{
    while(scanf("%d %d", &n, &m) && n && m)
    {
        init();	input();
        //floyd();
        //dijk();
        spfa();
		//printf("%d\n", map[1][n]);
        printf("%d\n", dist[n]);
    }
    return 0;
}
```











