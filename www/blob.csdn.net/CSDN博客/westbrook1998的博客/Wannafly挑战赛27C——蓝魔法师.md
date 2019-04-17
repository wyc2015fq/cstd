# Wannafly挑战赛27C——蓝魔法师 - westbrook1998的博客 - CSDN博客





2018年10月27日 18:18:19[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：86








不会树形dp

```
#include <bits/stdc++.h>
using namespace std;
const int N=2e3+50;
const int MOD=998244353;
typedef long long ll;
int n,k;
int u,v;
int cnt;
int head[N];
struct Edge{
    int v,next;
}edge[N<<1];
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
ll dp[N][N],sz[N],tmp[N];
void dfs(int u,int p){
    //只有自身一个节点的情况
    //sz保存包括自身的所有子节点数
    //dp[i][j]表示只考虑根为i的子树,包含节点i的联通块大小为j的方案个数
    //dp[i][0]表示dp[i][1]+...+dp[i][k]
    sz[u]=dp[u][1]=1;
    for(int t=head[u];t!=-1;t=edge[t].next){
        int v=edge[t].v;
        if(v==p){
            continue;
        }
        dfs(v,u);
        //??
        memset(tmp,0,sizeof(tmp));
        //这里不是遍历u的所有子节点，而是遍历所有子孙节点
        for(int i=1;i<=sz[u];i++){
            for(int j=0;j<=sz[v] && i+j<=k;j++){
                //u提供连通分量的i个 v(u的子节点，与u相连)提供j个，连起来该连通分量大小就是i+j
                tmp[i+j]=(tmp[i+j]+(dp[u][i]*dp[v][j])%MOD)%MOD;
            }
        }
        for(int i=0;i<=k;i++){
            dp[u][i]=tmp[i];
        }
        sz[u]+=sz[v];
    }
    for(int i=1;i<=k;i++){
        dp[u][0]=(dp[u][0]+dp[u][i])%MOD;
    }
}
int main(void){
    freopen("data.txt","r",stdin);
    scanf("%d%d",&n,&k);
    init();
    for(int i=0;i<n-1;i++){
        scanf("%d%d",&u,&v);
        addEdge(u,v);
    }
    dfs(1,0);
    printf("%lld\n",dp[1][0]);
    return 0;
}
```



