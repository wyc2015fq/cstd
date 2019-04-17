# Candies（POJ-3159） - Alex_McAvoy的博客 - CSDN博客





2018年12月20日 17:21:06[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：46








> 
# Problem Description

During the kindergarten days, flymouse was the monitor of his class. Occasionally the head-teacher brought the kids of flymouse’s class a large bag of candies and had flymouse distribute them. All the kids loved candies very much and often compared the numbers of candies they got with others. A kid A could had the idea that though it might be the case that another kid B was better than him in some aspect and therefore had a reason for deserving more candies than he did, he should never get a certain number of candies fewer than B did no matter how many candies he actually got, otherwise he would feel dissatisfied and go to the head-teacher to complain about flymouse’s biased distribution.

snoopy shared class with flymouse at that time. flymouse always compared the number of his candies with that of snoopy’s. He wanted to make the difference between the numbers as large as possible while keeping every kid satisfied. Now he had just got another bag of candies from the head-teacher, what was the largest difference he could make out of it?

# **Input**

 The input contains a single test cases. The test cases starts with a line with two integers N and M not exceeding 30 000 and 150 000 respectively. N is the number of kids in the class and the kids were numbered 1 through N. snoopy and flymouse were always numbered 1 and N. Then follow M lines each holding three integers A, B and c in order, meaning that kid A believed that kid B should never get over c candies more than he did.

# Output

Output one line with only the largest difference desired. The difference is guaranteed to be finite.

# Sample Input

**2 21 2 52 1 4**

# Sample Output

**5**


题意：有 n 个人分糖，有 m 个条件需要满足，每个条件 a b c 表示为：b 的糖数-a 的糖数 <= c，问满足 m 个条件的情况下，要使 n 号的糖数 - 1 号糖数差最大为多少

思路：差分约束题，对 m 个条件来说，若 b-a<=c，那么从 a 到 b 有一条长为 c 的边，现在要求 dis[n] 与 dis[1] 的差最大，因此初始化应令 dis[1]=0，且 dis[i]=INF

由题意可知，所有的 c 都是正数，因此不会存在负环，故直接求 1 号到其他所有点单源最短路即可得到答案

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
void add(int from,int to,int w){
    edge[num]=Edge(from,to,w);
    next[num]=head[from];
    head[from]=num++;
}
struct HeapNode{
    int dis;
    int x;
    HeapNode(int dis,int x):dis(dis),x(x){}
    bool operator < (const HeapNode &rhs) const{
        return dis>rhs.dis;
    }
};
int dijkstra(int n){
    for(int i=0;i<n;i++)
        dis[i]=INF;
    dis[0]=0;

    priority_queue<HeapNode> Q;
    Q.push(HeapNode(dis[0],0));
    while(!Q.empty()){
        HeapNode x=Q.top();
        Q.pop();

        int u=x.x;
        if(vis[u])
            continue;
        vis[u]=true;
        for(int i=head[u];i!=-1;i=next[i]){
            Edge &e=edge[i];
            if(dis[e.to]>dis[u]+e.w){
                dis[e.to]=dis[u]+e.w;
                Q.push(HeapNode(dis[e.to],e.to));
            }
        }
    }
    return dis[n-1];
}
int main(){
    int n,m;
    scanf("%d%d",&n,&m);
    memset(head,-1,sizeof(head));
    while(m--){
        int x,y,w;
        scanf("%d%d%d",&x,&y,&w);
        x--,y--;
        add(x,y,w);
    }
    printf("%d\n",dijkstra(n));
    return 0;
}
```





