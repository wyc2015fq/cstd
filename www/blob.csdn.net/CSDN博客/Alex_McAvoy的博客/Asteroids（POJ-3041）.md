# Asteroids（POJ-3041） - Alex_McAvoy的博客 - CSDN博客





2018年12月02日 18:12:24[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：73
个人分类：[POJ																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Bessie wants to navigate her spaceship through a dangerous asteroid field in the shape of an N x N grid (1 <= N <= 500). The grid contains K asteroids (1 <= K <= 10,000), which are conveniently located at the lattice points of the grid. 

Fortunately, Bessie has a powerful weapon that can vaporize all the asteroids in any given row or column of the grid with a single shot.This weapon is quite expensive, so she wishes to use it sparingly.Given the location of all the asteroids in the field, find the minimum number of shots Bessie needs to fire to eliminate all of the asteroids.

# **Input**

* Line 1: Two integers N and K, separated by a single space. 

* Lines 2..K+1: Each line contains two space-separated integers R and C (1 <= R, C <= N) denoting the row and column coordinates of an asteroid, respectively.

# Output

* Line 1: The integer representing the minimum number of times Bessie must shoot.

# Sample Input

**3 41 11 32 23 2**

# Sample Output

**2**


题意：对于一个 n*n 个图，在某几个点有些障碍物，现在要消除这些障碍物，每次可以消除一行或一列，问最少消除几次

思路：将行看做左点集，列看做右点集，若（i,j）存在障碍物，则在左 i 右 j 间连一条边，现在要在所建的二分图中，选择最少的点使得所有边都至少有一个点被选中，实质就是一个求最小覆盖数问题

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
int hungarian(int n){
    int ans=0;
    for(int i=0;i<n;i++){
        memset(vis,false,sizeof(vis));
        if(dfs(i))
            ans++;
    }
    return ans;
}
int main(){
    int n,k;
    while(scanf("%d%d",&n,&k)!=EOF&&(n+k)){
        memset(link,-1,sizeof(link));
        for(int i=0;i<n;i++)
            G[i].clear();

        while(k--){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
        }

        printf("%d\n",hungarian(n));
    }
    return 0;
}
```






