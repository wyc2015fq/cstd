# The Cow Prom（POJ-3180） - Alex_McAvoy的博客 - CSDN博客





2018年10月23日 20:19:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：26
个人分类：[POJ																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The N (2 <= N <= 10,000) cows are so excited: it's prom night! They are dressed in their finest gowns, complete with corsages and new shoes. They know that tonight they will each try to perform the Round Dance. 

Only cows can perform the Round Dance which requires a set of ropes and a circular stock tank. To begin, the cows line up around a circular stock tank and number themselves in clockwise order consecutively from 1..N. Each cow faces the tank so she can see the other dancers. 

They then acquire a total of M (2 <= M <= 50,000) ropes all of which are distributed to the cows who hold them in their hooves. Each cow hopes to be given one or more ropes to hold in both her left and right hooves; some cows might be disappointed. 

For the Round Dance to succeed for any given cow (say, Bessie), the ropes that she holds must be configured just right. To know if Bessie's dance is successful, one must examine the set of cows holding the other ends of her ropes (if she has any), along with the cows holding the other ends of any ropes they hold, etc. When Bessie dances clockwise around the tank, she must instantly pull all the other cows in her group around clockwise, too. Likewise, 

if she dances the other way, she must instantly pull the entire group counterclockwise (anti-clockwise in British English). 

Of course, if the ropes are not properly distributed then a set of cows might not form a proper dance group and thus can not succeed at the Round Dance. One way this happens is when only one rope connects two cows. One cow could pull the other in one direction, but could not pull the other direction (since pushing ropes is well-known to be fruitless). Note that the cows must Dance in lock-step: a dangling cow (perhaps with just one rope) that is eventually pulled along disqualifies a group from properly performing the Round Dance since she is not immediately pulled into lockstep with the rest. 

Given the ropes and their distribution to cows, how many groups of cows can properly perform the Round Dance? Note that a set of ropes and cows might wrap many times around the stock tank.

# Input

Line 1: Two space-separated integers: N and M 

Lines 2..M+1: Each line contains two space-separated integers A and B that describe a rope from cow A to cow B in the clockwise direction.

# Output

Line 1: A single line with a single integer that is the number of groups successfully dancing the Round Dance.

# Sample Input

**5 42 43 51 24 1**

# **Sample Output**

**1**


题意：有编号从 1-N 的牛，给出 M 组有向的连接方案，当能组成一个环时（连接数大于等于2的闭环）被视为连接成功，求连接成功的组数

思路：实质为求大于等于2的强连通分量个数，用 Kosaraju 算法，然后缩点染色，最后判断每种颜色的个数，如果大于1，最后输出个数+1

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
#define N 200001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
int n,m;
vector<int> g[N];
vector<int> gt[N];
bool vis[N];
int color[N];
int num[N];
int cnt[N];
int sig;
void dfs1(int x){
    vis[x]=true;
    for(int i=0;i<g[x].size();i++){
        int y=g[x][i];
        if(vis[y]==false)
            dfs1(y);
    }
    num[sig++]=x;
}
void dfs2(int x){
    vis[x]=true;
    color[x]=sig;
    for(int i=0;i<gt[x].size();i++){
        int y=gt[x][i];
        if(vis[y]==false)
            dfs2(y);
    }
}
void Kosaraju(){
    sig=1;
    memset(vis,0,sizeof(vis));
    for(int i=1;i<=n;i++)//对原图搜索
        if(vis[i]==false)
            dfs1(i);

    sig=0;
    memset(vis,0,sizeof(vis));
    for(int i=n;i>=1;i--)//对反图搜索
        if(vis[num[i]]==0){
            sig++;
            dfs2(num[i]);
        }

    memset(cnt,0,sizeof(cnt));
    for(int i=1;i<=n;i++)//缩点，统计颜色个数
        cnt[color[i]]++;

    int res=0;
    for(int i=1;i<=sig;i++)
        if(cnt[i]>1)
            res++;
    printf("%d\n",res);
}
int main(){
    while(scanf("%d%d",&n,&m)!=EOF){
        for(int i=1;i<=n;i++){
            g[i].clear();
            gt[i].clear();
        }
        for(int i=0;i<m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            g[x].push_back(y);
            gt[y].push_back(x);
        }
        Kosaraju();
    }
    return 0;
}
```






