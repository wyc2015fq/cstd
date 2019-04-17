# Repairing Company（POJ-3216） - Alex_McAvoy的博客 - CSDN博客





2018年12月03日 22:32:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：45
个人分类：[POJ																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Lily runs a repairing company that services the Q blocks in the city. One day the company receives M repair tasks, the ith of which occurs in block pi, has a deadline ti on any repairman’s arrival, which is also its starting time, and takes a single repairman di time to finish. Repairmen work alone on all tasks and must finish one task before moving on to another. With a map of the city in hand, Lily want to know the minimum number of repairmen that have to be assign to this day’s tasks.

# **Input**

The input contains multiple test cases. Each test case begins with a line containing Q and M (0 < Q ≤ 20, 0 < M ≤ 200). Then follow Q lines each with Q integers, which represent a Q × Q matrix Δ = {δij}, where δij means a bidirectional road connects the ith and the jth blocks and requires δij time to go from one end to another. If δij = −1, such a road does not exist. The matrix is symmetric and all its diagonal elements are zeroes. Right below the matrix are M lines describing the repairing tasks. The ith of these lines contains pi, ti and di. Two zeroes on a separate line come after the last test case.

# Output

For each test case output one line containing the minimum number of repairmen that have to be assigned.

# Sample Input

**1 201 1 101 5 100 0**

# Sample Output

**2**


题意： 给出 Q 条街道 M 个任务，构造一个 Q*Q 的矩阵，然后给出 i、j 表示第 i 个点到第 j 个点的距离，其中 -1 表示不可到达，再给出 M 行，每行有 p、t、d 分别表示任务在 p 点，开始时间是 t，完成花费时间是 d，问最少派多少个人可以完成这 M 个任务

思路：将每个任务看做一个点，如果一个人做完任务 i 后且能及时到达 j 去做任务，就连一条从 i 到 j 的有向边，从而得到一个 DAG 图，由于要派遣最少的工人数目，因此找出该图的最小路径覆盖即可

要注意的是，判断任务 i 到任务 j 存在有向边，需要判断 i 的结束时间+i 到 j 的最短路径时间 <= j 的开始时间，最短路径时间则需要用 Floyd 算法求最短路

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
int hungarian(int x){

    int ans=0;
    for(int i=1;i<=x;i++){
        memset(vis,false,sizeof(vis));
        if(dfs(i))
            ans++;
    }
    return ans;
}
int dis[N][N];
struct Node{
    int p,t,d;
    bool judge(Node &rhs){
        return t+d+dis[p][rhs.p]<=rhs.t;
   }
}a[N];
int main(){
    int q,m;
    while(scanf("%d%d",&q,&m)!=EOF&&(q+m)){
        memset(link,-1,sizeof(link));
        for(int i=0;i<=m;i++)
            G[i].clear();

        for(int i=1;i<=q;i++){
            for(int j=1;j<=q;j++){
                scanf("%d",&dis[i][j]);
                if(dis[i][j]==-1)
                    dis[i][j]=INF;
            }
        }
        for(int k=1;k<=q;k++)
            for(int i=1;i<=q;i++)
                for(int j=1;j<=q;j++)
                    if(dis[i][k]<INF&&dis[k][j]<INF)
                        if(dis[i][j]>dis[i][k]+dis[k][j])
                            dis[i][j]=dis[i][k]+dis[k][j];

        for(int i=1;i<=m;i++)
            scanf("%d%d%d",&a[i].p,&a[i].t,&a[i].d);

        for(int i=1;i<=m;i++){
            for(int j=1;j<=m;j++){
                if(i!=j){
                    if(a[i].judge(a[j])){
                        G[i].push_back(j);
                    }
                }
            }
        }

        printf("%d\n",m-hungarian(m));
    }
    return 0;
}
```






