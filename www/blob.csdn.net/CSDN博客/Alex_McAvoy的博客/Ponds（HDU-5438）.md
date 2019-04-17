# Ponds（HDU-5438） - Alex_McAvoy的博客 - CSDN博客





2019年01月23日 11:05:34[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54
个人分类：[HDU																[图论——AOV网与拓扑排序](https://blog.csdn.net/u011815404/article/category/7813285)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

Betty owns a lot of ponds, some of them are connected with other ponds by pipes, and there will not be more than one pipe between two ponds. Each pond has a value vv. 

Now Betty wants to remove some ponds because she does not have enough money. But each time when she removes a pond, she can only remove the ponds which are connected with less than two ponds, or the pond will explode. 

Note that Betty should keep removing ponds until no more ponds can be removed. After that, please help her calculate the sum of the value for each connected component consisting of a odd number of ponds

# Input

The first line of input will contain a number T(1≤T≤30) which is the number of test cases. 

For each test case, the first line contains two number separated by a blank. One is the number p(1≤p≤104) which represents the number of ponds she owns, and the other is the number m(1≤m≤105) which represents the number of pipes. 

The next line contains pp numbers v1,...,vp, where vi(1≤vi≤108) indicating the value of pond i. 

Each of the last mm lines contain two numbers a and b, which indicates that pond a and pond b are connected by a pipe.

# Output

For each test case, output the sum of the value of all connected components consisting of odd number of ponds after removing all the ponds connected with less than two pipes.

# **Sample Input**

**17 71 2 3 4 5 6 71 41 54 52 32 63 62 7**

# Sample Output

**21**


题意：给出 n 个点 m 条边的无向图，要求删除入度小于等于 1 的点，由于删除后可能会形成 x 个连通分支，求连通分支中点数为奇数的权值和

思路：可以用拓扑排序来删点，删完后跑一遍 dfs 算权值和

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
}edge[N];
int head[N],cnt;
int n, m;
LL value[N];
int deg[N];//度数
bool vis[N];
int num;//点个数
LL tmp;//每个连通分量的权值和
void addEdge(int next,int to){
    edge[cnt].to=to;
    edge[cnt].next=head[next];
    head[next]=cnt++;
}
void topSort(){
    queue <int> q;

    for(int i=1;i<=n;i++){
        if(deg[i]==0)
            vis[i]=true;
        if(deg[i]==1){
            vis[i]=true;
            q.push(i);
        }
    }

    while(!q.empty()){
        int x=q.front();
        q.pop();
        for(int i=head[x];i!=-1;i=edge[i].next){
            int y=edge[i].to;
            deg[y]--;

            if(deg[y]==0)
                vis[y]=true;
            if(deg[y]==1){
                vis[y]=true;
                q.push(y);
            }
        }
    }

}
void dfs(int x, int father){
    vis[x]=true;
    num++;
    tmp+=value[x];

    for(int i=head[x];i!=-1;i=edge[i].next){
        int y=edge[i].to;
        if(!vis[y]&&y!=father)
            dfs(y,x);
    }

    return;
}
int main()
{
    int T;
    scanf("%d", &T);
    while(T--){
        cnt=0;
        memset(head,-1,sizeof((head)));
        memset(deg,0,sizeof(deg));
        memset(vis,false,sizeof(vis));

        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++)
            scanf("%lld",&value[i]);
        for(int i=1;i<=m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            addEdge(x,y);
            addEdge(y,x);
            deg[x]++;
            deg[y]++;
        }

        topSort();

        LL ans=0;
        for(int i=1;i<=n;i++){
            if(!vis[i]){
                num=0;
                tmp=0;
                dfs(i,0);
                if(num&1)
                    ans+=tmp;
            }
        }
        printf("%lld\n",ans);
    }
}
```






