# hdu4289 Control  最大流最小割 - 紫芝的博客 - CSDN博客





2018年10月07日 10:24:58[紫芝](https://me.csdn.net/qq_40507857)阅读数：50
个人分类：[图论																[进击的网络流																[最大流Dinic算法](https://blog.csdn.net/qq_40507857/article/category/8104974)




　　You, the head of Department of Security, recently received a top-secret information that a group of terrorists is planning to transport some WMD 1 from one city (the source) to another one (the destination). You know their date, source and destination, and they are using the highway network. 

　　The highway network consists of bidirectional highways, connecting two distinct city. A vehicle can only enter/exit the highway network at cities only. 

　　You may locate some SA (special agents) in some selected cities, so that when the terrorists enter a city under observation (that is, SA is in this city), they would be caught immediately. 

　　It is possible to locate SA in all cities, but since controlling a city with SA may cost your department a certain amount of money, which might vary from city to city, and your budget might not be able to bear the full cost of controlling all cities, you must identify a set of cities, that: 

　　* all traffic of the terrorists must pass at least one city of the set. 

　　* sum of cost of controlling all cities in the set is minimal. 

　　You may assume that it is always possible to get from source of the terrorists to their destination. 

------------------------------------------------------------ 

1 Weapon of Mass Destruction

### **Input**

　　There are several test cases. 

　　The first line of a single test case contains two integer N and M ( 2 <= N <= 200; 1 <= M <= 20000), the number of cities and the number of highways. Cities are numbered from 1 to N. 

　　The second line contains two integer S,D ( 1 <= S,D <= N), the number of the source and the number of the destination. 

　　The following N lines contains costs. Of these lines the ith one contains exactly one integer, the cost of locating SA in the ith city to put it under observation. You may assume that the cost is positive and not exceeding 10 7. 

　　The followingM lines tells you about highway network. Each of these lines contains two integers A and B, indicating a bidirectional highway between A and B. 

　　Please process until EOF (End Of File). 

### Output

　　For each test case you should output exactly one line, containing one integer, the sum of cost of your selected set. 

　　See samples for detailed information.

### Sample Input

5 6
5 3
5
2
3
4
12
1 5
5 4
2 3
2 4
4 3
2 1
### Sample Output

3
### 题意：

**给一个无向图，有些不法分子要从vs点vt点，现在要抓住所有的不法分子阻止他们去vt，那么就要控制某一些城市等待他们，控制每个城市花费不同，问最少花费是多少。**

### 解题：

**最小割，割断所有的通路，花费使得最少，这样就一定能抓住所有的不法分子。算法导论上讲到，最大流图中，每个割都是最小割，并且最小割在数值上等于最大流.**

所以我们只要用最大流求解即可，而这里建图是关键，这里将每个点拆成两个点（i ,i + n）然后i与i + n 的权值为该点花费，反向边为0 ，(x,y)则建立(x + n,y)和 (y + n,x)权值为inf 这里他们的反向边都为0；跑一边最大流算法就好了。。

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
#include<vector>
#define INF 1e9
using namespace std;
const int maxn=1e6+10;

struct Edge
{
    int to,cap,next;
}edge[maxn<<1];
int head[maxn];
int cnt=0;
void init()
{
    cnt=0;
    memset(head,-1,sizeof head);
}
void AddEdge(int start,int to,int cap)
{
    edge[cnt].to=to;
    edge[cnt].cap=cap;
    edge[cnt].next=head[start];
    head[start]=cnt++;

    edge[cnt].to=start;
    edge[cnt].cap=0;
    edge[cnt].next=head[to];
    head[to]=cnt++;
}

int src,tar;
int d[maxn];//深度
int q[maxn<<1];

//汇点是否成功标号，即是否找到增广路
bool bfs()
{
    memset(d,-1,sizeof d);
    int front=0,tail=0;
    q[tail++]=src;
    d[src]=0;
    while(front<tail){
        int x=q[front++];
        if(x==tar)  return true;

        for(int i=head[x];i!=-1;i=edge[i].next){
            int temp=edge[i].to;
            //没有标记，且可行流大于0
            if(d[temp]==-1&&edge[i].cap>0){
                d[temp]=d[x]+1;
                q[tail++]=temp;
            }
        }
    }
    return false;
}

int dfs(int x,int cap)
{
    if(x==tar)  return cap;
    int flow=0;
    for(int i=head[x];i!=-1;i=edge[i].next){
        int temp=edge[i].to;
        if(d[temp]==d[x]+1&&edge[i].cap){
            int f=dfs(temp,min(cap-flow,edge[i].cap));
            edge[i].cap-=f;
            edge[i^1].cap+=f;
            flow+=f;
            if(flow==cap) break;
        }
    }
    if(!flow)   d[x]=-2;//防止重搜
    return flow;
}

int max_flow()
{
    int flow=0,f=0;
    while(bfs()){
        while((f=dfs(src,INF))>0)
            flow+=f;
            //printf("%d\n",flow);
    }
    return flow;
}
int main()
{
   int n,m;
   while(scanf("%d%d",&n,&m)!=EOF){
    init();
    scanf("%d%d",&src,&tar);
    tar+=n;
    for(int i=1;i<=n;i++)
    {
        int cost;
        scanf("%d",&cost);
        AddEdge(i,i+n,cost);
    }
    for(int i=1;i<=m;i++)
    {
        int u,v;
        scanf("%d%d",&u,&v);
        AddEdge(u+n,v,INF);
        AddEdge(v+n,u,INF);
    }
    printf("%d\n",max_flow());
   }
    return 0;
}
```](https://blog.csdn.net/qq_40507857/article/category/7964413)](https://blog.csdn.net/qq_40507857/article/category/7659793)





