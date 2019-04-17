# Drainage Ditches（POJ-1273） - Alex_McAvoy的博客 - CSDN博客





2018年12月29日 10:32:37[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：55








> 
# Problem Description

Every time it rains on Farmer John's fields, a pond forms over Bessie's favorite clover patch. This means that the clover is covered by water for awhile and takes quite a long time to regrow. Thus, Farmer John has built a set of drainage ditches so that Bessie's clover patch is never covered in water. Instead, the water is drained to a nearby stream. Being an ace engineer, Farmer John has also installed regulators at the beginning of each ditch, so he can control at what rate water flows into that ditch. 

Farmer John knows not only how many gallons of water each ditch can transport per minute but also the exact layout of the ditches, which feed out of the pond and into each other and stream in a potentially complex network. 

Given all this information, determine the maximum rate at which water can be transported out of the pond and into the stream. For any given ditch, water flows in only one direction, but there might be a way that water can flow in a circle. 

# **Input**

The input includes several cases. For each case, the first line contains two space-separated integers, N (0 <= N <= 200) and M (2 <= M <= 200). N is the number of ditches that Farmer John has dug. M is the number of intersections points for those ditches. Intersection 1 is the pond. Intersection point M is the stream. Each of the following N lines contains three integers, Si, Ei, and Ci. Si and Ei (1 <= Si, Ei <= M) designate the intersections between which this ditch flows. Water will flow through this ditch from Si to Ei. Ci (0 <= Ci <= 10,000,000) is the maximum rate at which water will flow through the ditch.

# Output

For each case, output a single integer, the maximum rate at which water may emptied from the pond.

# Sample Input

**5 41 2 401 4 202 4 202 3 303 4 10**

# Sample Output

**50**


题意：给出 m 个池塘，n 个水渠，给出 n 个水渠连接的点和能流过的最大流量，求从源点到汇点流过的最大流量

思路：最大流问题，套用 Dinic 模版即可，要注意的是，是 m 个点 n 条边。

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
#define N 1001
#define LL long long
using namespace std;
int n,m;
int S,T;
int level[N];
struct Edge{
    int cap;
    int flow;
}edge[N][N];
bool bfs(){
    memset(level,0,sizeof(level));

    queue<int> Q;
    Q.push(S);
    level[S]=1;
    while(!Q.empty()){
        int x=Q.front();
        Q.pop();
        for(int y=1;y<=n;y++){
            if(!level[y]&&edge[x][y].cap>edge[x][y].flow){
                level[y]=level[x]+1;
                Q.push(y);
            }
        }
    }
    return level[T]!=0;
}

int dfs(int x,int cp){
    if(x==n)
        return cp;

    int flow=cp;
    for(int y=1;y<=n;y++){
        if(level[x]+1==level[y]){
            if(edge[x][y].cap>edge[x][y].flow){
                int minn=min(flow,edge[x][y].cap-edge[x][y].flow);
                int newFlow=dfs(y,minn);
                edge[x][y].flow+=newFlow;
                edge[y][x].flow-=newFlow;
                flow-=newFlow;
            }
        }

        if(flow==0)
            break;
    }

    return cp-flow;
}
int dinic(){
    int flow=0;
    int tf=0;
    while(bfs()){
        while(tf=dfs(1,INF)){
            flow+=tf;
        }
    }
    return flow;
}
int main(){
    while(scanf("%d%d",&m,&n)!=EOF&&(m+n)){
        memset(edge,0,sizeof(edge));
        while(m--){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            edge[x][y].cap+=w;
        }
        S=1,T=n;
        printf("%d\n",dinic());
    }
    return 0;
}
```





