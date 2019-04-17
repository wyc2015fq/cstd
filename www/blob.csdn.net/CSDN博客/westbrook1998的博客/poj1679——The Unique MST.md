# poj1679——The Unique MST - westbrook1998的博客 - CSDN博客





2018年08月13日 22:14:06[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：29标签：[最小生成树																[次小生成树](https://so.csdn.net/so/search/s.do?q=次小生成树&t=blog)](https://so.csdn.net/so/search/s.do?q=最小生成树&t=blog)







> 
Given a connected undirected graph, tell if its minimum spanning tree is unique. 


Definition 1 (Spanning Tree): Consider a connected, undirected graph G = (V, E). A spanning tree of G is a subgraph of G, say T = (V’, E’), with the following properties:  

1. V’ = V.  

2. T is connected and acyclic. 
Definition 2 (Minimum Spanning Tree): Consider an edge-weighted, connected, undirected graph G = (V, E). The minimum spanning tree T = (V, E’) of G is the spanning tree that has the smallest total cost. The total cost of T means the sum of the weights on all the edges in E’.  

Input 

The first line contains a single integer t (1 <= t <= 20), the number of test cases. Each case represents a graph. It begins with a line containing two integers n and m (1 <= n <= 100), the number of nodes and edges. Each of the following m lines contains a triple (xi, yi, wi), indicating that xi and yi are connected by an edge with weight = wi. For any two nodes, there is at most one edge connecting them. 

Output 

For each input, if the MST is unique, print the total cost of it, or otherwise print the string ‘Not Unique!’. 

Sample Input 

2 

3 3 

1 2 1 

2 3 2 

3 1 3 

4 4 

1 2 2 

2 3 2 

3 4 2 

4 1 2 

Sample Output 

3 

Not Unique!
这个思路就是求一下次小生成树，看是否相等，一个条件写漏!调了半天……

代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=105;
const int INF=0x3f3f3f3f;
int t,n,m,a,b,w;
int g[N][N];
int low[N];
bool vis[N];
bool used[N][N];
int pre[N];
int path[N][N];
int prim(){
    int pos=1;
    int Min=INF;
    int ans=0;
    for(int i=1;i<=n;i++){
        if(i!=pos){
            low[i]=g[pos][i];
        }
        pre[i]=pos;
    }
    vis[pos]=true;
    pre[pos]=-1;
    for(int i=1;i<n;i++){
        Min=INF;
        for(int j=1;j<=n;j++){
            if(!vis[j] && low[j]<Min){
                Min=low[j];
                pos=j;
            }
        }
        if(Min==INF){
            return -1;
        }
        ans+=Min;
        vis[pos]=true;
        used[pos][pre[pos]]=used[pre[pos]][pos]=true;
        for(int j=1;j<=n;j++){
            if(vis[j] && j!=pos){
                path[j][pos]=path[pos][j]=max(path[j][pre[pos]],low[pos]);
            }
            if(!vis[j] && low[j]>g[pos][j]){
                low[j]=g[pos][j];
                pre[j]=pos;
            }
        }
    }
    return ans;
}
int second_tree(int ans){
    int res=INF;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=n;j++){
            if(i!=j && !used[i][j]){
                res=min(res,ans-path[i][j]+g[i][j]);
            }
        }
    }
    return res;
}
void init(){
    _clr(g,INF);
    for(int i=1;i<=n;i++){
        g[i][i]=0;
    }
    _clr(vis,false);
    _clr(used,false);
    _clr(path,0);
    _clr(low,0);
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d",&t);
    while(t--){
        init();
        scanf("%d%d",&n,&m);
        for(int i=1;i<=m;i++){
            scanf("%d%d%d",&a,&b,&w);
            g[a][b]=g[b][a]=w;
        }
        int ans=prim();
        int res=second_tree(ans);
        if(ans==res){
            printf("Not Unique!\n");
        }
        else{
            printf("%d\n",ans);
        }
    }
    return 0;
}
```






