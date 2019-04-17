# Schedule Problem（HDU-1534） - Alex_McAvoy的博客 - CSDN博客





2018年12月23日 21:26:48[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：52








> 
# Problem Description

A project can be divided into several parts. Each part should be completed continuously. This means if a part should take 3 days, we should use a continuous 3 days do complete it. There are four types of constrains among these parts which are FAS, FAF, SAF and SAS. A constrain between parts is FAS if the first one should finish after the second one started. FAF is finish after finish. SAF is start after finish, and SAS is start after start. Assume there are enough people involved in the projects, which means we can do any number of parts concurrently. You are to write a program to give a schedule of a given project, which has the shortest time. 

# **Input**

The input file consists a sequences of projects.

Each project consists the following lines:

the count number of parts (one line) (0 for end of input)

times should be taken to complete these parts, each time occupies one line

a list of FAS, FAF, SAF or SAS and two part number indicates a constrain of the two parts

a line only contains a '#' indicates the end of a project 

# Output

Output should be a list of lines, each line includes a part number and the time it should start. Time should be a non-negative integer, and the start time of first part should be 0. If there is no answer for the problem, you should give a non-line output containing "impossible".

A blank line should appear following the output for each project.

# Sample Input

**3234SAF 2 1FAF 3 2#3111SAF 2 1SAF 3 2SAF 1 3#0**

# Sample Output

**Case 1:1 02 23 1**

**Case 2:impossible**


题意：有一由 n 个部分构成的工程，每一部分都有一个连续的运行时间 dis[i]，现在给出 m 条约束条件 FAS/FAF/SAF/SAS u v，问该系统是否有解，若有解则输出各个部分的开始时间

思路：用 S[i] 表示第 i 部分的开始时间，dis[i] 表示第 i 部分的运行时间，则根据 m 条约束条件，有：
- FAS u v：S[u]+dis[u]>= S[v]，有：S[v]<=S[u]+dis[u]，即建立 u 到 v 权值为 dis[u] 的边
- FAF u v：S[u]+dis[u]>=S[v]+dis[v]，有：S[v]<=S[u]+dis[u]-dis[v]，即建立 u 到 v 权值 为 dis[u]-dis[v] 的边
- SAF u v：S[u]>=S[v]+dis[v]，有：S[v]<=S[u]-dis[v]，即建立 u 到 v 权值为 -dis[v] 的边
- SAS u v：S[u]>=S[v]，有：S[v]<=S[u]，即建立 u 到 v 权值为 0 的边

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
bool SPFA(int s,int n){
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
            return true;

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
    return false;
}
int D[N];
int main(){
    int n;
    int Case=1;
    while(scanf("%d",&n)!=EOF&&(n)){
        init();
        for(int i=1;i<=n;i++)
            scanf("%d",&D[i]);

        char ch[10];
        while(scanf("%s",ch)!=EOF&&ch[0]!='#'){
            int x,y;
            scanf("%d%d",&x,&y);

            if(strcmp(ch,"FAS")==0)
                addEdge(x,y,D[x]);
            else if(strcmp(ch,"FAF")==0)
                addEdge(x,y,D[x]-D[y]);
            else if(strcmp(ch,"SAF")==0)
                addEdge(x,y,-D[y]);
            else if(strcmp(ch,"SAS")==0)
                addEdge(x,y,0);
        }
        for(int i=1;i<=n;i++)
            addEdge(0,i,0);

        if(Case>0)
            printf("\n");
        printf("Case %d:\n",Case++);

        if(SPFA(0,n+1))
            printf("impossible\n");
        else{
            int minn=INF;
            for(int i=1;i<=n;i++)
                minn=min(minn,dis[i]);
            for(int i=1;i<=n;i++)
                printf("%d %d\n",i,dis[i]-minn);
        }
    }
    return 0;
}
```





