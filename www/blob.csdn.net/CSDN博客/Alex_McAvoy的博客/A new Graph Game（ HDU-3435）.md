# A new Graph Game（ HDU-3435） - Alex_McAvoy的博客 - CSDN博客





2018年12月08日 20:52:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：63
个人分类：[HDU																[图论——二分图](https://blog.csdn.net/u011815404/article/category/8391132)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

An undirected graph is a graph in which the nodes are connected by undirected arcs. An undirected arc is an edge that has no arrow. Both ends of an undirected arc are equivalent--there is no head or tail. Therefore, we represent an edge in an undirected graph as a set rather than an ordered pair.

Now given an undirected graph, you could delete any number of edges as you wish. Then you will get one or more connected sub graph from the original one (Any of them should have more than one vertex).

You goal is to make all the connected sub graphs exist the Hamiltonian circuit after the delete operation. What’s more, you want to know the minimum sum of all the weight of the edges on the “Hamiltonian circuit” of all the connected sub graphs (Only one “Hamiltonian circuit” will be calculated in one connected sub graph! That is to say if there exist more than one “Hamiltonian circuit” in one connected sub graph, you could only choose the one in which the sum of weight of these edges is minimum).

  For example, we may get two possible sums:

![](https://img-blog.csdnimg.cn/20181208195202313.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

(1)  7 + 10 + 5 = 22

(2)  7 + 10 + 2 = 19

(There are two “Hamiltonian circuit” in this graph!)

# **Input**

In the first line there is an integer T, indicates the number of test cases. (T <= 20)

In each case, the first line contains two integers n and m, indicates the number of vertices and the number of edges. (1 <= n <=1000, 0 <= m <= 10000)

Then m lines, each line contains three integers a,b,c ,indicates that there is one edge between a and b, and the weight of it is c . (1 <= a,b <= n, a is not equal to b in any way, 1 <= c <= 10000)

# Output

Output “Case %d: “first where d is the case number counted from one. Then output “NO” if there is no way to get some connected sub graphs that any of them exists the Hamiltonian circuit after the delete operation. Otherwise, output the minimum sum of weight you may get if you delete the edges in the optimal strategy.

# Sample Input

**3**

**3 41 2 52 1 22 3 103 1 7 **

**3 21 2 31 2 4**

**2 21 2 31 2 4**

# Sample Output

**Case 1: 19Case 2: NOCase 3: 6**


题意：给出一 n 个点 m 条边的带权无向图，当这个图由 1 或多个不相交的有向环构成时，求这些有向环的最小权值

思路：与 [Cyclic Tour（HDU-1853）](https://blog.csdn.net/u011815404/article/details/84898177) 做法一致

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
#define N 1001
#define LL long long
using namespace std;
int n,m;
int G[N][N];
int Lx[N],Ly[N];
bool visX[N],visY[N];
int linkX[N],linkY[N];
bool dfs(int x){
    visX[x]=true;
    for(int y=1;y<=n;y++){
        if(!visY[y]){
            int temp=Lx[x]+Ly[y]-G[x][y];
            if(temp==0){
                visY[y]=true;
                if(linkY[y]==-1 || dfs(linkY[y])){
                    linkX[x]=y;
                    linkY[y]=x;
                    return true;
                }
            }
        }
    }
    return false;
}
void update(){
    int minn=INF;
    for(int i=1;i<=n;i++)
        if(visX[i])
            for(int j=1;j<=n;j++)
                if(!visY[j])
                    minn=min(minn,Lx[i]+Ly[j]-G[i][j]);

    for(int i=1;i<=n;i++)
        if(visX[i])
            Lx[i]-=minn;

    for(int i=1;i<=n;i++)
        if(visY[i])
            Ly[i]+=minn;
}
int KM(){
    memset(linkX,-1,sizeof(linkX));
    memset(linkY,-1,sizeof(linkY));

    for(int i=1;i<=n;i++){
        Lx[i]=Ly[i]=0;
        for(int j=1;j<=n;j++)
            Lx[i]=max(Lx[i],G[i][j]);
    }

    for(int i=1;i<=n;i++){
        while(true){
            memset(visX,false,sizeof(visX));
            memset(visY,false,sizeof(visY));

            if(dfs(i))
                break;
            else
                update();
        }
    }

    int ans=0;
    for(int i=1;i<=n;i++){
        if(G[linkY[i]][i]==-INF){
            return 1;
        }
        ans+=G[linkY[i]][i];
    }

    return ans;
}
int main(){
    int t;
    int Case=1;
    scanf("%d",&t);
    while(t--){
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
                G[i][j]=-INF;

        while(m--){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            G[x][y]=max(G[x][y],-w);//处理重边
            G[y][x]=G[x][y];//无向图
        }
        int res=-KM();
        if(res==-1)
            printf("Case %d: NO\n",Case++);
        else
            printf("Case %d: %d\n",Case++,res);
    }
    return 0;
}
```






