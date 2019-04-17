# 牛客练习赛32B Xor Path - westbrook1998的博客 - CSDN博客





2018年11月30日 22:06:19[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：177








> 
链接：[https://ac.nowcoder.com/acm/contest/272/B](https://ac.nowcoder.com/acm/contest/272/B)

来源：牛客网

时间限制：C/C++ 1秒，其他语言2秒

空间限制：C/C++ 262144K，其他语言524288K

64bit IO Format: %lld

题目描述

给定一棵n个点的树，每个点有权值。定义表示  到  的最短路径上，所有点的点权异或和。

对于，求所有的异或和。

输入描述:

第一行一个整数n。

接下来n-1行，每行2个整数u,v，表示u,v之间有一条边。

第n+1行有n个整数，表示每个点的权值。

输出描述:

输出一个整数，表示所有的异或和，其中。

示例1

输入

复制

4

1 2

1 3

1 4

1 2 3 4

输出

5
给一棵树,求所有任意两点路径上异或和的和

首先由异或的性质得到,我们只要求出每个点作为路径上的一点多少次(即被经过多少次)即可

然后可以考虑用树形dp来解决,先dfs下去之后根据子树的信息来更新父节点

这道题的状态转移分了几个部分,对一个节点来说,经过他的情况有几种,

1 子树每个节点都连向他,即`num[u]+=(siz[u]-1)`

2 他连向他的父节点的其他子树节点(兄弟子树),即`num[u]+=(n-siz[u])`

3 子树经过他和其他子树节点相连,即
```
for v in u.vec:
	tmp+=siz[v]*(siz[u]-1-siz[v]);
num[u]+=tmp/2
```

4 最后一个就是子树节点经过他和他的兄弟子树相连,即`nun[u]+=(siz[u]-1)*(n-siz[u])`

代码:

```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=5e5+50;
int siz[N];
ll val[N];
int n,u,v;
struct Edge{
    int v,next;
}edge[N*2];
int cnt,head[N];
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
ll num[N];
void dfs(int u,int f){
    siz[u]=1;
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        if(v==f){
            continue;
        }
        dfs(v,u);
        siz[u]+=siz[v];
    }
    num[u]=(siz[u]-1)*(n-siz[u]);
    ll tmp=0;
    for(int i=head[u];i!=-1;i=edge[i].next){
        int v=edge[i].v;
        if(v==f){
            continue;
        }
        tmp+=siz[v]*(siz[u]-1-siz[v]);
    }
    num[u]+=tmp/2;
    num[u]+=siz[u]-1;
    num[u]+=(n-siz[u]);
}
int main(void){
    scanf("%d",&n);
    init();
    for(int i=0;i<n-1;i++){
        scanf("%d%d",&u,&v);
        addEdge(u,v);
    }
    for(int i=1;i<=n;i++){
        scanf("%lld",&val[i]);
    }
    dfs(1,0);
    ll ans=0;
    for(int i=1;i<=n;i++){
        //printf("%lld ",num[i]);
        if(num[i]%2){
            ans^=val[i];
        }
    }
    //printf("\n");
    printf("%lld\n",ans);
    return 0;
}
```






