# Destroying Roads（CF-302B） - Alex_McAvoy的博客 - CSDN博客





2019年01月16日 19:38:40[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：28








> 
# Problem Description

In some country there are exactly n cities and m bidirectional roads connecting the cities. Cities are numbered with integers from 1 to n. If cities a and b are connected by a road, then in an hour you can go along this road either from city a to city b, or from city b to city a. The road network is such that from any city you can get to any other one by moving along the roads.

You want to destroy the largest possible number of roads in the country so that the remaining roads would allow you to get from city s1 to city t1 in at most l1 hours and get from city s2 to city t2 in at most l2 hours.

Determine what maximum number of roads you need to destroy in order to meet the condition of your plan. If it is impossible to reach the desired result, print -1.

# Input

The first line contains two integers n, m (1 ≤ n ≤ 3000, ) — the number of cities and roads in the country, respectively.

Next m lines contain the descriptions of the roads as pairs of integers ai, bi (1 ≤ ai, bi ≤ n, ai ≠ bi). It is guaranteed that the roads that are given in the description can transport you from any city to any other one. It is guaranteed that each pair of cities has at most one road between them.

The last two lines contains three integers each, s1, t1, l1 and s2, t2, l2, respectively (1 ≤ si, ti ≤ n, 0 ≤ li ≤ n).

# Output

Print a single number — the answer to the problem. If the it is impossible to meet the conditions, print -1.

# Examples

**Input**

5 4

1 2

2 3

3 4

4 5

1 3 2

3 5 2

**Output**

0

**Input**

5 4

1 2

2 3

3 4

4 5

1 3 2

2 4 2

**Output**

1

**Input**

5 4

1 2

2 3

3 4

4 5

1 3 2

3 5 1

**Output**

-1


题意：给出 n 个点 m 条边权为 1 的无向边，要求满足从 s1 到 t1 的距离不超过 d1，s2 到 t2 的距离不超过 t2，尽可能去破坏最多的道路，若可以输出要破坏的道路数量，否则输出 -1

思路： 由于要求从 s1/s2 到 t1/t2 的距离不超过 d1/d2，且要破坏最多的道路，因此求 s1/s2 到 t1/t2 的最短路，然后判断能否在 d1/d2 范围内到达，如果能在范围内到达则将 s1-t1、s2-t2 两条路的边数相加，然后枚举找这两条路重叠的边数，最后的结果等于总的边数减去两条路的边数再加上重叠的边数

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
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define MOD 16007
#define INF 0x3f3f3f3f
#define N 5001
#define LL long long
using namespace std;
int n,m;
vector<int> G[N];
int dis[N][N];
bool vis[N];
void SPFA(){
    for(int i=1;i<=n;i++){
        memset(vis,false,sizeof(vis));
        vis[i]=true;
        queue<int> Q;
        Q.push(i);
        while(!Q.empty()){
            int x=Q.front();
            Q.pop();
            for(int j=0;j<G[x].size();j++){
                int y=G[x][j];
                if(vis[y])
                    continue;

                vis[y]=true;
                dis[i][y]=dis[i][x]+1;
                Q.push(y);
            }
        }
    }
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF){
        for(int i=1;i<=m;i++)
            G[i].clear();

        for(int i=1;i<=m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
            G[y].push_back(x);
        }

        int s1,t1,d1,s2,t2,d2;
        scanf("%d%d%d%d%d%d",&s1,&t1,&d1,&s2,&t2,&d2);
        SPFA();
        if(dis[s1][t1]>d1||dis[s2][t2]>d2)
            printf("-1\n");
        else{
            int res=dis[s1][t1]+dis[s2][t2];
            for(int i=1;i<=n;i++){
                for(int j=1;j<=n;j++){
                    if(dis[s1][i]+dis[i][j]+dis[j][t1]<=d1&&dis[s2][i]+dis[i][j]+dis[j][t2]<=d2)
                        res=min(res,dis[s1][i]+dis[i][j]+dis[j][t1]+dis[s2][i]+dis[j][t2]);
                    if(dis[s1][i]+dis[i][j]+dis[j][t1]<=d1&&dis[t2][i]+dis[i][j]+dis[j][s2]<=d2)
                        res=min(res,dis[s1][i]+dis[i][j]+dis[j][t1]+dis[t2][i]+dis[j][s2]);
                }
            }
            printf("%d\n",m-res);
        }
    }
    return 0;
}
```





