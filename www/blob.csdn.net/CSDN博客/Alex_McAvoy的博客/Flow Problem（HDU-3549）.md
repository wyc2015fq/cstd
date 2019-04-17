# Flow Problem（HDU-3549） - Alex_McAvoy的博客 - CSDN博客





2018年12月28日 12:23:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：32








> 
# Problem Description

Network flow is a well-known difficult problem for ACMers. Given a graph, your task is to find out the maximum flow for the weighted directed graph.

# **Input**

The first line of input contains an integer T, denoting the number of test cases.

For each test case, the first line contains two integers N and M, denoting the number of vertexes and edges in the graph. (2 <= N <= 15, 0 <= M <= 1000)

Next M lines, each line contains three integers X, Y and C, there is an edge from X to Y and the capacity of it is C. (1 <= X, Y <= N, 1 <= C <= 1000)

# Output

For each test cases, you should output the maximum flow from source 1 to sink N.

# Sample Input

**23 21 2 12 3 13 31 2 12 3 11 3 1**

# Sample Output

**Case 1: 1Case 2: 2**


题意：给出一 n 个点 m 条边的有向图，求从 1 号点到 n 号点的最大流

思路：网络流最大流问题裸题，套 EK 算法模版即可，值得注意的地方是重边的处理

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
#define N 101
#define LL long long
using namespace std;
struct EdmondsKarp{
    int n;
    int cap[N][N];
    int flow[N][N];

    void init(int n){
        this->n=n;
        memset(cap,0,sizeof(cap));
        memset(flow,0,sizeof(flow));
    }
    int maxFlow(int s,int t){
        int res=0;
        int dis[N];
        int p[N];

        queue<int> Q;
        while(true){
            memset(dis,0,sizeof(dis));
            dis[s]=INF;
            Q.push(s);

            while(!Q.empty()){
                int x=Q.front();
                Q.pop();
                for(int y=1;y<=n;y++){
                    if(!dis[y] && cap[x][y]>flow[x][y]){
                        p[y]=x;
                        Q.push(y);
                        dis[y]=min(dis[x],cap[x][y]-flow[x][y]);
                    }
                }
            }

            if(dis[t]==0)
                break;

            for(int x=t;x!=s;x=p[x]){
                flow[p[x]][x]+=dis[t];
                flow[x][p[x]]-=dis[t];
            }
            res+=dis[t];
        }
        return res;
    }

}EK;
int main(){
    int t;
    scanf("%d",&t);
    int Case=1;
    while(t--){
        int n,m;
        scanf("%d%d",&n,&m);
        EK.init(n);
        while(m--){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            EK.cap[x][y]=+w;
        }
        printf("Case %d: %d\n",Case++,EK.maxFlow(1,n));
    }
    return 0;
}
```





