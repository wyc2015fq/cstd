# Dolls（HDU-4160） - Alex_McAvoy的博客 - CSDN博客





2018年12月03日 21:51:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：59
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Do you remember the box of Matryoshka dolls last week? Adam just got another box of dolls from Matryona. This time, the dolls have different shapes and sizes: some are skinny, some are fat, and some look as though they were attened. Specifically, doll i can be represented by three numbers wi, li, and hi, denoting its width, length, and height. Doll i can fit inside another doll j if and only if wi < wj , li < lj , and hi < hj .

That is, the dolls cannot be rotated when fitting one inside another. Of course, each doll may contain at most one doll right inside it. Your goal is to fit dolls inside each other so that you minimize the number of outermost dolls.

# **Input**

The input consists of multiple test cases. Each test case begins with a line with a single integer N, 1 ≤ N ≤ 500, denoting the number of Matryoshka dolls. Then follow N lines, each with three space-separated integers wi, li, and hi (1 ≤ wi; li; hi ≤ 10,000) denoting the size of the ith doll. Input is followed by a single line with N = 0, which should not be processed.

# Output

For each test case, print out a single line with an integer denoting the minimum number of outermost dolls that can be obtained by optimally nesting the given dolls.

# Sample Input

**35 4 827 10 10100 32 52331 2 12 1 11 1 241 1 12 3 23 2 24 4 40**

# Sample Output

**132**


题意： 有 n 个木偶，每个木偶有三个指标 w、l、h，若第 i 个木偶的三项指标对应小于第 j 个木偶的三项指标，那么 i 木偶可以放入 j 木偶中，且一个木偶里面只能放一个别的木偶，问在这 n 个木偶最优嵌套方案下，最多有几个木偶不被任何木偶嵌套

思路：若 i 木偶可以放入 j 木偶中，则连一条 i 到 j 的有向边，从而得到一个 DAG 图，现在要在该 DAG 图中找最少的简单路径，这些路径中没有交集且正好完全覆盖了 DAG 的各个顶点，实质就是要求 DAG 的最小路径覆盖问题

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
struct Node{
    int w,l,h;
    bool judge(Node &rhs){
        return (w<rhs.w&&l<rhs.l&&h<rhs.h);
    }
}a[N];
int main(){
    int n;
    while(scanf("%d",&n)!=EOF&&(n)){
        memset(link,-1,sizeof(link));
        for(int i=0;i<n;i++)
            G[i].clear();

        for(int i=1;i<=n;i++)
            scanf("%d%d%d",&a[i].w,&a[i].l,&a[i].h);

        for(int i=1;i<=n;i++){
            for(int j=1;j<=n;j++){
                if(i!=j){
                    if(a[i].judge(a[j])){
                        G[i].push_back(j);
                    }
                }
            }
        }

        printf("%d\n",n-hungarian(n));
    }
    return 0;
}
```






