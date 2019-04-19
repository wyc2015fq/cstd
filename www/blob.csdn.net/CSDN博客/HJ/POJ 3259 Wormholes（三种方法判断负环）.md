# POJ  3259  Wormholes（三种方法判断负环） - HJ - CSDN博客
2017年01月18日 02:08:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：363
Description
While exploring his many farms, Farmer John has discovered a number of amazing wormholes. A wormhole is very peculiar because it is a one-way path that delivers you to its destination at a time that is BEFORE you entered the wormhole! Each of FJ’s farms comprises N (1 ≤ N ≤ 500) fields conveniently numbered 1..N, M (1 ≤ M ≤ 2500) paths, and W (1 ≤ W ≤ 200) wormholes.
As FJ is an avid time-traveling fan, he wants to do the following: start at some field, travel through some paths and wormholes, and return to the starting field a time before his initial departure. Perhaps he will be able to meet himself :) .
To help FJ find out whether this is possible or not, he will supply you with complete maps to F (1 ≤ F ≤ 5) of his farms. No paths will take longer than 10,000 seconds to travel and no wormhole can bring FJ back in time by more than 10,000 seconds.
Input 
Line 1: A single integer, F. F farm descriptions follow. 
Line 1 of each farm: Three space-separated integers respectively: N, M, and W 
Lines 2..M+1 of each farm: Three space-separated numbers (S, E, T) that describe, respectively: a bidirectional path between S and E that requires T seconds to traverse. Two fields might be connected by more than one path. 
Lines M+2..M+W+1 of each farm: Three space-separated numbers (S, E, T) that describe, respectively: A one way path from S to E that also moves the traveler back T seconds.
Output 
Lines 1..F: For each farm, output “YES” if FJ can achieve his goal, otherwise output “NO” (do not include the quotes).
Sample Input
2 
3 3 1 
1 2 2 
1 3 4 
2 3 1 
3 1 3 
3 2 1 
1 2 3 
2 3 4 
3 1 8
Sample Output
NO 
YES
Hint 
For farm 1, FJ cannot travel back in time. 
For farm 2, FJ could travel back in time by the cycle 1->2->3->1, arriving back at his starting location 1 second before he leaves. He could start from anywhere on the cycle to accomplish this.
题目大意： 
     规定n个点，可以从任一点出发，要求判断最后能否回到该点来。已知一些连通边，双向连通边为正权，单向连通边为负。
解法： 
    判断两点（1）能否构成环（2）环的权值和是否为负数；
以下是三种解法：
**解法一：**
    Bellman-Ford（如果循环过后发现仍然可以继续松弛，则出现负环）
