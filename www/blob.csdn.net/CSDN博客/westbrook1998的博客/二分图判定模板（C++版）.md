# 二分图判定模板（C++版） - westbrook1998的博客 - CSDN博客





2018年11月03日 22:02:39[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：47








dfs染色法

若是二分图也就说明不存在奇环

```
#include <bits/stdc++.h>
using namespace std;
const int N=1e4+50;
const int M=1e5+50;
int n,m;
int u,v;
struct Edge{
    int v,next;
}edge[M];
int cnt,head[N];
int color[N];
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
bool dfs(int u,int c){
    color[u]=c;
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        if(color[v]==c){
            return false;
        }
        if(color[v]==0 && !dfs(v,-c)){
            return false;
        }
    }
    return true;
}
void solve(){
    memset(color,0,sizeof(color));
    for(int i=0;i<n;i++){
        if(color[i]==0){
            if(!dfs(i,1)){
                printf("Wrong\n");
                return;
            }
        }
    }
    printf("Correct\n");
}
int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%d%d",&n,&m);
        init();
        while(m--){
            scanf("%d%d",&u,&v);
            addEdge(u,v);
        }
        solve();
    }
    return 0;
}
```



