# Cyclic Tour（HDU-1853） - Alex_McAvoy的博客 - CSDN博客





2018年12月08日 19:49:39[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：45
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

There are N cities in our country, and M one-way roads connecting them. Now Little Tom wants to make several cyclic tours, which satisfy that, each cycle contain at least two cities, and each city belongs to one cycle exactly. Tom wants the total length of all the tours minimum, but he is too lazy to calculate. Can you help him?

# **Input**

There are several test cases in the input. You should process to the end of file (EOF).

The first line of each test case contains two integers N (N ≤ 100) and M, indicating the number of cities and the number of roads. The M lines followed, each of them contains three numbers A, B, and C, indicating that there is a road from city A to city B, whose length is C. (1 ≤ A,B ≤ N, A ≠ B, 1 ≤ C ≤ 1000).

# Output

Output one number for each test case, indicating the minimum length of all the tours. If there are no such tours, output -1.

# Sample Input

**6 91 2 52 3 53 1 103 4 124 1 84 6 115 4 75 6 96 5 46 51 2 12 3 13 4 14 5 15 6 1**

# Sample Output

**42-1**


题意：给一个 n 个点 m 条边的带权有向图，现要求这 n 个点正好被 1 或多个不相交的有向环覆盖，求这些有向环的最小边权值

思路：可以将 n 个点分为两部分，i 与 i‘，分别作为左右点集，因此若原图中存在边（i,j），则二分图中存在（i，j'），从而建图

若原图能由多个不相交的有向环覆盖，那么二分图一定存在完全匹配，比如：原图中有向环为 1-2-3-1，则二分图的完全匹配就是 1-2'，2-3'，3-1'

由于有向环覆盖对应一个二分图的完全匹配，该完全匹配的权值对应有向环覆盖的权值，因此原图权值最大的有向环匹配就是二分图最优匹配的值

题目要求边权值最小，因此将所有边取负数，再使用 KM 算法，得到的值再取负即可

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
int G[N][N];
int Lx[N],Ly[N];
bool visX[N],visY[N];
int linkX[N],linkY[N];
bool dfs(int x){
    visX[x]=true;
    for(int y=1;y<=n;y++){
        if(!visY[y]){
            int temp=Lx[x]+Ly[y]-G[x][y];
            if(temp==0){
                visY[y]=true;
                if(linkY[y]==-1 || dfs(linkY[y])){
                    linkX[x]=y;
                    linkY[y]=x;
                    return true;
                }
            }
        }
    }
    return false;
}
void update(){
    int minn=INF;
    for(int i=1;i<=n;i++)
        if(visX[i])
            for(int j=1;j<=n;j++)
                if(!visY[j])
                    minn=min(minn,Lx[i]+Ly[j]-G[i][j]);

    for(int i=1;i<=n;i++)
        if(visX[i])
            Lx[i]-=minn;

    for(int i=1;i<=n;i++)
        if(visY[i])
            Ly[i]+=minn;
}
int KM(){
    memset(linkX,-1,sizeof(linkX));
    memset(linkY,-1,sizeof(linkY));

    for(int i=1;i<=n;i++){
        Lx[i]=Ly[i]=0;
        for(int j=1;j<=n;j++)
            Lx[i]=max(Lx[i],G[i][j]);
    }

    for(int i=1;i<=n;i++){
        while(true){
            memset(visX,false,sizeof(visX));
            memset(visY,false,sizeof(visY));

            if(dfs(i))
                break;
            else
                update();
        }
    }

    int ans=0;
    for(int i=1;i<=n;i++){
        if(G[linkY[i]][i]==-INF){
            return 1;
        }
        ans+=G[linkY[i]][i];
    }

    return ans;
}
int main(){
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                G[i][j]=-INF;

        while(m--){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            G[x][y]=max(G[x][y],-w);//可能有重边
        }
        printf("%d\n",-KM());
    }
    return 0;
}
```






