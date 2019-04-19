# 2018年东北农业大学春季校赛 H题 wyh的吃鸡（bfs） - HJ - CSDN博客
2018年04月22日 16:08:20[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：109
[https://www.nowcoder.com/acm/contest/93/H](https://www.nowcoder.com/acm/contest/93/H)
解题思路： 
用优先队列优化一下，再BFS进行搜一遍
```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<queue>
using namespace std;
typedef long long int ll;
const ll INF=0x7fffffff;
const int N=110;
struct node{
    int x,y,t;
    ll len;
    node(int a,int b,ll c,int d):x(a),y(b),len(c),t(d) {};
    bool operator<(const node &p)const{
        return len>p.len;
    }
};
priority_queue<node> q;
int n,k,sx,sy;
int px[]={1,-1,0,0},py[]={0,0,1,-1};
bool vis[N][N][3];
char map[N][N];
ll bfs()
{
    while(!q.empty())
      q.pop();
    memset(vis,0,sizeof(vis));
    q.push(node(sx,sy,0,2));
    while(!q.empty())
    {
        node u=q.top();
        q.pop();
        if(vis[u.x][u.y][u.t])
          continue;
        vis[u.x][u.y][u.t]=true;
        if(map[u.x][u.y]=='X')
          return u.len;
        for(int i=0;i<4;i++)
        {
            int dx=u.x+px[i];
            int dy=u.y+py[i];
            if(dx>=0&&dy>=0&&dx<n&&dy<n&&!vis[dx][dy][u.t]&&map[dx][dy]!='O')
              q.push(node(dx,dy,u.len+u.t,map[dx][dy]=='C'?1:u.t));
        }
    }
    return INF;
}
int main()
{
    int t;
    cin>>t;
    while(t--)
    {
        cin>>n>>k;
        for(int i=0;i<n;i++)
          for(int j=0;j<n;j++)
          {
              cin>>map[i][j];
              if(map[i][j]=='S')
                sx=i,sy=j;
          }
        ll ans=bfs();
        if(ans<=k)
          printf("YES\n%lld\n",ans);
        else
          printf("NO\n");
    }
    return 0;
}
```
