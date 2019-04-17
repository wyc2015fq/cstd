# Currency Exchange（POJ-1860） - Alex_McAvoy的博客 - CSDN博客





2018年11月11日 14:01:53[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：161








> 
# Problem Description

Several currency exchange points are working in our city. Let us suppose that each point specializes in two particular currencies and performs exchange operations only with these currencies. There can be several points specializing in the same pair of currencies. Each point has its own exchange rates, exchange rate of A to B is the quantity of B you get for 1A. Also each exchange point has some commission, the sum you have to pay for your exchange operation. Commission is always collected in source currency. 

For example, if you want to exchange 100 US Dollars into Russian Rubles at the exchange point, where the exchange rate is 29.75, and the commission is 0.39 you will get (100 - 0.39) * 29.75 = 2963.3975RUR. 

You surely know that there are N different currencies you can deal with in our city. Let us assign unique integer number from 1 to N to each currency. Then each exchange point can be described with 6 numbers: integer A and B - numbers of currencies it exchanges, and real RAB, CAB, RBA and CBA - exchange rates and commissions when exchanging A to B and B to A respectively. 

Nick has some money in currency S and wonders if he can somehow, after some exchange operations, increase his capital. Of course, he wants to have his money in currency S in the end. Help him to answer this difficult question. Nick must always have non-negative sum of money while making his operations.

# **Input**

The first line of the input contains four numbers: N - the number of currencies, M - the number of exchange points, S - the number of currency Nick has and V - the quantity of currency units he has. The following M lines contain 6 numbers each - the description of the corresponding exchange point - in specified above order. Numbers are separated by one or more spaces. 1<=S<=N<=100, 1<=M<=100, V is real number, 0<=V<=103. 

For each point exchange rates and commissions are real, given with at most two digits after the decimal point, 10-2<=rate<=102, 0<=commission<=102.

Let us call some sequence of the exchange operations simple if no exchange point is used more than once in this sequence. You may assume that ratio of the numeric values of the sums at the end and at the beginning of any simple sequence of the exchange operations will be less than 104.

# Output

If Nick can increase his wealth, output YES, in other case output NO to the output file.

# Sample Input

**3 2 1 20.0**

**1 2 1.00 1.00 1.00 1.00**

**2 3 1.10 1.00 1.10 1.00**

# Sample Output

**YES**


题意： 给出 n 种货币，以及 m 对货币之间的兑换率，现在手上有 S 货币，问能否通过不断的 对换来增加之间 S 货币的总量

思路：实质是给出一个 n 个点 m 条边的无向图，要从 S 出发，寻找图上是否存在一个递增的环，Ford 求的是最短路，而这里要找最长路，因此需要更改松弛情况，且当结点入队次数大于 n 时，那么图中存在递增环。

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
struct Edge
{
    int from;
    int to;
    double r;
    double c;
    Edge(int f,int t,double r,double c):from(f),to(t),r(r),c(c){}
};
struct BellmanFord
{
    int n,m;
    vector<Edge> edges;
    vector<int> G[N];
    bool inq[N];
    int cnt[N];
    double dis[N];

    void init(int n){//初始化
        this->n=n;
        for(int i=0;i<n;i++)
            G[i].clear();
        edges.clear();
    }

    void AddEdge(int from,int to,double r,double c){//添边
        edges.push_back(Edge(from,to,r,c));
        m=edges.size();
        G[from].push_back(m-1);
    }

    bool bellman_ford(int s,double val){
        memset(inq,0,sizeof(inq));
        memset(cnt,0,sizeof(cnt));

        for(int i=0;i<n;i++){
            if(i==s)
                dis[i]=val;
            else
                dis[i]=0;
        }

        queue<int> Q;
        Q.push(s);
        while(!Q.empty()){
            int u=Q.front();
            inq[u]=false;
            Q.pop();

            for(int i=0;i<G[u].size();i++){
                Edge &e=edges[G[u][i]];
                if(dis[e.to]<(dis[u]-e.c)*e.r)//松弛条件改变
                {
                    dis[e.to]=(dis[u]-e.c)*e.r;

                    if(!inq[e.to]){
                        inq[e.to]=true;
                        Q.push(e.to);

                        if(++cnt[e.to]>n)
                            return true;
                    }
                }
            }
        }
        return false;
    }
}BF;

int main()
{
    int n,m,s;
    double v;
    while(scanf("%d%d%d%lf",&n,&m,&s,&v)!=EOF)
    {
        s--;
        BF.init(n);

        while(m--)
        {
            int x,y;
            double r1,c1,r2,c2;
            scanf("%d%d%lf%lf%lf%lf",&x,&y,&r1,&c1,&r2,&c2);

            x--,y--;
            BF.AddEdge(x,y,r1,c1);
            BF.AddEdge(y,x,r2,c2);
        }
        printf("%s\n",BF.bellman_ford(s,v)?"YES":"NO");
    }
    return 0;
}
```