```cpp
#include<iostream>
using namespace std;
#define INF 0x3f3f3f3f
struct edge{
    int s,e,t;
}edge[50005];
int n,m,k,cnt,dis[1001];
int Bellman_Ford()
{
    for(int i=0;i<n-1;i++)
    {
        int flag=0;
        for(int j=0;j<cnt;j++)
          if(dis[edge[j].e]>dis[edge[j].s]+edge[j].t)
          {
              dis[edge[j].e]=dis[edge[j].s]+edge[j].t;
              flag=1;
          }
        if(!flag)       //已经不能再松弛了，说明不可能再构成负环了，直接break
          break;
    } 
    for(int j=0;j<cnt;j++)            //还能继续松弛，则说明出现负环
      if(dis[edge[j].e]>dis[edge[j].s]+edge[j].t)
        return 1;
    return 0;
}
int main()
{
    int t,u,v,w;
    cin>>t;
    while(t--)
    {
        memset(dis,INF,sizeof(dis));
        cin>>n>>m>>k;
        cnt=0;
        while(m--)
        {
            cin>>u>>v>>w;
            edge[cnt].s=edge[cnt+1].e=u;
            edge[cnt].e=edge[cnt+1].s=v;
            edge[cnt].t=edge[cnt+1].t=w;
            cnt+=2;
        }
        while(k--)
        {
            cin>>u>>v>>w;
            edge[cnt].s=u;
            edge[cnt].e=v;
            edge[cnt++].t=-w;
        }
        if(Bellman_Ford())
          cout<<"YES"<<endl;
        else
          cout<<"NO"<<endl;
    }
    return 0;
}
```
**解法二**
    SPFA（Bellman-Ford的优先队列表示） 
      判断入队次数如果超过Ｎ结点数，则说明出现负环
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
using namespace std;
const int maxn=10010;
const int INF=0x3f3f3f3f;
int map[maxn][maxn],dis[maxn],vis[maxn];
int que[maxn],cnt[maxn];
int n,m,k,s;
int SPFA()
{
    for(int i=1;i<=n;i++)
      dis[i]=INF;
    dis[s]=0;
    vis[s]=1;
    int head=0,tail=1;
    que[0]=s;
    while(head<tail)
    {
        int pos=que[head];
        for(int i=1;i<=n;i++)
        {
            if(dis[i]>dis[pos]+map[pos][i])
            {
                dis[i]=dis[pos]+map[pos][i];
                if(!vis[i])
                {
                    que[tail++]=i;
                    vis[i]=1;
                    cnt[i]++;
                    if(cnt[i]>n)
                      return 1;
                }
            }
        }
        vis[pos]=0;
        head++;
    }
    return 0;
}
int main()
{
    int t,a,b,c;
    scanf("%d",&t);
    while(t--)
    {
        memset(que,0,sizeof(que));
        memset(cnt,0,sizeof(cnt));
        memset(vis,0,sizeof(vis));
        scanf("%d%d%d",&n,&m,&k);   
        for(int i=1;i<=n;i++)
          for(int j=1;j<=n;j++)
            map[i][j]=INF;
        while(m--)
        {
            scanf("%d%d%d",&a,&b,&c);
            if(c<map[a][b])
              map[a][b]=map[b][a]=c;
        } 
        while(k--)
        {
            scanf("%d%d%d",&a,&b,&c);
            map[a][b]=-c; 
        }
        s=0;           //加入虚拟的起始点0，到其它点的距离也为0 
        for(int i=1;i<=n;i++)
          map[s][i]=0;
        if(SPFA())
          printf("YES\n");
        else
          printf("NO\n"); 
    }
    return 0;
}
```
**解法三**
　　Floyd（判断方法同Bellman-Ford，看循环过后是否还能继续松弛）
```cpp
#include<iostream>
#include<cstdio>
#include<cstring> 
using namespace std;
#define INF 0x3f3f3f3f
int map[505][505],n,m,k;
int floyd()
{
    for(int k=1;k<=n;k++)
      for(int i=1;i<=n;i++)
      {  
          for(int j=1;j<=n;j++)
            if(map[i][j]>map[i][k]+map[k][j])
              map[i][j]=map[i][k]+map[k][j];
          if(map[i][i]<0)          //若map[i][i]!=0,则说明出现环，而如果;map[i][i]<0，进一步说明是负环 
            return 1;
      }
    return 0;
} 
int main()
{
    int t;
    scanf("%d",&t);
    while(t--)
    {
        int i,j,a,b,c;
        scanf("%d%d%d",&n,&m,&k);
        memset(map,INF,sizeof(map));
        for(i=1;i<=n;i++)
          map[i][i]=0;
        while(m--)
        {
            scanf("%d%d%d",&a,&b,&c);
            if(c<map[a][b])
              map[a][b]=map[b][a]=c;
        }
        while(k--)
        {
            scanf("%d%d%d",&a,&b,&c);
            map[a][b]=-c;
        } 
        if(floyd())
          printf("YES\n");
        else 
          printf("NO\n");
    }
    return 0;
}
```
用一个负环刚好复习一下最短路算法！！！
