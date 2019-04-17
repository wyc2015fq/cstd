# The Perfect Stall（POJ-1274） - Alex_McAvoy的博客 - CSDN博客





2018年11月26日 23:36:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：28
个人分类：[POJ																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Farmer John completed his new barn just last week, complete with all the latest milking technology. Unfortunately, due to engineering problems, all the stalls in the new barn are different. For the first week, Farmer John randomly assigned cows to stalls, but it quickly became clear that any given cow was only willing to produce milk in certain stalls. For the last week, Farmer John has been collecting data on which cows are willing to produce milk in which stalls. A stall may be only assigned to one cow, and, of course, a cow may be only assigned to one stall. 

Given the preferences of the cows, compute the maximum number of milk-producing assignments of cows to stalls that is possible. 

# **Input**

The input includes several cases. For each case, the first line contains two integers, N (0 <= N <= 200) and M (0 <= M <= 200). N is the number of cows that Farmer John has and M is the number of stalls in the new barn. Each of the following N lines corresponds to a single cow. The first integer (Si) on the line is the number of stalls that the cow is willing to produce milk in (0 <= Si <= M). The subsequent Si integers on that line are the stalls in which that cow is willing to produce milk. The stall numbers will be integers in the range (1..M), and no stall will be listed twice for a given cow.

# Output

For each case, output a single line with a single integer, the maximum number of milk-producing stall assignments that can be made.

# Sample Input

**5 52 2 53 2 3 42 1 53 1 2 51 2 **

# Sample Output

**4**


题意：给出 n 头牛，m 个牛棚，每个牛只在自己喜欢的牛棚产奶，每个牛棚只能容纳一头牛，问最多有多少头牛可以同时产奶

思路：将牛和牛棚分别视为两个点集，由于牛只能在一个牛棚，每个牛棚也只能容纳一头牛，因此可视为二分图，根据题意，实质即为求图的最大匹配

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
            if(link[y]==-1 || dfs(link[y])){
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
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        memset(link,-1,sizeof(link));
        for(int i=0;i<N;i++)
            G[i].clear();

        for(int i=1;i<=n;i++){
            int num;
            scanf("%d",&num);
            while(num--){
                int j;
                scanf("%d",&j);
                G[i].push_back(j);
            }
        }
        printf("%d\n",hungarian(n));

    }
    return 0;
}
```






