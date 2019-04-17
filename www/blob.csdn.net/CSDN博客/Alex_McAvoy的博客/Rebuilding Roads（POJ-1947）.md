# Rebuilding Roads（POJ-1947） - Alex_McAvoy的博客 - CSDN博客





2018年10月09日 20:32:33[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：35
个人分类：[POJ																[动态规划——树形 DP](https://blog.csdn.net/u011815404/article/category/8104361)](https://blog.csdn.net/u011815404/article/category/7504500)








# Problem Description

The cows have reconstructed Farmer John's farm, with its N barns (1 <= N <= 150, number 1..N) after the terrible earthquake last May. The cows didn't have time to rebuild any extra roads, so now there is exactly one way to get from any given barn to any other barn. Thus, the farm transportation system can be represented as a tree. 

Farmer John wants to know how much damage another earthquake could do. He wants to know the minimum number of roads whose destruction would isolate a subtree of exactly P (1 <= P <= N) barns from the rest of the barns.

# **Input**

* Line 1: Two integers, N and P 

* Lines 2..N: N-1 lines, each with two integers I and J. Node I is node J's parent in the tree of roads. 

# Output

A single line containing the integer that is the minimum number of roads that need to be destroyed for a subtree of P nodes to be isolated.

# Sample Input

11 6

1 2

1 3

1 4

1 5

2 6

2 7

2 8

4 9

4 10

4 11

# Sample Output

2

————————————————————————————————————————————————————

题意：给出一棵 n 个节点的有根树，现在要删掉一些边使其变成有 m 个节点的树，求最少去掉的边数

思路：树形DP的分组背包问题

对于节点 x 的某个子节点，要么将其及其所有的子节点删掉，这样仅需删除节点x及其子节点的所有边，要么在子节点的子树中选择一些边删点，这样需要构造出有 j 个节点的子树，然后再将重复上述过程即可

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
#define N 301
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
struct Edge{
    int to;
    int next;
}edge[N*2];
int n,m;
int head[N],f[N];
int dp[N][N];
int cnt;
void addEdge(int x,int y){
    edge[cnt].to=y;
    edge[cnt].next=head[x];
    head[x]=cnt;
    cnt++;
}
int treeDP(int x,int father){
    int num=0;
    for(int i=0;i<=m;i++)
        dp[x][i]=INF;
    dp[x][1]=0;
    for(int i=head[x];i!=-1;i=edge[i].next){
        int y=edge[i].to;
        if(father==y)//到达底层
            continue;

        num=treeDP(y,x)+1;
        for(int j=m;j>0;j--)
            for(int k=0;k<j;k++)
                if(!k)//加子树
                    dp[x][j]=dp[x][j]+1;
                else//不加子树
                    dp[x][j]=min(dp[x][j],dp[x][j-k]+dp[y][k]);
    }
}
int main()
{
    while(scanf("%d%d",&n,&m)!=EOF){
        cnt=0;
        memset(f,-1,sizeof(f));
        memset(dp,0,sizeof(dp));
        memset(head,-1,sizeof(head));

        for(int i=1;i<n;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            addEdge(x,y);
            f[y]=x;
        }

        int root=1;
        while(f[root]!=-1)//寻找根节点
            root=f[root];

        treeDP(root,-1);
        int res=dp[root][m];
        for(int i=1;i<=n;i++)
            res=min(res,dp[i][m]+1);
        printf("%d\n",res);
    }

	return 0;
}
```






