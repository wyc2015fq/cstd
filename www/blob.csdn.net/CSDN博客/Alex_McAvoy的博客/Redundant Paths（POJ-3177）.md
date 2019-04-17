# Redundant Paths（POJ-3177） - Alex_McAvoy的博客 - CSDN博客





2018年10月29日 10:08:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：44








> 
# Problem Description

In order to get from one of the F (1 <= F <= 5,000) grazing fields (which are numbered 1..F) to another field, Bessie and the rest of the herd are forced to cross near the Tree of Rotten Apples. The cows are now tired of often being forced to take a particular path and want to build some new paths so that they will always have a choice of at least two separate routes between any pair of fields. They currently have at least one route between each pair of fields and want to have at least two. Of course, they can only travel on Official Paths when they move from one field to another. 

Given a description of the current set of R (F-1 <= R <= 10,000) paths that each connect exactly two different fields, determine the minimum number of new paths (each of which connects exactly two fields) that must be built so that there are at least two separate routes between any pair of fields. Routes are considered separate if they use none of the same paths, even if they visit the same intermediate field along the way. 

There might already be more than one paths between the same pair of fields, and you may also build a new path that connects the same fields as some other path.

# Input

Line 1: Two space-separated integers: F and R 

Lines 2..R+1: Each line contains two space-separated integers which are the fields at the endpoints of some path.

# Output

Line 1: A single integer that is the number of new paths that must be built.

# Sample Input

7 7

1 2

2 3

3 4

2 5

4 5

5 6

5 7

# Sample Output

2


题意：给出一个 n 个点 m 条边的无向图，现在要向图中加边，求最少加几条边，能使图变为边双连通图

思路：与 [Road Construction（POJ-3352）](https://blog.csdn.net/u011815404/article/details/83501751)相似，不同的是，本题有重边，要先进行去重

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 20001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
vector<int> G[N];
int n,m;
int dfn[N],low[N];
int degree[N];
int block_cnt;
bool repeat[N][N];//去重用数组
int Tarjan(int x,int father){
    int lowx=dfn[x]=++block_cnt;

    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];

        if(y==father)
            continue;

        if(dfn[y]==0){
            int lowy=Tarjan(y,x);
            lowx=min(lowx,lowy);
        }
        else if(dfn[y]<dfn[x]){
            lowx=min(lowx,dfn[y]);
        }
    }
    return low[x]=lowx;
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF){
        block_cnt=0;
        memset(dfn,0,sizeof(dfn));
        memset(degree,0,sizeof(degree));
        for(int i=0;i<n;i++)
            G[i].clear();

        //去重存储
        for(int i=1;i<=m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            repeat[x][y]=true;
            repeat[y][x]=true;
        }
        for(int i=1;i<=n;i++){
            for(int j=i+1;j<=n;j++){
                if(repeat[i][j]){
                    G[i].push_back(j);
                    G[j].push_back(i);
                }
            }
        }

        Tarjan(1,-1);//求所有点的low值
        for(int x=1;x<=n;x++){//遍历每条边
            for(int i=0;i<G[x].size();i++){
                int y=G[x][i];
                if(low[x]!=low[y])//每个不同的low值代表一个边双连通分量
                    degree[low[y]]++;
            }
        }

        int cnt=0;
        for(int i=1;i<=n;i++)
            if(degree[i]==1)
                cnt++;
        printf("%d\n",(cnt+1)/2);//加边条数
    }
    return 0;
}
```



