# Cow Picnic（POJ-3256） - Alex_McAvoy的博客 - CSDN博客





2018年07月23日 15:45:39[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：77








# Problem Description

The cows are having a picnic! Each of Farmer John's K (1 ≤ K ≤ 100) cows is grazing in one of N (1 ≤ N ≤ 1,000) pastures, conveniently numbered 1...N. The pastures are connected by M (1 ≤ M ≤ 10,000) one-way paths (no path connects a pasture to itself).

The cows want to gather in the same pasture for their picnic, but (because of the one-way paths) some cows may only be able to get to some pastures. Help the cows out by figuring out how many pastures are reachable by all cows, and hence are possible picnic locations.

# **Input**

Line 1: Three space-separated integers, respectively: K, N, and M 

Lines 2..K+1: Line i+1 contains a single integer (1..N) which is the number of the pasture in which cow i is grazing. 

Lines K+2..M+K+1: Each line contains two space-separated integers, respectively A and B (both 1..N and A != B), representing a one-way path from pasture A to pasture B.

# Output

Line 1: The single integer that is the number of pastures that are reachable by all cows via the one-way paths.

# Sample Input

**2 4 4231 21 42 33 4**

# Sample Output

**2**

————————————————————————————————————————————————————

题意：k 头牛，n 个牧场，m 条单向的路，求所有牛能到达的牧场数

思路：对每头牛进行 dfs 搜索，最后统计每个农场到达的牛数。

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
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 123
#define E 1e-6
using namespace std;
int k,n,m;
int cow[N];
int sum[N];
int vis[N];
vector<int> g[N];
void dfs(int i)
{
    vis[i]=1;//标记为已到达
    int len=g[i].size();//从当前牧场可到达其他牧场的个数
    for(int j=0;j<len;j++)//枚举所有可到达的牧场
    {
        int next=g[i][j];
        if(!vis[next])//如果下一个牧场未到达，继续向下搜索
            dfs(next);
    }
}
int main()
{
    scanf("%d%d%d",&k,&n,&m);
    for(int i=1;i<=k;i++)
        scanf("%d",&cow[i]);
    while(m--)//vector邻接表建图
    {
        int x,y;
        scanf("%d%d",&x,&y);
        g[x].push_back(y);
    }

    for(int i=1;i<=k;i++)
    {
        dfs(cow[i]);//dfs每一头牛
        for(int j=1;j<=n;j++)//统计每一个牧场的到达数
        {
            sum[j]+=vis[j];
            vis[j]=0;
        }
    }

    int cnt=0;
    for(int i=1;i<=n;i++)//枚举所有牧场
        if(sum[i]==k)//如果该牧场所有牛均能到达
            cnt++;

    printf("%d\n",cnt);

    return 0;
}
```





