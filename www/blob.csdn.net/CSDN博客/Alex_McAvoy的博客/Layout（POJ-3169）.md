# Layout（POJ-3169） - Alex_McAvoy的博客 - CSDN博客





2018年12月21日 15:03:14[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54
个人分类：[POJ																[图论——差分约束系统](https://blog.csdn.net/u011815404/article/category/8543163)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Like everyone else, cows like to stand close to their friends when queuing for feed. FJ has N (2 <= N <= 1,000) cows numbered 1..N standing along a straight line waiting for feed. The cows are standing in the same order as they are numbered, and since they can be rather pushy, it is possible that two or more cows can line up at exactly the same location (that is, if we think of each cow as being located at some coordinate on a number line, then it is possible for two or more cows to share the same coordinate). 

Some cows like each other and want to be within a certain distance of each other in line. Some really dislike each other and want to be separated by at least a certain distance. A list of ML (1 <= ML <= 10,000) constraints describes which cows like each other and the maximum distance by which they may be separated; a subsequent list of MD constraints (1 <= MD <= 10,000) tells which cows dislike each other and the minimum distance by which they must be separated. 

Your job is to compute, if possible, the maximum possible distance between cow 1 and cow N that satisfies the distance constraints.

# **Input**

Line 1: Three space-separated integers: N, ML, and MD. 

Lines 2..ML+1: Each line contains three space-separated positive integers: A, B, and D, with 1 <= A < B <= N. Cows A and B must be at most D (1 <= D <= 1,000,000) apart. 

Lines ML+2..ML+MD+1: Each line contains three space-separated positive integers: A, B, and D, with 1 <= A < B <= N. Cows A and B must be at least D (1 <= D <= 1,000,000) apart.

# Output

Line 1: A single integer. If no line-up is possible, output -1. If cows 1 and N can be arbitrarily far apart, output -2. Otherwise output the greatest possible distance between cows 1 and N.

# Sample Input

**4 2 11 3 102 4 202 3 3**

# Sample Output

**27**


题意：有 n 头牛站成一条线等待吃饭，奶牛排在队伍的顺序与编号相同，有些奶牛很瘦，因此有可能两头或更多头牛站在同一位置上，即允许两头或更多的牛拥有相同的横坐标，ML 头牛互有好感，他们希望彼此之间的距离不超过给定的数 L，MD 头牛互相反感，他们希望彼此的距离不小于一个给定的数 D，现在要求根据给定的条件进行判断，若不存在满足要求的方案输出 -1，若 1 号牛与 n 号牛的距离可以任意大输出 -2，否则计算在满足所有条件的情况下 1 号牛与 n 号牛可能的最大距离

思路：设 dis[i] 为第 i 头牛距离最左端 1 号牛的距离，因为牛按照编号从左到右排列，因此必然有 dis[i]-dis[i-1]>=0，此外，根据 ML 条好感条件 i、j、w 可知：dis[j]-dis[i]<=w，根据 MD 条反感条件 i、j、w 可知：dis[j]-dis[i]>=w

综上所述，建立一个以 1 号牛为源点，共有 n 个点的有向图，对于好感条件 dis[j]-dis[i]<=w 可推得 dis[j]<=dis[i]+w 建立一条从 i 到 j 权值为 w 的边，对于反感条件 dis[j]-dis[i]>=w 可推得 dis[i]<=dis[j]-w 建立一条从 j 到 i 权值为 -w 的边，对于题设 dis[i]-dis[i-1]>=0 可推得 dis[i-1]<=dis[i] 建立一条从 i 到 i-1 边权为 0 的边。

由于图不一定强连通，即使从 1 号牛能到达的点不存在负环，不代表其他的强连通分量里面没有负环，即算出 1 号到 n 号的距离，可能该差分约束系统仍然无解，因此仍然需要判断负环来看是否有解，再来求 dis[n]

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
#define N 1000001
#define LL long long
using namespace std;

struct Edge{
    int from;
    int to;
    int w;
    Edge(){}
    Edge(int from,int to,int w):from(from),to(to),w(w){}
}edge[N];
int head[N],next[N],num;
int dis[N];
bool vis[N];
int outque[N];
void init(){
    num=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int from,int to,int w){
    edge[num]=Edge(from,to,w);
    next[num]=head[from];
    head[from]=num++;
}
int SPFA(int s,int n){
    memset(vis,false,sizeof(vis));
    memset(outque,0,sizeof(outque));
    for(int i=0;i<=n;i++)
        dis[i]=INF;
    dis[s]=0;

    queue<int> Q;
    Q.push(s);
    while(!Q.empty()){
        int x=Q.front();
        Q.pop();
        vis[x]=false;

        outque[x]++;
        if(outque[x]>n-1)
            return -1;

        for(int i=head[x];i!=-1;i=next[i]){
            Edge &e=edge[i];
            if(dis[e.to]>dis[x]+e.w){
                dis[e.to]=dis[x]+e.w;
                if(!vis[e.to]){
                    vis[e.to]=true;
                    Q.push(e.to);
                }
            }
        }
    }

    if(dis[n]==INF)
        return -2;
    else
        return dis[n];
}
int main(){
    int n,ml,md;
    while(scanf("%d%d%d",&n,&ml,&md)!=EOF&&(n+ml+md)){
        init();
        while(ml--){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            addEdge(x,y,w);
        }
        while(md--){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            addEdge(y,x,-w);
        }
        for(int i=2;i<=n;i++)
            addEdge(i,i-1,0);

        printf("%d\n",SPFA(1,n));
    }
    return 0;
}
```






