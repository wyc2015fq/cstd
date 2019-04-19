# hdu 4292 Food（Dinic） - HJ - CSDN博客
2017年09月25日 20:38:09[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：209
个人分类：[====ACM====																[---------OJ---------																[HDU																[----------图论---------																[最小割最大流定理](https://blog.csdn.net/feizaoSYUACM/article/category/7238005)](https://blog.csdn.net/feizaoSYUACM/article/category/6308029)](https://blog.csdn.net/feizaoSYUACM/article/category/6308041)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
[http://acm.hdu.edu.cn/showproblem.php?pid=4292](http://acm.hdu.edu.cn/showproblem.php?pid=4292)
注意构图分成五个部分
s –>  f  –> n –> n’ –> d –> t
其中s表示源点，t表示汇点，f表示食物种类，d表示饮料种类，n表示人，注意还要加上n个n’点（因为每个人都只能选择份食物和一份饮料，每个人的残量最多只能为1）
代码实现用Dinic（dfs+bfs），也可以用ISAP更快一点，只不过Dinic也能过。
```cpp
#include<iostream>
#include<cstdio>
#include<queue>
#include<cstring>
using namespace std;
#define N 880
#define maxn 200100
#define INF 0x3f3f3f3f
struct edge{
    int v,cap,to;          //v表示点标号 cap表示容量 to表示指向的下一个点的标号 
}g[maxn];
int cnt,n,f,d,ans,ss,first[N],dist[N];
queue<int> q;
void addedge(int u,int v,int cap)        //构边 
{
    g[cnt].v=v;
    g[cnt].cap=cap;
    g[cnt].to=first[u];
    first[u]=cnt++;                           //正向边 
    g[cnt].v=u;
    g[cnt].cap=0;
    g[cnt].to=first[v];
    first[v]=cnt++;                       //反向边 
}
bool bfs()
{
    memset(dist,0,sizeof(dist));
    dist[0]=1;
    q.push(0);
    while(!q.empty())
    {
        int u=q.front();
        q.pop();
        for(int e=first[u];~e;e=g[e].to)
        {
            int v=g[e].v,cap=g[e].cap;
            if(!dist[v]&&cap>0)
            {
                dist[v]=dist[u]+1;
                q.push(v);
            }
        }
    }
    if(dist[ss+1]==0)
      return false;
    return true;
}
int dfs(int u,int flow)
{
    int mm;
    if(u==ss+1)
      return flow;
    for(int e=first[u];~e;e=g[e].to)
    {
        int v=g[e].v,cap=g[e].cap;
        if(dist[v]==dist[u]+1&&cap>0&&(mm=dfs(v,min(cap,flow))))
        {
            g[e].cap-=mm;
            g[e^1].cap+=mm;
            return mm;
        }
    } 
    dist[u]=-1;         //表示源点不能到达汇点 
    return 0;
}
void Dinic()
{
    ans=0;
    int d;       //d保存增广路的最小残量 
    while(bfs())      //存在增广路 
    {
        while(d=dfs(0,INF))      //找出任意一一条增广路  
          ans+=d; 
    }
}
int main()
{
    while(scanf("%d%d%d",&n,&f,&d)!=EOF)
    {
        ss=2*n+f+d;         //  s->f->n->n'->d->t   总共是2*n+f+d个中间点 
        cnt=0;
        int x;
        char ch;
        memset(first,-1,sizeof(first));
        for(int i=1;i<=f;i++)
        {
            scanf("%d",&x);
            addedge(0,i,x);
        }
        for(int i=1;i<=d;i++)
        {
            scanf("%d",&x);
            addedge(n*2+f+i,ss+1,x);
        }
        for(int i=1;i<=n;i++)
        {
            getchar();
            for(int j=1;j<=f;j++)
            {
                scanf("%c",&ch);
                if(ch=='Y')
                  addedge(j,f+i,1);
            }
        }
        for(int i=1;i<=n;i++)
        {
            addedge(f+i,n+f+i,1);
            getchar();
            for(int j=1;j<=d;j++)
            {
                scanf("%c",&ch);
                if(ch=='Y')
                  addedge(n+f+i,f+n*2+j,1);
            }
        }
        Dinic();
        printf("%d\n",ans);
    }
    return 0;
}
```
