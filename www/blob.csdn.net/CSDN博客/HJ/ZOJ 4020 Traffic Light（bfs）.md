# ZOJ 4020 Traffic Light（bfs） - HJ - CSDN博客
2018年04月24日 20:34:35[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：118
[http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=4020](http://acm.zju.edu.cn/onlinejudge/showProblem.do?problemCode=4020)
题目大意：有n*m个点，每个点在每一秒都会有变化，0变成1,1变成0。有一个人要从指定的起点走到终点去，每走一步需要一秒，如果当前的点为0，则他下一步只能上下走；如果当前的点为1，则他下一步只能左右走。问最少下需要多少秒才能从起点到终点？
解题思路：裸的bfs，只要在每次判断步数为奇数还是偶数，按照规定的方向走一遍即可。注意这里需要将vis访问数组和ans判断奇偶数组二维降到一维，否则会爆内存。
```cpp
#include<iostream>
#include<cstdio> 
#include<queue>
#include<vector>
#include<cstring> 
using namespace std;
typedef long long ll;
const int maxn=3e5+10;
#define INF 0x3f3f3f3f
struct node{
    int x,y;
    node(int x,int y):x(x),y(y) {};
};
queue<node> q;
vector<int> g[maxn];
int vis[maxn],ans[maxn];
int n,m,sx,sy,ex,ey;
int px[]={1,-1,0,0},py[]={0,0,1,-1};
bool check(int dx,int dy,int x,int y)
{
    if(!vis[dx*m+dy]&&dx>=0&&dy>=0&&dx<n&&dy<m)
    {
        ans[dx*m+dy]=ans[x*m+y]+1;
        vis[dx*m+dy]=1;
        q.push(node(dx,dy));
    }
}
void change(int i,int j,int x,int y)
{
    for(int k=i;k<j;k++)  
    {
        int dx=x+px[k];
        int dy=y+py[k];
        check(dx,dy,x,y);
    }
}
void bfs()
{
    memset(vis,0,sizeof(vis));
    memset(ans,-1,sizeof(ans)); 
    q.push(node(sx,sy));
    vis[sx*m+sy]=1;
    ans[sx*m+sy]=0;
    while(!q.empty())
    {
        node u=q.front();
        q.pop();
        int x=u.x,y=u.y;
        int blag=ans[x*m+y]%2?!g[x][y]:g[x][y];  //判断从起点到当前点的步数的奇偶性 
        blag?change(2,4,x,y):change(0,2,x,y); //按照规定bfs压入队列 
    }
}
int main()
{
    int t,x;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&n,&m);
        for(int i=0;i<n;i++)
        {
            g[i].clear();
            for(int j=0;j<m;j++)
            {
                scanf("%d",&x);
                g[i].push_back(x);
            }
        }
        cin>>sx>>sy>>ex>>ey;
        sx--,sy--,ex--,ey--;
        bfs();
        cout<<ans[ex*m+ey]<<endl;
    }
    return 0;
}
```
