# Hawk-and-Chicken（HDU-3639） - Alex_McAvoy的博客 - CSDN博客





2019年01月19日 18:52:54[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：45
个人分类：[HDU																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Kids in kindergarten enjoy playing a game called Hawk-and-Chicken. But there always exists a big problem: every kid in this game want to play the role of Hawk. 

So the teacher came up with an idea: Vote. Every child have some nice handkerchiefs, and if he/she think someone is suitable for the role of Hawk, he/she gives a handkerchief to this kid, which means this kid who is given the handkerchief win the support. Note the support can be transmitted. Kids who get the most supports win in the vote and able to play the role of Hawk.(A note:if A can win 

support from B(A != B) A can win only one support from B in any case the number of the supports transmitted from B to A are many. And A can't win the support from himself in any case. 

If two or more kids own the same number of support from others, we treat all of them as winner. 

Here's a sample: 3 kids A, B and C, A gives a handkerchief to B, B gives a handkerchief to C, so C wins 2 supports and he is choosen to be the Hawk.

# Input

There are several test cases. First is a integer T(T <= 50), means the number of test cases. 

Each test case start with two integer n, m in a line (2 <= n <= 5000, 0 <m <= 30000). n means there are n children(numbered from 0 to n - 1). Each of the following m lines contains two integers A and B(A != B) denoting that the child numbered A give a handkerchief to B.

# Output

For each test case, the output should first contain one line with "Case x:", here x means the case number start from 1. Followed by one number which is the total supports the winner(s) get. 

Then follow a line contain all the Hawks' number. The numbers must be listed in increasing order and separated by single spaces.

# **Sample Input**

**24 33 22 02 1**

**3 31 02 10 2**

# Sample Output

**Case 1: 20 1Case 2: 20 1 2**


题意：t 组数据，每组数据有 n 个人，每人都有一种手帕，他们之间的手帕具有传递性，现在给出 m 个行为 x y，表示 x 把手帕给了 y，得到不同种类的手帕最多的人是赢家，如果多个人同时最多，那么他们都是赢家，对于每组数据，问赢家最多能得到的手帕种类是多少，以及有哪些人是赢家

思路：首先进行强连通缩点，使得所有的强连通分量缩成一个 DAG 图，每个 DAG 图上点的权值代表相应强连通分量缩点前的个数。

在同一个连通分量里的传递出去的手帕数为连通分量内点的个数 -1，假设出度不为 0 的人 x 得到的手帕数最多，那么一定有一个人 y 被 x 给了手帕，必定 y 得到的手帕最多，假设矛盾，故而得到手帕种类最多的一定是出度为 0 的

为方便统计，缩点后建立反向图，然后在反图上对入度为 0 的点，即原 DAG 图上出度为 0 的点做 DFS，将路上的所有点的权值都统计，最后的权值就是包括该点内，所有遇到的点的权值和 -1（排除自己）

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
#define E 1e-6
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 100001
#define LL long long
using namespace std;

struct Edge{
    int to;
    int next;
};
Edge edge[N];//原图
Edge egdeT[N];//反图
int head[N],cnt;//原图
int headT[N],cntT;//反图
bool vis[N];//原图
bool visT[N];//反图
int dfn[N],low[N];
int bolckTime;//时间戳
int scc;//强连通分量个数
int inDegree[N];//入度
int Stack[N],top;
int belong[N];//缩点后的强连通分量
int res[N];//存储结果
int num[N];//第i个强连通分量中点的个数
void addEdge(int x,int y){//原图添边
    edge[cnt].to=y;
    edge[cnt].next=head[x];
    head[x]=cnt++;
}
void addEdgeT(int x,int y){//反图添边
    egdeT[cntT].to=y;
    egdeT[cntT].next=headT[x];
    headT[x]=cntT++;
}
void Tarjan(int x){
    dfn[x]=low[x]=++bolckTime;
    Stack[top++]=x;
    vis[x]=true;

    for (int i=head[x];i!=-1;i=edge[i].next){
        int y=edge[i].to;
        if(!dfn[y]){
            Tarjan(y);
            if(low[x]>low[y])
                low[x]=low[y];
        }
        else if(vis[y]&&low[x]>dfn[y])
            low[x]=dfn[y];
    }

    int y;
    if(low[x]==dfn[x]){
        scc++;
        do{
            y=Stack[--top];
            vis[y]=false;
            belong[y]=scc;
            num[scc]++;
        }while(y!=x);
    }

    return ;
}
int dfs(int x){
    visT[x]=true;
    int sum=num[x];
    for(int i=headT[x];i!=-1;i=egdeT[i].next){
        int y=egdeT[i].to;
        if(!visT[y])
            sum+=dfs(y);
    }
    return sum;
}

void solve(int n)
{
    memset(num,0,sizeof(num));
    memset(dfn,0,sizeof(dfn));
    memset(inDegree,0,sizeof(inDegree));
    memset(vis,false,sizeof(vis));
    top=bolckTime=scc=0;

    for(int i=0;i<n;i++)
        if(!dfn[i])
            Tarjan(i);


    for(int x=0;x<n;x++){
        for(int i=head[x];i!=-1;i=edge[i].next){
            int y=edge[i].to;
            if (belong[x]==belong[y])
                continue;

            addEdgeT(belong[y],belong[x]);//反向建图
            inDegree[belong[x]]++;
        }
    }


    int maxx=-INF;
    memset(res,-INF,sizeof(res));
    for(int i=1;i<=scc;i++){
        if(inDegree[i]==0){
            memset(visT,false,sizeof(visT));
            res[i]=dfs(i);
            maxx=max(maxx,res[i]);
        }
    }
    printf("%d\n",maxx-1);

    bool flag=true;
    for(int i=0;i<n;i++){
        if(res[belong[i]]==maxx){
            if(flag){
                flag=false;
                printf("%d",i);
            }
            else
                printf(" %d",i);
        }
    }
    printf("\n");
}

int main(){
    int t;
    scanf("%d",&t);

    int Case=1;
    while (t--){
        int n,m;
        scanf("%d%d",&n,&m);

        cnt=cntT=0;
        memset(headT,-1,sizeof(headT));
        memset(head,-1,sizeof(head));

        for (int i=0;i<m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            addEdge(x,y);
        }
        printf("Case %d: ",Case++);
        solve(n);
    }
    return 0;
}
```






