# 树的直径/最长路模板（C++） - westbrook1998的博客 - CSDN博客





2018年10月21日 23:06:04[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：143








dfs版本

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
const int N=1e5+50;
const int M=1e6+50;
int n,u,v;
struct Edge{
    int v,next;
}edge[M];
int cnt;
int head[N],dep[N];
bool vis[N];
void init(){
    cnt=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int u,int v){
    edge[cnt]=Edge{v,head[u]};
    head[u]=cnt++;
    edge[cnt]=Edge{u,head[v]};
    head[v]=cnt++;
}
//dfs方法
//dfs相当于起到一个分层的作用，层次最深的就是离根最远的点
void dfs(int u,int d){
    vis[u]=true;
    dep[u]=d;
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        if(!vis[v]){
            dfs(v,d+1);
        }
    }
    return;
}
int main(void){
    scanf("%d",&n);
    init();
    for(int i=0;i<n-1;i++){
        scanf("%d%d",&u,&v);
        addEdge(u,v);
    }
    //找到离根节点最远的点s
    memset(vis,false,sizeof(vis));
    dfs(1,0);
    int s=0;
    for(int i=1;i<=n;i++){
        if(dep[i]>dep[s]){
            s=i;
        }
    }
    //再dfs一遍找到离s最远的点t
    //st就是树的直径
    memset(vis,0,sizeof(vis));
    dfs(s,0);
    int t=0;
    for(int i=1;i<=n;i++){
        if(dep[i]>dep[t]){
            t=i;
        }
    }
    printf("%d\n",dep[t]);
    return 0;
}
```

树形dp方法

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <vector>
using namespace std;
const int N=1e5+50;
const int M=1e6+50;
int n,u,v;
struct Edge{
    int v,next;
}edge[M];
int cnt;
int head[N],fir[N],sec[N];
bool vis[N];
void init(){
    cnt=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int u,int v){
    edge[cnt]=Edge{v,head[u]};
    head[u]=cnt++;
    edge[cnt]=Edge{u,head[v]};
    head[v]=cnt++;
}
int ans;
//树形dp
void dfs(int u){
    vis[u]=1;
    //最长路和次长路
    fir[u]=0;
    sec[u]=0;
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        if(!vis[v]){
            //递归下去更新子节点信息
            dfs(v);
            //子节点的最长路+1更长，可以更新当前节点信息
            //当前节点的最长路变成次长路
            if(fir[v]+1>=fir[u]){
                sec[u]=fir[u];
                fir[u]=fir[v]+1;
            }
            //子节点信息只能更新次长路
            else if(fir[v]+1>sec[u]){
                sec[u]=fir[v]+1;
            }
        }
    }
    //更新整个树的信息
    ans=max(ans,fir[u]+sec[u]);
    return;
}
int main(void){
    scanf("%d",&n);
    init();
    for(int i=0;i<n-1;i++){
        scanf("%d%d",&u,&v);
        addEdge(u,v);
    }
    dfs(1);
    printf("%d\n",ans);
    return 0;
}
```



