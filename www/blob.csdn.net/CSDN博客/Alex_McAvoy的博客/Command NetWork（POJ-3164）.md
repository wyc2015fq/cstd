# Command NetWork（POJ-3164） - Alex_McAvoy的博客 - CSDN博客





2019年01月05日 18:31:43[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：69
个人分类：[POJ																[图论——最小树形图](https://blog.csdn.net/u011815404/article/category/8586883)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

After a long lasting war on words, a war on arms finally breaks out between littleken’s and KnuthOcean’s kingdoms. A sudden and violent assault by KnuthOcean’s force has rendered a total failure of littleken’s command network. A provisional network must be built immediately. littleken orders snoopy to take charge of the project.

With the situation studied to every detail, snoopy believes that the most urgent point is to enable littenken’s commands to reach every disconnected node in the destroyed network and decides on a plan to build a unidirectional communication network. The nodes are distributed on a plane. If littleken’s commands are to be able to be delivered directly from a node A to another node B, a wire will have to be built along the straight line segment connecting the two nodes. Since it’s in wartime, not between all pairs of nodes can wires be built. snoopy wants the plan to require the shortest total length of wires so that the construction can be done very soon.

# **Input**

The input contains several test cases. Each test case starts with a line containing two integer N (N ≤ 100), the number of nodes in the destroyed network, and M (M ≤ 104), the number of pairs of nodes between which a wire can be built. The next N lines each contain an ordered pair xi and yi, giving the Cartesian coordinates of the nodes. Then follow M lines each containing two integers i and j between 1 and N (inclusive) meaning a wire can be built between node i and node j for unidirectional command delivery from the former to the latter. littleken’s headquarter is always located at node 1. Process to end of file.

# Output

For each test case, output exactly one line containing the shortest total length of wires to two digits past the decimal point. In the cases that such a network does not exist, just output ‘poor snoopy’.

# Sample Input

**4 60 64 60 07 201 21 32 33 43 13 24 30 01 00 11 21 34 12 3**

# Sample Output

**31.19poor snoopy**


题意：给出 n 个点 m 条有向边，首先给出从 1 号点到 n 号点的笛卡尔坐标，然后再给出 m 条边，1 号点始终为根节点，求最小树形图

思路：朱刘算法第一题。。。边的权值用 double 型，求一下距离直接建图后，然后套模版。。。注意模版需要改为 double 型

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
struct Node{
    double x,y;
}node[N];
struct Edge{
    int x,y;
    double w;
}edge[N];
int vis[N];
int id[N];
int pre[N];
double in[N];
double zhuLiu(int root,int n,int m){
    double res=0;
    while(true){
        for(int i=0;i<n;i++)
            in[i]=INF;

        for(int i=0;i<m;i++){
            int x=edge[i].x;
            int y=edge[i].y;
            if(edge[i].w<in[y] && x!=y){
                pre[y]=x;
                in[y]=edge[i].w;
            }
        }

        for(int i=0;i<n;i++){
            if(i==root)
                continue;
            if(in[i]==INF)
                return -1;
        }

        int cnt=0;
        in[root]=0;
        memset(id,-1,sizeof(id));
        memset(vis,-1,sizeof(vis));
        for(int i=0;i<n;i++){
            res+=in[i];

            int y=i;
            while(vis[y]!=i&&id[y]==-1&&y!=root){
                vis[y]=i;
                y=pre[y];
            }

            if(y!=root&&id[y]==-1){
                for(int x=pre[y];x!=y;x=pre[x])
                    id[x]=cnt;
                id[y]=cnt++;
            }
        }
        if(cnt==0)
            break;
        for(int i=0;i<n;i++)
            if(id[i]==-1)
                id[i]=cnt++;

        for(int i=0;i<m;i++){
            int x=edge[i].x;
            int y=edge[i].y;
            edge[i].x=id[x];
            edge[i].y=id[y];

            if(id[x]!=id[y])
                edge[i].w-=in[y];
        }

        n=cnt;
        root=id[root];
    }
    return res;
}
double calculate(Node a,Node b){
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
}
int main(){
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF&&(n+m)){
        for(int i=0;i<n;i++)
            scanf("%lf%lf",&node[i].x,&node[i].y);
        for(int i=0;i<m;i++){
            scanf("%d%d",&edge[i].x,&edge[i].y);
            edge[i].x--;
            edge[i].y--;
            if(edge[i].x!=edge[i].y)
                edge[i].w=calculate(node[edge[i].x],node[edge[i].y]);
            else
                edge[i].w=INF;
        }
        double res=zhuLiu(0,n,m);
        if(res==-1)
            printf("poor snoopy\n");
        else
            printf("%.2f\n",res);
    }
    return 0;
}
```






