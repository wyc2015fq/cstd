# Codeforces977E——Cyclic Components - westbrook1998的博客 - CSDN博客





2018年08月15日 11:46:17[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：41








> 
You are given an undirected graph consisting of n vertices and m edges. Your task is to find the number of connected components which are cycles. 

  Here are some definitions of graph theory. 

  An undirected graph consists of two sets: set of nodes (called vertices) and set of edges. Each edge connects a pair of vertices. All edges are bidirectional (i.e. if a vertex a is connected with a vertex b, a vertex b is also connected with a vertex a). An edge can’t connect vertex with itself, there is at most one edge between a pair of vertices. 

  Two vertices u and v belong to the same connected component if and only if there is at least one path along edges connecting u and v. 

  A connected component is a cycle if and only if its vertices can be reordered in such a way that: 

  the first vertex is connected with the second vertex by an edge, 

  the second vertex is connected with the third vertex by an edge, 

  … 

  the last vertex is connected with the first vertex by an edge, 

  all the described edges of a cycle are distinct. 

  A cycle doesn’t contain any other edges except described above. By definition any cycle contains three or more vertices. 
![这里写图片描述](https://odzkskevi.qnssl.com/cc882a4a20428391902fb03b020b7880?v=1534000726)

   There are 6 connected components, 2 of them are cycles: [7,10,16] and [5,11,9,15]. 

  Input 

  The first line contains two integer numbers n and m (1≤n≤2⋅105, 0≤m≤2⋅105) — number of vertices and edges. 

  The following m lines contains edges: edge i is given as a pair of vertices vi, ui (1≤vi,ui≤n, ui≠vi). There is no multiple edges in the given graph, i.e. for each pair (vi,ui) there no other pairs (vi,ui) and (ui,vi) in the list of edges. 

  Output 

  Print one integer — the number of connected components which are also cycles. 

  Examples 

  Input 

  5 4 

  1 2 

  3 4 

  5 4 

  3 5 

  Output 

  1 

  Input 

  17 15 

  1 8 

  1 12 

  5 11 

  11 9 

  9 15 

  15 5 

  4 13 

  3 13 

  4 3 

  10 16 

  7 10 

  16 7 

  14 3 

  14 4 

  17 6 

  Output 

  2 

  Note 

  In the first example only component [3,4,5] is also a cycle. 

  The illustration above corresponds to the second example.
题意是给一个图，求单链环的个数，用dfs 

建图的同时记录度数，然后在dfs的时候如果有度数不等于2的，标记一下！！ 不要直接退出，然后外边枚举搜索的第一个点，如果没有标记，则为一个环

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
const int N=200050;
int n,m;
vector<int> g[N];
int degree[N];
int a,b;
int cnt;
int ans;
bool vis[N];
int gidx[N];
int flag;
void dfs(int u){
    vis[u]=true;
    if(degree[u]!=2){
        flag=1;
    }
    int l=g[u].size();
    for(int i=0;i<l;i++){
        if(vis[g[u][i]]){
            continue;
        }
        dfs(g[u][i]);
    }
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d%d",&n,&m);
    for(int i=0;i<m;i++){
        scanf("%d%d",&a,&b);
        g[a].push_back(b);
        g[b].push_back(a);
        degree[a]++;
        degree[b]++;
    }
    for(int i=1;i<=n;i++){
        flag=0;
        if(!vis[i]){
            dfs(i);
            if(!flag){
                ans++;
            }
        }
    }
    printf("%d\n",ans);
    return 0;
}
```





