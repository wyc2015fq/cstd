# Ikki's Story IV - Panda's Trick（POJ-3207） - Alex_McAvoy的博客 - CSDN博客





2019年03月02日 14:44:41[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：88
个人分类：[POJ																[图论——2-SAT](https://blog.csdn.net/u011815404/article/category/8348292)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

liympanda, one of Ikki’s friend, likes playing games with Ikki. Today after minesweeping with Ikki and winning so many times, he is tired of such easy games and wants to play another game with Ikki.

liympanda has a magic circle and he puts it on a plane, there are n points on its boundary in circular border: 0, 1, 2, …, n − 1. Evil panda claims that he is connecting m pairs of points. To connect two points, liympanda either places the link entirely inside the circle or entirely outside the circle. Now liympanda tells Ikki no two links touch inside/outside the circle, except on the boundary. He wants Ikki to figure out whether this is possible…

Despaired at the minesweeping game just played, Ikki is totally at a loss, so he decides to write a program to help him.

# **Input**

The input contains exactly one test case.

In the test case there will be a line consisting of of two integers: n and m (n ≤ 1,000, m ≤ 500). The following m lines each contain two integers ai and bi, which denote the endpoints of the ith wire. Every point will have at most one link.

# Output

Output a line, either “panda is telling the truth...” or “the evil panda is lying again”.

# Sample Input

**4 20 13 2**

# Sample Output

**panda is telling the truth...**


题意：给出一个圆，圆上有 n 个点，按顺序在圆上排列，现要将 n 个点连 m 条线，线可以是直线，也可以是曲线，现给出要 m 条线的两个端点，一条线可以从圆内连，也可以从圆外连，但任意一个点最多只能作为一条线的端点，要求所连的任意两条线不相交，是否可能

思路：

以每条线段作为点，对于两点之间的一条位于圆内的曲线 A，那么肯定有一条对应于圆外的曲线 A'，可以看出，这两个点满足 2-SAT 中** A 与 非 A **的关系

对于每对两条在圆内或圆外的曲线来说，通过他们的起点、终点坐标即可判断是否矛盾

对于一条边 i，在圆内记为 i，在圆外记为 i'，设边 i 连接点 A、B，边 j 连接点 C、D，那么，i 与 j 在圆内是否相交，就是线段 AB 与线段 CD 是否相交，可用坐标进行判断

因此，若 i 与 j 在圆内不能共存，则在圆外也一定不能共存，设 0 为在圆内， 1 为在圆外，则：
- 边 i 在圆内，那么边 j 一定在圆外：（i,0,j,1）
- 边 i 在圆外，那么边 j 一定在圆内：（i,1,j,0）
- 边 j 在圆内，那么边 i 一定在圆外：（j,0,i,1）
- 边 j 在圆外，那么边 i 一定在圆内：（j,1,i,0）

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
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=1000000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

bool vis[N*2];
int Stack[N*2],top;
vector<int> G[N*2];
void init(int n){
    memset(vis,false,sizeof(vis));
    for(int i=0;i<2*n;i++)
        G[i].clear();
}
void addOrClause(int x,int xVal,int y,int yVal){
    x=x*2+xVal;
    y=y*2+yVal;
    G[x^1].push_back(y);
    G[y^1].push_back(x);
}
void addAndClause(int x,int xval,int y,int yval) {
    x=x*2+xval;
    y=y*2+yval;
    G[x].push_back(y);
}
bool dfs(int x){
    if(vis[x^1])
        return false;

    if(vis[x])
        return true;
    vis[x]=true;

    Stack[top++]=x;

    for(int i=0;i<G[x].size();i++)
        if(!dfs(G[x][i]))
            return false;

    return true;
}
bool twoSAT(int n){
    for(int i=0;i<2*n;i+=2){
        if(!vis[i] && !vis[i+1]){
            top=0;

            if(!dfs(i)){
                while(top>0)
                    vis[Stack[--top]]=false;

                if(!dfs(i+1))
                    return false;
            }
        }
    }
    return true;
}
struct Edge{
    int x;
    int y;
    Edge(){}
    Edge(int x,int y):x(x),y(y){}
}e[N];
int cnt;
bool judge(int i,int j){//判断两边是否相交
    if( e[i].x<=e[j].x && e[i].y>=e[j].x && e[i].y<=e[j].y)
        return true;
    if(e[i].x<=e[j].y && e[i].y>=e[j].y && e[i].x>=e[j].x)
        return true;
    return false;
}
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        cnt=0;
        init(m);

        for(int i=0;i<m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            if(x>y)
                swap(x,y);
            e[cnt++]=Edge(x,y);
        }

        for(int i=0;i<m;i++){
            for(int j=i+1;j<m;j++){
                if(judge(i,j)){//判断两边是否相交，若相交，则需要添加限制条件
                    addAndClause(i,0,j,1);//边 i 在圆内，那么边 j 一定在圆外
                    addAndClause(i,1,j,0);//边 i 在圆外，那么边 j 一定在圆内
                    addAndClause(j,0,i,1);//边 j 在圆内，那么边 i 一定在圆外
                    addAndClause(j,1,i,0);//边 j 在圆外，那么边 i 一定在圆内
                }
            }
        }

        if(twoSAT(m))
            printf("panda is telling the truth...\n");
        else
            printf("the evil panda is lying again\n");

    }

    return 0;
}
```






