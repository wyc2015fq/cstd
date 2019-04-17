# bzoj1954The xor-longest Path - westbrook1998的博客 - CSDN博客





2018年11月27日 21:43:52[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：33








给一个带权树,求出树中异或值最大的一个路径的异或值

异或的性质使得可以用类似前缀和那种思想

我们先求出每个节点到根的异或值,然后插入字典树中,然后就变成01字典树的模板题了,枚举所有异或值,查询与这个异或值(节点到根的异或值)异或最大的值
代码:

```
#include <bits/stdc++.h>
using namespace std;
const int N=1e5+50;
const int M=2e5+50;
int n,u,v,w;
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
//先求出每个节点到根路径的异或值
int xo[N];
void dfs(int u,int f){
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        int w=edge[i].w;
        if(v==f){
            continue;
        }
        xo[v]=xo[u]^w;
        dfs(v,u);
    }
}
int tree[33*N][2];
int num;
void init_trie(){
    memset(tree,0,sizeof(tree));
    num=0;
}
void insert(int x){
    int root=0;
    for(int i=30;i>=0;i--){
        int id=(x>>i)&1;
        if(!tree[root][id]){
            tree[root][id]=++num;
        }
        root=tree[root][id];
    }
}
int query(int x){
    int root=0;
    int ans=0;
    for(int i=30;i>=0;i--){
        //printf("%d\n",i);
        int id=(x>>i)&1;
        if(tree[root][id^1]){
            root=tree[root][id^1];
            ans+=(1<<i);
            //printf("%d %d\n",x,1<<i);
        }else{
            root=tree[root][id];
        }
    }
    return ans;
}
int main(void){
    while(~scanf("%d",&n)){
        init();
        for(int i=0;i<n-1;i++){
            scanf("%d%d%d",&u,&v,&w);
            addEdge(u,v,w);
        }
        dfs(1,0);
        init_trie();
        for(int i=1;i<=n;i++){
            insert(xo[i]);
        }
        int ans=0;
        for(int i=1;i<=n;i++){
            ans=max(ans,query(xo[i]));
        }
        printf("%d\n",ans);
    }
    return 0;
}
```




