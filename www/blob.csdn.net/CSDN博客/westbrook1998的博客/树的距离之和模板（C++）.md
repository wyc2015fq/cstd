# 树的距离之和模板（C++） - westbrook1998的博客 - CSDN博客





2018年10月22日 09:26:35[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：29








求出每个节点到到其他所有节点的距离之和

先dfs(1)预处理求出每个节点所在的子树的节点数量（包含本身）

再通过树形dp求解
```
#include <cstdio>
#include <algorithm>
#include <cstring>
typedef long long ll;
using namespace std;
const int N=1000050;
int n,u,v;
int cnt;
int head[N];
struct Edge{
    int v,next;
}edge[N];
int num[N];
ll dp[N];
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
void dfs(int u,int d){
    //num数组保存u节点所在子树的节点数（包括本身）
    vis[u]=true;
    num[u]=1;
    //先选定1为根
    //dp数组表示节点到其他节点的距离之和
    //对根而言距离也就是各节点的深度
    dp[1]+=d;
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        if(!vis[v]){
            dfs(v,d+1);
            num[u]+=num[v];
        }
    }
}
void solve(int u){
    vis[u]=true;
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        if(!vis[v]){
            //v是u的子节点，v到所有节点的距离dp[v]就比u到所有节点的距离dp[u]少num[v]
            //同时还要多上父节点u的其他子树的距离n-num[x]
            dp[v]=dp[u]+(n-num[v])-num[v];
            solve(v);
        }
    }
}
int main(void){
    scanf("%d",&n);
    init();
    for(int i=0;i<n-1;i++){
        scanf("%d%d",&u,&v);
        addEdge(u,v);
    }
    memset(vis,false,sizeof(vis));
    dfs(1,0);
    for(int i=1;i<=n;i++){
        printf("%d %d\n",i,num[i]);
    }
    memset(vis,false,sizeof(vis));
    solve(1);
    for(int i=1;i<=n;i++){
        printf("%lld\n",dp[i]);
    }
    return 0;
}
```




