# Codeforces999E——Reachability from the Capital - westbrook1998的博客 - CSDN博客





2018年08月10日 18:25:28[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：36








> 
There are n cities and m roads in Berland. Each road connects a pair of cities. The roads in Berland are one-way. 

  What is the minimum number of new roads that need to be built to make all the cities reachable from the capital? 

  New roads will also be one-way. 

  Input 

  The first line of input consists of three integers n, m and s (1≤n≤5000,0≤m≤5000,1≤s≤n) — the number of cities, the number of roads and the index of the capital. Cities are indexed from 1 to n. 

  The following m lines contain roads: road i is given as a pair of cities ui, vi (1≤ui,vi≤n, ui≠vi). For each pair of cities (u,v), there can be at most one road from u to v. Roads in opposite directions between a pair of cities are allowed (i.e. from u to v and from v to u). 

  Output 

  Print one integer — the minimum number of extra roads needed to make all the cities reachable from city s. If all the cities are already reachable from s, print 0. 

  Examples 

  Input 

  9 9 1 

  1 2 

  1 3 

  2 3 

  1 5 

  5 6 

  6 1 

  1 8 

  9 8 

  7 1 

  Output 

  3 

  Input 

  5 4 5 

  1 2 

  2 3 

  3 4 

  4 1 

  Output 

  1 

  Note 

  The first example is illustrated by the following: 
![这里写图片描述](https://odzkskevi.qnssl.com/1e13b487ce653856df2f3e7dca133292?v=1533831383)

  For example, you can add roads (6,4), (7,9), (1,7) to make all the cities reachable from s=1. 

  The second example is illustrated by the following: 
![这里写图片描述](https://odzkskevi.qnssl.com/83a1ec3a38b2b28bfea66f71618fc714?v=1533831383)

  In this example, you can add any one of the roads (5,1), (5,2), (5,3), (5,4) to make all the cities reachable from s=5.
这题是看了大佬的博客才大概有的思路 

其实就是求一个有向图有多少个不同于`capital`点的根节点，因为只要从`capital`连一个到这些根节点，根节点以下的点也可以到达 

所以就是用`dfs`来做 

然后注意一个很重要的问题，环的问题，因为`dfs`是是循环进行的，也就是`dfs(1,1) dfs(2,2)` 这样，所以当有环的情况而且`capital`点不是1的话，他会在1的`dfs`时被记录，变成根节点是1，但其实这种含有`capital`点的环的根节点应该算`capital`点，所以在`for`循环之前要先`dfs(s,s)`
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <cstring>
using namespace std;
const int MAXN=5050;
int n,m,s;
int u,v;
//保存根节点
int fa[MAXN];
int vis[MAXN];
vector<int> G[MAXN];
set<int> res;
void dfs(int u,int f){
    //如果父亲节点已经是根节点
    if(fa[u]==f){
        return;
    }
    fa[u]=f;
    for(int i=0;i<G[u].size();i++){
        dfs(G[u][i],f);
    }
}
int main(void){
    scanf("%d%d%d",&n,&m,&s);
    while(m--){
        scanf("%d%d",&u,&v);
        G[u].push_back(v);
    }
    //先对起点dfs 比如环1-2-3-4-1  2为起点 如果不先dfs(2,2) 最终2的根节点记为1 而实际上要记为2
    dfs(s,s);
    //遍历一遍确定根节点
    for(int i=1;i<=n;i++){
        if(fa[i]==0){
            dfs(i,i);
        }
    }
    //标记s是否是根节点之一 如果是 则结果要减去1
    int flag=0;
    for(int i=1;i<=n;i++){
        //printf("%d ",fa[i]);
        if(fa[i]==s){
            flag=1;
        }
        res.insert(fa[i]);
    }
    //printf("\n");
    printf("%d\n",res.size()-flag);
    return 0;
}
```






