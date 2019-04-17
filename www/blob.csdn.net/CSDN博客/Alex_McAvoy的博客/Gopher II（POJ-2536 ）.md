# Gopher II（POJ-2536 ） - Alex_McAvoy的博客 - CSDN博客





2018年11月27日 00:02:33[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：29
个人分类：[POJ																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

The gopher family, having averted the canine threat, must face a new predator. 

The are n gophers and m gopher holes, each at distinct (x, y) coordinates. A hawk arrives and if a gopher does not reach a hole in s seconds it is vulnerable to being eaten. A hole can save at most one gopher. All the gophers run at the same velocity v. The gopher family needs an escape strategy that minimizes the number of vulnerable gophers.

# **Input**

The input contains several cases. The first line of each case contains four positive integers less than 100: n, m, s, and v. The next n lines give the coordinates of the gophers; the following m lines give the coordinates of the gopher holes. All distances are in metres; all times are in seconds; all velocities are in metres per second.

# Output

Output consists of a single line for each case, giving the number of vulnerable gophers.

# Sample Input

**2 2 5 101.0 1.02.0 2.0100.0 100.020.0 20.0**

# Sample Output

**1**


题意：有 n 个地鼠和 m 个洞，每个洞只能容纳 1 只地鼠，当鹰飞来时，n 个地鼠若能在 s 秒内从当前位置回到洞中，就能不被鹰抓住，每只地鼠的速度为 v，求被抓住的地鼠个数的最小值

思路：将地鼠与洞看做二分图，如果一个地鼠能在 s 秒内到达洞，那么就在其之间建立一条边，根据题意，被抓住的个数=n-最大匹配数

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
struct Point{
    double x;
    double y;
}P1[N],P2[N];
double get_dis(int a,int b){
     return sqrt( (P1[a].x-P2[b].x)*(P1[a].x-P2[b].x)+(P1[a].y-P2[b].y)*(P1[a].y-P2[b].y) );
}
int main(){
    int n,m,s,v;
    while(scanf("%d%d%d%d",&n,&m,&s,&v)!=EOF&&n){
        memset(link,-1,sizeof(link));
        for(int i=0;i<N;i++)
            G[i].clear();

        for(int i=1;i<=n;i++)
            scanf("%lf%lf",&P1[i].x,&P1[i].y);
        for(int i=1;i<=m;i++)
            scanf("%lf%lf",&P2[i].x,&P2[i].y);

        for(int i=1;i<=n;i++){
            for(int j=1;j<=m;j++){
                if(get_dis(i,j)<=s*v){
                    G[i].push_back(j);
                }
            }
        }
        printf("%d\n",n-hungarian(m));

    }
    return 0;
}
```






