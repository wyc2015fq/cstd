# Critical Links（LightOJ - 1026） - Alex_McAvoy的博客 - CSDN博客





2019年01月23日 20:03:42[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：37
个人分类：[LightOJ																[图论——图的连通性](https://blog.csdn.net/u011815404/article/category/8236848)](https://blog.csdn.net/u011815404/article/category/8037630)








> 
# Problem Description

In a computer network a link L, which interconnects two servers, is considered critical if there are at least two servers A and B such that all network interconnection paths between A and B pass through L. Removing a critical link generates two disjoint sub-networks such that any two servers of a sub-network are interconnected. For example, the network shown in figure 1 has three critical links that are marked red: 0 - 1, 3 - 4 and 6 - 7 in figure 2.

![](https://img-blog.csdnimg.cn/20190123191513653.jpg)

Figure 1: Original Graph

![](https://img-blog.csdnimg.cn/2019012319152061.jpg)

Figure 2: The Critical Links

It is known that:

1.      The connection links are bi-directional.

2.      A server is not directly connected to itself.

3.      Two servers are interconnected if they are directly connected or if they are interconnected with the same server.

4.      The network can have stand-alone sub-networks.

Write a program that finds all critical links of a given computer network.

# Input

Input starts with an integer T (≤ 15), denoting the number of test cases.

Each case starts with a blank line. The next line will contain n (0 ≤ n ≤ 10000) denoting the number of nodes. Each of the next n lines will contain some integers in the following format:

u (k) v1 v2 ... vk

Where u is the node identifier, k is the number of adjacent nodes; v1, v2 ... vk are the adjacent nodes of u. You can assume that there are at most 100000 edges in total in a case. Dataset is huge, so use faster i/o methods.

# Output

For each case, print the case number first. Then you should print the number of critical links and the critical links, one link per line, starting from the beginning of the line, as shown in the sample output below. The links are listed in ascending order according to their first element and then second element. Since the graph is bidirectional, print a link u v if u < v.

# Sample Input

**3**

**80 (1) 11 (3) 2 0 32 (2) 1 33 (3) 1 2 44 (1) 37 (1) 66 (1) 75 (0)**

**0**

**20 (1) 11 (1) 0**

# Sample Output

**Case 1:3 critical links0 - 13 - 46 - 7Case 2:0 critical linksCase 3:1 critical links0 - 1**


题意：给出一个 无向图，求桥的个数

思路：求桥模版题，注意输入格式

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
    int next;
    int to;
}edge[N*2];
int head[N],edgeCnt;
struct Node{
    int from;
    int to;
    bool operator < (const Node &rhs)const{
        return from<rhs.from||(from==rhs.from&&to<rhs.to);
    }
}node[N];
int nodeCnt;
int father[N];
int dfn[N],low[N];
int block_cnt;
void addEdge(int next,int to){
    edge[edgeCnt].to=to;
    edge[edgeCnt].next=head[next];
    head[next]=edgeCnt++;
}
void tarjin(int x,int father){
    low[x]=dfn[x]=block_cnt++;
    for(int i=head[x];i!=-1;i=edge[i].next){
        int y=edge[i].to;
        if(!dfn[y]){
            tarjin(y,x);
            if(low[x]>low[y])
                low[x]=low[y];
            if(low[y]>dfn[x]){
                node[nodeCnt].from=x;
                node[nodeCnt].to=y;
                if(x>y)
                    swap(node[nodeCnt].from,node[nodeCnt].to);
                nodeCnt++;
            }
        }
        else if(y!=father)
            low[x]=min(low[x],dfn[y]);
    }
}
int main(){
    int t;
    scanf("%d",&t);
    int Case=1;
    while(t--){
        memset(low,0,sizeof(low));
        memset(dfn,0,sizeof(dfn));
        memset(head,-1,sizeof((head)));
        nodeCnt=0;
        edgeCnt=1;
        block_cnt=1;

        int n;
        scanf("%d",&n);
        int m=n-1;
        for(int i=0;i<n;i++){
            int a,b,c;
            scanf("%d (%d)",&a,&b);

            while(b--){
                scanf("%d",&c);
                addEdge(a,c);
            }
        }

        for(int i=0;i<=m;i++)
            if(!dfn[i])
                tarjin(i,-1);
        sort(node,node+nodeCnt);

        printf("Case %d:\n",Case++);
        printf("%d critical links\n",nodeCnt);
        for(int i=0;i<nodeCnt;i++)
            printf("%d - %d\n",node[i].from,node[i].to);
    }

	return 0;
}
```






