# Codeforces D. Fair 多源BFS求最短路 - 紫芝的博客 - CSDN博客





2018年06月02日 21:30:33[紫芝](https://me.csdn.net/qq_40507857)阅读数：118









Some company is going to hold a fair in Byteland. There are n

 towns in Byteland and m
 two-way roads between towns. Of course, you can reach any town from any other town using roads.

There are k
 types of goods produced in Byteland and every town produces only one type. To hold a fair you have to bring at least sdifferent types of goods. It costs d(u,v) coins to bring goods from town u to town v where d(u,v) is the length of the shortest path from uto v
. Length of a path is the number of roads in this path.

The organizers will cover all travel expenses but they can choose the towns to bring goods from. Now they want to calculate minimum expenses to hold a fair in each of n


 towns.



Input

There are 4

 integers n, m, k, s in the first line of input (1≤n≤105, 0≤m≤105, 1≤s≤k≤min(n,100)
) — the number of towns, the number of roads, the number of different types of goods, the number of different types of goods necessary to hold a fair.

In the next line there are n
 integers a1,a2,…,an (1≤ai≤k), where ai is the type of goods produced in the i-th town. It is guaranteed that all integers between 1 and k occur at least once among integers ai
.

In the next m
 lines roads are described. Each road is described by two integers uv (1≤u,v≤n, u≠v

) — the towns connected by this road. It is guaranteed that there is no more than one road between every two towns. It is guaranteed that you can go from any town to any other town via roads.



Output

Print n

 numbers, the i-th of them is the minimum number of coins you need to spend on travel expenses to hold a fair in town i

. Separate numbers with spaces.



Examples



input
Copy

5 5 4 3
1 2 4 3 2
1 2
2 3
3 4
4 1
4 5



output
Copy

2 2 2 2 3 



input
Copy

7 6 3 2
1 2 3 3 2 2 1
1 2
2 3
3 4
2 5
5 6
6 7



output
Copy

1 1 1 2 2 1 1 





Note

Let's look at the first sample.

To hold a fair in town 1

 you can bring goods from towns 1 (0 coins), 2 (1 coin) and 4 (1 coin). Total numbers of coins is 2
.

Town 2
: Goods from towns 2 (0), 1 (1), 3 (1). Sum equals 2
.

Town 3
: Goods from towns 3 (0), 2 (1), 4 (1). Sum equals 2
.

Town 4
: Goods from towns 4 (0), 1 (1), 5 (1). Sum equals 2
.

Town 5

: Goods from towns 5 (0), 4 (1), 3 (2). Sum equals 3.

题意：

有n个城市，m条路，保证任意城市都相通，保证任意两个城市之间都只有1条路径。现在，要在某一个城市举办一场盛会，每个城市都会生产1种商品（不同城市之间生产的商品可能相同）共有k种不同的商品，现在，举办盛会需要s种不同的商品。每种商品都需要走到相应的城市去取。分别输出在n个城市举办盛会需要走的路（路径以单位路径来算例如，如果1----2----3，那么，1到3要走的路为2）

思路：

特产最多100种，可以计算每种特产到每个城镇的最短路，mov【i】【j】计算出所有j特产到所有i的最短距离，

这样的话是1e7，还可以勉强接受。



然后把所有到i城镇的特产的最短路 排序，取前s个就是i点的最短路径了。
直接将所有生产i型商品的城市一次性全加入队列，然后bfs

```cpp
#include<iostream>
#include<cstdio>
#include<vector>
#include<queue>
#include "algorithm"
using namespace std;
const int MAX=1e6+5;
int n,m,k,s;
int good[MAX],mov[MAX][105];
//mov[i][j]记录以生产j商品的城市为举办地，到i城市的最小花费
vector<int> E[MAX];
void bfs(int x)
{
    queue<int> Q;
    int i,u,v;
    Q.push(x+n);
    while(!Q.empty()){
        u=Q.front();
        Q.pop();
        for(i=0;i<E[u].size();i++){
            v=E[u][i];
            if(mov[v][x]==0){
                mov[v][x]=mov[u][x]+1;
                Q.push(v);
            }
        }
    }
}
int main()
{
    int i,j;
    int u,v,cnt;
    scanf("%d%d%d%d",&n,&m,&k,&s);
    for(i=1;i<=n;i++){
        scanf("%d",&good[i]);
        E[good[i]+n].push_back(i);//如果我们直接用一个for循环将生产good[i]型商品的城市加入队列的话，就会超时，所以，我们借用这个vector来处理！
    }
    for(i=0;i<m;i++){
        scanf("%d%d",&u,&v);
        E[u].push_back(v);
        E[v].push_back(u);
    }
    for(i=1;i<=k;i++) bfs(i);
    for(i=1;i<=n;i++){
        cnt=0;
        sort(mov[i]+1,mov[i]+k+1);
        for(j=1;j<=s;j++)
            cnt+=mov[i][j]-1;
        printf("%d ",cnt);
    }
    return 0;
}
```




```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn=1e5+10,inf=0x3f3f3f3f3f;
int n,m,s,k,ans[maxn];
vector<int> edge[maxn],color[maxn];
int dis[107][maxn];
void bfs(vector<int>color,int *dis){
    fill(dis+1,dis+1+n,-1);
    queue<int>q;
    for (auto u:color){
        dis[u]=0;
        q.push(u);
    }
    while(!q.empty()){
        int u=q.front();
        q.pop();
        for (int v:edge[u])
            if(dis[v]==-1) dis[v]=dis[u]+1,q.push(v);
    }
}
int main(){
    scanf("%d%d%d%d",&n,&m,&k,&s);
    for (int i=1,tmp;i<=n;i++) scanf("%d",&tmp),color[tmp].push_back(i);
    for (int i=1,u,v;i<=m;i++){
        scanf("%d%d",&u,&v);
        edge[u].push_back(v);
        edge[v].push_back(u);
    }
    for (int i=1;i<=k;i++) bfs(color[i],dis[i]);
    for (int i=1;i<=n;i++){
        for (int j=1;j<=k;j++)
            ans[j]=dis[j][i];
        sort(ans+1,ans+1+k);
        printf("%d",accumulate(ans+1,ans+1+s,0));
        printf("%c",i==n?'\n':' ');
    }
    return 0;
}
```




