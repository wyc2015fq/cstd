# hdu 1532 Drainage Ditches（最大流 三种模板：EK、Dinic、isap） - HJ - CSDN博客
2017年10月14日 17:45:28[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：163
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[最小割最大流定理](https://blog.csdn.net/feizaoSYUACM/article/category/7238005)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.split.hdu.edu.cn/showproblem.php?pid=1532](http://acm.split.hdu.edu.cn/showproblem.php?pid=1532)
题目大意： 
给出边数N,点数M，每条边都是单向的，问从1点到M的最大流是多少。
最大流的三种模板EK，Dinic和isap
第一种：EK算法
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#include<queue>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=205;
struct Edge{
    int from,to,cap,flow;
    Edge(int u,int v,int c,int f):from(u),to(v),cap(c),flow(f){}
};
struct Edmonds_Karp{
    int n,m;
    vector<Edge> edges;      //边数的两倍 
    vector<int> g[maxn];    //邻接表 g[i][j]表示节点i的第j条边在e数组中的序号 
    int a[maxn];      //当源点到i的可改进量 
    int p[maxn];       //最短路树上p的入弧编号 
    void Init(int n)
    {
        for(int i=0;i<n;i++)
          g[i].clear();
        edges.clear();
    }
    void addedge(int from,int to,int cap)
    {
        edges.push_back(Edge(from,to,cap,0));
        edges.push_back(Edge(to,from,0,0));
        m=edges.size();
        g[from].push_back(m-2);
        g[to].push_back(m-1);
    } 
    int Maxflow(int s,int t)
    {
        int flow=0;
        for(;;)
        {
            memset(a,0,sizeof(a));
            queue<int> q;
            q.push(s);
            a[s]=INF;
            while(!q.empty())
            {
                int x=q.front();
                q.pop();
                for(int i=0;i<g[x].size();i++)
                {
                    Edge &e=edges[g[x][i]];
                    if(!a[e.to]&&e.cap>e.flow)
                    {
                        p[e.to]=g[x][i];
                        a[e.to]=min(a[x],e.cap-e.flow);
                        q.push(e.to);
                    }
                }
                if(a[t])
                  break;
            }       
            if(!a[t])
              break;
            for(int u=t;u!=s;u=edges[p[u]].from)
            {
                edges[p[u]].flow+=a[t];
                edges[p[u]^1].flow-=a[t];
            }       
            flow+=a[t]; 
        }
        return flow;
    }
}EK;
int main()
{
    int n,m,a,b,c;
    while(~scanf("%d%d",&n,&m))
    {
        EK.Init(m);
        for(int i=0;i<n;i++)
        {
            scanf("%d%d%d",&a,&b,&c);
            a--;
            b--;
            EK.addedge(a,b,c);
        }
        printf("%d\n",EK.Maxflow(0,m-1));
    }
    return 0;
}
```
第二种：Dinic算法
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#include<queue>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=205;
struct Edge{
    int from,to,cap,flow;
    Edge(int u,int v,int c,int f):from(u),to(v),cap(c),flow(f){}
};
struct Dinic{
    int n,m,s,t;
    vector<Edge> edges;
    vector<int> g[maxn];
    bool vis[maxn];
    int d[maxn];
    int cur[maxn];
    void Init(int n)
    {
        this->n=n;
        for(int i=0;i<n;i++)
          g[i].clear();
        edges.clear();
    }
    void addedge(int from,int to,int cap)
    {
        edges.push_back(Edge(from,to,cap,0));
        edges.push_back(Edge(to,from,0,0));
        m=edges.size();
        g[from].push_back(m-2);
        g[to].push_back(m-1);
    }
    bool bfs()
    {
        memset(vis,0,sizeof(vis));
        queue<int> q;
        q.push(s);
        d[s]=0;
        vis[s]=1;
        while(!q.empty())
        {
            int x=q.front();
            q.pop();
            for(int i=0;i<g[x].size();i++)
            {
                Edge &e=edges[g[x][i]];
                if(!vis[e.to]&&e.cap>e.flow)
                {
                    vis[e.to]=1;
                    d[e.to]=d[x]+1;
                    q.push(e.to);
                }
            }
        }
        return vis[t];
    }
    int dfs(int x,int a)
    {
        if(x==t||a==0)
          return a;
        int flow=0,f;
        for(int &i=cur[x];i<g[x].size();i++)
        {
            Edge &e=edges[g[x][i]];
            if(d[x]+1==d[e.to]&&(f=dfs(e.to,min(a,e.cap-e.flow)))>0)
            {
                edges[g[x][i]].flow+=f;
                edges[g[x][i]^1].flow-=f;
                flow+=f;
                a-=f;
                if(a==0)
                  break;
            } 
        }
        return flow;
    }
    int Maxflow(int s,int t)
    {
        this->s=s;
        this->t=t;
        int flow=0;
        while(bfs())
        {
            memset(cur,0,sizeof(cur));
            flow+=dfs(s,INF);
        } 
        return flow;
    }
}DC; 
int main()
{
    int n,m,a,b,c;
    while(~scanf("%d%d",&n,&m))
    {
        DC.Init(m);
        while(n--)
        {
            scanf("%d%d%d",&a,&b,&c);
            a--;
            b--;
            DC.addedge(a,b,c); 
        }
        printf("%d\n",DC.Maxflow(0,m-1));
    }
    return 0;
}
```
第三种：isap算法
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<vector>
#include<queue>
#define INF 0x3f3f3f3f
using namespace std;
const int maxn=1005;
int src,sink,num_nodes;           //源点 汇点 
int p[maxn];           //可增广路上的上一条弧的编号 
int num[maxn];         //和t的最短距离等于i的节点数量 
int cur[maxn];          //当前弧下标 
int d[maxn];          //残量网络中节点i到汇点t的最短距离
bool vis[maxn]; 
struct Edge{
    int from,to,cap,flow;
    Edge(int u,int v,int c,int f):from(u),to(v),cap(c),flow(f){}
};
vector<int> g[maxn];
vector<Edge> edges;
void Init()
{
    edges.clear();
    for(int i=0;i<num_nodes;i++)
      g[i].clear();
}
void addedge(int from,int to,int cap)
{
    edges.push_back(Edge(from,to,cap,0));
    edges.push_back(Edge(to,from,0,0));
    int m=edges.size();
    g[from].push_back(m-2);
    g[to].push_back(m-1);
}
int augment()
{
    int u=sink,flow=INF;
    //从汇点到源点通过追踪增广路径  flow为路上最小的残量 
    while(u!=src)
    {
        Edge &e=edges[p[u]];
        flow=min(flow,e.cap-e.flow);
        u=edges[p[u]].from;
    }
    u=sink;
    //从汇点到源点更新流量 
    while(u!=src)
    {
        edges[p[u]].flow+=flow;
        edges[p[u]^1].flow-=flow;
        u=edges[p[u]].from;
    }
    return flow;
}
bool bfs()
{
    memset(vis,0,sizeof(vis));
    queue<int> q;
    q.push(sink);
    vis[sink]=1;
    d[sink]=0;
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int i=0;i<g[u].size();i++)
        {
            Edge &e=edges[g[u][i]^1];
            if(!vis[e.from]&&e.cap>e.flow)
            {
                vis[e.from]=1;
                d[e.from]=d[u]+1;
                q.push(e.from);
            }
        }
    } 
    return vis[src];
}
int isap()
{
    int flow=0,u=src;
    bfs();  
    memset(num,0,sizeof(num));
    memset(cur,0,sizeof(cur));  
    for(int i=0;i<num_nodes;i++)
      num[d[i]]++;
    while(d[src]<num_nodes)
    {
        if(u==sink)
        {
            flow+=augment();
            u=src;
        }
        bool advanced=false;
        for(int i=cur[u];i<g[u].size();i++)
        {
            Edge &e=edges[g[u][i]];
            if(d[u]==d[e.to]+1&&e.cap>e.flow)
            {
                advanced=true;
                p[e.to]=g[u][i];
                cur[u]=i;
                u=e.to;
                break;
            }
        }
        if(!advanced)    //retreat
        {
            int m=num_nodes-1;
            for(int i=0;i<g[u].size();i++)
              if(edges[g[u][i]].cap>edges[g[u][i]].flow)
                m=min(m,d[edges[g[u][i]].to]);
            if(--num[d[u]]==0)       //gap优化 
              break;
            num[d[u]=m+1]++;
            cur[u]=0;
            if(u!=src)
              u=edges[p[u]].from;
        }
    }
    return flow;
}
int main()
{
    int n,a,b,c;
    while(~scanf("%d%d",&n,&num_nodes))
    {
        Init();
        src=0;
        sink=num_nodes-1;
        for(int i=0;i<n;i++)
        {
            scanf("%d%d%d",&a,&b,&c);
            a--;
            b--;
            addedge(a,b,c);
        } 
        printf("%d\n",isap());
    }
    return 0;
}
```
