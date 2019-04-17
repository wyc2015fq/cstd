# Harry Potter and the Present II（HDU-3991） - Alex_McAvoy的博客 - CSDN博客





2018年12月04日 16:59:44[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：28
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Harry and his friends buy a lot of presents now, but they immediately find they had bought too many presents, so they decide to distribute them to the magicians who need them.

To simplify the problem, here we assume the magic world has N cities and M roads, and there are Q requests, at time Ti, someone at city Pi need a present. After planning the route for a while, Harry realizes he can’t do it by himself, he need to find some friends to help him. But the new question is, how many friends Harry should ask at least? We can assume every magician can choose the original city and time to begin his/her journey, and all of them have the same speed, one unit distance per unit time. And if he/she is exactly in city Pi at time Ti, he/she will give the present in a wink, yes, as you see, they are magicians.

# **Input**

The first line contains a single integer T, indicating the number of test cases.

Each test case begins with three integers N, M, Q. Then M lines following, each line contains three integers Ai, Bi, Ci, describing an undirected road. Then Q lines following, each line contains two integers Pi, Ti, describing a request.

Technical Specification

1. 1 <= T <= 50

2. 1 <= N <= 100

3. 1 <= M, Q <= 1 000

4. 0 <= Ai, Bi, Pi < N, Ai != Bi

5. 0 <= Ci, Ti <= 1 000 000 000

# Output

For each test case, output the case number first, then the least friend number.

# Sample Input

**22 1 20 1 10 11 22 1 20 1 20 11 2**

# Sample Output

**Case 1: 0Case 2: 1**


题意：给出一 n 个点 m 条边的图，再给出 q 个任务，每个任务以 c、t 的形式给出，c 表示城市编号，t 表示任务需要在 t 这个时间完成，每个人可以在任何时间点出现在任何城市，问最少要几个人才能完成所有城市的任务

思路：把每个任务看成一个点，如果完成任务 i 后还能在任务 j 开始之前到达 j ，那么就连一条左 i 右 j 的有向边， 如果一个人完成一个城市的任务后，还来得及去另一个地方做任务，那么就可以继续走下去，从而形成一条单向路径

为了让人最少且每个礼物都送到，就要找出最少的简单路径且所有路径正好覆盖了图的所有点，因此所需人数就是该 DAG 图的最小路径覆盖数

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
LL dis[N][N];
struct Node{
    int p,t;
    bool judge(Node &rhs){
        return rhs.t>=t+dis[p][rhs.p];
   }
}a[N];
int main(){
    int t;
    int Case=1;
    scanf("%d",&t);
    while(t--){
        int n,m,q;
        scanf("%d%d%d",&n,&m,&q);

        memset(dis,INF,sizeof(dis));
        for(int i=0;i<n;i++)
            for(int j=0;j<n;j++)
                if(i==j)
                    dis[i][j]=0;

        memset(link,-1,sizeof(link));
        for(int i=0;i<=m;i++)
            G[i].clear();

        while(m--){
            int x,y;
            LL d;
            scanf("%d%d%lld",&x,&y,&d);
            dis[x][y]=dis[y][x]=d;
        }

        for(int k=1;k<=n;k++)
            for(int i=1;i<=n;i++)
                for(int j=1;j<=n;j++)
                    if(dis[i][k]<INF&&dis[k][j]<INF)
                        if(dis[i][j]>dis[i][k]+dis[k][j])
                            dis[i][j]=dis[i][k]+dis[k][j];

        for(int i=1;i<=q;i++)
            scanf("%d%d",&a[i].p,&a[i].t);

        for(int i=1;i<=q;i++){
            for(int j=1;j<=q;j++){
                if(i!=j){
                    if(a[i].judge(a[j])){
                        G[i].push_back(j);
                    }
                }
            }
        }

        printf("Case %d: %d\n",Case++,q-hungarian(q)-1);
    }
    return 0;
}
```






