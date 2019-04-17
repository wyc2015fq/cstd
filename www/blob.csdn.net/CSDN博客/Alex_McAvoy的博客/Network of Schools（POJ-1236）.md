# Network of Schools（POJ-1236） - Alex_McAvoy的博客 - CSDN博客





2018年10月24日 18:27:44[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：43








> 
# Problem Description

A number of schools are connected to a computer network. Agreements have been developed among those schools: each school maintains a list of schools to which it distributes software (the “receiving schools”). Note that if B is in the distribution list of school A, then A does not necessarily appear in the list of school B 

You are to write a program that computes the minimal number of schools that must receive a copy of the new software in order for the software to reach all schools in the network according to the agreement (Subtask A). As a further task, we want to ensure that by sending the copy of new software to an arbitrary school, this software will reach all schools in the network. To achieve this goal we may have to extend the lists of receivers by new members. Compute the minimal number of extensions that have to be made so that whatever school we send the new software to, it will reach all other schools (Subtask B). One extension means introducing one new member into the list of receivers of one school. 

# Input

The first line contains an integer N: the number of schools in the network (2 <= N <= 100). The schools are identified by the first N positive integers. Each of the next N lines describes a list of receivers. The line i+1 contains the identifiers of the receivers of school i. Each list ends with a 0. An empty list contains a 0 alone in the line.

# Output

Your program should write two lines to the standard output. The first line should contain one positive integer: the solution of subtask A. The second line should contain the solution of subtask B.

# Sample Input

5

2 4 3 0

4 5 0

0

0

1 0

# Sample Output

1

2


题意：给出 n 个结点，以及每个结点可到达的点，求有几个强连通分量，然后问至少在可到达的点中添加几个点，使得图变为强连通图

思路：先求出图的所有强连通分量，然后进行缩点构建新图，第一问是新图中入度为 0 点的个数，第二问是新图中入度0点数与出度0点数最大的一个

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
bool in[N],out[N];
bool vis[N];//标记数组
int sccno[N];//记录结点i属于哪个强连通分量
int block_cnt;//时间戳
int sig;//记录强连通分量个数
void Tarjan(int x){
    vis[x]=true;
    dfn[x]=low[x]=++block_cnt;//每找到一个新点，纪录当前节点的时间戳
    S.push(x);//当前结点入栈

    for(int i=0;i<G[x].size();i++){//遍历整个栈
        int y=G[x][i];//当前结点的下一结点
        if(vis[y]==false){//若未被访问过
            Tarjan(y);
            low[x]=min(low[x],low[y]);
        }
        else if(!sccno[y])//若已被访问过，且不属于任何一个连通分量
            low[x]=min(low[x],dfn[y]);
    }

    if(dfn[x]==low[x]){//满足强连通分量要求
        sig++;//记录强连通分量个数

        while(true){//记录元素属于第几个强连通分量
            int temp=S.top();
            S.pop();
            sccno[temp]=sig;
            if(temp==x)
                break;
        }
    }
}
int main()
{
    while(scanf("%d",&n)!=EOF)
    {
        for(int i=0;i<n;i++)
            G[i].clear();
        for(int i=0;i<n;i++){
            int y;
            while(scanf("%d",&y)!=EOF&&y){
                y--;
                G[i].push_back(y);
            }
        }


        sig=0;
        block_cnt=0;
        memset(vis,0,sizeof(vis));
        memset(dfn,0,sizeof(dfn));
        memset(low,0,sizeof(low));
        memset(sccno,0,sizeof(sccno));

        for(int i=0;i<n;i++)
            if(vis[i]==false)
                Tarjan(i);

        memset(in,false,sizeof(in));
        memset(out,false,sizeof(out));
        for(int i=1;i<=sig;i++)
            in[i]=out[i]=true;

        for(int x=0;x<n;x++)
            for(int i=0;i<G[x].size();i++){
                int y=G[x][i];
                if(sccno[x]!=sccno[y])
                    in[sccno[y]]=out[sccno[x]]=false;
            }
        int a=0,b=0;
        for(int i=1;i<=sig;i++){
            if(in[i])
                a++;
            if(out[i])
                b++;
        }
        if(sig==1)
            printf("1\n0\n");
        else
            printf("%d\n%d\n",a,max(a,b));
    }
}
```





