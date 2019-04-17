# Proving Equivalences（HDU-2767） - Alex_McAvoy的博客 - CSDN博客





2018年10月24日 20:48:06[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：36
个人分类：[HDU																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Consider the following exercise, found in a generic linear algebra textbook.

Let A be an n × n matrix. Prove that the following statements are equivalent:

1. A is invertible.

2. Ax = b has exactly one solution for every n × 1 matrix b.

3. Ax = b is consistent for every n × 1 matrix b.

4. Ax = 0 has only the trivial solution x = 0. 

The typical way to solve such an exercise is to show a series of implications. For instance, one can proceed by showing that (a) implies (b), that (b) implies (c), that (c) implies (d), and finally that (d) implies (a). These four implications show that the four statements are equivalent.

Another way would be to show that (a) is equivalent to (b) (by proving that (a) implies (b) and that (b) implies (a)), that (b) is equivalent to (c), and that (c) is equivalent to (d). However, this way requires proving six implications, which is clearly a lot more work than just proving four implications!

I have been given some similar tasks, and have already started proving some implications. Now I wonder, how many more implications do I have to prove? Can you help me determine this? 

# Input

On the first line one positive number: the number of testcases, at most 100. After that per testcase:

* One line containing two integers n (1 ≤ n ≤ 20000) and m (0 ≤ m ≤ 50000): the number of statements and the number of implications that have already been proved.

* m lines with two integers s1 and s2 (1 ≤ s1, s2 ≤ n and s1 ≠ s2) each, indicating that it has been proved that statement s1 implies statement s2. 

# Output

Per testcase:

* One line with the minimum number of additional implications that need to be proved in order to prove that all statements are equivalent. 

# Sample Input

2

4 0

3 2

1 2

1 3

# Sample Output

4

2


题意：给你一个有向图，问在图中最少要加多少条边能使得该图变成一个强连通图

思路：缩点模板题

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
int n,m;
vector<int> G[N];
stack<int> S;
int dfn[N],low[N];
bool vis[N];
int sccno[N];
bool in[N],out[N];//记录入度、出度是否为0
int block_cnt;
int sig;
void Tarjan(int x){
    vis[x]=true;
    dfn[x]=low[x]=++block_cnt;
    S.push(x);

    for(int i=0;i<G[x].size();i++){
        int y=G[x][i];
        if(vis[y]==false){
            Tarjan(y);
            low[x]=min(low[x],low[y]);
        }
        else if(!sccno[y])
            low[x]=min(low[x],dfn[y]);
    }

    if(dfn[x]==low[x]){
        sig++;
        while(true){
            int temp=S.top();
            S.pop();
            sccno[temp]=sig;
            if(temp==x)
                break;
        }
    }
}
void shrink(){//缩点
    memset(in,false,sizeof(in));
    memset(out,false,sizeof(out));
    for(int i=1;i<=sig;i++){
        in[i]=true;
        out[i]=true;
    }

    for(int x=0;x<n;x++){
        for(int i=0;i<G[x].size();i++){
            int y=G[x][i];
            if(sccno[x]!=sccno[y]){//统计每个点出度、入度是否为0
                out[sccno[x]]=false;
                in[sccno[y]]=false;
            }
        }
    }
}
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        for(int i=0;i<n;i++)
            G[i].clear();
        while(m--){
            int x,y;
            scanf("%d%d",&x,&y);
            x--;
            y--;
            G[x].push_back(y);
        }

        sig=0;
        block_cnt=0;
        memset(vis,false,sizeof(vis));
        memset(dfn,0,sizeof(dfn));
        memset(low,0,sizeof(low));
        memset(sccno,0,sizeof(sccno));

        for(int i=0;i<n;i++)
            if(vis[i]==false)
                Tarjan(i);

        shrink();
        int a=0,b=0;
        for(int i=1;i<=sig;i++){//统计入度、出度为0的点的个数
            if(in[i])
                a++;
            if(out[i])
                b++;
        }
        int res=max(a,b);
        if(sig==1)//强连通分量为1时
            res=0;
 
        printf("%d\n",res);
    }
}
```






