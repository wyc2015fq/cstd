# Maximum Clique（HDU-1530） - Alex_McAvoy的博客 - CSDN博客





2019年01月23日 16:01:03[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35
个人分类：[HDU																[图论——最大团](https://blog.csdn.net/u011815404/article/category/8635501)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Given a graph G(V, E), a clique is a sub-graph g(v, e), so that for all vertex pairs v1, v2 in v, there exists an edge (v1, v2) in e. Maximum clique is the clique that has maximum number of vertex. 

# Input

Input contains multiple tests. For each test: 

The first line has one integer n, the number of vertex. (1 < n <= 50) 

The following n lines has n 0 or 1 each, indicating whether an edge exists between i (line number) and j (column number). 

A test with n = 0 signals the end of input. This test should not be processed.

# Output

One number for each test, the number of vertex in maximum clique. 

# **Sample Input**

**50 1 1 0 11 0 1 1 11 1 0 1 10 1 1 0 11 1 1 1 00**

# Sample Output

**4**


题意：给出具 n 个点的无向图的邻接矩阵，求这个图的最大团的点数

思路：Bron-Kerbosch 算法模板题

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
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 1001
#define LL long long
using namespace std;
int n;
int G[N][N];
int cnt[N];//cnt[i]为>=i的最大团点数
int group[N];//最大团的点
int vis[N];//记录点的位置
int res;//最大团的数目
bool dfs(int pos,int num){//num为已取的点数
    for(int i=pos+1;i<=n;i++){
        if(cnt[i]+num<=res)//剪枝，若取i但cnt[i]+已经取了的点数仍<ans
            return false;

        if(G[pos][i]){//与当前团中元素比较，取Non-N(i)
            int j;
            for(j=0;j<num;j++)
                if(!G[i][vis[j]])
                    break;
            if(j==num){//若为空，则皆与i相邻，则此时将i加入到最大团中
                vis[num]=i;
                if(dfs(i,num+1))
                    return true;
            }
        }
    }

    if(num>res){//每添加一个点最多使最大团数+1,后面的搜索就没有意义了
        res=num;//最大团中点的数目
        for(int i=1;i<=num;i++)//最大团的元素
            group[i]=vis[i-1];
        return true;
    }
    return false;
}
void maxClique(){
    res=-1;
    for(int i=n;i>=1;i--){//枚举所有点
        vis[0]=i;
        dfs(i,1);
        cnt[i]=res;
    }
}
int main(){
    while(scanf("%d",&n)!=EOF&&(n)){
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                scanf("%d",&G[i][j]);
        maxClique();

       printf("%d\n",res);//最大团的个数
    }
    return 0;
}
```






