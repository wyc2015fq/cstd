# Prim模板（C++版） - westbrook1998的博客 - CSDN博客





2018年10月28日 16:57:28[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：62








hiho1097

Prim和Dijkstra很像，这里也是用邻接矩阵存的，应该也能改成堆优化的吧，然后就是松弛条件那里和dijk不一样

```
#include <bits/stdc++.h>
using namespace std;
const int N=1e3+50;
const int INF=0x3f3f3f3f;
int n;
int cost[N][N];
int dis[N];
bool vis[N];
int Prim(){
    int ans=0;
    memset(vis,false,sizeof(vis));
    vis[0]=true;
    for(int i=1;i<n;i++){
        dis[i]=cost[0][i];
    }
    for(int i=1;i<n;i++){
        int Min=INF;
        int k=-1;
        for(int j=0;j<n;j++){
            if(!vis[j] && Min>dis[j]){
                k=j;
                Min=dis[j];
            }
        }
        if(k==-1){
            //不连通
            return -1;
        }
        ans+=Min;
        vis[k]=true;
        for(int j=0;j<n;j++){
            if(!vis[j] && dis[j]>cost[k][j]){
                dis[j]=cost[k][j];
            }
        }
    }
    return ans;
}
int main(void){
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            scanf("%d",&cost[i][j]);
        }
    }
    int ans=Prim();
    printf("%d\n",ans);
    return 0;
}
```

堆优化版本…有几个地方要注意一下

```
#include <bits/stdc++.h>
using namespace std;
const int N=350;
const int M=2e4+50;
const int INF=1e8+50;
int n,m,u,v,w,_max;
struct Edge{
    int v,w,next;
}edge[M];
int cnt,head[N];
void init(){
    cnt=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int u,int v,int w){
    edge[cnt]=Edge{v,w,head[u]};
    head[u]=cnt++;
    edge[cnt]=Edge{u,w,head[v]};
    head[v]=cnt++;
}
bool vis[N];
int dis[N];
struct node{
    int v,w;
    bool operator <(const node &rhs)const{
        return w>rhs.w;
    }
}tmp;
int Prim(){
    int ans=0;
    for(int i=0;i<=n;i++){
        dis[i]=INF;
        vis[i]=false;
    }
    vis[1]=true;
    dis[1]=0;
    priority_queue<node> q;
    //与起点直接相连的点需要加入队列，并更新dis
    for(int i=head[1];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        int w=edge[i].w;
        dis[v]=w;
        q.push(node{v,dis[v]});
    }
    while(!q.empty()){
        tmp=q.top();
        q.pop();
        int u=tmp.v;
        if(vis[u]){
            continue;
        }
        vis[u]=true;
        int w=tmp.w;
        //不连通
        if(w==INF){
            return -1;
        }
        ans+=w;
        _max=max(_max,w);
        for(int i=head[u];i!=-1;i=edge[i].next){
            int v=edge[i].v;
            int w=edge[i].w;
            //松弛条件的不同
            if(!vis[v] && dis[v]>w){
                dis[v]=w;
                q.push(node{v,dis[v]});
            }
        }
    }
    return ans;
}
int main(void){
    scanf("%d%d",&n,&m);
    init();
    while(m--){
        scanf("%d%d%d",&u,&v,&w);
        addEdge(u,v,w);
    }
    _max=0;
    Prim();
    printf("%d %d\n",n-1,_max);
    return 0;
}
```



