# 树的重心模板（C++） - westbrook1998的博客 - CSDN博客





2018年10月22日 08:01:45[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：131








> 
树的重心

性质1 重心到所有节点的距离和最小（边权为1）

性质2 两棵树合并，新的重心在两棵树重心的路径上

性质3 一棵树添加或删除一个节点，重心最多移动一条边的位置

性质4 重心的最大的子树的节点数最小
```
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const int N=100050;
const int M=100050;
const int INF=0x3f3f3f3f;
int n,u,v,Min;
int cnt,head[N];
struct Edge{
    int to,next;
}edge[M];
int siz[N],dp[N];
void init(){
    cnt=0;
    Min=INF;
    memset(head,-1,sizeof(head));
    memset(siz,0,sizeof(siz));
    memset(dp,0,sizeof(dp));
}
void addEdge(int u,int v){
    edge[cnt]=Edge{v,head[u]};
    head[u]=cnt++;
    edge[cnt]=Edge{u,head[v]};
    head[v]=cnt++;
}
void dfs(int u,int p){
    //siz数组保存该节点的子节点数量
    siz[u]=1;
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].to;
        if(v==p){
            continue;
        }
        dfs(v,u);
        //用子节点信息更新该节点的子节点总数
        siz[u]+=siz[v];
        //dp数组保存删去u节点后，所有子连通块的最大节点数
        //找到最大的子连通块的节点数
        if(dp[u]<siz[v]){
            dp[u]=siz[v];
        }
    }
    //dp[u]是u的子树中的最大连通块节点数
    //而n-siz[u]则是u的上方子树（实际上是父节点和兄弟节点）的节点数（必连通）
    dp[u]=max(dp[u],n-siz[u]);
    Min=min(Min,dp[u]);
}
int main(void){
    scanf("%d",&n);
    init();
    for(int i=0;i<n-1;i++){
        scanf("%d%d",&u,&v);
        addEdge(u,v);
    }
    dfs(1,-1);
    for(int i=1;i<=n;i++){
    	//可能有多个重心
        if(dp[i]==Min){
            printf("%d ",i);
        }
    }
    printf("\n");
    return 0;
}
```





