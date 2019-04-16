# POJ 1459  Power Network 最大流 Dinic算法 - 紫芝的博客 - CSDN博客





2018年10月06日 17:25:45[紫芝](https://me.csdn.net/qq_40507857)阅读数：34








A power network consists of nodes (power stations, consumers and dispatchers) connected by power transport lines. A node u may be supplied with an amount s(u) >= 0 of power, may produce an amount 0 <= p(u) <= p max(u) of power, may consume an amount 0 <= c(u) <= min(s(u),c max(u)) of power, and may deliver an amount d(u)=s(u)+p(u)-c(u) of power. The following restrictions apply: c(u)=0 for any power station, p(u)=0 for any consumer, and p(u)=c(u)=0 for any dispatcher. There is at most one power transport line (u,v) from a node u to a node v in the net; it transports an amount 0 <= l(u,v) <= l max(u,v) of power delivered by u to v. Let Con=Σ uc(u) be the power consumed in the net. The problem is to compute the maximum value of Con. 

![](https://odzkskevi.qnssl.com/1c1354d5a7c820146d570a7875b230b3?v=1538547963)



An example is in figure 1. The label x/y of power station u shows that p(u)=x and pmax(u)=y. The label x/y of consumer u shows that c(u)=x and c max(u)=y. The label x/y of power transport line (u,v) shows that l(u,v)=x and l max(u,v)=y. The power consumed is Con=6. Notice that there are other possible states of the network but the value of Con cannot exceed 6. 

Input

There are several data sets in the input. Each data set encodes a power network. It starts with four integers: 0 <= n <= 100 (nodes), 0 <= np <= n (power stations), 0 <= nc <= n (consumers), and 0 <= m <= n^2 (power transport lines). Follow m data triplets (u,v)z, where u and v are node identifiers (starting from 0) and 0 <= z <= 1000 is the value of l max(u,v). Follow np doublets (u)z, where u is the identifier of a power station and 0 <= z <= 10000 is the value of p max(u). The data set ends with nc doublets (u)z, where u is the identifier of a consumer and 0 <= z <= 10000 is the value of c max(u). All input numbers are integers. Except the (u,v)z triplets and the (u)z doublets, which do not contain white spaces, white spaces can occur freely in input. Input data terminate with an end of file and are correct.

Output

For each data set from the input, the program prints on the standard output the maximum amount of power that can be consumed in the corresponding network. Each result has an integral value and is printed from the beginning of a separate line.

Sample Input

```
2 1 1 2 (0,1)20 (1,0)10 (0)15 (1)20
7 2 3 13 (0,0)1 (0,1)2 (0,2)5 (1,0)1 (1,2)8 (2,3)1 (2,4)7
         (3,5)2 (3,6)5 (4,2)7 (4,3)5 (4,5)1 (6,0)5
         (0)5 (1)2 (3)2 (4)1 (5)4
```

Sample Output

```
15
6
```

Hint

The sample input contains two data sets. The first data set encodes a network with 2 nodes, power station 0 with pmax(0)=15 and consumer 1 with cmax(1)=20, and 2 power transport lines with lmax(0,1)=20 and lmax(1,0)=10. The maximum value of Con is 15. The second data set encodes the network from figure 1.

**题意:**

总共有n个节点，其中有发电站np个、用户nc个和调度器n-np-nc个三种节点以及M条电线(用于传输电流的)，每个发电站有一个最大发电量，每个用户有个最大接受电量，现在有m条**有向边**，边有一个最大的电流量，表示最多可以流出这么多电，现在从发电站发电到用户，问最多可以发多少电(被用户接受).

**分析:**

**  建立一个超级源点s,s到任意发电站i有(s,i,,p[i]) (表示该发电站最多能发p[i]电)**

**  建立一个超级汇点t,任意用户j到t有边(j,t,c[j]) (表示该用户最多能消费c[j]电)**

**  然后对于题目中描述的M条电线(u,v)L, 就有边(u,v,L).**

       最终我们求本图的最大流即可.(源点到汇点t的最大流就是所有用户能消费的最大电量)

**AC代码:**

```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<queue>
#include<vector>
#define INF 1e9
using namespace std;
const int maxn=100+5;
 
struct Edge
{
    int from,to,cap,flow;
    Edge(){}
    Edge(int f,int t,int c,int fl):from(f),to(t),cap(c),flow(fl){}
};
 
struct Dinic
{
    int n,m,s,t;
    vector<Edge> edges;
    vector<int> G[maxn];
    int cur[maxn];
    bool vis[maxn];
    int d[maxn];
 
    void init(int n,int s,int t)
    {
        this->n=n, this->s=s, this->t=t;
        edges.clear();
        for(int i=0;i<n;i++) G[i].clear();
    }
 
    void AddEdge(int from,int to,int cap)
    {
        edges.push_back( Edge(from,to,cap,0) );
        edges.push_back( Edge(to,from,0,0) );
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
        for(int& i=cur[x];i<G[x].size();i++)
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
            memset(cur,0,sizeof(cur));
            ans+= DFS(s,INF);
        }
        return ans;
    }
}DC;
 
int main()
{
    int n,np,nc,m;
    while(scanf("%d%d%d%d",&n,&np,&nc,&m)==4)
    {
        DC.init(n+2,0,n+1);
        for(int i=0;i<m;i++)
        {
            int u,v,w;
            scanf(" (%d,%d)%d",&u,&v,&w);
            ++u,++v;
            DC.AddEdge(u,v,w);
        }
        for(int i=0;i<np;i++)
        {
            int u,w;
            scanf(" (%d)%d",&u,&w);
            ++u;
            DC.AddEdge(0,u,w);
        }
        for(int i=0;i<nc;i++)
        {
            int u,w;
            scanf(" (%d)%d",&u,&w);
            ++u;
            DC.AddEdge(u,n+1,w);
        }
        printf("%d\n",DC.max_flow());
    }
    return 0;
}
```

**用邻接矩阵存图的超时代码**

```cpp
//#include<bits/stdc++.h>
#include<cstdio>
#include<cstdio>
#include<algorithm>
#include<cstring>
#include<vector>
#include<queue>
using namespace std;
#define INF 0x3f3f3f3f
const int maxn=1e3+10;

int F,D,N;
int g[maxn][maxn];
int lv[maxn];

//bfs分层图过程
bool bfs(int src,int tar)
{
    memset(lv,0,sizeof lv);
    queue<int>que;
    que.push(src);
    lv[src]=1;//源点的深度为1

    while(!que.empty()){
        int cur=que.front();
        que.pop();
        for(int i=src;i<=tar;i++)
        //若该残量不为0，并未分配深度
            if(!lv[i]&&g[cur][i]){
                lv[i]=lv[cur]+1;
                if(i==tar)  return 1;
                que.push(i);
            }
        }
    return 0;
}

//寻找增广路
//cur是当前节点，src是源点，tar是汇点，totflow当前流量
int dfs(int cur,int src,int tar,int totflow){
    int ret=0;
    if(cur==tar||!totflow)//当已经到达汇点，直接返回
        return totflow;
    for(int i=src;i<=tar;i++){
        if(totflow==0)  break;//若没有增广路
    //满足分层图和残量不为0两个条件
        if(g[cur][i]&&lv[cur]+1==lv[i]){
            int f=min(totflow,g[cur][i]);
            int flowdown=dfs(i,src,tar,f);//向下增广
            ret+=flowdown;
            totflow-=flowdown;
            g[cur][i]-=flowdown;//正向边减
            g[i][cur]+=flowdown;//反向边加
        }
    }
    return ret;
}

int dinic(int src,int tar,int num)
{
    int ret=0;//记录最大流量
    while(bfs(src,tar)){
        int tmp=dfs(src,src,tar,num);
        if(!tmp)    break;
        ret+=tmp;
    }
    return ret;
}

int main()
{
    int n,nc,np,m;
    while(scanf("%d %d %d %d",&n,&np,&nc,&m)!=EOF){
        int src=0;
        int tar=n+1;
        int u,v,z;

        for(int i=1;i<=m;i++)
        {
            scanf(" (%d,%d)%d",&u,&v,&z);
            g[u+1][v+1]=z;
        }

        for(int i=1;i<=np;i++){
            scanf(" (%d)%d",&u,&z);
            g[0][u+1]=z;
        }


        for(int i=1;i<=nc;i++){
            scanf(" (%d)%d",&u,&z);
            g[u+1][tar]=z;
        }

        printf("%d\n",dinic(src,tar,n));
    }
    return 0;
}
```





