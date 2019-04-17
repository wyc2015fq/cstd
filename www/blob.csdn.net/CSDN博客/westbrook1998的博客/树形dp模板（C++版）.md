# 树形dp模板（C++版） - westbrook1998的博客 - CSDN博客





2018年10月27日 21:05:11[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：156








poj2342

最简单的树形dp入门，树上的最大点权独立集

```
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=6e3+50;
const int M=2e4+50;
struct Edge{
    int v,next;
}edge[M];
int cnt,head[N];
//dp[i][1/0]表示该点取或不取所能获得的最大权值
int dp[N][2];
int n,u,v;
bool ind[N];
void init(){
    cnt=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int u,int v){
    edge[cnt]=Edge{v,head[u]};
    head[u]=cnt++;
}
void dfs(int u){
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        dfs(v);
        dp[u][1]+=dp[v][0];
        dp[u][0]+=max(dp[v][0],dp[v][1]);
    }
}
int main(void){
    while(~scanf("%d",&n)){
        init();
        memset(ind,false,sizeof(ind));
        memset(dp,0,sizeof(dp));
        for(int i=1;i<=n;i++){
            scanf("%d",&dp[i][1]);
        }
        while(scanf("%d%d",&v,&u) && (u+v)){
            addEdge(u,v);
            ind[v]=true;
        }
        int root=-1;
        for(int i=1;i<=n;i++){
            if(!ind[i]){
                root=i;
                break;
            }
        }
        dfs(root);
        printf("%d\n",max(dp[root][0],dp[root][1]));
    }
    return 0;
}
```



