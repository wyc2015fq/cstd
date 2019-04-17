# Jimmy’s Assignment（HDU 1845） - Alex_McAvoy的博客 - CSDN博客





2018年11月28日 21:27:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：27








> 
# Problem Description

Jimmy is studying Advanced Graph Algorithms at his university. His most recent assignment is to find a maximum matching in a special kind of graph. This graph is undirected, has N vertices and each vertex has degree 3. Furthermore, the graph is 2-edge-connected (that is, at least 2 edges need to be removed in order to make the graph disconnected). A matching is a subset of the graph’s edges, such that no two edges in the subset have a common vertex. A maximum matching is a matching having the maximum cardinality.

  Given a series of instances of the special graph mentioned above, find the cardinality of a maximum matching for each instance.

# **Input**

The first line of input contains an integer number T, representing the number of graph descriptions to follow. Each description contains on the first line an even integer number N (4<=N<=5000), representing the number of vertices. Each of the next 3*N/2 lines contains two integers A and B, separated by one blank, denoting that there is an edge between vertex A and vertex B. The vertices are numbered from 1 to N. No edge may appear twice in the input.

# Output

For each of the T graphs, in the order given in the input, print one line containing the cardinality of a maximum matching.

# Sample Input

**241 21 31 42 32 43 441 21 31 42 32 43 4**

# Sample Output

**22**


题意：给出一 3-正则图，如果想让该图不连通，至少需要删除两条边（每个点都处于一个环中且原图连通），现在要尽量找出多的点对，使得每对点中都有边

思路：对边的两点 x、y 做两条边，然后找最大匹配数，由于左右两侧所代表的点是相同的，比如从左边 1 出发选择了右边的 3，后面的匹配时右边的 1 也就不能选了，为解决这种问题，需要构造双向边，然后跑最大匹配，最后结果除以 2 即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 10001
#define LL long long
using namespace std;
bool vis[N];
int link[N];
vector<int> G[N];
bool dfs(int x){
    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(!vis[y]){
            vis[y]=true;
            if(link[y]==-1 || dfs(link[y]))	{
                link[y]=x;
                return true;
            }
        }
    }
    return false;
}
int hungarian(int n)
{
    int ans=0;
    for(int i=1;i<=n;i++){
        memset(vis,false,sizeof(vis));
        if(dfs(i))
            ans++;
    }
    return ans;
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        int n;
        scanf("%d",&n);

        memset(link,-1,sizeof(link));
        for(int i=0;i<N;i++)
            G[i].clear();

        int x,y;
        for(int i=0;i<n*3/2;i++){
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
            G[y].push_back(x);
        }

        printf("%d\n",hungarian(n)/2);

    }
    return 0;
}
```

————————————————————————————————————————————————————

大神的思路：[点击这里](http://blog.sina.com.cn/s/blog_677a3eb30100llyn.html)

对于一 n 个点的三正则图，求最大匹配。

根据握手定理，n 一定是偶数。由于三正则图，而且题目提示是 2 边连通，所以图中不存在桥，也就是一定可以找到一条回路经过每个顶点至少一次（强连通的定义：强连通图一定存在一条回路记过每个顶点至少一次）

由于是三则图，每个顶点的度是 3，如果这条回路经过某个顶点 2 次，那么这个顶点的度就是 4，与条件矛盾。

因此，这条经过每个顶点一次的交错路就可以作出 n/2 匹配。

```cpp
#include<stdio.h>
int main(){
    int T;
    scanf("%d",&T);
    while(T--){
        int n;
        scanf("%d",&n);
        int x,y;
        for(int i=1;i<=n*3/2;i++)
            scanf("%d%d",&x,&y);
        printf("%d\n",n/2);
    }
    return 0;
}
```






