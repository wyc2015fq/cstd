# Financial Crisis（HDU-3749） - Alex_McAvoy的博客 - CSDN博客





2018年10月29日 10:07:49[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：31
个人分类：[HDU																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Because of the financial crisis, a large number of enterprises go bankrupt. In addition to this, other enterprises, which have trade relation with the bankrup enterprises, are also faced with closing down. Owing to the market collapse, profit decline and funding chain intense, the debt-ridden entrepreneurs

have to turn to the enterprise with stably developing for help.

Nowadays, there exist a complex net of financial trade relationship between enterprises. So if one of enterprises on the same financial chain is faced with bankrupt, leading to cashflow's obstruction, the other enterprises related with it will be influenced as well. At the moment, the foresight entrepreneurs are expected the safer cooperation between enterprises. In this sense, they want to know how many financial chains between some pairs of enterprises are independent with each other. The indepence is defined that if there exist two roads which are made up of enterprises(Vertexs) and their financial trade relations(Edge) has the common start point S and end point T, and expect S and T, none of other enterprise in two chains is included in these two roads at the same time. So that if one of enterpirse bankrupt in one of road, the other will not be obstructed.

Now there are N enterprises, and have M pair of financial trade relations between them, the relations are Mutual. They need to ask about Q pairs of enterprises' safety situations. When two enterprises have two or more independent financial chains, we say they are safe enough, you needn't provide exact answers.

# Input

The Input consists of multiple test cases. The first line of each test case contains three integers, N ( 3 <= N <= 5000 ), M ( 0 <= M <= 10000 ) and Q ( 1 <= Q <= 1000 ). which are the number of enterprises, the number of the financial trade relations and the number of queries.

The next M lines, each line contains two integers, u, v ( 0 <= u, v < N && u != v ), which means enterpirse u and enterprise v have trade relations, you can assume that the input will not has parallel edge.

The next Q lines, each line contains two integers, u, v ( 0 <= u, v < N && u != v ), which means entrepreneurs will ask you the financial safety of enterpirse u and enterprise v.

The last test case is followed by three zeros on a single line, which means the end of the input.

# Output

For each case, output the test case number formated as sample output. Then for each query, output "zero" if there is no independent financial chains between those two enterprises, output "one" if there is only one such chains, or output "two or more".

# Sample Input

3 1 2

0 1

0 2

1 0

4 4 2

0 1

0 2

1 2

2 3

1 2

1 3

0 0 0

# Sample Output

Case 1:

zero

one

Case 2:

two or more

one


题意：给出一 n 个点 m 条边有 q 条询问的无向图，保证输入时无重边无自环，对于每条询问，要计算 u 点与 v 点之间有几条除首尾相连外，其他点不重复的路径，如果有 0 或 1 条输出 0 或 1，如果有 2 条以上，输出 two or more

思路：首先判断图是否连通，如果不连通，直接输出 0 即可，然后再计算点双连通分量，如果两点属于同一个点双连通分量，直接输出 two or more 即可，剩余的情况，虽然表示两点连通，但在不同的点双连通分量类，直接输出 1 即可

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
#define N 20001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
vector<int> G[N],bcc[N];
vector<int> belong[N];//第i个节点属于的所有点双连通分量的编号
int n,m,q;
int dfn[N],low[N];
int bccno[N];
int block_cnt,sig;
struct Edge{
    int x;
    int y;
};
stack<Edge> S;
int fa[N];
int Find(int x){
    if(fa[x]==-1)
        return x;
    return fa[x]=Find(fa[x]);
}
void Union(int x,int y){
    x=Find(x);
    y=Find(y);
    if(x!=y)
        fa[x]=y;
}
void Tarjan(int x,int father){
    low[x]=dfn[x]=++block_cnt;

    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];

        if(y==father)
            continue;

        Edge e;
        e.x=x;
        e.y=y;
        if(dfn[y]==0){
            S.push(e);
            Tarjan(y,x);
            low[x]=min(low[x],low[y]);

            if(dfn[x]<=low[y]){
                sig++;
                bcc[sig].clear();
                while(true){
                    Edge temp=S.top();
                    S.pop();
                    if(bccno[temp.x]!=sig){
                        bcc[sig].push_back(temp.x);
                        bccno[temp.x]=sig;
                        belong[temp.x].push_back(sig);
                    }
                    if(bccno[temp.y]!=sig){
                        bcc[sig].push_back(temp.y);
                        bccno[temp.y]=sig;
                        belong[temp.y].push_back(sig);
                    }

                    if(temp.x==x && temp.y==y)
                        break;
                }
            }
        }
        else if(dfn[y]<dfn[x]){
            S.push(e);
            low[x]=min(low[x],dfn[y]);
        }
    }
}
int main()
{
    int Case=1;
    while(scanf("%d%d%d",&n,&m,&q)!=EOF&&n){
        sig=block_cnt=0;
        memset(dfn,0,sizeof(dfn));
        memset(bccno,0,sizeof(bccno));
        memset(fa,-1,sizeof(fa));
        for(int i=0;i<n;i++)
            G[i].clear();

        for(int i=1;i<=m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            G[x].push_back(y);
            G[y].push_back(x);
            Union(x,y);
        }

        for(int i=0;i<n;i++)
            if(dfn[i]==0)
                Tarjan(i,-1);

        printf("Case %d:\n",Case++);
        while(q--){
            int x,y;
            scanf("%d%d",&x,&y);

            if(Find(x)!=Find(y))//并查集判断连通性
                printf("zero\n");
            else{
                bool flag=false;
                for(int i=0;i<belong[x].size()&&!flag;i++){
                    for(int j=0;j<belong[y].size()&&!flag;j++){
                        if(belong[x][i]==belong[y][j]){
                            int cnt=belong[x][i];
                            if(bcc[cnt].size()>2){
                                printf("two or more\n");
                                flag=true;
                            }
                        }
                    }
                }

                if(!flag)
                    printf("one\n");
            }
        }
    }
    return 0;
}
```






