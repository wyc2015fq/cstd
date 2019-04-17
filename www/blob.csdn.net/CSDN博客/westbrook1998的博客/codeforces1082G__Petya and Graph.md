# codeforces1082G__Petya and Graph - westbrook1998的博客 - CSDN博客





2018年11月30日 14:38:25[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：59








据说是模板题…

求最大权闭合子图

最大权闭合子图参考[这里](https://www.cnblogs.com/dilthey/p/7565206.html)

然后把题目的边看成原模板中的正权点,原先的点就看成原模板中的负权点,跑最大流求出最小割,然后用总的边权减去最小割即可
代码:

```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=2e5+50;
const int M=2e6+50;
const ll INF=1e18;
int n,m;
ll val[N];
struct Edge{
    int v;
    ll cap,flow;
    int next;
}edge[M];
int cnt,head[N];
void init(){
    cnt=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int u,int v,ll cap,ll cost){
    edge[cnt]=Edge{v,cap,0,head[u]};
    head[u]=cnt++;
    edge[cnt]=Edge{u,0,0,head[v]};
    head[v]=cnt++;
}
int s,t;
int dep[N];
bool bfs(){
    queue<int> q;
    memset(dep,0,sizeof(dep));
    dep[s]=1;
    q.push(s);
    while(!q.empty()){
        int u=q.front();
        q.pop();
        for(int i=head[u];i!=-1;i=edge[i].next){
            int v=edge[i].v;
            ll w=edge[i].cap-edge[i].flow;
            if(w>0 && dep[v]==0){
                dep[v]=dep[u]+1;
                q.push(v);
            }
        }
    }
    return dep[t]!=0;
}
int cur[N];
ll dfs(int u,ll flow){
    if(u==t){
        return flow;
    }
    for(int &i=cur[u];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        ll w=edge[i].cap-edge[i].flow;
        if(dep[v]==dep[u]+1 && w!=0){
            ll dis=dfs(v,min(w,flow));
            if(dis>0){
                edge[i].flow+=dis;
                edge[i^1].flow-=dis;
                return dis;
            }
        }
    }
    return 0;
}
ll dinic(){
    ll ans=0;
    while(bfs()){
        for(int i=0;i<=n+m;i++){
            cur[i]=head[i];
        }
        while(ll d=dfs(s,INF)){
            ans+=d;
        }
    }
    return ans;
}
int main(void){
    scanf("%d%d",&n,&m);
    s=0;
    t=n+m+1;
    init();
    ll w;
    int u,v;
    ll sum=0;
    for(int i=1;i<=n;i++){
        scanf("%lld",&w);
        addEdge(m+i,t,w,0);
    }
    for(int i=1;i<=m;i++){
        scanf("%d%d%lld",&u,&v,&w);
        sum+=w;
        addEdge(s,i,w,0);
        addEdge(i,m+u,INF,0);
        addEdge(i,m+v,INF,0);
    }
    ll ans=dinic();
    printf("%lld\n",sum-ans);
    return 0;
}
```




