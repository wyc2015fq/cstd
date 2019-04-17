# 二分图最大匹配匈牙利算法（C++版） - westbrook1998的博客 - CSDN博客





2018年11月03日 22:45:29[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：119








```
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=2050;
const int M=10050;
int n,m;
int u,v;
struct Edge{
    int to,next;
}edge[M];
int cnt;
int head[N];
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
int link[N];
bool vis[N];
bool dfs(int u){
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].to;
        if(!vis[v]){
            vis[v]=true;
            if(link[v]==-1 || dfs(link[v])){
                link[v]=u;
                return true;
            }
        }
    }
    return false;
}
int hungary(){
    int res=0;
    memset(link,-1,sizeof(link));
    //这里扫的是所有点，所以dfs里就只记录单向对应点即可，但最后求的答案要除以2
    for(int u=1;u<=n;u++){
        memset(vis,false,sizeof(vis));
        if(dfs(u)){
            res++;
        }
    }
    return res;
}
int main(void){
    scanf("%d%d",&n,&m);
    init();
    while(m--){
        scanf("%d%d",&u,&v);
        addEdge(u,v);
    }
    printf("%d\n",hungary()/2);
    // for(int i=1;i<=n;i++){
    //     printf("%d %d\n",i,link[i]);
    // }
    return 0;
}
```



