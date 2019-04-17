# Instrction Arrangement（HDU-4109） - Alex_McAvoy的博客 - CSDN博客





2018年12月23日 21:19:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：88
个人分类：[HDU																[图论——差分约束系统](https://blog.csdn.net/u011815404/article/category/8543163)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Ali has taken the Computer Organization and Architecture course this term. He learned that there may be dependence between instructions, like WAR (write after read), WAW, RAW.

If the distance between two instructions is less than the Safe Distance, it will result in hazard, which may cause wrong result. So we need to design special circuit to eliminate hazard. However the most simple way to solve this problem is to add bubbles (useless operation), which means wasting time to ensure that the distance between two instructions is not smaller than the Safe Distance.

The definition of the distance between two instructions is the difference between their beginning times.

Now we have many instructions, and we know the dependent relations and Safe Distances between instructions. We also have a very strong CPU with infinite number of cores, so you can run as many instructions as you want simultaneity, and the CPU is so fast that it just cost 1ns to finish any instruction.

Your job is to rearrange the instructions so that the CPU can finish all the instructions using minimum time.

# **Input**

The input consists several testcases.

The first line has two integers N, M (N <= 1000, M <= 10000), means that there are N instructions and M dependent relations.

The following M lines, each contains three integers X, Y , Z, means the Safe Distance between X and Y is Z, and Y should run after X. The instructions are numbered from 0 to N - 1.

# Output

Print one integer, the minimum time the CPU needs to run.

# Sample Input

**5 21 2 13 4 1**

# Sample Output

**2**


题意：给出 n 条指令的系统，有 m 个 x y z 形式的依赖关系，表示 y 指令必须在 x 指令后 z 秒执行，每条指令执行需要 1 秒，问该系统的指令执行完至少要多久

思路：用 dis[i] 表示第 i 条指令执行的时刻，对于每条依赖关系 x y z，有 dis[y]-dis[x]>=z ，即：dis[x]<=dis[y]-z，从而可以添加 y 到 x 的边权为 -z 的边，这样 m 条约束条件就构成了从 1 到 n 号点的差分约束系统

现在要求最小运行时间，设：第一条指令是从第 0 秒开始执行，最后一条指令是在第 t 秒开始执行，则所有指令执行完是第 t+1 秒，添加一个超级源点，使该点到其他点距离为 0，然后求出一组可行解，最后遍历该可行解，得到指令运行的最大值 maxx 与最小值 minn，结果 maxx-minn+1 即为所求

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
void init(){
    num=0;
    memset(head,-1,sizeof(head));
}
void addEdge(int from,int to,int w){
    edge[num]=Edge(from,to,w);
    next[num]=head[from];
    head[from]=num++;
}
void SPFA(int s,int n){
    memset(vis,false,sizeof(vis));
    for(int i=0;i<=n;i++)
        dis[i]=INF;
    dis[s]=0;

    queue<int> Q;
    Q.push(s);
    while(!Q.empty()){
        int x=Q.front();
        Q.pop();
        vis[x]=false;

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
}
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        init();
        while(m--){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            x++,y++;
            addEdge(y,x,-w);
        }
        for(int i=1;i<=n;i++)
            addEdge(0,i,0);

        SPFA(0,n);
        int maxx=-INF,minn=INF;
        for(int i=1;i<=n;i++){
            maxx=max(maxx,dis[i]);
            minn=min(minn,dis[i]);
        }

        printf("%d\n",maxx-minn+1);
    }
    return 0;
}
```






