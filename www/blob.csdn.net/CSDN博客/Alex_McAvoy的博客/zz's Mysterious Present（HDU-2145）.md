# zz's Mysterious Present（HDU-2145） - Alex_McAvoy的博客 - CSDN博客





2018年11月11日 13:58:11[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：106








> 
# Problem Description

There are m people in n cities, and they all want to attend the party which hold by zz. They set out at the same time, and they all will choose the best way they think, but due to someone take a ride, someone drive, and someone take a taxi, they have different speed. Can you find out who will get zz's mysterious present? The first one get the party will get the present . If there are several people get at the same time, the one who stay in the city which is farther from the city where is zz at begin will get the present. If there are several people get at the same time and the distance from the city he is at begin to the city where zz is, the one who has the larger number will get the present.

# **Input**

The first line: three integers n, m and k. m is the total number of the people, and n is the total number of cities, and k is the number of the way.(0<n<=300, 0<m<=n, 0<k<5000)

The second line to the (k+1)th line: three integers a, b and c. There is a way from a to b, and the length of the way is c.(0<a,b<=n, 0<c<=100)

The (k+2)th line: one integer p(0<p<=n), p is the city where zz is.

The (k+3)th line: m integers. the ith people is at the place p[i] at begin.(0<p[i]<=n)

The (k+4)th line: m integers. the speed of the ith people is speed[i];(0<speed[i]<=100) 

All the ways are directed.

# Output

For each case, output the one who get the present in one line. If no one can get the present, output "No one".

# Sample Input

**3 1 31 2 21 3 32 3 1321**

# Sample Output

**1**


题意：给出一个 n 个点的有向图，m 个人要到达点 p，每个人都有各自的速度，求最先到达 p 点的人的编号，如果多人同时到达，优先输出初始点离 p 最远的，如果多个人初始距离同样远，优先输出编号最大的。

思路：建一个反向图，求点 p 到个人的距离，从而可以得出每个人到达的时间，最后遍历所有人的信息，按花费时间最少、初始距离最远、初始编号最大进行排序，最后输出即可

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
    int speed;
    int place;
    int id;
    int dis;
    double time;
}a[N];
int n,m,k;
int G[N][N];
int dis[N];
bool vis[N];
void Dijkstra(int s){
    memset(vis,false,sizeof(vis));
    vis[s]=true;
    for(int i=1;i<=n;i++)
        if(i!=s)
            dis[i]=G[s][i];
    dis[s]=0;

    for(int i=1;i<=n-1;i++){
        int minn=INF;
        int x=-1;
        for(int j=1;j<=m;j++){
            if(vis[j])
                continue;
            if(minn>dis[j]){
                minn=dis[j];
                x=j;
            }
        }

        if(x==-1)
            break;
        vis[x]=true;
        for(int j=1;j<=n;j++)
            if(!vis&&dis[j]>dis[x]+G[x][j])
                dis[j]=dis[x]+G[x][j];
    }
}
bool cmp(Node a,Node b){
    if(fabs(a.time-b.time)>E)
        return a.time>b.time;
    else if(a.dis!=b.dis)
        return a.dis>b.dis;
    else
        return a.id>b.id;
}
int main()
{
    while(scanf("%d%d%d",&n,&m,&k)!=EOF&&(n+m+k))
    {
        memset(G,INF,sizeof(G));
        for(int i=1;i<=n;i++)
            G[i][i]=0;

        for(int i=1;i<=k;i++){
            int x,y,w;
            scanf("%d%d%d",&x,&y,&w);
            G[y][x]=min(G[y][x],w);
        }

        int start;
        scanf("%d",&start);
        Dijkstra(start);
        for(int i=1;i<=m;i++){
            scanf("%d",&a[i].place);
            a[i].id=i;
            a[i].dis=dis[a[i].place];
        }

        for(int i=1;i<=m;i++){
            scanf("%d",&a[i].speed);
            if(dis[a[i].place]==INF)
                a[i].time=INF;
            else
                a[i].time=1.0*a[i].dis/a[i].speed;
        }

        sort(a+1,a+1+m,cmp);
        if(a[1].dis==INF)
            printf("No one\n");
		else
            printf("%d\n",a
                   [1].id);

    }
    return 0;
}
```





