# Kindergarten（POJ-3692） - Alex_McAvoy的博客 - CSDN博客





2018年11月29日 22:09:28[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60
个人分类：[POJ																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

In a kindergarten, there are a lot of kids. All girls of the kids know each other and all boys also know each other. In addition to that, some girls and boys know each other. Now the teachers want to pick some kids to play a game, which need that all players know each other. You are to help to find maximum number of kids the teacher can pick.

# **Input**

The input consists of multiple test cases. Each test case starts with a line containing three integers

G, B (1 ≤ G, B ≤ 200) and M (0 ≤ M ≤ G × B), which is the number of girls, the number of boys and

the number of pairs of girl and boy who know each other, respectively.

Each of the following M lines contains two integers X and Y (1 ≤ X≤ G,1 ≤ Y ≤ B), which indicates that girl X and boy Y know each other.

The girls are numbered from 1 to G and the boys are numbered from 1 to B.

The last test case is followed by a line containing three zeros.

# Output

For each test case, print a line containing the test case number( beginning with 1) followed by a integer which is the maximum number of kids the teacher can pick.

# Sample Input

**2 3 31 11 22 32 3 51 11 22 12 22 30 0 0**

# Sample Output

**Case 1: 3Case 2: 4**


题意：班里有 g 个女孩 b 个男孩，所有女生都相互认识，男生也相互认识，给出 m 对关系表示哪个女孩与哪个男孩认识，现在要选一些学生来组成一个团体，使得团体中的所有人都认识，求团体的最大人数

思路：女生在左点集，男生在右点集，对于女生 i 与男生 j 不认识的话，就连接一条边，现在要在图中选最多的点，使得所选点集中不存在边，也就是求二分图的最大独立集，即二分图的点数-最大匹配数

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
int g,b,m;
bool vis[N];
int link[N];
bool G[N][N];
bool dfs(int x){
    for(int y=1;y<=b;y++){
        if(G[x][y]&&!vis[y]){
            vis[y]=true;
            if(link[y]==-1 || dfs(link[y]))	{
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
    int Case=1;
    while(scanf("%d%d%d",&g,&b,&m)!=EOF&&(g+b+m)){
        memset(link,-1,sizeof(link));
        memset(G,true,sizeof(G));

        while(m--){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x][y]=false;
        }

        printf("Case %d: %d\n",Case++,g+b-hungarian(g));
    }
    return 0;
}
```






