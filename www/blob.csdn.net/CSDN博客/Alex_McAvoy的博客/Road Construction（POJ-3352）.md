# Road Construction（POJ-3352） - Alex_McAvoy的博客 - CSDN博客





2018年10月29日 10:08:25[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：27
个人分类：[POJ																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

It's almost summer time, and that means that it's almost summer construction time! This year, the good people who are in charge of the roads on the tropical island paradise of Remote Island would like to repair and upgrade the various roads that lead between the various tourist attractions on the island.

The roads themselves are also rather interesting. Due to the strange customs of the island, the roads are arranged so that they never meet at intersections, but rather pass over or under each other using bridges and tunnels. In this way, each road runs between two specific tourist attractions, so that the tourists do not become irreparably lost.

Unfortunately, given the nature of the repairs and upgrades needed on each road, when the construction company works on a particular road, it is unusable in either direction. This could cause a problem if it becomes impossible to travel between two tourist attractions, even if the construction company works on only one road at any particular time.

So, the Road Department of Remote Island has decided to call upon your consulting services to help remedy this problem. It has been decided that new roads will have to be built between the various attractions in such a way that in the final configuration, if any one road is undergoing construction, it would still be possible to travel between any two tourist attractions using the remaining roads. Your task is to find the minimum number of new roads necessary.

# Input

The first line of input will consist of positive integers n and r, separated by a space, where 3 ≤ n ≤ 1000 is the number of tourist attractions on the island, and 2 ≤ r ≤ 1000 is the number of roads. The tourist attractions are conveniently labelled from 1 to n. Each of the following r lines will consist of two integers, v and w, separated by a space, indicating that a road exists between the attractions labelled v and w. Note that you may travel in either direction down each road, and any pair of tourist attractions will have at most one road directly between them. Also, you are assured that in the current configuration, it is possible to travel between any two tourist attractions.

# Output

One line, consisting of an integer, which gives the minimum number of roads that we need to add.

# Sample Input

10 12

1 2

1 3

1 4

2 5

2 6

5 6

3 7

3 8

7 8

4 9

4 10

9 10

# Sample Output

2


题意：给出一个 n 个点 m 条边的无向图，现在要向图中加边，求最少加几条边，能使图变为边双连通图

思路：

如果图本身就是一个边双连通图，那么就不需要向图中加边，因此先用 Tarjan 算法求出图中所有的桥。

对于桥来说，它是连接两个不同的边双连通分量的，即 low[i] 值相同的点必定属于同一个边双连通分量，因此如果想要添加一条边，必须在分属不同边双连通分量的两个点之间添加，因此可以将求出桥后的图进行缩点，即把每个边双连通分量看作一个点，用桥去连接每个缩点，故而只需在新的图上添边使新图变成一个边双连通图即可。

对于一棵无向树，要使其加边变为双连通图，则至少在树上加 (leaf+1)/2 条边，leaf 为树上边连通度为 1 的点

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

        for(int i=1;i<=m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
            G[y].push_back(x);
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






