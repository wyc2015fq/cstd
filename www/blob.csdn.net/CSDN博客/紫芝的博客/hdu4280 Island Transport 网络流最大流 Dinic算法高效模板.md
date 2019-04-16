# hdu4280 Island Transport  网络流最大流  Dinic算法高效模板 - 紫芝的博客 - CSDN博客





2018年10月06日 21:10:06[紫芝](https://me.csdn.net/qq_40507857)阅读数：64








#  Island Transport  

　　In the vast waters far far away, there are many islands. People are living on the islands, and all the transport among the islands relies on the ships. 

　　You have a transportation company there. Some routes are opened for passengers. Each route is a straight line connecting two different islands, and it is bidirectional. Within an hour, a route can transport a certain number of passengers in one direction. For safety, no two routes are cross or overlap and no routes will pass an island except the departing island and the arriving island. Each island can be treated as a point on the XY plane coordinate system. X coordinate increase from west to east, and Y coordinate increase from south to north. 

　　The transport capacity is important to you. Suppose many passengers depart from the westernmost island and would like to arrive at the easternmost island, the maximum number of passengers arrive at the latter within every hour is the transport capacity. Please calculate it. 

Input

　　The first line contains one integer T (1<=T<=20), the number of test cases. 

　　Then T test cases follow. The first line of each test case contains two integers N and M (2<=N,M<=100000), the number of islands and the number of routes. Islands are number from 1 to N. 

　　Then N lines follow. Each line contain two integers, the X and Y coordinate of an island. The K-th line in the N lines describes the island K. The absolute values of all the coordinates are no more than 100000. 

　　Then M lines follow. Each line contains three integers I1, I2 (1<=I1,I2<=N) and C (1<=C<=10000) . It means there is a route connecting island I1 and island I2, and it can transport C passengers in one direction within an hour. 

　　It is guaranteed that the routes obey the rules described above. There is only one island is westernmost and only one island is easternmost. No two islands would have the same coordinates. Each island can go to any other island by the routes. 

Output

　　For each test case, output an integer in one line, the transport capacity. 

Sample Input

```
2
5 7
3 3
3 0
3 1
0 0
4 5
1 3 3
2 3 4
2 4 3
1 5 6
4 5 3
1 4 4
3 4 2
6 7
-1 -1
0 1
0 2
1 0
1 1
2 3
1 2 1
2 3 6
4 5 5
5 6 3
1 4 6
2 5 5
3 6 4
```

Sample Output

```
9
6
```

### 题意：

n个点，要从最西边（X最小）的城市向最东边的（X最大）的城市运送人。求最大运送量。

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
#include<vector>
#define INF 1e9
using namespace std;
const int maxn=1e5+10;

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
    int T;
    scanf("%d",&T);
    while(T--){
        int n,m;
        scanf("%d%d",&n,&m);
        int west=INF,east=-INF;
        //int src,tar;
        for(int i=1;i<=n;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            if(x<west)  src=i,west=x;
            if(x>east)  tar=i,east=x;
        }
        init();
        for(int i=1;i<=m;i++){
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            AddEdge(u,v,w);
            AddEdge(v,u,w);
        }
        printf("%d\n",max_flow());
    }
    return 0;
}
```

STL超时代码

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
    int from,to,cap,flow;
    Edge(){}
    Edge(int f,int t,int c,int fl):from(f),to(t),cap(c),flow(fl){}
};

struct Dinic
{
    int n,m,s,t;
    vector<Edge> edges;//存边
    vector<int> G[maxn];//邻接表存图
    bool vis[maxn];//标记节点的深度
    int d[maxn];//节点深度

    void init(int n,int s,int t)
    {
        this->n=n, this->s=s, this->t=t;
        edges.clear();
        for(int i=0;i<n;i++) G[i].clear();
    }

    void AddEdge(int from,int to,int cap)
    {
        edges.push_back( Edge(from,to,cap,0) );
        edges.push_back( Edge(to,from,cap,0) );
        m=edges.size();
        G[from].push_back( m-2 );
        G[to].push_back(m-1);
    }

    bool BFS()
    {
        queue<int> Q;
        memset(vis,0,sizeof(vis));
        vis[s]=true;
        d[s]=0;
        Q.push(s);
        while(!Q.empty())
        {
            int x= Q.front(); Q.pop();
            for(int i=0;i<G[x].size();++i)
            {
                Edge& e=edges[G[x][i]];
                if(!vis[e.to] && e.cap>e.flow)
                {
                    vis[e.to]=true;
                    d[e.to]=d[x]+1;
                    Q.push(e.to);
                }
            }
        }
        return vis[t];
    }

    int DFS(int x,int a)
    {
        if(x==t || a==0) return a;
        int flow=0,f;
        for(int i=0;i<G[x].size();i++)
        {
            Edge& e=edges[G[x][i]];
            if(d[e.to]==d[x]+1 && (f=DFS(e.to,min(a,e.cap-e.flow) ) )>0)
            {
                e.flow +=f;
                edges[G[x][i]^1].flow -=f;
                flow +=f;
                a-=f;
                if(a==0) break;
            }
        }
        return flow;
    }

    int max_flow()
    {
        int ans=0;
        while(BFS())
        {
            ans+= DFS(s,INF);
        }
        return ans;
    }
}DC;

int main()
{
    int T;
    scanf("%d",&T);
    while(T--){
        int n,m;
        scanf("%d%d",&n,&m);
        int west=INF,east=-INF;
        int src,tar;
        for(int i=1;i<=n;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            if(x<west)  src=i,west=x;
            if(x>east)  tar=i,east=x;
        }
        DC.init(n,src,tar);
        for(int i=1;i<=m;i++){
            int u,v,w;
            scanf("%d%d%d",&u,&v,&w);
            DC.AddEdge(u,v,w);
        }
        printf("%d\n",DC.max_flow());
    }
    return 0;
}
```





