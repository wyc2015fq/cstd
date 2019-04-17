# poj3259——Wormholes - westbrook1998的博客 - CSDN博客





2018年08月18日 22:01:35[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：123








> 
While exploring his many farms, Farmer John has discovered a number of amazing wormholes. A wormhole is very peculiar because it is a one-way path that delivers you to its destination at a time that is BEFORE you entered the wormhole! Each of FJ’s farms comprises N (1 ≤ N ≤ 500) fields conveniently numbered 1..N, M (1 ≤ M ≤ 2500) paths, and W (1 ≤ W ≤ 200) wormholes. 

  As FJ is an avid time-traveling fan, he wants to do the following: start at some field, travel through some paths and wormholes, and return to the starting field a time before his initial departure. Perhaps he will be able to meet himself :) . 

  To help FJ find out whether this is possible or not, he will supply you with complete maps to F (1 ≤ F ≤ 5) of his farms. No paths will take longer than 10,000 seconds to travel and no wormhole can bring FJ back in time by more than 10,000 seconds. 

  Input 

  Line 1: A single integer, F. F farm descriptions follow.  

  Line 1 of each farm: Three space-separated integers respectively: N, M, and W  

  Lines 2.. M+1 of each farm: Three space-separated numbers ( S, E, T) that describe, respectively: a bidirectional path between S and E that requires T seconds to traverse. Two fields might be connected by more than one path.  

  Lines M+2.. M+ W+1 of each farm: Three space-separated numbers ( S, E, T) that describe, respectively: A one way path from S to E that also moves the traveler back T seconds. 

  Output 

  Lines 1.. F: For each farm, output “YES” if FJ can achieve his goal, otherwise output “NO” (do not include the quotes). 

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
这题是负权环的，也是用spfa，和上一题相反，主要这个题意一开始没看懂

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=1000;
const int INF=0x3f3f3f3f;
int t;
int n,m,w;
int u,v,wi;
int g[N][N];
int dis[N];
bool vis[N];
int cnt[N];
bool spfa(){
    for(int i=1;i<=n;i++){
        dis[i]=INF;
        vis[i]=false;
        cnt[i]=0;
    }
    vis[1]=true;
    dis[1]=0;
    cnt[1]=1;
    queue<int> q;
    q.push(1);
    while(!q.empty()){
        int cur=q.front();
        q.pop();
        vis[cur]=false;
        for(int i=1;i<=n;i++){
            if(i==cur || g[cur][i]==INF){
                continue;
            }
            if(dis[i]>dis[cur]+g[cur][i]){
                dis[i]=dis[cur]+g[cur][i];
                if(!vis[i]){
                    cnt[i]++;
                    //判断负环 第n+1次加入队列
                    if(cnt[i]>=n){
                        return true;
                    }
                    q.push(i);
                    vis[i]=true;
                }
            }
        }
    }
    return false;
}
int main(void){
    //freopen("data.txt","r",stdin);
    scanf("%d",&t);
    while(t--){
        _clr(g,INF);
        scanf("%d%d%d",&n,&m,&w);
        while(m--){
            scanf("%d%d%d",&u,&v,&wi);
            if(wi<g[u][v]){
                g[u][v]=g[v][u]=wi;
            }
        }
        while(w--){
            scanf("%d%d%d",&u,&v,&wi);
            if(-wi<g[u][v]){
                //虫洞是单向的
                g[u][v]=-wi;
            }
        }
        if(spfa()){
            printf("YES\n");
        }
        else{
            printf("NO\n");
        }
    }
    return 0;
}
```





